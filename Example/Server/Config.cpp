
#include "Server/Config.h"
#include <wormnet/wm_std.h>
#include <wormnet/TidyXML.h>
#include <wormnet/ILogs.h>

//配置文件名, 不包含目录
#define SREVER_CONFIG_FILENAME	"Server.xml"


CPanguConfig::CPanguConfig()
{
    m_wMaxPlayer = 500;

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
    m_strEtcDir = szName;

    strcat(szName, SREVER_CONFIG_FILENAME);
    m_strConFileName = szName;
    m_dwBuild -= m_dwBuild;
}

CPanguConfig::~CPanguConfig()
{

}

CPanguConfig* CPanguConfig::Instance()
{
    static CPanguConfig oConf;

    return &oConf;
}

bool CPanguConfig::LoadConf()
{
    /// 日志初始化
    worm::InitLogs("example_log", true);

    worm::CTidyXML oXml;
    if (!oXml.LoadFile(m_strConFileName.c_str()))
    {
        LOG(ERROR) << "XML 加载文件: [" << m_strConFileName.c_str() << "] 失败" ;
        return false;
    }
    // 日志颜色
    std::string strLogColor;
    if (!oXml.AsString("LogColors", strLogColor))
    {
        LOG(ERROR) << "日志打印参数未找到";
        return false;
    }

    // 游戏大区id
    if (!oXml.AsUint16("RealmId", m_wRealmId))
    {
        LOG(ERROR) << "未找到游戏大区id: RealmId";
        return false;
    }

    // 服务器版本
    if (!oXml.AsUint32("Version", m_dwBuild))
    {
        LOG(ERROR) << "未找到 Version 配置";
        return false;
    }

    // 使用ip
    if (!oXml.AsString("ServerIp", m_strIp))
    {
        LOG(ERROR) << "未找到服务器ip: ServerIp";
        return false;
    }
    // 使用端口
    if (!oXml.AsUint16("ServerPort", m_wPort))
    {
        LOG(ERROR) << "未找到服务器端口: ServerPort";
        return false;
    }
    // 最大在线人数
    if (!oXml.AsUint16("max_player", m_wMaxPlayer))
    {
        LOG(ERROR) << "未设置最大在线人数: max_player";
        return false;
    }
    LOG(INFO) << "设置最大在线人数: " << m_wMaxPlayer;

    /// 日志目录
    std::string strLogDir;
    oXml.AsString("LogsDir", strLogDir);

    return true;
}
