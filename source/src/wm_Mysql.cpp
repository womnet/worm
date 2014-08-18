
#ifdef WIN32
#include <Windows.h>
#undef ERROR
#endif
#include <mysql/mysql.h>
#include <stdio.h>
#include "wm_Mysql.h"
#include "wm_MysqlResult.h"
#include "wormnet/ILogs.h"

#define MYSQL_CHARACTER_UTF8    "utf8"

namespace worm
{

wmMysql::wmMysql()
{
    m_pMysql = NULL;
}

wmMysql::~wmMysql()
{
    Close();
}

bool wmMysql::Open(const char* pStrInfo)
{
    if (!Initialize(pStrInfo))
    {
        return false;
    }

    MYSQL *pMysqlInit = NULL;
    pMysqlInit = mysql_init(NULL);
    if (NULL == pMysqlInit)
    {
        LOG(ERROR) << "初始会MySQL连接错误\n";
        return false;
    }
    mysql_options(pMysqlInit, MYSQL_SET_CHARSET_NAME, MYSQL_CHARACTER_UTF8);

    char const* unix_socket = 0;
    m_pMysql = mysql_real_connect(pMysqlInit, m_szIP, m_szUser,
        m_szPassword, m_szDBName, m_wPort, unix_socket, 0);

    if (NULL == m_pMysql)
    {
        LOG(ERROR) << "连接到MySQL失败. 错误信息: " << m_szDBName << ":" << mysql_error(pMysqlInit);
        mysql_close(pMysqlInit);

        return false;
    }

    LOG(INFO) << "成功连接到MySQL数据库: " << m_szIP << "@" << m_szDBName;
    /*----------设置自动提交开启---------*/
    // It seems mysql 5.0.x have enabled this feature
    // by default. In crash case you can lose data!!!
    // So better to turn this off
    // ---
    // This is wrong since Trinity use transactions,
    // autocommit is turned of during it.
    // Setting it to on makes atomic updates work
    if (mysql_autocommit((MYSQL*)m_pMysql, 1))
    {
        LOG(ERROR) << "mysql_autocommit (自动提交) 设置失败";
    }
    /*-------------------------------------*/

    // set connection properties to UTF8 to properly handle locales for different
    // server configs - core sends data in UTF8, so MySQL must expect UTF8 too
    //PExecute("SET NAMES `utf8`");
    //PExecute("SET CHARACTER SET `utf8`");
    char szSql[256] = "\0";
    sprintf(szSql, "SET NAMES `%s`", MYSQL_CHARACTER_UTF8);
    DirectExecute(szSql);
    sprintf(szSql, "SET CHARACTER SET `%s`", MYSQL_CHARACTER_UTF8);
    DirectExecute(szSql);

    //设置mysql断开时自动连接
#if MYSQL_VERSION_ID >= 50003
    my_bool my_true = 1;
    if (mysql_options((MYSQL*)m_pMysql, MYSQL_OPT_RECONNECT, &my_true))
    {
        printf("Set MYSQL_OPT_RECONNECT faild.\n");
    }
#else
#warning "Your mySQL client lib version does not support reconnecting after a timeout.\nIf this causes you any trouble we advice you to upgrade your mySQL client libs to at least mySQL 5.0.13 to resolve this problem."
#endif

    mysql_thread_init();

    if (m_bAsync)
    {
        wm_CreateThread(&wmDatabase::AsyncRun, this);
    }

    return true;
}

void wmMysql::Close(void)
{
    if (NULL != m_pMysql)
    {
        mysql_thread_end();

        mysql_close((MYSQL*)m_pMysql);
        m_pMysql = NULL;
    }

    m_bStop = true;
    m_oStopSem.WaitSemaphore();
}

bool wmMysql::BeginTransaction()
{
    if (NULL == m_pMysql)
    {
        return false;
    }

    if (0 != mysql_query((MYSQL*)m_pMysql, "START TRANSACTION"))
    {
        //PrintString("mysql_query: (%s) faild\n", sql);
        return false;
    }

    return true;
}

bool wmMysql::CommitTransaction()
{
    if (NULL == m_pMysql)
    {
        return false;
    }

    if (0 != mysql_query((MYSQL*)m_pMysql, "COMMIT"))
    {
        //PrintString("mysql_query: (%s) faild\n", sql);
        return false;
    }

    return true;
}

bool wmMysql::RollbackTransaction()
{
    if (NULL == m_pMysql)
    {
        return false;
    }

    if (0 != mysql_query((MYSQL*)m_pMysql, "ROLLBACK"))
    {
        //PrintString("mysql_query: (%s) faild\n", sql);
        return false;
    }

    return true;
}

bool wmMysql::DirectExecute(const char* pStrSql)
{
    assert(NULL != pStrSql);

    if (NULL == m_pMysql)
    {
        return false;
    }

    m_oSqlLock.Lock();

    if(0 != mysql_query((MYSQL*)m_pMysql, pStrSql))
    {
        //PrintString("mysql_query: (%s) faild\n", sql);
        m_oSqlLock.Unlock();
        return false;
    }

    m_oSqlLock.Unlock();


    return true;
}

bool wmMysql::DirectExecute(CSqlTaskSet* const pTaskSet)
{
    if (NULL == m_pMysql)
    {
        return false;
    }

    std::vector<ISqlTask*>& aTaskList = *(pTaskSet->TaskList());

    m_oSqlLock.Lock();
    if (pTaskSet->IsTransaction())
    {
        BeginTransaction();
        for (uint32 i=0; i<aTaskList.size(); ++i)
        {
            if (0 != mysql_query((MYSQL*)m_pMysql, aTaskList[i]->sql()))
            {
                RollbackTransaction();
                m_oSqlLock.Unlock();
                return false;
            }
        }
        CommitTransaction();
    }
    else
    {
        for (uint32 i=0; i<aTaskList.size(); ++i)
        {
            if (0 != mysql_query((MYSQL*)m_pMysql, aTaskList[i]->sql()))
            {
                m_oSqlLock.Unlock();
                return false;
            }
        }
    }
    m_oSqlLock.Unlock();

    return true;
}

bool wmMysql::Query(const char *pSql, CDbResult*& pDbResult)
{
    assert(NULL != pSql);

    pDbResult = NULL;
    if (NULL == m_pMysql)
    {
        return false;
    }

    MYSQL_RES* pResult = NULL;
    uint64 qwRowCount = 0;
    uint32 dwFieldCount = 0;
    pResult = NULL;

    m_oSqlLock.Lock();

    if(0 != mysql_query((MYSQL*)m_pMysql, pSql))
    {
        m_oSqlLock.Unlock();
        LOG(WARNING) << "执行SQL语句失败: " << pSql;
        return false;
    }
    m_oSqlLock.Unlock();

    pResult = mysql_store_result((MYSQL*)m_pMysql);
    
    if (NULL != pResult)
    {
        qwRowCount = mysql_affected_rows((MYSQL*)m_pMysql);
        if (0 != qwRowCount)
        {
            dwFieldCount = mysql_field_count((MYSQL*)m_pMysql);
            pDbResult = new wmMysqlResult(pResult, qwRowCount&0xffffffff, dwFieldCount);
        }
        mysql_free_result(pResult);
        pResult = NULL;
    }

    return true;
}

bool wmMysql::Query(CSqlTaskSet* const pTaskSet)
{
    std::vector<ISqlTask*>& aTaskList =*(pTaskSet->TaskList());

    if (pTaskSet->IsTransaction())
    {
        BeginTransaction();
        for (uint32 i=0; i<aTaskList.size(); ++i)
        {
            if (NULL == aTaskList[i])
            {
                continue;
            }
            if (!Query(aTaskList[i]->sql(), aTaskList[i]->GetResult()))
            {
                RollbackTransaction();
                pTaskSet->SetSucess(false);
                return false;
            }
        }
        CommitTransaction();
    }
    else
    {
        for (uint32 i=0; i<aTaskList.size(); ++i)
        {
            if (NULL == aTaskList[i])
            {
                continue;
            }
            if (!Query(aTaskList[i]->sql(), aTaskList[i]->GetResult()))
            {
                pTaskSet->SetSucess(false);
                return false;
            }
        }
    }
    pTaskSet->SetSucess(true);

    return true;
}

void wmMysql::KeepAlive()
{
    if (NULL != m_pMysql)
    {
        mysql_ping((MYSQL*)m_pMysql);
    }
}

}
