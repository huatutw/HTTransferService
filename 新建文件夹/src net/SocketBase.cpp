// SocketBase.cpp: implementation of the CSocketBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketBase.h"
#include "stdio.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD CSocketBase::m_dwPort = 0;
int CSocketBase::m_nTimeOut = 15;

CSocketBase::CSocketBase()
{
	
}

CSocketBase::~CSocketBase()
{

}

// DWORD CSocketBase::GetInitSetting(int nPort)
// {
// 	m_dwPort = nPort;
// 	return m_dwPort;
// }

BOOL  CSocketBase::RecvStringData(CIPCSocket& s,LPSTR strContent,int nSize)
{
	int Length = 0;
	int Read = 0;
	if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,sizeof(int),(LPBYTE)&Length,Read))
	{
		return FALSE;
	}

	if (Length == 0)
		return TRUE;
	if (Length >= nSize)		//如果预留的数据不足
	{
		LPSTR strContentEx = new CHAR[Length+1];
		memset(strContentEx,0,sizeof(CHAR)*(Length+1));
		if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,Length,(LPBYTE)strContentEx,Read))
		{
			delete [] strContentEx;
			return FALSE;
		}
		memcpy(strContent,strContentEx,nSize-1);
		delete [] strContentEx;

	}else
	{
		memset(strContent,0,nSize*sizeof(TCHAR));
		if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,sizeof(CHAR)*Length,(LPBYTE)strContent,Read))
			return FALSE;
	}
	return TRUE;
}

BOOL  CSocketBase::RecvStringData(CIPCSocket& s,LPWSTR strContent,int nSize)
{
	int Length = 0;
	int Read = 0;
	if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,sizeof(int),(LPBYTE)&Length,Read))
	{
		return FALSE;
	}

	if (Length == 0)
		return TRUE;
	if (Length >= nSize)		//如果预留的数据不足
	{
		LPWSTR strContentEx = new WCHAR[Length+1];
		memset(strContentEx,0,sizeof(WCHAR)*(Length+1));
		if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,Length*sizeof(WCHAR),(LPBYTE)strContentEx,Read))
		{
			delete [] strContentEx;
			return FALSE;
		}
		memcpy(strContent,strContentEx,nSize-1);
		delete [] strContentEx;
	}else
	{
		memset(strContent,0,nSize*sizeof(TCHAR));
		if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,sizeof(WCHAR)*Length,(LPBYTE)strContent,Read))
			return FALSE;
	}
	return TRUE;
}

