// DatabaseConnect.h: interface for the CDatabaseConnect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASECONNECT_H__30E4CEE4_0B71_4B93_864E_45D9E078588F__INCLUDED_)
#define AFX_DATABASECONNECT_H__30E4CEE4_0B71_4B93_864E_45D9E078588F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDatabaseConnect  
{
public:
	CDatabaseConnect();
	virtual ~CDatabaseConnect();

#ifdef DATABASE_SUPPORT
public:
	static BOOL InitDatabaseConnect();
	static BOOL InitBackupDatabaseConnect();
public:
	static _ConnectionPtr m_pConnection;
	static _RecordsetPtr m_pRecordset;
#endif
};

#endif // !defined(AFX_DATABASECONNECT_H__30E4CEE4_0B71_4B93_864E_45D9E078588F__INCLUDED_)
