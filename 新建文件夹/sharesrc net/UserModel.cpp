// UserModel.cpp: implementation of the CUserModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserModel.h"
#include "VirtualMemFile.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern LPTSTR STRDUP(LPCTSTR strData);

CUserModel::CUserModel()
{

}

CUserModel::~CUserModel()
{
 	MAP_USER::iterator itUser;
 	for (itUser = m_mapUser.begin(); itUser != m_mapUser.end(); itUser++)
 	{
		CUserInfoV4 *pUserInfo = itUser->second;
		if (pUserInfo)
		{
			delete pUserInfo;
			pUserInfo = NULL;
		}
 	}
	
	MAP_LEVEL::iterator itLevel;
 	for (itLevel = m_mapLevel.begin(); itLevel != m_mapLevel.end(); itLevel++)
 	{
		CLevelInfoV4 *pLevelInfo = itLevel->second;
		if (pLevelInfo)
		{
			delete pLevelInfo;
			pLevelInfo = NULL;
		}
 	}

	MAP_DEPARTMENT::iterator itDepart;
 	for (itDepart = m_mapDepartment.begin(); itDepart != m_mapDepartment.end(); itDepart++)
 	{
		CDepartmentV4 *pDepart = itDepart->second;
		if (pDepart)
		{
			delete pDepart;
			pDepart = NULL;
		}
 	}
}

CUserInfoV4* CUserModel::AddOneUser(LPTSTR strLoginName, LPTSTR m_strUserName, int nDepartID, LPTSTR strDepartName)
{
	if (NULL == strLoginName || _tcslen(strLoginName) == 0)
		return NULL;
	if (NULL == m_strUserName || _tcslen(m_strUserName) == 0)
		return NULL;
	
	CUserInfoV4* pUserInfo = FindUserByLoginName(strLoginName);
	if (pUserInfo)
	{
		pUserInfo->AddOntDepart(nDepartID, strDepartName);
		return NULL;
	}

	pUserInfo = new CUserInfoV4;

	pUserInfo->m_strLoginName = STRDUP(strLoginName);
	pUserInfo->m_strUserName = STRDUP(m_strUserName);
	pUserInfo->AddOntDepart(nDepartID, strDepartName);

	m_mapUser.insert(std::pair<CAString,CUserInfoV4*>(pUserInfo->m_strLoginName, pUserInfo));

	return pUserInfo;
}

CUserInfoV4* CUserModel::FindUserByLoginName(LPTSTR strLoginName)
{
	MAP_USER::iterator it = m_mapUser.find(strLoginName);

	if (it != m_mapUser.end())
	{
		return it->second;
	}

	return NULL;
}

CLevelInfoV4* CUserModel::FindLevelByLevelID(int nLevelID)
{
	MAP_LEVEL::iterator it = m_mapLevel.find(nLevelID);

	if (it != m_mapLevel.end())
	{
		return it->second;
	}

	return NULL;
}

CLevelInfoV4* CUserModel::FindLevelByLevelName(LPTSTR strLevelName)
{
	std::map<int,CLevelInfoV4*>::iterator i;
	for (i = m_mapLevel.begin(); i != m_mapLevel.end(); i++)
	{
		if(0 == _tcscmp(i->second->m_strLevelName, strLevelName))
		{
			return i->second;
		}
	}
	
	return NULL;
}

CLevelInfoV4* CUserModel::AddOneLevel(int nLevelID, LPTSTR strLevelName)
{
	if (NULL == strLevelName || _tcslen(strLevelName) == 0)
		return NULL;
	
	CLevelInfoV4* pLevelInfo = FindLevelByLevelID(nLevelID);
	if (pLevelInfo)
	{
		return NULL;
	}

	pLevelInfo = new CLevelInfoV4;

	pLevelInfo->m_nLevelID = nLevelID;
	pLevelInfo->m_strLevelName = STRDUP(strLevelName);

	m_mapLevel.insert(std::pair<int, CLevelInfoV4*>(nLevelID, pLevelInfo));

	return pLevelInfo;
}

