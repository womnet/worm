
#ifndef _WORMNET_BYTE_BUFFER_H
#define _WORMNET_BYTE_BUFFER_H

#include <stdio.h>
#include <assert.h>
#include "wormnet/wm_std.h"
#include "wormnet/ILogs.h"
#include "wormnet/wm_types.h"

/// #define _BIG_PACKET

#ifdef _BIG_PACKET
/// 包的最大长度: 100K
#define MAX_PACKET_SIZE     102400
/// 包的最大缓存
#define MAX_PACKET_BUFF     102528
/// 包的最小长度
#define MIN_PACKET_SIZE     12
/// 包头长度
#define PACKET_HEADER_SIZE  8
#else
/// 包的最大长度: 7K
#define MAX_PACKET_SIZE     7168
/// 包的最大缓存
#define MAX_PACKET_BUFF     7232
/// 包的最小长度
#define MIN_PACKET_SIZE     10
/// 包头长度
#define PACKET_HEADER_SIZE  6
#endif // ifdef _BIG_PACKET

namespace worm
{
class WORM_DLL_EXPORT CByteBuffer
{
public:
    CByteBuffer(const uint32& dwSize=MAX_PACKET_BUFF);
	CByteBuffer(const CByteBuffer& packet);
    CByteBuffer(char* pData, const uint32& dwSize);
    virtual ~CByteBuffer();
    CByteBuffer& operator=(const CByteBuffer& packet);

	virtual void Clear(void);

    uint16 GetBufSize(void) const { return m_dwBufferSize; }

	void operator<<(const char& value);
	void operator<<(const uint8& value);

	void operator<<(const uint16& value);
    void operator<<(const int16& value);

    void operator<<(const int32& value);
	void operator<<(const uint32& value);

    void operator<<(const int64& value);
    void operator<<(const uint64& value);

	void operator<<(const float& value);
	void operator<<(const double& value);
	///   
	/// 将字符串压入缓存
	/// @param str 源字符串
	/// @return 无
    /// @note 格式为: 2字节字符串长度, 字符串内容(不包含'\0')
	/// 
	void operator<<(const char* str);
	void operator<<(const bool& value);
    //
    // 函数名   : Append
    // 功能     : 向数据包添加数据
    // 参数     : 
    //          [in] pData: 要添加的数据指针
    //          [in] nLen: 要添加的数据长度
    //
	void Append(const void* pData, const uint16& wSize);
	//
    // 函数名   : operator>>
    // 功能     : 重载 >> 操作符, 从数据包读取数据. 下面一系列只是参数类型不同
    // 参数     : 
    //          [out] value: 保存读取的数据
    // 返回值   : true 成功, false 失败
    //
	void operator>>(uint8& value);
	void operator>>(char& value);
	void operator>>(bool& value);
	void operator>>(uint16& value);
	void operator>>(short& value);
	void operator>>(uint32& value);
    void operator>>(uint64& value);
	void operator>>(int& value);
	void operator>>(float& value);
	void operator>>(double& value);
    ///   
    /// 读取字符串
    /// @param pBuf 目标缓存
    /// @param wBufSize 缓存大小
    /// @return 无
    /// @see operator<<(const char* str)
    /// 
    void ReadString(char* pBuf, const uint16& wBufSize);

	void ReadData(void* const pBuf, const uint16& wBufSize);
	void ReadSkip(const uint16& wSize);

	uint16 wpos(void);

	void RewriteData(const uint16& nPos, const uint8& byValue);
    void RewriteData(const uint16& nPos, const uint16& wValue);
    void RewriteData(const uint16& nPos, const uint32& dwValue);

    char* data() const { return m_pBuf; }
    uint16 size() const;
	char* write_buf() const { return m_pEnd; }
	char* read_buf() const { return m_pReadCurPos; }

    virtual void Encryption(class ICryp* pCryp, const uint8* pKey, const uint32 nKeyLen);

protected:
	virtual bool IsRead(const uint16& wSize);
	virtual bool IsPush(const uint16& wSize);

protected:
    // 当前使用的缓存
    char*                   m_pBuf;
	// 当前的读包位置
	char*				    m_pReadCurPos;
	// 当前包的尾部, 添加数据时往这里添加
	char*				    m_pEnd;
	// 可用的缓存大小
	uint32				    m_dwBufferSize;
};

}

#endif // ifndef _SRAK_SHARE_PACKET_H_
