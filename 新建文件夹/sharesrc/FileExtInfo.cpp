// FileExtInfo.cpp: implementation of the CFileExtInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileExtInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileExtInfo::CFileExtInfo()
{
	m_nType = ITEM_EXT;
}

CFileExtInfo::~CFileExtInfo()
{
	int size = m_FileExtDeque.size();
	for (int i=0;i<size;i++)
	{
		char* lpszInfo = m_FileExtDeque.front();
		m_FileExtDeque.pop_front();
		free(lpszInfo);
	}	
}

BOOL CFileExtInfo::FindExtByName(const char* lpszExtName)
{
	for (int i=0;i<m_FileExtDeque.size();i++)
	{
		if (stricmp(m_FileExtDeque[i],"*") == 0 || stricmp(m_FileExtDeque[i],lpszExtName) == 0)
			return TRUE;
	}
	return FALSE;
}

BOOL CFileExtInfo::IsExtNameValid(const char* lpszExtName)
{
	return TRUE;
}

BOOL CFileExtInfo::AddOneExtName(const char* lpszExtName)
{
	if (!IsExtNameValid(lpszExtName))
		return FALSE;
	if (FindExtByName(lpszExtName))
		return FALSE;
	m_FileExtDeque.push_front(strdup(lpszExtName));
	return TRUE;
}

#ifndef NO_MFC_VERSION
void CFileExtInfo::Serialize(CArchive& ar)
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
				char* lpszExtName = (char*)calloc(len+1,sizeof(char));
				ar.Read(lpszExtName,len);
				
				m_FileExtDeque.push_back(lpszExtName);
			}
		}
	}else
	{
		ar << m_clsVersion;

		int size = m_FileExtDeque.size();
		ar << size;
		for (int i=0;i<m_FileExtDeque.size();i++)
		{
			int len = strlen(m_FileExtDeque[i]);
			ar << len;
			if (len > 0)
			{
				ar.Write(m_FileExtDeque[i],len);
			}
		}
	}
}

void CFileExtInfo::Serialize_Unicode(CArchive& ar)
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
			
				LPWSTR ExtName = (LPWSTR)calloc(len+1,sizeof(WCHAR));

				ar.Read(ExtName,len*sizeof(WCHAR));
				
				LPSTR tmp = W2A(ExtName);
				char* lpszExtName = (char*)calloc(strlen(tmp)+1,sizeof(char));
				strcpy(lpszExtName,tmp);
				
				m_FileExtDeque.push_back(lpszExtName);
				free(ExtName);
			}
		}
	}else
	{
		ar << m_clsVersion;

		int size = m_FileExtDeque.size();
		ar << size;
		for (int i=0;i<m_FileExtDeque.size();i++)
		{
			int len = strlen(m_FileExtDeque[i]);
			ar << len;
			if (len > 0)
			{
				LPWSTR ExtName = A2W(m_FileExtDeque[i]);
				ar.Write(ExtName,len*sizeof(WCHAR));
			}
		}
	}
}
#else
void CFileExtInfo::Serialize(HANDLE hFile,BOOL bRead)
{
	if (bRead)
	{
		DWORD read = 0;
		ReadFile(hFile,&m_clsVersion,sizeof(int),&read,NULL);
		int size;
		ReadFile(hFile,&size,sizeof(int),&read,NULL);
		for (int i=0;i<size;i++)
		{
			int len = 0;
			ReadFile(hFile,&len,sizeof(int),&read,NULL);
			if (len > 0)
			{
				char* lpszExtName = (char*)calloc(len+1,sizeof(char));
				ReadFile(hFile,lpszExtName,len,&read,NULL);
				m_FileExtDeque.push_back(lpszExtName);
			}
		}
	}else
	{
	}
}

void CFileExtInfo::Serialize_Unicode(HANDLE hFile,BOOL bRead)
{
	USES_CONVERSION;
	if (bRead)
	{
		DWORD read = 0;
		ReadFile(hFile,&m_clsVersion,sizeof(int),&read,NULL);
		int size;
		ReadFile(hFile,&size,sizeof(int),&read,NULL);
		for (int i=0;i<size;i++)
		{
			int len = 0;
			ReadFile(hFile,&len,sizeof(int),&read,NULL);
			if (len > 0)
			{					
				LPWSTR ExtName = (LPWSTR)calloc(len+1,sizeof(WCHAR));

				ReadFile(hFile,ExtName,len*sizeof(WCHAR),&read,NULL);
				
				LPSTR tmp = W2A(ExtName);
				char* lpszExtName = (char*)calloc(strlen(tmp)+1,sizeof(char));
				strcpy(lpszExtName,tmp);
				
				m_FileExtDeque.push_back(lpszExtName);
				free(ExtName);
			}
		}
	}else
	{
	}
}
#endif

