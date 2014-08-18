
#ifndef _WORMNET_CRYPTOGRAPHY_TEA_H
#define _WORMNET_CRYPTOGRAPHY_TEA_H

#include "wormnet/wm_cryp.h"

namespace worm
{

class WORM_DLL_EXPORT CTea : public ICryp
{
public:
    CTea();
    ~CTea();
    
    bool Encryption(void* pInData, const uint32& dwDataSize, void* pOutBuf, const uint8* pKey, const uint32& dwKeyLen);
    bool Decryption(void* pInData, const uint32& dwDataSize, void* pOutBuf, const uint8* pKey, const uint32& dwKeyLen);
};

}

#endif // ifndef _WORMNET_CRYPTOGRAPHY_TEA_H
