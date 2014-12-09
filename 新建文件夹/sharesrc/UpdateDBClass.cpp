// UpdateDBClass.cpp: implementation of the CUpdateDBClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpdateDBClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUpdateDBClass::CUpdateDBClass()
{
	m_ptrConnection = NULL;
	m_strConnect = _T("");
}

CUpdateDBClass::~CUpdateDBClass()
{
	if(m_ptrConnection)
	{
		m_ptrConnection->Close();
		m_ptrConnection = NULL;
	}
}

CAString CUpdateDBClass::GetUpdateDBPath()
{
	TCHAR buffer[1024] = {0};
	GetModuleFileName(NULL,buffer,1024);
	
	CAString strModulePath = buffer;
	int pos = strModulePath.ReverseFind('\\');
	strModulePath = strModulePath.Left(pos+1);
	strModulePath += _T("DGMSUpdateDB.mdb");

	return strModulePath;
}

DWORD CUpdateDBClass::ConnectDB()
{
	::CoInitialize(NULL);
	HRESULT hr = m_ptrConnection.CreateInstance("ADODB.Connection");
	m_strConnect.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=True;Data Source=\"%s\";Jet OLEDB:Database Password=%s;"),
		GetUpdateDBPath(),
		_T("12345678901234567890"));
	
	try
	{
		hr = m_ptrConnection->Open(_bstr_t(m_strConnect), "", "", 0);
	}
	catch (_com_error e)
	{
		CAString strError;
		strError.Format(_T("Open access database failed.\r\n%d\r\n%s\r\n%s"), e.WCode(), e.ErrorMessage(), m_strConnect);
		OutputDebugString(strError);
		return ERROR_DGMSDB_CONNECT;
	}

	return ERROR_SUCCESS;
}

int CUpdateDBClass::DBQuery(ADODB::_RecordsetPtr& ptrRecordset,LPCTSTR strQuerySQL)
{
	HRESULT hr = ptrRecordset.CreateInstance("ADODB.Recordset");
	if (FAILED(hr))
	{
		return ERROR_DGMSDB_CREATERECORDSET;
	}
	ptrRecordset->CursorLocation = ADODB::adUseClient;
	OutputDebugString(strQuerySQL);
	hr = ptrRecordset->Open(_variant_t(strQuerySQL),
		_variant_t((IDispatch *)m_ptrConnection, TRUE), 
		ADODB::adOpenStatic,
		ADODB::adLockOptimistic, 
		ADODB::adCmdText);
	
	if (FAILED(hr))
	{
		return ERROR_DGMSDB_QUERY;
	}
	
	if (ptrRecordset->adoEOF)
		return ERROR_DGMSDB_NORECORD;
	
	return ERROR_SUCCESS;
}

int CUpdateDBClass::ExecuteSQL(CAString strSQL)
{
	try
	{
		OutputDebugString(strSQL);
		m_ptrConnection->Execute((_bstr_t)strSQL, 0, 0);
	}
	catch (_com_error e)
	{
		CAString strError;
		strError.Format(_T("Open access database failed.\r\n%s\r\n%s"), e.ErrorMessage(), m_strConnect);
		OutputDebugString(strError);
		return ERROR_DGMSDB_CONNECT;
	}
	
	return ERROR_SUCCESS;
}
