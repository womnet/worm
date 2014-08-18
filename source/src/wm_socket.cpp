
#include <assert.h>
#include "wm_socket.h"
#include "wormnet/ISocketHander.h"
#include "wormnet/wm_string.h"

namespace worm
{

wmSocket::wmSocket()
{
    m_dwIndex = 0xffff;
    m_qwGUID -= m_qwGUID;
    m_pSocket = NULL;
    m_pSocketHandler = NULL;
    m_bClose = true;
    memset(m_szRemoteIp, 0, sizeof(m_szRemoteIp));
    m_wRemoteProt -= m_wRemoteProt;
}

wmSocket::~wmSocket()
{
}

void wmSocket::SetRemoteIp(const char* ip)
{
    strlcpy(m_szRemoteIp, ip, sizeof(m_szRemoteIp));
}

}
