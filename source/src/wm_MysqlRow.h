
#ifndef _WORMNET_DBI_MYSQL_ROW_H
#define _WORMNET_DBI_MYSQL_ROW_H

#include "wormdb/DbRow.h"

namespace worm
{

class WORM_DLL_EXPORT wmMysqlRow : public CDbRow
{
    friend class wmMysqlResult;
public:
    ~wmMysqlRow();

private:
    wmMysqlRow(const uint32& dwFieldCount, void* pRow);
    wmMysqlRow();

};

}

#endif // ifndef _WORMNET_DBI_MYSQL_ROW_H
