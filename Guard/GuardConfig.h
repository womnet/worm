
#ifndef _WORMNET_GUARD_CONFIG_H
#define _WORMNET_GUARD_CONFIG_H

#include <wormnet/wm_types.h>
#include <string>

class CGuardConfig
{
public:
    ~CGuardConfig();
    static CGuardConfig& Instance();

    bool LoadConf();

private:
    CGuardConfig();

private:
    /// 服务器根目录
    std::string             m_strRoot;
    // 配置文件名称, 全路径
    std::string             m_strConFileName;

public:
    /// 检测时间间隔
    uint32                  m_dwSecond;
    /// 脚本地址
    std::string             m_strGuard;
};

#define g_GuardConf       (CGuardConfig::Instance())

#endif // ifndef _WORMNET_GUARD_CONFIG_H
