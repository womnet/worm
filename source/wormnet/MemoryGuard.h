
#ifndef _WORMNET_SYSTEM_MEMORY_GUARD_H
#define _WORMNET_SYSTEM_MEMORY_GUARD_H

#ifdef WIN32
#include <windows.h>
#undef ERROR
#else
#include <memory.h>
#endif //

#include "wormnet/wm_types.h"
#include <assert.h>

namespace worm
{
template <class Type>
class WORM_DLL_EXPORT CMemoryGuard
{
public:
    CMemoryGuard(const uint32& dwCount=1024)
    {
        if (dwCount < 1024)
        {
            m_pBuffer = new Type[1024];
            memset(m_pBuffer, 0, sizeof(Type)*1024);
        }
        else
        {
            m_pBuffer = new Type[dwCount];
            memset(m_pBuffer, 0, sizeof(Type)*dwCount);
        }
    }
    ~CMemoryGuard()
    {
        delete[] m_pBuffer;
        m_pBuffer = NULL;
    }

    Type* GetBuffer(void)
    {
        return m_pBuffer;
    }
    
private:
    Type*       m_pBuffer;
};
}

#endif // ifndef _WORMNET_SYSTEM_MEMORY_GUARD_H
