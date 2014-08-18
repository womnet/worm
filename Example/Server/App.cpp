
#include "Server/App.h"
#include "Server/PlayerSocketHander.h"
#include "Server/Config.h"
#include <wormnet/wm_Exception.h>

/// 一次UPDATE最少的间隔
#define UPDATE_SLEEP        25

CApp::CApp()
{
    m_qwPreUpdateTime -= m_qwPreUpdateTime;
}

CApp::~CApp()
{

}

CApp* CApp::Instance()
{
    static CApp oApp;

    return &oApp;
}

bool CApp::Open()
{
    if (!worm::wmApp::Open())
    {
        return false;
    }

    if (!g_pPanguConf->LoadConf())
    {
        return false;
    }

    CPlayerSocketHandler* pHandler = new CPlayerSocketHandler[g_pPanguConf->m_wMaxPlayer];
    m_pServer = worm::ICreateServerModule();
    if (!m_pServer->Init(pHandler, sizeof(CPlayerSocketHandler), g_pPanguConf->m_wMaxPlayer))
    {
        delete[] pHandler;
        return false;
    }

    if (!m_pServer->Open(g_pPanguConf->m_strIp.c_str(), g_pPanguConf->m_wPort))
    {
        delete[] pHandler;
        return false;
    }

    LOG(INFO) << "服务器启动: " << g_pPanguConf->m_strIp.c_str() << g_pPanguConf->m_wPort;

    return true;
}

void CApp::StopNow()
{
    m_bStopEvent = true;
}

void CApp::Close()
{
    worm::wmApp::Close();
}

void CApp::CtrlBreak()
{
    StopNow();
}

void CApp::Update(const uint32& dwDiff)
{
    const uint64& qwCurTime = GetServerTime();
    uint32 dwSleep = (qwCurTime - m_qwPreUpdateTime) & 0xffffffff;
    m_qwPreUpdateTime = qwCurTime;
    if (dwSleep < UPDATE_SLEEP)
    {
        dwSleep = UPDATE_SLEEP - dwSleep;
        Sleep(dwSleep);
    }
}
