// MiniDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MiniDlg.h"
#include "SysUtil.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiniDlg dialog


CMiniDlg::CMiniDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiniDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMiniDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//pPage = NULL;
}


void CMiniDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiniDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMiniDlg, CDialog)
	//{{AFX_MSG_MAP(CMiniDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonHi)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonLow)
	ON_WM_MOUSEMOVE()
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDC_BUTTON3, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniDlg message handlers

BOOL CMiniDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(GameBackSpr.Load(".\\data\\Mini\\x2gameback.spr", 555)==FALSE) AfxMessageBox("x2gameback.spr 파일을 읽을 수 없습니다");
	//if(InitBtnSpr.Load(".\\data\\Mini\\StartBtn.spr", 555)==FALSE) AfxMessageBox("StartBtn.spr 파일을 읽을 수 없습니다");
	if(InitBtnSpr.Load(".\\data\\StartBtn.spr", 555)==FALSE) AfxMessageBox("StartBtn.spr 파일을 읽을 수 없습니다");
	if(StartBtnSpr.Load(".\\data\\Mini\\continue.spr", 555)==FALSE) AfxMessageBox("continue.spr 파일을 읽을 수 없습니다");
	if(EndBtnSpr.Load(".\\data\\Mini\\Gstop.spr", 555)==FALSE) AfxMessageBox("Gstop.spr 파일을 읽을 수 없습니다");
	if(FocusSpr.Load(".\\data\\Focus.spr", 555)==FALSE) AfxMessageBox("Focus.spr 파일을 읽을 수 없습니다");
	if(DrawBonusSpr.Load(".\\data\\draw.spr", 555)==FALSE) AfxMessageBox("draw.spr 파일을 읽을 수 없습니다");


	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);


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

	CenterWindow(g_pGameView->GetOwner());

	SetTimer(MINIGAME_TIMER , 55 , NULL);

	X2Cnt = 0;
	CloseGameCnt = 0;
	bInitGame = FALSE;

	((C62CutPokerDlg *)AfxGetMainWnd())->m_hSubWindow = m_hWnd;

	m_MnGame.InitGame();
	

	m_bMiniBtnMouseDown = FALSE; 
	m_nMiniBtnIndex = 0;
	
	m_nMiniOffSetX[0] = 120;
	m_nMiniOffSetX[1] = 245;
	
	m_nMiniOffSetY[0] = 400;
	m_nMiniOffSetY[1] = 400;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMiniDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// 더블 버퍼링용 메모리 DC 생성
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	MemDC.SelectObject(Page.hBmp);

	Page.PutSprAuto(0, 0, &GameBackSpr, 0);

	m_MnGame.Draw();
	
	FucButtonDraw(&MemDC);
	FocusEffectDraw(&Page);

	
	// 마우스 좌표 체크
	POINT mpos;
	GetCursorPos(&mpos);
	ScreenToClient(&mpos);
	CString str;
	str.Format("Mouse (%d, %d)", mpos.x, mpos.y);
	MemDC.SelectObject(&Font2);
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(255,255,255));
	//MemDC.TextOut(100,100, str);		

	CRect rt;
	// Win Money
	int nWinMoney = m_MnGame.GetWinMoney();
	CString Winmstr;
	str = NumberToOrientalString(nWinMoney);
	Winmstr.Format("%s원",str);
	
	const int nXp = 150;
	const int nYp = 480;
	rt.SetRect(nXp,nYp,206+nXp,13+nYp);
	MemDC.DrawText(Winmstr,&rt,DT_RIGHT);

	// Win Money
	int nBankMoney = m_MnGame.GetBankMoney();

	CString Bankmstr;
	str = NumberToOrientalString(nBankMoney);					
	Bankmstr.Format("%s원",str);

	const int nXp2 = 150;
	const int nYp2 = 508;
	rt.SetRect(nXp2,nYp2,206+nXp2,13+nYp2);
	MemDC.DrawText(Bankmstr,&rt,DT_RIGHT);

	CRect rect;
	GetClientRect(&rect);

	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);
	// 부분 갱신 플래그 해제
//	bSetInvalidate = FALSE;
	MemDC.DeleteDC();

	SetFocus();

	// Do not call CDialog::OnPaint() for painting messages
}


