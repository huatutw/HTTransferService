// LogLogRecordset.h: interface for the CLogLogRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGLOGRECORDSET_H__AB8D9628_0055_408E_914E_35E8C8255287__INCLUDED_)
#define AFX_LOGLOGRECORDSET_H__AB8D9628_0055_408E_914E_35E8C8255287__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogRecordset.h"

class CLogLogRecordset : public CLogRecordset  
{
public:
	CLogLogRecordset();
	virtual ~CLogLogRecordset();

#ifdef DATABASE_SUPPORT
	void ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL);
#endif
	void BuildTestData();
};

#endif // !defined(AFX_LOGLOGRECORDSET_H__AB8D9628_0055_408E_914E_35E8C8255287__INCLUDED_)
