// LogMailRecord.cpp: implementation of the CLogMailRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogMailRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogMailRecord::CLogMailRecord()
{
	m_strAddressee = NULL;
	m_strSender = NULL;
	m_strCarboncopy = NULL;
	m_strAttchments = NULL;
}

CLogMailRecord::~CLogMailRecord()
{
	if (m_strAddressee)
	{
		delete[] m_strAddressee;
		m_strAddressee = NULL;
	}

	if (m_strCarboncopy)
	{
		delete[] m_strCarboncopy;
		m_strCarboncopy = NULL;
	}

	if (m_strSender)
	{
		delete[] m_strSender;
		m_strSender = NULL;
	}

	if (m_strAttchments)
	{
		delete[] m_strAttchments;
		m_strAttchments = NULL;
	}
}

void CLogMailRecord::RecordToMemory(CVirtualMemFile& mem)
{
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteString(m_strIndex);
	mem.WriteString(m_strComputer);
	mem.WriteString(m_strUser);
	mem.WriteString(m_strIP);
	mem.WriteString(m_strTime);
	mem.WriteString(m_strSender);
	mem.WriteString(m_strAddressee);
	mem.WriteString(m_strCarboncopy);
	mem.WriteString(m_strAttchments);
	mem.WriteData(&m_nDeleted,sizeof(int));
	mem.WriteString(m_strGUID);
}

void CLogMailRecord::MemoryToRecord(CVirtualMemFile& mem)
{
	mem.ReadData(&m_nLogType,sizeof(int));
	m_strIndex = mem.ReadString();
	m_strComputer = mem.ReadString();
	m_strUser = mem.ReadString();
	m_strIP = mem.ReadString();
	m_strTime = mem.ReadString();
	m_strSender = mem.ReadString();
	m_strAddressee = mem.ReadString();
	m_strCarboncopy = mem.ReadString();
	m_strAttchments = mem.ReadString();
	mem.ReadData(&m_nDeleted,sizeof(int));
	m_strGUID = mem.ReadString();
}

LPTSTR CLogMailRecord::GetInsertSQL(LPTSTR strSQL)
{
#ifdef LOG_ARAM
#ifdef DATABASE_SUPPORT
	_stprintf(strSQL,_T("INSERT INTO Log_Mail (mail_ip,mail_computer,mail_time,mail_user,mail_sender,mail_address,mail_carboncopy,mail_attchments,mail_deleted, manage_guid) SELECT '%s','%s','%s','%s','%s','%s','%s','%s',%d,'%s'"),
		m_strIP,m_strComputer,m_strTime,m_strUser,m_strSender,m_strAddressee, m_strCarboncopy, m_strAttchments, m_nDeleted, m_strGUID);

	ADODB::_ConnectionPtr ptrConnection = NULL;
	int nID = 0;
	CWebDataFun::ExecuteSQL(ptrConnection, strSQL);
#endif
#endif
	return strSQL;

}

void CLogMailRecord::InitTestData()
{

}