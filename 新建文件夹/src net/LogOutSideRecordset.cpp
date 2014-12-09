// LogOutSideRecordset.cpp: implementation of the CLogOutSideRecordset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogOutSideRecordset.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogOutSideRecordset::CLogOutSideRecordset()
{
	m_nRecordType = LOG_OUTSIDE;
}

CLogOutSideRecordset::~CLogOutSideRecordset()
{

}

#ifdef DATABASE_SUPPORT

void CLogOutSideRecordset::ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL)
{
	USES_CONVERSION;
	CDatabaseConnect::m_pRecordset->Open(_variant_t(strTotalSQL), _variant_t((IDispatch *)CDatabaseConnect::m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
	
	if(!CDatabaseConnect::m_pRecordset->adoEOF)
	{
		_variant_t vtValue = CDatabaseConnect::m_pRecordset->GetCollect((long)(0));
		LPTSTR strStr = OLE2T(_bstr_t(vtValue));
		m_nTotalCount = _ttoi(strStr);
	}
	CDatabaseConnect::m_pRecordset->Close();
	
	CDatabaseConnect::m_pRecordset->Open(_variant_t(strSQL), _variant_t((IDispatch *)CDatabaseConnect::m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
	
	while(!(CDatabaseConnect::m_pRecordset->adoEOF))  
	{
		CLogOutSideRecord* pRecord = NULL;
		pRecord = new CLogOutSideRecord;
		
		for(int iIndex = 0; iIndex < CDatabaseConnect::m_pRecordset->GetFields()->GetCount(); iIndex++)
		{
			_variant_t vtValue = CDatabaseConnect::m_pRecordset->GetCollect((long)(iIndex));
			
			if(vtValue.vt != VT_NULL)
			{
				LPTSTR strStr1 = OLE2T(_bstr_t(vtValue));
				switch(iIndex)
				{
				case 0:
					pRecord->m_strIndex = STRDUP(strStr1);
					break;
				case 1:
					pRecord->m_strIP = STRDUP(strStr1);
					break;
				case 2:
					pRecord->m_strComputer = STRDUP(strStr1);
					break;
				case 3:
					pRecord->m_strTime = STRDUP(strStr1);
					break;
				case 4:
					pRecord->m_strUser = STRDUP(strStr1);
					break;
				case 5:
					pRecord->m_strFileName = STRDUP(strStr1);
					break;
				case 6:
					pRecord->m_nOpenCount = _ttoi(strStr1);
					break;
				case 7:
					pRecord->m_nOpenDay = _ttoi(strStr1);
					break;
				case 8:
					pRecord->m_nDeleted = _ttoi(strStr1);
					break;
				case 9:
					pRecord->m_strGUID = STRDUP(strStr1);
					break;
				}
			}
		}
		
		m_dequeLog.push_back(pRecord);
		CDatabaseConnect::m_pRecordset->MoveNext();
	}
	CDatabaseConnect::m_pRecordset->Close();
}
#endif

void  CLogOutSideRecordset::BuildTestData()
{
	CLogFileRecord* pRecord = NULL;
	
	pRecord = new CLogFileRecord;
	pRecord->InitTestData();
	m_dequeLog.push_back(pRecord);
	
	pRecord = new CLogFileRecord;
	pRecord->InitTestData();
	m_dequeLog.push_back(pRecord);
	
	pRecord = new CLogFileRecord;
	pRecord->InitTestData();
	m_dequeLog.push_back(pRecord);
}