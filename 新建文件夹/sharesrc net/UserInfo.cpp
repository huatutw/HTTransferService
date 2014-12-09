// UserInfo.cpp: implementation of the CUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "DGAdmin.h"
#include "UserInfo.h"
#include "DGCommonFunc.h"
#include "DGVersion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserInfo::CUserInfo()
{
	m_nUserType = USER_NORMAL;
	m_nClsVersion = DG_USER_FILE3;
	m_pszUserInfo = NULL;
	m_pszUserDescribe = NULL;
	m_pszUserName = NULL;
	m_nPopedom = 0;
	m_nOverTime = 0;
	m_pszPassword = NULL;

	m_pLevel = NULL;

	m_dwCryptLevel = 0;
}

CUserInfo::~CUserInfo()
{
	if (m_pszUserInfo)
	{
		free(m_pszUserInfo);
		m_pszUserInfo = NULL;
	}

	if (m_pszUserName)
	{
		free(m_pszUserName);
		m_pszUserName = NULL;
	}

	if (m_pszUserDescribe)
	{
		free(m_pszUserDescribe);
		m_pszUserDescribe = NULL;
	}

	if (m_pszPassword)
	{
		free(m_pszPassword);
		m_pszPassword = NULL;
	}

}

void CUserInfo::ClearPolicyArray()
{
//	m_arrPolicy.clear();
}
void CUserInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_nClsVersion;

		int len = 0;
		ar >> len;
		if (len > 0)
		{
			if (m_pszUserInfo)
				free(m_pszUserInfo);
			m_pszUserInfo = (LPTSTR)calloc(len+1,sizeof(TCHAR));
			ar.Read(m_pszUserInfo,len*sizeof(TCHAR));
		}

		ar >> len;
		if (len > 0)
		{
			if (m_pszUserName)
				free(m_pszUserName);
			m_pszUserName = (LPTSTR)calloc(len+1,sizeof(TCHAR));
			ar.Read(m_pszUserName,len*sizeof(TCHAR));
		}

		ar >> len;
		if (len > 0)
		{
			if (m_pszUserDescribe)
				free(m_pszUserDescribe);
			m_pszUserDescribe = (LPTSTR)calloc(len+1,sizeof(TCHAR));
			ar.Read(m_pszUserDescribe,len*sizeof(TCHAR));
		}
		ar >> len;
		if (m_pszPassword)
		{
				free(m_pszPassword);
				m_pszPassword = NULL;
		}
		if (len > 0)
		{
			m_pszPassword = (LPTSTR)calloc(len+1,sizeof(TCHAR));
			ar.Read(m_pszPassword,len*sizeof(TCHAR));
		}


		ar >> m_nUserType;
		ar >> m_nOverTime;
		ar >> m_nPopedom;

		if (m_nClsVersion == DG_USER_FILE3)
		{
			ar >> m_dwCryptLevel;
		}else
		{

		}
	}
	else
	{
		m_nClsVersion = DG_USER_FILE3;
		ar << m_nClsVersion;

		int nLen = 0;
		if (m_pszUserInfo)
			nLen = _tcslen(m_pszUserInfo);
		else
			nLen = 0;
		ar << nLen;
		if (nLen > 0)
		{
			ar.Write(m_pszUserInfo,nLen*sizeof(TCHAR));
		}
		
		if (m_pszUserName)
			nLen = _tcslen(m_pszUserName);
		else
			nLen = 0;
		ar << nLen;
		if (nLen > 0)
		{
			ar.Write(m_pszUserName,nLen*sizeof(TCHAR));
		}


		if (m_pszUserDescribe)
			nLen = _tcslen(m_pszUserDescribe);
		else
			nLen = 0;
		ar << nLen;
		if (nLen > 0)
		{
			ar.Write(m_pszUserDescribe,nLen*sizeof(TCHAR));
		}

		if (m_pszPassword)
			nLen = _tcslen(m_pszPassword);
		else
			nLen = 0;
		ar << nLen;
		if (nLen > 0)
		{
			ar.Write(m_pszPassword,nLen*sizeof(TCHAR));
		}

		ar << m_nUserType;
		ar << m_nOverTime;
		ar << m_nPopedom;

		ar << m_dwCryptLevel;

	}
}

