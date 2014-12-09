#include "afx.h"
#include "afxwin.h"

enum
{
	ERROR_FILE_CRYPTED = 1,	   //用于表示文件是一个合法的加密文件
	ERROR_ALREADY_CRYPTE = 2,  //已经是加密文件或文件不能操作
	ERROR_CANNOT_OPENFILE = 3, //不能打开该文件
	ERROR_NO_MAPPING = 4,	   //文件映象句柄不能创建
	ERROR_NO_MAPVIEW = 5,	   //无法创建内存映象
	ERROR_KEYLEN_OVERFLOW = 6, //密钥长度过长
	ERROR_DECRYPT_NORMAL = 7,  //试图解密一个正常的文件
	ERROR_NOPRIV_FILE = 8,
};

enum 
{
	OFFLINE_LOCALTIME,
	OFFLINE_FORBID,
};

enum
{
	OPER_FORBID,
	OPER_PROMPT,
};

struct CryptFileTime
{
	char	   btFlag[10];				//离线时间的标记
	SYSTEMTIME tOverTime;
	int		   nOffineLine;				//离线处理方式
	int		   nFileOper;				//受限文件处理方式
};

extern "C" int add(int x,int y);
extern "C" int IsCryptedFile(LPCTSTR lpszFileName,DWORD dogID,const char* lpszKey,int nKeyLength,BOOL bFullCheck,DWORD* dwCryptLevel = NULL,DWORD* dwMajorVersion = NULL,DWORD* dwMinorVersion = NULL,DWORD* dwCheck = NULL);
extern "C" int EnCryptFile(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern "C" int EnCryptFile2(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,DWORD dwCryptLevel);
extern "C" int DeCryptFile(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern "C" int DeCryptFile2(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern "C" int EnCryptFile3(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,DWORD dwCryptLevel);
extern "C" int EnCryptFile4(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,DWORD dwCryptLevel);
extern "C" int ModifyFileLimitTime(LPCTSTR lpszFileName,CryptFileTime timeInfo,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern "C" int ClearFileLimitTime(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern "C" int GetFileLimitTime(LPCTSTR lpszFileName,CryptFileTime& timeInfo,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern "C" int ModifyFileKey(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,DWORD newDogID,LPCSTR newKey,int nNewKeyLength);
extern "C" int ModifyFileGUID(LPCTSTR lpszFileName,GUID guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern "C" int GetFileGUID(LPCTSTR lpszFileName,GUID& guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern "C" int ModifyFileOutsideGUID(LPCTSTR lpszFileName,GUID guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern "C" int GetFileOutsideGUID(LPCTSTR lpszFileName,GUID& guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern "C" BOOL ReadCryptFile(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength,LPBYTE bt,DWORD dwSize);
extern "C" BOOL dcfile(LPCTSTR fpath, CString strpass);
extern "C" BOOL ecfile(LPCTSTR fpath, CString strpass);
