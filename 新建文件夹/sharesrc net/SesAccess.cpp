// SesAccess.cpp: implementation of the CSesAccess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DGCommonFunc.h"
#include "SesAccess.h"
#include "DGStruct.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CHECK_STATE  if (S4_SUCCESS != m_dwResult) goto errorline
					
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSesAccess::CSesAccess(const BYTE* btUserPin,int UserPinLen,const BYTE* btDevPin,int DevUserPin)
{
	memcpy(lpUserPin,btUserPin,UserPinLen);
	memcpy(lpDevPin,btDevPin,DevUserPin);
	m_dwResult = 0;
}


CSesAccess::CSesAccess()
{

}

CSesAccess::~CSesAccess()
{

}

LPBYTE CSesAccess::ReadSesFile(const char* DirID,const char* FileID,WORD dwFileID,DWORD* dwSize)
{
	if (!(m_dwResult = OpenS4ByIndex(FIRST_S4_INDEX,&stS4Ctx)))
	{
//		AfxMessageBox("没有找到锁");
		return NULL;
	}
	
	m_dwResult = S4ChangeDir(&stS4Ctx, "\\");
	m_dwResult = S4VerifyPin(&stS4Ctx, lpDevPin, sizeof(lpDevPin), S4_DEV_PIN);
	if (DirID != NULL)
	{
		m_dwResult = S4ChangeDir(&stS4Ctx, DirID);
		m_dwResult = S4VerifyPin(&stS4Ctx, lpDevPin, sizeof(lpDevPin), S4_DEV_PIN);
	}

	EFINFO stEFInfo;
	DWORD dwRet = 0;
	m_dwResult = S4Control(&stS4Ctx,S4_EF_INFO,(PVOID)FileID,4,&stEFInfo, sizeof(EFINFO),&dwRet);

	if (m_dwResult != S4_SUCCESS)
	{
		AfxMessageBox(TEXT("数据文件不存在"));
	}
	BYTE btSize[3] = {0};
	LPBYTE buff = new BYTE[stEFInfo.EfSize];
	DWORD ret = 0;
	
	m_dwResult = S4ChangeDir(&stS4Ctx, "\\");
	dwRet=S4VerifyPin(&stS4Ctx,lpUserPin,8,S4_USER_PIN);

	int count = stEFInfo.EfSize / 128 + 1;
	for (int i=0;i<count;i++)
	{
		WORD offset = 128*i;
		btSize[0] = offset >> 8;
		btSize[1] = offset & 0xff;
		if (i == count - 1)
		{
			btSize[2] = stEFInfo.EfSize % 128;
			m_dwResult = S4ExecuteEx(&stS4Ctx,"a001",S4_VM_EXE,&btSize,3,buff+i*128,128,&ret);
		}else
		{
			btSize[2] = 128;
			m_dwResult = S4ExecuteEx(&stS4Ctx,"a001",S4_VM_EXE,&btSize,3,buff+i*128,128,&ret);
		}
	}

	S4Close(&stS4Ctx);
	return buff;
}

BOOL CSesAccess::WriteSesDataFile(const char* FileID,const char* lpszFileName,DWORD dwFileType,DWORD* dwBytesWritten)
{
/*	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD dwFileSize = GetFileSize(hFile,NULL);
	if (dwFileSize == 0)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	LPBYTE buffer = new BYTE[dwFileSize];
	DWORD read = 0;
	ReadFile(hFile,buffer,dwFileSize,&read,NULL);
	CloseHandle(hFile);*/
	if (!(m_dwResult = OpenS4ByIndex(FIRST_S4_INDEX,&stS4Ctx)))
	{
		return FALSE;
	}

	m_dwResult = S4ChangeDir(&stS4Ctx,"\\");
	m_dwResult = S4VerifyPin(&stS4Ctx, lpDevPin, sizeof(lpDevPin), S4_DEV_PIN);

	DWORD FileSize = 0;
	m_dwResult = PS4WriteFile(&stS4Ctx, FileID, lpszFileName, &FileSize, S4_CREATE_NEW, S4_HEX_FILE, dwBytesWritten);
//	m_dwResult = S4WriteFile(&stS4Ctx,FileID,0,buffer,dwFileSize,dwFileSize,&read,S4_CREATE_NEW,dwFileType);
	S4Close(&stS4Ctx);

	return TRUE;
}

BOOL CSesAccess::WriteSesDataFile(const char* FileID,LPBYTE btBuffer,DWORD dwBufSize,DWORD dwFileType,DWORD* dwBytesWritten)
{
	if (!(m_dwResult = OpenS4ByIndex(FIRST_S4_INDEX,&stS4Ctx)))
	{
		return FALSE;
	}

	m_dwResult = S4ChangeDir(&stS4Ctx,"\\");
	m_dwResult = S4VerifyPin(&stS4Ctx, lpDevPin, sizeof(lpDevPin), S4_DEV_PIN);

	DWORD write = 0;
	m_dwResult = S4WriteFile(&stS4Ctx,FileID,0,btBuffer,dwBufSize,dwBufSize,&write,S4_CREATE_NEW,dwFileType);
	S4Close(&stS4Ctx);

	return TRUE;
}
//清除锁内所有的文件数据以及目录,并建立根目录

