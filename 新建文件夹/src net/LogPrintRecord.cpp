// LogPrintRecord.cpp: implementation of the CLogPrintRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogPrintRecord.h"
#include "atlbase.h"


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
	LPTSTR strSnapshotPath = GetSnapshotPath();
	
	_stprintf(strSQL,_T("INSERT INTO Log_Print (print_ip,print_computer,print_time,print_user,print_doc,print_snapshot,print_deleted,manage_guid) SELECT '%s','%s','%s','%s','%s','%s%s',%d,'%s'"),
			  m_strIP,m_strComputer,m_strTime,m_strUser,m_strPrintFileName,strSnapshotPath,m_strSnapshot,m_nDeleted,m_strGUID);

	return strSQL;
}

void CLogPrintRecord::InitTestData()
{
	
}
//
LPTSTR CLogPrintRecord::GetSnapshotPath()
{
	LPTSTR strSnapshotPath = new TCHAR[MAX_PATH];
	TCHAR path[MAX_PATH] = {0};   

	CRegKey key;
	if (key.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\{85767B7A-3294-E50E-A593-2096E94B8E76}\\Log\\LogPrint"),KEY_READ|KEY_WOW64_32KEY) != ERROR_SUCCESS)
	{
		if (key.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\DocGuarder\\Log\\LogPrint"),KEY_READ|KEY_WOW64_32KEY) != ERROR_SUCCESS)
		{
			::GetTempPath(MAX_PATH,path);   
			_stprintf(strSnapshotPath, _T("%s"), path);
		}
		return strSnapshotPath;
	}

	TCHAR value[1024] = {0};
	DWORD length = 1024;

	key.QueryValue(value,_T("Snapshotpath"),&length);
	if(key.QueryValue(value,_T("Snapshotpath"),&length) == ERROR_SUCCESS)
	{
		if(0 != _tcslen(value))
			_stprintf(strSnapshotPath, _T("%s"), value);
		else
		{
			::GetTempPath(MAX_PATH,path);   
			_stprintf(strSnapshotPath, _T("%s"), value);
		}
	}
	else
	{
		::GetTempPath(MAX_PATH,path);
		_stprintf(strSnapshotPath, _T("%s"), path);
	}
	return strSnapshotPath;
}