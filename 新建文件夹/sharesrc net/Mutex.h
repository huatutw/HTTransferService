#ifndef		__MUTEX_H
#define		__MUTEX_H
#include <Windows.h>

class MutexLock
{
public:
MutexLock()
{
	InitializeCriticalSection(&m_mutex);
}
~MutexLock()
{
	DeleteCriticalSection(&m_mutex);
}

void MutexLock::Lock()
{
	EnterCriticalSection(&m_mutex);
}
void MutexLock::UnLock()
{
	LeaveCriticalSection(&m_mutex);
}


CRITICAL_SECTION				m_mutex;

};
#endif