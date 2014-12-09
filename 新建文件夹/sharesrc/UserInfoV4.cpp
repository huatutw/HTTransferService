// UserInfoV4.cpp: implementation of the CUserInfoV4 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserInfoV4.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserInfoV4::CUserInfoV4()
{
	m_strLoginName = NULL;
	m_strUserName = NULL;
	m_strPassword = NULL;

	m_dwRightGroup = 0;
	m_dwOverTime = 0;
}

CUserInfoV4::~CUserInfoV4()
{
	if (m_strLoginName)
	{
		delete m_strLoginName;
		m_strLoginName = NULL;
	}

	if (m_strUserName)
	{
		delete m_strUserName;
		m_strUserName = NULL;
	}

	if (m_strPassword)
	{
		delete m_strPassword;
		m_strPassword = NULL;
	}

	m_arrOpenFile.clear();
	m_arrPrintFile.clear();
	m_arrAdjustFile.clear();
	m_arrDecryptFile.clear();
	m_arrDepartmentID.clear();
	m_arrReadFile.clear();
	m_arrOutSideFile.clear();
	m_arrApprovalDecrypt.clear();
	m_arrApprovalOutSide.clear();
}

void CUserInfoV4::SetUserRight(DWORD lRight)
{
	m_dwRightGroup = lRight;
}

DWORD CUserInfoV4::GetUserRight()
{
	return m_dwRightGroup;
}

void CUserInfoV4::SetOverTime(int dwOverTime)
{
	m_dwOverTime = dwOverTime;
}

int  CUserInfoV4::GetOverTime()
{
	return m_dwOverTime;
}

void CUserInfoV4::SetCurrentLevel(int level)
{
	m_nLevelID = level;
}

int	 CUserInfoV4::GetCurrentLevel()
{
	return m_nLevelID;
}

void CUserInfoV4::SetOneLevelInfo(int nType,int nLevelID)
{
	switch(nType)
	{
	case RIGHT_OPEN:
		{
			//如果没有在等级表中
			if (!CanOpen(nLevelID))
			{
				m_arrOpenFile.insert(std::pair<int,int>(nLevelID,nLevelID));
			}
		}
		break;
	case RIGHT_ADJUST:
		{
			//如果没有在等级表中
			if (!CanAdjust(nLevelID))
			{
				m_arrAdjustFile.insert(std::pair<int,int>(nLevelID,nLevelID));
			}
		}
		break;
	case RIGHT_DECRYPT:
		{
			//如果没有在等级表中
			if (!CanDecrypt(nLevelID))
			{
				m_arrDecryptFile.insert(std::pair<int,int>(nLevelID,nLevelID));
			}
		}
		break;
	case RIGHT_PRINT:
		{
			//如果没有在等级表中
			if (!CanPrint(nLevelID))
			{
				m_arrPrintFile.insert(std::pair<int,int>(nLevelID,nLevelID));
			}
		}
		break;
	case RIGHT_READ:
		{
			//如果没有在等级表中
			if (!CanRead(nLevelID))
			{
				m_arrReadFile.insert(std::pair<int,int>(nLevelID,nLevelID));
			}
		}
		break;
	case RIGHT_OUTSIDE:
		{
			//如果没有在等级表中
			if (!CanOutSide(nLevelID))
			{
				m_arrOutSideFile.insert(std::pair<int,int>(nLevelID,nLevelID));
			}
		}
		break;
	case RIGHT_APPROVALDECRYPT:
		{
			//如果没有在等级表中
			if (!CanApprovalDecrypt(nLevelID))
			{
				m_arrApprovalDecrypt.insert(std::pair<int,int>(nLevelID,nLevelID));
			}
		}
		break;
	case RIGHT_APPROVALOUTSIDE:
		{
			//如果没有在等级表中
			if (!CanApprovalOutSide(nLevelID))
			{
				m_arrApprovalOutSide.insert(std::pair<int,int>(nLevelID,nLevelID));
			}
		}
		break;
	}
}

BOOL CUserInfoV4::CanOpen(int nLevelID)
{
	return m_arrOpenFile.find(nLevelID) != m_arrOpenFile.end();
}

BOOL CUserInfoV4::CanAdjust(int nLevelID)
{
	return m_arrAdjustFile.find(nLevelID) != m_arrAdjustFile.end();
}

BOOL CUserInfoV4::CanDecrypt(int nLevelID)
{
	return m_arrDecryptFile.find(nLevelID) != m_arrDecryptFile.end();
}

BOOL CUserInfoV4::CanPrint(int nLevelID)
{
	return m_arrPrintFile.find(nLevelID) != m_arrPrintFile.end();
}

BOOL CUserInfoV4::CanRead(int nLevelID)
{
	return m_arrReadFile.find(nLevelID) != m_arrReadFile.end();
}

BOOL CUserInfoV4::CanOutSide(int nLevelID)
{
	return m_arrOutSideFile.find(nLevelID) != m_arrOutSideFile.end();
}

BOOL CUserInfoV4::CanApprovalDecrypt(int nLevelID)
{
	return m_arrApprovalDecrypt.find(nLevelID) != m_arrApprovalDecrypt.end();
}

BOOL CUserInfoV4::CanApprovalOutSide(int nLevelID)
{
	return m_arrApprovalOutSide.find(nLevelID) != m_arrApprovalOutSide.end();
}


