// LogBase.h: interface for the CLogBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGBASE_H__29D500AF_9D6F_4ECC_BAAF_B47145481504__INCLUDED_)
#define AFX_LOGBASE_H__29D500AF_9D6F_4ECC_BAAF_B47145481504__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <deque>

#include "VirtualMemFile.h"

enum
{
	LOG_FILE = 1,			//文件日志
	LOG_PRINT,				//打印日志
	LOG_MAIL,				//邮件日志
	LOG_OUTSIDE,			//外发日志
	LOG_USB,				//USB锁管理
	LOG_MANAGE,				//管理日志
	LOG_LOG,				//日志管理
	LOG_LAND,				//登入登出日志
	LOG_FILEACCREDIT,		//文件授权日志
	LOG_WORK,				//工作记录
	FILE_ACCREDIT,			//文件授权信息
	//国密内容
	FILE_UPLOAD,			//文件上传
	
};

extern LPTSTR STRDUP(LPCTSTR strData);

class CLogBase  
{
public:
	CLogBase();
	virtual ~CLogBase();

	virtual void RecordToMemory(CVirtualMemFile& mem) = 0;
	virtual void MemoryToRecord(CVirtualMemFile& mem) = 0;

	//从数据包解析记录
	BOOL GetDataFromBuffer(LPBYTE btData,DWORD dwSize);

	//将记录写入数据包
	BOOL SetDataToBuffer(LPBYTE* btData,DWORD& dwSize);

	//得到计算机信息
	void GetLocalInfo();

	//将记录写入数据库
	virtual LPTSTR GetInsertSQL(LPTSTR strSQL) = 0;

	virtual void InitTestData() = 0;

public:
	int		m_nLogType;	
	LPTSTR	m_strIndex;		//日志序号
	LPTSTR	m_strComputer;	//计算机
	LPTSTR	m_strUser;		//人员
	LPTSTR	m_strIP;		//IP
	LPTSTR  m_strTime;		//时间
	LPTSTR  m_strGUID;			//操作编码
	int		m_nDeleted;
	BOOL	m_bUnicodeToAnsi;	//由unicode串转化为Ansi串
};

typedef std::deque<CLogBase*> DEQUE_LOG;
extern void GetHostAddressEx(const char* strComputerName,char* lpszIP);
#endif // !defined(AFX_LOGBASE_H__29D500AF_9D6F_4ECC_BAAF_B47145481504__INCLUDED_)
