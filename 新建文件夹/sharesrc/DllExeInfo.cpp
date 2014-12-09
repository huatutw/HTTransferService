// DllExeInfo.cpp: implementation of the CDllExeInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DllExeInfo.h"
#include "ModulVer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDllExeInfo::CDllExeInfo()
{
	m_nType = ITEM_APP;
	m_nCheckNum = 0;
	m_bInitCheck = FALSE;
}

CDllExeInfo::~CDllExeInfo()
{
	int size = m_ConfigDeque.size();
	for (int i=0;i<size;i++)
	{
		DllExeInfo* pInfo = m_ConfigDeque.front();
		m_ConfigDeque.pop_front();
		free(pInfo->lpszModuleName);
		delete pInfo;
	}
	m_nCheckNum = 0;
}

BOOL CDllExeInfo::AddDllExeInfo(const char* lpszModuleName)
{
	if (!IsModuleValid(lpszModuleName))
		return FALSE;

	if (FindDllExeInfo(lpszModuleName))
		return FALSE;

	DllExeInfo* pInfo = new DllExeInfo;
	memset(pInfo,0,sizeof(DllExeInfo));

	pInfo->bChecked = FALSE;
	pInfo->lpszModuleName = strdup(lpszModuleName);

	m_ConfigDeque.push_front(pInfo);
	return TRUE;
}

void CDllExeInfo::DeleteDllExeInfo(const char* lpszModuleName)
{
	MODULE_DEQUE::iterator i;
	for (i=m_ConfigDeque.begin();i!=m_ConfigDeque.end();++i)
	{
		if (stricmp((*i)->lpszModuleName,lpszModuleName) == 0)
		{
			free((*i)->lpszModuleName);
			m_ConfigDeque.erase(i);
			break;
		}
	}
}

BOOL CDllExeInfo::IsModuleValid(const char* lpszModuleName)
{
	int len = 0;
	len = strlen(lpszModuleName);
	if (len <= 4) //不能加入小于等于4个字节的模块名 即如1.exe,1.dll是合法的配置模块
		return FALSE;

	if (stricmp(".exe",&lpszModuleName[len-4]) == 0 ||
		stricmp(".dll",&lpszModuleName[len-4]) == 0)
		return TRUE;

	return FALSE;
}

DllExeInfo* CDllExeInfo::FindDllExeInfo(const char* lpszModuleName)
{
	if (!IsModuleValid(lpszModuleName))
		return NULL;

	for (int i=0;i<m_ConfigDeque.size();i++)
	{
		if (stricmp(m_ConfigDeque[i]->lpszModuleName,lpszModuleName) == 0)
			return m_ConfigDeque[i];

	}
	return NULL;
}

void CDllExeInfo::CheckDllExeInfo(const char* lpszModuleName)
{
	DllExeInfo* pDllExeInfo = FindDllExeInfo(lpszModuleName);
	if (pDllExeInfo && !pDllExeInfo->bChecked)
	{
		pDllExeInfo->bChecked = TRUE;
		m_nCheckNum++;
	}
}

BOOL CDllExeInfo::IsAllModuleCheckSuccess()
{
	if (m_ConfigDeque.size() == m_nCheckNum)
		return TRUE;
	return FALSE;
}

#ifndef NO_MFC_VERSION
void CDllExeInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_clsVersion;

		int size;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			int len = 0;
			ar >> len;
			if (len > 0)
			{
				DllExeInfo* pInfo = new DllExeInfo;
				pInfo->lpszModuleName = (char*)calloc(len+1,sizeof(char));
				ar.Read(pInfo->lpszModuleName,len);
				pInfo->bChecked = FALSE;
				m_ConfigDeque.push_back(pInfo);
			}
		}
	}else
	{
		ar << m_clsVersion;

		int size = m_ConfigDeque.size();
		ar << size;
		for (int i=0;i<m_ConfigDeque.size();i++)
		{
			int len = strlen(m_ConfigDeque[i]->lpszModuleName);
			ar << len;
			if (len > 0)
			{
				ar.Write(m_ConfigDeque[i]->lpszModuleName,len);
			}
		}
	}
}

void CDllExeInfo::Serialize_Unicode(CArchive& ar)
{
	USES_CONVERSION;
	if (ar.IsLoading())
	{
		ar >> m_clsVersion;

		int size;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			int len = 0;
			ar >> len;
			if (len > 0)
			{
				DllExeInfo* pInfo = new DllExeInfo;

				LPWSTR ModuleName = (LPWSTR)calloc(len+1,sizeof(WCHAR));

				ar.Read(ModuleName,len*sizeof(WCHAR));

				LPSTR tmp = W2A(ModuleName);
				pInfo->lpszModuleName = (char*)calloc(strlen(tmp)+1,sizeof(char));
				strcpy(pInfo->lpszModuleName,tmp);
				free(ModuleName);
				pInfo->bChecked = FALSE;
				m_ConfigDeque.push_back(pInfo);
			}
		}
	}else
	{
		ar << m_clsVersion;

		int size = m_ConfigDeque.size();
		ar << size;
		for (int i=0;i<m_ConfigDeque.size();i++)
		{
			int len = strlen(m_ConfigDeque[i]->lpszModuleName);
			ar << len;
			
			if (len > 0)
			{
				LPWSTR ModuleName = A2W(m_ConfigDeque[i]->lpszModuleName);
				ar.Write(ModuleName,len*sizeof(WCHAR));
			}
		}
	}
}
#else
void CDllExeInfo::Serialize_Unicode(HANDLE hFile,BOOL bRead)
{
	USES_CONVERSION;
	if (bRead)
	{
		DWORD read;
		ReadFile(hFile,&m_clsVersion,sizeof(int),&read,NULL);

		int size;
		ReadFile(hFile,&size,sizeof(int),&read,NULL);
		for (int i=0;i<size;i++)
		{
			int len = 0;
			ReadFile(hFile,&len,sizeof(int),&read,NULL);
			if (len > 0)
			{
				DllExeInfo* pInfo = new DllExeInfo;

				LPWSTR ModuleName = (LPWSTR)calloc(len+1,sizeof(WCHAR));

				ReadFile(hFile,ModuleName,len*sizeof(WCHAR),&read,NULL);

				LPSTR tmp = W2A(ModuleName);
				pInfo->lpszModuleName = (char*)calloc(strlen(tmp)+1,sizeof(char));
				strcpy(pInfo->lpszModuleName,tmp);
				free(ModuleName);

				pInfo->bChecked = FALSE;
				m_ConfigDeque.push_back(pInfo);
			}
		}
	}else
	{
	}
}

