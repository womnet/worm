
#ifndef _WORMNET_CRYPTOGRAPHY_INTERFACE_H
#define _WORMNET_CRYPTOGRAPHY_INTERFACE_H

#include "wormnet/wm_types.h"

namespace worm
{

class WORM_DLL_EXPORT ICryp
{
public:
    virtual ~ICryp() {}

    ///   
    /// 加密数据
    /// @param pInData 要加密的数据
    /// @param dwDataSize 数据长度
    /// @param pOutBuf 加密后的数据缓存
    /// @param pKey 密钥
    /// @param dwKeyLen 密钥长度
    /// @note pInData 可以与 pOutBuf 指向同一块内存
    /// 
    virtual bool Encryption(void* pInData, const uint32& dwDataSize, void* pOutBuf, const uint8* pKey, const uint32& dwKeyLen) = 0;
    ///   
    /// 解密数据
    /// @see Encryption
    /// 
    virtual bool Decryption(void* pInData, const uint32& dwDataSize, void* pOutBuf, const uint8* pKey, const uint32& dwKeyLen) = 0;

protected:
    ICryp() {}
};

enum wmECrypType
{
    /// tea
    WM_CRYP_TYPE_TEA    = 0, 
};

WORM_DLL_EXPORT ICryp* ICreateCrypModule(const wmECrypType& eCrypType);
WORM_DLL_EXPORT void IReleaseCrypModule(ICryp*& pCryp);

}

#endif // ifndef _WORMNET_CRYPTOGRAPHY_INTERFACE_H
