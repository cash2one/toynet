// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "LoginDlg.h"

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_ID = _T("");
	m_Pass = _T("");
	m_ServerIP = _T("");
	m_ServerPort = 0;
	//}}AFX_DATA_INIT
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Text(pDX, IDC_EDIT_ID, m_ID);
	DDV_MaxChars(pDX, m_ID, 12);
	DDX_Text(pDX, IDC_EDIT_PASS, m_Pass);
	DDV_MaxChars(pDX, m_Pass, 12);				// [SSO 작업] - 수정
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_ServerIP);
	DDV_MaxChars(pDX, m_ServerIP, 20);
	DDX_Text(pDX, IDC_EDIT_SERVERPORT, m_ServerPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	g_Where = WH_FIRST;

	m_ID = g_LoginID;
	m_Pass = g_LoginPass;
	m_ServerIP = g_ServIP;
	m_ServerPort = g_ServPort;

	UpdateData(FALSE);

	GetDlgItem(IDC_EDIT_ID)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(g_Where != WH_FIRST) return;
	UpdateData(TRUE);
	if(m_ID.GetLength()==0) { AfxMessageBox("아이디를 입력하세요"); return; }
	if(m_Pass.GetLength()==0) { AfxMessageBox("비밀번호를 입력하세요"); return; }

	CDialog::OnOK();
}

void CLoginDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