//======================================================================
// 在第一层查找部门
//======================================================================
CDepartmentV4* CUserModel::FindDepartmentByID(int nDepartmentID)
{
	MAP_DEPARTMENT::iterator it = m_mapDepartment.find(nDepartmentID);

	if (it != m_mapDepartment.end())
	{
		return it->second;
	}

	return NULL;
}

/*
//======================================================================
// 在结构所有的成员中查找部门
//======================================================================
CDepartmentV4* CUserModel::FindDepartmentFromALL(int nDepartmentID)
{
	MAP_DEPARTMENT::iterator itDepart;
 	for (itDepart = m_mapDepartment.begin(); itDepart != m_mapDepartment.end(); itDepart++)
 	{
		CDepartmentV4*	pDepartment	= NULL;

		pDepartment = itDepart->second;
		if (pDepartment->m_nDepartID == nDepartmentID)
		{
			return pDepartment;
		}
		else
		{
			CDepartmentV4*	pDepart = NULL;
			pDepart = pDepartment->FindSubDepartmentFromALL(nDepartmentID);
			if (pDepart)
				return pDepart;
		}
	}

	return NULL;
}
*/

CDepartmentV4* CUserModel::AddOneDepartment(int nDepartID, LPTSTR strDepartName, BOOL bRoot/* = FALSE*/)
{
	if (NULL == strDepartName || _tcslen(strDepartName) == 0)
		return NULL;
	
	CDepartmentV4* pDepartInfo = FindDepartmentByID(nDepartID);
	if (pDepartInfo)
	{
		return NULL;
	}

	pDepartInfo = new CDepartmentV4;

	pDepartInfo->m_nDepartID = nDepartID;
	pDepartInfo->m_strDepartName = STRDUP(strDepartName);
	pDepartInfo->m_bRoot = bRoot;

	m_mapDepartment.insert(std::pair<int, CDepartmentV4*>(nDepartID, pDepartInfo));

	return pDepartInfo;
}

/*
CDepartmentV4* CUserModel::AddOneSubDepartToDepart(int nParDepartID, int nDepartID, LPTSTR strDepartName)
{
	CDepartmentV4	*pDepartInfo	= NULL;

	if (NULL == strDepartName || _tcslen(strDepartName) == 0)
		return NULL;

	//CDepartmentV4* pParDepart = FindDepartmentByID(nParDepartID);
	CDepartmentV4* pParDepart = FindDepartmentFromALL(nParDepartID);
	if (pParDepart)
	{
		pDepartInfo = pParDepart->AddOneSubDepartment(nDepartID, strDepartName);
		if (pDepartInfo)
		{
			//m_mapDepartment.insert(std::pair<int, CDepartmentV4*>(nDepartID, pDepartInfo));

			//m_mapDepartment.erase(nParDepartID);
			//m_mapDepartment.insert(std::pair<int, CDepartmentV4*>(nParDepartID, pParDepart));
		}
	}
	else
	{
		// 为第一级部门，或者视图??

		return NULL;
	}

	return pDepartInfo;
}
*/
CDepartmentV4* CUserModel::AddOneSubDepartToDepart(CDepartmentV4 *pParDepart, int nDepartID, LPTSTR strDepartName)
{
	CDepartmentV4	*pDepartInfo	= NULL;

	if (NULL == strDepartName || _tcslen(strDepartName) == 0)
		return NULL;

	pDepartInfo = pParDepart->AddOneSubDepartment(nDepartID, strDepartName);

	return pDepartInfo;
}

