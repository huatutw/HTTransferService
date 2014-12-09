#include "StdAfx.h"
#include "DGMSUpdateClientUIF.h"
#include "DGCommonFunc.h"
#include "astring.h"
#include "spIPCComm.h"
#include "socketbase.h"
#include "Cryp.h"
#include "io.h"
#include "ModulVer.h"


CDGMSUpdateClientUIF::CDGMSUpdateClientUIF(void)
{
}

CDGMSUpdateClientUIF::~CDGMSUpdateClientUIF(void)
{
}

CAString CDGMSUpdateClientUIF::GetFileMD5(CAString strFilePathName)
{
	USES_CONVERSION;
	BYTE out[32] = {0};
	CCryp cCrpy;
	cCrpy.FileToMD5(strFilePathName.GetBuffer(1),out);
	strFilePathName.ReleaseBuffer();
	TCHAR tcMD5[128] = {0};
	TCHAR tcTemp[128] = {0};
	for(int nfor = 0; nfor < 16; nfor++)   
	{   
		_stprintf(tcTemp,_T("%02x"),out[nfor]);
		_tcscat(tcMD5,tcTemp);
	}

	CAString strMD5 = _T("");
	strMD5 = tcMD5;

	return strMD5;

}

CAString CDGMSUpdateClientUIF::GetClientID()
{
	USES_CONVERSION;
	TCHAR tcClientID[BUFF_SIZE] = {0};
	CAString strClientId = _T("");
	CRegKey key;
	if (key.Open(HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\DocGuarder"),KEY_READ) == ERROR_SUCCESS)
	{
		TCHAR value[1024] = _T("");
		DWORD length = 1024;

		if(key.QueryValue(value,_T("ClientId"),&length) == ERROR_SUCCESS)
		{
			if(0 != _tcslen(value))
			{
				char out[1024] = {0};
				DGCommonFunc::FromBase64((const unsigned char*)T2A(value),strlen(T2A(value)),out);
				strClientId = out;
			}
		}
	}
	key.Close();

	return strClientId;
}
 

void CDGMSUpdateClientUIF::WritePrivateProfileInt(LPCTSTR strKey,LPCTSTR strName,int dwCount,LPCTSTR strIni)
{
	TCHAR strNumber[1024] = {0};
	_stprintf(strNumber,_T("%d"),dwCount);
	WritePrivateProfileString(strKey,strName,strNumber,strIni);
}


DWORD CDGMSUpdateClientUIF::UploadClientVersion()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);

	USES_CONVERSION;

	TCHAR strIP[1024] = {0};
	DWORD dwServerPort;
	LPCSTR pszRemoteAddr;
	int nRet = -1;
	if (!DGCommonFunc::ReadRegUpdateData(strIP,dwServerPort))
	{
		_tcscpy(strIP,_T("127.0.0.1"));
		dwServerPort = 9555;
	}

	pszRemoteAddr = T2A(strIP);
	while (TRUE)
	{
		SOCKET sLogComm = SPCreateConnectSocket(pszRemoteAddr,dwServerPort);

		if (sLogComm == INVALID_SOCKET)
		{
			Sleep(10000);
			continue;
		}
		CIPCSocket s(sLogComm,TRUE);

		CSocketBase::SendStringData(s,_T("UploadClientVersion"),_tcslen(_T("UploadClientVersion")));
		OutputDebugString(L"UploadClientVersion");
		TCHAR strReply[BUFF_SIZE] = {0};
		if (!CSocketBase::RecvStringData(s,strReply,BUFF_SIZE))
		{
			return FALSE;
		}

		if (_tcscmp(strReply,_T("Continue")) == 0)
		{
			CAString strClientID = GetClientID();
			CSocketBase::SendStringData(s, strClientID);

			TCHAR buffer[MAX_PATH] = {0};
			GetModuleFileName(NULL,buffer,MAX_PATH);
			CAString strClientPath = buffer;
			int pos = strClientPath.ReverseFind(TEXT('\\'));
			strClientPath = strClientPath.Left(pos+1);

			CAString strModuleListPath = strClientPath + _T("ModuleList.ini");

 			DeleteFile(strModuleListPath);
 			if(!CSocketBase::RecvFile1(s, strModuleListPath.GetBuffer(1)))
 			{
 				strModuleListPath.ReleaseBuffer();
 				return FALSE;
 			}
			strModuleListPath.ReleaseBuffer();

			int nModuleCount = GetPrivateProfileInt(_T("ModuleCount"), _T("Count"), 0, strModuleListPath);

			for(int nfor = 0; nfor < nModuleCount; nfor++)
			{
				DGMS_ModuleInfo ModuleInfo;
				memset(&ModuleInfo, 0, sizeof(DGMS_ModuleInfo));

				CAString strSectionName = _T("");
				strSectionName.Format(_T("Module%d"), nfor);

				TCHAR tcModuleName[LEN_NAME] = {0};
				GetPrivateProfileString(strSectionName, _T("ModuleName"), _T(""), tcModuleName, LEN_NAME, strModuleListPath);

				CAString strModulePathName = _T("");
				strModulePathName.Format(_T("%s%s"), strClientPath, tcModuleName);

				_tcscpy(ModuleInfo.strModuleName, tcModuleName);

				WIN32_FILE_ATTRIBUTE_DATA data;
				if (GetFileAttributesEx(strModulePathName,GetFileExInfoStandard,(LPVOID)&data))
				{
					CModuleVersion ver;

					ver.GetFileVersionInfoEx(strModulePathName.GetBuffer(1));
					strModulePathName.ReleaseBuffer();
					TCHAR value[128] = {0};
					ver.GetValue(_T("ProductVersion"),value);

					SYSTEMTIME modifytime;
					FILETIME   localtime;
					FileTimeToLocalFileTime(&data.ftLastWriteTime, &localtime);
					FileTimeToSystemTime(&localtime, &modifytime);

					TCHAR strModifyTime[1024] = {0};
					_stprintf(strModifyTime, _T("%d-%02d-%02d %02d:%02d:%02d"),
						modifytime.wYear,
						modifytime.wMonth,
						modifytime.wDay,
						modifytime.wHour,
						modifytime.wMinute,
						modifytime.wSecond);

					CAString strModuleMD5 = GetFileMD5(strModulePathName);

					_tcscpy(ModuleInfo.strVersionName, value);
					_tcscpy(ModuleInfo.strModuleMD5, strModuleMD5);
					_tcscpy(ModuleInfo.strLastModify, strModifyTime);
					ModuleInfo.nModuleSize = data.nFileSizeLow;
				}
				else
				{
					CAString strModuleNotFound = LoadStringByID(_AtlBaseModule.m_hInst, IDS_ERROR_MODULE_NOTFOUND); 
					_tcscpy(ModuleInfo.strVersionName, strModuleNotFound);
					_tcscpy(ModuleInfo.strModuleMD5, strModuleNotFound);
					_tcscpy(ModuleInfo.strLastModify, strModuleNotFound);
					ModuleInfo.nModuleSize = 0;
				}
				
				CSocketBase::SendData(s, &ModuleInfo, sizeof(DGMS_ModuleInfo));
			}
			break;
		}
	}

	return ERROR_SUCCESS;
}

