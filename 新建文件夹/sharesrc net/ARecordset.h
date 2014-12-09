// ARecordset.h: interface for the CARecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARECORDSET_H__E84E56C6_0F47_4892_A4AE_E860824CAD23__INCLUDED_)
#define AFX_ARECORDSET_H__E84E56C6_0F47_4892_A4AE_E860824CAD23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdbcCommDef.h"
class CAString;
class CAdoRecordset;
class CAConnect;
class CADORecordBinding;

class CADateTime;
class CAField;


class CARecordset
{
public:
	CARecordset();
	virtual ~CARecordset();
	CARecordset(CAConnect *pAConnect);

public:
	CAdoRecordset	*m_pAdoRecordset;
	CAConnect		*m_pAConnect;

protected:
	void	Release();

public:
	//=======================================
	// 在 Recordset 中开启一个指标
	//=======================================
	bool	Open( LPCTSTR lpszSource, CAConnect *pAConnect, 
				  enum AdbcCursorType nCursorType = adbcOpenStatic, 
				  enum AdbcLockType nLockType = adbcLockOptimistic, 
				  enum AdbcCommandType Options = adbcCmdText );
	//=======================================
	// 关闭一个 Recordset 以及相关的物件
	//=======================================
	bool	Cancel();
	bool	Close();

	//=======================================
	// 关联数据库
	//=======================================
	CAConnect*	GetConnection();
	void		SetAdoConnection(CAConnect *pAConnect);

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
	// 回传 Recordset 中的资料录总数量
	//=======================================
	long GetRecordCount();
	//=======================================
	// 回传 Recordset 中的字段数
	//=======================================
	long GetFieldsCount();

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
	// 获得字段名
	//=======================================
	CAString GetFieldName(long lIndex);
	
	//=======================================
	// 设置字段值
	//=======================================
	bool SetValue(const _variant_t & Index, const _variant_t  pvar ); 
	bool SetValue(long nIndex, const _variant_t value);
	bool SetValue(long nIndex, LPCTSTR lpszValue);
	bool SetValue(long nIndex, const double value);
	bool SetValue(long nIndex, const float  value);
	bool SetValue(long nIndex, const long   value);
	bool SetValue(long nIndex, const DWORD  value);
	bool SetValue(long nIndex, const int    value);
	bool SetValue(long nIndex, const short  value);
	bool SetValue(long nIndex, const BYTE   value);
	bool SetValue(long nIndex, const bool   value);
	bool SetValue(long nIndex, const CADateTime	value);
	bool SetValue(long nIndex, const CURRENCY value);
	bool SetValue(long nIndex, const char cData);	

	bool SetValueByName(LPCTSTR lpszFieldName, const _variant_t value);
	bool SetValueByName(LPCTSTR lpszFieldName, LPCTSTR lpszValue);
	bool SetValueByName(LPCTSTR lpszFieldName, const double value);
	bool SetValueByName(LPCTSTR lpszFieldName, const float  value);
	bool SetValueByName(LPCTSTR lpszFieldName, const long   value);
	bool SetValueByName(LPCTSTR lpszFieldName, const DWORD  value);
	bool SetValueByName(LPCTSTR lpszFieldName, const int    value);
	bool SetValueByName(LPCTSTR lpszFieldName, const short  value);
	bool SetValueByName(LPCTSTR lpszFieldName, const BYTE   value);
	bool SetValueByName(LPCTSTR lpszFieldName, const bool   value);
	bool SetValueByName(LPCTSTR lpszFieldName, const CADateTime value);
	bool SetValueByName(LPCTSTR lpszFieldName, const CURRENCY value);
	bool SetValueByName(LPCTSTR lpszFieldName, const char cData);	
	
	//==========================================
	// 获得字段值
	//==========================================
	_variant_t GetValue (const _variant_t & Index);
	bool GetValueByName(LPCTSTR lpszFieldName, VARIANT& vFieldVal);
	
