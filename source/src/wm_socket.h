
#ifndef _WORMNET_SERVER_SOCKET_H
#define _WORMNET_SERVER_SOCKET_H

#include "wormnet/wm_std.h"

namespace worm
{

class WORM_DLL_EXPORT wmSocket
{
    friend class wmLibeventServer;
    friend class wmRaknetServer;
public:
    wmSocket();
    virtual ~wmSocket();

    void SetIndex(const uint32& dwIndex) { m_dwIndex = dwIndex; }
    const uint32& GetIndex() const { return m_dwIndex; }
    const uint64& GetGUID() { return m_qwGUID; };
    void SetGUID(const uint64& qwGuid) { m_qwGUID = qwGuid; }
    /// 
    void SetRemoteIp(const char* ip);
    const char* GetRemoteIp() const { return m_szRemoteIp; }
    /// 连接使用的端口
    void SetRemotePort(const uint16& wPort) { m_wRemoteProt = wPort; }
    uint16 GetRemotePort() const { return m_wRemoteProt; }
    /// 设置关闭状态
    void SetClose() { m_bClose = true; }
    const bool& IsClose() const { return m_bClose; }

    void SetSocket(void* p) { m_pSocket = p; }
    const void* GetSocket() const { return m_pSocket; }

protected:
    uint32                  m_dwIndex;
    // 连接的唯一id
    uint64                  m_qwGUID;
    /// 根据各接口的实际情况设置
    void*                   m_pSocket;
    /// 事件处理接口
    class ISocketHandler*   m_pSocketHandler;
    // 连接使用的ip和端口
    uint16                  m_wRemoteProt;
    char                    m_szRemoteIp[129];
    /// 连接是否已经关闭
    bool                    m_bClose;
};

}

#endif // ifndef _WORMNET_SERVER_SOCKET_H
