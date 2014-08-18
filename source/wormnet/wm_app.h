
#ifndef _WORMNET_FRAMEWORK_APPLICATION_H
#define _WORMNET_FRAMEWORK_APPLICATION_H

#include "wormnet/wm_std.h"
#include <wormnet/wm_thread.h>

/// 防止定义冲突, 重定义 evutil_socket_t
#ifdef WIN32
#define wm_socket_t intptr_t
#else
#define wm_socket_t int
#endif

#define APP_UPDATE_TIME 25

namespace worm
{

class WORM_DLL_EXPORT wmApp
{
public:
    wmApp();
    virtual ~wmApp();

    virtual bool Open();
    virtual void Run();
    virtual void Close();

    virtual void StopNow() = 0;
    /// 获取服务器运行时间, 毫秒
    const uint64& GetServerTime() { return m_qwServerTime; }

private:
    static WORM_THREAD_DECLARATION(CtrlCEvent);
    static void SignalCallback(wm_socket_t qwSig, short nEvents, void* pParam);

protected:
    virtual void CtrlBreak() = 0;
    virtual void Update(const uint32& dwDiff) = 0;

private:
    void*               m_pCtrlCEventBase;

protected:
    /// 服务器停止判断, 为true时为停止
    bool                m_bStopEvent;
    /// 服务器
    class wmServer*     m_pServer;
    /// 服务器运行时间, 毫秒
    uint64              m_qwServerTime;
};

}

#endif // ifndef _WORMNET_FRAMEWORK_APPLICATION_H
