#ifdef P_WIN32
#ifndef P_WIN32_GCC
#pragma warning(disable:4786) // disable spurious C4786 warnings
#endif
#endif
#include "Transporte.h"
#include "debug.h"
#ifdef P_LINUX
#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#endif

#ifdef P_WIN32_GCC
#include <stdio.h>
#define _snprintf snprintf
#endif

#include <gadget_tools.h>

#ifdef P_WIN32
std::map<unsigned, PuertoSerie::PortInformation> PuertoSerie::Handles;
#endif

PuertoSerie::PuertoSerie(LogFile &LogF,
						 unsigned nPuerto, unsigned Velocidad, Paridades Paridad,
						 unsigned char BitsStop, unsigned char BitsDatos) throw(Excepcion) : Transporte(LogF)
{
	Open(nPuerto, Velocidad, Paridad, BitsStop, BitsDatos);
}

void
PuertoSerie::Open(unsigned nPuerto, unsigned Velocidad, Paridades Paridad, unsigned char BitsStop, unsigned char BitsDatos) throw(Excepcion)
{
	Log.Write("PuertoSerie", "Open COM%u, %u bauds", nPuerto, Velocidad);

#ifdef P_WIN32
	PortNumber = nPuerto;
	PortInformation Port;
	std::map<unsigned, PortInformation>::iterator i = Handles.find(nPuerto);
	if (i == Handles.end())
	{
		memset(&Port.Overlapped, 0, sizeof Port.Overlapped);
		if (!(Port.Overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		{
			std::string ErrDesc = Excepcion::GetLastErrorDesc();
			Log.Write("PuertoSerie", "Error CreateEvent: %s", ErrDesc.c_str());
			throw Excepcion(Excepcion::TRANSPORTE_ERROR_APERTURA, ErrDesc);
		}

		char ComName[20];
#ifdef P_WIN32_VC_2005
		sprintf_s(ComName, sizeof ComName, "\\\\.\\COM%d", nPuerto);
#else
		sprintf(ComName, "\\\\.\\COM%d", nPuerto);
#endif

		Port.Handle = CreateFile(ComName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

		if (Port.Handle == INVALID_HANDLE_VALUE)
		{
			unsigned ErrNo = GetLastError();
			Excepcion::Errores e = ErrNo == ERROR_ACCESS_DENIED?
						Excepcion::TRANSPORTE_ERROR_YA_ABIERTO :
						Excepcion::TRANSPORTE_ERROR_APERTURA;
			std::string ErrDesc = Excepcion::GetLastErrorDesc(ErrNo);
			CloseHandle(Port.Overlapped.hEvent);
			Log.Write("PuertoSerie", "Error open serial %s: %s", ComName, ErrDesc.c_str());
			throw Excepcion(e, ErrDesc + " [" + ComName + "]");
		}
		Port.Count = 1;
	}
	else
	{
		Log.Write("PuertoSerie", "Reopen");
		Port = i->second;
		Port.Count++;
	}
	Handles[nPuerto] = Port;

	DCB dcb;
	dcb.DCBlength = sizeof(dcb);

	if (!GetCommState(Port.Handle, &dcb))
	{
		std::string ErrDesc = Excepcion::GetLastErrorDesc();
		Log.Write("PuertoSerie", "GetCommState: %s", ErrDesc.c_str());
		Close();
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_PARAMETROS_SERIE, ErrDesc);
	}

	dcb.BaudRate = Velocidad;
	dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
	dcb.fTXContinueOnXoff = TRUE;
	dcb.fErrorChar = FALSE;
	dcb.fNull = FALSE;
	dcb.fAbortOnError = FALSE;
	dcb.wReserved = 0;
	dcb.ByteSize = BitsDatos;

	switch (Paridad)
	{
		case PAR:
			dcb.Parity = EVENPARITY;
			break;
		case IMPAR:
			dcb.Parity = ODDPARITY;
			break;
		case ESPACIO:
			dcb.Parity = SPACEPARITY;
			break;
		case MARCA:
			dcb.Parity = MARKPARITY;
			break;
		default:
		case NINGUNA:
			dcb.Parity = NOPARITY;
			break;
	}
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fAbortOnError = FALSE;
	dcb.StopBits = (BitsStop == 2) ? TWOSTOPBITS : ONESTOPBIT;

	if (!SetCommState(Port.Handle, &dcb) )
	{
		std::string ErrDesc = Excepcion::GetLastErrorDesc();
		Log.Write("PuertoSerie", "SetCommState: %s", ErrDesc.c_str());
		Close();
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_PARAMETROS_SERIE, ErrDesc);
	}
	LastTimeout = 0xFFFFFFFF;
	LastSpeed = Velocidad;
	LastParity = Paridad;
	LastBitsStop = BitsStop;
	LastWordLength = BitsDatos;
	EsUSBGadget = GadgetTools::IsGadgetSerialPort(nPuerto);
#endif

#ifdef P_LINUX
	char Buffer[50];

	sprintf(Buffer, "/dev/ttyS%d", nPuerto - 1);
	fd = open(Buffer, O_RDWR);
	if (fd < 0)
	{
		std::string ErrDesc = Excepcion::GetLastErrorDesc();
		Log.Write("PuertoSerie", "Error open: %s", ErrDesc.c_str());
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_APERTURA, ErrDesc);
	}

	struct termios Params;
	if (tcgetattr(fd, &Params) < 0)
	{
		std::string ErrDesc = Excepcion::GetLastErrorDesc();
		Log.Write("PuertoSerie", "tcgetattr: %s", ErrDesc.c_str());
		Close();
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_APERTURA, ErrDesc);
	}
	cfmakeraw(&Params);

	speed_t Bauds;
	switch(Velocidad)
	{
		case 1200:
			Bauds = B1200;
			break;
		case 2400:
			Bauds = B2400;
			break;
		case 4800:
			Bauds = B4800;
			break;
		case 9600:
			Bauds = B9600;
			break;
		case 19200:
			Bauds = B19200;
			break;
		case 38400:
			Bauds = B38400;
			break;
		case 57600:
			Bauds = B57600;
			break;
		case 115200:
			Bauds = B115200;
			break;
		default:
		{
			Close();	
			char Buf[100];
			sprintf(Buf, "%u: velocidad incorrecta", Velocidad);
			throw Excepcion(Excepcion::TRANSPORTE_ERROR_APERTURA, Buf);
		}
	}

	if (cfsetospeed(&Params, Bauds) < 0 || cfsetispeed(&Params, Bauds) < 0)
	{
		Close();
		char Buf[100];
		sprintf(Buf, "%u: falla cambio velocidad", Velocidad);
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_APERTURA, Buf);
	}
	Params.c_cflag &= ~(CS7 | CS8 | CSTOPB);

	unsigned StopBits;
	switch(BitsStop)
	{
		default:
		case 1:
			StopBits = 0;
			break;
		case 2:
			StopBits = CSTOPB;
			break;
	}

	unsigned WordLength;
	switch (BitsDatos)
	{
		case 7:
			WordLength = CS7;
			break;
		default:
		case 8:
			WordLength = CS8;
			break;
	}

	Params.c_cflag |= (StopBits | WordLength);
	Params.c_cc[VMIN] = 1;

	switch (Paridad)
	{
		case PAR:
			Params.c_cflag |= PARENB;
			Params.c_cflag &= ~PARODD;
			break;
		case IMPAR:
			Params.c_cflag |= PARENB;
			Params.c_cflag |= PARODD;
			break;
		default:
		case NINGUNA:
			Params.c_cflag &= ~PARENB;
			break;
	}
	Params.c_iflag &= ~(IXON | IXOFF); // Sin protocolo

	if (tcsetattr(fd, TCSANOW, &Params) < 0)
	{
		std::string ErrDesc = Excepcion::GetLastErrorDesc();
		Log.Write("PuertoSerie", "tcsetattr: %s", ErrDesc.c_str());
		Close();
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_PARAMETROS_SERIE, ErrDesc);
	}
