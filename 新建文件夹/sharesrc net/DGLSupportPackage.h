#pragma once
#include "ErrorRecorder.h"
#include <string>
using namespace std;
#include "AString.h"
#pragma  comment(lib,"ws2_32.lib")

#ifndef LEN_NAME
#define LEN_NAME 128
#endif
struct DG_Approval
{
	int nType;
	int nTempID;
	TCHAR strTempName[LEN_NAME];
};

typedef std::deque<DG_Approval> deqDGApproval;
#include "DGStruct.h"

typedef BOOL (_cdecl *DGLP_CheckUserPassword)(LPTSTR strUser,LPTSTR strPass,LPSTR lpIp,DWORD dwPort,CErrorRecorder& cr); 
typedef BOOL (_cdecl *DGLP_TestServer)(LPSTR lpIp,UINT uPort,LPTSTR lpRetStr ,CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_TestLogServer)(LPSTR lpIp,UINT uPort, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_TestValidateServer)(LPSTR lpIP,UINT uPort, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_TestUpdateServer)(LPSTR lpIP,UINT uPort, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_TestPermissionServer)(LPSTR lpIP,UINT uPort, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_SendData)(LPSTR lpIP,UINT uPort,LPTSTR lpCmd,LPVOID lpData, int nLen, CErrorRecorder& cr);
typedef int (_cdecl *DGLP_ModifyUserPass)(LPCTSTR strUser,LPCTSTR strPass,LPSTR lpIp,DWORD dwPort,LPTSTR strNewPass, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_CheckDogDefault)(LPSTR lpIP,LPTSTR lpLoginName,LPTSTR lpClientId,DWORD dwPort, Offline_Info2& outInfo, CErrorRecorder& cr); 
typedef BOOL (_cdecl *DGLP_CheckDogInfo)(LPTSTR strUser,LPTSTR strPass,LPSTR strIP, LPTSTR lpClientId,DWORD dwPort, BOOL& bConnect, Offline_Info2& outInfo, CErrorRecorder& cr);
typedef int (_cdecl *DGLP_GetOfflineInfo)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR strLoginName, CErrorRecorder& cr);
typedef int (_cdecl *DGLP_GetOfflineInfo1)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR strLoginName, LPTSTR lpCLientId, CErrorRecorder& cr);
typedef int (_cdecl *DGLP_RequestOutofNetWork)(LPSTR lpSvrIp, DWORD dwSvrPort, LPTSTR lpClientID,CErrorRecorder& cr);
typedef int	(_cdecl *DGLP_RequestOutofNetWork1)(LPSTR lpSvrIp, DWORD dwSvrPort, LPTSTR lpClientID,CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_GetSystemParameter)(LPSTR lpSvrIp, DWORD dwSvrPort,int &nFlushInfo, int &nCheckDog, int &nNetError ,CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_GetDGWebSetFile)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpPath, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_SendArrayData)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd,int nArraryCount,LPVOID lpArray, int nLen, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_CheckSoftOffline)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpUser, LPTSTR lpClientId,BOOL &bHaveSoftOffline, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_SetAlertsInfo)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpUser, BOOL bRemindOnce,LPTSTR lpMinute, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_UploadFileToServer)(LPSTR lpSvrIp, DWORD dwSvrPort,int nRecordType, LPTSTR lpPath, LPTSTR lpUserName, LPTSTR lpServerFilePath,CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_CheckOfflineInfo)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpClientId,BOOL bAddDogCount,LPTSTR lpUserName, int& nType,CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_SendArrayFiles)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd,int nArraryCount,wstring* strFiles, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_RecvUserCfgFile)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd, CAString strUserName, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_RecvNewestInitData)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd, CAString strUsrName, DG_Approval* deqDGApprovalList, int& nLen,CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_RecvDGLandingInitFile)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd,CAString strCommand,CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLP_CheckFirstLogin)(LPSTR lpSvrIp, DWORD dwSvrPort ,LPCTSTR lpUser, BOOL& bFirstLogin,CErrorRecorder &cr);
typedef BOOL (_cdecl *DGLP_SubmitFirstLogin)(LPSTR lpSvrIp, DWORD dwSvrPort ,LPCTSTR lpUser, CErrorRecorder &cr);
typedef BOOL (_cdecl *DGLP_RecvWarterMarkModule)(LPSTR lpSvrIp,DWORD dwSvrPort,CErrorRecorder &cr);
typedef BOOL (_cdecl *DGLP_GetModules)(LPSTR lpSvrIp, DWORD dwSvrPort ,DWORD& dwModules, CErrorRecorder &cr);
typedef BOOL (_cdecl *DGLP_SendDSMFiles)(LPSTR lpSvrIp, DWORD dwSvrPort ,LPTSTR pXmlPath, CErrorRecorder &cr);

