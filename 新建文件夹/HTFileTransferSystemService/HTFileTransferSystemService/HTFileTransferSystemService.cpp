// HTFileTransferSystemService.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "HTFileTransferSystemService_i.h"
#include "ThreadPool.h"
#include "FileTransferServiceThread.h"
#include "spIPCComm.h"
#include "SocketBase.h"
#include "ClientInfo.h"

#include <TlHelp32.h>
#include <stdio.h>

class CHTFileTransferSystemServiceModule : public CAtlServiceModuleT< CHTFileTransferSystemServiceModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_HTFileTransferSystemServiceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_HTFILETRANSFERSYSTEMSERVICE, "{9BBAA3D8-D689-4761-AAE5-CC8D816E9F71}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for 
		// your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.

		return S_OK;
	}

	HRESULT PreMessageLoop(int nShowCmd);
	HRESULT PostMessageLoop();
	HRESULT RegisterAppId(bool bService = false) throw();

	void main_thread() ;

	static DWORD WINAPI MainThread( LPVOID lpParameter ) ;
};

CHTFileTransferSystemServiceModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	TCHAR szSvrName[1024] = { 0 };
	TCHAR szRegKey[1024] = { 0 };
	LoadString(NULL, IDS_SERVICENAME,szSvrName, 1024);
	_stprintf(szRegKey, L"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal\\%s",szSvrName);


	CRegKey DocKey;
	if (DocKey.Open(HKEY_LOCAL_MACHINE,szRegKey) != ERROR_SUCCESS)
	{
		DocKey.Create(HKEY_LOCAL_MACHINE,szRegKey);
	} 
	DocKey.SetValue(_T("Service"), NULL);
	DocKey.Close();



	memset((char*)szRegKey, 0, sizeof(szRegKey));
	_stprintf(szRegKey, L"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network\\%s", szSvrName);
	if (DocKey.Open(HKEY_LOCAL_MACHINE,szRegKey) != ERROR_SUCCESS)
	{
		DocKey.Create(HKEY_LOCAL_MACHINE,szRegKey);
	} 
	DocKey.SetValue(_T("Service"), NULL);
	DocKey.Close();


    return _AtlModule.WinMain(nShowCmd);
}

HRESULT CHTFileTransferSystemServiceModule::RegisterAppId(bool bService) throw() 
{
	HRESULT hr = S_OK; 
	BOOL res = CAtlServiceModuleT<CHTFileTransferSystemServiceModule, IDS_SERVICENAME>::RegisterAppId(bService);   
	if(bService) 
	{ 
		if(IsInstalled()) 
		{                 
			SC_HANDLE hSCM = ::OpenSCManagerW(NULL, NULL, SERVICE_CHANGE_CONFIG); 
			SC_HANDLE hService = NULL; 
			if (hSCM == NULL) 
				hr = AtlHresultFromLastError(); 
			else 
			{ 
				hService = ::OpenService(hSCM, m_szServiceName, SERVICE_CHANGE_CONFIG); 
				if(hService != NULL) 
				{ 
					const int m_szServiceNameLen = 4096; 
					const int m_szServiceDescriptionLen = 2000; 
					CAString strServiceDescription = L"HTFileTransferSystemService"; 
					SERVICE_DESCRIPTION sdBuf = {strServiceDescription.GetBuffer(0)};
					strServiceDescription.ReleaseBuffer();
					res = ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sdBuf); 
					if (res)
					{
						res = ChangeServiceConfig( 
							hService,        // handle of service 
							SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type: no change 
							SERVICE_AUTO_START,  // service start type 
							SERVICE_NO_CHANGE, // error control: no change 
							NULL,              // binary path: no change 
							NULL,              // load order group: no change 
							NULL,              // tag ID: no change 
							NULL,              // dependencies: no change 
							NULL,              // account name: no change 
							NULL,              // password: no change 
							NULL);
						if (res != 0)
						{
							hr = AtlHresultFromLastError();
						}
					}
					else
					{
						hr = AtlHresultFromLastError();
					}
					::CloseServiceHandle(hService); 
				} 
				else 
				{
					hr = AtlHresultFromLastError(); 
				}
				::CloseServiceHandle(hSCM); 
			} 
		} 
	} 
	return   hr; 
}

HRESULT CHTFileTransferSystemServiceModule::PreMessageLoop(int nShowCmd)
{

	HRESULT hr = CAtlServiceModuleT<CHTFileTransferSystemServiceModule,IDS_SERVICENAME>::PreMessageLoop(nShowCmd);
	if(SUCCEEDED(hr))
	{
		HANDLE hMainThread = CreateThread(NULL, 0, MainThread, this, 0, NULL ) ;
		CloseHandle( hMainThread ) ;

		hr = S_OK;
	}
	return hr;
}

DWORD WINAPI CHTFileTransferSystemServiceModule::MainThread( LPVOID lpParameter )
{ 
	CHTFileTransferSystemServiceModule* pClient = ( CHTFileTransferSystemServiceModule* ) lpParameter ;

	pClient->main_thread();	

	return 0 ;
}

void CHTFileTransferSystemServiceModule::main_thread()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);

	SOCKET sockListen = NULL;
	int nCreateCount = 0;
	int nPort = 9985 ;

	int n1 = 0;
	sockListen = SPCreateListenSocket("0.0.0.0",nPort);
	n1 = int(sockListen);
	if(n1 < 0)
	{
		OutputDebugString(L"HTFileTransferSystemService CreateListen Failed.") ;
	}

	CThreadPool*	ThreadPool = new CThreadPool(HTUpdateService);
	while(1)
	{

		SOCKET sComm = 0;
		try
		{

			SOCKADDR_IN sinRemote;
			int nlen = sizeof(sinRemote);

			sComm = accept(sockListen,(struct sockaddr *)&sinRemote, &nlen );

			if((int)sComm > 0)
			{
				CFileTransferServiceThread* m_DGServiceTask = new CFileTransferServiceThread;
				m_DGServiceTask->m_sComm = sComm;
				ThreadPool->add_task_to_deq(m_DGServiceTask);
			}
			else
			{
				Sleep(100);
			}
		}
		catch(...)
		{

			if ((int)sComm > 0)
			{
				closesocket(sComm);
			}
		}
	}
	if(ThreadPool)
	{
		delete ThreadPool;
		ThreadPool = NULL;
	}

}

HRESULT CHTFileTransferSystemServiceModule::PostMessageLoop()
{
	// TODO: Add your specialized code here and/or call the base class
	return CAtlServiceModuleT<CHTFileTransferSystemServiceModule,100>::PostMessageLoop();
}