void CMiniDlg::InitPage(CPage *ppage)
{
	//pPage = ppage;
	Page.Init(GAMEVIEW_WIDTH, GAMEVIEW_HEIGHT, 16);

	m_MnGame.Init( &Page );

	// 버튼 초기화
	X2InitBtn.Init(this, &Page, MINISTART_OFFSET_X, MINISTART_OFFSET_Y-12, &InitBtnSpr, 0,IDM_MINI_INIT);
	X2InitBtn.Show(TRUE);
	X2InitBtn.m_Width = 178;
	X2InitBtn.m_Height = 44;

	// 버튼 초기화
	X2PlayBtn.Init(this, &Page, MINISTART_OFFSET_X, MINISTART_OFFSET_Y, &StartBtnSpr, 0,IDM_MINI_START);
	X2PlayBtn.Show(TRUE);
	X2PlayBtn.m_Width = 100;
	X2PlayBtn.m_Height = 44;

	X2EndBtn.Init(this, &Page, MINISTOP_OFFSET_X, MINISTOP_OFFSET_Y, &EndBtnSpr, 0,IDM_MINI_STOP);
	X2EndBtn.Show(TRUE);
	X2EndBtn.m_Width = 100;
	X2EndBtn.m_Height = 44;

}

void CMiniDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_MnGame.OnTimer();

	Invalidate(FALSE);

	X2Cnt++;
	
	CDialog::OnTimer(nIDEvent);
}

void CMiniDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{

	CDialog::OnLButtonDown(nFlags, point);
}

void CMiniDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer( MINIGAME_TIMER );
	X2Cnt = 0;
	Game.bResultClosed = TRUE;
	
}

void CMiniDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CMiniDlg::OnButtonHi() 
{
	// TODO: Add your control notification handler code here
	int nResult = m_MnGame.CheckWinGame( 1 );
	m_MnGame.SetGameResult( nResult );
	
}

void CMiniDlg::OnButtonLow() 
{
	// TODO: Add your control notification handler code here
	int nResult = m_MnGame.CheckWinGame( 0 );
	m_MnGame.SetGameResult( nResult );
	
}

BOOL CMiniDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(wParam)
	{
		case IDM_MINI_START:
		{
			m_MnGame.IsGame();
		}
		break;

		case IDM_MINI_STOP:
		{
			PushStopBtn();
		}
		break;

		case IDM_MINI_INIT:
		{
			m_MnGame.InitGame();
		}
		break;

		case IDC_BUTTON_CHANGEDISPLAY:
		{
		}
		break;
	}

	
	return CDialog::OnCommand(wParam, lParam);
}

void CMiniDlg::PushStopBtn()
{
	if( m_MnGame.IsPlayGame() )
		return;

	m_MnGame.StopGame();

}

void CMiniDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnMouseMove(nFlags, point);
}

