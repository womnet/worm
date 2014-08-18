
#ifndef _SHRAK_CIRCULAR_QUEUE_H
#define _SHRAK_CIRCULAR_QUEUE_H

#include <assert.h>
#include <shrak/shrak_types.h>

namespace Shrak
{

enum ESeekDef
{
    /// 开始
    ESD_BEGIN   = 0, 
    /// 结束
    ESD_END     = 1, 
};

template <class TEle> class WORM_DLL_EXPORT CCircularQueue
{
public:
    CCircularQueue()
    {
        m_dwHead -= m_dwHead;
        m_dwTail -= m_dwTail;
        m_dwCount -= m_dwCount;
        m_dwSize -= m_dwSize;
        m_dwNext -= m_dwNext;
        m_dwNextOdd -= m_dwNextOdd;

        m_pEleList = NULL;
    }

    void resize(const uint32& dwSize)
    {
        assert(0 != dwSize);

        m_dwSize = dwSize;
        m_pEleList = new TEle[dwSize];
    }
    ///   
    /// 添加一个元素到队列中
    /// @param ele 要添加的元素
    /// @return 无
    /// @see 
    /// @note 队列满时会覆盖最旧的一个
    /// 
    uint32 push(const TEle& ele)
    {
        assert(NULL != m_pEleList);

        uint32 dwRet = m_dwTail;

        if (m_dwCount == m_dwSize)
        {
            m_pEleList[m_dwTail] = ele;
            ++m_dwTail;
            m_dwTail %= m_dwSize;
            ++m_dwHead;
            m_dwHead %= m_dwSize;
        }
        else
        {
            m_pEleList[m_dwTail] = ele;
            ++m_dwCount;

            ++m_dwTail;
            m_dwTail %= m_dwSize;
        }

        return dwRet;
    }

    void remove_front(void)
    {
        assert(NULL != m_pEleList);

        if (m_dwHead != m_dwTail)
        {
            ++m_dwHead;
            m_dwHead %= m_dwSize;
        }
    }

    void seek(const ESeekDef& eSeek)
    {
        assert(NULL != m_pEleList);

        switch (eSeek)
        {
        case ESD_END:
            m_dwNext = m_dwTail;
            m_dwNextOdd -= m_dwNextOdd;
            break;
        case ESD_BEGIN:
        default:
            m_dwNext = m_dwHead;
            m_dwNextOdd = m_dwCount;
        }
    }

    bool next(TEle& ele)
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

    uint32 begin(void)
    {
        return m_dwHead;
    }
    uint32 end(void)
    {
        return m_dwTail;
    }
    uint32 count(void)
    {
        return m_dwCount;
    }

private:
    /// 当前读取位置
    uint32          m_dwHead;
    /// 当前写入位置
    uint32          m_dwTail;
    /// 当前有效数量
    uint32          m_dwCount;
    /// 队列大小
    uint32          m_dwSize;

    /// 队列信息
    TEle*           m_pEleList;

    /// 读取下一个的索引
    uint32          m_dwNext;
    /// 剩余的数量
    uint32          m_dwNextOdd;
};

}

#endif // ifndef _SHRAK_CIRCULAR_QUEUE_H