#endif
}

PuertoSerie::~PuertoSerie()
{
	Close();
}

void
PuertoSerie::Close()
{
#ifdef P_WIN32
	std::map<unsigned, PortInformation>::iterator i = Handles.find(PortNumber);
	if (i != Handles.end())
	{
		PortInformation p = i->second;
		if (!--p.Count)
		{
			Log.Write("PuertoSerie", "Close COM%u", i->first);
			CloseHandle(p.Handle);
			CloseHandle(p.Overlapped.hEvent);
			Handles.erase(i);
		}
		else Handles[PortNumber] = p;
	}
#endif

#ifdef P_LINUX
	close(fd);
#endif
}

void
PuertoSerie::EnviarByte(unsigned char Byte) throw(Excepcion)
{
	EnviarString(&Byte, 1);
}

#ifdef P_WIN32
PuertoSerie::PortInformation
PuertoSerie::GetPort() const throw(Excepcion)
{
	std::map<unsigned, PortInformation>::const_iterator i = Handles.find(PortNumber);
	if (i == Handles.end() || i->second.Handle == INVALID_HANDLE_VALUE)
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_PUERTO_NO_ABIERTO);
	return i->second;
}
#endif

unsigned char
PuertoSerie::LeerByte(unsigned TiempoEspera) throw(Excepcion)
{
	unsigned char c;
	LeerString(&c, 1, TiempoEspera);
	return c;
};

