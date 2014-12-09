// VirtualMemFile.cpp: implementation of the CVirtualMemFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VirtualMemFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVirtualMemFile::CVirtualMemFile()
{
	m_btContent = NULL;
	m_nCurrentPointer = 0;
	m_nEndPointer = 0;
	m_nAutoGrow = FALSE;
	m_nTotalMemory = 0;
	//yl20120112修改这里貌似重复了
	//m_btContent = NULL;
}

CVirtualMemFile::CVirtualMemFile(int nMemorySize)
{	
	if (nMemorySize > 0)
	{
		m_btContent = new BYTE[nMemorySize];
		memset(m_btContent, 0, nMemorySize);
	}
	else
	{
		m_btContent = NULL;
	}
	m_nCurrentPointer = 0;
	m_nEndPointer = 0;
	//自动增加内存
	m_nAutoGrow = TRUE;
	m_nTotalMemory = nMemorySize;
}

CVirtualMemFile::~CVirtualMemFile()
{
	if (m_btContent)
	{
		delete[] m_btContent;
	}
}

BOOL CVirtualMemFile::AttachMem(LPBYTE btData,int nSize)
{
	if (nSize <= 0)
		return FALSE;
	if (m_btContent)
	{
		delete[] m_btContent;
		m_btContent = NULL;
	}
	
	m_nCurrentPointer = 0;
	m_nEndPointer = 0;
	m_nTotalMemory = 0;

	m_btContent = new BYTE[nSize];
	memset(m_btContent, 0, nSize);
	if (NULL == m_btContent)
		return FALSE;

	memcpy(m_btContent, btData, nSize);
	m_nTotalMemory = nSize;

	return TRUE;
}

LPBYTE CVirtualMemFile::DetachMem()
{
	//这里如果先DetachMem后调用析构函数
	//会造成内存泄露-------------------yl 20120112增加注释
	LPBYTE bt = m_btContent;
	m_btContent = NULL;
	return bt;
}

