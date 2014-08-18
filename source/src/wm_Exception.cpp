
#include "wormnet/wm_std.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "wormnet/wm_Exception.h"
#include "wormnet/ILogs.h"

namespace worm
{

/// 异常日志的缓存默认大小
#define EXCEPTION_LOG_SIZE      (4096)

wmException::wmException(const char *pStr, ...)
{
    if (NULL == pStr)
    {
        assert(false);
        return;
    }

    char* pLog = new char[EXCEPTION_LOG_SIZE];

    va_list ap;
    va_start(ap, pStr);
    vsnprintf(pLog, EXCEPTION_LOG_SIZE, pStr, ap);
    va_end(ap);
    LOG(ERROR) << pLog;

    delete []pLog;
    pLog = NULL;
}

wmException::~wmException()
{
}

}
