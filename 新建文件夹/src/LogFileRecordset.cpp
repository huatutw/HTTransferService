// LogFileRecordset.cpp: implementation of the CLogFileRecordset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFileRecordset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFileRecordset::CLogFileRecordset()
{
	m_nRecordType = LOG_FILE;
}

CLogFileRecordset::~CLogFileRecordset()
{
	
}

#ifdef DATABASE_SUPPORT

void CLogFileRecordset::ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL)
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
		CLogFileRecord* pRecord = NULL;
		pRecord = new CLogFileRecord;
		
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
					pRecord->m_nOperType = _ttoi(strStr1);
					break;
				case 7:
					pRecord->m_nInternalError = _ttoi(strStr1);
					break;
				case 8:
					pRecord->m_nSystemError = _ttoi(strStr1);
					break;
				case 9:
					pRecord->m_nDeleted = _ttoi(strStr1);
					break;
				case 10:
					pRecord->m_strGUID = STRDUP(strStr1);
					break;
				case 11:
					pRecord->m_strServerFilePath = STRDUP(strStr1);
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

void  CLogFileRecordset::BuildTestData()
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