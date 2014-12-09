// UpdateBaseClass.h: interface for the CUpdateBaseClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATEBASECLASS_H__EBC1D72C_B7DE_4551_A681_73C0CCCBB070__INCLUDED_)
#define AFX_UPDATEBASECLASS_H__EBC1D72C_B7DE_4551_A681_73C0CCCBB070__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>

#define KEY_LEN 128
#define STR_LEN 1024

enum
{
	UPDATE_DOWNLOADFILE = 1,	//下载文件: run，regsiter
	UPDATE_DELETEFILE = 2,		//删除文件
	UPDATE_REGOP = 3,			//注册表管理
	UPDATE_COPYFILE = 4,		//复制文件
	UPDATE_UNINSTALL = 6,		//反安装
}; 

enum
{
	PATH_SYSTEM = 1,
	PATH_TARGET = 2,
	PATH_TEMP = 3,
	PATH_USER = 4,
};

enum
{
	FILE_COPY = 1,
	FILE_MOVE = 2,
};

extern int STR_ReverseFind(LPCTSTR lpszString,TCHAR c);

extern void CopyString(LPTSTR* ppDestString,LPCTSTR pSourceString);

class CUpdateBaseClass
{
public:
	CUpdateBaseClass();
	virtual ~CUpdateBaseClass();

	BOOL DeleteFileEx(LPTSTR lpszFileName);

	int m_nUpdateType;	

};

typedef std::deque<CUpdateBaseClass*> UPDATE_LIST;

#endif // !defined(AFX_UPDATEBASECLASS_H__EBC1D72C_B7DE_4551_A681_73C0CCCBB070__INCLUDED_)
