// ClientDog.cpp: implementation of the CClientDog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClientDog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientDog::CClientDog(WORD pass1,WORD pass2,WORD pass3,WORD pass4)
{
	m_dwPass1 = pass1;
	m_dwPass2 = pass2;
	m_dwPass3 = pass3;
	m_dwPass4 = pass4;

	strcpy((char*)m_btFlag,"DocGuarder");
	int i = 0;
	for (i=0;i<16;i++)
	{
		m_wModule[i] = 0;
		m_dwModuleCount[i] = 0;
	}

	for (i=0;i<128;i++)
	{
		m_btKey[i] = 0;
	}
	m_dwDogVersion = 0;
	m_dwDogID = 0;
}

CClientDog::CClientDog(const char* pass1,const char* pass2,const char* pass3,const char* pass4)
{
	if (!pass1 || !pass2 || !pass3 || !pass4)
	{
		m_dwPass1 = 0;
		m_dwPass2 = 0;
		m_dwPass3 = 0;
		m_dwPass4 = 0;
	}else
	{
		m_dwPass1 = BuffToDWORD(pass1);
		m_dwPass2 = BuffToDWORD(pass2);
		m_dwPass3 = BuffToDWORD(pass3);
		m_dwPass4 = BuffToDWORD(pass4);
	}

	strcpy((char*)m_btFlag,"DocGuarder");
	int i = 0;
	for (i=0;i<16;i++)
	{
		m_wModule[i] = 0;
		m_dwModuleCount[i] = 0;
	}

	for (i=0;i<128;i++)
	{
		m_btKey[i] = 0;
	}
	m_dwDogVersion = 0;
	m_dwDogID = 0;
}

CClientDog::~CClientDog()
{

}

WORD CClientDog::WriteDogData()
{
	WORD Err = CheckDog();
	if (ERR_SUCCESS != Err)
		return Err;
	
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
		return wRetCode;
	}
	
	WORD p1=0;
	WORD p2=10;
	WORD p3=0;
	WORD p4=0;

	wRetCode = Rockey(RY_WRITE, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,m_btFlag);

	p1 += p2;
	p2 = sizeof(DWORD);

	memcpy(buffer,&m_dwDogID,sizeof(DWORD));

	wRetCode = Rockey(RY_WRITE, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer );

	p1 += p2;
	p2 = 16*sizeof(WORD);

	memcpy(buffer,m_wModule,16*sizeof(WORD));

	wRetCode = Rockey(RY_WRITE, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer);

	p1 += p2;
	p2 = 16*sizeof(DWORD);

	memcpy(buffer,m_dwModuleCount,16*sizeof(DWORD));

	wRetCode = Rockey(RY_WRITE, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer);

	p1 += p2;
	p2 = 128;

	wRetCode = Rockey(RY_WRITE, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,m_btKey);


	p1 += p2;
	p2 = sizeof(WORD);

	memcpy(buffer,&m_dwDogVersion,sizeof(DWORD));

	wRetCode = Rockey(RY_WRITE, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer);

	wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);

	return ERR_SUCCESS;
}

WORD CClientDog::ReadDogData()
{
	WORD Err = CheckDog();
	if (ERR_SUCCESS != Err)
		return Err;
	
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
		return wRetCode;
	}
	
	WORD p1=0;
	WORD p2=10;
	WORD p3=0;
	WORD p4=0;

	wRetCode = Rockey(RY_READ, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,m_btFlag);

