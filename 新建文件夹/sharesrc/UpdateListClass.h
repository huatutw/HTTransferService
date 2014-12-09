// UpdateListClass.h: interface for the CUpdateListClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATELISTCLASS_H__74C93222_8B2D_415B_9EB9_B5781662DD4D__INCLUDED_)
#define AFX_UPDATELISTCLASS_H__74C93222_8B2D_415B_9EB9_B5781662DD4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "UpdateBaseClass.h"


class CUpdateListClass  
{
public:
	CUpdateListClass();
	virtual ~CUpdateListClass();

	UPDATE_LIST m_listUpdate;			//更新列表

	void FreeMemory();

	BOOL ReadUpdateInfoFromFile(LPCTSTR lpszUpdateFile);  //从配置读取更新信息
};

#endif // !defined(AFX_UPDATELISTCLASS_H__74C93222_8B2D_415B_9EB9_B5781662DD4D__INCLUDED_)
