// SelCardChoiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "SelCardChoiceDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelCardChoiceDlg dialog


CSelCardChoiceDlg::CSelCardChoiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelCardChoiceDlg::IDD, pParent)
{
	hTimer = NULL;
	Select = 0;
	Index = 0;
	m_nTime = 0;
	DisCard = 0;
	DisCardIndex = 0;
	bChoice = 0;

	//{{AFX_DATA_INIT(CSelCardChoiceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelCardChoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelCardChoiceDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelCardChoiceDlg, CDialog)
	//{{AFX_MSG_MAP(CSelCardChoiceDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelCardChoiceDlg message handlers


void CSelCardChoiceDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	Select = -1;//m_aCard[0];
	//Index = 0;
	Index = 1;
	
	CDialog::OnCancel();
}

void CSelCardChoiceDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(g_RI.FormKind == 0)
	{
		Select = m_aCard[0];
		//Index = 0;
		Index = 1;
	}
	else
	{
		if(bChoice == 0) // 버린카드 없이 엔터
		{
			Select = m_aCard[0];
			Index = 1;
			DisCardIndex = 4;

		}
		else
		{
			switch(bChoice) //버린카드
			{
			case 1:
				Select = m_aCard[1];
				Index = 2;
				DisCardIndex = 1;
				break;
			case 2:
				Select = m_aCard[0];
				Index = 1;
				DisCardIndex = 2;
				break;
			case 3:
				Select = m_aCard[0];
				Index = 1;
				DisCardIndex = 3;
				break;
			case 4:
				Select = m_aCard[0];
				Index = 1;
				DisCardIndex = 4;
				break;
			}
		}
	}

	CDialog::OnOK();
}

void CSelCardChoiceDlg::OnPaint() 
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

	int xSize = 71;
	int ySize = 96;




	if(g_RI.FormKind == 1)
	{
		if(bChoice == 0)
		{
			CMyBitmap bmp;
			bmp.LoadBitmapFile(".\\image\\SelThrowCard.bmp");
			bmp.TransDraw(dc.m_hDC, 37, 11,  240,14, 0, 0, RGB(255, 0, 255));
			bmp.DeleteObject();
		//	bmp.DeleteObject();
			//dc.SetBkMode(TRANSPARENT);
			//CString str1;
			//dc.SetTextColor(RGB(225,250,255));
			//str1.Format("%s","버릴카드를 선택하거라");
			//dc.TextOut(23,10,str1);
		}
		else
		{
			
			CMyBitmap bmp;
			bmp.LoadBitmapFile(".\\image\\SelOpenCard.bmp");
			bmp.TransDraw(dc.m_hDC, 37, 11, 240,14, 0, 0, RGB(255, 0, 255));
			bmp.DeleteObject();
		//	bmp.DeleteObject();
		//	dc.SetBkMode(TRANSPARENT);
		//	CString str1;
		//	dc.SetTextColor(RGB(225,250,255));
		//	str1.Format("%s","오픈카드를 선택하거라");
		//	dc.TextOut(23,10,str1);


		}
	}

	
	Page.PutSprAuto(0, 0, &BigCardSpr, m_aCard[0]);
	Page.FlipToGDI(dc.m_hDC, CardRect1.left, CardRect1.top, 1);
	
	Page.PutSprAuto(0, 0, &BigCardSpr, m_aCard[1]);
	Page.FlipToGDI(dc.m_hDC, CardRect2.left, CardRect2.top, 1);
	
	Page.PutSprAuto(0, 0, &BigCardSpr, m_aCard[2]);
	Page.FlipToGDI(dc.m_hDC, CardRect3.left, CardRect3.top, 1);

	if(g_RI.FormKind == 1)
	{
		Page.PutSprAuto(0, 0, &BigCardSpr, m_aCard[3]);
		Page.FlipToGDI(dc.m_hDC, CardRect4.left, CardRect4.top, 1);
	}

	dc.SelectObject(&Font1);
	dc.SetTextColor(RGB(255,255,255));
	
	dc.SetBkMode(TRANSPARENT);

	CString str;
	dc.SetTextColor(RGB(225,250,255));
	str.Format("%s","[Delete]");
	dc.TextOut(20,141,str);
	str.Format("%s","[End]");
	dc.TextOut(101,141,str);
	str.Format("%s","[PageDn]");
	dc.TextOut(164,141,str);
	str.Format("%s","[PageUp]");
	dc.TextOut(237,141,str);

