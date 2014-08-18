
#ifndef _WORMNET_SERVER_INTERFACE_SERVER_MODULE_H
#define _WORMNET_SERVER_INTERFACE_SERVER_MODULE_H

#include "wormnet/wm_types.h"

namespace worm
{

WORM_DLL_EXPORT class wmServer* ICreateServerModule();
WORM_DLL_EXPORT void IReleaseServerModule(class wmServer*& pServer);

}

#endif // ifndef _WORMNET_SERVER_INTERFACE_SERVER_MODULE_H
