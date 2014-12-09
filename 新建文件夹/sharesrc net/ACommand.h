// ACommand.h: interface for the CACommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACOMMAND_H__29A7B0EF_E9B4_44A9_81F2_F5425FEB92BF__INCLUDED_)
#define AFX_ACOMMAND_H__29A7B0EF_E9B4_44A9_81F2_F5425FEB92BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAString;
class CADateTime;
class CAVariantConv;

class CAdoCommand;
class CARecordset;
class CAdoParameter;
class CAdoParameters;
class CAParameters;
class CAParameter;
class CAConnect;


#include "AdbcCommDef.h"

//////////////////////////////////////////////////////
//
// 注意：使用前先用SetActiveConnection连接CAConnect
// 
//////////////////////////////////////////////////////

class CACommand  
{
public:	
	CAdoCommand		*m_pAdoCommand;
	CAConnect		*m_pAConnect;
public:
	CACommand();
	virtual ~CACommand();
	CACommand( CAConnect &AConnect, CAString strCommandText = _T(""), enum AdbcCommandType AdbcCommandType = adbcCmdStoredProc );

public:	
	//=================================================
	// 执行在 CommandText 属性中所指定的 SQL 指令或程序
	//=================================================
	bool	Execute( CAString strCommandText = _T(""), enum AdbcCommandType Options = adbcCmdText );
	bool	Execute( VARIANT *RecordsAffected, VARIANT *Parameters, enum AdbcCommandType Options );
	//=================================================
	// 处理存储过程
	//=================================================
	bool	ClearParameter();
	bool	DeleteParameter(const _variant_t & Index );
	bool	RunStoredProc( CAString strStoredName );
	bool	AddParameter(CAString strName, enum AdbcDataType Type, 
						 enum AdbcParaDirection Direction, long lSize, 
						 const _variant_t & Value = vtMissing);
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const CAString &Value = _T("") );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const double &Value = 0.0 );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const float &Value = 0.0 );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const long &Value = 0L );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const int &Value = 0 );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const BYTE &Value = 0 );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const char &Value = '\0' );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const DWORD &Value = 0 );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const UINT &Value = 0 );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const bool &Value = false );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const CADateTime &Value );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const CURRENCY &Value );
	bool	AddParameter( CAString strName, enum AdbcDataType Type,
						 enum AdbcParaDirection Direction, long lSize, 
						 const short &Value = 0 );

	bool	GetParamValue(CAString strName, CAString &strValue);
	bool	GetParamValue(CAString strName, int &nValue);
	bool	GetParamValue(CAString strName, bool &bValue);
	bool	GetParamValue(CAString strName, VARIANT &varValue);

	//=================================================
	// 在 Parameters 物件集合中建立一个新的 Parameter 物件
	//=================================================
	CAParameter*	CreateParameter( CAString strName, enum AdbcDataType Type,
									 enum AdbcParaDirection Direction, long lSize, 
									 const _variant_t &Value = vtMissing );

	bool			Append( CAParameter *pAParameter );

	CAParameter*	GetParameter (long nIndex);
	CAParameter*	GetParameter(CAString strName);
	
	void			Release();
	CAdoCommand*	GetCommand();

	bool			Cancel();
	_variant_t		GetValue( long index );
	_variant_t		GetValue( CAString strName );

	//=================================================
	// 物件也可以指定一个名称
	//=================================================
	CAString			GetName();
    void			SetName( CAString strName );

	//=================================================
	// 属性中所指定的命令类型
	//=================================================
	void			SetCommandType ( enum AdbcCommandType plCmdType );
    enum AdbcCommandType GetCommandType ();

	//=================================================
	// 连接数据库
	//=================================================
	bool			SetConnection( CAConnect *pAConnect );
	void			SetActiveConnection( CAConnect *pAConnect );
	CAConnect*		GetActiveConnection();

	//=================================================
	// 是否要在执行前建立一个敘述
	//=================================================
	bool			GetPrepared();
	void			SetPrepared(bool bPrepared);

	CAParameters*	GetParameters ();
	
	//=================================================
	// 执行一个指令、或该指令传回一个错误讯息前，
	// 所需要的等待时间（单位：分钟）。 
	//=================================================
	long			GetCommandTimeout();
	void			SetCommandTimeout(long pl);

	//=================================================
	// 一段代表所要执行指令的文字
	//=================================================
	CAString			GetCommandText();
	void			SetCommandText(CAString strCommandText);

	//=================================================
	// 用来确认目前的 Command 物件是开启或关闭的状态。
	// 例如：如果是开启，则回传值是 adStateOpen；
	// 反之则是 adStateClosed。 
	//=================================================
	long			GetState();

	CAParameter		operator [](int index);
	CAParameter		operator [](CAString strParamName);
};

#endif // !defined(AFX_ACOMMAND_H__29A7B0EF_E9B4_44A9_81F2_F5425FEB92BF__INCLUDED_)