DWORD CDGMSUpdateClientUIF::UploadClientInfo()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);

	USES_CONVERSION;

	TCHAR strIP[1024] = {0};
	DWORD dwServerPort;
	LPCSTR pszRemoteAddr;
	int nRet = -1;
	if (!DGCommonFunc::ReadRegUpdateData(strIP,dwServerPort))
	{
		_tcscpy(strIP,_T("127.0.0.1"));
		dwServerPort = 9555;
	}

	pszRemoteAddr = T2A(strIP);
	
	SOCKET sLogComm = SPCreateConnectSocket(pszRemoteAddr,dwServerPort);

	if (sLogComm == INVALID_SOCKET)
	{
		return FALSE;
	}
	CIPCSocket s(sLogComm,TRUE);

	CSocketBase::SendStringData(s,_T("UploadClientInfo"),_tcslen(_T("UploadClientInfo")));
	TCHAR strReply[BUFF_SIZE] = {0};
	CSocketBase::RecvStringData(s,strReply,BUFF_SIZE);

	if (_tcscmp(strReply,_T("Continue")) == 0)
	{
		CAString strClientID = _T("");
		CAString strComputerName = _T("");

		strClientID = GetClientID();

		TCHAR tcComputerName[1024] = {0};
		DWORD size = 1023;
		GetComputerName(tcComputerName,&size);
		strComputerName = tcComputerName;

		CSocketBase::SendStringData(s, strClientID);
		CSocketBase::SendStringData(s, strComputerName);

		CSocketBase::RecvData(s, &nRet, sizeof(int));
	}

	return nRet;
}

