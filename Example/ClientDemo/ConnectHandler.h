
#ifndef _CLIENTDEMO_CONNECT_HANDLE_H
#define _CLIENTDEMO_CONNECT_HANDLE_H

#include <wormnet/wm_types.h>
#include <wormnet/ILogs.h>
#include <wormnet/wm_Channel.h>

#define CRPY_KEY "[/28!~&0sM1-)G&z"

class CConnectHandler
{
public:
    ~CConnectHandler();

    static CConnectHandler* Instance();

    void Open();
    void Run();

    static void ConnectionAccepted(const bool& bIsSuccess);
    static void ReceiveData(char* pData, unsigned int& dwSize);
    static void Disconnect();

private:
    CConnectHandler();

private:
    worm::wmChannel*            m_pChannel;
};

#endif