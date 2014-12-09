// RecursionDir.cpp: implementation of the CRecursionDir class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RecursionDir.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LPTSTR CRecursionDir::m_lpszFileFilter = NULL;
BOOL CRecursionDir::m_bTest = TRUE;
TCHAR CRecursionDir::m_lpszRelationPath[1024] = {0};

CRecursionDir::CRecursionDir()
{
	m_lpszFileFilter = NULL;
}

CRecursionDir::~CRecursionDir()
{
	if (m_lpszFileFilter)
		delete[] m_lpszFileFilter;
	m_lpszFileFilter = NULL;
}

int CRecursionDir::RecursionDir(LPCTSTR lpszDir,DEALFILE CallBackFunc,PVOID pParam1,PVOID pParam2)
{
	TCHAR strPath[2048];

	_tcscpy(strPath,lpszDir);
	if (strPath[_tcslen(strPath)-1] != TEXT('\\'))
	{
		_tcscat(strPath,TEXT("\\"));
	}

	_tcscat(strPath,TEXT("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(strPath,&data);
	if (hFind == NULL || hFind == INVALID_HANDLE_VALUE)
		return REC_ERROR_NODIR;

	
	while(TRUE)
	{
		_tcscpy(strPath,data.cFileName);

		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //如果是目录
		{
			if (_tcsicmp(strPath,TEXT(".")) != 0 && _tcsicmp(strPath,TEXT("..")) != 0)
			{
				_tcscpy(strPath,lpszDir);
				if (strPath[_tcslen(strPath)-1] != TEXT('\\'))
				{
					_tcscat(strPath,TEXT("\\"));
				}
				_tcscat(strPath,data.cFileName);
				RecursionDir(strPath,CallBackFunc,pParam1,pParam2);
			}
		}else
		{
			_tcscpy(strPath,lpszDir);
			if (strPath[_tcslen(strPath)-1] != TEXT('\\'))
			{
				_tcscat(strPath,TEXT("\\"));
			}
			_tcscat(strPath,data.cFileName);
			CallBackFunc(strPath,pParam1,pParam2);

		}
		if (!FindNextFile(hFind,&data))
			break;
	}

	FindClose(hFind);
	return REC_ERROR_SUCCESS;
}

int CRecursionDir::RecursionDirEx(LPCTSTR lpszDir,DEALFILE CallBackFunc,PVOID pParam1,PVOID pParam2,LPCTSTR lpszNetCopyPath)
{
	TCHAR strPath[2048] = {0};
	TCHAR strNetPath[8192] = {0};

	_tcscpy(strNetPath,lpszNetCopyPath);

	_tcscpy(strPath,lpszDir);
	if (strPath[_tcslen(strPath)-1] != TEXT('\\'))
	{
		_tcscat(strPath,TEXT("\\"));
	}

	_tcscat(strPath,TEXT("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(strPath,&data);
	if (hFind == NULL || hFind == INVALID_HANDLE_VALUE)
		return REC_ERROR_NODIR;

	
	while(TRUE)
	{
		_tcscpy(strPath,data.cFileName);

		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //如果是目录
		{
			if (_tcsicmp(strPath,TEXT(".")) != 0 && _tcsicmp(strPath,TEXT("..")) != 0)
			{
				_tcscpy(strPath,lpszDir);
				if (strPath[_tcslen(strPath)-1] != TEXT('\\'))
				{
					_tcscat(strPath,TEXT("\\"));
				}

				TCHAR strFilePath[8192] = {0};

				_tcscpy(strFilePath,strNetPath);
				if (strFilePath[_tcslen(strFilePath)-1] != TEXT('\\'))
				{
					_tcscat(strFilePath,TEXT("\\"));
				}

				_tcscat(strFilePath,data.cFileName);
				_tcscat(strPath,data.cFileName);	
				CreateDirectory(strFilePath,NULL);
				RecursionDirEx(strPath,CallBackFunc,pParam1,pParam2,strFilePath);
			}
		}else
		{
			_tcscpy(strPath,lpszDir);
			if (strPath[_tcslen(strPath)-1] != TEXT('\\'))
			{
				_tcscat(strPath,TEXT("\\"));
			}
			_tcscat(strPath,data.cFileName);

			TCHAR strFilePath[8192] = {0};

			_tcscpy(strFilePath,strNetPath);
			if (strFilePath[_tcslen(strFilePath)-1] != TEXT('\\'))
			{
				_tcscat(strFilePath,TEXT("\\"));
			}

//			_tcscat(strFilePath,data.cFileName);
			CallBackFunc(strPath,strFilePath,data.cFileName);

		}
		if (!FindNextFile(hFind,&data))
			break;
	}

	FindClose(hFind);
	return REC_ERROR_SUCCESS;
}