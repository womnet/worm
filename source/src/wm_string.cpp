
#include <stdio.h>
#include <assert.h>
#include "wormnet/wm_types.h"
#include <string.h>
//#include <iconv/iconv.h>

WORM_DLL_EXPORT size_t strlcpy(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}

#ifndef WIN32
WORM_DLL_EXPORT char* strupr(char* src)
{
	if (NULL == src)
	{
		return NULL;
	}

	char* dst = src;
	while (*dst!='\0')
	{
		if((*dst <= 'z')&&(*dst >= 'a'))
		{
			*dst -= 'a' - 'A';
		}
		++dst;
	}

	return src;
}

WORM_DLL_EXPORT char* strlwr(char* src)
{
	if (NULL == src)
	{
		return NULL;
	}

	char* dst = src;
	while (*dst!='\0')
	{
		if((*dst <= 'Z')&&(*dst >= 'A'))
		{
			*dst += 'a' - 'A';
		}
		++dst;
	}

	return src;
}

#endif //ifndef WIN32

/*
#define UTF8_ONE_BYTE       (0x00)
#define UTF8_TWO_BYTE       (0x06)
#define UTF8_THREE_BYTE     (0x0e)
#define UTF8_FOUR_BYTE      (0x1e)
#define UTF8_FIVE_BYTE      (0x3e)
#define UTF8_SIX_BYTE       (0x7e)
#define UTF8_SPECIAL_BYTE   (0x02)

WORM_DLL_EXPORT bool is_utf8_special_byte(const uint8& byChar)
{  
    return (byChar>>6 == UTF8_SPECIAL_BYTE);   
}

WORM_DLL_EXPORT void StrConv(const char* pFromCode, const char* pToCode, const char* pSrc, size_t dwSrcSize, char* pDst, size_t dwBufSize)
{
    //打开字符集转换 
    iconv_t hIconv = iconv_open(pToCode, pFromCode);
    if (-1 == (long)hIconv)
    {
        //打开失败，可能不支持的字符集 
        return;
    }

    //开始转换
#ifdef WIN32
    iconv(hIconv, &pSrc, &dwSrcSize, (char**)(&pDst), &dwBufSize);
#else
    iconv(hIconv, (char**)(&pSrc), &dwSrcSize, (char**)(&pDst), &dwBufSize);
#endif //
    //关闭字符集转换 
    iconv_close(hIconv);
}

WORM_DLL_EXPORT uint8 IsUtf8(const char* pSrc)
{
    uint32 dwSrcLen = strlen(pSrc);
    uint8 byChar = 0;
    int j = 0;
    int i =0;

    /// 判断uft8
    byChar = pSrc[0];
    /// 英文字符直接复制
    if (byChar > 0)
    {
        return 0;
    }

    if (byChar>>5 == UTF8_TWO_BYTE)
    {
        /// 是GBK
        if (byChar >= 0x81 && byChar <= 0xfe)
        {
            byChar = pSrc[1];
            if (byChar>=0x40 && byChar<=0xfe)
            {
                return 0;
            }
        }

        return 2;
    }
    else if (byChar>>4 == UTF8_THREE_BYTE)
    {

        return 3;
    }
    else if (byChar>>3 == UTF8_FOUR_BYTE)
    {
        return 4;
    }
    else if (byChar>>2 == UTF8_FIVE_BYTE)
    {
        return 5;
    }
    else if (byChar>>1 == UTF8_SIX_BYTE)
    {
        return 6;
    }
    else
    {
        return 0;
    }
}

WORM_DLL_EXPORT char* ToUtf8(const char* pSrc, const uint32& dwSrcLen, char* pDst)
{
    char buf[4];
    uint8 byChar = 0;

    uint8 byTmp = 0;
    int i = 0;
    int j = 0; 
    for (; i!=dwSrcLen; )
    {
        byTmp = IsUtf8(&pSrc[i]);
        if (0 != byTmp)
        {
            memcpy(&pDst[j], &pSrc[i], byTmp);
            i += byTmp;
            j += byTmp;
            continue;
        }
        byChar = pSrc[i];
        /// 是gbk的就转成utf8
        if (byChar>=0x81 && byChar<=0xfe)
        {
            buf[0] = pSrc[i];
            byChar = pSrc[i+1];
            if (byChar>=0x40 && byChar<=0xfe)
            {

                buf[1] = byChar;
                buf[2] = 0;

                StrConv("GBK", "UTF-8", buf, 2, &pDst[j], 4);
                j += 3;
                i += 2;
                continue;
            }
        }

        // 其他直接复制就可以
        pDst[j] = pSrc[i];
        ++j;
        ++i;
    }

    pDst[j] = '\0';

    return pDst;
}

WORM_DLL_EXPORT char* ToGBK(const char* pSrc, const uint32& dwSrcLen, char* pDst)
{
    uint8 byChar = 0;
    int j = 0;
    int i =0;
    for (; i!=dwSrcLen; )
    {
        /// 英文字符直接复制
        if (pSrc[i] > 0)
        {
            pDst[j] = pSrc[i];
            ++j;
            ++i;
            continue;
        }

        /// 判断uft8
        byChar = pSrc[i];
        if (byChar>>5 == UTF8_TWO_BYTE)
        {
            /// 是GBK就跳过
            if (byChar >= 0x81 && byChar <= 0xfe)
            {
                byChar = pSrc[i+1];
                if (byChar>=0x40 && byChar<=0xfe)
                {
                    pDst[j] = pSrc[i];
                    ++i;
                    ++j;
                    pDst[j] = pSrc[i];
                    ++i;
                    ++j;
                    continue;
                }
            }

            StrConv("UTF-8", "GBK", &pSrc[i], 2, &pDst[j], 3);
            j += 2;
            continue;
        }
        else if (byChar>>4 == UTF8_THREE_BYTE)
        {
            StrConv("UTF-8", "GBK", &pSrc[i], 3, &pDst[j], 3);
            j += 2;
            i += 3;
        }
        else if (byChar>>3 == UTF8_FOUR_BYTE)
        {

            StrConv("UTF-8", "GBK", &pSrc[i], 4, &pDst[j], 3);
            j += 2;
            i += 4;
        }
        else if (byChar>>2 == UTF8_FIVE_BYTE)
        {

            StrConv("UTF-8", "GBK", &pSrc[i], 5, &pDst[j], 3);
            j += 2;
            i += 5;
        }
        else if (byChar>>1 == UTF8_SIX_BYTE)
        {
            StrConv("UTF-8", "GBK", &pSrc[i], 6, &pDst[j], 3);
            j += 2;
            i += 6;
        }
        else
        {
            pDst[j] = pSrc[i];
            ++i;
            ++j;
            pDst[j] = pSrc[i];
            ++i;
            ++j;
        }
    }
    pDst[j] = '\0';

    return pDst;
}
*/