void CDGMSUpdateClientUIF::GetModuleFileFromServer(CAString& m_strModulePath, CAString& m_strClientModuleFilePath, 
											   CAString& m_strClientTempModuleFilePath)
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);

	USES_CONVERSION;

	BOOL bIsNeedUpdate = TRUE;

	m_strClientModuleFilePath = m_strModulePath + _T("ClientModule.ini");
	m_strClientTempModuleFilePath = m_strModulePath + _T("ClientModuleTemp.ini");
	CAString strMD5 = GetFileMD5(m_strClientModuleFilePath);

	TCHAR strIP[1024] = {0};
	DWORD dwServerPort;
	LPCSTR pszRemoteAddr;
// 	if (!DGCommonFunc::ReadRegUpdateData(strIP,dwServerPort))
// 	{
// 		_tcscpy(strIP,_T("127.0.0.1"));
// 		dwServerPort = 9555;
// 	}
// 
// 	pszRemoteAddr = T2A(strIP);

 	while(TRUE)
	{
		if (!DGCommonFunc::ReadRegUpdateData(strIP,dwServerPort))
		{
			_tcscpy(strIP,_T("127.0.0.1"));
			dwServerPort = 9555;
		}

		pszRemoteAddr = T2A(strIP);

		char IP[32] = {0};
		SOCKET sLogComm = SPCreateConnectSocket(pszRemoteAddr,dwServerPort);

		if (sLogComm == INVALID_SOCKET)
		{
			OutputDebugString(_T("sLogComm == INVALID_SOCKET"));
#ifdef _DEBUG
			Sleep(30000);
#else
			Sleep(8000);
#endif
			continue;
		}
		CIPCSocket s(sLogComm,TRUE);

		CSocketBase::SendStringData(s,_T("GetModuleFile"),_tcslen(_T("GetModuleFile")));
		TCHAR strReply[BUFF_SIZE] = {0};
		CSocketBase::RecvStringData(s,strReply,BUFF_SIZE);

		OutputDebugString(L"GetModuleFile");

		if (_tcscmp(strReply,_T("Continue")) == 0)
		{
			CSocketBase::SendStringData(s, strMD5);

			CSocketBase::RecvData(s, &bIsNeedUpdate, sizeof(BOOL));

			if(bIsNeedUpdate)
			{
				if(CSocketBase::RecvFile1(s, m_strClientModuleFilePath.GetBuffer(1)))
				{
					m_strClientModuleFilePath.ReleaseBuffer();
				}
			}

			int nCount = GetPrivateProfileInt(_T("ModuleCount"), _T("Count"), 0, m_strClientModuleFilePath);

			for(int nfor = 0; nfor < nCount; nfor++)
			{
				CAString strSectionName = _T("");
				strSectionName.Format(_T("Module%d"), nfor);

				TCHAR tcModuleName[LEN_NAME] = {0};
				GetPrivateProfileString(strSectionName, _T("ModuleName"), _T(""), tcModuleName, LEN_NAME, m_strClientModuleFilePath);

				CAString strModulePathName;
				strModulePathName.Format(_T("%s%s"), m_strModulePath, tcModuleName);

				CAString strModuleMD5 = GetFileMD5(strModulePathName);
				WritePrivateProfileString(strSectionName, _T("FileMD5"), strModuleMD5, m_strClientModuleFilePath);

			}

			return;
		}
		else
		{
#ifdef _DEBUG
			Sleep(30000);
#else
			Sleep(300000);
#endif
			continue;
		}
	}
}

