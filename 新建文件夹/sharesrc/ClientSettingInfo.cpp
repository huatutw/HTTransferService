// ClientSettingInfo.cpp: implementation of the CClientSettingInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClientSettingInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientSettingInfo::CClientSettingInfo()
{
	memset(m_strLastDomain,0,DG_MAX_TEMP);
	m_dwDomainCount = 0;
}

CClientSettingInfo::~CClientSettingInfo()
{
	int size = m_listDomain.size();

	int i = 0;
	for (i=0;i<size;i++)
	{
		if (m_listDomain[i])
		{
			delete m_listDomain[i];
		}
	}
	m_listDomain.clear();
}

BOOL CClientSettingInfo::ReadFromIniFile()
{
	TCHAR strIniFile[DG_MAX_PATH] = {0};
	CCommonFunc::GetTempFilePath(_T("DGClientInfo.ini"),strIniFile);

	TCHAR strNumber[DG_MAX_NUMBER] = {0};
	m_dwDomainCount = GetPrivateProfileInt(_T("Domain"),_T("DomainCount"),0,strIniFile);

	GetPrivateProfileString(_T("Domain"),_T("LastDomain"),_T(""),m_strLastDomain,DG_MAX_TEMP,strIniFile);
	int i=0;
	for (i=0;i<m_dwDomainCount;i++)
	{
		TCHAR strDomainKey[DG_MAX_TEMP] = {0};
		_stprintf(strDomainKey,_T("DomainName%d"),i+1);
		TCHAR strDomainName[DG_MAX_TEMP] = {0};
		DWORD dwSize = DG_MAX_TEMP;

		GetPrivateProfileString(_T("Domain"),strDomainKey,_T(""),strDomainName,dwSize,strIniFile);
		
		m_listDomain.push_back(CCommonFunc::BackupString(strDomainName));
	}
	
	return TRUE;
}

BOOL CClientSettingInfo::WriteToIniFile()
{
	TCHAR strIniFile[DG_MAX_PATH] = {0};
	CCommonFunc::GetTempFilePath(_T("DGClientInfo.ini"),strIniFile);

	TCHAR strNumber[DG_MAX_NUMBER] = {0};
	WritePrivateProfileString(_T("Domain"),_T("DomainCount"),CCommonFunc::Int2Str(m_dwDomainCount,strNumber),strIniFile);

	WritePrivateProfileString(_T("Domain"),_T("LastDomain"),m_strLastDomain,strIniFile);

	int i=0;
	for (i=0;i<m_dwDomainCount;i++)
	{
		TCHAR strDomainKey[DG_MAX_TEMP] = {0};
		_stprintf(strDomainKey,_T("DomainName%d"),i+1);
		WritePrivateProfileString(_T("Domain"),strDomainKey,m_listDomain[i],strIniFile);	
	}
	return TRUE;
}

void CClientSettingInfo::AddOneDomainList(LPCTSTR strDomainName)
{
	int size = m_listDomain.size();

	int i = 0;
	for (i=0;i<size;i++)
	{
		if (_tcsicmp(m_listDomain[i],strDomainName) == 0)
		{
			return;
		}
	}

	m_listDomain.push_back(CCommonFunc::BackupString(strDomainName));

	m_dwDomainCount = m_listDomain.size();
}