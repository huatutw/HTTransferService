// AdoRecordset.h: interface for the CAdoRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADORECORDSET_H__6CDA9FA4_5451_4E41_889D_EAC28CE3AC76__INCLUDED_)
#define AFX_ADORECORDSET_H__6CDA9FA4_5451_4E41_889D_EAC28CE3AC76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAString;
class CADateTime;

class CAdoConnect;
class CADORecordBinding;

#include "AdbcCommDef.h"

class CAdoRecordset  
{
protected:
	ADODB::_RecordsetPtr	m_ptrRecordset;

	CAdoConnect				*m_pAdoConnect;
	
	CAString				m_strFind;
	IADORecordBinding		*m_pAdoRecordBinding;
	enum AdbcSearchDirection	m_SearchDirection;
	_variant_t				m_varBookmark;

protected:
	void	Release();
	inline	void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};

public:
	CAdoRecordset();
	virtual ~CAdoRecordset();
	CAdoRecordset(CAdoConnect *pAdoConnect);

	//=======================================
	// 在 Recordset 中开启一个指标
	//=======================================
	bool	Open( CAString strSource, 
				  CAdoConnect *pAdoConnect,
				  enum AdbcCursorType nCursorType = adbcOpenStatic,
				  enum AdbcLockType nLockType = adbcLockOptimistic,
				  enum AdbcCommandType Options = adbcCmdText );
	//=======================================
	// 关闭一个 Recordset 以及相关的物件
	//=======================================
	bool Cancel();
	bool Close();
	//=======================================
	// 建立一笔新的资料录。
	//=======================================
	bool AddNew(const _variant_t &FieldList = vtMissing, const _variant_t &Values = vtMissing);
	bool AddNew(CADORecordBinding &pAdoRecordBinding);
	//======================================
	// 存储目前资料录上所做的任何改变
	//======================================
	bool Update();
	//======================================
	// 批次更新
	//======================================
	bool UpdateBatch(enum AdbcAffect AffectRecords = adbcAffectAll);
	//======================================
	// 取消更新。
	//======================================	
	bool CancelUpdate();
	//======================================
	// 取消批次更新
	//======================================
	bool CancelBatch(enum AdbcAffect AffectRecords = adbcAffectAll);
	//======================================
	// 在一个正在开启的 Recordset 中刪除一笔资料录
	//======================================
	bool Delete(enum AdbcAffect AffectRecords = adbcAffectCurrent);
	
	//=======================================
	// 目前资料录的编辑状态
	//=======================================
	enum AdbcEditMode GetEditMode();

	//=======================================
	// 判断目前资料录的位置是否在第一笔资料录之前
	//=======================================
	bool IsEOF();
	//=======================================
	// 判断目前资料录是否在最后一笔资料录之后
	//=======================================
	bool IsBOF();
	bool IsOpen();
	//=======================================
	// 回传一个表示 Recordset 物件是开启、或是关闭的状态。
	//=======================================
	long GetState();
	//=======================================
	// 目前资料录的状态。通常我们可以根据这個回传值，
	// 然后决定是否要批次处理、或其它的运作方式。
	//=======================================
	long GetStatus();

	//=======================================
	// 一个查询可以回传的资料录数目的最大数量
	//=======================================
	long GetMaxRecordCount();
	bool SetMaxRecordCount(long count);
	
	//=======================================
	// 光标位置
	//=======================================
	enum AdbcCursorLocation GetCursorLocation();
	bool SetCursorLocation(enum AdbcCursorLocation CrLocation = adbcUseClient);
	
	//=======================================
	// 光标类型
	//=======================================
	enum AdbcCursorType GetCursorType();
	bool SetCursorType(enum AdbcCursorType AdbcCursorType = adbcOpenStatic);
	
	//=======================================
	// 可以用来定义目前资料录的位置
	//=======================================
	_variant_t GetBookmark();
	bool SetBookmark(_variant_t varBookMark = _variant_t((long)adbcBookmarkFirst));
	
	//=======================================
	// 目前资料录指标的绝对位置
	//=======================================
	long GetAbsolutePosition();
	bool SetAbsolutePosition(int nPosition);

	long GetAbsolutePage();
	bool SetAbsolutePage(int nPage);

	//=======================================
	// 设定組成一页的资料录数量
	//=======================================
	long GetPageSize();
	bool SetPageSize(long nPageSize);
	//=======================================
	// 存储在本机记忆体暂存区的资料录数目
	//=======================================
	bool SetCacheSize(const long& lCacheSize);
	//=======================================
	// 回传 Recordset 等于包含几「页」的资料量
	//=======================================	
	long GetPageCount();

	//=======================================
	// 回传 Recordset 中的资料录总数量
	//=======================================
	long GetRecordCount();
	//=======================================
	// 回传 Recordset 中的字段数
	//=======================================
	long GetFieldsCount();
	
	//=======================================
	// 连接对象 
	//=======================================
	CAdoConnect* GetConnection() {return m_pAdoConnect;}
	void SetAdoConnection(CAdoConnect *pAdoConnect);

	//=======================================
	// 记录集对象
	//=======================================
	ADODB::_RecordsetPtr& GetRecordset();
	void	SetRecordset(ADODB::_RecordsetPtr ptrRecordset);

	//=======================================
	// 得到最后的错误信息
	//=======================================
	CAString GetLastError();

	//=======================================
	// 字段属性
	// 字段集、字段对象
	//=======================================
	ADODB::FieldsPtr GetFields();
	ADODB::FieldPtr  GetField(long lIndex);
	ADODB::FieldPtr  GetField(LPCTSTR lpszFieldName);
	
	//=======================================
	// 获得字段名
	//=======================================
	CAString GetFieldName(long lIndex);
	
	//=======================================
	// 获得字段数据类型
	//=======================================
	enum AdbcDataType GetFieldType(long lIndex);
	enum AdbcDataType GetFieldType(LPCTSTR lpszFieldName);

	//=======================================
	// 获得字段实际长度
	//=======================================
	long  GetFieldAttributes(long lIndex);
	long  GetFieldAttributes(LPCTSTR lpszFieldName);

	//=======================================
	// 获得定义的字段长度
	//=======================================
	long  GetFieldDefineSize(long lIndex);
	long  GetFieldDefineSize(LPCTSTR lpszFieldName);

	//=======================================
	// 字段实际长度
	//=======================================
	long  GetFieldActualSize(long lIndex);
	long  GetFieldActualSize(LPCTSTR lpszFieldName);

	//=======================================
	// 判断字段是否为NULL
	//=======================================
	bool  IsFieldNull(long nIndex);
	bool  IsFieldNull(LPCTSTR lpFieldName);

	//=======================================
	// 藉由執行原有的查询更新资料
	//=======================================
	bool Requery(enum AdbcConnectOption Options = adbcConnectUnspecified);
	bool AdbcResync(enum AdbcAffect AffectRecords = adbcAffectAll, 
				enum AdbcResync ResyncValues = adbcResyncAllValues);   
	//=======================================
	// 从底层的资料库中更新资料。
	//=======================================
	bool RecordBinding(CADORecordBinding &pAdoRecordBinding);

	//=======================================
	// 中移动目前资料录的位置到第一笔、最后一笔、
	// 下一笔、前一笔的位置。 
	//=======================================
	bool MoveFirst();
	bool MovePrevious();
	bool MoveNext();
	bool MoveLast();
	//=======================================
	// 移动目前资料录的位置
	//=======================================
	bool Move(long lRecords, _variant_t Start = _variant_t((long)adbcBookmarkFirst));
	
	//=======================================
	// 查找指定的记录
	//=======================================
	bool Find(LPCTSTR lpszFind, enum AdbcSearchDirection SrDirection = adbcSearchForward);
	bool FindNext();

	//=======================================
	// 保存/载入持久性文件
	//=======================================
	bool Save(LPCTSTR strFileName = _T(""), 
			  enum AdbcPersistFormat PsFormat = adbcPersistXML);
	bool Load(LPCTSTR strFileName);
	
	//=======================================
	// 设置字段值
	//=======================================
	bool SetValue(const _variant_t Index, const _variant_t pvar ); 
	bool SetValue(long nIndex, const _variant_t value);
	bool SetValue(long nIndex, const CAString value);
	bool SetValue(long nIndex, const double value);
	bool SetValue(long nIndex, const float  value);
	bool SetValue(long nIndex, const long   value);
	bool SetValue(long nIndex, const DWORD  value);
	bool SetValue(long nIndex, const int    value);
	bool SetValue(long nIndex, const short  value);
	bool SetValue(long nIndex, const BYTE   value);
	bool SetValue(long nIndex, const bool   value);
	bool SetValue(long nIndex, const CADateTime value);
	bool SetValue(long nIndex, const CURRENCY value);
	bool SetValue(long nIndex, const char cData);	

	bool SetValueByName(LPCTSTR strFieldName, const _variant_t value);
	bool SetValueByName(LPCTSTR strFieldName, const CAString value);
	bool SetValueByName(LPCTSTR strFieldName, const double value);
	bool SetValueByName(LPCTSTR strFieldName, const float  value);
	bool SetValueByName(LPCTSTR strFieldName, const long   value);
	bool SetValueByName(LPCTSTR strFieldName, const DWORD  value);
	bool SetValueByName(LPCTSTR strFieldName, const int    value);
	bool SetValueByName(LPCTSTR strFieldName, const short  value);
	bool SetValueByName(LPCTSTR strFieldName, const BYTE   value);
	bool SetValueByName(LPCTSTR strFieldName, const bool   value);
	bool SetValueByName(LPCTSTR strFieldName, const CADateTime value);
	bool SetValueByName(LPCTSTR strFieldName, const CURRENCY value);
	bool SetValueByName(LPCTSTR strFieldName, const char cData);	

	//==========================================
	// 获得字段值
	//==========================================
	_variant_t GetValue (const _variant_t & Index);
	bool	GetValueByName(CAString strFieldName, VARIANT& vFieldVal);
	bool	GetValueByName(CAString strFieldName, CAString& strData);
	bool	GetValueByName(CAString strFieldName, double& dData);
	bool	GetValueByName(CAString strFieldName, float& fData);
	bool	GetValueByName(CAString strFieldName, long& lData);
	bool	GetValueByName(CAString strFieldName, int& nData);
	bool	GetValueByName(CAString strFieldName, BYTE& byData);
	bool	GetValueByName(CAString strFieldName, char& cData);
	bool	GetValueByName(CAString strFieldName, UINT& unData);
	bool	GetValueByName(CAString strFieldName, DWORD &dwData);
	bool	GetValueByName(CAString strFieldName, bool& bData);
	bool	GetValueByName(CAString strFieldName, CADateTime& tData);
	bool	GetValueByName(CAString strFieldName, CURRENCY& cData);
	bool	GetValueByName(CAString strFieldName, short &nData);	

	bool	GetValue(long nIndex, CAString& strData);
	bool	GetValue(long nIndex, double& dData);
	bool	GetValue(long nIndex, float& fData);
	bool	GetValue(long nIndex, long& lData);
	bool	GetValue(long nIndex, int& nData);
	bool	GetValue(long nIndex, BYTE& byData);
	bool	GetValue(long nIndex, char& cData);	
	bool	GetValue(long nIndex, DWORD &dwData);
	bool	GetValue(long nIndex, UINT& unData);
	bool	GetValue(long nIndex, bool& bData);
	bool	GetValue(long nIndex, CADateTime& tData);
	bool	GetValue(long nIndex, CURRENCY& cData);
	bool	GetValue(long nIndex, short &nData);

	//=======================================
	// BLOB 数据存取 
	//=======================================
	bool AppendChunk(ADODB::FieldPtr pField, LPVOID lpData, UINT nBytes);
	bool AppendChunk(long nIndex, LPVOID lpData, UINT nBytes);
	bool AppendChunk(LPCTSTR strFieldName, LPVOID lpData, UINT nBytes);
	
	bool AppendChunkFile(long nIndex, LPCTSTR lpszFileName);
	bool AppendChunkFile(LPCTSTR strFieldName, LPCTSTR lpszFileName);
	bool AppendChunkString(LPCTSTR strFieldName, LPCTSTR lpszString);
	bool AppendChunkString(long nIndex, LPCTSTR lpszString);

	bool GetChunk(ADODB::FieldPtr pField, LPVOID lpData);
	bool GetChunk(long nIndex, LPVOID lpData);
	bool GetChunk(LPCTSTR strFieldName, LPVOID lpData);
