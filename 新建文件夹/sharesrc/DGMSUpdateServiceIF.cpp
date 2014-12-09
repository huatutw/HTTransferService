// NewUpdateServiceIF.cpp: implementation of the CNewUpdateServiceIF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DGMSUpdateServiceIF.h"
#include "UpdateStruct.h"
#include "UpdateDBClass.h"
#include "WebDataFunIF.h"
#include "Cryp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDGMSUpdateServiceIF::CDGMSUpdateServiceIF()
{

}

CDGMSUpdateServiceIF::~CDGMSUpdateServiceIF()
{

}

CAString GetFileMD5(CAString strFilePathName)
{
	USES_CONVERSION;
	BYTE out[32] = {0};
	CCryp cCrpy;
	cCrpy.FileToMD5(strFilePathName.GetBuffer(1),out);
	strFilePathName.ReleaseBuffer();
	TCHAR tcMD5[128] = {0};
	TCHAR tcTemp[128] = {0};
	for(int nfor = 0; nfor < 16; nfor++)   
	{   
		_stprintf(tcTemp,_T("%02x"),out[nfor]);
		_tcscat(tcMD5,tcTemp);
	}
	
	CAString strMD5 = _T("");
	strMD5 = tcMD5;
	
	return strMD5;
}

int CDGMSUpdateServiceIF::ReplyCommandUploadClientVersion(CIPCSocket& s, LPCTSTR szHost)
{	
	TCHAR tcClientID[LEN_NAME] = {0};

	if(!CSocketBase::RecvStringData(s, tcClientID, LEN_NAME))
	{
		return 0;
	}

	TCHAR buffer[MAX_PATH] = {0};
	GetModuleFileName(NULL,buffer,MAX_PATH);
	CAString strServerPath = buffer;
	int pos = strServerPath.ReverseFind(TEXT('\\'));
	strServerPath = strServerPath.Left(pos+1);
	
	CAString strPackagePath = strServerPath + _T("package\\");
	CAString strModuleListPath = strPackagePath + _T("ModuleList.ini");
	
	CSocketBase::SendFile1(s, strModuleListPath);
	
	int nModuleCount = GetPrivateProfileInt(_T("ModuleCount"), _T("Count"), 0, strModuleListPath);
	
	CUpdateDBClass UpdateDBClass;
	if(ERROR_SUCCESS == UpdateDBClass.ConnectDB())
	{	
		for(int nfor = 0; nfor < nModuleCount; nfor++)
		{
			CAString strSectionName = _T("");
			strSectionName.Format(_T("Module%d"), nfor);
			TCHAR tcModuleMD5[LEN_MD5] = {0};
			GetPrivateProfileString(strSectionName, _T("ModuleMD5"), _T(""), tcModuleMD5, LEN_MD5, strModuleListPath);
			
			DGMS_ModuleInfo ModuleInfo;
			CAString strSQL;
			memset(&ModuleInfo, 0, sizeof(DGMS_ModuleInfo));
			if (!CSocketBase::RecvData(s, &ModuleInfo, sizeof(DGMS_ModuleInfo)))
			{
				return 0;
			}
			
			strSQL.Format(_T("SELECT ClientID FROM DGMS_VERSION WHERE ClientID = '%s' AND ModuleName = '%s'"),
				tcClientID,
				ModuleInfo.strModuleName);
			
			ADODB::_RecordsetPtr ptrRecordset;
			int ret = UpdateDBClass.DBQuery(ptrRecordset,strSQL);
			
			if(0 == _tcscmp(tcModuleMD5, ModuleInfo.strModuleMD5))
			{
				ModuleInfo.bIsMatch = 1;
			}
			else
			{
				ModuleInfo.bIsMatch = 0;
			}
			
			if(ret == ERROR_DGMSDB_NORECORD)
			{
				strSQL.Format(_T("INSERT INTO DGMS_VERSION (ClientID, ModuleName, VersionName, ModuleMD5, ModuleSize, LastModify, IsMatch) VALUES ('%s', '%s', '%s', '%s', %d, '%s', %d)"),
					tcClientID,
					ModuleInfo.strModuleName,
					ModuleInfo.strVersionName,
					ModuleInfo.strModuleMD5,
					ModuleInfo.nModuleSize,
					ModuleInfo.strLastModify,
					ModuleInfo.bIsMatch);
			}
			
			if(ret == ERROR_SUCCESS)
			{
				strSQL.Format(_T("UPDATE DGMS_VERSION SET VersionName = '%s', ModuleMD5 = '%s', ModuleSize = %d, LastModify = '%s', IsMatch = %d WHERE ClientID = '%s' AND ModuleName = '%s'"),
					ModuleInfo.strVersionName,
					ModuleInfo.strModuleMD5,
					ModuleInfo.nModuleSize,
					ModuleInfo.strLastModify,
					ModuleInfo.bIsMatch,
					tcClientID,
					ModuleInfo.strModuleName);
			}
			
			UpdateDBClass.ExecuteSQL(strSQL);
		}
	}

	return 1;
}

