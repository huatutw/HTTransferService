// ADataTime.h: interface for the CADateTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADATATIME_H__0EA64CB3_9B71_4539_A918_AD632E5B0617__INCLUDED_)
#define AFX_ADATATIME_H__0EA64CB3_9B71_4539_A918_AD632E5B0617__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CADateTime  
{
public:
	CADateTime();
	virtual ~CADateTime();

	DATE		m_Date;

public:
	BOOL	FormatTime(DATE dtSrc, struct tm& tmDest);
	int		GetYear();
	int		GetMonth();
	int		GetDay();
	int		GetHour();
	int		GetMinute();
	int		GetSecond();
	int		GetDayOfWeek();
	int		GetDayOfYear();
};

#endif // !defined(AFX_ADATATIME_H__0EA64CB3_9B71_4539_A918_AD632E5B0617__INCLUDED_)
