#pragma once
#include "ErrorRecorder.h"
#include <tchar.h>
#include <WinSCard.h>

class CCryptWrapper
{
public:
	enum
	{
		ENCRYPT_TYPE_NORMAL = 0,
		ENCRYPT_TYPE_DOCGUARDER_ONE = 1,
		ENCRYPT_TYPE_DOCGUARDER_THREE = 3,
		ENCRYPT_TYPE_DOCGUARDER_FOUR = 4,
		ENCRYPT_TYPE_DOCGUARDER_FIVE = 5,
		ENCRYPT_TYPE_AES_ONE = 0x10,
		ENCRYPT_TYPE_AES_TWO = 0x20,
	};

#define ENCRYPT_HEAD_LENGTH	1024


#define AES_GUID_LEN	36
#define AES_MD5_LEN		32
#define AES_OWNER_LEN	128
#define AES_LOG_INFO_LEN	128

	typedef struct _AES_Header_Info
	{
		DWORD	m_nMajVer;
		DWORD	m_nMinVer;
		BYTE	m_btGUID[AES_GUID_LEN+1];
		BYTE	m_btOwner[AES_OWNER_LEN];
		SYSTEMTIME m_time;
		BYTE	m_keyMD5[AES_MD5_LEN];
		DWORD	m_dwCrc32;
		BYTE	m_btOwnerUserName[AES_OWNER_LEN];

		DWORD	m_dwPermission;
		SYSTEMTIME m_ExpireDate;

		WCHAR wszLogInfo[AES_LOG_INFO_LEN];
	} AES_HeaderInfo;

#define DOCGUARDER_SMALL_HEAD_LENGTH	15
#define DOCGUARDER_SMALL_RESERVE_LENGTH	5

	struct FileFlag
	{
		BYTE btHead[DOCGUARDER_SMALL_HEAD_LENGTH];
		DWORD MajorVersion;
		DWORD MinorVersion;
		DWORD DogID;
		BYTE btReserve[DOCGUARDER_SMALL_RESERVE_LENGTH];
	};


#define KEY_LEN	128
#define ENCODE_LEN	256
#define TABLE_LEN	256
#define RESERVE_LEN_ONE	348

#define STRING_GUID_LENGTH	38

	struct CryptFileHeadOne
	{
		FileFlag fileflag;

		BYTE btKey[KEY_LEN];
		BYTE btEncode[ENCODE_LEN];
		BYTE btEncodeTable[TABLE_LEN];
		BYTE btReserve[RESERVE_LEN_ONE];
	};

	struct CryptFileTime
	{
		char	   btFlag[10];				//离线时间的标记
		SYSTEMTIME tOverTime;
		int		   nOffineLine;				//离线处理方式
		int		   nFileOper;				//受限文件处理方式
	};

#define DOCGUARDER_USER_LEN	128
#define RESERVE_LEN_THREE	124

	struct CryptFileHeadThree
	{
		FileFlag fileflag;

		BYTE btKey[KEY_LEN];
		BYTE btEncode[ENCODE_LEN];
		BYTE btEncodeTable[TABLE_LEN];

		DWORD dwEncryptLevel;
		DWORD dwEncryptLevelCheck;

		CryptFileTime timeInfo;

		GUID guidFile;

		GUID guidOutside;

		BYTE btOwner[DOCGUARDER_USER_LEN];
		DWORD dwPermission;
		GUID guidAuthorize;

		BYTE btReserve[RESERVE_LEN_THREE];
	};

#define MD5_CHECK_LEN	16
#define RESERVE_LEN_MID	352
#define RESERVE_LEN_FOUR	272
#define AES_ENCRYPTED_PART_LEN	688

	struct CryptFileHeadFour
	{
		FileFlag fileflag;

		BYTE btKeyCheck[MD5_CHECK_LEN];
		BYTE btEncode[ENCODE_LEN];
		BYTE btUserCheck[MD5_CHECK_LEN];
		BYTE btReserveMid[RESERVE_LEN_MID];

		DWORD dwEncryptLevel;
		DWORD dwEncryptLevelCheck;

		CryptFileTime timeInfo;

		GUID guidFile;

		GUID guidOutside;

		BYTE btReserve[RESERVE_LEN_FOUR];
	};

#define ENCRYPT_LOG_INFO_LEN	256
#define ENCRYPT_LOG_INFO_LEN_FIVE	260
#define RESERVE_LEN_FIVE	284

#define EF_AES_KEY_LEN	16
#define EF_AES_VECTOR_LEN	16

	struct CryptFileHeadFive
	{
		FileFlag fileflag;

