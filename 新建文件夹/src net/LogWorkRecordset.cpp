// LogWorkRecordset.cpp: implementation of the CLogWorkRecordset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogWorkRecordset.h"
#include "DatabaseConnect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogWorkRecordset::CLogWorkRecordset()
{
	m_nRecordType = LOG_WORK;
}

CLogWorkRecordset::~CLogWorkRecordset()
{

}

#ifdef DATABASE_SUPPORT
void CLogWorkRecordset::ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL)
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
		CLogWorkRecord* pRecord = NULL;
		pRecord = new CLogWorkRecord;
		
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
					pRecord->m_nPerTime = _ttoi(strStr1);
					break;
				case 6:
					pRecord->m_nTotalTime = _ttoi(strStr1);
					break;
				case 7:
					pRecord->m_fEff = _tcstod(strStr1, NULL);
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
	
void CLogWorkRecordset::BuildTestData()
{
	
}
