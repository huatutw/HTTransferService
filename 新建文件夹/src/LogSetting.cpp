// LogSetting.cpp: implementation of the CLogSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogSetting::CLogSetting()
{
	m_nMaxLogCount = 100000;	
	m_nOverOperType = 1;		//超过最大记录的操作类型
	m_nPerPageCount = 100;		//每页的记录数
}

CLogSetting::~CLogSetting()
{

}

void CLogSetting::MemoryToRecord(CVirtualMemFile& mem)
{
	mem.ReadData(&m_nLogType,sizeof(int));
	mem.ReadData(&m_nPerPageCount,sizeof(int));
	mem.ReadData(&m_nMaxLogCount,sizeof(int));	
	mem.ReadData(&m_nOverOperType,sizeof(int));	
}

void CLogSetting::RecordToMemory(CVirtualMemFile& mem)
{
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteData(&m_nPerPageCount,sizeof(int));
	mem.WriteData(&m_nMaxLogCount,sizeof(int));
	mem.WriteData(&m_nOverOperType,sizeof(int));
}

LPTSTR CLogSetting::GetInsertSQL(LPTSTR strSQL)
{
	_stprintf(strSQL,_T("INSERT INTO Log_Setting (setting_logtype,setting_perpagecount,setting_maxlogcount,setting_maxlogcount) SELECT '%d','%d','%d','%d'"),
		m_nLogType,m_nPerPageCount,m_nMaxLogCount,m_nOverOperType);
	return strSQL;
}