// RocketND4.h: interface for the CRocketND4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROCKETND4_H__791905FA_2C43_4ECC_9B0B_4F0467D2553E__INCLUDED_)
#define AFX_ROCKETND4_H__791905FA_2C43_4ECC_9B0B_4F0467D2553E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Rockey4_ND_32.h"

class CRocketND4  
{
public:
	CRocketND4();
	virtual ~CRocketND4();
	
	WORD m_dwPass1;	  //一级密码1
	WORD m_dwPass2;   //一级密码2
	WORD m_dwPass3;	  //二级密码1
	WORD m_dwPass4;	  //二级密码2

	int	 m_nErrorCode;
	WORD CheckDog();		//检查微狗是否存在
	BOOL GetCurDogCount();  //得到当前的符合口令的微狗个数

	BOOL ClearAllDogData();
	DWORD GetDogID();

	void ClsSetLastError(int error);

	BOOL SetModuleMode(WORD wModuleCode,WORD wModuleContent);
	BOOL IsModuleValid(WORD wModuleCode);

	BOOL WriteDogData(LPBYTE bt,int size);
	BOOL ReadDogData(LPBYTE bt,int size);
	WORD BuffToDWORD(const char* buffer);
};

#endif // !defined(AFX_ROCKETND4_H__791905FA_2C43_4ECC_9B0B_4F0467D2553E__INCLUDED_)
