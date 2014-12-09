#pragma once
#include "stdafx.h"
#include "cfgmgr.h"
#include "DGCommonFunc.h"
#include "pugixml.h"

using namespace std;
using namespace pugi;


CCfgMgr::CCfgMgr()
{
	m_CfgData			= L"";	
	m_CfgXmlPath	= GetCfgPathFromReg();	

	m_CfgData			= LoadCfgXml();

}

CCfgMgr::~CCfgMgr(void)
{
}

wstring CCfgMgr::GetCfgPathFromReg()
{
	CRegKey DocKey;
	wstring strKeyPath = L"software\\DocGuarder\\connect";
	if (DocKey.Open(HKEY_LOCAL_MACHINE, strKeyPath.c_str(),KEY_READ|KEY_WRITE|KEY_WOW64_32KEY) != ERROR_SUCCESS)
	{
		OutputDebugString(L"cannot get systempath.\n");
	}
	TCHAR szValue[1024] = { 0 };
	ULONG nLen = 1024;
	DocKey.QueryValue(szValue,_T("SystemPath"), &nLen);
	wstring strSystemPath = szValue;
	DocKey.Close();	

	strKeyPath = L"software\\" + strSystemPath+ L"\\common\\core";
	OutputDebugString(strKeyPath.c_str());

	if (DocKey.Open(HKEY_LOCAL_MACHINE, strKeyPath.c_str(),KEY_READ|KEY_WRITE|KEY_WOW64_32KEY) != ERROR_SUCCESS)
	{
		OutputDebugString(L"cannot get cfgPath.\n");
	}
	nLen = 1024;
	memset(szValue, 0, nLen);
	DocKey.QueryValue(szValue,_T("cfgpath"), &nLen);
	DocKey.Close();

	wstring strPath = szValue;
	OutputDebugString(strPath.c_str());
	
	if (strPath.rfind(L"\\") != 0)
	{
		strPath += L"\\";
	}
	strPath += L"cfg.xml";
	return strPath;
}

void CCfgMgr::DecryptCfgXmlByRC4()
{
	TCHAR strError[1024] = { 0 };
	HANDLE hCFGFile = CreateFile(m_CfgXmlPath.c_str(),GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hCFGFile != NULL && hCFGFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hCFGFile,NULL);
		
		_stprintf(strError, L"FileSize: %d", dwFileSize);
	
		OutputDebugString(strError);

		LPBYTE bt = new BYTE[dwFileSize];
		DWORD read;
		ReadFile(hCFGFile,bt,dwFileSize,&read,NULL);
		CloseHandle(hCFGFile);
		int nVersion;
		memcpy(&nVersion,bt,sizeof(int));
		if (nVersion != 0)
		{
			DGCommonFunc::Encrypt_RC4(bt,dwFileSize,"Huatu_Config_2014_04_29");
			wstring strCfgXmlTempPath = m_CfgXmlPath + L".tmp";
			HANDLE hCfgTempFile = CreateFile(strCfgXmlTempPath.c_str(),GENERIC_WRITE |GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE != hCfgTempFile)
			{
				SetFilePointer(hCfgTempFile,0,0,FILE_BEGIN);
				WriteFile(hCfgTempFile,bt,dwFileSize,&read,NULL);
				CloseHandle(hCfgTempFile);	
			}						
		}		
		delete [] bt;
	}
}

wstring CCfgMgr::LoadCfgXml()
{
	DecryptCfgXmlByRC4();
	xml_document doc;
	wstring strCfgXmlTempPath = m_CfgXmlPath + L".tmp";
	if (!doc.load_file(strCfgXmlTempPath.c_str()))
	{
		return L"";
	}
	xml_string_writer write;
	doc.print(write, L"", format_raw, encoding_wchar);	
	wstring	strCfgData = write.result.c_str();
	DeleteFile(strCfgXmlTempPath.c_str());
	return strCfgData;
}

BOOL CCfgMgr::ReadCfgData(TCHAR *szAttributeName, BOOL &bAble,  BOOL bFlush)
{
	bAble = FALSE;
	if (bFlush)
	{
		m_CfgData = LoadCfgXml();
	}

	if (0 == m_CfgData.length())
	{
		return FALSE;
	}

	xml_document doc;
	int nLen = m_CfgData.length()*2+1;
	if (!doc.load_buffer(m_CfgData.c_str(), nLen))
	{
		return FALSE;
	}
	xml_node au_node = doc.child(L"user_cfg").child(L"au");
	while(au_node)
	{
		wstring strValue = au_node.attribute(L"index_name").value();
		if (szAttributeName == strValue)
		{
			strValue = au_node.attribute(L"able").value();
			if (strValue == L"1")
			{
				bAble = TRUE;
				return TRUE;
			}			
			return TRUE;
		}
		au_node = au_node.next_sibling(L"au");
	}
	return FALSE;
}