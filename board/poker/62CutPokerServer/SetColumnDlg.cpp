// SetColumnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "SetColumnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetColumnDlg dialog


CSetColumnDlg::CSetColumnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetColumnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetColumnDlg)
	m_Value = _T("");
	m_ColumnName = _T("");
	m_bAutoIndex = FALSE;
	m_StartVal = 1;
	m_Cipher = 0;
	//}}AFX_DATA_INIT
}


void CSetColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetColumnDlg)
	DDX_Text(pDX, IDC_EDIT_VALUE, m_Value);
	DDV_MaxChars(pDX, m_Value, 19);
	DDX_Text(pDX, IDC_STATIC_COLUMNNAME, m_ColumnName);
	DDX_Check(pDX, IDC_CHECK_AUTOINDEX, m_bAutoIndex);
	DDX_Text(pDX, IDC_EDIT_STARTVAL, m_StartVal);
	DDX_Text(pDX, IDC_EDIT_CIPHER, m_Cipher);
	DDV_MinMaxInt(pDX, m_Cipher, 0, 10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetColumnDlg, CDialog)
	//{{AFX_MSG_MAP(CSetColumnDlg)
	ON_BN_CLICKED(IDC_CHECK_AUTOINDEX, OnCheckAutoindex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetColumnDlg message handlers

BOOL CSetColumnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
	/////////// 여기부터 
	if(m_bDontEnableAutoIndexCheck) {
		m_bAutoIndex = FALSE;
		GetDlgItem(IDC_CHECK_AUTOINDEX)->EnableWindow(FALSE);
	}

	EnableAutoIndex(m_bAutoIndex);
	
	GetDlgItem(IDC_EDIT_VALUE)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetColumnDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CSetColumnDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CSetColumnDlg::EnableAutoIndex(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_STARTVAL)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_CIPHER)->EnableWindow(bEnable);
}

void CSetColumnDlg::OnCheckAutoindex() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EnableAutoIndex(m_bAutoIndex);
}

