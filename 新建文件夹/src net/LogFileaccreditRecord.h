// LogFileaccreditRecord.h: interface for the CLogFileaccreditRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILEACCREDITRECORD_H__E95C5D9A_3C26_4A52_882A_AF982F66D629__INCLUDED_)
#define AFX_LOGFILEACCREDITRECORD_H__E95C5D9A_3C26_4A52_882A_AF982F66D629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogBase.h"

class CLogFileaccreditRecord : public CLogBase 
{
public:
	CLogFileaccreditRecord();
	virtual ~CLogFileaccreditRecord();
public:
	void RecordToMemory(CVirtualMemFile& mem);
	void MemoryToRecord(CVirtualMemFile& mem);
	
	LPTSTR GetInsertSQL(LPTSTR strSQL);
	
	void InitTestData();
	
public:
	LPTSTR m_strFileName;	//文件名
	LPTSTR m_strFileGuid;	//文件GUID
	LPTSTR m_strCanopenuser;		//授权的用户
	int m_nPurviewOper;		//授权操作
	LPTSTR m_strOutTime;	//过期时间
};

#endif // !defined(AFX_LOGFILEACCREDITRECORD_H__E95C5D9A_3C26_4A52_882A_AF982F66D629__INCLUDED_)
