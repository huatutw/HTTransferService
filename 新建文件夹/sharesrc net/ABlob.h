// ABlob.h: interface for the CABlob class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABLOB_H__123A0920_0637_43E6_8B44_CF3BEEF93A80__INCLUDED_)
#define AFX_ABLOB_H__123A0920_0637_43E6_8B44_CF3BEEF93A80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAString;
class CAConnect;
class CARecordset;


////////////////////////////////////////////////////////////
// BLOB处理类
// 此类采用ADO处理长数据的底层函数，封装了一些常用的BLOB处理
// 
////////////////////////////////////////////////////////////

class CABlob
{
public:
	CABlob();
	virtual ~CABlob();

public:
	static bool	ModifyFileToBlob( CAConnect* pAConnect, CAString strFileName, 
							      CAString strBlobField, CAString strWhere, 
							      CAString strTableName );
	static bool ModifyStringToBlob ( CAConnect* pAConnect, CAString strBuf, 
								     CAString strBlobField, CAString strWhere, 
								     CAString strTableName );
	static UINT SaveFileToBlob( CAConnect* pAConnect, CAString strFileName, 
								CAString strBlobField, CAString strTableName );
	static UINT SaveStringToBlob( CAConnect* pAConnect, CAString strBuf,
								  CAString strBlobField, CAString strTableName);

	static bool ReadStringFromBlob( CAConnect* pAConnect, CAString &strBuf, 
								    CAString strBlobField, CAString strWhere, 
								    CAString strTableName );
	static bool ReadFileFromBlob( CAConnect* pAConnect, CAString strFileName,
							      CAString strBlobField, CAString strWhere, 
								  CAString strTableName );

	//================================================================
	// 以下替换JfOleDB原有的函数，BLOB表字段被固定 Jiessie 2003/11/15
	//================================================================
	static UINT SaveFileToDB( CAConnect &AConnect, CAString strFileName, CAString strBlobTableName, bool bCompress );
	static UINT SaveStringToDB( CAConnect &AConnect, CAString strBlobTableName , CAString strBuf );
	static bool ReadStringFromDB( CAConnect &AConnect, UINT nID, CAString strBlobTableName, CAString &strBuf );
	static bool ReadFileFromDB( CAConnect &AConnect, UINT nID, CAString strFileName, CAString strBlobTableName, bool &bCompress );

	static bool ModifyFileInDB( CAConnect &AConnect, UINT nID, CAString strFileName, CAString strBlobTableName, bool bCompress );
	static bool ModifyStringInDB( CAConnect &AConnect, UINT nID, CAString strBlobTableName, CAString strBuf );
	static void DeleteBlobFromDB( CAConnect &AConnect, CAString strBlobTableName, UINT nID );
	static void DeleteFileFromDB( CAConnect &AConnect, CAString strBlobTableName, UINT nID );
};

#endif // !defined(AFX_ABLOB_H__123A0920_0637_43E6_8B44_CF3BEEF93A80__INCLUDED_)
