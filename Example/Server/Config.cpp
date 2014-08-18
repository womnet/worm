
#include "Server/Config.h"
#include <wormnet/wm_std.h>
#include <wormnet/TidyXML.h>
#include <wormnet/ILogs.h>

//�����ļ���, ������Ŀ¼
#define SREVER_CONFIG_FILENAME	"Server.xml"


CPanguConfig::CPanguConfig()
{
    m_wMaxPlayer = 500;

    char szName[256];
    memset(szName, 0, sizeof(szName));
    GetModuleFileName(NULL, szName, sizeof(szName));
    //ȥ����ǰ�Ŀ�ִ���ļ�����
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
    /// ��־��ʼ��
    worm::InitLogs("example_log", true);

    worm::CTidyXML oXml;
    if (!oXml.LoadFile(m_strConFileName.c_str()))
    {
        LOG(ERROR) << "XML �����ļ�: [" << m_strConFileName.c_str() << "] ʧ��" ;
        return false;
    }
    // ��־��ɫ
    std::string strLogColor;
    if (!oXml.AsString("LogColors", strLogColor))
    {
        LOG(ERROR) << "��־��ӡ����δ�ҵ�";
        return false;
    }

    // ��Ϸ����id
    if (!oXml.AsUint16("RealmId", m_wRealmId))
    {
        LOG(ERROR) << "δ�ҵ���Ϸ����id: RealmId";
        return false;
    }

    // �������汾
    if (!oXml.AsUint32("Version", m_dwBuild))
    {
        LOG(ERROR) << "δ�ҵ� Version ����";
        return false;
    }

    // ʹ��ip
    if (!oXml.AsString("ServerIp", m_strIp))
    {
        LOG(ERROR) << "δ�ҵ�������ip: ServerIp";
        return false;
    }
    // ʹ�ö˿�
    if (!oXml.AsUint16("ServerPort", m_wPort))
    {
        LOG(ERROR) << "δ�ҵ��������˿�: ServerPort";
        return false;
    }
    // �����������
    if (!oXml.AsUint16("max_player", m_wMaxPlayer))
    {
        LOG(ERROR) << "δ���������������: max_player";
        return false;
    }
    LOG(INFO) << "���������������: " << m_wMaxPlayer;

    /// ��־Ŀ¼
    std::string strLogDir;
    oXml.AsString("LogsDir", strLogDir);

    return true;
}
