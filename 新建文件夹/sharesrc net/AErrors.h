// AErrors.h: interface for the CAErrors class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AERRORS_H__A82608E3_D659_434F_8343_63B0CB1BF43C__INCLUDED_)
#define AFX_AERRORS_H__A82608E3_D659_434F_8343_63B0CB1BF43C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAString;
class CAError;
class CAdoErrors;


///////////////////////////////////////
// 错误集合
//	
///////////////////////////////////////

class CAErrors  
{
public:
	CAErrors();
	virtual ~CAErrors();

protected:
	CAdoErrors	*m_pAdoErrors;

public:
	void		SetErrors(CAdoErrors *pAdoErrors);
	//=============================================
	// 可以用来取得 Errors 物件集合中个別 Error 物件的內容
	//=============================================
	CAError*	GetItem( const _variant_t & Index );
	//=============================================
	// 移除 Errors 集合中的所有 Error 物件
	//=============================================
	BOOL		Clear();
};

#endif // !defined(AFX_AERRORS_H__A82608E3_D659_434F_8343_63B0CB1BF43C__INCLUDED_)
