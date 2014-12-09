#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <string>
#include <map>
#include "Mutex.h"
#include "pugixml.h"
using namespace pugi;
using namespace std;

/*

*/

enum DRC_PRODUCTS
{
	drc_dgclient = 0,
	drc_dgupdate,
};

enum DRC_CALLBACK_EVENT_CMD
{
	drc_repeated = 1,
};

enum DRC_CLIENT_ERROR
{
	drc_ok			= 0,
	drc_already_set = -10000,
	drc_unsupport_product,
	drc_setting_uncompleteness,
};

typedef void (*DRC_CALLBACK_GET_INDEX)(TCHAR* _index, int _len_index);
typedef void (*DRC_CALLBACK_TRIGGLE_EVENT)(int _cmd);

struct				DRC_CALLBACK_INFO{
	DRC_CALLBACK_GET_INDEX		cb_index;
	DRC_CALLBACK_TRIGGLE_EVENT  cb_event;
	TCHAR						index[1024];
};


typedef map<int, DRC_CALLBACK_INFO> MAPPRODUCTINFO;


class CDRCClient
{
public:
	CDRCClient(void);
	~CDRCClient(void);

	/*
	回调中需要完成获取产品Index的操作
	*/
	int set_cb_get_index(int _product, DRC_CALLBACK_GET_INDEX _cb);

	/*
	当回调被触发时，根据回调cmd事件进行解析，
	回调内容处理，需要阻塞进行，否则会出现重复回调。
	*/
	int set_cb_triggle_event(int _product, DRC_CALLBACK_TRIGGLE_EVENT _cb);

	/*
		对同一个产品id，必须同时成功调用set_cb_get_index(),set_cb_triggle_event(),
		该函数方能调用成功
	*/
	int					init(string _ip, int _port);
	int					uninit();

	void				thread_run();

private:

	void				init_net();
	SOCKET			connect_service();	
	/*
	检测网络是否通讯正常,如果网络异常则需要重新链接.
	*/
	bool				check_data_pipe();
	bool				check_pipe_alive();

	bool				check_mutex(int _product, wstring _mutex);
	bool				check_product(int _product);
	bool				check_set_completeness();

	void				set_event_cb(int _product, DRC_CALLBACK_TRIGGLE_EVENT _cb);
	void				set_index_cb(int _product, DRC_CALLBACK_GET_INDEX _cb);

	/*
	第一次与服务器建立链接时,需要重置注册状态
	*/
	void				reset_products_register_state();

	bool				dealwith_products();

	bool				protocol_register_product(TCHAR* _index, int _product, bool& _repeated);
	
	bool				send_xml(TCHAR* _index, int _product, TCHAR* _cmd);

	bool				recv_xml(wstring& _xml);

	MutexLock					m_lock;
	MAPPRODUCTINFO		m_map_product;

	string							m_ip;
	int								m_port;
	HANDLE						m_safe_exit;
	bool							m_exit;

	SOCKET						m_sk;

};
