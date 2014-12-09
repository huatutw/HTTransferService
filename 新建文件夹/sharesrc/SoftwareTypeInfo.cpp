// SoftwareTypeInfo.cpp: implementation of the CSoftwareTypeInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoftwareTypeInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoftwareTypeInfo::CSoftwareTypeInfo()
{
	m_nType				= 1;
	m_lpszSoftTypeName	= NULL;	

	m_bSaveThisType		= TRUE;
}

CSoftwareTypeInfo::~CSoftwareTypeInfo()
{
	if (m_lpszSoftTypeName)
		free(m_lpszSoftTypeName);

	int size = m_arrSoftVersion.size();

	for (int i=0;i<size;i++)
	{
		CSoftwareVersionInfo* pInfo = m_arrSoftVersion.front();
		m_arrSoftVersion.pop_front();
		delete pInfo;
	}
	m_arrSoftVersion.clear();

}

CSoftwareVersionInfo* CSoftwareTypeInfo::FindSoftVersionByName(const char* lpszSoftVersion)
{
	for (int i=0;i<m_arrSoftVersion.size();i++)
	{
		if (stricmp(m_arrSoftVersion[i]->m_lpszSoftVersion,lpszSoftVersion) == 0)
			return m_arrSoftVersion[i];
	}
	return NULL;
}

#ifndef NO_MFC_VERSION
void CSoftwareTypeInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_clsVersion;

		int len = 0;
		ar >> len;
		if (len > 0)
		{
			m_lpszSoftTypeName = (char*)calloc(len+1,sizeof(char));
			ar.Read(m_lpszSoftTypeName,len);
		}
		int size = 0;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			CSoftwareVersionInfo* pInfo = new CSoftwareVersionInfo;
			pInfo->Serialize(ar);
			m_arrSoftVersion.push_back(pInfo);
		}
		
	}else
	{
		ar << m_clsVersion ;

		int len  = strlen(m_lpszSoftTypeName);
		ar << len;
		if (len > 0)
			ar.Write(m_lpszSoftTypeName,len);

		int size = m_arrSoftVersion.size();
		ar << size;

		for (int i=0;i<m_arrSoftVersion.size();i++)
		{
			m_arrSoftVersion[i]->Serialize(ar);
		}
	}
}

void CSoftwareTypeInfo::Serialize_Unicode(CArchive& ar)
{
	USES_CONVERSION;
	if (ar.IsLoading())
	{
		ar >> m_clsVersion;

		int len = 0;
		ar >> len;
		if (len > 0)
		{
			m_lpszSoftTypeName = (char*)calloc(len+1,sizeof(char));
			LPWSTR SoftTypeName = (LPWSTR)calloc(len+1,sizeof(WCHAR));
			ar.Read(SoftTypeName,len*sizeof(WCHAR));
			LPSTR tmp = W2A(SoftTypeName);
			m_lpszSoftTypeName = (char*)calloc(strlen(tmp)+1,sizeof(char));
			strcpy(m_lpszSoftTypeName,tmp);
			free(SoftTypeName);
		}
		int size = 0;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			CSoftwareVersionInfo* pInfo = new CSoftwareVersionInfo;
			pInfo->Serialize_Unicode(ar);
			m_arrSoftVersion.push_back(pInfo);
		}
		
	}else
	{
		ar << m_clsVersion ;

		int len  = strlen(m_lpszSoftTypeName);
		ar << len;
		if (len > 0)
		{
			LPWSTR SoftTypeName = A2W(m_lpszSoftTypeName);
			ar.Write(SoftTypeName,len*sizeof(WCHAR));
		}

		int size = m_arrSoftVersion.size();
		ar << size;

		for (int i=0;i<m_arrSoftVersion.size();i++)
		{
			m_arrSoftVersion[i]->Serialize(ar);
		}
	}
}
#else
void CSoftwareTypeInfo::Serialize(HANDLE hFile,BOOL bRead)
{
	if (bRead)
	{
		DWORD read;
		ReadFile(hFile,&m_clsVersion,sizeof(int),&read,NULL);
		int len = 0;
		ReadFile(hFile,&len,sizeof(int),&read,NULL);
		if (len > 0)
		{
			m_lpszSoftTypeName = (char*)calloc(len+1,sizeof(char));
			ReadFile(hFile,m_lpszSoftTypeName,len,&read,NULL);
		}
		int size = 0;
		ReadFile(hFile,&size,sizeof(int),&read,NULL);
		for (int i=0;i<size;i++)
		{
			CSoftwareVersionInfo* pInfo = new CSoftwareVersionInfo;
			pInfo->Serialize(hFile,bRead);
			m_arrSoftVersion.push_back(pInfo);
		}
	}else
	{

	}
}

void CSoftwareTypeInfo::Serialize_Unicode(HANDLE hFile,BOOL bRead)
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
			LPWSTR SoftTypeName = (LPWSTR)calloc(len+1,sizeof(WCHAR));
			ReadFile(hFile,SoftTypeName,len*sizeof(WCHAR),&read,NULL);
			
			LPSTR tmp = W2A(SoftTypeName);
			m_lpszSoftTypeName = (char*)calloc(strlen(tmp)+1,sizeof(char));
			strcpy(m_lpszSoftTypeName,tmp);
			free(SoftTypeName);
			
		}
		int size = 0;
		ReadFile(hFile,&size,sizeof(int),&read,NULL);
		for (int i=0;i<size;i++)
		{
			CSoftwareVersionInfo* pInfo = new CSoftwareVersionInfo;
			pInfo->Serialize_Unicode(hFile,bRead);
			m_arrSoftVersion.push_back(pInfo);
		}
	}else
	{

	}
}
#endif

CSoftwareVersionInfo* CSoftwareTypeInfo::CheckModule(const char* lpszModuleName)
{
	for (int i=0;i<m_arrSoftVersion.size();i++)
	{
		if (m_arrSoftVersion[i]->CheckModule(lpszModuleName))
			return m_arrSoftVersion[i];
	}
	return NULL;
}

BOOL CSoftwareTypeInfo::DeleteItem(CSoftwareVersionInfo *pVersionInfo)
{
	SOFTVER_DEQUE::iterator	i;
	CSoftwareVersionInfo		*pTemp = NULL;

	for (i = m_arrSoftVersion.begin(); i!=m_arrSoftVersion.end(); i++)
	{
		if (stricmp((*i)->m_lpszSoftVersion,pVersionInfo->m_lpszSoftVersion) == 0)
		{
			pTemp = (*i);
			m_arrSoftVersion.erase(i);
			delete pTemp;

			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CSoftwareTypeInfo::CheckOleCLSID(CLSID clsid)
{
	for (int i=0;i<m_arrSoftVersion.size();i++)
	{
		if (m_arrSoftVersion[i]->CheckOleCLSID(clsid))
			return TRUE;
	}
	return FALSE;
}