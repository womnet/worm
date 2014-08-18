
#include "wormnet/wm_event.h"

namespace worm
{

wmEvent::wmEvent()
{
#ifdef WIN32
    /// 为了和linux机制相同, 新事件为手动重置, 初始为无事件
    m_hHandle  = CreateEvent(NULL, FALSE, FALSE, NULL);
#else
    pthread_cond_init(&m_hHandle, NULL);
#endif // 
}

wmEvent::~wmEvent()
{
#ifdef WIN32
    ::CloseHandle(m_hHandle);
    m_hHandle = NULL;
#else
    pthread_cond_destroy(&m_hHandle);
#endif // 
}

void wmEvent::Set(void)
{
#ifdef WIN32
    ::SetEvent(m_hHandle);
#else
    pthread_cond_signal(&m_hHandle);
#endif //
}

void wmEvent::Reset(void)
{
#ifdef WIN32
    ::ResetEvent(m_hHandle);
#endif //
}

void wmEvent::Wait(wmMutex& oMutex)
{
#ifdef WIN32
    WaitForSingleObject(m_hHandle, INFINITE);
#else
    oMutex.Lock();
    pthread_cond_wait(&m_hHandle, &oMutex.Get());
    oMutex.Unlock();
#endif //
}

}
