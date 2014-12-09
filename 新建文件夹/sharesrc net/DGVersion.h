
#define VERSION_OUTSIDESETTINGEX	0x10
enum
{
	VERSION_STANDARD_HUATU			= 0x1,		//华途标准版本   ok
	VERSION_STANDARD_DAHENG			= 0x2,		//大恒标准版本
	VERSION_XINMEIXING				= 0x4,		//新美星版本     ok
	VERSION_SHUANGYOU				= 0x8,		//双友版本
	VERSION_STANDARD_CONFIG			= 0x10,		//具有配置程序的版本
	VERSION_STANDARD_USBSTOR		= 0x20,		//是否对U盘进行全盘扫描
	VERSION_STANDARD_DOMAIN			= 0x40,		//域运行模式     ok
	VERSION_STANDARD_SCANOVER		= 0x80,		//扫描过程结束后 
	VERSION_STANDARD_NETERRORTIME	= 0x100,	//网络故障时间
	VERSION_UPDATE_BIGANT			= 0x200,	//bigant集成版
	VERSION_DGCRYPT_NETLOAD			= 0x400,	//加解密插件需要手工输入登录  ok
	VERSION_DGCRYPT_PASSWORD		= 0x800,	//加解密插件需要输入口令   ok
	VERSION_DGCRYPT_NODECRYPT		= 0x1000,	//不提供右键解密功能
	VERSION_DGCLIENT_OPENCLOSE		= 0x2000,	//非域模式登录使用开关客户端(云南)
	VERSION_DGCLIENT_ALWAYSOPENCLOSE = 0x4000,  //客户端始终为开关客户端模式(一重)
	VERSION_MAIL_MEASURE			= 0x8000,	//右键菜单增加邮件处理功能
	VERSION_RUNDLL_ACCESS			= 0x10000,	//关闭客户端时对加密文件拒绝访问
	VERSION_DGCLIENT_USBSTOR_FORBID = 0x20000,	//移动存储禁用
	VERSION_DGCLIENT_ALWAYSSHOWTRAY = 0x40000,  //客户端总是显示托盘但不能控制
	VERSION_DGCLIENT_NOSHORTCUR		= 0x80000,	//不在桌面和程序创建快捷方式
	VERSION_DGADMIN_CHECKPASS		= 0x100000,	//检查密码的强度
	VERSION_DGADMIN_MIGUO			= 0x200000, //国密版本
	VERSION_RUNDLL_SHANGQI			= 0x400000,	//上海汽车特别版本
	VERSION_DGCRYPT_CHECKDOG		= 0x800000, 
	VERSION_RUNDLL_WORKLOG			= 0x1000000, 
	VERSION_LOGON_NOCASE			= 0x2000000, //登录时不区分大小写
	VERSION_NEW_UPDATE				= 0x4000000, //使用新的更新模块
	VERSION_OA_DECRYPT				= 0x8000000, //OA文档解密权限分离
	VERSION_RUNDLL_TRAYNODISPLAY	= 0x10000000, //开关客户端不显示托盘
	VERSION_ALL_MAIL				= 0x20000000, //开关客户端不显示托盘	= 0x10000000, //开关客户端不显示托盘
	VERSION_SPECIAL_OFFLINE			= 0x40000000, //离线加密失效
	VERSION_DGCLIENT_DEFAULTOPEN	= 0x80000000,	//客户端有开关客户端权限时默认开启状态
};


enum
{
	VERSION_DGCLIENT_CONTROLCONNECT	= 0x1,	//控制connect
	VERSION_CAN_USEDMALLOW			= 0x2,	//右键可进行DocMentor文件转换
	VERSION_CHANGE_CONFIGMODE		= 0x4,  //客户端切换配置模式
	VERSION_CHECK_DOGSERVER			= 0x8,  //客户端检验狗服务
	VERSION_SEMIROUTSIDE            = 0x10, //森马制定外发
	VERSION_DOCPLUS					= 0x20, //doc+版本
	VERSION_HIDEDGPATH				= 0x40, //客户端隐藏安装目录
	VERSION_HOF					    = 0x200, //doc+版本
    VERSION_LANDING_ENCRYPTION      = 0x400, //是否允许落地加密 
	VERSION_STARTUSENEWENCRYPTMODE	= 0x1000	//是否启用新加密方式
};

//华途发布标准版本
//#define DG_CURRENT_VERSION  VERSION_STANDARD_DAHENG | VERSION_DGCRYPT_PASSWORD

//#define DG_CURRENT_VERSION  VERSION_STANDARD_DAHENG | VERSION_DGCRYPT_PASSWORD

//协承昌
//#define DG_CURRENT_VERSION  VERSION_STANDARD_HUATU | VERSION_DGCRYPT_PASSWORD | VERSION_DGCRYPT_NETLOAD | VERSION_STANDARD_NETERRORTIME
//杭叉
//#define DG_CURRENT_VERSION  VERSION_STANDARD_HUATU | VERSION_DGCRYPT_PASSWORD | VERSION_DGCRYPT_NETLOAD | VERSION_UPDATE_BIGANT | VERSION_XINMEIXING
//派尼尔
//#define DG_CURRENT_VERSION  VERSION_STANDARD_HUATU | VERSION_DGCRYPT_PASSWORD | VERSION_DGCRYPT_NETLOAD | VERSION_UPDATE_BIGANT

//永安机模
//#define DG_CURRENT_VERSION  VERSION_STANDARD_HUATU | VERSION_DGCRYPT_PASSWORD | VERSION_DGCRYPT_NETLOAD | VERSION_UPDATE_BIGANT | VERSION_DGCLIENT_NOSHORTCUR

