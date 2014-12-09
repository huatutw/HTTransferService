// ExtraRegKey.cpp: implementation of the CExtraRegKey class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExtraRegKey.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExtraRegKey::CExtraRegKey()
{

}

CExtraRegKey::~CExtraRegKey()
{

}

BOOL CExtraRegKey::QueryValue(DWORD& dwValue,LPCTSTR strRoot,LPCTSTR strName)
{
	CRegKey _key;
	if (_key.Open(HKEY_LOCAL_MACHINE,strRoot,KEY_READ) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	LONG ret = _key.QueryValue(dwValue,strName);
	_key.Close();

	return TRUE;
}

BOOL CExtraRegKey::SetKeyValue(DWORD dwValue,LPCTSTR strRoot,LPCTSTR strSubItem,LPCTSTR strName)
{
	CRegKey _key;
	if (_key.Open(HKEY_LOCAL_MACHINE,strRoot,KEY_WRITE) != ERROR_SUCCESS)
	{
		if (_key.Create(HKEY_LOCAL_MACHINE,strRoot,REG_NONE,REG_OPTION_NON_VOLATILE,KEY_WRITE) != ERROR_SUCCESS)
		{
			return FALSE;
		}
		_key.Close();
	}

	if (_key.Open(HKEY_LOCAL_MACHINE,strSubItem,KEY_WRITE) != ERROR_SUCCESS)
	{
		if (_key.Create(HKEY_LOCAL_MACHINE,strSubItem,REG_NONE,REG_OPTION_NON_VOLATILE,KEY_WRITE))
		{
			return FALSE;
		}
	}

	BOOL bRet = (_key.SetValue(dwValue,strName) == ERROR_SUCCESS)?TRUE:FALSE;
	_key.Close();

	return bRet;
}

BOOL CExtraRegKey::TestWrite(LPCTSTR strSubItem)
{
	CRegKey _key;

	if (_key.Open(HKEY_LOCAL_MACHINE,strSubItem,KEY_WRITE) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	_key.Close();
	return TRUE;
}
