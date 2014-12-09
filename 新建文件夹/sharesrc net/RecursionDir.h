// RecursionDir.h: interface for the CRecursionDir class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECURSIONDIR_H__67D00916_7C7E_40DC_84BF_68A4182AEBB4__INCLUDED_)
#define AFX_RECURSIONDIR_H__67D00916_7C7E_40DC_84BF_68A4182AEBB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum  //REC_ERROR_CODE
{
	REC_ERROR_SUCCESS = 0,
	REC_ERROR_NODIR = 1, //找不到目录
};

typedef  BOOL (CALLBACK* DEALFILE)(LPCTSTR lpszFileName,PVOID pParam1,PVOID pParam2);

class CRecursionDir  
{
public:
	CRecursionDir();
	virtual ~CRecursionDir();

	static LPTSTR m_lpszFileFilter;
	static BOOL m_bTest;
	static TCHAR m_lpszRelationPath[1024]; //相对路径
	//遍历的目录及其参数
	static int RecursionDir(LPCTSTR lpszDir,DEALFILE CallBackFunc,PVOID pParam1,PVOID pParam2);
	static int RecursionDirEx(LPCTSTR lpszDir,DEALFILE CallBackFunc,PVOID pParam1,PVOID pParam2,LPCTSTR lpszNetCopyPath);
};

#endif // !defined(AFX_RECURSIONDIR_H__67D00916_7C7E_40DC_84BF_68A4182AEBB4__INCLUDED_)
