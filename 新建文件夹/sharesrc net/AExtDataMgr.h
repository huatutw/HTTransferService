// ExtDataMgr.h: interface for the CAExtDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTDATAMGR_H__3113D697_19AB_4F66_8F61_17A3F66E821C__INCLUDED_)
#define AFX_EXTDATAMGR_H__3113D697_19AB_4F66_8F61_17A3F66E821C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
class CAString;

class CAExtDataMgr  
{
public:
	CAExtDataMgr();
	virtual ~CAExtDataMgr();

	map<CAString, CAString>		m_mapUserData;

	void		SetUserData(CAString strName, CAString strValue);
	BOOL		GetUserData(CAString strName, CAString &strValue);
	BOOL		RemoveUserData( CAString strName );
	BOOL		RemoveAllUserData();
};

#endif // !defined(AFX_EXTDATAMGR_H__3113D697_19AB_4F66_8F61_17A3F66E821C__INCLUDED_)