typedef BOOL (_cdecl *DGLP_GetRoleCfgFile)(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd, CAString strUserName, CErrorRecorder& cr);

class CDGLSupportPackage
{
public:
	CDGLSupportPackage(void);
	~CDGLSupportPackage(void);

	/*
	函数功能：
	初始化Winsock服务
	*/
	BOOL InitNetwork(void);

	/*
	函数功能：
	注销Winsock服务
	*/
	BOOL UnInitNetwork(void);

	/*
	函数功能:
	向服务器验证用户密码是否正确
	参数:
	strUser :用户名
	strPass :用户密码
	lpIp	:服务器Ip
	dwPort	:服务器端口
	cr		:错误集对象
	返回值:
	true 密码正确
	false 密码错误
	*/
	BOOL CheckUserPassword(LPTSTR strUser,LPTSTR strPass,LPSTR lpIp,DWORD dwPort,CErrorRecorder& cr);

	/*
	函数功能:
	测试服务
	参数:
	lpIp	:服务器Ip
	dwPort	:服务器端口
	cr		:错误集对象
	lpRetStr: 服务器验证字符串
	返回值:
	true 服务运行正常
	false 服务不正常运行
	*/
	BOOL TestServer(LPSTR lpIp,UINT uPort, LPTSTR lpRetStr ,CErrorRecorder& cr);

