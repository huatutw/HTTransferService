// ClientInfo.cpp: implementation of the CClientInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "TestHookDll.h"
#include "ClientInfo.h"
#include "ClientDog.h"
#include "DGCommonFunc.h"
//#include "WebDataFun.h"
/*#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI* GETDOGDATA)(int nDogMoudule,DWORD* pdwDogCount,DWORD* pdwDogID,BYTE btKey[128]);
typedef BOOL (WINAPI* CHECKUSERPASS)(LPCTSTR strUser,LPCTSTR strPass,int* nUserType,DWORD* pdwPriv);
typedef BOOL (WINAPI* CHECKUSERPASS2)(LPCTSTR strUser,LPCTSTR strPass,int* nUserType,DWORD* pdwPriv,DWORD* pdwCryptLevel,int* pnPolicyType,INT_DEQUE& PolicyDeque);

typedef int (WINAPI* GETUSERCOUNT)();
typedef BOOL (WINAPI* GETUSERINFO)(int index,char* pszUserName,char* pszUserInfo,char* pszUserDescribe,char* pszPassword,int& nUserType,int& nOverTime,long& nPopedom);
typedef BOOL (WINAPI* GETUSERINFO2)(int index,char* pszUserName,char* pszUserInfo,char* pszUserDescribe,char* pszPassword,int& nUserType,int& nOverTime,long& nPopedom,DWORD* pdwCryptLevel,int* pnPolicyType,INT_DEQUE& PolicyDeque,char* pszGroupName);
typedef LPBYTE (WINAPI* GETBUFFER)(int& size);

LPBYTE GetContentFromFile(LPCTSTR lpszFileName,LPHANDLE phFile,LPHANDLE phMap,DWORD* pdwSize)
{
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		*phFile = NULL;
		*phMap = NULL;
		return NULL;
	}
	
	DWORD dwHigh = 0;
	DWORD dwLow = GetFileSize(hFile,&dwHigh);
	
	if (dwLow == 0)
		dwLow = *pdwSize;
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,dwHigh,dwLow,NULL);
	
	if (hMap == NULL || hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		*phFile = NULL;
		*phMap = NULL;
		return NULL;		
	}
	
	LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_READ | FILE_MAP_WRITE,0,0,dwLow);
	
	if (lpAddr)
	{
		*phFile = hFile;
		*phMap = hMap;
		*pdwSize = dwLow;
		return lpAddr;
	}
	
	CloseHandle(hMap);
	CloseHandle(hFile);
	*phFile = NULL;
	*phMap = NULL;
	return NULL;
}

BOOL LoadNewLicense(LicenseInfo4& info)
{
	
	TCHAR lpszSystemDir[1024] = {0};
	
	GetSystemDirectory(lpszSystemDir,1023);
	
	CAString strSystem = lpszSystemDir;
	
	if (strSystem.GetAt(strSystem.GetLength()-1) != TEXT('\\'))
		strSystem += TEXT("\\");
	strSystem += TEXT("DGLicense.lic");
	
	HANDLE hFile = CreateFile(strSystem.GetBuffer(0),GENERIC_READ ,FILE_SHARE_READ|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	strSystem.ReleaseBuffer();
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"Open new licfile failed");
		return FALSE;
	}
	
	DWORD dwHigh = 0;
	DWORD dwLow = GetFileSize(hFile,&dwHigh);
	if (dwLow < sizeof(info))
	{
		return FALSE;
	}
	
	LPBYTE bt = new BYTE[dwLow];
	DWORD read;
	ReadFile(hFile,bt,dwLow,&read,NULL);
	DGCommonFunc::Encrypt_RC4(bt,sizeof(LicenseInfo4),"DocGuarder_Encrypt_System_Made_in_HangZhou_2006_8_29");

	memcpy((char*)&info, bt, sizeof(info));
	
	if (bt)
	{
		delete [] bt;
		bt = NULL;
	}

	BOOL bRet = FALSE;
	char szVam[10] ={ 0 };
	memcpy(szVam, info.btFlag, 6);
	
	if (strcmp(szVam,"vamtoo") == 0)
	{
		bRet = TRUE;
	}
	else
	{
		OutputDebugStringA(szVam);
		OutputDebugString(L" 检测新软授权失败");
	}

	CloseHandle(hFile);
		
	return bRet;
	
}
BOOL LoadLicFile(LicenseInfo3& info,DWORD& cbSize)
{
	TCHAR lpszSystemDir[1024] = {0};
	
	GetSystemDirectory(lpszSystemDir,1023);
	
	CAString strSystem = lpszSystemDir;
	
	if (strSystem.GetAt(strSystem.GetLength()-1) != TEXT('\\'))
		strSystem += TEXT("\\");
	strSystem += TEXT("DGLicense.lic");
	
	OutputDebugString(strSystem);

	HANDLE hFile = CreateFile(strSystem.GetBuffer(0),GENERIC_READ ,FILE_SHARE_READ|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	strSystem.ReleaseBuffer();
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"Open old licfile failed");
		return FALSE;
	}
	
	DWORD dwHigh = 0;
	DWORD dwLow = GetFileSize(hFile,&dwHigh);
	
	
	
	LPBYTE bt = new BYTE[dwLow];
	DWORD read;
	ReadFile(hFile,bt,dwLow,&read,NULL);

	if (dwLow != sizeof(LicenseInfo3))
	{
		CloseHandle(hFile);
		if (bt)
		{
			delete [] bt;
			bt = NULL;
		}
		return FALSE;
	}

	DGCommonFunc::Encrypt_RC4(bt,sizeof(LicenseInfo3),"DocGuarder_Encrypt_System_Made_in_HangZhou_2006_8_29");
	
	cbSize = dwLow;

	
	memcpy(&info,bt,sizeof(LicenseInfo3));
	CloseHandle(hFile);
	if (bt)
	{
		delete [] bt;
		bt = NULL;
	}
	return TRUE;
}

CClientInfo::CClientInfo()
{

	BOOL bHasDog = FALSE ;
	m_dwAuthorizedMoules = 0;
	m_nStrategy = 0;
	m_bNewDog= FALSE;
	m_bNewSoftLic = FALSE;
	m_dwDogCount = 0;
	m_dwDogID = 0;
	m_lDeadline = 0 ;
	memset(m_btKey,0,128);
	CClientDogEx newDog;
	CClientDog oldDog;
	int nDogRet = 0;

#ifdef HUATU
	newDog.m_dwPass1 = 0x4CEF;
	newDog.m_dwPass2 = 0x70B3;
	newDog.m_dwPass3 = 0x7038;
	newDog.m_dwPass4 = 0x83B3;

	oldDog.m_dwPass1 = 0x4CEF;
	oldDog.m_dwPass2 = 0x70B3;
	oldDog.m_dwPass3 = 0x7038;
	oldDog.m_dwPass4 = 0x83B3;
#else
	newDog.m_dwPass1 = 0xBFEF;
	newDog.m_dwPass2 = 0x8370;
	newDog.m_dwPass3 = 0xB038;
	newDog.m_dwPass4 = 0x7080;

	oldDog.m_dwPass1 = 0xBFEF;
	oldDog.m_dwPass2 = 0x8370;
	oldDog.m_dwPass3 = 0xB038;
	oldDog.m_dwPass4 = 0x7080;
#endif
	
	nDogRet = newDog.ReadDogData();

	CAString strRet = L"" ;
	strRet.Format( L"nDogRet = %d", nDogRet ) ;
	OutputDebugString(strRet) ;
	if( nDogRet== ERR_SUCCESS)
	{
	    bHasDog = TRUE ;
		int nPro = 0;
		while (!newDog.m_dwProduct[nPro])
		{
			nPro++;
			if (nPro >= 8)
			{
				break;
			}
		}
		m_nMode = 0;
		m_bNewDog = TRUE;
		OutputDebugString(_T("read new dog succeed"));
		m_dwDogCount = newDog.m_nCount[nPro];
		m_nStrategy = newDog.m_dwTactics[nPro];
		if(strlen((char*)newDog.m_btKey) >= 128)
		{
			memset(m_btKey,0,128);
			memcpy(m_btKey,newDog.m_btKey,127);
		}
		else
		{
			memcpy(m_btKey,newDog.m_btKey,128);
		}
		SYSTEMTIME st;
		memcpy(&st, &newDog.m_tTimeOver[nPro], sizeof(SYSTEMTIME));
		struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};

		CAString strEndTime = L"" ;
		strEndTime.Format( _T("hare dogEnd Time: %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond ) ;
		OutputDebugString( strEndTime ) ;

		m_lDeadline = 0;
		m_dwDogID = newDog.m_dwDogID;
		USES_CONVERSION;
		CAString strError;
		strError.Format(L"Strategy: %d DogCount: %d Key: %s DogID: %u",m_nStrategy, m_dwDogCount,  A2T((char *)newDog.m_btKey));
		OutputDebugString(strError);
		ParseVersion(newDog.m_dwProduct, newDog.m_nProductViersion);

	
	}
	else if (nDogRet == ERR_NEWOPENOLDDOG && (!oldDog.ReadDogData()))
	{
		bHasDog = TRUE ;
		m_nMode = 0;
		OutputDebugString(_T("读老狗成功"));
		m_dwDogCount = oldDog.m_dwModuleCount[0];
		
		if(strlen((char*)oldDog.m_btKey) >= 128)
		{
			memset(m_btKey,0,128);
			memcpy(m_btKey,oldDog.m_btKey,127);
		}
		else
		{
			memcpy(m_btKey,oldDog.m_btKey,128);
		}
		
		m_dwDogID = oldDog.m_dwDogID;
	}
	else 
	{
		m_nMode = 1;
		OutputDebugString(_T("读狗失败"));
		USES_CONVERSION;
		LicenseInfo4 NewInfo = { 0 };
		m_bNewSoftLic = LoadNewLicense(NewInfo);
		LicenseInfo3 info;
		DWORD cbSize;
		OutputDebugString(_T("开始读软授权"));
		if ((!m_bNewSoftLic) && LoadLicFile(info,cbSize) )
		{
			OutputDebugString(TEXT("GetList"));
			if (info.mask & BAND_MAC)
			{
				CAString strMac;
				strMac = DGCommonFunc::GetMacByNetBIOS();
				CAString strMac1;
				strMac1 = info.strMacAddress;
				//if (strcmp(T2A(strMac),info.strMacAddress) != 0)
				if(strMac != strMac1)
				{
					OutputDebugString(strMac);
					OutputDebugString(strMac1);
					OutputDebugString(_T("MAC地址不匹配"));
					return;
				}
			}
			
			if (info.mask & BAND_HARDDISK)
			{
				OutputDebugString(_T("BAND_HARDDISK"));
				CAString strHardDisk = DGCommonFunc::GetHardDiskSerial();
				OutputDebugString(strHardDisk);
				if (strcmp(T2A(strHardDisk),info.strHardID) != 0)
				{
					OutputDebugString(strHardDisk);
					CAString str = info.strHardID;
					OutputDebugString(str);
					OutputDebugString(_T("硬件号不匹配"));
					return;
				}
			}
			
			m_dwDogCount = info.uCount;
			m_dwDogID = info.uLicenseID;
			m_nTime = info.nTime;
			if (strlen(info.strUserPass)>=128)
			{
				memset(m_btKey,0,128);
				memcpy(m_btKey,info.strUserPass,127);
			}
			else
			{
				memcpy(m_btKey,info.strUserPass,128);
			}
		
			OutputDebugStringA(info.strUserPass);
		}
		else if(m_bNewSoftLic)
		{
			OutputDebugString(_T("读取新软授权成功"));
			m_nMode = 1;
  			if (NewInfo.dwBandMask & BAND_MAC)
  			{
  				CAString strMac;
 				strMac = DGCommonFunc::GetMacByNetBIOS();
 				CAString strMac1;
 				strMac1 = NewInfo.cBandInfo;
  				if(strMac != strMac1)
 				{
  					OutputDebugString(strMac);
  					OutputDebugString(strMac1);
  					OutputDebugString(_T("MAC地址不匹配"));
  					return;
 				}
  			}
  			
  			if (NewInfo.dwBandMask & BAND_HARDDISK)
 			{
  				OutputDebugString(_T("BAND_HARDDISK"));
 				CAString strHardDisk = DGCommonFunc::GetHardDiskSerial();
  				OutputDebugString(strHardDisk);
  				if (strcmp(T2A(strHardDisk),NewInfo.cBandInfo) != 0)
 				{
 					OutputDebugString(strHardDisk);
  					CAString str = NewInfo.cBandInfo;
 					OutputDebugString(str);
 					OutputDebugString(_T("硬件号不匹配"));
  					return;
  				}
  			}

			int nSoftPro = 0;
			while (!NewInfo.dwProduct[nSoftPro])
			{
				nSoftPro++;
				if (nSoftPro >= 8)
				{
					break;
				}
			}
			m_nStrategy = NewInfo.dwTactics[nSoftPro] ;
			m_dwDogCount = NewInfo.nCount[nSoftPro];
			m_dwDogID = NewInfo.dwDogID;
			SYSTEMTIME sysEnd = { 0 };
			memcpy((char*)&sysEnd, (char*)&NewInfo.tTimeOver[nSoftPro], sizeof(sysEnd));
			struct tm gmEnd = {sysEnd.wSecond, sysEnd.wMinute, sysEnd.wHour, sysEnd.wDay, sysEnd.wMonth-1, sysEnd.wYear-1900, 
				               sysEnd.wDayOfWeek, 0, 0};
		    m_lDeadline = 0;
			
			SYSTEMTIME st = { 0 };
			SYSTEMTIME stN = { 0 };
			memcpy((char*)&st, (char*)&NewInfo.tTimeOver[nSoftPro], sizeof(st));
			CAString strTime;
			strTime.Format(L" %d-%d-%d %d:%d:%d", st.wYear, st.wMonth, st.wDay, st.wHour,st.wMinute,st.wSecond);
			OutputDebugString(strTime);
			struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900,st.wDayOfWeek, 0, 0};
			unsigned int tEnd = 0;

			GetLocalTime(&stN);
			struct tm gmN = {stN.wSecond, stN.wMinute, stN.wHour, stN.wDay, stN.wMonth-1, stN.wYear-1900, stN.wDayOfWeek, 0, 0};
			unsigned int tBeg = 0;
			m_nTime = ((tEnd - tBeg)/60)/60;
			strTime.Format(L"nTime = %d ", m_nTime);
			OutputDebugString(strTime);
			if (strlen((const char*)NewInfo.btKey)>=128)
			{
				memset(m_btKey,0,128);
				memcpy(m_btKey,NewInfo.btKey,127);
			}
			else
			{
				memcpy(m_btKey,NewInfo.btKey,128);
			}
			ParseVersion(NewInfo.dwProduct, NewInfo.nProductViersion);
		}
		else
		{
			OutputDebugString(L"读软授权失败");
		}
	}
	
	m_pCurrentNode = NULL;

	if( bHasDog )
	{
		TCHAR szSystemPath[1024] = { 0 } ;
		GetSystemDirectory( szSystemPath, 1023 ) ;
		
		CAString strSystemPath = szSystemPath ;
		if (strSystemPath.GetAt(strSystemPath.GetLength()-1) != TEXT('\\'))
			strSystemPath += TEXT("\\");
	    strSystemPath += L"DGLicense.lic" ;

		if( PathFileExists( strSystemPath ) )
		{
			OutputDebugString(L"读狗成功，软授权删除");
            DeleteFile( strSystemPath ) ;
		} 
	}
}

CClientInfo::~CClientInfo()
{
	int size = m_arrNodeInfo.size();
	for (int i=0;i<size;i++)
	{
		CClientNodeInfo* info = m_arrNodeInfo.front();
		m_arrNodeInfo.pop_front();
		delete info;
	}
	m_arrOverTimeNodeInfo.clear();
	FreeLibrary(m_hBaseDll);
	m_hBaseDll = NULL;
}

int CClientInfo::GetDogInfo()
{
	return m_dwDogCount;
}

extern LPTSTR STRDUP(LPCTSTR strData);

CClientNodeInfo* CClientInfo::FindNodeByKeyInNormal(LPCTSTR strComputer,LPCTSTR strMac)
{
// 	CString strSql = "";
// 	strSql.Format(_T("SELECT CN_IP, CN_COMPUTER, CN_LOGINNAME, CN_MAC, CN_RECENTTIME, CN_OVERTIME, CN_VER")
// 		_T(" FROM TN_MONITOR")
// 		_T(" WHERE CN_LOGINNAME = '%s' AND CN_MAC = '%s'"),
// 		strComputer, strMac);
// 	TRACE(strSql);
// 	CDatabaseConnect::m_pRecordset->Open(_variant_t(strSql), _variant_t((IDispatch *)CDatabaseConnect::m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
// 	
// 	if(!(CDatabaseConnect::m_pRecordset->adoEOF))  
// 	{
// 		CClientNodeInfo *ClientNodeInfo = new CClientNodeInfo;
// 		ClientNodeInfo->m_strIP	=		STRDUP(OLE2T(_bstr_t(CDatabaseConnect::m_pRecordset->GetCollect((long)(0)))));
// 		ClientNodeInfo->m_strComputer =	STRDUP(OLE2T(_bstr_t(CDatabaseConnect::m_pRecordset->GetCollect((long)(1)))));
// 		ClientNodeInfo->m_strUser =		STRDUP(OLE2T(_bstr_t(CDatabaseConnect::m_pRecordset->GetCollect((long)(2)))));
// 		ClientNodeInfo->m_strMac =		STRDUP(OLE2T(_bstr_t(CDatabaseConnect::m_pRecordset->GetCollect((long)(3)))));
// 		
// 		COleDateTime Date;
// 		CString strRecentTime =	STRDUP(OLE2T(_bstr_t(CDatabaseConnect::m_pRecordset->GetCollect((long)(4)))));
// 		Date.ParseDateTime(strRecentTime, 0);
// 		Date.GetAsSystemTime(ClientNodeInfo->m_PrevTime);
// 
// 		ClientNodeInfo->m_bOverTime =	_ttoi(OLE2T(_bstr_t(CDatabaseConnect::m_pRecordset->GetCollect((long)(5)))));
// 		ClientNodeInfo->m_pszVersion =	STRDUP(OLE2T(_bstr_t(CDatabaseConnect::m_pRecordset->GetCollect((long)(6)))));
// 		
// 		return ClientNodeInfo;
// 	}

	CLIENT_DEQUE::iterator i;

	for (i=m_arrNodeInfo.begin();i!=m_arrNodeInfo.end();++i)
	{
		if (_tcsicmp((*i)->m_strComputer,strComputer) == 0 &&
			_tcsicmp((*i)->m_strMac,strMac) == 0)
		{
			return (*i);
		}
	}

	return NULL;
}

CClientNodeInfo* CClientInfo::FindNodeByKeyInOverTime(LPCTSTR strComputer,LPCTSTR strMac)
{

	CLIENT_DEQUE::iterator i;
	for (i=m_arrOverTimeNodeInfo.begin();i!=m_arrOverTimeNodeInfo.end();++i)
	{
		if (_tcsicmp((*i)->m_strComputer,strComputer) == 0 &&
			_tcsicmp((*i)->m_strMac,strMac) == 0)
		{
			return (*i);
		}
	}
	return NULL;
}

int CClientInfo::GetNodeTypeInfo(int& OverTimeNode)
{
	CheckAllOverTimeNode();

	int size = m_arrNodeInfo.size();

	int count = 0;
	for (int i=0;i<size;i++)
	{
		CClientNodeInfo* pInfo = m_arrNodeInfo[i];
		if (!pInfo->m_bOverTime)
			count ++;
	}

	OverTimeNode = size - count;

	return count;
}

BOOL CClientInfo::AddNewClientNode(LPCTSTR strComputer,LPCTSTR strMac,LPCTSTR strCurUser,LPCTSTR szHost)
{
	CClientNodeInfo* pNormalInfo = FindNodeByKeyInNormal(strComputer,strMac);
	if (pNormalInfo) //如果是节点信息已经存在则刷新节点的上次访问时间
	{
		int nOverTime = 0;
		if (!pNormalInfo->m_bOverTime || (pNormalInfo->m_bOverTime && GetNodeTypeInfo(nOverTime) < GetDogInfo()))
		{
			GetLocalTime(&pNormalInfo->m_PrevTime);
			pNormalInfo->m_dwPrevTickCount = GetTickCount();
			pNormalInfo->GetClientPriv2(m_hBaseDll,strCurUser,TRUE);
			if (pNormalInfo->m_strIP)
				free(pNormalInfo->m_strIP);
			pNormalInfo->m_strIP = _tcsdup(szHost);
			m_pCurrentNode = pNormalInfo;
			return TRUE;
		}else
		{
			return FALSE;
		}
	}

/*	CClientNodeInfo* pOverTimeInfo = FindNodeByKeyInNormal(strComputer,strMac);
	if (pOverTimeInfo)
	{
		GetSystemTime(&pOverTimeInfo->m_PrevTime);
		pOverTimeInfo->m_dwPrevTickCount = GetTickCount();
		DeleteNode(pOverTimeInfo);

		m_arrNodeInfo.push_front(pOverTimeInfo);
		return TRUE;
	}*/
	int nOverTime = 0;
	int n = GetDogInfo();
	if (GetNodeTypeInfo(nOverTime) >= GetDogInfo()) //节点已满
	{
		return FALSE;
	}
 
	pNormalInfo = new CClientNodeInfo;
	pNormalInfo->GetClientPriv2(m_hBaseDll,strCurUser);
	pNormalInfo->m_strComputer = _tcsdup(strComputer);
	pNormalInfo->m_strMac = _tcsdup(strMac);
	pNormalInfo->m_strIP = _tcsdup(szHost);
	m_pCurrentNode = pNormalInfo;
	m_arrNodeInfo.push_front(pNormalInfo);

	return TRUE;
}

