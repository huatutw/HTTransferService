#include "StdAfx.h"
#include "DRCSvr.h"
#pragma comment(lib, "ws2_32.lib")
#include "DRCBusinessUnit.h"


CDRCSvr::CDRCSvr(void)
{
}

CDRCSvr::~CDRCSvr(void)
{
}

bool	CDRCSvr::init_service(int _listen, int _clt_nuits)
{
	init_net();

	SOCKET sk_svr = create_listen(_listen);

	if ((int) sk_svr <= 0)
	{
		return false;
	}

	for (int i = 0; i < _clt_nuits; i++)
	{
		CDRCBusinessUnit* p = new CDRCBusinessUnit;
		p->init(this, sk_svr);
		m_vct_units.push_back(p);
	}

	return true;
}
bool    CDRCSvr::uninit_service()
{
	for (int i = 0; i < m_vct_units.size(); i++)
	{
		CDRCBusinessUnit* p = m_vct_units[i];
		if (p)
		{
			p->uninit();
			delete p;
			p = NULL;
		}
	}

	m_vct_units.clear();
	return true;

}

void	CDRCSvr::init_net()
{

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,2),&wsadata);

}

SOCKET  CDRCSvr::create_listen(SOCKET _sk)
{

	SOCKET sockid;

	if ((sockid = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)
		return 0;

	struct sockaddr_in srv_addr = {0};
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	srv_addr.sin_port = htons(_sk);
	if (bind(sockid,(struct sockaddr*)&srv_addr,sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		goto error;

	listen(sockid,500);
	return sockid;

error:
	closesocket(sockid);
	return 0;

}

bool    CDRCSvr::check_repeat_login(int _product, SOCKET _sk)
{

	bool bret = false;

	switch(_product)
	{
	case drc_dgupdate:
		{
			m_lock_dgupdate.Lock();

			MAPBYSOCKET::iterator it = m_sk_dgupdate.find(_sk);
			if (it != m_sk_dgupdate.end())
			{
				bret = true;
			}

			
			m_lock_dgupdate.UnLock();
		}
		break;
	case drc_dgclient:
		{

			m_lock_dgclient.Lock();
			
			MAPBYSOCKET::iterator it = m_sk_dgclient.find(_sk);
			if (it != m_sk_dgclient.end())
			{
				bret = true;
			}

			
			m_lock_dgclient.UnLock();
		}
		break;
	}

	return bret;
}
bool    CDRCSvr::check_repeat_client(int _product, wstring _index)
{

	bool bret = false;

	switch(_product)
	{
		case drc_dgupdate:
			{
				OutputDebugString(_index.c_str());
				m_lock_dgupdate.Lock();
				MAPBYINDEX::iterator it = m_ind_dgupdate.find(_index);
				if (it != m_ind_dgupdate.end())
				{
					bret = true;
				}
				m_lock_dgupdate.UnLock();
			}
			break;
		case drc_dgclient:
			{
				OutputDebugString(_index.c_str());
				m_lock_dgclient.Lock();
				MAPBYINDEX::iterator it = m_ind_dgclient.find(_index);
				if (it != m_ind_dgclient.end())
				{
					bret = true;
				}
				m_lock_dgclient.UnLock();
			}
			break;
	}

	return bret;
}
bool    CDRCSvr::add_client(int _product, wstring _index, int _sk)
{

	bool bret = true;

	switch(_product)
	{
	case drc_dgupdate:
		{

			m_lock_dgupdate.Lock();
			MAPBYINDEX::iterator it = m_ind_dgupdate.find(_index);
			if (it != m_ind_dgupdate.end())
			{
				bret = false;
			}
			else
			{
				m_ind_dgupdate[_index] = _sk;
				m_sk_dgupdate[_sk] = _index;
			}
			m_lock_dgupdate.UnLock();
		}
		break;
	case drc_dgclient:
		{

			m_lock_dgclient.Lock();
			MAPBYINDEX::iterator it = m_ind_dgclient.find(_index);
			if (it != m_ind_dgclient.end())
			{
				bret = false;
			}
			else
			{
				m_ind_dgclient[_index] =_sk;
				m_sk_dgclient[_sk] = _index;
			}
			m_lock_dgclient.UnLock();
		}
		break;
	}


	return bret;
}
bool    CDRCSvr::remove_client(int _sk)
{

	bool bret = false;

	m_lock_dgclient.Lock();
	MAPBYSOCKET::iterator it_sk = m_sk_dgclient.find(_sk);
	MAPBYINDEX::iterator it_index;

	if (it_sk != m_sk_dgclient.end())
	{
		wstring str_index = it_sk->second;
		m_sk_dgclient.erase(it_sk);
		it_index = m_ind_dgclient.find(str_index);
		if (it_index != m_ind_dgclient.end())
		{
			m_ind_dgclient.erase(it_index);
		}
		bret = true;
	}
	m_lock_dgclient.UnLock();


	m_lock_dgupdate.Lock();
	it_sk = m_sk_dgupdate.find(_sk);
	if (it_sk != m_sk_dgupdate.end())
	{
		wstring str_index = it_sk->second;
		m_sk_dgupdate.erase(it_sk);
		it_index = m_ind_dgupdate.find(str_index);
		if (it_index != m_ind_dgupdate.end())
		{
			m_ind_dgupdate.erase(it_index);
		}
		bret = true;
	}
	m_lock_dgupdate.UnLock();
	return bret;
}