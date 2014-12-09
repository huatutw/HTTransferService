// RegOperation.cpp: implementation of the CRegOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegOperation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegOperation::CRegOperation()
{
	m_nUpdateType = UPDATE_REGOP;
	m_nRegType = REG_LOCAL_MACHINE;
	m_nValueType = REG_INTEGER;
	m_nOPType = 0;
	m_strKey = NULL;
	m_strValue = NULL;
	m_strItem = NULL;
}

CRegOperation::~CRegOperation()
{
	if (m_strValue)
	{
		delete m_strValue;
		m_strValue = NULL;
	}

	if (m_strKey)
	{
		delete m_strKey;
		m_strKey = NULL;
	}

	if (m_strItem)
	{
		delete m_strItem;
		m_strItem = NULL;
	}
}
