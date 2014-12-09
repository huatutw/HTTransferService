#pragma once
#include "ErrorRecorder.h"
#include "DGStruct.h"

// struct Outside_CheckDog
// {
// 	DWORD dwLable;
// 	TCHAR strCompanyMD5[40];
// 	BYTE  Key[128];				//加密密钥
// 	DWORD DogID;				//狗号
// };

//T3 Business Mode
typedef BOOL (_cdecl *T3BM_MainTestT3Service)(CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_TestT3Service)(CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_GetXmlInfoFromServ)(int nSize, LPTSTR lpData,LPTSTR lpXmlPath,CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_ApplyAuthorization)(int nSize, LPTSTR lpData,LPTSTR lpXmlPath,CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_GetUserListfoFromServ)(LPTSTR lpData,LPTSTR lpXmlPath,CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_LoginDVSystem)(LPSTR lpSvrIp, DWORD dwSvrPort,T3_DBS_LoginInfo t3LoginInfo,T3_DBS_RTLoginInfo& t3RtnLoginInfo,  CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_CheckSetupInfo)(LPSTR lpSvrIp, DWORD dwSvrPort,T3_DBS_SetupInfo t3SetupInfo,T3_DBS_RTSetupInfo& t3RtnSetupInfo, CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_CheckFilePermission)(T3_DBS_CheckFileInfo t3FilePermission, T3_DBS_RTCheckFileInfo& t3RtnFilePermission, CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_CheckFilePassword)(T3_DBS_CheckFileInfo t3CheckInfo, T3_DBS_RTCheckFileInfo& t3RtnData, CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_OpenT3File)(T3_DBS_CheckFileInfo t3FilePermission,T3_DBS_RTCheckFileInfo& t3RtnData, CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_GetT3FileInfo)(LPCTSTR lpstrPath, int& nPermission, BOOL& bSetWatermark, CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_GetT3FileRemainDate)(LPCTSTR lpstrPath, LONGLONG& lRemainTime, CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_ReadFileInfoFromIni)(LPCTSTR lpstrPath, int& nPermission, CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_GetDogData)(DWORD& dwDogID, char* Key, CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_GetFileInfoFromServ)(T3_DBS_CheckFileInfo t3FileInfo, LPTSTR lpXmlPath, CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_GetWatermarkState)(LPCTSTR lpstrPath, CErrorRecorder& cr);
typedef BOOL (_cdecl *T3BM_GetAccreditInfo)(Outside_CheckDog& DogInfo);

class CT3Business
{
public:
	CT3Business(void);
	~CT3Business(void);

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
	

	BOOL MainTestT3Service(CErrorRecorder& cr);
	/*
	函数功能:
	测试外发服务
	true 服务运行正常
	false 服务不正常运行
	*/
	BOOL TestT3Service(CErrorRecorder& cr);

	/*
	函数功能：
	从服务器接收公司和账号信息
	*/

	BOOL GetUserListfoFromServ(LPTSTR lpData,LPTSTR lpXmlPath,CErrorRecorder& cr);

		/*
	函数功能：
	上传加密狗绑定信息到服务器
	*/

	BOOL ApplyAuthorization(int nSize, LPTSTR lpData,LPTSTR lpXmlPath,CErrorRecorder& cr);


	/*
	函数功能：
	上传文件信息，并从服务器获取文件ID
	*/
	BOOL GetXmlInfoFromServ(int nSize, LPTSTR lpData,LPTSTR lpXmlPath,CErrorRecorder& cr);


	/*
	函数功能：
	登陆服务器，通过返回值t3RtnLoginInfo判断是否正确登陆和DV剩余安装次数
	*/
	BOOL LoginDVSystem(LPSTR lpSvrIp, DWORD dwSvrPort,T3_DBS_LoginInfo t3LoginInfo,T3_DBS_RTLoginInfo& t3RtnLoginInfo,  CErrorRecorder& cr);

	/*
	函数功能：
	DV安装完成后向服务器发送指令，服务器修改剩余的安装次数，返回0成功，1失败
	*/
	BOOL CheckSetupInfo(LPSTR lpSvrIp, DWORD dwSvrPort,T3_DBS_SetupInfo t3SetupInfo,T3_DBS_RTSetupInfo& t3RtnSetupInfo,CErrorRecorder& cr);

