#include "stdafx.h"
#include "DGCommonFunc.h"
#include <atlbase.h>
//#include "Cryp.h"
#include   "Nb30.h"   
#pragma   comment   (lib,"netapi32.lib")   

char DGCommonFunc::LMoveBit(int base, int MoveNum) 
{ 
	char result=base; 
	if(MoveNum==0)return 1; 
	if(MoveNum==1)return MoveNum; 
	result=base<<(MoveNum-1); 
	return result; 
}

void DGCommonFunc::ToBase64( const unsigned char *in, int inlen, char *out )
{
	const char base64digits[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	for (; inlen >= 3; inlen -= 3)
	{
		*out++ = base64digits[in[0] >> 2];
		*out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		*out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
		*out++ = base64digits[in[2] & 0x3f];
		in += 3;
	}

	if (inlen > 0)
	{
		U8 fragment;
		
		*out++ = base64digits[in[0] >> 2];
		fragment = (in[0] << 4) & 0x30;
		if (inlen > 1)
			fragment |= in[1] >> 4;
		*out++ = base64digits[fragment];
		*out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
		*out++ = '=';
	}
	*out = '\0';
};

void DGCommonFunc::FromBase64( const unsigned char *base64code, int base64length, char *out )
{
	char base64_alphabet[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	char buf[4]; 
	int i,j; 
	int k; 
	int l=0; 
	U8 temp1[4],temp2; 
	U32 base64a=(base64length/4)-1; 
	U32 base64b=0; 
	int m_padnum = 0;
	for(;base64b<base64a+1;base64b++) 
	{ 
		for(i=0;i<4;i++) 
		{ 
			buf[i]=*(base64code+(base64b*4)+i); 
			for(j=0;j<65;j++) 
			{ 
				if(buf[i]==base64_alphabet[j]) 
				{ 
					temp1[i]=j; 
					break; 
				} 
			} 
		} 
		i--; 
		for(k=1;k<4;k++) 
		{ 
			if(temp1[i-(k-1)]==64){
				m_padnum++; 
				continue;} 
			temp1[i-(k-1)]=temp1[i-(k-1)]/LMoveBit(2,(k-1)*2); 
			temp2=temp1[i-k]; 
			temp2=temp2&(LMoveBit(2,k*2)-1); 
			temp2*=LMoveBit(2,8-(2*k));//move 4 
			temp1[i-(k-1)]=temp1[i-(k-1)]+temp2; 
			out[base64b*3+(3-k)]=temp1[i-(k-1)]; 
		} 
	}
}

void DGCommonFunc::prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key)
{
	unsigned char t;
	unsigned char index1;
	unsigned char index2;
	unsigned char* state;
	int counter;

	state = &key->state[0];
	for(counter = 0; counter < 256; counter++)
	state[counter] = counter;
	key->x = 0;
	key->y = 0;
	index1 = 0;
	index2 = 0;
	for(counter = 0; counter < 256; counter++)
	{
		index2 = (key_data_ptr[index1] + state[counter] + index2) % 256;
		swap_byte(&state[counter], &state[index2]);
		index1 = (index1 + 1) % key_data_len;
	}
}

void DGCommonFunc::rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *key)
{
  unsigned char t;
  unsigned char x;
  unsigned char y;
  unsigned char* state;
  unsigned char xorIndex;
  UINT counter;

  x = key->x;
  y = key->y;
  state = &key->state[0];
  for(counter = 0; counter < buffer_len; counter++)
  {
    x = (x + 1) % 256;
    y = (state[x] + y) % 256;
    swap_byte(&state[x], &state[y]);
    xorIndex = (state[x] + state[y]) % 256;
    buffer_ptr[counter] ^= state[xorIndex];
  }
  key->x = x;
  key->y = y;
}

BOOL DGCommonFunc::Encrypt_RC4(LPBYTE lpszBuffer,DWORD dwSize,char* lpszPassword)
{
	if (lpszPassword == NULL || strlen(lpszPassword) == 0)
		return FALSE;
	rc4_key key;
	prepare_key((LPBYTE)lpszPassword,strlen(lpszPassword),&key);
	rc4(lpszBuffer,dwSize,&key);
	return TRUE;
}

