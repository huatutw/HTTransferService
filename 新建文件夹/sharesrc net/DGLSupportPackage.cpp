#include "StdAfx.h"
#include "DGLSupportPackage.h"
#include <WinSock2.h>
//#include "DGCommonFunc.h"
CDGLSupportPackage::CDGLSupportPackage(void)
{

	m_hInst=NULL;
	m_hInst = LoadLibrary(L"DGLocalSupport.dll");
	if(m_hInst==NULL)
	{
		CErrorRecorder cr;
		_DG_ERROR_PUSH(cr,DGCrypt_LoadDGLocalSupportDllFailed);
	}
	m_pCheckUserPassword		= NULL;
	m_pTestLogServer				= NULL;
	m_pTestValidateServer			= NULL;
	m_pTestUpdateServer			= NULL;
	m_pTestPermissionServer		= NULL;
	m_pSendData						= NULL;
	m_pModifyUserPass				= NULL;
	m_pCheckDogDefault			= NULL;
	m_pCheckDogInfo				= NULL;
	m_pGetOfflineInfo				= NULL;
	m_pGetOfflineInfo1				= NULL;
	m_pRequestOutofNetWork	= NULL;
	m_pRequestOutofNetWork1	= NULL;
	m_pGetSystemParameter		= NULL;
	m_pGetDGWebSetFile			= NULL;
	m_pSendArrayData				= NULL;
	m_pCheckSoftOffline			= NULL;
	m_pSetAlertsInfo					= NULL;
	m_pUploadFileToServer			= NULL;
	m_pCheckOfflineInfo			= NULL;
	m_pSendArrayFiles				= NULL;
	m_pRecvUserCfgFile				= NULL;	
	m_pRecvDGLandingInitFile	= NULL;
	m_pRecvNewestInitData		= NULL;
	m_pSubmitFirstLogin			= NULL;
	m_pRecvWarterMarkModule	= NULL;
	m_pGetModules					= NULL;
	m_pSendDSMFiles				= NULL;
	m_pGetRoleCfgFile				= NULL;
	InitFunction(m_hInst);
}

CDGLSupportPackage::~CDGLSupportPackage(void)
{
	if (m_hInst)
	{
		FreeLibrary(m_hInst);
	}
}

BOOL CDGLSupportPackage::InitNetwork()
{
	WSADATA wsaData;
	CErrorRecorder cr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
	{
		_DG_ERROR_PUSH(cr,DGCrypt_InitNetServiceFailed);
		return FALSE;
	}
	return TRUE;

}

BOOL CDGLSupportPackage::UnInitNetwork()
{
	int nResult = WSACleanup();
	if (nResult != 0)
	{
		CErrorRecorder cr;
		_DG_ERROR_PUSH(cr,DGCrypt_CleanNetSocketFailed);
		return FALSE;
	}
	return TRUE;
}

