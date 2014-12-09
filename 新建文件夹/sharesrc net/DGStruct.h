#pragma once

#define LEN_MAX 8000
#ifndef LEN_NAME
#define LEN_NAME	128
#endif


#ifndef LEN_TIME
#define LEN_TIME	32
#endif
#include <vector>
using namespace std;

//ipc name
#ifndef		IPC_Name_LocalUpdateService					
#define		IPC_Name_LocalUpdateService					"HTLocalUpdateService"
#endif

#ifndef IPC_Name_HTLUpdateService					
#define	IPC_Name_HTLUpdateService						"HTLUpdateService"
#endif

#ifndef IPC_Name_DGLocalLog
#define IPC_Name_DGLocalLog								"DGLocalLogService"
#endif

#ifndef IPC_Name_DGLicense
#define IPC_Name_DGLicense								"DGLocalLicenseModule"
#endif

#ifndef HT_PROINFO
#define HT_PROINFO

#define HTP_H3			 0
#define HTP_H5			 1
#define HTP_H7			 2

#define HTV_H3_Standard	 1
#define HTV_H3_Group	 2

#define HTM_LevelMgr	 0x1
#define HTM_DGFlow		 0x2
#define HTM_Authorize	 0x4
#define HTM_Outside		 0x8

#endif
enum DG_ERROR_RECORDER
{
	//DGCrypt文件处理错误号
	DGCrypt_SocketConnectError		=1001,//socket创建连接时错误
	DGCrypt_UserModalError,				  //用户界面显示失败
	DGCrypt_RecvDataFilure,				//在初始用户信息的时接受数据失败
	DGCrypt_DecryptAppError,			//对文件不可进行操作
	DGCrypt_OffLineCannotDecrypt,		//离线状态下不可解密文件
	DGCrypt_OperModeError,				//选择的操作模式不正确
	DGCrypt_SendDeryptFileFailure,		//发送解密文件失败
	DGCrypt_DonotFindFile,				//没有发现文件
	DGCrypt_DonotFileFirectory,			//没有发现文件路径
	DGCrypt_EncryptZIPFailure,			//加密压缩文件失败

	DGCrypt_DncryptZIPFailure,			//解密压缩文件失败
	DGCrypt_CopyFileError,				//拷贝文件发生错误
	DGCrypt_HavenotSelectOper,			//没有选择操作
	DGCrypt_CreateCompressProcessToTempFailed,//创建解压压缩文件到临时文件夹进程失败
	DGCrypt_EncryptedFileError,			//文件已经被加密
	DGCrypt_DncryptedFileError,			//文件不是加密文件
	DGCrypt_NoPermissionToDecryptFile,	//没有权限解密文件
	DGCrypt_TestLogServerSocketFailure,	//测试日志服务socket连接失败
	DGCrypt_TestLogServerRcvDataFailure,//测试日志服务接收数据失败
	DGCrypt_LogServerFailure,			//测试出日志服务失败

	DGCrypt_AuthorServerNoConnect,		//授权服务没有连接
	DGCrypt_AuthorServerSocketConnectFailure,//授权服务没有连接
	DGCrypt_AuthorServerGetDataFailure,	//授权服务连接数据失败
	DGCrypt_AuthorServerGetValueFailure,//授权服务获取值失败
	DGCrypt_GetAuthorPermisioneFailure,	//获取授权权限失败
	DGCrypt_AuthorFileOffLine,			//离网不能解密授权文件
	DGCrypt_AuthorFileNoDecryptPermision,//对授权文件没有解密权限
	DGCrypt_GetLoginNameFailreFromRrg,	//从注册表中获取login名失败
	DGCrypt_GetSystemInfoFailure,		//获取共享内存值失败
	DGCrypt_OpenFileMappingFailure,		//打开共享内存失败

	DGCrypt_InitUserInfoFailure,		//初始化用户信息失败
	DGCrypt_OpenFileFailure,			//打开文件失败
	DGCrypt_GetUserModelFailure,		//获取用户模式失败,
	DGCrypt_GetLeveTypelFailure,		//获取文件等级类型失败
	DGCrypt_GetFileLevelFailure,		//获取文件等级信息失败
	DGCrypt_GetFileAuthorGuidFailure,	//获取文件授权GUID失败
	DGCrypt_ChangeLevelHavenotFile,		//没有发现可以调整等级的文件
	DGCrypt_ChangeLevelFileNoEncrypt,	//调整等级的文件不是加密文件
	DGCrypt_GetOffLineInfoFailure,		//获取离线情况下失败,
	DGCrypt_HaveNotPermissionChangeLevel,//没有调整等级的权限

