#include <deque>
using namespace std;

#define LEVEL_ARR_SIZE 6000
typedef deque<int> INT_DEQUE;

#define U8 BYTE
#define U32 unsigned long
#define BUFF_SIZE 512

typedef struct rc4_key
{      
   unsigned char state[256];       
   unsigned char x;        
   unsigned char y;
} rc4_key;

#define swap_byte(x,y) t = *(x); *(x) = *(y); *(y) = t

struct Offline_Info	// DocGuarder2.0结构
{
	DWORD DogID;
	BYTE Key[128];
	DWORD dwCurPriv;
	DWORD dwOverTime;
};
 
#ifndef DOCGUARDER42
struct Offline_Info2 // DocGuarder3.0结构
{
	BYTE  Key[128];
	DWORD DogID;
	DWORD dwCurPriv;
	DWORD dwOverTime;
	DWORD dwCryptLevel;
	int	  nPolicyType;
	int	  nPolicyCount;
	int	  arrPolicy[LEVEL_ARR_SIZE];		//支持1000个等级的例外策略
	int   bandmask;				//绑定信息
	char strNetSerial[32];		
	char strHardSerial[32];
	char strPassword[32];
	char strIP[16];				 //支持广域网回收节点
	BOOL  bUseTimeControl;		 //是否使用时钟功能  TRUE: 则下三个参数不起作用, FALSE: 则 dwOverTime 不起作用
	tm    tmBase;			     //锁内的基准时间
	int dwStartTimeSecond;     //离开始使用的时间,(秒为单位)
	int dwEndTimeSecond;		 //离结束使用的时间,(秒为单位)
};

struct Offline_Info3 // 离网审批结构
{
	TCHAR		strLoginName[BUFF_SIZE];			//离网登录名
	TCHAR		strComputerName[BUFF_SIZE];		//离网计算机名
	TCHAR		strClientID[40];			//离网客户端ID
	TCHAR		strNetErorID[40];			//离网授权ID，供加时使用
	int			nTime;						//离网时间
	BOOL		bIsAddTime;					//是否增加时间
	SYSTEMTIME	stOverDate;					//审批文件可使用时效
};

struct SystemApi32Use		//对应用程序必须传递的数据
{
	BYTE  Key[128];			//加密密钥
	DWORD DogID;			//狗号
	DWORD dwCurPriv;		//当前的用户权限
	DWORD dwCryptLevel;		//用户的等级
	int	  nPolicyType;		//策略的类型  (暂时无效)
	int	  nPolicyCount;		//策略的实际个数
	int	  arrPolicy[LEVEL_ARR_SIZE];  //支持1000个等级的打开策略
};

#else
struct Offline_Info2 // DocGuarder3.0结构
{
	BYTE  Key[128];
	DWORD DogID;
	LONGLONG dwCurPriv;
	DWORD dwOverTime;
	DWORD dwCryptLevel;
	int	  nPolicyType;
	int	  nPolicyCount;
	int	  arrPolicy[LEVEL_ARR_SIZE];		//支持1000个等级的例外策略
	int   bandmask;				//绑定信息
	char strNetSerial[32];		
	char strHardSerial[32];
	char strPassword[32];
	char strIP[16];				 //支持广域网回收节点
	BOOL  bUseTimeControl;		 //是否使用时钟功能  TRUE: 则下三个参数不起作用, FALSE: 则 dwOverTime 不起作用
	tm    tmBase;			     //锁内的基准时间
	int dwStartTimeSecond;     //离开始使用的时间,(秒为单位)
	int dwEndTimeSecond;		 //离结束使用的时间,(秒为单位)
};

struct Offline_Info3 // 离网审批结构
{
	TCHAR		strLoginName[BUFF_SIZE];			//离网登录名
	TCHAR		strComputerName[BUFF_SIZE];		//离网计算机名
	TCHAR		strClientID[40];			//离网客户端ID
	TCHAR		strNetErorID[40];			//离网授权ID，供加时使用
	int			nTime;						//离网时间
	BOOL		bIsAddTime;					//是否增加时间
	SYSTEMTIME	stOverDate;					//审批文件可使用时效
};

