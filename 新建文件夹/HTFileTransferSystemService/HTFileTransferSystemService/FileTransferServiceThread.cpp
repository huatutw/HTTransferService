#include "StdAfx.h"

#include <ShlObj.h>
#include "FileTransferServiceThread.h"
#include "pugiconfig.h"
#include "pugixml.h"
#include "CryptWrapper.h"
#include "ErrorRecorder.h"

using namespace std ;
using namespace pugi ;

CFileTransferServiceThread::CFileTransferServiceThread(void)
{
}

CFileTransferServiceThread::~CFileTransferServiceThread(void)
{
}

int StrToBin(TCHAR* inWord, BYTE* OutBin, int source_len)
{
	int t;
	int t2;
	int count = 0;
	BYTE temBin[2];
	temBin[0] = 1;
	temBin[1] = 1;

	if (source_len < 1)
		return 0;

	for(t = 0 ;t < source_len; t ++)
	{   
		t2 = inWord[t];
		if( t2 > 127 )
		{
			temBin[0] =  t2 >> 8 ;
			temBin[1] =  t2;
			OutBin[count] = temBin[0];
			count += 1;
			OutBin[count] = temBin[1];
			count += 1;
		}
		else
		{
			OutBin[count] = t2;
			count += 1;
		}
	}
	return count;
}

void CFileTransferServiceThread::DealCommond()
{
	::CoInitialize(NULL);
	CIPCSocket s(m_sComm,TRUE);
	TCHAR m_strCommand[BUFF_SIZE] = {0}; 
	TCHAR strReply[BUFF_SIZE] = {0};
	_tcscpy(strReply,_T("Continue"));
	if(!CSocketBase::RecvStringData(s,m_strCommand,BUFF_SIZE,NULL))
	{
		return;
	}
	OutputDebugString(m_strCommand);

	CSocketBase::SendStringData(s,strReply, _tcslen(strReply));

	if (!_tcsicmp(m_strCommand, L"UploadFile"))
	{
		UploadFile(s);
	}

	if (!_tcsicmp(m_strCommand, L"UploadFileFolder"))
	{
		UploadFileFolder(s);
	}

	if (!_tcsicmp(m_strCommand, L"GetDownLoadInformation"))
	{
		GetDownLoadInformation(s);
	}

	if (!_tcsicmp(m_strCommand, L"DownLoadFile"))
	{
		DownLoadFile(s);
	}

	if (!_tcsicmp(m_strCommand, L"TestConnect"))
	{
		TestConnect(s);
	}
} 

void CFileTransferServiceThread::DoWork()
{
    DealCommond() ;
}

BOOL CFileTransferServiceThread::InitSystemInfo(SystemApi32Use& info)
{
	HANDLE hMap = OpenFileMapping(FILE_MAP_READ,FALSE,TEXT("rundll32_systemapi32"));
	memset(&info,0,sizeof(SystemApi32Use));
	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_READ,0,0,sizeof(SystemApi32Use));

		memcpy(&info,lpAddr,sizeof(SystemApi32Use));
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else
	{
		return FALSE;
	}
	return TRUE;
}

