#include "stdafx.h"
#include "cryptbase.h"
#include "Cryp.h"
#include "DGCommonFunc.h"
/*加密文件的结构
  
	加密后的文件比原文件长1k字节
	
	加密文件的0-32位(共32位):    文件标识
	
	DogID可以解开置换信息密钥

	置换信息密钥用于解开置换信息

	DogID->btKey->EncodeTable->FileContent

	RC4算法
	加密文件的32-160位(共128位): 置换信息密钥,用于加密后面的512字节的置换信息
	
	加密文件的32-288位(共256位):  加密置换表
	加密文件的288-544位(共256位): 置换的轮换表(根据字节所在的文件位置)

	以后的位,暂保留

*/
#define HEAD_LEN 1024
#define KEY_LEN 128
#define ENCODE_LEN 256
#define TABLE_LEN 256

#define CRYPT_LEVEL_POS 0x2a0  //672
#define CRYPT_TIME_POS	0x2a0+2*sizeof(DWORD)
#define CRYPT_GUID_POS  (CRYPT_TIME_POS+sizeof(CryptFileTime))
#define CRYPT_COMPUTER_POS  (CRYPT_GUID_POS+sizeof(GUID))

#define RESERVE_LEN 344

//DocGuarder文件的版本,目前有1.0 和 3.0
#define MAJOR_VERSION	3
#define MINOR_VERSION	0

//1.0文件的文件标识为 "DOCGUARDER_FILE"
//3.0文件的文件标识为 "DG_FILE VER 3.0"

#define XOR_CODE1 0x5c
#define XOR_CODE2 0xc5

#define CHECK_CODE 0x89abcdef

// enum
// {
// 	ERROR_FILE_CRYPTED = 1,	   //用于表示文件是一个合法的加密文件
// 	ERROR_ALREADY_CRYPTE = 2,  //已经是加密文件或文件不能操作
// 	ERROR_CANNOT_OPENFILE = 3, //不能打开该文件
// 	ERROR_NO_MAPPING = 4,	   //文件映象句柄不能创建
// 	ERROR_NO_MAPVIEW = 5,	   //无法创建内存映象
// 	ERROR_KEYLEN_OVERFLOW = 6, //密钥长度过长
// 	ERROR_DECRYPT_NORMAL = 7,  //试图解密一个正常的文件
// 	ERROR_NOPRIV_FILE = 8,
// };

int nSystemErrorCode = 0;

class CFileTime
{
public:
	CFileTime() {}
	~CFileTime() {}

	void GetTimeInfo(HANDLE hFile)
	{
		GetFileTime(hFile,&m_tCreate,&m_tAccess,&m_tWrite);
	}
	void SetTimeInfo(HANDLE hFile)
	{
		SetFileTime(hFile,&m_tCreate,&m_tAccess,&m_tWrite);
		CloseHandle(hFile);
	}
private:
	FILETIME m_tCreate;
	FILETIME m_tWrite;
	FILETIME m_tAccess;
};

struct FileFlag
{
	BYTE btHead[15];			//DOCGUARDER_FILE
	DWORD MajorVersion;			//主版本号
	DWORD MinorVersion;			//小版本号
	DWORD DogID;				//微狗的ID 用于加密btKey
	BYTE btReserve[5];			//此5位保留,默认值位FF
};

struct CryptFileHead
{
	FileFlag fileflag;					//文件标识
	
	BYTE	 btKey[KEY_LEN];			//加密密钥	存在微狗中,由微狗传递
	BYTE	 btEncode[ENCODE_LEN];		//加密置换表
	BYTE	 btEncodeTable[TABLE_LEN];	//轮换表
	BYTE	 btReserve[RESERVE_LEN];	//保留位
};



void CreateEncode(BYTE btEncode[ENCODE_LEN])
{
	for (int i=0;i<ENCODE_LEN;i++)
	{
		btEncode[i] = i;
	}

	srand((unsigned int)time(NULL));
	
	for (i=0;i<10000;i++)
	{
		int index1 = rand() % ENCODE_LEN;
		int index2 = rand() % ENCODE_LEN;
		BYTE bt = btEncode[index1];
		btEncode[index1] = btEncode[index2];
		btEncode[index2] = bt;
	}
}

void CreateEncodeTable(BYTE btTable[TABLE_LEN])
{
	srand((unsigned int)time(NULL));

	for (int i=0;i<TABLE_LEN;i++)
	{
		btTable[i] = rand() % 256;
	}
}

void CreateKey(BYTE btKey[KEY_LEN],LPBYTE bt,int len)
{
	memset(btKey,0,KEY_LEN);
	if (len > KEY_LEN)
		return;
	memcpy(btKey,bt,len);
}

void FileFlag_XOR(FileFlag& fileflag)
{
	BYTE byte[sizeof(FileFlag)];
	int size = sizeof(FileFlag);

	memcpy(byte,&fileflag,sizeof(FileFlag));

	for (int i=0;i<sizeof(FileFlag);i++)
	{
		byte[i] ^= XOR_CODE1;
	}
	memcpy(&fileflag,byte,sizeof(FileFlag));
}

BOOL IsByteEqual(LPBYTE byte1,LPBYTE byte2,int len)
{
	for (int i=0;i<len;i++)
	{
		if (byte1[i] != byte2[i])
			return FALSE;
	}
	return TRUE;
}

void CryptKey(LPBYTE btKey,int nKeyLength)
{
//	return;
	for (int i=0;i<nKeyLength;i++)
	{
		btKey[i] ^= XOR_CODE2;
	}
}

//判断某个文件是否为加密文件
//只有返回ERROR_FILE_CRYPTED才表示指定文件是合乎判断条件的加密文件
//返回0 表示不是一个加密文件
//其他的值表示文件操作过程中出错,不能确认文件是否为一个加密文件

int IsCryptedFile(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,BOOL bFullCheck,DWORD* dwCryptLevel,DWORD* dwMajorVersion,DWORD* dwMinorVersion,DWORD* dwCheck)
{
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}

	if (nKeyLength > 128)
	{
		nKeyLength = 128;
	}

	if (lpszKey == NULL && bFullCheck)
	{
		return ERROR_SUCCESS;
	}
	int bCheck = ERROR_SUCCESS;
	DWORD dwLow;
	DWORD dwHigh;
	dwLow = GetFileSize(hFile,&dwHigh);

	if (dwHigh == 0 && dwLow < HEAD_LEN)
	{
		CloseHandle(hFile);
		return bCheck;
	}
	
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READONLY,dwHigh,dwLow,NULL);

	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_READ,0,0,HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			CloseHandle(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}

		CryptFileHead filehead;

		memcpy(filehead.fileflag.btHead,lpAddr,15);
		memcpy(&filehead.fileflag.MajorVersion,lpAddr+15,sizeof(DWORD));
		
		memcpy(&filehead.fileflag.MinorVersion,lpAddr+15+sizeof(DWORD),sizeof(DWORD));
		memcpy(&filehead.fileflag.DogID,lpAddr+15+2*sizeof(DWORD),sizeof(DWORD));
		memcpy(filehead.fileflag.btReserve,lpAddr+27,5);
		
		FileFlag_XOR(filehead.fileflag);



		if (filehead.fileflag.MajorVersion == 1 && filehead.fileflag.MinorVersion == 0)
		{
			memcpy(filehead.btKey,lpAddr+32,KEY_LEN);

			CryptKey(filehead.btKey,KEY_LEN);
			if (bFullCheck)
			{
				if (IsByteEqual(filehead.fileflag.btHead,(LPBYTE)"DOCGUARDER_FILE",15) && filehead.fileflag.DogID == dogID && IsByteEqual(filehead.btKey,(LPBYTE)lpszKey,nKeyLength))
					bCheck = ERROR_FILE_CRYPTED;
			}else
			{
				if (IsByteEqual(filehead.fileflag.btHead,(LPBYTE)"DOCGUARDER_FILE",15))
					bCheck = ERROR_FILE_CRYPTED;
			}
		}

		if (filehead.fileflag.MajorVersion == 3 && filehead.fileflag.MinorVersion == 0)
		{
			memcpy(filehead.btKey,lpAddr+32,KEY_LEN);

			CryptKey(filehead.btKey,KEY_LEN);
			if (bFullCheck)
			{
				if (IsByteEqual(filehead.fileflag.btHead,(LPBYTE)"DG_FILE VER 3.0",15) && filehead.fileflag.DogID == dogID && IsByteEqual(filehead.btKey,(LPBYTE)lpszKey,nKeyLength))
					bCheck = ERROR_FILE_CRYPTED;
				else
				{
					bCheck = ERROR_DOGKEY;
				}
			}else
			{
				if (IsByteEqual(filehead.fileflag.btHead,(LPBYTE)"DG_FILE VER 3.0",15))
					bCheck = ERROR_FILE_CRYPTED;
			}
		}

		if (filehead.fileflag.MajorVersion == 4 && filehead.fileflag.MinorVersion == 0)
		{
			if (bFullCheck)
			{
				CCryp crypt;
	
				crypt.MD5Update((LPBYTE)lpszKey,strlen((char*)lpszKey));

				BYTE Key_MD5[16] = {0};
				crypt.MD5Final(Key_MD5);
				int POS = 32;
				BYTE Key_MD5_File[16] = {0};
				memcpy(Key_MD5_File,lpAddr+POS,16);
				if (IsByteEqual(filehead.fileflag.btHead,(LPBYTE)"DG_FILE VER 4.0",15) && filehead.fileflag.DogID == dogID && IsByteEqual(Key_MD5_File,Key_MD5,16))
					bCheck = ERROR_FILE_CRYPTED;
			}else
			{
				if (IsByteEqual(filehead.fileflag.btHead,(LPBYTE)"DG_FILE VER 4.0",15))
					bCheck = ERROR_FILE_CRYPTED;
			}
		}


		if (dwCryptLevel)
		{
			memcpy(dwCryptLevel,lpAddr+CRYPT_LEVEL_POS,sizeof(DWORD));
		}

		if (dwMajorVersion)
		{
			*dwMajorVersion = filehead.fileflag.MajorVersion;
		}

		if (dwMinorVersion)
		{
			*dwMinorVersion = filehead.fileflag.MinorVersion;
		}

		if (dwCheck)
		{
			memcpy(dwCheck,lpAddr+CRYPT_LEVEL_POS+sizeof(DWORD),sizeof(DWORD));
		}
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else
	{
		CloseHandle(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}
	CloseHandle(hFile);

	return bCheck;
}

