
#include "wormnet/wm_std.h"
#include <string.h>
#include <math.h>
#include "wormdb/DbField.h"

namespace worm
{

CDbField::CDbField(void)
{
    m_szValue = NULL;
}

CDbField::~CDbField()
{
    if(NULL != m_szValue)
    {
        delete []m_szValue;
        m_szValue = NULL;
    }
}

void CDbField::SetValue(const char *pStrValue)
{
    if(NULL != m_szValue)
    {
        delete []m_szValue;
        m_szValue = NULL;
    }

    if (NULL != pStrValue)
    {
        m_szValue = new char[strlen(pStrValue)+1];
        strcpy(m_szValue, pStrValue);
    }
    else
    {
        m_szValue = NULL;
    }
}

float CDbField::GetFloat() const
{
    if (NULL != m_szValue)
    {
        return static_cast<float>(atof(m_szValue));
    }
    else
    {
        return 0.0f;
    }
}

bool CDbField::GetBool() const
{
    if (NULL != m_szValue)
    {
        return (atoi(m_szValue)>0);
    }
    else
    {
        return false;
    }
}

uint8 CDbField::GetUInt8() const
{
    uint8 nRet = 0;

    if (NULL != m_szValue)
    {
        nRet = atoi(m_szValue) & 0xff;
    }

    return nRet;
}

int8 CDbField::GetInt8(void)
{
    return int8(GetUInt8());
}

uint16 CDbField::GetUInt16() const
{
    uint16 nRet = 0;
    if (NULL != m_szValue)
    {
        nRet = atoi(m_szValue) & 0xffff;
    }

    return nRet;
}

short CDbField::GetInt16() const
{
    return int16(GetUInt16());
}

uint32 CDbField::GetUInt32() const
{
    uint32 nRet = 0;
    if (NULL != m_szValue)
    {
        sscanf(m_szValue, "%u", &nRet);
    }

    return nRet;
}

int32 CDbField::GetInt32() const
{
    int32 nRet = 0;
    if (NULL != m_szValue)
    {
        sscanf(m_szValue, "%d", &nRet);
    }

    return nRet;
}

uint64 CDbField::GetUInt64() const
{
    uint64 value = 0;
    if (NULL != m_szValue)
    {
        sscanf(m_szValue, "%llu", &value);		
    }

    return value;
}

int64 CDbField::GetInt64() const
{
    int64 nRet = 0;
    if (NULL != m_szValue)
    {
        sscanf(m_szValue, "%lld", &nRet);
    }

    return nRet;
}

}
