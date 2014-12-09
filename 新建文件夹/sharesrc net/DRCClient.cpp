#include "StdAfx.h"
#include "DRCClient.h"
#include <Windows.h>
#include <WinError.h>
#include <process.h>

#pragma warning(disable: 4996)

unsigned int __stdcall thr_driver(void* pvoid)
{

	CDRCClient* pSvr = (CDRCClient*)pvoid;
	pSvr->thread_run();

	return 0;

}
CDRCClient::CDRCClient(void)
{

	m_ip   = "";
	m_port = 0;

	m_sk   = 0;
	m_exit = false;
	m_safe_exit = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	HANDLE h = (HANDLE)_beginthreadex(NULL, 0, thr_driver, this, 0 , NULL);
	CloseHandle(h);

}

CDRCClient::~CDRCClient(void)
{

	m_exit = true;
	WaitForSingleObject(m_safe_exit, 2000);
	closesocket(m_sk);

}

int CDRCClient::set_cb_get_index(int _product, DRC_CALLBACK_GET_INDEX _cb)
{

	if (!check_product(_product))
	{
		return drc_unsupport_product;
	}

	if (!check_mutex(_product, L"set_cb_get_index"))
	{
		return drc_already_set;
	}
	
	set_index_cb(_product, _cb);

	return drc_ok;
}

int CDRCClient::set_cb_triggle_event(int _product, DRC_CALLBACK_TRIGGLE_EVENT _cb)
{

	if (!check_product(_product))
	{
		return drc_unsupport_product;
	}

	if (!check_mutex(_product, L"set_cb_triggle_event"))
	{
		return drc_already_set;
	}

	set_event_cb(_product, _cb);

	return drc_ok;
}

int CDRCClient::init(string _ip, int _port)
{

	m_ip		= _ip;
	m_port	= _port;

	init_net();

	if (!check_set_completeness())
	{
		return drc_setting_uncompleteness;
	}

	return drc_ok;
}

int CDRCClient::uninit()
{

	return drc_ok;

}

void CDRCClient::init_net()
{

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,2),&wsadata);

}