void ChangeEncodeByKey(BYTE btEncode[ENCODE_LEN],LPBYTE btKey,int nKeyLength)
{
	for (int i=0;i<ENCODE_LEN;i++)
	{
		btEncode[i] ^= btKey[i % nKeyLength];
	}
}


int EnCryptFile(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,FALSE) == ERROR_FILE_CRYPTED)
		return ERROR_ALREADY_CRYPTE;
	
	CryptFileHead filehead;
	
	memcpy(filehead.fileflag.btHead,(LPBYTE)"DOCGUARDER_FILE",15);

	filehead.fileflag.MajorVersion = MAJOR_VERSION;
	filehead.fileflag.MinorVersion = MINOR_VERSION;

	filehead.fileflag.DogID = dogID;
	for (int i=0;i<5;i++)
	{
		filehead.fileflag.btReserve[i] = 0xff;
	}

	FileFlag_XOR(filehead.fileflag);  //将文件标识隐含

	CreateKey(filehead.btKey,(LPBYTE)lpszKey,nKeyLength);
	CreateEncode(filehead.btEncode);

	CreateEncodeTable(filehead.btEncodeTable);
	
	memset(filehead.btReserve,0xff,RESERVE_LEN);

	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
		return ERROR_CANNOT_OPENFILE;

	CFileTime ft;

	ft.GetTimeInfo(hFile);

	DWORD dwLow;
	DWORD dwHigh;
	dwLow = GetFileSize(hFile,&dwHigh);
	//超过512M的文件不进行处理
	if (dwHigh > 0 || dwLow>0x20000000)
	{
		CloseHandle(hFile);
		return ERROR_NO_MAPVIEW;
	}	
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,dwHigh,dwLow+HEAD_LEN,NULL);

	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,dwLow+HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			return ERROR_NO_MAPVIEW;
		}
		//将文件内容往后移1K字节,留出1K字节的空间
	/*	for (i=dwLow-1;i>=0;i--)
		{
			lpAddr[HEAD_LEN+i] = lpAddr[i];
		}*/
		memmove(lpAddr+HEAD_LEN,lpAddr,dwLow);
		//将移空的前1K字节清零
		memset(lpAddr,0,HEAD_LEN);
	
		//将文件的原有部分加密
		for (i=0;i<dwLow;i++)
		{
			lpAddr[HEAD_LEN+i] = filehead.btEncode[lpAddr[HEAD_LEN+i]];
		}

		//以下将写入文件前1K的加密信息
		int POS = 0;
		memcpy(lpAddr+POS,filehead.fileflag.btHead,15);

		POS += sizeof(filehead.fileflag.btHead);

		memcpy(lpAddr+POS,&filehead.fileflag.MajorVersion,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(lpAddr+POS,&filehead.fileflag.MinorVersion,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(lpAddr+POS,&filehead.fileflag.DogID,sizeof(DWORD));

		POS += sizeof(DWORD);
	
		memset(lpAddr+POS,0xff,5);

		POS += 5;

		CryptKey(filehead.btKey,KEY_LEN);

		memcpy(lpAddr+POS,filehead.btKey,KEY_LEN);

		POS += KEY_LEN;

		ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);

		memcpy(lpAddr+POS,filehead.btEncode,ENCODE_LEN);

		POS += ENCODE_LEN;

		memcpy(lpAddr+POS,filehead.btEncodeTable,TABLE_LEN);

		POS += TABLE_LEN;

		memcpy(lpAddr+POS,filehead.btReserve,RESERVE_LEN);

		::FlushViewOfFile(lpAddr,dwLow+HEAD_LEN);
		UnmapViewOfFile(lpAddr);
		
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		return ERROR_NO_MAPPING;		
	}	
	ft.SetTimeInfo(hFile);

	return 0;
}

int EnCryptFile2(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,DWORD dwCryptLevel)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,FALSE) == ERROR_FILE_CRYPTED)
		return ERROR_ALREADY_CRYPTE;
	
	CryptFileHead filehead;
	
	memcpy(filehead.fileflag.btHead,(LPBYTE)"DG_FILE VER 3.0",15);

	filehead.fileflag.MajorVersion = MAJOR_VERSION;
	filehead.fileflag.MinorVersion = MINOR_VERSION;

	filehead.fileflag.DogID = dogID;
	for (int i=0;i<5;i++)
	{
		filehead.fileflag.btReserve[i] = 0xff;
	}

	FileFlag_XOR(filehead.fileflag);  //将文件标识隐含

	CreateKey(filehead.btKey,(LPBYTE)lpszKey,nKeyLength);
	CreateEncode(filehead.btEncode);

	CreateEncodeTable(filehead.btEncodeTable);
	
	memset(filehead.btReserve,0xff,RESERVE_LEN);

	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}

	CFileTime ft;
	ft.GetTimeInfo(hFile);
	
	DWORD dwLow;
	DWORD dwHigh;
	dwLow = GetFileSize(hFile,&dwHigh);

	//超过512M的文件不进行处理
	if (dwHigh > 0 || dwLow>0x20000000)
	{
		CloseHandle(hFile);
		return ERROR_NO_MAPVIEW;
	}
	
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,dwHigh,dwLow+HEAD_LEN,NULL);

	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,dwLow+HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}
		//将文件内容往后移1K字节,留出1K字节的空间
//		for (i=dwLow-1;i>=0;i--)
//		{
//			lpAddr[HEAD_LEN+i] = lpAddr[i];
//		}
		memmove(lpAddr+HEAD_LEN,lpAddr,dwLow);
		//将移空的前1K字节清零
		memset(lpAddr,0,HEAD_LEN);
	
		//将文件的原有部分加密
		for (i=0;i<dwLow;i++)
		{
			lpAddr[HEAD_LEN+i] = filehead.btEncode[lpAddr[HEAD_LEN+i]];
		}

		//以下将写入文件前1K的加密信息
		int POS = 0;
		memcpy(lpAddr+POS,filehead.fileflag.btHead,15);

		POS += sizeof(filehead.fileflag.btHead);

		memcpy(lpAddr+POS,&filehead.fileflag.MajorVersion,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(lpAddr+POS,&filehead.fileflag.MinorVersion,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(lpAddr+POS,&filehead.fileflag.DogID,sizeof(DWORD));

		POS += sizeof(DWORD);
	
		memset(lpAddr+POS,0xff,5);

		POS += 5;

		CryptKey(filehead.btKey,KEY_LEN);

		memcpy(lpAddr+POS,filehead.btKey,KEY_LEN);

		POS += KEY_LEN;

		ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);

		memcpy(lpAddr+POS,filehead.btEncode,ENCODE_LEN);

		POS += ENCODE_LEN;

		memcpy(lpAddr+POS,filehead.btEncodeTable,TABLE_LEN);

		POS += TABLE_LEN;

//密级
		memcpy(lpAddr+POS,&dwCryptLevel,sizeof(DWORD));

		POS += sizeof(DWORD);
//校验码
		DWORD dwCheck = dwCryptLevel^CHECK_CODE;
		memcpy(lpAddr+POS,&dwCheck,sizeof(DWORD));

		POS += sizeof(DWORD);
//
		memcpy(lpAddr+POS,filehead.btReserve,RESERVE_LEN);

		::FlushViewOfFile(lpAddr,dwLow+HEAD_LEN);
		UnmapViewOfFile(lpAddr);
		
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;		
	}	
	ft.SetTimeInfo(hFile);

	return 0;
}

int  DeCryptFile(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	int nRet = 0;
	nRet = IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE);

	if (ERROR_FILE_CRYPTED != nRet) //只有符合全部的条件
		return nRet;
	
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	CFileTime ft;

	ft.GetTimeInfo(hFile);
	DWORD dwLow;
	DWORD dwHigh;
	dwLow = GetFileSize(hFile,&dwHigh);
	//超过512M的文件不进行处理
	if (dwHigh > 0 || dwLow>0x20000000)
	{
		CloseHandle(hFile);
		return ERROR_NO_MAPVIEW;
	}
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,dwHigh,dwLow,NULL);

	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,dwLow);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}

		CryptFileHead filehead;

		int POS = 0;
		memcpy(filehead.fileflag.btHead,lpAddr,15);

		POS += sizeof(filehead.fileflag.btHead);

		memcpy(&filehead.fileflag.MajorVersion,lpAddr+POS,sizeof(DWORD));
		
		POS += sizeof(DWORD);

		memcpy(&filehead.fileflag.MinorVersion,lpAddr+POS,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(&filehead.fileflag.DogID,lpAddr+POS,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(filehead.fileflag.btReserve,lpAddr+27,5);

		POS += 5;

		memcpy(filehead.btKey,lpAddr+POS,KEY_LEN);

		CryptKey(filehead.btKey,KEY_LEN);

		POS += KEY_LEN;

		memcpy(filehead.btEncode,lpAddr+POS,ENCODE_LEN);

		POS += ENCODE_LEN;

		memcpy(filehead.btEncodeTable,lpAddr+POS,TABLE_LEN);
		
		FileFlag_XOR(filehead.fileflag);

	/*	for (int i=0;i<dwLow-HEAD_LEN;i++)
		{
			lpAddr[i] = lpAddr[HEAD_LEN+i];
		}*/
		memmove(lpAddr,lpAddr+HEAD_LEN,dwLow-HEAD_LEN);

		ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);

		BYTE btDecrypt[ENCODE_LEN];
		for (int i=0;i<ENCODE_LEN;i++)
		{
			btDecrypt[filehead.btEncode[i]] = i;
		}

		for (i=0;i<dwLow-HEAD_LEN;i++)
		{
			lpAddr[i] = btDecrypt[lpAddr[i]];
		}
		::FlushViewOfFile(lpAddr,dwLow);
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}

	SetFilePointer(hFile,-HEAD_LEN,0,FILE_END);
	SetEndOfFile(hFile);
	ft.SetTimeInfo(hFile);
	return 0;
}