void CDGLSupportPackage::InitFunction(HMODULE hInst)
{
	if (hInst)
	{
		m_pCheckUserPassword		= (DGLP_CheckUserPassword)GetProcAddress(hInst, "CheckUserPassword");
		m_pTestLogServer				= (DGLP_TestLogServer)GetProcAddress(hInst, "TestLogServer");
		m_pTestValidateServer			= (DGLP_TestValidateServer)GetProcAddress(hInst, "TestValidateServer");
		m_pTestUpdateServer			= (DGLP_TestUpdateServer)GetProcAddress(hInst, "TestUpdateServer");
		m_pTestPermissionServer		= (DGLP_TestPermissionServer)GetProcAddress(hInst, "TestPermissionServer");
		m_pSendData						= (DGLP_SendData)GetProcAddress(hInst, "SendData");
		m_pModifyUserPass				= (DGLP_ModifyUserPass)GetProcAddress(hInst, "ModifyUserPass");
		m_pCheckDogDefault			= (DGLP_CheckDogDefault)GetProcAddress(hInst, "CheckDogDefault");
		m_pCheckDogInfo				= (DGLP_CheckDogInfo)GetProcAddress(hInst, "CheckDogInfo");
		m_pGetOfflineInfo				= (DGLP_GetOfflineInfo)GetProcAddress(hInst, "GetOfflineInfo");
		m_pGetOfflineInfo1				= (DGLP_GetOfflineInfo1)GetProcAddress(hInst, "GetOfflineInfo1");
		m_pRequestOutofNetWork	= (DGLP_RequestOutofNetWork)GetProcAddress(hInst, "RequestOutofNetWork");
		m_pRequestOutofNetWork1	= (DGLP_RequestOutofNetWork1)GetProcAddress(hInst, "RequestOutofNetWork1");
		m_pGetSystemParameter		= (DGLP_GetSystemParameter)GetProcAddress(hInst, "GetSystemParameter");
		m_pGetDGWebSetFile			= (DGLP_GetDGWebSetFile)GetProcAddress(hInst,"GetDGWebSetFile");
		m_pSendArrayData				= (DGLP_SendArrayData)GetProcAddress(hInst, "SendArrayData");
		m_pCheckSoftOffline			= (DGLP_CheckSoftOffline)GetProcAddress(hInst, "CheckSoftOffline");
		m_pSetAlertsInfo					= (DGLP_SetAlertsInfo)GetProcAddress(hInst,"SetAlertsInfo");
		m_pUploadFileToServer			= (DGLP_UploadFileToServer)GetProcAddress(hInst, "UploadFileToServer");
		m_pCheckOfflineInfo			= (DGLP_CheckOfflineInfo)GetProcAddress(hInst, "CheckOfflineInfo");
		m_pSendArrayFiles				= (DGLP_SendArrayFiles)GetProcAddress(hInst, "SendArrayFiles");
		m_pRecvUserCfgFile				= (DGLP_RecvUserCfgFile)GetProcAddress(hInst, "RecvUserCfgFile");
		m_pRecvDGLandingInitFile	= (DGLP_RecvDGLandingInitFile)GetProcAddress(hInst, "RecvDGLandingInitFile");
		m_pRecvNewestInitData		= (DGLP_RecvNewestInitData)GetProcAddress(hInst, "RecvNewestInitData");
		m_pTestServer					= (DGLP_TestServer)GetProcAddress(hInst, "TestServer");
		m_pCheckFirstLogin				= (DGLP_CheckFirstLogin)GetProcAddress(hInst, "CheckFirstLogin");
		m_pSubmitFirstLogin			= (DGLP_SubmitFirstLogin)GetProcAddress(hInst, "SubmitFirstLogin");
		m_pRecvWarterMarkModule	= (DGLP_RecvWarterMarkModule)GetProcAddress(hInst, "RecvWarterMarkModule");
		m_pGetModules					= (DGLP_GetModules)GetProcAddress(hInst, "GetModules");
		m_pSendDSMFiles				= (DGLP_SendDSMFiles)GetProcAddress(hInst, "SendDSMFiles");
		m_pGetRoleCfgFile				= (DGLP_GetRoleCfgFile)GetProcAddress(hInst, "GetRoleCfgFile");
	}
}

