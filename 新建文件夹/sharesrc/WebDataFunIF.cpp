// WebDataFunIF.cpp: implementation of the CWebDataFunIF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebDataFunIF.h"
#include "DGCommonFunc.h"
#include "DMErrorCodeIF.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebDataFunIF::CWebDataFunIF()
{

}

CWebDataFunIF::~CWebDataFunIF()
{

}

CAString CWebDataFunIF::m_strServerName	= _T("");
CAString CWebDataFunIF::m_strDatabaseName	= _T("");
CAString CWebDataFunIF::m_strLoginName	= _T("");
CAString CWebDataFunIF::m_strPassword	= _T("");
CAString CWebDataFunIF::m_strConnect	= _T("");

char LMoveBit(int base, int MoveNum) 
{ 
	char result=base; 
	if(MoveNum==0)return 1; 
	if(MoveNum==1)return MoveNum; 
	result=base<<(MoveNum-1); 
	return result; 
}

void FromBase64( const unsigned char *base64code, int base64length, char *out )
{
	char base64_alphabet[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	
	char buf[4]; 
	int i,j; 
	int k; 
	int l=0; 
	U8 temp1[4],temp2; 
	U32 base64a=(base64length/4)-1; 
	U32 base64b=0; 
	int m_padnum = 0;
	for(;base64b<base64a+1;base64b++) 
	{ 
		for(i=0;i<4;i++) 
		{ 
			buf[i]=*(base64code+(base64b*4)+i); 
			for(j=0;j<65;j++) 
			{ 
				if(buf[i]==base64_alphabet[j]) 
				{
					temp1[i]=j; 
					break; 
				} 
			} 
		}
		i--; 
		for(k=1;k<4;k++) 
		{ 
			if(temp1[i-(k-1)]==64){
				m_padnum++;
				continue;}
			temp1[i-(k-1)]=temp1[i-(k-1)]/LMoveBit(2,(k-1)*2);
			temp2=temp1[i-k];
			temp2=temp2&(LMoveBit(2,k*2)-1);
			temp2*=LMoveBit(2,8-(2*k));//move 4 
			temp1[i-(k-1)]=temp1[i-(k-1)]+temp2; 
			out[base64b*3+(3-k)]=temp1[i-(k-1)]; 
		}
	}
}

BOOL CWebDataFunIF::GetFilePath(CAString &strFolderPath)
{
	//CWaitCursor		cursor;
//	TCHAR			FolderPath[MAX_PATH];
	
	TCHAR sysPath[MAX_PATH] = {0};
	GetSystemDirectory(sysPath, MAX_PATH);
	strFolderPath = sysPath;
	if(_T("\\") != strFolderPath.Right(1))
		strFolderPath += _T("\\");

// 	GetModuleFileName(NULL, FolderPath, MAX_PATH);
// 	strFolderPath = FolderPath;
// 	strFolderPath = strFolderPath.Left( strFolderPath.ReverseFind('\\') + 1);
	strFolderPath += _T("DGServer.ini");
	/*
	CFileStatus status;
	if( CFile::GetStatus( strFolderPath, status ) )
	{
#ifdef _DEBUG
		afxDump << _T("Full file name = ") << status.m_szFullName << _T("\n");
#endif
		
		return TRUE;
	}
	else
		return FALSE;
		*/

	USES_CONVERSION;
 	if (_access(W2A(strFolderPath.GetBuffer(1)), 0) == -1)
	{
		strFolderPath.ReleaseBuffer();
		return FALSE;
	}
	else
	{
		strFolderPath.ReleaseBuffer();
		return TRUE;
	}
}

BOOL CWebDataFunIF::GetConnectString()
{
	USES_CONVERSION;
	//CWaitCursor	cursor;
	CAString		strReturnValue	= _T("");
	CAString		strIniFile		= _T("");

	CAString		strLoginName	= _T("");
	CAString		strPassword		= _T("");

/*	CAString		strServerName	= _T("");
	CAString		strDatabaseName	= _T("");

	strServerName		= _T("192.168.1.124");
	strDatabaseName		= _T("DocGuarder");
	strLoginName		= _T("sa");
	strPassword			= _T("huatu713");
*/

	if (!GetFilePath(strIniFile))
	{
		OutputDebugString(_T("DGServer.ini文件没有被发现"));
		return FALSE;
	}
	strReturnValue.Empty();
	GetPrivateProfileString( _T("DGDBServer"), _T("IP"), _T("127.0.0.1"), strReturnValue.GetBuffer(200), 200, strIniFile);
	strReturnValue.ReleaseBuffer();
	m_strServerName = strReturnValue;

	strReturnValue.Empty();
	GetPrivateProfileString( _T("DGDBServer"), _T("DBName"), _T("DocGuarder"), strReturnValue.GetBuffer(200), 200, strIniFile);
	strReturnValue.ReleaseBuffer();
	m_strDatabaseName = strReturnValue;
	if (m_strDatabaseName.IsEmpty())
		m_strDatabaseName = _T("DocGuarder");

	strReturnValue.Empty();
	GetPrivateProfileString( _T("DGDBServer"), _T("UserName"), _T("sa"), strReturnValue.GetBuffer(200), 200, strIniFile);
	strReturnValue.ReleaseBuffer();
	m_strLoginName = strReturnValue;

	TCHAR tcReturnPass[255] = {0};
	GetPrivateProfileString( _T("DGDBServer"), _T("Password"), _T(""), tcReturnPass, 255, strIniFile);

	char out[1024] = {0};
	FromBase64((const unsigned char*)T2A(tcReturnPass),strlen(T2A(tcReturnPass)),out);
	m_strPassword = out;

	if (m_strConnect.IsEmpty())
	{
		m_strConnect.Format(_T(" Provider=SQLOLEDB.1; \
									  Persist Security Info=True; \
									  Data Source=%s; \
									  Initial Catalog=%s; \
									  User Id=%s; \
									  Password=%s; "),
									  m_strServerName,
									  m_strDatabaseName,
									  m_strLoginName,
									  m_strPassword);
	}


	return TRUE;
}

int CWebDataFunIF::DBQuery(ADODB::_ConnectionPtr& ptrConnection,ADODB::_RecordsetPtr& ptrRecordset,LPCTSTR strQuerySQL)
{
   	HRESULT hr = ptrRecordset.CreateInstance("ADODB.Recordset");
	if (FAILED(hr))
	{
		return ERROR_DB_CREATERECORDSET;
	}
	ptrRecordset->CursorLocation = ADODB::adUseClient;
	hr = ptrRecordset->Open(_variant_t(strQuerySQL),
		_variant_t((IDispatch *)ptrConnection, TRUE), 
		ADODB::adOpenStatic,
		ADODB::adLockOptimistic, 
		ADODB::adCmdText);
	
	if (FAILED(hr))
	{
		return ERROR_DB_QUERY;
	}
	
	if (ptrRecordset->adoEOF)
		return ERROR_DB_NORECORD;
	
	return ERROR_SUCCESS;
}

DWORD CWebDataFunIF::GetLoginInfoByClientID(ADODB::_ConnectionPtr& ptrConnection, CAString strClientID, CAString& strLoginName, CAString& strUserName)
{
	int ret = 0;
	HRESULT hr	= S_OK;
	if(NULL == ptrConnection)
	{
		CAString strError = _T("");
		
		hr = ptrConnection.CreateInstance("ADODB.Connection");
		if (FAILED(hr))
		{
			if (!FAILED(::CoInitialize(NULL)))
			{
				OutputDebugString(_T("m_ptrConnection.CreateInstance again"));
				ptrConnection.CreateInstance("ADODB.Connection");
			}
		}
		if (m_strConnect.IsEmpty())
			GetConnectString();
		ptrConnection->put_ConnectionTimeout(30L);
		
		hr = ptrConnection->Open(_bstr_t(m_strConnect), "", "", 0);
		if (FAILED(hr))
		{
			strError.Format(_T("连接数据库出错！ServerName = '%s'，DataBaseName = '%s"));
			m_strServerName, m_strDatabaseName;	
			
			OutputDebugString(strError);
			
			strError.Format(_T("用户名:%s,密码：%s"), m_strLoginName, m_strPassword);
			OutputDebugString(strError);
			
			return ERROR_DB_CONNECT;
		}
		else
		{
			OutputDebugString(_T("数据库连接成功"));
		}
	}
	
 	ADODB::_RecordsetPtr	ptrMonitor;
	CAString strSQL = _T("");
	strSQL.Format(_T("SELECT CN_LOGINNAME FROM TN_MONITOR WHERE CN_CLIENTID = '%s'"),
		strClientID);
	
	ret = CWebDataFunIF::DBQuery(ptrConnection,ptrMonitor,strSQL);
	
	if (ret == ERROR_SUCCESS)
	{
		_variant_t	vFieldValLoginName;
		
		vFieldValLoginName = ptrMonitor->GetCollect(long(0));
		if(vFieldValLoginName.vt != VT_NULL)
		{
			LPTSTR strStr1 = OLE2T(_bstr_t(vFieldValLoginName));
			strLoginName = strStr1;
			
			ADODB::_RecordsetPtr	ptrUser;
			strSQL.Format(_T("SELECT Col_Name FROM hs_user WHERE Col_LoginName = '%s'"),
				strLoginName);
			
			ret = CWebDataFunIF::DBQuery(ptrConnection,ptrUser,strSQL);
			
			if (ret == ERROR_SUCCESS)
			{
				_variant_t	vFieldValUserName;
				
				vFieldValUserName = ptrUser->GetCollect(long(0));
				if(vFieldValUserName.vt != VT_NULL)
				{
					LPTSTR strStr2 = OLE2T(_bstr_t(vFieldValUserName));
					strUserName = strStr2;
				}
				
				ptrUser->Close();
			}
		}
		
		ptrMonitor->Close();
	}
}