
#ifndef _LEGEND_SERVER_CONFIG_H
#define _LEGEND_SERVER_CONFIG_H

#include <wormnet/wm_types.h>
#include <string>

class CPanguConfig
{
public:
    ~CPanguConfig();
    static CPanguConfig* Instance();

    bool LoadConf();

private:
    CPanguConfig();

public:
    // 使用的ip和端口
    std::string				m_strIp;
    uint16					m_wPort;
    // 最大管理的玩家数量
    uint16					m_wMaxPlayer;

    /// 服务器根目录
    std::string             m_strRoot;
    /// 配置文件目录
    std::string             m_strEtcDir;
    // 配置文件名称, 全路径
    std::string				m_strConFileName;
    // 数据库配置
    /*std::string             m_strLogsDBConf;
    std::string				m_strAuthDBConf;
    std::string             m_strCharactersDBConf;
    std::string             m_strWorldDBConf;*/
    // 服务器当前版本
    uint32                  m_dwBuild;
    // 服务器所属的分区
    uint16                  m_wRealmId;
    // 默认的同时进行的活动数量
    uint8                   m_byTokenNum;
    /// 登录允许连续出错的次数, 超出后锁定
    uint8                   m_byWrongPassMaxCount;
};

#define g_pPanguConf        (CPanguConfig::Instance())

#endif // ifndef _LEGEND_SERVER_CONFIG_H
