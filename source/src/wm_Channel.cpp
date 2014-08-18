
#include "wormnet/wm_Channel.h"

namespace worm
{

worm::wmChannel::wmChannel(wmConnectedCB pConnectedCB, wmReceiveCB pReceiveCB, wmDisconnectCB pDisconnectCB)
{
    assert(NULL != pConnectedCB);
    assert(NULL != pReceiveCB);
    assert(NULL != pDisconnectCB);

    m_pConnectedCB = pConnectedCB;
    m_pReceiveCB = pReceiveCB;
    m_pDisconnectCB = pDisconnectCB;
    m_bStopEvent = false;
}

worm::wmChannel::~wmChannel()
{

}

}
