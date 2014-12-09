#include "StdAfx.h"
#include "TaskUnit.h"

CTaskUnit::CTaskUnit(void)
{
}

CTaskUnit::~CTaskUnit(void)
{
	closesocket(m_sComm);
}

void	CTaskUnit::DoWork()
{
	HANDLE h = CreateEvent(NULL , 0 , 0, NULL);
	WaitForSingleObject(h,500);
	CloseHandle(h);
	OutputDebugString(L"CTaskUnit::DoWork()");
}



