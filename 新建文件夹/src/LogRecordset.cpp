// LogRecordset.cpp: implementation of the CLogRecordset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogRecordset.h"
//#include "WebDataFun.h"
#include "LogFileRecordset.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogRecordset::CLogRecordset()
{
	m_dwNextPos = 0;
}

CLogRecordset::~CLogRecordset()
{
	ClearMemory();
}

void CLogRecordset::ClearMemory()
{
	int nRecordCount = m_dequeLog.size();

	for (int i=0;i<nRecordCount;i++)
	{
		CLogBase* pRecord = m_dequeLog.front();
		m_dequeLog.pop_front();
		delete pRecord;
	}
}

BOOL CLogRecordset::GetDataFromBuffer(LPBYTE btData,DWORD dwSize,BOOL bUnicodeToAnsi)
{
	if (btData == NULL || dwSize == 0)
		return FALSE;

// #ifndef TEST_PROJECT
// 	ClearMemory();		//释放内存
// #endif

	CVirtualMemFile mem;
	mem.AttachMem(btData,dwSize);
	
//////////////////////////////////////////////////////////////////////////
//读取Record信息
	int nRecordCount = 0;
	mem.ReadData(&nRecordCount,sizeof(int));

	for (int i=0;i<nRecordCount;i++)
	{
		CLogBase* pRecord = NULL;
		switch (m_nRecordType)
		{
// #ifdef RECORD_SEPERATE
// #ifdef RECORD_FILE
// 		case LOG_FILE:
// 			pRecord = new CLogFileRecord;
// 			break;
// #endif
// #ifdef RECORD_MANAGE
// 		case LOG_MANAGE:
// 			pRecord = new CLogManageRecord;
// 			break;
// #endif
// #ifdef RECORD_PRINT
// 		case LOG_PRINT:
// 			pRecord = new CLogPrintRecord;
// 			break;
// #endif
// #ifdef RECORD_MAIL
// 		case LOG_MAIL:
// 			pRecord = new CLogMailRecord;
// 			break;
// #endif
// #ifdef RECORD_LOG
// 		case LOG_LOG:
// 			pRecord = new CLogLogRecord;
// 			break;
// #endif
// #ifdef RECORD_FILEACCREDIT
// 		case LOG_FILEACCREDIT:
// 			pRecord = new CLogFileaccreditRecord;
// 			break;
// #endif
// #ifdef RECORD_ACCREDIT
// 		case FILE_ACCREDIT:
// 			pRecord = new CFileAccreditRecord;
// 			break;
// #endif
// #ifdef RECORD_WORD
// 		case LOG_WORK:
// 			pRecord = new CLogWorkRecord;
// 			break;
// #endif
//#else
		case LOG_FILE:
			pRecord = new CLogFileRecord;
			break;
		case LOG_MANAGE:
			pRecord = new CLogManageRecord;
			break;
		case LOG_PRINT:
			pRecord = new CLogPrintRecord;
			break;
		case LOG_MAIL:
			pRecord = new CLogMailRecord;
			break;
		case LOG_LOG:
			pRecord = new CLogLogRecord;
			break;
		case LOG_FILEACCREDIT:
			pRecord = new CLogFileaccreditRecord;
			break;
		case LOG_OUTSIDE:
			pRecord = new CLogOutSideRecord;
			break;
		case LOG_WORK:
			pRecord = new CLogWorkRecord;
			break;
		case FILE_ACCREDIT:
			pRecord = new CFileAccreditRecord;
			break;
//#endif
		}
		pRecord->m_bUnicodeToAnsi = bUnicodeToAnsi;
		pRecord->MemoryToRecord(mem);
		pRecord->m_bUnicodeToAnsi = FALSE;
		m_dequeLog.push_back(pRecord);
	}
	mem.ReadData(&m_nTotalCount,sizeof(int));
	m_dwNextPos = mem.Seek(0,123);
//////////////////////////////////////////////////////////////////////////
	return TRUE;
}

BOOL CLogRecordset::SetDataToBuffer(LPBYTE* btData,DWORD& dwSize,BOOL bUnicodeToAnsi)
{
	CVirtualMemFile mem(1024*512);	//申请512的内存
	
	int nRecordCount = m_dequeLog.size();

	mem.WriteData(&nRecordCount,sizeof(int));
	for (int i=0;i<nRecordCount;i++)
	{
		m_dequeLog[i]->m_bUnicodeToAnsi = bUnicodeToAnsi;
		m_dequeLog[i]->RecordToMemory(mem);
		m_dequeLog[i]->m_bUnicodeToAnsi = FALSE;
	}
	mem.WriteData(&m_nTotalCount,sizeof(int));

	*btData = new BYTE[mem.GetDataSize()];
	memcpy(*btData,mem.GetData(),mem.GetDataSize());

	dwSize = mem.GetDataSize();

	return TRUE;
}

#ifdef DATABASE_SUPPORT
void CLogRecordset::WriteToDatabase(int nLogType)
{
	USES_CONVERSION;
	
 	TCHAR strSQL[1024] = {0};
	for (int i=0;i<m_dequeLog.size();i++)
	{
		memset(strSQL,0,1024*sizeof(TCHAR));
		m_dequeLog[i]->GetInsertSQL(strSQL);
		/*CWebDataFun::ExecuteSQL(strSQL);*/
#ifndef LOG_ARAM
		CDatabaseConnect::m_pConnection->Execute((_bstr_t)(strSQL),NULL,0);
#endif
	}
}
#endif