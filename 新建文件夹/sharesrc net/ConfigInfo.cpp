// ConfigInfo.cpp: implementation of the CConfigInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigInfo.h"
//#include "shfolder.h"

#include <atlconv.h> 
#include "VirtualMemFile.h"

/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/
/////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////////////

CConfigInfo::CConfigInfo()
{
	m_pCurSoftware = NULL;
#ifdef _UNICODE
	m_clsVersion = 1;		//Unicode版本
#else
	m_clsVersion = 0;		//原始版本
#endif
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

CSoftwareTypeInfo* CConfigInfo::FindSoftTypeByName(LPCTSTR lpszSoftType)
{
	for (int i=0;i<m_arrSoftType.size();i++)
	{
		if (_tcsicmp(m_arrSoftType[i]->m_lpszSoftTypeName,lpszSoftType) == 0)
		{
			return m_arrSoftType[i];
		}
	}
	return NULL;
}

/*void CConfigInfo::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_clsVersion ;

		int size = 0;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			CSoftwareTypeInfo* pInfo = new CSoftwareTypeInfo;
			pInfo->Serialize(ar);
			m_arrSoftType.push_back(pInfo);
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
}
*/

BOOL CConfigInfo::SetDataToBuffer(LPBYTE* btData, DWORD& dwSize, BOOL bUnicodeToAnsi = FALSE)
{
	CVirtualMemFile mem(1024*512);		// 申请512k的内存

	int		nNum	= 0;
	int		nLen	= 0;

	mem.WriteData(&m_clsVersion, sizeof(int));	// 将版本记入内存	
	int nSize = 0;
	for (nNum = 0; nNum < m_arrSoftType.size(); nNum++)
	{
		if (m_arrSoftType[nNum]->m_bSaveThisType)
			nSize++;
	}
	mem.WriteData(&nSize, sizeof(int));

	for (nNum = 0; nNum < m_arrSoftType.size(); nNum++)	// CSoftwareTypeInfo
	{
		if (m_arrSoftType[nNum]->m_bSaveThisType)
		{
			CSoftwareTypeInfo* pTypeInfo = m_arrSoftType[nNum];

			mem.WriteData(&pTypeInfo->m_clsVersion, sizeof(int));
			nLen = _tcslen(pTypeInfo->m_lpszSoftTypeName);
			//mem.WriteData(&nLen, sizeof(int));
			if (nLen > 0)
				mem.WriteString(pTypeInfo->m_lpszSoftTypeName);
			
			nSize = pTypeInfo->m_arrSoftVersion.size();
			mem.WriteData(&nSize, sizeof(int));
			for (int nVerInfo = 0; nVerInfo < pTypeInfo->m_arrSoftVersion.size(); nVerInfo++) // CSoftwareVersionInfo
			{
				CSoftwareVersionInfo* pVersionInfo = pTypeInfo->m_arrSoftVersion[nVerInfo];
				mem.WriteData(&pVersionInfo->m_clsVersion, sizeof(int));
				nLen = _tcslen(pVersionInfo->m_lpszSoftVersion);
				//mem.WriteData(&nLen, sizeof(int));
				if (nLen > 0)
					mem.WriteString(pVersionInfo->m_lpszSoftVersion);

				CDllExeInfo* pDllInfo = pVersionInfo->m_pDllExeInfo;		// CDllExeInfo
				mem.WriteData(&pDllInfo->m_clsVersion, sizeof(int));
				nSize = pDllInfo->m_ConfigDeque.size();
				mem.WriteData(&nSize, sizeof(int));
				for (int nConfig = 0; nConfig < pDllInfo->m_ConfigDeque.size(); nConfig++)
				{
					nLen = _tcslen(pDllInfo->m_ConfigDeque[nConfig]->lpszModuleName);
					//mem.WriteData(&nLen, sizeof(int));
					if (nLen > 0)
					{
						LPTSTR strData = pDllInfo->m_ConfigDeque[nConfig]->lpszModuleName;
						mem.WriteString(strData);
					}
				}

				COleCfgInfo*	pOleInfo	= pVersionInfo->m_pOleCfgInfo;	// COleCfgInfo
				mem.WriteData(&pOleInfo->m_clsVersion, sizeof(int));
				nSize = pOleInfo->m_CLSIDDeque.size();
				mem.WriteData(&nSize, sizeof(int));
				for (int nCLSID = 0; nCLSID < pOleInfo->m_CLSIDDeque.size(); nCLSID++)
				{
					mem.WriteData(&pOleInfo->m_CLSIDDeque[nCLSID], sizeof(CLSID));
				}

				CFileExtInfo* pFileInfo = pVersionInfo->m_pFileExtInfo;		// CFileExtInfo
				mem.WriteData(&pFileInfo->m_clsVersion, sizeof(int));
				nSize = pFileInfo->m_FileExtDeque.size();
				mem.WriteData(&nSize, sizeof(int));
				for (int nFile = 0; nFile < pFileInfo->m_FileExtDeque.size(); nFile++)
				{
					nLen = _tcslen(pFileInfo->m_FileExtDeque[nFile]);
					//mem.WriteData(&nLen, sizeof(int));
					if (nLen > 0)
					{
						LPTSTR strData = pFileInfo->m_FileExtDeque[nFile];
						mem.WriteString(strData);
					}
				}
			}
		}
	}

	dwSize = mem.GetDataSize();

	*btData = new BYTE[mem.GetDataSize()];
	memcpy(*btData,mem.GetData(),mem.GetDataSize());
	
	return TRUE;
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
  int counter;

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

// extern PFNSHGETFOLDERPATH GetFuncPtr_SHGetFolderPath();
// 
// CString GetSystemTempPath(CString strFileName)
// {
// 	TCHAR path[1024] = {0};
// 	PFNSHGETFOLDERPATH pSHGetFolderPath = NULL;
// 
// 	pSHGetFolderPath = GetFuncPtr_SHGetFolderPath();
// 
// 	TCHAR pszLink[MAX_PATH];
//     BOOL bFound = FALSE;
//    // Find the current user's Start Menu Programs folder
// 	if (pSHGetFolderPath)
// 	{
// 	  bFound = SUCCEEDED(pSHGetFolderPath(NULL, CSIDL_TEMPLATES, 
// 						   NULL, 0, path));
// 
// 	}
// 	CString strPath = path;
// 	strPath += "\\";
// 	strPath += strFileName;
// 
// 	return strPath;
// }

/*
BOOL CConfigInfo::LoadConfigInfoFromFile(LPCTSTR lpszFileName)
{
	//CFileFind find;
	//if (!find.FindFile(lpszFileName))
	//	return FALSE;

	USES_CONVERSION;
	if (_access(W2A(lpszFileName), 0) == -1)
	{
		return FALSE;
	}

	CAString strTmpFile;
	DWORD dwProcessID = GetCurrentProcessId();
	strTmpFile.Format(TEXT("%d.cfg"),dwProcessID);


	CopyFile(lpszFileName,strTmpFile,FALSE);

	HANDLE hFile = CreateFile(strTmpFile,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile,NULL);
		
		LPBYTE bt = new BYTE[dwFileSize];
		DWORD read;
		ReadFile(hFile,bt,dwFileSize,&read,NULL);
		int nVersion;
		memcpy(&nVersion,bt,sizeof(int));
		if (nVersion != 0)
		{
			Encrypt_RC4(bt,dwFileSize,"DocGuarder_Encrypt_System_Made_in_HangZhou_2007_4_18");
			SetFilePointer(hFile,0,0,FILE_BEGIN);
			WriteFile(hFile,bt,dwFileSize,&read,NULL);
		}
		FlushFileBuffers(hFile);
		CloseHandle(hFile);
		delete[] bt;		
	}

	CFile file(strTmpFile,CFile::modeRead | CFile::shareDenyWrite);
	CArchive ar(&file,CArchive::load);
	
	Serialize(ar);

	ar.Close();
	file.Close();

	DeleteFile(strTmpFile);
	return TRUE;
}

void CConfigInfo::SaveConfigInfoToFile(LPCTSTR lpszFileName)
{
	CFile file(lpszFileName,CFile::modeWrite | CFile::modeCreate);
	CArchive ar(&file,CArchive::store);
	
	Serialize(ar);

	ar.Close();
	file.Close();

	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile,NULL);
		
		LPBYTE bt = new BYTE[dwFileSize];
		DWORD read;
		ReadFile(hFile,bt,dwFileSize,&read,NULL);
		Encrypt_RC4(bt,dwFileSize,"DocGuarder_Encrypt_System_Made_in_HangZhou_2007_4_18");
		SetFilePointer(hFile,0,0,FILE_BEGIN);
		WriteFile(hFile,bt,dwFileSize,&read,NULL);
		CloseHandle(hFile);
		delete[] bt;
	}
}
*/
BOOL CConfigInfo::CheckModule(LPCTSTR lpszModuleName)
{
	if (m_pCurSoftware)
		return TRUE;
	
	LPCTSTR lpMdName = _tcsrchr(lpszModuleName,TEXT('\\'));	//只取文件名进行判断，去掉路径
	if(lpMdName == NULL)
		lpMdName = lpszModuleName;
	else
		lpMdName++;

	for (int i=0;i<m_arrSoftType.size();i++)
	{
		CSoftwareVersionInfo* pInfo = m_arrSoftType[i]->CheckModule(lpMdName);
		if (pInfo)
		{
			m_pCurSoftware = pInfo;
			return TRUE;
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
		if (_tcsicmp((*i)->m_lpszSoftTypeName,pInfo->m_lpszSoftTypeName) == 0)
		{
			pTemp = (*i);
			m_arrSoftType.erase(i);
			delete pTemp;

			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CConfigInfo::AddOneMail(LPCTSTR lpszMail,int bControl)
{
	if (lpszMail == NULL)
		return FALSE;
	CSoftwareTypeInfo* pTypeInfo = FindSoftTypeByName(TEXT("E-Mail"));

	if (pTypeInfo == NULL)
		return FALSE;
	
	int i=0;
	for (i=0;i<pTypeInfo->m_arrSoftVersion.size();i++)
	{
		CSoftwareVersionInfo* pVerInfo = pTypeInfo->m_arrSoftVersion[i];
		STRING_DEQUE::iterator j;
		for (j=pVerInfo->m_pFileExtInfo->m_FileExtDeque.begin();j!=pVerInfo->m_pFileExtInfo->m_FileExtDeque.end();++j)
		{
			switch(bControl)
			{
			case 0: //自动发送邮箱
				{
					LPTSTR lpszExt = *j;
					TCHAR CC[1024] = {0};
					_stprintf(CC,TEXT("##%s"),lpszMail);
					if (_tcslen(lpszExt) > 2 && lpszExt[0] == TEXT('#') && lpszExt[1] == TEXT('#'))
					{
						pVerInfo->m_pFileExtInfo->m_FileExtDeque.erase(j);
						break;
					}
				}
				break;
			case 1: //接收者安全邮箱
				{
					TCHAR CC[1024] = {0};
					_stprintf(CC,TEXT("@@%s"),lpszMail); 
					if (_tcsicmp(*j,CC) == 0) //如果有重复的则返回
					{
						return TRUE;
					}
				}
				break;
			case 2:	//发送者安全邮箱
				{
					TCHAR CC[1024] = {0};
					_stprintf(CC,TEXT("!!%s"),lpszMail); 
					if (_tcsicmp(*j,CC) == 0) //如果有重复的则返回
					{
						return TRUE;
					}
				}
				break;
			}

		}
	}

	for (i=0;i<pTypeInfo->m_arrSoftVersion.size();i++)
	{
		CSoftwareVersionInfo* pVerInfo = pTypeInfo->m_arrSoftVersion[i];
		switch(bControl)
		{
		case 0:
			{
				TCHAR CC[1024] = {0};
				_stprintf(CC,TEXT("##%s"),lpszMail);
				
				LPTSTR mail = (LPTSTR)calloc(_tcslen(CC)+1,sizeof(TCHAR));
				_tcscpy(mail,CC);
				pVerInfo->m_pFileExtInfo->m_FileExtDeque.push_back(mail);
			}
			break;
		case 1:
			{
				TCHAR CC[1024] = {0};
				_stprintf(CC,TEXT("@@%s"),lpszMail);
				LPTSTR mail = (LPTSTR)calloc(_tcslen(CC)+1,sizeof(TCHAR));
				_tcscpy(mail,CC);
				pVerInfo->m_pFileExtInfo->m_FileExtDeque.push_back(mail);
			}
			break;
		case 2:
			{
				TCHAR CC[1024] = {0};
				_stprintf(CC,TEXT("!!%s"),lpszMail);
				LPTSTR mail = (LPTSTR)calloc(_tcslen(CC)+1,sizeof(TCHAR));
				_tcscpy(mail,CC);
				pVerInfo->m_pFileExtInfo->m_FileExtDeque.push_back(mail);
			}
			break;
		}
		break;
	}

	return FALSE;
}


LPTSTR CConfigInfo::GetExtraCCMailAddress()
{
	CSoftwareTypeInfo* pTypeInfo = FindSoftTypeByName(TEXT("E-Mail"));

	if (pTypeInfo == NULL)
		return TEXT("");

	for (int i=0;i<pTypeInfo->m_arrSoftVersion.size();i++)
	{
		CSoftwareVersionInfo* pVerInfo = pTypeInfo->m_arrSoftVersion[i];
		for (int j=0;j<pVerInfo->m_pFileExtInfo->m_FileExtDeque.size();j++)
		{
			LPTSTR lpszExt = pVerInfo->m_pFileExtInfo->m_FileExtDeque[j];
			if (_tcslen(lpszExt) > 2 && lpszExt[0] == TEXT('#') && lpszExt[1] == TEXT('#'))
			{
				/*CString strMail;
				strMail = lpszExt+2;
				return strMail;*/

				return lpszExt+2;
			}
		}
	}

	return NULL;
}
	
void CConfigInfo::GetSafeMailAddress(/*CStringList& maillist*/deque<LPTSTR> *pMailList)
{
	CSoftwareTypeInfo* pTypeInfo = FindSoftTypeByName(TEXT("E-Mail"));

	if (pTypeInfo == NULL)
		return;

	for (int i=0;i<pTypeInfo->m_arrSoftVersion.size();i++)
	{
		CSoftwareVersionInfo* pVerInfo = pTypeInfo->m_arrSoftVersion[i];
		for (int j=0;j<pVerInfo->m_pFileExtInfo->m_FileExtDeque.size();j++)
		{
			LPTSTR lpszExt = pVerInfo->m_pFileExtInfo->m_FileExtDeque[j];
			if (_tcslen(lpszExt) > 2 && lpszExt[0] == TEXT('@') && lpszExt[1] == TEXT('@'))
			{
				/*
				CString strMail;
				strMail = lpszExt+2;
				maillist.AddTail(strMail);
				*/
				pMailList->push_back(lpszExt+2);
			}
		}
	}	
}

void CConfigInfo::GetSafeRevMailAddress(/*CStringList& maillist*/deque<LPTSTR> *pMailList)
{
	CSoftwareTypeInfo* pTypeInfo = FindSoftTypeByName(TEXT("E-Mail"));

	if (pTypeInfo == NULL)
		return;

	for (int i=0;i<pTypeInfo->m_arrSoftVersion.size();i++)
	{
		CSoftwareVersionInfo* pVerInfo = pTypeInfo->m_arrSoftVersion[i];
		for (int j=0;j<pVerInfo->m_pFileExtInfo->m_FileExtDeque.size();j++)
		{
			LPTSTR lpszExt = pVerInfo->m_pFileExtInfo->m_FileExtDeque[j];
			if (_tcslen(lpszExt) > 2 && lpszExt[0] == TEXT('!') && lpszExt[1] == TEXT('!'))
			{
				/*
				CString strMail;
				strMail = lpszExt+2;
				maillist.AddTail(strMail);
				*/
				pMailList->push_back(lpszExt+2);
			}
		}
	}	
}

BOOL CConfigInfo::ClearMailSetting()
{
	CSoftwareTypeInfo* pTypeInfo = FindSoftTypeByName(TEXT("E-Mail"));

	if (pTypeInfo == NULL)
		return FALSE;

	for (int i=0;i<pTypeInfo->m_arrSoftVersion.size();i++)
	{
		CSoftwareVersionInfo* pVerInfo = pTypeInfo->m_arrSoftVersion[i];
		delete pVerInfo->m_pFileExtInfo;
		pVerInfo->m_pFileExtInfo = new CFileExtInfo;
		break;
	}
	return TRUE;
}