CDowithUserInfo::CDowithUserInfo()
{
	m_nClsVersion = 2; //2.0版本 m_nClsVersion == 1
					   //3.0版本 m_nClsVersion == 2
	m_pCurUserInfo = NULL;
}
CDowithUserInfo::~CDowithUserInfo()
{
	int i;
	int size = m_ptrUserInfo.size();
	for (i=0;i<size;i++)
	{
		CUserInfo* pInfo = m_ptrUserInfo[i];
		delete pInfo;
	}
	m_ptrUserInfo.clear();

	size = m_ptrLevelInfo.size();

	for (i=0;i<size;i++)
	{
		CUserLevel* pLevel = m_ptrLevelInfo[i];
		delete pLevel;
	}
	m_ptrLevelInfo.clear();
	m_mapUserInfo.clear();
}

LPTSTR CDowithUserInfo::GetUserLevel(LPTSTR strUser)
{
	USERINFO_DEQUE::iterator	i;
	CString			strUsrTmp, strPossTmp;

	//寻找用户名完全匹配的用户
	for (i = m_ptrUserInfo.begin(); i!=m_ptrUserInfo.end(); i++)
	{
		strUsrTmp = (*i)->m_pszUserInfo;
		if (_tcsicmp(strUsrTmp,strUser) == 0)
		{
			return (*i)->m_pLevel->m_lpszLevelName;
		}	
	}
	return NULL;
}
void CDowithUserInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_nClsVersion;

		int size = 0;
		ar >> size;
		int i;
		for (i=0;i<size;i++)
		{
			CUserInfo* pInfo = new CUserInfo;
			pInfo->Serialize(ar);
			m_ptrUserInfo.push_back(pInfo);

			m_mapUserInfo.insert(pair<CString,CUserInfo*>(pInfo->m_pszUserInfo,pInfo));

		}

		if (m_nClsVersion == USR_FILE_VER3)
		{
			int levelsize = 0;
			ar >> levelsize;
			for (int i=0;i<levelsize;i++)
			{
				CUserLevel* pLevel = new CUserLevel;
				pLevel->Serialize(ar);
				m_ptrLevelInfo.push_back(pLevel);
			}
		}

		for (i=0;i<m_ptrUserInfo.size();i++)
		{
			CUserInfo* pInfo = m_ptrUserInfo[i];
			pInfo->m_pLevel = FindLevelByNameAndValue(NULL,pInfo->m_dwCryptLevel);
		}
	}
	else
	{
		ar << m_nClsVersion;

		int size = m_ptrUserInfo.size();
		
		ar << size;
		for (int i=0;i<size;i++)
		{
			m_ptrUserInfo[i]->Serialize(ar);
		}

		if (m_nClsVersion >= USR_FILE_VER3)
		{
			int levelsize = m_ptrLevelInfo.size();
			ar << levelsize;
			for (int i=0;i<levelsize;i++)
			{
				CUserLevel* pLevel = m_ptrLevelInfo[i];
				pLevel->Serialize(ar);
			}
		}
	}
}

void CDowithUserInfo::LoadUserInfo(LPCTSTR lpszFile)
{
	CFile file(lpszFile,CFile::modeRead);
//	file.Open(lpszFile,CFile::modeRead);
	CArchive ar(&file,CArchive::load);
	Serialize(ar);
	ar.Close();
	file.Close();
}

void CDowithUserInfo::SaveUsersInfoToFile(LPCTSTR lpszFileName)
{
	CFile file(lpszFileName,CFile::modeWrite | CFile::modeCreate);
	CArchive ar(&file,CArchive::store);
	
	Serialize(ar);

	ar.Close();
	file.Close();
}

