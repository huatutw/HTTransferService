// AdoParameters.h: interface for the CAdoParameters class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOPARAMETERS_H__EE922DF4_E9C9_42B8_8436_898607A11FAA__INCLUDED_)
#define AFX_ADOPARAMETERS_H__EE922DF4_E9C9_42B8_8436_898607A11FAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CAString;

class CAdoParameter;


class CAdoParameters
{
public:
	CAdoParameters();
	virtual ~CAdoParameters();

	ADODB::ParametersPtr	m_ptrParameters;

public:
	//==========================================
	// 新增一个 Parameter 到集合中。 
	//==========================================
	BOOL			Append(CAdoParameters *pAdoParameters);
	//==========================================
	// 刪除集合中的一个 Parameter 物件。 
	//==========================================
	BOOL			Delete(int nIndex);
	//==========================================
	// 更新 Parameters 物件集合中所做的改变。 
	//==========================================
	BOOL			Refresh();
	//==========================================
	// 回传集合中的 Parameter 的数量
	//==========================================
	long			GetCount();
	//==========================================
	// 可以用来取得集合中 parameter 物件的內容
	//==========================================
	CAdoParameter*	GetItem(int nIndex);

	ADODB::ParametersPtr&	operator =(ADODB::ParametersPtr& pParameters);
};

#endif // !defined(AFX_ADOPARAMETERS_H__EE922DF4_E9C9_42B8_8436_898607A11FAA__INCLUDED_)