//大文件支持
int  DeCryptFile2(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}

	if (ERROR_FILE_CRYPTED != IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE)) //只有符合全部的条件
		return ERROR_DECRYPT_NORMAL;
	
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	CFileTime ft;

	ft.GetTimeInfo(hFile);
	DWORD dwLow;
	DWORD dwHigh;
	dwLow = GetFileSize(hFile,&dwHigh);

	LARGE_INTEGER largeSize;
	largeSize.LowPart = dwLow;
	largeSize.HighPart = dwHigh;

	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,dwHigh,dwLow,NULL);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	DWORD dwAlloc = sysInfo.dwAllocationGranularity*512; //32MB

	if (hMap)
	{
		LARGE_INTEGER CurrentData;
		CurrentData.QuadPart = 0;
		
		BOOL bFirst = TRUE;

		BYTE btDeCrypt[ENCODE_LEN] = {0};
		while(TRUE)
		{
			if (CurrentData.QuadPart < largeSize.QuadPart)
			{		
				if ((CurrentData.QuadPart + dwAlloc + HEAD_LEN) < largeSize.QuadPart)
				{
					//每次多申请 1K的内容
					LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,CurrentData.HighPart,CurrentData.LowPart,dwAlloc+HEAD_LEN);

					if (bFirst) //取得btDeCrypt
					{
						CryptFileHead filehead;

						int POS = 0;
						memcpy(filehead.fileflag.btHead,lpAddr,15);

						POS += sizeof(filehead.fileflag.btHead);

						memcpy(&filehead.fileflag.MajorVersion,lpAddr+POS,sizeof(DWORD));
						
						POS += sizeof(DWORD);

						memcpy(&filehead.fileflag.MinorVersion,lpAddr+POS,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(&filehead.fileflag.DogID,lpAddr+POS,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(filehead.fileflag.btReserve,lpAddr+27,5);

						POS += 5;

						memcpy(filehead.btKey,lpAddr+POS,KEY_LEN);

						CryptKey(filehead.btKey,KEY_LEN);

						POS += KEY_LEN;

						memcpy(filehead.btEncode,lpAddr+POS,ENCODE_LEN);

						POS += ENCODE_LEN;

						memcpy(filehead.btEncodeTable,lpAddr+POS,TABLE_LEN);
						
						FileFlag_XOR(filehead.fileflag);

						memmove(lpAddr,lpAddr+HEAD_LEN,dwAlloc);

						ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);

						for (int i=0;i<ENCODE_LEN;i++)
						{
							btDeCrypt[filehead.btEncode[i]] = i;
						}

						for (i=0;i<dwAlloc;i++)
						{
							lpAddr[i] = btDeCrypt[lpAddr[i]];
						}

						
						bFirst = FALSE;
					}else
					{
						memmove(lpAddr,lpAddr+HEAD_LEN,dwAlloc);
						for (int i=0;i<dwAlloc;i++)
						{
							lpAddr[i] = btDeCrypt[lpAddr[i]];
						}
	
					}
					::FlushViewOfFile(lpAddr,dwAlloc);
						
					UnmapViewOfFile(lpAddr);
					CurrentData.QuadPart += dwAlloc;
				}else		//32M+1K的剩余部分
				{
					LARGE_INTEGER dwLeft;
					dwLeft.QuadPart = largeSize.QuadPart - CurrentData.QuadPart;

					LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,CurrentData.HighPart,CurrentData.LowPart,dwLeft.LowPart);

					if (lpAddr == NULL)
					{
						CloseHandle(hMap);
						ft.SetTimeInfo(hFile);
						nSystemErrorCode = GetLastError();
						return ERROR_NO_MAPVIEW;
					}

					CurrentData.QuadPart += dwLeft.QuadPart;

					if (bFirst)
					{
						CryptFileHead filehead;

						int POS = 0;
						memcpy(filehead.fileflag.btHead,lpAddr,15);

						POS += sizeof(filehead.fileflag.btHead);

						memcpy(&filehead.fileflag.MajorVersion,lpAddr+POS,sizeof(DWORD));
						
						POS += sizeof(DWORD);

						memcpy(&filehead.fileflag.MinorVersion,lpAddr+POS,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(&filehead.fileflag.DogID,lpAddr+POS,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(filehead.fileflag.btReserve,lpAddr+27,5);

						POS += 5;

						memcpy(filehead.btKey,lpAddr+POS,KEY_LEN);

						CryptKey(filehead.btKey,KEY_LEN);

						POS += KEY_LEN;

						memcpy(filehead.btEncode,lpAddr+POS,ENCODE_LEN);

						POS += ENCODE_LEN;

						memcpy(filehead.btEncodeTable,lpAddr+POS,TABLE_LEN);
						
						FileFlag_XOR(filehead.fileflag);

						memmove(lpAddr,lpAddr+HEAD_LEN,dwLeft.LowPart-HEAD_LEN);

						ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);

						for (int i=0;i<ENCODE_LEN;i++)
						{
							btDeCrypt[filehead.btEncode[i]] = i;
						}

						for (i=0;i<dwLeft.LowPart-HEAD_LEN;i++)
						{
							lpAddr[i] = btDeCrypt[lpAddr[i]];
						}
						::FlushViewOfFile(lpAddr,dwLeft.LowPart-HEAD_LEN);
						
						UnmapViewOfFile(lpAddr);
						
						bFirst = FALSE;
					}else
					{
						memmove(lpAddr,lpAddr+HEAD_LEN,dwLeft.LowPart-HEAD_LEN);
						for (int i=0;i<dwLeft.LowPart-HEAD_LEN;i++)
						{
							lpAddr[i] = btDeCrypt[lpAddr[i]];
						}
						::FlushViewOfFile(lpAddr,dwLeft.LowPart-HEAD_LEN);
						
						UnmapViewOfFile(lpAddr);
					}
				}
			}else
			{
				break;
			}
					
		}
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}

	largeSize.QuadPart -= HEAD_LEN;
	DWORD dwOffset = SetFilePointer(hFile,largeSize.LowPart,&largeSize.HighPart,FILE_BEGIN);
	if (GetLastError() != ERROR_SUCCESS)
	{
		ft.SetTimeInfo(hFile);
		return 0;
	}
	SetEndOfFile(hFile);
	ft.SetTimeInfo(hFile);
	return 0;
}

