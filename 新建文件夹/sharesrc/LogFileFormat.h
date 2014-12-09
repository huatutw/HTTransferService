// LogFileFormat.h: interface for the CLogFileFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILEFORMAT_H__A570EEE5_0205_4555_9098_93368B9C8E7D__INCLUDED_)
#define AFX_LOGFILEFORMAT_H__A570EEE5_0205_4555_9098_93368B9C8E7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>

class CLogFileFormat  
{
public:
	CLogFileFormat();
	virtual ~CLogFileFormat();

	DWORD m_dwVersion;

	DWORD m_dwIndexTableOffset;		//Ë÷Òý±íµÄÆ«ÒÆ

	std::deque<DWORD> m_arrIndexTable;

	

};

#endif // !defined(AFX_LOGFILEFORMAT_H__A570EEE5_0205_4555_9098_93368B9C8E7D__INCLUDED_)
