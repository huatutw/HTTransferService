
#pragma once
#include <string>
using namespace std;
#define	GLOBAL_CHECKPASSWD_ON_CLIENTRUN				L"global_checkpasswd_on_clientrun"
#define GLOBAL_CLOSEMACRO_ON_CLIENTRUN					L"global_closemacro_on_clientrun"
#define GLOBAL_CHECK_DGCLIENT_ID								L"global_check_dgclient_id"
#define GLOBAL_CHECK_UPDATE_ID									L"global_check_update_id"
#define	SECTION_CHECKDOG_ON_CLIENTRUN					L"section_checkdog_on_clientrun"
#define SECTION_HIDEICON												L"section_hideicon"
#define SECTION_HIDEPROPETY										L"section_hidepropety"
#define	SECTION_HIDETRAY												L"section_hidetray"


class CCfgMgr
{
public:
	CCfgMgr();
	~CCfgMgr(void);
	/*
	功能：寻找cfg.xml文件中，index_name的value为AttributeName，判断able的value是否为1
	参数：AttributeName：index_name的value；bAble：able的value；bFlush：是否重新加载cfg.xml
	返回：成功返回TRUE;失败返回FALSE
	*/
	BOOL			ReadCfgData(TCHAR *szAttributeName, BOOL &bAble, BOOL bFlush = FALSE);

private:
	wstring		GetCfgPathFromReg();
	wstring		LoadCfgXml();
	void			DecryptCfgXmlByRC4();

	wstring		m_CfgXmlPath;
	wstring		m_CfgData;
};