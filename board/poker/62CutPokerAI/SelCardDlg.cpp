// SelCardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "SelCardDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelCardDlg dialog


CSelCardDlg::CSelCardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelCardDlg::IDD, pParent)
{
	hTimer = NULL;
	Select = 0;
	Index = 0;
	m_nTime = 0;

	//{{AFX_DATA_INIT(CSelCardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelCardDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelCardDlg, CDialog)
	//{{AFX_MSG_MAP(CSelCardDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelCardDlg message handlers

void CSelCardDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	Select = -1;//m_aCard[0];
	//Index = 0;
	Index = 1;
	
	CDialog::OnCancel();
}

void CSelCardDlg::OnOK() 
{
	// TODO: Add extra validation here
	Select = m_aCard[0];
	//Index = 0;
	Index = 1;
	
	CDialog::OnOK();
}

void CSelCardDlg::OnPaint() 
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

	int xSize = 62;
	int ySize = 83;



	
	Page.PutSprAuto(0, 0, &BigCardSpr, m_aCard[0]);
	Page.FlipToGDI(dc.m_hDC, CardRect1.left, CardRect1.top, 1);
	
	Page.PutSprAuto(0, 0, &BigCardSpr, m_aCard[1]);
	Page.FlipToGDI(dc.m_hDC, CardRect2.left, CardRect2.top, 1);
	
	Page.PutSprAuto(0, 0, &BigCardSpr, m_aCard[2]);
	Page.FlipToGDI(dc.m_hDC, CardRect3.left, CardRect3.top, 1);
	

	dc.SelectObject(&Font1);
	dc.SetTextColor(RGB(255,255,255));

	dc.SetBkMode(TRANSPARENT);

	CString str;
	dc.SetTextColor(RGB(225,250,255));
	str.Format("%s","[Delete]");
	dc.TextOut(23,141,str);
	str.Format("%s","[End]");
	dc.TextOut(105,141,str);
	str.Format("%s","[PageDn]");
	dc.TextOut(164,141,str);

/*
	CString str;
	dc.SetTextColor(RGB(225,250,255));
	str.Format("%s","[Delete]");
	dc.TextOut(23,141,str);
	str.Format("%s","[End]");
	dc.TextOut(105,141,str);
	str.Format("%s","[PageDN]");
	dc.TextOut(164,141,str);
*/
	// Do not call CDialog::OnPaint() for painting messages
}

void CSelCardDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	//return;
	if(nIDEvent==hTimer) {
		m_nTime++;
		if(m_nTime> 7) {
		
			Select = -1;
			KillTimer(hTimer);
			OnCancel();
		}
		
		if(g_Where != WH_GAME || g_RI.State == 0 || Play[0].JoinState != 1 || Play[0].PlayState != 1)
		{
			Select = -1;
			KillTimer(hTimer);
			OnCancel();			
		}
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CSelCardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	Back.LoadBitmapFile(".\\image\\SelCard.bmp");

	// 폰트 생성
	if(Font1.m_hObject == NULL)
	Font1.CreateFont(12,0,0,0,
		//FW_NORMAL,
		FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	if(Font2.m_hObject == NULL)
	Font2.CreateFont(12,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");


	m_nTime = 0;

	// 카드 사이즈
	int xSize = 71;
	int ySize = 96;

	Page.Init(xSize, ySize, 16);
	CardRect1.SetRect(11, 37, 11+xSize, 37+ySize);
	CardRect2.SetRect(11+xSize+5, 37, 11+xSize*2+5, 37+ySize);
	CardRect3.SetRect(11+xSize*2+5+5, 37, 11+xSize*3+5+5, 37+ySize);

	hTimer = SetTimer(SELECTDLG_TIMER, 1000, NULL);

/*
	// 게임 도움말 메시지를 메인서버에게 알림
	CCL_GAMEHELPMSG ghmsg;
	ghmsg.Set(g_RI.RoomNum, Play[0].UI.ID, 0);
	SockMan.SendData(g_MainSrvSID, ghmsg.pData, ghmsg.GetTotalSize());
*/
	RECT rc;
	GetWindowRect( &rc );
	rc.right = rc.left + 247;
	rc.bottom = rc.top + 158;
	MoveWindow( &rc );

	CenterWindow(g_pGameView->GetOwner());
	// 사운드 출력
//	Sound.Play(SND35);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelCardDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(CardRect1.PtInRect(point))
	{
		Select = m_aCard[0];
		Index = 1;
		CDialog::OnOK();
		return;
	}
	else if(CardRect2.PtInRect(point))
	{
		Select = m_aCard[1];
		Index = 2;
		CDialog::OnOK();
		return;
	}
	else if(CardRect3.PtInRect(point))
	{
		Select = m_aCard[2];
		Index = 3;
		CDialog::OnOK();
		return;
	}
	else
	{
		PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CSelCardDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	//if(Back.m_hObject) Back.DeleteObject();
	Back.DeleteObject();
	Font1.DeleteObject();
	Font2.DeleteObject();

	if(hTimer)
	{
		KillTimer(hTimer);
		hTimer = NULL;
	}

	
}

void CSelCardDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(CardRect1.PtInRect(point)) SetCursor(g_pGameView->hCurHand);
	if(CardRect2.PtInRect(point)) SetCursor(g_pGameView->hCurHand);
	if(CardRect3.PtInRect(point)) SetCursor(g_pGameView->hCurHand);
	
	CDialog::OnMouseMove(nFlags, point);
}

int CSelCardDlg::DoModal(int card1, int card2, int card3) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_aCard[0] = card1;
	m_aCard[1] = card2;
	m_aCard[2] = card3;

	Select = card1;
	Index = 1;// -1

	
	return CDialog::DoModal();
}

/*
BOOL CSelCardDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN) {
		if((UINT)pMsg->wParam==VK_INSERT || (UINT)pMsg->wParam==VK_DELETE){ // 1번카드
			Select = m_aCard[0];
			Index = 1;
			CDialog::OnOK();
			return TRUE;
		}
		if((UINT)pMsg->wParam==VK_HOME || (UINT)pMsg->wParam==VK_END){    // 2번카드
			Select = m_aCard[1];
			Index = 2;
			CDialog::OnOK();
			return TRUE;
		}
		if((UINT)pMsg->wParam==0x21 || (UINT)pMsg->wParam==0x22){      // 3번카드
			Select = m_aCard[2];
			Index = 3;
			CDialog::OnOK();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
*/


BOOL CSelCardDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN) {
		if((UINT)pMsg->wParam==VK_DELETE){ // 1번카드
			Select = m_aCard[0];
			Index = 1;
			CDialog::OnOK();
			return TRUE;
		}
		if((UINT)pMsg->wParam==VK_END){    // 2번카드
			Select = m_aCard[1];
			Index = 2;
			CDialog::OnOK();
			return TRUE;
		}
		if((UINT)pMsg->wParam==0x22){      // 3번카드
			Select = m_aCard[2];
			Index = 3;
			CDialog::OnOK();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
