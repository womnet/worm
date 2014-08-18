
#ifndef _LEGEND_APPLICATION_APP_H
#define _LEGEND_APPLICATION_APP_H

#include <wormnet/MemoryGuard.h>
#include <wormdb/wm_database.h>
#include <wormdb/IDatabaseModule.h>

#include <wormnet/IServerModule.h>
#include <wormnet/wm_server.h>
#include <wormnet/wm_app.h>
#include <wormnet/ILogs.h>
#include <wormnet/wm_Exception.h>

class CApp : public worm::wmApp
{
public:
    ~CApp();
    static CApp* Instance();

    bool Open();
    void StopNow();
    void Close();

    worm::wmServer* GetServer() const { return m_pServer; }

private:
    CApp();
    void CtrlBreak();
    void Update(const uint32& dwDiff);

private:
    /// 上次update的服务器时间
    uint64              m_qwPreUpdateTime;
};

#define g_pApp      (CApp::Instance())

#endif // ifndef _LEGEND_APPLICATION_APP_H
