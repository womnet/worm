
#ifndef _WORMNET_DBI_MYSQL_H
#define _WORMNET_DBI_MYSQL_H

#include "wormdb/wm_database.h"

namespace worm
{

class WORM_DLL_EXPORT wmMysql : public wmDatabase
{
public:
    wmMysql();
    ~wmMysql();

    bool Open(const char* pStrInfo);
    void Close(void);

    bool DirectExecute(const char* pStrSql);
    bool DirectExecute(CSqlTaskSet* const pTaskSet);
    bool Query(const char *pSql, CDbResult*& pDbResult);
    bool Query(CSqlTaskSet* const pTaskSet);
    void KeepAlive();

private:
    bool BeginTransaction();
    bool CommitTransaction();
    bool RollbackTransaction();

private:
    void*           m_pMysql;
};

}

#endif // ifndef _WORMNET_DBI_MYSQL_H
