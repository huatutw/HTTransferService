// DogData.h: interface for the CDogData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOGDATA_H__27176B8F_B5FB_4D06_979B_6E5541E5DFC0__INCLUDED_)
#define AFX_DOGDATA_H__27176B8F_B5FB_4D06_979B_6E5541E5DFC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDogData  
{
public:
	CDogData();
	virtual ~CDogData();
public:
	DWORD m_dwDogID;
	WORD  m_wModule[16];
	DWORD m_dwModuleCount[16];
	BYTE m_btKey[128];

public:
	void CreateEncode(DWORD seed);
	
};

#endif // !defined(AFX_DOGDATA_H__27176B8F_B5FB_4D06_979B_6E5541E5DFC0__INCLUDED_)
