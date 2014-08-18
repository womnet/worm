
#ifndef _WORMNET_DBI_SQL_QUERY_TASK_H
#define _WORMNET_DBI_SQL_QUERY_TASK_H

#include "wormnet/ISqlTask.h"
#include <vector>

namespace worm
{

class WORM_DLL_EXPORT CSqlQueryTask : public ISqlTask
{
public:
    CSqlQueryTask(const char* pSql);
    virtual ~CSqlQueryTask();
    virtual bool Execute(class wmDatabase* pDb);

protected:
    CSqlQueryTask();
};

class CSqlAsyncTask : public CSqlQueryTask
{
public:
    CSqlAsyncTask(const char* pSql, IQueryCallback* pCallback);
    ~CSqlAsyncTask();

    const char* GetSql() const
    {
        assert(NULL != m_pSql);
        return m_pSql;
    }
};

}

#endif // ifndef _WORMNET_DBI_SQL_QUERY_TASK_H
