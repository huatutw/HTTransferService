#pragma once
#include "Mutex.h"
#include <time.h>
#include <string>
#include <deque>
#include "AString.h"

using namespace std;

class CLogger
{
public:
	~CLogger(void);	

	static CLogger*				CreateInstance(CAString strLogPath);
	static void					DeleteInstance();
	void						check_log_run();
	void						add_log_to_deque(wstring strLog);

private:
	CLogger(CAString strLogPath);					
	static CLogger*			 _this;
	
	BOOL						output_log_to_file(wstring strLogInfo);
	wstring						get_current_time();	

	BOOL						m_bExit;
	HANDLE						m_hExit;
	MutexLock					m_lock;
	deque<wstring>				m_dqe;
	wstring						m_strLogPath;
};
