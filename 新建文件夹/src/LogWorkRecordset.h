// LogWorkRecordset.h: interface for the CLogWorkRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGWORKRECORDSET_H__5746BC6E_0C98_42AE_B1E9_D3B0697D16B2__INCLUDED_)
#define AFX_LOGWORKRECORDSET_H__5746BC6E_0C98_42AE_B1E9_D3B0697D16B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogRecordset.h"

class CLogWorkRecordset : public CLogRecordset  
{
public:
	CLogWorkRecordset();
	virtual ~CLogWorkRecordset();

public:
#ifdef DATABASE_SUPPORT
	void ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL);
#endif
	
	void BuildTestData();
};

#endif // !defined(AFX_LOGWORKRECORDSET_H__5746BC6E_0C98_42AE_B1E9_D3B0697D16B2__INCLUDED_)
