/*
*线程池模块
*thread_pool_run():作业线程
*add_task_run()：将任务丢至线程池中
*print_state_run()：调试时使用，实时显示线程池中有多少线程在跑
*read_config_run()：读取配置信息，是否开启调试线程及日志输出模块
*/

#pragma once
#include "windows.h"
#include "TaskUnit.h"
#include "Logger.h"
#include "Mutex.h"
#include <deque>

using namespace std;

enum Service_Type
{
	DogService4 = 0,
	DogFlow,
	HTUpdateService,
	DGFileApprovalServer,
	DGFileUploadService,
	MobileServer,
};

class CThreadPool
{
public:
	CThreadPool(int nServiceType);
	~CThreadPool(void);

	void								init_thread_pool(int nThreadSum ,int nThrTimeOut = 2);			
	void								add_task_to_deq(CTaskUnit* TaskUnit);
	//线程函数
	void								thread_pool_run();
	BOOL								add_task_run();
	void								print_state_run();
	void								read_config_run();

private:
	void								get_cfg_path();
	void								get_log_path();
	void								read_cfg();

	BOOL								m_bThreadPoolRun;
	BOOL								m_bAddTaskRun;
	BOOL								m_bEndPrint;
	BOOL								m_bReadCfg;
	BOOL								m_bLogger;
	BOOL								m_bDebug;
	BOOL								m_bInit;

	int									m_nThreadSum;
	int									m_nThreadTimeOut;
	int									m_TimeCount;
	int									m_ServiceType;
	LONG								m_nThreadRunCount;


	CAString								m_strIniPath;
	CAString								m_strLogPath;

	HANDLE								m_hWaitingWork;//通知线程有任务
	HANDLE								m_hGotgWork;//回复主线程,线程已经获得
	HANDLE								m_hAddTask;

	MutexLock							m_LockDeq;

	deque<CTaskUnit*>					m_DequeTask;
};
