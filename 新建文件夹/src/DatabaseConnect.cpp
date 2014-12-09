// DatabaseConnect.cpp: implementation of the CDatabaseConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdio.h"
#include "DatabaseConnect.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef unsigned int	U32;
typedef unsigned char	U8;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef DATABASE_SUPPORT
_ConnectionPtr CDatabaseConnect::m_pConnection = NULL;
_RecordsetPtr  CDatabaseConnect::m_pRecordset = NULL;
#endif

CDatabaseConnect::CDatabaseConnect()
{

}

CDatabaseConnect::~CDatabaseConnect()
{

}

extern void FromBase64( const unsigned char *base64code, int base64length, char *out );

#ifdef DATABASE_SUPPORT
BOOL CDatabaseConnect::InitDatabaseConnect()
{
	USES_CONVERSION;
	TCHAR sysPath[MAX_PATH] = {0};
	GetSystemDirectory(sysPath, MAX_PATH);
	
	TCHAR tcDGServerIniFile[MAX_PATH] = {0};
	_stprintf(tcDGServerIniFile, _T("%s\\DGServer.ini"), sysPath);
	
	TCHAR tcIP[32] = {0};
	GetPrivateProfileString(_T("DGDBServer"),_T("IP"),_T("."),tcIP,32,tcDGServerIniFile);

	TCHAR tcDBName[200] = {0};
	GetPrivateProfileString( _T("DGDBServer"), _T("DBName"), _T("DocGuarder"), tcDBName, 200, tcDGServerIniFile);
	
	TCHAR tcUserName[40] = {0};
	GetPrivateProfileString(_T("DGDBServer"),_T("UserName"),_T(""),tcUserName,40,tcDGServerIniFile);
	
	TCHAR tcPassword[40] = {0};
	GetPrivateProfileString(_T("DGDBServer"),_T("Password"),_T(""),tcPassword,40,tcDGServerIniFile);
	
	char out[40] = {0};
	FromBase64((const unsigned char*)T2A(tcPassword),strlen(T2A(tcPassword)),out);
	CAString strPassword = out;

	m_pRecordset.CreateInstance("ADODB.Recordset");
	
	HRESULT hr;
	m_pRecordset->PutCursorLocation(adUseClient);  

	hr = m_pConnection.CreateInstance("ADODB.Connection");///创建Connection对象
	if(SUCCEEDED(hr))
	{
		//TCHAR strLink[MAX_PATH + 100] = {0};
		//_stprintf(strLink,_T("Driver=SQL Server;Server=%s;Database=DocGuarder;UID=%s;PWD=%s"), tcIP, tcUserName, tcPassword);
		//hr = m_pConnection->Open(_bstr_t(strLink),"","",adModeUnknown);///连接数据库
		
		TCHAR strLink[MAX_PATH + 100] = {0};
		_stprintf(strLink,_T("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s"), strPassword, tcUserName, tcDBName, tcIP);
		hr = m_pConnection->Open(_bstr_t(strLink),"","",adModeUnknown);///连接数据库
	}

	return TRUE;
}

//连接备用数据库
BOOL CDatabaseConnect::InitBackupDatabaseConnect()
{
	USES_CONVERSION;
	TCHAR sysPath[MAX_PATH] = {0};
	GetSystemDirectory(sysPath, MAX_PATH);
	
	TCHAR tcDGServerIniFile[MAX_PATH] = {0};
	_stprintf(tcDGServerIniFile, _T("%s\\DGServer.ini"), sysPath);

	TCHAR tcBackupIP[32] = {0};
	GetPrivateProfileString(_T("DGDBServer0"),_T("IP"),_T("."),tcBackupIP,32,tcDGServerIniFile);
	
	TCHAR tcBackupUserName[40] = {0};
	GetPrivateProfileString(_T("DGDBServer0"),_T("UserName"),_T(""),tcBackupUserName,40,tcDGServerIniFile);
	
	TCHAR tcBackupPassword[40] = {0};
	GetPrivateProfileString(_T("DGDBServer0"),_T("Password"),_T(""),tcBackupPassword,40,tcDGServerIniFile);
	
	char backupout[40] = {0};
	FromBase64((const unsigned char*)T2A(tcBackupPassword),strlen(T2A(tcBackupPassword)),backupout);
	CAString strBackupPassword = backupout;

	m_pRecordset.CreateInstance("ADODB.Recordset");
	
	HRESULT hr;
	m_pRecordset->PutCursorLocation(adUseClient);  

	hr = m_pConnection.CreateInstance("ADODB.Connection");///创建Connection对象
	if(SUCCEEDED(hr))
	{		
		TCHAR strBackupLink[MAX_PATH + 100] = {0};
		_stprintf(strBackupLink,_T("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=DocGuarder;Data Source=%s"), strBackupPassword, tcBackupUserName, tcBackupIP);
		hr = m_pConnection->Open(_bstr_t(strBackupLink),"","",adModeUnknown);///连接备用数据库
	}
	
	return TRUE;
}
#endif