void CClientInfo::DeleteOverTimeNode(CClientNodeInfo* pInfo)
{
	CLIENT_DEQUE::iterator i;
	for (i=m_arrOverTimeNodeInfo.begin();i!=m_arrOverTimeNodeInfo.end();++i)
	{
		if ((*i) == pInfo)
		{
			m_arrOverTimeNodeInfo.erase(i);
			return;
		}
	}
}

BOOL CClientInfo::CheckAllOverTimeNode()
{
	int size = m_arrNodeInfo.size();
	for (int i=0;i<size;i++)
	{
		CClientNodeInfo* pOverTimeInfo = FindNodeByKeyInOverTime(m_arrNodeInfo[i]->m_strComputer,m_arrNodeInfo[i]->m_strMac);

		if (m_arrNodeInfo[i]->IsOverTime(GetTickCount())) //该节点超时
		{
			if (!pOverTimeInfo)
			{
				m_arrOverTimeNodeInfo.push_back(m_arrNodeInfo[i]);
			}
		}else //如果没有超时,则将该节点从超时队列中去除
		{
			DeleteOverTimeNode(m_arrNodeInfo[i]);
		}
	}
	return TRUE;
}

void CClientInfo::DeleteNormalNode(CClientNodeInfo* pInfo)
{
	CLIENT_DEQUE::iterator i;
	for (i=m_arrNodeInfo.begin();i!=m_arrNodeInfo.end();++i)
	{
		if ((*i) == pInfo)
		{
			m_arrNodeInfo.erase(i);
			return;
		}
	}	
}

