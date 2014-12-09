// DownloadFile.cpp: implementation of the CDownloadFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloadFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloadFile::CDownloadFile()
{
	m_nUpdateType = UPDATE_DOWNLOADFILE;

	m_lpszSourceFile = NULL;
	m_lpszDestSubPath = NULL;
	m_lpszCommand = NULL;
	m_lpszLogin = NULL;
	m_lpszPass = NULL;
	m_lpszDomain = NULL;

	m_nPathType = PATH_SYSTEM;
	m_bNeedRegister = FALSE;
	m_bNeedRegServer = FALSE;
	m_bNeedRun = FALSE;
}

CDownloadFile::~CDownloadFile()
{
	if (m_lpszSourceFile)
	{
		delete m_lpszSourceFile;
		m_lpszSourceFile = NULL;
	}

	if (m_lpszDestSubPath)
	{
		delete m_lpszDestSubPath;
		m_lpszDestSubPath = NULL;
	}
}

void CDownloadFile::MakeDestFilePath()
{
	if (m_lpszDestSubPath == NULL)
	{
		return;
	}

	TCHAR strTemp[1024] = {0};
	_tcscpy(strTemp,m_lpszDestSubPath);

	switch(m_nPathType)
	{
	case PATH_SYSTEM:
		{
			TCHAR strSysDir[1024] = {0};
			GetSystemDirectory(strSysDir,1024);

			if (strSysDir[_tcslen(strSysDir)-1] == '\\')
			{
				_tcscat(strSysDir,strTemp);
			}else
			{
				_tcscat(strSysDir,_T("\\"));
				_tcscat(strSysDir,strTemp);
			}

			delete m_lpszDestSubPath;
			CopyString(&m_lpszDestSubPath,strSysDir);
			memset(strTemp,0,STR_LEN);
		}
		break;
	case PATH_TARGET:
		{
			TCHAR strCurrentDir[1024] = {0};
			GetModuleFileName(NULL,strCurrentDir,1024);

			int pos = STR_ReverseFind(strCurrentDir,'\\');
			
			_tcscpy(strCurrentDir+pos+1,strTemp);
			delete m_lpszDestSubPath;
			CopyString(&m_lpszDestSubPath,strCurrentDir);
			memset(strTemp,0,STR_LEN);				
		}
		break;
	case PATH_TEMP:
		{
			TCHAR strTempDir[1024] = {0};
			GetTempPath(1024,strTempDir);

			if (strTempDir[_tcslen(strTempDir)-1] == '\\')
			{
				_tcscat(strTempDir,strTemp);
			}else
			{
				_tcscat(strTempDir,_T("\\"));
				_tcscat(strTempDir,strTemp);
			}
			delete m_lpszDestSubPath;
			CopyString(&m_lpszDestSubPath,strTempDir);
			memset(strTemp,0,STR_LEN);
		}
		break;
	}
}
