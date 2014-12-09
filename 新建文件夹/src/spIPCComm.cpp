#include "stdafx.h"
//#include "winsock.h"
#include "Winsock2.h"
#include "spIPCComm.h"

using namespace commIPC;
/*

通讯类，by wenyy 2002-02-01
封装了通信类的读写操作，(类不是线程安全的)
v.01 支持Socket
v.02 支持NamedPipe
通信类中主要封装了通信读写操作
所有类功能的说明见CIPCComm定义
v.01 socket communication

2002-06-25
by wenyy
将文件编译成为DLL 形式
同时引出相关的全局函数

  socket create function :
static SOCKET CreateListenSocket(LPCSTR pszLocalAddr,UINT uPort)
static SOCKET CreateConnectSocket(LPCSTR pszRemoteAddr,int iPort,LPCSTR pszLocalAddr)

  socket read/write function :
virtual int Recv(int iWant,BYTE* pbBuf,int& iRead);
virtual int Send(int iWant,const BYTE *pbBuf,int& iWrote);

  socket test function :
virtual int TestRead(int iTimeOut);
virtual int TestConnect(int iTimeOut);

*/
extern "C" SOCKET SPCreateUDPSocket(int iLocalPort,LPCSTR pszLocalAddr)
{
	SOCKET sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock == INVALID_SOCKET)
		return INVALID_SOCKET ;
	//bind
	SOCKADDR_IN sinLocal;
	sinLocal.sin_family=AF_INET;
	sinLocal.sin_port=htons(iLocalPort);
	if(pszLocalAddr )
	{
		LONG lAddr= inet_addr(pszLocalAddr);
		memcpy((LPSTR)&(sinLocal.sin_addr),&lAddr,sizeof(lAddr));
	}
	else
	{
		LONG lAddr= INADDR_ANY;
		memcpy(&(sinLocal.sin_addr),&lAddr,sizeof(lAddr));
	}
	int iStatus=bind(sock,(struct sockaddr FAR*)&sinLocal,sizeof(sinLocal));

	if(iStatus==SOCKET_ERROR)
	{
		closesocket(sock);
		return INVALID_SOCKET ;
	}
   
	return sock;
}

BOOL GetConnectSocketInfo(SOCKADDR_IN &sinRemote,LPCSTR pszRemoteAddr,int iPort,LPCSTR pszLocalAddr)
{
	struct hostent *remoteHost;
  
	struct in_addr addr;
    if (isalpha(pszRemoteAddr[0])) {        
      
        remoteHost = gethostbyname(pszRemoteAddr);
    } else {

        addr.s_addr = inet_addr(pszRemoteAddr);
        if (addr.s_addr == INADDR_NONE) {
            return FALSE;
        } else
            remoteHost = gethostbyaddr((char *) &addr, 4, AF_INET);
    }
	if (remoteHost == NULL) {
        DWORD dwError = WSAGetLastError();
        if (dwError != 0) {
            if (dwError == WSAHOST_NOT_FOUND) {
                return FALSE;
            } else if (dwError == WSANO_DATA) {
                return FALSE;
            } else {
                return FALSE;
            }
        }
    } else {
        switch (remoteHost->h_addrtype) {
        case AF_INET:
            break;
        case AF_INET6:
            break;
        case AF_NETBIOS:
            break;
        default:
            break;
        }
        addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
    }

	memcpy(&sinRemote.sin_addr,&addr,sizeof(in_addr));

	return TRUE;
}

BOOL IsValidIPAddress(LPCSTR strIP)
{
	int len = strlen(strIP);
	int count = 0;
	for (int i=0;i<len;i++)
	{
		if (strIP[i] == '.')
		{
			count ++ ;
		}else
		{
			if (strIP[i] < '0' || strIP[i] > '9')
			{
				return FALSE;
			}
		}
	}
	if (count != 3)
	{
		return FALSE;
	}
	return TRUE;
}

extern "C" SOCKET SPCreateConnectSocket(LPCSTR pszRemoteAddr,int iPort,LPCSTR pszLocalAddr)
{
	SOCKET sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock == INVALID_SOCKET)
		return INVALID_SOCKET;
	 
// 	int TimeOut=6000;   //设置发送超时6秒   
// 	if(::setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
// 	{   
// 		return INVALID_SOCKET;   
// 	}   

	BOOL bValue = TRUE;
	::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bValue, sizeof(BOOL));
