#pragma once
//#include "SocketBase.h"

class CTaskUnit
{
public:

	CTaskUnit(void);
	virtual ~CTaskUnit(void);

	virtual void						DoWork();

	SOCKET							m_sComm;

private:
};
