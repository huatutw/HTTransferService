// UpdateDBClass.h: interface for the CUpdateDBClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATEDBCLASS_H__9846AF40_B2DC_4828_BD02_0205622D1735__INCLUDED_)
#define AFX_UPDATEDBCLASS_H__9846AF40_B2DC_4828_BD02_0205622D1735__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import <C:\\Program Files\\Common Files\\system\\ole db\\Oledb32.dll> raw_interfaces_only exclude("IDBPromptInitialize", "IDataInitialize") no_auto_exclude
#import <C:\\Program Files\\Common Files\\system\\ado\\Msado15.dll> rename("EOF", "adoEOF") rename("BOF", "adoBOF")
#import <C:\\Program Files\\Common Files\\system\\ado\\msadox.dll>

//#include "DMErrorCode.h"
#include "UpdateStruct.h"

class CUpdateDBClass  
{
public:
	CUpdateDBClass();
	virtual ~CUpdateDBClass();

	CAString GetUpdateDBPath();
	DWORD ConnectDB();
	int DBQuery(ADODB::_RecordsetPtr& ptrRecordset,LPCTSTR strQuerySQL);
	int ExecuteSQL(CAString strSQL);
	
	CAString m_strConnect;

	ADODB::_ConnectionPtr m_ptrConnection;
};

#endif // !defined(AFX_UPDATEDBCLASS_H__9846AF40_B2DC_4828_BD02_0205622D1735__INCLUDED_)