//京荣船舶
//#define DG_CURRENT_VERSION  VERSION_STANDARD_HUATU | VERSION_DGCRYPT_PASSWORD | VERSION_DGCRYPT_NETLOAD | VERSION_UPDATE_BIGANT
//测试

//#define DG_CURRENT_VERSION  VERSION_STANDARD_HUATU | VERSION_DGCRYPT_PASSWORD | VERSION_DGCRYPT_NETLOAD | VERSION_UPDATE_BIGANT

extern DWORD gCurrentConfig;
extern DWORD gCurrentConfigEx;



#define DG_RIGHT_MODULE //授权模块

/*inline int GetCurrentVersion()
{
	//既有华途又有北京,则以华途为准
	int version  = DG_CURRENT_VERSION;

	if ((version & VERSION_STANDARD_HUATU) 
		&& (version & VERSION_STANDARD_DAHENG))
	{
		return version - VERSION_STANDARD_DAHENG;
	}
	return DG_CURRENT_VERSION;
}*/

inline int GetDGServerCurrentVersion()
{
	if (gCurrentConfig == 0)
	{
		char buffer[1024] = {0};
		GetSystemDirectoryA(buffer,1023);
		char strConfigFile[1024] = {0};
		sprintf(strConfigFile,"%s\\%s",buffer,"config.ini");
		FILE* fp = fopen(strConfigFile,"r");
		if (fp)
		{
			fscanf(fp,"%x",&gCurrentConfig);
			fclose(fp);
		}	
	}
	
	if (gCurrentConfig == 0)
	{
		gCurrentConfig = VERSION_STANDARD_DAHENG | VERSION_DGCRYPT_PASSWORD;
	}
	return gCurrentConfig;
}

#include "atlbase.h"
#include "ShFolder.h"
#include "shlobj.h"

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

inline BOOL DGVIsWow64()
{
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(L"kernel32"),
		"IsWow64Process"
		);
	BOOL           bIsWow64 = FALSE;

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			// Error handle
			return FALSE;
		}
	}
	return bIsWow64;
}


inline int GetClientCurrentVersion()
{

	USES_CONVERSION;
	if (gCurrentConfig == 0)
	{
		TCHAR buffer[1024] = {0};
		TCHAR strConfigFile[1024] = {0};
		CRegKey key;
		if (key.Open(HKEY_LOCAL_MACHINE,TEXT("software\\{F4C09C2D-E206-75B7-2DBF-15DBC6957210}\\Client"),KEY_READ|KEY_WOW64_32KEY) != ERROR_SUCCESS)
		{
			if (key.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Client"),KEY_READ|KEY_WOW64_32KEY) != ERROR_SUCCESS)
			{
			}
		}
		DWORD length = 1024;
		key.QueryValue(buffer,_T("DGPath"),&length);
		_stprintf(strConfigFile,_T("%s\\%s"),buffer,_T("config.ini"));
				
		
		if(_tcslen(buffer) == 0)
		{
			SHGetSpecialFolderPath(NULL,buffer,CSIDL_PROGRAM_FILES,0);
			_stprintf(strConfigFile,_T("%s\\DG\\%s"),buffer,_T("config.ini"));
		}
		
		FILE* fp = _tfopen(strConfigFile,_T("r"));
		if (fp)
		{
			fscanf(fp,"%x",&gCurrentConfig);
			fclose(fp);
		}	
	}
	if (gCurrentConfig == 0)
	{
		gCurrentConfig = VERSION_STANDARD_DAHENG | VERSION_DGCRYPT_PASSWORD;
	}
	
	return gCurrentConfig;
}

inline int GetClientCurrentVersionEx()
{
	USES_CONVERSION;
	if (gCurrentConfigEx == 0)
	{
		TCHAR buffer[1024] = {0};
		TCHAR strConfigFile[1024] = {0};
		CRegKey key;
		if (key.Open(HKEY_LOCAL_MACHINE,TEXT("software\\{F4C09C2D-E206-75B7-2DBF-15DBC6957210}\\Client"),KEY_READ|KEY_WOW64_32KEY) != ERROR_SUCCESS)
		{
			if (key.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Client"),KEY_READ|KEY_WOW64_32KEY) != ERROR_SUCCESS)
			{
			}
		}
		DWORD length = 1024;
		key.QueryValue(buffer,_T("DGPath"),&length);
		_stprintf(strConfigFile,_T("%s\\%s"),buffer,_T("config.ini"));
        if(_tcslen(buffer) == 0)
        {
            SHGetSpecialFolderPath(NULL,buffer,CSIDL_PROGRAM_FILES,0);
            _stprintf(strConfigFile,_T("%s\\DG\\%s"),buffer,_T("config.ini"));
        }
		gCurrentConfigEx = GetPrivateProfileInt(_T("ConfigEx"), _T("Config"), 0, strConfigFile);
	}
	return gCurrentConfigEx;
}



//华途(大恒)标准版本
/*
	加密锁标
	右键加解密菜单
	U盘插入不自动加密
不同:
    创建的快捷方式
	登录图片
	关于对话框
*/

//双友版本
/*
	客户端无加密锁标
	无右键菜单
	U盘插入自动加密
	客户端不需要登录器
	生成客户端安装盘是直接结合用户名和密码
*/

//新美星版本
/*
	增加解密管理员(仅此管理员可以进行解密操作)
*/

//域运行模式
/*
	管理端具有域用户导入功能
	客户端直接在本地验证登录用户,不需要连接口令
	LogonUser
*/
//