// PackageFileInfo.h: interface for the CPackageFileInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGEFILEINFO_H__730DBF3C_16D9_48B5_ABFC_5D072B42DDC3__INCLUDED_)
#define AFX_PACKAGEFILEINFO_H__730DBF3C_16D9_48B5_ABFC_5D072B42DDC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>

enum  FileControlMode
{
	FILE_BY_NONE = 0,
	FILE_BY_COUNT,
	FILE_BY_TIME,
};

enum FileEditMode
{
	FILE_READONLY_MODE = 0,
	FILE_EDIT_MODE,
};

class CPackageFileInfo  
{
public:
	CPackageFileInfo();
	virtual ~CPackageFileInfo();

	void Init(LPCTSTR strFileName,LPCTSTR strPathName,LPCTSTR strFileGUID);
	void ResetContent();

	void SetFileName(LPCTSTR strFileName);
	void SetPathName(LPCTSTR strPathName);
	void SetFileGUID(LPCTSTR strFileGUID);

	void WriteData(CMemFile& file);
	void ReadData(CMemFile& file);

	BOOL IsFileValid(SYSTEMTIME tFirstUser,SYSTEMTIME tCurrent);

	void GetLicFromSystem();		//从系统文件或者注册表得到最新的授权

public:
	LPTSTR m_lpszFileName;			//文件名
	LPTSTR m_lpszPathName;			//全路径名
	LPTSTR m_lpszFileGUID;			//文件ID

	int		_ctrlType;				//控制类型
	int		m_nCounts;				//根据控制类型的不同，决定是次数
	int		m_nDays;				//天数
	int		_editMode;				//编辑模式
	BOOL	m_bCanPrint;			//是否可以打印
};

typedef std::deque<CPackageFileInfo*> PACKAGE_FILE_INFO;

#endif // !defined(AFX_PACKAGEFILEINFO_H__730DBF3C_16D9_48B5_ABFC_5D072B42DDC3__INCLUDED_)
