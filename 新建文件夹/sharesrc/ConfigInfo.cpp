// ConfigInfo.cpp: implementation of the CConfigInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigInfo.h"
#include "shfolder.h"

/////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////////////

CConfigInfo::CConfigInfo()
{
	m_pCurSoftType = NULL;
	m_pCurSoftware = NULL;
	m_clsVersion = 0;
}

CConfigInfo::~CConfigInfo()
{
	int size = m_arrSoftType.size();
	for (int i=0;i<size;i++)
	{
		CSoftwareTypeInfo* pInfo = m_arrSoftType[i];
		delete pInfo;
	}
	m_arrSoftType.clear();
}

CSoftwareTypeInfo* CConfigInfo::FindSoftTypeByName(const char* lpszSoftType)
{
	for (int i=0;i<m_arrSoftType.size();i++)
	{
		if (stricmp(m_arrSoftType[i]->m_lpszSoftTypeName,lpszSoftType) == 0)
		{
			return m_arrSoftType[i];
		}
	}
	return NULL;
}

#define swap_byte(x,y) t = *(x); *(x) = *(y); *(y) = t

typedef struct rc4_key
{      
   unsigned char state[256];       
   unsigned char x;        
   unsigned char y;
} rc4_key;

void prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key)
{
	int i;
	unsigned char t;
	unsigned char swapByte;
	unsigned char index1;
	unsigned char index2;
	unsigned char* state;
	short counter;

	state = &key->state[0];
	for(counter = 0; counter < 256; counter++)
	state[counter] = counter;
	key->x = 0;
	key->y = 0;
	index1 = 0;
	index2 = 0;
	for(counter = 0; counter < 256; counter++)
	{
		index2 = (key_data_ptr[index1] + state[counter] + index2) % 256;
		swap_byte(&state[counter], &state[index2]);
		index1 = (index1 + 1) % key_data_len;
	}
}

void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *key)
{
  unsigned char t;
  unsigned char x;
  unsigned char y;
  unsigned char* state;
  unsigned char xorIndex;
  short counter;

  x = key->x;
  y = key->y;
  state = &key->state[0];
  for(counter = 0; counter < buffer_len; counter++)
  {
    x = (x + 1) % 256;
    y = (state[x] + y) % 256;
    swap_byte(&state[x], &state[y]);
    xorIndex = (state[x] + state[y]) % 256;
    buffer_ptr[counter] ^= state[xorIndex];
  }
  key->x = x;
  key->y = y;
}

BOOL Encrypt_RC4(LPBYTE lpszBuffer,DWORD dwSize,char* lpszPassword)
{
	if (lpszPassword == NULL || strlen(lpszPassword) == 0)
		return FALSE;
	rc4_key key;
	prepare_key((LPBYTE)lpszPassword,strlen(lpszPassword),&key);
	rc4(lpszBuffer,dwSize,&key);
	return TRUE;
}

PFNSHGETFOLDERPATHA GetFuncPtr_SHGetFolderPathA()
{
   static HMODULE hMod = NULL;
   PFNSHGETFOLDERPATHA pSHGetFolderPath = NULL;

   // Load SHFolder.dll only once
   if (!hMod)
      hMod = LoadLibrary(_T("SHFolder.dll"));

   if (hMod)
      // Obtain a pointer to the SHGetFolderPathA function
      pSHGetFolderPath = (PFNSHGETFOLDERPATHA)GetProcAddress(hMod, 
         "SHGetFolderPathA");

   return pSHGetFolderPath;
}

BOOL CConfigInfo::LoadConfigInfoFromFile(const char* lpszFileName)
{
	WIN32_FIND_DATAA data;
	HANDLE hFind = FindFirstFileA(lpszFileName,&data);
	if (hFind == NULL || hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}else
	{
		FindClose(hFind);
	}
	char strTmpFile[1024] = {0};
	DWORD dwProcessID = GetCurrentProcessId();

	char TempPath[MAX_PATH];

	PFNSHGETFOLDERPATHA pSHGetFolderPath = NULL;

	pSHGetFolderPath = GetFuncPtr_SHGetFolderPathA();

	BOOL bFound = FALSE;
	if (pSHGetFolderPath)
	{
		bFound = SUCCEEDED(pSHGetFolderPath(NULL, CSIDL_TEMPLATES, NULL, 0, TempPath));
	}

	if (!bFound)
	{
		CreateDirectory(_T("c:\\temp\\"),NULL);
		CreateDirectory(_T("c:\\temp"),NULL);

		sprintf(strTmpFile,"c:\\temp\\%d.cfg",dwProcessID);
	}else
	{
		sprintf(strTmpFile,"%s\\%d.cfg",TempPath,dwProcessID);
	}

	
	
	if (!CopyFileA(lpszFileName,strTmpFile,FALSE))
	{
		return FALSE;
	}

	HANDLE hCFGFile = CreateFileA(strTmpFile,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hCFGFile != NULL && hCFGFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hCFGFile,NULL);
		
		LPBYTE bt = new BYTE[dwFileSize];
		DWORD read;
		ReadFile(hCFGFile,bt,dwFileSize,&read,NULL);
		int nVersion;
		memcpy(&nVersion,bt,sizeof(int));
		if (nVersion != 0)
		{
			Encrypt_RC4(bt,dwFileSize,"DocGuarder_Encrypt_System_Made_in_HangZhou_2007_4_18");
			SetFilePointer(hCFGFile,0,0,FILE_BEGIN);
			WriteFile(hCFGFile,bt,dwFileSize,&read,NULL);
		}
		CloseHandle(hCFGFile);
		delete [] bt;
	}

	USES_CONVERSION;