SOCKET CDRCClient::connect_service()
{

	SOCKET sockid;
	if ((sockid = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)
	{
		return 0;
	}
	
	struct sockaddr_in svr_addr;
	svr_addr.sin_addr.s_addr=inet_addr(m_ip.c_str());
	svr_addr.sin_family=AF_INET; 
	svr_addr.sin_port=htons(m_port);
	if (connect(sockid, (struct sockaddr*) &svr_addr, sizeof(struct sockaddr)) == -1)
	{
		closesocket(sockid);
		return 0;
	}

	int authorize_cmd = 2014;
	int len_send = send(sockid, (char *)&authorize_cmd, sizeof(int), 0);
	if (len_send < int(sizeof(int)))
	{
		closesocket(sockid);
		return 0;
	}

	return sockid;
}

bool CDRCClient::check_mutex(int _product, wstring _mutex)
{

	TCHAR str_mutex[1024] ={ 0 };
	_stprintf(str_mutex, L"%s%d", _mutex.c_str(), _product);
	CreateMutex(NULL, FALSE, str_mutex);
	int error_mutex = GetLastError();
	if( error_mutex == ERROR_ALREADY_EXISTS || error_mutex == ERROR_ACCESS_DENIED)
	{		
		return false;
	}
	
	return true;
}

bool CDRCClient::check_product(int _product)
{

	switch(_product)
	{
		case drc_dgupdate:
			break;
		case drc_dgclient:
			break;
		default:
			return false;
	}

	return true;
}

bool CDRCClient::check_set_completeness()
{

	bool bcomplete = true;
	m_lock.Lock();

	MAPPRODUCTINFO::iterator it = m_map_product.begin();

	for ( ; it != m_map_product.end(); it++)
	{

		if (!it->second.cb_event || !it->second.cb_index)
		{
			bcomplete = false;
		}
	}

	m_lock.UnLock();
	return bcomplete;
}

void CDRCClient::set_event_cb(int _product, DRC_CALLBACK_TRIGGLE_EVENT _cb)
{

	m_lock.Lock();

	MAPPRODUCTINFO::iterator it = m_map_product.find(_product);

	if (it == m_map_product.end())
	{

		DRC_CALLBACK_INFO info = { 0 };
		info.cb_event		   = _cb;
		m_map_product[_product] = info;

	}
	else
	{
		it->second.cb_event = _cb;
	}
	m_lock.UnLock();

}
void CDRCClient::set_index_cb(int _product, DRC_CALLBACK_GET_INDEX _cb)
{
	
	m_lock.Lock();

	MAPPRODUCTINFO::iterator it = m_map_product.find(_product);

	if (it == m_map_product.end())
	{

		DRC_CALLBACK_INFO info = { 0 };
		info.cb_index		   = _cb;
		m_map_product[_product] = info;

	}
	else
	{
		it->second.cb_index = _cb;
	}
	m_lock.UnLock();

}

void CDRCClient::thread_run()
{

	while(!m_exit)
	{

		if (!check_data_pipe())
		{
			
			Sleep(100);
			continue;
		}


		if (!check_set_completeness())
		{

			Sleep(100);
			continue;

		}

		dealwith_products();
		
		Sleep(100);

	}
	
	SetEvent(m_safe_exit);
}

bool CDRCClient::check_data_pipe()
{

	if ( m_sk <= 0)
	{

		m_sk = connect_service();

		if (m_sk <= 0)
		{
			return false;
		}
		else
		{
			reset_products_register_state();
		}
	}

	if (!check_pipe_alive())
	{
		m_sk = 0;
		return false;
	}

	return true;
}

void CDRCClient::reset_products_register_state()
{

	m_lock.Lock();

	MAPPRODUCTINFO::iterator it = m_map_product.begin();

	for ( ; it != m_map_product.end(); it++)
	{

		memset((char*)it->second.index, 0, sizeof(it->second.index));

	}

	m_lock.UnLock();

}

bool CDRCClient::check_pipe_alive()
{

	return send_xml(L"", 0, L"drc_client_heart");

}

bool CDRCClient::dealwith_products()
{

	m_lock.Lock();

	MAPPRODUCTINFO::iterator it = m_map_product.begin();

	for ( ; it != m_map_product.end() ; it++)
	{
		
		TCHAR _index[1024] = { 0 };
		int _len_index = 1024;

		//通过回调获取上层产品index
		it->second.cb_index(_index, _len_index);

		if ((wcslen(_index) > 0) && _tcscmp(_index, it->second.index) != 0 )
		{
			
			bool repeated = false;
			if (protocol_register_product(_index, it->first, repeated))
			{
				if ( repeated )
				{
					//通过回调告诉上层,id出现重复，需要上层完成id修改。
					it->second.cb_event(drc_repeated);
				}
				else
				{
					_tcscpy( it->second.index, _index );
				}
			}

		}
		
	}
	m_lock.UnLock();

	return true;
}



/*
send:
<drc
<data cmd="" product="" index="">
<\data>
\drc>


recv:
<drc
	<result product="" repeated = "" 0:未重复， 1重复 registered = "" 0注册失败，注册成功 >
	<\result>
\drc>
*/
bool CDRCClient::protocol_register_product(TCHAR* _index, int _product, bool& _repeated)
{
	if (!send_xml(_index, _product, L"drc_register_product"))
	{
		return false;
	}
	OutputDebugString(_index);

	wstring str_xml = L"";
	if (!recv_xml(str_xml))
	{
		return false;
	}
	
	xml_document doc;
	doc.load_buffer(str_xml.c_str(),str_xml.length()*2);	
	_repeated	= _ttoi(doc.child(L"drc").child(L"result").attribute(L"repeated").value());
	
	return true;
}

bool CDRCClient::send_xml(TCHAR* _index, int _product, TCHAR* _cmd)
{

	xml_document doc;
	xml_node data_node = doc.append_child(L"drc").append_child(L"data");
	data_node.append_attribute(L"cmd").set_value(_cmd);
	
	switch(_product)
	{

	case drc_dgclient:
		{
			data_node.append_attribute(L"product").set_value(L"0");
		}
		break;

	case drc_dgupdate:
		{
			data_node.append_attribute(L"product").set_value(L"1");
		}
		break;

	default:
		return false;
	}
		
	data_node.append_attribute(L"index").set_value(_index);
	xml_string_writer wr;
	doc.print(wr,L"",format_raw | format_indent, encoding_wchar);
	wstring xml_send = wr.result;

	int len_xml = xml_send.length()*sizeof(TCHAR);
	int len_send = 0;
	len_send = send(m_sk, (char*)&len_xml,sizeof(int), 0);
	if ( len_send < int(sizeof(int)))
	{
		return false;
	}

	int sum_sended = 0;
	while(len_xml > 0 )
	{

		int len_sended = send(m_sk, ((char*)xml_send.c_str())+sum_sended,len_xml, 0);

		if ( len_sended <= 0)
		{
			return false;
		}
		len_xml -= len_sended;
		sum_sended += len_sended;

	}

	//OutputDebugString(xml_send.c_str());
	return true;
}

bool CDRCClient::recv_xml(wstring& _xml)
{
	int data_len = 0;
	int recv_len = recv(m_sk, (char*)&data_len, sizeof(int), 0);
	if(recv_len <= 0 )
	{
		return false;
	}
	
	int nszLen = (data_len/sizeof(TCHAR))+1;
	TCHAR* szStr = new TCHAR[nszLen];
	memset((char*)szStr, 0, nszLen*sizeof(TCHAR));

	int sum_recved = 0;

	while(data_len > 0)
	{


		int recved = recv(m_sk ,((char*)szStr)+sum_recved, data_len, 0);
		if (recved <= 0)
		{
			return false;
		}
		data_len  -= recved;
		sum_recved += recved;

	}

	_xml = szStr;
	if (szStr)
	{
		delete [] szStr;
		szStr = NULL;
	}

	return true;
}
