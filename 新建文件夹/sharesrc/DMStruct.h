#pragma once
#include "DMErrorCode.h"

#define DM_SOCKET_BUFF_SIZE	4096
#define DM_DEPARTMENTS_LEN	4096
#define DM_SLEVELS_LEN		4096
enum
{
	PERMISSION_NONE = 0x00,
	PERMISSION_READ = 0x01,     //读取
	PERMISSION_WRITE = 0x02,    //写入
	PERMISSION_PRINT = 0x04,    //打印
	PERMISSION_AUTHORIZE = 0x08,//授权
	PERMISSION_DECRYPT = 0x10,  //解密
	PERMISSION_ADMINISTRATOR = 0x20,//密级管理员
	PERMISSION_VISIBLE = 0x40	//可见
};

enum
{
	OPERATOR_NONE = 0x00,
	OPERATOR_READ = 0x01,     //读取
	OPERATOR_WRITE = 0x02,    //写入
	OPERATOR_PRINT = 0x04,    //打印
	OPERATOR_DECRYPT = 0x08,  //解密
	OPERATOR_MODIFYPERMISSION = 0x10,	//更改权限
	OPERATOR_CHANGEOWNER = 0x20,		//更改所有者
	OPERATOR_CHANGESLEVEL = 0x40,      //更改密级
	OPERATOR_CRYPT		= 0x80,			//加密
	OPERATOR_CLEAR = 0x100,		//删除权限信息
};

enum
{
	LOG_OPT_NONE    = 0x00,
	LOG_OPT_READ	= 0x01,
	LOG_OPT_WRITE	= 0x02,
	LOG_OPT_PRINT	= 0x04,
	LOG_OPT_AUTHORIZE = 0x08,
	LOG_OPT_DECRYPT = 0x10,
	LOG_OPT_ADMIN	= 0x20,
};

enum
{
	EMP_TYPE_NONE = 0,
	EMP_TYPE_USER = 1,
	EMP_TYPE_DEPARTMENT = 2,
	EMP_TYPE_ROLE = 3,
};


enum
{
	BALLON_NONE = 0,
	BALLON_INFO,
	BALLON_WARNING,
	BALLON_ERROR,
};

enum
{
	FPREQUEST_STATUS_INIT = 0x01,
	FPREQUEST_STATUS_ACCEPT = 0x02,
	FPREQUEST_STATUS_CLOSED = 0x04,
	FPREQUEST_STATUS_OFFLINE_INIT = 0x08,
	FPREQUEST_STATUS_REJECT = 0x10,
	FPREQUEST_STATUS_RECEIVER_CLOSED = 0x20,

	FPREQUEST_STATUS_READ_FLAG = 0x80, // 此位置上表示已read, 没置上表示未read
	FPREQUEST_STATUS_RECEIVER_READ_FLAG = 0x100,
};

enum
{
	DAU_USER = 0, //0、用户 1、部门
	DAU_DEPARTMENT = 1,
};

enum
{
	LOGIN_TYPE_NORMAL = 0,
	LOGIN_TYPE_DOMAIN = 1,
};

#define LEN_VERSION 32
#define LEN_MD5  33
#define LEN_GUID 37
#define LEN_NAME 128
#define LEN_DESC 1024
#define LEN_FILE_NAME	1024
#define LEN_TIME	32

#define LEN_MAGIC_NUM	38

struct DM_File
{
	TCHAR strFileID[LEN_GUID];
	TCHAR strFileName[LEN_FILE_NAME];
	TCHAR strFileSLevelID[LEN_GUID];
	TCHAR strFileDesc[LEN_DESC];
	TCHAR strFileCreator[LEN_NAME];
	TCHAR strFileCreatorName[LEN_NAME];
	TCHAR strFileOwner[LEN_NAME];
	TCHAR strFileOwnerName[LEN_NAME];
	TCHAR strFileCreateTime[LEN_TIME];
	TCHAR strFilePermissionLastModified[LEN_TIME];
	DWORD dwLogOption;
};

struct DM_FilePermission
{
	TCHAR strFileID[LEN_GUID];
	DWORD dwEmpType;
	TCHAR strEmpID[LEN_NAME];
	TCHAR strEmpName[LEN_NAME];
	DWORD dwPermission;
	TCHAR strTimeLimit[LEN_TIME];
};


struct DM_SLevelInfo
{
	TCHAR strSLevelGUID[LEN_GUID];
	TCHAR strSLevelName[LEN_NAME];
	TCHAR strSLevelDesc[LEN_DESC];
	DWORD dwLogOption;
};

