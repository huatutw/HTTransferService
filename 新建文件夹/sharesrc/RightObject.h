// RightObject.h: interface for the CRightObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIGHTOBJECT_H__C44AF987_5719_4701_BE6E_89936E077CB8__INCLUDED_)
#define AFX_RIGHTOBJECT_H__C44AF987_5719_4701_BE6E_89936E077CB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <map>

using namespace std;

enum
{
	RIGHT_NORMAL,		//基本权限
	RIGHT_LOG,			//日志权限
	RIGHT_FILE,			//文件权限
	RIGHT_RIGHT,		//授权权限
	RIGHT_SPEC,			//特殊权限
	RIGHT_MAIL,			//邮件权限
};

class CRightObject  
{
public:
	CRightObject();
	virtual ~CRightObject();
	
	TCHAR m_strRightName[32];   //权限名称
	TCHAR m_strRightDes[256];	//描述
	DWORD m_dwFeature;			//权限特征
	DWORD m_dwPara;				//权限参数	3.0版本保留此参数
	int	  m_nRightType;			//权限类型
};

typedef map<int,CRightObject*>  MAP_RIGHT;

#endif // !defined(AFX_RIGHTOBJECT_H__C44AF987_5719_4701_BE6E_89936E077CB8__INCLUDED_)
