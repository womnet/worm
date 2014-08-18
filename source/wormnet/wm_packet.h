
#ifndef _WORMNET_CUSTOM_PACKET_H
#define _WORMNET_CUSTOM_PACKET_H

#include "wormnet/ByteBuffer.h"

namespace worm
{

/// 数据包的数据头
struct SDataHeader
{
    /// 交易码
    uint16      m_wOpcode;
    /// 返回码
    uint16      m_wRetCode;
    /// 数据, 使用时做指针使用
    uint8       m_pData[4];

    void Reset()
    {
        m_wOpcode -= m_wOpcode;
        m_wRetCode -= m_wRetCode;
    }

    ///  
    /// 获取数据头的长度
    /// @return 4
    /// 
    uint8 DataHeaderSize() { return 4; }

    SDataHeader()
    {
        Reset();
    }
};

struct SPacketHeader
{
#ifdef _BIG_PACKET
    /// 包的验证码
    int32       m_VerCode:8;

    /// 数据包的总长度, 包含SPacketHeader长度
    int32      m_PacketSize:21;
    /// 是否加密
    int32      m_bCryp:1;
    /// 是否压缩
    int32      m_bZip:1;
    /// 保留
    int32      m_Reserve1:1;

    /// 未加密时的数据长度, 从 SDataHeader开始
    int32      m_DataSize:21;
    /// 保留
    int32      m_Reserve2:11;

    /// 获取包头的数据头的总长度
    uint8 PacketHeaderSize() { return 12; }
#else
    /// 包的验证码
    uint16      m_VerCode;

    /// 数据包的总长度, 包含SPacketHeader长度
    uint16      m_PacketSize:13;
    /// 是否加密
    uint16      m_bCryp:1;
    /// 是否压缩
    uint16      m_bZip:1;
    /// 保留
    uint16      m_Reserve1:1;

    /// 未加密时的数据长度, 从 SDataHeader开始
    uint16      m_DataSize:13;
    /// 保留
    uint16      m_Reserve2:3;

    /// 获取包头的数据头的总长度
    uint8 PacketHeaderSize() { return 10; }
#endif // ifdef _BIG_PACKET

    /// 数据头
    SDataHeader     m_sDataHeader;

    void Reset()
    {
        m_VerCode -= m_VerCode;
        m_PacketSize -= m_PacketSize;
        m_DataSize -= m_DataSize;
        m_sDataHeader.Reset();
    }

    SPacketHeader()
    {
        Reset();
    }
};


class WORM_DLL_EXPORT wmPacket : public CByteBuffer
{
public:
    wmPacket(const uint32& dwSize=MAX_PACKET_BUFF);
    ~wmPacket();

    wmPacket(const wmPacket& oPacket);
    wmPacket& operator=(const wmPacket& oPacket);

    void Reset();

    void SetOpcode(const uint16& wOpcode)
    {
        m_pPacketHeader->m_sDataHeader.m_wOpcode = wOpcode;
    }
    const uint16 GetOpcode()
    {
        return m_pPacketHeader->m_sDataHeader.m_wOpcode;
    }
    void SetRetCode(const uint16& wRetCode)
    {
        m_pPacketHeader->m_sDataHeader.m_wRetCode = wRetCode;
    }
    const uint16& GetRetCode() const
    {
        return m_pPacketHeader->m_sDataHeader.m_wRetCode;
    }
    void Encryption(class ICryp* pCryp, const uint8* pKey, const uint32& dwKeyLen);
    void Decryption(class ICryp* pCryp, const uint8* pKey, const uint32& dwKeyLen);

    SPacketHeader* PacketHeader() { return m_pPacketHeader; }

    ///  
    /// 获取包中的数据的长度
    /// @return 数据长度 >= 0
    /// @note 不包括包头和数据头的长度
    /// 
    uint16 DataSize();

    virtual bool IsValid();

    void Make();

private:
    SPacketHeader*      m_pPacketHeader;
};

}

#endif // ifndef _WORMNET_CUSTOM_PACKET_H
