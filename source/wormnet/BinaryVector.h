
#ifndef _WORMNET_BINARY_VECTORY_H
#define _WORMNET_BINARY_VECTORY_H

#include <wm_Exception.h>

namespace worm
{

template <class TElement> class WORM_DLL_EXPORT CBinaryVector
{
public:
    CBinaryVector()
    {
        m_pEleList = NULL;
        m_ppEleBuf = NULL;
        m_dwIdle -= m_dwIdle;
        m_dwMax -= m_dwMax;

        resize(128);
    }
    ~CBinaryVector()
    {
        release();
    }

    void resize(const uint32& wMax)
    {
        if (m_dwMax >= wMax)
        {
            return;
        }
        release();

        if (0 == wMax)
        {
            throw wmException("CBinaryVector<TElement>::SetMax == 0");
            m_dwMax = 1;
        }
        else
        {
            m_dwMax = wMax;
        }
        m_ppEleBuf = new(TElement*, m_dwMax);
        m_pEleList = new(TElement, m_dwMax);

        ZeroMemory(m_ppEleBuf, sizeof(TElement*)*m_dwMax);
        ZeroMemory(m_pEleList, sizeof(TElement)*m_dwMax);

        m_dwIdle = m_dwMax;

        for (uint32 i=0; i<m_dwMax; ++i)
        {
            m_ppEleBuf[i] = &m_pEleList[i];
        }
    }
    void clear(void)
    {
        for (uint32 i=0; i<m_dwMax; ++i)
        {
            m_ppEleBuf[i] = &m_pEleList[i];
        }
        m_dwIdle = m_dwMax;
    }
    ///    
    /// 获得一个空闲的索引
    /// @param : 
    /// @return: 
    /// @see 
    /// @note 
    /// 
    uint32 get(void)
    {
        if (0 == m_dwIdle)
        {
            throw worm::wmException("CArrayLink::Get array is full");
            return m_dwMax;
        }

        --m_dwIdle;

        return m_dwIdle;
    }
    ///    
    /// 释放一个占用的索引
    /// @param wIndex: 
    /// @return: 
    /// @see 
    /// @note 
    /// 
    void erase(const uint32& wIndex)
    {
        if (wIndex>m_dwMax || m_dwIdle==m_dwMax)
        {
            assert(false);
            return;
        }

        if (m_dwIdle != wIndex)
        {
            TElement* pEle = m_ppEleBuf[wIndex];
            m_ppEleBuf[wIndex] = m_ppEleBuf[m_dwIdle];
            m_ppEleBuf[m_dwIdle] = pEle;
        }

        ++m_dwIdle;
    }

    ///    
    /// 获取开始的索引
    /// @param : 无
    /// @return: 开始索引值
    /// 
    uint32 begin(void) { return m_dwIdle; }
    ///    
    /// 队列的结束索引
    /// @param : 无
    /// @return: 结束索引
    /// 
    uint32 end(void) const { return m_dwMax; }
    ///    
    /// 判断队列是否满了
    /// @param : 无
    /// @return: true 为满了
    /// 
    bool full(void) const { return (0==m_dwIdle); }
    bool empty(void) const { return (m_dwMax==m_dwIdle); }
    uint32 capacity(void) const { return m_dwMax; }
    uint32 size(void) const { return (m_dwMax-m_dwIdle); }
    bool push_back(const TElement& ele)
    {
        if (0 == m_dwIdle)
        {
            //PrintError("CBinaryVector<TElement>::push_back array is full\n");
            return false;
        }
        --m_dwIdle;
        *(m_ppEleBuf[m_dwIdle]) = ele;
        return true;
    }

    TElement& operator[](const uint32& wIndex)
    {
        if (wIndex < m_dwMax)
        {
            return *(m_ppEleBuf[wIndex]);
        }

        throw worm::wmException("Shrak::CArrayLink<TElement>::operator[] index %u overflow!! max: %u", wIndex, m_dwMax);

        return m_pEleList[0];
    }
    /// 重载赋值运算
    CBinaryVector<TElement>& operator=(const CBinaryVector<TElement>& oArray)
    {
        if (this != &oArray)
        {
            resize(oArray.m_dwMax);
            m_dwIdle = oArray.m_dwIdle;
            uint32 dwIndex = 0;
            for (uint32 i=0; i<m_dwMax; ++i)
            {
                m_pEleList[i] = oArray.m_pEleList[i];
                dwIndex = oArray.m_ppEleBuf[i] - &oArray.m_pEleList[0];

                m_ppEleBuf[i] = &m_pEleList[dwIndex];
            }
        }

        return *this;
    }
    /// 重载拷贝构造函数
    CBinaryVector<TElement>(const CBinaryVector& oArray)
    {
        if (this != &oArray)
        {
            m_pEleList = NULL;
            m_ppEleBuf = NULL;
            m_dwIdle -= m_dwIdle;
            m_dwMax -= m_dwMax;

            resize(oArray.m_dwMax);
            m_dwIdle = oArray.m_dwIdle;
            uint32 dwIndex = 0;
            for (uint32 i=0; i<m_dwMax; ++i)
            {
                m_pEleList[i] = oArray.m_pEleList[i];
                dwIndex = oArray.m_ppEleBuf[i] - &oArray.m_pEleList[0];

                m_ppEleBuf[i] = &m_pEleList[dwIndex];
            }
        }
    }

private:
    void release(void)
    {
        if (NULL != m_pEleList)
        {
            delete[]m_pEleList);
            m_pEleList = NULL;
        }
        if (NULL != m_ppEleBuf)
        {
            delete[]m_ppEleBuf);
            m_ppEleBuf = NULL;
        }
    }

public:
    /// 保存数组列表
    TElement*           m_pEleList;
    TElement**          m_ppEleBuf;
    /// 数组大小
    uint32              m_dwMax;
    /// 可用的数量
    uint32              m_dwIdle;
};

}

#endif // ifndef _WORMNET_BINARY_VECTORY_H
