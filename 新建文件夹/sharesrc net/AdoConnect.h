// AdoConnect.h: interface for the CAdoConnect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOCONNECT_H__107036CD_353B_45E1_A7DA_6C8686459181__INCLUDED_)
#define AFX_ADOCONNECT_H__107036CD_353B_45E1_A7DA_6C8686459181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdbcCommDef.h"

class CAdoRecordset;
class CAdoErrors;
class CAdoError;
class CAString;

class CAError;
class CAErrors;

class CAdoConnect  
{
public:
	CAdoConnect();
	virtual ~CAdoConnect();

protected:
	void			Release();
	inline void		TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};

	ADODB::_ConnectionPtr	m_ptrConnection;
public:
	CAString		m_strConnectString;
	int				m_lConnectionState;
	CAString		m_strDataSource;
	int				m_nQueryTimeOut;

public:
	bool GetConnectionString( CAString &strConnectString );
	bool GetFreeConnectString( CAString &strConnectString );

	ADODB::_ConnectionPtr& GetConnection();
	void			SetConnection(ADODB::_ConnectionPtr ptrConnection);

	ADODB::_ConnectionPtr operator =(ADODB::_ConnectionPtr &ptrConnection);
	void operator =(CAdoConnect &AdoConnect);
	//==============================================
	// 错误处理
	//==============================================
	CAString			GetComError(const _com_error& e);
	CAString			GetProviderError();
	CAString			GetLastErrorText();
	CAString			GetLastSQLState();
	CAdoErrors*			GetErrors();
	CAdoError*			GetError(long index);

	//==============================================
	// 连接信息
	//==============================================
	CAString			GetProviderName();
	//==============================================
	// 回传 ADO 的版本号码。
	//==============================================
	CAString			GetVersion();
	//==============================================
	// 预设资料库。
	//==============================================
	CAString			GetDefaultDatabase();

	//==============================================
	// 回传值代表 Connection 是开启或关闭。
	// 对一个开启的 Connection 而言，回传值是 adStateOpen；
	// 如果 Connection 是关闭的状态，则回传 adStateClosed。
	//==============================================
	int				GetState();	
	bool			IsOpen();

	//==============================================
	// 连接模式
	//==============================================
	enum AdbcConnectMode GetMode();
	bool			SetMode(enum AdbcConnectMode mode);

	//==============================================
	// 连接到资料库、或回传一个错误讯息所需要的等待时间。 
	//==============================================
	void			SetConnectionTimeout(long lTimeout = 15);
	long			GetConnectionTimeout();

	//==============================================
	// 这是伺服器端的 script，可以检視资料錄的AdbcSchema，
	// 例如：资料表、栏位…等。 
	//==============================================
	CAdoRecordset*	OpenSchema(enum AdbcSchema QueryType);

	//==============================================
	// 开启一个新的Connection物件
	//==============================================
	bool		Open( CAString strConnectionString, 
					  enum AdbcConnectOption nOption = adbcConnectUnspecified );
	bool		Open( CAString strConnectionString, 
					  CAString strUser, 
					  CAString strPassword, 
					  int nOption );
	bool		Connect(LPCTSTR lpszConnectString = NULL, 
						const bool& bRetry = false,
						LPCTSTR lpszInitialCatalog = NULL);
	//==============================================
	// 关闭一个 Connection 物件
	//==============================================	
	bool		Close();

	//==============================================
	// 开始一个新的交易
	//==============================================
	long		BeginTrans();
	//==============================================
	// 存储交易过程中所做的改变，並且結束交易。
	// 也可以在这个时候开始一个新的交易。 
	//==============================================
	bool		CommitTrans();
	//==============================================
	// 取消交易过程中所做的改变，
	// 也可以在此时开始另外一个新的交易
	//==============================================
	bool		RollbackTrans();

	bool SetCursorLocation(enum AdbcCursorLocation CrLocation);
	void SetCommandTimeout(long pl);
	//==============================================
	// 執行一个查询、SQL指令或程序
	//==============================================
	bool		Execute(CAString CommandText, int &nRecordsAffected, enum AdbcCommandType Options = adbcCmdText );
	bool		Cancel();
};

#endif // !defined(AFX_ADOCONNECT_H__107036CD_353B_45E1_A7DA_6C8686459181__INCLUDED_)