DWORD CFileTransferServiceThread::GetFileLevel(CAString strDirectoryPath,DWORD &dwSmallFileLevel)
{
	DWORD dwFileLevel = 0 ;

	CAString strStandard = L"" ;
	strStandard = strDirectoryPath ;
	strDirectoryPath += L"\\*.*" ;
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(strDirectoryPath,&data);
	if (hFind == NULL || hFind == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"Find File fail.") ;
	}

	CAString strFileName = L"" ;
	while(TRUE)
	{
		strFileName = data.cFileName ;
		CAString strDownPath = L"" ;
		strDownPath = strStandard ;
		strDownPath += L"\\" ;
		strDownPath += strFileName ;
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(_tcsicmp(strFileName,TEXT(".")) != 0 && _tcsicmp(strFileName,TEXT("..")) != 0)
			{
				GetFileLevel(strDownPath, dwSmallFileLevel) ;
			}
		}
		else
		{
			CErrorRecorder cr;
			char newkey[129] = {0};
			BYTE btKey[128] = {0} ;
			DWORD dwDogID = 0 ;
			GetDogAndKey(btKey,dwDogID) ;
			memcpy(newkey,btKey,128);
			int nSize = strlen(newkey);
			CCryptWrapper cf(strDownPath,btKey,nSize,dwDogID);

			if(cf.IsCryptedFile(cr))
			{
				CAString strGuid;
				GUID guid;
				int nRet = cf.GetFileAuthorizeGuid(guid,cr);
				nRet = cf.GetFileOutsideGuid(guid,cr);
				cf.GetFileLevel(dwFileLevel, cr);
			}
            if( dwSmallFileLevel > dwFileLevel )
			{
                dwSmallFileLevel = dwFileLevel ;
			}
		}
		if (!FindNextFile(hFind,&data))
			break;
	}
	FindClose(hFind);
	return dwSmallFileLevel ;
}

void CFileTransferServiceThread::MakeDownLoadListInformationXML(CAString strDirectoryPath,xml_node &node_root ) 
{
	CAString strStandard = L"" ;
	strStandard = strDirectoryPath ;

	strDirectoryPath += L"\\*.*" ;
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(strDirectoryPath,&data);
	if (hFind == NULL || hFind == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"Find File fail.") ;
	}

	int i = 1 ;
	CAString strFileName = L"" ;
	while(TRUE)
	{
		strFileName = data.cFileName ;
		CAString strDownPath = L"" ;
		strDownPath = strStandard ;
		strDownPath += L"\\" ;
		strDownPath += strFileName ;
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(_tcsicmp(strFileName,TEXT(".")) != 0 && _tcsicmp(strFileName,TEXT("..")) != 0)
			{
				xml_node node_xml = node_root.append_child(L"fl") ;
				node_xml.append_attribute( L"Lev" ).set_value( L"" ) ;
				node_xml.append_attribute( L"Type" ).set_value( L"fl" ) ;
				node_xml.append_attribute( L"Name" ).set_value( strFileName ) ;
				//node_xml.append_attribute( L"Num" ).set_value( i ) ;
				i ++ ;
				MakeDownLoadListInformationXML( strDownPath, node_xml ) ;
			}
		}
		else
		{
			DWORD dwFileLevel = 10;
			BYTE btKey[128] = {0} ;
			char newkey[129] = {0};
			DWORD dwDogID = 0 ;
			GetDogAndKey(btKey,dwDogID) ;
			memcpy(newkey,btKey,128);
			int nSize = strlen(newkey);
			CErrorRecorder cr;
			CCryptWrapper cf(strDownPath,btKey,nSize,dwDogID);

			if(cf.IsCryptedFile(cr))
			{
				CAString strGuid;
				GUID guid;
				int nRet = cf.GetFileAuthorizeGuid(guid,cr);
				nRet = cf.GetFileOutsideGuid(guid,cr);
				cf.GetFileLevel(dwFileLevel, cr);
			}
			CAString strFileLevel = L"" ;
			strFileLevel.Format(L"%d", dwFileLevel) ;

			xml_node node_xml = node_root.append_child(L"fl") ;
			node_xml.append_attribute( L"Lev" ).set_value( strFileLevel ) ;
			node_xml.append_attribute( L"Type" ).set_value( L"fi" ) ;
			node_xml.append_attribute( L"Name" ).set_value( strFileName ) ;
			//node_xml.append_attribute( L"Num" ).set_value( i ) ;
			i ++ ;
		}
		if (!FindNextFile(hFind,&data))
			break;
	}
	FindClose(hFind);
}

