// SoftwareVersionInfo.h: interface for the CSoftwareVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOFTWAREVERSIONINFO_H__6474DF7D_747D_4E76_B196_9292DA7C70D4__INCLUDED_)
#define AFX_SOFTWAREVERSIONINFO_H__6474DF7D_747D_4E76_B196_9292DA7C70D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DllExeInfo.h"
#include "OleCfgInfo.h"
#include "FileExtInfo.h"

class CSoftwareVersionInfo : public CTreeNode
{
public:
	CSoftwareVersionInfo();
	virtual ~CSoftwareVersionInfo();

//	virtual void Serialize(CArchive& ar);

	LPTSTR m_lpszSoftVersion; //软件的版本名称

	CDllExeInfo* m_pDllExeInfo;
	COleCfgInfo* m_pOleCfgInfo;
	CFileExtInfo* m_pFileExtInfo;

	CSoftwareVersionInfo* CheckModule(LPCTSTR lpszModuleName);
	
	BOOL CheckExt(LPCTSTR lpszFileName);
};

typedef deque<CSoftwareVersionInfo*> SOFTVER_DEQUE;

#endif // !defined(AFX_SOFTWAREVERSIONINFO_H__6474DF7D_747D_4E76_B196_9292DA7C70D4__INCLUDED_)
