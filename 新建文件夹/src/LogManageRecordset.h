// LogManageRecordset.h: interface for the CLogManageRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMANAGERECORDSET_H__8E06D875_B73F_427C_BB1C_8A771FBFB40B__INCLUDED_)
#define AFX_LOGMANAGERECORDSET_H__8E06D875_B73F_427C_BB1C_8A771FBFB40B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogRecordset.h"

class CLogManageRecordset : public CLogRecordset  
{
public:
	CLogManageRecordset();
	virtual ~CLogManageRecordset();

#ifdef DATABASE_SUPPORT
	void ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL);
#endif

	void BuildTestData();
};

#endif // !defined(AFX_LOGMANAGERECORDSET_H__8E06D875_B73F_427C_BB1C_8A771FBFB40B__INCLUDED_)
