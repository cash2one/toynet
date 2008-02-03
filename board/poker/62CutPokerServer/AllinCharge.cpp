// AllinCharge.cpp : implementation file
//
// [올인시 충전]
#include "stdafx.h"
#include "62CutPokerServer.h"
#include "AllinCharge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllinCharge
CString CAllinCharge::m_DSN;
CString CAllinCharge::m_Table;
CString CAllinCharge::m_UID;
CString CAllinCharge::m_Pass;


IMPLEMENT_DYNAMIC(CAllinCharge, CRecordset)

CAllinCharge::CAllinCharge(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CAllinCharge)
	m_INDEX = 0;
	m_ID = _T("");
	m_OLDMONEY = _T("");
	m_AMOUNTM = _T("");
	m_NEWMONEY = _T("");
	m_GAMECODE = 0;
	m_nFields = 7;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CAllinCharge::GetDefaultConnect()
{
	//return _T("ODBC;DSN=TestDB_BDGostop");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CAllinCharge::GetDefaultSQL()
{
	//return _T("[AllinCharge]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CAllinCharge::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CAllinCharge)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[IDX]"), m_INDEX);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[BeforeM]"), m_OLDMONEY);
	RFX_Text(pFX, _T("[AMountM]"), m_AMOUNTM);
	RFX_Text(pFX, _T("[AfterM]"), m_NEWMONEY);
	RFX_Date(pFX, _T("[RegDate]"), m_DATE);
	RFX_Long(pFX, _T("[GAMECODE]"), m_GAMECODE);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CAllinCharge diagnostics

#ifdef _DEBUG
void CAllinCharge::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAllinCharge::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
