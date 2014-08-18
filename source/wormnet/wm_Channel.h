
////////////////////////////////////////////////////////////////////// 
///    COPYRIGHT NOTICE 
///    Copyright (c) 2012, IDK
///    All rights reserved. 
/// 
/// @file wm_Channel.h
/// @brief 连接服务器的接口
/// 
/// 实现一个支持raknet和libevent连接到服务器的通用接口
/// 
/// @author idk
/// @date 创建日期: 2012年6月6日
//////////////////////////////////////////////////////////////////////

#ifndef _WORMNET_CHANNEL_H
#define _WORMNET_CHANNEL_H

#include "wormnet/wm_std.h"
#include "wormnet/wm_mutex.h"
#include "wormnet/ByteBuffer.h"
#include <wormnet/IChannelModule.h>

namespace worm
{

class WORM_DLL_EXPORT wmChannel
{
public:
    virtual ~wmChannel();

    virtual bool ConnectServer(const char* szIp, const uint16& nPort) = 0;
    virtual void CloseConnection() = 0;
    virtual void Send(CByteBuffer& oPacket) = 0;
    virtual void EventLoop() = 0;

protected:
    wmChannel(wmConnectedCB pConnectedCB, wmReceiveCB pReceiveCB, wmDisconnectCB pDisconnectCB);
    wmChannel();

protected:
    /// 为ture时, 退出接收数据线程
    bool                    m_bStopEvent;
    /// 连接到服务器后的回调
    wmConnectedCB           m_pConnectedCB;
    /// 接收到数据时的回调
    wmReceiveCB             m_pReceiveCB;
    /// 断开连接时的回调
    wmDisconnectCB          m_pDisconnectCB;
};

}

#endif // ifndef _WORMNET_CHANNEL_H