unsigned
PuertoSerie::LeerString(unsigned char *Ptr, unsigned Max, unsigned TiempoEspera) throw(Excepcion)
{
#ifdef P_WIN32
	PortInformation p;
	try
	{
		p = GetPort();
	}
	catch(Excepcion::Errores e)
	{
		if (e == Excepcion::TRANSPORTE_ERROR_PUERTO_NO_ABIERTO)
			Open(PortNumber, LastSpeed, LastParity, LastBitsStop, LastWordLength);
		else throw;
	}

	DWORD CountRead;
	SetTimeout(TiempoEspera);
	if (!ReadFile(p.Handle, Ptr, Max, &CountRead, &p.Overlapped))
	{
		if (GetLastError() != ERROR_IO_PENDING) // Error
			throw Excepcion(Excepcion::TRANSPORTE_ERROR_LECTURA, Excepcion::GetLastErrorDesc());

		// ERROR_IO_PENDING: Hay que esperar el evento.
		const unsigned TIMEOUT_SLICE = 200; //msecs
		while (WaitForSingleObject(p.Overlapped.hEvent, Eventos? TIMEOUT_SLICE : INFINITE) == WAIT_TIMEOUT)
			Eventos->Evento(0);
		if (!GetOverlappedResult(p.Handle, &p.Overlapped, &CountRead, FALSE))
			throw Excepcion(Excepcion::TRANSPORTE_ERROR_LECTURA, Excepcion::GetLastErrorDesc());
	}
	if(!CountRead)
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO);
	return CountRead;
#endif

#ifdef P_LINUX
	Select(fd, TiempoEspera);
	int rc;
	if ((rc = read(fd, Ptr, Max)) == -1)
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_LECTURA, Excepcion::GetLastErrorDesc());
	return rc;
#endif
};

void
PuertoSerie::EnviarString(const unsigned char *Ptr, unsigned Len) throw(Excepcion)
{
#ifdef P_WIN32
	PortInformation p;
	try
	{
		p = GetPort();
	}
	catch(Excepcion::Errores e)
	{
		if (e == Excepcion::TRANSPORTE_ERROR_PUERTO_NO_ABIERTO)
			Open(PortNumber, LastSpeed, LastParity, LastBitsStop, LastWordLength);
		else throw;
	}

	unsigned long BytesWritten;

	bool Exit = false;

	bool Fail =	false;
	while(!Exit)
	{
		if (!WriteFile (p.Handle, Ptr, Len,	&BytesWritten, &p.Overlapped))
		{
			if (GetLastError() != ERROR_IO_PENDING)	// Error
				Fail = true;
			else
			{
				// ERROR_IO_PENDING: Hay que esperar el	evento.
				const unsigned TIMEOUT_SLICE = 200;	//msecs
				while (WaitForSingleObject(p.Overlapped.hEvent,	Eventos? TIMEOUT_SLICE : INFINITE) == WAIT_TIMEOUT)
					Eventos->Evento(0);
				if (!GetOverlappedResult(p.Handle, &p.Overlapped, &BytesWritten, FALSE))
					Fail = true;
			}
		}

		if (!Fail && BytesWritten != Len)
			Fail = true;

		if (Fail && EsUSBGadget)
		{
			// Resetear puerto USB gadget

			Log.Write("PuertoSerie", "Reset USB COM%u", PortNumber);
			std::map<unsigned, PortInformation>::iterator i = Handles.find(PortNumber);
			CloseHandle(i->second.Handle);
			CloseHandle(i->second.Overlapped.hEvent);
			i->second.Count = 0;
			Handles.erase(i);

			GadgetTools::ResetGadgetSerialPort(PortNumber);
		}
		else
			Exit = true;
	}
	if (Fail)
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_ESCRITURA, Excepcion::GetLastErrorDesc());
#endif

#ifdef P_LINUX
	if (write(fd, Ptr, Len) != Len)
		throw Excepcion(Excepcion::TRANSPORTE_ERROR_ESCRITURA, Excepcion::GetLastErrorDesc());
#endif
}

bool
PuertoSerie::InQueueEmpty() const throw(Excepcion)
{
#ifdef P_WIN32
	DWORD Errores;
	COMSTAT Stat;

	PortInformation p;
	p = GetPort();

	ClearCommError(p.Handle, &Errores, &Stat);
	return Stat.cbInQue == 0;
#endif

#ifdef P_LINUX
	try
	{
		Select(fd, 0);
		return false;
	}
	catch(Excepcion)
	{
		return true;
	}
#endif
}

#ifdef P_WIN32
void
PuertoSerie::SetTimeout(unsigned Timeout)
{
	if (Timeout == LastTimeout)
		return;

	LastTimeout = Timeout;

	COMMTIMEOUTS Timeouts;

    Timeouts.ReadIntervalTimeout = 0;
    Timeouts.ReadTotalTimeoutConstant = Timeout;
    Timeouts.ReadTotalTimeoutMultiplier = 0;
    Timeouts.WriteTotalTimeoutMultiplier = 0;
    Timeouts.WriteTotalTimeoutConstant = 0;

	PortInformation p;
	try
	{
		p = GetPort();
	}
	catch(Excepcion::Errores e)
	{
		if (e == Excepcion::TRANSPORTE_ERROR_PUERTO_NO_ABIERTO)
			Open(PortNumber, LastSpeed, LastParity, LastBitsStop, LastWordLength);
		else throw;
	}

   	SetCommTimeouts(p.Handle, &Timeouts );
};
#endif
