#pragma once
#include "ErrorRecorder.h"
#include "spIPCComm.h"
#include "SocketBase.h"
#include "DGStruct.h"

typedef BOOL (_cdecl *HTLU_InitClientUnit)(CErrorRecorder& cr);
typedef BOOL (_cdecl *HTLU_InitSvrUnit)(CErrorRecorder& cr);
typedef BOOL (_cdecl *HTLU_UninitSvrUnit)(CErrorRecorder& cr);

typedef BOOL (_cdecl *HTLU_TestHTUpdateService)(CErrorRecorder& cr);
typedef BOOL (_cdecl *HTLU_GetCompanyInfo)(LPTSTR lpXmlPath, CErrorRecorder& cr);
typedef BOOL (_cdecl *HTLU_GetRestorePcInfo)(LPTSTR lpXmlPath, int nSize, LPCTSTR lpPcId, CErrorRecorder& cr);

class CHTLUpdateClass
{
public:
	CHTLUpdateClass(void);
	~CHTLUpdateClass(void);
	
	BOOL											InitNetWork();
	BOOL											UnInitNetework();
	BOOL											InitClientUnit(CErrorRecorder& cr);
	BOOL											InitSvrUnit(CErrorRecorder& cr);
	BOOL											UninitSvrUnit(CErrorRecorder& cr);
	BOOL											TestHTUpdateService(CErrorRecorder& cr);
	BOOL											GetCompanyInfo(LPTSTR lpXmlPath, CErrorRecorder& cr);
	BOOL											GetRestorePcInfo(LPTSTR lpXmlPath, int nSize, LPCTSTR lpPcId, CErrorRecorder& cr);

private:
	void											InitFunction(HMODULE hInst);

	HMODULE									m_hInst;
	HTLU_InitClientUnit						m_pInitClientUnit;
	HTLU_InitSvrUnit							m_pInitSvrUnit;
	HTLU_UninitSvrUnit						m_pUninitSvrUnit;
	HTLU_TestHTUpdateService			m_pTestHTUpdateService;
	HTLU_GetCompanyInfo					m_pGetCompanyInfo;
	HTLU_GetRestorePcInfo				m_pGetRestorePcInfo;
};
