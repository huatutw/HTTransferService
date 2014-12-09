// AConnect.h: interface for the CAConnect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACONNECT_H__AF55B31A_46D3_4EFE_B81B_E1B36B26414B__INCLUDED_)
#define AFX_ACONNECT_H__AF55B31A_46D3_4EFE_B81B_E1B36B26414B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CAString;

class CAExtDataMgr;
class CAdoConnect;
class CARecordset;
class CAErrors;
class CAError;

#include "AdbcCommDef.h"

//////////////////////////////////////////////////////////////////////////////
// 使用说明：Jiessie 2003/11/17
//	1. 在使用本类前需要初始化COM，可以调用 CoInitialize(NULL)
//	   来初始化, 用完后CoUninitialize()释放
//	2. 在使用记录集对象前先调用CAConnect::Open()（不同的数据库类型，如MS SQL
//	   可以用OpenSQLserver）连接数据库，连接上后可给多个CARecordSet对象使用。
//	3. 使用记录集CARecordset执行SQL语句之前，要使用构建方法或调用SetAdoConnection()
//	   关联到连接对象。
//	4. 保证 msado15.dll、msadox.dll、Oledb32.dll 全路径正确, 
//     若不为staAfx.h文件中定义的路径，更改为相应的正确路径。
//////////////////////////////////////////////////////////////////////////////
//using namespace std;

class CAConnect  
{
public:
	CAConnect();
	virtual ~CAConnect();

protected:
	CAdoConnect			*m_pAdoConnect;
//	CAExtDataMgr		*m_pExtDataMgr;
	

public:
	short				m_nDBType;				// 数据源的类型
	DWORD				m_dwFlag;	
	DWORD				m_dwExceptionProcess;
	UINT				m_nExceptionMsg;
//	HWND				m_hExceptionWnd;		// 接收异常信息的Wnd
	CAString			m_strErrorInfo;
	
public:
	void			ModifyExceptionFlag( DWORD dAdd , DWORD dRemove );
	BOOL			ReConnect();
	BOOL			GetConnectionString(CAString &strConnectString);
	BOOL			GetFreeConnectString(CAString &strConnectString );
	BOOL			GetUserData( LPCTSTR lpszVarName , CAString &strValue );
	BOOL			SetUserData( LPCTSTR lpszVarName, LPCTSTR lpszValue );
	BOOL			RemoveUserData( LPCTSTR lpszVarName );
	void			Release();
	void			InitConnect();
	void			SetConnectFlag( DWORD dwFlag );
	void			Exceptioin( UINT nCause, HRESULT hr = 0, CAString strErrorInfo = _T(""));
	CAdoConnect*	GetConnection();
	void			SetConnection(CAdoConnect *pAdoConnect);

	CAdoConnect*	operator =(CAdoConnect *pAdoConnect);
	void operator =(CAConnect &AConnect);

	//==============================================
	// 开启一个新的Connection物件
	//==============================================
	BOOL		Open( CAString lpszConnectionString, enum AdbcConnectOption nOption = adbcConnectUnspecified );
	BOOL		Open( CAString lpszConnectionString, CAString lpszUser, CAString lpszPassword, int nOption );
	BOOL		Connect( CAString strConnectString = _T(""), const bool& bRetry = false,
						 CAString strInitialCatalog = _T(""));
	BOOL		OpenSQLServer( CAString lpszServerName, CAString lpszDataBaseName, 
							   CAString lpszUser, CAString lpszPassword, 
						       enum AdbcConnectOption lOptions = adbcConnectUnspecified);
	BOOL		OpenOracleServer( LPCTSTR lpszServerName, LPCTSTR lpszDataBaseName, 
							      LPCTSTR lpszUser, LPCTSTR lpszPassword, 
								  enum AdbcConnectOption lOptions = adbcConnectUnspecified);
	BOOL		OpenAccess( LPCTSTR lpszMDBPath, 
						   LPCTSTR lpszPassword = NULL, 
							enum AdbcConnectOption lOptions = adbcConnectUnspecified);
	BOOL		OpenUDLFile( CAString strFileName, enum AdbcConnectOption lOptions = adbcConnectUnspecified);
	//==============================================
	// 关闭一个 Connection 物件
	//==============================================
	BOOL		Close();

	//==============================================
	// 开始一个新的交易
	//==============================================
	long			BeginTrans();
	//==============================================
	// 存储交易过程中所做的改变，並且結束交易。
	// 也可以在这个时候开始一个新的交易。 
	//==============================================
	BOOL			CommitTrans();
	//==============================================
	// 取消交易过程中所做的改变，
	// 也可以在此时开始另外一个新的交易
	//==============================================
	BOOL			RollbackTrans();

	//==============================================
	// 執行一个查询、SQL指令或程序
	//==============================================
	BOOL			Execute(LPCTSTR lpszCommandText);
	BOOL			Execute(LPCTSTR lpszCommandText, int &nRecordsAffected, enum AdbcCommandType Options = adbcCmdText );
	BOOL			Cancel();

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
	// 连接模式
	//==============================================
	enum AdbcConnectMode GetMode();
	BOOL			SetMode(enum AdbcConnectMode mode);

	//==============================================
	// 回传值代表 Connection 是开启或关闭。
	// 对一个开启的 Connection 而言，回传值是 adStateOpen；
	// 如果 Connection 是关闭的状态，则回传 adStateClosed。
	//==============================================
	int				GetState();	
	BOOL			IsOpen();

	//==============================================
	// 连接到资料库、或回传一个错误讯息所需要的等待时间。 
	//==============================================
	void			SetConnectionTimeout(long lTimeout = 15);
	long			GetConnectionTimeout();

	//==============================================
	// 这是伺服器端的 script，可以检視资料錄的AdbcSchema，
	// 例如：资料表、栏位…等。 
	//==============================================
	CARecordset*	OpenSchema(enum AdbcSchema QueryType);

	BOOL SetCursorLocation(enum AdbcCursorLocation CursorLocation/* = adbcUseClient*/);
	void SetCommandTimeout(long pl);
	//===============================================
	// 错误处理
	//===============================================
	CAString		GetComError(const _com_error& e);
	CAString		GetProviderError();
	void			SetErrorInfo( CAString strError );
	CAString		GetLastErrorText();
	CAString		GetErrorInfo( bool bAddLastError = true );
	CAString		GetLastSQLState();
	CAErrors*		GetErrors();
	CAError*		GetError(long index);
};

#endif // !defined(AFX_ACONNECT_H__AF55B31A_46D3_4EFE_B81B_E1B36B26414B__INCLUDED_)
