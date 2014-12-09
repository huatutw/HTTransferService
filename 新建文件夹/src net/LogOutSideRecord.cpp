// LogOutSideRecord.cpp: implementation of the CLogOutSideRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogOutSideRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogOutSideRecord::CLogOutSideRecord()
{
	m_strFileName = NULL;
	m_nLogType = LOG_OUTSIDE;
}

CLogOutSideRecord::~CLogOutSideRecord()
{
	if (m_strFileName)
	{
		delete[] m_strFileName;
		m_strFileName = NULL;
	}
}

void CLogOutSideRecord::RecordToMemory(CVirtualMemFile& mem)
{
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteString(m_strIndex);
	mem.WriteString(m_strComputer);
	mem.WriteString(m_strUser);
	mem.WriteString(m_strIP);
	mem.WriteString(m_strTime);
	mem.WriteString(m_strFileName);
	mem.WriteData(&m_nOpenCount,sizeof(int));
	mem.WriteData(&m_nOpenDay,sizeof(int));
	mem.WriteData(&m_bCanPrint,sizeof(BOOL));
	mem.WriteData(&m_nDeleted,sizeof(int));
	mem.WriteString(m_strGUID);
}

void CLogOutSideRecord::MemoryToRecord(CVirtualMemFile& mem)
{
	mem.ReadData(&m_nLogType,sizeof(int));
	m_strIndex = mem.ReadString();
	m_strComputer = mem.ReadString();
	m_strUser = mem.ReadString();
	m_strIP = mem.ReadString();
	m_strTime = mem.ReadString();
	m_strFileName = mem.ReadString();
	mem.ReadData(&m_nOpenCount,sizeof(int));
	mem.ReadData(&m_nOpenDay,sizeof(int));
	mem.ReadData(&m_bCanPrint,sizeof(BOOL));
	mem.ReadData(&m_nDeleted,sizeof(int));
	m_strGUID = mem.ReadString();
}

void CLogOutSideRecord::InitTestData()
{
	//	共同信息
}

LPTSTR CLogOutSideRecord::GetInsertSQL(LPTSTR strSQL)
{
	_stprintf(strSQL,_T("INSERT INTO Log_OutSide (outside_ip,outside_computer,outside_time,outside_user,outside_doc,outside_opencount,outside_openday,outside_canprint,outside_deleted,manage_guid) SELECT '%s','%s','%s','%s','%s', %d, %d, %d, %d, '%s'"),
		m_strIP,m_strComputer,m_strTime,m_strUser,m_strFileName,m_nOpenCount,m_nOpenDay,m_bCanPrint,m_nDeleted,m_strGUID);
	return strSQL;
}