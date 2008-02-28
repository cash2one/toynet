// StatisticDB.cpp : implementation file
//

#include "stdafx.h"
//#include "gostop.h"
#include "StatisticDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticDB

// for static member
CString CStatisticDB::m_DSN;
CString CStatisticDB::m_Table;
CString CStatisticDB::m_UID;
CString CStatisticDB::m_Pass;

IMPLEMENT_DYNAMIC(CStatisticDB, CRecordset)

CStatisticDB::CStatisticDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CStatisticDB)
	m_KeyIndex = 0;
	m_GameCode = 0;
	m_ServerCode = 0;
	m_CreditPoint = 0;
	m_UsePoint = 0;
	m_BankPoint = 0;
	m_LogTime = _T("");
	m_nFields = 7;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CStatisticDB::GetDefaultConnect()
{
//	return _T("ODBC;DSN=OrgoMatgoDB");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CStatisticDB::GetDefaultSQL()
{
//	return _T("[tb_statistic]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CStatisticDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CStatisticDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[KeyIndex]"), m_KeyIndex);
	RFX_Long(pFX, _T("[GameCode]"), m_GameCode);
	RFX_Long(pFX, _T("[ServerCode]"), m_ServerCode);
	RFX_Long(pFX, _T("[CreditPoint]"), m_CreditPoint);
	RFX_Long(pFX, _T("[UsePoint]"), m_UsePoint);
	RFX_Long(pFX, _T("[BankPoint]"), m_BankPoint);
	RFX_Text(pFX, _T("[LogTime]"), m_LogTime);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CStatisticDB diagnostics

#ifdef _DEBUG
void CStatisticDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CStatisticDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
