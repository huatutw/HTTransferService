#include "StdAfx.h"
#include "ThreadPool.h"
#include "process.h"
#include <WINDOWS.H>


unsigned int __stdcall _thr_thread_pool(void* pvoid)
{

	CThreadPool* pWp = (CThreadPool*)pvoid;

	pWp->thread_pool_run();


	return 0;
}

unsigned int __stdcall _thr_add_task(void* pvoid)
{

	CThreadPool* pWp = (CThreadPool*)pvoid;

	pWp->add_task_run();

	return 0;
}

unsigned int __stdcall _thr_print_state(void* pvoid)
{

	CThreadPool* pWp = (CThreadPool*)pvoid;

	pWp->print_state_run();

	return 0;
}

unsigned int __stdcall _thr_read_config(void* pvoid)
{

	CThreadPool* pWp = (CThreadPool*)pvoid;

	pWp->read_config_run();

	return 0;
}

CThreadPool::CThreadPool(int nServiceType)
{
	m_nThreadSum				= 250;
	m_nThreadRunCount		= 0;

	m_ServiceType				= DogService4;
	m_bInit						= FALSE;
	m_bDebug					= FALSE;
	m_bLogger					= FALSE;
	m_bThreadPoolRun		= FALSE;
	m_bAddTaskRun			= FALSE;
	m_bEndPrint				= FALSE;
	m_bReadCfg				= FALSE;
	m_ServiceType				= nServiceType;
	m_strIniPath				= L"";
	m_strLogPath				= L"";	

	get_cfg_path();
	read_cfg();	
	get_log_path();

	m_hWaitingWork		= CreateEvent(NULL, false, false, NULL);
	m_hGotgWork			= CreateEvent(NULL, false, false, NULL);
	m_hAddTask			= CreateEvent(NULL, false, false, NULL);

	HANDLE hAddTask = (HANDLE)_beginthreadex(NULL, 0, _thr_add_task, this, 0, NULL);
	CloseHandle(hAddTask);
	HANDLE hReadCfg = (HANDLE)_beginthreadex(NULL, 0, _thr_read_config, this, 0, NULL);
	CloseHandle(hReadCfg);
	HANDLE hPrintState = (HANDLE)_beginthreadex(NULL, 0, _thr_print_state, this, 0, NULL);
	CloseHandle(hPrintState);
	
	init_thread_pool(m_nThreadSum);
}

CThreadPool::~CThreadPool(void)
{
	m_bThreadPoolRun	= TRUE;
	m_bAddTaskRun		= TRUE;
	m_bEndPrint			= TRUE;
	m_bReadCfg			= TRUE;

	CloseHandle(m_hWaitingWork);
	CloseHandle(m_hGotgWork);
	CloseHandle(m_hAddTask);	
}

void CThreadPool::init_thread_pool(int nThreadSum ,int nThrTimeOut)
{
	m_nThreadTimeOut = nThrTimeOut*1000;
	for (int i = 0; i < m_nThreadSum; i++)
	{
		HANDLE h = (HANDLE)_beginthreadex(NULL, 0, _thr_thread_pool, this, 0, NULL);
		CloseHandle(h);
	}
}

void CThreadPool::add_task_to_deq(CTaskUnit* TaskUnit)
{
	m_LockDeq.Lock();
	m_DequeTask.push_back(TaskUnit);

	m_LockDeq.UnLock();	
}

void CThreadPool::thread_pool_run()
{
	CLogger* pl = CLogger::CreateInstance(m_strLogPath);

	while (!m_bThreadPoolRun)
	{
	
		try
		{
			int nRet  = WaitForSingleObject(m_hWaitingWork,m_nThreadTimeOut);
			
			if ( nRet == WAIT_OBJECT_0)
			{
				int sTime = GetTickCount();
				InterlockedIncrement(&m_nThreadRunCount);
				
				CTaskUnit* TaskUnit = NULL;
				m_LockDeq.Lock();
				if (m_DequeTask.size() > 0 )
				{
					TaskUnit = m_DequeTask.front();

					m_DequeTask.pop_front();
				}
				
				m_LockDeq.UnLock();

				SetEvent(m_hGotgWork);
				if (TaskUnit)
				{
					TaskUnit->DoWork();
					delete TaskUnit;
					TaskUnit = NULL;
				}
				
				
				InterlockedDecrement(&m_nThreadRunCount);
				
				
				if (m_bLogger)
				{
					int eTime = GetTickCount();
					int ntime = eTime - sTime;				
					TCHAR szData[2048] ={ 0 };
					m_LockDeq.Lock();
					_stprintf(szData,L" task:%d ms ;thr:%d; thrid:%d; deq:%d", ntime, m_nThreadRunCount, GetCurrentThreadId(), m_DequeTask.size());
					m_LockDeq.UnLock();
					OutputDebugString(szData);
					pl->add_log_to_deque(szData);
				}
			}
		}
		catch(...)
		{

		}
	}	
}

