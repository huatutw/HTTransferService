// UserInfoV4.h: interface for the CUserInfoV4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERINFOV4_H__744A2520_3901_4583_A04E_AAA6788BD729__INCLUDED_)
#define AFX_USERINFOV4_H__744A2520_3901_4583_A04E_AAA6788BD729__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LevelInfoV4.h"
#include <map>

enum
{
	RIGHT_OPEN = 1,
	RIGHT_ADJUST,
	RIGHT_DECRYPT,
	RIGHT_PRINT,
	RIGHT_READ,
	RIGHT_OUTSIDE,
	RIGHT_APPROVALDECRYPT,
	RIGHT_APPROVALOUTSIDE,
};

class CUserInfoV4  
{
public:
	CUserInfoV4();
	virtual ~CUserInfoV4();

	LPTSTR m_strLoginName;		//登录名
	LPTSTR m_strUserName;		//用户名
	LPTSTR m_strPassword;		//用户密码

	BOOL m_bIsBinding;			//是否已与计算机绑定
public:
	void SetUserRight(DWORD lRight);
	DWORD GetUserRight();
	void SetOverTime(int dwOverTime);
	int  GetOverTime();
	void SetCurrentLevel(int level);
	int	 GetCurrentLevel();

	void SetOneLevelInfo(int nType,int nLevelID);

	BOOL CanOpen(int nLevelID);
	BOOL CanAdjust(int nLevelID);
	BOOL CanDecrypt(int nLevelID);
	BOOL CanPrint(int nLevelID);
	BOOL CanRead(int nLevelID);
	BOOL CanOutSide(int nOutSideID);
	BOOL CanApprovalDecrypt(int nLevelID);
	BOOL CanApprovalOutSide(int nLevelID);

	int GetOpenFileSize();
	int GetAdjustFileSize();
	int GetDecryptFileSize();
	int GetPrintFileSize();
	int GetReadFileSize();
	int GetOutSideFileSize();
	int GetApprovalDecryptSize();
	int GetApprovalOutSideSize();
	int GetUserDepartCount();

	int GetOpenFile(int nOpen);
	int GetAdjustFile(int nAdjust);
	int GetDecryptFile(int nDecrypt);
	int GetPrintFile(int nPrint);
	int GetReadFile(int nPrint);
	int GetOutSideFile(int nOutSide);
	int GetApprovalDecrypt(int nApprovalDecrypt);
	int GetApprovalOutSide(int nApprovalOutSide);

	int GetDepartID(int nDepart);

	BOOL ExistDeparement(int nDepartID);
	void AddOntDepart(int nDepartID, LPTSTR strDepartName);
	BOOL IsUserDepart(int nDepartID);
private:

#ifdef DOCGUARDER42
	LONGLONG  m_dwRightGroup;	//用户权限
#else
	DWORD	m_dwRightGroup;
#endif
	
	int		m_dwOverTime;		//允许离网时间(小时)

	int		m_nLevelID;			//等级
	
	std::map<int,int>			m_arrOpenFile;		//打开等级
	std::map<int,int>			m_arrPrintFile;		//打印等级
	std::map<int,int>			m_arrAdjustFile;	//调整等级
	std::map<int,int>			m_arrDecryptFile;	//解密等级
	std::map<int,int>			m_arrReadFile;		//浏览等级
	std::map<int,int>			m_arrOutSideFile;		//外发等级
	std::map<int,int>			m_arrApprovalDecrypt;	//审批解密
	std::map<int,int>			m_arrApprovalOutSide;	//审批外发

	std::map<int,CAString>		m_arrDepartmentID;	//用户所属的部门ID	
};

typedef std::map<CAString,CUserInfoV4*> MAP_USER;


#endif // !defined(AFX_USERINFOV4_H__744A2520_3901_4583_A04E_AAA6788BD729__INCLUDED_)