	DGCrypt_HaveNotThisFile,			//不存在这个文件
	DGCrypt_HavaNotConnectClient,		//没有连接客户端
	DGCrypt_GetSystemInfoFailed,		//获取共享内存的信息失败
	DGCrypt_CreateConnectSocketFailed,	//创建连接Socket失败
	DGCrypt_RecvLong2StringDataFailed,	//接受Long2命令的数据失败
	DGCrypt_GetUserInfoError,			//获取用户的权限信息错误
	DGCrypt_CreateDeviceError,			//打开设备失败
	DGCrypt_GetSpeFolderPathFailed,		//获取特殊文件路径失败
	DGCrypt_LoadOfflineConfigInfoFailed,//加载离线配置信息失败
	DGCrypt_GetClientStateFailed,		//获取用户离线信息失败

	DGCrypt_OpenRegFailed,				//打开注册表获取服务信息失败
	DGCrypt_InitWinSockFailed,			//初始化网络设置失败
	DGCrypt_SocketStoreVersionError,	//WindowSocker版本错误
	DGCrypt_DogExistHaveNotMappingValue,//狗存在但是没有获取到共享内存的值
	DGCrypt_CanccelEnterDecrypt,		//登入取消，不进行文件处理
	DGCrypt_GetUserNameFailed,			//获取用户名失败
	DGCrypt_OpenRegGetUserInfoFailed,	//打开注册表获取用户信息失败
	DGCrypt_ReadRegLogServiceFailed,	//读取注册表中日志服务失败
	DGCrypt_LogServiceError,			//日志服务错误
	DGCrypt_ThereAreNotSelectAnyFile,	//没有选取任何文件进行操作

	DGCrypt_HaveNotGetUSerModal,		//沒有取得组织结构
	DGCrypt_SocketCreateFailed,			//Socket创建失败
	DGCrypt_InitNetServiceFailed,		//初始化网络服务失败
	DGCrypt_CleanNetSocketFailed,		//注销Windows Socket失败
	DGCrypt_LoadDGLocalSupportDllFailed,//加载DGlocalSupportDll失败
	DGCrypt_NoFileTypeCanOperate,		//没有可操作的文件类型
	DGCrypt_EncryptFileFailed,			//不支持对此文件进行操作
	DGCrypt_DecryptFileFailed,			//解密文件失败
	DGCrypt_FileHadEncrypted,			//文件已经被加密
	DGCrypt_NoConnctAuthortionService,	//没有连接授权服务

	DGCrypt_OfflineCanNotUploadFile,	//离线情况下不上传文件
	DGCrypt_UploadEncryptFileFailed,	//解密时上传密文失败
	DGCrypt_UploadPermisiontFileFailed,	//离线状态下不上传授权文件密文
	DGCrypt_OfflineCanNotUploadPermissionFile,//离线情况下不上传授权文件密文
	DGCrypt_NoPrivDecryptFile,			//没有权限解密文件
	DGCrypt_ConnectServiceFailedPassowrdOrUserNameError,//链接服务失败，Ip或端口错误
	DGCrypt_CheckDogInfo_ERROR,			//获取用户的权限失败
	DGCrypt_FileHaveNotEncrypted,		//文件不是加密文件
	DGCrypt_NoAllowOperateToFile,		//文件不支持该操作



	//DGClientSet错误集
	DGClientSet_GetDGServerIniInfoFailure = 2001,	//获取DGServer.ini里面的信息文件失败
	DGClientSet_GetSpeFolderPathFailure,//获取特殊文件夹路径失败
	DGClientSet_LoginNameIsEmpty,		//登入用户名为空
	DGClientSet_LoginPasswordEmpty,		//登入密码为空
	DGClientSet_TestLogServerFailure,	//日志服务失败
	DGClientSet_ConncetDatabaseFailure,	//连接数据库失败
	DGClientSet_LoginPasswordError,		//登入密码错误
	DGClientSet_GetHostIPFailed,		//获取主机的IP失败
	DGClientSet_FindControldedProcess,	//发现受控程序，退出容易损坏
	DGClientSet_HavenotGetPort,			//没有在注册表里发现端口号

