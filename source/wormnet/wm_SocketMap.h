
#ifndef _WORMNET_HASH_MAP_H
#define _WORMNET_HASH_MAP_H

#include "wormnet/wm_list.h"

namespace worm
{

struct SHashValue
{
    uint64                  m_qwKey;
    /// 为NULL时未使用
    class ISocketHandler*   m_pValue;

    SHashValue()
    {
        m_qwKey -= m_qwKey;
        m_pValue = NULL;
    }
};

struct SHashValueList
{
    /// 第一个值
    SHashValue              m_sValue;
    /// 有冲突时的保存到列表
    wmList<SHashValue>      m_SocketList;
};

class WORM_DLL_EXPORT wmSocketMap
{
public:
    wmSocketMap();
    ~wmSocketMap();

    void Init(uint32 dwSize);

    void insert(const uint64& qwKey, class ISocketHandler*& pValue);
    SHashValue* find(const uint64& qwKey);
    void erase(const uint64& qwKey);

private:
    uint64 hash_key(const uint64& qwKey);

private:
    SHashValueList* m_pHashMap;
    uint32          m_dwSize;
};

}

#endif // ifndef _WORMNET_MAP_H
