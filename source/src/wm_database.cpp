
#include <string.h>
#include <stdlib.h>
#include "wormdb/wm_database.h"
#include "wormnet/wm_string.h"

namespace worm
{

wmDatabase::wmDatabase()
{
    memset(m_szIP, 0, sizeof(m_szIP));
    m_wPort -= m_wPort;
    memset(m_szUser, 0, sizeof(m_szUser));
    memset(m_szPassword, 0, sizeof(m_szPassword));
    memset(m_szDBName, 0, sizeof(m_szDBName));
    m_bAsync = false;
    m_bStop = false;

    m_queueAsyncRequest = new std::queue<CSqlAsyncTaskSet*>;
    m_queueAsyncResult = new std::queue<CSqlAsyncTaskSet*>;

    m_oStopSem.CreateSemaphore(0, 1);
}

wmDatabase::~wmDatabase()
{
    delete m_queueAsyncRequest;
    m_queueAsyncRequest = NULL;
    delete m_queueAsyncResult;
    m_queueAsyncResult = NULL;
}

bool wmDatabase::Initialize(const char* pStrInfo)
{
    //从字符串中提取参数, 格式: ip;端口;用户名;密码;编码;数据库名
    char szDbConfig[256] = "\0";
    strlcpy(szDbConfig, pStrInfo, sizeof(szDbConfig));

    //取ip
    char* pTmp = strtok(szDbConfig, ";");
    if (NULL == pTmp) return false;
    strlcpy(m_szIP, pTmp, sizeof(m_szIP));
    //取端口
    pTmp = strtok(NULL, ";");
    if (NULL == pTmp) return false;
    m_wPort = atoi(pTmp) & 0xffff;
    //用户名
    pTmp = strtok(NULL, ";");
    if (NULL == pTmp) return false;
    strlcpy(m_szUser, pTmp, sizeof(m_szUser));
    //密码
    pTmp = strtok(NULL, ";");
    if (NULL == pTmp) return false;
    strlcpy(m_szPassword, pTmp, sizeof(m_szPassword));
    //编码
    /*pTmp = strtok(NULL, ";");
    if (NULL == pTmp) return false;
    strlcpy(m_szCharacter, pTmp, sizeof(m_szCharacter));*/
    //数据库名
    pTmp = strtok(NULL, ";");
    if (NULL == pTmp) return false;
    strlcpy(m_szDBName, pTmp, sizeof(m_szDBName));
    /// 是否支持异步操作
    pTmp = strtok(NULL, ";");
    if (NULL != pTmp)
    {
        m_bAsync = (0==strcmp("async", pTmp));
    }
    else
    {
        m_bAsync = false;
    }

    if (!m_bAsync)
    {
        m_oStopSem.PostSemaphore();
    }

    return true;
}

void wmDatabase::CallbackAsyncQuery()
{
    CSqlAsyncTaskSet* pTaskSet = NULL;

    m_oQueueLock.Lock();

    for (; !m_queueAsyncResult->empty();)
    {
        pTaskSet = m_queueAsyncResult->front();
        m_queueAsyncResult->pop();
        pTaskSet->Callback();
        delete pTaskSet;
    }

    m_oQueueLock.Unlock();
}

/// 每次循环最大处理的异步sql语句, 防止锁队列后无法添加新的异步sql
#define MAX_ASYNC_SQL       5

WORM_THREAD_DECLARATION(wmDatabase::AsyncRun)
{
    assert(NULL != arguments);
    wmDatabase* pThis = (wmDatabase*)arguments;
    CSqlAsyncTaskSet* pTask = NULL;
    uint16 i = 0;

    for (;;)
    {
#ifdef _DEBUG
        Sleep(2);
#else
        Sleep(1);
#endif // 

        pThis->m_oQueueLock.Lock();

        for (i=0; !pThis->m_queueAsyncRequest->empty() && i!=MAX_ASYNC_SQL; ++i)
        {
            pTask = pThis->m_queueAsyncRequest->front();
            pThis->m_queueAsyncRequest->pop();
            pTask->Execute(pThis);
            if (pTask->IsCallback())
            {
                pThis->m_queueAsyncResult->push(pTask);
            }
        }

        if (pThis->m_bStop && pThis->m_queueAsyncRequest->empty())
        {
            pThis->m_oQueueLock.Unlock();
            break;
        }

        pThis->m_oQueueLock.Unlock();
    }

    pThis->m_oStopSem.PostSemaphore();

    return 0;
}

}