// int EnCryptFile3(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,DWORD dwCryptLevel)
// {
// 	if (nKeyLength > KEY_LEN)
// 	{
// 		nKeyLength = KEY_LEN;
// 	}
// 	if (nKeyLength <= 0)
// 		return ERROR_KEYLEN_OVERFLOW;
// 	
// 	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,FALSE) == ERROR_FILE_CRYPTED)
// 		return ERROR_ALREADY_CRYPTE;
// 	
// 	CryptFileHead filehead;
// 	
// 	memcpy(filehead.fileflag.btHead,(LPBYTE)"DG_FILE VER 3.0",15);
// 
// 	filehead.fileflag.MajorVersion = MAJOR_VERSION;
// 	filehead.fileflag.MinorVersion = MINOR_VERSION;
// 
// 	filehead.fileflag.DogID = dogID;
// 	for (DWORD i=0;i<5;i++)
// 	{
// 		filehead.fileflag.btReserve[i] = 0xff;
// 	}
// 
// 	FileFlag_XOR(filehead.fileflag);  //将文件标识隐含
// 
// 	CreateKey(filehead.btKey,(LPBYTE)lpszKey,nKeyLength);
// 	CreateEncode(filehead.btEncode);
// 
// 	CreateEncodeTable(filehead.btEncodeTable);
// 	
// 	BYTE btEncodeFile[ENCODE_LEN] = {0};
// 	memcpy(btEncodeFile,filehead.btEncode,ENCODE_LEN);
// 
// 	memset(filehead.btReserve,0xff,RESERVE_LEN);
// 
// 	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
// 	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
// 	{
// 		nSystemErrorCode = GetLastError();
// 		return ERROR_CANNOT_OPENFILE;
// 	}
// 	CFileTime ft;
// 
// 	ft.GetTimeInfo(hFile);
// 	DWORD dwLow;
// 	DWORD dwHigh;
// 	dwLow = GetFileSize(hFile,&dwHigh);
// 	
// 	LARGE_INTEGER largeSize;
// 	largeSize.LowPart = dwLow;
// 	largeSize.HighPart = dwHigh;
// 
// 	largeSize.QuadPart += HEAD_LEN;
// 
// 	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,largeSize.HighPart,largeSize.LowPart,NULL);
// 
// 	SYSTEM_INFO sysInfo;
// 	GetSystemInfo(&sysInfo);
// 
// 	DWORD dwAlloc = sysInfo.dwAllocationGranularity*512*16; //512MB
// 
// 	if (hMap)
// 	{
// 		LARGE_INTEGER CurrentData;
// 		CurrentData.QuadPart = 0;
// 
// 		BOOL bFirst = TRUE;
// 
// 		BYTE btTmpLeftBuffer[HEAD_LEN] = {0};  //上一次操作的尾部1K数据
// 		BYTE btTmp[HEAD_LEN] = {0};
// 		while(TRUE)
// 		{
// 			//文件未结束
// 			if (CurrentData.QuadPart <= largeSize.QuadPart)
// 			{
// 				if (CurrentData.QuadPart + dwAlloc < largeSize.QuadPart)
// 				{
// 					LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,CurrentData.HighPart,CurrentData.LowPart,dwAlloc);
// 					if (lpAddr == NULL)
// 					{
// 						ft.SetTimeInfo(hFile);
// 						CloseHandle(hMap);
// 						nSystemErrorCode = GetLastError();
// 						return ERROR_NO_MAPVIEW;
// 					}					
// 					if (bFirst)
// 					{
// 						//保留64M的最后1024字节，用于下一次的操作
// 						memcpy(btTmpLeftBuffer,(LPBYTE)lpAddr+dwAlloc-HEAD_LEN,HEAD_LEN);
// 
// 						memmove(lpAddr+HEAD_LEN,lpAddr,dwAlloc-HEAD_LEN);
// 						//将移空的前1K字节清零
// 						memset(lpAddr,0,HEAD_LEN);
// 					
// 						//将文件的原有部分加密
// 						for (i=0;i<dwAlloc-HEAD_LEN;i++)
// 						{
// 							lpAddr[HEAD_LEN+i] = btEncodeFile[lpAddr[HEAD_LEN+i]];
// 						}
// 
// 						//以下将写入文件前1K的加密信息
// 						int POS = 0;
// 						memcpy(lpAddr+POS,filehead.fileflag.btHead,15);
// 
// 						POS += sizeof(filehead.fileflag.btHead);
// 
// 						memcpy(lpAddr+POS,&filehead.fileflag.MajorVersion,sizeof(DWORD));
// 
// 						POS += sizeof(DWORD);
// 
// 						memcpy(lpAddr+POS,&filehead.fileflag.MinorVersion,sizeof(DWORD));
// 
// 						POS += sizeof(DWORD);
// 
// 						memcpy(lpAddr+POS,&filehead.fileflag.DogID,sizeof(DWORD));
// 
// 						POS += sizeof(DWORD);
// 					
// 						memset(lpAddr+POS,0xff,5);
// 
// 						POS += 5;
// 
// 						CryptKey(filehead.btKey,KEY_LEN);
// 
// 						memcpy(lpAddr+POS,filehead.btKey,KEY_LEN);
// 
// 						POS += KEY_LEN;
// 
// 						ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);
// 
// 						memcpy(lpAddr+POS,filehead.btEncode,ENCODE_LEN);
// 
// 						POS += ENCODE_LEN;
// 
// 						memcpy(lpAddr+POS,filehead.btEncodeTable,TABLE_LEN);
// 
// 						POS += TABLE_LEN;
// 
// 				//密级
// 						memcpy(lpAddr+POS,&dwCryptLevel,sizeof(DWORD));
// 
// 						POS += sizeof(DWORD);
// 				//校验码
// 						DWORD dwCheck = dwCryptLevel^CHECK_CODE;
// 						memcpy(lpAddr+POS,&dwCheck,sizeof(DWORD));
// 
// 						POS += sizeof(DWORD);
// 				//
// 						memcpy(lpAddr+POS,filehead.btReserve,RESERVE_LEN);
// 
// 						bFirst = FALSE;
// 					}else
// 					{
// 						memcpy(btTmp,(LPBYTE)lpAddr+dwAlloc-HEAD_LEN,HEAD_LEN);
// 						
// 						memmove(lpAddr+HEAD_LEN,lpAddr,dwAlloc-HEAD_LEN);
// 						
// 						//将上次数据块的尾部复制到数据块的头部
// 						memcpy(lpAddr,btTmpLeftBuffer,HEAD_LEN);
// 					
// 						memcpy(btTmpLeftBuffer,btTmp,HEAD_LEN);
// 						//将文件的原有部分加密
// 						for (i=0;i<dwAlloc;i++)
// 						{
// 							lpAddr[i] = btEncodeFile[lpAddr[i]];
// 						}
// 					}
// 
// 					::FlushViewOfFile(lpAddr,dwAlloc);
// 					UnmapViewOfFile(lpAddr);
// 					
// 				}else
// 				{
// 					LARGE_INTEGER LeftBufferSize;
// 					LeftBufferSize.QuadPart= largeSize.QuadPart - CurrentData.QuadPart;
// 					LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,CurrentData.HighPart,CurrentData.LowPart,LeftBufferSize.LowPart);
// 					if (lpAddr == NULL)
// 					{
// 						CloseHandle(hMap);
// 						ft.SetTimeInfo(hFile);
// 						
// 						nSystemErrorCode = GetLastError();
// 						return ERROR_NO_MAPVIEW;
// 					}
// 					if (bFirst)   //文件不大于64M,则按原有方式加密
// 					{
// 						memmove(lpAddr+HEAD_LEN,lpAddr,LeftBufferSize.LowPart-HEAD_LEN);
// 						//将移空的前1K字节清零
// 						memset(lpAddr,0,HEAD_LEN);
// 					
// 						//将文件的原有部分加密
// 						for (i=0;i<LeftBufferSize.LowPart-HEAD_LEN;i++)
// 						{
// 							lpAddr[HEAD_LEN+i] = btEncodeFile[lpAddr[HEAD_LEN+i]];
// 						}
// 
// 						//以下将写入文件前1K的加密信息
// 						int POS = 0;
// 						memcpy(lpAddr+POS,filehead.fileflag.btHead,15);
// 
// 						POS += sizeof(filehead.fileflag.btHead);
// 
// 						memcpy(lpAddr+POS,&filehead.fileflag.MajorVersion,sizeof(DWORD));
// 
// 						POS += sizeof(DWORD);
// 
// 						memcpy(lpAddr+POS,&filehead.fileflag.MinorVersion,sizeof(DWORD));
// 
// 						POS += sizeof(DWORD);
// 
// 						memcpy(lpAddr+POS,&filehead.fileflag.DogID,sizeof(DWORD));
// 
// 						POS += sizeof(DWORD);
// 					
// 						memset(lpAddr+POS,0xff,5);
// 
// 						POS += 5;
// 
// 						CryptKey(filehead.btKey,KEY_LEN);
// 
// 						memcpy(lpAddr+POS,filehead.btKey,KEY_LEN);
// 
// 						POS += KEY_LEN;
// 
// 						ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);
// 
// 						memcpy(lpAddr+POS,filehead.btEncode,ENCODE_LEN);
// 
// 						POS += ENCODE_LEN;
// 
// 						memcpy(lpAddr+POS,filehead.btEncodeTable,TABLE_LEN);
// 
// 						POS += TABLE_LEN;
// 
// 				//密级
// 						memcpy(lpAddr+POS,&dwCryptLevel,sizeof(DWORD));
// 
// 						POS += sizeof(DWORD);
// 				//校验码
// 						DWORD dwCheck = dwCryptLevel^CHECK_CODE;
// 						memcpy(lpAddr+POS,&dwCheck,sizeof(DWORD));
// 
// 						POS += sizeof(DWORD);
// 				//
// 						memcpy(lpAddr+POS,filehead.btReserve,RESERVE_LEN);
// 
// 						bFirst = FALSE;
// 					}else //文件结尾
// 					{						
// 						memmove(lpAddr+HEAD_LEN,lpAddr,LeftBufferSize.LowPart-HEAD_LEN);
// 						
// 						//将上次数据块的尾部复制到数据块的头部
// 						memcpy(lpAddr,btTmpLeftBuffer,HEAD_LEN);
// 					
// 						//将文件的原有部分加密
// 						for (i=0;i<LeftBufferSize.LowPart;i++)
// 						{
// 							lpAddr[i] = btEncodeFile[lpAddr[i]];
// 						}					
// 					}
// 					
// 					::FlushViewOfFile(lpAddr,LeftBufferSize.LowPart);
// 					UnmapViewOfFile(lpAddr);
// 				}
// 			}else
// 			{
// 				break;
// 			}
// 			CurrentData.QuadPart += dwAlloc;
// 		}
// 		
// 		CloseHandle(hMap);
// 	}else{
// 		ft.SetTimeInfo(hFile);
// 		nSystemErrorCode = GetLastError();
// 		return ERROR_NO_MAPPING;		
// 	}	
// 	ft.SetTimeInfo(hFile);
// 
// 	return 0;
// }

int EnCryptFile3(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,DWORD dwCryptLevel)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,FALSE) == ERROR_FILE_CRYPTED)
		return ERROR_ALREADY_CRYPTE;
	
	CryptFileHead filehead;
	
	memcpy(filehead.fileflag.btHead,(LPBYTE)"DG_FILE VER 3.0",15);

	filehead.fileflag.MajorVersion = MAJOR_VERSION;
	filehead.fileflag.MinorVersion = MINOR_VERSION;

	filehead.fileflag.DogID = dogID;
	for (DWORD i=0;i<5;i++)
	{
		filehead.fileflag.btReserve[i] = 0xff;
	}

	FileFlag_XOR(filehead.fileflag);  //将文件标识隐含

	CreateKey(filehead.btKey,(LPBYTE)lpszKey,nKeyLength);
	CreateEncode(filehead.btEncode);

	CreateEncodeTable(filehead.btEncodeTable);
	
	BYTE btEncodeFile[ENCODE_LEN] = {0};
	memcpy(btEncodeFile,filehead.btEncode,ENCODE_LEN);

	memset(filehead.btReserve,0xff,RESERVE_LEN);

	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}

	TCHAR strFileTmp[1024] = {0};
	_stprintf(strFileTmp,_T("%s.tmp"),lpszFileName);
	HANDLE hFileTmp = CreateFile(strFileTmp,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_HIDDEN,NULL);
	
	BYTE lpAddr[HEAD_LEN] = {0};

	int POS = 0;
	memcpy(lpAddr+POS,filehead.fileflag.btHead,15);

	POS += sizeof(filehead.fileflag.btHead);

	memcpy(lpAddr+POS,&filehead.fileflag.MajorVersion,sizeof(DWORD));

	POS += sizeof(DWORD);

	memcpy(lpAddr+POS,&filehead.fileflag.MinorVersion,sizeof(DWORD));

	POS += sizeof(DWORD);

	memcpy(lpAddr+POS,&filehead.fileflag.DogID,sizeof(DWORD));

	POS += sizeof(DWORD);

	memset(lpAddr+POS,0xff,5);

	POS += 5;

	CryptKey(filehead.btKey,KEY_LEN);

	memcpy(lpAddr+POS,filehead.btKey,KEY_LEN);

	POS += KEY_LEN;

	ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);

	memcpy(lpAddr+POS,filehead.btEncode,ENCODE_LEN);

	POS += ENCODE_LEN;

	memcpy(lpAddr+POS,filehead.btEncodeTable,TABLE_LEN);

	POS += TABLE_LEN;