struct SystemApi32Use		//对应用程序必须传递的数据
{
	BYTE  Key[128];			//加密密钥
	DWORD DogID;			//狗号
	LONGLONG dwCurPriv;		//当前的用户权限
	DWORD dwCryptLevel;		//用户的等级
	int	  nPolicyType;		//策略的类型  (暂时无效)
	int	  nPolicyCount;		//策略的实际个数
	int	  arrPolicy[LEVEL_ARR_SIZE];  //支持1000个等级的打开策略
};
#endif

enum
{
	POLICY_OWN = 0,
	POLICY_BELOW,
	POLICY_ALL,
	POLICY_USER,
};

enum
{
	BAND_MAC = 0x01,
	BAND_HARDDISK = 0x02,
	BAND_PASSWORD = 0x04,
	BAND_INTERNET = 0x08,
};

class DGCommonFunc
{
public:
//	以下三个函数用于将字符串转换为规定字符集合的字符串
	static char LMoveBit(int base, int MoveNum);
	static void ToBase64( const unsigned char *in, int inlen, char *out );
	static void FromBase64( const unsigned char *base64code, int base64length, char *out );
////////////////////////////////////////////////////////////////////////////////////////////
//  以下为rc4算法的函数

	static void prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key);
	static void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *key);
	static BOOL Encrypt_RC4(LPBYTE lpszBuffer,DWORD dwSize,char* lpszPassword);
//  将string转换为IP, 127.0.0.1 -> 127 0 0 1
#ifndef NO_MFC_VERSION
	static BOOL String2IP(CAString strIP,BYTE& ip1,BYTE& ip2,BYTE& ip3,BYTE& ip4);
#endif
//  读取注册表的数据
//	static BOOL ReadRegData(LPTSTR strUser,LPTSTR strPass,LPTSTR strServerIP,DWORD& dwServerPort,LPTSTR strUpdateIP,DWORD& dwUpdatePort);
	static BOOL ReadRegData(LPTSTR strUser,LPTSTR strPass);
	static BOOL ReadRegDataEx(LPWSTR strUser,LPWSTR strPass);
	static BOOL ReadRegServerData(LPTSTR strServerIP,DWORD& dwServerPort);
	
	static BOOL ReadRegLogServerData(LPTSTR strLogServerIP,DWORD& dwLogServerPort);

	static BOOL ReadRegServerData2(LPTSTR strServerIP2,DWORD& dwServerPort2);

	static BOOL ReadRegUpdateData(LPTSTR strUpdateIP,DWORD& dwUpdatePort);
	static BOOL ReadRegUpdateData(DWORD& dwServerPort,DWORD& dwUpdatePort);
	static BOOL WriteRegServerData(DWORD dwServerPort);
	static BOOL WriteRegUpdateData(DWORD dwUpdatePort);
//  从服务器验证用户,并得到当前用户的权限
//	Offline_Info2* GetLoginData(const char* strUser,const char* strPass,CString strServer,DWORD dwPort,int& nErrorType);
	static BOOL IsInExceptList(int* arrPolicy,int count,DWORD dwCryptLevel);
	static BOOL IsUserCanAccess(int* arrPolicy,int count,DWORD dwCryptLevel,DWORD dwFileLevel);
	static BOOL IsUserCanAccess2(int* arrPolicy,int count,DWORD dwCryptLevel,DWORD dwFileLevel,int nPrivType);
//  得到硬件信息
#ifndef NO_MFC_VERSION
	static CAString GetMacByNetBIOS();
	static BOOL GetMacByCmd( CAString &strMac );
	static CAString GetHardDiskSerial();
#endif
};
