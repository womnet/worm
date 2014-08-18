
#ifdef WIN32
#include <Windows.h>
#undef ERROR
#else
#include <string.h>
#endif //
#include <stdio.h>
#include <assert.h>

#include "wormnet/wm_time.h"

WORM_DLL_EXPORT time_t Str2Time(const char* szTime)
{
	assert(NULL != szTime);

	struct tm sTm;
	sscanf(szTime, "%4d-%2d-%2d %2d:%2d:%2d", 
		&sTm.tm_year, &sTm.tm_mon, &sTm.tm_mday, 
		&sTm.tm_hour, &sTm.tm_min, &sTm.tm_sec);

	sTm.tm_year -= 1900;
	--sTm.tm_mon;
	sTm.tm_isdst=-1;

	return mktime(&sTm);
}

//获取当前时间, 毫秒
WORM_DLL_EXPORT uint32 getMSTime(void)
{
#ifdef WIN32
	return GetTickCount();
#else
	struct timeval	tv;
	struct timezone	tz;
	gettimeofday(&tv, &tz);

	return (tv.tv_sec*1000+tv.tv_usec/1000);
#endif 
}

WORM_DLL_EXPORT uint32 getMSTimeDiff(uint32 oldMSTime, uint32 newMSTime)
{
	if (oldMSTime > newMSTime)
	{
		return (0xFFFFFFFF - oldMSTime) + newMSTime;
	}
	else
	{
		return newMSTime - oldMSTime;
	}
}

WORM_DLL_EXPORT uint32 GetMSTimeDiffToNow(uint32 oldMSTime)
{
    return getMSTimeDiff(oldMSTime, getMSTime());
}

WORM_DLL_EXPORT time_t IsSameDay(const time_t& timeSrc, const time_t& timeDst)
{
    time_t timeRet = 0;

    if (timeSrc > timeDst)
    {
        timeRet = timeSrc  / ONE_DAY_SECOND;
        timeRet -= (timeDst/ONE_DAY_SECOND);
    }
    else
    {
        timeRet = timeDst  / ONE_DAY_SECOND;
        timeRet -= (timeSrc/ONE_DAY_SECOND);
    }

    return timeRet;
}

WORM_DLL_EXPORT char* GetTimestamp(void)
{
    static char szCurTime[32] = "\0";
    time_t timeCur;
    time(&timeCur);

    tm* pTm = localtime(&timeCur);
    //       YYYY   year
    //       MM     month (2 digits 01-12)
    //       DD     day (2 digits 01-31)
    //       HH     hour (2 digits 00-23)
    //       MM     minutes (2 digits 00-59)
    //       SS     seconds (2 digits 00-59)
    sprintf(szCurTime, "%04d-%02d-%02d %02d:%02d:%02d", \
        pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, \
        pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

    return szCurTime;
}

WORM_DLL_EXPORT char* GetToday(void)
{
    static char szCurTime[32] = "\0";
    time_t timeCur;
    time(&timeCur);

    tm* pTm = localtime(&timeCur);
    sprintf(szCurTime, "%04d-%02d-%02d", \
        pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday);

    return szCurTime;
}

WORM_DLL_EXPORT void GetCurTime(uint32& dwCur)
{
    time_t timeCur;
    time(&timeCur);

    dwCur = timeCur & 0xffffffff;
}
