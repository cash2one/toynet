// StartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "StartDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartDlg dialog


CStartDlg::CStartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStartDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStartDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStartDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartDlg, CDialog)
	//{{AFX_MSG_MAP(CStartDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartDlg message handlers

void CStartDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CStartDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}




BOOL CStartDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	ctlID1 = ctlID2 = ctlID3 = 1000+rand()%100;

	for(;;)
	{
		if(ctlID1 != ctlID2  && ctlID1 != ctlID3 && ctlID2 != ctlID3 ) break;
		ctlID2 = 1000+rand()%100;
		ctlID3 = 1000+rand()%100;
	}
	

	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	Back.LoadBitmapFile(".\\image\\warning.bmp");

	//RECT rect={10,10,100,50};
	//m_test.Create("버튼",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this,100);
	//m_test.ShowWindow(SW_SHOW);

	RECT rect={0,40,50,80};
	m_test1.Create("button","",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this,ctlID1);
	m_test1.Init(135,292, ".\\image\\Common\\Btn_close.bmp",4);
	m_test1.ShowWindow(SW_SHOW);

	
	m_test2.Create("button","",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this,ctlID2);
	CRect rt(0,0,0,0);
	rt.OffsetRect(-1000, -1000);
	m_test2.MoveWindow(&rt);
		
	
	m_test3.Create("button","",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this,ctlID3);
	m_test3.MoveWindow(&rt);
	
		
	//ShowWindow(SW_SHOW);

//	m_Close.Init(125,292, ".\\image\\LBtn_profileclose.bmp",3);
//	m_Close.ShowWindow(SW_HIDE);

	RECT rc;
	GetWindowRect( &rc );
	rc.right = rc.left + 323;
	rc.bottom = rc.top + 328;
	MoveWindow( &rc );

	CenterWindow();
	
 
//	CRect mv = rc;
//	mv.OffsetRect(rand()%500 - rc.left, rand()%100 -rc.top); 
//	MoveWindow(&mv);  

	
	//CreateWindow("button","Enumerate",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
	//mv.left+50, mv.top+30, mv.right+60, mv.bottom+25, this->m_hWnd, NULL, NULL,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStartDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
	
	// Do not call CDialog::OnPaint() for painting messages
}


BOOL CStartDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN) return 1;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CStartDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
	
}

LRESULT CStartDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_COMMAND)
	{
		if(wParam == ctlID1)
		{
			CDialog::OnOK();
			return 1;
		}

		if(wParam == ctlID2 || wParam == ctlID3 )
		{
			//CDialog::OnCancel();
			g_pMainDlg->SendMessage(WM_CLOSE, 0, 0);
			return 1;
		}
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}
