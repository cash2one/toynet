// GameDB.cpp : implementation file
//

#include "stdafx.h"
#include "GameDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameDB

// for static member
CString CGameDB::m_DSN;
CString CGameDB::m_Table;
CString CGameDB::m_UID;
CString CGameDB::m_Pass;


IMPLEMENT_DYNAMIC(CGameDB, CRecordset)

CGameDB::CGameDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CGameDB)
	m_ID      = _T("");
	m_Vic     = 0;
	m_RSF        = 0;
	m_SF         = 0;
	m_FCARD      = 0;
	m_CARD04     = FALSE;
	m_CARD05     = FALSE;
	m_CARD06     = FALSE;
	m_CARD07     = FALSE;
	m_CARD08     = FALSE;
	m_CARD09     = FALSE;
	m_CARD10     = FALSE;
	m_CARD11     = FALSE;
	m_CARD12     = FALSE;
	m_CARD13     = FALSE;
	m_CARD14     = FALSE;
	m_CARD15     = FALSE;
	m_PMoney      = _T("0");
	m_LMoney	 = _T("0");	// [수호천사]
	m_History1 = _T("");
	m_History2 = _T("");
	m_History3 = _T("");
	m_HistoryIndex = 0;
	m_Fail = 0;
	m_Fold = 0;
	m_IsConnected = 0;
	m_Level = 0;
	m_Level2 = 0;
	m_WinPer = 0;
	m_nFields = 29;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CGameDB::GetDefaultConnect()
{
	//return _T("ODBC;DSN=NetmarbleUserDB;UID=Sqladmin;PWD=net885;");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CGameDB::GetDefaultSQL()
{
	//return _T("[TetrisUserDB]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CGameDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CGameDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[Vic]"), m_Vic);
	RFX_Long(pFX, _T("[RSF]"), m_RSF);
	RFX_Long(pFX, _T("[SF]"), m_SF);
	RFX_Long(pFX, _T("[FCARD]"), m_FCARD);
	RFX_Bool(pFX, _T("[CARD04]"), m_CARD04);
	RFX_Bool(pFX, _T("[CARD05]"), m_CARD05);
	RFX_Bool(pFX, _T("[CARD06]"), m_CARD06);
	RFX_Bool(pFX, _T("[CARD07]"), m_CARD07);
	RFX_Bool(pFX, _T("[CARD08]"), m_CARD08);
	RFX_Bool(pFX, _T("[CARD09]"), m_CARD09);
	RFX_Bool(pFX, _T("[CARD10]"), m_CARD10);
	RFX_Bool(pFX, _T("[CARD11]"), m_CARD11);
	RFX_Bool(pFX, _T("[CARD12]"), m_CARD12);
	RFX_Bool(pFX, _T("[CARD13]"), m_CARD13);
	RFX_Bool(pFX, _T("[CARD14]"), m_CARD14);
	RFX_Bool(pFX, _T("[CARD15]"), m_CARD15);
	RFX_Text(pFX, _T("[PMoney]"), m_PMoney);
	RFX_Text(pFX, _T("[LostMoney]"), m_LMoney);		// [수호천사]
	RFX_Text(pFX, _T("[History1]"), m_History1);
	RFX_Text(pFX, _T("[History2]"), m_History2);
	RFX_Text(pFX, _T("[History3]"), m_History3);
	RFX_Byte(pFX, _T("[HistoryIndex]"), m_HistoryIndex);
	RFX_Long(pFX, _T("[Fail]"), m_Fail);
	RFX_Long(pFX, _T("[Fold]"), m_Fold);
	RFX_Byte(pFX, _T("[IsConnected]"), m_IsConnected);
	RFX_Byte(pFX, _T("[Level]"), m_Level);
	RFX_Byte(pFX, _T("[Level2]"), m_Level2);
	RFX_Byte(pFX, _T("[WinPer]"), m_WinPer);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CGameDB diagnostics

#ifdef _DEBUG
void CGameDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CGameDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
