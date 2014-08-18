////////////////////////////////////////////////////////////////////// 
///    COPYRIGHT NOTICE 
///    Copyright (c) 2011, 妙妙岛工作室
///    All rights reserved. 
/// 
/// @file PlayerSocketHander.h
/// @brief 一个玩家的连接socket
/// @author idk
/// @date 创建日期: 2011年11月29日
//////////////////////////////////////////////////////////////////////
#ifndef _LEGEND_PLAYER_SOCKET_HANDLER_H
#define _LEGEND_PLAYER_SOCKET_HANDLER_H

#include "Server/App.h"
#include <wormnet/ISocketHander.h>
#include <wormnet/ILogs.h>
#include <wormnet/wm_timer.h>
#include <wormnet/ByteBuffer.h>

class CPlayerSocketHandler : public worm::ISocketHandler
{
public:
    CPlayerSocketHandler();
    ~CPlayerSocketHandler();

    void OnAccept();
    void OnClose();
    void OnRead(void* pData, const uint16& wSize);

    void Send(worm::CByteBuffer& oPacket);
};

#endif // ifndef _LEGEND_PLAYER_SOCKET_HANDLER_H