DWORD CDGMSUpdateClientUIF::UpdateByModuleFileProcess(CAString& m_strModulePath, CAString& m_strClientModuleFilePath,
												  CAString& m_strClientTempModuleFilePath, CAString& m_strServerModuleFilePath, CAString& m_strRollBackPath)
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);

	USES_CONVERSION;

	BOOL bUpdateError = FALSE;
	BOOL bIsNeedUpdate = TRUE;

	CAString strDGPath = _T("");
	TCHAR strIP[1024] = {0};
	DWORD dwServerPort;
	LPCSTR pszRemoteAddr;

	if (!DGCommonFunc::ReadRegUpdateData(strIP,dwServerPort))
	{
		_tcscpy(strIP,_T("127.0.0.1"));
		dwServerPort = 9555;
	}
	pszRemoteAddr = T2A(strIP);

	while(TRUE)
	{
		CRegKey key;
		DWORD dwRestartUpdate = FALSE;
		if (key.Open(HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\DocGuarder\\Client"),KEY_READ) == ERROR_SUCCESS)
		{
			if(key.QueryValue(dwRestartUpdate,_T("RestartUpdate")) == ERROR_SUCCESS)
			{
				if(1 == dwRestartUpdate)
				{
					OutputDebugString(_T("Need RestartUpdate"));
#ifdef _DEBUG
					Sleep(30000);
#else
					Sleep(300000);
#endif
					continue;
				}
			}
		}

		char IP[32] = {0};
		SOCKET sLogComm = SPCreateConnectSocket(pszRemoteAddr,dwServerPort);

		if (sLogComm == INVALID_SOCKET)
		{
			OutputDebugString(_T("sLogComm == INVALID_SOCKET"));
#ifdef _DEBUG
			Sleep(30000);
#else
			Sleep(300000);
#endif
			continue;
		}
		CIPCSocket s(sLogComm,TRUE);

		CSocketBase::SendStringData(s,_T("UpdateByModuleFile"),_tcslen(_T("UpdateByModuleFile")));
		TCHAR strReply[BUFF_SIZE] = {0};
		CSocketBase::RecvStringData(s,strReply,BUFF_SIZE);
		OutputDebugString(L"UpdateByModuleFile");

		if (_tcscmp(strReply,_T("Continue")) == 0)
		{
			if(m_strClientModuleFilePath.IsEmpty())
			{
				m_strClientModuleFilePath =  m_strModulePath + _T("ClientModule.ini");
			}

			if(m_strClientTempModuleFilePath.IsEmpty())
			{
				m_strClientTempModuleFilePath =  m_strModulePath + _T("ClientModuleTemp.ini");
			}

			m_strServerModuleFilePath = m_strModulePath + _T("ServerModule.ini");

			CAString strModuleFileMD5 = GetFileMD5(m_strClientModuleFilePath);

			CSocketBase::SendStringData(s, strModuleFileMD5);

			CSocketBase::RecvData(s, &bIsNeedUpdate, sizeof(BOOL));


			if(bIsNeedUpdate)
			{
				if(!CSocketBase::RecvFile1(s, m_strServerModuleFilePath.GetBuffer(1)))
				{
					m_strServerModuleFilePath.ReleaseBuffer();
					goto RECV_ERROR;
				}
				m_strServerModuleFilePath.ReleaseBuffer();

				DeleteFile(m_strClientTempModuleFilePath);
				int nCount = GetPrivateProfileInt(_T("ModuleCount"), _T("Count"), 0, m_strServerModuleFilePath);
				WritePrivateProfileInt(_T("ModuleCount"), _T("Count"), nCount, m_strClientTempModuleFilePath);

				deqDMNeedUpdateModule deqNeedUpdae;
				for(int nfor = 0; nfor < nCount; nfor++)
				{
					CAString strSectionName = _T("");
					strSectionName.Format(_T("Module%d"), nfor);

					TCHAR tcModuleName[LEN_NAME] = {0};
					GetPrivateProfileString(strSectionName, _T("ModuleName"), _T(""), tcModuleName, LEN_NAME, m_strServerModuleFilePath);

					TCHAR tcServerModuleMD5[LEN_NAME] = {0};
					GetPrivateProfileString(strSectionName, _T("FileMD5"), _T(""), tcServerModuleMD5, LEN_NAME, m_strServerModuleFilePath);

					CAString strClientModulePathName;
					strClientModulePathName.Format(_T("%s%s"), m_strModulePath, tcModuleName);
					CAString strTempFilePathName;
					strTempFilePathName.Format(_T("%s%s.utmp"), m_strModulePath, tcModuleName);
					CAString strClientModuleMD5 = GetFileMD5(strClientModulePathName);
					CAString strTempFileMD5 = GetFileMD5(strTempFilePathName);

					if(0 != strClientModuleMD5.CompareNoCase(tcServerModuleMD5) && 0 != strTempFileMD5.CompareNoCase(tcServerModuleMD5))
					{
						DM_NeedUpdateModule DMNeedUpdateModule;
						_tcscpy(DMNeedUpdateModule.strModuleName, tcModuleName);
						_tcscpy(DMNeedUpdateModule.strModuleMD5, tcServerModuleMD5);
						_tcscpy(DMNeedUpdateModule.strSectionName, strSectionName);

						WritePrivateProfileString(strSectionName, _T("ModuleName"), tcModuleName, m_strClientTempModuleFilePath);
						WritePrivateProfileString(strSectionName, _T("FileMD5"), strClientModuleMD5, m_strClientTempModuleFilePath);

						deqNeedUpdae.push_back(DMNeedUpdateModule);
					}
					else
					{
						WritePrivateProfileString(strSectionName, _T("ModuleName"), tcModuleName, m_strClientTempModuleFilePath);
						WritePrivateProfileString(strSectionName, _T("FileMD5"), tcServerModuleMD5, m_strClientTempModuleFilePath);
					}
				}

				int nNeedUpdateModuleCount = deqNeedUpdae.size();

				CSocketBase::SendData(s, &nNeedUpdateModuleCount, sizeof(int));

				for(int n = 0; n < nNeedUpdateModuleCount; n++)
				{
					CSocketBase::SendStringData(s, deqNeedUpdae[n].strModuleName);

					CAString strTempFilePathName;
					strTempFilePathName.Format(_T("%s%s.utmp"), m_strModulePath, deqNeedUpdae[n].strModuleName);

					if(!CSocketBase::RecvFile1(s, strTempFilePathName.GetBuffer(1)))
					{
						strTempFilePathName.ReleaseBuffer();
						goto RECV_ERROR;
					}
					else
					{
						strTempFilePathName.ReleaseBuffer();

						CAString strTempFileMD5 = GetFileMD5(strTempFilePathName);

						if(0 == strTempFileMD5.CompareNoCase(deqNeedUpdae[n].strModuleMD5))
						{
							WritePrivateProfileString(deqNeedUpdae[n].strSectionName, _T("ModuleName"), deqNeedUpdae[n].strModuleName, m_strClientTempModuleFilePath);
							WritePrivateProfileString(deqNeedUpdae[n].strSectionName, _T("FileMD5"), deqNeedUpdae[n].strModuleMD5, m_strClientTempModuleFilePath);
						}
					}
				}

				CAString strClientTempModuleFileMD5;
				CAString strServerModuleFileMD5;

				strClientTempModuleFileMD5 = GetFileMD5(m_strClientTempModuleFilePath);
				strServerModuleFileMD5 = GetFileMD5(m_strServerModuleFilePath);

				if(0 == strClientTempModuleFileMD5.CompareNoCase(strServerModuleFileMD5))
				{
					m_strRollBackPath = m_strModulePath + _T("RollBack\\");
					if(!PathIsDirectory(m_strRollBackPath))
					{
						CreateDirectory(m_strRollBackPath,NULL);
					}

					int nCount = GetPrivateProfileInt(_T("ModuleCount"), _T("Count"), 0, m_strClientTempModuleFilePath);

					for(int nfor = 0; nfor < nCount; nfor++)
					{
						CAString strSectionName = _T("");
						strSectionName.Format(_T("Module%d"), nfor);

						TCHAR tcModuleName[LEN_NAME] = {0};
						GetPrivateProfileString(strSectionName, _T("ModuleName"), _T(""), tcModuleName, LEN_NAME, m_strClientTempModuleFilePath);

						CAString strOldModulePathName;
						strOldModulePathName.Format(_T("%s%s"), m_strModulePath, tcModuleName);
						CAString strRollBackFilePathName;
						strRollBackFilePathName.Format(_T("%s%s"), m_strRollBackPath, tcModuleName);
						CAString strTempFilePathName;
						strTempFilePathName.Format(_T("%s%s.utmp"), m_strModulePath, tcModuleName);

						if(0 == _waccess(strTempFilePathName, 0))
						{
							if(0 == _waccess(strOldModulePathName, 0))
							{
								if(MoveFileEx(strOldModulePathName, strRollBackFilePathName, MOVEFILE_REPLACE_EXISTING|MOVEFILE_DELAY_UNTIL_REBOOT))
								{
									MoveFileEx(strTempFilePathName, strOldModulePathName, MOVEFILE_REPLACE_EXISTING|MOVEFILE_DELAY_UNTIL_REBOOT);
								}
							}
							else
							{
								MoveFileEx(strTempFilePathName, strOldModulePathName, MOVEFILE_REPLACE_EXISTING|MOVEFILE_DELAY_UNTIL_REBOOT);
							}
						}
					}

					if (nNeedUpdateModuleCount > 0)
					{
						WSADATA wsaData;
						WORD wVersionRequested = MAKEWORD(2, 2);
						int nResult = WSAStartup(wVersionRequested, &wsaData);
						USES_CONVERSION;

						CAString strIP = _T("127.0.0.1");

						SOCKET sComm = SPCreateConnectSocket(T2A(strIP.GetBuffer(1)),9447);

						strIP.ReleaseBuffer();
						CIPCSocket s(sComm,TRUE);

						CSocketBase::SendStringData(s,_T("UpdateFinishedAndRestart"));

						TCHAR tcReply[BUFF_SIZE] = {0};
						CSocketBase::RecvStringData(s,tcReply,BUFF_SIZE,NULL);
						OutputDebugString(L"UpdateFinishedAndRestart");

						CRegKey key;
						DWORD dwRestartUpdate = TRUE;
						if (ERROR_SUCCESS == key.Open(HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\DocGuarder\\Client"),KEY_WRITE))
						{
							key.SetDWORDValue(_T("RestartUpdate"), dwRestartUpdate);
							key.Close();
						}
					}

					return 0;
				}
			}
		}

RECV_ERROR:
#ifdef _DEBUG
		Sleep(30000);
#else
		Sleep(300000);
#endif
		continue;
	}

	/*	return 0;*/
}
