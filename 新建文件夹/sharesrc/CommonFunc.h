// CommonFunc.h: interface for the CCommonFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONFUNC_H__752254A7_3BCE_4BF8_89AE_79B2A9F6F0CE__INCLUDED_)
#define AFX_COMMONFUNC_H__752254A7_3BCE_4BF8_89AE_79B2A9F6F0CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DG_MAX_PATH 4096
#define DG_MAX_TEMP 1024
#define DG_MAX_NUMBER 64

class CCommonFunc  
{
public:
	CCommonFunc();
	virtual ~CCommonFunc();

	static void GetTempFilePath(LPCTSTR strFileName,LPTSTR strPathName);

	static LPTSTR Int2Str(int num,LPTSTR strNumber);

	static LPTSTR BackupString(LPCTSTR strTemplate);
};

#endif // !defined(AFX_COMMONFUNC_H__752254A7_3BCE_4BF8_89AE_79B2A9F6F0CE__INCLUDED_)
