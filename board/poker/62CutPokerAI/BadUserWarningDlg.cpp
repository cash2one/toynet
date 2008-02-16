// BadUserWarningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "BadUserWarningDlg.h"

#include "MyBitmap.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBadUserWarningDlg dialog


CBadUserWarningDlg::CBadUserWarningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBadUserWarningDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBadUserWarningDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBadUserWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBadUserWarningDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBadUserWarningDlg, CDialog)
	//{{AFX_MSG_MAP(CBadUserWarningDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBadUserWarningDlg message handlers

BOOL CBadUserWarningDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);


	m_OkBtn.Init(40,135,"image\\commonbtn\\Btn_Yes.bmp", 4);
	m_CancelBtn.Init(150,135,"image\\commonbtn\\Btn_No.bmp", 4);	
	
	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 276;
	rc1.bottom = rc1.top + 176;
	MoveWindow( &rc1 );

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBadUserWarningDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CMyBitmap Back;
	Back.LoadBitmapFile("image\\112\\BadUserWarning.bmp");

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	::DeleteObject(h);
	h = NULL;
	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();
	// Do not call CDialog::OnPaint() for painting messages
}

void CBadUserWarningDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CBadUserWarningDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