/*
	CString str;
	dc.SetTextColor(RGB(225,250,255));
	str.Format("%s","[Insert]");
	dc.TextOut(23,141,str);
	str.Format("%s","[Home]");
	dc.TextOut(98,141,str);
	str.Format("%s","[PageUp]");
	dc.TextOut(166,141,str);
	str.Format("%s","[PageDn]");
	dc.TextOut(239,141,str);
*/	
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CSelCardChoiceDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	//return;
	if(nIDEvent==hTimer){
		m_nTime++;
		if(m_nTime> 9){
		
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

BOOL CSelCardChoiceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	Back.LoadBitmapFile(".\\image\\SelCardChoice.bmp");

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

	//g_RI.FormKind = 1;
	
	int xpos = 3;

	if(g_RI.FormKind == 0)
	{
		Page.Init(xSize, ySize, 16);
		CardRect1.SetRect(11, 37, 11+xSize, 37+ySize);
		CardRect2.SetRect(11+xSize+5, 37, 11+xSize*2+5, 37+ySize);
		CardRect3.SetRect(11+xSize*2+5+5, 37, 11+xSize*3+5+5, 37+ySize);
	}
	else // ###초이스 카드###
	{
		Page.Init(xSize, ySize, 16);
		CardRect1.SetRect(11, 37, 11+xSize, 37+ySize);
		CardRect2.SetRect(11+xSize+xpos, 37, 11+xSize*2+xpos, 37+ySize);
		CardRect3.SetRect(11+xSize*2+xpos*2, 37, 11+xSize*3+xpos+2, 37+ySize);
		CardRect4.SetRect(11+xSize*3+xpos*3, 37, 11+xSize*4+xpos*3, 37+ySize);
	}

	hTimer = SetTimer(SELECTDLGCHOICE_TIMER, 1000, NULL);

/*
	// 게임 도움말 메시지를 메인서버에게 알림
	CCL_GAMEHELPMSG ghmsg;
	ghmsg.Set(g_RI.RoomNum, Play[0].UI.ID, 0);
	SockMan.SendData(g_MainSrvSID, ghmsg.pData, ghmsg.GetTotalSize());
*/
	RECT rc;
	GetWindowRect( &rc );
	rc.right = rc.left + 315;
	rc.bottom = rc.top + 158;
	MoveWindow( &rc );

	CenterWindow(g_pGameView->GetOwner());

	((C62CutPokerDlg *)AfxGetMainWnd())->m_hSubWindow = m_hWnd;

	// 사운드 출력
//	Sound.Play(SND35);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelCardChoiceDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(g_RI.FormKind == 0)
	{
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
	}
	else // 카드 선택중
	{
		if(bChoice == 0) // 카드 버리기
		{
			if(CardRect1.PtInRect(point))
			{
				DisCard = m_aCard[0];
				DisCardIndex = 1;
				bChoice = 1;
				m_aCard[0] = 52;
				//Invalidate();
				InvalidateRect(&CardRect1);
				CRect rect;
				rect.SetRect(37, 11,  277, 25);
				InvalidateRect(&rect);
			//	CDialog::OnOK();
			//	return;
			}
			else if(CardRect2.PtInRect(point))
			{
				DisCard = m_aCard[1];
				DisCardIndex = 2;
				bChoice = 2;
				m_aCard[1] = 52;
				//Invalidate();
				InvalidateRect(&CardRect2);
				CRect rect;
				rect.SetRect(37, 11,  277, 25);
				InvalidateRect(&rect);
			//	CDialog::OnOK();
			//	return;
			}
			else if(CardRect3.PtInRect(point))
			{
				DisCard = m_aCard[2];
				DisCardIndex = 3;
				bChoice = 3;
				m_aCard[2] = 52;
				//Invalidate();
				InvalidateRect(&CardRect3);
				CRect rect;
				rect.SetRect(37, 11,  277, 25);
				InvalidateRect(&rect);
			//	CDialog::OnOK();
			//	return;
			}
			else if(CardRect4.PtInRect(point))
			{
				DisCard = m_aCard[3];
				DisCardIndex = 4;
				bChoice = 4;
				m_aCard[3] = 52;
			//	Invalidate();
				InvalidateRect(&CardRect4);
				CRect rect;
				rect.SetRect(37, 11,  277, 25);
				InvalidateRect(&rect);
			//	CDialog::OnOK();
			//	return;
			}
			else
			{
				PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
			}
			
		}
		else // 카드 오픈중
		{
			if(CardRect1.PtInRect(point))
			{
				if(bChoice != 1)
				{
					Select = m_aCard[0];
					Index = 1;
					CDialog::OnOK();
				}
				return;
			}
			else if(CardRect2.PtInRect(point))
			{
				if(bChoice != 2)
				{
					Select = m_aCard[1];
					Index = 2;
					CDialog::OnOK();
				}
				return;
			}
			else if(CardRect3.PtInRect(point))
			{
				if(bChoice != 3)
				{
					Select = m_aCard[2];
					Index = 3;
					CDialog::OnOK();
				}
				return;
			}
			else if(CardRect4.PtInRect(point))
			{
				if(bChoice != 4)
				{
					Select = m_aCard[3];
					Index = 4;
					CDialog::OnOK();
				}
				return;
			}
			else
			{
				PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
			}
		}

	}


	CDialog::OnLButtonDown(nFlags, point);
}

void CSelCardChoiceDlg::OnDestroy() 
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

void CSelCardChoiceDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(CardRect1.PtInRect(point)) SetCursor(g_pGameView->hCurHand);
	if(CardRect2.PtInRect(point)) SetCursor(g_pGameView->hCurHand);
	if(CardRect3.PtInRect(point)) SetCursor(g_pGameView->hCurHand);
	
	if(g_RI.FormKind == 1)
		if(CardRect4.PtInRect(point)) SetCursor(g_pGameView->hCurHand);
	
	CDialog::OnMouseMove(nFlags, point);
}