CUserInfoV4* CUserModel::AddOneSubUserToDepart(int nParDepartID, LPTSTR strLoginName, LPTSTR strUserName)
{
	CUserInfoV4	*pUserInfo	= NULL;

	if (NULL == strLoginName || _tcslen(strLoginName) == 0)
		return NULL;

	if (NULL == strUserName || _tcslen(strUserName) == 0)
		return NULL;

	CDepartmentV4* pParDepart = FindDepartmentByID(nParDepartID);
	if (pParDepart)
	{
		pUserInfo = pParDepart->AddOneSubUser(strLoginName, strUserName);
		if (pUserInfo)
		{
			//m_mapDepartment.erase(nParDepartID);
			//m_mapDepartment.insert(std::pair<int, CDepartmentV4*>(nParDepartID, pParDepart));
		}
	}
	else
		return NULL;

	return pUserInfo;
}

CLevelInfoV4* CUserModel::GetMapLevelByIndex(int nMapLevelIndex)
{
	int index = 0;
	std::map<int,CLevelInfoV4*>::iterator i;
	for (i = m_mapLevel.begin(); i != m_mapLevel.end(); i++)
	{
		if(index == nMapLevelIndex)
			return i->second;
		index++;
	}
	
	return NULL;
}

CUserInfoV4* CUserModel::GetMapUserByIndex(int nMapUserIndex)
{
	int index = 0;
	std::map<CAString,CUserInfoV4*>::iterator i;
	for (i = m_mapUser.begin(); i != m_mapUser.end(); i++)
	{
		if(index == nMapUserIndex)
			return i->second;
		index++;
	}
	
	return NULL;
}

CDepartmentV4* CUserModel::GetMapDepartByIndex(int nMapDepartIndex)
{
	int index = 0;
	std::map<int,CDepartmentV4*>::iterator i;
	for (i = m_mapDepartment.begin(); i != m_mapDepartment.end(); i++)
	{
		if(index == nMapDepartIndex)
			return i->second;
		index++;
	}
	
	return NULL;
}

extern LPTSTR STRDUP(LPCTSTR strData);

BOOL CUserModel::SetDataToBuffer(LPBYTE* btData,DWORD& dwSize,BOOL bUnicodeToAnsi,int nInfoType)
{
	CVirtualMemFile mem(1024*512);	//申请512的内存

	if(nInfoType & LEVELINFOR)
	{
		//将等级MAP记入内存
		int nLevelMapSize = m_mapLevel.size();
		mem.WriteData(&nLevelMapSize, sizeof(int));
		
		for(int nfor = 0; nfor < nLevelMapSize; nfor++)
		{
			CLevelInfoV4 * LevelInfoV4 = GetMapLevelByIndex(nfor);
			
			int nLevelID = LevelInfoV4->m_nLevelID;
			LPTSTR strLevelName = STRDUP(LevelInfoV4->m_strLevelName);
			
			mem.WriteData(&nLevelID, sizeof(int));
			mem.WriteString(strLevelName);	
			
			if(strLevelName)
			{
				delete strLevelName;
				strLevelName = NULL;
			}
		}
	}
	
	if(nInfoType & USERINFO)
	{
		//将用户MAP记入内存
		int nUserMapSize = m_mapUser.size();
		mem.WriteData(&nUserMapSize, sizeof(int));
		
		for(int nfor = 0; nfor < nUserMapSize; nfor++)
		{
			CUserInfoV4 * UserInfoV4 = GetMapUserByIndex(nfor);
			
			mem.WriteString(UserInfoV4->m_strLoginName);
			mem.WriteString(UserInfoV4->m_strUserName);
			
			int nUserDepartCount = 0;
			nUserDepartCount = UserInfoV4->GetUserDepartCount();
			mem.WriteData(&nUserDepartCount, sizeof(int));
			
			if(0 != nUserDepartCount)
			{
				for(int n = 0; n < nUserDepartCount; n++)
				{
					int nDepartID = UserInfoV4->GetDepartID(n);
					mem.WriteData(&nDepartID, sizeof(int));
				}
			}
		}
	}
	
	//将部门MAP记入内存
	if(nInfoType & DEPARTMENTINFO)
	{
		int nDepartMapSize = m_mapDepartment.size();
		mem.WriteData(&nDepartMapSize, sizeof(int));
		
		for(int nfor = 0; nfor < nDepartMapSize; nfor++)
		{
			CDepartmentV4 * DepartmentV4 = GetMapDepartByIndex(nfor);
			
			int nDepartID = DepartmentV4->m_nDepartID;
			LPTSTR strDepartName = STRDUP(DepartmentV4->m_strDepartName);
			
			mem.WriteData(&nDepartID, sizeof(int));
			mem.WriteString(strDepartName);	
			
			if(strDepartName)
			{
				delete strDepartName;
				strDepartName = NULL;
			}
			
			int nSubDepartSize = DepartmentV4->m_arrDepartment.size();
			mem.WriteData(&nSubDepartSize, sizeof(int));
			
			if(0 != nSubDepartSize )
				DepartmentV4->WriteSubDepartmentToBuffer(mem, nSubDepartSize);
		}
	}

	dwSize = mem.GetDataSize();

	*btData = new BYTE[mem.GetDataSize()];
	memcpy(*btData,mem.GetData(),mem.GetDataSize());
	
	return TRUE;
}

