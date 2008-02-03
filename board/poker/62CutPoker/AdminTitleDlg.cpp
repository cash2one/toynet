// AdminTitleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "AdminTitleDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminTitleDlg dialog


CAdminTitleDlg::CAdminTitleDlg(CString strName, CWnd* pParent /*=NULL*/)
	: CDialog(CAdminTitleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdminTitleDlg)
	m_strPrevTitle = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CAdminTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminTitleDlg)
	DDX_Control(pDX, IDC_COMBO_NAME, m_ctrlTitle);
	DDX_Text(pDX, IDC_EDIT_ROOMNAME, m_strPrevTitle);
	DDX_CBString(pDX, IDC_COMBO_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminTitleDlg, CDialog)
	//{{AFX_MSG_MAP(CAdminTitleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminTitleDlg message handlers

BOOL CAdminTitleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME1")));
	m_ctrlTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME2")));
	m_ctrlTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME3")));
	m_ctrlTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME4")));
	m_ctrlTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME5")));
	m_ctrlTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME6")));
	m_ctrlTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME7")));
	m_ctrlTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME8")));
	m_ctrlTitle.AddString((LPCTSTR)g_StrMan.Get(_T("RNAME9")));

	m_strName = g_StrMan.Get(_T("RNAME1"));
	//m_ctrlTitle.SelectString( 0, (LPCTSTR)g_StrMan.Get(_T("RNAME9")) );
	

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CAdminTitleDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CAdminTitleDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_strName.GetLength()==0 || m_strName.GetLength()>30){
		AfxMessageBox(g_StrMan.Get(_T("ROOMTITLE")));
		m_ctrlTitle.SetFocus();
		return;
	}	
	
	CDialog::OnOK();
}