	DGClientSet_LoginNameError,			//登入账号错误
	DGClientSet_PasswordDonotEmpty,		//密码不能为空
	DGClientSet_LoginPasswordNoneMatch,	//密码不匹配
	DGClientSet_DoMainPasswordError,	//域密码错误
	DGClientSet_ModifyPasswordFailed,	//修改密码失败
	DGClientSet_PasswordError,			//老密码错误
	DGClientSet_PasswordShort,			//密码太短，密码由字符、数字、下划线组成
	DGClientSet_PasswordSimple,			//密码过于简单
	DGClientSet_NoAllowModifyPassword,	//不允许修改密码
	DGClientSet_ConnectDatabaseFailed,	//连接数据库失败，检测IP和port是否正确

	DGClientSet_PasswordOrLoginNameError,//密码或者是登入账号错误
	DGClientSet_TestUpdateServerFailed,	//测试更新服务失败
	DGClientSet_TestValidateServerFailed,//测试服务失败
	DGClientSet_TestDogServiceFailed,	//测试狗服务失败
	DGClientSet_TestLogServiceFailed,	//检测日志服务失败
	DGClientSet_TestPerssionServiceFailed,//检测授权服务失败
	DGClientSet_TestHofServiceFailed,	//检测外发服务失败

	//DGClient 错误集
	DGClient_ProcessNoWOW64 = 3001,			//程序不是运行在WOW64环境下
	DGClient_GetSpecFolderPathFailed,	//获取特殊文件路径失败
	DGClient_GetHostIpFailed,			//获取主机IP地址失败
	DGClient_OpenFileFailed,			//打开文件失败
	DGClient_ProcessAlreadExist,		//进程已经存在
	DGClient_SocketStartupFailed,		//启动网络初始化失败
	DGClient_GetProcessIDTokenFailed,	//获取进程令牌失败
	DGClient_CheckIntegrityLevelExist,	//检测完整等级进程存在
	DGClient_OpenOfflineConfigFileFailed,//打开离线配置文件失败
	DGClient_SendCommandToClientUFailed,//发送命令道clienU失败

	DGClient_CreateDGWebSetFailed,		//创建DGWeb配置文件失败
	DGClient_CannotFindDiskNumber,		//找不到磁盘个数
	DGClient_ControlDeviceIOFailed,		//控制设备IO失败
	DGClient_GetDeviceInfoError,		//获取磁盘信息错误
	DGClient_CreateClientListenSocketFailed,//创建Client监听Socket失败
	DGClient_GetDGFlowSetFailed,		//获取法外配置文件失败
	DGClient_GetDGFlowSetIniFailed,		//获取DGFlow ini配置文件失败
	DGClient_GetDGFlowURLFailed,		//获取外发的网址失败
	DGClient_ClientNotRuning,			//客户端没在运行
	DGClient_ShowDGBubbleDlgMsgException,//显示提示对话框异常

	DGClient_OpenClientConnectRegFailed,//打开连接服务器注册表失败
	DGClient_OpenProcessTokenFailed,	//打开程序令牌失败
	DGClient_RegisterDeviceNotificationFailed,//注册设备失败
	DGClient_UserNoPriv,				//用户没有权限
	DGClient_DoRegisterDeviceFailed,	//注册设备失败
	DGClient_GetProcessHandleFailed,	//获取进程句柄失败
	DGClient_GetNextProcessHandleFailed,//获取下一个进程句柄失败
	DGClient_OpenFileMappingFailed,		//打开共享内存失败
	DGClient_ViewFileToMapFailed,		//文件影身失败
	DGClient_OpenMutexFailed,			//没有互斥量

	DGClient_HaveNotdwCurPriv,			//没有权限
	DGClient_CreateFileFailed,			//打开文件失败
	DGClient_GetDeviceGuidFailed,		//该获取GUID相关的设备信息集句柄失败
	DGClient_CreateFileError,			//读取文件错误
	DGClient_HaveNotAddOfflineTime,		//没有增加离网时间
	DGClient_GetFileSizeError,			//获取文件的大小错误
	DGClient_MutexHadAlreadExisted,		//互斥量已经存在
	DGClient_OpenMappingFailed,			//打开共享内存失败
	DGClient_ClientStopRunning,			//客户端通知运行
	DGClient_NoPrivControlUSB,			//没有权限控制USB接口

