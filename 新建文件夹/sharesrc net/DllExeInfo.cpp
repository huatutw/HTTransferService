// DllExeInfo.cpp: implementation of the CDllExeInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DllExeInfo.h"
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
extern LPTSTR STRDUP(LPCTSTR strData);

CDllExeInfo::CDllExeInfo()
{
	m_nType = ITEM_APP;
	m_nCheckNum = 0;
}

CDllExeInfo::~CDllExeInfo()
{
	int size = m_ConfigDeque.size();
	for (int i=0;i<size;i++)
	{
		DllExeInfo* pInfo = m_ConfigDeque.front();
		m_ConfigDeque.pop_front();
		delete pInfo->lpszModuleName;
		delete pInfo;
	}
	m_nCheckNum = 0;
}

BOOL CDllExeInfo::AddDllExeInfo(LPCTSTR lpszModuleName)
{
	if (!IsModuleValid(lpszModuleName))
		return FALSE;

	if (FindDllExeInfo(lpszModuleName))
		return FALSE;

	DllExeInfo* pInfo = new DllExeInfo;
	memset(pInfo,0,sizeof(DllExeInfo));

	pInfo->bChecked = FALSE;
//	pInfo->lpszModuleName = (LPTSTR)malloc(sizeof(TCHAR)*(_tcslen(lpszModuleName)+1));//strdup(lpszModuleName);
//	_tcscpy(pInfo->lpszModuleName,lpszModuleName);
	pInfo->lpszModuleName = STRDUP(lpszModuleName);

	m_ConfigDeque.push_front(pInfo);
	return TRUE;
}

void CDllExeInfo::DeleteDllExeInfo(LPCTSTR lpszModuleName)
{
	MODULE_DEQUE::iterator i;
	for (i=m_ConfigDeque.begin();i!=m_ConfigDeque.end();++i)
	{
		if (_tcsicmp((*i)->lpszModuleName,lpszModuleName) == 0)
		{
			delete((*i)->lpszModuleName);
			m_ConfigDeque.erase(i);
			break;
		}
	}
}

BOOL CDllExeInfo::IsModuleValid(LPCTSTR lpszModuleName)
{
	int len = 0;
	len = _tcslen(lpszModuleName);
	if (len <= 4) //不能加入小于等于4个字节的模块名 即如1.exe,1.dll是合法的配置模块
		return FALSE;

/*	if (stricmp(".exe",&lpszModuleName[len-4]) == 0 ||
		stricmp(".dll",&lpszModuleName[len-4]) == 0 ||
		stricmp(".com",&lpszModuleName[len-4]) == 0)
		return TRUE;*/

	return TRUE;
}

DllExeInfo* CDllExeInfo::FindDllExeInfo(LPCTSTR lpszModuleName)
{
	if (!IsModuleValid(lpszModuleName))
		return NULL;

	for (int i=0;i<m_ConfigDeque.size();i++)
	{
		if (_tcsicmp(m_ConfigDeque[i]->lpszModuleName,lpszModuleName) == 0)
			return m_ConfigDeque[i];

	}
	return NULL;
}

void CDllExeInfo::CheckDllExeInfo(LPCTSTR lpszModuleName)
{
	DllExeInfo* pDllExeInfo = FindDllExeInfo(lpszModuleName);
	if (pDllExeInfo && !pDllExeInfo->bChecked)
	{
		pDllExeInfo->bChecked = TRUE;
		m_nCheckNum++;
	}
}

BOOL CDllExeInfo::IsAllModuleCheckSuccess()
{
	if (m_ConfigDeque.size() == m_nCheckNum)
		return TRUE;
	return FALSE;
}
/*
void CDllExeInfo::Serialize(CArchive& ar)
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
				DllExeInfo* pInfo = new DllExeInfo;
				pInfo->lpszModuleName = (LPTSTR)calloc(len+1,sizeof(TCHAR));
				ar.Read(pInfo->lpszModuleName,len*sizeof(TCHAR));
				pInfo->bChecked = FALSE;
				m_ConfigDeque.push_back(pInfo);
			}
		}
	}else
	{
		ar << m_clsVersion;

		int size = m_ConfigDeque.size();
		ar << size;
		for (int i=0;i<m_ConfigDeque.size();i++)
		{
			int len = _tcslen(m_ConfigDeque[i]->lpszModuleName);
			ar << len;
			if (len > 0)
			{
				ar.Write(m_ConfigDeque[i]->lpszModuleName,len*sizeof(TCHAR));
			}
		}
	}
}
*/
BOOL CDllExeInfo::CheckModule(LPCTSTR lpszModuleName)
{
	for (int i=0;i<m_ConfigDeque.size();i++)
	{
		DllExeInfo* pInfo = m_ConfigDeque[i];
		if (pInfo->bChecked == FALSE && _tcsicmp(pInfo->lpszModuleName,lpszModuleName) == 0)
		{
			m_nCheckNum++;
			pInfo->bChecked = TRUE;
		}
	}
	if (m_nCheckNum == m_ConfigDeque.size())
		return TRUE;

	return FALSE;
}

BOOL CDllExeInfo::DelDllExeInfo(LPTSTR lpszModuleName)
{
	MODULE_DEQUE::iterator i;
	for (i=m_ConfigDeque.begin();i!=m_ConfigDeque.end();++i)
	{
		if (_tcsicmp((*i)->lpszModuleName,lpszModuleName) == 0)
		{
			DllExeInfo	*pInfo;
			pInfo = *i;
			m_ConfigDeque.erase(i);

			delete(pInfo->lpszModuleName);
			delete pInfo;

			return TRUE;
		}
	}
	
	return FALSE;
}
