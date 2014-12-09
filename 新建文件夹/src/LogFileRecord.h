// LogFileRecord.h: interface for the CLogFileRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILERECORD_H__11F24753_AE5B_4553_9037_D78F39E86ACE__INCLUDED_)
#define AFX_LOGFILERECORD_H__11F24753_AE5B_4553_9037_D78F39E86ACE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogBase.h"

enum
{
	FILE_ADJUST,
	FILE_ENCRYPT,
	FILE_DECRYPT,
	FILE_MODIFYTIME,
	FILE_DELETETIME,
	FILE_OPEN,
	FILE_CLOSE,
	FILE_COPY,
	FILE_DELETE,
	FILE_EDIT,
	FILE_MOVE,
};

class CLogFileRecord : public CLogBase  
{
public:
	CLogFileRecord();
	virtual ~CLogFileRecord();

public:
	void RecordToMemory(CVirtualMemFile& mem);
	void MemoryToRecord(CVirtualMemFile& mem);

	LPTSTR GetInsertSQL(LPTSTR strSQL);

	void InitTestData();

public:
	int m_nOperType;		//加密，解密，调整等级
	int m_nSystemError;		//系统错误
	int m_nInternalError;	//内部错误
	LPTSTR m_strFileName;	//文件名
	LPTSTR m_strServerFilePath;		//4.0服务器上的存储路径
};

#endif // !defined(AFX_LOGFILERECORD_H__11F24753_AE5B_4553_9037_D78F39E86ACE__INCLUDED_)