void CClientInfo::DeleteNode(CClientNodeInfo* pInfo)
{
	DeleteOverTimeNode(pInfo);
	DeleteNormalNode(pInfo);
}

BOOL CClientInfo::CheckPass(LPCTSTR strUser,LPCTSTR strPass,int* nUserType,DWORD* pdwPriv)
{
	if (m_hBaseDll)
	{	
		CHECKUSERPASS CheckPassWord = (CHECKUSERPASS)GetProcAddress(m_hBaseDll,"CheckPassWord");
		if (CheckPassWord)
		{
			return CheckPassWord(strUser,strPass,nUserType,pdwPriv);
		}
	}
	return FALSE;
}

CClientNodeInfo* CClientInfo::FindNodeByIP(LPCTSTR strIP)
{
	CLIENT_DEQUE::iterator i;

	for (i=m_arrNodeInfo.begin();i!=m_arrNodeInfo.end();++i)
	{
		if (_tcsicmp((*i)->m_strIP,strIP) == 0)
		{
			return (*i);
		}
	}

	return NULL;
}

BOOL CClientInfo::CheckPass2(LPCTSTR strUser,LPCTSTR strPass,int* nUserType,DWORD* pdwPriv,DWORD* pdwCryptLevel,int* pnPolicyType,INT_DEQUE& PolicyDeque)
{
	if (m_hBaseDll)
	{	
		CHECKUSERPASS2 CheckPassWord2 = (CHECKUSERPASS2)GetProcAddress(m_hBaseDll,"CheckPassWord2");
		if (CheckPassWord2)
		{
			return CheckPassWord2(strUser,strPass,nUserType,pdwPriv,pdwCryptLevel,pnPolicyType,PolicyDeque);
		}
	}
	return FALSE;
}


