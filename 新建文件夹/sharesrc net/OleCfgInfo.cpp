// OleCfgInfo.cpp: implementation of the COleCfgInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OleCfgInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleCfgInfo::COleCfgInfo()
{
	m_nType = ITEM_OLE;
}

COleCfgInfo::~COleCfgInfo()
{
	m_CLSIDDeque.clear();
}

BOOL COleCfgInfo::FindCLSIDInDeque(CLSID clsid,int* pIndex)
{
	for (int i=0;i<m_CLSIDDeque.size();i++)
	{
		if (IsEqualCLSID(clsid,m_CLSIDDeque[i]))
		{
			if (pIndex != NULL)
			{
				*pIndex = i;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COleCfgInfo::AddCLSIDToDeque(CLSID clsid)
{
	if (!FindCLSIDInDeque(clsid))
	{
		m_CLSIDDeque.push_front(clsid);
		return TRUE;
	}
	return FALSE;
}

BOOL COleCfgInfo::DeleteCLSIDFromDeque(CLSID clsid)
{
	int index = FindCLSIDInDeque(clsid);
	if (index != -1)
	{
		DEQUE_CLSID::iterator i;
		for (i=m_CLSIDDeque.begin();i!=m_CLSIDDeque.end();++i)
		{
			if (IsEqualCLSID(clsid,*i))
			{
				m_CLSIDDeque.erase(i);
				return TRUE;
			}
		}
	}

	return FALSE;
}
/*
void COleCfgInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_clsVersion;

		int size = 0;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			CLSID clsid;
			ar.Read(&clsid,sizeof(CLSID));
			m_CLSIDDeque.push_back(clsid);
		}
	}else
	{
		ar << m_clsVersion;

		int size = m_CLSIDDeque.size();
		ar << size;
		for (int i=0;i<m_CLSIDDeque.size();i++)
		{
			ar.Write(&m_CLSIDDeque[i],sizeof(CLSID));
		}
	}
}
*/