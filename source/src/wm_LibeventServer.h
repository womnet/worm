
#ifndef _WORMNET_SERVER_LIBEVENT_SERVER_H
#define _WORMNET_SERVER_LIBEVENT_SERVER_H

#include <event2/event_struct.h>
#include <queue>
#include <wormnet/wm_thread.h>
#include "wormnet/wm_server.h"

namespace worm
{

struct SSendBevBuf
{
    uint64                  m_qwSocketKey;
    CByteBuffer             m_oPacket;

    SSendBevBuf()
    {
        m_qwSocketKey -= m_qwSocketKey;
    }
};

class WORM_DLL_EXPORT wmLibeventServer : public wmServer
{
public:
    wmLibeventServer();
    ~wmLibeventServer();

    bool Init(class ISocketHandler* pHandlerArray, const uint32& dwHandlerSize, const uint16& wMax);
    bool Open(const char* szIp, const uint16& nPort);
    void EventLoop();
    void Send(CByteBuffer& oPacket, class wmSocket* const pTarget);
    void CloseConnection(class wmSocket* const pTarget);
    void Shutdown(void);

private:
    class wmSocket* GetIdleSession();
    static void AcceptCallback(struct evconnlistener* pListener, evutil_socket_t fd, struct sockaddr *address, int socklen, void* pParam);
    static void EventCallback(struct bufferevent *bev, short events, void* pParam);
    static void ReadCallback(struct bufferevent *bev, void* pParam);

    class wmSocket* GetSocket();

private:
    struct event_base*                      m_pBase;
    /// 要关闭的连接队列, <连接guid>
    std::queue<uint64>                      m_queueClose;
};

}

#endif // ifndef _WORMNET_SERVER_LIBEVENT_SERVER_H
