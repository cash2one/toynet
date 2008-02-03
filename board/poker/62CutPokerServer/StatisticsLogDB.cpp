// StatisticsLogDB.cpp : implementation file
//

#include "stdafx.h"
#include "StatisticsLogDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsLogDB
// for static member
CString CStatisticsLogDB::m_DSN;
CString CStatisticsLogDB::m_Table;
CString CStatisticsLogDB::m_UID;
CString CStatisticsLogDB::m_Pass;

IMPLEMENT_DYNAMIC(CStatisticsLogDB, CRecordset)

CStatisticsLogDB::CStatisticsLogDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	COleDateTime TempDate; 
	TempDate.SetDateTime(0,0,0,0,0,0);
	//{{AFX_FIELD_INIT(CStatisticsLogDB)
	m_c_GameCode = 0;
	m_c_ServerCode = 0;
	m_c_Kind = 0;
	m_c_BigData_0 = _T("");
	m_c_BigData_1 = _T("");
	m_c_BigData_2 = _T("");
	m_c_BigData_3 = _T("");
	m_c_BigData_4 = _T("");
	m_c_SmallData_0 = 0;
	m_c_SmallData_1 = 0;
	m_c_SmallData_2 = 0;
	m_c_SmallData_3 = 0;
	m_c_SmallData_4 = 0;
	m_c_SmallData_5 = 0;
	m_c_SmallData_6 = 0;
	m_c_SmallData_7 = 0;
	m_c_SmallData_8 = 0;
	m_c_SmallData_9 = 0;
	m_c_SmallData_10 = 0;
	m_c_SmallData_11 = 0;
	m_c_SmallData_12 = 0;
	m_c_SmallData_13 = 0;
	m_c_SmallData_14 = 0;
	m_c_SmallData_15 = 0;
	m_c_SmallData_16 = 0;
	m_c_SmallData_17 = 0;
	m_c_SmallData_18 = 0;
	m_c_SmallData_19 = 0;
	m_Date = TempDate;
	m_c_extra = 0;
	m_nFields = 30;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CStatisticsLogDB::GetDefaultConnect()
{
//	return _T("ODBC;DSN=NetmarbleTestDB");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CStatisticsLogDB::GetDefaultSQL()
{
	//return _T("[LogRecordDB]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CStatisticsLogDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CStatisticsLogDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[c_GameCode]"), m_c_GameCode);
	RFX_Long(pFX, _T("[c_ServerCode]"), m_c_ServerCode);
	RFX_Long(pFX, _T("[c_Kind]"), m_c_Kind);
	RFX_Text(pFX, _T("[c_BigData_0]"), m_c_BigData_0);
	RFX_Text(pFX, _T("[c_BigData_1]"), m_c_BigData_1);
	RFX_Text(pFX, _T("[c_BigData_2]"), m_c_BigData_2);
	RFX_Text(pFX, _T("[c_BigData_3]"), m_c_BigData_3);
	RFX_Text(pFX, _T("[c_BigData_4]"), m_c_BigData_4);
	RFX_Long(pFX, _T("[c_SmallData_0]"), m_c_SmallData_0);
	RFX_Long(pFX, _T("[c_SmallData_1]"), m_c_SmallData_1);
	RFX_Long(pFX, _T("[c_SmallData_2]"), m_c_SmallData_2);
	RFX_Long(pFX, _T("[c_SmallData_3]"), m_c_SmallData_3);
	RFX_Long(pFX, _T("[c_SmallData_4]"), m_c_SmallData_4);
	RFX_Long(pFX, _T("[c_SmallData_5]"), m_c_SmallData_5);
	RFX_Long(pFX, _T("[c_SmallData_6]"), m_c_SmallData_6);
	RFX_Long(pFX, _T("[c_SmallData_7]"), m_c_SmallData_7);
	RFX_Long(pFX, _T("[c_SmallData_8]"), m_c_SmallData_8);
	RFX_Long(pFX, _T("[c_SmallData_9]"), m_c_SmallData_9);
	RFX_Long(pFX, _T("[c_SmallData_10]"), m_c_SmallData_10);
	RFX_Long(pFX, _T("[c_SmallData_11]"), m_c_SmallData_11);
	RFX_Long(pFX, _T("[c_SmallData_12]"), m_c_SmallData_12);
	RFX_Long(pFX, _T("[c_SmallData_13]"), m_c_SmallData_13);
	RFX_Long(pFX, _T("[c_SmallData_14]"), m_c_SmallData_14);
	RFX_Long(pFX, _T("[c_SmallData_15]"), m_c_SmallData_15);
	RFX_Long(pFX, _T("[c_SmallData_16]"), m_c_SmallData_16);
	RFX_Long(pFX, _T("[c_SmallData_17]"), m_c_SmallData_17);
	RFX_Long(pFX, _T("[c_SmallData_18]"), m_c_SmallData_18);
	RFX_Long(pFX, _T("[c_SmallData_19]"), m_c_SmallData_19);
	RFX_Date(pFX, _T("[RegDate]"), m_Date);
	RFX_Long(pFX, _T("[c_extra]"), m_c_extra);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CStatisticsLogDB diagnostics

#ifdef _DEBUG
void CStatisticsLogDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CStatisticsLogDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
