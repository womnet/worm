
#ifndef _WORMNET_SERVER_INTERFACE_SOCKHET_HANDERL_H
#define _WORMNET_SERVER_INTERFACE_SOCKHET_HANDERL_H

#include "wormnet/wm_types.h"

namespace worm
{

class WORM_DLL_EXPORT ISocketHandler
{
public:
    ISocketHandler();
    virtual ~ISocketHandler();

    virtual void OnAccept(void) = 0;
    virtual void OnClose(void) = 0;
    ///   
    /// 处理收到的数据
    /// @param pData 数据
    /// @param wSize 数据长度
    /// @return 无
    /// @see
    /// @note
    /// 
    virtual void OnRead(void* pData, const uint16& wSize) = 0;
    ///   
    /// 设置对应的wmSocket指针
    /// @param pSocket socket值
    /// @note pSocket不能为NULL
    /// 
    void SetSocket(class wmSocket* pSocket) { m_pSocket = pSocket; }
    ///   
    /// 获取wmSocket指针
    /// @return socket指针
    /// 
    class wmSocket* Socket() const { return m_pSocket; }
    ///   
    /// 判断连接是否已经关闭
    /// @return true 为已经关闭
    /// 
    const bool& IsClose();
    ///   
    /// 获取连接使用的ip地址
    /// @return ip地址的字符串
    /// 
    const char* GetRemoteIp() const;
    ///   
    /// 获取连接使用的端口
    /// @return 端口
    /// 
    uint16 GetRemotePort() const;
    const uint64& GetGUID() const;

private:
    class wmSocket*         m_pSocket;
};

}

#endif // ifdef _WORMNET_SERVER_INTERFACE_SOCKHET_HANDERL_H
