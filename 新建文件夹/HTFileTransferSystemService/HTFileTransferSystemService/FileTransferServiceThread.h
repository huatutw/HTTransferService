#pragma once
#include "stdafx.h"

#include <vector>
#include "TaskUnit.h"
#include "SocketBase.h"
#include "spIPCComm.h"
#include "DGCommonFunc.h"
#include "pugiconfig.h"
#include "pugixml.h"

using namespace pugi ;
using namespace std ;

class CFileTransferServiceThread :
	public CTaskUnit
{
public:
	CFileTransferServiceThread(void);
	~CFileTransferServiceThread(void);

	void DealCommond();
	virtual void DoWork();

	CAString				m_strConnect;
	SOCKET				    m_sComm;

private:
	BOOL UploadFile(CIPCSocket& s) ;
	BOOL UploadFileFolder(CIPCSocket& s) ;
	BOOL GetDownLoadInformation(CIPCSocket& s) ;
	BOOL DownLoadFile(CIPCSocket& s) ;
	BOOL TestConnect(CIPCSocket& s) ;
	BOOL MakeDownLoadFileInformationXML(CAString &strDownLoadFileXML,CAString strDirectoryPath);
	BOOL InitSystemInfo(SystemApi32Use& info) ;
	BOOL CreateHTDirectory(CAString strPath);
	BOOL GetDogAndKey(BYTE  *btKey, DWORD &dwDogID ) ;

	void MakeDownLoadListInformationXML(CAString strDirectoryPath,xml_node &node_root ) ;
	void GetMapDownLoadList(vector<CAString> &vcNeedSendFiles, CAString strFileList) ;
	void GetAllFilesPathInDirectory( vector<CAString> &vcNeedSendFiles, CAString strDirectoryPath ) ;

	DWORD CFileTransferServiceThread::GetLevelOfFile(CAString strFilePath) ;
	DWORD CFileTransferServiceThread::GetFileLevel(CAString strDirectoryPath,DWORD &dwSmallFileLevel) ;
};
