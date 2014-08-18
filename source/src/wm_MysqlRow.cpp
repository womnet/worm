
#ifdef WIN32
#include <Windows.h>
#undef ERROR
#endif

#include <assert.h>
#include <mysql/mysql.h>
#include "wm_MysqlRow.h"
#include "wormdb/DbField.h"

namespace worm
{

wmMysqlRow::wmMysqlRow(const uint32& dwFieldCount, void* pRow) : CDbRow()
{
    if (0 == dwFieldCount)
    {
        assert(0);
        return;
    }

    m_pFields = new CDbField[dwFieldCount];
    for (uint32 i=0; dwFieldCount!=i; ++i)
    {
        m_pFields[i].SetValue((MYSQL_ROW(pRow))[i]);
    }
}

wmMysqlRow::~wmMysqlRow()
{
    if (NULL != m_pFields)
    {
        delete []m_pFields;
        m_pFields = NULL;
    }
}

}
