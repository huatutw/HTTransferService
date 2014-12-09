#pragma once

class CUpdateMyself
{
public:
	CUpdateMyself(void);
	~CUpdateMyself(void);

	BOOL IsNeedUpdateMyself(CString strTempPath);
	void UpdateMyself();
	void UpdateSecService();
	void UpdateLocalService();

	CString m_strAppDataPath;
	CString m_strProgramPath;

	HANDLE m_hServiceMutex;
	HANDLE m_hSecServiceMutex;

};
