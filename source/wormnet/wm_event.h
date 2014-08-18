
#ifndef _WORMNET_SYSTEM_EVENT_H
#define _WORMNET_SYSTEM_EVENT_H

#ifndef WIN32
#include <pthread.h>
#endif //
#include "wormnet/wm_mutex.h"

namespace worm
{

class WORM_DLL_EXPORT wmEvent
{
public:
    wmEvent();
    ~wmEvent();

    void Set(void);
    void Reset(void);
    ///   
    /// 等待, 只到事件为有效
    /// @param 无
    /// @note 如果无事件会一直阻塞
    /// 
    void Wait(wmMutex& oMutex);

private:
#ifdef WIN32
    HANDLE              m_hHandle;
#else
    pthread_cond_t      m_hHandle;
#endif // ifdef WIN32
};

}

#endif // ifndef _WORMNET_SYSTEM_EVENT_H
