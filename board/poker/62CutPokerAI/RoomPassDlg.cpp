// RoomPassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "RoomPassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoomPassDlg dialog


CRoomPassDlg::CRoomPassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoomPassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRoomPassDlg)
	m_Pass = _T("");
	//}}AFX_DATA_INIT
}


void CRoomPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoomPassDlg)
	DDX_Text(pDX, IDC_EDIT_PASS, m_Pass);
	DDV_MaxChars(pDX, m_Pass, 4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRoomPassDlg, CDialog)
	//{{AFX_MSG_MAP(CRoomPassDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoomPassDlg message handlers

BOOL CRoomPassDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT_PASS)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRoomPassDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CRoomPassDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
