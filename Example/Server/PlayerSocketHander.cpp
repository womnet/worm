
#include <wormnet/wm_Exception.h>
#include "Server/Config.h"
#include "Server/PlayerSocketHander.h"


CPlayerSocketHandler::CPlayerSocketHandler()
{
}

CPlayerSocketHandler::~CPlayerSocketHandler()
{
}

void CPlayerSocketHandler::Send(worm::CByteBuffer& oPacket)
{
    g_pApp->GetServer()->Send(oPacket, Socket());
}

void CPlayerSocketHandler::OnAccept()
{
    worm::CByteBuffer oPacket;
    char szTmp[64] = "connect success";
    oPacket << (uint8)133;
    oPacket.Append(szTmp, strlen(szTmp)+1);
    Send(oPacket);
}

void CPlayerSocketHandler::OnClose()
{
    LOG(INFO) << "OnClose: " <<  this->GetRemoteIp() << ":" << this->GetRemotePort();
}

void CPlayerSocketHandler::OnRead(void* pData, const uint16& wSize)
{
    LOG(INFO) << "OnRead: " << (char*)pData;
}

