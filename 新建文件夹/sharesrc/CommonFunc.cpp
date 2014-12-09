// CommonFunc.cpp: implementation of the CCommonFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommonFunc::CCommonFunc()
{

}

CCommonFunc::~CCommonFunc()
{

}

void CCommonFunc::GetTempFilePath(LPCTSTR strFileName,LPTSTR strPathName)
{
	TCHAR strTempPath[DG_MAX_PATH+1] = {0};
	GetTempPath(DG_MAX_PATH,strTempPath);
	
	if (strTempPath[_tcslen(strTempPath)-1] == '\\')
	{
		_stprintf(strPathName,_T("%s%s"),strTempPath,strFileName);
	}else
	{
		_stprintf(strPathName,_T("%s\\%s"),strTempPath,strFileName);
	}
}

LPTSTR CCommonFunc::Int2Str(int num,LPTSTR strNumber)
{
	_stprintf(strNumber,_T("%d"),num);
	return strNumber;
}

LPTSTR CCommonFunc::BackupString(LPCTSTR strTemplate)
{
	if (NULL == strTemplate)
	{
		return NULL;
	}

	int len = _tcslen(strTemplate);

	if (len == 0)
		return NULL;

	LPTSTR strNewString = new TCHAR[len+1];

	memset(strNewString,0,sizeof(TCHAR)*(len+1));

	_tcscpy(strNewString,strTemplate);
}