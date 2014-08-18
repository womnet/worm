
#include "GuardConfig.h"
#include <wormnet/wm_std.h>
#include <glog/logging.h>
#include <wormnet/TidyXML.h>

//配置文件名, 不包含目录
#define GUARD_CONF  "guard.xml"

CGuardConfig::CGuardConfig()
{
    char szName[256];
    memset(szName, 0, sizeof(szName));
    GetModuleFileName(NULL, szName, sizeof(szName));
    //去掉当前的可执行文件名称
#ifdef WIN32
    strrchr(szName, '\\')[1] = 0;
#else
    strrchr(szName, '/')[1] = 0;
#endif //

    m_strRoot = szName;

    strcat(szName, GUARD_CONF);
    m_strConFileName = szName;
}

CGuardConfig::~CGuardConfig()
{

}

CGuardConfig& CGuardConfig::Instance()
{
    static CGuardConfig oConf;

    return oConf;
}

bool CGuardConfig::LoadConf()
{
    worm::CTidyXML oXml;
    if (!oXml.LoadFile(m_strConFileName.c_str()))
    {
        LOG(ERROR) << "XML 加载文件: [" << m_strConFileName.c_str() << "] 失败";
        return false;
    }
    /// 间隔时间
    if (!oXml.AsUint32("sleep", m_dwSecond))
    {
        LOG(ERROR) << "未设置间隔时间: sleep";
        return false;
    }
    if (0 == m_dwSecond)
    {
        m_dwSecond = 8;
    }
    LOG(INFO) << "间隔时间: " << m_dwSecond;

    if (!oXml.AsString("guard", m_strGuard))
    {
        LOG(ERROR) << "未设置脚本: guard";
        return false;
    }

    return true;
}
