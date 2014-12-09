#include "StdAfx.h"
#include "UpdateMyself.h"
#include "shlwapi.h"
#include "winsvc.h"
#include "atlbase.h"

CUpdateMyself::CUpdateMyself(void)
{
	TCHAR szAppdata[1024] = {0};
	LPITEMIDLIST  ppidl;  
	SHGetSpecialFolderLocation( NULL, CSIDL_COMMON_APPDATA, &ppidl);
	SHGetPathFromIDList(ppidl,szAppdata);
	m_strAppDataPath = szAppdata;


	TCHAR szProgram[1024] = {0};
	SHGetSpecialFolderPath(NULL, szProgram, CSIDL_PROGRAM_FILES, 0);
	m_strProgramPath = szProgram;		//工作目录，如DG

	WSAData wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nRessult = WSAStartup(wVersionRequested, &wsaData);
}

CUpdateMyself::~CUpdateMyself(void)
{
	int nResult = WSACleanup();
}

/*
自更新的时候在注册表记录状态:，0：没有自更新，1：正在自更新，保护服务不进行操作，2：相应组件将要有自更新操作
*/

BOOL CUpdateMyself::IsNeedUpdateMyself(CString strTempPath)
{
	int flag = 0;
	CString strUpdatePath = m_strProgramPath + L"\\HTUpdate";
	strTempPath += L"\\HTUpdate";

	CString strSrcFile = strTempPath + L"\\HTLocalUpdateService.exe";
	CString strNewFile = strUpdatePath + L"\\HTLocalUpdateService.exe";
	if (PathFileExists(strSrcFile))
	{
		flag = 1;
		CString strTempFile = strNewFile + L".tmp";
		_wrename(strNewFile, strTempFile);
		if (MoveFileEx(strSrcFile, strNewFile, MOVEFILE_REPLACE_EXISTING))
		{
			CRegKey key;
			if (key.Open(HKEY_LOCAL_MACHINE, L"Software\\HTLUpdate", KEY_READ|KEY_WRITE|KEY_WOW64_32KEY) == ERROR_SUCCESS)
			{
				key.SetValue((DWORD)2, L"UpdateMyself");
			}
			key.Close();
			DeleteFile(strTempFile);
		}
		else
		{
			_wrename(strTempFile, strNewFile);
		}
	}

	strSrcFile = strTempPath + L"\\HTLUpdateSecService.exe";
	strNewFile = strUpdatePath + L"\\HTLUpdateSecService.exe";
	if (PathFileExists(strSrcFile))
	{
		flag = 1;
		CString strTempFile = strNewFile + L".tmp";
		_wrename(strNewFile, strTempFile);
		if (MoveFileEx(strSrcFile, strNewFile, MOVEFILE_REPLACE_EXISTING))
		{
			CRegKey key;
			if (key.Open(HKEY_LOCAL_MACHINE, L"Software\\HTLUpdate", KEY_READ|KEY_WRITE|KEY_WOW64_32KEY) == ERROR_SUCCESS)
			{
				key.SetValue((DWORD)2, L"HTUSecService");
			}
			key.Close();
			DeleteFile(strTempFile);
		}
		else
		{
			_wrename(strTempFile, strNewFile);
		}
	}

	strSrcFile = strTempPath + L"\\HTLUpdateSupport.dll";
	strNewFile = strUpdatePath + L"\\HTLUpdateSupport.dll";
	if (PathFileExists(strSrcFile))
	{
		flag = 1;
		CString strTempFile = strNewFile + L".tmp";
		_wrename(strNewFile, strTempFile);
		if (MoveFileEx(strSrcFile, strNewFile, MOVEFILE_REPLACE_EXISTING))
		{
			CRegKey key;
			if (key.Open(HKEY_LOCAL_MACHINE, L"Software\\HTLUpdate", KEY_READ|KEY_WRITE|KEY_WOW64_32KEY) == ERROR_SUCCESS)
			{
				key.SetValue((DWORD)2, L"UpdateMyself");
			}
			key.Close();
			DeleteFile(strTempFile);
		}
		else
		{
			_wrename(strTempFile, strNewFile);
		}
	}

	if (flag == 1)
	{
		return TRUE;
	}
	return FALSE;
}