BOOL CFileTransferServiceThread::MakeDownLoadFileInformationXML(CAString &strDownLoadFileXML,CAString strDirectoryPath)
{
	CAString strStandard = L"" ;
	strStandard = strDirectoryPath ;

	strDirectoryPath += L"\\*.*" ;
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(strDirectoryPath,&data);
	if (hFind == NULL || hFind == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"Find File fail.") ;
	}

	xml_document dcXML ;
	xml_node node_root ;
	xml_node node_xml ;
	node_root = dcXML.append_child(L"DownLoadFileSList") ;

	int i = 1 ;
	CAString strFileName = L"" ;
	while(TRUE)
	{
		strFileName = data.cFileName ;
		CAString strDownPath = L"" ;
		strDownPath = strStandard ;
		strDownPath += L"\\" ;
		strDownPath += strFileName ;
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(_tcsicmp(strFileName,TEXT(".")) != 0 && _tcsicmp(strFileName,TEXT("..")) != 0)
			{
				DWORD dwFileLevel = 0 ;
				DWORD dwSmallFileLevel = 3 ;
				dwFileLevel = GetFileLevel(strDownPath,dwSmallFileLevel) ;
				CAString strFileLevel = L"" ;
				strFileLevel.Format(L"%d", dwFileLevel) ;

				node_xml = node_root.append_child(L"df") ;
				node_xml.append_attribute( L"Flag" ).set_value( 0 ) ;
				node_xml.append_attribute( L"文件等级" ).set_value( strFileLevel ) ;
				node_xml.append_attribute( L"类型" ).set_value( L"文件夹" ) ;
				node_xml.append_attribute( L"文件名" ).set_value( strFileName ) ;
				node_xml.append_attribute( L"编号" ).set_value( i ) ;
				i ++ ;
			}
		}
		else
		{
			DWORD dwFileLevel = 10;
			BYTE btKey[128] = {0} ;
			char newkey[129] = {0};
			DWORD dwDogID = 0 ;
			GetDogAndKey(btKey,dwDogID) ;
			memcpy(newkey,btKey,128);
			int nSize = strlen(newkey);
			CErrorRecorder cr;
			CCryptWrapper cf(strDownPath,btKey,nSize,dwDogID);

			if(cf.IsCryptedFile(cr))
			{
				CAString strGuid;
				GUID guid;
				int nRet = cf.GetFileAuthorizeGuid(guid,cr);
				nRet = cf.GetFileOutsideGuid(guid,cr);
				cf.GetFileLevel(dwFileLevel, cr);
			}
			CAString strFileLevel = L"" ;
			strFileLevel.Format(L"%d", dwFileLevel) ;

			node_xml = node_root.append_child(L"df") ;
			node_xml.append_attribute( L"Flag" ).set_value( 0 ) ;
			node_xml.append_attribute( L"文件等级" ).set_value( strFileLevel ) ;
			node_xml.append_attribute( L"类型" ).set_value( L"文件" ) ;
			node_xml.append_attribute( L"文件名" ).set_value( strFileName ) ;
			node_xml.append_attribute( L"编号" ).set_value( i ) ;
            i ++ ;
		}
		if (!FindNextFile(hFind,&data))
			break;
	}
	FindClose(hFind);

	xml_string_writer wr ;
	dcXML.print(wr, L"", format_default, encoding_wchar) ;
	strDownLoadFileXML = wr.result.c_str() ;

	return TRUE ;
}

BOOL  CFileTransferServiceThread::CreateHTDirectory(CAString strPath)
{
	CAString strTemp = strPath.Left(strPath.Find('\\'));
	CAString strLast = strPath.Right(strPath.GetLength() - strPath.Find('\\') - 1);
	while(strLast.Find('\\') != -1)
	{
		strTemp += L"\\";
		strTemp += strLast.Left(strLast.Find('\\'));
		if (!PathFileExists(strTemp))
		{
			CreateDirectory(strTemp, NULL);
		}	
		strLast = strLast.Right(strLast.GetLength() - strLast.Find('\\') - 1);
	}

	return TRUE;
}

