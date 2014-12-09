// UserModel.h: interface for the CUserModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERMODEL_H__D869064D_CBA5_47FA_8B6D_7C73C55A0CF1__INCLUDED_)
#define AFX_USERMODEL_H__D869064D_CBA5_47FA_8B6D_7C73C55A0CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserInfoV4.h"
#include "LevelInfoV4.h"
#include "DepartmentV4.h"

enum
{
	USERINFO = 0x1,
	LEVELINFOR = 0x2,
	DEPARTMENTINFO = 0x4,
};

class CUserModel  
{
public:
	CUserModel();
	virtual ~CUserModel();

public:
	CUserInfoV4* AddOneUser(LPTSTR strLoginName, LPTSTR m_strUserName, int nDepartID, LPTSTR strDepartName);
	CUserInfoV4* FindUserByLoginName(LPTSTR strLoginName);
	
	CLevelInfoV4*	FindLevelByLevelID(int nLevelID);
	CLevelInfoV4*	FindLevelByLevelName(LPTSTR strLevelName);
	CLevelInfoV4*	AddOneLevel(int nLevelID, LPTSTR strLevelName);

	CDepartmentV4*	FindDepartmentByID(int nDepartmentID);
	//CDepartmentV4*	FindDepartmentFromALL(int nDepartmentID);
	CDepartmentV4*	AddOneDepartment(int nDepartID, LPTSTR strDepartName, BOOL bRoot = FALSE);

	//CDepartmentV4*	AddOneSubDepartToDepart(int nParDepartID, int nDepartID, LPTSTR strDepartName);
	CDepartmentV4*	AddOneSubDepartToDepart(CDepartmentV4 *pParDepart, int nDepartID, LPTSTR strDepartName);

	CUserInfoV4*	AddOneSubUserToDepart(int nParDepartID, LPTSTR strLoginName, LPTSTR strUserName);

	CLevelInfoV4*	GetMapLevelByIndex(int nMapLevelIndex);
	CUserInfoV4*	GetMapUserByIndex(int nMapUserIndex);
	CDepartmentV4*	GetMapDepartByIndex(int nMapDepartIndex);

	int GetMapLevelSize();
	int GetMapUserSize();
	int GetMapDepartSize();

	BOOL SetDataToBuffer(LPBYTE* btData,DWORD& dwSize,BOOL bUnicodeToAnsi,int nInfoType);
	BOOL GetDataFromBuffer(LPBYTE btData,DWORD dwSize,BOOL bUnicodeToAnsi,int nInfoType);

private:
	MAP_USER m_mapUser;
	MAP_LEVEL m_mapLevel;
	MAP_DEPARTMENT m_mapDepartment;
};

#endif // !defined(AFX_USERMODEL_H__D869064D_CBA5_47FA_8B6D_7C73C55A0CF1__INCLUDED_)
