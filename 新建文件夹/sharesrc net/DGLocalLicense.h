#pragma once
#include "ErrorRecorder.h"
/*
* 摘　　要：
对离网处理进行封装
离网优先级： 时钟锁<--软离网<-故障
*/

typedef BOOL (_cdecl *DGLL_Init)(CErrorRecorder &cr);
typedef BOOL (_cdecl *DGLL_UnInit)(CErrorRecorder &cr);
typedef BOOL (_cdecl *DGLL_CheckLicense)(BOOL& bDefaultLogin,CErrorRecorder &cr);
typedef BOOL (_cdecl *DGLL_OnFlashClock)(LPTSTR lpLicPath, CErrorRecorder &cr);
typedef BOOL (_cdecl *DGLL_GetOfflineInfo)(int& nMode, int& nLeftTime, CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLL_OnOffline)(LPTSTR lpLicPath,CErrorRecorder& cr);
typedef BOOL (_cdecl *DGLL_OnBackNet)(CErrorRecorder &cr);
typedef BOOL (_cdecl *DGLL_OnStoppageTime)(int nTime, CErrorRecorder& cr);


class CDGLocalLicense
{
public:
	CDGLocalLicense(void);
	~CDGLocalLicense(void);
	/*
	函数功能:
	离网模块初始化
	参数:
	cr		:错误集对象
	返回值:
	true 初始化成功
	false 
	*/
	BOOL Init(CErrorRecorder &cr);

	/*
	函数功能:
	离网模块反初始化
	参数:
	cr		:错误集对象
	返回值:
	true 反初始化成功
	false 
	*/
	BOOL UnInit(CErrorRecorder &cr);

	/*
	函数功能:
	检查离网模块
	参数:
	cr		:错误集对象
	返回值:
	true 可以进入离网状态
	false 找不到离网信息
	*/
	BOOL CheckLicense(BOOL& bDefaultLogin,CErrorRecorder &cr);

	/*
	函数功能:
	刷新时钟锁
	参数:
	cr		:错误集对象
	返回值:
	true 刷新时钟锁成功
	false 
	*/
	BOOL OnFlashClock(LPTSTR lpLicPath, CErrorRecorder &cr);

	/*
	函数功能:
	获取离网的方式和剩余时间
	参数:
	cr		:错误集对象
	返回值:
	true 获取离网信息成功
	false 
	*/
	BOOL GetOfflineInfo(int& nMode, int& nLeftTime, CErrorRecorder& cr);

	/*
	函数功能:
	脱离网络
	参数:
	cr		:错误集对象
	返回值:
	true 脱离网络成功
	false 
	*/
	BOOL OnOffline(LPTSTR lpLicPath,CErrorRecorder& cr);

	/*
	函数功能:
	回到网络
	参数:
	cr		:错误集对象
	返回值:
	true 回到网络成功
	false 
	*/
	BOOL OnBackNet(CErrorRecorder& cr);

	/*
	函数功能:
	增加离网时间
	参数:
	cr		:错误集对象
	返回值:
	true 增加离网时间成功
	false 
	*/
	BOOL OnStoppageTime(int nTime, CErrorRecorder& cr);

private:
	void InitFunction(HMODULE hInst);

	HMODULE				m_hInst;
	DGLL_Init			m_pInit;
	DGLL_UnInit			m_pUnInit;
	DGLL_CheckLicense	m_pCheckLicense;
	DGLL_OnFlashClock	m_pOnFlashClock;
	DGLL_GetOfflineInfo m_pGetOfflineInfo;
	DGLL_OnOffline		m_pOnOffline;
	DGLL_OnBackNet		m_pOnBackNet;
	DGLL_OnStoppageTime	m_pOnStoppageTime;
};
