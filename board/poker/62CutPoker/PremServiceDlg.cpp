// PremServiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "PremServiceDlg.h"

#include "DialogCtrlReposition.h"
#include "Avatar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	PREMSERVICEDLG_TIMER	434

/////////////////////////////////////////////////////////////////////////////
// CPremServiceDlg dialog


CPremServiceDlg::CPremServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPremServiceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPremServiceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPremServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPremServiceDlg)
	DDX_Control(pDX, IDOK, m_OKBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPremServiceDlg, CDialog)
	//{{AFX_MSG_MAP(CPremServiceDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPremServiceDlg message handlers

BOOL CPremServiceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	//char avatardir[MAX_PATH] = {0,};
	char avatardir[MAX_PATH] = "Avatar";
	//GetBaseAvatarDir(avatardir, MAX_PATH-1);

	// 비트맵 모양으로 다이얼로그 리전을 설정
	CMyBitmap Back;
	CString fname;
	//fname.Format("%s\\Common\\image\\PremBack.bmp", avatardir);
	fname.Format("%s\\Common\\image\\PremBack.bmp", avatardir);
	if(Back.LoadBitmapFile(fname)) {
		m_BmpRegion.SetWindowRgn(m_hWnd, (HBITMAP)Back, RGB(255,0,255));
		Back.DeleteObject();
	}

	// 최상위 윈도우로 설정
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	// 변수 초기화
	m_TimerCount = 0;
	m_hTimer = NULL;

	// 버튼 초기화
	fname.Format("%s\\Common\\image\\prem_close.bmp", avatardir);
	m_OKBtn.Init(fname, 4);

	// 타이머 세팅
	m_hTimer = SetTimer(PREMSERVICEDLG_TIMER, 2500, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPremServiceDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == PREMSERVICEDLG_TIMER) {
		PostMessage(WM_COMMAND, IDOK);
	}

	CDialog::OnTimer(nIDEvent);
}

void CPremServiceDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	//char avatardir[MAX_PATH] = {0,};
	char avatardir[MAX_PATH] = "Avatar";

	//GetBaseAvatarDir(avatardir, MAX_PATH-1);

	CMyBitmap Back;
	CString fname;
	fname.Format("%s\\Common\\image\\PremBack.bmp", avatardir);
	Back.LoadBitmapFile(fname);
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();
	// Do not call CDialog::OnPaint() for painting messages
}

void CPremServiceDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//DestroyWindow();
	PostMessage(WM_COMMAND, IDCANCEL);
	
	//CDialog::OnOK();
}

void CPremServiceDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	DestroyWindow();
	//CDialog::OnCancel();
}

BOOL CPremServiceDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(GetSafeHwnd()) 
		return FALSE;

	return CDialog::Create(IDD, pParentWnd);
}

void CPremServiceDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_hTimer) {
		KillTimer(m_hTimer);
		m_hTimer = NULL;
	}
}

void CPremServiceDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}
