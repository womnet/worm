
#ifndef _PANGU_FLASHPOLICY_APPLICATION_H
#define _PANGU_FLASHPOLICY_APPLICATION_H

#include <wormnet/IServerModule.h>
#include <wormnet/wm_server.h>
#include <wormnet/wm_app.h>
#include <wormnet/wm_timer.h>

#ifdef WIN32
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif /// ifdef WIN32
#define HashMap     std::tr1::unordered_map

class CPolicyApp : public worm::wmApp
{
public:
    ~CPolicyApp();
    static CPolicyApp* Instance();

    bool Open();
    void StopNow();

    worm::wmServer* GetServer() const { return m_pServer; }

    void AddSocket(class CPolicySocketHandler* pSocket);
    void DeleteSocket(class CPolicySocketHandler* pSocket);

private:
    CPolicyApp();

    void CtrlBreak();
    void Update(const uint32& dwDiff);

private:
    /// 连接的socket
    HashMap<uint64, class CPolicySocketHandler*>    m_mapSocket;
    /// 检测连接的定时器
    worm::SIntervalTimer                            m_sTimer;
    /// 上次update的服务器时间
    uint64                                          m_qwPreUpdateTime;
};

#define g_pPolicyApp        (CPolicyApp::Instance())

#endif //
