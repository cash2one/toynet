// AvatarDB.cpp : implementation file
//

#include "stdafx.h"
#include "AvatarDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAvatarDB

// for static member
CString CAvatarDB::m_DSN;
CString CAvatarDB::m_Table;
CString CAvatarDB::m_UID;
CString CAvatarDB::m_Pass;


IMPLEMENT_DYNAMIC(CAvatarDB, CRecordset)

CAvatarDB::CAvatarDB(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CAvatarDB)
	m_ID = _T("");
	m_layer0 = 0;
	m_layer1 = 0;
	m_layer2 = 0;
	m_layer3 = 0;
	m_layer4 = 0;
	m_layer5 = 0;
	m_layer6 = 0;
	m_layer7 = 0;
	m_layer8 = 0;
	m_layer9 = 0;
	m_layer10 = 0;
	m_layer11 = 0;
	m_layer12 = 0;
	m_layer13 = 0;
	m_layer14 = 0;
	m_layer15 = 0;
	m_layer16 = 0;
	m_layer17 = 0;
	m_layer18 = 0;
	m_layer19 = 0;
	m_layer20 = 0;
	m_layer21 = 0;
	m_layer22 = 0;
	m_layer23 = 0;
	m_layer24 = 0;
	m_layer25 = 0;

	m_nFields = 27;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CAvatarDB::GetDefaultConnect()
{
	//return _T("ODBC;DSN=NetmarbleAvatarDB;UID=Sqladmin;PWD=net885;");
	CString rtn;
	rtn.Format("ODBC;DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)m_DSN, (LPCTSTR)m_UID, (LPCTSTR)m_Pass);
	return rtn;
}

CString CAvatarDB::GetDefaultSQL()
{
	//return _T("[MainAvatarDB]");
	CString rtn;
	rtn.Format("%s", (LPCTSTR)m_Table);
	return rtn;
}

void CAvatarDB::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CAvatarDB)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[id]"), m_ID);
	RFX_Long(pFX, _T("[layer0]"), m_layer0);
	RFX_Long(pFX, _T("[layer1]"), m_layer1);
	RFX_Long(pFX, _T("[layer2]"), m_layer2);
	RFX_Long(pFX, _T("[layer3]"), m_layer3);
	RFX_Long(pFX, _T("[layer4]"), m_layer4);
	RFX_Long(pFX, _T("[layer5]"), m_layer5);
	RFX_Long(pFX, _T("[layer6]"), m_layer6);
	RFX_Long(pFX, _T("[layer7]"), m_layer7);
	RFX_Long(pFX, _T("[layer8]"), m_layer8);
	RFX_Long(pFX, _T("[layer9]"), m_layer9);
	RFX_Long(pFX, _T("[layer10]"), m_layer10);
	RFX_Long(pFX, _T("[layer11]"), m_layer11);
	RFX_Long(pFX, _T("[layer12]"), m_layer12);
	RFX_Long(pFX, _T("[layer13]"), m_layer13);
	RFX_Long(pFX, _T("[layer14]"), m_layer14);
	RFX_Long(pFX, _T("[layer15]"), m_layer15);
	RFX_Long(pFX, _T("[layer16]"), m_layer16);
	RFX_Long(pFX, _T("[layer17]"), m_layer17);
	RFX_Long(pFX, _T("[layer18]"), m_layer18);
	RFX_Long(pFX, _T("[layer19]"), m_layer19);
	RFX_Long(pFX, _T("[layer20]"), m_layer20);
	RFX_Long(pFX, _T("[layer21]"), m_layer21);
	RFX_Long(pFX, _T("[layer22]"), m_layer22);
	RFX_Long(pFX, _T("[layer23]"), m_layer23);
	RFX_Long(pFX, _T("[layer24]"), m_layer24);
	RFX_Long(pFX, _T("[layer25]"), m_layer25);

	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CAvatarDB diagnostics

#ifdef _DEBUG
void CAvatarDB::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAvatarDB::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
