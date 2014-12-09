// UserBaseObject.cpp: implementation of the CUserBaseObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserBaseObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserBaseObject::CUserBaseObject()
{
	m_lpszObjectName = NULL;
	m_lpszObjectDescript = NULL;
	m_lpszObjectID = NULL;
}

CUserBaseObject::~CUserBaseObject()
{
	if (m_lpszObjectName)
		delete[] m_lpszObjectName;
	m_lpszObjectName = NULL;

	if (m_lpszObjectDescript)
		delete[] m_lpszObjectDescript;
	m_lpszObjectDescript = NULL;

	if (m_lpszObjectID)
		delete[] m_lpszObjectID;
	m_lpszObjectID = NULL;
}

CUserBaseObject* CUserBaseObject::GetBaseObject()
{
	return NULL;
}

void CUserBaseObject::AddObject(CUserBaseObject* object)
{

}

void CUserBaseObject::RemoveObject(CUserBaseObject* object)
{
	
}