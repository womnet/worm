
/***********************************************************
Copyright (c) 2011, 妙妙岛工作室

 为了linux兼容编写的一些标准函数

 文件历史:
 修改者: 	    IDK
*************************************************************/

#ifndef _WORMNET_STD_H
#define _WORMNET_STD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "wormnet/MemoryGuard.h"

#ifdef WIN32
#include <Windows.h>
#undef ERROR
#else
#include <unistd.h>
#endif // ifndef WIN32

#include "wormnet/wm_types.h"

//
// 函数名   : ip2str
// 功能     : 将ipv4格式的二进制ip地址, 转换为字符串格式
// 参数     : 
//          [in] ip: 二进制的ip地址
//          [out] szIp: 转换后的ip字符串
// 返回值   : 转换后的ip字符串
//
extern char* ip2str(const uint32& ip, char* szIp);

#ifndef WIN32
//
// 函数名   : Sleep
// 功能     : 在xnix下sleep毫秒
// 参数     : 
//          [in] n: sleep毫秒
//
#define Sleep(n)	usleep(n*1000);
//
// 函数名   : GetModuleFileName
// 功能     : xnix下获取当前可执行文件的完整路径, 包括文件名
// 参数     : 
//          [in] p: 设置为NULL即可
//          [out] szFileName: 路径
//          [in] nSize: 保存路径的缓存大小
// 返回值   : 0 成功, 否则为失败原因
//
extern uint32 GetModuleFileName(void* p, char* szFileName, int nSize);

#define ZeroMemory(p,size)  memset(p, 0, size)
#endif //

#define finite(X) _finite(X)

#define wm_hton16(value) ( ((value<<8) | (value>>8)) & 0xffff )
#define wm_hton32(value) ( (value<<24) | ((value<<8)&0xff0000) | ((value>>8)&0xff00) | ((value>>24)&0xff) )
#define wm_ntoh16(value) wm_hton16(value)
#define wm_ntoh32(value) wm_hton32(value)

#endif // ifndef _WORMNET_STD_H
