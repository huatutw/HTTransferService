// DocGuarderUserObject.cpp: implementation of the CDocGuarderUserObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DocGuarderUserObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDocGuarderUserObject::CDocGuarderUserObject()
{
	m_lpszLoginName = NULL;
	m_lpszPassword = NULL;
	m_bIsDomainMode = FALSE;
}

CDocGuarderUserObject::~CDocGuarderUserObject()
{
	ReleaseObject();
}

void CDocGuarderUserObject::ReleaseObject()
{
	if (m_lpszLoginName)
		delete[] m_lpszLoginName;
	m_lpszLoginName = NULL;

	if (m_lpszPassword)
		delete[] m_lpszPassword;
	m_lpszPassword = NULL;
}

CUserBaseObject* CDocGuarderUserObject::GetBaseObject()
{
	return this;
}