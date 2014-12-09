#include "StdAfx.h"
#include "SocketBase.h"
#include "LogFileRecordset.h"
#include "LogManageRecordset.h"
#include "LogPrintRecordset.h"
#include "LogSettingset.h"
#include "LogMailRecordset.h"
#include "LogLogRecordset.h"
#include "LogFileaccreditRecordset.h"
#include "LogOutSideRecordset.h"
#include "LogWorkRecordset.h"
#include "FileAccreditRecordset.h"

extern LPTSTR GetGUID(LPTSTR strGUID);

LPTSTR GetSnapshotPath()
{
	LPTSTR strSnapshotPath = new TCHAR[MAX_PATH];
	memset(strSnapshotPath,0,MAX_PATH);

	CRegKey key;
	if (key.Open(HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\{85767B7A-3294-E50E-A593-2096E94B8E76}\\Log\\LogPrint"),KEY_READ) == ERROR_SUCCESS)
	{
		TCHAR value[1024] = _T("");
		DWORD length = 1024;
		
		key.QueryValue(value,_T("Snapshotpath"),&length);
		if(key.QueryValue(value,_T("Snapshotpath"),&length) == ERROR_SUCCESS)
		{
			if(0 != _tcslen(value))
				_stprintf(strSnapshotPath, _T("%s"), value);
			else
			{
// 				TCHAR path[MAX_PATH];   
// 				::GetTempPath(MAX_PATH,path);   
// 				_stprintf(strSnapshotPath, _T("%s"), value);

				TCHAR	FolderPath[MAX_PATH] = {0};
				GetModuleFileName(NULL, FolderPath, MAX_PATH);
				CString strFolderPath = FolderPath;
				strFolderPath = strFolderPath.Left( strFolderPath.ReverseFind('\\') + 1);

				_stprintf(strSnapshotPath, _T("%s%s\\%s\\"), strFolderPath, _T("LOG"), _T("SnapshotPath"));
			}
		}
		else
		{
// 			TCHAR path[MAX_PATH];   
// 			::GetTempPath(MAX_PATH,path);   
// 			_stprintf(strSnapshotPath, _T("%s"), path);

			TCHAR	FolderPath[MAX_PATH] = {0};
			GetModuleFileName(NULL, FolderPath, MAX_PATH);
			CString strFolderPath = FolderPath;
			strFolderPath = strFolderPath.Left( strFolderPath.ReverseFind('\\') + 1);
			
			_stprintf(strSnapshotPath, _T("%s%s\\%s\\"), strFolderPath, _T("LOG"), _T("SnapshotPath"));
		}
	}else
	{
// 		TCHAR path[MAX_PATH];   
// 		::GetTempPath(MAX_PATH,path);   
// 		_stprintf(strSnapshotPath, _T("%s"), path);

		TCHAR	FolderPath[MAX_PATH] = {0};
		GetModuleFileName(NULL, FolderPath, MAX_PATH);
		CString strFolderPath = FolderPath;
		strFolderPath = strFolderPath.Left( strFolderPath.ReverseFind('\\') + 1);
		
		_stprintf(strSnapshotPath, _T("%s%s\\%s\\"), strFolderPath, _T("LOG"), _T("SnapshotPath"));
	}

	WIN32_FIND_DATA wfd;    
	HANDLE hFile = FindFirstFile(strSnapshotPath, &wfd);    
	if(hFile == INVALID_HANDLE_VALUE)  
	{ 
		int pos = 0;
		CString strPath = strSnapshotPath;
		while(TRUE)
		{
			pos = strPath.Find('\\',pos);
			
			if (pos == -1)
				break;
			CString strDir = strPath.Left(pos++);
			CreateDirectory(strDir,NULL);
		}
	}
	return strSnapshotPath;
}

// void ReadSnapshotFile(CIPCSocket& s)
// {
// 	TCHAR strSnapshotName[BUFF_SIZE] = {0};
// 	TCHAR strSnapshotPath[BUFF_SIZE] = {0};
// 	if (!CSocketBase::RecvStringData(s,strSnapshotName,BUFF_SIZE,NULL))
// 		return;
// 
// //	LPTSTR lpSnapshotPath = GetSnapshotPath();
// 
// //	_stprintf(strSnapshotPath, "%s%s", lpSnapshotPath, strSnapshotName);
// 
// 	HANDLE hFile = CreateFile(strSnapshotName,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
// 
// 	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
// 	{
// 		LPBYTE btData = NULL;
// 		DWORD dwSize = 0;
// 		DWORD wrote = 0;
// 		
// 		dwSize = GetFileSize(hFile, NULL);
// 		btData = (LPBYTE)calloc(dwSize ,sizeof(BYTE));
// 		ReadFile(hFile,btData,dwSize,&wrote,NULL);
// 
// 		CSocketBase::SendDataEx(s,btData,dwSize);
// 		
// 		delete[] btData;
// 		CloseHandle(hFile);
// 	}
// }

void RecvBatchLogData(CIPCSocket& s)
{
	OutputDebugString(L"SendBatchLogData");

#ifdef MEMORY_DETECT
	CMemoryState state1;
	CMemoryState state2;
	CMemoryState diff;
	state1.Checkpoint();
#endif
	//上传的记录类型
	int nRecordType = 0;
	if (!CSocketBase::RecvData(s,&nRecordType,sizeof(int)))
		return;

	//数据包
	LPBYTE btRecordData = NULL;
	int dwDataSize = 0;
	if (!CSocketBase::RecvDataEx(s,&btRecordData,dwDataSize))
		return;

	//////////////////////////////////////////////////////////////////////////
	//解析数据
	CLogRecordset* pRecordset = NULL;

	switch(nRecordType)
	{
	case LOG_FILE:
		pRecordset = new CLogFileRecordset;
		break;
	case LOG_PRINT:
		pRecordset = new CLogPrintRecordset;
		break; 
	case LOG_MAIL:
		pRecordset = new CLogMailRecordset;
		break;
	case LOG_OUTSIDE:
		pRecordset = new CLogOutSideRecordset;
		break;
	case LOG_USB:
		break;
	case LOG_MANAGE:
		pRecordset = new CLogManageRecordset;
		break;
	case LOG_LOG:
		pRecordset = new CLogLogRecordset;
		break;
	case LOG_FILEACCREDIT:
		pRecordset = new CLogFileaccreditRecordset;
		break;
// 	case LOG_WORK:
// 		pRecordset = new CLogWorkRecordset;
// 		break;
// 	case FILE_ACCREDIT:
// 		pRecordset = new CFileAccreditRecordset;
// 		break;
	default:
		{
			delete pRecordset;
			delete[] btRecordData;
			return;
		}
		break;
	}

	LPBYTE btData = btRecordData;
	DWORD PrevPos = 0;
	while(TRUE)
	{
		pRecordset->GetDataFromBuffer(btRecordData,dwDataSize);
		pRecordset->WriteToDatabase(nRecordType);
		if (pRecordset->m_dwNextPos < PrevPos)
			break;

		PrevPos = pRecordset->m_dwNextPos;

		if (pRecordset->m_dwNextPos < dwDataSize)
		{
			btRecordData+=pRecordset->m_dwNextPos;
			dwDataSize -= pRecordset->m_dwNextPos;
			pRecordset->m_dwNextPos = 0;
		}
		else
			break;
		pRecordset->ClearMemory();
	}
	
	delete pRecordset;
	delete[] btData;

#ifdef MEMORY_DETECT
	state2.Checkpoint();
	diff.Difference(state1,state2);
	diff.DumpStatistics();
#endif
	//////////////////////////////////////////////////////////////////////////
}

// void GetLogDataSQL(CIPCSocket& s)
// {
// 	CLogRecordset* pRecordset = NULL;
// 	TCHAR strTotalSQL[BUFF_SIZE] = {0};
// 	if (!CSocketBase::RecvStringData(s,strTotalSQL,BUFF_SIZE,NULL))
// 		return;
// 	
// 	TCHAR strSQL[BUFF_SIZE] = {0};
// 	if (!CSocketBase::RecvStringData(s,strSQL,BUFF_SIZE,NULL))
// 		return;
// 	
// 	if(_tcscmp(strSQL,_T("0")) == 0 || _tcscmp(strSQL,_T("")) == 0)
// 	{
// 		TCHAR strPerPageNum[BUFF_SIZE] = {0};
// 		if (!CSocketBase::RecvStringData(s,strPerPageNum,BUFF_SIZE,NULL))
// 			return;
// 
// 		int nRecordType = 0;	
// 		if (!CSocketBase::RecvData(s,&nRecordType,sizeof(int)))
// 			return;
// 
// 		if(nRecordType == FILE_ACCREDIT)
// 		{
// 			pRecordset = new CFileAccreditRecordset;
// 			pRecordset->ReadFromDatabase(strTotalSQL, strSQL);
// 			
// 			LPBYTE btData = NULL;
// 			DWORD dwSize = 0;
// 			pRecordset->SetDataToBuffer(&btData,dwSize);
// 
// 			CSocketBase::SendDataEx(s,btData,dwSize);
// 			
// 			delete[] btData;
// 			delete pRecordset;
// 
// 			return;
// 		}
// 	}
// 
// 	int nRecordType = 0;	
// 	if (!CSocketBase::RecvData(s,&nRecordType,sizeof(int)))
// 		return;
// 
// #ifndef TEST_PROJECT
// 	switch(nRecordType)
// 	{
// 	case LOG_FILE:
// 		pRecordset = new CLogFileRecordset;
// 		break;
// 	case LOG_MANAGE:
// 		pRecordset = new CLogManageRecordset;
// 		break;
// 	case LOG_PRINT:
// 		pRecordset = new CLogPrintRecordset;
// 		break;
// 	case LOG_MAIL:
// 		pRecordset = new CLogMailRecordset;
// 		break;
// 	case LOG_LOG:
// 		pRecordset = new CLogLogRecordset;
// 		break;
// 	case LOG_FILEACCREDIT:
// 		pRecordset = new CLogFileaccreditRecordset;
// 		break;
// 	case LOG_OUTSIDE:
// 		pRecordset = new CLogOutSideRecordset;
// 		break;
// 	case LOG_WORK:
// 		pRecordset = new CLogWorkRecordset;
// 		break;
// 	case FILE_ACCREDIT:
// 		pRecordset = new CFileAccreditRecordset;
// 		break;
// // 	case FILE_UPLOAD:
// // 		{
// // 			CFileUploadRecordset * pRecordset = NULL;
// // 			pRecordset = new CFileUploadRecordset;
// // 
// // 			pRecordset->ReadFromDatabase(strSQL);
// // 			
// // 			LPBYTE btData = NULL;
// // 			DWORD dwSize = 0;
// // 			pRecordset->SetDataToBuffer(&btData,dwSize);
// // 
// // 			CSocketBase::SendDataEx(s,btData,dwSize);
// // 			
// // 			delete[] btData;
// // 			delete pRecordset;
// // 			return;
// // 		}
// // 		break;
// 	default:
// 		break;
// 	}
// 	pRecordset->ReadFromDatabase(strTotalSQL, strSQL);
// 
// 	LPBYTE btData = NULL;
// 	DWORD dwSize = 0;
// 	pRecordset->SetDataToBuffer(&btData,dwSize);
// #else
// 	LPBYTE btData = NULL;
// 	DWORD dwSize = 0;
// 
// 	switch(nRecordType)
// 	{
// 	case LOG_FILE:
// 		{
// 			if (g_pLogFileRecordset)
// 				g_pLogFileRecordset->SetDataToBuffer(&btData,dwSize);
// 		}
// 		break;
// 	case LOG_MANAGE:
// 		{
// 			if (g_pLogManageRecordset)
// 				g_pLogManageRecordset->SetDataToBuffer(&btData,dwSize);
// 		}
// 		break;
// 	case LOG_PRINT:
// 		{
// 		if (g_pLogPrintRecordset)
// 				g_pLogPrintRecordset->SetDataToBuffer(&btData,dwSize);
// 		}
// 		break;
// 	case LOG_MAIL:
// 		{
// 			if (g_pLogMailRecordset)
// 				g_pLogMailRecordset->SetDataToBuffer(&btData,dwSize);
// 		}
// 		break;
// 	case LOG_LOG:
// 		{
// 			if (g_pLogLogRecordset)
// 				g_pLogLogRecordset->SetDataToBuffer(&btData,dwSize);
// 		}
// 		break;
// 	case FILE_ACCREDIT:
// 		{
// 			if (g_pFileAccreditRecordset)
// 				g_pFileAccreditRecordset->SetDataToBuffer(&btData,dwSize);
// 		}
// 		break;
// 	case FILE_UPLOAD:
// 		{
// 			if(g_pFileUploadRecordset)
// 			   g_pFileUploadRecordset->SetDataToBuffer(&btData,dwSize);	
// 		}
// 		break;
// 
// #endif
// 
// 	CSocketBase::SendDataEx(s,btData,dwSize);
// 
// 	delete[] btData;
// 	delete pRecordset;
// }
// 
// void ExecuteSQL(CIPCSocket& s)
// {
// 	TCHAR strExecuteSQL[BUFF_SIZE] = {0};
// 	LPTSTR strSQL = NULL;
// 	if (!CSocketBase::RecvStringData(s,strExecuteSQL,BUFF_SIZE,&strSQL))
// 		return;
// 	if (strSQL == NULL)
// 		CDatabaseConnect::m_pRecordset->Open(_variant_t(strExecuteSQL), _variant_t((IDispatch *)CDatabaseConnect::m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
// 	else
// 	{	
// 		CDatabaseConnect::m_pRecordset->Open(_variant_t(strSQL), _variant_t((IDispatch *)CDatabaseConnect::m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
// 		delete[] strSQL;
// 	}
// }
// 
// void GetLogSettingData(CIPCSocket& s)
// {
// 	CLogSettingset* pSetting = new CLogSettingset;
// 
// 	pSetting->ReadFromDatabase(_T("Select count(*) from Log_Setting"),_T("Select * from Log_Setting"));
// 	
// 	LPBYTE btData = NULL;
// 	DWORD dwSize = 0;
// 	pSetting->SetDataToBuffer(&btData,dwSize);
// 
// 	CSocketBase::SendDataEx(s,btData,dwSize);
// 	delete pSetting;
// 	delete[] btData;
// }
// 
void GetSnapshotFile(CIPCSocket& s)
{
	int nFileCount = 0;
	if (!CSocketBase::RecvData(s,&nFileCount,sizeof(int)))
		return;
	for (int i=0;i<nFileCount;i++)
	{
		if (!CSocketBase::RecvFileToFolder(s,GetSnapshotPath()))
			return;
	}
}

void GetPrintLogData(CIPCSocket& s)
{
	OutputDebugString(L"SendPrintLogData");

	int nRecordCount = 0;

	CAString strGuid;
	TCHAR tcGUID[40] = {0};
	GetGUID(tcGUID);
	strGuid = tcGUID;
	
	if(!CSocketBase::RecvData(s, &nRecordCount, sizeof(int)))
		return;
	
	ADODB::_ConnectionPtr ptrConnection = NULL;
	
	for(int nfor = 0; nfor < nRecordCount; nfor++)
	{
		DG_Log_Print_Record stLogPrintRecord;
		
		CSocketBase::RecvData(s, &stLogPrintRecord, sizeof(DG_Log_Print_Record));
		
		LPTSTR strSnapshotPath = GetSnapshotPath();
		
		CAString strSQL;
		CAString strPrintSnapshot = stLogPrintRecord.wcPrintSnapshot;
		
		if (strPrintSnapshot.IsEmpty())
		{
			strSQL.Format(_T("INSERT INTO Log_Print \
													  (print_ip, print_computer, print_time, print_user, print_doc, print_deleted, log_guid) \
													  VALUES ('%s', '%s', '%s', '%s', '%s', 0, '%s')"),
													  stLogPrintRecord.wcPrintIP,
													  stLogPrintRecord.wcPrintComputer,
													  stLogPrintRecord.wcPrintTime,
													  stLogPrintRecord.wcPrintUser,
													  stLogPrintRecord.wcPrintDocName,
													  strGuid);
			OutputDebugString(strSQL);
		}
		else
		{
			strSQL.Format(_T("INSERT INTO Log_Print \
													  (print_ip, print_computer, print_time, print_user, print_doc, print_snapshot, print_deleted, log_guid) \
													  VALUES ('%s', '%s', '%s', '%s', '%s', '%s%s', 0, '%s')"),
													  stLogPrintRecord.wcPrintIP,
													  stLogPrintRecord.wcPrintComputer,
													  stLogPrintRecord.wcPrintTime,
													  stLogPrintRecord.wcPrintUser,
													  stLogPrintRecord.wcPrintDocName,
													  strSnapshotPath,
													  stLogPrintRecord.wcPrintSnapshot,
													  strGuid);
			
			OutputDebugString(strSQL);
		}
		
		if(CWebDataFun::ExecuteSQL(ptrConnection, strSQL))
		{
			vecDGLogAlarmRule vecDGLogAlarmRuleList;
			vecDGLogAlarmRuleDetail vecDGLogAlarmRuleDetailList;
			if(ERROR_SUCCESS == CWebDataFun::GetLogAlarmRule(ptrConnection, vecDGLogAlarmRuleList, vecDGLogAlarmRuleDetailList, LOG_PRINT))
			{
				BOOL bIsAlarmLog = FALSE;
				int	 nLogId = 0;
				int  nRuleId = 0;
				CAString strReceive = _T("");
				CWebDataFun::IsAlarmLog(ptrConnection, vecDGLogAlarmRuleDetailList, strGuid, LOG_PRINT, bIsAlarmLog, nLogId, nRuleId, strReceive);
				
 				if(TRUE == bIsAlarmLog)
 				{
					CAString strInsertSql = _T("");
					strInsertSql.Format(_T("INSERT INTO LogAlarm_Record (CN_LogID, CN_RuleID, CN_State, CN_Type, CN_Receive) VALUES (%d, %d, 0, %d, '%s')"),
						nLogId,
						nRuleId,
						LOG_PRINT,
						strReceive);
					
					CWebDataFun::ExecuteSQL(ptrConnection, strInsertSql);
				}
			}
		}			
	}
}

void GetAuthrozeFileLog(CIPCSocket& s)
{
	int nRecordCount = 0;
	
	CAString strGuid;
	TCHAR tcGUID[40] = {0};
	GetGUID(tcGUID);
	strGuid = tcGUID;
	ADODB::_ConnectionPtr ptrConnection = NULL;
	
	if(!CSocketBase::RecvData(s, &nRecordCount, sizeof(int)))
		return;
	
	for(int nfor = 0; nfor < nRecordCount; nfor++)
	{
		DG_Log_FileOperation_Record stLogFileOperation = {0};
		DBS_FileInfo stAuturozeFileInfo = {0};

		//接收授权日志基本信息
		if(!CSocketBase::RecvData(s, &stLogFileOperation, sizeof(DG_Log_FileOperation_Record)))
			return;

		_tcscpy(stLogFileOperation.wcLogGuid, strGuid);
		CWebDataFun::WriteLogFileToDB(ptrConnection, stLogFileOperation);

		int nSLevelCount = 0;
		//接收文件密级对应的权限数量
		if(!CSocketBase::RecvData(s, &nSLevelCount, sizeof(int)))
			return;

		for(int i = 0; i < nSLevelCount; i++)
		{
			DBS_SLevelInfo stSLevelInfo = {0};
			//接收文件密级对应的权限
			if(!CSocketBase::RecvData(s, &stSLevelInfo, sizeof(DBS_SLevelInfo)))
				return;

			CWebDataFun::WriteLogSLevelToDB(ptrConnection, stSLevelInfo, strGuid);
		}

		int nPermissionCount = 0;
		//接收特殊权限的数量
		if(!CSocketBase::RecvData(s, &nPermissionCount, sizeof(int)))
			return;

		for(int j = 0; j < nPermissionCount; j++)
		{
			DBS_FilePermission stFilePermission = {0};
			//接收特殊权限
			if(!CSocketBase::RecvData(s, &stFilePermission, sizeof(DBS_FilePermission)))
				return;

			CWebDataFun::WriteLogFilePermissionToDB(ptrConnection, stFilePermission, strGuid);
		}
	}

	ptrConnection->Close();
}