//密级
	memcpy(lpAddr+POS,&dwCryptLevel,sizeof(DWORD));

	POS += sizeof(DWORD);
//校验码
	DWORD dwCheck = dwCryptLevel^CHECK_CODE;
	memcpy(lpAddr+POS,&dwCheck,sizeof(DWORD));

	POS += sizeof(DWORD);
//
	memcpy(lpAddr+POS,filehead.btReserve,RESERVE_LEN);
	
	DWORD wrote=0;
	WriteFile(hFileTmp,lpAddr,HEAD_LEN,&wrote,NULL);

	const int sp = 512*1024;
	LPBYTE btData = new BYTE[sp];
	while(TRUE)
	{
		DWORD read = 0;
		ReadFile(hFile,btData,sp,&read,NULL);

		if (read == 0)
		{
			break;
		}
		
		for (i=0;i<read;i++)
		{
			btData[i] = btEncodeFile[btData[i]];
		}
		WriteFile(hFileTmp,btData,read,&wrote,NULL);
	}

	CloseHandle(hFile);
	CloseHandle(hFileTmp);
	DeleteFile(lpszFileName);

	MoveFile(strFileTmp,lpszFileName);
	return 0;
}

int EnCryptFile4(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,DWORD dwCryptLevel)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,FALSE) == ERROR_FILE_CRYPTED)
		return ERROR_ALREADY_CRYPTE;
	

	CryptFileHead filehead;
//前32个字节的加密	
//主要包括文件版本信息以及dogID
	memcpy(filehead.fileflag.btHead,(LPBYTE)"DG_FILE VER 3.0",15);

	filehead.fileflag.MajorVersion = 4;
	filehead.fileflag.MinorVersion = 0;

	filehead.fileflag.DogID = dogID;
	for (DWORD i=0;i<5;i++)
	{
		filehead.fileflag.btReserve[i] = 0xff;
	}

	FileFlag_XOR(filehead.fileflag);  //将文件标识隐含
/////////////////////////////////////////////////////////////
//创建文件的Encode以及TableEncode
	CreateKey(filehead.btKey,(LPBYTE)lpszKey,nKeyLength);
	CreateEncode(filehead.btEncode);

	CreateEncodeTable(filehead.btEncodeTable);
	
	BYTE btEncodeFile[ENCODE_LEN] = {0};
	memcpy(btEncodeFile,filehead.btEncode,ENCODE_LEN);

	memset(filehead.btReserve,0xff,RESERVE_LEN);

	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	CFileTime ft;

	ft.GetTimeInfo(hFile);
	DWORD dwLow;
	DWORD dwHigh;
	dwLow = GetFileSize(hFile,&dwHigh);
	
	LARGE_INTEGER largeSize;
	largeSize.LowPart = dwLow;
	largeSize.HighPart = dwHigh;

	largeSize.QuadPart += HEAD_LEN;

	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,largeSize.HighPart,largeSize.LowPart,NULL);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	DWORD dwAlloc = sysInfo.dwAllocationGranularity*512; //32MB

	if (hMap)
	{
		LARGE_INTEGER CurrentData;
		CurrentData.QuadPart = 0;

		BOOL bFirst = TRUE;

		BYTE btTmpLeftBuffer[HEAD_LEN] = {0};  //上一次操作的尾部1K数据
		BYTE btTmp[HEAD_LEN] = {0};
		while(TRUE)
		{
			//文件未结束
			if (CurrentData.QuadPart <= largeSize.QuadPart)
			{
				if (CurrentData.QuadPart + dwAlloc < largeSize.QuadPart)
				{
					LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,CurrentData.HighPart,CurrentData.LowPart,dwAlloc);
					if (lpAddr == NULL)
					{
						ft.SetTimeInfo(hFile);
						CloseHandle(hMap);
						nSystemErrorCode = GetLastError();
						return ERROR_NO_MAPVIEW;
					}					
					if (bFirst)
					{
						//保留64M的最后1024字节，用于下一次的操作
						memcpy(btTmpLeftBuffer,(LPBYTE)lpAddr+dwAlloc-HEAD_LEN,HEAD_LEN);

						memmove(lpAddr+HEAD_LEN,lpAddr,dwAlloc-HEAD_LEN);
						//将移空的前1K字节清零
						memset(lpAddr,0,HEAD_LEN);
					
						//将文件的原有部分加密
						for (i=0;i<dwAlloc-HEAD_LEN;i++)
						{
							lpAddr[HEAD_LEN+i] = btEncodeFile[lpAddr[HEAD_LEN+i]];
						}

						//以下将写入文件前1K的加密信息
						int POS = 0;
						memcpy(lpAddr+POS,filehead.fileflag.btHead,15);

						POS += sizeof(filehead.fileflag.btHead);

						memcpy(lpAddr+POS,&filehead.fileflag.MajorVersion,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(lpAddr+POS,&filehead.fileflag.MinorVersion,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(lpAddr+POS,&filehead.fileflag.DogID,sizeof(DWORD));

						POS += sizeof(DWORD);
					
						memset(lpAddr+POS,0xff,5);

						POS += 5;

						CryptKey(filehead.btKey,KEY_LEN);

						memcpy(lpAddr+POS,filehead.btKey,KEY_LEN);

						POS += KEY_LEN;

						ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);

						memcpy(lpAddr+POS,filehead.btEncode,ENCODE_LEN);

						POS += ENCODE_LEN;

						memcpy(lpAddr+POS,filehead.btEncodeTable,TABLE_LEN);

						POS += TABLE_LEN;

				//密级
						memcpy(lpAddr+POS,&dwCryptLevel,sizeof(DWORD));

						POS += sizeof(DWORD);
				//校验码
						DWORD dwCheck = dwCryptLevel^CHECK_CODE;
						memcpy(lpAddr+POS,&dwCheck,sizeof(DWORD));

						POS += sizeof(DWORD);
				//
						memcpy(lpAddr+POS,filehead.btReserve,RESERVE_LEN);

						bFirst = FALSE;
					}else
					{
						memcpy(btTmp,(LPBYTE)lpAddr+dwAlloc-HEAD_LEN,HEAD_LEN);
						
						memmove(lpAddr+HEAD_LEN,lpAddr,dwAlloc-HEAD_LEN);
						
						//将上次数据块的尾部复制到数据块的头部
						memcpy(lpAddr,btTmpLeftBuffer,HEAD_LEN);
					
						memcpy(btTmpLeftBuffer,btTmp,HEAD_LEN);
						//将文件的原有部分加密
						for (i=0;i<dwAlloc;i++)
						{
							lpAddr[i] = btEncodeFile[lpAddr[i]];
						}
					}

					::FlushViewOfFile(lpAddr,dwAlloc);
					UnmapViewOfFile(lpAddr);
					
				}else
				{
					LARGE_INTEGER LeftBufferSize;
					LeftBufferSize.QuadPart= largeSize.QuadPart - CurrentData.QuadPart;
					LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,CurrentData.HighPart,CurrentData.LowPart,LeftBufferSize.LowPart);
					if (lpAddr == NULL)
					{
						CloseHandle(hMap);
						ft.SetTimeInfo(hFile);
						
						nSystemErrorCode = GetLastError();
						return ERROR_NO_MAPVIEW;
					}
					if (bFirst)   //文件不大于64M,则按原有方式加密
					{
						memmove(lpAddr+HEAD_LEN,lpAddr,LeftBufferSize.LowPart-HEAD_LEN);
						//将移空的前1K字节清零
						memset(lpAddr,0,HEAD_LEN);
					
						//将文件的原有部分加密
						for (i=0;i<LeftBufferSize.LowPart;i++)
						{
							lpAddr[HEAD_LEN+i] = btEncodeFile[lpAddr[HEAD_LEN+i]];
						}

						//以下将写入文件前1K的加密信息
						int POS = 0;
						memcpy(lpAddr+POS,filehead.fileflag.btHead,15);

						POS += sizeof(filehead.fileflag.btHead);

						memcpy(lpAddr+POS,&filehead.fileflag.MajorVersion,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(lpAddr+POS,&filehead.fileflag.MinorVersion,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(lpAddr+POS,&filehead.fileflag.DogID,sizeof(DWORD));

						POS += sizeof(DWORD);
					
						memset(lpAddr+POS,0xff,5);

						POS += 5;

						CryptKey(filehead.btKey,KEY_LEN);

						memcpy(lpAddr+POS,filehead.btKey,KEY_LEN);

						POS += KEY_LEN;

						ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);

						memcpy(lpAddr+POS,filehead.btEncode,ENCODE_LEN);

						POS += ENCODE_LEN;

						memcpy(lpAddr+POS,filehead.btEncodeTable,TABLE_LEN);

						POS += TABLE_LEN;

				//密级
						memcpy(lpAddr+POS,&dwCryptLevel,sizeof(DWORD));

						POS += sizeof(DWORD);
				//校验码
						DWORD dwCheck = dwCryptLevel^CHECK_CODE;
						memcpy(lpAddr+POS,&dwCheck,sizeof(DWORD));

						POS += sizeof(DWORD);
				//
						memcpy(lpAddr+POS,filehead.btReserve,RESERVE_LEN);

						bFirst = FALSE;
					}else //文件结尾
					{						
						memmove(lpAddr+HEAD_LEN,lpAddr,LeftBufferSize.LowPart-HEAD_LEN);
						
						//将上次数据块的尾部复制到数据块的头部
						memcpy(lpAddr,btTmpLeftBuffer,HEAD_LEN);
					
						//将文件的原有部分加密
						for (i=0;i<LeftBufferSize.LowPart;i++)
						{
							lpAddr[i] = btEncodeFile[lpAddr[i]];
						}					
					}
					
					::FlushViewOfFile(lpAddr,LeftBufferSize.LowPart);
					UnmapViewOfFile(lpAddr);
				}
			}else
			{
				break;
			}
			CurrentData.QuadPart += dwAlloc;
		}
		
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;		
	}	
	ft.SetTimeInfo(hFile);

	return 0;
}

