#include "StdAfx.h"
#include "LConfig.h"
#include "DGVersion.h"

CLConfig::CLConfig(void)
{
}

CLConfig::~CLConfig(void)
{
}

BOOL CLConfig::IsDomainMode(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	bRtn = GetClientCurrentVersion() & VERSION_RUNDLL_SHANGQI;
	return bRtn;
}

BOOL CLConfig::IsAutoDownloadCfg(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	bRtn = GetClientCurrentVersion()&0x8000;
	return bRtn;
}

BOOL CLConfig::IsDeniedAccessEncrypteFile(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	bRtn = GetClientCurrentVersion()&VERSION_RUNDLL_ACCESS;
	return bRtn;
}

BOOL CLConfig::IsChangeCFGNormal(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	bRtn = GetClientCurrentVersionEx()&0x4;
	return bRtn;
}

BOOL CLConfig::IsDGClientDefaultOpen(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	bRtn = GetClientCurrentVersion()&0x80000000;
	return bRtn;
}

BOOL CLConfig::IsTrayNoDisplay(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	bRtn = GetClientCurrentVersion()&VERSION_RUNDLL_TRAYNODISPLAY;
	return bRtn;
}

BOOL CLConfig::IsNonDomainModeOpenClose(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	bRtn = GetClientCurrentVersion()&VERSION_DGCLIENT_OPENCLOSE;
	return bRtn;
}

BOOL CLConfig::IsAlwaysShowTray(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	bRtn = GetClientCurrentVersion()&VERSION_DGCLIENT_ALWAYSSHOWTRAY;
	return bRtn;
}

BOOL CLConfig::IsUsbForbid(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	bRtn = GetClientCurrentVersion()&VERSION_DGCLIENT_USBSTOR_FORBID;
	return bRtn;
}

BOOL CLConfig::IsFullScanUsb(CErrorRecorder &cr)
{
	BOOL bRtn = FALSE;
	bRtn = GetClientCurrentVersion()&VERSION_STANDARD_USBSTOR;
	return bRtn;
}