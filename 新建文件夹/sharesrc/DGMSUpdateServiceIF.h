// NewUpdateServiceIF.h: interface for the CNewUpdateServiceIF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUPDATESERVICEIF_H__D9E2DA71_D294_439D_B714_FA5BA23F9837__INCLUDED_)
#define AFX_NEWUPDATESERVICEIF_H__D9E2DA71_D294_439D_B714_FA5BA23F9837__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SocketBase.h"

class CDGMSUpdateServiceIF  
{
public:
	CDGMSUpdateServiceIF();
	virtual ~CDGMSUpdateServiceIF();

	int ReplyCommandUploadClientVersion(CIPCSocket& s, LPCTSTR szHost);
	int ReplyCommandUploadClientInfo(CIPCSocket& s, LPCTSTR szHost);
	int ReplyCommandGetModuleFile(CIPCSocket& s);
	int ReplyCommandUpdateByModuleFile(CIPCSocket& s);

};

#endif // !defined(AFX_NEWUPDATESERVICEIF_H__D9E2DA71_D294_439D_B714_FA5BA23F9837__INCLUDED_)