BOOL CDowithUserInfo::DeleteItem(CUserInfo *pInfo)
{
	USERINFO_DEQUE::iterator	i;
	CUserInfo		*pTemp = NULL;

	BOOL bCheck = FALSE;

	for (i = m_ptrUserInfo.begin(); i!=m_ptrUserInfo.end(); i++)
	{
		if ((*i)->m_pszUserInfo == NULL)
		{
			pTemp = (*i);
			m_ptrUserInfo.erase(i);
			delete pTemp;
			bCheck = TRUE;
		}
	}

	if (bCheck == TRUE)
	{
		return TRUE;
	}
	
	for (i = m_ptrUserInfo.begin(); i!=m_ptrUserInfo.end(); i++)
	{
		if (_tcsicmp((*i)->m_pszUserInfo,pInfo->m_pszUserInfo) == 0)
		{
			pTemp = (*i);
			m_ptrUserInfo.erase(i);
			delete pTemp;

			return TRUE;
		}
	}
	return FALSE;
}
//
//BOOL CDowithUserInfo::CheckPass2(CString strUser, CString strPassword)
//{
//	USERINFO_DEQUE::iterator	i;
//	CUserInfo		*pTemp = NULL;
//	CString			strUsrTmp, strPossTmp;
//
//	//寻找用户名完全匹配的用户
//	int size = m_mapUserInfo.size();
//	pTemp = (CUserInfo*)m_mapUserInfo[strUser];
//
//	if (NULL == pTemp)
//	{
//		return FALSE;
//	}
//
//	strUsrTmp = pTemp->m_pszUserInfo;
//
//	if (_tcscmp(strUsrTmp,strUser) == 0)
//	{
//		//如果是域模式,不检查密码
//		m_pCurUserInfo = pTemp;
//		if (GetClientCurrentVersion() & VERSION_RUNDLL_SHANGQI)
//		{
//			OutputDebugString(_T("上海汽车"));
//			if (_tcscmp(strPassword,_T("shangqi_special")) == 0)
//			{
//				return TRUE;
//			}
//		}
//		if (GetClientCurrentVersion() & VERSION_STANDARD_DOMAIN)
//		{
//			TCHAR strDomain[1024] = {0};
//			GetEnvironmentVariable(TEXT("USERDOMAIN"),strDomain,1024);
//
//			OutputDebugString(strDomain);
//			OutputDebugString(strUser);
//			OutputDebugString(strPassword);
//			extern BOOL WINAPI SSPLogonUser(LPTSTR szDomain, LPTSTR szUser, LPTSTR szPassword);
//			if (SSPLogonUser(strDomain,strUser.GetBuffer(1),strPassword.GetBuffer(1)))
//			{
//				strUser.ReleaseBuffer();
//				strPassword.ReleaseBuffer();
//				return TRUE;
//			}else
//			{
//				int error = GetLastError();
//				CString strError;
//				strError.Format(_T("Domain Pass Error Code = %d"),error);
//				OutputDebugString(strError);
//			}
//			strUser.ReleaseBuffer();
//			strPassword.ReleaseBuffer();
//
//		}else
//		{
//			if (_tcscmp(pTemp->m_pszPassword,strPassword) == 0)
//				return TRUE;
//		}
//	}
//
//	return FALSE;
//}

BOOL CDowithUserInfo::CheckPass(CString strUser, CString strPassword)
{
//	return CheckPass2(strUser,strPassword);

	USERINFO_DEQUE::iterator	i;
	CUserInfo		*pTemp = NULL;
	CString			strUsrTmp, strPossTmp;

	//寻找用户名完全匹配的用户
	for (i = m_ptrUserInfo.begin(); i!=m_ptrUserInfo.end(); i++)
	{
		strUsrTmp = (*i)->m_pszUserInfo;
		OutputDebugString((*i)->m_pszUserInfo);
		OutputDebugString((*i)->m_pszPassword);
		if (_tcscmp(strUsrTmp,strUser) == 0)
		{
			//如果是域模式,不检查密码
			m_pCurUserInfo = (*i);
			if (GetClientCurrentVersion() & VERSION_STANDARD_DOMAIN)
			{
				if (strUser.Find(TEXT("\\")) >= 0)
				return TRUE;
			}

			if (_tcscmp((*i)->m_pszPassword,strPassword) == 0)
				return TRUE;
			break;
		}	
	}

	return FALSE;
}

