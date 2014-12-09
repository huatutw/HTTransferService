// FileExtInfo.cpp: implementation of the CFileExtInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileExtInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern LPTSTR STRDUP(LPCTSTR strData);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileExtInfo::CFileExtInfo()
{
	m_nType = ITEM_EXT;
}

CFileExtInfo::~CFileExtInfo()
{
	int size = m_FileExtDeque.size();
	for (int i=0;i<size;i++)
	{
		LPTSTR lpszInfo = m_FileExtDeque.front();
		m_FileExtDeque.pop_front();
		delete(lpszInfo);
	}	
}

BOOL CFileExtInfo::FindExtByName(LPCTSTR lpszExtName)
{
	for (int i=0;i<m_FileExtDeque.size();i++)
	{
		if (_tcsicmp(m_FileExtDeque[i],lpszExtName) == 0)
			return TRUE;
	}
	return FALSE;
}

BOOL CFileExtInfo::IsExtNameValid(LPCTSTR lpszExtName)
{
	return TRUE;
}

BOOL CFileExtInfo::AddOneExtName(LPCTSTR lpszExtName)
{
	if (!IsExtNameValid(lpszExtName))
		return FALSE;
	if (FindExtByName(lpszExtName))
		return FALSE;

// 	LPTSTR pExt = (LPTSTR)malloc(sizeof(TCHAR)*(_tcslen(lpszExtName)+1));
// 	_tcscpy(pExt, lpszExtName);
	LPTSTR pExt = STRDUP(lpszExtName);
	m_FileExtDeque.push_front(pExt);

	return TRUE;
}
/*
void CFileExtInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_clsVersion;

		int size;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			int len = 0;
			ar >> len;
			if (len > 0)
			{
				LPTSTR lpszExtName = (LPTSTR)calloc(len+1,sizeof(TCHAR));
				ar.Read(lpszExtName,len*sizeof(TCHAR));
				
				m_FileExtDeque.push_back(lpszExtName);
			}
		}
	}else
	{
		ar << m_clsVersion;

		int size = m_FileExtDeque.size();
		ar << size;
		for (int i=0;i<m_FileExtDeque.size();i++)
		{
			int len = _tcslen(m_FileExtDeque[i]);
			ar << len;
			if (len > 0)
			{
				ar.Write(m_FileExtDeque[i],len*sizeof(TCHAR));
			}
		}
	}
}
*/
BOOL CFileExtInfo::CheckExt(LPCTSTR lpszFileName)
{
	if(lpszFileName == NULL)
		return FALSE;
	LPCTSTR lpStr = _tcsrchr(lpszFileName, TEXT('.'));
	if(lpStr == NULL)
		lpStr = lpszFileName;
	else 
		lpStr++;

	for (int i=0;i<m_FileExtDeque.size();i++)
	{
		if (_tcsicmp(m_FileExtDeque[i],lpStr) == 0)
			return TRUE;
	}
	return FALSE;
}

BOOL CFileExtInfo::DelExtInfo(LPTSTR lpszExtName)
{
	STRING_DEQUE::iterator i;
	for (i=m_FileExtDeque.begin();i!=m_FileExtDeque.end();++i)
	{
		if (_tcsicmp((*i),lpszExtName) == 0)
		{
			delete(*i);
			m_FileExtDeque.erase(i);
			
			return TRUE;
		}
	}
	
	return FALSE;
}
