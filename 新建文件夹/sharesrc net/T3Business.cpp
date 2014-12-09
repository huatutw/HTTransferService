#include "StdAfx.h"
#include "T3Business.h"

CT3Business::CT3Business(void)
{
	m_hInst						= NULL;
	m_pMainTestT3Service		= NULL;
	m_pTestT3Service			= NULL;
	m_pGetUserListfoFromServ	= NULL;
	m_pGetXmlInfoFromServ		= NULL;
	m_pApplyAuthorization		= NULL;
	m_pLoginDVSystem			= NULL;
	m_pCheckSetupInfo			= NULL;
	m_pCheckFilePermission		= NULL;
	m_pCheckFilePassword		= NULL;
	m_pOpenT3File				= NULL;
	m_pGetT3FileInfo			= NULL;
	m_pGetT3FileRemainDate		= NULL;
	m_pReadFileInfoFromIni		= NULL;
	m_pGetDogData				= NULL;
	m_pGetFileInfoFromServ		= NULL;
	m_pGetWatermarkState		= NULL;
	m_pGetAccreditInfo			= NULL;
	m_hInst = LoadLibrary(L"T3BusinessMode.dll");
	OutputDebugString(L"LoadLibrary T3BusinessMode.dll");
	if (m_hInst)
	{
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		int nResult = WSAStartup(wVersionRequested, &wsaData);
		InitFunction(m_hInst);
	}
	else
		OutputDebugString(L"µ÷ÓÃT3BusinessMode.dllÊ§°Ü");
}

CT3Business::~CT3Business(void)
{
	if (m_hInst)
	{
		FreeLibrary(m_hInst);
	}
	int nResult = WSACleanup();
}

BOOL CT3Business::InitNetwork()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
	{
		return FALSE;
	}
	return TRUE;

}

BOOL CT3Business::UnInitNetwork()
{
	int nResult = WSACleanup();
	if (nResult != 0)
	{
		return FALSE;
	}
	return TRUE;
}

void CT3Business::InitFunction(HMODULE hInst)
{
	if (hInst)
	{
		m_pMainTestT3Service		= (T3BM_MainTestT3Service)GetProcAddress(hInst, "MainTestT3Service");
		m_pTestT3Service			= (T3BM_TestT3Service)GetProcAddress(hInst, "TestT3Service");
		m_pGetUserListfoFromServ	= (T3BM_GetUserListfoFromServ)GetProcAddress(hInst, "GetUserListfoFromServ");
		m_pGetXmlInfoFromServ		= (T3BM_GetXmlInfoFromServ)GetProcAddress(hInst, "GetXmlInfoFromServ");
		m_pApplyAuthorization		= (T3BM_ApplyAuthorization)GetProcAddress(hInst, "ApplyAuthorization");
		m_pLoginDVSystem			= (T3BM_LoginDVSystem)GetProcAddress(hInst, "LoginDVSystem");
		m_pCheckSetupInfo			= (T3BM_CheckSetupInfo)GetProcAddress(hInst, "CheckSetupInfo");
		m_pCheckFilePermission		= (T3BM_CheckFilePermission)GetProcAddress(hInst, "CheckFilePermission");
		m_pCheckFilePassword		= (T3BM_CheckFilePassword)GetProcAddress(hInst, "CheckFilePassword");
		m_pOpenT3File				= (T3BM_OpenT3File)GetProcAddress(hInst, "OpenT3File");
		m_pReadFileInfoFromIni		= (T3BM_ReadFileInfoFromIni)GetProcAddress(hInst, "ReadFileInfoFromIni");
		m_pGetT3FileInfo			= (T3BM_GetT3FileInfo)GetProcAddress(hInst, "GetT3FileInfo");
		m_pGetT3FileRemainDate		= (T3BM_GetT3FileRemainDate)GetProcAddress(hInst, "GetT3FileRemainDate");
		m_pGetDogData				= (T3BM_GetDogData)GetProcAddress(hInst, "GetDogData");
		m_pGetFileInfoFromServ		= (T3BM_GetFileInfoFromServ)GetProcAddress(hInst, "GetFileInfoFromServ");
		m_pGetWatermarkState		= (T3BM_GetWatermarkState)GetProcAddress(hInst, "GetWatermarkState");
		m_pGetAccreditInfo			= (T3BM_GetAccreditInfo)GetProcAddress(hInst, "GetAccreditInfo");
	}
}


BOOL CT3Business::MainTestT3Service(CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pMainTestT3Service)
	{
		bRtn = m_pMainTestT3Service(cr);
	}
	return bRtn;
}
BOOL CT3Business::TestT3Service(CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pTestT3Service)
	{
		bRtn = m_pTestT3Service(cr);
	}
	return bRtn;
}

