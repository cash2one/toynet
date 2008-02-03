// Pre_UserLimitGMLogDB.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "62CutPokerServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPre_UserLimitGMLogDB
CString CPre_UserLimitGMLogDB::m_DSN;
CString CPre_UserLimitGMLogDB::m_Table;
CString CPre_UserLimitGMLogDB::m_UID;
CString CPre_UserLimitGMLogDB::m_Pass;

IMPLEMENT_DYNAMIC(CPre_UserLimitGMLogDB, CRecordset)

CPre_UserLimitGMLogDB::CPre_UserLimitGMLogDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CPre_UserLimitGMLogDB)
	m_NO = _T("");
	m_ID = _T("");
	m_MoneyType = 0;
	m_Amount = 0;
	m_BeforeM = 0;
	m_AfterM = 0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CPre_UserLimitGMLogDB::GetDefaultConnect()
{
	//return _T("ODBC;DSN=NetmarbleTestDB");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CPre_UserLimitGMLogDB::GetDefaultSQL()
{
	//return _T("[Pre_UserLimitGMLog]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CPre_UserLimitGMLogDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CPre_UserLimitGMLogDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[NO]"), m_NO);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[MoneyType]"), m_MoneyType);
	RFX_Long(pFX, _T("[Amount]"), m_Amount);
	RFX_Long(pFX, _T("[BeforeM]"), m_BeforeM);
	RFX_Long(pFX, _T("[AfterM]"), m_AfterM);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPre_UserLimitGMLogDB diagnostics

#ifdef _DEBUG
void CPre_UserLimitGMLogDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPre_UserLimitGMLogDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG