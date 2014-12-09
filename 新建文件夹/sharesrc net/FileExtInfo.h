// FileExtInfo.h: interface for the CFileExtInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEEXTINFO_H__88DA9DA0_F743_40FF_978A_8403DF6F200B__INCLUDED_)
#define AFX_FILEEXTINFO_H__88DA9DA0_F743_40FF_978A_8403DF6F200B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <deque>

using namespace std;

#include "TreeNode.h"

typedef deque<LPTSTR> STRING_DEQUE;

class CFileExtInfo : public CTreeNode
{
public:
	BOOL DelExtInfo(LPTSTR lpszExtName);
	CFileExtInfo();
	virtual ~CFileExtInfo();

	STRING_DEQUE m_FileExtDeque;

	BOOL FindExtByName(LPCTSTR lpszExtName);
	BOOL IsExtNameValid(LPCTSTR lpszExtName);
	BOOL AddOneExtName(LPCTSTR lpszExtName);

//	virtual void Serialize(CArchive& ar);

	BOOL CheckExt(LPCTSTR lpszFileName);
};

typedef deque<CFileExtInfo*> FILEEXT_DEQUE;

#endif // !defined(AFX_FILEEXTINFO_H__88DA9DA0_F743_40FF_978A_8403DF6F200B__INCLUDED_)