	/*
	函数功能:
	测试日志服务
	参数:
	lpIp	:服务器Ip
	dwPort	:服务器端口
	cr		:错误集对象
	返回值:
	true 服务运行正常
	false 服务不正常运行
	*/
	BOOL TestLogServer(LPSTR lpIp,UINT uPort, CErrorRecorder& cr);
	/*
	函数功能:
	测试验证服务是否运行正常
	参数:
	lpIp	:服务器Ip
	dwPort	:服务器端口
	cr		:错误集对象
	返回值:
	true 服务运行正常
	false 服务不正常运行
	*/
	BOOL TestValidateServer(LPSTR lpIP,UINT uPort, CErrorRecorder& cr);
	/*
	函数功能:
	测试更新服务是否运行正常
	参数:
	lpIp	:服务器Ip
	dwPort	:服务器端口
	cr		:错误集对象
	返回值:
	true 服务运行正常
	false 服务不正常运行
	*/
	BOOL TestUpdateServer(LPSTR lpIP,UINT uPort, CErrorRecorder& cr);
	/*
	函数功能:
	测试权限服务是否运行正常
	参数:
	lpIp	:服务器Ip
	dwPort	:服务器端口
	cr		:错误集对象
	返回值:
	true 服务运行正常
	false 服务不正常运行
	*/
	BOOL TestPermissionServer(LPSTR lpIP,UINT uPort, CErrorRecorder& cr);
	/*
	函数功能:
	向服务发送数据.
	参数:
	lpIp	:服务器Ip
	dwPort	:服务器端口
	lpCmd	:通讯指令
	lpData	:数据指针
	nLen	:数据长度
	cr		:错误集对象
	*/
	BOOL SendData(LPSTR lpIP,UINT uPort,LPTSTR lpCmd,LPVOID lpData, int nLen, CErrorRecorder& cr);
	/*
	函数功能:
	修改用户名密码
	参数:
	strUser :用户名
	strPass :用户密码
	lpIp	:服务器Ip
	dwPort	:服务器端口
	strNewPass :新密码
	cr		:错误集对象
	*/
	int ModifyUserPass(LPCTSTR strUser,LPCTSTR strPass,LPSTR lpIp,DWORD dwPort,LPTSTR strNewPass, CErrorRecorder& cr);
		/*
	函数功能:
	验证默认登录名信息
	参数:
	lpLoginName :用户名
	lpIp	:服务器Ip
	dwPort	:服务器端口
	lpClientId :客户端id号
	outInfo	:获取结构信息
	cr		:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL CheckDogDefault(LPSTR lpIP,LPTSTR lpLoginName,LPTSTR lpClientId,DWORD dwPort, Offline_Info2& outInfo, CErrorRecorder& cr);
	/*
	函数功能:
	验证登陆用户登陆信息
	参数:
	lpLoginName :用户名
	strPass		:用户密码
	lpIp	:服务器Ip
	dwPort	:服务器端口
	lpClientId :客户端id号
	outInfo	:获取结构信息
	cr		:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL CheckDogInfo(LPTSTR strUser,LPTSTR strPass,LPSTR strIP, LPTSTR lpClientId,DWORD dwPort, BOOL& bConnect, Offline_Info2& outInfo, CErrorRecorder& cr);
	/*
	函数功能:
	获取离网信息
	*/
	int GetOfflineInfo(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR strLoginName, CErrorRecorder& cr);
	int GetOfflineInfo1(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR strLoginName, LPTSTR lpCLientId, CErrorRecorder& cr);
	/*
	函数功能:
	应答离网操作是否成功
	*/
	int RequestOutofNetWork(LPSTR lpSvrIp, DWORD dwSvrPort, LPTSTR lpClientID,CErrorRecorder& cr);
	int RequestOutofNetWork1(LPSTR lpSvrIp, DWORD dwSvrPort, LPTSTR lpClientID,CErrorRecorder& cr);
	/*
	函数功能:
	获取系统参数
	参数:
	nFlushInfo		:刷新配置间隔
	nCheckDog		:连接够服务间隔
	nNetError		:网络错误号
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL GetSystemParameter(LPSTR lpSvrIp, DWORD dwSvrPort,int &nFlushInfo, int &nCheckDog, int &nNetError ,CErrorRecorder& cr);
	/*
	函数功能:
	获取web配置文件
	参数:
	lpPath			:配置下载路径路径
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL GetDGWebSetFile(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpPath, CErrorRecorder& cr);
	/*
	函数功能:
	发送一个数组结构到服务器
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	lpCmd			:协议命令
	nArraryCount	:数组个数
	lpArray			:数组头地址
	nLen			:数组元素长度
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL SendArrayData(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd,int nArraryCount,LPVOID lpArray, int nLen, CErrorRecorder& cr);
	/*
	函数功能:
	监测是否软离网
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	lpUser			:登录名
	lpClientId		:客户端ID号
	bHaveSoftOffline:是否有传出参数
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL CheckSoftOffline(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpUser, LPTSTR lpClientId,BOOL &bHaveSoftOffline, CErrorRecorder& cr);
	/*
	函数功能:
	设置弹出消息设置
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	lpUser			:登录名
	bRemindOnce		:仅提醒一次
	lpMinute		:提醒间隔(分钟)
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL SetAlertsInfo(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpUser, BOOL bRemindOnce,LPTSTR lpMinute, CErrorRecorder& cr);
	/*
	函数功能:
	上传文件到服务器
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	nRecordType		:文件类型
	lpPath			:文件路径
	lpUserName		:登录名
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL UploadFileToServer(LPSTR lpSvrIp, DWORD dwSvrPort,int nRecordType, LPTSTR lpPath, LPTSTR lpUserName, LPTSTR lpServerFilePath,CErrorRecorder& cr);
	/*
	函数功能:
	使用文件离网时,向服务器校验信息
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	lpClientId		:客户端ID
	bAddDogCount	:
	lpUserName		:登录名
	nType			:校验结果类型
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL CheckOfflineInfo(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpClientId,BOOL bAddDogCount,LPTSTR lpUserName, int& nType,CErrorRecorder& cr);
	/*
	函数功能:
	向服务器批量发送文件
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	lpCmd			:协议命令
	nArraryCount	:数组个数
	strFiles		:路径数组头地址
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL SendArrayFiles(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd,int nArraryCount,wstring* strFiles, CErrorRecorder& cr);
	
		/*
	函数功能:
	获取用户CFG文件信息保存到Normal.cfg
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	lpCmd			:协议命令 "GetUserCfgFile2"
	strUserName		:用户名
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL RecvUserCfgFile(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd, CAString strUserName, CErrorRecorder& cr);

	/*
	函数功能:
	获取落地加密Ini信息
	参数:
	lpSvrIp			:更新服务器Ip
	dwSvrPort		:更新服务器端口
	lpCmd			:协议命令 "GetDGLandingIni"
	strCommand		:字符串
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL RecvDGLandingInitFile(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd,CAString strCommand,CErrorRecorder& cr);

	/*
	函数功能:
	生成ApprovalList.ini配置文件
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口+3
	lpCmd			:协议命令 "GetUpdateApprovalList"
	strUserName		:用户名
	deqDGApprovalList：存放接收信息的指针
	nLen			:DG_Approval类型数据的个数
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL RecvNewestInitData(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd, CAString strUsrName, DG_Approval* deqDGApprovalList, int& nLen,CErrorRecorder& cr);
	/*
	函数功能:
	判断用户是否是首次登陆
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	strUserName		:用户名
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL CheckFirstLogin(LPSTR lpSvrIp, DWORD dwSvrPort ,LPCTSTR lpUser, BOOL& bFirstLogin,CErrorRecorder &cr);

	/*
	函数功能:
	提交服务,用户已经首次登陆
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	strUserName		:用户名
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL SubmitFirstLogin(LPSTR lpSvrIp, DWORD dwSvrPort ,LPCTSTR lpUser, CErrorRecorder &cr);

		/*
	函数功能:
	提交服务,用户已经首次登陆
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL RecvWarterMarkModule(LPSTR lpSvrIp, DWORD dwSvrPort , CErrorRecorder &cr);

	
	/*
	函数功能:
	获取服务器所安装模块
	参数:
	lpSvrIp			:服务器Ip
	dwSvrPort		:服务器端口
	dwModules		:模块flag
	cr				:错误集对象
	返回值:
	ture成功,false有错误.
	*/
	BOOL GetModules(LPSTR lpSvrIp, DWORD dwSvrPort ,DWORD& dwModules, CErrorRecorder &cr);

