// LogLandRecordset.h: interface for the CLogLandRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGLANDRECORDSET_H__66D611F6_2229_4D84_8650_285D0BCADEAE__INCLUDED_)
#define AFX_LOGLANDRECORDSET_H__66D611F6_2229_4D84_8650_285D0BCADEAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogRecordset.h"

class CLogLandRecordset : public CLogRecordset 
{
public:
	CLogLandRecordset();
	virtual ~CLogLandRecordset();
public:
	
#ifdef DATABASE_SUPPORT
	void ReadFromDatabase(LPCTSTR strTotalSQL,LPCTSTR strSQL);
#endif
	
	void BuildTestData();
};

#endif // !defined(AFX_LOGLANDRECORDSET_H__66D611F6_2229_4D84_8650_285D0BCADEAE__INCLUDED_)
