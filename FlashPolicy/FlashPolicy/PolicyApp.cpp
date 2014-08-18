
#include "FlashPolicy/PolicyApp.h"
#include <wormnet/wm_string.h>
#include "FlashPolicy/PolicySocketHandler.h"
#include <wormnet/ILogs.h>

/// ɳ���Ӧ
#define FLASH_POLICY_RESPON "<?xml version=\"1.0\" encoding=\"utf-8\" ?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>\r\n\r\n\0"
/// ���ӱ��ֵ��ʱ��, ����
#define MAX_CONNECTED_TIME  120000
/// ��ⶨʱ, ����
#define CHECK_INTERVAL_TIME 15000

/// ���� sleep
#define UPDATE_SLEEP        25

extern worm::CByteBuffer g_oResponPacket;

CPolicyApp::CPolicyApp() : worm::wmApp()
{
    m_pServer = NULL;
    m_qwPreUpdateTime -= m_qwPreUpdateTime;
}

CPolicyApp::~CPolicyApp()
{

}

CPolicyApp* CPolicyApp::Instance()
{
    static CPolicyApp oApp;

    return &oApp;
}

/// ͬʱ������֤���������
#define MAX_POLICY          (100)
/// flash����Ķ˿�
#define PORT_FLASH_POLICY   (843)

bool CPolicyApp::Open()
{
    if (!worm::wmApp::Open())
    {
        return false;
    }

    CPolicySocketHandler* pHandler = new CPolicySocketHandler[MAX_POLICY];
    m_pServer = worm::ICreateServerModule();
    if (!m_pServer->Init(pHandler, sizeof(CPolicySocketHandler), MAX_POLICY))
    {
        delete[](pHandler);
        return false;
    }

    g_oResponPacket.Append(FLASH_POLICY_RESPON, strlen(FLASH_POLICY_RESPON)+1);

    if (!m_pServer->Open("0.0.0.0", PORT_FLASH_POLICY))
    {
        delete[](pHandler);
        return false;
    }
    LOG(INFO) << "Flash ɳ������!";

    m_sTimer.SetInterval(CHECK_INTERVAL_TIME);

    return true;
}

void CPolicyApp::CtrlBreak()
{
    StopNow();
}

void CPolicyApp::Update(const uint32& dwDiff)
{
    m_sTimer.Update(dwDiff);
    if (m_sTimer.IsPassed())
    {
        HashMap<uint64, class CPolicySocketHandler*>::iterator iterSocket = m_mapSocket.begin();
        for (; m_mapSocket.end()!=iterSocket; ++iterSocket)
        {
            if (GetServerTime()-iterSocket->second->GetStartTime() > MAX_CONNECTED_TIME)
            {
                g_pPolicyApp->GetServer()->CloseConnection(iterSocket->second->Socket());
            }
        }
        m_sTimer.Reset();
    }

    const uint64& qwCurTime = GetServerTime();
    uint32 dwSleep = (qwCurTime - m_qwPreUpdateTime) & 0xffffffff;
    m_qwPreUpdateTime = qwCurTime;
    if (dwSleep < UPDATE_SLEEP)
    {
        dwSleep = UPDATE_SLEEP - dwSleep;
        Sleep(dwSleep);
    }
}

void CPolicyApp::StopNow()
{
    m_bStopEvent = true;

    Sleep(2000);
}

void CPolicyApp::AddSocket(class CPolicySocketHandler* pSocket)
{
    m_mapSocket[pSocket->GetGUID()] = pSocket;
}

void CPolicyApp::DeleteSocket(class CPolicySocketHandler* pSocket)
{
    m_mapSocket.erase(pSocket->GetGUID());
}
