////////////////////////////////////////////////////////////////////// 
///    COPYRIGHT NOTICE 
///    Copyright (c) 2011, 妙妙岛工作室
///    All rights reserved. 
/// 
/// @file TidyXML.h
/// @brief 简化的XML文件解析
/// 
/// 简化XML文件的读取操作, 同时对XML文件的格式支持也少了
/// 
/// @author idk
/// @date 创建日期: 2011年12月4日
//////////////////////////////////////////////////////////////////////

#ifndef _WORMNET_TIDY_XML_H
#define _WORMNET_TIDY_XML_H

#include "wormnet/wm_types.h"
#include <string>

namespace worm
{

class WORM_DLL_EXPORT CTidyXML
{
public:
	CTidyXML();
	~CTidyXML();

	//
    // 函数名   : LoadFile
    // 功能     : 解析xml从一个文件
    // 参数     : 
    //          [in] szFilename: 文件名
    // 返回值   : true 成功; false 失败
    //
	bool LoadFile(const char* szFilename);
	//
    // 函数名   : LoadFile
    // 功能     : 解析xml从一个字符串
    // 参数     : 
    //          [in] string: xml内容
    // 返回值   : true 成功; false 失败
    //
	//bool LoadString(const char* string);
    //
    // 函数名   : AsXXXXX
    // 功能     : 获取一个节点的值, 节点名称可包含一系列子节点, 用 : 隔开. 例: "class:subclass"
    // 参数     : 
    //          [in] szNode: xml内容
    //          [out] value: 读取的值
    // 返回值   : true 成功; false 失败
    //
	bool AsInt8(char* szNode, char& value);
	bool AsUint8(char* szNode, uint8& value);
	bool AsInt16(char* szNode, short& value);
	bool AsUint16(char* szNode, uint16& value);
	bool AsInt32(char* szNode, int& value);
	bool AsUint32(char* szNode, uint32& value);
	bool AsString(char* szNode, std::string& value);
	bool AsFloat(char* szNode, float& value);
	bool AsUint64(char* szNode, uint64& value);
	bool AsInt64(char* szNode, int64& value);

private:
	// 根结点
	class CXmlNode*		m_pRootNode;
};

}

#endif // ifndef _WORMNET_TIDY_XML_H
