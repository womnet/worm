
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
    // ʹ�õ�ip�Ͷ˿�
    std::string				m_strIp;
    uint16					m_wPort;
    // ��������������
    uint16					m_wMaxPlayer;

    /// ��������Ŀ¼
    std::string             m_strRoot;
    /// �����ļ�Ŀ¼
    std::string             m_strEtcDir;
    // �����ļ�����, ȫ·��
    std::string				m_strConFileName;
    // ���ݿ�����
    /*std::string             m_strLogsDBConf;
    std::string				m_strAuthDBConf;
    std::string             m_strCharactersDBConf;
    std::string             m_strWorldDBConf;*/
    // ��������ǰ�汾
    uint32                  m_dwBuild;
    // �����������ķ���
    uint16                  m_wRealmId;
    // Ĭ�ϵ�ͬʱ���еĻ����
    uint8                   m_byTokenNum;
    /// ��¼������������Ĵ���, ����������
    uint8                   m_byWrongPassMaxCount;
};

#define g_pPanguConf        (CPanguConfig::Instance())

#endif // ifndef _LEGEND_SERVER_CONFIG_H
