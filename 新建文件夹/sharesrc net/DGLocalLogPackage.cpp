#include "StdAfx.h"
#include "DGLocalLogPackage.h"
#include <string>
using namespace std;

CDGLocalLogPackage::CDGLocalLogPackage(void)
{
	m_hInst					= NULL;
	m_pInitClientUnit		= NULL;
	m_pSendLog				= NULL;
	m_pInitSvrUnit			= NULL;
	m_pUninitSvrUnit		= NULL;
}

CDGLocalLogPackage::~CDGLocalLogPackage(void)
{
	if (m_hInst)
	{
		FreeLibrary(m_hInst);
	}
}
void CDGLocalLogPackage::InitFunction()
{

	if (m_hInst)
	{
		m_pInitClientUnit	= (DGLLog_InitClientUnit)GetProcAddress(m_hInst, "InitClientUnit");
		m_pSendLog			= (DGLLog_SendLog)GetProcAddress(m_hInst, "SendLog");
		m_pInitSvrUnit		= (DGLLog_InitSvrUnit)GetProcAddress(m_hInst, "InitSvrUnit");
		m_pUninitSvrUnit    = (DGLLog_UninitSvrUnit)GetProcAddress(m_hInst, "UninitSvrUnit");
	}
}

BOOL CDGLocalLogPackage::InitClientUnit(CErrorRecorder& cr)
{

	TCHAR szName[MAX_PATH] = { 0 };
	GetModuleFileName(NULL,szName, MAX_PATH);
	wstring str = szName;
	int nPos = str.find_last_of('\\');
	str = str.substr(0,nPos+1);
	memset((char*)szName, 0,sizeof(szName));
	_stprintf(szName,L"%s%s", str.c_str(), L"DGLocalLogRecorder.dll");
	m_hInst = LoadLibrary(szName);
	if (!m_hInst)
	{
		_DG_ERROR_PUSH(cr,DGLocalLog_DllNotFound);
	}

	InitFunction();

	BOOL bRet = FALSE;
	if (m_pInitClientUnit)
	{
		bRet = m_pInitClientUnit(cr);
	}
	else
	{
		_DG_ERROR_PUSH(cr,DGLocalLog_InterfaceNotFound);
		return FALSE;
	}
	return bRet;
}

BOOL CDGLocalLogPackage::InitSvrUnit(CErrorRecorder& cr)
{

	TCHAR szName[MAX_PATH] = { 0 };
	GetModuleFileName(NULL,szName, MAX_PATH);
	wstring str = szName;
	int nPos = str.find_last_of('\\');
	str = str.substr(0,nPos+1);
	memset((char*)szName, 0,sizeof(szName));
	_stprintf(szName,L"%s%s", str.c_str(), L"DGLocalLogRecorder.dll");
	m_hInst = LoadLibrary(szName);
	if (!m_hInst)
	{
		_DG_ERROR_PUSH(cr,DGLocalLog_DllNotFound);
	}

	InitFunction();

	BOOL bRet = FALSE;
	if (m_pInitSvrUnit)
	{
		bRet = m_pInitSvrUnit(cr);
	}
	else
	{
		_DG_ERROR_PUSH(cr,DGLocalLog_InterfaceNotFound);
		return FALSE;
	}
	return TRUE;
}

BOOL CDGLocalLogPackage::SendLog(int nType, char* pData, int nLen,CErrorRecorder& cr)
{

	BOOL bRet = FALSE;
	if (m_pSendLog)
	{
		bRet = m_pSendLog(nType, pData, nLen, cr);
	}
	else
	{
		_DG_ERROR_PUSH(cr,DGLocalLog_InterfaceNotFound);
		return FALSE;
	}
	return TRUE;
}

BOOL CDGLocalLogPackage::UninitSvrUnit(CErrorRecorder& cr)
{

	if (m_pUninitSvrUnit)
	{
		m_pUninitSvrUnit();
	}
	else
	{
		_DG_ERROR_PUSH(cr,DGLocalLog_InterfaceNotFound);
		return FALSE;
	}
	return TRUE;
}