int ModifyFileLimitTime(LPCTSTR lpszFileName,CryptFileTime timeInfo,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	
	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE) != ERROR_FILE_CRYPTED)
		return ERROR_DECRYPT_NORMAL;

	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}

	CFileTime ft;

	ft.GetTimeInfo(hFile);

	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,HEAD_LEN,NULL);

	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}
		
		memcpy(lpAddr+CRYPT_TIME_POS,&timeInfo,sizeof(CryptFileTime));
		
		::FlushViewOfFile(lpAddr,HEAD_LEN);
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}

	ft.SetTimeInfo(hFile);

	return 0;
}

int ClearFileLimitTime(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	
	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE) != ERROR_FILE_CRYPTED)
		return ERROR_DECRYPT_NORMAL;

	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}

	CFileTime ft;

	ft.GetTimeInfo(hFile);

	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,HEAD_LEN,NULL);

	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}
		
		memset(lpAddr+CRYPT_TIME_POS,0xFF,sizeof(CryptFileTime));
		
		::FlushViewOfFile(lpAddr,HEAD_LEN);
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}

	ft.SetTimeInfo(hFile);

	return 0;
}
int GetFileLimitTime(LPCTSTR lpszFileName,CryptFileTime& timeInfo,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE) != ERROR_FILE_CRYPTED)
		return ERROR_ALREADY_CRYPTE;

	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}

	CFileTime ft;

	ft.GetTimeInfo(hFile);

	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,HEAD_LEN,NULL);

	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}
		
		memcpy(&timeInfo,lpAddr+CRYPT_TIME_POS,sizeof(CryptFileTime));
		
		::FlushViewOfFile(lpAddr,HEAD_LEN);
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}

	ft.SetTimeInfo(hFile);

	return 0;
}

//修改
int ModifyFileKey(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,DWORD newDogID,LPCSTR newKey,int nNewKeyLength)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;

	if (nNewKeyLength > KEY_LEN || nNewKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;
	DWORD dwCryptLevel = 0;
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE,&dwCryptLevel,&dwMajorVersion,&dwMinorVersion) != ERROR_FILE_CRYPTED)
		return ERROR_DECRYPT_NORMAL;

	if (dwMajorVersion < 3)
		return 0;

	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	CFileTime ft;

	ft.GetTimeInfo(hFile);

	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,HEAD_LEN,NULL);

	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}

		CryptFileHead filehead;

		int POS = 0;
		memcpy(filehead.fileflag.btHead,lpAddr,15);

		POS += sizeof(filehead.fileflag.btHead);

		memcpy(&filehead.fileflag.MajorVersion,lpAddr+POS,sizeof(DWORD));
		
		POS += sizeof(DWORD);

		memcpy(&filehead.fileflag.MinorVersion,lpAddr+POS,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(&filehead.fileflag.DogID,lpAddr+POS,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(filehead.fileflag.btReserve,lpAddr+27,5);

		POS += 5;

		memcpy(filehead.btKey,lpAddr+POS,KEY_LEN);

		CryptKey(filehead.btKey,KEY_LEN);

		POS += KEY_LEN;

		memcpy(filehead.btEncode,lpAddr+POS,ENCODE_LEN);

		POS += ENCODE_LEN;

		memcpy(filehead.btEncodeTable,lpAddr+POS,TABLE_LEN);
		
		FileFlag_XOR(filehead.fileflag);
		
		ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);

		memset(filehead.btKey,0,KEY_LEN);
		memcpy(filehead.btKey,newKey,nNewKeyLength);

		filehead.fileflag.DogID = newDogID;
		
		FileFlag_XOR(filehead.fileflag);  //将文件标识隐含

		POS = 0;
		memcpy(lpAddr+POS,filehead.fileflag.btHead,15);

		POS += sizeof(filehead.fileflag.btHead);

		memcpy(lpAddr+POS,&filehead.fileflag.MajorVersion,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(lpAddr+POS,&filehead.fileflag.MinorVersion,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(lpAddr+POS,&filehead.fileflag.DogID,sizeof(DWORD));

		POS += sizeof(DWORD);

		memset(lpAddr+POS,0xff,5);

		POS += 5;

		CryptKey(filehead.btKey,KEY_LEN);

		memcpy(lpAddr+POS,filehead.btKey,KEY_LEN);

		POS += KEY_LEN;

		ChangeEncodeByKey(filehead.btEncode,(LPBYTE)newKey,nNewKeyLength);

		memcpy(lpAddr+POS,filehead.btEncode,ENCODE_LEN);

		POS += ENCODE_LEN;

		memcpy(lpAddr+POS,filehead.btEncodeTable,TABLE_LEN);

		POS += TABLE_LEN;

	//密级
		memcpy(lpAddr+POS,&dwCryptLevel,sizeof(DWORD));

		POS += sizeof(DWORD);
	//校验码
		DWORD dwCheck = dwCryptLevel^CHECK_CODE;
		memcpy(lpAddr+POS,&dwCheck,sizeof(DWORD));

		POS += sizeof(DWORD);
	//
		memcpy(lpAddr+POS,filehead.btReserve,RESERVE_LEN);

		::FlushViewOfFile(lpAddr,HEAD_LEN);
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}

	ft.SetTimeInfo(hFile);
	return 0;
}

int ModifyFileGUID(LPCTSTR lpszFileName,GUID guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}

	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE) != ERROR_FILE_CRYPTED)
		return ERROR_DECRYPT_NORMAL;
	
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	
	CFileTime ft;
	
	ft.GetTimeInfo(hFile);
	
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,HEAD_LEN,NULL);
	
	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}
		
		memcpy(lpAddr+CRYPT_GUID_POS,&guid,sizeof(GUID));
		
		::FlushViewOfFile(lpAddr,HEAD_LEN);
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}
	
	ft.SetTimeInfo(hFile);
	
	return 0;
}

int GetFileGUID(LPCTSTR lpszFileName,GUID& guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	
	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE) != ERROR_FILE_CRYPTED)
		return ERROR_ALREADY_CRYPTE;
	
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	
	CFileTime ft;
	
	ft.GetTimeInfo(hFile);
	
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,HEAD_LEN,NULL);
	
	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}
		
		memcpy(&guid,lpAddr+CRYPT_GUID_POS,sizeof(GUID));
		
		::FlushViewOfFile(lpAddr,HEAD_LEN);
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}
	
	ft.SetTimeInfo(hFile);
	
	return 0;
}

int ModifyFileOutsideGUID(LPCTSTR lpszFileName,GUID guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	if (nKeyLength > KEY_LEN || nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE) != ERROR_FILE_CRYPTED)
		return ERROR_DECRYPT_NORMAL;
	
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	
	CFileTime ft;
	
	ft.GetTimeInfo(hFile);
	
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,HEAD_LEN,NULL);
	
	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}
		
		memcpy(lpAddr+CRYPT_COMPUTER_POS,&guid,sizeof(GUID));
		
		::FlushViewOfFile(lpAddr,HEAD_LEN);
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}
	
	ft.SetTimeInfo(hFile);
	
	return 0;
}

int GetFileOutsideGUID(LPCTSTR lpszFileName,GUID& guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	if (nKeyLength > KEY_LEN || nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE) != ERROR_FILE_CRYPTED)
		return ERROR_ALREADY_CRYPTE;
	
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	
	CFileTime ft;
	
	ft.GetTimeInfo(hFile);
	
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,HEAD_LEN,NULL);
	
	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,HEAD_LEN);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}

		memcpy(&guid,lpAddr+CRYPT_COMPUTER_POS,sizeof(GUID));
		
		::FlushViewOfFile(lpAddr,HEAD_LEN);
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}
	
	ft.SetTimeInfo(hFile);
	
	return 0;
}

__int64 epass(CString strpass)
{
// 	DWORD plen;
// 	char *ppass;
// 	__int64 mc= 1987010632167;
// 	
// 	ppass = strpass.GetBuffer(0);
// 	
// 	plen = strlen(ppass);
// 	
// 	for(int i=0; i<(int)plen; i++)
// 	{
// 		mc ^= ppass[i]|128;
// 	}
// 	return mc;
	return 0;
}

BOOL ecfile(LPCTSTR fpath, CString strpass)
{
	char *data;
	CFile *file;
	DWORD flen;
	
	__int64 password = epass(strpass);//密码初始化
	
	file = new CFile;
	if ( !file->Open(fpath, CFile::shareDenyNone|CFile::modeReadWrite))
	{
		return FALSE;
	}
	
	flen = file->GetLength();
	data = new char[(int)flen];//为文件分配内存
	
	file->SeekToBegin();
	file->Read(data, flen);
	
	//这里把文件的所有字节都进行加密了
	for(int i=0; i<(int)flen; i++)
	{
		data[i] ^= password;
		data[i] ^= flen;                //因为每次加密后文件的大小都会增加5字节,所以如果两次用同一密码进行加密也没关系
	}
	
	file->SeekToBegin();
	file->Write(data, flen);
	delete[] data;                          //先释放内存
	
	//添加密码验证信息
	char cpass[5] = "HTSO";
	for(int j=0; j<5; j++)
	{
		cpass[j] ^= password;
	}
	file->SeekToEnd();
	file->Write(&cpass, 5);        //在文件尾添加密码严整部分,,这样文件就会增加5字节
	file->Close();
	delete file;
	
	return TRUE;
}

BOOL dcfile(LPCTSTR fpath, CString strpass)
{
	char *data;
	CFile *file;
	DWORD flen;
	char love[5];
	
	file = new CFile;
	if( !file->Open(fpath, CFile::shareDenyNone|CFile::modeReadWrite))
	{
		return FALSE;
	}
	
	flen = file->GetLength();
	data = new char[(int)flen];
	
	//检验密码是不是正确
	file->Seek(-5, CFile::end);
	file->Read(&love, 5);
	
	__int64 password = epass(strpass);
	
	for(int i=0; i<5; i++)
	{
		love[i] ^= password;
	}
	
	if(strcmp(love, "HTSO")!=0)
	{
		return FALSE;
	}
	
	//解密
	file->SeekToBegin();
	file->Read(data, flen);
	
	//按照原来的方法进行解密
	for(int j=0; j<(int)flen; j++)
	{
		data[j] ^= password;
		data[j] ^= (flen-5);
	}
	file->SeekToBegin();
	file->Write(data, flen);
	file->SetLength(flen-5);     //删除加密是添加的密码验证部分
	file->Close();
	
	delete[] data;
	delete file;
	return TRUE;
}