BOOL CFileTransferServiceThread::GetDogAndKey(BYTE  *btKey, DWORD &dwDogID )
{
	TCHAR tcProgramFile[MAX_PATH]={0} ;
	SHGetSpecialFolderPath(NULL,tcProgramFile,CSIDL_PROGRAM_FILES,0) ;
	PathAppend(tcProgramFile, L"\\Key.ini") ;
	CAString strIni = tcProgramFile ;

	TCHAR szKey[ 128 ] ;
	GetPrivateProfileString( L"Information" , L"Key" , NULL , szKey , 128 , strIni ) ;
	OutputDebugString(szKey) ;
	StrToBin(szKey,btKey,_tcslen(szKey)) ;
	dwDogID = GetPrivateProfileInt( L"Information" , L"ID" , 0 , strIni ) ;

	return TRUE ;
}

BOOL CFileTransferServiceThread::UploadFile(CIPCSocket& s)
{
	CAString strFilePath = L"" ;
	CAString strFileDirectory = L"" ;
	TCHAR tcProgramFile[MAX_PATH]={0} ;
	TCHAR szFileName[BUFF_SIZE] = {0} ;

	CSocketBase::RecvStringData(s,szFileName,BUFF_SIZE,NULL) ;
	
	SHGetSpecialFolderPath(NULL,tcProgramFile,CSIDL_PROGRAM_FILES,0) ;
	PathAppend(tcProgramFile, L"\\HuaTu Files\\") ;
    strFileDirectory = tcProgramFile ;
	strFilePath = tcProgramFile ;
	strFilePath += szFileName ;
	CAString strLast = strFilePath.Left(strFilePath.ReverseFind('\\') + 1);
	if( !PathFileExists(strLast) )
	{
		CreateHTDirectory(strFilePath);
	}

	if( PathFileExists(strFilePath) )
	{
		DeleteFile(strFilePath) ;
	}
	LPTSTR lpszText = strFilePath.GetBuffer(0);
	strFilePath.ReleaseBuffer();
	int nSuccess = CSocketBase::RecvFile1(s, lpszText ) ;

	return TRUE ;
}

BOOL CFileTransferServiceThread::UploadFileFolder(CIPCSocket& s)
{
	CAString strFilePath = L"" ;
	CAString strFileDirectory = L"" ;
	TCHAR tcProgramFile[MAX_PATH]={0} ;
	TCHAR szFileName[BUFF_SIZE] = {0} ;

	CSocketBase::RecvStringData(s,szFileName,BUFF_SIZE,NULL) ;

	SHGetSpecialFolderPath(NULL,tcProgramFile,CSIDL_PROGRAM_FILES,0) ;
	PathAppend(tcProgramFile, L"\\HuaTu Files\\") ;
	strFileDirectory = tcProgramFile ;
	if( !PathFileExists(strFileDirectory) )
	{
		CreateDirectory(strFileDirectory, NULL) ;
	}
	strFileDirectory += szFileName ;
	if( !PathFileExists(strFileDirectory) )
	{
		CreateDirectory(strFileDirectory, NULL) ;
	}

	return TRUE ;
}

