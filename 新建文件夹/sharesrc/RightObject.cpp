// RightObject.cpp: implementation of the CRightObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RightObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRightObject::CRightObject()
{
	memset(m_strRightName,0,32);
	memset(m_strRightDes,0,256);
	m_dwFeature = 0;
	m_dwPara = 0;
	m_nRightType = RIGHT_NORMAL;
}

CRightObject::~CRightObject()
{

}
