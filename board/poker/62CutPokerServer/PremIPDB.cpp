// PremIPDB.cpp : implementation file
//

#include "stdafx.h"
#include "PremIPDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPremIPDB

// for static member
CString CPremIPDB::m_DSN;
CString CPremIPDB::m_Table;
CString CPremIPDB::m_UID;
CString CPremIPDB::m_Pass;


IMPLEMENT_DYNAMIC(CPremIPDB, CRecordset)

CPremIPDB::CPremIPDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CPremIPDB)
	m_IP = _T("");
	m_Flag = 0;

	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CPremIPDB::GetDefaultConnect()
{
	//return _T("ODBC;DSN=PremIPDB;UID=Sqladmin;PWD=net885;");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CPremIPDB::GetDefaultSQL()
{
	//return _T("[PremIPDB]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CPremIPDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CPremIPDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[ip]"), m_IP);
	RFX_Long(pFX, _T("[flag]"), m_Flag);

	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPremIPDB diagnostics

#ifdef _DEBUG
void CPremIPDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPremIPDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
