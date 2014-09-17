
#ifdef WIN32
#include <Windows.h>
#undef ERROR
#endif

#include "wormnet/wm_std.h"
#include "wm_MysqlResult.h"
#include <mysql/mysql.h>
#include "wm_MysqlRow.h"

namespace worm
{

wmMysqlResult::wmMysqlResult(void* const pResult, const uint32& dwRowCount, const uint32& dwFieldCount)
: CDbResult()
{
    m_dwRowCount = dwRowCount;

    m_dwCurRow -= m_dwCurRow;
    if (0 != dwRowCount)
    {
        MYSQL_ROW row = mysql_fetch_row((MYSQL_RES*)pResult);

        m_ppRow = new CDbRow*[m_dwRowCount];

        for ( uint32 i=0; NULL!=row; ++i)
        {
            m_ppRow[i] = new wmMysqlRow(dwFieldCount, row);
            row = mysql_fetch_row((MYSQL_RES*)pResult);
        }
    }
    else
    {
        m_ppRow = NULL;
    }
}

wmMysqlResult::~wmMysqlResult()
{
}

}
