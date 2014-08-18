
#ifndef _WORMNET_DBI_INTERFACE_TASK
#define _WORMNET_DBI_INTERFACE_TASK

#include "wormnet/wm_string.h"
#include "wormdb/DbCallback.h"
#include "wormnet/wm_std.h"
#include <assert.h>
#include "wormdb/DbResult.h"

namespace worm
{

class WORM_DLL_EXPORT ISqlTask
{
public:
    virtual ~ISqlTask()
    {
        if (NULL != m_pSql)
        {
            delete []m_pSql;
            delete m_pResult;
        }
    }
    virtual bool Execute(class wmDatabase* pDb) = 0;
    const char* sql() const { return m_pSql; }
    /*bool IsCallback() const { return (NULL!=m_pCallback); }
    void Callback()
    {
        m_pCallback->SetResult(m_pResult);
        m_pCallback->Execute();
    }*/
    // bool IsSucess() const { return m_bSucess; }
    // void SetSucess(const bool& b) { m_bSucess = b; }

    CDbResult*& GetResult() { return m_pResult; }

protected:
    ISqlTask();
    ISqlTask(const char* pSql) : m_pCallback(NULL)
    {
        assert(NULL != pSql);

        const size_t dwSize = strlen(pSql) + 1;
        m_pSql = new char[dwSize];
        memcpy(m_pSql, pSql, dwSize);

        //m_bSucess = false;
        m_pCallback = NULL;
        m_pResult = NULL;
    }

protected:
    /// 要执行的sql语句
    char*                               m_pSql;
    /// 回调接口
    class IQueryCallback*               m_pCallback;
    /// 查询结果
    CDbResult*                          m_pResult;
    /// 是否执行成功
    //bool                                m_bSucess;
};

}

#endif // ifndef _WORMNET_DBI_INTERFACE_TASK
