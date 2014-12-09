// AParameter.h: interface for the CAParameter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARAMETER_H__69BBF5B0_02DE_4B3F_B5D6_31745AB6492C__INCLUDED_)
#define AFX_APARAMETER_H__69BBF5B0_02DE_4B3F_B5D6_31745AB6492C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CAString;

class CAdoParameter;
class CADateTime;

class CAParameter		
{
public:
	CAParameter();
	virtual ~CAParameter();
	CAParameter(enum AdbcDataType Type, long lSize, enum AdbcParaDirection Direction, CAString strName);

public:
	CAdoParameter *m_pAdoParameter;

	CAdoParameter* GetParameter();
	CAdoParameter* operator =(CAdoParameter *pAdoParameter);

public:
	//========================================
	// 物件的名称
	//========================================
	CAString GetName();
	BOOL SetName(CAString strName);

	//========================================
	// 物件的精度
	//========================================
	BOOL SetPrecision(char nPrecision);

	//========================================
	// 物件的小数位数
	//========================================
	BOOL SetNumericScale(int nScale);

	//========================================
	// 是否该 Parameter 可输出、输入或兩者都可以。
	// 以及是否它是程序的回传值。 
	//========================================
	enum AdbcParaDirection GetDirection();
	BOOL SetDirection(enum AdbcParaDirection Direction);

	//========================================
	// Parameter 的值的大小（单位：Byte）。 
	//========================================
	int GetSize();
	BOOL SetSize(int size);

	//========================================
	// Parameter 的资料型态。 
	//========================================
	enum AdbcDataType GetType();
	BOOL SetType(enum AdbcDataType Type);

	//========================================
	// 获得指定给Parameter的值 
	//========================================
	BOOL GetValue(CADateTime &value);
	BOOL GetValue(CAString &value);
	BOOL GetValue(double &value);
	BOOL GetValue(long &value);
	BOOL GetValue(int &value);
	BOOL GetValue(short &value);
	BOOL GetValue(BYTE &value);
	BOOL GetValue(bool &value);

	//========================================
	// 设置指定Parameter的值 
	//========================================
	BOOL SetValue(const float &value);
	BOOL SetValue(const short &value);
	BOOL SetValue(const BYTE &value);
	BOOL SetValue(const CADateTime &value);
	BOOL SetValue(const CAString &value);
	BOOL SetValue(const double &value);
	BOOL SetValue(const long &value);
	BOOL SetValue(const int &value);
	BOOL SetValue(const bool &value);
	BOOL SetValue(const _variant_t &value);
};

#endif // !defined(AFX_APARAMETER_H__69BBF5B0_02DE_4B3F_B5D6_31745AB6492C__INCLUDED_)
