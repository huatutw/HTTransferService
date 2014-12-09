// AField.h: interface for the CAField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AFIELD_H__1E6932C3_2234_47DF_9C6D_CCDD284CF7E6__INCLUDED_)
#define AFX_AFIELD_H__1E6932C3_2234_47DF_9C6D_CCDD284CF7E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CAString;
class CAdoFild;

class CAField  
{
public:
	CAString GetFieldName();
	CAField();
	virtual ~CAField();

//	CAdoFild	m_AdoField;
};

#endif // !defined(AFX_AFIELD_H__1E6932C3_2234_47DF_9C6D_CCDD284CF7E6__INCLUDED_)
