
/***********************************************************
Copyright (c) 2011, 妙妙岛工作室

 为了linux兼容编写的一些时间处理函数

 文件历史:
 修改者: 	    IDK
*************************************************************/

#ifndef _SRAK_SHARE_TIME_H_
#define _SRAK_SHARE_TIME_H_

#include "wormnet/wm_types.h"
#include <time.h>
#ifndef WIN32
#include <sys/time.h>
#endif //

/// 24小时转换为秒
#define ONE_DAY_SECOND            (86400)

//
// 函数名   : Str2Time
// 功能     : 将字符串格式的的日期时间, 转换为 srak_tiime_t
// 参数     : 
//          [in] szTime: 字符串格式的日期时间
// 返回值   : time_t格式的日期时间
//
WORM_DLL_EXPORT time_t Str2Time(const char* szTime);
//
// 函数名   : getMSTime
// 功能     : 获取开机到现在过去的时间. 同timeGetTime, 但在xnix下没有
// 返回值   : 毫秒
//
WORM_DLL_EXPORT uint32 getMSTime(void);
//
// 函数名   : getMSTimeDiff
// 功能     : 获取两个时间的差
// 参数     : 
//          [in] nOldMSTime: 旧时间值
//          [in] nNewMSTime: 新时间值
// 返回值   : 毫秒差值
//
WORM_DLL_EXPORT uint32 getMSTimeDiff(uint32 nOldMSTime, uint32 nNewMSTime);
//
// 函数名   : GetMSTimeDiffToNow
// 功能     : 获取当前时间与指定时间的差
// 参数     : 
//          [in] oldMSTime: 旧时间值
// 返回值   : 毫秒差值
//
WORM_DLL_EXPORT uint32 GetMSTimeDiffToNow(uint32 oldMSTime);
///   
/// 判断指定的两个时间是否是同一天
/// @param timeSrc 第一个时间
/// @param timeDst 第二个时间
/// @return 为0时表示为同一天, 否则返回相差的天数
/// 
WORM_DLL_EXPORT time_t IsSameDay(const time_t& timeSrc, const time_t& timeDst);
///   
/// 获取一个精确到秒的时间戳字符串
/// @return 当前时间戳
/// 
WORM_DLL_EXPORT char* GetTimestamp(void);
///   
/// 获取一个精确到天的时间戳字符串
/// @return 当前时间戳
/// 
WORM_DLL_EXPORT char* GetToday(void);
///  
/// 获取当前时间, 使用32位数值获取
/// @param dwCur 保存生成的时间
/// @return 无
/// @note 为了与windows兼容的函数
/// 
WORM_DLL_EXPORT void GetCurTime(uint32& dwCur);

#endif // ifndef _SRAK_SHARE_TIME_H_
