// VariantMgr.h: interface for the CAVariantConv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIANTMGR_H__CB2A2F5E_E3DC_4F69_828A_E339752814DA__INCLUDED_)
#define AFX_VARIANTMGR_H__CB2A2F5E_E3DC_4F69_828A_E339752814DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAString;
class CADateTime;

////////////////////////////////////////////////
// 变量转换
////////////////////////////////////////////////

#define		VARIANT_TYPE_BOOL			(0)
#define		VARIANT_TYPE_INT			(1)
#define		VARIANT_TYPE_LONG			(2)
#define		VARIANT_TYPE_FLOAT			(3)
#define		VARIANT_TYPE_DWORD			(4)
#define		VARIANT_TYPE_DOUBLE			(5)
#define		VARIANT_TYPE_BYTE			(6)
#define		VARIANT_TYPE_STRING			(7)
#define		VARIANT_TYPE_CHAR			(8)
#define		VARIANT_TYPE_OLECURRENCY	(9)
#define		VARIANT_TYPE_OLEDATATIME	(10)

class CAVariantConv  
{
public:
	CAVariantConv();
	virtual ~CAVariantConv();

	//=====================================
	// 从VARIANT获得其它类型数据数据
	//=====================================
	void Load( const VARIANT &vData );
	BOOL GetData( bool &bData );
	BOOL GetData( int &nData );
	BOOL GetData( short &sData );
	BOOL GetData( UINT &unData );
	BOOL GetData( long &lData );
	BOOL GetData( DWORD &dwData );
	BOOL GetData( double &dData );
	BOOL GetData( float &fData );
	BOOL GetData( BYTE &byData );
	BOOL GetData( CAString &strData );
	BOOL GetData( char &cData );
	BOOL GetData( CADateTime &dtData );
	BOOL GetData( CURRENCY &cyData );

	//======================================
	// 从其它类型数据获得VARIANT
	//======================================
	void Load( bool bData );
	void Load( int nData );
	void Load( short sData );
	void Load( UINT unData );
	void Load( long lData );
	void Load( DWORD dwData );
	void Load( double dData );
	void Load( float fData );
	void Load( BYTE byData );
	void Load( CAString strData );
	void Load( char cData );
	void Load( CADateTime dtData );
	void Load( CURRENCY cyData );
	BOOL GetData( VARIANT &vData );

public:
	int				m_nType;

	bool			m_bData;
	int				m_nData;
	long			m_lData;
	DWORD			m_dwData;
	float			m_fData;
	double			m_dData;
	BYTE			m_byData;
	char			m_cData;
	CURRENCY		m_cyData;
	CAString		m_strData;
	CADateTime		m_dtData;
};

#endif // !defined(AFX_VARIANTMGR_H__CB2A2F5E_E3DC_4F69_828A_E339752814DA__INCLUDED_)