void CUpdateMyself::UpdateLocalService()
{
	USES_CONVERSION;
	CRegKey key;
	if (key.Open(HKEY_LOCAL_MACHINE, L"Software\\HTLUpdate", KEY_READ|KEY_WRITE|KEY_WOW64_32KEY) == ERROR_SUCCESS)
	{
		key.SetValue((DWORD)1, L"UpdateMyself");
	}
	CString strUpdatePath = m_strProgramPath + L"\\HTUpdate";
	CString strCommand;

	strCommand.Format(L"%s\\%s -unregserver", strUpdatePath, L"HTLocalUpdateService.exe");
	WinExec(T2A(strCommand), SW_HIDE);
	Sleep(20000);
	
	CString strTempFile = strUpdatePath + L"\\HTLocalUpdateService.exe.tmp";
	while (PathFileExists(strTempFile))
	{
		if (DeleteFile(strTempFile))
		{
			break;
		}
		SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM != NULL)
		{
			SC_HANDLE hService = ::OpenService(hSCM, L"HTLocalUpdateService", SERVICE_QUERY_STATUS);
			if (hService != NULL)
			{
				SERVICE_STATUS status;     
				::QueryServiceStatus( hService, &status); 

				if (status.dwCurrentState == SERVICE_RUNNING)
				{						
					CString strCommand = L"sc stop HTLocalUpdateService";
					WinExec(T2A(strCommand), SW_HIDE);						
				}
			}
			::CloseServiceHandle(hService);
			::CloseServiceHandle(hSCM);
		}	
		Sleep(1000);	
	}

	strTempFile.Empty();
	strTempFile = strUpdatePath + L"\\HTLUpdateSupport.dll.tmp";
	OutputDebugString(strTempFile);
	while (PathFileExists(strTempFile))
	{
		OutputDebugString(strTempFile);
		if (DeleteFile(strTempFile))
		{
			break;
		}
		SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM != NULL)
		{
			SC_HANDLE hService = ::OpenService(hSCM, L"HTLocalUpdateService", SERVICE_QUERY_STATUS);
			if (hService != NULL)
			{
				SERVICE_STATUS status;     
				::QueryServiceStatus( hService, &status); 

				if (status.dwCurrentState == SERVICE_RUNNING)
				{						
					CString strCommand = L"sc stop HTLocalUpdateService";
					WinExec(T2A(strCommand), SW_HIDE);						
				}
			}
			::CloseServiceHandle(hService);
			::CloseServiceHandle(hSCM);
		}	
		Sleep(1000);	
	}

	strCommand = L"";
	strCommand.Format(L"%s\\%s -service", strUpdatePath, L"HTLocalUpdateService.exe");
	OutputDebugString(strCommand);
	WinExec(T2A(strCommand), SW_HIDE);

	strCommand = L"";
	strCommand.Format(L"sc start HTLocalUpdateService");
	OutputDebugString(strCommand);
	WinExec(T2A(strCommand), SW_HIDE);

	key.SetValue((DWORD)0, L"UpdateMyself");
	key.Close();
}

void CUpdateMyself::UpdateSecService()
{
	USES_CONVERSION;
	CRegKey key;
	if (key.Open(HKEY_LOCAL_MACHINE, L"Software\\HTLUpdate", KEY_READ|KEY_WRITE|KEY_WOW64_32KEY) == ERROR_SUCCESS)
	{
		key.SetValue((DWORD)1, L"HTUSecService");
	}
	CString strUpdatePath = m_strProgramPath + L"\\HTUpdate";
	CString strCommand;

	strCommand.Empty();
	strCommand.Format(L"%s\\%s -unregserver", strUpdatePath, L"HTLUpdateSecService.exe");
	WinExec(T2A(strCommand), SW_HIDE);
	Sleep(1000);

	CString strTempFile = strUpdatePath + L"\\HTLUpdateSecService.exe.tmp";
	while (PathFileExists(strTempFile))
	{
		if (DeleteFile(strTempFile))
		{
			break;
		}
		SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM != NULL)
		{
			SC_HANDLE hService = ::OpenService(hSCM, L"HTLUpdateSecService", SERVICE_QUERY_STATUS);
			if (hService != NULL)
			{
				SERVICE_STATUS status;     
				::QueryServiceStatus( hService, &status); 

				if (status.dwCurrentState == SERVICE_RUNNING)
				{						
					CString strCommand = L"sc stop HTLUpdateSecService";
					WinExec(T2A(strCommand), SW_HIDE);						
				}
			}
			::CloseServiceHandle(hService);
			::CloseServiceHandle(hSCM);
		}	
		Sleep(1000);	
	}

	strCommand = L"";
	strCommand.Format(L"%s\\%s -service", strUpdatePath, L"HTLUpdateSecService.exe");
	OutputDebugString(strCommand);
	WinExec(T2A(strCommand), SW_HIDE);

	strCommand = L"";
	strCommand.Format(L"sc start HTLUpdateSecService");
	OutputDebugString(strCommand);
	WinExec(T2A(strCommand), SW_HIDE);

	key.SetValue((DWORD)0, L"HTUSecService");
	key.Close();
}

void CUpdateMyself::UpdateMyself()
{
	USES_CONVERSION;
	CRegKey key;
	DWORD dwLocalService = 0;
	DWORD dwSecService = 0;
	if (key.Open(HKEY_LOCAL_MACHINE, L"Software\\HTLUpdate", KEY_READ|KEY_WRITE|KEY_WOW64_32KEY) == ERROR_SUCCESS)
	{
		key.QueryValue(dwSecService, L"HTUSecService");
		key.QueryValue(dwLocalService, L"UpdateMyself");
	}

	if (dwSecService != 0)
	{
		UpdateSecService();
	}

//	if (dwLocalService != 0)
//	{
		UpdateLocalService();
//	}
}
