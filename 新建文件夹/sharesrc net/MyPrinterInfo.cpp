#include "StdAfx.h"
#include "MyPrinterInfo.h"

#include <Windows.h>
#include <WinSpool.h>

#define safe_delete_array(p)  if (NULL != p) {	delete [] p; 	p = NULL;}


//#define __PrinterInfo_Ini_Path				L".\\PrinterInfo.ini"
#define __PrinterInfo_Ini_AppName			L"PrinterInfo"


CMyPrinterInfo::CMyPrinterInfo(void)
{
	m_pInfo = NULL;
}

CMyPrinterInfo::~CMyPrinterInfo(void)
{

	safe_delete_array(m_pInfo);

}

int CMyPrinterInfo::GetMyPrinterInfo( int& nArraySize, MyPrinter_Info* pPrinterInfoArray)
{

	int nRet = 0;
	//EnumPrinters
	DWORD Flags = PRINTER_ENUM_LOCAL;   //local   printers 
	DWORD cbBuf; 
	DWORD pcReturned = 0; 

	DWORD Level = 2; 
	TCHAR Name[500];
	PPRINTER_INFO_2 pPrinterEnum = NULL; 

	memset(Name, 0, sizeof(TCHAR)*500); 

	//检测需要开辟内存大小
	EnumPrinters(Flags, Name, Level, NULL, 0, &cbBuf, &pcReturned);

	pPrinterEnum = (LPPRINTER_INFO_2)LocalAlloc(LPTR, cbBuf+ 4)   ; 

	if   (!pPrinterEnum) 
	{ 
		return   -1; 
	} 

	EnumPrinters( Flags, Name,Level,(LPBYTE)pPrinterEnum,cbBuf,	&cbBuf,	&pcReturned);  

	if ( nArraySize == 0)
	{

		//检测重复
		for ( int i = 0; i < pcReturned; i++)
		{

			if ( 1 == isNeedSendToDB(pPrinterEnum[i].pDriverName,false))
			{

				nRet++;

			}

		}

		nArraySize = nRet;

		LocalFree(pPrinterEnum);

		return 1;
	}

	for ( int i = 0; i < pcReturned; i++)
	{

		if ( 1 == isNeedSendToDB(pPrinterEnum[i].pDriverName,true))
		{

			memcpy(pPrinterInfoArray[nRet].szDriverName, pPrinterEnum[nRet].pDriverName, wcslen(pPrinterEnum[i].pDriverName)*2);

			memcpy(pPrinterInfoArray[nRet].szPrinterName, pPrinterEnum[nRet].pPrinterName, wcslen(pPrinterEnum[i].pPrinterName)*2);

			nRet++;

		}

	}
	
	LocalFree(pPrinterEnum);

	return nRet;
}

MyPrinter_Info* CMyPrinterInfo::GetInfo(int& nSize)
{

	safe_delete_array(m_pInfo);


	//获取需要开辟数组大小
	GetMyPrinterInfo(nSize,m_pInfo);
	
	if ( 0 == nSize )
	{

		//没有打印机信息
		return NULL;

	}

	m_pInfo = new MyPrinter_Info[nSize];

	memset(m_pInfo,0,sizeof(MyPrinter_Info)*nSize);

	nSize = GetMyPrinterInfo(nSize,m_pInfo);

	return m_pInfo;

}

int CMyPrinterInfo::isNeedSendToDB(TCHAR* PrinterDriverName, bool bWriteToIni)
{

	if ( !PrinterDriverName )
	{

		return -1;

	}

	TCHAR str[128];

	memset(str,0,sizeof(str));

	//遍历INI中打印机驱动名

	CString strPrintInfoIniPath;
	TCHAR tcDGPath[4096+1] = {0};
	GetModuleFileName(NULL,tcDGPath,4096);
	CAString strDGPath = tcDGPath;
	int pos = strDGPath.ReverseFind(TEXT('\\'));
	strDGPath = strDGPath.Left(pos+1);
	strPrintInfoIniPath = strDGPath + _T("PrinterInfo.ini");

	int nPos = 1;

	for ( ; ; nPos++ )
	{

		TCHAR tPos[8] = {0};

		_stprintf_s(tPos,L"%d",nPos);

		memset(str,0,sizeof(str));

		GetPrivateProfileString(__PrinterInfo_Ini_AppName,tPos,NULL,str,128,strPrintInfoIniPath);

		if ( 0 == _tcscmp(str,L""))
		{
			//遍历到末尾,跳出循环
			break;
		}

		if ( 0 == _tcscmp(str,PrinterDriverName) )
		{

			//已经有不需要上传
			return 0;

		}

	}

	//将没有上传的打印机驱动名添加到INI中
	if (bWriteToIni)
	{

		TCHAR tPos[8] = {0};

		_stprintf_s(tPos,L"%d",nPos);

		WritePrivateProfileString(__PrinterInfo_Ini_AppName,tPos,PrinterDriverName,strPrintInfoIniPath);

	}

	return 1;
}
