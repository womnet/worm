

#ifndef _WORMNET_INTERFACE_CHANNEL_MODULE_H
#define _WORMNET_INTERFACE_CHANNEL_MODULE_H

#include "wormnet/wm_types.h"

typedef void (*wmConnectedCB)(const bool&);
typedef void (*wmReceiveCB)(char*, unsigned int&);
typedef void (*wmDisconnectCB)();

namespace worm
{

WORM_DLL_EXPORT class wmChannel* ICreateChannelModule(wmConnectedCB pConnectedCB, 
        wmReceiveCB pReceiveCB, wmDisconnectCB pDisconnectCB);
WORM_DLL_EXPORT void IReleaseChannelModule(class wmChannel*& pChannel);

}

#endif // ifndef _WORMNET_INTERFACE_CHANNEL_MODULE_H
