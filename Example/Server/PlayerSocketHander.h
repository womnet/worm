////////////////////////////////////////////////////////////////////// 
///    COPYRIGHT NOTICE 
///    Copyright (c) 2011, ���������
///    All rights reserved. 
/// 
/// @file PlayerSocketHander.h
/// @brief һ����ҵ�����socket
/// @author idk
/// @date ��������: 2011��11��29��
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
