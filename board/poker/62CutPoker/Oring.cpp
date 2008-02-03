// Oring.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "62CutPokerDlg.h"
#include "global.h"
#include "Oring.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COring dialog


COring::COring(CWnd* pParent /*=NULL*/)
	: CDialog(COring::IDD, pParent)
{
	m_ShowDlg = FALSE;
	//{{AFX_DATA_INIT(COring)
	//}}AFX_DATA_INIT
}


void COring::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COring)
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COring, CDialog)
	//{{AFX_MSG_MAP(COring)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_LEVEL, OnButtonLevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COring message handlers

BOOL COring::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 배경 그림 읽기
	Back.LoadBitmapFile(".\\image\\Bankruptcy\\Oring.bmp");

	m_Ok.Init(69, 132, ".\\image\\commonbtn\\Btn_Yes.bmp",4);
	m_Cancel.Init(167, 132, ".\\image\\commonbtn\\Btn_No.bmp",4);
//	m_Level.Init(IDB_BTN_LEVEL1, IDB_BTN_LEVEL2, IDB_BTN_LEVEL3, NULL);

	// 캐릭터 뷰 초기화

//	CRect rc;
//	rc.SetRect(0,0,85,120);
//	rc.OffsetRect(16,27);
//	m_CharView.Create(this, rc, 6, 3232);

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 323;
	rc1.bottom = rc1.top + 176;
	MoveWindow( &rc1 );

	m_ShowDlg = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COring::OnOK() 
{
	// TODO: Add extra validation here
	g_pMainDlg->bNowLogout = FALSE; // 서버에 알리고 죽는다.
	
	g_Where = WH_TITLE;
	Sound.Play(SND45);
	g_pMainDlg->BrowserMove( g_BannerURL2 );
	//g_pMainDlg->OnCancel();

	m_ShowDlg = FALSE;
	
	CDialog::OnOK();
}

// 추가 레벨 상점
void COring::OnButtonLevel() 
{
	// TODO: Add your control notification handler code here
	g_pMainDlg->bNowLogout = TRUE;
	
	g_Where = WH_TITLE;
	Sound.Play(SND45);
	//g_pMainDlg->OnCancel();
	
//	BrowserMove( g_ProfileURL ); // 프로필 URL 레벨 상점으로 사용
	
	CDialog::OnOK();
	
}


void COring::OnCancel() 
{
	// TODO: Add extra cleanup here
	Sound.Play(SND45);
	m_ShowDlg = FALSE;
	CDialog::OnCancel();
}



void COring::OnPaint() 
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

	::DeleteObject(h);
	h = NULL;
	
	// Do not call CDialog::OnPaint() for painting messages
}

void COring::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void COring::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
}


int COring::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class

	m_ShowDlg = TRUE;
	
	return CDialog::DoModal();
}