	DGClient_ServiceHaveNotCall,		//服务没有响应
	DGClient_UnUSBSTOR,					//USB没有被控制
	DGClient_OpenDiskFailed,			//打开磁盘失败
	DGClient_FlowConnectFailed,			//审批连接失败
	DGClient_NoDispalyTrayIcon,			//不显示托盘图标


	DGLSupport_RecvCmdError,			//在DGLOCALSupport中接收cmd错误
	DGLSupport_RecvDataError,			//在DGLOCALSupport中接收数据错误错误
	DGLSupport_CreateConnectSocketError,//在DGLOCALSupport中连接服务器失败
	DGLSupport_CheckLogSvrFailed		,//检测日志服务失败
	DGLSupport_CheckDogSvrFailed		,//检测狗服务失败
	DGLSupport_CheckUpdateSvrFailed		,//检测更新服务失败
	DGLSupport_CheckFilePermissionSvrFailed,//检测权限服务失败
	DGLSupport_RecvFileError			,//接受文件失败 
	DGLSupport_InsufficientSpace		,//RecvNewestInitData接口用来接收审批信息的空间不够大

	DGLLicense_AlreadyExitInstance,//实例已经存在
	DGLLicense_InstanceNotFound,//实例不存在
	DGLLicense_OfflineInfoNotFound,//该情况将不生成离线文件
	DGLLicense_AlreadyInit,//实例已经存在
	DGLLicense_SrvsInfoNotFound,//没有找到服务器信息

	DGLocalLog_AlreadyInit,//日志对象已经初始化
	DGLocalLog_DllNotFound,//日志dll组件加载失败
	DGLocalLog_IpcSendLogFailed,//IPC发送日志失败
	DGLocalLog_InterfaceNotFound,//接口没有找到
	DGLocalLog_AlreadyExitInstance,//日志服务实例已经存在
	DGLocalLog_InstanceNotCreate,//实例没有创建
	DGLocalLog_DGSendPrintLogFailed,//发送打印日志失败
	DGLocalLog_DGSendPrintLogSnpFailed,//发送快照失败
	DGLocalLog_DGSendFileOperateLogFailed,//发送文件日志失败

	T3Mode_RecvCmdError,			//在T3BusinessMode中接收cmd错误
	T3Mode_RecvDataError,			//在T3BusinessMode中接收数据错误
	T3Mode_CreateConnectSocketError,//在T3BusinessMode中连接服务器失败
	T3Mode_CheckT3SvrFailed,		//检测外发服务失败
	T3Mode_CannotOpenFile,			//无法打开外发文件
	T3Mode_ErrorPassword,			//密码不正确
	T3Mode_ErrorInstallDV,			//服务器更新DV安装数出错，DV安装失败
	T3Mode_ErrorUserName,			//没有该用户
	T3Mode_ErrorReadRegister,		//从注册表读取数据失败
	T3Mode_ErrorGetFileGuid,		//读取文件GUID失败
	T3Mode_ErrorLoadDLL,			//导入T3BusinessMode.dll失败

	HTLUpdate_ReadRegisterError,		//从注册表读取相关更新的数据失败
	HTLUpdate_CreateConnectSocketError,	//连接服务器失败
	HTLUpdate_RecvCmdError,				//接收cmd错误
	HTLUpdate_RecvDataError,			//接收数据错误
	HTLUpdate_RecvFileError,			//从服务器接收文件失败
	HTLUpdate_RecvThreadError,			//从服务器接收文件的函数出现异常
	HTLUpdate_GetFileMD5Error,			//获取文件MD5码失败
	HTLUpdate_GetFileVersionIDError,	//获取文件版本号失败
	HTLUpdate_InstallServiceError,		//注册服务出错


	HTUpdate_GetXmlFailed,				//获取xml失败
	HTUpdate_ConnectDBFailed,			//连接DB失败
	HTUpdate_SelectDBFailed,			//查询失败
	HTUpdate_RecvFailed,				//接受数据失败
	HTUpdate_CreateIPCFailed,			//创建IPC失败

};


