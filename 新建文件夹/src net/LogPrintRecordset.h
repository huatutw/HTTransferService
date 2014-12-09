// LogPrintRecordset.h: interface for the CLogPrintRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPRINTRECORDSET_H__F430D9BC_DC93_4275_8005_BCCA4BF74A8E__INCLUDED_)
#define AFX_LOGPRINTRECORDSET_H__F430D9BC_DC93_4275_8005_BCCA4BF74A8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogRecordset.h"

class CLogPrintRecordset : public CLogRecordset  
{
public:
	CLogPrintRecordset();
	virtual ~CLogPrintRecordset();

#ifdef DATABASE_SUPPORT
	void ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL);
#endif
	void BuildTestData();
};

#endif // !defined(AFX_LOGPRINTRECORDSET_H__F430D9BC_DC93_4275_8005_BCCA4BF74A8E__INCLUDED_)
