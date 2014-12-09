// LogLogRecord.cpp: implementation of the CLogLogRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogLogRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogLogRecord::CLogLogRecord()
{
	m_nOperType = 0;
	m_nLogType = 0;
}

CLogLogRecord::~CLogLogRecord()
{

}

void CLogLogRecord::RecordToMemory(CVirtualMemFile& mem)
{
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteString(m_strIndex);
	mem.WriteString(m_strComputer);
	mem.WriteString(m_strUser);
	mem.WriteString(m_strIP);
	mem.WriteString(m_strTime);
	mem.WriteData(&m_nOperType,sizeof(int));
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteData(&m_nDeleted,sizeof(int));
	mem.WriteString(m_strGUID);
}

void CLogLogRecord::MemoryToRecord(CVirtualMemFile& mem)
{
	mem.ReadData(&m_nLogType,sizeof(int));
	m_strIndex = mem.ReadString();
	m_strComputer = mem.ReadString();
	m_strUser = mem.ReadString();
	m_strIP = mem.ReadString();
	m_strTime = mem.ReadString();
	mem.ReadData(&m_nOperType,sizeof(int));
	mem.ReadData(&m_nLogType,sizeof(int));
	mem.ReadData(&m_nDeleted,sizeof(int));
	m_strGUID = mem.ReadString();
}

LPTSTR CLogLogRecord::GetInsertSQL(LPTSTR strSQL)
{
	_stprintf(strSQL,_T("INSERT INTO Log_LogManage (manage_ip,manage_computer,manage_time,manage_user,manage_oper,manage_logtype,manage_deleted,manage_guid) SELECT '%s','%s','%s','%s','%d','%d',%d, '%s'"),
			  m_strIP,m_strComputer,m_strTime,m_strUser,m_nOperType,m_nLogType,m_nDeleted,m_strGUID);
	return strSQL;
}

void CLogLogRecord::InitTestData()
{

}