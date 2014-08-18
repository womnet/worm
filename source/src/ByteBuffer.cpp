
#include <string.h>
#include "wormnet/ByteBuffer.h"
#include "wormnet/wm_Exception.h"
#include "wormnet/wm_std.h"

namespace worm
{

CByteBuffer::CByteBuffer(const uint32& dwSize)
{
	if (0 == dwSize)
    {
        assert(0);
        m_dwBufferSize = MAX_PACKET_BUFF;
    }
    else
    {
        m_dwBufferSize = dwSize;
    }

    m_pBuf = new char[m_dwBufferSize];
	memset(m_pBuf, 0, m_dwBufferSize);
	m_pReadCurPos = m_pBuf;
	m_pEnd = m_pReadCurPos;
}

CByteBuffer::CByteBuffer(const CByteBuffer& oBuffer)
{
    if (&oBuffer == this)
    {
        return;
    }
    
    if (0 == oBuffer.m_dwBufferSize)
    {
        assert(0);
        throw wmException("CByteBuffer(const CByteBuffer& oBuffer): dwSize == 0");
        return;
    }

    m_dwBufferSize = oBuffer.m_dwBufferSize;

    m_pBuf = new char[m_dwBufferSize];
    memcpy(m_pBuf, oBuffer.m_pBuf, oBuffer.size());
    m_pReadCurPos = m_pBuf + (oBuffer.m_pReadCurPos - oBuffer.m_pBuf);
    m_pEnd = m_pBuf + oBuffer.size();
}

CByteBuffer::CByteBuffer(char* pData, const uint32& dwSize)
{
    if (NULL==pData || 0==dwSize)
    {
        assert(0);
        m_dwBufferSize = MAX_PACKET_BUFF;
        m_pBuf = new char[m_dwBufferSize];
        memset(m_pBuf, 0, m_dwBufferSize);
        m_pReadCurPos = m_pBuf;
        m_pEnd = m_pReadCurPos;

        return;
    }

    if (dwSize < MAX_PACKET_BUFF)
    {
        m_dwBufferSize = MAX_PACKET_BUFF;
    }
    else
    {
        m_dwBufferSize = dwSize;
    }
    m_pBuf = new char[m_dwBufferSize];
    memset(m_pBuf, 0, m_dwBufferSize);
    memcpy(m_pBuf, pData, dwSize);
    
    m_pReadCurPos = m_pBuf;
    m_pEnd = m_pReadCurPos;
}

CByteBuffer::~CByteBuffer()
{
    delete []m_pBuf;
    m_pBuf = NULL;
}

CByteBuffer& CByteBuffer::operator=(const CByteBuffer& oBuffer)
{
    if (&oBuffer == this)
    {
        return *this;
    }

    if (0 == oBuffer.m_dwBufferSize)
    {
        assert(0);
        throw wmException("CByteBuffer(const CByteBuffer& oBuffer): dwSize == 0");
        return *this;
    }

    if (m_dwBufferSize < oBuffer.m_dwBufferSize)
    {
        delete []m_pBuf;
        m_pBuf = new char[oBuffer.m_dwBufferSize];;
    }

    m_dwBufferSize = oBuffer.m_dwBufferSize;
    memcpy(m_pBuf, oBuffer.m_pBuf, oBuffer.size());
    m_pReadCurPos = m_pBuf + (oBuffer.m_pReadCurPos - oBuffer.m_pBuf);
    m_pEnd = m_pBuf + oBuffer.size();

    return *this;
}

void CByteBuffer::Clear(void)
{
    memset(m_pBuf, 0, size());
    m_pReadCurPos = m_pBuf;
    m_pEnd = m_pReadCurPos;
}

bool CByteBuffer::IsRead(const uint16& wSize)
{
    const uint16 wOddSize = (m_pReadCurPos + wSize - m_pBuf) & 0xffff;
	if (wOddSize > size())
    {
        throw wmException("CByteBuffer::IsRead. Request read [%u] byte overflow", wSize);
        return false;
    }
    else
    {
        return true;
    }
}

bool CByteBuffer::IsPush(const uint16& wSize)
{
    const uint16 wOddSize = m_dwBufferSize - size();
	if (wSize > wOddSize)
	{
        throw wmException("CByteBuffer::IsPush. Request push [%u] byte, buffer odd [%u] byte", \
            wSize, wOddSize);
		return false;
	}
	else
	{
		return true;
	}
}

void CByteBuffer::RewriteData(const uint16& wPos, const uint8& byValue)
{
	if (size() == m_dwBufferSize)
	{
        throw wmException("CByteBuffer::RewriteData uint8: Request rewrite data postion: %u overflow", \
            wPos);
        return;
	}

    m_pBuf[wPos] = byValue;
}

void CByteBuffer::RewriteData(const uint16& wPos, const uint16& wValue)
{
    if (size() > m_dwBufferSize-2)
	{
        throw wmException("CByteBuffer::RewriteData uint16: Request rewrite data postion: %u overflow", \
            wPos);
        return;
	}

    *((uint16*)&m_pBuf[wPos]) = wValue;
}

void CByteBuffer::RewriteData(const uint16& wPos, const uint32& dwValue)
{
    if (size() > m_dwBufferSize-4)
	{
        throw wmException("CByteBuffer::RewriteData uint32 Request rewrite data postion: %u, overflow", \
            wPos);
        return;
	}

    *((uint32*)&m_pBuf[wPos]) = dwValue;
}

void CByteBuffer::operator<<(const char& value)
{
	assert(NULL != m_pEnd);

	if (!IsPush(1)) return;

	*m_pEnd = value;
	++m_pEnd;
}

void CByteBuffer::operator<<(const uint8& value)
{
	assert(NULL != m_pEnd);

	if (!IsPush(1)) return;

	*(uint8*)m_pEnd = value;
	++m_pEnd;
}

void CByteBuffer::operator<<(const bool& value)
{
	assert(NULL != m_pEnd);

	if (!IsPush(1)) return;

	*(uint8*)m_pEnd = value?1:0;
	++m_pEnd;
}

void  CByteBuffer::operator<<(const uint16& value)
{
	assert(NULL != m_pEnd);
	if (!IsPush(2)) return;

	*(uint16*)m_pEnd = value;
	m_pEnd += 2;
}

void CByteBuffer::operator<<(const int16& value)
{
    assert(NULL != m_pEnd);
    if (!IsPush(2)) return;

    *(int16*)m_pEnd = value;
    m_pEnd += 2;
}

void CByteBuffer::operator<<(const int32& value)
{
    assert(NULL != m_pEnd);
    if (!IsPush(4)) return;

    *(int32*)m_pEnd = value;
    m_pEnd += 4;
}

void  CByteBuffer::operator<<(const uint32& value)
{
	assert(NULL != m_pEnd);
	if (!IsPush(4)) return;

	*(uint32*)m_pEnd = value;
	m_pEnd += 4;
}

void CByteBuffer::operator<<(const int64& value)
{
    assert(NULL != m_pEnd);
    if (!IsPush(8)) return;

    *(int64*)m_pEnd = value;
    m_pEnd += 8;
}

void CByteBuffer::operator<<(const uint64& value)
{
    assert(NULL != m_pEnd);
    if (!IsPush(8)) return;

    *(uint64*)m_pEnd = value;
    m_pEnd += 8;
}

void  CByteBuffer::operator<<(const float& value)
{
	assert(NULL != m_pEnd);
	if (!IsPush(4)) return;

	*(float*)m_pEnd = value;
	m_pEnd += 4;
}

void  CByteBuffer::operator<<(const double& value)
{
	assert(NULL != m_pEnd);
	if (!IsPush(8)) return;

	*(double*)m_pEnd = value;
	m_pEnd += 8;
}

void CByteBuffer::operator<<(const char* str)
{
    assert(NULL != m_pEnd);
    assert(NULL != str);

    uint16 wLen = (strlen(str)+sizeof(uint16)) & 0xffff;
    if (!IsPush(wLen)) return;

    wLen -= sizeof(uint16);

    *(uint16*)m_pEnd = wLen;
    m_pEnd += sizeof(uint16);

    memcpy(m_pEnd, str, wLen);
    m_pEnd += wLen;
}

void CByteBuffer::Append(const void* pData, const uint16& wSize)
{
	assert( NULL != m_pEnd);
	if (0 == wSize)
	{
		return;
	}

	if (!IsPush(wSize)) return;

	memcpy(m_pEnd, pData, wSize);
	m_pEnd += wSize;
}

uint16 CByteBuffer::wpos(void)
{
	uint16 wRet = (m_pEnd - m_pBuf) & 0xffff;
	return wRet;
}

void CByteBuffer::operator>>(uint8& value)
{
	assert(NULL != m_pReadCurPos);
	if (!IsRead(sizeof(uint8)))
	{
		return;
	}

	value = *(uint8*)m_pReadCurPos;
	m_pReadCurPos += sizeof(uint8);
}

void CByteBuffer::operator>>(char& value)
{
	assert(NULL != m_pReadCurPos);
	if (!IsRead(sizeof(char)))
	{
		return;
	}

	value = *(char*)m_pReadCurPos;
	m_pReadCurPos += sizeof(char);
}

void CByteBuffer::operator>>(bool& b)
{
	if (!IsRead(sizeof(uint8)))
	{
		return;
	}

	b = (*(char*)m_pReadCurPos == 1);
	m_pReadCurPos += sizeof(bool);
}

void CByteBuffer::operator>>(uint16& value)
{
	assert(NULL != m_pReadCurPos);
	if (!IsRead(2))
	{
		return;
	}

	value = *(uint16*)m_pReadCurPos;
	m_pReadCurPos += 2;
}

void CByteBuffer::operator>>(short& value)
{
	assert(NULL != m_pReadCurPos);
	if (!IsRead(sizeof(short)))
	{
		return;
	}

	value = *(short*)m_pReadCurPos;
	m_pReadCurPos += sizeof(short);
}

void CByteBuffer::operator>>(uint32& value)
{
	assert(NULL != m_pReadCurPos);
	if (!IsRead(4))
	{
		return;
	}

	value = *(uint32*)m_pReadCurPos;
	m_pReadCurPos += 4;
}

void CByteBuffer::operator>>(uint64& value)
{
    assert(NULL != m_pReadCurPos);
    if (!IsRead(8))
    {
        return;
    }

    value = *(uint64*)m_pReadCurPos;
    m_pReadCurPos += 8;
}

void CByteBuffer::operator>>(int& value)
{
	assert(NULL != m_pReadCurPos);
	if (!IsRead(sizeof(int)))
	{
		return;
	}

	value = *(int*)m_pReadCurPos;
	m_pReadCurPos += sizeof(int);
}

void CByteBuffer::operator>>(float& value)
{
	assert(NULL != m_pReadCurPos);
	if (!IsRead(4))
	{
		return;
	}

	value = *(float*)m_pReadCurPos;
	m_pReadCurPos += 4;
}

void CByteBuffer::operator>>(double& value)
{
	assert(NULL != m_pReadCurPos);
	if (!IsRead(8))
	{
		return;
	}

	value = *(double*)m_pReadCurPos;
	m_pReadCurPos += 8;
}

void CByteBuffer::ReadString(char* pBuf, const uint16& wBufSize)
{
    assert(NULL != m_pReadCurPos);
    assert(NULL != pBuf);
    assert(0 != wBufSize);

    if (!IsRead(sizeof(uint16)))
    {
        return;
    }

    // 包中的字符串长度
    uint16 wStrLen = *(uint16*)m_pReadCurPos;
    m_pReadCurPos += sizeof(uint16);

    if (!IsRead(wStrLen))
    {
        return;
    }

    if (wBufSize <= wStrLen)
    {
        memcpy(pBuf, m_pReadCurPos, wBufSize-1);
        pBuf[wBufSize-1] = 0;
    }
    else
    {
        memcpy(pBuf, (char*)m_pReadCurPos, wStrLen);
        pBuf[wStrLen] = 0;
    }

    m_pReadCurPos += wStrLen;
}

void CByteBuffer::ReadData(void* pBuf, const uint16& wBufSize)
{
	assert(NULL != m_pReadCurPos);
	assert(NULL != pBuf);
	assert(0 != wBufSize);

	if (!IsRead(wBufSize))
	{
		return;
	}

    memcpy(pBuf, (char*)m_pReadCurPos, wBufSize);
	m_pReadCurPos += wBufSize;
}

void CByteBuffer::ReadSkip(const uint16& wSize)
{
	assert(NULL != m_pReadCurPos);
	if (!IsRead(wSize)) return;

	uint8* pResutl = (uint8*)m_pReadCurPos;
	m_pReadCurPos += wSize;
}

uint16 CByteBuffer::size() const
{
    uint16 wSize = (m_pEnd - m_pBuf) & 0x1fff;

    return wSize;
}

void CByteBuffer::Encryption(class ICryp* pCryp, const uint8* pKey, const uint32 nKeyLen)
{

}

}