// 	TimeOut=6000;		//设置接收超时6秒   
// 	if(::setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
// 	{   
// 		return INVALID_SOCKET;   
// 	}   

	//设置非阻塞方式连接   
	unsigned long ul = 1;   
	int ret = ioctlsocket(sock, FIONBIO, (unsigned long*)&ul);   
	if(ret==SOCKET_ERROR)
		return INVALID_SOCKET;   
	
	//connect
	SOCKADDR_IN sinRemote;
	sinRemote.sin_family=AF_INET;
	sinRemote.sin_port=htons(iPort);
	if (IsValidIPAddress(pszRemoteAddr))
	{
		LONG lAddr= inet_addr(pszRemoteAddr);
		memcpy((LPSTR)&(sinRemote.sin_addr),&lAddr,sizeof(lAddr));
	}else
	{
		if ( !GetConnectSocketInfo(sinRemote,pszRemoteAddr,iPort,pszLocalAddr))
		{
			LONG lAddr= inet_addr(pszRemoteAddr);
			memcpy((LPSTR)&(sinRemote.sin_addr),&lAddr,sizeof(lAddr));
		}
	}

	int iStatus = connect(sock,(struct sockaddr FAR*)&sinRemote,sizeof(sinRemote));

	struct timeval timeout   ; 
	fd_set r; 
	
	FD_ZERO(&r); 
	FD_SET(sock, &r); 
	timeout.tv_sec = 3;   //连接超时5秒 
	timeout.tv_usec =0; 
	ret = select(0, 0, &r, 0, &timeout); 
	if( ret <= 0 ) 
	{ 
		::closesocket(sock); 
		return INVALID_SOCKET; 
	} 

	unsigned   long   ul1=   0   ; 
	ret = ioctlsocket(sock,   FIONBIO,   (unsigned   long*)&ul1); 
	if(ret == SOCKET_ERROR)
	{ 
		::closesocket(sock); 
		return INVALID_SOCKET; 
	}

	return sock;
}

extern "C" SOCKET SPCreateAcceptSocket(SOCKET sockListen,char* pszHost,int *piPort)
{
	SOCKADDR_IN sinRemote;
	int iLen=sizeof(sinRemote);
	SOCKET sComm = accept(sockListen,(struct sockaddr *)&sinRemote,&iLen);
	if(sComm != INVALID_SOCKET)
	{
		if(pszHost)
			strcpy(pszHost,inet_ntoa(sinRemote.sin_addr));
		if(piPort)
			*piPort = ntohs(sinRemote.sin_port);
	}

	BOOL bValue = TRUE;
	::setsockopt(sComm, IPPROTO_TCP, TCP_NODELAY, (char*)&bValue, sizeof(BOOL));

	return sComm;
}

extern "C" SOCKET SPCreateListenSocket(LPCSTR pszLocalAddr,UINT uPort)
{
	SOCKET sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==INVALID_SOCKET)
		return INVALID_SOCKET ;
	//bind
	SOCKADDR_IN sinLocal;
	sinLocal.sin_family=AF_INET;
	sinLocal.sin_port=htons(uPort);
	if(pszLocalAddr)
	{
		LONG lAddr= inet_addr(pszLocalAddr);
		memcpy((LPSTR)&(sinLocal.sin_addr),&lAddr,sizeof(lAddr));
	}
	else
	{
		LONG lAddr= INADDR_ANY;
		memcpy(&(sinLocal.sin_addr),&lAddr,sizeof(lAddr));
	}
	int iStatus=bind(sock,(struct sockaddr FAR*)&sinLocal,sizeof(sinLocal));

	if(iStatus==SOCKET_ERROR)
	{
		closesocket(sock);
		return INVALID_SOCKET ;
	}
	iStatus=listen(sock,100);
	if(SOCKET_ERROR==iStatus)
	{
		closesocket(sock);
		return INVALID_SOCKET ;
	}
	return sock;
}

/////////////////////////////////
//  socket implement
/////////////////////////////////
CIPCSocket::CIPCSocket(SOCKET sockComm,BOOL fAutoDel)
{
//	ASSERT(sockComm !=0);
//	ASSERT(sockComm != INVALID_SOCKET);
	m_fAutoDel=fAutoDel;
	m_sockComm = sockComm;
}

CIPCSocket::~CIPCSocket()
{
	if(m_fAutoDel)
	{
		closesocket(m_sockComm);
	}
}

