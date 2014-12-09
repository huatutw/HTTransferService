// LogLandRecord.h: interface for the CLogLandRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGLANDRECORD_H__DADD4E72_3988_4959_AC3C_86EAE78969AF__INCLUDED_)
#define AFX_LOGLANDRECORD_H__DADD4E72_3988_4959_AC3C_86EAE78969AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogBase.h"
enum
{
	LAND_LOGIN,
	LAND_EXIT,
};

class CLogLandRecord : public CLogBase 
{
public:
	CLogLandRecord();
	virtual ~CLogLandRecord();

public:
	void RecordToMemory(CVirtualMemFile& mem);
	void MemoryToRecord(CVirtualMemFile& mem);
	
	LPTSTR GetInsertSQL(LPTSTR strSQL);
	
	void InitTestData();

public:
	int m_nOperType;		//操作类型,0:登入1:登出
};

#endif // !defined(AFX_LOGLANDRECORD_H__DADD4E72_3988_4959_AC3C_86EAE78969AF__INCLUDED_)
