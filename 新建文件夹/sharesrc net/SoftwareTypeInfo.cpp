// SoftwareTypeInfo.cpp: implementation of the CSoftwareTypeInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoftwareTypeInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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
		delete(m_lpszSoftTypeName);

	int size = m_arrSoftVersion.size();

	for (int i=0;i<size;i++)
	{
		CSoftwareVersionInfo* pInfo = m_arrSoftVersion.front();
		m_arrSoftVersion.pop_front();
		delete pInfo;
	}
	m_arrSoftVersion.clear();

}

CSoftwareVersionInfo* CSoftwareTypeInfo::FindSoftVersionByName(LPCTSTR lpszSoftVersion)
{
	for (int i=0;i<m_arrSoftVersion.size();i++)
	{
		if (_tcsicmp(m_arrSoftVersion[i]->m_lpszSoftVersion,lpszSoftVersion) == 0)
			return m_arrSoftVersion[i];
	}
	return NULL;
}
/*
void CSoftwareTypeInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_clsVersion;

		int len = 0;
		ar >> len;
		if (len > 0)
		{
			m_lpszSoftTypeName = (LPTSTR)calloc(len+1,sizeof(TCHAR));
			ar.Read(m_lpszSoftTypeName,len*sizeof(TCHAR));
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

		int len  = _tcslen(m_lpszSoftTypeName);
		ar << len;
		if (len > 0)
			ar.Write(m_lpszSoftTypeName,len*sizeof(TCHAR));

		int size = m_arrSoftVersion.size();
		ar << size;

		for (int i=0;i<m_arrSoftVersion.size();i++)
		{
			m_arrSoftVersion[i]->Serialize(ar);
		}
	}
}
*/
CSoftwareVersionInfo* CSoftwareTypeInfo::CheckModule(LPCTSTR lpszModuleName)
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
		if (_tcsicmp((*i)->m_lpszSoftVersion,pVersionInfo->m_lpszSoftVersion) == 0)
		{
			pTemp = (*i);
			m_arrSoftVersion.erase(i);
			delete pTemp;

			return TRUE;
		}
	}
	
	return FALSE;
}
