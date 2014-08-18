
#ifndef _WORMNET_CIRCULAR_BUFFER_H
#define _WORMNET_CIRCULAR_BUFFER_H

#include <assert.h>
#include "wormnet/wm_semaphore.h"
#include "wormnet/wm_mutex.h"

namespace worm
{
template <class T>
class WORM_DLL_EXPORT CCircularBuffer
{
public:
    CCircularBuffer()
    {
        m_bCancel = false;
        m_dwBegin -= m_dwBegin;
        m_dwEnd -= m_dwEnd;
        m_dwTotal -= m_dwTotal;
        m_dwCount -= m_dwCount;
        m_pQueue = NULL;

    }
    ~CCircularBuffer()
    {
        m_lockTask.Lock();
        m_bCancel = true;
        m_lockTask.Unlock();

        delete []m_pQueue;
        m_pQueue = NULL;
    }
    ///   
    /// 初始化环形数组
    /// @param nCount 数组大小
    /// @return 无
    /// @see 
    /// @note nCount 必须 > 0
    /// 
    void Initial(const uint32& dwCount)
    {
        if (0 != m_dwTotal)
        {
            delete []m_pQueue;
            m_pQueue = NULL;

            m_semNotEmpty.DestorySemaphore();
            m_semNotFull.DestorySemaphore();
        }
        m_dwTotal = dwCount;
        if (0 == dwCount)
        {
            assert(0);
            m_dwTotal = 1024;
        }
        m_pQueue = new T[m_dwTotal];

        m_semNotFull.CreateSemaphore(m_dwTotal, m_dwTotal);
        m_semNotEmpty.CreateSemaphore(0, m_dwTotal);
    }
    ///   
    /// 向数组中添加一条数据
    /// @param Item 要添加的数据
    /// @return 无
    /// @see 
    /// @note 
    /// 
    void Enqueue(T& Item)
    {
        assert(m_dwTotal > 0);

        if (IsCancel())
        {
            return;
        }

        m_semNotFull.WaitSemaphore();

        m_lockTask.Lock();
        m_pQueue[m_dwEnd] = Item;
        ++m_dwEnd;
        m_dwEnd = m_dwEnd % m_dwTotal;
        ++m_dwCount;
        m_lockTask.Unlock();

        m_semNotEmpty.PostSemaphore();
    }
    ///   
    /// 从队列中读取一条数据
    /// @param Item 用来保存读取的数据
    /// @return true 为成功, false 表示队列已经关闭
    /// @see 
    /// @note 
    /// 
    bool Dequeue(T& Item)
    {
        if (IsCancel())
        {
            return false;
        }

        m_semNotEmpty.WaitSemaphore();

        m_lockTask.Lock();
        Item = m_pQueue[m_dwBegin];
        ++m_dwBegin;
        m_dwBegin = m_dwBegin % m_dwTotal;
        --m_dwCount;
        m_lockTask.Unlock();

        m_semNotEmpty.PostSemaphore();

        return true;
    }
    ///   
    /// 功能详述
    /// @return 
    /// @see 
    /// @note 
    /// 
    virtual void Release()
    {
        m_lockTask.Lock();
        m_bCancel = true;
        m_dwBegin -= m_dwBegin;
        m_dwEnd -= m_dwEnd;
        m_dwCount -= m_dwCount;
        m_semNotEmpty.PostSemaphore();
        m_lockTask.Unlock();
    }
    //
    // 函数名   : IsCancel
    // 功能     : 判断此队列是否有效
    // 返回值   : true 无效
    //
    bool IsCancel(void)
    {
        m_lockTask.Lock();
        bool bRet = m_bCancel;
        m_lockTask.Unlock();
        return bRet;
    }

    bool full(void)
    {
        m_lockTask.Lock();
        bool bRet = (m_dwCount==m_dwTotal);
        m_lockTask.Unlock();

        return bRet;
    }
    bool empty()
    {
        m_lockTask.Lock();
        bool bRet = (0 == m_dwCount);
        m_lockTask.Unlock();

        return bRet;
    }
    uint32 size()
    {
        return m_dwTotal;
    }

private:
    // 非满信号量
    wmSem                   m_semNotFull;
    // 非空信号
    wmSem                   m_semNotEmpty;

    // 缓存读索引
    uint32                  m_dwBegin;
    // 缓存写索引
    uint32                  m_dwEnd;
    // 缓存的总数
    uint32                  m_dwTotal;
    // 缓存的当前以用的数量
    uint32                  m_dwCount;
    // 队列
    T*                      m_pQueue;
    // 队列是否已经取消
    bool                    m_bCancel;
    // 临界锁
    wmMutex                 m_lockTask;
};

}

#endif // ifndef _WORMNET_CIRCULAR_BUFFER_H
