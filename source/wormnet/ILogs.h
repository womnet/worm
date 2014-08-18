
#ifndef _WORMNET_LOGS_MODULE_H
#define _WORMNET_LOGS_MODULE_H

#include <wormnet/wm_types.h>
#include <glog/logging.h>

#define WM_GLOG_INFO            0
#define WM_GLOG_WARNING         1
#define WM_GLOG_ERROR           2

namespace worm
{
/// 
/// 初始化日志模块
/// @pararm pLogName 日志名称标识
/// @param bToStderr 日志输出方式, true为到控制台; 否则为文件
/// @return 无
/// 
WORM_DLL_EXPORT void InitLogs(const char* pLogName, const bool& bToStderr);
///  
/// 设置各级别的日志对应的文件目录和文件名前缀
/// @param nLvl 日志等级
/// @param pFileNmaePre 目录和文件名前缀
/// @return 无
/// 
WORM_DLL_EXPORT void SetLogDestFileName(const int& nLvl, const char* pFileName);
}

#endif // ifndef _WORMNET_LOGS_MODULE_H
