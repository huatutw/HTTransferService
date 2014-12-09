#ifndef _SP_COMM_H_
#define _SP_COMM_H_

#include <winsock2.h>

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
将文件编译成为DLL 形式，同时引出相关的全局函数，但建议使用相关类中的静态成员函数
添加UDP支持
添加两个新类：CTCPSocket，CUDPSocket，分别支持TCP和UDP协议
由于UDP包的最大限制关系所以在发送UDP包时需要检测包的最大尺寸
*/

//使用下面的代码来引用DLL
/*
#if defined(_DEBUG)
	#pragma comment(lib, "commIPC_D.lib")
#else
	#pragma comment(lib, "commIPC.lib")
#endif
*/

namespace commIPC

//定义错误类型
//成功
#define SP_ERR_SUCCESS			0
//网络错误
#define SP_ERR_NETWORK			1
//没有完成读或写,只读入或写出部分要求要求数据
#define SP_ERR_NOT_FINISH		2
//超过时间
#define SP_ERR_TIMEOUT			3
//无数据
#define SP_ERR_NODATA			4
//UDP包长度超过系统限制
#define SP_UDP_TOOLONG			5
//UDP接收时缓冲区长度太小
#define SP_UDP_SMALLBUF			6
//未实现该调用
#define SP_ERR_NOT_IMPLEMENT	98
//一般错误
#define SP_ERR_GENERAL			99

{//定义名字空间

//pszLocalAddr为本地所使用的IP地址,如果为NULL,表明使用任意IP地址,系统自动选择
//pszLocalAddr需要绑定的本地地址，如果为NULL表明所有地址
//返回INVALID_SOCKET表明失败
extern "C" SOCKET SPCreateListenSocket(LPCSTR pszLocalAddr,UINT uPort);
//创建客户端SOCKET
//pszLocalAddr需要绑定的本地地址，如果为NULL表明所有地址
//返回INVALID_SOCKET表明失败
extern "C" SOCKET SPCreateConnectSocket(LPCSTR pszRemoteAddr,int iPort,LPCSTR pszLocalAddr=NULL);
//服务器放建立与客户端的连接
//pszHost,iPort用于得到对方主机的地址和端口
extern "C" SOCKET SPCreateAcceptSocket(SOCKET sockListen,char* pszHost=NULL,int *piPort=NULL);
//创建UDP Socket，对于接收方必须指明端口，发送方可以不指明
//pszLocalAddr 本地与Socket绑定的地址
extern "C" SOCKET SPCreateUDPSocket(int iLocalPort=0,LPCSTR pszLocalAddr=NULL);

class CIPCComm
{
public :
	CIPCComm(){};
	~CIPCComm(){};
public:
	//下面的所有函数都可能返回 SP_ERR_GENERAL:表示发生未知错误

	//读:直到完全读入数据，或是错误发生
	//返回：SP_ERR_SUCCESS，SP_ERR_NETWORK,SP_ERR_GENERAL
	//iWant:需要读入的字节数
	//iRead:返回实际读入的字节数
	virtual int Recv(const int iWant,BYTE* pbBuf,int& iRead)=0;

	//写:直到完全写出数据，或是错误发生
	//返回：SP_ERR_SUCCESS，SP_ERR_NETWORK,SP_ERR_GENERAL
	//iWant:需要写出的字节数
	//iWrote:返回实际写出的字节数
	virtual int Send(const int iWant,const BYTE *pbBuf,int& iWrote)=0;

	//测试是否有数据到达
	//返回：SP_ERR_SUCCESS(有数据到达)，SP_ERR_NETWORK，SP_ERR_NODATA,SP_ERR_GENERAL
	//iTimeOut: 超时时间，秒
	virtual int TestRead(int iTimeOut)=0;

	//测试是否有连接到达
	//返回：SP_ERR_SUCCESS(有数据到达)，SP_ERR_NETWORK，SP_ERR_NODATA（无连接到达）,SP_ERR_GENERAL
	//iTimeOut: 超时时间，秒
	virtual int TestConnect(int iTimeOut)=0;

	//在规定时间内读入要求数量的数据,尽可能多的读入数据
	//返回SP_ERR_SUCCESS，SP_ERR_NETWORK，SP_ERR_NOT_FINISH（超时，只读入部分数据），SP_ERR_NODATA（未读入任何数据）,SP_ERR_GENERAL
	virtual int RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)=0;

	//检测数据并读入，在检测到数据后只进行一次读操作，
	//返回：SP_ERR_SUCCESS（读入要求数据），SP_ERR_NETWORK，SP_ERR_NOT_FINISH（检测到数据，但只读入部分数据），SP_ERR_NODATA（超时，未读入任何数据）,SP_ERR_GENERAL
	virtual int TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)=0;
};

