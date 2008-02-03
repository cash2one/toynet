// StatusDB.cpp : implementation file
//

#include "stdafx.h"
#include "StatusDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusDB

// for static member
CString CStatusDB::m_DSN;
CString CStatusDB::m_Table;
CString CStatusDB::m_UID;
CString CStatusDB::m_Pass;


IMPLEMENT_DYNAMIC(CStatusDB, CRecordset)

CStatusDB::CStatusDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CStatusDB)
	m_GameCode = 0;
	m_ServerCode = 0;
	m_UserNum = 0;
	m_ServerIP = _T("");
	m_ServerPort = 0;
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CStatusDB::GetDefaultConnect()
{
	//return _T("ODBC;DSN=NetmarbleUserDB;UID=Sqladmin;PWD=net885;");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CStatusDB::GetDefaultSQL()
{
	//return _T("[dbo].[ConnectStatusDB]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CStatusDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CStatusDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[GameCode]"), m_GameCode);
	RFX_Long(pFX, _T("[ServerCode]"), m_ServerCode);
	RFX_Long(pFX, _T("[UserNum]"), m_UserNum);
	RFX_Text(pFX, _T("[ServerIP]"), m_ServerIP);
	RFX_Long(pFX, _T("[ServerPort]"), m_ServerPort);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CStatusDB diagnostics

#ifdef _DEBUG
void CStatusDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CStatusDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
