
#include "wormnet/IChannelModule.h"
#include "ClientDemo/ConnectHandler.h"
#include <wormnet/wm_packet.h>

CConnectHandler::CConnectHandler()
{

}

CConnectHandler::~CConnectHandler()
{

}

CConnectHandler* CConnectHandler::Instance()
{
    static CConnectHandler oHandler;
    return &oHandler;
}

void CConnectHandler::ConnectionAccepted(const bool& bIsSuccess)
{
    LOG(INFO) << "ConnectionAccepted: \n";

    worm::wmPacket oPacket;
    oPacket.SetOpcode(1);
    oPacket << "04";
    oPacket << "123456";
    oPacket.Make();
    CConnectHandler::Instance()->m_pChannel->Send(oPacket);
}

void CConnectHandler::ReceiveData(char* pData, unsigned int& dwSize)
{
    LOG(INFO) << "ReceiveData: \n";
    Sleep(100);
    for (uint32 i=0; i!=dwSize; ++i)
    {
        printf("%02x-", (char)pData[i]);
    }

    worm::wmPacket oPacket;
    oPacket.SetOpcode(5);
    oPacket << uint32(106);
    oPacket << uint32(1403625600);
    oPacket.Make();
    CConnectHandler::Instance()->m_pChannel->Send(oPacket);
}

void CConnectHandler::Disconnect()
{
    LOG(INFO) << "Disconnect!\n";
}

void CConnectHandler::Open()
{
    m_pChannel = worm::ICreateChannelModule(&CConnectHandler::ConnectionAccepted, 
        &CConnectHandler::ReceiveData, &CConnectHandler::Disconnect);

    m_pChannel->ConnectServer("192.168.1.108", 8000);
}

void CConnectHandler::Run()
{
    for (;;)
    {
        Sleep(1);
        m_pChannel->EventLoop();
    }
}
