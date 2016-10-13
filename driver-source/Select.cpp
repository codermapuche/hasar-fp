#ifdef P_LINUX
#include <sys/select.h>
#include "min_max.h"
#endif

#ifdef P_WIN32_GCC
#include "min_max.h"
#endif

#ifdef P_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "Transporte.h"

namespace
{
	void DoSelect(Transporte::DESCRIPTOR fd, unsigned Timeout) throw(Excepcion)
	{
		fd_set readfds;
		fd_set writefds;
		fd_set exceptionfds;

		FD_ZERO (&readfds);
		FD_ZERO (&writefds);
		FD_ZERO (&exceptionfds);
		FD_SET  (fd, &readfds);

		struct timeval t;
			
		t.tv_sec  = Timeout / 1000;
		t.tv_usec = (Timeout % 1000) * 1000; 

		switch (select (fd + 1, &readfds, &writefds, &exceptionfds, &t))
		{
			case  0:
				throw Excepcion(Excepcion::TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO);
			case 1:
				return; // OK
			case -1:
			default:
				throw Excepcion(Excepcion::TRANSPORTE_ERROR_SELECT, Excepcion::GetLastErrorDesc());
		}
	}
}

void 
Transporte::Select(DESCRIPTOR fd, unsigned Timeout) const throw(Excepcion)
{
	const unsigned TIMEOUT_SLICE = 200; //msecs
	do
	{
		unsigned Tout = min(Timeout, TIMEOUT_SLICE);
		try
		{
			DoSelect(fd, Tout);
			return;
		}
		catch(Excepcion &e)
		{
			if (e.Codigo != Excepcion::TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO)
				throw;
			Eventos->Evento(0);
			Timeout -= Tout;
		}
	} while(Timeout);
	throw Excepcion(Excepcion::TRANSPORTE_ERROR_TIEMPO_DE_ESPERA_AGOTADO);	
}