BOOL CUserModel::GetDataFromBuffer(LPBYTE btData,DWORD dwSize,BOOL bUnicodeToAnsi,int nInfoType)
{
	if (btData == NULL || dwSize == 0)
		return FALSE;
	
	CVirtualMemFile mem;
	mem.AttachMem(btData,dwSize);

	if(nInfoType & LEVELINFOR)
	{
		//将等级信息从内存中读出
		int nLevelMapSize = 0;
		mem.ReadData(&nLevelMapSize, sizeof(int));
		
		for(int nfor = 0; nfor < nLevelMapSize; nfor++)
		{
			int nLevelID = 0;
			LPTSTR strLevelName = _T("");
			
			mem.ReadData(&nLevelID, sizeof(int));

			strLevelName = mem.ReadString();
			//AfxMessageBox(strLevelName);

			AddOneLevel(nLevelID, strLevelName);
		}
	}
	
	if(nInfoType & USERINFO)
	{
		//将用户信息从内存中读出
		int nUserMapSize = 0;
		mem.ReadData(&nUserMapSize, sizeof(int));
		
		for(int nfor = 0; nfor < nUserMapSize; nfor++)
		{
			LPTSTR strLoginName = _T("");
			LPTSTR strUserName = _T("");
			
			strLoginName = mem.ReadString();
			strUserName = mem.ReadString();
			
			int nUserDepartCount = 0;
			mem.ReadData(&nUserDepartCount, sizeof(int));
			
			if(0 != nUserDepartCount)
			{
				for(int n = 0; n < nUserDepartCount; n++)
				{
					int nDepartID = 0;
					mem.ReadData(&nDepartID, sizeof(int));
					LPTSTR str = _T("");
					AddOneUser(strLoginName, strUserName, nDepartID, str);
				}
			}	
		}
	}
	
	if(nInfoType & DEPARTMENTINFO)
	{
		//将部门信息从内存中读出
		int nDepartMapSize = 0;
		mem.ReadData(&nDepartMapSize, sizeof(int));
		
		for(int nfor = 0; nfor < nDepartMapSize; nfor++)
		{
			int nDepartID = 0;
			LPTSTR strDepartName = _T("");
			
			mem.ReadData(&nDepartID, sizeof(int));
			strDepartName = mem.ReadString();
			
			CDepartmentV4 * DepartmentV4 = AddOneDepartment(nDepartID, strDepartName, FALSE);
			
			int nSubDepartSize = 0;
			mem.ReadData(&nSubDepartSize, sizeof(int));
			
			if(0 != nSubDepartSize)
				DepartmentV4->ReadSubDepartmentFromBuffer(mem, nSubDepartSize);
		}
	}
	
	return TRUE;
}

int CUserModel::GetMapLevelSize()
{
	return m_mapLevel.size();
}

int CUserModel::GetMapUserSize()
{
	return m_mapUser.size();
}

int CUserModel::GetMapDepartSize()
{
	return m_mapDepartment.size();
}


