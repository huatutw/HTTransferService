// FileAccreditRecord.cpp: implementation of the CFileAccreditRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileAccreditRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileAccreditRecord::CFileAccreditRecord()
{
	m_strFileName = NULL;
	m_strCanopenuser = NULL;
	m_strFileGuid = NULL;
	m_nLogType = FILE_ACCREDIT;
	m_nPurviewOper = 0;
}

CFileAccreditRecord::~CFileAccreditRecord()
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

	if (m_strFileGuid)
	{
		delete[] m_strFileGuid;
		m_strFileGuid = NULL;
	}

	if (m_strOutTime)
	{
		delete[] m_strOutTime;
		m_strOutTime = NULL;
	}
}

void CFileAccreditRecord::RecordToMemory(CVirtualMemFile& mem)
{
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteString(m_strIndex);
	mem.WriteString(m_strComputer);
	mem.WriteString(m_strUser);
	mem.WriteString(m_strIP);
	mem.WriteString(m_strTime);
	mem.WriteString(m_strFileName);
	mem.WriteString(m_strCanopenuser);
	mem.WriteString(m_strFileGuid);
	mem.WriteData(&m_nPurviewOper,sizeof(int));
	mem.WriteString(m_strOutTime);
}

void CFileAccreditRecord::MemoryToRecord(CVirtualMemFile& mem)
{
	if (m_bUnicodeToAnsi)
	{
		mem.ReadData(&m_nLogType,sizeof(int));
		USES_CONVERSION;
		m_strIndex = _tcsdup(W2T(mem.ReadStringW()));
		m_strComputer = _tcsdup(W2T(mem.ReadStringW()));
		m_strUser = _tcsdup(W2T(mem.ReadStringW()));
		m_strIP = _tcsdup(W2T(mem.ReadStringW()));
		m_strTime = _tcsdup(W2T(mem.ReadStringW()));
		m_strFileName = _tcsdup(W2T(mem.ReadStringW()));
		m_strCanopenuser = _tcsdup(W2T(mem.ReadStringW()));
		m_strFileGuid = _tcsdup(W2T(mem.ReadStringW()));
		mem.ReadData(&m_nPurviewOper,sizeof(int));
		m_strOutTime = _tcsdup(W2T(mem.ReadStringW()));
	}else
	{
		mem.ReadData(&m_nLogType,sizeof(int));
		m_strIndex = mem.ReadString();
		m_strComputer = mem.ReadString();
		m_strUser = mem.ReadString();
		m_strIP = mem.ReadString();
		m_strTime = mem.ReadString();
		m_strFileName = mem.ReadString();
		m_strCanopenuser = mem.ReadString();
		m_strFileGuid = mem.ReadString();
		mem.ReadData(&m_nPurviewOper,sizeof(int));
		m_strOutTime = mem.ReadString();
	}
}

void CFileAccreditRecord::InitTestData()
{

}

LPTSTR CFileAccreditRecord::GetInsertSQL(LPTSTR strSQL)
{
	_stprintf(strSQL,_T("INSERT INTO File_Accredit (accredit_ip,accredit_computer,accredit_time,accredit_user,accredit_doc,accredit_canopenuser,accredit_guid,accredit_purviewoper,accredit_outtime) values ('%s','%s','%s','%s','%s','%s','%s',%d,'%s')"),
		m_strIP,m_strComputer,m_strTime,m_strUser,m_strFileName,m_strCanopenuser,m_strFileGuid,m_nPurviewOper,m_strOutTime);
	return strSQL;
}