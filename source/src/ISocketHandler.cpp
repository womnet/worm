
#include "wormnet/wm_cryp.h"
#include "wormnet/ISocketHander.h"
#include "wm_socket.h"

namespace worm
{

ISocketHandler::ISocketHandler()
{
}

ISocketHandler::~ISocketHandler()
{
}

const bool& ISocketHandler::IsClose()
{
    return m_pSocket->IsClose();
}

const char* ISocketHandler::GetRemoteIp() const
{
    return m_pSocket->GetRemoteIp();
}

uint16 ISocketHandler::GetRemotePort() const
{
    return m_pSocket->GetRemotePort();
}

const uint64& ISocketHandler::GetGUID() const
{
    return m_pSocket->GetGUID();
}

}
