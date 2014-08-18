
#ifndef _PANGU_FLASHPOLICY_SOCKET_HANDLER_H
#define _PANGU_FLASHPOLICY_SOCKET_HANDLER_H

#include <wormnet/ISocketHander.h>

class CPolicySocketHandler : public worm::ISocketHandler
{
public:
    CPolicySocketHandler();
    ~CPolicySocketHandler();

    void OnAccept();
    void OnClose();
    void OnRead(void* pData, const uint16& wSize);

    const uint64& GetStartTime() { return m_qwStartTime; }

private:
    uint64          m_qwStartTime;
};

#endif // ifndef _PANGU_FLASHPOLICY_SOCKET_HANDLER_H
