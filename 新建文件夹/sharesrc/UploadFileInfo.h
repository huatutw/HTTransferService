// UploadFileInfo.h: interface for the CUploadFileInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPLOADFILEINFO_H__0C3818AE_6FB4_44D9_8A6A_6AB0EAB4E94C__INCLUDED_)
#define AFX_UPLOADFILEINFO_H__0C3818AE_6FB4_44D9_8A6A_6AB0EAB4E94C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "..\SHARESRC\UpdateBaseClass.h"

class CUploadFileInfo : public CUpdateBaseClass  
{
public:
	CUploadFileInfo();
	virtual ~CUploadFileInfo();

	int m_nFileCount;
	TCHAR m_strFileList[1024];
};

#endif // !defined(AFX_UPLOADFILEINFO_H__0C3818AE_6FB4_44D9_8A6A_6AB0EAB4E94C__INCLUDED_)
