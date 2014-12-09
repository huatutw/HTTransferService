// ComputerInfo.h: interface for the CComputerInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPUTERINFO_H__B390ADF6_8B25_4C98_B3DD_1E35A0EDED69__INCLUDED_)
#define AFX_COMPUTERINFO_H__B390ADF6_8B25_4C98_B3DD_1E35A0EDED69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum
{
	BACKUP_CRYPTED_FILE = 0x1, //仅备份加密文件
	BACKUP_ALL_FILE	= 0x2,	   //传送所有文件
	BACKUP_RIGHT_NOW = 0x4,	   //即时备份
	BACKUP_PLAN = 0x8,		   //使用计划任务
		
};

enum
{
	EVERY_DAY = 0x1,			//每天备份
	EVERY_WEEK = 0x2,			//每周备份
	EVERY_MONTH = 0x3,			//每月备份
};

class CBackupPlanInfo
{
public:
	CBackupPlanInfo();
	virtual ~CBackupPlanInfo();

	int m_nPlanType;		//计划任务的类型
	
	SYSTEMTIME m_StartTime;	//计划开始的时间	
	
	
	SYSTEMTIME m_StartMonth; //计划开始的每月日期
};

class CComputerBackupInfo  
{
public:
	CComputerBackupInfo();
	virtual ~CComputerBackupInfo();
	
public:

	LPSTR m_lpszComputerName;	//计算机名
	LPSTR m_lpszComputerIP;		//计算机IP
	LPSTR m_lpszLoginName;		//计算机登录帐号

	int	m_nBackupOption;		//备份选项
	LPSTR m_lpszFilter;			//过滤参数
	LPSTR m_lpszServerIP;		//文件备份服务器的IP地址
	
	CBackupPlanInfo* m_pPlanInfo;	//计划信息

};


#endif // !defined(AFX_COMPUTERINFO_H__B390ADF6_8B25_4C98_B3DD_1E35A0EDED69__INCLUDED_)
