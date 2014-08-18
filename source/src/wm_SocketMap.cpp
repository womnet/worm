
#include "wormnet/wm_SocketMap.h"

namespace worm
{

#define MIN_HASH_MAP    1024

wmSocketMap::wmSocketMap()
{
    m_pHashMap = NULL;
}

wmSocketMap::~wmSocketMap()
{
    delete m_pHashMap;
    m_pHashMap = NULL;
}

void wmSocketMap::Init(uint32 dwSize)
{
    if (dwSize < MIN_HASH_MAP)
    {
        dwSize = MIN_HASH_MAP;
    }
    m_dwSize = dwSize;

    m_pHashMap = new SHashValueList[dwSize];
}

void wmSocketMap::insert(const uint64& qwKey, class ISocketHandler*& pValue)
{
    assert(NULL != m_pHashMap);
    uint32 dwIndex = hash_key(qwKey) % m_dwSize;
    if (qwKey == m_pHashMap[dwIndex].m_sValue.m_qwKey)
    {
        /// 已经有一个相同的则直接覆盖
        m_pHashMap[dwIndex].m_sValue.m_pValue = pValue;
    }
    else if (NULL == m_pHashMap[dwIndex].m_sValue.m_pValue)
    {
        /// 直接命中
        m_pHashMap[dwIndex].m_sValue.m_qwKey = qwKey;
        m_pHashMap[dwIndex].m_sValue.m_pValue = pValue;
    }
    else
    {
        /// 需要添加到列表中
        wmList<SHashValue>::SWormListNode* pNode = m_pHashMap[dwIndex].m_SocketList.front();
        for (; NULL!=pNode; pNode=pNode->m_pNext)
        {
            if (qwKey == pNode->m_oNode.m_qwKey)
            {
                break;
            }
        }

        if (NULL == pNode)
        {
            SHashValue sValue;
            sValue.m_qwKey = qwKey;
            sValue.m_pValue = pValue;
            m_pHashMap[dwIndex].m_SocketList.push_back(sValue);
        }
        else
        {
            pNode->m_oNode.m_pValue = pValue;
        }
    }
}

SHashValue* wmSocketMap::find(const uint64& qwKey)
{
    assert(NULL != m_pHashMap);

    uint32 dwIndex = hash_key(qwKey) % m_dwSize;
    if (qwKey == m_pHashMap[dwIndex].m_sValue.m_qwKey)
    {
        return &m_pHashMap[dwIndex].m_sValue;
    }
    else
    {
        wmList<SHashValue>::SWormListNode* pNode = m_pHashMap[dwIndex].m_SocketList.front();
        for (; NULL!=pNode; pNode=pNode->m_pNext)
        {
            if (qwKey == pNode->m_oNode.m_qwKey)
            {
                return &pNode->m_oNode;
            }
        }
    }

    return NULL;
}

void wmSocketMap::erase(const uint64& qwKey)
{
    assert(NULL != m_pHashMap);

    uint32 dwIndex = hash_key(qwKey) % m_dwSize;
    if (qwKey == m_pHashMap[dwIndex].m_sValue.m_qwKey)
    {
        m_pHashMap[dwIndex].m_sValue.m_qwKey -= m_pHashMap[dwIndex].m_sValue.m_qwKey;
        m_pHashMap[dwIndex].m_sValue.m_pValue = NULL;
    }
    else
    {
        wmList<SHashValue>::SWormListNode* pNode = m_pHashMap[dwIndex].m_SocketList.front();
        for (; NULL!=pNode; pNode=pNode->m_pNext)
        {
            if (qwKey == pNode->m_oNode.m_qwKey)
            {
                m_pHashMap[dwIndex].m_SocketList.erase(pNode);
                break;
            }
        }
    }
}

uint64 wmSocketMap::hash_key(const uint64& qwKey)
{
    uint8* pData = (uint8*)&qwKey;
    uint64 h = pData[0];
    h = 31*h + pData[1];
    h = 31*h + pData[2];
    h = 31*h + pData[3];
    h = 31*h + pData[4];
    h = 31*h + pData[5];
    h = 31*h + pData[6];
    h = 31*h + pData[7];

    return h;    
}

}
