#include "StdAfx.h"
#include "DRCBusinessUnit.h"
#include <process.h>


#ifdef FD_SETSIZE
#undef FD_SETSIZE
#define FD_SETSIZE  4096
#endif


unsigned int __stdcall	__thr_run(void* pVoid)
{

	CDRCBusinessUnit* pSrv = (CDRCBusinessUnit*)pVoid;

	pSrv->run();

	return 0;
}

CDRCBusinessUnit::CDRCBusinessUnit(void)
{

	m_exit = false;
	m_safe_exit = NULL;
}

CDRCBusinessUnit::~CDRCBusinessUnit(void)
{
}

bool CDRCBusinessUnit::init(CDRCSvr* _svr , SOCKET _sk)
{
	m_svr = _svr;
	m_exit = false;
	int iMode    = 1;
	ioctlsocket(_sk, FIONBIO, (u_long FAR*) &iMode);

	m_sk_listen = _sk;

	m_safe_exit = CreateEvent(NULL, TRUE, FALSE, NULL);

	CloseHandle((HANDLE)_beginthreadex(NULL, 0, __thr_run, this, 0 ,NULL ));


	return true;

}

void CDRCBusinessUnit::uninit()
{

	m_exit = true; 
	if ( (int)m_safe_exit > 0 )
	{
		WaitForSingleObject(m_safe_exit, 2000);
		CloseHandle(m_safe_exit);
	}


}


void CDRCBusinessUnit::run()
{

	while(!m_exit)
	{

		heart_beat();
	
		Sleep(10);

	}

	SetEvent(m_safe_exit);
}

void CDRCBusinessUnit::heart_beat()
{

	SOCKET sk_accept = accept(m_sk_listen, NULL, NULL);	

	if((int)sk_accept > 0)
	{
		
		//新的链接请求，需要通过授信后方能添加进列表 
		if (authorize_client(sk_accept))
		{
			OutputDebugString(L"push vct");
			m_vct_clts.push_back(sk_accept);

		}
		else
		{
			closesocket(sk_accept);
		}
		
	}

	if (m_vct_clts.size() <= 0)
	{
		Sleep(10);
		return ;
	}


	check_data();
	


}

/*

recv:
<drc
	<data cmd="" product="" index="">
	<\data>
\drc>

send:

*/

bool CDRCBusinessUnit::analyze_datagram(SOCKET _sk, int _data_len)
{
	
	wstring str_xml = L"";

	if (!recv_xml(_sk, _data_len, str_xml))
	{
		OutputDebugString(L"recv_xml 0");
		return false;
	}
	
	wstring str_cmd     = L"";
	wstring str_product = L"";
	wstring str_index   = L"";

	xml_document doc;

	doc.load_buffer(str_xml.c_str(),str_xml.length()*2);
	str_cmd		= doc.child(L"drc").child(L"data").attribute(L"cmd").value();
	str_product = doc.child(L"drc").child(L"data").attribute(L"product").value();
	str_index   = doc.child(L"drc").child(L"data").attribute(L"index").value();
	wstring str_send_xml = L"";



	if (!dealwith_datagram(_sk, str_cmd, str_product, str_index, str_send_xml))
	{
		return false;
	}

	if (str_send_xml.length() <= 0)
	{
		return true;
	}

	if (!send_xml(_sk, str_send_xml))
	{
		return false;
	}

	return true;

}

bool CDRCBusinessUnit::dealwith_datagram(SOCKET _sk, wstring _cmd, wstring _product, wstring _index, wstring& _send_xml)
{

	if ( _cmd == L"drc_client_heart")
	{
		return protocol_client_heart(_sk, _product, _index, _send_xml);

	}

	if ( _cmd == L"drc_register_product")
	{
		OutputDebugString(L"drc_register_product");
		return protocol_register_product(_sk, _product, _index, _send_xml);

	}
	return false;
}

bool CDRCBusinessUnit::protocol_client_heart(SOCKET _sk, wstring _product, wstring _index,wstring& _send_xml)
{


	return true;
}

