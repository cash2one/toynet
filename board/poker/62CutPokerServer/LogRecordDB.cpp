// LogRecordDB.cpp : implementation file
//

#include "stdafx.h"
#include "LogRecordDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogRecordDB

// for static member
CString CLogRecordDB::m_DSN;
CString CLogRecordDB::m_Table;
CString CLogRecordDB::m_UID;
CString CLogRecordDB::m_Pass;


IMPLEMENT_DYNAMIC(CLogRecordDB, CRecordset)

CLogRecordDB::CLogRecordDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	COleDateTime TempDate; 
	TempDate.SetDateTime(0,0,0,0,0,0);
	//{{AFX_FIELD_INIT(CLogRecordDB)
	m_MyLoseMoney = _T("");
	m_My_IP = _T("");
	m_Winner_IP = _T("");
	m_WinGetMoney = _T("");
	m_WinCurMoney = _T("");
	m_WinnerID = _T("");
	m_MyCurMoney = _T("");
	m_MyID = _T("");
	m_No = 0;
	m_Status = 0;
	m_GameCode = 0;
	m_nFields = 11;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CLogRecordDB::GetDefaultConnect()
{
	//return _T("ODBC;DSN=NetmarbleTestDB");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CLogRecordDB::GetDefaultSQL()
{
	//return _T("[LogRecordDB]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CLogRecordDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CLogRecordDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[My_LoseMoney]"), m_MyLoseMoney);
	RFX_Text(pFX, _T("[My_IP]"), m_My_IP);
	RFX_Text(pFX, _T("[Winner_IP]"), m_Winner_IP);
	RFX_Text(pFX, _T("[Win_GetMoney]"), m_WinGetMoney);
	RFX_Text(pFX, _T("[Win_CurMoney]"), m_WinCurMoney);
	RFX_Text(pFX, _T("[Win_ID]"), m_WinnerID);
	RFX_Text(pFX, _T("[My_CurMoney]"), m_MyCurMoney);
	RFX_Text(pFX, _T("[My_ID]"), m_MyID);
	RFX_Long(pFX, _T("[No]"), m_No);
	RFX_Int(pFX, _T("[Status]"), m_Status);
	RFX_Int(pFX, _T("[GameCode]"), m_GameCode);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CLogRecordDB diagnostics

#ifdef _DEBUG
void CLogRecordDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CLogRecordDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
