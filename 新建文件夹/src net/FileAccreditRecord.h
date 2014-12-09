// FileAccreditRecord.h: interface for the CFileAccreditRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEACCREDITRECORD_H__7908F3E4_9DA9_4C63_9841_76FD45247FA0__INCLUDED_)
#define AFX_FILEACCREDITRECORD_H__7908F3E4_9DA9_4C63_9841_76FD45247FA0__INCLUDED_
#include "LogBase.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum
{
		PURVIEW_OPEN			= 0x1,		//可打开
		PURVIEW_PRINT			= 0x2,		//可打印
		PURVIEW_LEVEL			= 0x4,		//保持原来的等级权限
		PURVIEW_READONLY		= 0x8,		//可浏览
};

class CFileAccreditRecord : public CLogBase 
{
public:
	CFileAccreditRecord();
	virtual ~CFileAccreditRecord();
public:
	void RecordToMemory(CVirtualMemFile& mem);
	void MemoryToRecord(CVirtualMemFile& mem);
	
	LPTSTR GetInsertSQL(LPTSTR strSQL);
	
	void InitTestData();
	
public:
	LPTSTR m_strFileName;	//文件名
	LPTSTR m_strCanopenuser;	//授权用户
	LPTSTR m_strFileGuid;		//文件GUID
	int m_nPurviewOper;	//授权的操作
	LPTSTR m_strOutTime;		//授权过期时间
};

#endif // !defined(AFX_FILEACCREDITRECORD_H__7908F3E4_9DA9_4C63_9841_76FD45247FA0__INCLUDED_)
