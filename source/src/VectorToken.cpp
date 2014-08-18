
#include <assert.h>
#include "wormnet/VectorToken.h"

namespace worm
{

SVectorTokens::SVectorTokens(const std::string &strSrc, const char& cSep, const uint32& dwReserve /*= 0*/)
{
    Token(strSrc.c_str(), cSep, dwReserve);
}

SVectorTokens::SVectorTokens(const char* pSrc, const char& cSep, const uint32& dwReserve /*= 0*/)
{
    Token(pSrc, cSep, dwReserve);
}

SVectorTokens::SVectorTokens()
{
    m_pStr = NULL;
    m_dwBufferLen -= m_dwBufferLen;
}

SVectorTokens::SVectorTokens(SVectorTokens& oVecTokens)
{
    assert(0 != oVecTokens.m_dwBufferLen);

    if (NULL != m_pStr)
    {
        delete []m_pStr;
    }

    m_dwBufferLen = oVecTokens.m_dwBufferLen;
    m_pStr = new char[m_dwBufferLen];
    memcpy(m_pStr, oVecTokens.m_pStr, m_dwBufferLen);

    clear();
    char* pTmp = m_pStr;
    for (uint32 i=0; i<oVecTokens.size(); ++i)
    {
        push_back(pTmp);
        pTmp += strlen(oVecTokens[i]);
        ++pTmp;
    }
}

void SVectorTokens::operator=(SVectorTokens& oVecTokens)
{
    assert(0 != oVecTokens.m_dwBufferLen);

    if (NULL != m_pStr)
    {
        delete []m_pStr;
    }

    m_dwBufferLen = oVecTokens.m_dwBufferLen;
    m_pStr = new char[m_dwBufferLen];
    memcpy(m_pStr, oVecTokens.m_pStr, m_dwBufferLen);

    clear();
    char* pTmp = m_pStr;
    for (uint32 i=0; i<oVecTokens.size(); ++i)
    {
        push_back(pTmp);
        pTmp += strlen(oVecTokens[i]);
        ++pTmp;
    }
}

SVectorTokens::~SVectorTokens()
{
    delete []m_pStr;
    m_pStr = NULL;
}

void SVectorTokens::Token(const char* pSrc, const char& cSep, const uint32& dwReserve /*= 0*/)
{
    if (NULL == pSrc)
    {
        return;
    }

    if (NULL != m_pStr)
    {
        delete []m_pStr;
    }

    m_dwBufferLen = strlen(pSrc) + 1;
    m_pStr = new char[m_dwBufferLen];

    memcpy(m_pStr, pSrc, m_dwBufferLen);
    if (0 != dwReserve)
    {
        reserve(dwReserve);
    }

    clear();

    char* pOsold = m_pStr;
    char* pOsnew = m_pStr;

    for (;;)
    {
        if (cSep == *pOsnew)
        {
            push_back(pOsold);
            pOsold = pOsnew + 1;

            *pOsnew = 0;
        }
        else if (*pOsnew == 0)
        {
            if (pOsold != pOsnew)
            {
                push_back(pOsold);
            }

            break;
        }

        ++pOsnew;
    }
}

}
