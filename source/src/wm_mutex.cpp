
#include "wormnet/wm_mutex.h"

namespace worm
{

wmMutex::wmMutex()
{
#ifdef WIN32
    InitializeCriticalSection(&m_mutex);
#else
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&m_mutex, &attr);
#endif //
}

wmMutex::~wmMutex()
{
#ifdef WIN32
    DeleteCriticalSection(&m_mutex);
#else
    pthread_mutex_destroy(&m_mutex);
#endif //
}

void wmMutex::Lock(void)
{
#ifdef WIN32
    EnterCriticalSection(&m_mutex);
#else
    pthread_mutex_lock(&m_mutex);
#endif //
}

void wmMutex::Unlock(void)
{
#ifdef WIN32
    LeaveCriticalSection(&m_mutex);
#else
    pthread_mutex_unlock(&m_mutex);
#endif //
}

}
