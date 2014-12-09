// DllExeInfo.h: interface for the CDllExeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLLEXEINFO_H__DDF3B3D0_0CDB_4DC0_B03A_F090924290A6__INCLUDED_)
#define AFX_DLLEXEINFO_H__DDF3B3D0_0CDB_4DC0_B03A_F090924290A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <deque>
using namespace std;

#include "TreeNode.h"

struct DllExeInfo
{
	LPTSTR lpszModuleName; //模块名
	BOOL bChecked;        //是否该模块已被检查
};

typedef deque<DllExeInfo*> MODULE_DEQUE;

class CDllExeInfo : public CTreeNode
{
public:
	BOOL DelDllExeInfo(LPTSTR lpszModuleName);
	CDllExeInfo();
	virtual ~CDllExeInfo();

	MODULE_DEQUE m_ConfigDeque;
	int m_nCheckNum;		//检查到匹配的个数

	BOOL IsModuleValid(LPCTSTR lpszModuleName);

	void CheckDllExeInfo(LPCTSTR lpszModuleName); //检查指定模块是否在配置中

	BOOL AddDllExeInfo(LPCTSTR lpszModuleName);
	void DeleteDllExeInfo(LPCTSTR lpszModuleName);

	DllExeInfo* FindDllExeInfo(LPCTSTR lpszModuleName);

	BOOL IsAllModuleCheckSuccess();

//	virtual void Serialize(CArchive& ar);
	
	BOOL CheckModule(LPCTSTR lpszModuleName);
};

#endif // !defined(AFX_DLLEXEINFO_H__DDF3B3D0_0CDB_4DC0_B03A_F090924290A6__INCLUDED_)
