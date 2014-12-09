// LogLandRecordset.cpp: implementation of the CLogLandRecordset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogLandRecordset.h"
#include "LogLandRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogLandRecordset::CLogLandRecordset()
{
	m_nRecordType = LOG_LAND;
}

CLogLandRecordset::~CLogLandRecordset()
{

}

#ifdef DATABASE_SUPPORT

void CLogLandRecordset::ReadFromDatabase(LPCTSTR strTotalSQL,LPCTSTR strSQL)
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
		CLogLandRecord* pRecord = NULL;
		pRecord = new CLogLandRecord;
		
		for(int iIndex = 0; iIndex < CDatabaseConnect::m_pRecordset->GetFields()->GetCount(); iIndex++)
		{
			USES_CONVERSION;
			
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
					pRecord->m_nOperType = _ttoi(strStr1);
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

void  CLogLandRecordset::BuildTestData()
{

}