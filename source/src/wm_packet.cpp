
#include <string.h>
#include "wormnet/wm_Exception.h"
#include "wormnet/wm_packet.h"
#include "wormnet/wm_cryp.h"
#include <zlib/zlib.h>

namespace worm
{

wmPacket::wmPacket(const uint32& dwSize/*=MAX_PACKET_BUFF*/) : CByteBuffer(dwSize)
{
    if (dwSize > MAX_PACKET_BUFF)
    {
        assert(0);
        throw wmException("wmPacket::wmPacket: 包大小不能超出 %u", MAX_PACKET_BUFF);
    }
    Reset();
}

wmPacket::wmPacket(const wmPacket& oPacket)
{
    if (&oPacket == this)
    {
        return;
    }

    if (0 == oPacket.m_dwBufferSize)
    {
        assert(0);
        throw wmException("wmPacket::wmPacket: size == 0");
        return;
    }

    m_dwBufferSize = oPacket.m_dwBufferSize;

    m_pBuf = new char[m_dwBufferSize];
    m_pPacketHeader = (SPacketHeader*)m_pBuf;
    memcpy(m_pBuf, oPacket.m_pBuf, oPacket.size());
    m_pReadCurPos = m_pBuf + (oPacket.m_pReadCurPos - oPacket.m_pBuf);
    m_pEnd = m_pBuf + (oPacket.m_pEnd - oPacket.m_pBuf);
}

wmPacket::~wmPacket()
{

}

wmPacket& wmPacket::operator=(const wmPacket& oPacket)
{
    if (&oPacket == this)
    {
        return *this;
    }

    if (0 == oPacket.m_dwBufferSize)
    {
        assert(0);
        throw wmException("wmPacket::operator=: size == 0");
        return *this;
    }

    if (m_dwBufferSize < oPacket.m_dwBufferSize)
    {
        delete []m_pBuf;
        m_pBuf = new char[oPacket.m_dwBufferSize];
        m_dwBufferSize = oPacket.m_dwBufferSize;
    }
    
    m_pPacketHeader = (SPacketHeader*)m_pBuf;
    memcpy(m_pBuf, oPacket.m_pBuf, oPacket.size());
    m_pReadCurPos = m_pBuf + (oPacket.m_pReadCurPos - oPacket.m_pBuf);
    m_pEnd = m_pBuf + oPacket.size();

    return *this;
}

void wmPacket::Reset()
{
    m_pPacketHeader = (SPacketHeader*)m_pBuf;
    m_pEnd = (char*)m_pPacketHeader->m_sDataHeader.m_pData;
    m_pReadCurPos = m_pEnd;
    m_pPacketHeader->m_bCryp = 0;
    m_pPacketHeader->m_bZip = 0;
    m_pPacketHeader->m_sDataHeader.m_wRetCode = 0;
}

bool wmPacket::IsValid()
{
    if (m_pPacketHeader->m_PacketSize < MIN_PACKET_SIZE)
    {
        return false;
    }

    if (m_pPacketHeader->m_PacketSize >= MAX_PACKET_SIZE)
    {
        return false;
    }

    uint16 wTmp = m_pPacketHeader->m_PacketSize ^ m_pPacketHeader->m_DataSize;

#ifdef _BIG_PACKET
    wTmp &= 0xff;
    return (wTmp == m_pPacketHeader->m_VerCode);
#else
    return (wTmp == m_pPacketHeader->m_VerCode);
#endif // 
}

void wmPacket::Make()
{
    m_pPacketHeader->m_DataSize = (m_pEnd-(char*)&m_pPacketHeader->m_sDataHeader) & 0x1fff;
    m_pPacketHeader->m_PacketSize = (m_pEnd-m_pBuf) & 0x1fff;
    m_pPacketHeader->m_VerCode = m_pPacketHeader->m_PacketSize ^ m_pPacketHeader->m_DataSize;
}

/// 大于此值时启用zip压缩
#define ENABLE_ZLIB_SIZE    (96)

void wmPacket::Encryption(class ICryp* pCryp, const uint8* pKey, const uint32& dwKeyLen)
{
    assert(0 == m_pPacketHeader->m_bCryp);
    assert(0 == m_pPacketHeader->m_bZip);

    m_pPacketHeader->m_DataSize = (m_pEnd-(char*)&m_pPacketHeader->m_sDataHeader) & 0x1fff;
    m_pPacketHeader->m_PacketSize = (m_pEnd-m_pBuf) & 0x1fff;

    int32 dwDataSize = m_pPacketHeader->m_DataSize;

    /// 先压缩后加密
#ifdef ENABLE_ZIP
    if (m_pPacketHeader->m_DataSize >= ENABLE_ZLIB_SIZE)
    {
        /// 压缩后的数据缓存
        uint8 pDataBuf[MAX_PACKET_SIZE];
        uLongf nBufferSize = MAX_PACKET_SIZE;
        //压缩
        if (Z_OK == compress(pDataBuf, &nBufferSize, (uint8*)&m_pPacketHeader->m_sDataHeader, m_pPacketHeader->m_DataSize))
        {
            // 压缩成功则把压缩后的数据复制到包缓存中
            memcpy((uint8*)&m_pPacketHeader->m_sDataHeader, pDataBuf, nBufferSize);
            m_pPacketHeader->m_bZip = 1;
            m_pPacketHeader->m_PacketSize = (nBufferSize+PACKET_HEADER_SIZE) & 0x1fff;
            dwDataSize = nBufferSize & 0x1fff;
        }
    }
#endif //

    /// 按8字节对齐
    dwDataSize += (8 - (dwDataSize&7));
    m_pPacketHeader->m_bCryp = pCryp->Encryption(&m_pPacketHeader->m_sDataHeader, dwDataSize, &m_pPacketHeader->m_sDataHeader, pKey, dwKeyLen);

    if (m_pPacketHeader->m_bCryp)
    {
        m_pPacketHeader->m_PacketSize = dwDataSize + PACKET_HEADER_SIZE;
    }

    m_pPacketHeader->m_VerCode = m_pPacketHeader->m_PacketSize ^ m_pPacketHeader->m_DataSize;

    m_pEnd = m_pBuf + m_pPacketHeader->m_PacketSize;
}

void wmPacket::Decryption(class ICryp* pCryp, const uint8* pKey, const uint32& dwKeyLen)
{
    static uint8 pUnzipBuf[MAX_PACKET_SIZE];

    pCryp->Decryption(&m_pPacketHeader->m_sDataHeader, \
        m_pPacketHeader->m_PacketSize-PACKET_HEADER_SIZE, \
        &m_pPacketHeader->m_sDataHeader, pKey, dwKeyLen);

    m_pEnd = m_pBuf + m_pPacketHeader->m_DataSize + PACKET_HEADER_SIZE;

    if (m_pPacketHeader->m_bZip)
    {
        uLongf nDataSize = MAX_PACKET_SIZE;
        if (Z_OK == uncompress(pUnzipBuf, &nDataSize, (uint8*)&(m_pPacketHeader->m_sDataHeader), m_pPacketHeader->m_PacketSize-PACKET_HEADER_SIZE))
        {
            memcpy(&m_pPacketHeader->m_sDataHeader, pUnzipBuf, nDataSize);
        }
        else
        {
            m_pPacketHeader->m_sDataHeader.m_wOpcode = 0xffff;
        }
    }
}

uint16 wmPacket::DataSize()
{
    return (m_pPacketHeader->m_DataSize - m_pPacketHeader->m_sDataHeader.DataHeaderSize());
}

}