struct DM_SLevelPermission
{
	TCHAR strFileSLevelID[LEN_GUID];
	DWORD dwEmpType;
	TCHAR strEmpID[LEN_NAME];
	TCHAR strEmpName[LEN_NAME];
	DWORD dwPermission;
};

struct DM_RoleUser
{
	TCHAR strRoleId[LEN_GUID];
	TCHAR strLoginName[LEN_NAME];
	TCHAR strUserName[LEN_NAME];
};

struct DM_Role
{
	TCHAR strRoleId[LEN_GUID];
	TCHAR strRoleName[LEN_NAME];
};


struct DM_FileInformationForSearch
{
	TCHAR strFileID[LEN_GUID];
	TCHAR strFileName[LEN_FILE_NAME];
	TCHAR strSLevelName[LEN_NAME];
	TCHAR strFileCreator[LEN_NAME];
	TCHAR strFileOwner[LEN_NAME];
	TCHAR strFileCreateTime[LEN_TIME];
};

struct DM_FileLog
{
	TCHAR strLogID[LEN_GUID];
	TCHAR strFileID[LEN_GUID];
	TCHAR strFileName[LEN_NAME];
	TCHAR strLoginName[LEN_NAME];
	TCHAR strUserName[LEN_NAME];
	DWORD dwOperType;
	TCHAR strLogDesc[LEN_DESC];
	TCHAR strComputer[LEN_NAME];
	TCHAR strIP[LEN_NAME];
	TCHAR strCreateTime[LEN_TIME];
};

struct DM_Set
{
	int nSyncInterval;
	TCHAR strDMServerIP[LEN_NAME];
	int nDMServerPort;
	TCHAR strAccessServerIP[LEN_NAME];
	int nAccessServerPort;
	TCHAR strLogServerIP[LEN_NAME];
	int nLogServerPort;
	TCHAR strUpdateServerIP[LEN_NAME];
	int nUpdateServerPort;
};

struct DM_Set1
{
	int nSyncInterval;
	int nLoginInterval;
	int nRequestCheckInterval;
	TCHAR strDMServerIP[LEN_NAME];
	int nDMServerPort;
	TCHAR strAccessServerIP[LEN_NAME];
	int nAccessServerPort;
	TCHAR strLogServerIP[LEN_NAME];
	int nLogServerPort;
	TCHAR strUpdateServerIP[LEN_NAME];
	int nUpdateServerPort;
};

struct DM_Set2
{
	int nSyncInterval;
	int nLoginInterval;
	int nRequestCheckInterval;
	TCHAR strDMServerIP[LEN_NAME];
	int nDMServerPort;
	TCHAR strAccessServerIP[LEN_NAME];
	int nAccessServerPort;
	TCHAR strLogServerIP[LEN_NAME];
	int nLogServerPort;
	TCHAR strUpdateServerIP[LEN_NAME];
	int nUpdateServerPort;
	TCHAR strRequestServerIP[LEN_NAME];
	int nRequestServerPort;
};

struct DM_Set3
{
	int nSyncInterval;
	int nLoginInterval;
	int nRequestCheckInterval;
	TCHAR strDMServerIP[LEN_NAME];
	int nDMServerPort;
	TCHAR strAccessServerIP[LEN_NAME];
	int nAccessServerPort;
	TCHAR strLogServerIP[LEN_NAME];
	int nLogServerPort;
	TCHAR strUpdateServerIP[LEN_NAME];
	int nUpdateServerPort;
	TCHAR strRequestServerIP[LEN_NAME];
	int nRequestServerPort;
	TCHAR strBackupServerName[LEN_NAME];
};

struct DM_DepartmentUser
{
	int nType;
	//0	user
	//1 department has sub
	TCHAR strDMName[LEN_NAME];
	TCHAR strDMLoginName[LEN_NAME];
};

struct DM_FPermsissionRequest
{
	TCHAR strRequestID[LEN_GUID];
	TCHAR strFileID[LEN_GUID];
	TCHAR strFileName[LEN_NAME];
	TCHAR strSender[LEN_NAME];
	TCHAR strSenderName[LEN_NAME];
	TCHAR strReceiver[LEN_NAME];
	TCHAR strReceiverName[LEN_NAME];
	DWORD dwPermission;
	int	  nStatus;
	TCHAR strRequestReason[LEN_DESC];
	TCHAR strResponseReason[LEN_DESC];
	TCHAR strRequestTime[LEN_TIME];
	TCHAR strResponseTime[LEN_TIME];
	int   nFlowPassed;
};

