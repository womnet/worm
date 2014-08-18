
#ifndef _WORMNET_DBI_ROW_H
#define _WORMNET_DBI_ROW_H

#include "wormdb/DbField.h"

namespace worm
{

class WORM_DLL_EXPORT CDbRow
{
    friend class wmMysqlResult;
    friend class wmMSSqlResult;

public:
    virtual ~CDbRow();

    CDbField* GetFields() { return m_pFields; }

protected:
    CDbRow();

protected:
    /// 此条记录中所有的字段内容
    CDbField*       m_pFields;
};

}

#endif /// ifndef _WORMNET_DBI_ROW_H