	CAString GetStrValueByName(LPCTSTR lpszFieldName, bool bTrimRight = false );
	CAString GetStrValue( long nIndex , bool bTrimRight = false );

	bool GetValueByName(LPCTSTR lpszFieldName, double& dData);
	bool GetValueByName(LPCTSTR lpszFieldName, float& fData);
	bool GetValueByName(LPCTSTR lpszFieldName, long& lData);
	bool GetValueByName(LPCTSTR lpszFieldName, int& nData);
	bool GetValueByName(LPCTSTR lpszFieldName, BYTE& byData);
	bool GetValueByName(LPCTSTR lpszFieldName, char& cData);
	bool GetValueByName(LPCTSTR lpszFieldName, UINT& nData);
	bool GetValueByName(LPCTSTR lpszFieldName, DWORD &dwData);
	bool GetValueByName(LPCTSTR lpszFieldName, bool& bData);
	bool GetValueByName(LPCTSTR lpszFieldName, CADateTime& tData);
	bool GetValueByName(LPCTSTR lpszFieldName, CURRENCY& cData);
	bool GetValueByName(LPCTSTR lpszFieldName, short &nData);
	bool GetValueByName(LPCTSTR lpszFieldName, CAString& strData, bool bTrimRight = false);

	bool GetValue(long nIndex, CAString& strData, bool bTrimRight = false);
	bool GetValue(long nIndex, double& dData);
	bool GetValue(long nIndex, float& fData);
	bool GetValue(long nIndex, long& lData);
	bool GetValue(long nIndex, int& nData);
	bool GetValue(long nIndex, BYTE& vFieldVal);
	bool GetValue(long nIndex, char& cData);
	bool GetValue(long nIndex, DWORD &dwData);
	bool GetValue(long nIndex, UINT& nData);
	bool GetValue(long nIndex, bool& bData);
	bool GetValue(long nIndex, CADateTime& tData);
	bool GetValue(long nIndex, CURRENCY& cData);
	bool GetValue(long nIndex, short &nData);

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
	// 目前资料录的编辑状态
	//=======================================
	enum AdbcEditMode GetEditMode();

	//=======================================
	// 一个查询可以回传的资料录数目的最大数量
	//=======================================
	long GetMaxRecordCount();
	bool SetMaxRecordCount(long count);

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
	// 获得定义的字段长度
	//=======================================
	long GetFieldDefineSize(long lIndex);
	long GetFieldDefineSize(LPCTSTR lpszFieldName);
	
	//=======================================
	// 目前资料录所在的「页」（绝对值）、
	// 或指定目前资料录指标要移往的页数
	//=======================================
	long GetAbsolutePage();
	bool SetAbsolutePage(int nPage);
	
	//=======================================
	// 目前资料录指标的绝对位置
	//=======================================
	long GetAbsolutePosition();
	bool SetAbsolutePosition(int nPosition);

	//=======================================
	// 记录集对象
	//=======================================
	CAdoRecordset*	GetRecordset();
	void			SetRecordset(CAdoRecordset *pAdoRecordset);

	//=======================================
	// 得到最后的错误信息
	//=======================================
	CAString GetLastError();
	//=======================================
	// 获得字段实际长度
	//=======================================
	long  GetFieldActualSize(long lIndex);
	long  GetFieldActualSize(LPCTSTR lpszFieldName);

	//=======================================
	// 判断字段是否为NULL
	//=======================================
	bool  IsFieldNull(long nIndex);
	bool  IsFieldNull(CAString strFieldName);

	//=======================================
	// 字段属性
	// 字段集、字段对象
	//=======================================
	ADODB::FieldsPtr GetFields();
	bool  GetField(long lIndex, CAField *pField , bool bCopyData = true );
	bool  GetField(LPCTSTR lpszFieldName, CAField *pField , bool bCopyData = true );
	