	/*
	函数功能:
	打开文件时获取文件权限信息，是否需要密码验证，是否可以打开，以及文件打印、修改权限
	*/
	BOOL CheckFilePermission(T3_DBS_CheckFileInfo t3FilePermission, T3_DBS_RTCheckFileInfo& t3RtnFilePermission,CErrorRecorder& cr);
	
	/*
	函数功能:
	在线打开文件时获输入校验密码
	*/
	BOOL CheckFilePassword(T3_DBS_CheckFileInfo t3CheckInfo, T3_DBS_RTCheckFileInfo& t3RtnData, CErrorRecorder& cr);

	/*
	函数功能:
	打开文件时获取文件权限信息，是否需要密码验证，是否可以打开，以及文件打印、修改权限，如果需要密码，
	弹出密码输入对话框，发送密码到服务器校验
	返回值:
	t3RtnFilePermission.bNeedCheckPass 0不需要密码验证
	t3RtnFilePermission.nRet  0正常 1 文件权限过期 2无法查到权限信息 3.打开次数用尽 4 密码错误 5账号不存在
	*/
	BOOL OpenT3File(T3_DBS_CheckFileInfo t3FilePermission,T3_DBS_RTCheckFileInfo& t3RtnData, CErrorRecorder& cr);

	/*
	函数功能：
	打开文件时，如果是本地验证，从ini文件读取权限判断能否打开，并在打开后修改ini文件中的值
	*/
	BOOL ReadFileInfoFromIni(LPCTSTR lpstrPath,int& nPermission, CErrorRecorder& cr);

	/*
	函数功能：
	获取文件权限，enum{Read = 0x10,Write =0x20，print = 0x40,};
	*/
	BOOL GetT3FileInfo(LPCTSTR lpstrPath, int& nPermission, BOOL& bSetWatermark, CErrorRecorder& cr);

	BOOL GetT3FileInfo(LPCTSTR lpstrPath, LONGLONG& lRemainTime, CErrorRecorder& cr);

	/*
	函数功能：
	通过注册表，读取狗号和密钥
	*/
	BOOL GetDogData(DWORD& dwDogID, char* Key, CErrorRecorder& cr);

	/*
	函数功能：
	连接服务器，获取外发文件的具体信息
	*/
	BOOL GetFileInfoFromServ(T3_DBS_CheckFileInfo t3FileInfo, LPTSTR lpXmlPath, CErrorRecorder& cr);


	BOOL GetWatermarkState(LPCTSTR lpstrPath, CErrorRecorder& cr);

	BOOL GetAccreditInfo(Outside_CheckDog& DogInfo);

private:
	void InitFunction(HMODULE hInst);

	HMODULE						m_hInst;
	T3BM_MainTestT3Service		m_pMainTestT3Service;
	T3BM_TestT3Service			m_pTestT3Service;
	T3BM_GetXmlInfoFromServ		m_pGetXmlInfoFromServ;
	T3BM_ApplyAuthorization		m_pApplyAuthorization;
	T3BM_GetUserListfoFromServ	m_pGetUserListfoFromServ;
	T3BM_LoginDVSystem			m_pLoginDVSystem;
	T3BM_CheckSetupInfo			m_pCheckSetupInfo;
	T3BM_CheckFilePermission	m_pCheckFilePermission;
	T3BM_CheckFilePassword		m_pCheckFilePassword;
	T3BM_OpenT3File				m_pOpenT3File;	
	T3BM_ReadFileInfoFromIni	m_pReadFileInfoFromIni;
	T3BM_GetT3FileInfo			m_pGetT3FileInfo;
	T3BM_GetT3FileRemainDate	m_pGetT3FileRemainDate;
	T3BM_GetDogData				m_pGetDogData;
	T3BM_GetAccreditInfo		m_pGetAccreditInfo;
	T3BM_GetFileInfoFromServ	m_pGetFileInfoFromServ;
	T3BM_GetWatermarkState		m_pGetWatermarkState;
	
};
