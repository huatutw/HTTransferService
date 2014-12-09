// FileAccreditRecordset.cpp: implementation of the CFileAccreditRecordset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileAccreditRecordset.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileAccreditRecordset::CFileAccreditRecordset()
{
	m_nRecordType = FILE_ACCREDIT;
}

CFileAccreditRecordset::~CFileAccreditRecordset()
{

}

#ifdef DATABASE_SUPPORT

void CFileAccreditRecordset::ReadFromDatabase(LPCTSTR strTotalSQL, LPCTSTR strSQL)
{
	CDatabaseConnect::m_pRecordset->Open(_variant_t(strTotalSQL), _variant_t((IDispatch *)CDatabaseConnect::m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);
	m_nTotalCount = CDatabaseConnect::m_pRecordset->RecordCount;

	while(!(CDatabaseConnect::m_pRecordset->adoEOF))  
	{
		CFileAccreditRecord* pRecord = NULL;
		pRecord = new CFileAccreditRecord;
		
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
					pRecord->m_strFileName = STRDUP(strStr1);
					break;
				case 6:
					pRecord->m_strCanopenuser = STRDUP(strStr1);
					break;
				case 7:
					pRecord->m_strFileGuid = STRDUP(strStr1);
					break;
				case 8:
					pRecord->m_nPurviewOper = _ttoi(strStr1);
					break;
				case 9:
					pRecord->m_strOutTime = STRDUP(strStr1);
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

void  CFileAccreditRecordset::BuildTestData()
{

}
