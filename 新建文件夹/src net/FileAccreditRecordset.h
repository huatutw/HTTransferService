// FileAccreditRecordset.h: interface for the CFileAccreditRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEACCREDITRECORDSET_H__A13BBEF2_8FCF_4EA9_900D_9C0D10916B61__INCLUDED_)
#define AFX_FILEACCREDITRECORDSET_H__A13BBEF2_8FCF_4EA9_900D_9C0D10916B61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogRecordset.h"

class CFileAccreditRecordset : public CLogRecordset 
{
public:
	CFileAccreditRecordset();
	virtual ~CFileAccreditRecordset();

public:
#ifdef DATABASE_SUPPORT
	void ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL);
#endif
	
	void BuildTestData();
};

#endif // !defined(AFX_FILEACCREDITRECORDSET_H__A13BBEF2_8FCF_4EA9_900D_9C0D10916B61__INCLUDED_)