#ifndef NO_MFC_VERSION

BOOL DGCommonFunc::String2IP(CAString strIP,BYTE& ip1,BYTE& ip2,BYTE& ip3,BYTE& ip4)
{
	int pos = strIP.Find(TEXT("."));
	if (pos == -1)
		return FALSE;
	CAString strNum = strIP.Left(pos);
	strIP = strIP.Right(strIP.GetLength()-pos-1);
	ip1 = _ttoi(strNum);
	
	pos = strIP.Find(TEXT("."));
	if (pos == -1)
		return FALSE;
	strNum = strIP.Left(pos);
	strIP = strIP.Right(strIP.GetLength()-pos-1);
	ip2 = _ttoi(strNum);

	pos = strIP.Find(TEXT("."));
	if (pos == -1)
		return FALSE;
	strNum = strIP.Left(pos);
	strIP = strIP.Right(strIP.GetLength()-pos-1);
	ip3 = _ttoi(strNum);
	ip4 = _ttoi(strIP);
	return TRUE;
}
#endif

/*BOOL DGCommonFunc::ReadRegData(LPTSTR strUser,LPTSTR strPass,LPTSTR strServerIP,DWORD& dwServerPort,LPTSTR strUpdateIP,DWORD& dwUpdatePort)
{
	CRegKey DocKey;
	
	if (DocKey.Open(HKEY_CURRENT_USER,TEXT("software\\DocGuarder\\Client"),KEY_READ) == ERROR_SUCCESS)
	{
		USES_CONVERSION;
		TCHAR keybuf[1024] = {0};
		DWORD dwLen = 1024;
		DocKey.QueryValue(keybuf,TEXT("username"),&dwLen);

		char out[1024] = {0};
		DGCommonFunc::FromBase64((const unsigned char*)T2A(keybuf),strlen(T2A(keybuf)),out);
		_tcscpy(strUser,A2T(out));

		dwLen = 1024;
		DocKey.QueryValue(keybuf,TEXT("password"),&dwLen);
		memset(out,0,1024);
		DGCommonFunc::FromBase64((const unsigned char*)T2A(keybuf),strlen(T2A(keybuf)),out);
		_tcscpy(strPass,A2T(out));

		DocKey.Close();
	}else
	{
		return FALSE;
	}

	if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Connect"),KEY_READ) == ERROR_SUCCESS)
	{
		TCHAR keybuf[1024] = {0};
		DWORD dwLen = 1024;
		DocKey.QueryValue(keybuf,TEXT("serverip"),&dwLen);
		_tcscpy(strServerIP,keybuf);

		dwLen = 1024;
		DocKey.QueryValue(keybuf,TEXT("updateip"),&dwLen);
		_tcscpy(strUpdateIP,keybuf);

		DocKey.QueryValue(dwServerPort,TEXT("serverport"));
		DocKey.QueryValue(dwUpdatePort,TEXT("updateport"));

		DocKey.Close();
	}else
	{
		return FALSE;
	}
	return TRUE;
}*/

