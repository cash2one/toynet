// PremiumDB.cpp : implementation file
//

#include "stdafx.h"
#include "PremiumDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPremiumDB

// for static member
CString CPremiumDB::m_DSN;
CString CPremiumDB::m_Table;
CString CPremiumDB::m_UID;
CString CPremiumDB::m_Pass;


IMPLEMENT_DYNAMIC(CPremiumDB, CRecordset)

CPremiumDB::CPremiumDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	COleDateTime TempDate; 
	TempDate.SetDateTime(0,0,0,0,0,0);
	//{{AFX_FIELD_INIT(CPremiumDB)
	m_UniqNO = _T("");
	m_PremCode = 0;
	m_PremDate = TempDate;
	m_LeadersCode = 0;			//2004.05.07 리더스총액 제한
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CPremiumDB::GetDefaultConnect()
{
	//return _T("ODBC;DSN=NetmarblePremiumDB;UID=Sqladmin;PWD=net885;");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CPremiumDB::GetDefaultSQL()
{
	//return _T("[MainPremiumDB]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CPremiumDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CPremiumDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[NO]"), m_UniqNO);
	RFX_Long(pFX, _T("[Prem_Code]"), m_PremCode);
	RFX_Date(pFX, _T("[Prem_Date]"), m_PremDate);
	RFX_Long(pFX, _T("[Menu_No]"), m_LeadersCode);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPremiumDB diagnostics

#ifdef _DEBUG
void CPremiumDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPremiumDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
