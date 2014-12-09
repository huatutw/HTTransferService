// ExtraRegKey.h: interface for the CExtraRegKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTRAREGKEY_H__268DD9D1_960F_42D1_BE24_E30354486EA8__INCLUDED_)
#define AFX_EXTRAREGKEY_H__268DD9D1_960F_42D1_BE24_E30354486EA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "atlbase.h"

class CExtraRegKey
{
public:
	CExtraRegKey();
	virtual ~CExtraRegKey();
	
public:
	BOOL QueryValue(DWORD& dwValue,LPCTSTR strRoot,LPCTSTR strName);
	BOOL SetKeyValue(DWORD dwValue,LPCTSTR strRoot,LPCTSTR strSubItem,LPCTSTR strName);
	BOOL TestWrite(LPCTSTR strSubItem);
};

#endif // !defined(AFX_EXTRAREGKEY_H__268DD9D1_960F_42D1_BE24_E30354486EA8__INCLUDED_)
