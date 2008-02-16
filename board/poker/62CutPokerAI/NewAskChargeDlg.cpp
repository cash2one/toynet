// NewAskChargeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "NewAskChargeDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CNewAskChargeDlg dialog

CNewAskChargeDlg::CNewAskChargeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewAskChargeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewAskChargeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_WebControl = NULL;
}


void CNewAskChargeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAskChargeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewAskChargeDlg, CDialog)
	//{{AFX_MSG_MAP(CNewAskChargeDlg)
		// NOTE: the ClassWizard will add message map macros here
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewAskChargeDlg message handlers

BOOL CNewAskChargeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect pappos;
	::SetWindowPos(m_hWnd,HWND_TOP,0,0,400,543,SWP_NOZORDER);

	GetClientRect(pappos);
	pappos.top+=79;
	pappos.bottom+=16;
	pappos.right+=16;
	//pappos.bottom-=79;

	Back.LoadBitmapFile(".\\Image\\allintitle.bmp");
	m_WebControl = new CWebCtrl4New();
	m_WebControl->Create(NULL,"WebControlWindow",WS_VISIBLE | WS_CHILD,pappos, this, 61327);
	m_WebControl->Navigate("http://www.netmarble.net/game/gameshop/allin/?gamecode=61");
//	g_ISASKSHOW = TRUE;	// ÀÌ°Ç È«´Ù¸®²¨ ¾Æ¹«°Å³ª ÇØº¸¼À
	//m_WebControl->Navigate("netmarble.net");
	//m_WebControl->SetBorderNone();

	CenterWindow();
	return TRUE;
}


void CNewAskChargeDlg::OnDestroy() 
{
	if(m_WebControl) delete m_WebControl;
	Back.DeleteObject();
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	
}


void CNewAskChargeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
}

void CNewAskChargeDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialog::OnLButtonDown(nFlags, point);
}