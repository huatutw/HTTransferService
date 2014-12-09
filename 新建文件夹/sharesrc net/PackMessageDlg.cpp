
#include "stdafx.h"
#include "PackMessageDlg.h"
CPackMessageDlg::CPackMessageDlg(void)
{
		m_hInst = 0;

		m_hInst = LoadLibrary(L"MessageDlgDll.dll");
		
		if(m_hInst)
			InitFunction(m_hInst);
		else
			OutputDebugString(L"loaddll failed!");

}


CPackMessageDlg::~CPackMessageDlg(void)
{

	if ( m_hInst )
	{

		FreeLibrary( m_hInst );

	}
	
}


void CPackMessageDlg::InitFunction(HMODULE hInst)
{
	if ( hInst )
	{
		m_pDlgInstance			= (DlgInstance)GetProcAddress(hInst,"MMD_AddMessage");
		m_pDlgAddMessageType	= (DlgAddMessageType)GetProcAddress(hInst,"MMD_AddMessageType");
		m_pDlgRemoveInfoByIndex = (DlgRemoveInfoByIndex)GetProcAddress(hInst,"MMD_RemoveInfoByIndex");
		m_pDlgRemoveAllInfo		= (DlgRemoveAllInfo)GetProcAddress(hInst,"MMD_RemoveAllInfo");
		m_pRemoveBubbleDlgByIndex = (DlgRemoveBubbleDlgByIndex)GetProcAddress(hInst,"MMD_RemoveBubbleDlgByIndex");
		m_pInitAprroveInfo		 = (DlgInitAprroveInfo)GetProcAddress(hInst,"MMD_InitApproveInfo");
	}
	
}

int CPackMessageDlg::AddMessage(MSG_DLG_INFO info)
{

	if ( m_pDlgInstance&& m_hInst)
	{
		return m_pDlgInstance(info);
	}
	
	return -1;
}

int CPackMessageDlg::AddMessageType(int nType, CString str)
{

	if ( m_pDlgAddMessageType&& m_hInst)
	{
		return m_pDlgAddMessageType(nType, str);
	}
	
	return -1;
}

void CPackMessageDlg::RemoveInfoByIndex(int nIndex)
{

	if (m_pDlgRemoveInfoByIndex && m_hInst)
	{
		m_pDlgRemoveInfoByIndex(nIndex);
	}

}

void CPackMessageDlg::RemoveBubbleDlgByIndex(int nIndex)
{

	if (m_pRemoveBubbleDlgByIndex && m_hInst )
	{
		m_pRemoveBubbleDlgByIndex(nIndex);
	}
}

void CPackMessageDlg::RemoveAllInfo()
{

	if (m_pDlgRemoveInfoByIndex && m_hInst)
	{
		m_pDlgRemoveAllInfo();
	}

}

void CPackMessageDlg::InitApproveInfo(TCHAR* szUser, TCHAR* szIniPath)
{

	if (m_pInitAprroveInfo && m_hInst)
	{
		m_pInitAprroveInfo(szUser, szIniPath);
	}
}