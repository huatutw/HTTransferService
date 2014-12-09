// HTModelTree.cpp : implementation file
//

#include "stdafx.h"
#include "HTModelTree.h"
#ifndef BUFF_SIZE
#define BUFF_SIZE 1024
#endif



// CHTModelTree

IMPLEMENT_DYNAMIC(CHTModelTree, CTreeCtrl)

CHTModelTree::CHTModelTree()
{

}

CHTModelTree::~CHTModelTree()
{

}



BEGIN_MESSAGE_MAP(CHTModelTree, CTreeCtrl)
	/*ON_NOTIFY(NM_CLICK, IDC_TREE_HTMODEL, &CHTModelTree::OnNMClickTreeModel)*/
	ON_NOTIFY_REFLECT(NM_CLICK, CHTModelTree::OnNMClick)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, CHTModelTree::OnItemexpandedTreeUsermodal)
END_MESSAGE_MAP()

DWORD CHTModelTree::InitHTModel()
{
	int nRet = GetDMView();
	if(ERROR_SUCCESS == nRet)
	{
		int nBaseRootID = this->GetItemData(m_hBaseRoot);
		nRet = GetGroupChild(nBaseRootID, m_hBaseRoot, TRUE);
	}

	return nRet;
}

DWORD CHTModelTree::InitHTModel1()
{
	int nRet = GetDMView1();
	if(ERROR_SUCCESS == nRet)
	{
		CAString pstrID;
		pstrID = *((CAString*)this->GetItemData(m_hBaseRoot));

		nRet = GetGroupChild1(pstrID, m_hBaseRoot, TRUE);
	}

	return nRet;
}

extern DWORD PostErrorMessageToTray(CString strTitle, CString strInfo, int nType);

DWORD CHTModelTree::GetDMView()
{
	USES_CONVERSION;
	SOCKET sComm = SPCreateConnectSocket(T2A(m_strIP), m_dwServerPort);
	if (sComm == INVALID_SOCKET)
	{
		return ERROR_DMSERVICE_CONNECT;
	}

	CIPCSocket s(sComm,TRUE);

	CSocketBase::SendStringData(s,_T("GetDMView"));

	TCHAR strReply[BUFF_SIZE] = {0};
	CSocketBase::RecvStringData(s,strReply,BUFF_SIZE,NULL);

	if (_tcscmp(strReply,_T("Continue")) != 0)
		return ERROR_DMSERVICE_CONNECT;

	int nGroupCount = 0;
	if(!CSocketBase::RecvData(s, &nGroupCount, sizeof(int)))
	{
		return ERROR_SOCKET_RECVDATA;
	}

	for(int nfor = 0; nfor < nGroupCount; nfor++)
	{
		DM_DepartmentAndUser DMGroupAndUser;
		CSocketBase::RecvData(s, &DMGroupAndUser, sizeof(DM_DepartmentAndUser));

		CString strItemText = _T("");
		if(DAU_USER == DMGroupAndUser.nType)
		{	
			strItemText.Format(_T("%s[%s]"), DMGroupAndUser.strUserName, DMGroupAndUser.strName);
		}
		else
		{
			strItemText = DMGroupAndUser.strName;
		}

		m_hBaseRoot = this->InsertItem(strItemText, NULL);
		this->SetItemImage(m_hBaseRoot, DMGroupAndUser.nType, DMGroupAndUser.nType);
		this->SetItemData(m_hBaseRoot, DMGroupAndUser.nId);
	}

	return ERROR_SUCCESS;
}