//////////////////////////////////////////////////////////////////////////
//参数:[in]LPVOID lpData,从m_currentPointer处写入dwDataSize个字节		//
//参数:[in]dwDataSize，写入的字节数										//
//注意:当内存不足时自动增长当内存后的内存								//
//	  为(m_nCurrentPointer + dwDataSize) * 2							//
//////////////////////////////////////////////////////////////////////////
BOOL CVirtualMemFile::WriteData(LPVOID lpData, int dwDataSize)
{
	//内存不足
	if (m_nCurrentPointer + dwDataSize > m_nTotalMemory)
	{
		if (m_nAutoGrow)
		{
			//自动增加内存
			LPBYTE btNewContent = new BYTE[(m_nCurrentPointer + dwDataSize) * 2];
			memset(btNewContent, 0, (m_nCurrentPointer + dwDataSize) * 2);

			memcpy(btNewContent, m_btContent, m_nCurrentPointer);
			delete[] m_btContent;
			m_btContent = btNewContent;
			
			m_nTotalMemory = (m_nCurrentPointer + dwDataSize) * 2;
		}
		else
		{
			return FALSE;
		}
	}	
	
	memcpy(m_btContent + m_nCurrentPointer, lpData, dwDataSize);
	m_nCurrentPointer += dwDataSize;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//参数:[out]LPVOID lpData,读取的数据存放在这个buffer					//
//参数:[in]dwDataSize,读取的字节数										//
//注意:该函数会将m_CurrentPointer设置在原先位置后面dwDataSize			//
//////////////////////////////////////////////////////////////////////////
BOOL CVirtualMemFile::ReadData(LPVOID lpData, int dwDataSize)
{
	if (m_nCurrentPointer + dwDataSize > m_nTotalMemory)
		return FALSE;

	memcpy(lpData, m_btContent + m_nCurrentPointer, dwDataSize);
	m_nCurrentPointer += dwDataSize;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//参数:[in]int offset,m_CurrentPointer偏移量							//
//参数:[in]int Mode,为FILE_BEGIN，FILE_CURRENT，FILE_END				//
//注意:根据mode设置m_currentPointer										//
//////////////////////////////////////////////////////////////////////////
DWORD CVirtualMemFile::Seek(int offset, int Mode)
{
	switch(Mode)
	{
		//文件开始处
	case FILE_BEGIN:
		{
			if (offset >= m_nTotalMemory)
			{
				m_nCurrentPointer = m_nTotalMemory;
			}
			else
			{
				m_nCurrentPointer = offset;
			}
		}
		break;
		//文件当前指向
	case FILE_CURRENT:
		{
			if (m_nCurrentPointer + offset >= m_nTotalMemory)
			{
				m_nCurrentPointer = m_nTotalMemory;
			}
			else
			{
				m_nCurrentPointer += offset;
			}
		}
		break;
		//文件尾部
	case FILE_END:
		{
			if (offset >= 0)
			{
				m_nCurrentPointer = m_nTotalMemory;
			}
			else
			{
				m_nCurrentPointer = m_nTotalMemory + offset;
			}
		}
		break;
	}
	return m_nCurrentPointer;
}

//////////////////////////////////////////////////////////////////////////
//Parameter:[in]LPTSTR strData,写入的字符串								//
//Return:BOOL 返回值总是TRUE											//
//Note:当写入字符串为空时，将0写入；先写入字符串长度，再写入字符串		//
//////////////////////////////////////////////////////////////////////////
BOOL CVirtualMemFile::WriteString(LPTSTR strData)
{
	int Length = 0;
	if (strData == NULL)
	{
		WriteData(&Length, sizeof(int));
		return TRUE;
	}
	
	Length = _tcslen(strData);
	WriteData(&Length, sizeof(int));
	if (Length == 0)
		return TRUE;
	WriteData(strData, sizeof(TCHAR) * Length);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//Parameter:[in]LPTSTR strData,写入的字符串								//
//Return:BOOL 返回值总是TRUE											//
//Note:当写入字符串为空时，将0写入；先写入字符串长度，再写入字符串		//
//////////////////////////////////////////////////////////////////////////
BOOL CVirtualMemFile::WriteStringW(LPTSTR strData)
{
	USES_CONVERSION;
	int Length = 0;
	if (strData == NULL)
	{
		WriteData(&Length, sizeof(int));
		return TRUE;
	}
	
	Length = _tcslen(strData);
	WriteData(&Length, sizeof(int));
	if (Length == 0)
		return TRUE;
	WriteData(T2W(strData), sizeof(WCHAR)*Length);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//Parameter:															//
//Return:LPTSTR，当读取的长度为空时，返回NULL							//
//Note:当写入字符串为空时，将0写入；先写入字符串长度，再写入字符串		//
//////////////////////////////////////////////////////////////////////////
LPTSTR CVirtualMemFile::ReadString()
{
	int Length = 0;
	ReadData(&Length, sizeof(int));

	if (Length == 0)
		return NULL;
	LPTSTR strData = new TCHAR[Length+1];
	memset(strData, 0, (Length + 1) * sizeof(TCHAR));
	
	ReadData(strData, sizeof(TCHAR)*Length);
	
	return strData;
}

//////////////////////////////////////////////////////////////////////////
//Parameter:															//
//Return:LPTSTR，当读取的长度为空时，返回NULL							//
//Note:当写入字符串为空时，将0写入；先写入字符串长度，再写入字符串		//
//////////////////////////////////////////////////////////////////////////
LPWSTR CVirtualMemFile::ReadStringW()
{
	int Length = 0;
	ReadData(&Length,sizeof(int));

	if (Length == 0)
		return NULL;
	LPWSTR strData = new WCHAR[Length + 1];
	memset(strData, 0, (Length + 1) * sizeof(WCHAR));
	
	ReadData(strData, sizeof(WCHAR) * Length);
	
	return strData;
}

//////////////////////////////////////////////////////////////////////////
//Parameter:															//
//Return:int,m_nCurrentPointer											//
//Note:																	//
//////////////////////////////////////////////////////////////////////////
int CVirtualMemFile::GetDataSize()
{
	return m_nCurrentPointer;
}

/************************************************************************/
/* 返回m_btContent                                                      */
/************************************************************************/
LPBYTE CVirtualMemFile::GetData()
{
	return m_btContent;
}