BOOL CT3Business::GetUserListfoFromServ(LPTSTR lpData,LPTSTR lpXmlPath,CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetUserListfoFromServ)
	{
		bRtn = m_pGetUserListfoFromServ(lpData, lpXmlPath, cr);
	}
	return bRtn;
}

BOOL CT3Business::GetXmlInfoFromServ(int nSize, LPTSTR lpData,LPTSTR lpXmlPath,CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetXmlInfoFromServ)
	{
		bRtn = m_pGetXmlInfoFromServ( nSize, lpData,lpXmlPath,cr);
	}
	return bRtn;
}

BOOL CT3Business::ApplyAuthorization(int nSize, LPTSTR lpData,LPTSTR lpXmlPath,CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pApplyAuthorization)
	{
		bRtn = m_pApplyAuthorization( nSize, lpData,lpXmlPath,cr);
	}
	return bRtn;
}

BOOL CT3Business::LoginDVSystem(LPSTR lpSvrIp, DWORD dwSvrPort,T3_DBS_LoginInfo t3LoginInfo,T3_DBS_RTLoginInfo& t3RtnLoginInfo, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pLoginDVSystem)
	{
		bRtn = m_pLoginDVSystem(lpSvrIp,dwSvrPort,t3LoginInfo,t3RtnLoginInfo,cr);
	}
	return bRtn;
}


BOOL CT3Business::CheckSetupInfo(LPSTR lpSvrIp, DWORD dwSvrPort,T3_DBS_SetupInfo t3SetupInfo,T3_DBS_RTSetupInfo& t3RtnSetupInfo, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pCheckSetupInfo)
	{
		bRtn = m_pCheckSetupInfo(lpSvrIp,dwSvrPort,t3SetupInfo,t3RtnSetupInfo,cr);
	}
	return bRtn;
}

BOOL CT3Business::CheckFilePermission(T3_DBS_CheckFileInfo t3FilePermission, T3_DBS_RTCheckFileInfo& t3RtnFilePermission, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pCheckFilePermission)
	{
		bRtn = m_pCheckFilePermission(t3FilePermission,t3RtnFilePermission,cr);
	}
	return bRtn;
}

BOOL CT3Business::CheckFilePassword(T3_DBS_CheckFileInfo t3CheckInfo, T3_DBS_RTCheckFileInfo& t3RtnData, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pCheckFilePassword)
	{
		bRtn = m_pCheckFilePassword(t3CheckInfo,t3RtnData,cr);
	}
	return bRtn;
}

BOOL CT3Business::OpenT3File(T3_DBS_CheckFileInfo t3FilePermission,T3_DBS_RTCheckFileInfo& t3RtnData, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pOpenT3File)
	{
		bRtn = m_pOpenT3File(t3FilePermission, t3RtnData, cr);
	}
	return bRtn;
}

BOOL CT3Business::ReadFileInfoFromIni(LPCTSTR lpstrPath, int& nPermission, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pReadFileInfoFromIni)
	{
		bRtn = m_pReadFileInfoFromIni(lpstrPath, nPermission, cr);
	}
	return bRtn;
}

BOOL CT3Business::GetT3FileInfo(LPCTSTR lpstrPath, int& nPermission, BOOL& bSetWatermark, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetT3FileInfo)
	{
		bRtn = m_pGetT3FileInfo(lpstrPath, nPermission, bSetWatermark, cr);
	}
	return bRtn;
}

BOOL CT3Business::GetT3FileInfo(LPCTSTR lpstrPath, LONGLONG& lRemainTime, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetT3FileRemainDate)
	{
		bRtn = m_pGetT3FileRemainDate(lpstrPath, lRemainTime, cr);
	}
	return bRtn;
}

BOOL CT3Business::GetDogData(DWORD& dwDogID, char* cKey, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetDogData)
	{
		bRtn = m_pGetDogData(dwDogID, cKey, cr);
	}
	return bRtn;
}

BOOL CT3Business::GetFileInfoFromServ(T3_DBS_CheckFileInfo t3FileInfo, LPTSTR lpXmlPath, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetFileInfoFromServ)
	{
		bRtn = m_pGetFileInfoFromServ(t3FileInfo, lpXmlPath, cr);
	}
	return bRtn;
}

BOOL CT3Business::GetWatermarkState(LPCTSTR lpstrPath, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetWatermarkState)
	{
		bRtn = m_pGetWatermarkState(lpstrPath, cr);
	}
	return bRtn;
}

BOOL CT3Business::GetAccreditInfo(Outside_CheckDog& DogInfo)
{
	BOOL bRtn = FALSE;
	if (m_pGetAccreditInfo)
	{
		bRtn = m_pGetAccreditInfo(DogInfo);
	}
	return bRtn;
}