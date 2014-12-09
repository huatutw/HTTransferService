// AParameters.h: interface for the CAParameters class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARAMETERS_H__C9BFB293_97DE_4B85_B981_7F57976A2E1B__INCLUDED_)
#define AFX_APARAMETERS_H__C9BFB293_97DE_4B85_B981_7F57976A2E1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAString;
class CAdoParameters;
class CAParameter;

class CAParameters  
{
public:
	CAParameters();
	virtual ~CAParameters();
	
	CAdoParameters	*m_pAdoParameters;

public:
	//==========================================
	// 新增一个 Parameter 到集合中。 
	//==========================================
	BOOL			Append(CAParameters &AParameters);
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
	CAParameter*	GetItem(int nIndex);
};

#endif // !defined(AFX_APARAMETERS_H__C9BFB293_97DE_4B85_B981_7F57976A2E1B__INCLUDED_)
