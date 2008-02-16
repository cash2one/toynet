// WebProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "WebProfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CWebProfileDlg::m_strProfileURL;

/////////////////////////////////////////////////////////////////////////////
// CWebProfileDlg dialog


CWebProfileDlg::CWebProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebProfileDlg::IDD, pParent)
{
	m_pWebCtrl = NULL;
	//{{AFX_DATA_INIT(CWebProfileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWebProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebProfileDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebProfileDlg, CDialog)
	//{{AFX_MSG_MAP(CWebProfileDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebProfileDlg message handlers

BOOL CWebProfileDlg::Create() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(CDialog::Create(IDD_WEBPROFILE_DIALOG, AfxGetMainWnd())) {
		ShowWindow(TRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL CWebProfileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	MoveWindow(0, 0, 475, 400, FALSE);
	CenterWindow();

	CRect rc;
	GetClientRect(rc);

	SetWindowText("이용자 상세 정보");

	if(!m_pWebCtrl) {
		m_pWebCtrl= new CWebCtrl();
	}

	// 웹 컨트롤 생성
	if(!m_pWebCtrl->GetSafeHwnd()) {
		if(m_pWebCtrl->Create(NULL, "WebControl", WS_VISIBLE|WS_CHILD, rc, this, 61321)) {
			if(m_strURL.GetLength() > 0) {
				CString str;
				str.Format("%s님의 상세 정보", m_strID);
				SetWindowText(str);
				m_pWebCtrl->Navigate(m_strURL);
			}
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWebProfileDlg::OnOK() 
{
	// TODO: Add extra validation here
	//CDialog::OnOK();
	return;
}

void CWebProfileDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
	//CDialog::OnCancel();
}

void CWebProfileDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pWebCtrl) {
		delete m_pWebCtrl;
		m_pWebCtrl = NULL;
	}
}

void CWebProfileDlg::SetProfileURL(LPCTSTR strurl)
{
	m_strProfileURL = strurl;
}

BOOL CWebProfileDlg::OpenProfile(LPCTSTR struserid)
{
	// 테스트용 m_strProfileURL = "http://61.78.51.62/Chat/window/info.asp?user_id=";
	
	if(m_strProfileURL.GetLength() == 0) return FALSE;

	m_strID = struserid;
	m_strURL.Format("%s%s", m_strProfileURL, struserid);

	if(!GetSafeHwnd()) {
		if(!Create()) return FALSE;
	}
	else {
		CString str;
		str.Format("%s님의 상세 정보", m_strID);
		SetWindowText(str);
		if(m_pWebCtrl && m_pWebCtrl->GetSafeHwnd()) m_pWebCtrl->Navigate(m_strURL);
	}

	return TRUE;
}