#ifndef NO_MFC_VERSION

	CFile file(A2T(strTmpFile),CFile::modeRead);
	CArchive ar(&file,CArchive::load);
	
	Serialize(ar);

	ar.Close();
	file.Close();
#else
	HANDLE hFile = CreateFileA(strTmpFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	Serialize(hFile,TRUE);
	CloseHandle(hFile);
#endif
	DeleteFileA(strTmpFile);
	return TRUE;
}

void CConfigInfo::SaveConfigInfoToFile(const char* lpszFileName)
{
#ifndef NO_MFC_VERSION
	USES_CONVERSION;
	CFile file(A2T((char*)lpszFileName),CFile::modeWrite | CFile::modeCreate);
	CArchive ar(&file,CArchive::store);
	
	Serialize(ar);

	ar.Close();
	file.Close();
#else
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	Serialize(hFile,TRUE);
	CloseHandle(hFile);
#endif

	HANDLE hCFGFile = CreateFileA(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hCFGFile != NULL && hCFGFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hCFGFile,NULL);
		
		LPBYTE bt = new BYTE[dwFileSize];
		DWORD read;
		ReadFile(hCFGFile,bt,dwFileSize,&read,NULL);
		Encrypt_RC4(bt,dwFileSize,"DocGuarder_Encrypt_System_Made_in_HangZhou_2007_4_18");
		SetFilePointer(hCFGFile,0,0,FILE_BEGIN);
		WriteFile(hCFGFile,bt,dwFileSize,&read,NULL);
		CloseHandle(hCFGFile);
		delete[] bt;
	}
}

#ifndef NO_MFC_VERSION
BOOL CConfigInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_clsVersion ;

//		if (m_clsVersion != 0)
//			return FALSE;
		switch(m_clsVersion)
		{
		case 0://老版本的配置文件
			{
				int size = 0;
				ar >> size;
				for (int i=0;i<size;i++)
				{
					CSoftwareTypeInfo* pInfo = new CSoftwareTypeInfo;
					pInfo->Serialize(ar);
					m_arrSoftType.push_back(pInfo);
				}
			}
			break;
		case 1:
			{
				int size = 0;
				ar >> size;
				for (int i=0;i<size;i++)
				{
					CSoftwareTypeInfo* pInfo = new CSoftwareTypeInfo;
					pInfo->Serialize_Unicode(ar);
					m_arrSoftType.push_back(pInfo);
				}
			}
			break;
		}

	}else
	{
		ar << m_clsVersion;

		int size=0;
		for (int n=0;n<m_arrSoftType.size();n++)
		{
			if (m_arrSoftType[n]->m_bSaveThisType)		
			{
				size++;
			}
		}
		ar << size;
		size = m_arrSoftType.size();
		for (int i=0;i<m_arrSoftType.size();i++)
		{
			if (m_arrSoftType[i]->m_bSaveThisType)		// 在导出配置中用
				m_arrSoftType[i]->Serialize(ar);
		}
	}
	return TRUE;
}
#else
BOOL CConfigInfo::Serialize(HANDLE hFile,BOOL bRead)
{
	if (bRead)
	{
		DWORD read;
		ReadFile(hFile,&m_clsVersion,sizeof(int),&read,NULL);
//		if (m_clsVersion != 0)
//			return FALSE;
		int size = 0;
		ReadFile(hFile,&size,sizeof(int),&read,NULL);

		switch (m_clsVersion)
		{
		case 0:
			{
				for (int i=0;i<size;i++)
				{
					CSoftwareTypeInfo* pInfo = new CSoftwareTypeInfo;
					pInfo->Serialize(hFile,bRead);
					m_arrSoftType.push_back(pInfo);
				}
			}
			break;
		case 1:
			{
				for (int i=0;i<size;i++)
				{
					CSoftwareTypeInfo* pInfo = new CSoftwareTypeInfo;
					pInfo->Serialize_Unicode(hFile,bRead);
					m_arrSoftType.push_back(pInfo);
				}
			}
			break;
		}

	}else
	{

	}
	return TRUE;
}
#endif	