	BOOL SendDSMFiles(LPSTR lpSvrIp, DWORD dwSvrPort ,LPTSTR pXmlPath, CErrorRecorder &cr);

	BOOL GetRoleCfgFile(LPSTR lpSvrIp, DWORD dwSvrPort,LPTSTR lpCmd, CAString strUserName, CErrorRecorder &cr);

private:
	void InitFunction(HMODULE hInst);

	HMODULE								m_hInst;
	DGLP_CheckUserPassword			m_pCheckUserPassword;
	DGLP_TestServer						m_pTestServer;
	DGLP_TestLogServer					m_pTestLogServer;
	DGLP_TestValidateServer			m_pTestValidateServer;
	DGLP_TestUpdateServer			m_pTestUpdateServer;
	DGLP_TestPermissionServer		m_pTestPermissionServer;
	DGLP_SendData						m_pSendData;
	DGLP_ModifyUserPass				m_pModifyUserPass;
	DGLP_CheckDogDefault			m_pCheckDogDefault;
	DGLP_CheckDogInfo					m_pCheckDogInfo;
	DGLP_GetOfflineInfo				m_pGetOfflineInfo;
	DGLP_GetOfflineInfo1				m_pGetOfflineInfo1;
	DGLP_RequestOutofNetWork		m_pRequestOutofNetWork;
	DGLP_RequestOutofNetWork1	m_pRequestOutofNetWork1;
	DGLP_GetSystemParameter		m_pGetSystemParameter;
	DGLP_GetDGWebSetFile			m_pGetDGWebSetFile;
	DGLP_SendArrayData				m_pSendArrayData;
	DGLP_CheckSoftOffline				m_pCheckSoftOffline;
	DGLP_SetAlertsInfo					m_pSetAlertsInfo;
	DGLP_UploadFileToServer			m_pUploadFileToServer;
	DGLP_CheckOfflineInfo				m_pCheckOfflineInfo;
	DGLP_SendArrayFiles				m_pSendArrayFiles;
	DGLP_RecvUserCfgFile				m_pRecvUserCfgFile;
	DGLP_GetRoleCfgFile				m_pGetRoleCfgFile;
	DGLP_RecvDGLandingInitFile		m_pRecvDGLandingInitFile;
	DGLP_RecvNewestInitData		m_pRecvNewestInitData;
	DGLP_CheckFirstLogin				m_pCheckFirstLogin;
	DGLP_SubmitFirstLogin				m_pSubmitFirstLogin;
	DGLP_RecvWarterMarkModule   m_pRecvWarterMarkModule;
	DGLP_GetModules					m_pGetModules;
	DGLP_SendDSMFiles					m_pSendDSMFiles;
};
