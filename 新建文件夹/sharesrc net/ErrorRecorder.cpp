#include "stdafx.h"
#include "ErrorRecorder.h"
#include <tchar.h>

CErrorRecorder::CErrorRecorder(LPCTSTR lpstrTrace)
{
	if (lpstrTrace == NULL)
	{
		lpstrTrace = L"Start";
		m_lpstrTraceOriginal = lpstrTrace;
		m_lpstrTraceCurrent = lpstrTrace;
	}
	else
	{
		m_lpstrTraceOriginal = lpstrTrace;
		m_lpstrTraceCurrent = L"Last";
	}

	m_dequeError.push_back(0);
	if (m_lpstrTraceOriginal)
	{
		SYSTEMTIME stOriginal;
		GetLocalTime(&stOriginal);
		SystemTimeToFileTime(&stOriginal, &m_ftOriginal);
		m_ftTrace = m_ftOriginal;
	}
}

CErrorRecorder::~CErrorRecorder(void)
{
	OutputFullErrors();
	m_dequeError.clear();
}

void CErrorRecorder::PushInitError(DWORD dwError)
{
	OutputFullErrors();
	m_dequeError.clear();
	m_dequeError.push_back(dwError);
}

void CErrorRecorder::PushError(DWORD dwError)
{
	m_dequeError.push_back(dwError);
}


DWORD CErrorRecorder::GetTailError()
{
	return m_dequeError.back();
}

void CErrorRecorder::AppandError(CErrorRecorder &errorRecorder)
{
	std::deque<DWORD>::iterator item = errorRecorder.m_dequeError.begin();
	for (item; item != errorRecorder.m_dequeError.end(); item++)
	{
		PushError(*item);
	}
	errorRecorder.PushInitError(0);
}

void CErrorRecorder::GetElapsed(const FILETIME& ftCurrent, LPFILETIME lpftTotal, LPFILETIME lpftElapsed)
{
	LARGE_INTEGER liCurrent = {ftCurrent.dwHighDateTime, ftCurrent.dwLowDateTime};
	LARGE_INTEGER liOriginal = {m_ftOriginal.dwHighDateTime, m_ftOriginal.dwLowDateTime};
	LARGE_INTEGER liTrace = {m_ftTrace.dwHighDateTime, m_ftTrace.dwLowDateTime};
	LARGE_INTEGER liBuff;
	
	liBuff.QuadPart = liCurrent.QuadPart - liOriginal.QuadPart;
	lpftTotal->dwHighDateTime = liBuff.HighPart;
	lpftTotal->dwLowDateTime = liBuff.LowPart;

	liBuff.QuadPart = liCurrent.QuadPart - liTrace.QuadPart;
	lpftElapsed->dwHighDateTime = liBuff.HighPart;
	lpftElapsed->dwLowDateTime = liBuff.LowPart;
}

#define ERROR_BUFF_LENGTH	4096
#define SMALL_BUFF	16
void CErrorRecorder::OutputFullErrors()
{
	//if ((m_dequeError.size() == 1) && (GetTailError() == ERROR_SUCCESS))
	//{
	//	return;
	//}

	//TCHAR tcErrors[ERROR_BUFF_LENGTH] = {};
	//TCHAR tcBuff[SMALL_BUFF] = {};
	//std::deque<DWORD>::reverse_iterator item = m_dequeError.rbegin();
	//for (item; item != m_dequeError.rend(); item++)
	//{
	//	_stprintf_s(tcBuff, SMALL_BUFF, _T("%x --> "), *item);
	//	_tcscat_s(tcErrors, ERROR_BUFF_LENGTH, tcBuff);
	//}

	//_tcscat_s(tcErrors, ERROR_BUFF_LENGTH, _T("current\r\n"));
	//OutputDebugString(tcErrors);
}


void CErrorRecorder::SetNewTrace(LPCTSTR lpstrTrace, BOOL bShowTotal, BOOL bShowElapsed)
{
	OutputDebugString(lpstrTrace);
	return ;
	if (lpstrTrace == NULL)
	{
		lpstrTrace = L"Current";
	}

	SYSTEMTIME stCurrent;
	GetLocalTime(&stCurrent);
	FILETIME ftCurrent;
	SystemTimeToFileTime(&stCurrent, &ftCurrent);

	FILETIME ftTotal;
	FILETIME ftElapsed;	
	GetElapsed(ftCurrent, &ftTotal, &ftElapsed);

	m_ftTrace = ftCurrent;

	if (bShowTotal)
	{
		LARGE_INTEGER liTotal = {ftTotal.dwHighDateTime, ftTotal.dwLowDateTime};
		TCHAR tcTrace[ERROR_BUFF_LENGTH] = {};
		_stprintf_s(tcTrace, ERROR_BUFF_LENGTH, _T("%s elapsed %f seconds"), lpstrTrace, ((float)liTotal.QuadPart) / 10000000);
		OutputDebugString(tcTrace);
	}

	if (bShowElapsed)
	{
		LARGE_INTEGER liElapsed = {ftElapsed.dwHighDateTime, ftElapsed.dwLowDateTime};
		TCHAR tcTrace[ERROR_BUFF_LENGTH] = {};
		_stprintf_s(tcTrace, ERROR_BUFF_LENGTH, _T("%s elapsed %f seconds"), lpstrTrace, ((float)liElapsed.QuadPart) / 10000000);
		OutputDebugString(tcTrace);
	}

	m_lpstrTraceCurrent = lpstrTrace;
}