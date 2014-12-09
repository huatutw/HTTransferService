// DepartmentV4.cpp: implementation of the CDepartmentV4 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DepartmentV4.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern LPTSTR STRDUP(LPCTSTR);

CDepartmentV4::CDepartmentV4()
{
	m_strDepartName = NULL;
	m_bRoot = FALSE;
}

CDepartmentV4::~CDepartmentV4()
{
	if (m_strDepartName)
	{
		delete[] m_strDepartName;
		m_strDepartName = NULL;
	}
	
	int size = m_arrDepartment.size();
	int nNum = 0;
	for (nNum = 0; nNum < size; nNum++)
	{
		CDepartmentV4* pDepartment = m_arrDepartment[nNum];
		delete pDepartment;
	}
	m_arrDepartment.clear();

	size = m_arrUser.size();
	for (nNum = 0; nNum < size; nNum++)
	{
		CUserInfoV4* pUser = m_arrUser[nNum];
		delete pUser;
	}
	m_arrUser.clear();
}

CDepartmentV4* CDepartmentV4::FindSubDepartmentByID(int nDepartID)
{
	std::deque<CDepartmentV4*>::iterator	it;
	CDepartmentV4							*pTemp	= NULL;

	int nSize = m_arrDepartment.size();
	for (int nNum = 0; nNum < nSize; nNum++)
	{
		pTemp = m_arrDepartment[nNum];
		if (pTemp->m_nDepartID == nDepartID)
		{
			return pTemp;
		}
	}

	return NULL;
}

/*
//==========================================================================
// 在下属的所有多级子部门中查询子部门
//==========================================================================
CDepartmentV4* CDepartmentV4::FindSubDepartmentFromALL(int nDepartID)
{
	int nSize = m_arrDepartment.size();
	for (int nNum = 0; nNum < nSize; nNum++)
	{
		CDepartmentV4* pDepartment = m_arrDepartment[nNum];

		if (pDepartment->m_nDepartID == nDepartID)
		{
			return pDepartment;
		}
		else
		{
			CDepartmentV4* pDepart	= NULL;
			pDepart = pDepartment->FindSubDepartmentFromALL(nDepartID);
			if (pDepart)
				return pDepart;
		}
	}

	return NULL;
}
*/

CDepartmentV4* CDepartmentV4::AddOneSubDepartment(int nDepartID, LPTSTR strDepartName)
{
	if (NULL == strDepartName || _tcslen(strDepartName) == 0)
		return NULL;

	CDepartmentV4* pDepartment = FindSubDepartmentByID(nDepartID);
	if (pDepartment)
	{
		return NULL;
	}

	pDepartment = new CDepartmentV4;
	pDepartment->m_nDepartID = nDepartID;
	pDepartment->m_strDepartName = STRDUP(strDepartName);

	m_arrDepartment.push_front(pDepartment);

	return pDepartment;
}

CUserInfoV4* CDepartmentV4::FindSubUserByLoginName(LPTSTR strLoginName)
{
	std::deque<CUserInfoV4*>::iterator	it;
	CUserInfoV4							*pTemp	= NULL;

	for (it = m_arrUser.begin(); it != m_arrUser.end(); it++)
	{
		if (_tcsicmp((*it)->m_strLoginName, strLoginName) == 0)
		{
			pTemp = (*it);

			return pTemp;
		}
	}

	return NULL;
}

CUserInfoV4* CDepartmentV4::AddOneSubUser(LPTSTR strLoginName, LPTSTR m_strUserName)
{
	if (NULL == strLoginName || _tcslen(strLoginName) == 0)
		return NULL;

	if (NULL == m_strUserName || _tcslen(m_strUserName) == 0)
		return NULL;

	CUserInfoV4* pUserInfo = FindSubUserByLoginName(strLoginName);
	if (pUserInfo)
	{
		return NULL;
	}

	pUserInfo = new CUserInfoV4;

	pUserInfo->m_strLoginName = STRDUP(strLoginName);
	pUserInfo->m_strUserName = STRDUP(m_strUserName);

	m_arrUser.push_front(pUserInfo);

	return pUserInfo;
}

void CDepartmentV4::WriteSubDepartmentToBuffer(CVirtualMemFile& mem, int nSubDepartSize)
{
	for(int nfor = 0; nfor < nSubDepartSize; nfor++)
	{
		int nDepartID = m_arrDepartment[nfor]->m_nDepartID;
		LPTSTR strDepartName = m_arrDepartment[nfor]->m_strDepartName;

		mem.WriteData(&nDepartID, sizeof(int));
		mem.WriteString(strDepartName);

		int nSubDepartmentSize = m_arrDepartment[nfor]->m_arrDepartment.size();
		mem.WriteData(&nSubDepartmentSize, sizeof(int));

		if(0 != nSubDepartmentSize)
			m_arrDepartment[nfor]->WriteSubDepartmentToBuffer(mem, nSubDepartmentSize);
	}
}

void CDepartmentV4::ReadSubDepartmentFromBuffer(CVirtualMemFile& mem, int nSubDepartSize)
{
	for(int nfor = 0; nfor < nSubDepartSize; nfor++)
	{
		int nDepartID = 0;
		LPTSTR strDepartName = _T("");
		
		mem.ReadData(&nDepartID, sizeof(int));
		strDepartName = mem.ReadString();
		
		CDepartmentV4 * Department = AddOneSubDepartment(nDepartID, strDepartName);
		
		int nSubDepartmentSize = 0;
		mem.ReadData(&nSubDepartmentSize, sizeof(int));

		if(0 != nSubDepartmentSize)
			Department->ReadSubDepartmentFromBuffer(mem, nSubDepartmentSize);
	}
}