BOOL CFileTransferServiceThread::GetDownLoadInformation(CIPCSocket& s)
{
	CAString strFileDirectory = L"" ;
    TCHAR tcProgramFile[MAX_PATH]={0} ;
	SHGetSpecialFolderPath(NULL,tcProgramFile,CSIDL_PROGRAM_FILES,0) ;
	PathAppend(tcProgramFile, L"\\HuaTu Files") ;
    strFileDirectory = tcProgramFile ;
	if( !PathFileExists(strFileDirectory) )
	{
		CreateDirectory(strFileDirectory, NULL) ;
	}

	xml_node node_root ;
	xml_document dcXML ;
	node_root = dcXML.append_child(L"DownLoadFileSList") ;
	xml_node node_rootHT =  node_root.append_child(L"All");
	node_rootHT.append_attribute( L"Name" ).set_value( L"HuaTu Files" ) ;
	MakeDownLoadListInformationXML(strFileDirectory, node_rootHT) ;

	xml_string_writer wr ;
	dcXML.print(wr, L"", format_default, encoding_wchar) ;
	CAString strDownLoadInformationXML = L"" ;
	strDownLoadInformationXML = wr.result.c_str() ;

	CSocketBase::SendStringData(s, strDownLoadInformationXML) ;

	return TRUE ;
}

DWORD CFileTransferServiceThread::GetLevelOfFile(CAString strFilePath)
{
	DWORD dwFileLevel = 0 ;
	GUID guid;
	char newkey[129] = {0};
	BYTE btKey[128] = {0} ;
	DWORD dwDogID = 0 ;
	GetDogAndKey(btKey,dwDogID) ;
	memcpy(newkey,btKey,128);
	int nSize = strlen(newkey);
	CErrorRecorder cr;

	CCryptWrapper cf(strFilePath,btKey,nSize,dwDogID);

	if(cf.IsCryptedFile(cr))
	{
		CAString strGuid;
		GUID guid;
		int nRet = cf.GetFileAuthorizeGuid(guid,cr);
		nRet = cf.GetFileOutsideGuid(guid,cr);
		cf.GetFileLevel(dwFileLevel, cr);
	}	
	return dwFileLevel ;
}

BOOL CFileTransferServiceThread::TestConnect(CIPCSocket& s)
{
	CAString strFilesCount = L"ConnectSuccess" ;
	CSocketBase::SendStringData(s, strFilesCount) ;

	return TRUE ;
}

BOOL CFileTransferServiceThread::DownLoadFile(CIPCSocket& s)
{
	TCHAR strFilesCount[BUFF_SIZE] = {0};
	TCHAR strFileName[BUFF_SIZE] = {0};
	if(!CSocketBase::RecvStringData(s,strFilesCount,BUFF_SIZE,NULL))
	{
		OutputDebugString(L"RecvStringData failed.") ;
	}

	int nFilesCount = _ttoi(strFilesCount) ;

	CAString strHTFilePath = L"" ;
	TCHAR tcProgramFile[MAX_PATH]={0} ;
	SHGetSpecialFolderPath(NULL,tcProgramFile,CSIDL_PROGRAM_FILES,0) ;
	PathAppend(tcProgramFile, L"\\HuaTu Files\\") ;

	for( int i = 0; i < nFilesCount; i ++ )
	{
		memset(strFileName,0,BUFF_SIZE) ;
        strHTFilePath = tcProgramFile ;
		if(!CSocketBase::RecvStringData(s,strFileName,BUFF_SIZE,NULL))
		{
			OutputDebugString(L"RecvStringData failed.") ;
		}
        strHTFilePath += strFileName ;
        CSocketBase::SendFile1(s, strHTFilePath);
	}

	/*TCHAR szXMLReply[XML_SIZE] = {0};
	if(!CSocketBase::RecvStringData(s,szXMLReply,XML_SIZE,NULL))
	{
		OutputDebugString(L"XML RecvStringData failed.") ;
	}*/

	/*CAString strStandard = L"" ;
	CAString strFileList = L"" ;
    vector<CAString> vcNeedSendFiles ;
    TCHAR tcProgramFile[MAX_PATH]={0} ;
	SHGetSpecialFolderPath(NULL,tcProgramFile,CSIDL_PROGRAM_FILES,0) ;
	PathAppend(tcProgramFile, L"\\HuaTu Files\\") ;
    strStandard = tcProgramFile ;

    strFileList = szXMLReply ;
    GetMapDownLoadList(vcNeedSendFiles,strFileList) ;

	TCHAR szUserLevel[BUFF_SIZE] = {0};
	CAString strUserLevel = L"" ;
	if(!CSocketBase::RecvStringData(s,szUserLevel,BUFF_SIZE,NULL))
	{
		OutputDebugString(L"RecvStringData failed.") ;
	}
	strUserLevel = szUserLevel ;

	int nSendFilesCount = 0 ;
	for( int i = 0; i < vcNeedSendFiles.size(); i ++ )
	{
		if( GetLevelOfFile(vcNeedSendFiles[i]) <= _ttoi(strUserLevel)  )
		{
			nSendFilesCount ++ ;
		}
	}

	CAString strFilesCount = L"" ;
	strFilesCount.Format(L"%d",nSendFilesCount) ;
    CSocketBase::SendStringData(s, strFilesCount) ;

	for(int i = 0; i < vcNeedSendFiles.size(); i ++)
	{
		CAString strFileName = L"" ;
		strFileName = vcNeedSendFiles[i].Right(vcNeedSendFiles[i].GetLength()-(strStandard.ReverseFind('\\')+1));

		if( GetLevelOfFile(vcNeedSendFiles[i]) <= _ttoi(strUserLevel)  )
		{
			CSocketBase::SendStringData(s, strFileName);
			CSocketBase::SendFile1(s, vcNeedSendFiles[i]);
		}
	}*/

    return TRUE ;
}

