////////////////////////////////////////////////////////////////////// 
///    COPYRIGHT NOTICE 
///    Copyright (c) 2012, IDK
///    All rights reserved. 
/// 
/// @file CircularArray.h
/// @brief 一个通用的环状列表
/// 
/// @author idk
/// @date 修改日期: 2012年10月27日
//////////////////////////////////////////////////////////////////////

#ifndef _WORMNET_CIRCULAR_ARRAY_H
#define _WORMNET_CIRCULAR_ARRAY_H

#include <assert.h>
#include <shrak/shrak_types.h>

namespace worm
{

template <class T> class WORM_DLL_EXPORT CCircularQueue
{
public:
    CCircularQueue()
    {
        m_dwBegin -= m_dwBegin;
        m_dwEnd -= m_dwEnd;
        m_dwCount -= m_dwCount;
        m_dwSize -= m_dwSize;
        m_dwNext -= m_dwNext;
        m_dwNextOdd -= m_dwNextOdd;

        m_pEleList = NULL;
    }
    ///  
    /// 设置环列表的长度
    /// @param dwSize 大小
    /// @return 无
    /// @note 已经保存的数据将全部删除
    /// 
    void resize(const uint32& dwSize)
    {       
        if (0 != m_dwSize)
        {
            delete []m_pEleList;
            m_pEleList = NULL;

            m_dwBegin -= m_dwBegin;
            m_dwEnd -= m_dwEnd;
            m_dwCount -= m_dwCount;
            m_dwSize -= m_dwSize;
            m_dwNext -= m_dwNext;
            m_dwNextOdd -= m_dwNextOdd;
        }

        if (0 == m_dwSize)
        {
            assert(0);
            m_dwSize = 256;
        }
        else
        {
            m_dwSize = dwSize;
        }

        m_dwSize = dwSize;
        m_pEleList = new T[dwSize];
    }
    ///   
    /// 添加一个元素到队列中
    /// @param ele 要添加的元素
    /// @return 无
    /// @see 
    /// @note 队列满时会覆盖最旧的一个
    /// 
    uint32 push(const T& ele)
    {
        assert(NULL != m_pEleList);

        uint32 dwRet = m_dwEnd;

        if (m_dwCount == m_dwSize)
        {
            m_pEleList[m_dwEnd] = ele;
            ++m_dwEnd;
            m_dwEnd %= m_dwSize;
            ++m_dwBegin;
            m_dwBegin %= m_dwSize;
        }
        else
        {
            m_pEleList[m_dwEnd] = ele;
            ++m_dwCount;

            ++m_dwEnd;
            m_dwEnd %= m_dwSize;
        }

        return dwRet;
    }
    ///  
    /// 从当前列表头删除一个元素
    /// @param  无
    /// @return 无
    /// 
    void remove_front(void)
    {
        assert(NULL != m_pEleList);

        if (m_dwBegin != m_dwEnd)
        {
            ++m_dwBegin;
            m_dwBegin %= m_dwSize;
        }
    }
    ///   
    /// 从开始位置后偏移指定位置
    /// @param dwSeek 偏移的数量
    /// @return 无
    /// @see 
    /// @note 偏移数量不能大于有效数量
    /// 
    void seek(const uint32& dwSeek)
    {
        assert(NULL != m_pEleList);
        if (dwSeek > m_dwCount)
        {
            assert(0);
            return;
        }

        uint32 dwTmp = m_dwSize - m_dwBegin;

        if (dwSeek >= dwTmp)
        {
            m_dwNext = dwSeek - dwTmp;
        }
        else
        {
            m_dwNext = dwSeek;
        }

        m_dwNextOdd = m_dwCount - dwSeek;
    }
    ///  
    /// 列表头
    /// @param ele 
    /// @return 
    /// @see 
    /// @note 
    /// 
    bool next(T& ele)
    {
        assert(NULL != m_pEleList);

        if (0 != m_dwNextOdd)
        {
            ele = m_pEleList[m_dwNext];
            ++m_dwNext;
            m_dwNext %= m_dwSize;
			--m_dwNextOdd;
            return true;
        }
        else
        {
            return false;
        }
    }
    ///  
    /// 获取当前有效的数量
    /// @param  无
    /// @return 有效数量
    /// 
    uint32 count(void) const
    {
        return m_dwCount;
    }

private:
    /// 当前读取位置
    uint32          m_dwBegin;
    /// 当前写入位置
    uint32          m_dwEnd;
    /// 当前有效数量
    uint32          m_dwCount;
    /// 队列大小
    uint32          m_dwSize;

    /// 队列信息
    T*              m_pEleList;

    /// 读取下一个的索引
    uint32          m_dwNext;
    /// 剩余的数量
    uint32          m_dwNextOdd;
};

}

#endif // ifndef _WORMNET_CIRCULAR_ARRAY_H
