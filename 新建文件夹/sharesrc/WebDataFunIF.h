// WebDataFunIF.h: interface for the CWebDataFunIF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBDATAFUNIF_H__189D482A_1E15_48AD_ABE9_495ADFAFDA2B__INCLUDED_)
#define AFX_WEBDATAFUNIF_H__189D482A_1E15_48AD_ABE9_495ADFAFDA2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SocketBase.h"
#include "io.h"
#include "adbclib.h"

class CWebDataFunIF  
{
public:
	CWebDataFunIF();
	virtual ~CWebDataFunIF();

public:
	//static CAConnect m_AConnect;
	static CAString	m_strServerName;
	static CAString	m_strDatabaseName;
	static CAString m_strLoginName;
	static CAString m_strPassword;
	static CAString	m_strConnect;

	// 中间函数
	static BOOL GetFilePath(CAString &strFolderPath);
	// 连接字符串
	static BOOL GetConnectString();	// 连接数据库，应用此类首先调用

	static int DBQuery(ADODB::_ConnectionPtr& ptrConnection,ADODB::_RecordsetPtr& ptrRecordset,LPCTSTR strQuerySQL);
	static DWORD GetLoginInfoByClientID(ADODB::_ConnectionPtr& ptrConnection, CAString strClientID, CAString& strLoginName, CAString& strUserName);

};

#endif // !defined(AFX_WEBDATAFUNIF_H__189D482A_1E15_48AD_ABE9_495ADFAFDA2B__INCLUDED_)