struct DM_FPermsissionRequest2
{
	TCHAR strRequestID[LEN_GUID];
	TCHAR strFileID[LEN_GUID];
	TCHAR strFileName[LEN_NAME];
	TCHAR strSender[LEN_NAME];
	TCHAR strSenderName[LEN_NAME];
	TCHAR strReceiver[LEN_NAME];
	TCHAR strReceiverName[LEN_NAME];
	DWORD dwPermission;
	int	  nStatus;
	TCHAR strRequestReason[LEN_DESC];
	TCHAR strResponseReason[LEN_DESC];
	TCHAR strRequestTime[LEN_TIME];
	TCHAR strResponseTime[LEN_TIME];
	int   nFlowPassed;
	TCHAR strExpireTime[LEN_TIME];
};

struct DM_DepartmentAndUser 
{
	int		nId;
	TCHAR	strName[LEN_NAME];
	TCHAR	strUserName[LEN_NAME];
	int		nType;				
	BOOL	bHaveChildren;
};

struct DM_DepartmentAndUser1 
{
	TCHAR	strID[LEN_GUID];
	TCHAR	strName[LEN_NAME];
	TCHAR	strUserName[LEN_NAME];
	int		nType;				
	BOOL	bHaveChildren;
};

struct DM_ClientInfo
{
	TCHAR strLoginName[LEN_NAME];
	TCHAR strUserName[LEN_NAME];
	TCHAR strComputerName[LEN_NAME];
	TCHAR strComputerIp[LEN_NAME];
	TCHAR strRecentTime[LEN_TIME];
	TCHAR strServerName[LEN_NAME];
};

struct DM_ClientInfo1
{
	TCHAR strLoginName[LEN_NAME];
	TCHAR strUserName[LEN_NAME];
	TCHAR strComputerName[LEN_NAME];
	TCHAR strComputerIp[LEN_NAME];
	TCHAR strRecentTime[LEN_TIME];
	TCHAR strServerName[LEN_NAME];
	int	  nState;
	TCHAR strVersion[LEN_NAME];
};

struct DM_ClientInfo2
{
	TCHAR strLoginName[LEN_NAME];
	TCHAR strUserName[LEN_NAME];
	TCHAR strComputerName[LEN_NAME];
	TCHAR strComputerIp[LEN_NAME];
	TCHAR strRecentTime[LEN_TIME];
	TCHAR strServerName[LEN_NAME];
	int	  nState;
	TCHAR strVersion[LEN_NAME];
	TCHAR strClientID[LEN_NAME];
};

struct DM_Version
{
	TCHAR strClientID[LEN_NAME];
	TCHAR strModuleName[LEN_NAME];
	TCHAR strVersionName[LEN_GUID];
	TCHAR strModuleMD5[LEN_MD5];
	int   nModuleSize;
	TCHAR strLastModify[LEN_TIME];
	TCHAR strVersionDesc[LEN_DESC];
};

struct DM_ModuleVersionForSearch//add new
{
	TCHAR strComputerName[LEN_NAME];//计算机名
	TCHAR strComputerIp[LEN_NAME];//计算机IP
    int	  nState;//状态
    TCHAR strVersionName[LEN_VERSION];//版本名
	TCHAR strModuleMD5[LEN_MD5];//模块MD5码
	int   nModuleSize;//模块大小
	TCHAR strLastModify[LEN_TIME];//最后修改时间
	TCHAR strVersionDesc[LEN_DESC];//版本描述
};

struct DM_GetVersionFromClient
{
	TCHAR strModuleName[LEN_NAME];
	TCHAR strVersionName[LEN_VERSION];
	TCHAR strModuleMD5[LEN_MD5];
	int   nModuleSize;
	TCHAR strLastModify[LEN_TIME];
};

struct DM_ModuleNameAndMD5
{
	TCHAR strModuleName[LEN_NAME];
	TCHAR strModuleMD5[LEN_MD5];
};

struct SetupSetting
{
	DWORD bRecoveCacheDB;
	DWORD bDisplay;
};

struct DM_AuthorizationFile
{
	DM_File *pdmFile;
	DM_FilePermission *pdmFilePermission;
	int nCount;
};

struct DM_ControlFileType
{
	TCHAR tcFileExt[LEN_NAME];
};
