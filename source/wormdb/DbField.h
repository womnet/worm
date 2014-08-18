
#ifndef _WORMNET_DBI_FIELD_H
#define _WORMNET_DBI_FIELD_H

#include <stdio.h>
#include "wormnet/wm_types.h"

namespace worm
{

class WORM_DLL_EXPORT CDbField
{
    friend class wmMysqlRow;
    friend class wmMSSqlRow;

public:
    virtual ~CDbField();

    const char *GetString() const { return (NULL==m_szValue)?"":m_szValue; }
    float GetFloat() const;
    bool GetBool() const;
    uint8 GetUInt8() const;
    int8 GetInt8(void);
    uint16 GetUInt16() const;
    int16 GetInt16() const;
    uint32 GetUInt32() const;
    int32 GetInt32() const;
    uint64 GetUInt64() const;
    int64 GetInt64() const;

protected:
    CDbField();

    CDbField(const CDbField& oField);
    void operator=(const CDbField& oField);

    void SetValue(const char* pStrValue);

protected:
    /// 字段的原始内容
    char*			m_szValue;
};

}

#endif // ifndef _WORMNET_DBI_FIELD_H
