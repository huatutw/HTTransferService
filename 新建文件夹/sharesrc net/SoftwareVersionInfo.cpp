// SoftwareVersionInfo.cpp: implementation of the CSoftwareVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoftwareVersionInfo.h"
/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoftwareVersionInfo::CSoftwareVersionInfo()
{
	m_nType = 2;
	m_pDllExeInfo = new CDllExeInfo;
	m_pOleCfgInfo = new COleCfgInfo;
	m_pFileExtInfo = new CFileExtInfo;
}

CSoftwareVersionInfo::~CSoftwareVersionInfo()
{
	if (m_lpszSoftVersion)
		delete m_lpszSoftVersion;

	m_lpszSoftVersion = NULL;
	delete m_pDllExeInfo;
	m_pDllExeInfo = NULL;

	delete m_pOleCfgInfo;
	m_pOleCfgInfo = NULL;

	delete m_pFileExtInfo;
	m_pFileExtInfo = NULL;
}
/*
void CSoftwareVersionInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_clsVersion;

		int len = 0;
		ar >> len;
		if (len > 0)
		{
			m_lpszSoftVersion = (LPTSTR)calloc(len+1,sizeof(TCHAR));
			ar.Read(m_lpszSoftVersion,len*sizeof(TCHAR));
		}
	}else
	{
		ar << m_clsVersion;

		int len  = _tcslen(m_lpszSoftVersion);
		ar << len;
		if (len > 0)
			ar.Write(m_lpszSoftVersion,len*sizeof(TCHAR));

	}
	m_pDllExeInfo->Serialize(ar);
	m_pOleCfgInfo->Serialize(ar);
	m_pFileExtInfo->Serialize(ar);
}
*/
CSoftwareVersionInfo* CSoftwareVersionInfo::CheckModule(LPCTSTR lpszModuleName)
{
	if (m_pDllExeInfo->CheckModule(lpszModuleName))
		return this;

	return NULL;
}

BOOL CSoftwareVersionInfo::CheckExt(LPCTSTR lpszFileName)
{
	return m_pFileExtInfo->CheckExt(lpszFileName);
}