// LogLogRecord.h: interface for the CLogLogRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGLOGRECORD_H__8D6D919F_7804_4055_8B87_59DF7709F13D__INCLUDED_)
#define AFX_LOGLOGRECORD_H__8D6D919F_7804_4055_8B87_59DF7709F13D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogBase.h"

enum
{
	OPER_DELETE = 1,
	OPER_EXPORT_LOG,
	OPER_EXPORT_EXCEL,
	OPER_REVERT,
	OPER_REALLYDELETE,
};
class CLogLogRecord : public CLogBase  
{
public:
	CLogLogRecord();
	virtual ~CLogLogRecord();

	void RecordToMemory(CVirtualMemFile& mem);
	void MemoryToRecord(CVirtualMemFile& mem);

	LPTSTR GetInsertSQL(LPTSTR strSQL);

	void InitTestData();
public:
	int		m_nOperType;		//操作类型
	int		m_nLogType;			//日志类型
};

#endif // !defined(AFX_LOGLOGRECORD_H__8D6D919F_7804_4055_8B87_59DF7709F13D__INCLUDED_)
