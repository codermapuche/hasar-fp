#include <string.h>
#include <time.h>
#include "Transporte.h"
#include "debug.h"

#ifdef P_LINUX
#include <errno.h>
#include <sys/ioctl.h>
#include "min_max.h"

const int INVALID_SOCKET = -1;
const int SOCKET_ERROR = -1;
#endif

#ifdef P_WIN32_GCC
#include "min_max.h"
#endif

namespace
{
	// Dada una dirección IP en ASCII la devuelve en un unsigned long.
	unsigned long GetAddress(const std::string &StringAddress) throw(Excepcion)
	{
		// Si es una dirección de IP válida:
		unsigned Cuenta = 0;
		for (unsigned i = 0; i < StringAddress.length(); i++)
			if (StringAddress[i] == '.')
				Cuenta++;
			else if (!isdigit(StringAddress[i]))
				throw Excepcion(Excepcion::TRANSPORTE_ERROR_FORMATO_IP, StringAddress);
		if (Cuenta != 3)
			throw Excepcion(Excepcion::TRANSPORTE_ERROR_FORMATO_IP, StringAddress);

		return ntohl(inet_addr(StringAddress.c_str()));
	}

#ifdef P_WIN32
	void InitWinSock() throw(Excepcion)
	{
		// Inicializar WinSock:
		WORD wVersionRequested = MAKEWORD(1, 1); 
		WSADATA wsaData;
	
		if (WSAStartup( wVersionRequested, &wsaData ) != 0) 
			throw Excepcion(Excepcion::TRANSPORTE_ERROR_INICIALIZACION_SOCKET, Excepcion::SocketGetLastErrorDesc());

		if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) 
		{
			WSACleanup();
			throw Excepcion(Excepcion::TRANSPORTE_ERROR_VERSION_SOCKET);
		}
	}
#endif
}

SocketTCP::SocketTCP(	LogFile &LogF,
						const std::string &DireccionRemota, 
						unsigned short PuertoRemoto,
						unsigned short PuertoLocal) throw(Excepcion) : Transporte(LogF)
{
#ifdef P_WIN32
	InitWinSock();
#endif

	// Abrir puerto:
	if ((SocketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
	{
		std::string ErrDesc = Excepcion::SocketGetLastErrorDesc();
		Log.Write("SocketTCP", "Error WSASocket: " + ErrDesc);
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_APERTURA, ErrDesc);
	}

	struct sockaddr_in Name;

	memset ((char *)&Name, 0, sizeof Name);
	Name.sin_family = AF_INET;
	Name.sin_addr.s_addr = htonl(INADDR_ANY);
	Name.sin_port = htons(PuertoLocal);

	if(bind(SocketDescriptor, (struct sockaddr *)&Name, sizeof Name) < 0)
	{
		std::string ErrDesc = Excepcion::SocketGetLastErrorDesc();
		Close();
		Log.Write("SocketTCP", "Error bind: " + ErrDesc);
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_BIND_SOCKET, ErrDesc);
	}

	// Dirección remota:
	Address = GetAddress(DireccionRemota);
	Port = PuertoRemoto;

	struct sockaddr_in to;
    memset (&to, 0, sizeof to);
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = htonl(Address);
	to.sin_port = htons(Port);
	
	if (connect(SocketDescriptor, (struct sockaddr *) &to, sizeof to) < 0 )
	{
		std::string ErrDesc = Excepcion::SocketGetLastErrorDesc();
		Log.Write("SocketTCP", "Error connect: " + ErrDesc);
		Close();
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_CONEXION, ErrDesc);
	}

	unsigned long Parameter = 1; // no bloqueante
#ifdef P_WIN32
	ioctlsocket(SocketDescriptor, FIONBIO, &Parameter);
#endif
#ifdef P_LINUX
	ioctl(SocketDescriptor, FIONBIO, &Parameter);
#endif

	pBuffer = Buffer;
	BytesRead = 0;
}

SocketTCP::~SocketTCP()
{
	Close();
}

void
SocketTCP::Close()
{
#ifdef P_WIN32
	closesocket(SocketDescriptor);
#endif
#ifdef P_LINUX
	close(SocketDescriptor);
#endif
}


void 
SocketTCP::EnviarByte(unsigned char Byte) throw(Excepcion)
{
	EnviarString(&Byte, 1);
}

unsigned char 
SocketTCP::LeerByte(unsigned TiempoEspera) throw(Excepcion)
{
	// Buffer vacío o agotado: leo del socket.
	if (pBuffer == Buffer + BytesRead)
	{
		int rc;

		Select(SocketDescriptor, TiempoEspera);

		if ((rc = recv(SocketDescriptor, Buffer, MAX_TCP_BUFFER, 0)) == SOCKET_ERROR)
		{
#ifdef P_WIN32
			unsigned ErrNo = WSAGetLastError();
			if (ErrNo == WSAEWOULDBLOCK)
#endif
#ifdef P_LINUX
			unsigned ErrNo = errno;
			if (ErrNo == EAGAIN)
#endif
				throw Excepcion(Excepcion::TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO);
			else
				throw Excepcion(Excepcion::TRANSPORTE_ERROR_LECTURA, Excepcion::SocketGetLastErrorDesc(ErrNo));
		}
		pBuffer = Buffer;
		BytesRead = rc;
	}
	return *(pBuffer++);
}

unsigned 
SocketTCP::LeerString(unsigned char *Ptr, unsigned Max, unsigned TiempoEspera) throw(Excepcion)
{
	unsigned i;
	try
	{
		for (i = 0; i < Max; i++)
			*Ptr++ = LeerByte(TiempoEspera);
	}
	catch(Excepcion &e)
	{
		if (e.Codigo == Excepcion::TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO)
			return i;
		throw;
	}
	return Max;
}


