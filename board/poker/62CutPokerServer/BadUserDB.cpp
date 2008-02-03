// BadUserDB.cpp : implementation file
//

#include "stdafx.h"
#include "BadUserDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBadUserDB

// for static member
CString CBadUserDB::m_DSN;
CString CBadUserDB::m_Table;
CString CBadUserDB::m_UID;
CString CBadUserDB::m_Pass;


IMPLEMENT_DYNAMIC(CBadUserDB, CRecordset)

CBadUserDB::CBadUserDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CBadUserDB)
	m_NO = 0;
	m_MyID = _T("");
	m_TargetID = _T("");
	m_GameCode = 0;
	m_ServerCode = 0;
	m_SubmitDate = _T("");
	m_SubmitReason = _T("");
	m_DealDate = _T("");
	m_DealResult = 0;
	m_DealReason = 0;
	m_WatchID = _T("");
	m_Contents = _T("");
	m_nFields = 12;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CBadUserDB::GetDefaultConnect()
{
	//return _T("ODBC;DSN=NetmarbleUserDB;UID=Sqladmin;PWD=net885;");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CBadUserDB::GetDefaultSQL()
{
	//return _T("[dbo].[BadIDBulletinBoard]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CBadUserDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CBadUserDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[NO]"), m_NO);
	RFX_Text(pFX, _T("[MyID]"), m_MyID);
	RFX_Text(pFX, _T("[TargetID]"), m_TargetID);
	RFX_Long(pFX, _T("[GameCode]"), m_GameCode);
	RFX_Long(pFX, _T("[ServerCode]"), m_ServerCode);
	RFX_Text(pFX, _T("[SubmitDate]"), m_SubmitDate);
	RFX_Text(pFX, _T("[SubmitReason]"), m_SubmitReason);
	RFX_Text(pFX, _T("[DealDate]"), m_DealDate);
	RFX_Long(pFX, _T("[DealResult]"), m_DealResult);
	RFX_Long(pFX, _T("[DealReason]"), m_DealReason);
	RFX_Text(pFX, _T("[WatchID]"), m_WatchID);
	RFX_Text(pFX, _T("[Contents]"), m_Contents, 4000);	// 최대 길이 지정
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CBadUserDB diagnostics

#ifdef _DEBUG
void CBadUserDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CBadUserDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
