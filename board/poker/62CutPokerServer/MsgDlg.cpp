// MsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "MsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg dialog


CMsgDlg::CMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgDlg)
	m_strMsg = _T("");
	//}}AFX_DATA_INIT
}


void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgDlg)
	DDX_Text(pDX, IDC_STATIC_MSG, m_strMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg message handlers

BOOL CMsgDlg::Create(CWnd* pParentWnd, LPCTSTR pstrMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_strMsg = pstrMsg;
	BOOL bRtn = CDialog::Create(IDD_DIALOG_MSG, pParentWnd);
	ShowWindow(SW_SHOW);
	return bRtn;
}

BOOL CMsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMsgDlg::OnOK() 
{
	// TODO: Add extra validation here
	//DestroyWindow();
	//CDialog::OnOK();
}

void CMsgDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
	//CDialog::OnCancel();
}

void CMsgDlg::SetMsg(LPCTSTR pstrMsg) 
{
	m_strMsg = pstrMsg;
	UpdateData(FALSE);
}