void CClientInfo::ParseVersion(WORD* pPro, FLOAT* pVer)
{
}

int CClientInfo::GetUserCount()
{
	if (m_hBaseDll)
	{
		GETUSERCOUNT pfnGetUserCount = (GETUSERCOUNT)GetProcAddress(m_hBaseDll,"GetUserCount");
		if (pfnGetUserCount)
		{
			return pfnGetUserCount();
		}
	}
	return 0;
}

/*BOOL CClientInfo::GetUserInfo(int index,char* pszUserName,char* pszUserInfo,char* pszUserDescribe,char* pszPassword,int& nUserType,int& nOverTime,long& nPopedom)
{
	if (m_hBaseDll)
	{
		GETUSERINFO pfnGetUserInfo = (GETUSERINFO)GetProcAddress(m_hBaseDll,"GetUserInfo");
		if (pfnGetUserInfo)
		{
			 return pfnGetUserInfo(index,pszUserName,pszUserInfo,pszUserDescribe,pszPassword,nUserType,nOverTime,nPopedom);	
		}
	}
	return FALSE;
}

BOOL CClientInfo::GetUserInfo2(int index,char* pszUserName,char* pszUserInfo,char* pszUserDescribe,char* pszPassword,int& nUserType,int& nOverTime,long& nPopedom,DWORD* pdwCryptLevel,int* pnPolicyType,INT_DEQUE& PolicyDeque,char* pszGroupName)
{
	if (m_hBaseDll)
	{
		GETUSERINFO2 pfnGetUserInfo2 = (GETUSERINFO2)GetProcAddress(m_hBaseDll,"GetUserInfo2");
		if (pfnGetUserInfo2)
		{
			 return pfnGetUserInfo2(index,pszUserName,pszUserInfo,pszUserDescribe,pszPassword,nUserType,nOverTime,nPopedom,pdwCryptLevel,pnPolicyType,PolicyDeque,pszGroupName);	
		}
	}
	return FALSE;
}*/