int CDGMSUpdateServiceIF::ReplyCommandUploadClientInfo(CIPCSocket& s, LPCTSTR szHost)
{
	TCHAR tcClientID[LEN_NAME] = {0};
	TCHAR tcComputerName[LEN_NAME] = {0};
	
	ADODB::_ConnectionPtr ptrConnection = NULL;
	CAString strLoginName = _T("");
	CAString strUserName = _T("");
	
	CUpdateDBClass UpdateDBClass;
	CAString strSQL = _T("");
	BOOL bIsMatch = FALSE;
	int nStatus = 1;
	int nRet = UpdateDBClass.ConnectDB();
	
	if(!CSocketBase::RecvStringData(s, tcClientID, LEN_NAME))
	{
		return 0;
	}
	
	if(!CSocketBase::RecvStringData(s, tcComputerName, LEN_NAME))
	{
		return 0;
	}

	CWebDataFunIF::GetLoginInfoByClientID(ptrConnection, tcClientID, strLoginName, strUserName);

	if(ERROR_SUCCESS == nRet)
	{
		ADODB::_RecordsetPtr ptrRecordset1;
		strSQL.Format(_T("SELECT CN_COMPUTER FROM DGMS_OLDCLIENT WHERE CN_COMPUTER = '%s'"),
			           tcComputerName);
		int ret = UpdateDBClass.DBQuery(ptrRecordset1,strSQL);

		if (ERROR_SUCCESS == ret)
		{
			strSQL.Format(_T("DELETE FROM DGMS_OLDCLIENT WHERE CN_COMPUTER = '%s'"),
				          tcComputerName);
			UpdateDBClass.ExecuteSQL(strSQL);
		}


		strSQL.Format(_T("SELECT ClientID FROM DGMS_VERSION WHERE ClientID = '%s' AND ModuleName <> '' AND IsMatch = 0"),
			tcClientID);
		
	    ret = UpdateDBClass.DBQuery(ptrRecordset1,strSQL);
		
		if (ret == ERROR_DGMSDB_NORECORD)
		{
			bIsMatch = TRUE;
		}
		else
		{
			bIsMatch = FALSE;
		}
		
		strSQL.Format(_T("SELECT CN_CLIENTID FROM DGMS_CLIENT WHERE CN_CLIENTID = '%s'"),
			tcClientID);
		
		ADODB::_RecordsetPtr ptrRecordset;
		ret = UpdateDBClass.DBQuery(ptrRecordset,strSQL);
		
		if(ret == ERROR_DGMSDB_NORECORD)
		{
			strSQL.Format(_T("INSERT INTO DGMS_CLIENT (CN_LOGINNAME, CN_USERNAME, CN_RECENTUPLOADTIME, CN_CLIENTID, CN_COMPUTER, CN_IP, IsMatch, Status) VALUES ('%s', '%s', Now(), '%s', '%s', '%s', %d, %d)"),
				strLoginName,
				strUserName,
				tcClientID,
				tcComputerName,
				szHost,
				bIsMatch,
				nStatus);
		}
		
		if(ret == ERROR_SUCCESS)
		{
			strSQL.Format(_T("UPDATE DGMS_CLIENT SET CN_LOGINNAME = '%s', CN_USERNAME = '%s', CN_RECENTUPLOADTIME = Now(), CN_COMPUTER = '%s', CN_IP = '%s', IsMatch = %d , Status = %d WHERE CN_CLIENTID = '%s'"),
				strLoginName,
				strUserName,
				tcComputerName,
				szHost,
				bIsMatch,
				nStatus,
				tcClientID);
		}
		
		nRet = UpdateDBClass.ExecuteSQL(strSQL);
	}
	
	CSocketBase::SendData(s, &nRet, sizeof(int));

	return 1;
}

