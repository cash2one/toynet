// MsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "MsgDlg.h"
#include "Global.h"

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
	iMode = 0;
	hTimerWait = NULL;
	//{{AFX_DATA_INIT(CMsgDlg)
	kljs = NULL;
	//}}AFX_DATA_INIT
}


void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgDlg)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg message handlers

void CMsgDlg::MsgBox(int mode, char *msgstr)
{
	iMode = mode;
	szMsgText.Format(msgstr);
	Create(IDD_MSG_DIALOG, NULL);
	ShowWindow(SW_SHOW);
}

BOOL CMsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_STATIC_MSG)->SetWindowText(szMsgText);

	switch(iMode)
	{
	case 0:
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		if(hTimerWait!=NULL) KillTimer(hTimerWait);
		hTimerWait = SetTimer(MSGBOX_WAIT_TIMER, 25000, NULL); // 25초
		break;
	case 1:
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		if(hTimerWait!=NULL) KillTimer(hTimerWait);
		hTimerWait = SetTimer(MSGBOX_WAIT_TIMER, 25000, NULL); // 25초
		break;
	case 2:
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		if(hTimerWait!=NULL) KillTimer(hTimerWait);
		hTimerWait = SetTimer(MSGBOX_WAIT_TIMER, 15000, NULL); // 15초
		break;
	case 3:
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		break;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMsgDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(hTimerWait!=NULL) { KillTimer(hTimerWait); hTimerWait=NULL; }

	g_Wait=FALSE;

	// 이미 대화창이 파괴된 경우라면 리턴
	if(m_hWnd==NULL) return;
	
	switch(iMode)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	}

	DestroyWindow();
	m_hWnd = NULL;
	//CDialog::OnOK();
}

void CMsgDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(hTimerWait!=NULL) { KillTimer(hTimerWait); hTimerWait=NULL; }

	g_Wait=FALSE;
	
	// 이미 대화창이 파괴된 경우라면 리턴
	if(m_hWnd==NULL) return;

	switch(iMode)
	{
	case 0:
		g_pMainDlg->OnCancel();
		return;
	case 1:
		g_pLobyDlg->iAccessRoomState=0;
	}

	DestroyWindow();
	m_hWnd = NULL;
	//CDialog::OnCancel();
}

void CMsgDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == MSGBOX_WAIT_TIMER)
	{
		if(hTimerWait!=NULL) { KillTimer(hTimerWait); hTimerWait=NULL; }
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

		switch(iMode)
		{
		case 0:
		case 1:
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
			break;
		case 2:
			g_pMainDlg->bNowLogout = TRUE;
			g_pMainDlg->OnCancel();
			return;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CMsgDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CMsgDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
}
