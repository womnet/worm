
#include "Tea.h"

namespace worm
{

//加密轮数
#define TEA_ROUNDS		16
//解密轮相关参数
#define TEA_LOG_ROUNDS	4
//一个关键数
#define TEA_DELTA		0x9e3779b9

//加解密时处理的数据
#define TEA_TMP_Y	(*((int32*)((uint8*)pOutBuf+(i<<2))))
#define TEA_TMP_Z	(*((int32*)((uint8*)pOutBuf+((i+1)<<2))))

CTea::CTea()
{

}

CTea::~CTea()
{

}

bool CTea::Encryption(void* pInData, const uint32& dwDataSize, void* pOutBuf, const uint8* pKey, const uint32& dwKeyLen)
{
    if (0==dwDataSize || (dwDataSize&7))
    {
        return false;
    }

    uint32 nSum = 0;

    uint32 i = 0;
    for (; i<(dwDataSize>>2); i+=2)
    {
        TEA_TMP_Y = *((uint32*)((uint8*)pInData+(i<<2)));
        TEA_TMP_Z = *((uint32*)((uint8*)pInData+((i+1)<<2)));

        for (int j=0; j<TEA_ROUNDS; j++)
        {   
            nSum += TEA_DELTA;
            TEA_TMP_Y += ((TEA_TMP_Z<<4)+pKey[0]) ^ (TEA_TMP_Z+nSum) ^ ((TEA_TMP_Z>>5)+pKey[1]);
            TEA_TMP_Z += ((TEA_TMP_Y<<4)+pKey[2]) ^ (TEA_TMP_Y+nSum) ^ ((TEA_TMP_Y>>5)+pKey[3]);
        }
        nSum -= nSum;
    }

    return true;
}

bool CTea::Decryption(void* pInData, const uint32& dwDataSize, void* pOutBuf, const uint8* pKey, const uint32& dwKeyLen)
{
    if (0==dwDataSize || (dwDataSize&7))
    {
        return false;
    }

    uint32 nSum = TEA_DELTA << TEA_LOG_ROUNDS;
    for (uint32 i=0; i<(dwDataSize>>2); i+=2)
    {
        TEA_TMP_Y = *((uint32*)((uint8*)pInData+(i<<2)));
        TEA_TMP_Z = *((uint32*)((uint8*)pInData+((i+1)<<2)));

        for (int j=0; j<TEA_ROUNDS; j++)
        {   
            TEA_TMP_Z -= ((TEA_TMP_Y<<4)+pKey[2]) ^ (TEA_TMP_Y+nSum) ^ ((TEA_TMP_Y>>5)+pKey[3]);
            TEA_TMP_Y -= ((TEA_TMP_Z<<4)+pKey[0]) ^ (TEA_TMP_Z+nSum) ^ ((TEA_TMP_Z>>5)+pKey[1]);
            nSum -= TEA_DELTA;
        }
        nSum = TEA_DELTA << TEA_LOG_ROUNDS;
    }

    return true;
}

}