int CSelCardChoiceDlg::DoModal(int card1, int card2, int card3, int card4) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_aCard[0] = card1; // 1
	m_aCard[1] = card2; // 2
	m_aCard[2] = card3; // 3
	m_aCard[3] = 0;     // 4  

	if(g_RI.FormKind == 1) // 초이스 카드
	{
		m_aCard[3] = card4;
	}

	Select = card1; // 0 ~ 51
	Index = 1;// -1
	
	// default 값
	DisCard = card4;
	DisCardIndex = 4;
	return CDialog::DoModal();
}
/*
BOOL CSelCardChoiceDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN) {


		// 3장
		if(g_RI.FormKind == 0)
		{
			if((UINT)pMsg->wParam==VK_DELETE){ // 1번카드
				Select = m_aCard[0];
				Index = 1;
				CDialog::OnOK();
				return TRUE;
			}
			if( (UINT)pMsg->wParam==VK_END){    // 2번카드
				Select = m_aCard[1];
				Index = 2;
				CDialog::OnOK();
				return TRUE;
			}
			if( (UINT)pMsg->wParam==0x22){      // 3번카드
				Select = m_aCard[2];
				Index = 3;
				CDialog::OnOK();
				return TRUE;
			}

		}
		else // 초이스 카드
		{
			if(bChoice == 0) 
			{// 카드 버리기
				if((UINT)pMsg->wParam==VK_INSERT)// || (UINT)pMsg->wParam==VK_DELETE){ // 1번카드
				{
					DisCard = m_aCard[0];
					DisCardIndex = 1;
					bChoice = 1;
					m_aCard[0] = 52;
					//Invalidate();
					InvalidateRect(&CardRect1);
					CRect rect;
					rect.SetRect(37, 11,  277, 25);
					InvalidateRect(&rect);
				//	CDialog::OnOK();
				//	return;
				}
				if((UINT)pMsg->wParam==VK_HOME)// || (UINT)pMsg->wParam==VK_END){    // 2번카드
				{
					DisCard = m_aCard[1];
					DisCardIndex = 2;
					bChoice = 2;
					m_aCard[1] = 52;
					//Invalidate();
					InvalidateRect(&CardRect2);
					CRect rect;
					rect.SetRect(37, 11,  277, 25);
					InvalidateRect(&rect);
				//	CDialog::OnOK();
				//	return;
				}
				if((UINT)pMsg->wParam==0x21)// || (UINT)pMsg->wParam==0x22){      // 3번카드
				{
					DisCard = m_aCard[2];
					DisCardIndex = 3;
					bChoice = 3;
					m_aCard[2] = 52;
					//Invalidate();
					InvalidateRect(&CardRect3);
					CRect rect;
					rect.SetRect(37, 11,  277, 25);
					InvalidateRect(&rect);
				//	CDialog::OnOK();
				//	return;
				}
				if((UINT)pMsg->wParam==0x22)// || (UINT)pMsg->wParam==0x22){      // 4번카드
				{
					DisCard = m_aCard[3];
					DisCardIndex = 4;
					bChoice = 4;
					m_aCard[3] = 52;
				//	Invalidate();
					InvalidateRect(&CardRect4);
					CRect rect;
					rect.SetRect(37, 11,  277, 25);
					InvalidateRect(&rect);
				//	CDialog::OnOK();
				//	return;
				}
					
			}
			else // 카드 오픈중
			{
				if((UINT)pMsg->wParam==VK_INSERT)// || (UINT)pMsg->wParam==VK_DELETE){ // 1번카드
				{
					if(bChoice != 1)
					{
						Select = m_aCard[0];
						Index = 1;
						CDialog::OnOK();
					}
					return TRUE;
				}
				else if((UINT)pMsg->wParam==VK_HOME)// || (UINT)pMsg->wParam==VK_END){    // 2번카드
				{
					if(bChoice != 2)
					{
						Select = m_aCard[1];
						Index = 2;
						CDialog::OnOK();
					}
					return TRUE;
				}
				else if((UINT)pMsg->wParam==0x21)// || (UINT)pMsg->wParam==0x22){      // 3번카드
				{
					if(bChoice != 3)
					{
						Select = m_aCard[2];
						Index = 3;
						CDialog::OnOK();
					}
					return TRUE;
				}
				else if((UINT)pMsg->wParam==0x22)// || (UINT)pMsg->wParam==0x22){      // 4번카드
				{
					if(bChoice != 4)
					{
						Select = m_aCard[3];
						Index = 4;
						CDialog::OnOK();
					}
					return TRUE;
				}
			
			}
		}




	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

*/