BOOL CThreadPool::add_task_run()
{
	while(!m_bAddTaskRun)
	{		
		if (m_DequeTask.size() <= 0)
		{
			Sleep(10);
			continue; 
		}
		SetEvent(m_hWaitingWork);
		m_bEndPrint = FALSE;		
		//等待线程接收任务
		int nRet = WaitForSingleObject(m_hGotgWork,m_nThreadTimeOut);
		if (nRet == WAIT_TIMEOUT)
		{
			OutputDebugString(L"add work unit timeout!!\n");				
		}			
	}	
	return FALSE;
}

void CThreadPool::print_state_run()
{
	HANDLE h = CreateEvent(NULL, false, false, NULL);

	while(!m_bEndPrint)
	{
		if (!m_bDebug)
		{
			Sleep(100);
			continue;
		}
		WaitForSingleObject(h, 500);

		TCHAR szData[1024] ={ 0 };
		_stprintf(szData, L"thr: %d.\n", m_nThreadRunCount);
		OutputDebugString(szData);
		
		if (m_DequeTask.size() == 0)
		{
			if (m_nThreadRunCount == 0)
			{
				m_bEndPrint = TRUE;
			}			 
		}
	}
	CloseHandle(h);
}

void CThreadPool::read_config_run()
{	
	while(!m_bReadCfg)
	{	
		read_cfg();
		Sleep(1000);
	}	
}

void CThreadPool::get_cfg_path()
{
	TCHAR szIniPath[1024] = { 0 };
	CRegKey docKey;
	if (ERROR_SUCCESS == docKey.Open(HKEY_LOCAL_MACHINE,_T("Software\\HCRD")))
	{		 
		DWORD dwlen;
		docKey.QueryValue(szIniPath, L"ServiceConfig", &dwlen);		
		docKey.Close();
		m_strIniPath = szIniPath;
	}
}

void CThreadPool::get_log_path()
{
	TCHAR szModulePath[1024] = {0};
	GetModuleFileName(NULL, szModulePath, 1024);
	CAString strLogPath = szModulePath;	
	strLogPath = strLogPath.Left(strLogPath.ReverseFind('\\')+1);
	m_strLogPath = strLogPath + m_strLogPath;
}

void CThreadPool::read_cfg()
{
	TCHAR szName[1024] = { 0 };
	const TCHAR* szIniPath = m_strIniPath.GetBuffer(0);
	m_strIniPath.ReleaseBuffer(0);
	
	switch(m_ServiceType)
	{
	case DogService4:
		{
			_stprintf(szName, L"DogService4");				
		}
		break;
		
	case DogFlow:
		{
			_stprintf(szName, L"DogFlow");				
		}
		break;
		
	case HTUpdateService:
		{
			_stprintf(szName, L"HTUpdateService");
		}
		break;
		
	case DGFileApprovalServer:
		{
			_stprintf(szName, L"DGFileApprovalServer");
		}
		break;
		
	case DGFileUploadService:
		{
			_stprintf(szName, L"DGFileUploadService");
		}
		break;
	case MobileServer:
		{
			_stprintf(szName, L"MobileServer");
		}
		
	default:
		break;
	}
	
	if (L"" == m_strLogPath)
	{
		m_strLogPath = szName;
		m_strLogPath += L".log";
	}

	TCHAR szState[1024]={0};
	GetPrivateProfileString(szName, L"Logger", L"", szState, 1024, szIniPath);
	if (szState[0] == L'1')
	{
		m_bLogger = TRUE;
	}
	else
	{
		m_bLogger = FALSE;
	}
	
	GetPrivateProfileString(szName, L"Debug", L"", szState, 1024, szIniPath);
	if (szState[0] == L'1')
	{
		m_bDebug = TRUE;
	}
	else
	{
		m_bDebug = FALSE;
	}
	
	GetPrivateProfileString(szName, L"nThread", L"", szState, 1024, szIniPath);
	if (szState[0] != L'\0')
	{
		m_nThreadSum = _ttoi(szState);
	}		
	m_bInit = TRUE;

}