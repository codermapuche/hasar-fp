#ifndef MUTEX_H
#define MUTEX_H

#ifdef P_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifdef P_LINUX
#include <pthread.h>
#endif

class Mutex
{
public:
	Mutex();
	~Mutex();
	bool Enter();
	bool TryEnter();
	bool Leave(void);
private:
#ifdef P_WIN32
	HANDLE hMutex;
#endif
#ifdef P_LINUX
	pthread_mutex_t mut;
#endif
};

#endif

