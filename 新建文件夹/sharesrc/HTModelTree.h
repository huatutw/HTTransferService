#pragma once
#include "SocketBase.h"
#include "AString.h"
#include "DMErrorCode.h"
#include "DMStruct.h"

// CHTModelTree

class CHTModelTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CHTModelTree)

public:
	CHTModelTree();
	virtual ~CHTModelTree();

	CImageList m_ImageList;
	HTREEITEM m_hBaseRoot;
	DWORD GetDMView();
	DWORD GetDMView1();
	DWORD GetGroupChild(int nGroupID, HTREEITEM hParentRoot, BOOL bExpand = FALSE, BOOL bCheck = FALSE);
	DWORD GetGroupChild1(CAString strGroupID, HTREEITEM hParentRoot, BOOL bExpand = FALSE, BOOL bCheck = FALSE);
	DWORD InitHTModel();
	DWORD InitHTModel1();
	BOOL  IsTempDepart(HTREEITEM hDepart);
	void  SetChildrenCheckBox(HTREEITEM hItem, BOOL bCheckSetValue);
	void  SetParentCheckBox(HTREEITEM hItem, BOOL bCheckSetValue);

	CString	m_strIP;
	DWORD	m_dwServerPort;

protected:

public:
	/*afx_msg void OnNMClickTreeModel(NMHDR *pNMHDR, LRESULT *pResult);*/
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemexpandedTreeUsermodal(NMHDR* pNMHDR, LRESULT* pResult); 
};


