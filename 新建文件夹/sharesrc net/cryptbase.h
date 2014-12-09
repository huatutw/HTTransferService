#ifndef CRYPTBASE_H
#define CRYPTBASE_H

//三个函数的返回值
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
	ERROR_FILE_NORMAL = 9,
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

class CDGFileTime
{
public:
	CDGFileTime() {}
	~CDGFileTime() {}

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

#define HEAD_LEN 1024
#define KEY_LEN 128
#define ENCODE_LEN 256
#define TABLE_LEN 256

#define CRYPT_LEVEL_POS 0x2a0  //672
#define CRYPT_TIME_POS	0x2a0+2*sizeof(DWORD)
#define CRYPT_GUID_POS  (CRYPT_TIME_POS+sizeof(CryptFileTime))
#define CRYPT_COMPUTER_POS  (CRYPT_GUID_POS+sizeof(GUID))
#define CRYPT_USER_POS CRYPT_COMPUTER_POS+32

#define RESERVE_LEN 344

//DocGuarder文件的版本,目前有1.0 和 3.0
#define MAJOR_VERSION	3
#define MINOR_VERSION	0

//1.0文件的文件标识为 "DOCGUARDER_FILE"
//3.0文件的文件标识为 "DG_FILE VER 3.0"

#define XOR_CODE1 0x5c
#define XOR_CODE2 0xc5

#define CHECK_CODE 0x89abcdef

//int nSystemErrorCode = 0;


//lpszFileName:文件名
//dogID:		硬件狗的号码
//lpszKey:		密钥
//nKeyLength:	密钥的长度 不能大于128位

//返回 0 表示成功,返回其他值见上述的错误码

//1.0文件的加密函数
extern int EnCryptFile(LPCTSTR lpszFileName,DWORD dogID,const char* lpszKey,int nKeyLength);

//3.0文件的加密函数
extern int EnCryptFile2(LPCTSTR lpszFileName,DWORD dogID,const char* lpszKey,int nKeyLength,DWORD dwCryptLevel);
extern int EnCryptFile3(LPCTSTR lpszFileName,DWORD dogID,const char* lpszKey,int nKeyLength,DWORD dwCryptLevel);

//lpszFileName:文件名
//dogID:		硬件狗的号码
//lpszKey:		密钥
//nKeyLength:	密钥的长度 不能大于128位
//返回 0 表示成功,返回其他值见上述的错误码
extern int DeCryptFile(LPCTSTR lpszFileName,DWORD dogID,const char* lpszKey,int nKeyLength);
extern int DeCryptFile2(LPCTSTR lpszFileName,DWORD dogID,const char* lpszKey,int nKeyLength);

//lpszFileName:文件名
//dogID:		硬件狗的号码
//lpszKey:		密钥
//nKeyLength:	密钥的长度 不能大于128位
//bFullCheck:	TRUE:要求函数判断该文件是否是给定的dogID和key加密的文件
//				FALSE:仅判断文件标识表明是Docguarder格式的文件即可

//返回 ERROR_FILE_CRYPTED 表示是一个符合条件的加密文件
//返回 0	表示不是一个DOCGUARDER格式的文件(即不是一个加密文件)
//返回其他表示无法判断是否为加密文件,错误码见上
extern int IsCryptedFile(LPCTSTR lpszFileName,DWORD dogID,const char* lpszKey,int nKeyLength,BOOL bFullCheck,DWORD* dwCryptLevel = NULL,DWORD* dwMajorVersion = NULL,DWORD* dwMinorVersion = NULL,DWORD* dwCheck = NULL);

extern int ModifyFileLimitTime(LPCTSTR lpszFileName,CryptFileTime timeInfo,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern int GetFileLimitTime(LPCTSTR lpszFileName,CryptFileTime& timeInfo,DWORD dogID,LPCSTR lpszKey,int nKeyLength);

extern int GetFileGUID(LPCTSTR lpszFileName,GUID& guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern int ModifyFileGUID(LPCTSTR lpszFileName,GUID guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern int ClearFileLimitTime(LPCTSTR lpszFileName,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
//extern int nSystemErrorCode;

extern int ModifyFileOutsideGUID(LPCTSTR lpszFileName,GUID guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength);
extern int GetFileOutsideGUID(LPCTSTR lpszFileName,GUID& guid,DWORD dogID,LPCSTR lpszKey,int nKeyLength);

//保护关键文件的加密
extern BOOL dcfile(LPCTSTR fpath, CString strpass);
extern BOOL ecfile(LPCTSTR fpath, CString strpass);
extern __int64 epass(CString strpass);
#endif