enum DG_IPC_LOG_TYPE
{
	DG_LOG_PRINT = 1,//打印日志
	DG_LOG_FILEOPERATE, //文件操作日志
	DG_LOG_AUTHORIZESLEVEL,//授权密级日志
	DG_LOG_AUTHORIZEPERIMISSION,//授权权限日志
	DG_LOG_DSMFILE,//授权文件日志
};
enum DG_IPC_CMD
{
	IPC_CMD_NewUserConfigInfo		= 1,
	IPC_CMD_GetDGLanding,
	IPC_CMD_UpdateAprroveInfo,
	IPC_CMD_ChangeCFGNormal,
	IPC_CMD_ChangeCFGClose,
	IPC_CMD_ReflashReg,
	IPC_CMD_UploadWaterMarkModule,
};
enum DG_OFFLINETYPE
{
	DGOffline_Normal = -1,
	DGOffline_Fault	 = 0,
	DGOffline_SoftOffline =1,
	DGOffline_ClockLock = 2,
};

enum DG_ONOFFLINETYPE
{
	DGOnOffline	 = 0,
	DGOnBackNet =1,
	DGOnFlashClockLock = 2,
};
enum DG_PURVIEW
{
	CLIENT_OPENCLOSE_STATE = 0x80,
};
struct DG_LogAlarmRule
{
	int	  nId;
	TCHAR tcName[LEN_NAME];
	TCHAR tcReceive[LEN_MAX];
	int	  nState;
	int   nType;
	TCHAR tcCreateTime[LEN_TIME];
};

#ifndef LEVEL_ARR_SIZE
#define LEVEL_ARR_SIZE 6000
#endif
#ifndef DOCGUARDER42
struct Offline_Info2 // DocGuarder3.0结构
{
	BYTE  Key[128];
	DWORD DogID;
	DWORD dwCurPriv;
	DWORD dwOverTime;
	DWORD dwCryptLevel;
	int	  nPolicyType;
	int	  nPolicyCount;
	int	  arrPolicy[LEVEL_ARR_SIZE];		//支持1000个等级的例外策略
	int   bandmask;				//绑定信息
	char strNetSerial[32];		
	char strHardSerial[32];
	char strPassword[32];
	char strIP[16];				 //支持广域网回收节点
	BOOL  bUseTimeControl;		 //是否使用时钟功能  TRUE: 则下三个参数不起作用, FALSE: 则 dwOverTime 不起作用
	tm    tmBase;			     //锁内的基准时间
	int dwStartTimeSecond;     //离开始使用的时间,(秒为单位)
	int dwEndTimeSecond;		 //离结束使用的时间,(秒为单位)
};

struct Offline_Info3 // 离网审批结构
{
	TCHAR		strLoginName[512];			//离网登录名
	TCHAR		strComputerName[512];		//离网计算机名
	TCHAR		strClientID[40];			//离网客户端ID
	TCHAR		strNetErorID[40];			//离网授权ID，供加时使用
	int			nTime;						//离网时间
	BOOL		bIsAddTime;					//是否增加时间
	SYSTEMTIME	stOverDate;					//审批文件可使用时效
};

struct SystemApi32Use		//对应用程序必须传递的数据
{
	BYTE  Key[128];			//加密密钥
	DWORD DogID;			//狗号
	DWORD dwCurPriv;		//当前的用户权限
	DWORD dwCryptLevel;		//用户的等级
	int	  nPolicyType;		//策略的类型  (暂时无效)
	int	  nPolicyCount;		//策略的实际个数
	int	  arrPolicy[LEVEL_ARR_SIZE];  //支持1000个等级的打开策略
};

#else
struct Offline_Info2 // DocGuarder3.0结构
{
	BYTE  Key[128];
	DWORD DogID;
	LONGLONG dwCurPriv;
	DWORD dwOverTime;
	DWORD dwCryptLevel;
	int	  nPolicyType;
	int	  nPolicyCount;
	int	  arrPolicy[LEVEL_ARR_SIZE];		//支持1000个等级的例外策略
	int   bandmask;				//绑定信息
	char strNetSerial[32];		
	char strHardSerial[32];
	char strPassword[32];
	char strIP[16];				 //支持广域网回收节点
	BOOL  bUseTimeControl;		 //是否使用时钟功能  TRUE: 则下三个参数不起作用, FALSE: 则 dwOverTime 不起作用
	tm    tmBase;			     //锁内的基准时间
	int dwStartTimeSecond;     //离开始使用的时间,(秒为单位)
	int dwEndTimeSecond;		 //离结束使用的时间,(秒为单位)
};