BOOL  CSocketBase::RecvStringData(CIPCSocket& s,LPTSTR strContent,int nSize,LPTSTR* strContentEx)
{
	int Length = 0;
	int Read = 0;
	if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,sizeof(int),(LPBYTE)&Length,Read))
	{
		return FALSE;
	}

	if (Length == 0)
		return TRUE;
	if (Length >= nSize)		//如果预留的数据不足
	{
		*strContentEx = new TCHAR[Length+1];
		memset(*strContentEx,0,sizeof(TCHAR)*(Length+1));
		if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,sizeof(TCHAR)*Length,(LPBYTE)*strContentEx,Read))
		{
			delete [] strContentEx;
			return FALSE;
		}
		memcpy(strContent,strContentEx,nSize-1);
		delete [] strContentEx;
	}else
	{
		memset(strContent,0,nSize*sizeof(TCHAR));
		if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,sizeof(TCHAR)*Length,(LPBYTE)strContent,Read))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL  CSocketBase::SendStringData(CIPCSocket& s,LPTSTR strContent,int nSize)
{
	int iWrote = 0;
	int dwSize = nSize;
	if (SP_ERR_NETWORK == s.Send(sizeof(int),(LPBYTE)&dwSize,iWrote))
	{
		return FALSE;
	}

	if (nSize != 0)
	{
		if (SP_ERR_NETWORK == s.Send(nSize*sizeof(TCHAR),(LPBYTE)strContent,iWrote))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL  CSocketBase::SendStringData(CIPCSocket& s,LPCWSTR strContent)
{
	int iWrote = 0;
	int nSize = 0;
	if (strContent == NULL)
	{
		s.Send(sizeof(int),(LPBYTE)&nSize,iWrote);
		return FALSE;
	}else
	{
		nSize = wcslen(strContent);
	}
	
	if (SP_ERR_NETWORK == s.Send(sizeof(int),(LPBYTE)&nSize,iWrote))
	{
		return FALSE;
	}

	if (nSize != 0)
	{
		if (SP_ERR_NETWORK == s.Send(nSize*sizeof(WCHAR),(LPBYTE)strContent,iWrote))
		{
			return FALSE;
		}
		
	}
	return TRUE;
}

BOOL  CSocketBase::SendStringData(CIPCSocket& s,LPCSTR strContent)
{
	int iWrote = 0;
	int nSize = 0;
	if (strContent == NULL)
	{
		s.Send(sizeof(int),(LPBYTE)&nSize,iWrote);
		return FALSE;
	}else
	{
		nSize = strlen(strContent);
	}
	
	if (SP_ERR_NETWORK == s.Send(sizeof(int),(LPBYTE)&nSize,iWrote))
	{
		return FALSE;
	}

	if (nSize != 0)
	{
		if (SP_ERR_NETWORK == s.Send(nSize*sizeof(CHAR),(LPBYTE)strContent,iWrote))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL  CSocketBase::RecvDataEx(CIPCSocket& s,LPBYTE* lpData,int& nSize)
{
	int iRead;
	int nDataSize = 0;
	if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,sizeof(int),(LPBYTE)&nDataSize,iRead))
		return FALSE;
	if (nDataSize == 0)
	{
		*lpData = NULL;
		nSize = 0;
		return TRUE;
	}

	*lpData = new BYTE[nDataSize];
	nSize = nDataSize;
	memset(*lpData,0,nDataSize);
	if (SP_ERR_SUCCESS !=s.RecvWithTimeOut(m_nTimeOut,nDataSize,(LPBYTE)*lpData,iRead))
		return FALSE;
	
	return TRUE;
}

BOOL  CSocketBase::RecvData(CIPCSocket& s,LPVOID lpData,int nSize)
{
	int iRead;
	if (SP_ERR_SUCCESS != s.RecvWithTimeOut(m_nTimeOut,nSize,(LPBYTE)lpData,iRead))
		return FALSE;
	return TRUE;
}

BOOL  CSocketBase::SendData(CIPCSocket& s,LPVOID lpData,int nSize)
{
	int iWrote;
	if (SP_ERR_NETWORK == s.Send(nSize,(LPBYTE)lpData,iWrote))
	{
		return FALSE;
	}
	return TRUE;
}

void  CSocketBase::SendDataEx(CIPCSocket& s,LPVOID lpData,int nSize)
{
	int iWrote;
	if (nSize == 0)
	{
		s.Send(sizeof(int),(LPBYTE)&nSize,iWrote);
		return;
	}

	s.Send(sizeof(int),(LPBYTE)&nSize,iWrote);
	s.Send(nSize,(LPBYTE)lpData,iWrote);	
}


void  CSocketBase::SendFile(CIPCSocket &s,LPCSTR lpszFileName)
{
	HANDLE hFile = CreateFileA(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwFileSize = 0;
	
	const char * strFileName = strrchr(lpszFileName,'\\');
	if (strFileName)
	{
		strFileName ++;
	}
	
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile,NULL);
		SendData(s,&dwFileSize,sizeof(DWORD));
		
		SendStringData(s,strFileName);
		
		DWORD read = 0;
		while(TRUE)
		{
			BYTE btData[4096] = {0};
			ReadFile(hFile,btData,4096,&read,NULL);
			if(read != 0)
				SendDataEx(s,btData,read);
			if (read < 4096)
			{
				break;
			}
		}
		CloseHandle(hFile);
	}else
	{
		SendData(s,&dwFileSize,sizeof(DWORD));
		return;
	}
}

void  CSocketBase::SendFile(CIPCSocket &s,LPCWSTR lpszFileName)
{
	HANDLE hFile = CreateFileW(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwFileSize = 0;
	
	const wchar_t* strFileName = wcsrchr(lpszFileName,'\\');
	if (strFileName)
	{
		strFileName ++;
	}
	
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile,NULL);
		SendData(s,&dwFileSize,sizeof(DWORD));
		
		SendStringData(s,strFileName);
		DWORD read = 0;
		
		while(TRUE)
		{
			BYTE btData[4096] = {0};
			
			ReadFile(hFile,btData,4096,&read,NULL);
			if(read != 0)
				SendDataEx(s,btData,read);
			if (read < 4096)
			{
				break;
			}	
		}
		CloseHandle(hFile);		
	}else
	{
		SendData(s,&dwFileSize,sizeof(DWORD));
		return;
	}
}