BOOL CConfigInfo::CheckModule(const char* lpszModuleName)
{
	if (m_pCurSoftware)
		return TRUE;
	
	LPCSTR lpMdName = strrchr(lpszModuleName,'\\');	//只取文件名进行判断，去掉路径
	if(lpMdName == NULL)
		lpMdName = lpszModuleName;
	else
		lpMdName++;

	for (int i=0;i<m_arrSoftType.size();i++)
	{
		CSoftwareVersionInfo* pInfo = m_arrSoftType[i]->CheckModule(lpMdName);
		if (pInfo)
		{
			m_pCurSoftType = m_arrSoftType[i];
			m_pCurSoftware = pInfo;
			return TRUE;
		}else
		{
	
		}
	}
	return NULL;
}

BOOL CConfigInfo::DeleteItem(CSoftwareTypeInfo *pInfo)
{
	SOFTTYPE_DEQUE::iterator	i;
	CSoftwareTypeInfo			*pTemp = NULL;

	for (i=m_arrSoftType.begin(); i!=m_arrSoftType.end(); i++)
	{
		if (stricmp((*i)->m_lpszSoftTypeName,pInfo->m_lpszSoftTypeName) == 0)
		{
			pTemp = (*i);
			m_arrSoftType.erase(i);
			delete pTemp;

			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CConfigInfo::CheckOleCLSID(CLSID clsid)
{
	for (int i=0;i<m_arrSoftType.size();i++)
	{
		if (m_arrSoftType[i]->CheckOleCLSID(clsid))
			return TRUE;
	}
	return FALSE;
}

BOOL SaveConfigToXML(const char* lpszFileName)
{
	return TRUE;
}

BOOL LoadConfigFromXML(const char* lpszFileName)
{
	return TRUE;
}

BOOL CConfigInfo::IsMailAddressCanSend(const char* lpszMail,BOOL bSender)
{
	if (lpszMail == NULL)
		return FALSE;

	char* strMail = strlwr(strdup(lpszMail));

	//双友集团的内部信任邮箱
/*	if (strstr(strMail,"@cntopsun.com") || strstr(strMail,"@cntopsungroup.com"))
	{
		free(strMail);
		return TRUE;
	}*/

	CSoftwareTypeInfo* pTypeInfo = FindSoftTypeByName("E-Mail");

	if (pTypeInfo == NULL)
		return FALSE;

	for (int i=0;i<pTypeInfo->m_arrSoftVersion.size();i++)
	{
		CSoftwareVersionInfo* pVerInfo = pTypeInfo->m_arrSoftVersion[i];
		for (int j=0;j<pVerInfo->m_pFileExtInfo->m_FileExtDeque.size();j++)
		{
			if (bSender)
			{
				char buffer[1024] = {0};
				strcpy(buffer,"!!");
				strcat(buffer,lpszMail);

				if (stricmp(pVerInfo->m_pFileExtInfo->m_FileExtDeque[j],buffer) == 0)
					return TRUE;
			}else
			{
				char buffer[1024] = {0};
				strcpy(buffer,"@@");
				strcat(buffer,lpszMail);

				if (stricmp(pVerInfo->m_pFileExtInfo->m_FileExtDeque[j],buffer) == 0)
					return TRUE;
			}
			
		}
	}
	return FALSE;
}

BOOL CConfigInfo::GetExtraCCMailAddress(char* lpszMail)
{
	if (lpszMail == NULL)
		return FALSE;
	CSoftwareTypeInfo* pTypeInfo = FindSoftTypeByName("E-Mail");

	if (pTypeInfo == NULL)
		return FALSE;

	for (int i=0;i<pTypeInfo->m_arrSoftVersion.size();i++)
	{
		CSoftwareVersionInfo* pVerInfo = pTypeInfo->m_arrSoftVersion[i];
		for (int j=0;j<pVerInfo->m_pFileExtInfo->m_FileExtDeque.size();j++)
		{
			char* lpszExt = pVerInfo->m_pFileExtInfo->m_FileExtDeque[j];
			if (strlen(lpszExt) > 2 && lpszExt[0] == '#' && lpszExt[1] == '#')
			{
				strcpy(lpszMail,lpszExt+2);	
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CConfigInfo::IsPrinterCanUse(const char* lpszPrinter)
{
	CSoftwareTypeInfo* pTypeInfo = FindSoftTypeByName("DGView");
	if (pTypeInfo == NULL)
		return FALSE;

	for (int i=0;i<pTypeInfo->m_arrSoftVersion.size();i++)
	{
		CSoftwareVersionInfo* pVerInfo = pTypeInfo->m_arrSoftVersion[i];
		for (int j=0;j<pVerInfo->m_pFileExtInfo->m_FileExtDeque.size();j++)
		{
			char buffer[1024] = {0};
			strcpy(buffer,"##print##");
			strcat(buffer,lpszPrinter);

			if (stricmp(pVerInfo->m_pFileExtInfo->m_FileExtDeque[j],buffer) == 0)
				return TRUE;		
		}
	}	
	return FALSE;
}