void CDllExeInfo::Serialize(HANDLE hFile,BOOL bRead)
{
	if (bRead)
	{
		DWORD read;
		ReadFile(hFile,&m_clsVersion,sizeof(int),&read,NULL);

		int size;
		ReadFile(hFile,&size,sizeof(int),&read,NULL);
		for (int i=0;i<size;i++)
		{
			int len = 0;
			ReadFile(hFile,&len,sizeof(int),&read,NULL);
			if (len > 0)
			{
				DllExeInfo* pInfo = new DllExeInfo;
				pInfo->lpszModuleName = (char*)calloc(len+1,sizeof(char));
				ReadFile(hFile,pInfo->lpszModuleName,len,&read,NULL);
				pInfo->bChecked = FALSE;
				m_ConfigDeque.push_back(pInfo);
			}
		}
	}else
	{
	}
}
#endif

BOOL CDllExeInfo::CheckModule(const char* lpszModuleName)
{
	InitCheckNum();
	for (int i=0;i<m_ConfigDeque.size();i++)
	{
		DllExeInfo* pInfo = m_ConfigDeque[i];
		if (pInfo->bChecked == FALSE && stricmp(pInfo->lpszModuleName,lpszModuleName) == 0)
		{
			m_nCheckNum++;
			pInfo->bChecked = TRUE;
		}
	}
	if (m_nCheckNum == m_ConfigDeque.size() && m_nCheckNum != 0 )
		return TRUE;

	return FALSE;
}

BOOL CDllExeInfo::DelDllExeInfo(char *lpszModuleName)
{
	MODULE_DEQUE::iterator i;
	for (i=m_ConfigDeque.begin();i!=m_ConfigDeque.end();++i)
	{
		if (stricmp((*i)->lpszModuleName,lpszModuleName) == 0)
		{
			DllExeInfo	*pInfo;
			pInfo = *i;
			m_ConfigDeque.erase(i);

			free(pInfo->lpszModuleName);
			delete pInfo;

			return TRUE;
		}
	}
	
	return FALSE;
}

void CDllExeInfo::InitCheckNum()
{
	if (!m_bInitCheck)
	{
		for (int i=0;i<m_ConfigDeque.size();i++)
		{
			DllExeInfo* pInfo = m_ConfigDeque[i];
			USES_CONVERSION;

			int pos1 = STR_Find((LPCTSTR)pInfo->lpszModuleName,_T("::"),0);
			int pos2 = STR_Find((LPCTSTR)pInfo->lpszModuleName,_T("%%"),0);

			if (pos1 >=0 || pos2 >= 0)
			{
				if (pos1 >= 0) //OriginalFilename::Splash.EXE
				{
					//char* StringID = STR_Left(pInfo->lpszModuleName,pos1);
					char StringID[512] = {0};
					_tcscpy(A2T(StringID), STR_Left((LPCTSTR)pInfo->lpszModuleName, pos1));
					char StringValue[128];
					strcpy(StringValue,pInfo->lpszModuleName+pos1+2);

					TCHAR ResBuffer[1024] = {0};
					
					TCHAR buffer[2048] = {0};
					GetModuleFileName(NULL, buffer, _MAX_PATH);
					CModuleVersion ver;
					ver.GetFileVersionInfo(buffer);
					char value[128] = {0};
					ver.GetValue(A2T(StringID),value);
					if (strcmp(StringValue,value) == 0)
					{
						m_nCheckNum++;
					}

				}else
				{
					//char* StringID = STR_Left(pInfo->lpszModuleName,pos2);
					char StringID[512] = {0};
					_tcscpy(A2T(StringID), STR_Left((LPCTSTR)pInfo->lpszModuleName, pos2));
					char StringValue[128];
					strcpy(StringValue,pInfo->lpszModuleName+pos2+2);
					int ID = atoi(StringID);
					char ResStringValue[128];
					if (LoadStringA(NULL,ID,ResStringValue,128) != 0)
					{
						if (strcmp(ResStringValue,StringValue) == 0)
						{
							m_nCheckNum++;
						}
					}				
				}
			}
		}
		
		m_bInitCheck = TRUE;
	}
}