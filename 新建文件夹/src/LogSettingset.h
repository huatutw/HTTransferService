// LogSettingset.h: interface for the CLogSettingset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETTINGSET_H__F50AA83C_EC2F_4F9B_B18A_0D168DCEAC55__INCLUDED_)
#define AFX_LOGSETTINGSET_H__F50AA83C_EC2F_4F9B_B18A_0D168DCEAC55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogSetting.h"
#include "VirtualMemFile.h"
#include "DatabaseConnect.h"

class CLogSettingset  
{
public:
	CLogSettingset();
	virtual ~CLogSettingset();

	DEQUE_SETTING	m_dequeLog;
	
	int			m_nTotalCount;			//总记录数

	//释放内存
	void ClearMemory();
	
	//从数据包解析记录
	BOOL GetDataFromBuffer(LPBYTE btData,DWORD dwSize);
	
	//将记录写入数据包
	BOOL SetDataToBuffer(LPBYTE* btData,DWORD& dwSize);
	
#ifdef DATABASE_SUPPORT
	//将数据写入数据库
	void WriteToDatabase();
	
	//从数据读取数据
	void ReadFromDatabase(LPCTSTR strTotalSQL,LPCTSTR strSQL);
#endif
	
};

#endif // !defined(AFX_LOGSETTINGSET_H__F50AA83C_EC2F_4F9B_B18A_0D168DCEAC55__INCLUDED_)
