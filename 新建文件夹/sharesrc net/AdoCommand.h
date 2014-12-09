// AdoCommand.h: interface for the CAdoCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOCOMMAND_H__77B202F4_4F72_4B55_8E39_898AB263E293__INCLUDED_)
#define AFX_ADOCOMMAND_H__77B202F4_4F72_4B55_8E39_898AB263E293__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAString;

class CAdoConnect;
class AdoRecordset;

class CAdoParameter;
class CAdoParameters;

#include "AdbcCommdef.h"


class CAdoCommand  
{
public:
	CAdoCommand();
	virtual ~CAdoCommand();
	CAdoCommand(CAdoConnect &AdoConnect, CAString strCommandText = _T(""), enum AdbcCommandType AdbcCommandType = adbcCmdStoredProc);

protected:
	ADODB::_CommandPtr		m_ptrCommand;
	CAdoConnect		*m_pAdoConnect;

	void			Release();

public:	
	ADODB::_CommandPtr&	GetCommand();
	bool			SetConnection(CAdoConnect *pAdoConnect);

	//=================================================
	// 执行在 CommandText 属性中所指定的 SQL 指令或程序
	//=================================================
	bool  Execute(CAString strCommandText = _T(""), 
				  enum AdbcCommandType Options = adbcCmdText);
	bool  Execute(VARIANT *RecordsAffected, 
				  VARIANT *Parameters, 
				  enum AdbcCommandType Options );

	//=================================================
	// 处理存储过程
	//=================================================
	bool	ClearParameter();
	bool	DeleteParameter(const long nIndex );
	bool	AddParameter(CAString strName, enum AdbcDataType Type, 
						 enum AdbcParaDirection Direction, long lSize, 
						 const _variant_t & Value = vtMissing);
	bool	GetParamValue(CAString strName, VARIANT &varValue);

	//=================================================
	// 在 Parameters 物件集合中建立一个新的 Parameter 物件
	//=================================================
	CAdoParameter*	CreateParameter( CAString strName, enum AdbcDataType Type, 
									 enum AdbcParaDirection Direction, long lSize, 
									 const _variant_t & Value = vtMissing);
	bool			Cancel();

	_variant_t		GetValue(long index);
	_variant_t		GetValue(LPCTSTR lpstrName);
	bool			GetValue(LPCTSTR lpstrName, VARIANT &vt);

	//=================================================
	// 物件也可以指定一个名称
	//=================================================
    CAString			GetName();
    void			SetName(CAString strName);

	//=================================================
	// 属性中所指定的命令类型
	//=================================================
	void			SetCommandType (enum AdbcCommandType plCmdType);
    enum AdbcCommandType GetCommandType ();

	//=================================================
	// 连接数据库
	//=================================================
	void			SetActiveConnection(CAdoConnect *pAdoConnect);
	CAdoConnect*	GetActiveConnection();

	bool			GetPrepared();
	void			SetPrepared(bool bPrepared);

	//=================================================
	// 是否要在执行前建立一个敘述
	//=================================================
	CAdoParameter*	GetParameter (long nIndex);
	CAdoParameter*	GetParameter(LPCTSTR lpstrName);
	bool			Append(CAdoParameter *pAdoParameter);

	CAdoParameters*	GetParameters();

	//=================================================
	// 执行一个指令、或该指令传回一个错误讯息前，
	// 所需要的等待时间（单位：分钟）。 
	//=================================================
	long			GetCommandTimeout();
	void			SetCommandTimeout(long lTime);

	//=================================================
	// 用来确认目前的 Command 物件是开启或关闭的状态。
	// 例如：如果是开启，则回传值是 adStateOpen；
	// 反之则是 adStateClosed。 
	//=================================================
	long			GetState();

	//=================================================
	// 一段代表所要执行指令的文字
	//=================================================
	CAString		GetCommandText();
	void			SetCommandText(CAString strCommandText);
	
	CAdoParameter	operator [](int index);
	CAdoParameter	operator [](LPCTSTR lpszParamName);
};

#endif // !defined(AFX_ADOCOMMAND_H__77B202F4_4F72_4B55_8E39_898AB263E293__INCLUDED_)
