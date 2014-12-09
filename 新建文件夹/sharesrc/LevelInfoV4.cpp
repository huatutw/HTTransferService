// LevelInfoV4.cpp: implementation of the CLevelInfoV4 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LevelInfoV4.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLevelInfoV4::CLevelInfoV4()
{

}

CLevelInfoV4::~CLevelInfoV4()
{
	if (m_strLevelName)
	{
		delete[] m_strLevelName;
	}
	m_strLevelName = NULL;
}
