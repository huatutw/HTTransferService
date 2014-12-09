// DeleteFile.h: interface for the CDeleteFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELETEFILE_H__FE12FA3E_FB2E_44DC_AA1A_3A097F828493__INCLUDED_)
#define AFX_DELETEFILE_H__FE12FA3E_FB2E_44DC_AA1A_3A097F828493__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UpdateBaseClass.h"

enum
{
	DELETE_DIRECT = 1,		//直接删除
	DELETE_MOVE = 2,		//移动至重启后删除
	DELETE_UNREGSERVER = 4, //反注册服务
	DELETE_UNREG = 8,		//反注册
};

class CDeleteFile : public CUpdateBaseClass
{
public:
	CDeleteFile();
	virtual ~CDeleteFile();

	void MakeDestFilePath();

	int m_nDeleteMode;				//删除模式

	LPTSTR m_lpszDestFileName;		//相对路径
	int    m_nPathType;				//路径类型
};

#endif // !defined(AFX_DELETEFILE_H__FE12FA3E_FB2E_44DC_AA1A_3A097F828493__INCLUDED_)