void  CSocketBase::SendFile1(CIPCSocket &s,LPCSTR lpszFileName)
{
	HANDLE hFile = CreateFileA(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwFileSize = 0;

	LPSTR strFileName = (LPSTR)(char*)strrchr(lpszFileName,'\\');
	if (strFileName)
	{
		strFileName ++;
	}
	
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile,NULL);
		SendData(s,&dwFileSize,sizeof(DWORD));

		SendStringData(s,strFileName);
		DWORD read = 0;
		while(TRUE)
		{
			BYTE btData[4096] = {0};
			ReadFile(hFile,btData,4096,&read,NULL);
			if(read != 0)
				SendDataEx(s,btData,read);
			else
			{
				SendData(s,&read,sizeof(int));
				break;
			}
			if (read < 4096)
			{
				break;
			}	
		}
		CloseHandle(hFile);

		if (read != 0)
		{
			int nSize = 0;
			SendData(s,&nSize,sizeof(int));
		}
	}else
	{
		SendData(s,&dwFileSize,sizeof(DWORD));
		return;
	}
}

void  CSocketBase::SendFile1(CIPCSocket &s,LPCWSTR lpszFileName)
{
	HANDLE hFile = CreateFileW(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwFileSize = 0;

	LPWSTR strFileName = (LPWSTR)(wchar_t*)wcsrchr(lpszFileName,'\\');
	if (strFileName)
	{
		strFileName ++;
	}
	
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile,NULL);
		SendData(s,&dwFileSize,sizeof(DWORD));

		SendStringData(s,strFileName);
		DWORD read = 0;
		while(TRUE)
		{
			BYTE btData[4096] = {0};
			ReadFile(hFile,btData,4096,&read,NULL);
			if(read != 0)
				SendDataEx(s,btData,read);
			else
			{
				SendData(s,&read,sizeof(int));
				break;
			}
			if (read < 4096)
			{
				break;
			}	
		}
		CloseHandle(hFile);

		if (read != 0)
		{
			int nSize = 0;
			SendData(s,&nSize,sizeof(int));
		}
	}else
	{
		SendData(s,&dwFileSize,sizeof(DWORD));
		return;
	}
}

LPTSTR GetGUID(LPTSTR strGUID)
{
	GUID guid;
	CoCreateGuid(&guid);
	OLECHAR oleGUID[100] = {0};
	StringFromGUID2(guid,oleGUID,100);
	USES_CONVERSION;
	_tcscpy(strGUID,OLE2T(oleGUID));
	return strGUID;
}
bool TP_SendData(char* pBuff,int nlen,SOCKET sock)
{

	int nSended = 0;

	while(1)
	{
		int nSend = send(sock,pBuff,nlen,0);
		nSended += nSend;

		if (nSended == nlen)
		{
			break;
		}

		pBuff = pBuff+nSend;

		if ( nSend == SOCKET_ERROR)
		{
			return false;
		}
	}

	return true;
}

