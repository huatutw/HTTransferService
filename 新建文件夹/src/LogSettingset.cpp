// LogSettingset.cpp: implementation of the CLogSettingset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogSettingset.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogSettingset::CLogSettingset()
{

}

CLogSettingset::~CLogSettingset()
{
	ClearMemory();
}

void CLogSettingset::ClearMemory()
{
	int nRecordCount = m_dequeLog.size();
	
	for (int i=0;i<nRecordCount;i++)
	{
		CLogSetting* pRecord = m_dequeLog.front();
		m_dequeLog.pop_front();
		delete pRecord;
	}
}

BOOL CLogSettingset::GetDataFromBuffer(LPBYTE btData,DWORD dwSize)
{
	if (btData == NULL || dwSize == 0)
		return FALSE;
	
#ifndef TEST_PROJECT
	ClearMemory();		//释放内存
#endif
	
	CVirtualMemFile mem;
	mem.AttachMem(btData,dwSize);
	
	//////////////////////////////////////////////////////////////////////////
	//读取Record信息
	int nRecordCount = 0;
	mem.ReadData(&nRecordCount,sizeof(int));
	
	for (int i=0;i<nRecordCount;i++)
	{
		CLogSetting* pRecord = NULL;
		pRecord = new CLogSetting;

		pRecord->MemoryToRecord(mem);
		m_dequeLog.push_back(pRecord);
	}
	mem.ReadData(&m_nTotalCount,sizeof(int));
	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}

BOOL CLogSettingset::SetDataToBuffer(LPBYTE* btData,DWORD& dwSize)
{
	CVirtualMemFile mem(1024*512);	//申请512的内存
	
	int nRecordCount = m_dequeLog.size();
	
	mem.WriteData(&nRecordCount,sizeof(int));
	for (int i=0;i<nRecordCount;i++)
	{
		m_dequeLog[i]->RecordToMemory(mem);
	}
	mem.WriteData(&m_nTotalCount,sizeof(int));
	
	*btData = new BYTE[mem.GetDataSize()];
	memcpy(*btData,mem.GetData(),mem.GetDataSize());

	dwSize = mem.GetDataSize();
	
	return TRUE;
}

#ifdef DATABASE_SUPPORT
void CLogSettingset::WriteToDatabase()
{
	USES_CONVERSION;
	HRESULT hr;
	CDatabaseConnect::m_pRecordset.CreateInstance("ADODB.Recordset");
	if (CDatabaseConnect::m_pRecordset == NULL)
	{
		int ret = GetLastError();
		return;
	}
	
	CDatabaseConnect::m_pConnection->PutCursorLocation(adUseClient);
	try
	{
		hr = CDatabaseConnect::m_pConnection.CreateInstance("ADODB.Connection");///创建Connection对象
		if(SUCCEEDED(hr))
		{
			hr = CDatabaseConnect::m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=DocGuarder.mdb;Persist Security Info=False;Jet OLEDB:DataBase Password=111","","",adModeUnknown);///连接数据库
		}
	}
	catch(_com_error e)///捕捉异常
	{ 
		return;
	}	
	
	TCHAR strSQL[1024] = {0};
	
	for (int i=0;i<m_dequeLog.size();i++)
	{
		memset(strSQL,0,1024*sizeof(TCHAR));
		m_dequeLog[i]->GetInsertSQL(strSQL);
		//		TRACE(strSQL);
		CDatabaseConnect::m_pConnection->Execute((_bstr_t)(strSQL),NULL,0);
	}
	
}

void CLogSettingset::ReadFromDatabase(LPCTSTR strTotalSQL,LPCTSTR strSQL)
{
	//取总记录数
	CDatabaseConnect::m_pRecordset->Open(_variant_t(strTotalSQL), _variant_t((IDispatch *)CDatabaseConnect::m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
	while(!(CDatabaseConnect::m_pRecordset->adoEOF))
	{
		m_nTotalCount = atoi((char*)(_bstr_t(CDatabaseConnect::m_pRecordset->GetCollect((long)0))));
		CDatabaseConnect::m_pRecordset->MoveNext();
	}
	CDatabaseConnect::m_pRecordset->Close();
	
	CDatabaseConnect::m_pRecordset->Open(_variant_t(strSQL), _variant_t((IDispatch *)CDatabaseConnect::m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
	
	while(!(CDatabaseConnect::m_pRecordset->adoEOF))  
	{
		CLogSetting* pRecord = NULL;
		pRecord = new CLogSetting;
		
		for(int iIndex = 0; iIndex < CDatabaseConnect::m_pRecordset->GetFields()->GetCount(); iIndex++)
		{
			USES_CONVERSION;
			
			LPTSTR strStr1 = OLE2T(_bstr_t(CDatabaseConnect::m_pRecordset->GetCollect((long)(iIndex))));
			switch(iIndex)
			{
			case 1:
				pRecord->m_nLogType = _ttoi(strStr1);
				break;
			case 2:
				pRecord->m_nPerPageCount = _ttoi(strStr1);
				break;
			case 3:
				pRecord->m_nMaxLogCount = _ttoi(strStr1);
				break;
			case 4:
				pRecord->m_nOverOperType = _ttoi(strStr1);
				break;
			}
		}
		m_dequeLog.push_back(pRecord);
		CDatabaseConnect::m_pRecordset->MoveNext();
	}
	CDatabaseConnect::m_pRecordset->Close();
}
#endif