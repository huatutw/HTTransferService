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
	try
	{
		hr = m_pConnection.CreateInstance("ADODB.Connection");///创建Connection对象
		if(SUCCEEDED(hr))
		{
			// 			TCHAR strLink[MAX_PATH + 100] = {0};
			// 			_stprintf(strLink,_T("Driver=SQL Server;Server=%s;Database=DocGuarder;UID=%s;PWD=%s"), tcIP, tcUserName, tcPassword);
			//  			hr = m_pConnection->Open(_bstr_t(strLink),"","",adModeUnknown);///连接数据库
			
			TCHAR strLink[MAX_PATH + 100] = {0};
			_stprintf(strLink,_T("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=DocGuarder;Data Source=%s"), strPassword, tcUserName, tcIP);
			OutputDebugString(strLink);
			hr = m_pConnection->Open(_bstr_t(strLink),"","",adModeUnknown);///连接数据库
		}
	}
	catch(_com_error e)///捕捉异常
	{ 
		return FALSE;
	}
	return TRUE;
}
#endif