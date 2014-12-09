// LogOutSideRecordset.h: interface for the CLogOutSideRecordset class.
//
//////////////////////////////////////////////////////////////////////
#include "LogRecordset.h"
#include "LogOutSideRecord.h"

#if !defined(AFX_LOGOUTSIDERECORDSET_H__225C139F_6D55_48A6_8F3A_724739E90224__INCLUDED_)
#define AFX_LOGOUTSIDERECORDSET_H__225C139F_6D55_48A6_8F3A_724739E90224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogOutSideRecordset : public CLogRecordset 
{
public:
	CLogOutSideRecordset();
	virtual ~CLogOutSideRecordset();

public:
#ifdef DATABASE_SUPPORT
	void ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL);
#endif
	
	void BuildTestData();
};

#endif // !defined(AFX_LOGOUTSIDERECORDSET_H__225C139F_6D55_48A6_8F3A_724739E90224__INCLUDED_)
