// SocketBase.h: interface for the CSocketBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETBASE_H__FAB2576B_EDFE_4BC0_857A_2DBBBF9C34A8__INCLUDED_)
#define AFX_SOCKETBASE_H__FAB2576B_EDFE_4BC0_857A_2DBBBF9C34A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "spIPCComm.h"

using namespace commIPC;

class CSocketBase  
{
public:
	CSocketBase();
	virtual ~CSocketBase();

public:
	static DWORD GetInitSettingFromReg();

	static BOOL  RecvStringData(CIPCSocket& s,LPTSTR strContent,int nSize,LPTSTR* strContentEx);
	static BOOL  RecvStringData(CIPCSocket& s,LPSTR strContent,int nSize);
	static BOOL  RecvStringData(CIPCSocket& s,LPWSTR strContent,int nSize);
	
	static BOOL  SendStringData(CIPCSocket& s,LPTSTR strContent,int nSize);
	
	static BOOL  SendStringData(CIPCSocket& s,LPCWSTR strContent);
	static BOOL  SendStringData(CIPCSocket& s,LPCSTR strContent);

	static BOOL  RecvDataEx(CIPCSocket& s,LPBYTE* lpData,int& nSize);
	static BOOL  RecvData(CIPCSocket& s,LPVOID lpData,int nSize);

	static void  SendFile1(CIPCSocket &s,LPCSTR lpszFileName);
	static void  SendFile1(CIPCSocket &s,LPCWSTR lpszFileName);

	static void  SendFile(CIPCSocket &s,LPCSTR lpszFileName);
	static void  SendFile(CIPCSocket &s,LPCWSTR lpszFileName);

	static BOOL  RecvFile1(CIPCSocket &s,LPTSTR lpszPath);
	static BOOL  RecvFileToFolder(CIPCSocket &s,LPTSTR lpszPath);
	static BOOL  RecvFile(CIPCSocket &s,LPTSTR lpszPath,LPTSTR lpszFileName);
	static BOOL  RecvFile(CIPCSocket &s,LPTSTR lpszPath);

	static BOOL  RecvFileEx(CIPCSocket &s,LPTSTR lpszPath);
	static void  SendFileEx(CIPCSocket &s,LPCWSTR lpszFileName);
	static BOOL  SendData(CIPCSocket& s,LPVOID lpData,int nSize);
	static void  SendDataEx(CIPCSocket& s,LPVOID lpData,int nSize);

public:
	static int	  m_nTimeOut;		//³¬Ê±
private:
	static DWORD  m_dwPort;			//¶Ë¿Ú	
};

#endif // !defined(AFX_SOCKETBASE_H__FAB2576B_EDFE_4BC0_857A_2DBBBF9C34A8__INCLUDED_)