BOOL CDowithUserInfo::IsLawUser(CString strUser, CString strPassword)
{
	USERINFO_DEQUE::iterator	i;
	CUserInfo		*pTemp = NULL;
	CString			strUsrTmp, strPossTmp;

	//寻找用户名完全匹配的用户
	for (i = m_ptrUserInfo.begin(); i!=m_ptrUserInfo.end(); i++)
	{
		strUsrTmp = (*i)->m_pszUserInfo;
		if (_tcsicmp(strUsrTmp,strUser) == 0)
		{
			m_pCurUserInfo = (*i);
			return TRUE;
		}	
	}

	//寻找只匹配域名或者机器名的用户
	for (i = m_ptrUserInfo.begin(); i!=m_ptrUserInfo.end(); i++)
	{
		strUsrTmp = (*i)->m_pszUserInfo;
		int pos = strUsrTmp.Find(TEXT("\\"));
		if (pos == -1)
		{
			pos = strUsrTmp.Find(TEXT("//"));
		}

		if (pos != -1)
			strUsrTmp = strUsrTmp.Left(pos);

		pos = strUser.Find(TEXT("\\"));
		if (pos == -1)
		{
			pos = strUser.Find(TEXT("//"));
		}
		
		if (pos != -1)
			strUser = strUser.Left(pos);

		if (_tcsicmp(strUsrTmp,strUser) == 0)
		{
			m_pCurUserInfo = (*i);
			return TRUE;
		}	
	}

	for (i = m_ptrUserInfo.begin(); i!=m_ptrUserInfo.end(); i++)
	{
		if (_tcsicmp((*i)->m_pszUserName,TEXT("默认用户")) == 0)
		{
			m_pCurUserInfo = (*i);
			return TRUE;
		}
	}
//	AfxMessageBox(_T("无效用户"));
	return FALSE;
}

void CDowithUserInfo::DeleteLevelFromList(CUserLevel* pUserLevel)
{
	LEVEL_DEQUE::iterator i;
	for (i=m_ptrLevelInfo.begin();i!=m_ptrLevelInfo.end();++i)
	{
		if ((*i) == pUserLevel)
		{
			m_ptrLevelInfo.erase(i);
			break;
		}
	}
	
	for (i=m_ptrLevelInfo.begin();i!=m_ptrLevelInfo.end();++i)
	{
		if ((*i) == pUserLevel)
		{
			m_ptrLevelInfo.erase(i);
			break;
		}
	}
}

void CDowithUserInfo::InitFirstusr()
{
	CUserInfo	*pUserInfo = new CUserInfo;

	pUserInfo->m_pszUserDescribe = (LPTSTR)calloc(3,sizeof(TCHAR));
	pUserInfo->m_pszUserInfo = (LPTSTR)calloc(3,sizeof(TCHAR));
	pUserInfo->m_pszUserName = (LPTSTR)calloc(3,sizeof(TCHAR));
	if (pUserInfo->m_pszPassword)
	{
		free(pUserInfo->m_pszPassword);
		pUserInfo->m_pszPassword = NULL;
	}
	pUserInfo->m_pszPassword = (LPTSTR)calloc(6,sizeof(TCHAR));
	pUserInfo->m_nUserType = USER_SA;

	_tcscpy(pUserInfo->m_pszUserDescribe, TEXT("sa"));
	_tcscpy(pUserInfo->m_pszUserInfo, TEXT("sa"));
	_tcscpy(pUserInfo->m_pszPassword , TEXT("admin"));
	_tcscpy(pUserInfo->m_pszUserName,TEXT("sa"));

	m_ptrUserInfo.push_back(pUserInfo);
}