int CIPCSocket::Recv(const int iWant,BYTE* pbBuf,int& iRead)
{
	int iRecved=0;
	iRead = 0;
	while(1)
	{
		int iLeft = iWant -iRecved;
//		ASSERT(iLeft != 0);
		int iRet = recv(m_sockComm,(char*)pbBuf+iRecved,iLeft,0);
		if(iRet ==0 || iRet == SOCKET_ERROR  )
			return SP_ERR_NETWORK;
		iRecved += iRet;
		iRead = iRecved;
		if(iRecved == iWant)
		{
			return SP_ERR_SUCCESS;
		}
	}
	return SP_ERR_NETWORK;
}

int CIPCSocket::Send(const int iWant,const BYTE *pbBuf,int& iWrote)
{
	int iSent=0;
	iWrote = 0;
	while(1)
	{
		int iLeft = iWant -iSent;
//		ASSERT(iLeft != 0);
		int iRet = send(m_sockComm,(char*)pbBuf+iSent,iLeft,0);
		if(iRet ==0 || iRet == SOCKET_ERROR  )
			return SP_ERR_NETWORK;
		iSent += iRet;
		iWrote = iSent;
		if(iSent == iWant)
			return SP_ERR_SUCCESS;
	}
	return SP_ERR_NETWORK;
}

int CIPCSocket::TestRead(int iTimeOut)
{
	struct timeval tvLeft={iTimeOut,0},tvExp={0,0};
	fd_set fdExp;
	FD_ZERO(&fdExp);
	FD_SET(m_sockComm,&fdExp);
	int iRet = select(1,NULL,NULL,&fdExp,&tvExp);
	switch (iRet)
	{
	case SOCKET_ERROR :
		return SP_ERR_NETWORK;
		break;
	case 1:
		return SP_ERR_NETWORK;
		break;
	}

	fd_set fdRecv;
	FD_ZERO(&fdRecv);
	FD_SET(m_sockComm,&fdRecv);
	iRet = select(1,&fdRecv,NULL,NULL,&tvLeft);
	switch (iRet)
	{
	case SOCKET_ERROR :
		return SP_ERR_NETWORK;
		break;
	case 0 :
		return SP_ERR_NODATA;
		break;
	case 1:
		return SP_ERR_SUCCESS;
		break;
	default:
		return SP_ERR_GENERAL;
		break;
	}
}
int CIPCSocket::TestConnect(int iTimeOut)
{
	return TestRead(iTimeOut);
	/*struct timeval tvLeft={iTimeOut,0};
	fd_set fdRecv;
	FD_ZERO(&fdRecv);
	FD_SET(m_sockComm,&fdRecv);
	int iRet = select(1,&fdRecv,NULL,NULL,&tvLeft);
	switch (iRet)
	{
	case SOCKET_ERROR :
		return SP_ERR_NETWORK;
		break;
	case 0 :
		return SP_ERR_NODATA;
		break;
	case 1:
		return SP_ERR_SUCESS;
		break;
	default:
		return SP_ERR_GENERAL;
		break;
	}*/
}

int CIPCSocket::RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)
{
	DWORD dwTick1 = GetTickCount(); 
	int iRecved=0,iLeft = iWant;
	iRead = 0;
	while(1)
	{
		int iTest = TestRead(1);
		if(SP_ERR_SUCCESS == iTest)
		{
			int iRet = recv(m_sockComm,(char*)pbBuf+iRecved,iLeft,0);
			if(iRet == SOCKET_ERROR ||iRet ==0)
			{
				return SP_ERR_NETWORK;
			}
			iRecved += iRet;
			iRead = iRecved;
			iLeft = iWant - iRecved;
		}
		else if(iTest == SP_ERR_NETWORK)
		{
			return SP_ERR_NETWORK;
		}

		if(iRead == iWant)
			return  SP_ERR_SUCCESS;

		DWORD dwTick2= GetTickCount();
		if(dwTick2-dwTick1 > (DWORD)iTimeout*1000)
		{//检查是否超时
			if (iRead ==0)
			{
				return SP_ERR_NODATA;
			}
			else
			{
				return SP_ERR_NOT_FINISH;
			}
		}
	}

	return SP_ERR_GENERAL;
}

