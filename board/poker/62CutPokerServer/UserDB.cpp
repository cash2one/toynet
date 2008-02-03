// UserDB.cpp : implementation file
//

#include "stdafx.h"
#include "UserDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserDB

// for static member
CString CUserDB::m_DSN;
CString CUserDB::m_Table;
CString CUserDB::m_UID;
CString CUserDB::m_Pass;


IMPLEMENT_DYNAMIC(CUserDB, CRecordset)

CUserDB::CUserDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CUserDB)
	m_UniqNO = _T("");		// [프리미엄 작업]
	m_ID = _T("");
	m_Passwd = _T("");
	m_Sex = FALSE;
	m_Desc = _T("");
	m_IDState = 0;
	m_RegID = _T("");		// [게임 아이템 작업]
	m_Certificate = 0;	// [성인인증]
	m_nFields = 8;			// [게임 아이템 작업]	[프리미엄 작업]
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CUserDB::GetDefaultConnect()
{
	//return _T("ODBC;DSN=NetmarbleUserDB;UID=Sqladmin;PWD=net885;");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CUserDB::GetDefaultSQL()
{
	//return _T("[MainUserDB]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CUserDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CUserDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[NO]"), m_UniqNO);			// [프리미엄 작업]
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Passwd]"), m_Passwd);
	RFX_Bool(pFX, _T("[Sex]"), m_Sex);
	RFX_Text(pFX, _T("[Desc]"), m_Desc);
	RFX_Byte(pFX, _T("[IDState]"), m_IDState);
	RFX_Text(pFX, _T("[RegID]"), m_RegID);			// [게임 아이템 작업]
	RFX_Bool(pFX, _T("[Certificate]"), m_Certificate);	// [성인인증]
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CUserDB diagnostics

#ifdef _DEBUG
void CUserDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CUserDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