/*

<drc
	<result product="" repeated = "" 0:未重复， 1重复 registered = "" 0注册失败，注册成功 >
	<\result>
\drc>
*/
bool CDRCBusinessUnit::protocol_register_product(SOCKET _sk, wstring _product, wstring _index,wstring& _send_xml)
{

	//防止重复注册
	if (m_svr->check_repeat_login(_ttoi(_product.c_str()), _sk))
	{
		return true;
	}

	bool repeated   = false;
	bool registered = true;

	if ( m_svr->check_repeat_client(_ttoi(_product.c_str()), _index))
	{
		repeated = true;
		OutputDebugString(L"repeated");
	}
	else
	{
		if (!m_svr->add_client( _ttoi(_product.c_str()), _index, _sk))
		{
			registered = false;
		}
	}

	xml_document xml_send;
	xml_send.append_child(L"drc").append_child(L"result");
	xml_send.child(L"drc").child(L"result").append_attribute(L"repeated").set_value(int(repeated));
	xml_send.child(L"drc").child(L"result").append_attribute(L"registered").set_value(int(registered));

	xml_string_writer wr;
	xml_send.print(wr,L"",format_raw | format_indent, encoding_wchar);
	_send_xml = wr.result;

	return true;
}

bool CDRCBusinessUnit::send_xml(SOCKET _sk, wstring& _xml)
{

	int len_xml = _xml.length()*sizeof(TCHAR);
	int len_send = 0;
	len_send = send(_sk, (char*)&len_xml,sizeof(int), 0);
	if ( len_send < int(sizeof(int)))
	{
		return false;
	}

	int sum_sended = 0;
	while(len_xml > 0 )
	{

		int len_sended = send(_sk, ((char*)_xml.c_str())+sum_sended,len_xml, 0);

		if ( len_sended <= 0)
		{
			return false;
		}
		len_xml -= len_sended;
		sum_sended += len_sended;
		
	}
	
	return true;
}

bool CDRCBusinessUnit::recv_xml(SOCKET _sk, int _data_len, wstring& _xml)
{

	TCHAR* szStr = NULL;

	int nszLen = (_data_len/sizeof(TCHAR))+1;
	szStr = new TCHAR[nszLen];
	memset((char*)szStr, 0, nszLen*sizeof(TCHAR));

	int sum_recved = 0;

	while(_data_len > 0)
	{

		 
		int recved = recv(_sk ,((char*)szStr)+sum_recved, _data_len, 0);
		if (recved <= 0 && (WSAGetLastError() != WSAEWOULDBLOCK))
		{
			if (szStr)
			{
				delete [] szStr;
				szStr = NULL;
			}
			return false;
		}
		else if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			Sleep(10);
			continue;
		}
		_data_len  -= recved;
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


void CDRCBusinessUnit::check_data()
{

	fd_set Fd_Read;
	FD_ZERO(&Fd_Read);

	for (vector<SOCKET>::iterator it  = m_vct_clts.begin(); it != m_vct_clts.end();  it++)
	{
		
		FD_SET(*it, &Fd_Read);
		
	}
	

	int sk_max = m_vct_clts.size() + 1 ;

	timeval	 sk_tm = { 0 };

	sk_tm.tv_sec  = 0;
	sk_tm.tv_usec = 100;
	int	ret = select( sk_max, &Fd_Read, NULL, NULL, &sk_tm);

	if(ret <= 0)
	{
		return ;
	}
	

	for (vector<SOCKET>::iterator it  = m_vct_clts.begin(); it != m_vct_clts.end(); )
	{

		if(FD_ISSET(*it, &Fd_Read))
		{
			
			int data_len = 0;
			int recv_len = recv(*it, (char*)&data_len, sizeof(int), 0);
			if(recv_len <= 0 && (WSAGetLastError() != WSAEWOULDBLOCK))
			{
				
				closesocket(*it);
				m_svr->remove_client(*it);
				it = m_vct_clts.erase(it);				
				continue;
			}
			else
			{
				
				if (!analyze_datagram(*it, data_len))
				{
					closesocket(*it);
					m_svr->remove_client(*it);
					it = m_vct_clts.erase(it);
					continue;
				}
			}
		}

		it++;


	}


	FD_ZERO(&Fd_Read);

}

bool CDRCBusinessUnit::authorize_client(SOCKET _sk)
{

	bool bret = false;

	if (m_vct_clts.size() >= FD_SETSIZE)
	{

		return bret;
	}

	int authorize_cmd = 0;
	int recved = recv(_sk , (char*)&authorize_cmd, sizeof(int), 0);

	if (recved < int(sizeof(int)))
	{
		return false;
	}

	if ( authorize_cmd != 2014)
	{
		return false;
	}
	
	return true;
}