int CIPCSocket::TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)
{
	int iRet = TestRead(iTimeout);
	iRead = 0;
	if(iRet == SP_ERR_SUCCESS)
	{
		iRet = recv(m_sockComm,(char*)pbBuf,iWant,0);
		if(iRet ==SOCKET_ERROR || iRet ==0)
			return SP_ERR_NETWORK;

		iRead = iRet;
		if(iRead ==0)
			return SP_ERR_NODATA;
		else
		{
			if(iRead == iWant)
				return SP_ERR_SUCCESS;
			else
				return SP_ERR_NOT_FINISH;
		}
	}
	return iRet;
}
///////////////////////////////////
// UDP implement
///////////////////////////////////
CUDPSocket::CUDPSocket(SOCKET sockComm,BOOL fAutoDel):CIPCSocket(sockComm,fAutoDel)
{
	m_uMaxUDP = 64;
	int iLen=sizeof(m_uMaxUDP);
	if(SOCKET_ERROR == getsockopt(sockComm,SOL_SOCKET,SO_MAX_MSG_SIZE,(char*)&m_uMaxUDP,&iLen) )
		m_uMaxUDP = 64;
}
int CUDPSocket::SendTo(const int iWant,const BYTE* pbBuf,int& iWrote,LPCSTR pszHost,const int iPort)
{
	iWrote = 0;
//	if(iWant > (int)m_uMaxUDP)
//		return SP_UDP_TOOLONG;
	SOCKADDR_IN sinRemote;
	sinRemote.sin_family=AF_INET;
	sinRemote.sin_port=htons(iPort);
	LONG lAddr= inet_addr(pszHost);
	memcpy((LPSTR)&(sinRemote.sin_addr),&lAddr,sizeof(lAddr));
	int iStatus = sendto(m_sockComm,(const char*)pbBuf,iWant,0,(struct sockaddr FAR*)&sinRemote,sizeof(sinRemote));
	if(iStatus ==SOCKET_ERROR)
	{
		if(WSAEMSGSIZE == GetLastError())
			return SP_UDP_TOOLONG;
		else
			return SP_ERR_GENERAL;
	}
	iWrote = iStatus;
	return SP_ERR_SUCCESS;

}
int CUDPSocket::RecvFrom(const int iWant,const BYTE* pbBuf,int& iRead,char* pszHost,int *piPort)
{
	iRead = 0;
	SOCKADDR_IN sinRemote;
	int iLen = sizeof(sinRemote);
	int iStatus = recvfrom(m_sockComm,(char*)pbBuf,iWant,0,(struct sockaddr FAR*)&sinRemote,&iLen);
	if(SOCKET_ERROR == iStatus)
	{
		if(WSAEMSGSIZE == GetLastError())
			return SP_UDP_SMALLBUF;
		else
			return SP_ERR_GENERAL;
	}
	iRead = iStatus;
	if(pszHost)
		strcpy(pszHost,inet_ntoa(sinRemote.sin_addr));
	if(piPort)
		*piPort = ntohs(sinRemote.sin_port);
	return SP_ERR_SUCCESS;
}

/////////////////////////////
//  named pipe implement
/////////////////////////////
extern "C" HANDLE SPCreateServerPipe(LPCSTR pszPipeName,int iTimeout)
{
//	ASSERT(iTimeout);
	HANDLE hP = CreateNamedPipeA(pszPipeName,PIPE_ACCESS_DUPLEX,
								PIPE_TYPE_BYTE|PIPE_WAIT,PIPE_UNLIMITED_INSTANCES,
								1024,1024,
								iTimeout*1000,NULL);
	if( INVALID_HANDLE_VALUE != hP)
	{
		if( ConnectNamedPipe(hP,NULL) )
			return hP;
		else
			CloseHandle(hP);
	}
	return INVALID_HANDLE_VALUE;
}

extern "C" HANDLE SPCreateClientPipe(LPCSTR pszPipeName,int iTimeout)
{
	if(WaitNamedPipeA(pszPipeName,iTimeout*1000))
	{
		//create a no share,read/write	
		HANDLE hP=CreateFileA(pszPipeName,GENERIC_WRITE|GENERIC_READ,
								0,NULL,
								CREATE_NEW,0,
								NULL);
		return hP;
	}
	return INVALID_HANDLE_VALUE;
}

CIPCNamedPipe::CIPCNamedPipe(HANDLE hPipe,BOOL fAutoDel)
{
//	ASSERT(hPipe !=NULL);
	m_fAutoDel=fAutoDel;
	m_hPipe = hPipe;
}

