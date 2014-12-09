// VirtualMemFile.h: interface for the CVirtualMemFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRTUALMEMFILE_H__A230D671_8E43_4C89_9F53_F6C3832B6A19__INCLUDED_)
#define AFX_VIRTUALMEMFILE_H__A230D671_8E43_4C89_9F53_F6C3832B6A19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVirtualMemFile  
{
public:
	CVirtualMemFile();
	CVirtualMemFile(int nMemorySize);

	virtual ~CVirtualMemFile();

public:
	BOOL AttachMem(LPBYTE btData,int nSize);
	LPBYTE DetachMem();

	BOOL WriteData(LPVOID lpData,int dwDataSize);
	BOOL ReadData(LPVOID lpData,int dwDataSize);

	BOOL WriteString(LPTSTR strData);
	BOOL WriteStringW(LPTSTR strData);
	LPTSTR ReadString();

	LPWSTR ReadStringW();

	DWORD Seek(int offset,int Mode);

	int GetDataSize();
	LPBYTE GetData();
private:
	LPBYTE m_btContent;
	int	   m_nCurrentPointer;	//当前指针
	int	   m_nEndPointer;		//结尾指针，可以认为就是Buffer的大小
	int    m_nTotalMemory;		//申请的内容大小
	BOOL   m_nAutoGrow;			//自动增长内存
};

#endif // !defined(AFX_VIRTUALMEMFILE_H__A230D671_8E43_4C89_9F53_F6C3832B6A19__INCLUDED_)
