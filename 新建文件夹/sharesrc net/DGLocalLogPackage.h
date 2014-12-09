#pragma once

/*
* 摘　　要：
DGLocalLogRecorder.dll接口封装

客户端调用日志模块说明:
需要先调用InitClientUnit()
服务端调用日志模块说明:
需要先调用InitSvrUnit()
使用结束后需要调用UninitSvrUnit();
*/
#include "ErrorRecorder.h"
#include "DGStruct.h"

typedef BOOL (_cdecl *DGLLog_InitClientUnit)(CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLLog_SendLog)(int nType, char* pData, int nLen, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLLog_InitSvrUnit)(CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLLog_UninitSvrUnit)();


class CDGLocalLogPackage
{
public:
	CDGLocalLogPackage(void);

	~CDGLocalLogPackage(void);

	/*
	函数功能:
	日志客户端模块初始化
	参数:
	cr		:错误集对象
	返回值:
	true 初始化成功
	false 
	*/
	BOOL InitClientUnit(CErrorRecorder& cr);

	
	/*
	函数功能:
	日志服务端初始化
	参数:
	cr		:错误集对象
	返回值:
	true 初始化成功
	false 
	*/
	BOOL InitSvrUnit(CErrorRecorder& cr);

	/*
	函数功能:
	向服务发送日志数据
	参数:
	nType		:参考DGStruct.h DG_IPC_LOG_TYPE枚举类型
	pData		:日志数据
	nLen		:数据长度
	返回值:
	true 初始化成功
	false 
	*/
	BOOL SendLog(int nType, char* pData, int nLen, CErrorRecorder& cr);



	/*
	函数功能:
	日志服务端反初始化
	参数:
	cr		:错误集对象
	返回值:
	true 反初始化成功
	false 
	*/
	BOOL UninitSvrUnit(CErrorRecorder &cr);


	
private:
	void InitFunction();

	HMODULE						m_hInst;

	DGLLog_InitClientUnit		m_pInitClientUnit;	
	DGLLog_SendLog				m_pSendLog;
	DGLLog_InitSvrUnit			m_pInitSvrUnit;
	DGLLog_UninitSvrUnit		m_pUninitSvrUnit;
};
