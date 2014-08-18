
#include "wormnet/TidyXML.h"
#include "wormnet/ILogs.h"
#include <tinyxml/tinyxml.h>
#include <map>

namespace worm
{

class CXmlNode
{
public:
	CXmlNode();
	~CXmlNode();
	//删除所有的节点信息
	void Release(void);

	//找到指定节点, 并获取值
	bool GetNodeValue(const char* szNodeName, std::string& strValue);
	//加载子节点信息, pNode为当前节点
	void LoadChild(CXmlNode* pNode, class TiXmlElement* pXmlEle);

private:
	//节点名称
	std::string							m_strNodeName;
	//值
	std::string							m_strValue;
	//子节点
	std::map<std::string, CXmlNode*>	m_mapChild;
};

CXmlNode::CXmlNode()
{
}

CXmlNode::~CXmlNode()
{
	Release();
}

void CXmlNode::Release(void)
{
	std::map<std::string, CXmlNode*>::iterator iterNode = m_mapChild.begin();
	for (; m_mapChild.end()!=iterNode; ++iterNode)
	{
		if (NULL != iterNode->second)
		{
			iterNode->second->Release();
		}
		delete iterNode->second;
	}
	m_mapChild.clear();
}

void CXmlNode::LoadChild(CXmlNode* pNode, class TiXmlElement* pXmlEle)
{
	CXmlNode* pNewNode = NULL;
	TiXmlElement* pXmlChild = NULL;

	for (; NULL!=pXmlEle; )
	{
		pNewNode = new CXmlNode();
		pNewNode->m_strNodeName = pXmlEle->Value();
		pXmlChild = pXmlEle->FirstChildElement();
		if (NULL != pXmlChild)
		{
			pNewNode->LoadChild(pNewNode, pXmlChild);
		}
		const char* pText = pXmlEle->GetText();
		if (NULL != pText)
		{
			pNewNode->m_strValue = pText;
		}
		pNode->m_mapChild[pNewNode->m_strNodeName] = pNewNode;

		pXmlEle = pXmlEle->NextSiblingElement();
	}
}

bool CXmlNode::GetNodeValue(const char* szNodeName, std::string& strValue)
{
	assert(NULL != szNodeName);
	if (strlen(szNodeName) == 0)
	{
        LOG(ERROR) << "CXmlNode::GetNodeValue szNode is empty";
		return false;
	}
	else if (strlen(szNodeName) > 255)
	{
        LOG(ERROR) << "CXmlNode::GetNodeValue szNode is too length! [%s]" << szNodeName;
	}

	char szNode[256] = "\0";
	strcpy(szNode, szNodeName);


	char* pTmp = strtok(szNode, ":");
	CXmlNode* pNode = this;
	std::map<std::string, CXmlNode*>::iterator iterNode;

	for (;;)
	{
		iterNode = pNode->m_mapChild.find(pTmp);
		if (pNode->m_mapChild.end() == iterNode)
		{
			return false;
		}
		pTmp = strtok(NULL, ":");
		if (NULL != pTmp)
		{
			pNode = iterNode->second;
		}
		else
		{
			break;
		}
	}

	strValue = iterNode->second->m_strValue;

	return true;
}

CTidyXML::CTidyXML()
{
	m_pRootNode = new CXmlNode();
}

CTidyXML::~CTidyXML()
{
	delete m_pRootNode;
	m_pRootNode = NULL;
}


bool CTidyXML::LoadFile(const char* szFilename)
{
	//先释放上次保存的
	m_pRootNode->Release();

	TiXmlDocument docXml;
	if (!docXml.LoadFile(szFilename, TIXML_ENCODING_LEGACY))
	{
		return false;
	}

	TiXmlElement* rootElement = docXml.RootElement();
	if (NULL == rootElement)
	{
        LOG(ERROR) << "CTidyXML::LoadFile(" << szFilename << ") 错误";
		return false;
	}

	TiXmlElement* pXmlEle = rootElement->FirstChildElement();
	m_pRootNode->LoadChild(m_pRootNode, pXmlEle);

	return true;
}

bool CTidyXML::AsInt8(char* szNode, char& value)
{
	std::string strValue;
	if (!m_pRootNode->GetNodeValue(szNode, strValue))
	{
		return false;
	}

	value = atoi(strValue.c_str()) & 0xff;

	return true;
}

bool CTidyXML::AsUint8(char* szNode, uint8& value)
{
	std::string strValue;
	if (!m_pRootNode->GetNodeValue(szNode, strValue))
	{
		return false;
	}

	value = atoi(strValue.c_str()) & 0xff;

	return true;
}

bool CTidyXML::AsInt16(char* szNode, short& value)
{
	std::string strValue;
	if (!m_pRootNode->GetNodeValue(szNode, strValue))
	{
		return false;
	}

	value = atoi(strValue.c_str()) & 0xffff;

	return true;
}

bool CTidyXML::AsUint16(char* szNode, uint16& value)
{
	std::string strValue;
	if (!m_pRootNode->GetNodeValue(szNode, strValue))
	{
		return false;
	}

	value = atoi(strValue.c_str()) & 0xffff;

	return true;
}

bool CTidyXML::AsInt32(char* szNode, int& value)
{
	std::string strValue;
	if (!m_pRootNode->GetNodeValue(szNode, strValue))
	{
		return false;
	}

	value = atoi(strValue.c_str());

	return true;
}

bool CTidyXML::AsUint32(char* szNode, uint32& value)
{
	std::string strValue;
	if (!m_pRootNode->GetNodeValue(szNode, strValue))
	{
		return false;
	}

	value = strtoul(strValue.c_str(), NULL, 10);

	return true;
}

bool CTidyXML::AsString(char* szNode, std::string& value)
{
	return m_pRootNode->GetNodeValue(szNode, value);
}

bool CTidyXML::AsFloat(char* szNode, float& value)
{
	std::string strValue;
	if (!m_pRootNode->GetNodeValue(szNode, strValue))
	{
		return false;
	}

	value = (float)atof(strValue.c_str());

	return true;
}

bool CTidyXML::AsUint64(char* szNode, uint64& value)
{
	std::string strValue;
	if (!m_pRootNode->GetNodeValue(szNode, strValue))
	{
		return false;
	}

	sscanf(strValue.c_str(), "%llu", &value);

	return true;
}

bool CTidyXML::AsInt64(char* szNode, int64& value)
{
	std::string strValue;
	if (!m_pRootNode->GetNodeValue(szNode, strValue))
	{
		return false;
	}

	sscanf(strValue.c_str(), "%lld", &value);

	return true;
}

}
