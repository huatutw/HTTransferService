// LogPrintRecord.cpp: implementation of the CLogPrintRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogPrintRecord.h"
#include "atlbase.h"

extern LPTSTR GetGUID(LPTSTR strGUID);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogPrintRecord::CLogPrintRecord()
{
	m_strPrintFileName = NULL;
	m_strSnapshot = NULL;
}

CLogPrintRecord::~CLogPrintRecord()
{
	if (m_strPrintFileName)
	{
		delete[] m_strPrintFileName;
		m_strPrintFileName = NULL;
	}

	if (m_strSnapshot)
	{
		delete[] m_strSnapshot;
		m_strSnapshot = NULL;
	}
}

void CLogPrintRecord::RecordToMemory(CVirtualMemFile& mem)
{
	if (m_bUnicodeToAnsi)
	{
		mem.WriteData(&m_nLogType,sizeof(int));
		mem.WriteStringW(m_strIndex);
		mem.WriteStringW(m_strComputer);
		mem.WriteStringW(m_strUser);
		mem.WriteStringW(m_strIP);
		mem.WriteStringW(m_strTime);
		mem.WriteStringW(m_strPrintFileName);
		mem.WriteStringW(m_strSnapshot);
		mem.WriteData(&m_nDeleted,sizeof(int));
		mem.WriteStringW(m_strGUID);
	}else
	{
		mem.WriteData(&m_nLogType,sizeof(int));
		mem.WriteString(m_strIndex);
		mem.WriteString(m_strComputer);
		mem.WriteString(m_strUser);
		mem.WriteString(m_strIP);
		mem.WriteString(m_strTime);
		mem.WriteString(m_strPrintFileName);
		mem.WriteString(m_strSnapshot);
		mem.WriteData(&m_nDeleted,sizeof(int));
		mem.WriteString(m_strGUID);
	}
}

void CLogPrintRecord::MemoryToRecord(CVirtualMemFile& mem)
{
	mem.ReadData(&m_nLogType,sizeof(int));
	m_strIndex = mem.ReadString();
	m_strComputer = mem.ReadString();
	m_strUser = mem.ReadString();
	m_strIP = mem.ReadString();	
	m_strTime = mem.ReadString();
	m_strPrintFileName = mem.ReadString();
	m_strSnapshot = mem.ReadString();
	mem.ReadData(&m_nDeleted,sizeof(int));
	m_strGUID = mem.ReadString();
}

LPTSTR CLogPrintRecord::GetInsertSQL(LPTSTR strSQL)
{
#ifdef LOG_ARAM
#ifdef DATABASE_SUPPORT
	LPTSTR strSnapshotPath = GetSnapshotPath();
	
	CAString strGuid;
	TCHAR tcGUID[40] = {0};
	GetGUID(tcGUID);
	strGuid = tcGUID;

	_stprintf(strSQL,_T("INSERT INTO Log_Print (print_ip,print_computer,print_time,print_user,print_doc,print_snapshot,print_deleted,manage_guid,log_guid) SELECT '%s', '%s', '%s', '%s', '%s', '%s%s', %d, '%s', '%s'"),
			  m_strIP,m_strComputer,m_strTime,m_strUser,m_strPrintFileName,strSnapshotPath,m_strSnapshot,m_nDeleted,m_strGUID,strGuid);

	ADODB::_ConnectionPtr ptrConnection = NULL;
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
#endif
#endif
	return strSQL;
}

void CLogPrintRecord::InitTestData()
{
	
}

LPTSTR CLogPrintRecord::GetSnapshotPath()
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
/*				CString strFolderPath = FolderPath;*/
				CAString strFolderPath = FolderPath;
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
			/*CString strFolderPath = FolderPath;*/
			CAString strFolderPath = FolderPath;
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
		/*CString strFolderPath = FolderPath;*/
		CAString strFolderPath = FolderPath;
		strFolderPath = strFolderPath.Left( strFolderPath.ReverseFind('\\') + 1);
		
		_stprintf(strSnapshotPath, _T("%s%s\\%s\\"), strFolderPath, _T("LOG"), _T("SnapshotPath"));
	}
	
	WIN32_FIND_DATA wfd;    
	HANDLE hFile = FindFirstFile(strSnapshotPath, &wfd);    
	if(hFile == INVALID_HANDLE_VALUE)  
	{ 
		int pos = 0;
	/*	CString strPath = strSnapshotPath;*/
		CAString strPath = strSnapshotPath;
		while(TRUE)
		{
			pos = strPath.Find('\\',pos);
			
			if (pos == -1)
				break;
			/*CString strDir = strPath.Left(pos++);*/
			CAString strDir = strPath.Left(pos++);
			CreateDirectory(strDir,NULL);
		}
	}
	return strSnapshotPath;
}