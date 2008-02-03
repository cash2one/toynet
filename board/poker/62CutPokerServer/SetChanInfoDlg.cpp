// SetChanInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "SetChanInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetChanInfoDlg dialog


CSetChanInfoDlg::CSetChanInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetChanInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetChanInfoDlg)
	m_ChanName = _T("");
	m_GameKind = 0;
	m_Limit1 = 0;
	m_Limit2 = 0;
	m_Grade = 0;
	//}}AFX_DATA_INIT
}


void CSetChanInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetChanInfoDlg)
	DDX_Text(pDX, IDC_EDIT_CHANNAME, m_ChanName);
	DDV_MaxChars(pDX, m_ChanName, 19);
	DDX_Text(pDX, IDC_EDIT_GAMEKIND, m_GameKind);
	DDX_Text(pDX, IDC_EDIT_LIMIT1, m_Limit1);
	DDX_Text(pDX, IDC_EDIT_LIMIT2, m_Limit2);
	DDX_Text(pDX, IDC_EDIT_GRADE, m_Grade);
	DDV_MinMaxInt(pDX, m_Grade, 1, 4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetChanInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSetChanInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetChanInfoDlg message handlers

BOOL CSetChanInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetChanInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CSetChanInfoDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
