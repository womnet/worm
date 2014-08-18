////////////////////////////////////////////////////////////////////// 
///    COPYRIGHT NOTICE 
///    Copyright (c) 2011, 妙妙岛工作室
///    All rights reserved. 
/// 
/// @file BuildPacket.h
/// @brief 将收到的数据组成完整的包
/// 
/// @author idk
/// @date 创建日期: 2011年12月5日
//////////////////////////////////////////////////////////////////////

#ifndef _WORMNET_BUILD_PACKET_H
#define _WORMNET_BUILD_PACKET_H

#include "wormnet/wm_packet.h"
#include <queue>

namespace worm
{

class WORM_DLL_EXPORT CBuildPacket
{
public:
    CBuildPacket();
    virtual ~CBuildPacket();

    ///   
    /// 组织包
    /// @param pData 收到的数据
    /// @param wDataSize 收到的数据长度
    /// @return false 表示包有异常, 否则为正常
    /// 
    bool BuildPacket(uint8* pData, uint16 wDataSize);

protected:
    /// 生成一个新的缓存包, 用来接受数据
    void NewPacket();
    /// 将一个完成的包压入队列
    virtual void PacketHandler() = 0;

protected:
    /// 当前使用的包
    wmPacket                m_oNewPacket;
    /// 当前已经收到的数据长度
    uint16                  m_wRecved;
    /// 包头
    SPacketHeader*          m_pPacketHeader;
    /// 包操作指针
    uint8*                  m_pRecvData;
};

}

#endif /// ifndef _WORMNET_BUILD_PACKET_H
