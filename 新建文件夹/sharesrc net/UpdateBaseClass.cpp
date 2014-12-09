// UpdateBaseClass.cpp: implementation of the CUpdateBaseClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpdateBaseClass.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CopyString(LPTSTR* ppDestString,LPCTSTR pSourceString)
{
	if (NULL == pSourceString)
		return;

	int size =  _tcslen(pSourceString);

	*ppDestString = new TCHAR[size + 1];
	memset(*ppDestString,0,(size+1)*sizeof(TCHAR));
	
	_tcscpy(*ppDestString,pSourceString);
}

int STR_ReverseFind(LPCTSTR lpszString,TCHAR c)
{
	if (lpszString == NULL)
		return -1;
	int len = _tcslen(lpszString);
	for (int i=len-1;i>=0;i--)
	{
		if (lpszString[i] == c)
			return i;
	}
	return -1;
}

CUpdateBaseClass::CUpdateBaseClass()
{
	m_nUpdateType = 0;
}

CUpdateBaseClass::~CUpdateBaseClass()
{

}

BOOL CUpdateBaseClass::DeleteFileEx(LPTSTR lpszFileName)
{
	TCHAR strTempPath[1024] = {0};
	//GetTempPath(1024,strTempPath);
	TCHAR tcDGPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, tcDGPath, MAX_PATH);
	CAString strDGPath = tcDGPath;
	int pos = strDGPath.ReverseFind(TEXT('\\'));
	strDGPath = strDGPath.Left(pos+1);

	CAString TempPath = strDGPath + _T("TEMP");
	CreateDirectory(TempPath, NULL);

	_tcscpy(strTempPath, TempPath);
 
	TCHAR strPath[1024] = {0};
	TCHAR strFileName[1024] = {0};

	CMyBaseClass::ParsePath(lpszFileName,strPath,strFileName);

	if (CMyBaseClass::IsPathWithSymbol(strTempPath))
	{	
		_tcscat(strTempPath,strFileName);
		_tcscat(strTempPath,_T(".tmp"));
	}else
	{
		_tcscat(strTempPath,_T("\\"));
		_tcscat(strTempPath,strFileName);
		_tcscat(strTempPath,_T(".tmp"));
	}

	if (MoveFileEx(lpszFileName,strTempPath,MOVEFILE_REPLACE_EXISTING))
	{
		return MoveFileEx(strTempPath,NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
	}
	else
	{
		return FALSE;
	}
	return FALSE;
}

