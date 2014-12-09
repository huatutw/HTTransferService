// DogData.cpp: implementation of the CDogData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DogData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDogData::CDogData()
{
	for (int i=0;i<16;i++)
	{
		m_wModule[i] = 0;
		m_dwModuleCount[i] = 0;
	}
}

CDogData::~CDogData()
{

}

void CDogData::CreateEncode(DWORD seed)
{
/*	for (int i=0;i<256;i++)
		m_btEncode[i] = i;
	srand(seed);
	for (int j=0;j<2000;j++)
	{
		int pos1 = rand() % 256;
		int pos2 = rand() % 256;
		BYTE c = m_btEncode[pos1];
		m_btEncode[pos1] = m_btEncode[pos2];
		m_btEncode[pos2] = c;
	}*/
}