int CUserInfoV4::GetOpenFileSize()
{
	return m_arrOpenFile.size();
}

int CUserInfoV4::GetAdjustFileSize()
{
	return m_arrAdjustFile.size();
}

int CUserInfoV4::GetDecryptFileSize()
{
	return m_arrDecryptFile.size();
}

int CUserInfoV4::GetPrintFileSize()
{
	return m_arrPrintFile.size();
}

int CUserInfoV4::GetUserDepartCount()
{
	return m_arrDepartmentID.size();
}

int CUserInfoV4::GetReadFileSize()
{
	return m_arrReadFile.size();
}

int CUserInfoV4::GetOutSideFileSize()
{
	return m_arrOutSideFile.size();
}

int CUserInfoV4::GetApprovalDecryptSize()
{
	return m_arrApprovalDecrypt.size();
}

int CUserInfoV4::GetApprovalOutSideSize()
{
	return m_arrApprovalOutSide.size();
}

int CUserInfoV4::GetOpenFile(int nOpen)
{
 	int index = 0;
 	std::map<int,int>::iterator i;
 	for (i = m_arrOpenFile.begin(); i != m_arrOpenFile.end(); i++)
 	{
		int nOpenFileID = i->second;
		if(index == nOpen)
			return nOpenFileID;
		index++;
 	}

	return -1;
}

int CUserInfoV4::GetAdjustFile(int nAdjust)
{
	int index = 0;
	std::map<int,int>::iterator i;
	for (i = m_arrAdjustFile.begin(); i != m_arrAdjustFile.end(); i++)
	{
		int nAdjustFileID = i->second;
		if(index == nAdjust)
			return nAdjustFileID;
		index++;
	}
	
	return -1;
}

int CUserInfoV4::GetDecryptFile(int nDecrypt)
{
	int index = 0;
	std::map<int,int>::iterator i;
	for (i = m_arrDecryptFile.begin(); i != m_arrDecryptFile.end(); i++)
	{
		int nDecryptFileID = i->second;
		if(index == nDecrypt)
			return nDecryptFileID;
		index++;
	}
	
	return -1;
}

int CUserInfoV4::GetPrintFile(int nPrint)
{
	int index = 0;
	std::map<int,int>::iterator i;
	for (i = m_arrPrintFile.begin(); i != m_arrPrintFile.end(); i++)
	{
		int nPrintFileID = i->second;
		if(index == nPrint)
			return nPrintFileID;
		index++;
	}
	
	return -1;
}

int CUserInfoV4::GetReadFile(int nRead)
{
	int index = 0;
	std::map<int,int>::iterator i;
	for (i = m_arrReadFile.begin(); i != m_arrReadFile.end(); i++)
	{
		int nReadFileID = i->second;
		if(index == nRead)
			return nReadFileID;
		index++;
	}
	
	return -1;
}

int CUserInfoV4::GetOutSideFile(int nOutSide)
{
	int index = 0;
	std::map<int,int>::iterator i;
	for (i = m_arrOutSideFile.begin(); i != m_arrOutSideFile.end(); i++)
	{
		int nOutSideFileID = i->second;
		if(index == nOutSide)
			return nOutSideFileID;
		index++;
	}
	
	return -1;
}

int CUserInfoV4::GetApprovalDecrypt(int nApprovalDecrypt)
{
	int index = 0;
	std::map<int,int>::iterator i;
	for (i = m_arrApprovalDecrypt.begin(); i != m_arrApprovalDecrypt.end(); i++)
	{
		int nReadApprovalDecryptID = i->second;
		if(index == nApprovalDecrypt)
			return nReadApprovalDecryptID;
		index++;
	}
	
	return -1;
}

int CUserInfoV4::GetApprovalOutSide(int nApprovalOutSide)
{
	int index = 0;
	std::map<int,int>::iterator i;
	for (i = m_arrApprovalOutSide.begin(); i != m_arrApprovalOutSide.end(); i++)
	{
		int nApprovalOutSideID = i->second;
		if(index == nApprovalOutSide)
			return nApprovalOutSideID;
		index++;
	}
	
	return -1;
}

int CUserInfoV4::GetDepartID(int nDepart)
{
	int index = 0;
	std::map<int,CAString>::iterator i;
	for (i = m_arrDepartmentID.begin(); i != m_arrDepartmentID.end(); i++)
	{
		int nDepartID = i->first;
		if(index == nDepart)
			return nDepartID;
		index++;
	}
	
	return -1;
}

BOOL CUserInfoV4::ExistDeparement(int nDepartID)
{
	return m_arrDepartmentID.find(nDepartID) != m_arrDepartmentID.end();
}

void CUserInfoV4::AddOntDepart(int nDepartID, LPTSTR strDepartName)
{
	if (!ExistDeparement(nDepartID))
	{
		m_arrDepartmentID.insert(std::pair<int, LPTSTR>(nDepartID, strDepartName));
	}
}

BOOL CUserInfoV4::IsUserDepart(int nDepartID)
{
	std::map<int,CAString>::iterator i;
	for (i = m_arrDepartmentID.begin(); i != m_arrDepartmentID.end(); i++)
	{
		int n = i->first;
		if(nDepartID == i->first)
			return TRUE;
	}
	
	return FALSE;
}
