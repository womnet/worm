
#ifndef WIN32
#include <arpa/inet.h>
#endif // ifndef WIN32
#include <event2/event.h>
#include <event2/bufferevent.h>
#include "wormnet/wm_LibeventChannel.h"

namespace worm
{

worm::wmLibeventChannel::wmLibeventChannel(wmConnectedCB pConnectedCB, wmReceiveCB pReceiveCB, wmDisconnectCB pDisconnectCB)
: wmChannel(pConnectedCB, pReceiveCB, pDisconnectCB)
{
#ifdef WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif

    m_pBase = NULL;
    m_pBev = NULL;
}

wmLibeventChannel::~wmLibeventChannel()
{

}

bool wmLibeventChannel::ConnectServer(const char* szIp, const uint16& nPort)
{
    struct sockaddr_in sin;

    m_pBase = event_base_new();
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(szIp) & 0xffffffff;
    sin.sin_port = wm_hton16(nPort);

    m_pBev = bufferevent_socket_new(m_pBase, -1, BEV_OPT_CLOSE_ON_FREE);

    if (bufferevent_socket_connect(m_pBev, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        bufferevent_free(m_pBev);
        m_pBev = NULL;
        return false;
    }

    bufferevent_setcb(m_pBev, ReadCallback, NULL, EventCallback, this);
    bufferevent_enable(m_pBev, EV_READ);

    return true;
}

void wmLibeventChannel::EventLoop()
{
    event_base_loop(m_pBase, EVLOOP_NONBLOCK);
}

void wmLibeventChannel::EventCallback(struct bufferevent* pBev, short nEvents, void* pParam)
{
    wmLibeventChannel* pThis = (wmLibeventChannel*)pParam;
    if (nEvents & BEV_EVENT_CONNECTED)
    {
        pThis->m_pConnectedCB(true);
    }

    if (nEvents & (BEV_EVENT_EOF | BEV_EVENT_ERROR))
    {
        pThis->m_pDisconnectCB();
        bufferevent_free(pThis->m_pBev);
        pThis->m_pBev = NULL;
    }
}

void wmLibeventChannel::ReadCallback(struct bufferevent* pBev, void* pParam)
{
    wmLibeventChannel* pThis = (wmLibeventChannel*)pParam;
    static char szBuf[4096];
    static uint32 dwRecv = 0;
    dwRecv = bufferevent_read(pBev, szBuf, sizeof(szBuf));
    pThis->m_pReceiveCB(szBuf, dwRecv);
}

void wmLibeventChannel::Send(CByteBuffer& oPacket)
{
    if (NULL == m_pBev)
    {
        return;
    }
    bufferevent_write(m_pBev, oPacket.data(), oPacket.size());
}

void wmLibeventChannel::CloseConnection()
{
     bufferevent_free(m_pBev);
}

}
