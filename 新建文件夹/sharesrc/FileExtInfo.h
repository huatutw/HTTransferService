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

typedef deque<char*> STRING_DEQUE;

class CFileExtInfo : public CTreeNode
{
public:
	BOOL DelExtInfo(char *lpszExtName);
	CFileExtInfo();
	virtual ~CFileExtInfo();

	STRING_DEQUE m_FileExtDeque;

	BOOL FindExtByName(const char* lpszExtName);
	BOOL IsExtNameValid(const char* lpszExtName);
	BOOL AddOneExtName(const char* lpszExtName);

#ifndef NO_MFC_VERSION
	virtual void Serialize(CArchive& ar);
	virtual void Serialize_Unicode(CArchive& ar);
#else
	virtual void Serialize(HANDLE hFile,BOOL bRead);
	virtual void Serialize_Unicode(HANDLE hFile,BOOL bRead);
#endif

	BOOL CheckExt(const char* lpszFileName);

};

typedef deque<CFileExtInfo*> FILEEXT_DEQUE;

#endif // !defined(AFX_FILEEXTINFO_H__88DA9DA0_F743_40FF_978A_8403DF6F200B__INCLUDED_)
