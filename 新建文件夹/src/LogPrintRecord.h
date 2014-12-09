// LogPrintRecord.h: interface for the CLogPrintRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPRINTRECORD_H__7C7D2944_DC33_4F01_8005_0B261284FF10__INCLUDED_)
#define AFX_LOGPRINTRECORD_H__7C7D2944_DC33_4F01_8005_0B261284FF10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogBase.h"

#ifdef LOG_ARAM

typedef deque<st_Log_Print_Record> dqLogPrintRecord;
#endif

class CLogPrintRecord : public CLogBase  
{
public:
	CLogPrintRecord();
	virtual ~CLogPrintRecord();

	void RecordToMemory(CVirtualMemFile& mem);
	void MemoryToRecord(CVirtualMemFile& mem);

	LPTSTR GetInsertSQL(LPTSTR strSQL);

	void InitTestData();

	LPTSTR GetSnapshotPath();
public:
	LPTSTR m_strPrintFileName;		//打印文档名
	LPTSTR m_strSnapshot;			//打印快照名
};

#endif // !defined(AFX_LOGPRINTRECORD_H__7C7D2944_DC33_4F01_8005_0B261284FF10__INCLUDED_)
