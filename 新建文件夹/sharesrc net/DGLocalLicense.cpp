#include "StdAfx.h"
#include "DGLocalLicense.h"

CDGLocalLicense::CDGLocalLicense(void)
{
	m_hInst = LoadLibrary(L"DGLocalLicense.dll");
	m_pInit				= NULL;
	m_pUnInit			= NULL;
	m_pCheckLicense		= NULL;
	m_pOnFlashClock		= NULL;
	m_pGetOfflineInfo	= NULL;
	m_pOnOffline		= NULL;
	m_pOnBackNet		= NULL;
	m_pOnStoppageTime	= NULL;
	InitFunction(m_hInst);
}

CDGLocalLicense::~CDGLocalLicense(void)
{
	if (m_hInst)
	{
		FreeLibrary(m_hInst);
	}
}

void CDGLocalLicense::InitFunction(HMODULE hInst)
{
	if (hInst)
	{
		m_pInit				= (DGLL_Init)GetProcAddress(hInst, "Init");
		m_pUnInit			= (DGLL_UnInit)GetProcAddress(hInst, "UnInit");
		m_pCheckLicense		= (DGLL_CheckLicense)GetProcAddress(hInst, "CheckLicense");
		m_pOnFlashClock		= (DGLL_OnFlashClock)GetProcAddress(hInst, "OnFlashClock");
		m_pGetOfflineInfo	= (DGLL_GetOfflineInfo)GetProcAddress(hInst, "GetOfflineInfo");
		m_pOnOffline		= (DGLL_OnOffline)GetProcAddress(hInst, "OnOffline");
		m_pOnBackNet		= (DGLL_OnBackNet)GetProcAddress(hInst, "OnBackNet");
		m_pOnStoppageTime	= (DGLL_OnStoppageTime)GetProcAddress(hInst, "OnStoppageTime");
	}
}

BOOL CDGLocalLicense::Init(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	if (m_pInit)
	{
		bRtn = m_pInit(cr);
	}
	return bRtn;
}
BOOL CDGLocalLicense::UnInit(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	if (m_pUnInit)
	{
		bRtn = m_pUnInit(cr);
	}
	return bRtn;
}
BOOL CDGLocalLicense::CheckLicense(BOOL& bDefaultLogin,CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	if (m_pCheckLicense)
	{
		bRtn = m_pCheckLicense(bDefaultLogin,cr);
	}
	return bRtn;
}
BOOL CDGLocalLicense::OnFlashClock(LPTSTR lpLicPath, CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	if (m_pOnFlashClock)
	{
		bRtn = m_pOnFlashClock(lpLicPath, cr);
	}
	return bRtn;
}
BOOL CDGLocalLicense::GetOfflineInfo(int& nMode, int& nLeftTime, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pGetOfflineInfo)
	{
		bRtn = m_pGetOfflineInfo(nMode, nLeftTime, cr);
	}
	return bRtn;
}
BOOL CDGLocalLicense::OnOffline(LPTSTR lpLicPath,CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pOnOffline)
	{
		bRtn = m_pOnOffline(lpLicPath, cr);
	}
	return bRtn;
}

BOOL CDGLocalLicense::OnBackNet(CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pOnBackNet)
	{
		bRtn = m_pOnBackNet(cr);
	}
	return bRtn;
}

BOOL CDGLocalLicense::OnStoppageTime(int nTime, CErrorRecorder& cr)
{
	BOOL bRtn = FALSE;
	if (m_pOnStoppageTime)
	{
		bRtn = m_pOnStoppageTime(nTime, cr);
	}
	return bRtn;
}