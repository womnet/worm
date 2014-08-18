
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
    /// ���ӵ�socket
    HashMap<uint64, class CPolicySocketHandler*>    m_mapSocket;
    /// ������ӵĶ�ʱ��
    worm::SIntervalTimer                            m_sTimer;
    /// �ϴ�update�ķ�����ʱ��
    uint64                                          m_qwPreUpdateTime;
};

#define g_pPolicyApp        (CPolicyApp::Instance())

#endif //
