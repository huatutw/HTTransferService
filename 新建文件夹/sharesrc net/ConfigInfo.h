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

	CSoftwareTypeInfo* FindSoftTypeByName(LPCTSTR lpszSoftType);

//	BOOL LoadConfigInfoFromFile(LPCTSTR lpszFileName);
//	void SaveConfigInfoToFile(LPCTSTR lpszFileName);
//	virtual void Serialize(CArchive& ar);

	CSoftwareVersionInfo* m_pCurSoftware; //当前符合配置的应用程序

	BOOL CheckModule(LPCTSTR lpszModuleName);

	BOOL AddOneMail(LPCTSTR lpszMail,int bControl);

	BOOL ClearMailSetting();
	LPTSTR GetExtraCCMailAddress();
	void GetSafeMailAddress(/*CStringList& maillist*/deque<LPTSTR> *pMailList);	   //得到接收方安全邮箱
	void GetSafeRevMailAddress(/*CStringList& maillist*/deque<LPTSTR> *pMailList); //得到发送方安全邮箱
public:
	int m_clsVersion;
public:
	BOOL DeleteItem(CSoftwareTypeInfo* pInfo);

	BOOL SetDataToBuffer(LPBYTE* btData, DWORD& dwSize, BOOL bUnicodeToAnsi);
};

extern CConfigInfo* g_pConfigInfo;

#endif // !defined(AFX_CONFIGINFO_H__5D629FC6_B756_4C82_AB7C_8B74D0C39372__INCLUDED_)
