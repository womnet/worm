
#include "wormnet/wm_Exception.h"
#include "wormnet/wm_LibeventChannel.h"

namespace worm
{

WORM_DLL_EXPORT class wmChannel* ICreateChannelModule(wmConnectedCB pConnectedCB, 
        wmReceiveCB pReceiveCB, wmDisconnectCB pDisconnectCB)
{
    wmChannel* pChannel = new wmLibeventChannel(pConnectedCB, pReceiveCB, pDisconnectCB);
    return pChannel;
}

WORM_DLL_EXPORT void IReleaseChannelModule(class wmChannel*& pChannel)
{
    delete pChannel;
    pChannel = NULL;
}

}
