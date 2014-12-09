// RegOperation.h: interface for the CRegOperation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGOPERATION_H__008ED1A1_E57D_44B3_80CE_D5F68B05BB65__INCLUDED_)
#define AFX_REGOPERATION_H__008ED1A1_E57D_44B3_80CE_D5F68B05BB65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UpdateBaseClass.h"

enum
{
	REG_LOCAL_MACHINE = 1,
	REG_CURRENT_USER,
}; 

enum
{
	REG_SETVALUE	= 1,
	REG_DELETEKEY	= 2,
	REG_CREATEKEY	= 3,
	REG_ADDVALUE	= 4,
	REG_REPLACEVALUE	= 5,
};

enum
{
	REG_INTEGER = 1,
	REG_STRING = 2,
};

class CRegOperation : public CUpdateBaseClass  
{
public:
	CRegOperation();
	virtual ~CRegOperation();

	int m_nRegType;
	LPTSTR m_strKey;

	int m_nValueType;

	LPTSTR m_strItem;
	LPTSTR m_strValue;
	DWORD m_dwValue;
	int m_nOPType; 
};

#endif // !defined(AFX_REGOPERATION_H__008ED1A1_E57D_44B3_80CE_D5F68B05BB65__INCLUDED_)