void 
SocketTCP::EnviarString(const unsigned char *Ptr, unsigned Len) throw(Excepcion)
{
	const char *p = reinterpret_cast<const char *>(Ptr);
	do
	{
		unsigned len = min(Len, MAX_TCP_BUFFER);
		int rc;
		if ((rc = send(SocketDescriptor, p, len, 0)) == SOCKET_ERROR)
			throw Excepcion(Excepcion::TRANSPORTE_ERROR_ESCRITURA);
		p += len;
		Len -= len;
	} while(Len);
}

bool
SocketTCP::InQueueEmpty() const throw(Excepcion)
{
	// Hay datos en el buffer local:
	if (pBuffer < Buffer + BytesRead)
		return false;

	// No hay bytes buffereados, miro en el socket:
	try
	{
		Select(SocketDescriptor, 0);
		return false;
	}
	catch(Excepcion)
	{
		return true;
	}
}


SocketUDP::SocketUDP(	LogFile &LogF,
						const std::string &DireccionRemota, 
						unsigned short PuertoRemoto,
						unsigned short PuertoLocal) throw(Excepcion) : Transporte(LogF)
{
#ifdef P_WIN32
	InitWinSock();
#endif

// Abrir puerto:
	if ((SocketDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) 
	{
		std::string ErrDesc = Excepcion::SocketGetLastErrorDesc();
		Log.Write("SocketUDP", "Error WSASocket: " + ErrDesc);
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_APERTURA, ErrDesc);
	}

	struct sockaddr_in Name;

	memset ((char *)&Name, 0, sizeof Name);
	Name.sin_family = AF_INET;
	Name.sin_addr.s_addr = htonl(INADDR_ANY);
	Name.sin_port = htons(PuertoLocal);

	if(bind(SocketDescriptor, (struct sockaddr *)&Name, sizeof Name) < 0)
	{
		std::string ErrDesc = Excepcion::SocketGetLastErrorDesc();
		Close();
		Log.Write("SocketUDP", "Error bind: " + ErrDesc);
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_BIND_SOCKET, ErrDesc);
	}

	// Dirección remota:
	Address = GetAddress(DireccionRemota);
	Port = PuertoRemoto;

	pBuffer = Buffer;
	BytesRead = 0;
}

SocketUDP::~SocketUDP()
{
	Close();
}

void
SocketUDP::Close()
{
#ifdef P_WIN32
	closesocket(SocketDescriptor);
#endif
#ifdef P_LINUX
	close(SocketDescriptor);
#endif
}


unsigned char 
SocketUDP::LeerByte(unsigned TiempoEspera) throw(Excepcion)
{
	// Buffer vacío o agotado: leo del socket.
	if (pBuffer == Buffer + BytesRead)
	{
		struct sockaddr_in from;
#ifdef P_WIN32
		int addrlen = sizeof from;
#endif
#ifdef P_LINUX
		socklen_t addrlen = sizeof from;
#endif
		int rc;
		Select(SocketDescriptor, TiempoEspera);

		if ((rc = recvfrom(SocketDescriptor, Buffer, MAX_UDP_BUFFER, 0,
						   (struct sockaddr *)&from, &addrlen)) == SOCKET_ERROR)
		{
#ifdef P_WIN32
			unsigned ErrNo = WSAGetLastError();
			if (ErrNo == WSAEWOULDBLOCK)
#endif
#ifdef P_LINUX
			unsigned ErrNo = errno;
			if (ErrNo == EAGAIN)
#endif
				throw Excepcion(Excepcion::TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO);
			else
				throw Excepcion(Excepcion::TRANSPORTE_ERROR_LECTURA, Excepcion::SocketGetLastErrorDesc(ErrNo));
		}

		pBuffer = Buffer;
		BytesRead = rc;
	}
	return *(pBuffer++);
}

bool
SocketUDP::InQueueEmpty() const throw(Excepcion)
{
	// Hay datos en el buffer local:
	if (pBuffer < Buffer + BytesRead)
		return false;

	// No hay bytes buffereados, miro en el socket:
	try
	{
		Select(SocketDescriptor, 0);
		return false;
	}
	catch(Excepcion)
	{
		return true;
	}
}

void 
SocketUDP::EnviarByte(unsigned char Byte) throw(Excepcion)
{
	EnviarString(&Byte, 1);
}

unsigned 
SocketUDP::LeerString(unsigned char *Ptr, unsigned Max, unsigned TiempoEspera) throw(Excepcion)
{
	unsigned i;
	try
	{
		for (i = 0; i < Max; i++)
			*Ptr++ = LeerByte(TiempoEspera);
	}
	catch(Excepcion &e)
	{
		if (e.Codigo == Excepcion::TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO)
			return i;
		throw;
	}
	return Max;
}

void 
SocketUDP::EnviarString(const unsigned char *Ptr, unsigned Len) throw(Excepcion)
{
	struct sockaddr_in to;

    memset ((char *)&to, 0, sizeof to);

	to.sin_family      = AF_INET;
	to.sin_addr.s_addr = htonl(Address);
	to.sin_port        = htons(Port);

	const char *p = reinterpret_cast<const char *>(Ptr);
	do
	{
		unsigned len = min(Len, MAX_UDP_BUFFER);
		int rc;
		if ((rc = sendto (SocketDescriptor, p, len, 0, 
			(struct sockaddr *) &to, sizeof to)) == SOCKET_ERROR)
			throw Excepcion(Excepcion::TRANSPORTE_ERROR_ESCRITURA);
		p += len;
		Len -= len;
	} while(Len);
}
