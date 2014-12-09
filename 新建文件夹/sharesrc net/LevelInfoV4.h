// LevelInfoV4.h: interface for the CLevelInfoV4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVELINFOV4_H__07C038E2_C9CD_477E_B3D1_B38145363D14__INCLUDED_)
#define AFX_LEVELINFOV4_H__07C038E2_C9CD_477E_B3D1_B38145363D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <map>

class CLevelInfoV4  
{
public:
	CLevelInfoV4();
	virtual ~CLevelInfoV4();

	int	   m_nLevelID;
	LPTSTR m_strLevelName;	//文件等级名称
};

typedef std::map<int,CLevelInfoV4*> MAP_LEVEL;

#endif // !defined(AFX_LEVELINFOV4_H__07C038E2_C9CD_477E_B3D1_B38145363D14__INCLUDED_)
