
#ifndef _WORMNET_DBI_IDATABASE_MODULE_H
#define _WORMNET_DBI_IDATABASE_MODULE_H

#include "wormnet/wm_types.h"

enum wmEDatabaseType
{
    /// mysql
    WM_DB_MYSQL         = 0, 
    /// microsoft sql server
    WM_DB_MSSQL         = 1, 
    /// oracle
    WM_DB_ORACLE        = 2, 

    MAX_WM_DB_TYPE, 
};

namespace worm
{

///   
/// 新建一个数据库模块接口
/// @param eDbType 使用的数据库类型
/// @return !=NULL为成功
/// 
WORM_DLL_EXPORT class wmDatabase* ICreateDatabaseModule();
///   
/// 释放一个数据库模块接口
/// @param pDatabase 数据库模块指针
/// @return
/// @see
/// @note 释放后此数据库不再可用
/// 
WORM_DLL_EXPORT void IReleaseDatabaseModule(class wmDatabase*& pDatabase);

}

#endif // ifndef _WORMNET_DBI_IDATABASE_MODULE_H
