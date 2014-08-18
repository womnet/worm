
#ifndef _WORMNET_EXCEPTION_H
#define _WORMNET_EXCEPTION_H

#include <wormnet/wm_types.h>

namespace worm
{

class WORM_DLL_EXPORT wmException
{
public:
    wmException(const char *pStr, ...);
    ~wmException();

private:
    wmException();
};
}

#endif // ifndef _WORMNET_EXCEPTION_H
