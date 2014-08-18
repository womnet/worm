
/***********************************************************
Copyright (c) 2011, 妙妙岛工作室

 为了linux兼容编写的一些字符串处理函数

 文件历史:
 修改者: 	    IDK
*************************************************************/

#ifndef _WORMNET_STRING_H
#define _WORMNET_STRING_H


#include "wormnet/wm_std.h"
#include "wormnet/wm_types.h"
#include <string.h>

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
extern size_t strlcpy(char *dst, const char *src, size_t siz);

#ifndef WIN32
//
// 函数名   : strupr
// 功能     : 将字符串中的全部字母转换为大写
// 参数     : 
//          [in] src: 原字符串, 转换后会内容会改变
// 返回值   : src
//
extern char* strupr(char* src);
//
// 函数名   : strupr
// 功能     : 将字符串中的全部字母转换为小写
// 参数     : 
//          [in] src: 原字符串, 转换后会内容会改变
// 返回值   : src
//
extern char* strlwr(char* src);
#endif //

/* 2014-2-18 需要iconv
extern void StrConv(const char* pFromCode, const char* pToCode, const char* pSrc, size_t dwSrcSize, char* pDst, size_t dwBufSize);
extern char* ToUtf8(const char* pSrc, const uint32& dwSrcLen, char* pDst);
extern char* ToGBK(const char* pSrc, const uint32& dwSrcLen, char* pDst);
*/

#endif // ifndef _WORMNET_STRING_H