BOOL CDGLSupportPackage::CheckUserPassword(LPTSTR strUser,LPTSTR strPass,LPSTR lpIp,DWORD dwPort,CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pCheckUserPassword)
	{
		bRtn = m_pCheckUserPassword(strUser, strPass, lpIp, dwPort, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::TestServer(LPSTR lpIp,UINT uPort, LPTSTR lpRetStr ,CErrorRecorder& cr)
{
	
	BOOL bRtn = FALSE;
	if (m_pTestServer)
	{
		bRtn = m_pTestServer(lpIp, uPort, lpRetStr, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::TestLogServer(LPSTR lpIp,UINT uPort, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pTestLogServer)
	{
		bRtn = m_pTestLogServer(lpIp, uPort, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::TestValidateServer(LPSTR lpIP,UINT uPort, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pTestValidateServer)
	{
		bRtn = m_pTestValidateServer(lpIP, uPort, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::TestUpdateServer(LPSTR lpIP,UINT uPort, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pTestUpdateServer)
	{
		bRtn = m_pTestUpdateServer(lpIP, uPort, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::TestPermissionServer(LPSTR lpIP,UINT uPort, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pTestPermissionServer)
	{
		bRtn = m_pTestPermissionServer(lpIP, uPort, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::SendData(LPSTR lpIP,UINT uPort,LPTSTR lpCmd,LPVOID lpData, int nLen, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pSendData)
	{
		bRtn = m_pSendData(lpIP, uPort,lpCmd, lpData, nLen, cr);
	}
	return bRtn;
}

int CDGLSupportPackage::ModifyUserPass(LPCTSTR strUser,LPCTSTR strPass,LPSTR lpIp,DWORD dwPort,LPTSTR strNewPass, CErrorRecorder& cr)
{
	int nRet = -1;
	if (m_pModifyUserPass)
	{
		nRet = m_pModifyUserPass(strUser, strPass, lpIp, dwPort, strNewPass, cr);
	}
	return nRet;
}

BOOL CDGLSupportPackage::CheckDogDefault(LPSTR lpIP,LPTSTR lpLoginName,LPTSTR lpClientId,DWORD dwPort, Offline_Info2& outInfo, CErrorRecorder& cr)
{

	BOOL bRtn = FALSE;
	if (m_pCheckDogDefault)
	{
		bRtn = m_pCheckDogDefault(lpIP, lpLoginName, lpClientId, dwPort, outInfo, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::CheckDogInfo(LPTSTR strUser,LPTSTR strPass,LPSTR strIP, LPTSTR lpClientId,DWORD dwPort, BOOL& bConnect, Offline_Info2& outInfo, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pCheckDogInfo)
	{
		bRtn = m_pCheckDogInfo(strUser, strPass, strIP, lpClientId, dwPort, bConnect ,outInfo, cr);

	}
	return bRtn;
}

int CDGLSupportPackage::GetOfflineInfo(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR strLoginName, CErrorRecorder& cr)
{
	int nRtn = 0;
	if (m_pGetOfflineInfo)
	{
		nRtn = m_pGetOfflineInfo(lpSvrIp, dwSvrPort, strLoginName, cr);
	}
	return nRtn;
}

int CDGLSupportPackage::GetOfflineInfo1(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR strLoginName, LPTSTR lpCLientId, CErrorRecorder& cr)
{
	int nRtn = 0;
	if (m_pGetOfflineInfo1)
	{
		nRtn = m_pGetOfflineInfo1(lpSvrIp, dwSvrPort, strLoginName, lpCLientId, cr);
	}
	return nRtn;
}

int CDGLSupportPackage::RequestOutofNetWork(LPSTR lpSvrIp, DWORD dwSvrPort, LPTSTR lpClientID,CErrorRecorder& cr)
{
	int nRtn = 0;
	if (m_pRequestOutofNetWork)
	{
		nRtn = m_pRequestOutofNetWork(lpSvrIp, dwSvrPort, lpClientID, cr);
	}
	return nRtn;
}

int CDGLSupportPackage::RequestOutofNetWork1(LPSTR lpSvrIp, DWORD dwSvrPort, LPTSTR lpClientID,CErrorRecorder& cr)
{
	int nRtn = 0;
	if (m_pRequestOutofNetWork1)
	{
		nRtn = m_pRequestOutofNetWork1(lpSvrIp, dwSvrPort, lpClientID, cr);
	}
	return nRtn;
}



BOOL CDGLSupportPackage::GetSystemParameter(LPSTR lpSvrIp, DWORD dwSvrPort,int &nFlushInfo, int &nCheckDog, int &nNetError ,CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetSystemParameter)
	{
		bRtn = m_pGetSystemParameter(lpSvrIp, dwSvrPort, nFlushInfo, nCheckDog, nNetError,cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::GetDGWebSetFile(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpPath, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetDGWebSetFile)
	{
		bRtn = m_pGetDGWebSetFile(lpSvrIp, dwSvrPort, lpPath, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::SendArrayData(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd,int nArraryCount,LPVOID lpArray, int nLen, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pSendArrayData)
	{
		bRtn = m_pSendArrayData(lpSvrIp, dwSvrPort, lpCmd, nArraryCount, lpArray,nLen, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::CheckSoftOffline(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpUser, LPTSTR lpClientId,BOOL &bHaveSoftOffline, CErrorRecorder& cr)
{
	BOOL bRtn;
	if (m_pCheckSoftOffline)
	{
		bRtn = m_pCheckSoftOffline(lpSvrIp, dwSvrPort, lpUser, lpClientId, bHaveSoftOffline, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::SetAlertsInfo(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpUser, BOOL bRemindOnce,LPTSTR lpMinute, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pSetAlertsInfo)
	{
		bRtn = m_pSetAlertsInfo(lpSvrIp, dwSvrPort, lpUser, bRemindOnce, lpMinute, cr);
	}
	return bRtn;
}



BOOL CDGLSupportPackage::UploadFileToServer(LPSTR lpSvrIp, DWORD dwSvrPort,int nRecordType, LPTSTR lpPath, LPTSTR lpUserName, LPTSTR lpServerFilePath,CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pUploadFileToServer)
	{
		bRtn = m_pUploadFileToServer(lpSvrIp, dwSvrPort, nRecordType, lpPath, lpUserName, lpServerFilePath, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::CheckOfflineInfo(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpClientId,BOOL bAddDogCount,LPTSTR lpUserName, int& nType,CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pCheckOfflineInfo)
	{
		bRtn = m_pCheckOfflineInfo(lpSvrIp, dwSvrPort, lpClientId, bAddDogCount,lpUserName,nType, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::SendArrayFiles(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd,int nArraryCount,wstring* strFiles, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pSendArrayFiles)
	{
		bRtn = m_pSendArrayFiles(lpSvrIp, dwSvrPort, lpCmd, nArraryCount, strFiles, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::RecvUserCfgFile(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd, CAString strUserName, CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	if (m_pRecvUserCfgFile)
	{
		bRtn = m_pRecvUserCfgFile(lpSvrIp, dwSvrPort, lpCmd, strUserName, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::RecvDGLandingInitFile(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd,CAString strCommand,CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	if (m_pRecvDGLandingInitFile)
	{
		bRtn = m_pRecvDGLandingInitFile(lpSvrIp,dwSvrPort,lpCmd, strCommand, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::RecvNewestInitData(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd, CAString strUsrName, DG_Approval* deqDGApprovalList, int& nLen,CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	if (m_pRecvNewestInitData)
	{
		bRtn = m_pRecvNewestInitData(lpSvrIp,dwSvrPort,lpCmd, strUsrName,deqDGApprovalList, nLen, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::CheckFirstLogin(LPSTR lpSvrIp, DWORD dwSvrPort ,LPCTSTR lpUser, BOOL& bFirstLogin,CErrorRecorder &cr)
{

	BOOL bRtn = FALSE;
	if (m_pCheckFirstLogin)
	{
		bRtn = m_pCheckFirstLogin(lpSvrIp,dwSvrPort,lpUser,bFirstLogin, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::SubmitFirstLogin(LPSTR lpSvrIp, DWORD dwSvrPort ,LPCTSTR lpUser, CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	if (m_pSubmitFirstLogin)
	{
		bRtn = m_pSubmitFirstLogin(lpSvrIp,dwSvrPort,lpUser, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::RecvWarterMarkModule(LPSTR lpSvrIp, DWORD dwSvrPort , CErrorRecorder &cr)
{

	BOOL bRtn = FALSE;
	if (m_pRecvWarterMarkModule)
	{
		bRtn = m_pRecvWarterMarkModule(lpSvrIp,dwSvrPort, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::GetModules(LPSTR lpSvrIp, DWORD dwSvrPort ,DWORD& dwModules, CErrorRecorder &cr)
{

	BOOL bRtn = FALSE;
	if (m_pGetModules)
	{
		bRtn = m_pGetModules(lpSvrIp,dwSvrPort,dwModules, cr);
	}
	return bRtn;
}
BOOL CDGLSupportPackage::SendDSMFiles(LPSTR lpSvrIp, DWORD dwSvrPort ,LPTSTR pXmlPath, CErrorRecorder &cr)
{

	BOOL bRtn = FALSE;
	if (m_pSendDSMFiles)
	{
		bRtn = m_pSendDSMFiles(lpSvrIp,dwSvrPort,pXmlPath, cr);
	}
	return bRtn;
}

BOOL CDGLSupportPackage::GetRoleCfgFile(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd, CAString strUserName, CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetRoleCfgFile)
	{
		bRtn = m_pGetRoleCfgFile(lpSvrIp, dwSvrPort, lpCmd, strUserName, cr);
	}
	return bRtn;
}