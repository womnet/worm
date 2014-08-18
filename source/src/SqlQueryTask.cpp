﻿
#include "wormdb/SqlQueryTask.h"
#include "wormdb/wm_database.h"

namespace worm
{

CSqlQueryTask::CSqlQueryTask(const char* pSql)
: ISqlTask(pSql)
{
}

CSqlQueryTask::~CSqlQueryTask()
{

}

bool CSqlQueryTask::Execute(class wmDatabase* pDb)
{
    assert(NULL != pDb);
    return pDb->Query(m_pSql, m_pResult);
}

CSqlAsyncTask::CSqlAsyncTask(const char* pSql, IQueryCallback* pCallback)
: CSqlQueryTask(pSql)
{
    m_pCallback = pCallback;
}

CSqlAsyncTask::~CSqlAsyncTask()
{
    
}

CSqlAsyncTaskSet::CSqlAsyncTaskSet(CSqlTaskSet* pTaskSet, IQueryCallback* const pCallback)
{
    m_pTaskSet = new CSqlTaskSet(*pTaskSet);
    m_pCallback = pCallback;
}

CSqlAsyncTaskSet::CSqlAsyncTaskSet(const char* pSql, IQueryCallback* const pCallback)
{
    m_pTaskSet = new CSqlTaskSet(1, false);
    m_pTaskSet->SetQuery(0, pSql);
    m_pCallback = pCallback;
}

CSqlAsyncTaskSet::~CSqlAsyncTaskSet()
{
    delete m_pTaskSet;
    m_pTaskSet = NULL;
}

bool CSqlAsyncTaskSet::Execute(class wmDatabase* pDb)
{
    assert(NULL != pDb);

    return pDb->Query(m_pTaskSet);
}

void CSqlAsyncTaskSet::Callback()
{
    m_pCallback->SetResult(m_pTaskSet);
    m_pCallback->Execute();
}

}