		BYTE btKeyCheck[MD5_CHECK_LEN];
		BYTE btEncode[ENCODE_LEN];
		
		DWORD dwEncryptLevel;
		DWORD dwEncryptLevelCheck;

		GUID guidOutside;

		BYTE btOwner[DOCGUARDER_USER_LEN];
		DWORD dwPermission;
		GUID guidAuthorize;
		BYTE btLogInfo[ENCRYPT_LOG_INFO_LEN_FIVE];

		BYTE btReserve[RESERVE_LEN_FIVE];
	};

public:
	CCryptWrapper(void);
	CCryptWrapper(LPCTSTR lpstrPath, LPCBYTE lpbtKey, int nLength, DWORD dwDogId);
	CCryptWrapper(LPCTSTR lpstrPath, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, DWORD dwFileLevel);

	~CCryptWrapper(void);

private:
	void Release();

	BOOL IsFileConsistent(LPCTSTR lpstrPath);

	BOOL IsDogAndKeyConsistent(LPCBYTE lpbtKey, int nLength, DWORD dwDogId);

	DWORD GetFileCryptInfoInternal(CErrorRecorder& errorRecorder);

public:
	void ResetFilePath(LPCTSTR lpstrPath);

	void ResetDogAndKey(LPCBYTE lpbtKey, int nLength, DWORD dwDogId);

	BOOL IsCryptedFileSimple(LPCTSTR lpstrPath, CErrorRecorder& errorRecorde);

	BOOL IsCryptedFile(CErrorRecorder& errorRecorder);

	BOOL IsCryptedFile(LPCTSTR lpstrPath, CErrorRecorder& errorRecorder);