BOOL CSelCardChoiceDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN) {


		// 3장
		if(g_RI.FormKind == 0)
		{
			if((UINT)pMsg->wParam==VK_DELETE){ // 1번카드
				Select = m_aCard[0];
				Index = 1;
				CDialog::OnOK();
				return TRUE;
			}
			if( (UINT)pMsg->wParam==VK_END){    // 2번카드
				Select = m_aCard[1];
				Index = 2;
				CDialog::OnOK();
				return TRUE;
			}
			if( (UINT)pMsg->wParam==VK_NEXT){      // 3번카드
				Select = m_aCard[2];
				Index = 3;
				CDialog::OnOK();
				return TRUE;
			}

		}
		else // 초이스 카드
		{
			if(bChoice == 0) 
			{// 카드 버리기
				if((UINT)pMsg->wParam==VK_DELETE) // 1번카드
				{
					DisCard = m_aCard[0];
					DisCardIndex = 1;
					bChoice = 1;
					m_aCard[0] = 52;
					//Invalidate();
					InvalidateRect(&CardRect1);
					CRect rect;
					rect.SetRect(37, 11,  277, 25);
					InvalidateRect(&rect);
				//	CDialog::OnOK();
				//	return;
				}
				if((UINT)pMsg->wParam==VK_END)    // 2번카드
				{
					DisCard = m_aCard[1];
					DisCardIndex = 2;
					bChoice = 2;
					m_aCard[1] = 52;
					//Invalidate();
					InvalidateRect(&CardRect2);
					CRect rect;
					rect.SetRect(37, 11,  277, 25);
					InvalidateRect(&rect);
				//	CDialog::OnOK();
				//	return;
				}
				if((UINT)pMsg->wParam==VK_NEXT)      // 3번카드
				{
					DisCard = m_aCard[2];
					DisCardIndex = 3;
					bChoice = 3;
					m_aCard[2] = 52;
					//Invalidate();
					InvalidateRect(&CardRect3);
					CRect rect;
					rect.SetRect(37, 11,  277, 25);
					InvalidateRect(&rect);
				//	CDialog::OnOK();
				//	return;
				}
				if((UINT)pMsg->wParam==VK_PRIOR)      // 4번카드
				{
					DisCard = m_aCard[3];
					DisCardIndex = 4;
					bChoice = 4;
					m_aCard[3] = 52;
				//	Invalidate();
					InvalidateRect(&CardRect4);
					CRect rect;
					rect.SetRect(37, 11,  277, 25);
					InvalidateRect(&rect);
				//	CDialog::OnOK();
				//	return;
				}
					
			}
			else // 카드 오픈중
			{
				if((UINT)pMsg->wParam==VK_DELETE) // 1번카드
				{
					if(bChoice != 1)
					{
						Select = m_aCard[0];
						Index = 1;
						CDialog::OnOK();
					}
					return TRUE;
				}
				else if( (UINT)pMsg->wParam==VK_END )    // 2번카드
				{
					if(bChoice != 2)
					{
						Select = m_aCard[1];
						Index = 2;
						CDialog::OnOK();
					}
					return TRUE;
				}
				else if((UINT)pMsg->wParam==VK_NEXT)      // 3번카드
				{
					if(bChoice != 3)
					{
						Select = m_aCard[2];
						Index = 3;
						CDialog::OnOK();
					}
					return TRUE;
				}
				else if((UINT)pMsg->wParam==VK_PRIOR)      // 4번카드
				{
					if(bChoice != 4)
					{
						Select = m_aCard[3];
						Index = 4;
						CDialog::OnOK();
					}
					return TRUE;
				}
			
			}
		}




	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