//  	if ((0 != stricmp((char*)m_btFlag,"DocGuarder")) && (0 == stricmp((char*)m_btFlag,"vamtoo")))
//  		return ERR_OLDOPENNEWDOG;
// 
// 	if (0 != stricmp((char*)m_btFlag,"DocGuarder"))
// 		return ERR_NOTVTDOG;

	if (!RtlEqualMemory(m_btFlag,"DocGuarder",10))
	{
		if (RtlEqualMemory(m_btFlag,"vamtoo",6))
		{
			return ERR_OLDOPENNEWDOG;
		} 
		else
		{
			return ERR_NOTVTDOG;
		}
	}

	p1 += p2;
	p2 = sizeof(DWORD);

	wRetCode = Rockey(RY_READ, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer );
	
	memcpy(&m_dwDogID,buffer,sizeof(DWORD));

	p1 += p2;
	p2 = 16*sizeof(WORD);

	wRetCode = Rockey(RY_READ, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer );

	memcpy(m_wModule,buffer,sizeof(WORD)*16);

	p1 += p2;
	p2 = 16*sizeof(DWORD);

	wRetCode = Rockey(RY_READ, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer);

	memcpy(m_dwModuleCount,buffer,sizeof(DWORD)*16);

	p1 += p2;
	p2 = 128;
	
	memset(m_btKey,0,128);
	wRetCode = Rockey(RY_READ, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,m_btKey);

	p1 += p2;
	p2 = sizeof(WORD);
	
	wRetCode = Rockey(RY_READ, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer);

	memcpy(&m_dwDogVersion,buffer,sizeof(WORD));

	wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);

	return wRetCode;
}

WORD CClientDog::WriteDogBuffer(LPBYTE bt, int size)
{
	WORD Err = CheckDog();
	if (ERR_SUCCESS != Err)
		return Err;
	
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

WORD CClientDog::ReadDogBuffer(LPBYTE bt, int size)
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
		return wRetCode;
	}
	
	WORD p1=0;
	WORD p2=size;
	WORD p3=0;
	WORD p4=0;
	
	wRetCode = Rockey(RY_READ, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,bt);
	wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	if (wRetCode != ERR_SUCCESS)
	{
		ClsSetLastError(wRetCode);
		return wRetCode;
	}
	return ERR_SUCCESS;
}

BOOL CClientDog::ClearAllDogData()
{
	WORD Err = CheckDog();
	if (ERR_SUCCESS != Err)
		return Err;
	
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

CClientDogEx::CClientDogEx(WORD pass1,WORD pass2,WORD pass3,WORD pass4)
{
	m_dwPass1 = pass1;
	m_dwPass2 = pass2;
	m_dwPass3 = pass3;
	m_dwPass4 = pass4;
	
	strcpy((char*)m_btFlag,"vamtoo");
	//for (int i=0;i<MAXCOUNT;i++)
	//{
	//	m_wModule[i] = 0;
	//	m_dwModuleCount[i] = 0;
	//}
	int i = 0;
	for (i=0;i<128;i++)
	{
		m_btKey[i] = 0;
	}
	
	SYSTEMTIME tmptime = {0};

	for(i = 0; i < MAXCOUNT ;i++)
	{
		m_dwProduct[i] = 0;
		m_nCount[i] = 0;
		m_tTimeOver[i] = tmptime;
		m_dwTactics[i] = 0;
		m_dwModuleFlag[i] = 0;
		m_nProductViersion[i] = 0;

	}
	m_dwDogVersion = 0;
	m_dwDogID = 0;
}

CClientDogEx::CClientDogEx(const char* pass1,const char* pass2,const char* pass3,const char* pass4)
{
	if (!pass1 || !pass2 || !pass3 || !pass4)
	{
		m_dwPass1 = 0;
		m_dwPass2 = 0;
		m_dwPass3 = 0;
		m_dwPass4 = 0;
	}else
	{
		m_dwPass1 = BuffToDWORD(pass1);
		m_dwPass2 = BuffToDWORD(pass2);
		m_dwPass3 = BuffToDWORD(pass3);
		m_dwPass4 = BuffToDWORD(pass4);
	}
	
	strcpy((char*)m_btFlag,"vamtoo");
//	for (int i=0;i<MAXCOUNT;i++)
//	{
	//	m_wModule[i] = 0;
	//	m_dwModuleCount[i] = 0;
//	}
	
	for (int i=0;i<128;i++)
	{
		m_btKey[i] = 0;
	}
	m_dwDogVersion = 1;
	m_dwDogID = 0;
}

CClientDogEx::~CClientDogEx()
{
	
}

WORD CClientDogEx::WriteDogData()
{
	WORD Err = CheckDog();
	if (ERR_SUCCESS != Err)
		return Err;
	
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
		return wRetCode;
	}
	stDogInfo2 dog2;
	size_t nlen = sizeof(dog2);
	RtlZeroMemory(&dog2,nlen);
	memcpy(&dog2.btFlag,&m_btFlag,10);
	memcpy(&dog2.dwDogVersion,&m_dwDogVersion,sizeof(DWORD));
	memcpy(&dog2.dwDogID,&m_dwDogID,sizeof(DWORD));
	memcpy(&dog2.btKey,&m_btKey,128);
	memcpy(&dog2.dwProduct,m_dwProduct,sizeof(m_dwProduct));
	memcpy(&dog2.nCount,m_nCount,sizeof(m_nCount));
	memcpy(&dog2.tTimeOver,m_tTimeOver,sizeof(m_tTimeOver));
	memcpy(&dog2.dwTactics,m_dwTactics,sizeof(m_dwTactics));
	memcpy(&dog2.dwModuleFlag,m_dwModuleFlag,sizeof(m_dwModuleFlag));
	memcpy(&dog2.nProductViersion,m_nProductViersion,sizeof(m_nProductViersion));
	memcpy(&dog2.btGuid,&m_guid,sizeof(m_guid));
	memcpy(&dog2.nReserve,&m_nReserve,sizeof(int));
	memcpy(buffer,&dog2,nlen);
	WORD p1=0;
	WORD p2=nlen;
	WORD p3=0;
	WORD p4=0;
	wRetCode = Rockey(RY_WRITE, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer); 	
 	wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	return wRetCode;
}

