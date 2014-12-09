
#ifndef _WORMNET_DBI_SQL_TASK_SET_H
#define _WORMNET_DBI_SQL_TASK_SET_H

#include "wormnet/ISqlTask.h"
#include "wormdb/DbCallback.h"
#include <vector>

namespace worm
{

class WORM_DLL_EXPORT CSqlTaskSet
{
    friend class CSqlAsyncTaskSet;
public:
    CSqlTaskSet(const uint16& wSize, const bool& bTransaction);
    CSqlTaskSet(const CSqlTaskSet& oSet);
    virtual ~CSqlTaskSet();

    void SetQuery(const uint16& wIndex, const char* pSql);
    bool IsTransaction() const { return m_bTransaction; }

    CDbResult*& GetResult(const uint8& byIndex=0);

    std::vector<ISqlTask*>* TaskList() { return m_aTaskList; }

    bool IsSucess() const { return m_bSucess; }
    void SetSucess(const bool& b) { m_bSucess = b; }

	void ClearTask();

private:
    CSqlTaskSet();

private:
    /// 要执行的sql语句集
    std::vector<ISqlTask*>*         m_aTaskList;
    /// 此语句集是否是事务
    bool                            m_bTransaction;
    /// 是否全部执行成功
    bool                            m_bSucess;
};

class WORM_DLL_EXPORT CSqlAsyncTaskSet
{
public:
    CSqlAsyncTaskSet(CSqlTaskSet* const pTaskSet, IQueryCallback* const pCallback);
    CSqlAsyncTaskSet(const char* pSql, IQueryCallback* const pCallback);
    ~CSqlAsyncTaskSet();

    bool Execute(class wmDatabase* pDb);
    bool IsCallback() const { return (NULL!=m_pCallback); }
    void Callback();

private:
    CSqlAsyncTaskSet();

private:
    /// 回调接口
    IQueryCallback*     m_pCallback;
    /// 任务集
    CSqlTaskSet*        m_pTaskSet;
};

}

#endif // ifndef _WORMNET_DBI_SQL_TASK_SET_H
