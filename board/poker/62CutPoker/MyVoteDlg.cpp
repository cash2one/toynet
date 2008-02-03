// MyVoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "MyVoteDlg.h"

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyVoteDlg dialog


CMyVoteDlg::CMyVoteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyVoteDlg::IDD, pParent)
{
	hTimer = NULL;
	NowLeftTime = 15;
	//{{AFX_DATA_INIT(CMyVoteDlg)
	m_BanishReson = _T("");
	m_TargetID = _T("");
	//}}AFX_DATA_INIT
}


void CMyVoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyVoteDlg)
	DDX_Control(pDX, IDOK, m_btnAssent);
	DDX_Control(pDX, IDCANCEL, m_btnConcept);
	DDX_Control(pDX, IDC_PROGRESS, m_TimeProgress);
	DDX_Text(pDX, IDC_EDIT_BANISHRESON, m_BanishReson);
	DDX_Text(pDX, IDC_EDIT_TARGETID, m_TargetID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyVoteDlg, CDialog)
	//{{AFX_MSG_MAP(CMyVoteDlg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyVoteDlg message handlers

BOOL CMyVoteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);
	// TODO: Add extra initialization here

	Back.LoadBitmapFile(".\\image\\vote\\voteback.BMP");		

	m_btnAssent.Init(44,209, ".\\image\\vote\\btn_ok.BMP", 4);
	m_btnConcept.Init(130,209, ".\\image\\vote\\btn_cancel.BMP", 4);

	UpdateData(FALSE);
	
	CRect trt;
	trt.SetRect(0,0,221,15);
	trt.OffsetRect(17,188);
	m_TimeProgress.MoveWindow(&trt);

	m_TimeProgress.SetBkColor(RGB(178,241,255));
	m_TimeProgress.SetBarColor(RGB(14,59,78));
	hTimer = SetTimer(MYVOTE_TIMER, 1000, NULL);
	NowLeftTime = 15;
	m_TimeProgress.SetRangeEx(15, NowLeftTime);


	RECT rc1;	
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 276;
	rc1.bottom = rc1.top + Back.GetHeight();
	MoveWindow( &rc1 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyVoteDlg::OnOK() 
{
	// TODO: Add extra validation here
	CSV_MYBANISHVOTE mvmsg;
	mvmsg.Set(TRUE);
	SockMan.SendData(g_MainSrvSID, mvmsg.pData, mvmsg.GetTotalSize());
	CDialog::OnOK();
}

void CMyVoteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CSV_MYBANISHVOTE mvmsg;
	mvmsg.Set(FALSE);
	SockMan.SendData(g_MainSrvSID, mvmsg.pData, mvmsg.GetTotalSize());
	CDialog::OnCancel();
}

void CMyVoteDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	Back.DeleteObject();
	
	// TODO: Add your message handler code here
	if(hTimer) KillTimer(hTimer);
	hTimer = NULL;
}

void CMyVoteDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == MYVOTE_TIMER)
	{
		NowLeftTime--;
		m_TimeProgress.SetPosEx(NowLeftTime);
		if(NowLeftTime<=0)
		{
			OnCancel();
			return;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CMyVoteDlg::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;	

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	::DeleteObject(h);	
	cdc.DeleteDC();
	
	// Do not call CDialog::OnPaint() for painting messages
}

HBRUSH CMyVoteDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	UINT nID = pWnd->GetDlgCtrlID(); //스태틱텍스트박스 투명하게 하기박스
	if(IDC_EDIT_TARGETID == nID || IDC_EDIT_BANISHRESON )
	{
		pDC->SetBkMode(TRANSPARENT);		
		return (HBRUSH)::GetStockObject(NULL_BRUSH);                      		
	}
	
//	m_TimeProgress.OnCtlColor(pDC,pWnd,nCtlColor);
//	((*CProgressCtrl)GetDlgItem(IDC_PROGRESS))->OnCtlColor(pDC,pWnd,nCtlColor);


	
	return hbr;	
}
