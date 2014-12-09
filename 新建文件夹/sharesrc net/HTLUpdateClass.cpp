#include "StdAfx.h"
#include "HTLUpdateClass.h"
#include <WinSock2.h>

CHTLUpdateClass::CHTLUpdateClass(void)
{
	m_hInst = LoadLibrary(L"HTLUpdateSupport.dll");
	m_pInitClientUnit			= NULL;
	m_pInitSvrUnit				= NULL;
	m_pUninitSvrUnit				= NULL;
	m_pTestHTUpdateService	= NULL;
	m_pGetCompanyInfo		= NULL;
	m_pGetRestorePcInfo		= NULL;

	InitNetWork();
	InitFunction(m_hInst);
}

CHTLUpdateClass::~CHTLUpdateClass(void)
{
	if (m_hInst)
	{
		UnInitNetework();
		FreeLibrary(m_hInst);
	}
}

BOOL CHTLUpdateClass::InitNetWork()
{
	WSAData wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nRessult = WSAStartup(wVersionRequested, &wsaData);
	if (nRessult != 0)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CHTLUpdateClass::UnInitNetework()
{
	int nResult = WSACleanup();
	if (nResult != 0)
	{
		return FALSE;
	}
	return TRUE;
}

void CHTLUpdateClass::InitFunction(HMODULE hInst)
{
	if (m_hInst)
	{
		m_pInitClientUnit				= (HTLU_InitClientUnit)GetProcAddress(m_hInst, "InitClientUnit");
		m_pInitSvrUnit					= (HTLU_InitSvrUnit)GetProcAddress(m_hInst, "InitSvrUnit");
		m_pUninitSvrUnit					= (HTLU_UninitSvrUnit)GetProcAddress(m_hInst, "UninitSvrUnit");
		m_pTestHTUpdateService		= (HTLU_TestHTUpdateService)GetProcAddress(m_hInst, "TestHTUpdateService");
		m_pGetCompanyInfo			= (HTLU_GetCompanyInfo)GetProcAddress(m_hInst, "GetCompanyInfo");
		m_pGetRestorePcInfo			= (HTLU_GetRestorePcInfo)GetProcAddress(m_hInst, "GetRestorePcInfo");
	}
}

BOOL CHTLUpdateClass::InitClientUnit(CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pInitClientUnit)
	{
		bRtn = m_pInitClientUnit(cr);
	}
	return bRtn;
}

BOOL CHTLUpdateClass::InitSvrUnit(CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pInitSvrUnit)
	{
		bRtn = m_pInitSvrUnit(cr);
	}
	return FALSE;
}

BOOL CHTLUpdateClass::UninitSvrUnit(CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pInitSvrUnit)
	{
		bRtn = m_pUninitSvrUnit(cr);
	}
	return bRtn;
}

BOOL	 CHTLUpdateClass::	TestHTUpdateService(CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pTestHTUpdateService)
	{
		bRtn = m_pTestHTUpdateService(cr);
	}
	return bRtn;
}

BOOL	 CHTLUpdateClass::GetCompanyInfo(LPTSTR lpXmlPath, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetCompanyInfo)
	{
		bRtn = m_pGetCompanyInfo(lpXmlPath, cr);
	}
	return bRtn;
}

BOOL	 CHTLUpdateClass::GetRestorePcInfo(LPTSTR lpXmlPath, int nSize, LPCTSTR lpPcId, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetRestorePcInfo)
	{
		bRtn = m_pGetRestorePcInfo(lpXmlPath, nSize, lpPcId, cr);
	}
	return bRtn;
}