CIPCNamedPipe::~CIPCNamedPipe()
{
	if(m_fAutoDel)
	{
		CloseHandle(m_hPipe);
	}
}


int CIPCNamedPipe::Recv(const int iWant,BYTE* pbBuf,int& iRead)
{
	iRead=0;
	while(1)
	{
		DWORD dwRead;
		BOOL fRet = ReadFile(m_hPipe,pbBuf,iWant-iRead,&dwRead,NULL);
		if(fRet)
		{
			iRead += dwRead;
			if(iRead == iWant)
				return SP_ERR_SUCCESS;
		}
		else
			return SP_ERR_NETWORK;
	}
}

int CIPCNamedPipe::Send(const int iWant,const BYTE *pbBuf,int& iWrote)
{
	iWrote=0;
	while(1)
	{
		DWORD dwWrote;
		BOOL fRet = WriteFile(m_hPipe,pbBuf,iWant - iWrote,&dwWrote,NULL);
		if(fRet)
		{
			iWrote += dwWrote;
			if(iWrote ==iWant)
				return SP_ERR_SUCCESS;
		}
		else
			return SP_ERR_NETWORK;
	}
}

int CIPCNamedPipe::TestRead(int iTimeOut)
{
	//return SP_ERR_NOT_IMPLEMENT;
	DWORD dwRead,dwTotal,dwLeft;
	DWORD dwT1=GetTickCount();

	while(1)
	{
		BOOL fRet = PeekNamedPipe(m_hPipe,NULL,0,
									&dwRead,&dwTotal,&dwLeft);
		if (!fRet)
			return SP_ERR_NETWORK;
		if(dwTotal)
			return SP_ERR_SUCCESS;
		DWORD dwT2 = GetTickCount();
		if(dwT2-dwT1 > (DWORD)iTimeOut*1000)
			return SP_ERR_NODATA;
		Sleep(500);
	}
	return SP_ERR_NODATA;
}
int CIPCNamedPipe::TestConnect(int iTimeOut)
{
	return SP_ERR_NOT_IMPLEMENT;
}

int CIPCNamedPipe::RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)
{
	iRead =0;
	DWORD dwTick1 = GetTickCount();
	int iRecved=0,iLeft = iWant;
	iRead = 0;
	while(1)
	{
		int iTest = TestRead(1);
		if(SP_ERR_SUCCESS == iTest)
		{
			DWORD dwRead,dwTotal,dwLeftQueue;
			BOOL fRet = PeekNamedPipe(m_hPipe,NULL,0,
										&dwRead,&dwTotal,&dwLeftQueue);
			DWORD dwWantRead = (dwTotal>(DWORD)iLeft)?iLeft:dwTotal;
			fRet = ReadFile(m_hPipe,pbBuf,dwWantRead,&dwRead,NULL);
			if(fRet == FALSE)
				return SP_ERR_NETWORK;
			iRecved += dwRead;
			iRead = iRecved;
			iLeft = iWant - iRecved;
		}
		else if(iTest != SP_ERR_NODATA)
			return iTest;

		if(iRead == iWant)
			return  SP_ERR_SUCCESS;

		DWORD dwTick2= GetTickCount();
		if(dwTick2-dwTick1 > (DWORD)iTimeout*1000)
		{//检查是否超时
			if (iRead ==0)
				return SP_ERR_NODATA;
			else
				return SP_ERR_NOT_FINISH;
		}
	}
	return SP_ERR_GENERAL;
}

int CIPCNamedPipe::TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)
{
	iRead =0;
	int iTest = TestRead(iTimeout);
	if (iTest != SP_ERR_SUCCESS)
		return iTest;
	else
	{
		DWORD dwRead,dwTotal,dwLeftQueue;
		BOOL fRet = PeekNamedPipe(m_hPipe,NULL,0,
									&dwRead,&dwTotal,&dwLeftQueue);
		DWORD dwWantRead = (dwTotal>(DWORD)iWant)?iWant:dwTotal;
		fRet = ReadFile(m_hPipe,pbBuf,dwWantRead,&dwRead,NULL);
		iRead= dwRead;
		if(fRet)
		{
			if(dwRead ==0)
				return SP_ERR_NODATA;
			else if( dwRead == (DWORD)iWant)
				return SP_ERR_SUCCESS;
			else
				return SP_ERR_NOT_FINISH;
		}
		else
			return SP_ERR_NETWORK;
	}
	return SP_ERR_GENERAL;
}
