
#ifndef _WORMNET_SYSTEM_MUTEX_H
#define _WORMNET_SYSTEM_MUTEX_H

#ifdef WIN32
#include <windows.h>
#undef ERROR
#else
#include <pthread.h>
#endif
#include <wormnet/wm_types.h>

namespace worm
{

class WORM_DLL_EXPORT wmMutex
{
public:
    wmMutex();
    ~wmMutex();

    void Lock(void);
    void Unlock(void);

#ifdef WIN32
    CRITICAL_SECTION& Get(void)
#else
    pthread_mutex_t& Get(void)
#endif //
    {
        return m_mutex;
    }


private:
#ifdef WIN32
    CRITICAL_SECTION            m_mutex;
#else
    pthread_mutex_t             m_mutex;
#endif //
};

class WORM_DLL_EXPORT wmLockGuard
{
public:
    wmLockGuard(wmMutex& oLock) : m_oLock(oLock)
    {
        m_oLock.Lock();
    }
    ~wmLockGuard()
    {
        m_oLock.Unlock();
    }

private:
    wmLockGuard();

private:
    wmMutex& m_oLock;
};

}

#endif // ifndef _WORMNET_SYSTEM_MUTEX_H