struct Offline_Info3 // 离网审批结构
{
	TCHAR		strLoginName[BUFF_SIZE];			//离网登录名
	TCHAR		strComputerName[BUFF_SIZE];		//离网计算机名
	TCHAR		strClientID[40];			//离网客户端ID
	TCHAR		strNetErorID[40];			//离网授权ID，供加时使用
	int			nTime;						//离网时间
	BOOL		bIsAddTime;					//是否增加时间
	SYSTEMTIME	stOverDate;					//审批文件可使用时效
};

struct SystemApi32Use		//对应用程序必须传递的数据
{
	BYTE  Key[128];			//加密密钥
	DWORD DogID;			//狗号
	LONGLONG dwCurPriv;		//当前的用户权限
	DWORD dwCryptLevel;		//用户的等级
	int	  nPolicyType;		//策略的类型  (暂时无效)
	int	  nPolicyCount;		//策略的实际个数
	int	  arrPolicy[LEVEL_ARR_SIZE];  //支持1000个等级的打开策略
};
#endif


struct DG_LogAlarmRuleDetail
{
	int   nRid;
	int   nOperate;
	TCHAR tcExpression[LEN_MAX];
	int   nType;
	TCHAR tcField[LEN_NAME];
	TCHAR tcReceive[LEN_MAX];
};

struct	DG_NeedApproveInfo
{
	int nColID;
	TCHAR cColName[255];
};

enum DG_IPC_TYPE
{
	IPC_TYPE_LOG =1,
};
struct st_Log_Print_Record  
{
	int nIpcType;
	int	  nPrintID;
	WCHAR wcPrintIP[32];
	WCHAR wcPrintComputer[40];
	WCHAR wcPrintTime[40];
	WCHAR wcPrintUser[50];
	WCHAR wcPrintDocName[MAX_PATH];
	WCHAR wcPrintSnapshot[MAX_PATH];
	BOOL  bPrintDeleted;
	WCHAR wcManageGuid[38];
	WCHAR wcLogGuid[38];
};

struct DG_Log_Print_Record  
{
	int	  nPrintID;
	WCHAR wcPrintIP[32];
	WCHAR wcPrintComputer[40];
	WCHAR wcPrintTime[40];
	WCHAR wcPrintUser[50];
	WCHAR wcPrintDocName[MAX_PATH];
	WCHAR wcPrintSnapshot[MAX_PATH];
	BOOL  bPrintDeleted;
	WCHAR wcManageGuid[38];
	WCHAR wcLogGuid[38];
};

struct DG_Log_FileOperation_Record  
{
	int	  nLogFileID;
	WCHAR wcLogFileIP[32];					//IP
	WCHAR wcLogFileComputer[40];			//计算机名
	WCHAR wcLogFileTime[40];                //操作时间
	WCHAR wcLogFileUser[50];                //用户名
	WCHAR wcLogFileName[MAX_PATH];			//文件路径
	int	  nFileSlevel;						//文件等级
	int   nFileOper;						//操作类型
	int   nInternalError;					//内部错误
	int   nSystemError;					    //系统错误
	BOOL  bDeleted;							//是否删除
	WCHAR wcManageGuid[40];					//管理GUID
	WCHAR wcFileServerPath[MAX_PATH];		//上传服务器上的路径
	WCHAR wcLogGuid[40];					//日志GUID
	WCHAR wcFileGuid[40];					//文件GUID
	WCHAR wcFileOperateGuid[40];					//操作ID
};

struct ST_Log_Uninstall_Record
{
	int nModule;
	TCHAR szIP[32];
	TCHAR szComputerName[128];
	TCHAR szUninstallTime[128];
	TCHAR szUser[128];
	TCHAR szModule[128];
};


struct DBS_SLevelInfo
{
	int nType;// 1:用户	2:部门
	int nPermission; //权限
	TCHAR szNameId[256];//logname
	TCHAR szName[256];//用户名/部门名
	TCHAR szSlevelName[256];//密级名
	TCHAR szSleveId[256];//密级Id
};

struct DBS_FilePermission
{
	int nType;// 1:用户	2:部门
	int nPermission; //权限
	TCHAR szEmpId[256];//用户loginid/部门名
	TCHAR szName[256];//用户名/部门名
	TCHAR szSlevelName[256];//密级名
	TCHAR szFileId[256];//密级Id
	TCHAR szExpireDate[256];//附加权限有效期
};

