#pragma once

#define LEN_NAME 128
#define LEN_MD5 33
#define LEN_TIME 32
#define LEN_VERSION 32
#define LEN_DESC 1024

// defined by Zhang Wang
#define DMGS_SOCKET_BUFF_SIZE 4096
#define FILE_SIZE 512
#define LEN_ID   39
#define LEN_DESC1 256

struct DGMS_ModuleInfo
{
	TCHAR strClientID[LEN_NAME];
	TCHAR strModuleName[LEN_NAME];
	TCHAR strModuleMD5[LEN_MD5];
	TCHAR strVersionName[LEN_VERSION];
	int   nModuleSize;
	TCHAR strLastModify[LEN_TIME];
	TCHAR strDesc[LEN_DESC];
	BOOL  bIsMatch;
};

struct DGMS_ModuleInfoForMap
{
	int   nFlag;
	TCHAR strLoginName[LEN_NAME];
	TCHAR strUserName[LEN_NAME];
	TCHAR strRecentUploadTime[LEN_TIME];
	TCHAR strClientID[LEN_NAME];
	TCHAR strComputer[LEN_NAME];
	TCHAR strIp[LEN_VERSION];
	BOOL  bIsMatch;
	int   nStatus;
};

struct DGMS_UploadClientInfo
{
	TCHAR strLoginName[LEN_NAME];
	TCHAR strUserName[LEN_NAME];
	TCHAR strRecentUploadTime[LEN_TIME];
	TCHAR strClientID[LEN_NAME];
	TCHAR strComputer[LEN_NAME];
	TCHAR strIp[LEN_VERSION];
	BOOL  bIsMatch;
    int   nStatus;
};

struct DGMS_ModuleVersionForSearch
{
	TCHAR strComputerName[LEN_NAME]; 
	TCHAR strComputerIp[LEN_NAME];
	TCHAR strVersionName[LEN_VERSION]; 
	TCHAR strModuleMD5[LEN_MD5]; 
	int   nModuleSize; 
	TCHAR strLastModify[LEN_TIME]; 
	BOOL  bIsMatch;
	int   nStatus;
};

struct DGMS_LogRecord
{
	TCHAR strUpdateID[LEN_ID];
    TCHAR strUpdateTime[LEN_TIME];
    int   nUpdateType;
	TCHAR strUpdateDesc[LEN_DESC1];
};

struct DGMS_LogItem
{
    TCHAR strUpdateID[LEN_ID];
	TCHAR strComputerName[LEN_NAME];
    TCHAR strLoginName[LEN_NAME];
};

struct DGMS_LogItemSample
{ 
	TCHAR strComputerName[LEN_NAME];
};

struct DGMS_StrComputerOnly
{
	TCHAR strComputerName[LEN_NAME];
};

typedef enum DGMS_ERROR
{
	// used by Chen Yi
	ERROR_DGMSSEG1 = -100000,
	ERROR_DGMSDB_CONNECT = -99999,
	ERROR_DGMSDB_CREATERECORDSET,
	ERROR_DGMSDB_QUERY,
	ERROR_DGMSDB_NORECORD,

	// used by Zhang Wang
	ERROR_DGMSSEG5 = -30000,
    ERROR_DGMS_CONNECT_SERVER_FAILED,
    ERROR_DGMS_SEND_MSG_TO_SERVER_FAILED,
    ERROR_DGMS_COMMAND_RETRIEVE_UPDATE_LIST_FAILED,
    ERROR_DGMS_COMMAND_RETRIEVE_MODULENAME_LIST_FAILED,
	ERROR_DGMSDB_REPEAT_UPDATEID,

 
} DGMS_ErrorCode;
