// LogMailRecordset.h: interface for the CLogMailRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMAILRECORDSET_H__D2C3147B_AD0B_4464_86D9_8C8EEB440794__INCLUDED_)
#define AFX_LOGMAILRECORDSET_H__D2C3147B_AD0B_4464_86D9_8C8EEB440794__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogRecordset.h"

class CLogMailRecordset : public CLogRecordset
{
public:
	CLogMailRecordset();
	virtual ~CLogMailRecordset();
public:
	
#ifdef DATABASE_SUPPORT
	void ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL);
#endif

	void BuildTestData();
	
};

#endif // !defined(AFX_LOGMAILRECORDSET_H__D2C3147B_AD0B_4464_86D9_8C8EEB440794__INCLUDED_)
