#ifndef TRANSPORTE_H
#define TRANSPORTE_H

#include "Excepcion.h"
#include "Eventos.h"
#include "debug.h"

#ifdef P_WIN32
#ifndef P_WIN32_GCC
#pragma warning(disable:4786) // disable spurious C4786 warnings
#ifdef P_WIN32_VC_2005
#pragma warning(disable:4290) // VC++ 2005 no soporta especificación de excepciones
#endif // P_WIN32_VC_2005
#endif // P_WIN32_GCC
#include <map>
#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // P_WIN32

#ifdef P_LINUX
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

class Transporte
{
public:
	Transporte (LogFile &LogF) : Log (LogF) {} 
	virtual ~Transporte() {}

	virtual void EnviarByte(unsigned char Byte) throw(Excepcion) = 0;
	virtual void EnviarString(const unsigned char *Ptr, unsigned Len) throw(Excepcion) = 0;
	virtual void EnviarString(const std::string &s) throw(Excepcion)
	{
		EnviarString(reinterpret_cast<const unsigned char *>(s.c_str()), s.length());
	}
	virtual unsigned char LeerByte(unsigned TiempoEspera) throw(Excepcion) = 0;
	virtual unsigned LeerString(unsigned char *Ptr, unsigned Max, unsigned TiempoEspera) throw(Excepcion) = 0;
	virtual bool InQueueEmpty() const throw(Excepcion) = 0;
	virtual void Close() = 0;

	void EstablecerManejadorBusyWaiting(ManejadorDeEventos *pEventos)
	{
		Eventos = pEventos;
	}
#ifdef P_LINUX
	typedef int DESCRIPTOR;
#endif

#ifdef P_WIN32
	typedef SOCKET DESCRIPTOR;
#endif

protected:
	ManejadorDeEventos *Eventos;

	void Select(DESCRIPTOR fd, unsigned Timeout) const throw(Excepcion);

	LogFile &Log;
};

class PuertoSerie : public Transporte
{
public:
	enum Paridades
	{
		PAR, IMPAR, NINGUNA,
#ifndef P_LINUX
		ESPACIO, MARCA
#endif
	};

	PuertoSerie(LogFile &LogF,
				unsigned nPuerto, unsigned Velocidad = 9600, Paridades Paridad = NINGUNA,
				unsigned char BitsStop = 1, unsigned char BitsDatos = 8) throw(Excepcion);
	virtual ~PuertoSerie();

	void EnviarByte(unsigned char Byte) throw(Excepcion);
	unsigned char LeerByte(unsigned TiempoEspera) throw(Excepcion);
	unsigned LeerString(unsigned char *Ptr, unsigned Max, unsigned TiempoEspera) throw(Excepcion);
	void EnviarString(const unsigned char *Ptr, unsigned Len) throw(Excepcion);
	bool InQueueEmpty() const throw(Excepcion);
	void Close();
private:
	void Open(unsigned nPuerto, unsigned Velocidad, Paridades Paridad, unsigned char BitsStop, unsigned char BitsDatos) throw(Excepcion);

#ifdef P_WIN32
	void SetTimeout(unsigned Timeout);
	unsigned LastTimeout;
	struct PortInformation
	{
		HANDLE Handle;
		OVERLAPPED Overlapped;
		unsigned Count;
	};

	PortInformation GetPort() const throw(Excepcion);
	unsigned PortNumber;
	static std::map<unsigned, PortInformation> Handles;

	unsigned LastSpeed, LastBitsStop, LastWordLength;
	Paridades LastParity;
	bool EsUSBGadget;
#endif
#ifdef P_LINUX
	int fd;
#endif
};

class SocketTCP : public Transporte
{
public:
	SocketTCP(	LogFile &LogF,
				const std::string &DireccionRemota,
				unsigned short PuertoRemoto,
				unsigned short PuertoLocal = 0) throw(Excepcion);
	virtual ~SocketTCP();

	void EnviarByte(unsigned char Byte) throw(Excepcion);
	unsigned char LeerByte(unsigned TiempoEspera) throw(Excepcion);
	unsigned LeerString(unsigned char *Ptr, unsigned Max, unsigned TiempoEspera) throw(Excepcion);
	void EnviarString(const unsigned char *Ptr, unsigned Len) throw(Excepcion);
	bool InQueueEmpty() const throw(Excepcion);
	void Close();
private:
#define MAX_TCP_BUFFER 450U
	char Buffer[MAX_TCP_BUFFER];
	char *pBuffer;
	unsigned BytesRead;

#ifdef P_WIN32
	SOCKET SocketDescriptor;
#endif

#ifdef P_LINUX
	int SocketDescriptor;
#endif

	unsigned short Port;
	unsigned long Address;
};

class SocketUDP : public Transporte
{
public:
	SocketUDP(	LogFile &LogF,
				const std::string &DireccionRemota,
				unsigned short PuertoRemoto,
				unsigned short PuertoLocal = 0) throw(Excepcion);
	virtual ~SocketUDP();

	void EnviarByte(unsigned char Byte) throw(Excepcion);
	unsigned char LeerByte(unsigned TiempoEspera) throw(Excepcion);
	unsigned LeerString(unsigned char *Ptr, unsigned Max, unsigned TiempoEspera) throw(Excepcion);
	void EnviarString(const unsigned char *Ptr, unsigned Len) throw(Excepcion);
	bool InQueueEmpty() const throw(Excepcion);
	void Close();
private:
#define MAX_UDP_BUFFER 450U
	char Buffer[MAX_UDP_BUFFER];
	char *pBuffer;
	unsigned BytesRead;
	unsigned short Port;
	unsigned long Address;

#ifdef P_WIN32
	SOCKET SocketDescriptor;
#endif


#ifdef P_LINUX
	int SocketDescriptor;
#endif
};

#endif

