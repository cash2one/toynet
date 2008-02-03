// EventPrizeDB.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "EventPrizeDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventPrizeDB

// for static member
CString CEventPrizeDB::m_DSN;
CString CEventPrizeDB::m_Table;
CString CEventPrizeDB::m_UID;
CString CEventPrizeDB::m_Pass;

IMPLEMENT_DYNAMIC(CEventPrizeDB, CRecordset)

CEventPrizeDB::CEventPrizeDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	COleDateTime TempDate; 
	TempDate.SetDateTime(0,0,0,0,0,0);
	//{{AFX_FIELD_INIT(CEventPrizeDB)
	m_ID = _T("");
	m_LastPlayDate = TempDate;
	m_GameCount = 0;
	m_EventCoin = 0;
	m_GameCode = 0;

	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CEventPrizeDB::GetDefaultConnect()
{
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
	//return _T("ODBC;DSN=NetmarbleTestDB");
}

CString CEventPrizeDB::GetDefaultSQL()
{
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;

	//return _T("[EventTicketTable]");
}

void CEventPrizeDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CEventPrizeDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Date(pFX, _T("[LastPlayDate]"), m_LastPlayDate);
	RFX_Long(pFX, _T("[GameCount]"), m_GameCount);
	RFX_Long(pFX, _T("[EventCoin]"), m_EventCoin);
	RFX_Long(pFX, _T("[GameCode]"), m_GameCode);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CEventPrizeDB diagnostics

#ifdef _DEBUG
void CEventPrizeDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CEventPrizeDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
