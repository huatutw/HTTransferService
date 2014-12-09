// DeleteFile.cpp: implementation of the CDeleteFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeleteFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeleteFile::CDeleteFile()
{
	m_nUpdateType = UPDATE_DELETEFILE;
	m_nDeleteMode = DELETE_DIRECT;
	m_lpszDestFileName = NULL;
	m_nPathType = PATH_SYSTEM;
}

CDeleteFile::~CDeleteFile()
{
	if (m_lpszDestFileName)
	{
		delete m_lpszDestFileName;
		m_lpszDestFileName = NULL;
	}
}

void CDeleteFile::MakeDestFilePath()
{
	if (m_lpszDestFileName == NULL)
	{
		return;
	}

	TCHAR strTemp[1024] = {0};
	wcscpy(strTemp,m_lpszDestFileName);

	switch(m_nPathType)
	{
	case PATH_SYSTEM:
		{
			TCHAR strSysDir[1024] = {0};
			GetSystemDirectory(strSysDir,1024);

			if (strSysDir[_tcslen(strSysDir)-1] == '\\')
			{
				wcscat(strSysDir,strTemp);
			}else
			{
				wcscat(strSysDir,_T("\\"));
				wcscat(strSysDir,strTemp);
			}

			delete(m_lpszDestFileName);
			CopyString(&m_lpszDestFileName,strSysDir);
			memset(strTemp,0,STR_LEN);
		}
		break;
	case PATH_TARGET:
		{
			TCHAR strCurrentDir[1024] = {0};
			GetModuleFileName(NULL,strCurrentDir,1024);

			int pos = STR_ReverseFind(strCurrentDir,'\\');
			
			_tcscpy(strCurrentDir+pos+1,strTemp);
			delete(m_lpszDestFileName);
			CopyString(&m_lpszDestFileName,strCurrentDir);
			memset(strTemp,0,STR_LEN);						
		}
		break;
	case PATH_TEMP:
		{
			TCHAR strTempDir[1024] = {0};
			GetTempPath(1024,strTempDir);

			if (strTempDir[_tcslen(strTempDir)-1] == '\\')
			{
				wcscat(strTempDir,strTemp);
			}else
			{
				wcscat(strTempDir,_T("\\"));
				wcscat(strTempDir,strTemp);
			}

			delete(m_lpszDestFileName);
			CopyString(&m_lpszDestFileName,strTempDir);
			memset(strTemp,0,STR_LEN);
		}
		break;
	}
}