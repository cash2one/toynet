// BadUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "BadUserDlg.h"
#include "BadUserWarningDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBadUserDlg dialog

// 정적 변수
int CBadUserDlg::SubmitCnt = 0;

CBadUserDlg::CBadUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBadUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBadUserDlg)
	m_SubmitReason = _T("");
	m_TargetID = _T("");
	//}}AFX_DATA_INIT

	m_bAnnounce = FALSE;
}


void CBadUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBadUserDlg)
	DDX_Control(pDX, IDC_BTN_ANNOUNCE, m_btn_Announce);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Text(pDX, IDC_EDIT_REASON, m_SubmitReason);
	DDV_MaxChars(pDX, m_SubmitReason, 60);
	DDX_Text(pDX, IDC_EDIT_TARGETID, m_TargetID);
	DDV_MaxChars(pDX, m_TargetID, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBadUserDlg, CDialog)
	//{{AFX_MSG_MAP(CBadUserDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBadUserDlg message handlers

void CBadUserDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(m_TargetID.GetLength() < 4)
	{
		g_pMainDlg->m_MsgBoxDlg.MsgBox( 0 , "신고 대상ID를 기입하세요." );
		return;
	}

	if(m_SubmitReason.GetLength() < 4)
	{
		g_pMainDlg->m_MsgBoxDlg.MsgBox( 0 , "신고 사유를 기입하세요." );
		return;
	}

	CBadUserWarningDlg Dlg;
	int rtn = Dlg.DoModal();
	if(rtn == IDCANCEL) { CDialog::OnCancel(); return; }

	CDialog::OnOK();
}

BOOL CBadUserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	if(g_pMainDlg->hTimerBadUser != NULL)
	{
		g_pMainDlg->m_MsgBoxDlg.MsgBox( 0 , "신고후 1분간은 신고기능을 이용할 수 없습니다." );
		CDialog::OnCancel(); 
		return TRUE;
	}

	if(SubmitCnt >= 3)
	{
		g_pMainDlg->m_MsgBoxDlg.MsgBox( 0 , "접속후 총 세번 이상 신고할 수 없습니다." );
		CDialog::OnCancel(); 
		return TRUE;
	}

	GetDlgItem(IDC_EDIT_MYID)->SetWindowText(g_MyInfo.UI.ID);// ### [ 관전기능 ] ###
	GetDlgItem(IDC_EDIT_TARGETID)->SetFocus();
	
	// 배경 그림 읽기
	Back.LoadBitmapFile(".\\image\\112\\BadUserBack.bmp");

	// 버튼 초기화
	m_OkBtn.Init(82, 319,".\\image\\112\\BBtn_112.bmp", 4);
	m_CancelBtn.Init(182, 319,".\\image\\commonbtn\\Btn_cancel.bmp", 4);
	m_btn_Announce.Init(112,265,".\\image\\commonbtn\\btn_check.bmp",NULL,this);

	m_bAnnounce = FALSE;

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBadUserDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	dc.SetTextColor(RGB(0,0,0));

	::DeleteObject(h);
	h = NULL;
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CBadUserDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
	
}

BOOL CBadUserDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(wParam == IDC_BTN_ANNOUNCE){
		m_bAnnounce = m_btn_Announce.GetCheck();
	}
	
	return CDialog::OnCommand(wParam, lParam);
}
