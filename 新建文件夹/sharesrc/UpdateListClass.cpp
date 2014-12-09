// UpdateListClass.cpp: implementation of the CUpdateListClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpdateListClass.h"

#include "DownloadFile.h"
#include "DeleteFile.h"
#include "RegOperation.h"
#include "CopyFile.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUpdateListClass::CUpdateListClass()
{

}

CUpdateListClass::~CUpdateListClass()
{
	FreeMemory();
}

void CUpdateListClass::FreeMemory()
{
	int size = m_listUpdate.size();
	for (int i=0;i<size;i++)
	{
		delete m_listUpdate[i];
	}

	m_listUpdate.clear();	
}

//从配置读取更新信息
BOOL CUpdateListClass::ReadUpdateInfoFromFile(LPCTSTR lpszUpdateFile)
{
	FreeMemory();

	int nCount = GetPrivateProfileInt(_T("DocGuarder"),_T("UpdateCount"),0,lpszUpdateFile);

	int i = 0;

	for (i=0;i<nCount;i++)
	{
		TCHAR strKey[KEY_LEN] = {0};
		_stprintf(strKey,_T("Update%d"),i+1);

		int nUpdateType = GetPrivateProfileInt(strKey,_T("UpdateType"),0,lpszUpdateFile);

		switch (nUpdateType)
		{
		case UPDATE_DOWNLOADFILE:
			{
				CDownloadFile* pDownload = new CDownloadFile;
				
				TCHAR strTemp[STR_LEN] = {0};
				GetPrivateProfileString(strKey,_T("SourceFile"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
				CopyString(&pDownload->m_lpszSourceFile,strTemp);
				memset(strTemp,0,STR_LEN);
				
				
				pDownload->m_nPathType = GetPrivateProfileInt(strKey,_T("PathType"),0,lpszUpdateFile);
				
				if (pDownload->m_nPathType == PATH_USER)
				{
					GetPrivateProfileString(strKey,_T("UserPath"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
				}else
				{
					GetPrivateProfileString(strKey,_T("DestSubPath"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
				}
				
				CopyString(&pDownload->m_lpszDestSubPath,strTemp);
				
				memset(strTemp,0,STR_LEN);
				GetPrivateProfileString(strKey,_T("FileMd5"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
				CopyString(&pDownload->m_lpszFileMD5,strTemp);
				
				memset(strTemp,0,STR_LEN);
				
				pDownload->m_bNeedRegister	= GetPrivateProfileInt(strKey,_T("Register"),0,lpszUpdateFile);
				pDownload->m_bNeedRegServer = GetPrivateProfileInt(strKey,_T("RegServer"),0,lpszUpdateFile);
				pDownload->m_bNeedRun		= GetPrivateProfileInt(strKey,_T("Run"),0,lpszUpdateFile);
				pDownload->m_bIsUpdteSuccess = GetPrivateProfileInt(strKey,_T("UpdateSuccess"),0,lpszUpdateFile);
				
				
				if (pDownload->m_bNeedRun)
				{
					GetPrivateProfileString(strKey,_T("DomainName"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
					CopyString(&pDownload->m_lpszDomain,strTemp);
					memset(strTemp,0,STR_LEN);
					
					GetPrivateProfileString(strKey,_T("LoginName"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
					CopyString(&pDownload->m_lpszLogin,strTemp);
					memset(strTemp,0,STR_LEN);
					
					GetPrivateProfileString(strKey,_T("Password"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
					CopyString(&pDownload->m_lpszPass,strTemp);
					memset(strTemp,0,STR_LEN);
					
					GetPrivateProfileString(strKey,_T("Command"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
					CopyString(&pDownload->m_lpszCommand,strTemp);
					memset(strTemp,0,STR_LEN);
					
				}
				
				m_listUpdate.push_back(pDownload);			
			}
			break;
		case UPDATE_DELETEFILE:
			{
				CDeleteFile* pDelete = new CDeleteFile;

				TCHAR strTemp[STR_LEN] = {0};
				GetPrivateProfileString(strKey,_T("FilePath"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
				CopyString(&pDelete->m_lpszDestFileName,strTemp);
				memset(strTemp,0,STR_LEN);

				pDelete->m_nDeleteMode = GetPrivateProfileInt(strKey,_T("DeleteMode"),0,lpszUpdateFile);
				pDelete->m_nPathType = GetPrivateProfileInt(strKey,_T("PathType"),0,lpszUpdateFile);
				pDelete->m_bIsUpdteSuccess = GetPrivateProfileInt(strKey,_T("UpdateSuccess"),0,lpszUpdateFile);
				m_listUpdate.push_back(pDelete);		
			}
			break;
		case UPDATE_REGOP:
			{
				CRegOperation* pRegister = new CRegOperation;
				TCHAR strTemp[STR_LEN] = {0};
				GetPrivateProfileString(strKey,_T("RegKey"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
				CopyString(&pRegister->m_strKey,strTemp);
				memset(strTemp,0,STR_LEN);

				pRegister->m_nRegType = GetPrivateProfileInt(strKey,_T("RegType"),0,lpszUpdateFile);
				pRegister->m_nOPType = GetPrivateProfileInt(strKey,_T("OPType"),0,lpszUpdateFile);
				pRegister->m_bIsUpdteSuccess = GetPrivateProfileInt(strKey,_T("UpdateSuccess"),0,lpszUpdateFile);
				
				if (pRegister->m_nOPType == REG_SETVALUE || pRegister->m_nOPType == REG_ADDVALUE || pRegister->m_nOPType == REG_REPLACEVALUE)
				{
					pRegister->m_nValueType = GetPrivateProfileInt(strKey,_T("ValueType"),0,lpszUpdateFile);
					GetPrivateProfileString(strKey,_T("RegItem"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
					CopyString(&pRegister->m_strItem,strTemp);
					memset(strTemp,0,STR_LEN);
					switch(pRegister->m_nValueType)
					{
					case REG_INTEGER:
						pRegister->m_dwValue = GetPrivateProfileInt(strKey,_T("RegValue"),0,lpszUpdateFile);
						break;
					case REG_STRING:
						GetPrivateProfileString(strKey,_T("RegValue"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
						CopyString(&pRegister->m_strValue,strTemp);
						memset(strTemp,0,STR_LEN);
						break;
					}
				}
				m_listUpdate.push_back(pRegister);
			}
			break;
		case UPDATE_UNINSTALL:
			{
				CUpdateBaseClass* pUpdateBase = new CUpdateBaseClass;
				pUpdateBase->m_nUpdateType = UPDATE_UNINSTALL;
				pUpdateBase->m_bIsUpdteSuccess = 0;
				m_listUpdate.push_back(pUpdateBase);
			}
			break;
		case UPDATE_COPYFILE:
			{
				CCopyFile* pCopyFile = new CCopyFile;
				
				pCopyFile->m_nCopyType = GetPrivateProfileInt(strKey,_T("CopyType"),0,lpszUpdateFile);
				pCopyFile->m_bIsUpdteSuccess = GetPrivateProfileInt(strKey,_T("UpdateSuccess"),0,lpszUpdateFile);

				pCopyFile->m_nExistFileType = GetPrivateProfileInt(strKey,_T("ExistFileType"),0,lpszUpdateFile);
				TCHAR strTemp[STR_LEN] = {0};
				GetPrivateProfileString(strKey,_T("ExistFilePath"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
				pCopyFile->m_strExistFilePath = strTemp;
				memset(strTemp,0,STR_LEN);

				GetPrivateProfileString(strKey,_T("ExistFileName"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
				pCopyFile->m_strExistFileName = strTemp;
				memset(strTemp,0,STR_LEN);

				pCopyFile->m_nTargetFileType = GetPrivateProfileInt(strKey,_T("TargetFileType"),0,lpszUpdateFile);
				GetPrivateProfileString(strKey,_T("TargetFilePath"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
				pCopyFile->m_strTargetFilePath = strTemp;
				memset(strTemp,0,STR_LEN);

				GetPrivateProfileString(strKey,_T("TargetFileName"),_T(""),strTemp,STR_LEN,lpszUpdateFile);
				pCopyFile->m_strTargetFileName = strTemp;
				memset(strTemp,0,STR_LEN);

				m_listUpdate.push_back(pCopyFile);
			}
			break;

		}
	}
	return TRUE;
}