int wildcmp(LPCSTR wild, LPCSTR string) {
  // Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
	  LPCSTR cp = NULL, mp = NULL;

	  while ((*string) && (*wild != '*')) {
		if ((*wild != *string) && (*wild != '?')) {
		  return 0;
		}
		wild++;
		string++;
	  }

	  while (*string) {
		if (*wild == '*') {
		  if (!*++wild) {
			return 1;
		  }
		  mp = wild;
		  cp = string+1;
		} else if ((*wild == *string) || (*wild == '?')) {
		  wild++;
		  string++;
		} else {
		  wild = mp;
		  string = cp++;
		}
	  }

	  while (*wild == '*') {
		wild++;
	  }
	  return !*wild;
}

int wildicmp(LPCTSTR wild, LPCTSTR string)
{
	USES_CONVERSION;
	LPTSTR w = _tcslwr(_tcsdup(wild));
	LPTSTR s = _tcslwr(_tcsdup(string));

//	AfxMessageBox(w);
//	AfxMessageBox(s);

	OutputDebugString(s);
	OutputDebugString(TEXT("\n"));
	return wildcmp(T2A(w),T2A(s));
}

BOOL CFileExtInfo::CheckExt(const char* lpszFileName)
{
	if(lpszFileName == NULL)
		return FALSE;
	LPCSTR lpStr = strrchr(lpszFileName, '.');
	if(lpStr == NULL)
		lpStr = lpszFileName;
	else 
		lpStr++;

	//如果后缀匹配,或者配置中有*,则表示匹配

	BOOL bSpec = (stricmp(lpStr,"NOCONTROL") == 0) || (stricmp(lpStr,"HOOKLOAD") == 0);
	BOOL bMatch = FALSE;
	for (int i=0;i<m_FileExtDeque.size();i++)
	{
		if (bSpec)
		{
			if (stricmp(m_FileExtDeque[i],lpStr) == 0)
			{
				bMatch = TRUE;
				break;
			}
		}else
		{
			if (stricmp(m_FileExtDeque[i],"*") == 0 || stricmp(m_FileExtDeque[i],lpStr) == 0)
			{
				bMatch = TRUE;
				break;
			}
		}
	}

	//查看排除列表
	if (bMatch)
	{
		for (i=0;i<m_FileExtDeque.size();i++)
		{
			char Ext[1024] = {0};
			strcpy(Ext,"!#!#");
			strcat(Ext,lpStr);
			if (stricmp(m_FileExtDeque[i],Ext) == 0)
				return FALSE;
		}
	}

	return bMatch;
}

/*
BOOL CFileExtInfo::CheckExt(const char* lpszFileName)
{
	LPCSTR lpStr = _tcsrchr(lpszFileName, '\\');

	if (lpStr == NULL)
	{
		return FALSE;
	}else
	{
		lpStr++;
	}
	//如果后缀匹配,或者配置中有*,则表示匹配
	BOOL bMatch = FALSE;
	for (int i=0;i<m_FileExtDeque.size();i++)
	{
		if (wildicmp(m_FileExtDeque[i],lpStr))
		{
			bMatch = TRUE;
			break;
		}
	}

	//查看排除列表
	if (bMatch)
	{
		for (i=0;i<m_FileExtDeque.size();i++)
		{
			char Ext[1024] = {0};
			strcpy(Ext,"!#!#");
			strcat(Ext,lpStr);
			if (wildicmp("!#!#*",m_FileExtDeque[i]) && wildicmp(m_FileExtDeque[i],Ext))
				return FALSE;
		}
	}

	return bMatch;
}*/

BOOL CFileExtInfo::DelExtInfo(char *lpszExtName)
{
	STRING_DEQUE::iterator i;
	for (i=m_FileExtDeque.begin();i!=m_FileExtDeque.end();++i)
	{
		if (stricmp((*i),lpszExtName) == 0)
		{
			free(*i);
			m_FileExtDeque.erase(i);
			
			return TRUE;
		}
	}
	
	return FALSE;
}
