// AdoFild.h: interface for the CAdoFild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOFILD_H__697FAE14_FDD5_4F87_A5D0_7FC576AF4079__INCLUDED_)
#define AFX_ADOFILD_H__697FAE14_FDD5_4F87_A5D0_7FC576AF4079__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAString;
class CADateTime;

class CAdoFild  
{
public:
	CAdoFild();
	virtual ~CAdoFild();

	ADODB::FieldPtr		m_pField;
};

#endif // !defined(AFX_ADOFILD_H__697FAE14_FDD5_4F87_A5D0_7FC576AF4079__INCLUDED_)
