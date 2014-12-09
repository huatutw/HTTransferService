#include "StdAfx.h"
#include "Logger.h"
#include <process.h>

unsigned int __stdcall  log_run(void* p)
{
	CLogger* pLog = (CLogger*)p;
	if (pLog)
	{
		pLog->check_log_run();
	}
	return 0;
}

CLogger::CLogger(CAString strLogPath)
{
	m_bExit = FALSE;
	m_hExit = CreateEvent( NULL, FALSE, FALSE , NULL);
	m_strLogPath = strLogPath;
}

CLogger::~CLogger(void)
{	
	m_bExit = TRUE;
	WaitForSingleObject(m_hExit, 2000);
	CloseHandle(m_hExit);
}

 CLogger* CLogger::_this = NULL;

 CLogger* CLogger::CreateInstance(CAString strLogPath)
{
	if (!_this)
	{
		_this = new CLogger(strLogPath);

		HANDLE h = (HANDLE)_beginthreadex(NULL, 0, log_run, _this, 0, NULL);
		CloseHandle(h);
	}
	return _this;
}

void CLogger::DeleteInstance()
{
	if (_this)
	{
		delete _this;
		_this = NULL;
	}
}

void CLogger::add_log_to_deque(wstring strLog)
{
	m_lock.Lock();
	m_dqe.push_back(strLog);
	m_lock.UnLock();
}

void CLogger::check_log_run()
{
	while(!m_bExit)
	{
		if (m_dqe.size() <= 0)
		{
			Sleep(1000);
			continue;
		}
		
		wstring str = get_current_time();
		m_lock.Lock();
		str += m_dqe.front();
		m_dqe.pop_front();
		m_lock.UnLock();	

		output_log_to_file(str);		
	}

	Sleep(10*1000);
	SetEvent(m_hExit);
}

wstring CLogger::get_current_time()
{
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime (&rawtime );
	TCHAR szTime[1024] ={ 0 };
	_stprintf( szTime, L"%4d-%02d-%02d %02d:%02d:%02d ",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
		timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	wstring strTime = szTime;
	return strTime;
}

BOOL	 CLogger::output_log_to_file(wstring strLogInfo)
{
	HANDLE hNameFile = CreateFile(m_strLogPath.c_str(), GENERIC_WRITE |GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hNameFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwSize = GetFileSize (hNameFile, NULL); 
		SetFilePointer (hNameFile, 0, NULL, FILE_END);
		strLogInfo += L"\r\n\r\n";
		DWORD writesize = 0;
		if (0 == dwSize)
		{
			WORD a = 0xfeff;
			WriteFile(hNameFile, &a, 2, &writesize, NULL);
		}
		DWORD stringsize = wcslen(strLogInfo.c_str());
		WriteFile(hNameFile, (char*)strLogInfo.c_str(), stringsize*(sizeof(TCHAR)), &writesize, NULL);
		CloseHandle(hNameFile);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}