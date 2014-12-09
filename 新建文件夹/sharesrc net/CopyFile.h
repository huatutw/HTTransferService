// CopyFile.h: interface for the CCopyFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COPYFILE_H__A707363D_9696_4978_A45A_06568778DB55__INCLUDED_)
#define AFX_COPYFILE_H__A707363D_9696_4978_A45A_06568778DB55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UpdateBaseClass.h"

class CCopyFile: public CUpdateBaseClass  
{
public:
	CCopyFile();
	virtual ~CCopyFile();

	int m_nCopyType;				//0:复制文件 2:移动文件
	int m_nExistFileType;			//0:普通文件 1:system32文件 2:windows文件 3:当前目录下的文件
	CAString m_strExistFilePath;
	CAString m_strExistFileName;
		
	int m_nTargetFileType;			//0:普通文件 1:system32文件 2:windows文件 3:当前目录下的文件
	CAString m_strTargetFilePath;
	CAString m_strTargetFileName;
};

#endif // !defined(AFX_COPYFILE_H__A707363D_9696_4978_A45A_06568778DB55__INCLUDED_)