struct DBS_FileInfo
{
	TCHAR szGuid[64];
	TCHAR szSuffix[64];
	TCHAR szFilePath[MAX_PATH];
	TCHAR szCreateName[255];

};
struct DBS_AuthorizeInfo
{
	TCHAR szNewId[40];
	TCHAR szOrgId[40];
	TCHAR szUser[256];
};

struct st_Tn_Monitor
{
	WCHAR	wcIP[LEN_NAME];
	WCHAR	wcComputer[LEN_NAME];
	WCHAR	wcLoginName[LEN_NAME];
	WCHAR	wcRecentTime[LEN_TIME];
	WCHAR	wcVer[LEN_NAME];
	WCHAR	wcServerName[LEN_NAME];
	WCHAR	wcClientID[LEN_NAME];
	int		nState;
};

struct DBS_FileLogInfo
{
	TCHAR szGuid[64];
	TCHAR szSuffix[64];
	TCHAR szFilePath[MAX_PATH];
	TCHAR szSvrFilePath[MAX_PATH];
	TCHAR szCreateName[255];

};

struct DBS_LOG_FilePermission
{
	int nType;// 1:用户	2:部门
	int nPermission; //权限
	TCHAR szEmpId[256];//用户loginid/部门名
	TCHAR szName[256];//用户名/部门名
	TCHAR szSlevelName[256];//密级名
	TCHAR szFileId[256];//密级Id
	TCHAR szExpireDate[256];//附加权限有效期
	TCHAR szLogGuid[40];
};

struct DBS_LOG_SLevelInfo
{
	int nType;// 1:用户	2:部门
	int nPermission; //权限
	TCHAR szNameId[256];//logname
	TCHAR szName[256];//用户名/部门名
	TCHAR szSlevelName[256];//密级名
	TCHAR szSleveId[256];//密级Id
	TCHAR szLogGuid[40];
};


typedef std::vector<st_Tn_Monitor> vecStTnMonitor;

/*
外发相关的结构
*/
struct T3_DBS_LoginInfo
{
	TCHAR szLoginName[128];
	TCHAR szLoginPasswd[128];
};

struct T3_DBS_RTLoginInfo
{
	int nRet;//0 校验通过 1 无此账号 2密码错误
	int nSetupCount;
};

struct T3_DBS_SetupInfo
{
	TCHAR szName[128];
};

struct T3_DBS_RTSetupInfo
{
	int nRet;// 0 成功 1失败
};


struct T3_DBS_CheckFileInfo
{
	int nFileId;
	TCHAR szPasswd[128];
	TCHAR szName[128];

};

struct T3_DBS_RTCheckFileInfo
{
	bool bNeedCheckPass;
	int nRet;//查询结果 0正常 1 文件权限过期 2无法查到权限信息 3.打开次数用尽 4 密码错误 5账号不存在
	int nPermission;

};

struct Outside_CheckDog
{
	DWORD dwLable;
	TCHAR strCompanyMD5[40];	//公司
	BYTE  Key[128];				//加密密钥
	DWORD DogID;				//狗号
	TCHAR hardSerial[128];		//硬件绑定号
};


struct Outside_CheckDog2
{
	int nMode;
	DWORD dwDogID;
};

enum OutsideFile_Type
{
	DG_Outsidefile_Type_Normal = -1,		//明文
	DG_Outsidefile_Type_Encrypt = 0,		//密文
	DG_Outsidefile_Type_4_5 = 1,			//4.5外发文件
	DG_Outsidefile_Type_4_6 = 2,			//4.6外发文件
	DG_Outsidefile_Type_Compression,		//压缩文件
};



struct DG_SPIPCS
{
	int nType;
	int nComIndex;
	int nbaudrate;
	char cPhoneNumber[256];
	TCHAR szData[256];
};

/*
更新服务相关结构
*/

enum HTLU_IPC_LOG_TYPE
{
	IPC_HTLU_ServiceInfo = 1,	//服务器IP和端口信息
	IPC_HTLU_BasicPCInfo,			//个人计算机相关信息
	IPC_HTLU_ServiceTestContinue,
	IPC_HTLU_ServiceTestGetUserName,
};

struct HTLU_ServiceInfo
{
	TCHAR szServIP[1024];
	DWORD dwPort;
};

struct HTLU_BasicPCInfo
{
	TCHAR strDName[1024];
	TCHAR strUser[1024];
	TCHAR strGuid[1024];
	TCHAR strGID[1024];
	TCHAR strDes[4096];
};