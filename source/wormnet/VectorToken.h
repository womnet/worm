
#ifndef _WORMNET_VECTOR_TOKEN_H
#define _WORMNET_VECTOR_TOKEN_H

#include "wormnet/wm_std.h"
#include "wormnet/wm_types.h"
#include <vector>
#include <string>

namespace worm
{

struct SVectorTokens: public std::vector<char*>
{    
    SVectorTokens();
    SVectorTokens(SVectorTokens& oVecTokens);
    void operator=(SVectorTokens& oVecTokens);

    void Token(const char* pSrc, const char& cSep, const uint32& dwReserve = 0);
    SVectorTokens(const std::string &strSrc, const char& cSep, const uint32& dwReserve = 0);
    SVectorTokens(const char* pSrc, const char& cSep, const uint32& dwReserve = 0);
    ~SVectorTokens();

    char* GetSource() { return m_pStr; }

private:
    char*       m_pStr;
    /// new出来的数据长度
    uint32      m_dwBufferLen;
};

}

#endif
