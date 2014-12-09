#pragma once

#include <vector>
#include <map>
#include <string>
#include "Mutex.h"
using namespace std;

/*


*/


typedef map<wstring, int>	MAPBYINDEX;
typedef map<int, wstring>	MAPBYSOCKET;

enum DRC_PRODUCTS
{
	drc_dgclient = 0,
	drc_dgupdate,
};


class CDRCBusinessUnit;
class CDRCSvr
{
public:
	CDRCSvr(void);
	~CDRCSvr(void);
	

	/*
	初始化服务模块
	_listen：服务监听端口
	_clt_nuits ：业务处理模块,该值越大处理速度越快
	*/
	bool		init_service(int _listen = 8040, int _clt_nuits = 20);
	bool		uninit_service();

	/*
	检测不同客户端是否重复登陆
	_product:产品id
	_index: 产品索引

	ret: 
	true 客户端已经登陆
	false 客户端未登陆
	*/
	bool		check_repeat_client(int _product, wstring _index);

	/*
	检测同一客户端是否有重复login
	_product:产品id
	_sk: 产品索引

	ret: 
	true 客户端已经登陆
	false 客户端未登陆
	*/
	bool		check_repeat_login(int _product, SOCKET _sk);


	bool		add_client(int _product, wstring _index, int _sk);
	bool		remove_client(int _sk);


private:
	void										init_net();
	SOCKET									create_listen(SOCKET _sk);


	vector<CDRCBusinessUnit*>		m_vct_units;

	MutexLock								m_lock_dgclient;		
	MAPBYSOCKET						m_sk_dgclient;
	MAPBYINDEX							m_ind_dgclient;

	MutexLock								m_lock_dgupdate;		
	MAPBYSOCKET						m_sk_dgupdate;
	MAPBYINDEX							m_ind_dgupdate;
};
