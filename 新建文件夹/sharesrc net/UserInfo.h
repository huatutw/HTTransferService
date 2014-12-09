// UserInfo.h: interface for the CUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERINFO_H__4A4BDAB7_2517_4564_9467_963AA91BA2F4__INCLUDED_)
#define AFX_USERINFO_H__4A4BDAB7_2517_4564_9467_963AA91BA2F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>
#include <map>
using namespace std;

#define USER_SA			1
#define USER_MANAGER	2
#define USER_NORMAL		3
#define USER_LOGMGER	4

enum
{
	DG_USER_FILE2 = 0,			//2.0 CUserInfo版本
	DG_USER_FILE3 = 1,			//3.0 CUserInfo版本
};

enum
{
	USR_FILE_VER2 = 1,			//2.0版本的Usr文件的版本
	USR_FILE_VER3 = 2,			//3.0版本的Usr文件的版本
};

//密级对应的角色和策略对应表;

//文件本身需要ID,在服务器上能够校验特殊用户

typedef deque<int> INT_DEQUE;

class CUserLevel		 //
{
public:
	CUserLevel();
	virtual ~CUserLevel();
	
	LPTSTR m_lpszLevelName;	//等级的名称
	DWORD m_dwCryptLevel;	//等级值,属于唯一的值,每个CUserLevel
	
	int m_nPolicyType;		//策略类型

	INT_DEQUE m_arrOpenPolicy;  //可以打开的等级
	INT_DEQUE m_arrDownPolicy;	//可以将文件等级降低的范围
	INT_DEQUE m_arrPrintPolicy;	//设置哪些等级能够打印该等级的文件
	INT_DEQUE m_arrCryptPolicy; //设置哪些等级能够解密该等级的文件

	void Serialize(CArchive& ar);
};

typedef deque<CUserLevel*> LEVEL_DEQUE;

class CUserInfo  
{
public:
	CUserInfo();
	virtual ~CUserInfo();

public:
	int	m_nClsVersion;

public:
	LPTSTR  m_pszUserName;	//用户名
	LPTSTR  m_pszUserInfo;	//登录名
	LPTSTR  m_pszUserDescribe;
	LPTSTR  m_pszPassword;
	int	  m_nUserType;
	int	  m_nOverTime;
	
	// 0x01--print 0x02--backup 0x04--copy 0x08--eptry File  0x10 snap srceen 0x20 ole
	long  m_nPopedom;	

	CUserLevel* m_pLevel;

	DWORD m_dwCryptLevel;	//0xffffffff  高位表示ID的Level,低位表示ID的序号,每个用户有唯一的ID;  最多65535个Level,65535个用户

	void ClearPolicyArray();

public:
	virtual void Serialize(CArchive& ar);
};

typedef deque<CUserInfo*> USERINFO_DEQUE;

class CDowithUserInfo
{
public:
	CDowithUserInfo();
	virtual ~CDowithUserInfo();

public:
	USERINFO_DEQUE		m_ptrUserInfo;
	
	//登录名到user的映射
	map<CString,CUserInfo*>  m_mapUserInfo;

	LEVEL_DEQUE			m_ptrLevelInfo;

	int					m_nClsVersion;

	CUserInfo*			m_pCurUserInfo;
public:
	CUserLevel* FindLevelByNameAndValue(LPCTSTR strName,DWORD dwCryptLevel);
	LPTSTR GetUserLevel(LPTSTR strUser);

	void DeleteLevelFromList(CUserLevel* pUserLevel);

	void InitFirstusr();
	BOOL IsLawUser(CString strUser, CString strPassword);
	BOOL CheckPass(CString strUser, CString strPassword);
	//BOOL CheckPass2(CString strUser, CString strPassword);
	BOOL DeleteItem(CUserInfo	*pInfo);
	void SaveUsersInfoToFile(LPCTSTR lpszFileName);
	void LoadUserInfo(LPCTSTR lpszFile);
	void GetMaxUserID();
	void CheckSelf();
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(AFX_USERINFO_H__4A4BDAB7_2517_4564_9467_963AA91BA2F4__INCLUDED_)
