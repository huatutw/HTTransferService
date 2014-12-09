// SoftwareTypeInfo.h: interface for the CSoftwareTypeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOFTWARETYPEINFO_H__C59E3EBA_EE18_4D7C_BCF1_2FD194D279E2__INCLUDED_)
#define AFX_SOFTWARETYPEINFO_H__C59E3EBA_EE18_4D7C_BCF1_2FD194D279E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SoftwareVersionInfo.h"


class CSoftwareTypeInfo : public CTreeNode  
{
public:
	char*		m_lpszSoftTypeName; //软件名称: 如Office,AutoCAD等
	BOOL		m_bSaveThisType;

public:
	BOOL DeleteItem(CSoftwareVersionInfo *pVersionInfo);
	CSoftwareTypeInfo();
	virtual ~CSoftwareTypeInfo();

	CSoftwareVersionInfo* FindSoftVersionByName(const char* lpszSoftVersion);

	SOFTVER_DEQUE m_arrSoftVersion;	 //软件的各个版本

#ifndef NO_MFC_VERSION
	virtual void Serialize(CArchive& ar);
	virtual void Serialize_Unicode(CArchive& ar);
#else
	virtual void Serialize(HANDLE hFile,BOOL bRead);
	virtual void Serialize_Unicode(HANDLE hFile,BOOL bRead);
#endif

	CSoftwareVersionInfo* CheckModule(const char* lpszModuleName);

	BOOL CheckOleCLSID(CLSID clsid);
};

typedef deque<CSoftwareTypeInfo*> SOFTTYPE_DEQUE;

#endif // !defined(AFX_SOFTWARETYPEINFO_H__C59E3EBA_EE18_4D7C_BCF1_2FD194D279E2__INCLUDED_)
