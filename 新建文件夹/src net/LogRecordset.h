// LogRecordset.h: interface for the CLogRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGRECORDSET_H__E7746296_027A_4713_9B92_69CF6A60E89E__INCLUDED_)
#define AFX_LOGRECORDSET_H__E7746296_027A_4713_9B92_69CF6A60E89E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogFileRecord.h"
#include "LogManageRecord.h"
#include "LogLogRecord.h"
#include "LogPrintRecord.h"
#include "LogMailRecord.h"
#include "LogFileaccreditRecord.h"
#include "LogOutSideRecord.h"
#include "FileAccreditRecord.h"
#include "LogWorkRecord.h"
#include "DatabaseConnect.h"

enum
{
	LOG_DELETE = 0,
	LOG_BACKUP,
};

class CLogRecordset  
{
public:
	CLogRecordset();
	virtual ~CLogRecordset();

	int			m_nRecordType;			//记录集类型
	DEQUE_LOG	m_dequeLog;

	DWORD		m_dwNextPos;
	//释放内存
	void ClearMemory();

	//从数据包解析记录
	BOOL GetDataFromBuffer(LPBYTE btData,DWORD dwSize,BOOL bUnicodeToAnsi = FALSE);

	//将记录写入数据包
	BOOL SetDataToBuffer(LPBYTE* btData,DWORD& dwSize,BOOL bUnicodeToAnsi = FALSE);

#ifdef DATABASE_SUPPORT
	//将数据写入数据库
	void WriteToDatabase(int nLogType);

	//从数据读取数据
	virtual void ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL) = 0;
#endif

	virtual void BuildTestData() = 0;

//  以下数据仅在日志管理工具中使用

	int			m_nPages;				//总页数
	int			m_nRecordPerPage;		//每页记录数
	int			m_nCurrentPage;			//当前页
	int			m_nTotalCount;			//总记录数

/*#ifdef DATABASE_SUPPORT
public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
#endif*/
};

#endif // !defined(AFX_LOGRECORDSET_H__E7746296_027A_4713_9B92_69CF6A60E89E__INCLUDED_)
