#pragma once
#include <deque>

struct DGMS_ModuleInfo
{
	TCHAR strClientID[LEN_NAME];
	TCHAR strModuleName[LEN_NAME];
	TCHAR strModuleMD5[LEN_MD5];
	TCHAR strVersionName[LEN_VERSION];
	int   nModuleSize;
	TCHAR strLastModify[LEN_TIME];
	TCHAR strDesc[LEN_DESC];
	BOOL  bIsMatch;
};

struct DM_NeedUpdateModule
{
	TCHAR strModuleName[LEN_NAME];
	TCHAR strModuleMD5[LEN_NAME];
	TCHAR strSectionName[LEN_NAME];
};

typedef std::deque<DM_NeedUpdateModule> deqDMNeedUpdateModule;

class CDGMSUpdateClientUIF
{
public:
	CDGMSUpdateClientUIF(void);
	~CDGMSUpdateClientUIF(void);

	inline CAString LoadStringByID( HINSTANCE hIns , int nID )
	{
		CAString		strData;
		TCHAR			szMenu[MAX_PATH] = {0};
		LoadString( hIns , nID, szMenu, MAX_PATH );
		strData = szMenu;
		return strData;
	}

	CAString GetFileMD5(CAString strFilePathName);
	CAString GetClientID();
	// 	CAString m_strModulePath;
	// 	CAString m_strClientModuleFilePath;
	// 	CAString m_strClientTempModuleFilePath;
	// 	CAString m_strServerModuleFilePath;
	DWORD UploadClientInfo();
	DWORD UploadClientVersion();
	void WritePrivateProfileInt(LPCTSTR strKey,LPCTSTR strName,int dwCount,LPCTSTR strIni);
	void GetModuleFileFromServer(CAString& m_strModulePath, CAString& m_strClientModuleFilePath, 
		CAString& m_strClientTempModuleFilePath);
	DWORD UpdateByModuleFileProcess(CAString& m_strModulePath, CAString& m_strClientModuleFilePath, 
		CAString& m_strClientTempModuleFilePath, CAString& m_strServerModuleFilePath, CAString& m_strRollBackPath);


};