void CDowithUserInfo::GetMaxUserID()
{
//	int MaxID = 0;
//	for (int i=0;i<m_ptrUserInfo.size();i++)
//	{
//		CUserInfo* pInfo = m_ptrUserInfo[i];
//		if ((pInfo->m_dwCryptLevel & 0xffff) >= MaxID)
//		{
//			MaxID = pInfo->m_dwCryptLevel & 0xffff;
//		}
//	}
//	m_nCurUserID = MaxID+1;
}

CUserLevel* CDowithUserInfo::FindLevelByNameAndValue(LPCTSTR strName,DWORD dwCryptLevel)
{
	for (int i=0;i<m_ptrLevelInfo.size();i++)
	{
		CUserLevel* pLevel = m_ptrLevelInfo[i];
		if ((strName && _tcscmp(pLevel->m_lpszLevelName,strName) == 0) || dwCryptLevel == pLevel->m_dwCryptLevel)
		{
			return pLevel;
		}
	}
	return NULL;
}

CUserLevel::CUserLevel()
{
	m_lpszLevelName = NULL;
	m_dwCryptLevel = 0;
	m_nPolicyType = 1;
}

CUserLevel::~CUserLevel()
{
	if (m_lpszLevelName)
		free(m_lpszLevelName);

	m_arrOpenPolicy.clear();
	m_arrDownPolicy.clear();
}

void CUserLevel::Serialize(CArchive& ar)
{
	int i;
	if (ar.IsLoading())
	{
		ar >> m_dwCryptLevel;
		int len;
		ar >> len;
		if (len > 0)
		{
			m_lpszLevelName = (LPTSTR)calloc(len+1,sizeof(TCHAR));
			ar.Read(m_lpszLevelName,len*sizeof(TCHAR));
		}

		int nLoadType = 0;
		ar >> nLoadType;
		int size = 0;
		ar >> size;
		
		for (i=0;i<size;i++)
		{
			int policy;
			ar >> policy;
			m_arrOpenPolicy.push_back(policy);
		}
		ar >> size;
		for (i=0;i<size;i++)
		{
			int policy;
			ar >> policy;
			m_arrDownPolicy.push_back(policy);
		}

		if (nLoadType >= 1)
		{
			ar >> size;
			for (i=0;i<size;i++)
			{
				int policy;
				ar >> policy;
				m_arrPrintPolicy.push_back(policy);
			}

			ar >> size;
			for (i=0;i<size;i++)
			{
				int policy;
				ar >> policy;
				m_arrCryptPolicy.push_back(policy);
			}
		}
	}else
	{
		ar << m_dwCryptLevel;

		int len = 0;
		if (m_lpszLevelName)
			len = _tcslen(m_lpszLevelName);
		ar << len;
		if (len > 0)
		{
			ar.Write(m_lpszLevelName,len*sizeof(TCHAR));
		}

		ar << m_nPolicyType;
		int size = m_arrOpenPolicy.size();
		ar << size;
		for (int i=0;i<size;i++)
		{
			ar << m_arrOpenPolicy[i];
		}

		size = m_arrDownPolicy.size();
		ar << size;
		for (i=0;i<size;i++)
		{
			ar << m_arrDownPolicy[i];
		}
		if (m_nPolicyType >= 1)
		{
			size = m_arrPrintPolicy.size();
			ar << size;
			for (i=0;i<size;i++)
			{
				ar << m_arrPrintPolicy[i];
			}

			size = m_arrCryptPolicy.size();
			ar << size;
			for (i=0;i<size;i++)
			{
				ar << m_arrCryptPolicy[i];
			}
		}

	}
}

void CDowithUserInfo::CheckSelf()
{
	int size = m_ptrUserInfo.size();
	for (int i=0;i<size;i++)
	{
		if (!FindLevelByNameAndValue(NULL,m_ptrUserInfo[i]->m_dwCryptLevel))
		{	
			m_ptrUserInfo[i]->m_dwCryptLevel = 0;
		}
	}
}