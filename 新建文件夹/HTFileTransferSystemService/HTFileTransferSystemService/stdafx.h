// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif
#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include "AString.h"
#define BUFF_SIZE 4096
#define XML_SIZE 102400

using namespace ATL;

#define  MAXCOUNT 8

#define HTP_H3			 0
#define HTP_H5			 1
#define HTP_H7			 2
#define HTP_T3			 3

#define HTV_H3_Standard	 1
#define HTV_H3_Group	 2

#define HTM_LevelMgr	 0x1
#define HTM_DGFlow		 0x2
#define HTM_Authorize	 0x4
#define HTM_Outside		 0x8

#define  ERR_NODOG 300 //狗不存在
#define  ERR_OLDOPENNEWDOG 301//老的接口打开新的狗
#define  ERR_NOTVTDOG 302//不是我们的狗 
#define  ERR_WRITENODATE 303//写入的时候没有数据
#define  ERR_OLDWRITENEWDOG 304//老的接口写入4.6类型的数据
#define  ERR_UNKNOWDOG 305//未知狗的类型
#define  ERR_INVALIDKEY 306//无效的key
#define  ERR_INVALIDXML 307//无效的XML
#define	 ERR_INVALIDDOGID 308//无效的DOG ID
#define  ERR_INVALIDDOGVERSION 309//无效的Dog Version
#define  ERR_NEWWRITEOLD 310//用新的接口去写老狗
#define  ERR_WRITENEWDOGWITHOLDDOGVERSION 311//写新狗的时候，却使用了旧的DogVersion
#define  ERR_NEWOPENOLDDOG 312//新的接口打开老的狗
#define  ERR_OPENLICFAILED 313//打开授权文件失败
#define  ERR_CREATELICFAILED 314//创建授权文件失败
#define  ERR_INVALIDLICINFO 315//无效的软授权数据
#define  ERR_INVALIDPARM 400//无效的参数

struct stDogInfo2
{
	BYTE btFlag[10];//锁标识（默认为vamtoo）
	DWORD dwDogVersion;//锁版本（默认为1）
	DWORD dwDogID;//狗号
	BYTE  btKey[128];//密钥
	WORD  dwProduct[MAXCOUNT];//产品数（如dwModule [0] = 1表示可用DocGuarder）
	INT nCount[MAXCOUNT];//节点数（如nCount [0] = 100表示产品DocGuarder节点数100，-1表示无限制）
	SYSTEMTIME	tTimeOver[MAXCOUNT];//产品到期时间（如tTimeOver[0]表示DocGuarder的到期时间，全0表示无限制）
	DWORD dwTactics[MAXCOUNT];//产品到期策略 0：无策略、0x1：策略一、0x2：策略二......
	DWORD dwModuleFlag[MAXCOUNT];//模块标识（如：审批0x1外发0x2授权加密0x4）
	FLOAT nProductViersion[MAXCOUNT];//支持的产品版本
	BYTE btGuid[38];//GUID字符串
	INT	nReserve;//保留字段
};