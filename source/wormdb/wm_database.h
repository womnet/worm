
#ifndef _WORMNET_DBI_DATABASE_H
#define _WORMNET_DBI_DATABASE_H

#include <wormnet/wm_mutex.h>
#include <wormnet/wm_types.h>
#include <wormdb/SqlTaskSet.h>
#include <wormdb/DbResult.h>
#include <wormnet/wm_thread.h>
#include <queue>
#include <wormdb/SqlQueryTask.h>
#include <wormnet/wm_semaphore.h>

#define ASYNC_QUERY_ASSERT(p)\
{\
    if (NULL==p)\
    {\
        assert(0);\
        return;\
    }\
    if (!m_bAsync)\
    {\
        assert(0);\
        return;\
    }\
}

namespace worm
{

class WORM_DLL_EXPORT wmDatabase
{
public:
    wmDatabase();
    virtual ~wmDatabase();

    virtual bool Open(const char* pStrInfo) = 0;
    virtual void Close(void) = 0;
    virtual bool DirectExecute(const char* pStrSql) = 0;
    virtual bool DirectExecute(CSqlTaskSet* const pTaskSet) = 0;
    virtual bool Query(const char *pSql, CDbResult*& pResult) = 0;
    virtual bool Query(CSqlTaskSet* const pTaskSet) = 0;
    /// ping 数据库, 保持连接状态
    virtual void KeepAlive() = 0;

    void CallbackAsyncQuery();

    void Execute(const char* pSql)
    {
        ASYNC_QUERY_ASSERT(pSql);
        CSqlAsyncTaskSet* pTask = new CSqlAsyncTaskSet(pSql, NULL);
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTask);
        m_oQueueLock.Unlock();
    }