//	bool GetChunk(long nIndex, CBitmap &bitmap);
//	bool GetChunk(LPCTSTR strFieldName, CBitmap &bitmap);

	bool GetChunkFile(LPCTSTR strFieldName, LPCTSTR lpszFileName);
	bool GetChunkFile(long nIndex, LPCTSTR lpszFileName);
	bool GetChunkString(LPCTSTR strFieldName, CAString &strString);
	bool GetChunkString(long nIndex, CAString &strString);
	//========================================
	// 表示是否要使用 Filter（过滤器）
	//========================================
	bool SetFilter(LPCTSTR lpszFilter);

	//========================================
	// 排序
	//========================================
	bool SetSort(LPCTSTR lpszCriteria);

	//========================================
	// 确定是否 Recordset 可以支援某些特定的功能。
	//========================================
	bool Supports(enum AdbcCursorOption CrOptions = adbcAddNew);

	//========================================
	// 建立一个 Recordset 物件的副本。
	//========================================
	bool Clone(CAdoRecordset *pAdoRecordset);
	ADODB::_RecordsetPtr operator = (ADODB::_RecordsetPtr &ptrRecordset);
	void operator =(CAdoRecordset &AdoRecordset);
};

#endif // !defined(AFX_ADORECORDSET_H__6CDA9FA4_5451_4E41_889D_EAC28CE3AC76__INCLUDED_)
