#pragma once

#include "DRCSvr.h"
#include "pugixml.h"


using namespace pugi;

class CDRCBusinessUnit
{
public:
	CDRCBusinessUnit(void);
	~CDRCBusinessUnit(void);

	bool			init(CDRCSvr* _svr, SOCKET _sk);
	void			uninit();

	/*
	Ïß³Ìº¯Êý
	*/
	void			run();

private: 

	void			heart_beat();

	void			check_data();

	bool			analyze_datagram(SOCKET _sk, int _data_len);
	bool			dealwith_datagram(SOCKET _sk, wstring _cmd, wstring _product, wstring _index,wstring& _send_xml);

	bool			protocol_client_heart(SOCKET _sk, wstring _product, wstring _index,wstring& _send_xml);
	bool			protocol_register_product(SOCKET _sk,wstring _product, wstring _index,wstring& _send_xml);

	bool			authorize_client(SOCKET _sk);

	bool			recv_xml(SOCKET _sk, int _data_len, wstring& _xml);
	bool			send_xml(SOCKET _sk, wstring& _xml);

	


	vector<SOCKET>	m_vct_clts;



	CDRCSvr* 		m_svr;
	SOCKET			m_sk_listen;
	bool				m_exit;
	HANDLE			m_safe_exit;

};
