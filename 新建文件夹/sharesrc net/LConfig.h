#pragma once
/*
* Copyright (C), 2009-2012, Huatusoft Tech. Co., Ltd.
* All rights reserved.
*
* 文件名称 LConfig.h
* 文件标识：
* 摘　　要：
*

* 当前版本：1.0
* 原作者　：wmk
* 完成日期：2012年10月23日
* 修订记录：创建
*/

//#include "DGVersion.h"
#include "ErrorRecorder.h"
class CLConfig
{
public:
	CLConfig(void);
	~CLConfig(void);
	/*
	函数功能:
	判断是否为域用户登录
	参数:
	cr		:错误集对象
	返回值:
	true	域用户登录
	false	非域用户登录
	宏：
	VERSION_RUNDLL_SHANGQI			= 0x400000,	//计算机使用当前域用户登录
	*/
	BOOL IsDomainMode(CErrorRecorder &cr);

	/*
	函数功能：
	判断客户端是否自动下载配置
	参数：
	cr		:错误集对象
	返回值：
	true	客户端自动下载配置
	false	客户端不自动下载配置
	宏：
	VERSION_MAIL_MEASURE			= 0x8000,	//客户端自动下载配置
	*/
	BOOL IsAutoDownloadCfg(CErrorRecorder &cr);

	/*
	函数功能：
	判断关闭客户端时是否对加密文件拒绝访问
	参数：
	cr		:错误集对象
	返回值：
	true	关闭客户端时对加密文件拒绝访问
	false	关闭客户端时对加密文件允许访问
	宏：
	VERSION_RUNDLL_ACCESS			= 0x10000,	//关闭客户端时对加密文件拒绝访问
	*/
	BOOL IsDeniedAccessEncrypteFile(CErrorRecorder &cr);

	/*
	函数功能：
	判断关闭客户端是否切换配置模式
	参数：
	cr		:错误集对象
	返回值：
	true	客户端切换配置模式
	false	
	宏：
	VERSION_CHANGE_CONFIGMODE		= 0x4,  //客户端切换配置模式
	*/
	BOOL IsChangeCFGNormal(CErrorRecorder &cr);

	/*
	函数功能：
	获取客户端有开关客户端权限时的默认状态
	参数：
	cr		:错误集对象
	返回值：
	true	客户端有开关客户端权限时默认开启状态
	false	客户端有开关客户端权限时默认关闭状态
	宏：
	VERSION_DGCLIENT_DEFAULTOPEN	= 0x80000000,	//客户端有开关客户端权限时默认开启状态
	*/
	BOOL IsDGClientDefaultOpen(CErrorRecorder &cr);

	/*
	函数功能：
	判断开关客户端是否显示托盘
	参数：
	cr		:错误集对象
	返回值：
	true	开关客户端不显示托盘
	false	开关客户端显示托盘
	宏：
	VERSION_RUNDLL_TRAYNODISPLAY	= 0x10000000, //开关客户端不显示托盘
	*/
	BOOL IsTrayNoDisplay(CErrorRecorder &cr);

	/*
	函数功能：
	判断非域模式登录是否使用开关客户端
	参数：
	cr		:错误集对象
	返回值：
	true	非域模式登录使用开关客户端
	false	非域模式登录不使用开关客户端
	宏：
	VERSION_DGCLIENT_OPENCLOSE		= 0x2000,	//非域模式登录使用开关客户端(云南)
	*/
	BOOL IsNonDomainModeOpenClose(CErrorRecorder &cr);

	/*
	函数功能：
	判断客户端是否总是显示托盘
	参数：
	cr		:错误集对象
	返回值：
	true	客户端总是显示托盘但不能控制
	false	
	宏：
	VERSION_DGCLIENT_ALWAYSSHOWTRAY = 0x40000,  //客户端总是显示托盘但不能控制
	*/
	BOOL IsAlwaysShowTray(CErrorRecorder &cr);

	/*
	函数功能：
	判断移动存储是否禁用
	参数：
	cr		:错误集对象
	返回值：
	true	移动存储禁用
	false	
	宏：
	VERSION_DGCLIENT_USBSTOR_FORBID = 0x20000,	//移动存储禁用
	*/
	BOOL IsUsbForbid(CErrorRecorder &cr);

	/*
	函数功能：
	判断是否对U盘进行全盘扫描
	参数：
	cr		:错误集对象
	返回值：
	true	对U盘进行全盘扫描
	false	不对对U盘进行全盘扫描
	宏：
	VERSION_STANDARD_USBSTOR		= 0x20,		//是否对U盘进行全盘扫描
	*/
	BOOL IsFullScanUsb(CErrorRecorder &cr);



};