class CIPCSocket: public CIPCComm
{
public:
	//fAutoDel 是否在删除对象时自动关闭Socket
	CIPCSocket(SOCKET sockComm,BOOL fAutoDel=FALSE);
	~CIPCSocket();
	SOCKET GetSocket(void){return m_sockComm;};
	SOCKET DetachSocket(void){SOCKET s=m_sockComm;m_sockComm=INVALID_SOCKET;return s;};
public:
	//创建Socket helper function
	static SOCKET CreateListenSocket(LPCSTR pszLocalAddr,UINT uPort)
	{
		return SPCreateListenSocket(pszLocalAddr,uPort);
	}
	static SOCKET CreateConnectSocket(LPCSTR pszRemoteAddr,int iPort,LPCSTR pszLocalAddr)
	{
		return SPCreateConnectSocket( pszRemoteAddr, iPort, pszLocalAddr);
	}
	static SOCKET CreateUDPSocket(int iLocalPort=0,LPCSTR pszLocalAddr=NULL)
	{
		return SPCreateUDPSocket(iLocalPort,pszLocalAddr);
	}
	static SOCKET CreateAcceptSocket(SOCKET sockListen,char* pszHost=NULL,int *piPort=NULL)
	{
		return SPCreateAcceptSocket(sockListen,pszHost,piPort);
	}
	//
	virtual int Recv(const int iWant,BYTE* pbBuf,int& iRead);
	virtual int Send(const int iWant,const BYTE *pbBuf,int& iWrote);
	virtual int TestRead(int iTimeOut);
	virtual int TestConnect(int iTimeOut);
	static int TestRead(SOCKET sock,int iTimeOut)
	{
		CIPCSocket sTemp(sock,FALSE);
		return sTemp.TestRead(iTimeOut);
	};
	static int TestConnect(SOCKET sock,int iTimeOut)
	{
		CIPCSocket sTemp(sock,FALSE);
		return sTemp.TestConnect(iTimeOut);
	};
	virtual int RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
	virtual int TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
protected:
	SOCKET m_sockComm;
	BOOL m_fAutoDel;
};
//UDP通信类
class CUDPSocket: public CIPCSocket
{
public:
	CUDPSocket(SOCKET sockComm,BOOL fAutoDel=FALSE);
	//取消原有发送、接收函数调用
	virtual int Recv(const int iWant,BYTE* pbBuf,int& iRead)
	{return SOCKET_ERROR;};
	virtual int Send(const int iWant,const BYTE *pbBuf,int& iWrote)
	{return SOCKET_ERROR;};
	virtual int TestConnect(int iTimeOut)
	{return SOCKET_ERROR;};
	virtual int RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)
	{return SOCKET_ERROR;};
	virtual int TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)
	{return SOCKET_ERROR;};
	//允许的函数
	//发送数据：当发送数据长度过大，时返回：SP_UDP_TOOLONG
	//pszHost,iPort用于设置对方主机的地址和端口
	virtual int SendTo(const int iWant,const BYTE* pbBuf,int& iWrote,LPCSTR pszHost,const int iPort);
	//接收数据，iWant为缓冲区最大长度，如果在接收时该长度过小，会返回：SP_UDP_SMALLBUF
	//pszHost,iPort用于得到对方主机的地址和端口
	virtual int RecvFrom(const int iWant,const BYTE* pbBuf,int& iRead,char* pszHost=NULL,int *piPort=NULL);

protected:
	UINT m_uMaxUDP;//UDP包的最大长度
};
//TCP通信类,在基类上没有经过修改
class CTCPSocket: public CIPCSocket
{
public:
	CTCPSocket(SOCKET sockComm,BOOL fAutoDel=FALSE):CIPCSocket(sockComm,fAutoDel)
	{};

};

//pszPipeName \\.\pipe\pipe_name
//返回 INVALID_HANDLE_VALUE 表明失败
//创建服务器端管道,并等待连接
//iTimeout 为等待连接的超时市场，以秒计算
extern "C" HANDLE SPCreateServerPipe(LPCSTR pszPipeName,int iTimeout=10);
//创建客户端管道，并连接到服务器端
//pszPipeName \\server\pipe\pipe_name
//iTimeout 为超时时长
extern "C" HANDLE SPCreateClientPipe(LPCSTR pszPipeName,int iTimeout=10);

class CIPCNamedPipe: public CIPCComm
{
public:
	//fAutoDel 是否在删除对象时自动关闭Socket
	CIPCNamedPipe(HANDLE hPipe,BOOL fAutoDel=FALSE);
	~CIPCNamedPipe();
	HANDLE GetPipe(void){return m_hPipe;};
	HANDLE DetachPipe(void){HANDLE h=m_hPipe;m_hPipe=NULL;return h;};
public:
	static HANDLE CreateServerPipe(LPCSTR pszPipeName,int iTimeout=10)
	{
		return SPCreateServerPipe(pszPipeName,iTimeout);
	};
	static HANDLE CreateClientPipe(LPCSTR pszPipeName,int iTimeout=10)
	{
		return SPCreateClientPipe(pszPipeName,iTimeout);
	};

	virtual int Recv(const int iWant,BYTE* pbBuf,int& iRead);
	virtual int Send(const int iWant,const BYTE *pbBuf,int& iWrote);
	virtual int TestRead(int iTimeOut);
	virtual int TestConnect(int iTimeOut);
	static int TestRead(HANDLE hPipe,int iTimeOut)
	{
		CIPCNamedPipe sTemp(hPipe,FALSE);
		return sTemp.TestRead(iTimeOut);
	};
	static int TestConnect(HANDLE hPipe,int iTimeOut)
	{
		CIPCNamedPipe sTemp(hPipe,FALSE);
		return sTemp.TestConnect(iTimeOut);
	};
	virtual int RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
	virtual int TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
protected:
	HANDLE m_hPipe;
	BOOL m_fAutoDel;
};

}//完成名字空间的定义
#endif

