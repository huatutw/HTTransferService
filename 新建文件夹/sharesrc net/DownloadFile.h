// DownloadFile.h: interface for the CDownloadFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADFILE_H__A5451D5F_6382_4AB4_8E79_116589CEE626__INCLUDED_)
#define AFX_DOWNLOADFILE_H__A5451D5F_6382_4AB4_8E79_116589CEE626__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UpdateBaseClass.h"

class CDownloadFile : public CUpdateBaseClass  
{
public:
	CDownloadFile();
	virtual ~CDownloadFile();

	void MakeDestFilePath();

	LPTSTR m_lpszSourceFile;		//源文件路径
	LPTSTR m_lpszFileMD5;			//文件MD5
	LPTSTR m_lpszDestSubPath;		//目标路径
	LPTSTR m_lpszDomain;			//域名
	LPTSTR m_lpszLogin;				//登录名
	LPTSTR m_lpszPass;				//密码
	LPTSTR m_lpszCommand;			//命令行参数
	int    m_nPathType;				//路径类型
	BOOL   m_bNeedRegister;			//是否需要注册
	BOOL   m_bNeedRegServer;		//是否需要注册
	BOOL   m_bNeedRun;				//是否需要运行
};

#endif // !defined(AFX_DOWNLOADFILE_H__A5451D5F_6382_4AB4_8E79_116589CEE626__INCLUDED_)
