// MyBaseClass.h: interface for the CMyBaseClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBASECLASS_H__0F72704F_F5AE_4DA0_995B_68BF58BB1E27__INCLUDED_)
#define AFX_MYBASECLASS_H__0F72704F_F5AE_4DA0_995B_68BF58BB1E27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum
{
	BASE_PATH_SYSTEM = 1,		//系统路径，x86，x64
	BASE_PATH_MODULE = 2,		//当前执行路径
	BASE_PATH_TEMP = 3,			//临时路径
	BASE_PATH_RAND = 4,			//随机文件.tmp
};

class CMyBaseClass  
{
public:
	CMyBaseClass();
	virtual ~CMyBaseClass();

	static LPTSTR GetUserPath(int nPathType,LPTSTR strFileName);
	static BOOL IsProcessWow64(DWORD dwProcessId);

	//判断某个路径是否最后结尾的是斜杠
	static BOOL IsPathWithSymbol(LPTSTR lpszPath);

	static void ParsePath(LPTSTR strPathName,LPTSTR strPath,LPTSTR strFileName);

	static int MyReverseFind(LPTSTR str,TCHAR c);

};

#endif // !defined(AFX_MYBASECLASS_H__0F72704F_F5AE_4DA0_995B_68BF58BB1E27__INCLUDED_)
