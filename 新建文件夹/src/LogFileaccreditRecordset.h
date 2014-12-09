// LogFileaccreditRecordset.h: interface for the CLogFileaccreditRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILEACCREDITRECORDSET_H__11058E6C_E1A6_4149_AFBC_06923E3DD523__INCLUDED_)
#define AFX_LOGFILEACCREDITRECORDSET_H__11058E6C_E1A6_4149_AFBC_06923E3DD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogRecordset.h"

class CLogFileaccreditRecordset : public CLogRecordset
{
public:
	CLogFileaccreditRecordset();
	virtual ~CLogFileaccreditRecordset();
public:
	
#ifdef DATABASE_SUPPORT
	void ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL);
#endif
	
	void BuildTestData();
};

#endif // !defined(AFX_LOGFILEACCREDITRECORDSET_H__11058E6C_E1A6_4149_AFBC_06923E3DD523__INCLUDED_)
