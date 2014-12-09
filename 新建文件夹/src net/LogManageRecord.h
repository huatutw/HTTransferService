// LogManageRecord.h: interface for the CLogManageRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMANAGERECORD_H__4CD5A68B_7D2A_40AB_911C_29F6D81EC93A__INCLUDED_)
#define AFX_LOGMANAGERECORD_H__4CD5A68B_7D2A_40AB_911C_29F6D81EC93A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogBase.h"

enum
{
	MANAGE_ADDUSER = 1,			//增加用户
	MANAGE_DELETEUSER,			//删除用户
	MANAGE_MODIFYRIGHT,			//修改权限
};

class CLogManageRecord : public CLogBase  
{
public:
	CLogManageRecord();
	virtual ~CLogManageRecord();

public:
	void RecordToMemory(CVirtualMemFile& mem);
	void MemoryToRecord(CVirtualMemFile& mem);

	//将记录写入数据库
	void InitTestData();
	LPTSTR GetInsertSQL(LPTSTR strSQL);

public:
	int		m_nRecordType;		//增加用户，删除用户，修改权限
	LPTSTR	m_strDescript;		//修改的内容
	LPTSTR  m_strUserObject;	//被修改的用户名
};

#endif // !defined(AFX_LOGMANAGERECORD_H__4CD5A68B_7D2A_40AB_911C_29F6D81EC93A__INCLUDED_)
