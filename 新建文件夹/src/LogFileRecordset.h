// LogFileRecordset.h: interface for the CLogFileRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILERECORDSET_H__D5E7ECAC_789C_4D01_82B4_F4F28DF9FA6D__INCLUDED_)
#define AFX_LOGFILERECORDSET_H__D5E7ECAC_789C_4D01_82B4_F4F28DF9FA6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogRecordset.h"

class CLogFileRecordset : public CLogRecordset
{
public:
	CLogFileRecordset();
	virtual ~CLogFileRecordset();
public:

#ifdef DATABASE_SUPPORT
	void ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL);
#endif

	void BuildTestData();
};

#endif // !defined(AFX_LOGFILERECORDSET_H__D5E7ECAC_789C_4D01_82B4_F4F28DF9FA6D__INCLUDED_)