WORD CClientDogEx::ReadDogData()
{
	WORD Err = CheckDog();
	if (ERR_SUCCESS != Err)
		return Err;
	
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
		return wRetCode;
	}
	
	stDogInfo2 dog2;
	size_t nlen = sizeof(dog2);
	WORD p1=0;
	WORD p2=nlen;
	WORD p3=0;
	WORD p4=0;
 	wRetCode = Rockey(RY_READ, &handle, &dwPara1, &dwPara2, &p1,&p2,&p3,&p4,buffer);
	if (ERR_SUCCESS != wRetCode)
	{
		Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
		return wRetCode;
	}
	memcpy(&dog2,buffer,nlen);
	if (!RtlEqualMemory(dog2.btFlag,"vamtoo",6))
	{
		if (RtlEqualMemory(dog2.btFlag,"DocGuarder",10))
		{
			return ERR_NEWOPENOLDDOG;
		} 
		else
		{
			return ERR_NOTVTDOG;
		}
	}

	memcpy(m_btFlag,dog2.btFlag,10);
	m_dwDogID = dog2.dwDogID;
 	m_dwDogVersion = dog2.dwDogVersion;
	memcpy(m_dwProduct,dog2.dwProduct,sizeof(WORD)*MAXCOUNT);
	memcpy(m_nCount,dog2.nCount,sizeof(INT)*MAXCOUNT);
	memcpy(m_tTimeOver,dog2.tTimeOver,sizeof(SYSTEMTIME)*MAXCOUNT);
	memcpy(m_dwTactics,dog2.dwTactics,sizeof(DWORD)*MAXCOUNT);
	memcpy(m_dwModuleFlag,dog2.dwModuleFlag,sizeof(DWORD)*MAXCOUNT);
	memcpy(m_nProductViersion,dog2.nProductViersion,sizeof(FLOAT)*MAXCOUNT);
	memcpy(m_btKey,dog2.btKey,128);
	memcpy(m_guid,dog2.btGuid,40);
	m_nReserve = dog2.nReserve;
	
 	wRetCode = Rockey(RY_CLOSE,&handle, &dwPara1, &dwPara2, &m_dwPass1, &m_dwPass2, &m_dwPass3, &m_dwPass4, buffer);
	return wRetCode;
}