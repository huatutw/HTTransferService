// ClientNodeInfo.h: interface for the CClientNodeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTNODEINFO_H__54E82AD1_8CFA_453F_A3C9_2A91397378B9__INCLUDED_)
#define AFX_CLIENTNODEINFO_H__54E82AD1_8CFA_453F_A3C9_2A91397378B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define OVERTIME 100*60   //超时时间,10分钟

#include <deque>
using namespace std;

typedef deque<int> INT_DEQUE;

class CClientNodeInfo  
{
public:
	CClientNodeInfo();
	virtual ~CClientNodeInfo();

	LPTSTR m_strMac;				//网卡地址
	LPTSTR m_strComputer;		//计算机名
	LPTSTR m_strIP;

	BOOL m_bClient;				//是否为客户端登录

	LPTSTR m_strUser;			//用户名
	LPTSTR m_strPass;			//密码
	
	LPTSTR  m_pszVersion;


	BOOL m_bOverTime;			
	
	DWORD m_dwPriv;				//客户的权限
	DWORD m_dwOverTime;
	DWORD m_dwCryptLevel;
	int	  m_nPolicyType;
	INT_DEQUE m_arrPolicy;		

	DWORD GetClientPriv(HINSTANCE hBase,LPCTSTR strCurUser,BOOL bSystemDir = TRUE); //检查当前连接用户的权限
	DWORD GetClientPriv2(HINSTANCE hBase,LPCTSTR strCurUser,BOOL bSystemDir = TRUE);
	
	SYSTEMTIME m_PrevTime;			//上次连接时的时间
	DWORD	   m_dwPrevTickCount;	//系统启动后的间隔

	BOOL IsOverTime(DWORD dwCurTick); //判断是否超时
};

typedef deque<CClientNodeInfo*> CLIENT_DEQUE;

#endif // !defined(AFX_CLIENTNODEINFO_H__54E82AD1_8CFA_453F_A3C9_2A91397378B9__INCLUDED_)
