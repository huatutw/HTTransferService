// LogManageRecord.cpp: implementation of the CLogManageRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogManageRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogManageRecord::CLogManageRecord()
{
	m_nLogType = LOG_MANAGE;
	m_nRecordType = 0;
	m_strDescript = NULL;
	m_strUserObject = NULL;
}

CLogManageRecord::~CLogManageRecord()
{
	if (m_strDescript)
	{
		delete[] m_strDescript;
		m_strDescript = NULL;
	}

	if (m_strUserObject)
	{
		delete[] m_strUserObject;
		m_strUserObject = NULL;
	}
}

void CLogManageRecord::RecordToMemory(CVirtualMemFile& mem)
{
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteString(m_strIndex);
	mem.WriteString(m_strComputer);
	mem.WriteString(m_strUser);
	mem.WriteString(m_strIP);
	mem.WriteString(m_strTime);
	mem.WriteData(&m_nRecordType,sizeof(int));
	mem.WriteString(m_strDescript);
	mem.WriteString(m_strUserObject);
	mem.WriteData(&m_nDeleted,sizeof(int));
	mem.WriteString(m_strGUID);
}
	
void CLogManageRecord::MemoryToRecord(CVirtualMemFile& mem)
{
	mem.ReadData(&m_nLogType,sizeof(int));
	m_strIndex = mem.ReadString();
	m_strComputer = mem.ReadString();
	m_strUser = mem.ReadString();
	m_strIP = mem.ReadString();
	m_strTime = mem.ReadString();
	mem.ReadData(&m_nRecordType,sizeof(int));
	m_strDescript = mem.ReadString();
	m_strUserObject = mem.ReadString();
	mem.ReadData(&m_nDeleted,sizeof(int));
	m_strGUID = mem.ReadString();
}

void CLogManageRecord::InitTestData()
{
//	共同信息
	m_strComputer = STRDUP(_T("Microsoft"));
	m_strUser = STRDUP(_T("admin"));
	m_strIP = STRDUP(_T("127.0.0.188"));
	//GetSystemTime(&m_time);
// 文件日志信息
	m_nRecordType = MANAGE_ADDUSER;

	m_strDescript = STRDUP(_T("打印权限"));

	m_strUserObject = STRDUP(_T("sa"));	//
}

LPTSTR CLogManageRecord::GetInsertSQL(LPTSTR strSQL)
{
#ifdef LOG_ARAM
#ifdef DATABASE_SUPPORT
	_stprintf(strSQL,_T("INSERT INTO Log_Manage (manage_ip,manage_computer,manage_time,manage_user,manage_recordtype,manage_descript,manage_userobject,manage_deleted, manage_guid) SELECT '%s','%s','%s','%s','%d','%s','%s',%d,'%s'"),
			  m_strIP,m_strComputer,m_strTime,m_strUser,m_nRecordType,m_strDescript,m_strUserObject,m_nDeleted,m_strGUID);

	ADODB::_ConnectionPtr ptrConnection = NULL;
	int nID = 0;
	CWebDataFun::ExecuteSQL(ptrConnection, strSQL);
#endif
#endif
	return strSQL;

}