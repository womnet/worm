
#include "wm_LibeventServer.h"
#include "wormnet/wm_std.h"
#include "wormnet/IServerModule.h"
#include "wormnet/wm_Exception.h"

namespace worm
{

WORM_DLL_EXPORT class wmServer* ICreateServerModule()
{
    wmServer* pServer = new wmLibeventServer();
    return pServer;
}

WORM_DLL_EXPORT void IReleaseServerModule(class wmServer*& pServer)
{
    delete pServer;
    pServer = NULL;
}

}