BOOL  CSocketBase::RecvFile1(CIPCSocket &s,LPTSTR lpszPath)
{
	DWORD dwFileSize = 0;
	DWORD dwAllWrote = 0;
	if (!RecvData(s,&dwFileSize,sizeof(DWORD)))
		return FALSE;

	if (dwFileSize == 0)
		return TRUE;
	
	TCHAR strFileName[1024] = {0};
	if (!RecvStringData(s,strFileName,1024,NULL))
		return FALSE;


	HANDLE hFile = CreateFile(lpszPath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	while(TRUE)
	{
		LPBYTE btData = NULL;
		int size = 0;
		if (!RecvDataEx(s,&btData,size))
		{
			if (btData)
			{
				delete[] btData;
			}
			CloseHandle(hFile);
			return FALSE;
		}
		
		if (size != 0)
		{
			DWORD wrote = 0;
			WriteFile(hFile,btData,size,&wrote,NULL);
			if (btData)
			{
				delete[] btData;
			}
			
			
		}else
		{
			if (btData)
			{
				delete[] btData;
			}
			break;
		}
	}	
	CloseHandle(hFile);
	return TRUE;
}

BOOL  CSocketBase::RecvFile(CIPCSocket &s,LPTSTR lpszPath)
{
	DWORD dwFileSize = 0;
	DWORD dwAllWrote = 0;
	if (!RecvData(s,&dwFileSize,sizeof(DWORD)))
		return FALSE;
	
	if (dwFileSize == 0)
		return TRUE;
	
	TCHAR strFileName[1024] = {0};
	if (!RecvStringData(s,strFileName,1024,NULL))
		return FALSE;
	
	
	HANDLE hFile = CreateFile(lpszPath,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	while(TRUE)
	{
		LPBYTE btData = NULL;
		int size = 0;
		if (!RecvDataEx(s,&btData,size))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		
		if (size != 0)
		{
			DWORD wrote = 0;
			WriteFile(hFile,btData,size,&wrote,NULL);
			delete[] btData;
			dwAllWrote += wrote;
			
			if(dwAllWrote >= dwFileSize)
				break;
			
		}else
		{
			break;
		}
	}	
	CloseHandle(hFile);
	return TRUE;
}

BOOL  CSocketBase::RecvFileToFolder(CIPCSocket &s,LPTSTR lpszPath)
{
	DWORD dwFileSize = 0;
	DWORD dwAllWrote = 0;
	if (!RecvData(s,&dwFileSize,sizeof(DWORD)))
		return FALSE;
	
	if (dwFileSize == 0)
		return TRUE;
	
	TCHAR strFileName[1024] = {0};
	if (!RecvStringData(s,strFileName,1024,NULL))
		return FALSE;

	TCHAR strGUID[100] = {0};
	_stprintf(lpszPath,_T("%s\\%s%s"),lpszPath,strGUID,strFileName);
	
	HANDLE hFile = CreateFile(lpszPath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	while(TRUE)
	{
		LPBYTE btData = NULL;
		int size = 0;
		if (!RecvDataEx(s,&btData,size))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		
		if (size != 0)
		{
			DWORD wrote = 0;
			WriteFile(hFile,btData,size,&wrote,NULL);
			delete[] btData;
			dwAllWrote += wrote;
			
			if(dwAllWrote >= dwFileSize)
				break;
			
		}else
		{
			break;
		}
	}	
	CloseHandle(hFile);
	
	return TRUE;
}

BOOL  CSocketBase::RecvFileEx(CIPCSocket &s,LPTSTR lpszPath)
{
	HANDLE hFile = CreateFileW(lpszPath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		hFile = CreateFile(lpszPath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	}

	DWORD dwSrcFileSize = GetFileSize(hFile,NULL);

	int nSent = send(s.GetSocket(),(char*)&dwSrcFileSize,sizeof(DWORD),0);

	DWORD dwFileSize = 0;
	if (!RecvData(s,&dwFileSize,sizeof(DWORD)))
	{
		return FALSE;
	}

	SetFilePointer(hFile,dwSrcFileSize,NULL,FILE_BEGIN);	

	if (dwFileSize == 0)
	{
		return TRUE;
	}


	TCHAR strFileName[1024] = {0};
	if (!RecvStringData(s,strFileName,1024,NULL))
	{
		return FALSE;
	}

	if (nSent != sizeof(DWORD))
	{
		return FALSE;
	}

	while(TRUE)
	{
		char buff[4096] = { 0 };
		int nRecv = recv(s.GetSocket(),buff,sizeof(buff),0);
		if (nRecv == -1)
		{
			//连接异常断开		
			return FALSE;
		}
		DWORD wrote = 0;
		WriteFile(hFile,buff,nRecv,&wrote,NULL);
		dwFileSize -= nRecv;
		if ( dwFileSize <= 0)
		{
			break;
		}
	}	
	CloseHandle(hFile);
	return TRUE;
}
void  CSocketBase::SendFileEx(CIPCSocket &s,LPCWSTR lpszFileName)
{
	DWORD nFilePos = 0;
	recv(s.GetSocket(),(char*)&nFilePos,sizeof(nFilePos),0);

	HANDLE hFile = CreateFileW(lpszFileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwFileSize = 0;

	const wchar_t* strFileName = wcsrchr(lpszFileName,'\\');
	if (strFileName)
	{
		strFileName ++;
	}

	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile,NULL) - nFilePos;
		SendData(s,&dwFileSize,sizeof(DWORD));
		SetFilePointer(hFile,nFilePos,NULL,FILE_BEGIN);	
		SendStringData(s,strFileName);
		DWORD read = 0;

		while(TRUE)
		{
			BYTE btData[4096] = {0};

			ReadFile(hFile,btData,4096,&read,NULL);
			TP_SendData((char*)btData,read,s.GetSocket());
			if (read < 4096)
			{
				break;
			}	
		}
		CloseHandle(hFile);

		if (read != 0)
		{
			int nSize = 0;
			SendData(s,&nSize,sizeof(int));
		}

	}else
	{
		SendData(s,&dwFileSize,sizeof(DWORD));
		return;
	}
}

