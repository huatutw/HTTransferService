// ACommFun.h: interface for the CACommFun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACOMMFUN_H__3F70C238_69F3_483B_916B_0E58E932B8C4__INCLUDED_)
#define AFX_ACOMMFUN_H__3F70C238_69F3_483B_916B_0E58E932B8C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////
// 常用的一些SQL函数
// 
/////////////////////////////////////////////////
class CAString;

class CAConnect;
class CAField;


//#include <vector>
//#include <list>
//using namespace std;

class CACommFun  
{
public:
	CACommFun();
	virtual ~CACommFun();

	static BOOL		GetIntValueFromDB( CAConnect *pAConnect, CAString strSQL, int &nVal);
	static BOOL		GetStrValueFromDB( CAConnect *pAConnect, CAString strSQL, CAString &strVal );
	static int		RunSQLGetIntData( CAConnect *pAConnect , CAString strSQL );
	static CAString	RunSQLGetStrData( CAConnect *pAConnect , CAString strSQL );
	static BOOL		GetTableFields(CAConnect *pAConnect, CAString strTableName, std::vector<CAString> &asField);
	static BOOL		GetTableFieldList(CAConnect *pAConnect ,CAString strTableName, std::vector<CAField> &listField);

	static BOOL		ExistTable( CAConnect *pAConnect , CAString strTbName);
	static BOOL		DropTable(CAConnect *pAConnect, CAString strTableName);
	static BOOL		ExistRecord(CAConnect *pAConnect, CAString strSQL, CAString strFileName = _T("") , int nLine = 0);
	static BOOL		RunSQL(CAConnect *pAConnect, CAString strSQL, CAString strFileName = _T(""), int nLine = -1);
	static BOOL		TruncateTable(CAConnect *pAConnect, CAString strTable);
	static BOOL		AlterTable_AddColumn(CAConnect *pAConnect,CAString strTableName, CAString strColumnName ,CAString strFieldType , CAString strDefaultValue);
	static BOOL		AlterTable_AddDefault(CAConnect *pAConnect, CAString strTableName, CAString strColumnName, CAString strDefaultName, CAString strDefaultValue);
	static BOOL		AlterTable_DelColumn(CAConnect *pAConnect, CAString strTableName, CAString  strColumnName  );
	static BOOL		AltrTable_DelDef(CAConnect *pAConnect, CAString strTableName, CAString strColumnName);
	static BOOL		AlterTable_AlterColumn(CAConnect *pAConnect, CAString strTableName, CAString strColumnName, CAString strFieldType , CAString strDefaultValue);
	static BOOL		AlterTable_RenColumn(CAConnect *pAConnect, CAString strTableName , CAString strOldName, CAString strNewName);
	static int		InsertGetID(CAConnect *pAConnect, LPCTSTR lpszInsertCom , LPCTSTR lpszFieldID);
	static BOOL		IDInsert(CAConnect *pAConnect, LPCTSTR lpszTableName, LPCTSTR lpszColumnName);
};

#endif // !defined(AFX_ACOMMFUN_H__3F70C238_69F3_483B_916B_0E58E932B8C4__INCLUDED_)