BOOL ReadCryptFile(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,LPBYTE bt,DWORD dwSize)
{
	if (ERROR_FILE_CRYPTED != IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE)) //只有符合全部的条件
		return ERROR_DECRYPT_NORMAL;
	
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	CFileTime ft;

	ft.GetTimeInfo(hFile);
	DWORD dwLow;
	DWORD dwHigh;
	dwLow = GetFileSize(hFile,&dwHigh);
	//超过512M的文件不进行处理
	if (dwHigh > 0 || dwLow>0x20000000)
	{
		CloseHandle(hFile);
		return ERROR_NO_MAPVIEW;
	}
	
	if (dwHigh == 0 && dwLow<dwSize+HEAD_LEN)
	{
		CloseHandle(hFile);
		return ERROR_NO_MAPVIEW;
	}
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,dwHigh,dwLow,NULL);

	if (hMap)
	{
		LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,dwLow);
		if (lpAddr == NULL)
		{
			CloseHandle(hMap);
			ft.SetTimeInfo(hFile);
			nSystemErrorCode = GetLastError();
			return ERROR_NO_MAPVIEW;
		}

		CryptFileHead filehead;

		int POS = 0;
		memcpy(filehead.fileflag.btHead,lpAddr,15);

		POS += sizeof(filehead.fileflag.btHead);

		memcpy(&filehead.fileflag.MajorVersion,lpAddr+POS,sizeof(DWORD));
		
		POS += sizeof(DWORD);

		memcpy(&filehead.fileflag.MinorVersion,lpAddr+POS,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(&filehead.fileflag.DogID,lpAddr+POS,sizeof(DWORD));

		POS += sizeof(DWORD);

		memcpy(filehead.fileflag.btReserve,lpAddr+27,5);

		POS += 5;

		memcpy(filehead.btKey,lpAddr+POS,KEY_LEN);

		CryptKey(filehead.btKey,KEY_LEN);

		POS += KEY_LEN;

		memcpy(filehead.btEncode,lpAddr+POS,ENCODE_LEN);

		POS += ENCODE_LEN;

		memcpy(filehead.btEncodeTable,lpAddr+POS,TABLE_LEN);
		
		FileFlag_XOR(filehead.fileflag);

//		memmove(lpAddr,lpAddr+HEAD_LEN,dwLow-HEAD_LEN);

		ChangeEncodeByKey(filehead.btEncode,(LPBYTE)lpszKey,nKeyLength);

		BYTE btDecrypt[ENCODE_LEN];
		for (int i=0;i<ENCODE_LEN;i++)
		{
			btDecrypt[filehead.btEncode[i]] = i;
		}

		for (i=HEAD_LEN;i<HEAD_LEN+dwSize;i++)
		{
			bt[i-HEAD_LEN] = btDecrypt[lpAddr[i]];
		}

		::FlushViewOfFile(lpAddr,dwLow);
		UnmapViewOfFile(lpAddr);
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}
	ft.SetTimeInfo(hFile);
	return 0;
}


DWORD GetEncodeOffset(INT64 dwFilePosition)
{
//	return dwFilePosition % 3;
//	return 0;
	DWORD dwHigh = (dwFilePosition & 0x00000000FFFF0000) > 16;
	DWORD dwLow = dwFilePosition & 0x000000000000FFFF;
	return (dwHigh + dwLow) % 256;
}

