
#ifndef WIN32
#include <arpa/inet.h>
#endif // ifndef WIN32
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/buffer.h>

#include "wm_LibeventServer.h"
#include "wormnet/ISocketHander.h"
#include "wm_socket.h"
#include "wormnet/ILogs.h"

namespace worm
{

wmLibeventServer::wmLibeventServer()
{
    m_pBase = NULL;
}

wmLibeventServer::~wmLibeventServer()
{

}

bool wmLibeventServer::Init(class ISocketHandler* pHandlerArray, const uint32& dwHandlerSize, const uint16& wMax)
{
    if (0 == wMax)
    {
        return false;
    }

    if (NULL == pHandlerArray)
    {
        return false;
    }

    uint8* pTmp = (uint8*)pHandlerArray;

    m_pSocketBuffer = new wmSocket[wMax];
    m_pIdleSocket = new int[wMax];
    m_nIdleSocketNum = wMax;
    m_nSocketMax = wMax;
    m_mapSocketHandler.Init(wMax);

    for (uint16 i=0; i!=wMax; i++)
    {
        m_pSocketBuffer[i].SetIndex(i);
        m_pSocketBuffer[i].m_pSocketHandler = (ISocketHandler*)&pTmp[i*dwHandlerSize];
        m_pSocketBuffer[i].m_pSocketHandler->SetSocket(&m_pSocketBuffer[i]);
        m_pIdleSocket[i] = i;
    }

    return true;
}

bool wmLibeventServer::Open(const char* szIp, const uint16& wPort)
{
    struct evconnlistener* pListener;
    struct sockaddr_in sSin;

    m_pBase = event_base_new();
    if (NULL == m_pBase)
    {
        return false;
    }

    memset(&sSin, 0, sizeof(sSin));
    sSin.sin_family = AF_INET;
    sSin.sin_port = wm_hton16(wPort);
    sSin.sin_addr.s_addr = inet_addr(szIp) & 0xffffffff;

    pListener = evconnlistener_new_bind(m_pBase, AcceptCallback, this,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE|LEV_OPT_THREADSAFE, -1,
        (struct sockaddr*)&sSin,
        sizeof(sSin));

    if (NULL == pListener)
    {
        return false;
    }

    return true;
}

void wmLibeventServer::EventLoop()
{
    uint64 qwGuid = 0;
    SHashValue* pSocketHandler = NULL;
    for (; !m_queueClose.empty(); )
    {
        qwGuid = m_queueClose.front();
        m_queueClose.pop();
        pSocketHandler = m_mapSocketHandler.find(qwGuid);
        if (NULL != pSocketHandler)
        {
            m_pIdleSocket[m_nIdleSocketNum] = pSocketHandler->m_pValue->Socket()->GetIndex();
            ++m_nIdleSocketNum;
            bufferevent_free((bufferevent*)pSocketHandler->m_pValue->Socket()->GetSocket());
            m_mapSocketHandler.erase(qwGuid);
        }
        /*m_pIdleSocket[m_nIdleSocketNum] = pTarget->GetIndex();
    ++m_nIdleSocketNum;
    bufferevent_free((bufferevent*)pTarget->GetSocket());
    m_mapSocketHandler.erase(pTarget->GetGUID());*/
    }

    event_base_loop(m_pBase, EVLOOP_NONBLOCK);
}

void wmLibeventServer::Shutdown(void)
{
    if (!m_bStopEvent)
    {
        m_bStopEvent = true;
        Sleep(300);
    }
}

void wmLibeventServer::Send(CByteBuffer& oPacket, class wmSocket* const pTarget)
{
    if (NULL == pTarget)
    {
        return;
    }
    if (pTarget->IsClose())
    {
        return;
    }

    bufferevent_write((struct bufferevent *)pTarget->GetSocket(), oPacket.data(), oPacket.size());
}

void wmLibeventServer::AcceptCallback(struct evconnlistener* pListener, evutil_socket_t fd, struct sockaddr *address, int socklen, void* pParam)
{
     wmLibeventServer* pThis = (wmLibeventServer*)pParam;

     wmSocket* pSocket = pThis->GetSocket();
     if (NULL == pSocket)
     {
         return;
     }

     struct event_base *base = evconnlistener_get_base(pListener);
     struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
     bufferevent_setcb(bev, ReadCallback, NULL, EventCallback, pThis);
     bufferevent_enable(bev, EV_READ);

     pSocket->SetRemoteIp(address->sa_data);
     pSocket->SetSocket(bev);

     pSocket->SetGUID((uint64)bev);
     pSocket->m_bClose = false;
     pThis->m_mapSocketHandler.insert(pSocket->GetGUID(), pSocket->m_pSocketHandler);
     pSocket->m_pSocketHandler->OnAccept();
}

void wmLibeventServer::CloseConnection(class wmSocket* const pTarget)
{
    pTarget->SetClose();
    m_queueClose.push(pTarget->GetGUID());
}

class wmSocket* wmLibeventServer::GetSocket()
{
    wmSocket* pSocket = NULL;
    if (0 == m_nIdleSocketNum)
    {
        return NULL;
    }

    pSocket = &m_pSocketBuffer[m_pIdleSocket[0]];
    --m_nIdleSocketNum;
    m_pIdleSocket[0] = m_pIdleSocket[m_nIdleSocketNum];

    return pSocket;

}

void wmLibeventServer::EventCallback(struct bufferevent *bev, short events, void* pParam)
{
    wmLibeventServer* pThis = (wmLibeventServer*)pParam;

    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR))
    {
        SHashValue* pSocketHandler = pThis->m_mapSocketHandler.find((uint64)bev);
        if (NULL != pSocketHandler)
        {
            pSocketHandler->m_pValue->Socket()->SetClose();
            pSocketHandler->m_pValue->OnClose();
            pThis->m_pIdleSocket[pThis->m_nIdleSocketNum] = pSocketHandler->m_pValue->Socket()->GetIndex();
            ++pThis->m_nIdleSocketNum;
            pThis->m_mapSocketHandler.erase((uint64)bev);
            bufferevent_free(bev);
        }
    }
}


void wmLibeventServer::ReadCallback(struct bufferevent *bev, void* pParam)
{
    wmLibeventServer* pThis = (wmLibeventServer*)pParam;

    static char szBuf[MAX_PACKET_BUFF];

    SHashValue* pSocketHandler = pThis->m_mapSocketHandler.find((uint64)bev);
    if (NULL == pSocketHandler)
    {
        bufferevent_free(bev);
        return;
    }

    int nRecv = bufferevent_read(bev, szBuf, MAX_PACKET_BUFF);
    if (nRecv > 0)
    {
        pSocketHandler->m_pValue->OnRead(szBuf, nRecv);
    }
}

}
