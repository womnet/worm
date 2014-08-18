
/***********************************************************
Copyright (c) 2011, 妙妙岛工作室

T定时器的一些实现

文件历史:
修改者: 	    IDK
*************************************************************/

#ifndef SRAK_SHARE_TIMER_H_
#define SRAK_SHARE_TIMER_H_

#include "wormnet/wm_time.h"

namespace worm
{
// 
// 结构名称 : SIntervalTimer
// 功能     : 一个时间间隔的定时器, 定时器使用单位: 毫秒
// 作者     : IDK
//
struct SIntervalTimer
{
public:
    SIntervalTimer()
    {
        m_nInterval -= m_nInterval;
        m_dwLapse -= m_dwLapse;
    }

    //
    // 函数名   : Update
    // 功能     : 当前时间累加上流失的时间, 更新为当前时间
    // 参数     : 
    //          [in] nDiff: 流失的时间, 单位(毫秒)
    //
    void Update(const uint32& nDiff)
    {
        m_dwLapse += nDiff;
    }
    //
    // 函数名   : IsPassed
    // 功能     : 是否暂停了
    // 返回值   : true 已经暂停
    //
    bool IsPassed()
    {
        return (m_dwLapse>=m_nInterval);
    }
    //
    // 函数名   : Reset
    // 功能     : 复位当前计时
    //
    void Reset()
    {
        if (m_dwLapse >= m_nInterval)
        {
            m_dwLapse -= m_nInterval;
        }
    }
    //
    // 函数名   : SetCurrent
    // 功能     : 强制更新当前时间为指定时间
    // 参数     : 
    //          [in] current: 新时间
    //
    void SetCurrent(uint32 current)
    {
        m_dwLapse = current;
    }
    //
    // 函数名   : SetInterval
    // 功能     : 设置定时器间隔
    // 参数     : 
    //          [in] interval: 新间隔
    //
    void SetInterval(uint32 interval)
    {
        m_nInterval = interval;
    }
    //
    // 函数名   : GetInterval
    // 功能     : 获取当前设置的间隔
    // 返回值   : 秒
    //
    uint32 GetInterval() const
    {
        return m_nInterval;
    }
    //
    // 函数名   : GetCurrent
    // 功能     : 获取当前流失的时间
    // 返回值   : 秒
    //
    uint32 GetLapse() const
    {
        return m_dwLapse;
    }
    uint32 GetRequiredSecond() const
    {
        if (m_nInterval > m_dwLapse)
        {
            return (m_nInterval - m_dwLapse)/1000; 
        }
        else
        {
            return 0;
        }
    }

private:
    // 设定的触发间隔时间, 单位: 毫秒
    uint32		m_nInterval;
    // 当前流失的时间
    uint32		m_dwLapse;
};

// 
// 结构名称 : STimeTracker
// 功能     : 判断时间到期的时间追踪器
// 作者     : IDK
//
struct STimeTracker
{
public:
    STimeTracker()
    {
        m_dwExpiry -= m_dwExpiry;
    }
    //
    // 函数名   : STimeTracker
    // 功能     : 构造函数
    // 参数     : 
    //          [in] dwExpiry: 定时时间. 单位: 毫秒
    //
    STimeTracker(const uint32& dwExpiry)
    {
        m_dwExpiry = dwExpiry;
    }

    ~STimeTracker() {}
    //
    // 函数名   : Update
    // 功能     : 当前时间减去上流失的时间更新为当前时间
    // 参数     : 
    //          [in] timeDiff: 流失的时间
    //
    void Update(uint32 timeDiff)
    {
        if (m_dwExpiry > timeDiff)
        {
            m_dwExpiry -= timeDiff;
        }
        else
        {
            m_dwExpiry -= m_dwExpiry;
        }
    }
    //
    // 函数名   : IsPassed
    // 功能     : 是否暂停了
    // 返回值   : true 已经暂停
    //
    bool IsPassed() const
    {
        return (0 == m_dwExpiry);
    }
    //
    // 函数名   : Reset
    // 功能     : 复位当前计时器, 并指定新的定时
    // 参数     : 
    //          [in] dwInterval: 新的时间间隔
    //
    void Reset(uint32 dwInterval)
    {
        //assert(dwInterval > 0);
        m_dwExpiry = dwInterval;
    }
    //
    // 函数名   : GetExpiry
    // 功能     : 设置当前剩余的时间. 单位: 秒
    // 返回值   : >= 0 的剩余秒
    //
    uint32 GetExpiry() const
    {
        return m_dwExpiry;
    }

protected:
    // 到期时间
    uint32 m_dwExpiry;
};

// 
// 结构名称 : SPeriodicTimer
// 功能     : 一个循环的定时器, 定时单位: 毫秒
// 作者     : IDK
//
struct SPeriodicTimer
{
public:
    //
    // 函数名   : SPeriodicTimer
    // 功能     : 构造函数
    // 参数     : 
    //          [in] nPeriod: 循环时间??
    //          [in] nStartTime: 开始时间
    //
    SPeriodicTimer(int32 nPeriod, int32 nStartTime)
    {
        m_nPeriod = nPeriod;
        m_nExpireTime = nStartTime;
    }
    //
    // 函数名   : Update
    // 功能     : 更新当前计, 如果一轮完成则开始下一轮
    // 参数     : 
    //          [in] nDiff: 流失的时间
    // 返回值   : false 一轮未循环未完成时; true 进入下一轮
    //
    bool Update(const uint32& nDiff)
    {
        m_nExpireTime -= nDiff;
        if (m_nExpireTime > 0)
        {
            return false;
        }

        m_nExpireTime += (m_nPeriod>int32(nDiff) ? m_nPeriod : nDiff);

        return true;
    }
    //
    // 函数名   : SetPeriodic
    // 功能     : 设置循环时间和开始时间
    // 参数     : 
    //          [in] nPeriod: 循环时间??
    //          [in] nStartTime: 开始时间
    //
    void SetPeriodic(int32 nPeriod, int32 nStartTime)
    {
        m_nExpireTime = nStartTime;
        m_nPeriod = nPeriod;
    }
    //
    // 函数名   : TUpdate
    // 功能     : 更新当前计时
    // 参数     : 
    //          [in] nDiff: 流失的时间
    //
    void TUpdate(int32 nDiff)
    {
        m_nExpireTime -= nDiff;
    }
    //
    // 函数名   : TUpdate
    // 功能     : 更新当前计时
    // 参数     : 
    //          [in] nDiff: 流失的时间
    //
    bool TPassed() const
    {
        return m_nExpireTime <= 0;
    }
    //
    // 函数名   : TReset
    // 功能     : 立即进入下一轮循环
    // 参数     : 
    //          [in] nDiff: 流失的时间
    //          [in] nPeriod: 循环时间
    //
    void TReset(int32 nDiff, int32 nPeriod)
    {
        m_nExpireTime += nPeriod>nDiff ? nPeriod : nDiff;
    }

private:
    int32       m_nPeriod;
    int32       m_nExpireTime;
};

}

#endif //
