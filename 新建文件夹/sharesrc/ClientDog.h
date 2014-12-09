// ClientDog.h: interface for the CClientDog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTDOG_H__A3A68711_68F7_475A_B48F_36E42F7E4BE5__INCLUDED_)
#define AFX_CLIENTDOG_H__A3A68711_68F7_475A_B48F_36E42F7E4BE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "stdafx.h"
#include "RocketND4.h"

class CClientDog : public CRocketND4  
{
public:
	CClientDog(WORD pass1 = 0,WORD pass2 = 0,WORD pass3 = 0,WORD pass4 = 0);
	CClientDog(const char* pass1,const char* pass2,const char* pass3,const char* pass4);

	virtual ~CClientDog();

	BYTE	m_btFlag[10];

	DWORD m_dwDogID;
	WORD  m_wModule[16];
	DWORD m_dwModuleCount[16];
	BYTE  m_btKey[128];

	DWORD m_dwDogVersion;

	WORD WriteDogData();
	WORD ReadDogData();
	WORD WriteDogBuffer(LPBYTE bt, int size);
	WORD ReadDogBuffer(LPBYTE bt,int size);
	BOOL ClearAllDogData();
};

class CClientDogEx : public CRocketND4
{
public:
	CClientDogEx(WORD pass1 = 0,WORD pass2 = 0,WORD pass3 = 0,WORD pass4 = 0);
	CClientDogEx(const char* pass1,const char* pass2,const char* pass3,const char* pass4);
	
	virtual ~CClientDogEx();

	BYTE m_btFlag[10];
	DWORD m_dwDogID;
	BYTE  m_btKey[128];
	DWORD m_dwDogVersion;
	BYTE m_guid[40];
	INT m_nReserve;

	WORD m_dwProduct[MAXCOUNT];//产品数（如dwModule [0] = 1表示可用DocGuarder）
	int m_nCount[MAXCOUNT];//节点数（如nCount [0] = 100表示产品DocGuarder节点数100，-1表示无限制）
	SYSTEMTIME m_tTimeOver[MAXCOUNT];//产品到期时间（如tTimeOver[0]表示DocGuarder的到期时间，全0表示无限制）
	DWORD m_dwTactics[MAXCOUNT];//产品到期策略 0：无策略、0x1：策略一、0x2：策略二......
	DWORD m_dwModuleFlag[MAXCOUNT];	//模块标识（如：审批0x1外发0x2授权加密0x4）
	FLOAT m_nProductViersion[MAXCOUNT];//支持的产品版本

	WORD WriteDogData();
	WORD ReadDogData();
};

#endif // !defined(AFX_CLIENTDOG_H__A3A68711_68F7_475A_B48F_36E42F7E4BE5__INCLUDED_)
