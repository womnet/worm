
#ifndef _WORMNET_LIST_H
#define _WORMNET_LIST_H

#include "wormnet/wm_std.h"

namespace worm
{

template <class TElement> class WORM_DLL_EXPORT wmList
{
public:
    wmList()
    {
        m_pTailNode = NULL;
        m_pHeaderNode = NULL;
    }
    ~wmList()
    {
        if (NULL != m_pHeaderNode)
        {
            Release(m_pHeaderNode);
            m_pHeaderNode = NULL;
            m_pTailNode = NULL;
        }
    }

    struct SWormListNode
    {
        TElement        m_oNode;
        /// 指向下一个
        SWormListNode*  m_pNext;
        /// 指向上一个
        SWormListNode*  m_pPrev;

        SWormListNode()
        {
            m_pNext = NULL;
            m_pPrev = NULL;
        }
    };

    SWormListNode* front() { return m_pHeaderNode; }
    void push_back(const TElement& oEle)
    {
        if (NULL == m_pHeaderNode)
        {
            m_pHeaderNode = new SWormListNode;
            m_pHeaderNode->m_oNode = oEle;
            m_pTailNode = m_pHeaderNode;
        }
        else
        {
            m_pTailNode->m_pNext = new SWormListNode;
            m_pTailNode->m_pNext->m_pPrev = m_pTailNode;
            m_pTailNode = m_pTailNode->m_pNext;
            m_pTailNode->m_oNode = oEle;
        }
    }
    void erase(SWormListNode* pNode)
    {
        /// 是第一个
        if (NULL == pNode->m_pPrev)
        {
            /// 是不是只有一个
            if (NULL == pNode->m_pNext)
            {
                m_pHeaderNode = NULL;
                m_pTailNode = NULL;
            }
            else
            {
                m_pHeaderNode = pNode->m_pNext;
                pNode->m_pNext->m_pPrev = NULL;
            }
        }
        else
        {
            pNode->m_pPrev->m_pNext = pNode->m_pNext;
            if (NULL != pNode->m_pNext)
            {
                pNode->m_pNext->m_pPrev = pNode->m_pPrev;
            }
            else
            {
                m_pTailNode = pNode->m_pPrev;
            }
        }

        delete pNode;
        pNode = NULL;
    }

private:
    void Release(SWormListNode* pNode)
    {
        if (NULL != pNode->m_pNext)
        {
            Release(pNode->m_pNext);
        }
        delete pNode;
        pNode = NULL;
    }

private:
    SWormListNode*      m_pHeaderNode;
    SWormListNode*      m_pTailNode;
};
}

#endif // ifndef _WORMNET_LIST_H
