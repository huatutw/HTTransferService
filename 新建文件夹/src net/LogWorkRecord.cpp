// LogWorkRecord.cpp: implementation of the CLogWorkRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogWorkRecord.h"
#include "DatabaseConnect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogWorkRecord::CLogWorkRecord()
{
	m_nLogType = LOG_WORK;
	m_nPerTime = -1;
	m_nTotalTime = 0;
	m_fEff = 0;
}

CLogWorkRecord::~CLogWorkRecord()
{

}

void CLogWorkRecord::InitTestData()
{

}

void CLogWorkRecord::RecordToMemory(CVirtualMemFile& mem)
{
	mem.WriteData(&m_nLogType,sizeof(int));
	mem.WriteString(m_strIndex);
	mem.WriteString(m_strComputer);
	mem.WriteString(m_strUser);
	mem.WriteString(m_strIP);
	mem.WriteString(m_strTime);
	mem.WriteData(&m_nPerTime,sizeof(int));
	mem.WriteData(&m_nTotalTime,sizeof(int));
	mem.WriteData(&m_fEff,sizeof(float));
}

void CLogWorkRecord::MemoryToRecord(CVirtualMemFile& mem)
{
	if (m_bUnicodeToAnsi)
	{
		mem.ReadData(&m_nLogType,sizeof(int));
		USES_CONVERSION;
		m_strIndex = _tcsdup(W2T(mem.ReadStringW()));
		m_strComputer = _tcsdup(W2T(mem.ReadStringW()));
		m_strUser = _tcsdup(W2T(mem.ReadStringW()));
		m_strIP = _tcsdup(W2T(mem.ReadStringW()));
		m_strTime = _tcsdup(W2T(mem.ReadStringW()));
		mem.ReadData(&m_nPerTime,sizeof(int));
		mem.ReadData(&m_nTotalTime,sizeof(int));
		mem.ReadData(&m_fEff,sizeof(float));
	}else
	{
		mem.ReadData(&m_nLogType,sizeof(int));
		m_strIndex = mem.ReadString();
		m_strComputer = mem.ReadString();
		m_strUser = mem.ReadString();
		m_strIP = mem.ReadString();
		m_strTime = mem.ReadString();
		mem.ReadData(&m_nPerTime,sizeof(int));
		mem.ReadData(&m_nTotalTime,sizeof(int));
		mem.ReadData(&m_fEff,sizeof(float));
	}
}

LPTSTR CLogWorkRecord::GetInsertSQL(LPTSTR strSQL)
{
#ifdef DATABASE_SUPPORT	
	TCHAR tcSelectSQL[1024] = {0};
	_stprintf(tcSelectSQL,_T("SELECT workrecord_time, workrecord_worktime, workrecord_totaltime FROM Log_WorkRecord WHERE workrecord_time = '%s' AND workrecord_user = '%s'"), m_strTime, m_strUser);
	CDatabaseConnect::m_pRecordset->Open(_variant_t(tcSelectSQL), _variant_t((IDispatch *)CDatabaseConnect::m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
	
	if(!CDatabaseConnect::m_pRecordset->adoEOF)
	{
		_variant_t vtValue = CDatabaseConnect::m_pRecordset->GetCollect((long)(1));
		LPTSTR strStr = OLE2T(_bstr_t(vtValue));
		int nWorktime = _ttoi(strStr);

		vtValue = CDatabaseConnect::m_pRecordset->GetCollect((long)(2));
		strStr = OLE2T(_bstr_t(vtValue));
		int nTotaltime = _ttoi(strStr);
		
		if(m_nPerTime >= 0)
			nWorktime += m_nPerTime;
		
		nTotaltime += abs(m_nPerTime);
		m_fEff = float(nWorktime)/float(nTotaltime);
		
		_stprintf(strSQL,_T("UPDATE Log_WorkRecord SET workrecord_worktime = %d, workrecord_totaltime = %d, workrecord_efficiency = %f WHERE workrecord_time = '%s' AND workrecord_user = '%s'"), nWorktime, nTotaltime, m_fEff, m_strTime, m_strUser);
	}
	else
	{
		m_nTotalTime = abs(m_nPerTime);
		if(m_nPerTime < 0)
			m_nPerTime = 0;
		
		m_fEff = float(m_nPerTime)/float(m_nTotalTime);
		_stprintf(strSQL,_T("INSERT INTO Log_WorkRecord (workrecord_ip,workrecord_computer,workrecord_time,workrecord_user,workrecord_worktime,workrecord_totaltime,workrecord_efficiency,workrecord_deleted) values ('%s','%s','%s','%s',%d,%d,%f,%d)"),
			m_strIP,m_strComputer,m_strTime,m_strUser,m_nPerTime,m_nTotalTime,m_fEff, 0);
	}
	CDatabaseConnect::m_pRecordset->Close();
#endif	
	return strSQL;
}