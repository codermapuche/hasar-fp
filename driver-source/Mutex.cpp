#include "Mutex.h"
#ifdef P_LINUX
#include <sys/time.h>
#endif

#ifdef P_LINUX
namespace
{
	const pthread_mutex_t inimut = PTHREAD_MUTEX_INITIALIZER;
}
#endif

Mutex::Mutex()
#ifdef P_LINUX
	: mut(inimut)
#endif
{
#ifdef P_WIN32
	hMutex = CreateSemaphore(NULL, 1, 1, NULL);
#endif
}

Mutex::~Mutex()
{
#ifdef P_WIN32
	CloseHandle(hMutex);
#endif

#ifdef P_LINUX
	pthread_mutex_destroy(&mut);
#endif
}

bool
Mutex::Enter()
{
#ifdef P_WIN32
	if (!hMutex)
		return false;
	return WaitForSingleObject(hMutex, INFINITE) == WAIT_OBJECT_0;
#endif
#ifdef P_LINUX
	return pthread_mutex_lock(&mut) == 0;
#endif
}

bool
Mutex::TryEnter()
{
#ifdef P_WIN32
	if (!hMutex)
		return false;
	return WaitForSingleObject(hMutex, 0) == WAIT_OBJECT_0;
#endif
#ifdef P_LINUX
	return pthread_mutex_trylock(&mut) == 0;
#endif
}

bool
Mutex::Leave()
{
#ifdef P_WIN32
	if (!hMutex)
		return false;
	if (!ReleaseSemaphore(hMutex, 1, NULL))
		return false;
	return true;
#endif

#ifdef P_LINUX
	return pthread_mutex_unlock(&mut) == 0;
#endif
}

