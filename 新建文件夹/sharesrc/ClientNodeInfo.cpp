// ClientNodeInfo.cpp: implementation of the CClientNodeInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "TestHookDll.h"
#include "ClientNodeInfo.h"
//#include "userinfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef BOOL (WINAPI* GETUSERPRIVINFO)(LPCTSTR strUserName,DWORD* pdwPriv,DWORD* pdwOverTime,BOOL bSystemDir);
typedef BOOL (WINAPI* GETUSERPRIVINFO2)(LPCTSTR strUserName,DWORD* pdwPriv,DWORD* pdwOverTime,DWORD* pdwCryptLevel,int* pnPolicyType,INT_DEQUE& PolicyDeque,BOOL bSystemDir);
typedef BOOL (WINAPI REFRESHPRIVINFO)(BOOL bSystemDir);

CClientNodeInfo::CClientNodeInfo()
{
	m_bOverTime = FALSE;
	GetLocalTime(&m_PrevTime);
	m_dwPrevTickCount = GetTickCount();
	m_dwPriv = 0;
	m_strIP = NULL;
	m_dwOverTime = 0;
	m_dwCryptLevel = 0;
	m_nPolicyType = -1;
	m_strMac = NULL;
	m_strComputer = NULL;
	m_strIP = NULL;
	m_strUser = NULL;
	m_strPass = NULL;

	m_bClient = FALSE;
	m_pszVersion = NULL;
}

CClientNodeInfo::~CClientNodeInfo()
{
	if (m_strMac)
		free(m_strMac);

	if (m_strComputer)
		free(m_strComputer);

	if (m_strIP)
		free(m_strIP);

	if (m_strUser)
		free(m_strUser);

	if (m_strPass)
		free(m_strPass);

	if (m_pszVersion)
		free(m_strPass);

	m_strMac = NULL;
	m_strComputer = NULL;

	m_arrPolicy.clear();
}

BOOL CClientNodeInfo::IsOverTime(DWORD dwCurTick)			//ÅÐ¶ÏÊÇ·ñ³¬Ê±
{
	if ((dwCurTick - m_dwPrevTickCount)>OVERTIME*1000)
	{
		m_bOverTime = TRUE;
		return TRUE;
	}else
		m_bOverTime = FALSE;

	return FALSE;
}

DWORD CClientNodeInfo::GetClientPriv(HINSTANCE hBase,LPCTSTR strCurUser,BOOL bSystemDir)
{
	if (hBase)
	{
		GETUSERPRIVINFO GetUserPrivInfo = (GETUSERPRIVINFO)GetProcAddress(hBase,"GetUserPrivInfo");
		GetUserPrivInfo(strCurUser,&m_dwPriv,&m_dwOverTime,bSystemDir);
	}
	return 0;
}

// static BOOL GetSystemFilePath(LPTSTR* path,LPCTSTR lpszFileName)
// {
// 	TCHAR buffer[_MAX_PATH+1];
// 
// 	GetSystemDirectory(buffer,_MAX_PATH);
// 
// 	CString strTmp;
// 	int len = _tcslen(buffer);
// 	if (buffer[len-1] == '\\')
// 	{
// 		strTmp.Format(TEXT("%s%s"),buffer,lpszFileName);	
// 	}else
// 	{
// 		strTmp.Format(TEXT("%s\\%s"),buffer,lpszFileName);
// 	}
// 	*path = (LPTSTR)calloc(strTmp.GetLength()+1,sizeof(TCHAR));
// 	_tcscpy(*path,strTmp);
// 	return TRUE;
// }


BOOL WINAPI GetUserPrivInfo2(LPCTSTR strUserName,DWORD* pdwPriv,DWORD* pdwOverTime,DWORD* pdwCryptLevel,int* pnPolicyType,INT_DEQUE& PolicyDeque,BOOL bSystemDir)
{
// 	*pdwPriv = 0;
// 	
// 	CDowithUserInfo* g_pUserList = NULL;
// 	if (g_pUserList == NULL)
// 	{
// 		g_pUserList = new CDowithUserInfo;
// 		LPTSTR lpszUsrPath = NULL;
// 		if (bSystemDir)
// 		{
// 			GetSystemFilePath(&lpszUsrPath,TEXT("DocGuarder.usr"));
// 		}else
// 		{
// 			GetSystemFilePath(&lpszUsrPath,TEXT("DocGuarder.usr"));
// 		}
// 		try{
// 			g_pUserList->LoadUserInfo();
// 		}
// 		catch(...)
// 		{
// 			free(lpszUsrPath);
// 			*pdwPriv = 0;
// 			return FALSE;
// 		}
// 		free(lpszUsrPath);
// 	}
// 
// 	g_pUserList->IsLawUser(strUserName,"");
// 
// 	if (g_pUserList->m_pCurUserInfo)
// 	{
// 		if (g_pUserList->m_pCurUserInfo)
// 		{
// 			*pdwPriv = g_pUserList->m_pCurUserInfo->m_nPopedom;
// 			*pdwOverTime = g_pUserList->m_pCurUserInfo->m_nOverTime;
// 			*pdwCryptLevel = g_pUserList->m_pCurUserInfo->m_dwCryptLevel;
// //			*pnPolicyType = g_pUserList->m_pCurUserInfo->m_nPolicyType;
// //			for (int i=0;i<g_pUserList->m_pCurUserInfo->m_arrPolicy.size();i++)
// //			{
// //				PolicyDeque.push_back(g_pUserList->m_pCurUserInfo->m_arrPolicy[i]);
// //			}
// 		}
// 		delete g_pUserList;
// 		g_pUserList = NULL;
// 		return TRUE;
// 	}
// 
// 	delete g_pUserList;
// 	g_pUserList = NULL;
 	return FALSE;
}

DWORD CClientNodeInfo::GetClientPriv2(HINSTANCE hBase,LPCTSTR strCurUser,BOOL bSystemDir)
{
	if (hBase)
	{
		m_arrPolicy.clear();
		GetUserPrivInfo2(strCurUser,&m_dwPriv,&m_dwOverTime,&m_dwCryptLevel,&m_nPolicyType,m_arrPolicy,bSystemDir);
	}
	return 0;
}