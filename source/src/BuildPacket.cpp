
#include "wormnet/BuildPacket.h"

namespace worm
{


CBuildPacket::CBuildPacket()
{
    NewPacket();
}

CBuildPacket::~CBuildPacket()
{
   m_pPacketHeader = NULL;
   m_pRecvData = NULL;
}

bool CBuildPacket::BuildPacket(uint8* pData, uint16 wDataSize)
{
    assert(NULL != pData);
    assert(0 != wDataSize);

    /// 还需要的数据长度
    uint32 dwRecvSize = 0;
    
    for (;;)
    {
        if (m_wRecved < MIN_PACKET_SIZE)
        {
            dwRecvSize = MIN_PACKET_SIZE - m_wRecved;
            if (wDataSize < dwRecvSize)
            {
                memcpy(m_pRecvData+m_wRecved, pData, wDataSize);
                m_wRecved += wDataSize;
                return true;
            }
            memcpy(m_pRecvData+m_wRecved, pData, dwRecvSize);
            wDataSize -= dwRecvSize;
            m_wRecved += dwRecvSize;
            pData += dwRecvSize;

            if (!m_oNewPacket.IsValid())
            {
                m_wRecved -= m_wRecved;
                return false;
            }
            if (m_oNewPacket.size() == m_pPacketHeader->m_PacketSize)
            {
                PacketHandler();
                NewPacket();
            }

            if (0 == wDataSize)
            {
                return true;
            }
        }

        /// 包体数据
        dwRecvSize = m_pPacketHeader->m_PacketSize - m_wRecved;
        if (wDataSize < dwRecvSize)
        {
            m_oNewPacket.Append(pData, wDataSize);
            m_wRecved += wDataSize;
            return true;
        }
        m_oNewPacket.Append(pData, dwRecvSize);
        wDataSize -= dwRecvSize;
        pData += dwRecvSize;
        m_wRecved += dwRecvSize;
        PacketHandler();
        NewPacket();

        if (0 == wDataSize)
        {
            break;
        }
    }
    return true;
}

void CBuildPacket::NewPacket()
{
    m_oNewPacket.Reset();
    m_pPacketHeader = m_oNewPacket.PacketHeader();
    m_pRecvData = (uint8*)m_pPacketHeader;
    m_wRecved -= m_wRecved;
}

}