BOOL CSesAccess::RetsetDogContent()
{
	WORD dwSpaceLeft = 0;
	DWORD dwRet = 0;

	if (!(m_dwResult = OpenS4ByIndex(FIRST_S4_INDEX,&stS4Ctx)))
	{
		return FALSE;
	}

	m_dwResult = S4ChangeDir(&stS4Ctx,"\\");
	
	if (S4_SUCCESS == m_dwResult)
	{
		m_dwResult = S4VerifyPin(&stS4Ctx, lpDevPin, sizeof(lpDevPin), S4_DEV_PIN);
		CHECK_STATE;
		m_dwResult = S4EraseDir(&stS4Ctx,NULL);
		CHECK_STATE;
	}

	m_dwResult = S4Control(&stS4Ctx,S4_GET_DEVICE_USABLE_SPACE,NULL,0,&dwSpaceLeft,sizeof(WORD),&dwRet);
	
	S4CREATEDIRINFO DirInfo;
	DirInfo.dwS4CreateDirInfoSize = sizeof(S4CREATEDIRINFO);
	memcpy(DirInfo.szAtr, "DGuarder", 8);

	m_dwResult = S4CreateDirEx(&stS4Ctx,"\\",dwSpaceLeft,S4_CREATE_ROOT_DIR,&DirInfo);
	
	S4Close(&stS4Ctx);
	return TRUE;

errorline:
	S4Close(&stS4Ctx);
	return FALSE;
}

BOOL CSesAccess::GetSenseCurTime(tm& curtime)
{
	if (!(m_dwResult = OpenS4ByIndex(FIRST_S4_INDEX,&stS4Ctx)))
	{
		return FALSE;
	}
	m_dwResult = S4VerifyPin(&stS4Ctx, lpDevPin, sizeof(lpDevPin), S4_DEV_PIN);
	DWORD dwRet;
	m_dwResult = S4Control(&stS4Ctx,S4_GET_CURRENT_TIME,NULL,0,&curtime,sizeof(tm),&dwRet);
	S4Close(&stS4Ctx);
	if (m_dwResult != S4_SUCCESS)
	{
		return FALSE;	
	}
	return TRUE;
}

BOOL CSesAccess::IsDogExisting()
{
	if (!(m_dwResult = OpenS4ByIndex(FIRST_S4_INDEX,&stS4Ctx)))
	{
		return FALSE;
	}
	S4Close(&stS4Ctx);

	return TRUE;
}

BOOL CSesAccess::IsKeyFileExisting()
{
	if (!(m_dwResult = OpenS4ByIndex(FIRST_S4_INDEX,&stS4Ctx)))
	{
		return NULL;
	}
	EFINFO stEFInfo;
	DWORD dwRet = 0;
	m_dwResult = S4Control(&stS4Ctx,S4_EF_INFO,(PVOID)"d001",4,&stEFInfo, sizeof(EFINFO),&dwRet);

	if (m_dwResult != S4_SUCCESS)
	{
		S4Close(&stS4Ctx);
		return FALSE;
	}
	
	S4Close(&stS4Ctx);
	return TRUE;
}

BOOL CSesAccess::IsCanContinue(Offline_Info2& info)
{
	BOOL bCreateMap = TRUE;
	LPBYTE bt = NULL;
	DWORD dwSize = 0;
	if (bt = ReadSesFile(NULL,"d001",0xd001,&dwSize))
	{
		if(NULL == bt)
		{
			return FALSE;
		}

		memcpy(&info,bt,sizeof(Offline_Info2));
		delete[] bt;


		if (info.bUseTimeControl)  //使用起始时间进行控制
		{
			tm CurTime;

			if (GetSenseCurTime(CurTime))
			{
				CTime t1 = CTime(CurTime.tm_year+1900,
								 CurTime.tm_mon+1,
								 CurTime.tm_mday,
								 CurTime.tm_hour,
								 CurTime.tm_min,
								 CurTime.tm_sec);

				CTime t2 = CTime(info.tmBase.tm_year+1900,
								 info.tmBase.tm_mon+1,
								 info.tmBase.tm_mday,
								 info.tmBase.tm_hour,
								 info.tmBase.tm_min,
								 info.tmBase.tm_sec);
				CTimeSpan t = t1-t2;	
				int seconds = t.GetTotalSeconds();
				if (seconds <= 0)
					bCreateMap = FALSE;
				else
				{
					if (seconds < info.dwStartTimeSecond || seconds > info.dwEndTimeSecond)
					{
						bCreateMap = FALSE;
					}
				}
			}else
			{
				bCreateMap = FALSE;
			}
		}else
		{
			if (info.dwOverTime < 0)
			{
				bCreateMap = FALSE;
			}
/*			CTimeSpan t = t1-t2;
			
			int seconds = t.GetTotalSeconds();
			if (seconds <= 0)
				bCreateMap = FALSE;
			else
			{
				if (seconds > info.dwOverTime*3600)
					bCreateMap = FALSE;
			}*/
		}
	}else
	{
		bCreateMap = FALSE;
	}
	return bCreateMap;
}

CString CSesAccess::GetDogID()
{
	if (!(m_dwResult = OpenS4ByIndex(FIRST_S4_INDEX,&stS4Ctx)))
	{
		return _T("");
	}
	EFINFO stEFInfo;
	DWORD dwDogID = 0;
	BYTE buffer[8] = "";
	m_dwResult = S4Control(&stS4Ctx,S4_GET_SERIAL_NUMBER,NULL,0,(PVOID)buffer,8,&dwDogID);

	if (m_dwResult != S4_SUCCESS)
	{
		S4Close(&stS4Ctx);
		return _T("");
	}
	
	S4Close(&stS4Ctx);

	CString strDogID;

	strDogID.Format(TEXT("%x%x%x%x%x%x%x%x"),buffer[0],
								buffer[1],
								buffer[2],
								buffer[3],
								buffer[4],
								buffer[5],
								buffer[6],
								buffer[7]);
	return strDogID;
}