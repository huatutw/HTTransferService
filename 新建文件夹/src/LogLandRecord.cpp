// LogLandRecord.cpp: implementation of the CLogLandRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogLandRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogLandRecord::CLogLandRecord()
{
	m_nLogType = LOG_LAND;
}

CLogLandRecord::~CLogLandRecord()
{

}

void CLogLandRecord::RecordToMemory(CVirtualMemFile& mem)
{
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteString(m_strIndex);
	mem.WriteString(m_strComputer);
	mem.WriteString(m_strUser);
	mem.WriteString(m_strIP);
	mem.WriteString(m_strTime);
	mem.WriteData(&m_nOperType,sizeof(int));
}

void CLogLandRecord::MemoryToRecord(CVirtualMemFile& mem)
{
	mem.ReadData(&m_nLogType,sizeof(int));
	m_strIndex = mem.ReadString();
	m_strComputer = mem.ReadString();
	m_strUser = mem.ReadString();
	m_strIP = mem.ReadString();
	m_strTime = mem.ReadString();
	mem.ReadData(&m_nOperType,sizeof(int));
}

void CLogLandRecord::InitTestData()
{

}

LPTSTR CLogLandRecord::GetInsertSQL(LPTSTR strSQL)
{
#ifdef DATABASE_SUPPORT
	_stprintf(strSQL,_T("INSERT INTO Log_Land (land_ip,land_computer,land_time,land_user,land_oper,land_deleted) SELECT '%s','%s','%s','%s','%d',%d"),
		m_strIP,m_strComputer,m_strTime,m_strUser,m_nOperType,0);
	
	ADODB::_ConnectionPtr ptrConnection = NULL;
	int nID = 0;
	CWebDataFun::ExecuteSQL(ptrConnection, strSQL);
#endif
	return strSQL;

}