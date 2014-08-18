
#ifndef _WORMNET_DBI_MYSQL_RESULT_H
#define _WORMNET_DBI_MYSQL_RESULT_H

#include "wormdb/DbResult.h"

namespace worm
{

class WORM_DLL_EXPORT wmMysqlResult : public CDbResult
{
public:
    wmMysqlResult(void* const pResult, const uint32& dwRowCount, const uint32& dwFieldCount);
    ~wmMysqlResult();

private:
    wmMysqlResult();
};

}

#endif // ifndef _WORMNET_DBI_MYSQL_RESULT_H
