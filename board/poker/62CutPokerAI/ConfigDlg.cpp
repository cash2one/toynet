// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	nScreenMode = 0;
	//{{AFX_DATA_INIT(CConfigDlg)
	a = NULL;
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(IDC_RADIO_FULLSCREENMODE, OnRadioFullscreenmode)
	ON_BN_CLICKED(IDC_RADIO_WINDOWMODE, OnRadioWindowmode)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	nScreenMode = 0;
	CheckRadioButton(IDC_RADIO_FULLSCREENMODE, IDC_RADIO_WINDOWMODE, IDC_RADIO_WINDOWMODE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CConfigDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CConfigDlg::OnRadioFullscreenmode() 
{
	// TODO: Add your control notification handler code here
	nScreenMode = 1;
}

void CConfigDlg::OnRadioWindowmode() 
{
	// TODO: Add your control notification handler code here
	nScreenMode = 0;
}

void CConfigDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
}