	BOOL IsCryptedFile(LPCTSTR lpstrPath, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileCryptedType(DWORD& dwCryptedType, CErrorRecorder& errorRecorder);

	DWORD GetFileCryptedType(LPCTSTR lpstrPath, DWORD& dwCryptedType, CErrorRecorder& errorRecorder);

	DWORD GetFileCryptedType(LPCTSTR lpstrPath, DWORD& dwCryptedType, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileCryptInfo(LPVOID lpBuff, int& nBuffLength, CErrorRecorder& errorRecorder);

	DWORD GetFileCryptInfo(LPCTSTR lpstrPath, LPVOID lpBuff, int& nBuffLength, CErrorRecorder& errorRecorder);

	DWORD GetFileCryptInfo(LPCTSTR lpstrPath, LPVOID lpBuff, int& nBuffLength, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD DecryptFile(CErrorRecorder& errorRecorder);

	DWORD DecryptFile(LPCTSTR lpstrPath, CErrorRecorder& errorRecorder);

	DWORD DecryptFile(LPCTSTR lpstrPath, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD DecryptFileWithTarget(LPCTSTR lpstrTarget, CErrorRecorder& errorRecorder);

	DWORD DecryptFileWithTarget(LPCTSTR lpstrPath, LPCTSTR lpstrTarget, CErrorRecorder& errorRecorder);

	DWORD DecryptFileWithTarget(LPCTSTR lpstrPath, LPCTSTR lpstrTarget, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD EncryptFile(CErrorRecorder& errorRecorder);

	DWORD EncryptFile(LPCTSTR lpstrPath, CErrorRecorder& errorRecorder);

	DWORD EncryptFile(LPCTSTR lpstrPath, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD EncryptFile(LPCTSTR lpstrPath, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, DWORD dwFileLevel, CErrorRecorder& errorRecorder);

	DWORD EncryptFile(LPCTSTR lpstrPath, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, DWORD dwFileLevel, DWORD dwCryptedType, CErrorRecorder& errorRecorder);

	DWORD EncryptFile(const CryptFileHeadFive& headInfo, CErrorRecorder& errorRecorder);

	DWORD EncryptFile(LPCTSTR lpstrPath, const CryptFileHeadFive& headInfo, CErrorRecorder& errorRecorder);

	DWORD EncryptFile(LPCTSTR lpstrPath, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, const CryptFileHeadFive& headInfo, CErrorRecorder& errorRecorder);

	DWORD ModifyFileKey(LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);
	
	DWORD ModifyFileKey(LPCTSTR lpstrPath, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);
	
	DWORD ModifyFileKey(LPCTSTR lpstrPath, LPCBYTE lpbtKeyOld, int nLengthOld, DWORD dwDogIdOld, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFileLevel(DWORD dwCryptLevel, CErrorRecorder& errorRecorder);

	DWORD SetFileLevel(LPCTSTR lpstrPath, DWORD dwCryptLevel, CErrorRecorder& errorRecorder);

	DWORD SetFileLevel(LPCTSTR lpstrPath, DWORD dwCryptLevel, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileLevel(DWORD& dwCryptLevel, CErrorRecorder& errorRecorder);

	DWORD GetFileLevel(LPCTSTR lpstrPath, DWORD& dwCryptLevel, CErrorRecorder& errorRecorder);

	DWORD GetFileLevel(LPCTSTR lpstrPath, DWORD& dwCryptLevel, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFileLimitTime(const CryptFileTime& timeInfo, CErrorRecorder& errorRecorder);

	DWORD SetFileLimitTime(LPCTSTR lpstrPath, const CryptFileTime& timeInfo, CErrorRecorder& errorRecorder);

	DWORD SetFileLimitTime(LPCTSTR lpstrPath, const CryptFileTime& timeInfo, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileLimitTime(CryptFileTime& timeInfo, CErrorRecorder& errorRecorder);

	DWORD GetFileLimitTime(LPCTSTR lpstrPath, CryptFileTime& timeInfo, CErrorRecorder& errorRecorder);

	DWORD GetFileLimitTime(LPCTSTR lpstrPath, CryptFileTime& timeInfo, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD ClearFileLimitTime(CErrorRecorder& errorRecorder);

	DWORD ClearFileLimitTime(LPCTSTR lpstrPath, CErrorRecorder& errorRecorder);

	DWORD ClearFileLimitTime(LPCTSTR lpstrPath, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFileGuid(const GUID& guidFile, CErrorRecorder& errorRecorder);

	DWORD SetFileGuid(LPCTSTR lpstrPath, const GUID& guidFile, CErrorRecorder& errorRecorder);

	DWORD SetFileGuid(LPCTSTR lpstrPath, const GUID& guidFile, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileGuid(GUID& guidFile, CErrorRecorder& errorRecorder);

	DWORD GetFileGuid(LPCTSTR lpstrPath, GUID& guidFile, CErrorRecorder& errorRecorder);

	DWORD GetFileGuid(LPCTSTR lpstrPath, GUID& guidFile, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFileOutsideGuid(const GUID& guidOutside, CErrorRecorder& errorRecorder);

	DWORD SetFileOutsideGuid(LPCTSTR lpstrPath, const GUID& guidOutside, CErrorRecorder& errorRecorder);

	DWORD SetFileOutsideGuid(LPCTSTR lpstrPath, const GUID& guidOutside, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileOutsideGuid(GUID& guidOutside, CErrorRecorder& errorRecorder);

	DWORD GetFileOutsideGuid(LPCTSTR lpstrPath, GUID& guidOutside, CErrorRecorder& errorRecorder);

	DWORD GetFileOutsideGuid(LPCTSTR lpstrPath, GUID& guidOutside, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFileOwner(LPCTSTR lpstrOwner, CErrorRecorder& errorRecorder);

	DWORD SetFileOwner(LPCTSTR lpstrPath, LPCTSTR lpstrOwner, CErrorRecorder& errorRecorder);

	DWORD SetFileOwner(LPCTSTR lpstrPath, LPCTSTR lpstrOwner, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileOwner(LPTSTR lpstrOwner, int& nBuffLength, CErrorRecorder& errorRecorder);

	DWORD GetFileOwner(LPCTSTR lpstrPath, LPTSTR lpstrOwner, int& nBuffLength, CErrorRecorder& errorRecorder);

	DWORD GetFileOwner(LPCTSTR lpstrPath, LPTSTR lpstrOwner, int& nBuffLength, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFilePermission(DWORD dwPermission, CErrorRecorder& errorRecorder);

	DWORD SetFilePermission(LPCTSTR lpstrPath, DWORD dwPermission, CErrorRecorder& errorRecorder);

	DWORD SetFilePermission(LPCTSTR lpstrPath, DWORD dwPermission, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFilePermission(DWORD& dwPermission, CErrorRecorder& errorRecorder);

	DWORD GetFilePermission(LPCTSTR lpstrPath, DWORD& dwPermission, CErrorRecorder& errorRecorder);

	DWORD GetFilePermission(LPCTSTR lpstrPath, DWORD& dwPermission, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFileAuthorizeGuid(const GUID& guidAuthorize, CErrorRecorder& errorRecorder);

	DWORD SetFileAuthorizeGuid(LPCTSTR lpstrPath, const GUID& guidAuthorize, CErrorRecorder& errorRecorder);

	DWORD SetFileAuthorizeGuid(LPCTSTR lpstrPath, const GUID& guidAuthorize, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileAuthorizeGuid(GUID& guidAuthorize, CErrorRecorder& errorRecorder);

	DWORD GetFileAuthorizeGuid(LPCTSTR lpstrPath, GUID& guidAuthorize, CErrorRecorder& errorRecorder);

	DWORD GetFileAuthorizeGuid(LPCTSTR lpstrPath, GUID& guidAuthorize, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFileLogInfo(LPCTSTR lpstrLogInfo, CErrorRecorder& errorRecorder);

	DWORD SetFileLogInfo(LPCTSTR lpstrPath, LPCTSTR lpstrLogInfo, CErrorRecorder& errorRecorder);

	DWORD SetFileLogInfo(LPCTSTR lpstrPath, LPCTSTR lpstrLogInfo, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileLogInfo(LPTSTR lpstrLogInfo, int& nBuffLength, CErrorRecorder& errorRecorder);

	DWORD GetFileLogInfo(LPCTSTR lpstrPath, LPTSTR lpstrLogInfo, int& nBuffLength, CErrorRecorder& errorRecorder);

	DWORD GetFileLogInfo(LPCTSTR lpstrPath, LPTSTR lpstrLogInfo, int& nBuffLength, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFileOwnerName(LPCTSTR lpstrOwnerName, CErrorRecorder& errorRecorder);

	DWORD SetFileOwnerName(LPCTSTR lpstrPath, LPCTSTR lpstrOwnerName, CErrorRecorder& errorRecorder);

	DWORD SetFileOwnerName(LPCTSTR lpstrPath, LPCTSTR lpstrOwnerName, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileOwnerName(LPTSTR lpstrOwnerName, int& nBuffLength, CErrorRecorder& errorRecorder);

	DWORD GetFileOwnerName(LPCTSTR lpstrPath, LPTSTR lpstrOwnerName, int& nBuffLength, CErrorRecorder& errorRecorder);

	DWORD GetFileOwnerName(LPCTSTR lpstrPath, LPTSTR lpstrOwnerName, int& nBuffLength, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFileTime(const SYSTEMTIME& timeFile, CErrorRecorder& errorRecorder);

	DWORD SetFileTime(LPCTSTR lpstrPath, const SYSTEMTIME& timeFile, CErrorRecorder& errorRecorder);

	DWORD SetFileTime(LPCTSTR lpstrPath, const SYSTEMTIME& timeFile, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileTime(SYSTEMTIME& timeFile, CErrorRecorder& errorRecorder);

	DWORD GetFileTime(LPCTSTR lpstrPath, SYSTEMTIME& timeFile, CErrorRecorder& errorRecorder);

	DWORD GetFileTime(LPCTSTR lpstrPath, SYSTEMTIME& timeFile, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD SetFileExpireTime(const SYSTEMTIME& timeExpire, CErrorRecorder& errorRecorder);

	DWORD SetFileExpireTime(LPCTSTR lpstrPath, const SYSTEMTIME& timeExpire, CErrorRecorder& errorRecorder);

	DWORD SetFileExpireTime(LPCTSTR lpstrPath, const SYSTEMTIME& timeExpire, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	DWORD GetFileExpireTime(SYSTEMTIME& timeExpire, CErrorRecorder& errorRecorder);

	DWORD GetFileExpireTime(LPCTSTR lpstrPath, SYSTEMTIME& timeExpire, CErrorRecorder& errorRecorder);

	DWORD GetFileExpireTime(LPCTSTR lpstrPath, SYSTEMTIME& timeExpire, LPCBYTE lpbtKey, int nLength, DWORD dwDogId, CErrorRecorder& errorRecorder);

	BOOL IsEncryptedFileHead(LPBYTE lpBuff, DWORD& dwCryptType, LPBYTE lpbtHeadInfp);

	void GenerateFileHead(DWORD dwCryptType, LPBYTE lpBuff);

	void EncryptFileHead(DWORD dwCryptType, LPBYTE lpBuff);

	void DecryptData(LPBYTE pDest, LPBYTE pSource, DWORD dwCryptType, LPCBYTE lpBuff, LONGLONG start, ULONG length);
	void EncryptData(LPBYTE pDest, LPBYTE pSource, DWORD dwCryptType, LPCBYTE lpBuff, LONGLONG start, ULONG length);

private:
	LPTSTR m_lpstrPath;
	LPBYTE m_lpbtKey;
	int m_nKeyLength;
	DWORD m_dwDogId;
	DWORD m_dwFileLevel;

	BYTE m_btBuff[ENCRYPT_HEAD_LENGTH];

	DWORD m_dwCryptedType;

	BOOL m_bHasInfo;
};
