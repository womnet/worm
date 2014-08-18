
#ifndef _WORMNET_SERVER_SERVER_H
#define _WORMNET_SERVER_SERVER_H

#include "wormnet/wm_types.h"
#include "wormnet/ByteBuffer.h"
#include "wormnet/wm_mutex.h"
#include "wormnet/CircularBuffer.h"
#include "wormnet/ByteBuffer.h"
#include "wormnet/wm_SocketMap.h"

namespace worm
{

class WORM_DLL_EXPORT wmServer
{
public:
    virtual ~wmServer();

    virtual bool Init(class ISocketHandler* pHandlerArray, const uint32& dwHandlerSize, const uint16& wMax) = 0;
    virtual bool Open(const char* szIp, const uint16& nPort) = 0;
    virtual void EventLoop() = 0;
    virtual void Send(CByteBuffer& oPacket, class wmSocket* const pTarget) = 0;
    virtual void CloseConnection(class wmSocket* const pTarget) = 0;
    virtual void Shutdown(void) = 0;

    class ISocketHandler* GetSockethandler(const uint64& qwGuid);

protected:
    wmServer();
    virtual class wmSocket* GetSocket() = 0;

protected:
    /// 当前所有活动的socket
    wmSocketMap                             m_mapSocketHandler;
    /// socket 队列缓存
    class wmSocket*			                m_pSocketBuffer;
    /// 未使用的socket 索引列表
    int*							        m_pIdleSocket;
    // 可用的socket 数量
    int								        m_nIdleSocketNum;
    // socket 总数量
    int                                     m_nSocketMax;
    // 值为 true 时 接收数据线程将退出
    bool                                    m_bStopEvent;
};

}

#endif // ifndef _WORMNET_SERVER_SERVER_H
