// AException.h: interface for the CAException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEXCEPTION_H__EC4AD9EC_813E_47A7_A28F_3905735093A5__INCLUDED_)
#define AFX_AEXCEPTION_H__EC4AD9EC_813E_47A7_A28F_3905735093A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////
// �쳣֪ͨ
//
////////////////////////////////////////////////
class CAString;
class CAConnect;

class CAException //: public CException
{
public:
	CAException(CAConnect *pAConnect );
	CAException(CAConnect *pAConnect , CAString strErrorInfo);
	CAException(CAConnect *pAConnect , UINT nCause, HRESULT hr = 0 );
	virtual ~CAException();

public:
	CAConnect	*m_pAConnect;
	CAString	m_strErrorInfo;
	UINT		m_nCause;
	HRESULT		m_nHr;

	CAString GetError();
	CAString	GetErrorEx( LPCTSTR strFileName, int nLineNum , LPCTSTR strSql = _T("") );
	void	GetHRRESULTMessage(HRESULT hr, TCHAR  *msg);
};

#endif // !defined(AFX_AEXCEPTION_H__EC4AD9EC_813E_47A7_A28F_3905735093A5__INCLUDED_)