LRESULT CMiniDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	short mxp, myp;
	switch(message)
	{
		case WM_MOUSEMOVE :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);

			if( !bInitGame )
				X2InitBtn.OnMouseMove(mxp, myp);

			if( m_MnGame.GetGameContinue() == 0 || m_MnGame.GetGameContinue() == 2)
			{
				X2PlayBtn.OnMouseMove(mxp, myp);
				X2EndBtn.OnMouseMove(mxp, myp);
			}
		}
		break;

		case WM_LBUTTONDOWN :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);

			if( !bInitGame )
				X2InitBtn.OnLButtonDown(mxp, myp);

			if( m_MnGame.GetGameContinue() == 0 || m_MnGame.GetGameContinue() == 2)
			{
				X2PlayBtn.OnLButtonDown(mxp, myp);
				X2EndBtn.OnLButtonDown(mxp, myp);
			}
		}
		break;

		case WM_LBUTTONUP :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);
			if( !bInitGame )
			{
				X2InitBtn.OnLButtonUp(mxp, myp);
				Sound.Play(SND45);
			}
			
			if( m_MnGame.GetGameContinue() == 0 || m_MnGame.GetGameContinue() == 2)
			{
				X2PlayBtn.OnLButtonUp(mxp, myp);
				X2EndBtn.OnLButtonUp(mxp, myp);
				Sound.Play(SND45);
			}


		}
		break;

		
		case WM_CLOSE :
		{
			g_pGameView->X2StartBtn.Show(FALSE);
			m_MnGame.Reset();
		}
		break;
		

	}

	// 게임 시작 및 종료 버튼 효과 - jeong
	X2PlayBtn.OnMouseMove( m_nMiniOffSetX[m_nMiniBtnIndex], m_nMiniOffSetY[m_nMiniBtnIndex] );
	X2EndBtn.OnMouseMove( m_nMiniOffSetX[m_nMiniBtnIndex], m_nMiniOffSetY[m_nMiniBtnIndex] );
	
	if( m_bMiniBtnMouseDown )
	{
		m_bMiniBtnMouseDown = FALSE;
		
		mxp = m_nMiniOffSetX[m_nMiniBtnIndex];
		myp = m_nMiniOffSetY[m_nMiniBtnIndex];
		
		X2PlayBtn.OnLButtonDown(mxp, myp);
		X2EndBtn.OnLButtonDown(mxp, myp);
		
		X2PlayBtn.OnLButtonUp(mxp, myp);
		X2EndBtn.OnLButtonUp(mxp, myp);
	}

	
	
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CMiniDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( Game.bGameStart )			// 본게임 진행중 미니게임 키 막음
		return TRUE;

	if(pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_INSERT || pMsg->wParam == VK_DELETE)
		{
			m_MnGame.OnGameKey(pMsg->wParam);
			//m_MnGame.IsGame();
		}
		else if (pMsg->wParam == VK_HOME)
			m_MnGame.StopGame();
		else if(pMsg->wParam == 'M' || pMsg->wParam == 'C')
		{
			// 서버에 플레이어정보 돈 추가  - jeong
			int money = pMsg->wParam=='M'?100:-100;
			Play[0].PrevMoney +=money;			
			CSV_ASK_MONEYINFO aumsg;
			aumsg.Set(Play[0].UI.UNum, money, g_RI.RoomNum);
			SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());

		}

	}

	if( m_MnGame.GetWinGame() == 0 || m_MnGame.GetGameContinue() == 2 )
	{
		if(pMsg->message == WM_KEYDOWN)
		{
		
			if(pMsg->wParam == VK_LEFT)
			{	
				m_nMiniBtnIndex--;
				
				if( m_nMiniBtnIndex < 0)
					m_nMiniBtnIndex = 1;
			}
			else if( pMsg->wParam == VK_RIGHT )
			{
				m_nMiniBtnIndex++;
				if( m_nMiniBtnIndex > 1)
					m_nMiniBtnIndex = 0;
			}
			else if( pMsg->wParam == VK_DOWN )
			{
				m_bMiniBtnMouseDown = TRUE;
			}
		
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CMiniDlg::FocusEffectDraw(CPage *pPage)
{
	if( m_MnGame.GetWinGame() == -1)
		return;

	const int nXp = 450;
	const int nYp = 180;
	const int nWaitCnt = 12;
	const int nShowTimes = 35;				// 3 times

	if( m_MnGame.GetWinGame() == 0 )		// Win Image
	{
		if((X2Cnt/nWaitCnt)%2  == 0 && m_MnGame.GetWinCnt() < nShowTimes )
		{
			pPage->PutSprAuto(nXp , nYp , &FocusSpr, 5 );
			m_MnGame.PlusWinCnt();
		}
	}
	else if( m_MnGame.GetWinGame() == 1 )	// Lose Image
	{
		if((X2Cnt/nWaitCnt)%2  == 0 && m_MnGame.GetLoseCnt() < nShowTimes)
		{
			pPage->PutSprAuto(nXp , nYp , &FocusSpr, 6 );
			m_MnGame.PlusLoseCnt();

			CloseGameCnt++;

			if( CloseGameCnt > 32 )
				SendMessage(WM_CLOSE,0,0);
		}		
	}
	else if( m_MnGame.GetWinGame() == 2 )	// Draw Image
	{
		if((X2Cnt/nWaitCnt)%2  == 0 && m_MnGame.GetDrawCnt() < nShowTimes)
		{
			pPage->PutSprAuto(nXp , nYp , &DrawBonusSpr, 0 );
			m_MnGame.PlusDrawCnt();
		}		
	}
	else
		m_MnGame.InitWinLoseCnt();
	
}

void CMiniDlg::FucButtonDraw(CDC *pDC)
{
	if( !bInitGame )
		X2InitBtn.Draw(pDC);

	if( m_MnGame.GetWinGame() == 0 || m_MnGame.GetGameContinue() == 2 )
	{
		if( !m_MnGame.IsPlayGame() )
		{
			X2PlayBtn.Draw(pDC);
			X2EndBtn.Draw(pDC);
		}
	}
}

void CMiniDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	if(g_pLobyDlg->m_bFullScr)
	{
		CRect rect(0,0,800,600);
		MoveWindow(rect);
	}
}





