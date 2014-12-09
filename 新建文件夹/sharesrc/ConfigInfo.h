// ConfigInfo.h: interface for the CConfigInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGINFO_H__5D629FC6_B756_4C82_AB7C_8B74D0C39372__INCLUDED_)
#define AFX_CONFIGINFO_H__5D629FC6_B756_4C82_AB7C_8B74D0C39372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SoftwareTypeInfo.h"

class CConfigInfo  
{
public:
	CConfigInfo();
	virtual ~CConfigInfo();
	
	SOFTTYPE_DEQUE m_arrSoftType;

	CSoftwareTypeInfo* FindSoftTypeByName(const char* lpszSoftType);


	BOOL LoadConfigInfoFromFile(const char* lpszFileName);
	void SaveConfigInfoToFile(const char* lpszFileName);

	BOOL SaveConfigToXML(const char* lpszFileName);
	BOOL LoadConfigFromXML(const char* lpszFileName);

#ifndef NO_MFC_VERSION
	virtual BOOL Serialize(CArchive& ar);
#else
	virtual BOOL Serialize(HANDLE hFile,BOOL bRead);
#endif

	CSoftwareVersionInfo* m_pCurSoftware; //当前符合配置的应用程序
	CSoftwareTypeInfo* m_pCurSoftType;	  //当前符合配置的应用程序类型

	BOOL CheckModule(const char* lpszModuleName);
	BOOL CheckOleCLSID(CLSID clsid);

	BOOL IsMailAddressCanSend(const char* lpszMail,BOOL bSender);
	BOOL IsPrinterCanUse(const char* lpszPrinter);
	BOOL GetExtraCCMailAddress(char* lpszMail);
public:
	int m_clsVersion;
public:
	BOOL DeleteItem(CSoftwareTypeInfo* pInfo);
};

extern CConfigInfo* g_pConfigInfo;

#endif // !defined(AFX_CONFIGINFO_H__5D629FC6_B756_4C82_AB7C_8B74D0C39372__INCLUDED_)
