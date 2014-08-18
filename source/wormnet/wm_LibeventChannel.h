
#ifndef _WORMNET_LIBEVENT_CHANNEL_H
#define _WORMNET_LIBEVENT_CHANNEL_H

#include <event2/event_struct.h>
#include <wormnet/wm_thread.h>
#include <queue>
#include "wormnet/wm_Channel.h"

namespace worm
{

class WORM_DLL_EXPORT wmLibeventChannel : public wmChannel
{
public:
    wmLibeventChannel(wmConnectedCB pConnectedCB, wmReceiveCB pReceiveCB, wmDisconnectCB pDisconnectCB);
    ~wmLibeventChannel();

    bool ConnectServer(const char* szIp, const uint16& nPort);
    void CloseConnection();
    void Send(CByteBuffer& oPacket);
    void EventLoop();

private:
    wmLibeventChannel();
    static void EventCallback(struct bufferevent* pBev, short nEvents, void* pParam);
    static void ReadCallback(struct bufferevent* pBev, void* pParam);

private:
    struct event_base*                  m_pBase;
    struct bufferevent*                 m_pBev;
};

}

#endif // ifndef _WORMNET_LIBEVENT_CHANNEL_H
