// ItemDB.cpp : implementation file
//

#include "stdafx.h"
#include "ItemDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemDB

// for static member
CString CItemDB::m_DSN;
CString CItemDB::m_Table;
CString CItemDB::m_UID;
CString CItemDB::m_Pass;


IMPLEMENT_DYNAMIC(CItemDB, CRecordset)

CItemDB::CItemDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	COleDateTime TempDate; 
	TempDate.SetDateTime(0,0,0,0,0,0);
	//m_StartDate.SetDateTime(0,0,0,0,0,0);

	//{{AFX_FIELD_INIT(CItemDB)

	m_NO = 0;
	m_ID = _T("");
	m_PresID = _T("");
	m_Code = 0;
	m_GameCode = 0;
	m_UsedGameCode = 0;
	m_UseDay = 0;
	m_UseNum = 0;
	m_StartDate = TempDate;
	m_Use = 0;

	m_nFields = 10;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CItemDB::GetDefaultConnect()
{
	//return _T("ODBC;DSN=NetmarbleItemDB;UID=Sqladmin;PWD=net885;");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CItemDB::GetDefaultSQL()
{
	//return _T("[MainItemDB]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CItemDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CItemDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[c_no]"), m_NO);
	RFX_Text(pFX, _T("[id]"), m_ID);
	RFX_Text(pFX, _T("[c_present]"), m_PresID);
	RFX_Long(pFX, _T("[c_code]"), m_Code);
	RFX_Long(pFX, _T("[c_gametype]"), m_GameCode);
	RFX_Long(pFX, _T("[c_used_gametype]"), m_UsedGameCode);
	RFX_Date(pFX, _T("[c_start_date]"), m_StartDate);
	RFX_Long(pFX, _T("[c_use]"), m_Use);
	RFX_Long(pFX, _T("[c_use_day]"), m_UseDay);
	RFX_Long(pFX, _T("[c_use_num]"), m_UseNum);

	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CItemDB diagnostics

#ifdef _DEBUG
void CItemDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CItemDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
