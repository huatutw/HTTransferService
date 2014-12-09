// SoftwareVersionInfo.cpp: implementation of the CSoftwareVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoftwareVersionInfo.h"


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
		free(m_lpszSoftVersion);

	m_lpszSoftVersion = NULL;
	delete m_pDllExeInfo;
	m_pDllExeInfo = NULL;

	delete m_pOleCfgInfo;
	m_pOleCfgInfo = NULL;

	delete m_pFileExtInfo;
		m_pFileExtInfo = NULL;
}

#ifndef NO_MFC_VERSION
void CSoftwareVersionInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_clsVersion;

		int len = 0;
		ar >> len;
		if (len > 0)
		{
			m_lpszSoftVersion = (char*)calloc(len+1,sizeof(char));
			ar.Read(m_lpszSoftVersion,len);
		}
	}else
	{
		ar << m_clsVersion;

		int len  = strlen(m_lpszSoftVersion);
		ar << len;
		if (len > 0)
			ar.Write(m_lpszSoftVersion,len);

	}
	m_pDllExeInfo->Serialize(ar);
	m_pOleCfgInfo->Serialize(ar);
	m_pFileExtInfo->Serialize(ar);
}

void CSoftwareVersionInfo::Serialize_Unicode(CArchive& ar)
{
	USES_CONVERSION;

	if (ar.IsLoading())
	{
		ar >> m_clsVersion;

		int len = 0;
		ar >> len;
		if (len > 0)
		{
			LPWSTR SoftVersion = (LPWSTR)calloc(len+1,sizeof(WCHAR));

			ar.Read(SoftVersion,len*sizeof(WCHAR));

			LPSTR tmp = W2A(SoftVersion);
			m_lpszSoftVersion = (char*)calloc(strlen(tmp)+1,sizeof(char));
			strcpy(m_lpszSoftVersion,tmp);
			free(SoftVersion);
		}
	}else
	{
		ar << m_clsVersion;

		int len  = strlen(m_lpszSoftVersion);

		LPWSTR SoftVersion = A2W(m_lpszSoftVersion);
		ar << len;
		if (len > 0)
			ar.Write(SoftVersion,len*sizeof(WCHAR));

	}
	m_pDllExeInfo->Serialize_Unicode(ar);
	m_pOleCfgInfo->Serialize_Unicode(ar);
	m_pFileExtInfo->Serialize_Unicode(ar);
}
#else
void CSoftwareVersionInfo::Serialize(HANDLE hFile,BOOL bRead)
{
	if (bRead)
	{
		DWORD read;
		ReadFile(hFile,&m_clsVersion,sizeof(int),&read,NULL);
		int len = 0;
		ReadFile(hFile,&len,sizeof(int),&read,NULL);
		if (len > 0)
		{
			m_lpszSoftVersion = (char*)calloc(len+1,sizeof(char));
			ReadFile(hFile,m_lpszSoftVersion,len,&read,NULL);
		}
	}else
	{

	}
	m_pDllExeInfo->Serialize(hFile,bRead);
	m_pOleCfgInfo->Serialize(hFile,bRead);
	m_pFileExtInfo->Serialize(hFile,bRead);
}

void CSoftwareVersionInfo::Serialize_Unicode(HANDLE hFile,BOOL bRead)
{
	USES_CONVERSION;
	if (bRead)
	{
		DWORD read;
		ReadFile(hFile,&m_clsVersion,sizeof(int),&read,NULL);
		int len = 0;
		ReadFile(hFile,&len,sizeof(int),&read,NULL);
		if (len > 0)
		{
			LPWSTR SoftVersion = (LPWSTR)calloc(len+1,sizeof(WCHAR));

			ReadFile(hFile,SoftVersion,len*sizeof(WCHAR),&read,NULL);

			LPSTR tmp = W2A(SoftVersion);
			m_lpszSoftVersion = (char*)calloc(strlen(tmp)+1,sizeof(char));
			strcpy(m_lpszSoftVersion,tmp);
			free(SoftVersion);
		}
	}else
	{

	}
	m_pDllExeInfo->Serialize_Unicode(hFile,bRead);
	m_pOleCfgInfo->Serialize_Unicode(hFile,bRead);
	m_pFileExtInfo->Serialize_Unicode(hFile,bRead);
}
#endif

CSoftwareVersionInfo* CSoftwareVersionInfo::CheckModule(const char* lpszModuleName)
{
	if (m_pDllExeInfo->CheckModule(lpszModuleName))
		return this;

	return NULL;
}

BOOL CSoftwareVersionInfo::CheckExt(const char* lpszFileName)
{
	return m_pFileExtInfo->CheckExt(lpszFileName);
}

BOOL CSoftwareVersionInfo::CheckOleCLSID(CLSID clsid)
{
	if (m_pOleCfgInfo)
	{
		for (int i=0;i<m_pOleCfgInfo->m_CLSIDDeque.size();i++)
		{
			if (IsEqualCLSID(clsid,m_pOleCfgInfo->m_CLSIDDeque[i]))
				return TRUE;
		}
	}
	return FALSE;
}