//新加密算法的函数
int EnCryptFile3V4(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,DWORD dwCryptLevel)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}
	if (nKeyLength <= 0)
		return ERROR_KEYLEN_OVERFLOW;
	
	if (IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,FALSE) == ERROR_FILE_CRYPTED)
		return ERROR_ALREADY_CRYPTE;
	
	CryptFileHead filehead;
	
	memcpy(filehead.fileflag.btHead,(LPBYTE)"DG_FILE VER 4.0",15);

	filehead.fileflag.MajorVersion = 4;
	filehead.fileflag.MinorVersion = 0;

	filehead.fileflag.DogID = dogID;
	for (DWORD i=0;i<5;i++)
	{
		filehead.fileflag.btReserve[i] = 0xff;
	}

	FileFlag_XOR(filehead.fileflag);  //将文件标识隐含

	CreateKey(filehead.btKey,(LPBYTE)lpszKey,nKeyLength);
	CreateEncode(filehead.btEncode);

	CreateEncodeTable(filehead.btEncodeTable);
	
	BYTE btEncodeFile[ENCODE_LEN] = {0};
	memcpy(btEncodeFile,filehead.btEncode,ENCODE_LEN);

	memset(filehead.btReserve,0xff,RESERVE_LEN);
	BYTE m_btEncodeInfoEx[ENCODE_LEN*2] = {0};
	memcpy(m_btEncodeInfoEx,filehead.btEncode,ENCODE_LEN);
	memcpy(m_btEncodeInfoEx+ENCODE_LEN,filehead.btEncode,ENCODE_LEN);
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	CFileTime ft;

	ft.GetTimeInfo(hFile);
	DWORD dwLow;
	DWORD dwHigh;
	dwLow = GetFileSize(hFile,&dwHigh);
	
	LARGE_INTEGER largeSize;
	largeSize.LowPart = dwLow;
	largeSize.HighPart = dwHigh;

	largeSize.QuadPart += HEAD_LEN;

	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,largeSize.HighPart,largeSize.LowPart,NULL);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	DWORD dwAlloc = sysInfo.dwAllocationGranularity*512; //32MB

	if (hMap)
	{
		LARGE_INTEGER CurrentData;
		CurrentData.QuadPart = 0;

		BOOL bFirst = TRUE;

		BYTE btTmpLeftBuffer[HEAD_LEN] = {0};  //上一次操作的尾部1K数据
		BYTE btTmp[HEAD_LEN] = {0};
		while(TRUE)
		{
			//文件未结束
			if (CurrentData.QuadPart <= largeSize.QuadPart)
			{
				if (CurrentData.QuadPart + dwAlloc < largeSize.QuadPart)
				{
					LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,CurrentData.HighPart,CurrentData.LowPart,dwAlloc);
					if (lpAddr == NULL)
					{
						ft.SetTimeInfo(hFile);
						CloseHandle(hMap);
						nSystemErrorCode = GetLastError();
						return ERROR_NO_MAPVIEW;
					}					
					if (bFirst)
					{
						//保留64M的最后1024字节，用于下一次的操作
						memcpy(btTmpLeftBuffer,(LPBYTE)lpAddr+dwAlloc-HEAD_LEN,HEAD_LEN);

						memmove(lpAddr+HEAD_LEN,lpAddr,dwAlloc-HEAD_LEN);
						//将移空的前1K字节清零
						memset(lpAddr,0,HEAD_LEN);
					
						//将文件的原有部分加密


						//以下将写入文件前1K的加密信息
						int POS = 0;
						memcpy(lpAddr+POS,filehead.fileflag.btHead,15);

						POS += sizeof(filehead.fileflag.btHead);

						memcpy(lpAddr+POS,&filehead.fileflag.MajorVersion,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(lpAddr+POS,&filehead.fileflag.MinorVersion,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(lpAddr+POS,&filehead.fileflag.DogID,sizeof(DWORD));

						POS += sizeof(DWORD);
					
						memset(lpAddr+POS,0xff,5);

						POS += 5;

						CCryp crypt;
						BYTE Key_MD5[16] = {0};
						crypt.MD5Update((LPBYTE)lpszKey,strlen((char*)lpszKey));
						crypt.MD5Final(Key_MD5);

						memcpy(lpAddr+POS,Key_MD5,16);
						POS += 16;
						
						DGCommonFunc::Encrypt_RC4(filehead.btEncode,ENCODE_LEN,(LPSTR)lpszKey);

						memcpy(lpAddr+POS,filehead.btEncode,ENCODE_LEN);
						
						POS = 0x2a0;
						
				//密级
						memcpy(lpAddr+POS,&dwCryptLevel,sizeof(DWORD));

						POS += sizeof(DWORD);
				//校验码
						DWORD dwCheck = dwCryptLevel^CHECK_CODE;
						memcpy(lpAddr+POS,&dwCheck,sizeof(DWORD));

						POS += sizeof(DWORD);
				//
						memset(filehead.btReserve,0x00,RESERVE_LEN);
						memcpy(lpAddr+POS,filehead.btReserve,RESERVE_LEN);

						for (DWORD i=0;i<dwAlloc-HEAD_LEN;i++)
						{
							lpAddr[HEAD_LEN+i] = m_btEncodeInfoEx[GetEncodeOffset(i)+lpAddr[HEAD_LEN+i]];
						}

						bFirst = FALSE;
					}else
					{
						memcpy(btTmp,(LPBYTE)lpAddr+dwAlloc-HEAD_LEN,HEAD_LEN);
						
						memmove(lpAddr+HEAD_LEN,lpAddr,dwAlloc-HEAD_LEN);
						
						//将上次数据块的尾部复制到数据块的头部
						memcpy(lpAddr,btTmpLeftBuffer,HEAD_LEN);
					
						memcpy(btTmpLeftBuffer,btTmp,HEAD_LEN);
						//将文件的原有部分加密
						for (i=0;i<dwAlloc;i++)
						{
							lpAddr[i] = m_btEncodeInfoEx[GetEncodeOffset(CurrentData.QuadPart+i)+lpAddr[i]];
						}
					}

					::FlushViewOfFile(lpAddr,dwAlloc);
					UnmapViewOfFile(lpAddr);
					
				}else
				{
					LARGE_INTEGER LeftBufferSize;
					LeftBufferSize.QuadPart = largeSize.QuadPart - CurrentData.QuadPart;
					LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,CurrentData.HighPart,CurrentData.LowPart,LeftBufferSize.LowPart);
					if (lpAddr == NULL)
					{
						CloseHandle(hMap);
						ft.SetTimeInfo(hFile);
						
						nSystemErrorCode = GetLastError();
						return ERROR_NO_MAPVIEW;
					}
					if (bFirst)   //文件不大于64M,则按原有方式加密
					{
						memmove(lpAddr+HEAD_LEN,lpAddr,LeftBufferSize.LowPart-HEAD_LEN);
						//将移空的前1K字节清零
						memset(lpAddr,0,HEAD_LEN);

						//以下将写入文件前1K的加密信息
						int POS = 0;
						memcpy(lpAddr+POS,filehead.fileflag.btHead,15);

						POS += sizeof(filehead.fileflag.btHead);

						memcpy(lpAddr+POS,&filehead.fileflag.MajorVersion,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(lpAddr+POS,&filehead.fileflag.MinorVersion,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(lpAddr+POS,&filehead.fileflag.DogID,sizeof(DWORD));

						POS += sizeof(DWORD);
					
						memset(lpAddr+POS,0xff,5);

						POS += 5;

						CCryp crypt;
						BYTE Key_MD5[16] = {0};
						crypt.MD5Update((LPBYTE)lpszKey,strlen((char*)lpszKey));
						crypt.MD5Final(Key_MD5);

						memcpy(lpAddr+POS,Key_MD5,16);
						POS += 16;
						
						DGCommonFunc::Encrypt_RC4(filehead.btEncode,ENCODE_LEN,(LPSTR)lpszKey);

						memcpy(lpAddr+POS,filehead.btEncode,ENCODE_LEN);
						
						POS = 0x2a0;
						
				//密级
						memcpy(lpAddr+POS,&dwCryptLevel,sizeof(DWORD));

						POS += sizeof(DWORD);
				//校验码
						DWORD dwCheck = dwCryptLevel^CHECK_CODE;
						memcpy(lpAddr+POS,&dwCheck,sizeof(DWORD));

						POS += sizeof(DWORD);
				//
						memset(filehead.btReserve,0x00,RESERVE_LEN);
						memcpy(lpAddr+POS,filehead.btReserve,RESERVE_LEN);

						for (DWORD i=0;i<LeftBufferSize.LowPart-HEAD_LEN;i++)
						{
							lpAddr[HEAD_LEN+i] = m_btEncodeInfoEx[GetEncodeOffset(i)+lpAddr[HEAD_LEN+i]];
						}

						bFirst = FALSE;
					}else //文件结尾
					{						
						memmove(lpAddr+HEAD_LEN,lpAddr,LeftBufferSize.LowPart-HEAD_LEN);
						
						//将上次数据块的尾部复制到数据块的头部
						memcpy(lpAddr,btTmpLeftBuffer,HEAD_LEN);
					
						//将文件的原有部分加密
						for (i=0;i<LeftBufferSize.LowPart;i++)
						{
							lpAddr[i] = m_btEncodeInfoEx[GetEncodeOffset(CurrentData.QuadPart+i)+lpAddr[i]];
						}					
					}
					
					::FlushViewOfFile(lpAddr,LeftBufferSize.LowPart);
					UnmapViewOfFile(lpAddr);
				}
			}else
			{
				break;
			}
			CurrentData.QuadPart += dwAlloc;
		}
		
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;		
	}	
	ft.SetTimeInfo(hFile);

	return 0;
}

int  DeCryptFile2V4(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength)
{
	if (nKeyLength > KEY_LEN)
	{
		nKeyLength = KEY_LEN;
	}

	if (ERROR_FILE_CRYPTED != IsCryptedFile(lpszFileName,dogID,lpszKey,nKeyLength,TRUE)) //只有符合全部的条件
		return ERROR_DECRYPT_NORMAL;
	
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		nSystemErrorCode = GetLastError();
		return ERROR_CANNOT_OPENFILE;
	}
	CFileTime ft;

	ft.GetTimeInfo(hFile);
	DWORD dwLow;
	DWORD dwHigh;
	dwLow = GetFileSize(hFile,&dwHigh);

	LARGE_INTEGER largeSize;
	largeSize.LowPart = dwLow;
	largeSize.HighPart = dwHigh;

	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,dwHigh,dwLow,NULL);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	DWORD dwAlloc = sysInfo.dwAllocationGranularity; //32MB

	if (hMap)
	{
		LARGE_INTEGER CurrentData;
		CurrentData.QuadPart = 0;
		
		BOOL bFirst = TRUE;

		BYTE btDeCrypt[ENCODE_LEN*2] = {0};
		while(TRUE)
		{
			//当前文件是否已经读取完毕
			if (CurrentData.QuadPart < largeSize.QuadPart)
			{	
				//是否至少有一个可分配单元（64K）可以读取
				if ((CurrentData.QuadPart + dwAlloc + HEAD_LEN) < largeSize.QuadPart)
				{
					//每次多申请 1K的内容
					LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,CurrentData.HighPart,CurrentData.LowPart,dwAlloc+HEAD_LEN);

					if (bFirst) //取得btDeCrypt
					{
						int POS = 48;
						
						BYTE btEncode[ENCODE_LEN] = {0};
						BYTE btDecode[ENCODE_LEN] = {0};
						memcpy(btEncode,lpAddr+POS,256);
						DGCommonFunc::Encrypt_RC4(btEncode,ENCODE_LEN,(LPSTR)lpszKey);

						for (int i=0;i<ENCODE_LEN;i++)
						{
							btDecode[btEncode[i]] = i;
						}
						
						memcpy(btDeCrypt,btDecode,ENCODE_LEN);
						memcpy(btDeCrypt+ENCODE_LEN,btDecode,ENCODE_LEN);

						memmove(lpAddr,lpAddr+HEAD_LEN,dwAlloc);


						for (int j=0;j<dwAlloc;j++)
						{
							lpAddr[j] = btDeCrypt[lpAddr[j]] - GetEncodeOffset(CurrentData.QuadPart+j);
						}

						bFirst = FALSE;
					}else
					{
						memmove(lpAddr,lpAddr+HEAD_LEN,dwAlloc);
						for (int j=0;j<dwAlloc;j++)
						{
							lpAddr[j] = btDeCrypt[lpAddr[j]] - GetEncodeOffset(CurrentData.QuadPart+j);
						}
	
					}
					::FlushViewOfFile(lpAddr,dwAlloc);
						
					UnmapViewOfFile(lpAddr);
					CurrentData.QuadPart += dwAlloc;
				}else		//32M+1K的剩余部分
				{
					LARGE_INTEGER dwLeft;
					dwLeft.QuadPart = largeSize.QuadPart - CurrentData.QuadPart;

					LPBYTE lpAddr = (LPBYTE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,CurrentData.HighPart,CurrentData.LowPart,dwLeft.LowPart);

					if (lpAddr == NULL)
					{
						CloseHandle(hMap);
						ft.SetTimeInfo(hFile);
						nSystemErrorCode = GetLastError();
						return ERROR_NO_MAPVIEW;
					}

					CurrentData.QuadPart += dwLeft.QuadPart;

					if (bFirst)
					{
						CryptFileHead filehead;

						int POS = 0;
						memcpy(filehead.fileflag.btHead,lpAddr,15);

						POS += sizeof(filehead.fileflag.btHead);

						memcpy(&filehead.fileflag.MajorVersion,lpAddr+POS,sizeof(DWORD));
						
						POS += sizeof(DWORD);

						memcpy(&filehead.fileflag.MinorVersion,lpAddr+POS,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(&filehead.fileflag.DogID,lpAddr+POS,sizeof(DWORD));

						POS += sizeof(DWORD);

						memcpy(filehead.fileflag.btReserve,lpAddr+27,5);

						POS += 5;

						POS = 48;
						
						BYTE btEncode[ENCODE_LEN] = {0};
						BYTE btDecode[ENCODE_LEN] = {0};
						memcpy(btEncode,lpAddr+POS,256);
						DGCommonFunc::Encrypt_RC4(btEncode,ENCODE_LEN,(LPSTR)lpszKey);

						for (int i=0;i<ENCODE_LEN;i++)
						{
							btDecode[btEncode[i]] = i;
						}
						
						memcpy(btDeCrypt,btDecode,ENCODE_LEN);
						memcpy(btDeCrypt+ENCODE_LEN,btDecode,ENCODE_LEN);
						memmove(lpAddr,lpAddr+HEAD_LEN,dwLeft.LowPart-HEAD_LEN);

						for (int j=0;j<dwLeft.LowPart-HEAD_LEN;j++)
						{
							lpAddr[j] = btDeCrypt[lpAddr[j]] - GetEncodeOffset(CurrentData.QuadPart-dwLeft.LowPart+j);
						}	
							
						::FlushViewOfFile(lpAddr,dwLeft.LowPart-HEAD_LEN);
						
						UnmapViewOfFile(lpAddr);
						
						bFirst = FALSE;
					}else
					{
						memmove(lpAddr,lpAddr+HEAD_LEN,dwLeft.LowPart-HEAD_LEN);
						for (int j=0;j<dwLeft.LowPart-HEAD_LEN;j++)
						{
							lpAddr[j] = btDeCrypt[lpAddr[j]] - GetEncodeOffset(CurrentData.QuadPart-dwLeft.LowPart+j);
						}
						::FlushViewOfFile(lpAddr,dwLeft.LowPart-HEAD_LEN);
						
						UnmapViewOfFile(lpAddr);
					}
				}
			}else
			{
				break;
			}
					
		}
		CloseHandle(hMap);
	}else{
		ft.SetTimeInfo(hFile);
		nSystemErrorCode = GetLastError();
		return ERROR_NO_MAPPING;
	}

	SetFilePointer(hFile,-HEAD_LEN,0,FILE_END);
	SetEndOfFile(hFile);
	ft.SetTimeInfo(hFile);
	return 0;
}
