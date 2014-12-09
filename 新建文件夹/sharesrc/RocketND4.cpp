// RocketND4.cpp: implementation of the CRocketND4 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RocketND4.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRocketND4::CRocketND4()
{

}

CRocketND4::~CRocketND4()
{

}


WORD CRocketND4::CheckDog()		//检查微狗是否存在
{
	int count = GetCurDogCount();
	if (count == 0)
		return ERR_NODOG;
	if (count > 1)
	{
		//ClsSetLastError(ERR_MORE_DOG);
		return ERR_NODOG;
	}
	return ERR_SUCCESS;
}

BOOL CRocketND4::GetCurDogCount()//得到当前的符合口令的微狗个数
{
	WORD wRetCode = 0;
	DWORD dwPara1 = 0;
	DWORD dwPara2 = 0;
	BYTE buffer[1024];

	WORD handle[16] = {0};
	
	wRetCode = Rockey(RY_FIND, &handle[0], &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);

	if (wRetCode != ERR_SUCCESS)
	{
		ClsSetLastError(wRetCode);
		return 0;
	}

	int nDogCount = 1;
	
	while (wRetCode == ERR_SUCCESS)
	{
		wRetCode = Rockey(RY_FIND_NEXT, &handle[nDogCount], &dwPara1, &dwPara2,&m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
		if (wRetCode == ERR_NOMORE) 
			break;
		if (wRetCode != ERR_SUCCESS)
		{
			return nDogCount;
		}

		nDogCount++;
	}

	return nDogCount;
}

BOOL CRocketND4::ClearAllDogData()
{
	if (!CheckDog())
		return FALSE;

	int i = 0;
	for (i=0;i<16;i++)
		SetModuleMode(i,0);

	WORD wRetCode = 0;
	DWORD dwPara1 = 0;
	DWORD dwPara2 = 0;
	BYTE buffer[1024] = {0};
	WORD handle;

	wRetCode = Rockey(RY_FIND, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	wRetCode = Rockey(RY_OPEN, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	if (wRetCode != ERR_SUCCESS)
	{
		ClsSetLastError(wRetCode);
		return FALSE;
	}

	for (i=0;i<1024;i++)
	{
		buffer[i] = 0;
	}
	
	WORD p1=0;
	WORD p2=512;
	WORD p3=0;
	WORD p4=0;

	wRetCode = Rockey(RY_WRITE, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer );

	wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);

	return TRUE;
}

DWORD CRocketND4::GetDogID()
{
	WORD wRetCode = 0;
	DWORD dwPara1 = 0;
	DWORD dwPara2 = 0;
	BYTE buffer[1024] = {0};
	WORD handle;
	WORD p1=0,p2=0,p3=0,p4=0;
	wRetCode = Rockey(RY_FIND, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	return dwPara1;
}

void CRocketND4::ClsSetLastError(int error)
{
	m_nErrorCode = error;
}


BOOL CRocketND4::SetModuleMode(WORD wModuleCode,WORD wModuleContent)
{
	if (!CheckDog())
		return FALSE;
	
	WORD wRetCode = 0;
	DWORD dwPara1 = 0;
	DWORD dwPara2 = 0;
	BYTE buffer[1024] = {0};
	WORD handle;
	WORD p1=wModuleCode;//模块号
	WORD p2=wModuleContent;//模块字 非0表示该模块号有效
	WORD p3=0;//不允许递减
	WORD p4=0;//无用
	wRetCode = Rockey(RY_FIND, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	wRetCode = Rockey(RY_OPEN, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	wRetCode = Rockey(RY_SET_MOUDLE,&handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4, buffer);
	if (wRetCode != 0)
	{
		wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
		return FALSE;
	}
	wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	return TRUE;
}

BOOL CRocketND4::IsModuleValid(WORD wModuleCode)
{
	if (!CheckDog())
		return FALSE;
	
	WORD wRetCode = 0;
	DWORD dwPara1 = 0;
	DWORD dwPara2 = 0;
	BYTE buffer[1024] = {0};
	WORD handle;
	WORD p1=wModuleCode;//模块号
	WORD p2=0;//模块字 非0表示该模块号有效
	WORD p3=0;//不允许递减
	WORD p4=0;//无用
	wRetCode = Rockey(RY_FIND, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	wRetCode = Rockey(RY_OPEN, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	wRetCode = Rockey(RY_CHECK_MOUDLE,&handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4, buffer);
	wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	if (p2 == 1)
	{
		return TRUE;
	}
	return FALSE;
}

WORD CRocketND4::BuffToDWORD(const char* buffer)
{
	WORD d = 0;
	int len = strlen(buffer);

	if (len != 4)
		return FALSE;

	CAString str = buffer;
	str.MakeLower();

	WORD ret = 0; 
	for (int i=0;i<4;i++)
	{
		int nType = 0;
		if (str.GetAt(i) >= '0' && str.GetAt(i) <= '9')
			nType = 1;
		if (str.GetAt(i) >= 'a' && str.GetAt(i) <= 'f')
			nType = 2;
		if (nType == 0)
			return 0;
		if (nType == 1)
		{
			CAString strNum = str.GetAt(i);
			ret += _ttoi(strNum) << (3-i)*4;
		}

		if (nType == 2)
		{
			CAString strNum = str.GetAt(i);
			ret += (str.GetAt(i) - 'a' + 10) << (3-i)*4;
		}
	}
	d = ret;
	return d;
}

BOOL CRocketND4::WriteDogData(LPBYTE bt,int size)
{
	if (!CheckDog())
		return FALSE;
	
	WORD wRetCode = 0;
	DWORD dwPara1 = 0;
	DWORD dwPara2 = 0;
	BYTE buffer[1024];
	WORD handle;

	wRetCode = Rockey(RY_FIND, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	wRetCode = Rockey(RY_OPEN, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	if (wRetCode != ERR_SUCCESS)
	{
		ClsSetLastError(wRetCode);
		return FALSE;
	}
	
	WORD p1=0;
	WORD p2=size;
	WORD p3=0;
	WORD p4=0;

	wRetCode = Rockey(RY_WRITE, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,bt);

	wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);

	return TRUE;
}
	
BOOL CRocketND4::ReadDogData(LPBYTE bt,int size)
{
	WORD wRetCode = 0;
	DWORD dwPara1 = 0;
	DWORD dwPara2 = 0;
	BYTE buffer[1024];
	WORD handle;

	wRetCode = Rockey(RY_FIND, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	wRetCode = Rockey(RY_OPEN, &handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	if (wRetCode != ERR_SUCCESS)
	{
		ClsSetLastError(wRetCode);
		return FALSE;
	}
	
	WORD p1=0;
	WORD p2=size;
	WORD p3=0;
	WORD p4=0;

	wRetCode = Rockey(RY_READ, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,bt);

	wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	return TRUE;
}