DWORD CHTModelTree::GetDMView1()
{
	USES_CONVERSION;
	SOCKET sComm = SPCreateConnectSocket(T2A(m_strIP), m_dwServerPort);
	if (sComm == INVALID_SOCKET)
	{
		return ERROR_DMSERVICE_CONNECT;
	}

	CIPCSocket s(sComm,TRUE);

	CSocketBase::SendStringData(s,_T("GetDMView1"));

	TCHAR strReply[BUFF_SIZE] = {0};
	CSocketBase::RecvStringData(s,strReply,BUFF_SIZE,NULL);

	if (_tcscmp(strReply,_T("Continue")) != 0)
		return ERROR_DMSERVICE_CONNECT;

	int nGroupCount = 0;
	if(!CSocketBase::RecvData(s, &nGroupCount, sizeof(int)))
	{
		return ERROR_SOCKET_RECVDATA;
	}

	for(int nfor = 0; nfor < nGroupCount; nfor++)
	{
		DM_DepartmentAndUser1 DMGroupAndUser;
		CSocketBase::RecvData(s, &DMGroupAndUser, sizeof(DM_DepartmentAndUser1));

		CString strItemText = _T("");
		if(DAU_USER == DMGroupAndUser.nType)
		{	
			strItemText.Format(_T("%s[%s]"), DMGroupAndUser.strUserName, DMGroupAndUser.strName);
		}
		else
		{
			strItemText = DMGroupAndUser.strName;
		}

		m_hBaseRoot = this->InsertItem(strItemText, NULL);
		this->SetItemImage(m_hBaseRoot, DMGroupAndUser.nType, DMGroupAndUser.nType);

		CAString *pstrID = new CAString;
		*pstrID = DMGroupAndUser.strID;

		this->SetItemData(m_hBaseRoot, (DWORD)(pstrID));
	}

	return ERROR_SUCCESS;
}

DWORD CHTModelTree::GetGroupChild(int nGroupID, HTREEITEM hParentRoot, BOOL bExpand, BOOL bCheck)
{
	USES_CONVERSION;
	SOCKET sComm = SPCreateConnectSocket(T2A(m_strIP), m_dwServerPort);
	if (sComm == INVALID_SOCKET)
	{
		return ERROR_DMSERVICE_CONNECT;
	}

	CIPCSocket s(sComm,TRUE);

	CSocketBase::SendStringData(s,_T("GetGroupChild"));

	TCHAR strReply[BUFF_SIZE] = {0};
	CSocketBase::RecvStringData(s,strReply,BUFF_SIZE,NULL);

	if (_tcscmp(strReply,_T("Continue")) != 0)
		return ERROR_DMSERVICE_CONNECT;

	CSocketBase::SendData(s, &nGroupID, sizeof(int));

	int nCount = 0;
	if(!CSocketBase::RecvData(s, &nCount, sizeof(int)))
	{
		return ERROR_SOCKET_RECVDATA;
	}

	for(int nfor = 0; nfor < nCount; nfor++)
	{
		DM_DepartmentAndUser DMGroupAndUser;
		CSocketBase::RecvData(s, &DMGroupAndUser, sizeof(DM_DepartmentAndUser));

		CString strItemText = _T("");
		if(DAU_USER == DMGroupAndUser.nType)
		{	
			strItemText.Format(_T("%s[%s]"), DMGroupAndUser.strUserName, DMGroupAndUser.strName);
		}
		else
		{
			strItemText = DMGroupAndUser.strName;
		}

		HTREEITEM hRoot = this->InsertItem(strItemText, hParentRoot);
		this->SetItemImage(hRoot, DMGroupAndUser.nType, DMGroupAndUser.nType);
		this->SetItemData(hRoot, DMGroupAndUser.nId);

		if(DMGroupAndUser.bHaveChildren)
		{
			this->SetItemData(this->InsertItem(_T("temp"), hRoot), -10000);		//临时ID设为-10000
		}

		this->SetCheck(hRoot, bCheck);
	}

	if(bExpand)
	{
		this->Expand(hParentRoot, TVE_EXPAND);
	}

	return ERROR_SUCCESS;
}

