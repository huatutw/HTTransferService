// ClientInfo.h: interface for the CClientInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTINFO_H__6A71A44D_56CD_4BF2_8301_4FEF17281162__INCLUDED_)
#define AFX_CLIENTINFO_H__6A71A44D_56CD_4BF2_8301_4FEF17281162__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ClientNodeInfo.h"
class CClientDogEx;
#ifndef  MAXCOUNT
#define  MAXCOUNT 8
#endif
struct LicenseInfo2
{
	UINT mask;					//判断绑定的信息
	char strMacAddress[32];		//MAC地址
	char strHardID[32];			//硬盘号
	char strUserPass[1024];		//用户密码; 影响加密文件
	UINT uLicenseID;			//License标识 等同于DogID
	UINT uVersion;				//客户端版本  0x02012007D1 表示 2.1.2001版本
	UINT uLicenseVersion;		//License改动的次数,高版本的修改license工具才可以进行License操作
	UINT uCount;				//节点数
	SYSTEMTIME timeOver;			//过期时间
};

struct LicenseInfo3
{
	UINT mask;					//判断绑定的信息
	char strMacAddress[64];		//MAC地址
	char strHardID[64];			//硬盘号
	char strUserPass[1024];		//用户密码; 影响加密文件
	UINT uLicenseID;			//License标识 等同于DogID
	UINT uVersion;				//客户端版本  0x02012007D1 表示 2.1.2001版本
	UINT uLicenseVersion;		//License改动的次数,高版本的修改license工具才可以进行License操作
	UINT uCount;				//节点数
	SYSTEMTIME timeOver;			//过期时间
	int	 nTime;
	int  nReserve;
};


typedef struct LicenseInfo4
{
	BYTE btFlag[10];//软授权标识（默认为vamtoo）
	DWORD dwDogVersion;//软授权版本（默认为1）
	DWORD dwDogID;//狗号
	BYTE btKey[128];//密钥
	DWORD dwBandMask;//判断绑定的信息 BAND_MAC = 0x01BAND_HARDDISK = 0x02
	CHAR cBandInfo[128];//绑定信息
	WORD dwProduct[MAXCOUNT];//产品数（如dwModule [0] = 1表示可用DocGuarder）
	INT nCount[MAXCOUNT];//节点数（如nCount [0] = 100表示产品DocGuarder节点数100，-1表示无限制）
	SYSTEMTIME tTimeOver[MAXCOUNT];//产品到期时间（如tTimeOver[0]表示DocGuarder的到期时间，全0表示无限制）
	DWORD dwTactics[MAXCOUNT];//产品到期策略 0：无策略、0x1：策略一、0x2：策略二......
	DWORD dwModuleFlag[MAXCOUNT];//模块标识（如：审批0x1外发0x2授权加密0x4）
	FLOAT nProductViersion[MAXCOUNT];//支持的产品版本
	BYTE btGuid[40];//GUID字符串
	INT	nReserve;//保留字段
}LicenseInfo4,*PLicenseInfo4;
class CClientInfo  
{
public:
	CClientInfo();
	virtual ~CClientInfo();

	DWORD	m_dwAuthorizedMoules;

	HINSTANCE m_hBaseDll;

	CLIENT_DEQUE m_arrNodeInfo;			//正常连接的节点

	CLIENT_DEQUE m_arrOverTimeNodeInfo; //超时节点

	CClientNodeInfo* m_pCurrentNode;

//	CRocketDog m_dogAccess;				//微狗信息


	DWORD m_dwDogID;
	DWORD m_dwDogCount;
	BYTE  m_btKey[128];
	int	  m_nMode;
	int	  m_nTime;
	int	  m_nStrategy;
	LONGLONG  m_lDeadline;
	BOOL  m_bNewDog;
	BOOL  m_bNewSoftLic;

	CClientNodeInfo* FindNodeByKeyInNormal(LPCTSTR strComputer,LPCTSTR strMac);

	CClientNodeInfo* FindNodeByIP(LPCTSTR strIP);

	CClientNodeInfo* FindNodeByKeyInOverTime(LPCTSTR strComputer,LPCTSTR strMac);

	int GetNodeTypeInfo(int& OverTimeNode);
	void DeleteOverTimeNode(CClientNodeInfo* pInfo);

	void DeleteNormalNode(CClientNodeInfo* pInfo);

	void DeleteNode(CClientNodeInfo* pInfo);

	BOOL AddNewClientNode(LPCTSTR strComputer,LPCTSTR strMac,LPCTSTR strCurUser,LPCTSTR szHost);
	
	BOOL CheckAllOverTimeNode();

	BOOL CheckPass(LPCTSTR strUser,LPCTSTR strPass,int* nUserType,DWORD* pdwPriv);
	BOOL CheckPass2(LPCTSTR strUser,LPCTSTR strPass,int* nUserType,DWORD* pdwPriv,DWORD* pdwCryptLevel,int* pnPolicyType,INT_DEQUE& PolicyDeque);
	
	int GetUserCount();
	int GetDogInfo();



	void ParseVersion(WORD* pPro, FLOAT* pVer);
};

#endif // !defined(AFX_CLIENTINFO_H__6A71A44D_56CD_4BF2_8301_4FEF17281162__INCLUDED_)
