// ResultFindIdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "ResultFindIdDlg.h"

#include "MyBitmap.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultFindIdDlg dialog


CResultFindIdDlg::CResultFindIdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResultFindIdDlg::IDD, pParent)
{
	m_Str = "";
	//{{AFX_DATA_INIT(CResultFindIdDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CResultFindIdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResultFindIdDlg)
	DDX_Control(pDX, IDOK, m_OkBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResultFindIdDlg, CDialog)
	//{{AFX_MSG_MAP(CResultFindIdDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultFindIdDlg message handlers

BOOL CResultFindIdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	
	m_OkBtn.Init(122,101,"image\\commonbtn\\Btn_ok.bmp", 4);
	
	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 327;
	rc1.bottom = rc1.top + 145;
	MoveWindow( &rc1 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResultFindIdDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CMyBitmap Back;
	Back.LoadBitmapFile("image\\FindId\\ResultFindId.bmp");

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

		// 폰트 생성
	CFont Font1;
	Font1.CreateFont(12,0,0,0,
		FW_NORMAL,
		//FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	dc.SetBkMode(TRANSPARENT);

	CFont* pOldFont = dc.SelectObject(&Font1);
	
	CRect rt;
	rt.SetRect(0,0, 285,45);
	rt.OffsetRect(25,54); 
	dc.DrawText( m_Str, &rt, DT_LEFT | DT_WORDBREAK );

	::DeleteObject(h);
	h = NULL;
	cdc.SelectObject(pOldFont);
	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();
	Font1.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}

int CResultFindIdDlg::DoModal( CString  &str) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Str = str;
	
	return CDialog::DoModal();
}
