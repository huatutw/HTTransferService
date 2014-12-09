// LogFileaccreditRecord.cpp: implementation of the CLogFileaccreditRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFileaccreditRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFileaccreditRecord::CLogFileaccreditRecord()
{
	m_strFileName = NULL;
	m_nLogType = LOG_FILE;
	m_strCanopenuser = NULL;
	m_nPurviewOper = 0;		
	m_strOutTime = NULL;
	m_strFileGuid = NULL;
}

CLogFileaccreditRecord::~CLogFileaccreditRecord()
{
	if (m_strFileName)
	{
		delete[] m_strFileName;
		m_strFileName = NULL;
	}

	if (m_strCanopenuser)
	{
		delete[] m_strCanopenuser;
		m_strCanopenuser = NULL;
	}

	if (m_strOutTime)
	{
		delete[] m_strOutTime;
		m_strOutTime = NULL;
	}

	if (m_strFileGuid)
	{
		delete[] m_strFileGuid;
		m_strFileGuid = NULL;
	}
}

void CLogFileaccreditRecord::RecordToMemory(CVirtualMemFile& mem)
{
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteString(m_strIndex);
	mem.WriteString(m_strComputer);
	mem.WriteString(m_strUser);
	mem.WriteString(m_strIP);
	mem.WriteString(m_strTime);
	mem.WriteString(m_strFileName);
	mem.WriteString(m_strFileGuid);
	mem.WriteString(m_strCanopenuser);
	mem.WriteData(&m_nPurviewOper,sizeof(int));
	mem.WriteString(m_strOutTime);
	mem.WriteData(&m_nDeleted,sizeof(int));
	mem.WriteString(m_strGUID);
}

void CLogFileaccreditRecord::MemoryToRecord(CVirtualMemFile& mem)
{
	mem.ReadData(&m_nLogType,sizeof(int));
	m_strIndex = mem.ReadString();
	m_strComputer = mem.ReadString();
	m_strUser = mem.ReadString();
	m_strIP = mem.ReadString();
	m_strTime = mem.ReadString();
	m_strFileName = mem.ReadString();
	m_strFileGuid = mem.ReadString();
	m_strCanopenuser = mem.ReadString();
	mem.ReadData(&m_nPurviewOper,sizeof(int));
	m_strOutTime = mem.ReadString();
	mem.ReadData(&m_nDeleted,sizeof(int));
	m_strGUID = mem.ReadString();
}

void CLogFileaccreditRecord::InitTestData()
{
}

LPTSTR CLogFileaccreditRecord::GetInsertSQL(LPTSTR strSQL)
{
#ifdef LOG_ARAM
#ifdef DATABASE_SUPPORT
	_stprintf(strSQL,_T("INSERT INTO Log_Fileaccredit (accredit_ip,accredit_computer,accredit_time,accredit_user,accredit_doc,accredit_guid,accredit_canopenuser,accredit_purviewoper,accredit_outtime,accredit_deleted,manage_guid) SELECT '%s','%s','%s','%s','%s','%s','%s','%d','%s',%d,'%s'"),
		m_strIP,m_strComputer,m_strTime,m_strUser,m_strFileName,m_strFileGuid,m_strCanopenuser,m_nPurviewOper,m_strOutTime,m_nDeleted,m_strGUID);

	ADODB::_ConnectionPtr ptrConnection = NULL;
	int nID = 0;
	CWebDataFun::ExecuteSQL(ptrConnection, strSQL);
#endif
#endif
	return strSQL;
}