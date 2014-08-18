
#include <stdarg.h>
#include <assert.h>
#include "wormnet/ILogs.h"

namespace worm
{

WORM_DLL_EXPORT void InitLogs(const char* pLogName, const bool& bToStderr)
{
    assert(NULL != pLogName);
    google::InitGoogleLogging(pLogName);
    if (bToStderr)
    {
        google::LogToStderr();
    }
}

WORM_DLL_EXPORT void SetLogDestFileName(const int& nLvl, const char* pFileName)
{
    assert(NULL != pFileName);
    assert(nLvl < google::NUM_SEVERITIES);

    google::SetLogDestination(nLvl, pFileName);
}

}
