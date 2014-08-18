
#include "GuardApp.h"
#include "GuardConfig.h"

CGuardApp::CGuardApp()
{

}

CGuardApp::~CGuardApp()
{

}

bool CGuardApp::Open()
{
    if (!g_GuardConf.LoadConf())
    {
        return false;
    }

    return true;
}

void CGuardApp::Run()
{
    for (;;)
    {
#ifdef WIN32
        Sleep(g_GuardConf.m_dwSecond*1000);
#else
        sleep(g_GuardConf.m_dwSecond);
#endif
        system(g_GuardConf.m_strGuard.c_str());
    }
}

void CGuardApp::Close()
{

}