BOOL DGCommonFunc::ReadRegDataEx(LPWSTR strUser,LPWSTR strPass)
{
	CRegKey DocKey;

	if (DocKey.Open(HKEY_CURRENT_USER,TEXT("software\\DocGuarder\\Client"),KEY_READ) == ERROR_SUCCESS)
		//if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Client"),KEY_READ) == ERROR_SUCCESS)
	{
		USES_CONVERSION;
		TCHAR keybuf[1024] = {0};
		DWORD dwLen = 1024;
		if (DocKey.QueryValue(keybuf,TEXT("username"),&dwLen) != ERROR_SUCCESS)
			return FALSE;

		char out[1024] = {0};
		DGCommonFunc::FromBase64((const unsigned char*)T2A(keybuf),strlen(T2A(keybuf)),out);

		//转换成UNICODE,为支持繁体
		WCHAR szTemp[1024]={0};
		MultiByteToWideChar( CP_UTF8, 0, out, -1, szTemp, sizeof(szTemp)/2);
		memcpy((char*)strUser, szTemp,wcslen(szTemp)*2);

		dwLen = 1024;
		if (DocKey.QueryValue(keybuf,TEXT("password"),&dwLen)  != ERROR_SUCCESS)
			return FALSE;
		memset(out,0,1024);
		DGCommonFunc::FromBase64((const unsigned char*)T2A(keybuf),strlen(T2A(keybuf)),out);
		wcscpy(strPass,A2W(out));

		DocKey.Close();
	}else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL DGCommonFunc::ReadRegData(LPTSTR strUser,LPTSTR strPass)
{
	CRegKey DocKey;
	
	if (DocKey.Open(HKEY_CURRENT_USER,TEXT("software\\DocGuarder\\Client"),KEY_WOW64_32KEY|KEY_READ) == ERROR_SUCCESS)
	{
		USES_CONVERSION;
		TCHAR keybuf[1024] = {0};
		DWORD dwLen = 1024;
		if (DocKey.QueryValue(keybuf,TEXT("username"),&dwLen) != ERROR_SUCCESS)
			return FALSE;

		char out[1024] = {0};
		DGCommonFunc::FromBase64((const unsigned char*)T2A(keybuf),strlen(T2A(keybuf)),out);
		_tcscpy(strUser,A2T(out));

		dwLen = 1024;
		if (DocKey.QueryValue(keybuf,TEXT("password"),&dwLen)  != ERROR_SUCCESS)
			return FALSE;
		memset(out,0,1024);
		DGCommonFunc::FromBase64((const unsigned char*)T2A(keybuf),strlen(T2A(keybuf)),out);
		_tcscpy(strPass,A2T(out));

		DocKey.Close();
	}else
	{
		if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Client"),KEY_WOW64_32KEY|KEY_READ) == ERROR_SUCCESS)
		{
			USES_CONVERSION;
			TCHAR keybuf[1024] = {0};
			DWORD dwLen = 1024;
			if (DocKey.QueryValue(keybuf,TEXT("username"),&dwLen) != ERROR_SUCCESS)
				return FALSE;

			char out[1024] = {0};
			DGCommonFunc::FromBase64((const unsigned char*)T2A(keybuf),strlen(T2A(keybuf)),out);
			_tcscpy(strUser,A2T(out));

			dwLen = 1024;
			if (DocKey.QueryValue(keybuf,TEXT("password"),&dwLen)  != ERROR_SUCCESS)
				return FALSE;
			memset(out,0,1024);
			DGCommonFunc::FromBase64((const unsigned char*)T2A(keybuf),strlen(T2A(keybuf)),out);
			_tcscpy(strPass,A2T(out));

			DocKey.Close();
		}else
		{
			return FALSE;
		}		
	}

	return TRUE;
}

BOOL DGCommonFunc::ReadRegServerData(LPTSTR strServerIP,DWORD& dwServerPort)
{
	CRegKey DocKey;

	if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Connect"),KEY_WOW64_32KEY|KEY_READ) == ERROR_SUCCESS)
	{
		DWORD dwLen = 1024;
		DocKey.QueryValue(strServerIP,TEXT("serverip"),&dwLen);

		DocKey.QueryValue(dwServerPort,TEXT("serverport"));
		DocKey.Close();
	}else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL DGCommonFunc::ReadRegLogServerData(LPTSTR strLogServerIP,DWORD& dwLogServerPort)
{
	CRegKey DocKey;

	if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Connect"),KEY_WOW64_32KEY|KEY_READ) == ERROR_SUCCESS)
	{
		DWORD dwLen = 1024;
		DocKey.QueryValue(strLogServerIP,TEXT("logserverip"),&dwLen);
		
		DocKey.QueryValue(dwLogServerPort,TEXT("logserverport"));
		DocKey.Close();
	}else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL DGCommonFunc::ReadRegServerData2(LPTSTR strServerIP2,DWORD& dwServerPort2)
{
	CRegKey DocKey;

	if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Connect"),KEY_WOW64_32KEY|KEY_READ) == ERROR_SUCCESS)
	{
		DWORD dwLen = 1024;
		DocKey.QueryValue(strServerIP2,TEXT("serverip2"),&dwLen);

		DocKey.QueryValue(dwServerPort2,TEXT("serverport2"));
		DocKey.Close();
	}else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL DGCommonFunc::ReadRegUpdateData(LPTSTR strUpdateIP,DWORD& dwUpdatePort)
{
	CRegKey DocKey;
	if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Connect"),KEY_WOW64_32KEY|KEY_READ) == ERROR_SUCCESS)
	{		
		DWORD dwLen = 1024;
		DocKey.QueryValue(strUpdateIP,TEXT("updateip"),&dwLen);

		DocKey.QueryValue(dwUpdatePort,TEXT("updateport"));

		DocKey.Close();
	}else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL DGCommonFunc::ReadRegUpdateData(DWORD& dwServerPort,DWORD& dwUpdatePort)
{
	CRegKey DocKey;
	if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Server"),KEY_WOW64_32KEY|KEY_READ) == ERROR_SUCCESS)
	{
		DocKey.QueryValue(dwServerPort,TEXT("serverport"));
		DocKey.QueryValue(dwUpdatePort,TEXT("updateport"));
		DocKey.Close();
	}else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL DGCommonFunc::WriteRegServerData(DWORD dwServerPort)
{
	CRegKey DocKey;
	if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder"),KEY_WOW64_32KEY|KEY_WRITE) != ERROR_SUCCESS)
	{
		DocKey.Create(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder"),REG_NONE,REG_OPTION_NON_VOLATILE,KEY_WRITE);
	}else
	{
		DocKey.Close();
	}

	if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Server"),KEY_WOW64_32KEY|KEY_WRITE) != ERROR_SUCCESS)
	{
		DocKey.Create(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Server"),REG_NONE,REG_OPTION_NON_VOLATILE,KEY_WRITE);
		DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Server"),KEY_WOW64_32KEY|KEY_WRITE);	
	}

	DocKey.SetValue(dwServerPort,TEXT("serverport"));
	DocKey.Close();

	return TRUE;
}

BOOL DGCommonFunc::WriteRegUpdateData(DWORD dwUpdatePort)
{
	CRegKey DocKey;
	if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder"),KEY_WOW64_32KEY|KEY_WRITE) != ERROR_SUCCESS)
	{
		DocKey.Create(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder"),REG_NONE,REG_OPTION_NON_VOLATILE,KEY_WRITE);
	}else
	{
		DocKey.Close();
	}

	if (DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Server"),KEY_WOW64_32KEY|KEY_WRITE) != ERROR_SUCCESS)
	{
		DocKey.Create(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Server"),REG_NONE,REG_OPTION_NON_VOLATILE,KEY_WRITE);
		DocKey.Open(HKEY_LOCAL_MACHINE,TEXT("software\\DocGuarder\\Server"),KEY_WOW64_32KEY|KEY_WRITE);	
	}

	DocKey.SetValue(dwUpdatePort,TEXT("updateport"));
	DocKey.Close();

	return TRUE;
}

BOOL DGCommonFunc::IsInExceptList(int* arrPolicy,int count,DWORD dwCryptLevel)
{
	for (int i=0;i<count;i++)
	{
		int nPolicy = arrPolicy[i];
		if (nPolicy == (int)dwCryptLevel)
			return TRUE;
	}
	return FALSE;
}

BOOL DGCommonFunc::IsUserCanAccess(int* arrPolicy,int count,DWORD dwCryptLevel,DWORD dwFileLevel)
{
	if (dwFileLevel == dwCryptLevel)
		return TRUE;
	for (int i=0;i<count;i++)
	{
		int nPolicy = arrPolicy[i];
		if (nPolicy == (int)dwFileLevel)
			return TRUE;
	}
	return FALSE;	
}

BOOL DGCommonFunc::IsUserCanAccess2(int* arrPolicy,int count,DWORD dwCryptLevel,DWORD dwFileLevel,int nPrivType)
{
	int type = 0;
	for (int i=0;i<count;i++)
	{
		if (type == nPrivType)
		{
			if (type == 0 && dwCryptLevel == dwFileLevel) //文件打开权限,只要同等级就行了
			{
				return TRUE;
			}

			if (arrPolicy[i] == 0xFFFFFFFF)
			{
				type++;
				if (type > nPrivType)
					break;
			}

			if (dwFileLevel == arrPolicy[i])			  //有权限
				return TRUE;
		}else
		{
			if (arrPolicy[i] == 0xFFFFFFFF)
			{
				type++;
			}
		}
	}
	return FALSE;
}

typedef   struct   tagMAC_ADDRESS   
{   
	BYTE   b1,b2,b3,b4,b5,b6;   
}MAC_ADDRESS,*LPMAC_ADDRESS;   

typedef   struct   tagASTAT     
{     
	ADAPTER_STATUS   adapt;     
	NAME_BUFFER         NameBuff   [30];     
}ASTAT,*LPASTAT;     

UCHAR   GetAddressByIndex(int   lana_num,   ASTAT   &Adapter)   
{   
	NCB   ncb;     
	UCHAR   uRetCode;     
	memset(&ncb,   0,   sizeof(ncb)   );     
	ncb.ncb_command   =   NCBRESET;     
	ncb.ncb_lana_num   =   lana_num;     
	//指定网卡号,首先对选定的网卡发送一个NCBRESET命令,以便进行初始化     
	uRetCode   =   Netbios(&ncb   );     
	memset(&ncb,   0,   sizeof(ncb)   );     
	ncb.ncb_command   =   NCBASTAT;     
	ncb.ncb_lana_num   =   lana_num;       //指定网卡号     
	strcpy((char   *)ncb.ncb_callname,"*             "   );     
	ncb.ncb_buffer   =   (unsigned   char   *)&Adapter;     
	//指定返回的信息存放的变量     
	ncb.ncb_length   =   sizeof(Adapter);     
	//接着,可以发送NCBASTAT命令以获取网卡的信息     
	uRetCode   =   Netbios(&ncb   );     
	return   uRetCode;   
}     

#ifndef NO_MFC_VERSION
int   GetMAC(LPMAC_ADDRESS   pMacAddr)   
{   
	NCB   ncb;     
	UCHAR   uRetCode;   
	int   num   =   0;   
	LANA_ENUM   lana_enum;     
	memset(&ncb,   0,   sizeof(ncb)   );     
	ncb.ncb_command   =   NCBENUM;     
	ncb.ncb_buffer   =   (unsigned   char   *)&lana_enum;     
	ncb.ncb_length   =   sizeof(lana_enum);     
	////向网卡发送NCBENUM命令,以获取当前机器的网卡信息,如有多少个网卡   
	////每张网卡的编号等     
	uRetCode   =   Netbios(&ncb);   
	if   (uRetCode   ==   0)     
	{   
		num   =   lana_enum.length;   
		//对每一张网卡,以其网卡编号为输入编号,获取其MAC地址     
		for   (int   i   =   0;   i   <   num;   i++)   
		{   
			ASTAT   Adapter;   
			if(GetAddressByIndex(lana_enum.lana[i],Adapter)   ==   0)   
			{   
				pMacAddr[i].b1   =   Adapter.adapt.adapter_address[0];   
				pMacAddr[i].b2   =   Adapter.adapt.adapter_address[1];   
				pMacAddr[i].b3   =   Adapter.adapt.adapter_address[2];   
				pMacAddr[i].b4   =   Adapter.adapt.adapter_address[3];   
				pMacAddr[i].b5   =   Adapter.adapt.adapter_address[4];   
				pMacAddr[i].b6   =   Adapter.adapt.adapter_address[5];   
			}   
		}   
	}   
	return   num;   
}   

CAString DGCommonFunc::GetMacByNetBIOS()
{
	MAC_ADDRESS   m_MacAddr[10];                 //   比如最多10个网卡   
	int   n   =   GetMAC(m_MacAddr);                 //   获得网卡数量   
    
	CAString strMac;
	strMac.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"),   
		m_MacAddr[0].b1,m_MacAddr[0].b2,   
		m_MacAddr[0].b3,m_MacAddr[0].b4,   
		m_MacAddr[0].b5,m_MacAddr[0].b6);
	
	return strMac;         
}

BOOL DGCommonFunc::GetMacByCmd( CAString &strMac )
{
	const long MAX_COMMAND_SIZE = 10000;
	//获取MAC的命令行
	TCHAR szFetCmd[] = TEXT("ipconfig /all");
	//网卡MAC地址的前导信息
	const CAString str4Search = L"Physical Address. . . . . . . . . : ";

	strMac = "";
	BOOL bRet;

	SECURITY_ATTRIBUTES sa;
	HANDLE hReadPipe , hWritePipe;

	sa.nLength = sizeof( SECURITY_ATTRIBUTES );
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//创建管道

	bRet = CreatePipe( &hReadPipe , &hWritePipe , &sa , 0 );
	if( !bRet )
	{
	return false;
	} 

	//控制命令行窗口信息
	STARTUPINFO si;
	//返回进程信息
	PROCESS_INFORMATION pi;

	si.cb = sizeof( STARTUPINFO );
	GetStartupInfo( &si );
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	//隐藏命令行窗口
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//创建获取命令行进程

	bRet = CreateProcess( NULL , szFetCmd , NULL , NULL , TRUE , 0 , NULL , NULL , &si , &pi );
	//放置命令行输出缓冲区
	char szBuffer[ MAX_COMMAND_SIZE + 1 ];
	CAString strBuffer;
	if( bRet )
	{
	WaitForSingleObject( pi.hProcess , INFINITE );
	unsigned long count;
	CloseHandle( hWritePipe );

	memset( szBuffer , 0x00 , sizeof( szBuffer ) );

	bRet = ReadFile( hReadPipe , szBuffer , MAX_COMMAND_SIZE , &count , 0 ); 

	if( !bRet )
	{
	//关闭所有的句柄
	CloseHandle( hWritePipe );
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
	CloseHandle( hReadPipe );
	return false;
	}
	else
	{
	strBuffer = szBuffer;
	long ipos;

	ipos = strBuffer.Find( str4Search );
	strBuffer = strBuffer.Right( strBuffer.GetLength() - str4Search.GetLength() - ipos );
	strBuffer = strBuffer.Left( 17 );

	strMac = strBuffer;
	for( int i = 0 ; i < strBuffer.GetLength() ; i++ )
	{
	if( strBuffer.GetAt( i ) != '-' )
	{
//	strMac.AppendChar( strBuffer.GetAt( i ) );
	}
	}

	return true;
	}
	}
	else
	{
	return false;
	}
}

CAString DGCommonFunc::GetHardDiskSerial()
{
/*	char szSystemInfo[1024] = {0};
	UINT uSystemInfoLen = 0;
	OSVERSIONINFO ovi = { 0 };
    ovi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
    GetVersionEx( &ovi );
    
    if( ovi.dwPlatformId != VER_PLATFORM_WIN32_NT )
    {
        // Only Windows 2000, Windows XP, Windows Server 2003...
        return "";
    }
    else
    {
        if( !WinNTHDSerialNumAsPhysicalRead((LPBYTE)szSystemInfo, &uSystemInfoLen, 1024 ) )
        {
            WinNTHDSerialNumAsScsiRead( (LPBYTE)szSystemInfo, &uSystemInfoLen, 1024 );
        }
    }
	
	CAString strHardID = szSystemInfo;
	
	for (int i=0;i<1024;i++)
	{
		if (szSystemInfo[i] != ' ')
			break;
	}
	strHardID = szSystemInfo+i;
	int pos = strHardID.Find(TEXT(" "),0);
	if (pos != -1)
		strHardID = strHardID.Left(pos);
	return strHardID;*/
	CAString strSerialOut = L"";
#ifdef CALC_SERIAL
	DWORD dwSerial = 0;
	
	CAString strGUID = "";
	if (dwSerial == 0)
	{
		GetVolumeInformation(_T("c:\\"),NULL,0,&dwSerial,0,NULL,NULL,0);
	}

	BYTE out[64] = {0};
	memset(out,0,64);

	if (strGUID.IsEmpty())
	{
		sprintf((char*)out,"%x",dwSerial);
	}else
	{
		sprintf((char*)out,"%x-%s",dwSerial,strGUID);
	}
	
	
	CCryp crypt;
	
	crypt.MD5Update((LPBYTE)out,strlen((char*)out));
	memset(out,0,64);
	crypt.MD5Final(out);

	int size = strlen((char*)out);
	for (int i=0;i<size;i++)
	{
		CAString strNum;
		strNum.Format(_T("%x"),out[i]);
		strSerialOut += strNum;
	}
	strSerialOut.MakeUpper();
#endif
	return strSerialOut;
}

#endif