DWORD CHTModelTree::GetGroupChild1(CAString strGroupID, HTREEITEM hParentRoot, BOOL bExpand, BOOL bCheck)
{
	USES_CONVERSION;
	SOCKET sComm = SPCreateConnectSocket(T2A(m_strIP), m_dwServerPort);
	if (sComm == INVALID_SOCKET)
	{
		return ERROR_DMSERVICE_CONNECT;
	}

	CIPCSocket s(sComm,TRUE);

	CSocketBase::SendStringData(s,_T("GetGroupChild1"));

	TCHAR strReply[BUFF_SIZE] = {0};
	CSocketBase::RecvStringData(s,strReply,BUFF_SIZE,NULL);

	if (_tcscmp(strReply,_T("Continue")) != 0)
		return ERROR_DMSERVICE_CONNECT;

	CSocketBase::SendStringData(s, strGroupID);

	int nCount = 0;
	if(!CSocketBase::RecvData(s, &nCount, sizeof(int)))
	{
		return ERROR_SOCKET_RECVDATA;
	}

	for(int nfor = 0; nfor < nCount; nfor++)
	{
		DM_DepartmentAndUser1 DMGroupAndUser;
		CSocketBase::RecvData(s, &DMGroupAndUser, sizeof(DM_DepartmentAndUser1));

		CString strItemText = _T("");
		if(DAU_USER == DMGroupAndUser.nType)
		{	
			strItemText.Format(_T("%s[%s]"), DMGroupAndUser.strUserName, DMGroupAndUser.strName);
		}
		else
		{
			strItemText = DMGroupAndUser.strName;
		}

		HTREEITEM hRoot = this->InsertItem(strItemText, hParentRoot);
		this->SetItemImage(hRoot, DMGroupAndUser.nType, DMGroupAndUser.nType);

		CAString *pstrID = new CAString;
		*pstrID = DMGroupAndUser.strID;

		this->SetItemData(hRoot, (DWORD)(pstrID));

		if(DMGroupAndUser.bHaveChildren)
		{
			this->SetItemData(this->InsertItem(_T("temp"), hRoot), -10000);		//临时ID设为-10000
		}

		this->SetCheck(hRoot, bCheck);
	}

	if(bExpand)
	{
		this->Expand(hParentRoot, TVE_EXPAND);
	}

	return ERROR_SUCCESS;
}

void CHTModelTree::SetChildrenCheckBox(HTREEITEM hItem, BOOL bCheckSetValue)
{
	HTREEITEM cur = this->GetChildItem(hItem);

	while(cur != NULL)
	{
		this->SetCheck(cur, bCheckSetValue);

		SetChildrenCheckBox(cur, bCheckSetValue);
		cur = this->GetNextSiblingItem(cur);
	}  
	
}

void CHTModelTree::SetParentCheckBox(HTREEITEM hItem, BOOL bCheckSetValue)
{
	HTREEITEM cur = this->GetParentItem(hItem);

	if(NULL != cur)
	{
		if(0 != this->GetItemState(cur, TVIS_STATEIMAGEMASK) >> 12) 
		{
			this->SetCheck(cur, bCheckSetValue);
		}

		SetParentCheckBox(cur, bCheckSetValue);
	}	
}

BOOL CHTModelTree::IsTempDepart(HTREEITEM hDepart)
{
	HTREEITEM cur = this->GetChildItem(hDepart);

	if(NULL != cur)
	{
		int nId = this->GetItemData(cur);

		if(-10000 == nId)
		{
			this->DeleteItem(cur);
			return TRUE;
		}
	}

	return FALSE;
}
// CHTModelTree message handlers

void CHTModelTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_TREEVIEW *pNMTREEVIEW=(NM_TREEVIEW *)pNMHDR;
	UINT uFlags=0;   
	CPoint point,p;
	TVHITTESTINFO HitTestInfo;
	point = GetMessagePos();
	ScreenToClient(&point);
	HitTestInfo.pt=point;
	HTREEITEM hti=this->HitTest(point, &uFlags);

	if(hti != NULL)
	{
		this->SelectItem(hti);
		this->SetItemState(hti, TVS_TRACKSELECT, TVS_TRACKSELECT);

		if(uFlags & TVHT_ONITEMSTATEICON)
		{
			if(0 == this->GetCheck(hti))
			{
				SetChildrenCheckBox(hti, TRUE);
			}
			else
			{
				SetChildrenCheckBox(hti, FALSE);
				SetParentCheckBox(hti, FALSE);
			}
		}
	}

	*pResult = 0;
}

void CHTModelTree::OnItemexpandedTreeUsermodal(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR; 
	
	if(pNMTreeView->action & TVE_EXPAND) 
	{ 
		HTREEITEM hti = pNMTreeView->itemNew.hItem;

		if(IsTempDepart(hti))
		{
			BOOL bCheck = this->GetCheck(hti);
			
			CAString pstrID;
			pstrID = *((CAString*)this->GetItemData(hti));				
			
			GetGroupChild1(pstrID, hti, TRUE, bCheck);
			*pResult = 1;
			return;
		}
	} 

	*pResult = 0;
}
