// LogBase.cpp: implementation of the CLogBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
LPTSTR STRDUP(LPCTSTR strData)
{
	int len = _tcslen(strData);
	LPTSTR str = new TCHAR[len+1];

	memset(str,0,(len+1)*sizeof(TCHAR));
	_tcscpy(str,strData);
	return str;
}

CLogBase::CLogBase()
{
	m_strComputer = NULL;
	m_strUser = NULL;
	m_strIP = NULL;
	m_strTime = NULL;
	m_strIndex = NULL;
	m_strGUID = NULL;
	m_nLogType = 0;
	m_nDeleted = 0;
	m_bUnicodeToAnsi = FALSE;
}

CLogBase::~CLogBase()
{
	if (m_strComputer)
	{
		delete[] m_strComputer;
		m_strComputer = NULL;
	}

	if (m_strUser)
	{
		delete[] m_strUser;
		m_strUser = NULL;
	}

	if (m_strIP)
	{
		delete[] m_strIP;
		m_strIP = NULL;
	}

	if (m_strIndex)
	{
		delete[] m_strIndex;
		m_strIndex = NULL;
	}

	if (m_strGUID)
	{
		delete[] m_strGUID;
		m_strGUID = NULL;
	}

	if (m_strTime)
	{
		delete[] m_strTime;
		m_strTime = NULL;
	}
}

BOOL CLogBase::GetDataFromBuffer(LPBYTE btData,DWORD dwSize)
{
	if (btData == NULL || dwSize == 0)
		return FALSE;

	CVirtualMemFile mem;
	mem.AttachMem(btData,dwSize);
	
//////////////////////////////////////////////////////////////////////////
//读取Record信息
	MemoryToRecord(mem);
//////////////////////////////////////////////////////////////////////////
	return TRUE;
}

BOOL CLogBase::SetDataToBuffer(LPBYTE* btData,DWORD& dwSize)
{
	if (btData == NULL || dwSize == 0)
		return FALSE;

	CVirtualMemFile mem(1024);	//申请1K的内存

	RecordToMemory(mem);

	*btData = new BYTE[mem.GetDataSize()];
	memcpy(*btData,mem.GetData(),mem.GetDataSize());

	dwSize = mem.GetDataSize();
	
	return TRUE;
}

void CLogBase::GetLocalInfo()
{
	TCHAR buffer[32] = {0};
	DWORD size = 32;
	GetComputerName(buffer,&size);
	if (m_strComputer)
		delete[] m_strComputer;

	m_strComputer = STRDUP(buffer);
	
	SYSTEMTIME cur;
	GetLocalTime(&cur);

	TCHAR t[1024] = {0};
	_stprintf(t,_T("%d-%d-%d %d:%d:%d"),cur.wYear,cur.wMonth,cur.wDay,cur.wHour,cur.wMinute,cur.wSecond);
	
	m_strTime = STRDUP(t);
}

#ifndef SAN_MU_VERSION	
void GetHostAddressEx(const char* strComputerName,char* lpszIP)
{
	hostent* host = gethostbyname(strComputerName);
	if(host == NULL) 
		return ;
	
	//如果有多个网卡,则用host->h_addr_list[1],host->h_addr_list[2]
	in_addr * ptr = (struct in_addr *) host->h_addr_list[0];
	
	int a = ptr->S_un.S_un_b.s_b1;  // 211
	int b = ptr->S_un.S_un_b.s_b2;  // 40
	int c = ptr->S_un.S_un_b.s_b3;  // 35
	int d = ptr->S_un.S_un_b.s_b4;  // 76
	
	sprintf(lpszIP,"%d.%d.%d.%d",a,b,c,d);
}
#endif