    /// 回调是成员函数的处理
    template<class TClass>
    void AsyncQuery(TClass *pObject, void (TClass::*Method)(CSqlTaskSet*), const char *pSql)
    {
        ASYNC_QUERY_ASSERT(pSql);

        CSqlAsyncTaskSet* pTask = new CSqlAsyncTaskSet(pSql, new QueryCallback<TClass>(pObject, Method, NULL));
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTask);
        m_oQueueLock.Unlock();
    }
    template<class TClass, typename TParamType1>
    void AsyncQuery(TClass *pObject, void (TClass::*Method)(CSqlTaskSet*, TParamType1), TParamType1 param1, const char *pSql)
    {
        ASYNC_QUERY_ASSERT(pSql);

        CSqlAsyncTaskSet* pTask = new CSqlAsyncTaskSet(pSql, new QueryCallback<TClass, TParamType1>(pObject, Method, NULL, param1));
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTask);
        m_oQueueLock.Unlock();
    }
    template<class TClass, typename TParamType1, typename TParamType2>
    void AsyncQuery(TClass *pObject, void (TClass::*Method)(CSqlTaskSet*, TParamType1, TParamType2), TParamType1 param1, TParamType2 param2, const char *pSql)
    {
        ASYNC_QUERY_ASSERT(pSql);

        CSqlAsyncTaskSet* pTask = new CSqlAsyncTaskSet(pSql, new QueryCallback<TClass, TParamType1, TParamType2>(pObject, Method, NULL, param1, param2));
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTask);
        m_oQueueLock.Unlock();
    }
    template<class TClass, typename TParamType1, typename TParamType2, typename TParamType3>
    void AsyncQuery(TClass *pObject, void (TClass::*Method)(CSqlTaskSet*, TParamType1, TParamType2, TParamType3), TParamType1 param1, TParamType2 param2, TParamType3 param3, const char *pSql)
    {
        ASYNC_QUERY_ASSERT(pSql);

        CSqlAsyncTaskSet* pTask = new CSqlAsyncTaskSet(pSql, new QueryCallback<TClass, TParamType1, TParamType2, TParamType3>(pObject, Method, NULL, param1, param2, param3));
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTask);
        m_oQueueLock.Unlock();
    }
    // Query / static
    void AsyncQuery(void (*Method)(CSqlTaskSet*), const char *pSql)
    {
        ASYNC_QUERY_ASSERT(pSql);

        CSqlAsyncTaskSet* pTask = new CSqlAsyncTaskSet(pSql, new SQueryCallback<>(Method, NULL));
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTask);
        m_oQueueLock.Unlock();
    }
    template<typename TParamType1>
    void AsyncQuery(void (*Method)(CSqlTaskSet*, TParamType1), TParamType1 param1, const char *pSql)
    {
        ASYNC_QUERY_ASSERT(pSql);

        CSqlAsyncTaskSet* pTask = new CSqlAsyncTaskSet(pSql, new SQueryCallback<TParamType1>(Method, (CDbResult*)((CDbResult*)NULL), param1));
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTask);
        m_oQueueLock.Unlock();
    }
    template<typename TParamType1, typename TParamType2>
    void AsyncQuery(void (*Method)(CSqlTaskSet*, TParamType1, TParamType2), TParamType1 param1, TParamType2 param2, const char *pSql)
    {
        ASYNC_QUERY_ASSERT(pSql);

        CSqlAsyncTaskSet* pTask = new CSqlAsyncTaskSet(pSql, new SQueryCallback<TParamType1, TParamType2>(Method, (CDbResult*)((CDbResult*)NULL), param1, param2));
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTask);
        m_oQueueLock.Unlock();
    }
    template<typename TParamType1, typename TParamType2, typename TParamType3>
    void AsyncQuery(void (*Method)(CSqlTaskSet*, TParamType1, TParamType2, TParamType3), TParamType1 param1, TParamType2 param2, TParamType3 param3, const char *pSql)
    {
        ASYNC_QUERY_ASSERT(pSql);

        CSqlAsyncTaskSet* pTask = new CSqlAsyncTaskSet(pSql, new SQueryCallback<TParamType1, TParamType2>(Method, (CDbResult*)((CDbResult*)NULL), param1, param2));
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTask);
        m_oQueueLock.Unlock();
    }
    /// 处理pSql语句集合
    template<class TClass>
    void AsyncQuerySet(TClass *pObject, void (TClass::*Method)(CSqlTaskSet*), CSqlTaskSet* pQuerySet)
    {
        ASYNC_QUERY_ASSERT(pQuerySet);

        CSqlAsyncTaskSet* pTaskSet = new CSqlAsyncTaskSet(pQuerySet, new QueryCallback<TClass>(pObject, Method, pQuerySet));
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTaskSet);
        m_oQueueLock.Unlock();
    }
    template<class TClass, typename TParamType1>
    void AsyncQuerySet(TClass *pObject, void (TClass::*Method)(CSqlTaskSet*, TParamType1), TParamType1 param1, CSqlTaskSet* pQuerySet)
    {
        ASYNC_QUERY_ASSERT(pQuerySet);

        CSqlAsyncTaskSet* pTask = new CSqlAsyncTaskSet(pQuerySet, new QueryCallback<TClass, TParamType1>(pObject, Method, pQuerySet, param1));
        m_oQueueLock.Lock();
        m_queueAsyncRequest->push(pTask);
        m_oQueueLock.Unlock();
    }

protected:
    bool Initialize(const char* pStrInfo);
    void DelayExecute();
    /// 异步处理线程
    static WORM_THREAD_DECLARATION(AsyncRun);

protected:
    /// 数据库ip或主机名称
    char                            m_szIP[26];
    /// 端口
    uint16                          m_wPort;
    /// 用户名
    char                            m_szUser[32];
    /// 密码
    char                            m_szPassword[32];
    /// 连接的数据库
    char                            m_szDBName[64];
    /// 是否支持异步
    bool                            m_bAsync;
    /// 是否退出
    bool                            m_bStop;
    /// 异步sql执行请求
    std::queue<CSqlAsyncTaskSet*>*  m_queueAsyncRequest;
    /// 异步查询的结果
    std::queue<CSqlAsyncTaskSet*>*  m_queueAsyncResult;
    /// 队列锁
    wmMutex                         m_oQueueLock;
    /// sql 执行同步锁
    wmMutex                         m_oSqlLock;
    /// 退出信号
    wmSem                           m_oStopSem;
};

}

#endif // ifndef _WORMNET_DBI_DATABASE_H
