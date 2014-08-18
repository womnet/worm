
#include "wormnet/wm_server.h"

namespace worm
{

wmServer::wmServer()
{
    m_bStopEvent = false;
}

wmServer::~wmServer()
{
}

class ISocketHandler* wmServer::GetSockethandler(const uint64& qwGuid)
{
    SHashValue* pValue = m_mapSocketHandler.find(qwGuid);
    if (NULL == pValue)
    {
        return NULL;
    }

    return pValue->m_pValue;
}

}
