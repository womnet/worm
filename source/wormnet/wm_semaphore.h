
#ifndef _WORMNET_SYSTEM_EVENT_H
#define _WORMNET_SYSTEM_EVENT_H

#ifdef WIN32
#include <Windows.h>
#undef ERROR
#else
#include <pthread.h>
#include <semaphore.h>
#endif //
#include "wormnet/wm_types.h"

namespace worm
{

class WORM_DLL_EXPORT wmSem
{
public:
    wmSem();
    ~wmSem();

    /// 
    /// 创建信号量
    /// @param dwInitialCount 该信号量的初始值
    /// @param dwMaxCount 该信号量的最大值，该值必须大于 0
    /// @return 无
    /// @see Windows CreateSemaphore; linux: sem_init
    /// @note 
    /// 
    void CreateSemaphore(const uint32& dwInitialCount, const uint32& dwMaximumCount);
    ///   
    /// 设置一个信号量
    /// @return 无
    /// @see Windows ReleaseSemaphore; linux: sem_post
    /// @note 
    /// 
    void PostSemaphore();
    ///   
    /// 等待信号量
    /// @return 无
    /// @see Windows WaitForSingleObject; Linux sem_wait
    /// @note 如果信号量>0会立即返回, 否则阻塞, 直到>0时返回
    /// 
    void WaitSemaphore();
    ///   
    /// 关闭信号量
    /// @return 无
    /// @see Windows CloseHandle; Linux: sem_destroy
    /// @note 
    /// 
    void DestorySemaphore();

protected:
    bool                m_bInitial;
#ifdef WIN32
    HANDLE              m_hHandle;
#else
    sem_t				m_hHandle;
#endif // ifdef WIN32
};

}

#endif // ifndef _WORMNET_SYSTEM_EVENT_H
