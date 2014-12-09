#pragma once
#include <WTypes.h>
#include <deque>
#include <yvals.h>
#include <tchar.h>
#define _DG_ERROR_PUSH( cr, nError) \
	/*cr.PushError(nError); */	TCHAR tTrace[2048] = { 0 };\
	_stprintf(tTrace,L"File:%s Line:%d Fun:%s Error:%d",__FILEW__,__LINE__,__FUNCTIONW__, nError); cr.SetNewTrace(tTrace);

class CErrorRecorder
{
public:
	CErrorRecorder(LPCTSTR lpstrTrace = NULL);
	virtual ~CErrorRecorder(void);

	void PushInitError(DWORD dwError = GetLastError());
	void PushError(DWORD dwError);
	DWORD GetTailError();
	void AppandError(CErrorRecorder &errorRecorder);

	void SetNewTrace(LPCTSTR lpstrTrace, BOOL bShowTotal = TRUE, BOOL bShowElapsed = TRUE);

protected:
	virtual void OutputFullErrors();

	void GetElapsed(const FILETIME& ftCurrent, LPFILETIME lpftTotal, LPFILETIME lpftElapsed);

protected:
	std::deque<DWORD> m_dequeError;
	LPCTSTR m_lpstrTraceOriginal;
	LPCTSTR m_lpstrTraceCurrent;
	FILETIME m_ftOriginal;
	FILETIME m_ftTrace;
};
