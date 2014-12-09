// LogFileRecord.cpp: implementation of the CLogFileRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFileRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern LPTSTR GetGUID(LPTSTR strGUID);

CLogFileRecord::CLogFileRecord()
{
	m_strFileName = NULL;
	m_nLogType = LOG_FILE;
	m_strServerFilePath = NULL;
}

CLogFileRecord::~CLogFileRecord()
{
	if (m_strFileName)
	{
		delete[] m_strFileName;
		m_strFileName = NULL;
	}

	if (m_strServerFilePath)
	{
		delete[] m_strServerFilePath;
		m_strServerFilePath = NULL;
	}
}

void CLogFileRecord::RecordToMemory(CVirtualMemFile& mem)
{
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteString(m_strIndex);
	mem.WriteString(m_strComputer);
	mem.WriteString(m_strUser);
	mem.WriteString(m_strIP);
	mem.WriteString(m_strTime);
	mem.WriteString(m_strFileName);
	mem.WriteData(&m_nOperType,sizeof(int));
	mem.WriteData(&m_nSystemError,sizeof(int));
	mem.WriteData(&m_nInternalError,sizeof(int));
	mem.WriteData(&m_nDeleted,sizeof(int));
	mem.WriteString(m_strGUID);
	mem.WriteString(m_strServerFilePath);
}

void CLogFileRecord::MemoryToRecord(CVirtualMemFile& mem)
{
	mem.ReadData(&m_nLogType,sizeof(int));
	m_strIndex = mem.ReadString();
	m_strComputer = mem.ReadString();
	m_strUser = mem.ReadString();
	m_strIP = mem.ReadString();
	m_strTime = mem.ReadString();
	m_strFileName = mem.ReadString();
	mem.ReadData(&m_nOperType,sizeof(int));
	mem.ReadData(&m_nSystemError,sizeof(int));
	mem.ReadData(&m_nInternalError,sizeof(int));
	mem.ReadData(&m_nDeleted,sizeof(int));
	m_strGUID = mem.ReadString();
	m_strServerFilePath = mem.ReadString();
}

void CLogFileRecord::InitTestData()
{
//	共同信息
	m_strComputer = STRDUP(_T("Microsoft"));
	m_strUser = STRDUP(_T("admin"));
	m_strIP = STRDUP(_T("127.0.0.188"));
	m_strTime = STRDUP(_T("2007-11-23"));
	//GetSystemTime(&m_time);
// 文件日志信息
	m_nOperType = FILE_ENCRYPT;
	m_nSystemError = 0;
	m_nInternalError = 0;
	m_strFileName = STRDUP(_T("c:\\1.txt"));
}

LPTSTR CLogFileRecord::GetInsertSQL(LPTSTR strSQL)
{
#ifdef LOG_ARAM
#ifdef DATABASE_SUPPORT
	CAString strGuid;
	TCHAR tcGUID[40] = {0};
	GetGUID(tcGUID);
	strGuid = tcGUID;

	_stprintf(strSQL,_T("INSERT INTO Log_File (file_ip,file_computer,file_time,file_user,file_doc,file_oper,file_internal_error,file_system_error,file_deleted,manage_guid, file_serverpath, log_guid) SELECT '%s','%s','%s','%s','%s','%d','%d','%d',%d, '%s', '%s', '%s'"),
		m_strIP,m_strComputer,m_strTime,m_strUser,m_strFileName,m_nOperType,m_nInternalError,m_nSystemError,m_nDeleted,m_strGUID, m_strServerFilePath, strGuid);
	OutputDebugString(strSQL);

	ADODB::_ConnectionPtr ptrConnection = NULL;
	if(CWebDataFun::ExecuteSQL(ptrConnection, strSQL))
	{
		vecDGLogAlarmRule vecDGLogAlarmRuleList;
		vecDGLogAlarmRuleDetail vecDGLogAlarmRuleDetailList;
		if(ERROR_SUCCESS == CWebDataFun::GetLogAlarmRule(ptrConnection, vecDGLogAlarmRuleList, vecDGLogAlarmRuleDetailList, LOG_FILE))
		{
			BOOL bIsAlarmLog = FALSE;
			int	 nLogId = 0;
			int  nRuleId = 0;
			CAString strReceive = _T("");
			CWebDataFun::IsAlarmLog(ptrConnection, vecDGLogAlarmRuleDetailList, strGuid, LOG_FILE, bIsAlarmLog, nLogId, nRuleId, strReceive);

			if(TRUE == bIsAlarmLog)
			{
				CAString strInsertSql = _T("");
				strInsertSql.Format(_T("INSERT INTO LogAlarm_Record (CN_LogID, CN_RuleID, CN_State, CN_Type, CN_Receive) VALUES (%d, %d, 0, %d, '%s')"),
									nLogId,
									nRuleId,
									LOG_FILE,
									strReceive);

				CWebDataFun::ExecuteSQL(ptrConnection, strInsertSql);
			}
		}
	}
#endif
#endif
	return strSQL;

}