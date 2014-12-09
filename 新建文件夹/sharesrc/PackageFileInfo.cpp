// PackageFileInfo.cpp: implementation of the CPackageFileInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PackageFileInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPackageFileInfo::CPackageFileInfo()
{
	m_lpszFileName = NULL;
	m_lpszPathName = NULL;
	m_lpszFileGUID = NULL;

	_ctrlType = FILE_BY_NONE;
	
	m_nCounts = 0;
	m_nDays = 0;
}

CPackageFileInfo::~CPackageFileInfo()
{
	ResetContent();
}	

void CPackageFileInfo::ResetContent()
{
	if (m_lpszFileName)
	{
		delete[] m_lpszFileName;
		m_lpszFileName = NULL;
	}

	if (m_lpszPathName)
	{
		delete[] m_lpszPathName;
		m_lpszPathName = NULL;
	}

	if (m_lpszFileGUID)
	{
		delete[] m_lpszFileGUID;
		m_lpszFileGUID = NULL;
	}
}

void CPackageFileInfo::SetFileName(LPCTSTR strFileName)
{
	int iLen = _tcslen(strFileName);
	m_lpszFileName =  new TCHAR[iLen+1];
	memset(m_lpszFileName,0,(iLen+1)*sizeof(TCHAR));
	_tcscpy(m_lpszFileName,strFileName);
}

void CPackageFileInfo::SetPathName(LPCTSTR strPathName)
{
	int iLen = _tcslen(strPathName);
	m_lpszPathName =  new TCHAR[iLen+1];
	memset(m_lpszPathName,0,(iLen+1)*sizeof(TCHAR));
	_tcscpy(m_lpszPathName,strPathName);
}

void CPackageFileInfo::SetFileGUID(LPCTSTR strFileGUID)
{
	int iLen = _tcslen(strFileGUID);
	m_lpszFileGUID =  new TCHAR[iLen+1];
	memset(m_lpszFileGUID,0,(iLen+1)*sizeof(TCHAR));
	_tcscpy(m_lpszFileGUID,strFileGUID);	
}

void CPackageFileInfo::Init(LPCTSTR strFileName,LPCTSTR strPathName,LPCTSTR strFileGUID)
{
	SetFileName(strFileName);
	SetPathName(strPathName);
	SetFileGUID(strFileGUID);
}

void CPackageFileInfo::WriteData(CMemFile& file)
{
	int iLen =  _tcslen(m_lpszFileName);
	file.Write(&iLen,sizeof(int));
	if (iLen > 0)
		file.Write(m_lpszFileName,iLen*sizeof(TCHAR));

	iLen =  _tcslen(m_lpszPathName);
	file.Write(&iLen,sizeof(int));
	if (iLen > 0)
		file.Write(m_lpszPathName,iLen*sizeof(TCHAR));

	iLen =  _tcslen(m_lpszFileGUID);
	file.Write(&iLen,sizeof(int));
	if (iLen > 0)
		file.Write(m_lpszFileGUID,iLen*sizeof(TCHAR));

	file.Write(&_ctrlType,sizeof(int));
	file.Write(&m_nCounts,sizeof(int));
	file.Write(&m_nDays,sizeof(int));
	
	file.Write(&_editMode,sizeof(int));
	file.Write(&m_bCanPrint,sizeof(BOOL));
}

void CPackageFileInfo::ReadData(CMemFile& file)
{
	ResetContent();
	
	int iLen = 0;

	file.Read(&iLen,sizeof(int));
	if (iLen > 0)
	{
		m_lpszFileName = new TCHAR[iLen+1];
		memset(m_lpszFileName,0,(iLen+1)*sizeof(TCHAR));
		file.Read(m_lpszFileName,iLen*sizeof(TCHAR));
	}

	file.Read(&iLen,sizeof(int));
	if (iLen > 0)
	{
		m_lpszPathName = new TCHAR[iLen+1];
		memset(m_lpszPathName,0,(iLen+1)*sizeof(TCHAR));
		file.Read(m_lpszPathName,iLen*sizeof(TCHAR));
	}

	file.Read(&iLen,sizeof(int));
	if (iLen > 0)
	{
		m_lpszFileGUID = new TCHAR[iLen+1];
		memset(m_lpszFileGUID,0,(iLen+1)*sizeof(TCHAR));
		file.Read(m_lpszFileGUID,iLen*sizeof(TCHAR));
	}

	file.Read(&_ctrlType,sizeof(int));
	file.Read(&m_nCounts,sizeof(int));
	file.Read(&m_nDays,sizeof(int));
	file.Read(&_editMode,sizeof(int));
	file.Read(&m_bCanPrint,sizeof(BOOL));
}

void CPackageFileInfo::GetLicFromSystem()
{
	switch (_ctrlType)
	{
	case FILE_BY_COUNT:			//按次数授权
		{
			
		}
		break;
	case FILE_BY_TIME:			//按天数授权
		{
			
		}
		break;
	}
}

BOOL CPackageFileInfo::IsFileValid(SYSTEMTIME tFirstUse,SYSTEMTIME tCurrent)
{
	if (_ctrlType & FILE_BY_COUNT)
	{
		if (m_nCounts < 0)
		{
			return FALSE;
		}
	}

	if (_ctrlType & FILE_BY_TIME)
	{
		SYSTEMTIME tCur;
		GetLocalTime(&tCur);
		CTime tCurTime(tCur);
		
		CTime tStart(tFirstUse);
		CTime tPrev(tCurrent);

		CTimeSpan t = tCurTime - tStart;

		//超过天数
		if (t.GetTotalMinutes() > m_nDays*1440)
		{
			return FALSE;
		}
		
		t = tPrev - tCurTime;

		if (t.GetTotalSeconds() > 0)		//当前时间比上次记录的时间还要早
		{
			return FALSE;
		}
		return TRUE;
	}else
	{
		if (_ctrlType & FILE_BY_COUNT)
		{
			if (m_nCounts > 0)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}