void CFileTransferServiceThread::GetMapDownLoadList(vector<CAString> &vcNeedSendFiles , CAString strFileList)
{
	CAString strFileName = L"" ;
	xml_document doc;
	doc.load_buffer(strFileList.GetBuffer(0),strFileList.GetLength()*2);
	strFileList.ReleaseBuffer();
	xml_node nRoot = doc.child(L"DownLoadFileSList"); 
	xml_node nFile = nRoot.child(L"df");

	CAString strFilePath = L"" ;
	TCHAR tcProgramFile[MAX_PATH]={0} ;
	SHGetSpecialFolderPath(NULL,tcProgramFile,CSIDL_PROGRAM_FILES,0) ;
	PathAppend(tcProgramFile, L"\\HuaTu Files\\") ;
	while(nFile)
	{
		CAString strFlag = nFile.attribute(L"Flag").value() ;
		if( strFlag.Find(L"1") != -1 )
		{
			strFileName = nFile.attribute(L"文件名").value();
            strFilePath = tcProgramFile ;
            strFilePath += strFileName ;

			if( PathIsDirectory(strFilePath) )
			{
               GetAllFilesPathInDirectory( vcNeedSendFiles, strFilePath ) ;
			}
			else
			{
               vcNeedSendFiles.push_back(strFilePath) ;
			}

		}
		nFile = nFile.next_sibling(L"df");
	}
}

void CFileTransferServiceThread::GetAllFilesPathInDirectory( vector<CAString> &vcNeedSendFiles, CAString strDirectoryPath )
{
	CAString strStandard = L"" ;
	strStandard = strDirectoryPath ;

    strDirectoryPath += L"\\*.*" ;
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(strDirectoryPath,&data);
	if (hFind == NULL || hFind == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"Find File fail.") ;
	}

	CAString strFileName = L"" ;
	while(TRUE)
	{
		strFileName = data.cFileName ;
		CAString strDownPath = L"" ;
		strDownPath = strStandard ;
		strDownPath += L"\\" ;
		strDownPath += strFileName ;
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			if(_tcsicmp(strFileName,TEXT(".")) != 0 && _tcsicmp(strFileName,TEXT("..")) != 0)
			{
				GetAllFilesPathInDirectory( vcNeedSendFiles, strDownPath ) ;
			}
		}
		else
		{
            vcNeedSendFiles.push_back(strDownPath) ;
		}
		if (!FindNextFile(hFind,&data))
			break;
	}
	FindClose(hFind);
}