	//=======================================
	// 游标类型
	//=======================================
	enum AdbcCursorType	GetCursorType();
	bool 			SetCursorType(enum AdbcCursorType AdbcCursorType = adbcOpenStatic);

	//=======================================
	// 游标位置
	//=======================================
	enum AdbcCursorLocation	GetCursorLocation();
	bool				SetCursorLocation(enum AdbcCursorLocation CursorLocation = adbcUseClient);
		
	//=======================================
	// 可以用来定义目前资料录的位置
	//=======================================
	_variant_t	GetBookmark();
	bool		SetBookmark(_variant_t varBookMark = _variant_t((long)adbcBookmarkFirst));
	bool		SetBookmark(enum AdbcBookmark AdbcBookmark = adbcBookmarkFirst);
	
	//=======================================
	// 获得字段数据类型
	//=======================================
	enum AdbcDataType GetFieldType(long lIndex);
	enum AdbcDataType GetFieldType(LPCTSTR lpszFieldName);

	//=======================================
	// 获得字段属性
	//=======================================
	long  GetFieldAttributes(long lIndex);
	long  GetFieldAttributes(LPCTSTR lpszFieldName);

	//=======================================
	// 藉由執行原有的查询更新资料
	//=======================================
	bool Requery(enum AdbcConnectOption Options = adbcConnectUnspecified);
	//=======================================
	// 从底层的资料库中更新资料。
	//=======================================
	bool AdbcResync(enum AdbcAffect AffectRecords = adbcAffectAll, 
				enum AdbcResync ResyncValues = adbcResyncAllValues);   
	bool RecordBinding(CADORecordBinding &pAdoRecordBinding);

	//=======================================
	// 长数据保存 
	//=======================================
	bool AppendChunk(ADODB::FieldPtr pField, LPVOID lpData, UINT nBytes);
	bool AppendChunk(long nIndex, LPVOID lpData, UINT nBytes);
	bool AppendChunk(LPCTSTR lpszFieldName, LPVOID lpData, UINT nBytes);

	bool AppendChunkFile(long nIndex, LPCTSTR lpszFileName);
	bool AppendChunkFile(LPCTSTR lpszFieldName, LPCTSTR lpszFileName);
	bool AppendChunkString(LPCTSTR lpszFieldName, LPCTSTR lpszString);
	bool AppendChunkString(long nIndex, LPCTSTR lpszString);

	//=======================================
	// 获得长数据
	//=======================================
	bool GetChunk(ADODB::FieldPtr pField, LPVOID lpData);
	bool GetChunk(long nIndex, LPVOID lpData);
	bool GetChunk(LPCTSTR lpszFieldName, LPVOID lpData);
//	bool GetChunk(long nIndex, CBitmap &bitmap);
//	bool GetChunk(CAString strFieldName, CBitmap &bitmap);

	bool GetChunkFile(LPCTSTR lpszFieldName, LPCTSTR lpszFileName);
	bool GetChunkFile(long nIndex, LPCTSTR lpszFileName);
	bool GetChunkString(LPCTSTR lpszFieldName, CAString &strString);
	bool GetChunkString(long nIndex, CAString &strString);
	
	//=======================================
	// 查找指定记录
	//=======================================
	bool Find(LPCTSTR lpszFind, enum AdbcSearchDirection SrDirection = adbcSearchForward);
	bool FindNext();

	//=======================================
	// 把记录集保存\载入文件
	//=======================================
	bool Save(CAString strFileName = _T(""), 
			  enum AdbcPersistFormat PsFormat = adbcPersistXML);
	bool Load(CAString strFileName);


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
	bool Clone(CARecordset *pARecordset);
	CAdoRecordset* operator = (CAdoRecordset *pARecordset);
	void operator =(CARecordset &ARecordset);
};

#endif // !defined(AFX_ARECORDSET_H__E84E56C6_0F47_4892_A4AE_E860824CAD23__INCLUDED_)
