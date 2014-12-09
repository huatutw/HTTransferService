// MyBaseClass.cpp: implementation of the CMyBaseClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBaseClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBaseClass::CMyBaseClass()
{

}

CMyBaseClass::~CMyBaseClass()
{

}

BOOL CMyBaseClass::IsProcessWow64(DWORD dwProcessId)
{
	UINT unResult = 0;
	int nResult = 0;
	TCHAR szWinSysDir[MAX_PATH+1] = _T("");
	TCHAR szKernel32File[MAX_PATH+1+14] =_T("");
	HINSTANCE hLibKernel32 = NULL;
	BOOL bIsWow64Process = FALSE;
	BOOL (WINAPI *lpIsWow64Process)(HANDLE,PBOOL) = NULL;
	unResult = GetSystemDirectory(szWinSysDir,sizeof(szWinSysDir)/sizeof(TCHAR));
	if (unResult > 0) 
	{ 
		nResult = _stprintf(szKernel32File,_T("%s\\kernel32.dll"),szWinSysDir);
		if (nResult > 0) 
		{ 
			hLibKernel32 = LoadLibrary(szKernel32File);
		}
	} 
	if (NULL == hLibKernel32) 
	{ 
		hLibKernel32 = LoadLibrary(_T("kernel32.dll")); 
	} // Get the Address of Win32 API -- IsWow64Process() 
	if ( NULL != hLibKernel32 ) 
	{
		lpIsWow64Process = (BOOL (WINAPI *)(HANDLE,PBOOL)) GetProcAddress(hLibKernel32,"IsWow64Process");
	}
	if ( NULL != lpIsWow64Process ) 
	{ // Check whether the 32-bit program is running under WOW64 environment. 
		if (!lpIsWow64Process(GetCurrentProcess(),&bIsWow64Process)) 
		{ 
			FreeLibrary(hLibKernel32);
			return FALSE;
		}
	}
	return ( bIsWow64Process );
}

typedef UINT (WINAPI* GETWOWDIR)(
   LPTSTR lpBuffer,
   UINT uSize
);

LPTSTR CMyBaseClass::GetUserPath(int nPathType,LPTSTR strFileName)
{
	TCHAR strBasePath[1024] = {0};
	
	switch(nPathType)
	{
	case BASE_PATH_SYSTEM:
		{
			if (IsProcessWow64(GetCurrentProcessId()))
			{
				GETWOWDIR getwowdir = (GETWOWDIR)GetProcAddress(LoadLibrary(TEXT("kernel32.dll")),"GetSystemWow64DirectoryA");
				if (getwowdir)
				{
					getwowdir(strBasePath,1024);
				}else
				{
					GetSystemDirectory(strBasePath,1024);
				}
			}else
			{
				GetSystemDirectory(strBasePath,1024);
			}
		}
		break;
	case BASE_PATH_MODULE:
		{
			GetModuleFileName(NULL,strBasePath,1024);
		}
		break;
	case BASE_PATH_TEMP:
		{
			GetTempPath(1024,strBasePath);
		}
		break;
	case BASE_PATH_RAND:
		{
			GetTempPath(1024,strBasePath);
		}
		break;
	default:
		break;
	}

	
	return NULL;
}

BOOL CMyBaseClass::IsPathWithSymbol(LPTSTR lpszPath)
{
	if (lpszPath == NULL)
	{
		return FALSE;
	}

	int len = _tcslen(lpszPath);

	if (len <= 0)
	{
		return FALSE;
	}

	if (lpszPath[len-1] == '\\')
	{
		return TRUE;
	}

	return FALSE;
}

int CMyBaseClass::MyReverseFind(LPTSTR str,TCHAR c)
{
	if (str == NULL)
	{
		return -1;
	}

	int len = _tcslen(str);

	if (len == 0)
	{
		return -1;
	}

	for (int i=len-1;i>=0;i--)
	{
		if (str[i] == c)
		{
			return i;
		}
	}
	return -1;
}

void CMyBaseClass::ParsePath(LPTSTR strPathName,LPTSTR strPath,LPTSTR strFileName)
{
	int pos = MyReverseFind(strPathName,'\\');

	if (pos == -1)
		return;

	_tcscpy(strPath,strPathName);
	strPath[pos+1] = '\0';
	_tcscpy(strFileName,strPathName+pos+1);
}