int CDGMSUpdateServiceIF::ReplyCommandGetModuleFile(CIPCSocket& s)
{
	TCHAR tcMD5[BUFF_SIZE] = {0};
	
	if (!CSocketBase::RecvStringData(s, tcMD5, BUFF_SIZE))
	{
		return false;
	}
	
	CAString strServerModuleFilePath = _T("");
	BOOL bIsNeedUpdate = TRUE;
	
	TCHAR buffer[MAX_PATH] = {0};
	GetModuleFileName(NULL,buffer,MAX_PATH);
	CAString strServerPath = buffer; 
	int pos = strServerPath.ReverseFind(TEXT('\\'));
	strServerPath = strServerPath.Left(pos+1);
	
	strServerModuleFilePath = strServerPath + _T("UpdateModule.ini");
	
	if(0 == _waccess(strServerModuleFilePath, 0))
	{
		CAString strMD5 = GetFileMD5(strServerModuleFilePath);
		
		if(0 == strMD5.CompareNoCase(tcMD5))
		{
			bIsNeedUpdate = FALSE;
		}
		else
		{
			bIsNeedUpdate = TRUE;
		}
	}
	else
	{
		bIsNeedUpdate = FALSE;
	}
	
	CSocketBase::SendData(s, &bIsNeedUpdate, sizeof(BOOL));
	
	if(bIsNeedUpdate)
	{
		CSocketBase::SendFile1(s, strServerModuleFilePath);
	}
	
	return 1;
}

int CDGMSUpdateServiceIF::ReplyCommandUpdateByModuleFile(CIPCSocket& s)
{
    TCHAR tcClientModuleFileMD5[LEN_NAME] = {0};
	
	if (!CSocketBase::RecvStringData(s, tcClientModuleFileMD5, LEN_NAME))
	{
		return false;
	}
	
	CAString strServerModuleFilePath = _T("");
	BOOL bIsNeedUpdate = TRUE;
	
	TCHAR buffer[MAX_PATH] = {0};
	GetModuleFileName(NULL,buffer,MAX_PATH);
	CAString strServerPath = buffer;
	int pos = strServerPath.ReverseFind(TEXT('\\'));
	strServerPath = strServerPath.Left(pos+1);
	
	strServerModuleFilePath = strServerPath + _T("UpdateModule.ini");
	
	CAString strServerModuleFileMD5 = GetFileMD5(strServerModuleFilePath);
	
	if(0 == _waccess(strServerModuleFilePath, 0))
	{
		if(0 == strServerModuleFileMD5.CompareNoCase(tcClientModuleFileMD5))
		{
			bIsNeedUpdate = FALSE;
		}
		else
		{
			bIsNeedUpdate = TRUE;
		}
	}
	else
	{
		bIsNeedUpdate = FALSE;
	}
	
	CSocketBase::SendData(s, &bIsNeedUpdate, sizeof(BOOL));
	
	if(bIsNeedUpdate)
	{
		CSocketBase::SendFile1(s, strServerModuleFilePath);
	}
	else
	{
		return TRUE;
	}
	
	int nNeedUpdateModuleCount = 0;
	if (!CSocketBase::RecvData(s, &nNeedUpdateModuleCount, sizeof(int)))
	{
		return 0;
	}
	
	for(int nfor = 0; nfor < nNeedUpdateModuleCount; nfor++)
	{
		TCHAR tcModuleName[LEN_NAME] = {0};
		if (!CSocketBase::RecvStringData(s, tcModuleName, LEN_NAME))
		{
			return FALSE;
		}
		
		CAString strServerModulePathName;
		strServerModulePathName.Format(_T("%spackage\\%s"), strServerPath, tcModuleName);
		
		CSocketBase::SendFile1(s, strServerModulePathName);
	}
	
	return 1;
} 