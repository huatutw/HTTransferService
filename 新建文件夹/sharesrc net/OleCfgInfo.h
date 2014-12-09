// OleCfgInfo.h: interface for the COleCfgInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLECFGINFO_H__6BC3A051_630C_4EA9_9E12_5BEC237CFDBD__INCLUDED_)
#define AFX_OLECFGINFO_H__6BC3A051_630C_4EA9_9E12_5BEC237CFDBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <deque>
using namespace std;

typedef deque<CLSID> DEQUE_CLSID;

#include "TreeNode.h"

class COleCfgInfo  : public CTreeNode 
{
public:
	COleCfgInfo();
	virtual ~COleCfgInfo();

	DEQUE_CLSID m_CLSIDDeque;

	BOOL FindCLSIDInDeque(CLSID clsid,int* pIndex = NULL);
	BOOL AddCLSIDToDeque(CLSID clsid);
	BOOL DeleteCLSIDFromDeque(CLSID clsid);

//	virtual void Serialize(CArchive& ar);

};

typedef deque<COleCfgInfo*> OLECFG_DEQUE;
#endif // !defined(AFX_OLECFGINFO_H__6BC3A051_630C_4EA9_9E12_5BEC237CFDBD__INCLUDED_)
