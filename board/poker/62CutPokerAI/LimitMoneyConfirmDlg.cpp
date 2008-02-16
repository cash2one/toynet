// LimitMoneyConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "LimitMoneyConfirmDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLimitMoneyConfirmDlg dialog


CLimitMoneyConfirmDlg::CLimitMoneyConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLimitMoneyConfirmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLimitMoneyConfirmDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLimitMoneyConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLimitMoneyConfirmDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLimitMoneyConfirmDlg, CDialog)
	//{{AFX_MSG_MAP(CLimitMoneyConfirmDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLimitMoneyConfirmDlg message handlers

void CLimitMoneyConfirmDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CLimitMoneyConfirmDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CLimitMoneyConfirmDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CMyBitmap Back;
	Back.LoadBitmapFile("image\\LeadersMLimit\\LeadersConfirmBack.bmp");
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	dc.SetBkMode(TRANSPARENT);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CLimitMoneyConfirmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);



	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 355;
	rc1.bottom = rc1.top + 134;
	m_OkBtn.Init(80, 88, ".\\image\\LeadersMLimit\\Btn_LCOK.bmp",4);
	m_CancelBtn.Init(193, 88, ".\\image\\LeadersMLimit\\Btn_LCCancel.bmp",4);

	MoveWindow( &rc1 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
