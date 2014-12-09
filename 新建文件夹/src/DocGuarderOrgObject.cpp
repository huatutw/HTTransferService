// DocGuarderOrgObject.cpp: implementation of the CDocGuarderOrgObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DocGuarderOrgObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDocGuarderOrgObject::CDocGuarderOrgObject()
{
	m_pParentNode = NULL;
}

CDocGuarderOrgObject::~CDocGuarderOrgObject()
{
	ReleaseObject();
}

void CDocGuarderOrgObject::ReleaseObject()
{
	_objectlist.empty();
}

void CDocGuarderOrgObject::AddObject(CUserBaseObject* object)
{
	_objectlist.push_back(object);
}

void CDocGuarderOrgObject::RemoveObject(CUserBaseObject* object)
{
	USER_OBJECT::iterator i;
	for (i=_objectlist.begin();i!=_objectlist.end();++i)
	{
		if (*i == object)
		{
			_objectlist.erase(i);
			break;
		}
	}
}

CUserBaseObject* CDocGuarderOrgObject::GetBaseObject()
{
	return this;
}