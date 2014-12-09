// TreeNode.h: interface for the CTreeNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREENODE_H__74A21B70_340C_453D_A386_26B6D1A354AE__INCLUDED_)
#define AFX_TREENODE_H__74A21B70_340C_453D_A386_26B6D1A354AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum{
	SOFT_TYPE = 1,
	SOFT_VERSION = 2,
	ITEM_APP = 3,
	ITEM_EXT = 4,
	ITEM_OLE = 5,
};

class CTreeNode  
{
public:
	CTreeNode();
	virtual ~CTreeNode();
public:
	//virtual void Serialize(CArchive& ar)=0;
	int m_nType;
	int	m_clsVersion;
};

#endif // !defined(AFX_TREENODE_H__74A21B70_340C_453D_A386_26B6D1A354AE__INCLUDED_)
