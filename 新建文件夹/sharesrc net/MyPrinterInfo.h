#pragma once
#include <tchar.h>

//#define __PrinterInfo_Ini_Path				L".\\PrinterInfo.ini"
#define __PrinterInfo_Ini_AppName			L"PrinterInfo"

typedef struct _MyPrinter_Info
{

	TCHAR szPrinterName[128];

	TCHAR szDriverName[128];

}MyPrinter_Info;

class CMyPrinterInfo
{
public:
	CMyPrinterInfo(void);

	~CMyPrinterInfo(void);

	/*
	nSize :获得开辟数组大小

	return:
	返回打印机数组信息指针
	*/

	MyPrinter_Info* GetInfo(int& nSize);


	/*

	PrinterDriverName:打印机驱动名称
	bWriteToIni:是否要将没有上传的打印机驱动名称直接追加到INI文件中
	return:
	<0 :异常
	0  :已经上传
	1, :未上传
	*/
	int isNeedSendToDB(TCHAR* PrinterDriverName, bool bWriteToIni = true);

private:

	/*
	nArraySize 传入0,返回时nArraySize引用被修改为所需要开辟MyPrinter_Info数组个数
	pPrinterInfoArray 返回打印机信息数组，每个数组元素为每台打印机信息
	*/

	int GetMyPrinterInfo( int& nArraySize, MyPrinter_Info* pPrinterInfoArray);

	MyPrinter_Info* m_pInfo;

	
};
