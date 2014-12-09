// UpdateObject.h: interface for the CUpdateObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATEOBJECT_H__A6566974_2E8A_4C5C_96C8_42A60C8EDF1E__INCLUDED_)
#define AFX_UPDATEOBJECT_H__A6566974_2E8A_4C5C_96C8_42A60C8EDF1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>

using namespace std;

struct UpdateMachine
{
	LPTSTR strMachineName;
	BOOL   bIsUpdated;
};

typedef deque<UpdateMachine*> MACHINE_LIST;

enum
{
	OPER_UNINSTALL = 1,
	OPER_UPDATEFILE = 2,
};

class CUpdateObject  
{
public:
	CUpdateObject();
	virtual ~CUpdateObject();

	int m_nUpdateType;

	LPTSTR m_strUpdateFileName;					//更新的文件名

	MACHINE_LIST m_dequeMachine;				//需要更新的用户机器名
};

#endif // !defined(AFX_UPDATEOBJECT_H__A6566974_2E8A_4C5C_96C8_42A60C8EDF1E__INCLUDED_)
