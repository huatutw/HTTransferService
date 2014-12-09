// UploadFileInfo.cpp: implementation of the CUploadFileInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpdateBaseClass.h"
#include "UploadFileInfo.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUploadFileInfo::CUploadFileInfo()
{
	m_nUpdateType = UPDATE_FILEINFOMATION;
	memset(m_strFileList,0,1024*sizeof(TCHAR));
}

CUploadFileInfo::~CUploadFileInfo()
{

}
