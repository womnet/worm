
#include "wormdb/IDatabaseModule.h"
#include "wormdb/wm_database.h"
#include "wm_Mysql.h"

namespace worm
{

WORM_DLL_EXPORT class wmDatabase* ICreateDatabaseModule()
{
    wmDatabase* pDatabase = new(wmMysql);
    return pDatabase;
}

WORM_DLL_EXPORT void IReleaseDatabaseModule(class wmDatabase*& pDatabase)
{
    delete pDatabase;
    pDatabase = NULL;
}

}
