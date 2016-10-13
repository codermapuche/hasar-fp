#ifdef P_WIN32
#ifndef P_WIN32_GCC
#pragma warning(disable:4786) // disable spurious C4786 warnings
#endif
#endif

#include "Excepcion.h"

#ifdef P_WIN32
#include <winsock2.h>
#include <map>
#endif

#ifdef P_LINUX
#include <string.h>
#include <errno.h>
#endif

#ifdef P_WIN32_VC_2005
#define _snprintf _snprintf_s
#endif

#ifdef P_WIN32_GCC
#include <stdio.h>
#define _snprintf snprintf
#endif

namespace
{
	std::string GetErrorDescription(unsigned ErrNo)
	{
#ifdef P_WIN32
		void *Buf;
		char *p;

		if (!FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER |
							FORMAT_MESSAGE_FROM_SYSTEM |
							FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL,
							ErrNo,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
							(LPTSTR) &Buf,
							0,
							NULL ))
		{
			char Buf[100];
			_snprintf(Buf, sizeof Buf, "Error desconocido: %d (%x)", ErrNo, ErrNo);
			return std::string(Buf);
		}

		if ((p = strchr((char *)Buf, '\n')) != NULL)
			*p = 0;
		if ((p = strchr((char *)Buf, '\r')) != NULL)
			*p = 0;
		std::string s = static_cast<char *>(Buf);
		LocalFree(Buf);
#endif
#ifdef P_LINUX
		std::string s = strerror(ErrNo);
#endif
		return s;
	}
}

std::string
Excepcion::GetLastErrorDesc(unsigned ErrNo)
{
	if (!ErrNo)
#ifdef P_WIN32
		ErrNo = GetLastError();
#endif

#ifdef P_LINUX
		ErrNo = errno;
#endif
	return GetErrorDescription(ErrNo);
}

std::string
Excepcion::SocketGetLastErrorDesc(unsigned ErrNo)
{
#ifdef P_LINUX
	return GetErrorDescription(errno);
#endif
#ifdef P_WIN32
	if (!ErrNo)
		ErrNo = WSAGetLastError();

	static std::map<unsigned, std::string> WSAErrors;
	if (!WSAErrors.size())
	{
		WSAErrors[0] = "No error";
		WSAErrors[WSAEINTR] = "Interrupted system call";
		WSAErrors[WSAEBADF] = "Bad file number";
		WSAErrors[WSAEACCES] = "Permission denied";
		WSAErrors[WSAEFAULT] = "Bad address";
		WSAErrors[WSAEINVAL] = "Invalid argument";
		WSAErrors[WSAEMFILE] = "Too many open sockets";
		WSAErrors[WSAEWOULDBLOCK] = "Operation would block";
		WSAErrors[WSAEINPROGRESS] = "Operation now in progress";
		WSAErrors[WSAEALREADY] = "Operation already in progress";
		WSAErrors[WSAENOTSOCK] = "Socket operation on non-socket";
		WSAErrors[WSAEDESTADDRREQ] = "Destination address required";
		WSAErrors[WSAEMSGSIZE] = "Message too long";
		WSAErrors[WSAEPROTOTYPE] = "Protocol wrong type for socket";
		WSAErrors[WSAENOPROTOOPT] = "Bad protocol option";
		WSAErrors[WSAEPROTONOSUPPORT] = "Protocol not supported";
		WSAErrors[WSAESOCKTNOSUPPORT] = "Socket type not supported";
		WSAErrors[WSAEOPNOTSUPP] = "Operation not supported on socket";
		WSAErrors[WSAEPFNOSUPPORT] = "Protocol family not supported";
		WSAErrors[WSAEAFNOSUPPORT] = "Address family not supported";
		WSAErrors[WSAEADDRINUSE] = "Address already in use";
		WSAErrors[WSAEADDRNOTAVAIL] = "Can't assign requested address";
		WSAErrors[WSAENETDOWN] = "Network is down";
		WSAErrors[WSAENETUNREACH] = "Network is unreachable";
		WSAErrors[WSAENETRESET] = "Net connection reset";
		WSAErrors[WSAECONNABORTED] = "Software caused connection abort";
		WSAErrors[WSAECONNRESET] = "Connection reset by peer";
		WSAErrors[WSAENOBUFS] = "No buffer space available";
		WSAErrors[WSAEISCONN] = "Socket is already connected";
		WSAErrors[WSAENOTCONN] = "Socket is not connected";
		WSAErrors[WSAESHUTDOWN] = "Can't send after socket shutdown";
		WSAErrors[WSAETOOMANYREFS] = "Too many references, can't splice";
		WSAErrors[WSAETIMEDOUT] = "Connection timed out";
		WSAErrors[WSAECONNREFUSED] = "Connection refused";
		WSAErrors[WSAELOOP] = "Too many levels of symbolic links";
		WSAErrors[WSAENAMETOOLONG] = "File name too long";
		WSAErrors[WSAEHOSTDOWN] = "Host is down";
		WSAErrors[WSAEHOSTUNREACH] = "No route to host";
		WSAErrors[WSAENOTEMPTY] = "Directory not empty";
		WSAErrors[WSAEPROCLIM] = "Too many processes";
		WSAErrors[WSAEUSERS] = "Too many users";
		WSAErrors[WSAEDQUOT] = "Disc quota exceeded";
		WSAErrors[WSAESTALE] = "Stale NFS file handle";
		WSAErrors[WSAEREMOTE] = "Too many levels of remote in path";
		WSAErrors[WSASYSNOTREADY] = "Network subsystem is unavailable";
		WSAErrors[WSAVERNOTSUPPORTED] = "Winsock version not supported";
		WSAErrors[WSANOTINITIALISED] = "Winsock not yet initialized";
		WSAErrors[WSAHOST_NOT_FOUND] = "Host not found";
		WSAErrors[WSATRY_AGAIN] = "Non-authoritative host not found";
		WSAErrors[WSANO_RECOVERY] = "Non-recoverable errors";
		WSAErrors[WSANO_DATA] = "Valid name, no data record of requested type";
		WSAErrors[WSAEDISCON] = "Graceful disconnect in progress";
		WSAErrors[WSASYSCALLFAILURE] = "System call failure";
		WSAErrors[WSA_NOT_ENOUGH_MEMORY] = "Insufficient memory available";
		WSAErrors[WSA_OPERATION_ABORTED] = "Overlapped operation aborted";
		WSAErrors[WSA_IO_INCOMPLETE] = "Overlapped I/O object not signalled";
		WSAErrors[WSA_IO_PENDING] = "Overlapped I/O will complete later";
		WSAErrors[WSA_INVALID_PARAMETER] = "One or more parameters are invalid";
		WSAErrors[WSA_INVALID_HANDLE] = "Event object handle not valid";
	}
	std::map<unsigned, std::string>::iterator i = WSAErrors.find(ErrNo);
	if (i != WSAErrors.end())
		return i->second;
	return GetErrorDescription(ErrNo);
#endif
}


