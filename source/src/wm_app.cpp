
#include <event2/event.h>
#include <event2/event_struct.h>
#include "wormnet/wm_server.h"

#include <signal.h>
#include "wormnet/wm_app.h"
#include "wormnet/wm_time.h"

namespace worm
{

wmApp::wmApp()
{
    m_bStopEvent = false;
    m_pCtrlCEventBase = NULL;
    m_pServer = NULL;

    m_qwServerTime -= m_qwServerTime;
}

wmApp::~wmApp()
{

}

bool wmApp::Open()
{
#ifdef WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif

    wm_CreateThread(&wmApp::CtrlCEvent, this);

    return true;
}

void wmApp::Run()
{
    assert(NULL != m_pServer);

    uint32 dwRealCurTime = 0;
    uint32 dwRealPrevTime = getMSTime();
    uint32 dwDiff = 0;

    for (; !m_bStopEvent; )
    {
        m_pServer->EventLoop();

        dwRealCurTime = getMSTime();
        dwDiff = dwRealCurTime - dwRealPrevTime;

        m_qwServerTime += dwDiff;
        this->Update(dwDiff);

        dwRealPrevTime = dwRealCurTime;

#ifdef _DEBUG
        Sleep(1);
#else
        Sleep(0);
#endif //
    }
}

void wmApp::Close()
{
    if (NULL != m_pCtrlCEventBase)
    {
        event_base_free((struct event_base*)m_pCtrlCEventBase);
        m_pCtrlCEventBase = NULL;
    }
}

WORM_THREAD_DECLARATION(wmApp::CtrlCEvent)
{
    wmApp* pThis = (wmApp*)arguments;

    pThis->m_pCtrlCEventBase = event_base_new();
    struct event eventSignal;
    event_assign(&eventSignal, (struct event_base*)pThis->m_pCtrlCEventBase, \
        SIGINT, EV_SIGNAL|EV_PERSIST, (event_callback_fn)SignalCallback, pThis);
    event_add(&eventSignal, NULL);

    event_base_dispatch((struct event_base*)pThis->m_pCtrlCEventBase);

    return 0;
}

void worm::wmApp::SignalCallback(wm_socket_t qwSig, short nEvents, void* pParam)
{
    ((wmApp*)pParam)->CtrlBreak();
}

}
