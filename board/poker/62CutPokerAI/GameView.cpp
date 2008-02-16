// GameView.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "GameView.h"
#include "MsgBox.h"
#include "BadUserDlg.h"
#include "OptionDlg.h"
#include "Global.h"
#include "Oring.h"

#include "BanishVoteDlg.h"
#include "RoomOptionDlg.h"
#include "ProfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// Center Position
int g_CX = 81;
int g_CY = 37;

// MyChar Position
int g_MX = 81;
int g_MY = 37;

// Gamer Position
int g_GX = 81;
int g_GY = 37;

/*
// 게임뷰 타이머 프로시저
void CALLBACK EXPORT GameViewTimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if(nIDEvent == GAMEVIEW_TIMER)
	{
		//g_pGameView->PostMessage(UM_GAMEVIEW_TIMER, nIDEvent, dwTime);
		g_pGameView->SendMessage(UM_GAMEVIEW_TIMER, nIDEvent, dwTime);
	}
}
*/

// ### [ Frame++ ] ###
void CALLBACK EventTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) 
{ 
	if(g_pGameView->GetSafeHwnd()) 
	{
		static MSG wq_msg;
		if(::PeekMessage(&wq_msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE) != 0) 
		{ 
			// 이전에 포스트된 타이머 메시지가 아직도 처리되지 않은 경우라면 그냥 리턴 
			if(wq_msg.hwnd == g_pGameView->GetSafeHwnd() && wq_msg.wParam == GAMEVIEW_TIMER)
			{
				g_pGameView->MessagePump();
				return;
			}
		}

		g_pGameView->PostMessage(WM_TIMER, GAMEVIEW_TIMER, 0);
	}
} 


/////////////////////////////////////////////////////////////////////////////
// CGameView

CGameView::CGameView()
{
	g_pGameView = this;
	//hTimer = 0;
	bIsPlay = FALSE;
	hCurSel = hCurHand = hCurMove = hCurSize = hCurWait = NULL;

	bSetInvalidate = FALSE;
	// 마우스 상태변수
	bLClick = FALSE;
	// 나나기 예약상태
	bExitSubscript = FALSE;

	m_BtnMsgTip = "";
	TipX = 0;
	TipY = 0;
	m_titlestr="";

	// ### [ Frame++ ] ###
	m_hEventTimer = 0;

	m_betpage_x = 270;
	m_betpage_y = 105;
	m_diameter = 210;

	m_betpagesize_x = 258;
	m_betpagesize_y = 248;
		
	m_pgamebackspr = NULL;
	m_pcarddeckspr = NULL;
}

CGameView::~CGameView()
{
	// 쓰레드 타이머 제거
//	RefreshTimer.KillTimer();

}


BEGIN_MESSAGE_MAP(CGameView, CWnd)
	//{{AFX_MSG_MAP(CGameView)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGameView message handlers

BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.style |= WS_CLIPCHILDREN;
	return CWnd::PreCreateWindow(cs);
}

int CGameView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	hCurSel = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SELECT));
	hCurHand = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND));
	hCurMove = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_MOVE));
	hCurSize = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_ADJUSTSIZE));
	hCurWait = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_WAIT));

	// 폰트 생성
	if(Font1.m_hObject == NULL)
	Font1.CreateFont(15,0,0,0,
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
		//DEFAULT_PITCH | FF_SWISS,
		FF_MODERN | FIXED_PITCH | FF_SWISS,
		"굴림체");

	// 그래픽 라이브러리 초기화
	Page.Init(GAMEVIEW_WIDTH, GAMEVIEW_HEIGHT, 16);


	// 한글 라이브러리 초기화
	//g_Han.Init(Page.lpBit, 640, 480, Page.lPitchHalf);
	//g_Han.SetFont(3, 3);
	//g_Han.SetTextColor(RGBmix(31,31,31));

	// 더블 버퍼링용 메모리 DC 생성
	//MemDC.CreateCompatibleDC(0);
	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	//MemDC.SelectObject(Page.hBmp);

	// 한글 IME 초기화
	//Ime.Init(m_hWnd);

	// 플레이어 클래스 초기화
	for(int i = 0; i < MAX_NEWPLAYER; i++)
	{	// 캐릭터창 초기화
		Play[i].SetPlayerNum(i);
	}
	// 게임 초기화(전역변수)
	for(i = 0; i < MAX_PLAYER; i++) {	// 캐릭터창 초기화
		CharBox[i].Init(this, &Page, i);
		CharBox[i].SetPlayer(i);
		PlayScr[i].Init(this, &Page, i);
	}

	Game.Init(&Page, &BatPage);
	
		
	////////////////////////////////////////////////////////////////////////
	// 채팅창 초기화
	//ChatBox.Init(this, &Page);
	ChatBox.Init( this, &Page, &ChatBoxSpr, false, CHATBOX_FLAG_VSCROLL );
	ChatBox.SetWndMoveState( true );		//	이동 불가능
	ChatBox.SetWndSizeState( FALSE );
	ChatBox.DelStyle( CHATBOX_FLAG_TITLE );
	ChatBox.SetTitle( g_RI.Title, RGB( 182, 255, 118 ) );
	////////////////////////////////////////////////////////////////////////


	// 화면 갱신 타이머 생성
	//hTimer = SetTimer(GAMEVIEW_TIMER, 55, GameViewTimerProc);

	// ### [ Frame++ ] ###
	syncfps.Init(REFRESH_FPS);
	TIMECAPS caps; memset( &caps, 0, sizeof(caps) );
	timeGetDevCaps( &caps, sizeof(caps) ); 
	//m_hEventTimer = timeSetEvent((int)(1000.0f/REFRESH_FPS), max(10, caps.wPeriodMin), EventTimerProc, 0, TIME_PERIODIC|TIME_CALLBACK_FUNCTION); 
	m_hEventTimer = timeSetEvent((int)(1000.0f/REFRESH_FPS), (int)(1000.0f/REFRESH_FPS), EventTimerProc, 0, TIME_PERIODIC|TIME_CALLBACK_FUNCTION); 

	m_pgamebackspr = &GameBackSpr;
	m_pcarddeckspr = &BigCardSpr;
	pagebutton_init();

	

	return 0;
}


void CGameView::pagebutton_init()
{
	if(g_RI.bPlayer==0) {
		m_pcarddeckspr = &BigCardSpr;
		// 그래픽 버튼 초기화
		InviteBtn.Init(this, &Page, 633, 3, &InviteBtnSpr, 0,IDM_INVITE);
		OptionBtn.Init(this, &Page, 664, 3, &OptionBtnSpr, 0,IDM_OPTION);
		MinimizeBtn.Init(this, &Page, 695, 3, &MinimizeBtnSpr, 0, IDM_MINIMIZE);
		ChangeDisplayBtn.Init(this, &Page, 718, 3, &FullScreenBtnSpr, 0, IDM_CHANGEDISPLAY);
		ExitBtn.Init(this, &Page, 741, 3, &ExitBtnSpr, 0, IDM_EXIT);
		GameItemBtn.Init(this, &Page, 565, 3, &ItemBagSpr, 0, IDM_GAMEITEM);
		CaptureBtn.Init(this, &Page, 536, 3, &CaptureSpr, 0, IDM_CAPTURE);//화면캡쳐
		ObserverViewBtn.Init(this, &Page, 110, 31, &ObserverSpr, 0, IDM_OBSERVERVIEW);	// ### [ 관전기능 ] ###  관전 리스트 보기 
		sInOut_Btn.Init(this, &Page, 193, 31, &ObserverSpr, 1,IDM_PARTICIPATION); // 참여, 관전
				
		DieBtn.Init(this, &Page, 560, 485, &DieBtnSpr, 0, IDM_DIE);
		CheckBtn.Init(this, &Page, 638, 485, &CheckBtnSpr, 0, IDM_CHECK);
		CallBtn.Init(this, &Page, 716, 485, &CallBtnSpr, 0, IDM_CALL);
		PingBtn.Init(this, &Page, 560, 538, &PingBtnSpr, 0, IDM_PING);
		DadangkBtn.Init(this, &Page, 638, 538, &DadangkBtnSpr, 0, IDM_DADANG);
		

		if(g_RI.GameKind == 1) // 1이면 하프 , 0이면 쿼터
		MaxBtn.Init(this, &Page, 716, 538, &MaxBtnSpr, 1, IDM_MAX); // 쿼터
		else 
		MaxBtn.Init(this, &Page, 716, 538, &MaxBtnSpr, 0, IDM_MAX); // 하프

		AllInBtn.Init(this, &Page, 716, 538, &AllinBtnSpr, 0, IDM_ALLIN);//올인

		//62cut button[62]
		ChangeCard_OK.Init(this, &Page, 500, 466, &ChangeOKBtnSpr, 0,IDM_CHANGE_OK);
		ChangeCard_Cancel.Init(this, &Page, 500, 518, &ChangeCancelBtnSpr, 0,IDM_CHANGE_CANCEL);

		MasterBtn.Init(this, &Page, 675, 451, &RoomMasterSpr, 0,IDM_MASTER);
		BanishvoteBtn.Init(this, &Page, 736, 451, &BanishVoteSpr, 0,IDM_BANISHVOTE);

		GameStartBtn.Init(this, &Page, 306, 298, &GameStartBtnSpr, 0,IDM_START);//시작버튼
		GameStartBtn.Show(FALSE);

	} else {
		m_pcarddeckspr = &BigCardSpr;
		// 그래픽 버튼 초기화
		InviteBtn.Init(this, &Page, 633, 3, &InviteBtnSpr, 0,IDM_INVITE);
		OptionBtn.Init(this, &Page, 664, 3, &OptionBtnSpr, 0,IDM_OPTION);
		MinimizeBtn.Init(this, &Page, 695, 3, &MinimizeBtnSpr, 0, IDM_MINIMIZE);
		ChangeDisplayBtn.Init(this, &Page, 718, 3, &FullScreenBtnSpr, 0, IDM_CHANGEDISPLAY);
		ExitBtn.Init(this, &Page, 741, 3, &ExitBtnSpr, 0, IDM_EXIT);
		GameItemBtn.Init(this, &Page, 565, 3, &ItemBagSpr, 0, IDM_GAMEITEM);
		CaptureBtn.Init(this, &Page, 536, 3, &CaptureSpr, 0, IDM_CAPTURE);//화면캡쳐
		ObserverViewBtn.Init(this, &Page, 110, 31, &ObserverSpr, 0, IDM_OBSERVERVIEW);	// ### [ 관전기능 ] ###  관전 리스트 보기 
		sInOut_Btn.Init(this, &Page, 193, 31, &ObserverSpr, 1,IDM_PARTICIPATION); // 참여, 관전
				
		DieBtn.Init(this, &Page, 556, 497, &DieBtnSpr, 0, IDM_DIE);
		CheckBtn.Init(this, &Page, 634, 497, &CheckBtnSpr, 0, IDM_CHECK);
		CallBtn.Init(this, &Page, 712, 497, &CallBtnSpr, 0, IDM_CALL);
		PingBtn.Init(this, &Page, 556, 543, &PingBtnSpr, 0, IDM_PING);
		DadangkBtn.Init(this, &Page, 634, 543, &DadangkBtnSpr, 0, IDM_DADANG);
		MaxBtn.Init(this, &Page, 712, 543, &MaxBtnSpr, 1, IDM_MAX);

		//62cut button[62]
		ChangeCard_OK.Init(this, &Page, 500, 466, &ChangeOKBtnSpr, 0,IDM_CHANGE_OK);
		ChangeCard_Cancel.Init(this, &Page, 500, 518, &ChangeCancelBtnSpr, 0,IDM_CHANGE_CANCEL);
	}

	BatPage.Init(m_betpagesize_x, m_betpagesize_y, 16);		// 배팅 공간용 페이지 초기화
	BatPage.PutSprAuto(0, 0, m_pgamebackspr, 1);			// 배팅 칩 게임판을 지운다

//	GameItemBtn.Enable(FALSE); //[62]// 게임아이템
}

// ### [ Frame++ ] ###
void CGameView::MessagePump()
{
	MSG msg;
	while( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}


LRESULT CGameView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	PrcAIOperation();

	return CWnd::WindowProc(message, wParam, lParam);
}

void CGameView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	if(g_Where != WH_GAME) return;

	// 게임판 배경을 그림
//	if(bSetInvalidate==TRUE) {
//		Page.SetTempClip(PlayScr[0].Xp, PlayScr[0].Yp, PlayScr[0].Xp + BBOX_WIDTH*16, PlayScr[0].Yp+BBOX_HEIGHT*16);
//		Page.PutSprAuto(0, 0, &GameBackSpr, 0, TEMPCLIP);
//	}
//	else {
	Page.PutSprAuto(0, 0, m_pgamebackspr, 0);
//	}
	Page.PutFromPage(m_betpage_x, m_betpage_y, &BatPage);//////// 페이지 /////////

	// 더블 버퍼링용 메모리 DC 생성
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	MemDC.SelectObject(Page.hBmp);
	MemDC.SetBkMode(TRANSPARENT);

	// 그래픽 버튼 그리기
	ExitBtn.Draw(&MemDC);
	InviteBtn.Draw(&MemDC);
	ChangeDisplayBtn.Draw(&MemDC);
	OptionBtn.Draw(&MemDC);

//	MasterBtn.Draw(&MemDC);
	MinimizeBtn.Draw(&MemDC);
	GameItemBtn.Draw(&MemDC);
	CaptureBtn.Draw(&MemDC);

	//[62]
	if(g_bChangeCardStage && !g_MyObserver && Play[0].PlayState == 1 && Play[0].JoinState == 1 
		&& Game.m_nchange_card > 1000 / int(1000.0f/REFRESH_FPS)){
		ChangeCard_OK.Draw(&MemDC);
		ChangeCard_Cancel.Draw(&MemDC);
	}

	// ### [ 관전기능 ] ###
	//sInOut_Btn.Draw(&MemDC);
	//ObserverViewBtn.Draw(&MemDC);
	

	// 게임 진행관련 버튼
	DieBtn.Draw(&MemDC);
	CheckBtn.Draw(&MemDC);
	CallBtn.Draw(&MemDC);
	PingBtn.Draw(&MemDC);
	DadangkBtn.Draw(&MemDC);
	MaxBtn.Draw(&MemDC);
	if(m_bAllin)AllInBtn.Draw(&MemDC);//올인

	if(GameStartBtn.GetButtonState() > -1)
	{
		Page.PutSprAuto(306, 296, &EtcBtnSpr, 19,10,10);
	//	Page.PutSprAuto(316, 305, &EtcBtnSpr, 19,10,10);
		GameStartBtn.Draw(&MemDC); //시작버튼		
	}

//	MasterBtn.Draw(&MemDC);
//	BanishvoteBtn.Draw(&MemDC);
//	SortBtn.Draw(&MemDC);
	// 정보창 그리기
//	InfoBox.Draw(&MemDC);
		// 채팅박스 그리기
	ChatBox.Draw(&MemDC);

	// 캐릭터 박스 그리기
	CharBox[0].Draw(&MemDC);
	CharBox[1].Draw(&MemDC);
	CharBox[2].Draw(&MemDC);
	CharBox[3].Draw(&MemDC);
	CharBox[4].Draw(&MemDC);
	CharBox[5].Draw(&MemDC);
	
	

//################################################### 
// 임시
	// 플레이 스크린 그리기
	
	PlayScr[3].Draw(&MemDC);
	PlayScr[0].Draw(&MemDC);
	PlayScr[1].Draw(&MemDC);
	PlayScr[2].Draw(&MemDC);
	PlayScr[4].Draw(&MemDC);
	PlayScr[5].Draw(&MemDC);
	PlayScr[0].Draw(&MemDC);
	

#ifdef _DEBUG
	// 프레임 레이트 체크
	static float FPS = 0;
	static int framecnt = 0;
	static DWORD lasttick = timeGetTime();
	DWORD nowtick = timeGetTime();
	DWORD deltatick = nowtick - lasttick;
	framecnt++;
	if(deltatick >= 1000) {
		lasttick = nowtick;
		float secrate = (deltatick / 1000.0f) - 1.0f;
		FPS = (float)framecnt - (float)framecnt*secrate;
		framecnt = 0;
	}

	// 마우스 좌표 체크
	POINT mpos;
	GetCursorPos(&mpos);
	ScreenToClient(&mpos);
	CString str;
	str.Format("%5.1f FPS, Mouse (%d, %d)", FPS, mpos.x, mpos.y);

	MemDC.SelectObject(&Font2);
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(255,255,255));
	if(g_RI.bPlayer==0)
		MemDC.TextOut(530,300, str);		// change position of MouseInfo - jeong
		//MemDC.TextOut(327,72, str);
	else
		MemDC.TextOut(20,65, str);
/*
	//카드 값 표시
	for(int i=0;i<g_Max_Player;i++)
	{
		if(!g_MyObserver && Play[i].PlayState == 1)
		{
			str.Format("carddeck data       %s: %d  %d  %d  %d",Play[i].UI.ID,Game.CardDeck[i].GetCardNo(2),Game.CardDeck[i].GetCardNo(3),Game.CardDeck[i].GetCardNo(4),Game.CardDeck[i].GetCardNo(5));
			MemDC.TextOut(180,85+(i*30), str);
			int sp = Play[i].ServPNum;			
			str.Format("servercarddeck data %s: %d  %d  %d  %d (2cut)%d",Play[i].UI.ID,g_Poker.PS[sp].nCard[0],g_Poker.PS[sp].nCard[1],g_Poker.PS[sp].nCard[2],g_Poker.PS[sp].nCard[3],g_Poker.BC[sp].nCard[2]);
			MemDC.TextOut(180,100+(i*30), str);			
		}
	}
*/
#endif

	// ### [ 관전기능 ] ### 
	CString stro;
	if(g_RI.bPlayer==0) {

		//관전자 이미지그리기
		int cnt=0;
		for(int i=g_Max_Player; i<g_Max_NewPlayer-1; i++) {
			if(strlen(Play[i].UI.ID)==0) continue;
			Page.PutSprAuto(61+cnt*12, 37, g_pGameView->m_pgamebackspr, 7 + Play[i].UI.Sex );
			cnt++;
		}

		MemDC.SetTextColor( RGB(255, 255, 255) );
		MemDC.TextOut( 110, 8, m_titlestr );
		
	//	if(g_RI.bSuperMaster) {
	//		// 슈퍼방이면 슈퍼방임을 보여줌
	//		Page.PutSprAuto(540, 40, g_pGameView->m_pgamebackspr, 11+(Game.GameCnt/8)%5);
	//	}
		
		// ### [ 관전기능 ] ### 
		if(g_MyObserver) {
			//	if((Game.GameCnt/25)%2==0)
			Page.PutSprAuto(270, 80, g_pGameView->m_pgamebackspr, 5+(Game.GameCnt/25)%2);
		}
	} else {
		//관전자 이미지그리기
		int cnt=0;
		for(int i=g_Max_Player; i<g_Max_NewPlayer-1; i++) {
			if(strlen(Play[i].UI.ID)==0) continue;
			Page.PutSprAuto(60+cnt*12, 43, g_pGameView->m_pgamebackspr, 7 + Play[i].UI.Sex );
			cnt++;
		}

		MemDC.SetTextColor( RGB(255, 255, 255) );
		MemDC.TextOut( 122, 8, m_titlestr );
		
		//if(g_RI.bSuperMaster) {
			// 슈퍼방이면 슈퍼방임을 보여줌
		//	Page.PutSprAuto(540, 40, g_pGameView->m_pgamebackspr, 11+(Game.GameCnt/8)%5);
		//}
		
		// ### [ 관전기능 ] ### 
		if(g_MyObserver) {
			//	if((Game.GameCnt/25)%2==0)
			Page.PutSprAuto(274, 194, g_pGameView->m_pgamebackspr, 5+(Game.GameCnt/25)%2);
		}
	}


	//MemDC.SetTextColor( RGB(255, 255, 255) );
	//MemDC.TextOut( 178, 8, m_titlestr );
	// 버튼 튤팁 보여주기
	//MemDC.SetTextColor( RGB(225,250,255) );
	//MemDC.TextOut( TipX, TipY, m_BtnMsgTip );
	
	CRect rect;
	GetClientRect(&rect);

	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);
	// 부분 갱신 플래그 해제
//	bSetInvalidate = FALSE;
	MemDC.DeleteDC();

	// Do not call CWnd::OnPaint() for painting messages

		/*
	// 게임 도움말 메세지 그리기
	if(Game.HelpMsgCnt > 0)
	{
		WORD bcolor;
		if(Game.HelpMsgKind==0) bcolor = RGBmix(20,20,31);
		else bcolor = RGBmix(31,31,0);
		CRect grc(0, 0, 170, 25);
		grc.OffsetRect(315, 245);
		Page.DrawFillBoxGB(grc.left, grc.top, grc.right, grc.bottom, bcolor, 10);
		grc.DeflateRect(2,2);
		Page.DrawFillBoxGB(grc.left, grc.top, grc.right, grc.bottom, bcolor, 10);

		grc.OffsetRect(0, 7);
		MemDC.SetBkMode(TRANSPARENT);
		MemDC.SelectObject(&Font2);
		MemDC.SetTextColor(RGB(50,50,50));
		MemDC.DrawText(Game.szHelpMsg, &grc, DT_CENTER);
		grc.OffsetRect(-1, -1);
		MemDC.SetTextColor(RGB(255, 255, 255));
		MemDC.DrawText(Game.szHelpMsg, &grc, DT_CENTER);
	}

	// 유리 메세지 상자를 찍어줌
	if(Game.GlassMsgCnt > 0)
	{
		Game.GlassMsgGrade++;

		int nowgrade;
		if(Game.GlassMsgGrade > 10) nowgrade = 10;
		else nowgrade = Game.GlassMsgGrade;
		int textcolor = 255 - (int)(255 * (float)nowgrade/10);

		CRect grc(0, 0, 310, 45);
		grc.OffsetRect(245, 230);
		Page.DrawFillBoxGB(grc.left, grc.top, grc.right, grc.bottom, Game.GlassMsgColor, nowgrade);
		grc.DeflateRect(5,5);
		Page.DrawFillBoxGB(grc.left, grc.top, grc.right, grc.bottom, Game.GlassMsgColor, nowgrade);

		grc.OffsetRect(0, 10);
		MemDC.SetBkMode(TRANSPARENT);
		CString str;
		MemDC.SelectObject(&Font1);
		MemDC.SetTextColor(RGB(80,80,80));
		MemDC.DrawText(Game.szGlassMsg, &grc, DT_CENTER);
		grc.OffsetRect(-1, -1);
		MemDC.SetTextColor(RGB(textcolor,textcolor,textcolor));
		MemDC.DrawText(Game.szGlassMsg, &grc, DT_CENTER);
	}

	// 응답 대기 메세지 그리기
	if(Game.bGameStart==TRUE)
	{
		if(Game.TotalTurnCnt==0)
		{
			Page.DrawFillBoxGB(245,270,555,340, RGBmix(31,10,0), 9);
			Page.DrawFillBoxGB(250,275,545,335, RGBmix(31,10,0), 10);
			MemDC.SetBkMode(TRANSPARENT);
	
			CString str;
			MemDC.SelectObject(&Font1);
			MemDC.SetTextColor(RGB(0,0,0));
			str.Format("게임 종료 처리 중입니다");
			MemDC.TextOut(315, 295, str);
		}
		else if(Game.TotalTurnCnt<110)
		{
			Page.DrawFillBoxGB(245,270,555,340, RGBmix(31,31,0), 9);
			Page.DrawFillBoxGB(250,275,545,335, RGBmix(31,31,0), 10);
			MemDC.SetBkMode(TRANSPARENT);
	
			CString str;
			MemDC.SelectObject(&Font1);
			MemDC.SetTextColor(RGB(0,0,0));
			str.Format("상대방의 응답을 기다리는 중입니다");
			RECT helprc;
			SetRect( &helprc, 250, 275, 545, 335 );
			OffsetRect( &helprc, 0, 10 );
//			MemDC.TextOut(265, 285, str);
			MemDC.DrawText( str, &helprc, DT_CENTER );
			MemDC.SetTextColor(RGB(0,0,130));
			str.Format("[남은시간 : %d초]", Game.TotalTurnCnt);
			OffsetRect( &helprc, 0, 25 );
			MemDC.DrawText( str, &helprc, DT_CENTER );
		}
	}
*/
}

void CGameView::Reset()
{
	bExitSubscript = FALSE;
}

// 나의 부분만 갱신
void CGameView::SetInvalidate()
{
//	bSetInvalidate = TRUE;
//	CRect rc(PlayScr[0].Xp, PlayScr[0].Yp, PlayScr[0].Xp+BBOX_WIDTH*16, PlayScr[0].Yp+BBOX_HEIGHT*16);
//	InvalidateRect(rc, FALSE);
}

void CGameView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent==GAMEVIEW_TIMER)
	{
		if(g_Where == WH_GAME)
		{
			Game.OnTimer();
			
			ChatBox.OnTimer();

			int skipframe = syncfps.FrameSync(FALSE); // 동기화
			// 스킵된 프레임만큼 프레임 진행
			for(int i=0; i<skipframe; i++) {
			//	Game.OnTimer();
			}

			Invalidate(FALSE); 
		}
	}
	CWnd::OnTimer(nIDEvent);
}

void CGameView::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	//if(hTimer!=0) KillTimer(hTimer);

	// ### [ Frame++ ] ###
	if(m_hEventTimer) timeKillEvent(m_hEventTimer);
	m_hEventTimer = 0;


	Font1.DeleteObject();
	Font2.DeleteObject();
}


BOOL CGameView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	
	// TODO: Add your specialized code here and/or call the base class
	switch(wParam)
	{
	case IDM_INVITE:
		// 사운드 출력
		Sound.Play(SND45);

		if(g_pGameDlg->m_InviteDlg.IsWindowVisible())
		{
			g_pGameDlg->m_InviteDlg.SendMessage(WM_COMMAND, IDCANCEL);
			g_pGameDlg->m_InviteDlg.CenterWindow(this);
			g_pGameView->ChatBox.SendEdit.SetFocus();
		}
		else 
		{
			//g_pGameDlg->m_InviteDlg.User_ReloadList();
			g_pGameDlg->m_InviteDlg.m_UserList.DeleteAllItems();
			//g_pGameDlg->m_InviteDlg.MoveToSide();
			g_pGameDlg->m_InviteDlg.ShowWindow(SW_SHOW);
			g_pGameDlg->m_InviteDlg.CenterWindow(this);
			g_pGameView->ChatBox.SendEdit.SetFocus();

			CSV_LOBYOPEN lomsg;
			lomsg.Set(TRUE);
			SockMan.SendData(g_MainSrvSID, lomsg.pData, lomsg.GetTotalSize());
		}
		break;
	case IDM_EXIT:
		{
		// 사운드 출력
		Sound.Play(SND45);

		/*
		if(g_MyInfo.UI.PMoney <= 0){
			CString str;
			
			if(g_Where == WH_GAME){
				g_Where = WH_LOBY;
				g_pMainDlg->OnCancel();
			}				
			str.Format("게임머니가 부족하여, 게임을 계속 진행할 수 없습니다. 뉴포커(62Cut)페이지 상단의 충전소 버튼을 클릭하여 게임머니를 충전 받으시기 바랍니다.");
			g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)str.operator LPCTSTR());
		//	COring dlg;
		//	if(dlg.DoModal() == IDOK) {
		//		g_Wait = TRUE;				
		//		g_pMainDlg->OnCancel();
		//	}		

			g_Wait = TRUE;
			g_pMainDlg->bNowLogout = TRUE;	
			g_pMainDlg->OnCancel();
		}
		*/

		if(bExitSubscript==FALSE) // 예약
			Game.DoExitSubscript(1);
		else
			Game.DoExitSubscript(2);
		
/*
		//if(Game.bGameStart == TRUE)
		if(g_RI.State==1 && Play[0].JoinState==1 && Play[0].PlayState==1)
		{
			if(bExitSubscript==FALSE)
			{
				CMsgBox msgbox;
				int rtn = msgbox.DoModal("나가기 예약을 하시겠습니까?", "나가기 예약");
				if(rtn==IDOK)
				{
					bExitSubscript=TRUE;
					//Game.DoExitSubscript(0, TRUE);
					Game.DoExitSubscript(TRUE);
				}
			}
			else
			{
				CMsgBox msgbox;
				int rtn = msgbox.DoModal("나가기 예약을 취소 하시겠습니까?", "나가기 예약 취소");
				if(rtn==IDOK)
				{
					bExitSubscript=FALSE;
					//Game.DoExitSubscript(0, FALSE);
					Game.DoExitSubscript(FALSE);
				}
			}
		}
		else g_pGameDlg->ExitGame();
	*/
		}
		break;
	
	case IDM_CHANGEDISPLAY:
		{
			// 사운드 출력
			Sound.Play(SND45);
			g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_CHANGEDISPLAY);
		} break;

	case IDM_OPTION:
		{
			// 사운드 출력
			Sound.Play(SND45);
			COptionDlg Dlg;
			
			// ### [ 관전기능 ] ###
			int tmpSnd = g_MyInfo.nSndFxKind;
			if(Dlg.DoModal() == IDOK && g_Where == WH_GAME && tmpSnd != g_MyInfo.nSndFxKind)// ### [사운드 추가 작업] ###
			{
				// 서버에 알린다
				CSV_ASK_CHANGESNDFX sndFx;
				sndFx.Set(g_MyInfo.UI.UNum, g_MyInfo.ServPNum, g_RI.RoomNum, g_MyInfo.UI.ID,g_MyInfo.nSndFxKind);
				SockMan.SendData(g_MainSrvSID, sndFx.pData, sndFx.GetTotalSize());

			}
		} break;

	case IDM_BADUSER:
		{
			Sound.Play(SND45);
			CBadUserDlg Dlg;
			if(Dlg.DoModal() == IDOK)
			{
				CString strwatchid, strchat;
				ChatBox.ChatViewEdit.GetWindowText(strchat);
				if(ChatBox.ChatViewEdit.GetLineCount()<4 || strchat.GetLength()<10){ g_pMainDlg->m_MsgBoxDlg.MsgBox(0, "신고할 내용이 너무 짧습니다"); break; }

				// 4000자 이상은 잘라냄
				if(strchat.GetLength()>4000) strchat = strchat.Mid(strchat.GetLength() - 4000);

				int totcnt = 0;
				for(int i=1; i<3; i++)
				{
					if(strlen(Play[i].UI.ID)>0)
					{
						if(totcnt>0) strwatchid += ',';
						strwatchid += Play[i].UI.ID;
						totcnt++;
					}
				}

				// ### [ 관전기능 ] ###
				CSV_ASK_BADUSER abumsg;
				abumsg.Set(Dlg.m_bAnnounce, g_RI.RoomNum, g_MyInfo.UI.UNum, g_MyInfo.UI.ID, 
					(char*)Dlg.m_TargetID.operator LPCTSTR(),
					(char*)Dlg.m_SubmitReason.operator LPCTSTR(),
					(char*)strwatchid.operator LPCTSTR(), (char*)strchat.operator LPCTSTR());
				SockMan.SendData(g_MainSrvSID, abumsg.pData, abumsg.GetTotalSize());

				// 신고기능 이용회수 카운터 증가
				Dlg.SubmitCnt++;
				// 신고기능 딜레이 타이머 세팅
				g_pMainDlg->ResetBadUserTimer();
			}
		}
		break;
	case IDM_MINIMIZE:
		{
			g_pMainDlg->ShowWindow(SW_MINIMIZE);


		}
		break;
	
	case IDM_GAMEITEM:
		{

			g_pMainDlg->ShowMyItemBag();
		} 
		break;

	case IDM_MASTER:
		{
			CRoomOptionDlg dlg;
			dlg.DoModal();			
		}
		break;

	case IDM_BANISHVOTE:
		{			
			if(g_RI.NowUserNum <3){
				g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"방인원이 3명이상일때 가능합니다.");
				break;
			}
			
			CBanishVoteDlg dlg;
			dlg.DoModal();
			
		}break;

	case IDM_SORT:
		{

		}break;
		/////////////////////////////////////////////////////////////////////////////////////////
	//  [ 세븐 포커 ]
	case IDM_DIE:
		{
			// 사운드 출력
			Sound.Play(SND45);
			OnBtnFold();
	
		}
		break;

	case IDM_CHECK:
		{
			// 사운드 출력
			Sound.Play(SND45);
			OnBtnCheck();
	

		}
		break;

	case IDM_CALL:
		{
			// 사운드 출력
			Sound.Play(SND45);
			OnBtnCall();

		}
		break;
	
	case IDM_PING:
		{
			// 사운드 출력
			Sound.Play(SND45);
			OnBtnPing();
		}
		break;
	
	case IDM_DADANG:
		{
			// 사운드 출력
			Sound.Play(SND45);
			OnBtnDda();
		}
		break;	
	case IDM_MAX:
		{
			// 사운드 출력
			Sound.Play(SND45);
			OnBtnMax();
		}
		break;
	case IDM_ALLIN:
		{
			OnAllInBtn();//올인
			Sound.Play(SND45);
		}break;
	case IDM_CAPTURE:
		{
		
			g_pMainDlg->DoScreenCapture();

				
			/////////////////////////////////////////////////////
		/*
			Game.m_JackPotActive = 545646545646546;
			g_TmpJackPotMoney = 545445455445;
			g_JackPotMoney = 8989998989898989;
			break;
		*/
		
		/*

			Game.SetBackTime();
			
			
			//테스트
			POINT pt;
			for(int i=0; i<g_Max_Player;i++)  {
				Game.CardDeck[i].Reset();
				PlayScr[i].playscr_changeinitpos();
			}
		
			
			for(int j=0; j<6;j++)
			{
				for( i=0; i<g_Max_Player;i++) {
					Game.CardDeck[i].NewCardSet( rand()%52);
					Game.CardDeck[i].Card[j].bFront = TRUE;
					pt.x = 362;
					pt.y = 0;
					Game.CardDeck[i].Card[j].m_move.SetCurPos(pt);
					
					
					pt.x = Game.CardDeck[i].Card[j].Xp;
					pt.y = Game.CardDeck[i].Card[j].Yp;
					
					Game.CardDeck[i].Card[j].StraightMoveTo(pt, 10*3, 18*3, i*5+j*10);
				}
			}
		*/	
			/////////////////////////////////////////////////////

		} break;

	///////////////////////////////////////////////////////////
	// ### [ 관전기능 ] ###
	case IDM_PARTICIPATION: //겜참여
		{
			Participation_In();

		} break;
	case IDM_EMERGE:
		{
			Emerge_Out();
		} break;
	case IDM_OBSERVERVIEW:
		{
			Sound.Play(SND45);
			if(g_pGameDlg->m_ObserverDlg.IsWindowVisible())	{
				g_pGameDlg->m_ObserverDlg.SendMessage(WM_COMMAND, IDCANCEL);
				g_pGameDlg->m_ObserverDlg.CenterWindow(this);
				g_pGameView->ChatBox.SendEdit.SetFocus();
			}
			else {
				//g_pGameDlg->m_ObserverDlg.m_UserList.DeleteAllItems();
				g_pGameDlg->m_ObserverDlg.ShowWindow(SW_SHOW);
				g_pGameView->ChatBox.SendEdit.SetFocus();
					
			}
		} break;
	///////////////////////////////////////////////////////////
		
		///////////////////////////////////////////////////////////
		// 족보등급 보이기 , 채팅창 보이기
	case IDM_TABVIEWCHAT:
		{
			Game.SetChangeViewTabBtn(FALSE);

		} break;
		
	case IDM_TABVIEWRULE:
		{
			Game.SetChangeViewTabBtn(TRUE);
		} break;

		//62cut
	case IDM_CHANGE_OK:
		{			
			Game.SetChangeCard_62Cut();
			Sound.Play(SND45);
		}break;

	case IDM_CHANGE_CANCEL:
		{
			Game.SetChangeCard_Cancel_62Cut();
			Sound.Play(SND45);
		}break;
		
	case IDM_START://시작버튼
		{			
			CCL_ASK_STARTGAME smsg;
			smsg.Set(g_RI.RoomNum,g_MyInfo.ServPNum,g_MyInfo.UI.ID);
			SockMan.SendData(g_MainSrvSID, smsg.pData, smsg.GetTotalSize());
			Sound.Play(SND45);	
			GameStartBtn.Show(FALSE);
		}break;

/*	
	/////////////////////////////////////////////////////////////////////////////////////////
	// [ 하이로우 ]
	case IDM_HIGH:
		{
			// 사운드 출력
			Sound.Play(SND45);


		}
		break;
	
	case IDM_LOW:
		{
			// 사운드 출력
			Sound.Play(SND45);


		}
		break;
	
	case IDM_SWING:
		{
			// 사운드 출력
			Sound.Play(SND45);


		}
		break;
	*/
	}
	
	
	return CWnd::OnCommand(wParam, lParam);
}

void CGameView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CGameView::ClearFlag()
{
	m_bBPing=m_bBCheck=m_bBCall=m_bBMax=m_bBDDa=m_bAllin=FALSE;
}
	

void CGameView::SetBtnState(BOOL bDisable)
{
	if(bDisable){
		ClearFlag(); // 버튼플래그 클리어
//		m_bTurn = FALSE;

		DieBtn.Enable(FALSE);
		CheckBtn.Enable(FALSE);
		CallBtn.Enable(FALSE);
		PingBtn.Enable(FALSE);
		DadangkBtn.Enable(FALSE);
		MaxBtn.Enable(FALSE);
		ChangeCard_OK.Enable(FALSE);
		ChangeCard_Cancel.Enable(FALSE);
		AllInBtn.Enable(FALSE);
		AllInBtn.Show(FALSE);
	//	GameStartBtn.Show(FALSE);//시작버튼

		return;
	}
	
	// ### [ 관전기능 ] ###
	if( !g_MyObserver && Play[0].PlayState == 1 )// && Game.CurPlayer == Play[0].ServPNum)
	{
		ClearFlag();
		//시작버튼 비활성화
	//	GameStartBtn.Show(FALSE);

		int nMyIndex = Play[0].ServPNum;
		RAISEUSER ru; memset(&ru,0,sizeof(ru));
		//if(g_Poker.PS[nMyIndex].bActive)
		if(Game.CurPlayer == nMyIndex )
		{ // 내차례이다
			memcpy(&ru,&g_Poker.RU[nMyIndex],sizeof(ru));
			if(ru.bFold) return;
			INT64 nTotal = Game.GetRealBet();//g_Poker.nTotBat;
			INT64 nRaise = Game.GetRaiseBat();//g_Poker.nRaiseBat;
			//GetDlgItem(IDC_BTN_FOLD)->EnableWindow(TRUE); // 포기
			DieBtn.Enable(TRUE);
			if(g_Poker.bDistCard){
				//GetDlgItem(IDC_BTN_CHECK)->EnableWindow(TRUE); // 체크
				CheckBtn.Enable(TRUE);
				m_bBCheck = TRUE;
			}
			else{
				if(g_Poker.bBtnCheck){
					//GetDlgItem(IDC_BTN_CHECK)->EnableWindow(TRUE);  // 체크
					CheckBtn.Enable(TRUE);
					m_bBCheck = TRUE;
				}
				else{
					//GetDlgItem(IDC_BTN_CALL)->EnableWindow(TRUE);  // 콜
					CallBtn.Enable(TRUE);
					m_bBCall = TRUE;
				}
			}
			BOOL bOutable = FALSE;
			if(g_Poker.bBtnBet == FALSE && !g_Poker.bDistCard) return;
			if(ru.bCall == FALSE){ // 콜한적없다..배팅가능
				if(ru.nPMoney>nRaise){
					if(nRaise==0){
						// 초구나 7구만 삥가능하게 한다!!!!
						if(g_Poker.nRound==1 || g_Poker.nRound==5){
							//GetDlgItem(IDC_BTN_PING)->EnableWindow(TRUE); // 삥
							PingBtn.Enable(TRUE);
							m_bBPing = TRUE;
						}
					}
					if(nRaise>0 && !g_Poker.bDistCard){ // 한명이라도 엔코직전이라면...한다
						//GetDlgItem(IDC_BTN_DDA)->EnableWindow(TRUE);  // 따당
						DadangkBtn.Enable(TRUE);
						m_bBDDa = TRUE;
					}
					if(ru.nPMoney>Game.GetMaxMoney(nTotal)){
						//GetDlgItem(IDC_BTN_MAX)->EnableWindow(TRUE);  // 최대
						MaxBtn.Enable(TRUE);
						m_bBMax = TRUE;
					}					
				}
			}
			
			if(MaxBtn.GetButtonState() == 0 && ru.nPMoney > 0 && ru.nPMoney < Game.GetMaxMoney(nTotal))//올인
			{
				AllInBtn.Show(TRUE);
				AllInBtn.Enable(TRUE);
				m_bAllin = TRUE;
			}
		}
	}

	//AI 배팅버튼 - jeong
	//AISelectBetMode();

}

void CGameView::AISelectBetMode()
{
	// 배팅 대기시간 및 추후 알고리즘에 맞게 수정해야 함 - jeong
	//int nWaitTime = rand() % 8;
	//Sleep(nWaitTime*1000);

	//OnBtnFold();
	
	if( m_bAllin )
	{
		OnAllInBtn();
	}
	else if( m_bBDDa )
	{
		OnBtnDda();
	}
	else if ( m_bBMax )
	{
		OnBtnMax();
	}
	else if ( m_bBCall )
	{
		OnBtnCall();
	}
	else if ( m_bBCheck )
	{
		OnBtnCheck();
	}
	else if ( m_bBPing )
	{
		OnBtnPing();
	}
	else
		OnBtnFold();

	return;
}

/////////////////////////////////////////////////////////////////////////////////
// [세븐 포커] 베팅 버튼
//올인
void CGameView::OnAllInBtn()
{
	if(g_MyObserver == TRUE) return;
	if( !m_bAllin || Play[0].PlayState == 0) return;

	SetBtnState();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅
	pc.nBtn   = 4; // 올인

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  Play[0].UI.UNum, Play[0].ServPNum, Play[0].UI.ID, &pc);
	SockMan.SendData(g_MainSrvSID, raise.pData, raise.GetTotalSize());

	Game.bBtnClick = TRUE;
}

void CGameView::OnBtnPing()
{
	if(g_MyObserver == TRUE) return;
	if( !m_bBPing || Play[0].PlayState == 0) return;

	SetBtnState();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅
	pc.nBtn   = 0; // 삥

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  Play[0].UI.UNum, Play[0].ServPNum, Play[0].UI.ID, &pc);
	SockMan.SendData(g_MainSrvSID, raise.pData, raise.GetTotalSize());

	Game.bBtnClick = TRUE;
}

void CGameView::OnBtnDda()
{
	if(g_MyObserver == TRUE) return;
	if( !m_bBDDa || Play[0].PlayState == 0) return;
	
	SetBtnState();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅
	pc.nBtn   = 1; // 따당

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  Play[0].UI.UNum, Play[0].ServPNum, Play[0].UI.ID, &pc);
	SockMan.SendData(g_MainSrvSID, raise.pData, raise.GetTotalSize());
	Game.bBtnClick = TRUE;
}

void CGameView::OnBtnMax()
{
	if(g_MyObserver == TRUE) return;
	if( !m_bBMax || Play[0].PlayState == 0 ) return;

	SetBtnState();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅
	pc.nBtn   = 2; // 맥스

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  Play[0].UI.UNum, Play[0].ServPNum, Play[0].UI.ID, &pc);
	SockMan.SendData(g_MainSrvSID, raise.pData, raise.GetTotalSize());
	Game.bBtnClick = TRUE;
}

void CGameView::OnBtnFold()
{
	if(g_MyObserver == TRUE) return;
	if(Play[0].PlayState == 0) return;

	SetBtnState();

	POKERCLNT pc; 
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 0; // 게임포기
	pc.nBtn   = 0; // 삥

	CCL_ASK_FOLD fold;
	fold.Set(g_RI.RoomNum,  Play[0].UI.UNum, Play[0].ServPNum, Play[0].UI.ID, &pc);
	SockMan.SendData(g_MainSrvSID, fold.pData, fold.GetTotalSize());
	Game.bBtnClick = TRUE;
}

void CGameView::OnBtnCheck()
{
	if(g_MyObserver == TRUE) return;
	if( !m_bBCheck || Play[0].PlayState == 0) return;

	SetBtnState();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 1; // 배팅
	pc.nBtn   = 0; // 삥

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  Play[0].UI.UNum, Play[0].ServPNum, Play[0].UI.ID, &pc);
	SockMan.SendData(g_MainSrvSID, raise.pData, raise.GetTotalSize());
	Game.bBtnClick = TRUE;
}

void CGameView::OnBtnCall()
{
	if(g_MyObserver == TRUE) return;
	if( !m_bBCall || Play[0].PlayState == 0) return;

	SetBtnState();

	POKERCLNT pc; 
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 2; // 배팅
	pc.nBtn   = 0; // 삥

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  Play[0].UI.UNum, Play[0].ServPNum, Play[0].UI.ID, &pc);
	SockMan.SendData(g_MainSrvSID, raise.pData, raise.GetTotalSize());
	Game.bBtnClick = TRUE;
}

// ### [ 관전기능 ] ###
//겜참여 요청(관전 -> 겜참여)
void CGameView::Participation_In()
{
	if(g_MyObserver  == 1) 
	{
		CSV_ASK_PARTICIPATION parti;
		parti.Set(g_RI.RoomNum,  g_MyInfo.UI.UNum, g_MyInfo.ServPNum, g_MyInfo.UI.ID, g_MyInfo.nSndFxKind);
		SockMan.SendData(g_MainSrvSID, parti.pData, parti.GetTotalSize() );
	}

}

// ### [ 관전기능 ] ###
//참여중에서 대기로(관전)
void CGameView::Emerge_Out()
{
	if(g_MyObserver  == 0 &&  strcmp(g_RI.ID, g_MyInfo.UI.ID)!=0) // 겜중이 아닐때
	{
		CSV_ASK_EMERAGE emerge;
		emerge.Set(g_RI.RoomNum,  g_MyInfo.UI.UNum, g_MyInfo.ServPNum, g_MyInfo.UI.ID);
		SockMan.SendData(g_MainSrvSID, emerge.pData, emerge.GetTotalSize() );
		return;
	}
	if(strcmp(g_RI.ID, g_MyInfo.UI.ID)==0) {
		CString str;
		str.Format("방장은 관전으로 이동할 수 없습니다\n");
		g_pGameDlg->m_GameView.ChatBox.AddText(&str, RESERVER_COLOR);
	}
}

// ### [ 관전기능 ] ###
//관전, 참여
void CGameView::SetObserverBtn()
{
	int x =193;
	int y = 31;
	
//	if(g_RI.bPlayer!=0) {
//		x = 468;
//		y = 64;
//	}

	if(g_MyObserver == TRUE) //관전 -> 참여
	{
		sInOut_Btn.Init(this, &Page, x, y, &ObserverSpr, 1, IDM_PARTICIPATION); // 참여
	}
	else                     //참여 -> 관전
	{
		sInOut_Btn.Init(this, &Page, x, y, &ObserverSpr, 2, IDM_EMERGE); // 관전
	}

	//SetObserverViewBtn(g_MyObserver);
	g_pGameDlg->m_ObserverDlg.SetObserverViewBtn();
}

void CGameView::gameview_changeinitpos()
{
	if(g_RI.bPlayer == 0) {

		m_pgamebackspr = &GameBackSpr;
		m_pcarddeckspr = &BigCardSpr;
		m_betpage_x = 270;
		m_betpage_y = 105;
		m_diameter = 210;
		m_betpagesize_x = 258;
		m_betpagesize_y = 248;

	} else {

		m_pgamebackspr = &GameBackSpr_7;
		m_pcarddeckspr = &BigCardSpr;
		m_betpage_x = 270;
		m_betpage_y = 105;
		m_diameter =  210;
		m_betpagesize_x = 258;
		m_betpagesize_y = 248;
	}
	//버튼 이미지
	pagebutton_init();
}

// 화면좌표 변경
void CGameView::all_change_initpos(int bplayer)
{
	if(g_nchannel_usernum==0) {
		if(bplayer == 0) {
			g_Max_Player    = MAX_PLAYER - 1 ;
			g_Max_NewPlayer = g_Max_Player + MAX_OBSERVER;
			
			g_RI.bPlayer = 0;
		}
		else {
			g_Max_Player    = MAX_PLAYER;
			g_Max_NewPlayer = g_Max_Player + MAX_OBSERVER;
			
			g_RI.bPlayer = 1;
		}
	} else if(g_nchannel_usernum==1) {
		g_Max_Player    = MAX_PLAYER - 1 ;
		g_Max_NewPlayer = g_Max_Player + MAX_OBSERVER;
		
		g_RI.bPlayer = 0;
	} else {
		g_Max_Player    = MAX_PLAYER;
		g_Max_NewPlayer = g_Max_Player + MAX_OBSERVER;
		
		g_RI.bPlayer = 1;
	}

	gameview_changeinitpos();
	ChatBox.chat_changeinitpos();
	for(int i=0; i<g_Max_Player; i++) {
		CharBox[i].charbox_changeinitpos();
		PlayScr[i].playscr_changeinitpos();
		Game.CardDeck[i].carddeck_changeinitpos();
	}
}

//[62] 자동 배팅  스페이스바
void CGameView::OnAutoBatBtn()
{
	
#ifndef _DEBUG
	/*
	if(m_bBMax){
		OnBtnMax();return;
	}
	else if(m_bBDDa){
		OnBtnDda();return;
	}
	else if(m_bBCall){
		OnBtnCall();return;
	}	
	else if(m_bBPing){
		OnBtnPing();return;
	}
	else if(m_bBCheck){
		OnBtnCheck();return;
	}
	*/

	if(GameStartBtn.GetButtonState() > -1 && g_RI.State == 0)
	SendMessage(WM_COMMAND,IDM_START,0);

#else if

	if(GameStartBtn.GetButtonState() > -1 && g_RI.State == 0)
	SendMessage(WM_COMMAND,IDM_START,0);

	if(!m_bBPing && !m_bBCheck && !m_bBCall && !m_bBMax && !m_bBDDa)return;
	int LastPnum = g_Poker.LastPlayer;			
	int nLastCmd = g_Poker.RU[LastPnum].nLastCmd;
	if(6 == nLastCmd || 3 == nLastCmd)OnBtnCall();
	else{
		OnBtnMax();
	}		
#endif
}














/*

#include "CUtil.h"
#include "FolderLib\\Ini.h"

//	스크린샷 저장 bmp or jpg
BOOL CGameView::SaveScreenShot( int Quality )
{
	SetFocus();
	CString strFileExt = "jpg";


	CString strConfig;
	strConfig.Format( ".\\Config\\%s.ini", Play[0].UI.ID );
	CIni ini;
	ini.SetPath( strConfig );
	CString strPath = ini.GetProfileString( "SaveFolder", "Path", "C:\\Program Files\\Netmarble\\capture" );

	//	폴더가 존재하지 않으면
	{
		CUtil util;
		if( !util.IsPolderOrFileName( ( char * )strPath.operator LPCTSTR() ) )
		{
		//	CString strTemp = "C:\\Koongya";
		//	CreateDirectory( strTemp, NULL );
			CreateDirectory( strPath, NULL );
		}
	}

	char Path[MAX_PATH]={0,};
	GetCurrentDirectory(MAX_PATH,Path);
	CTime time;
	time = CTime::GetCurrentTime();

	//	저장될 파일 이름 정하기
	CString strTemp, strFilename;
//	strFilename.Format( "%s_%d.jpg", g_MyUI.ID, time );
	strFilename.Format( "capture%04d%02d%02d%02d%02d.jpg", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());//, time.GetSecond() );
	strTemp += strPath +"\\" + strFilename;

	//	85 - 권장
	Page.SaveToJpeg( ( char * )( LPCTSTR )strTemp, Quality );




	//	메세지 박스로 저장 위치 알려주기
	CString strSaveName, strSavePolder;
	strSavePolder = strTemp;
	strSaveName = strFilename;
	strSavePolder = strSavePolder.Left( strSavePolder.Find( strSaveName ) - 1 );

	strTemp.Empty();
	strTemp.Format( "[%s] 폴더에 [%s] 파일로 저장이 되었습니다", strSavePolder, strSaveName );

	static bool bProcess = false;
	if( bProcess )		return true;
	bProcess = true;
//	CMessageBox MsgBox;
//	MsgBox.DoModal( strTemp );
	bProcess = false;

	return true;
}


*/



void CGameView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	Game.OnLButtonDblclk(point.x, point.y);
	CWnd::OnLButtonDblClk(nFlags, point);
}


void CGameView::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	//lpwndpos->flags &= ~SWP_SHOWWINDOW;
}


void CGameView::PrcAIOperation()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	if(g_Where != WH_GAME) return;

	// 게임판 배경을 그림
//	if(bSetInvalidate==TRUE) {
//		Page.SetTempClip(PlayScr[0].Xp, PlayScr[0].Yp, PlayScr[0].Xp + BBOX_WIDTH*16, PlayScr[0].Yp+BBOX_HEIGHT*16);
//		Page.PutSprAuto(0, 0, &GameBackSpr, 0, TEMPCLIP);
//	}
//	else {
	Page.PutSprAuto(0, 0, m_pgamebackspr, 0);
//	}
	Page.PutFromPage(m_betpage_x, m_betpage_y, &BatPage);//////// 페이지 /////////

	// 더블 버퍼링용 메모리 DC 생성
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	MemDC.SelectObject(Page.hBmp);
	MemDC.SetBkMode(TRANSPARENT);

	// 그래픽 버튼 그리기
	ExitBtn.Draw(&MemDC);
	InviteBtn.Draw(&MemDC);
	ChangeDisplayBtn.Draw(&MemDC);
	OptionBtn.Draw(&MemDC);

//	MasterBtn.Draw(&MemDC);
	MinimizeBtn.Draw(&MemDC);
	GameItemBtn.Draw(&MemDC);
	CaptureBtn.Draw(&MemDC);

	//[62]
	if(g_bChangeCardStage && !g_MyObserver && Play[0].PlayState == 1 && Play[0].JoinState == 1 
		&& Game.m_nchange_card > 1000 / int(1000.0f/REFRESH_FPS)){
		ChangeCard_OK.Draw(&MemDC);
		ChangeCard_Cancel.Draw(&MemDC);
	}

	// ### [ 관전기능 ] ###
	//sInOut_Btn.Draw(&MemDC);
	//ObserverViewBtn.Draw(&MemDC);
	

	// 게임 진행관련 버튼
	DieBtn.Draw(&MemDC);
	CheckBtn.Draw(&MemDC);
	CallBtn.Draw(&MemDC);
	PingBtn.Draw(&MemDC);
	DadangkBtn.Draw(&MemDC);
	MaxBtn.Draw(&MemDC);
	if(m_bAllin)AllInBtn.Draw(&MemDC);//올인

	if(GameStartBtn.GetButtonState() > -1)
	{
		Page.PutSprAuto(306, 296, &EtcBtnSpr, 19,10,10);
	//	Page.PutSprAuto(316, 305, &EtcBtnSpr, 19,10,10);
		GameStartBtn.Draw(&MemDC); //시작버튼		
	}

//	MasterBtn.Draw(&MemDC);
//	BanishvoteBtn.Draw(&MemDC);
//	SortBtn.Draw(&MemDC);
	// 정보창 그리기
//	InfoBox.Draw(&MemDC);
		// 채팅박스 그리기
	ChatBox.Draw(&MemDC);

	// 캐릭터 박스 그리기
	CharBox[0].Draw(&MemDC);
	CharBox[1].Draw(&MemDC);
	CharBox[2].Draw(&MemDC);
	CharBox[3].Draw(&MemDC);
	CharBox[4].Draw(&MemDC);
	CharBox[5].Draw(&MemDC);
	
	

//################################################### 
// 임시
	// 플레이 스크린 그리기
	
	PlayScr[3].Draw(&MemDC);
	PlayScr[0].Draw(&MemDC);
	PlayScr[1].Draw(&MemDC);
	PlayScr[2].Draw(&MemDC);
	PlayScr[4].Draw(&MemDC);
	PlayScr[5].Draw(&MemDC);
	PlayScr[0].Draw(&MemDC);
	

#ifdef _DEBUG
	// 프레임 레이트 체크
	static float FPS = 0;
	static int framecnt = 0;
	static DWORD lasttick = timeGetTime();
	DWORD nowtick = timeGetTime();
	DWORD deltatick = nowtick - lasttick;
	framecnt++;
	if(deltatick >= 1000) {
		lasttick = nowtick;
		float secrate = (deltatick / 1000.0f) - 1.0f;
		FPS = (float)framecnt - (float)framecnt*secrate;
		framecnt = 0;
	}

	// 마우스 좌표 체크
	POINT mpos;
	GetCursorPos(&mpos);
	ScreenToClient(&mpos);
	CString str;
	str.Format("%5.1f FPS, Mouse (%d, %d)", FPS, mpos.x, mpos.y);

	MemDC.SelectObject(&Font2);
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(255,255,255));
	if(g_RI.bPlayer==0)
		MemDC.TextOut(530,300, str);		// change position of MouseInfo - jeong
		//MemDC.TextOut(327,72, str);
	else
		MemDC.TextOut(20,65, str);
/*
	//카드 값 표시
	for(int i=0;i<g_Max_Player;i++)
	{
		if(!g_MyObserver && Play[i].PlayState == 1)
		{
			str.Format("carddeck data       %s: %d  %d  %d  %d",Play[i].UI.ID,Game.CardDeck[i].GetCardNo(2),Game.CardDeck[i].GetCardNo(3),Game.CardDeck[i].GetCardNo(4),Game.CardDeck[i].GetCardNo(5));
			MemDC.TextOut(180,85+(i*30), str);
			int sp = Play[i].ServPNum;			
			str.Format("servercarddeck data %s: %d  %d  %d  %d (2cut)%d",Play[i].UI.ID,g_Poker.PS[sp].nCard[0],g_Poker.PS[sp].nCard[1],g_Poker.PS[sp].nCard[2],g_Poker.PS[sp].nCard[3],g_Poker.BC[sp].nCard[2]);
			MemDC.TextOut(180,100+(i*30), str);			
		}
	}
*/
#endif

	// ### [ 관전기능 ] ### 
	CString stro;
	if(g_RI.bPlayer==0) {

		//관전자 이미지그리기
		int cnt=0;
		for(int i=g_Max_Player; i<g_Max_NewPlayer-1; i++) {
			if(strlen(Play[i].UI.ID)==0) continue;
			Page.PutSprAuto(61+cnt*12, 37, g_pGameView->m_pgamebackspr, 7 + Play[i].UI.Sex );
			cnt++;
		}

		MemDC.SetTextColor( RGB(255, 255, 255) );
		MemDC.TextOut( 110, 8, m_titlestr );
		
	//	if(g_RI.bSuperMaster) {
	//		// 슈퍼방이면 슈퍼방임을 보여줌
	//		Page.PutSprAuto(540, 40, g_pGameView->m_pgamebackspr, 11+(Game.GameCnt/8)%5);
	//	}
		
		// ### [ 관전기능 ] ### 
		if(g_MyObserver) {
			//	if((Game.GameCnt/25)%2==0)
			Page.PutSprAuto(270, 80, g_pGameView->m_pgamebackspr, 5+(Game.GameCnt/25)%2);
		}
	} else {
		//관전자 이미지그리기
		int cnt=0;
		for(int i=g_Max_Player; i<g_Max_NewPlayer-1; i++) {
			if(strlen(Play[i].UI.ID)==0) continue;
			Page.PutSprAuto(60+cnt*12, 43, g_pGameView->m_pgamebackspr, 7 + Play[i].UI.Sex );
			cnt++;
		}

		MemDC.SetTextColor( RGB(255, 255, 255) );
		MemDC.TextOut( 122, 8, m_titlestr );
		
		//if(g_RI.bSuperMaster) {
			// 슈퍼방이면 슈퍼방임을 보여줌
		//	Page.PutSprAuto(540, 40, g_pGameView->m_pgamebackspr, 11+(Game.GameCnt/8)%5);
		//}
		
		// ### [ 관전기능 ] ### 
		if(g_MyObserver) {
			//	if((Game.GameCnt/25)%2==0)
			Page.PutSprAuto(274, 194, g_pGameView->m_pgamebackspr, 5+(Game.GameCnt/25)%2);
		}
	}


	//MemDC.SetTextColor( RGB(255, 255, 255) );
	//MemDC.TextOut( 178, 8, m_titlestr );
	// 버튼 튤팁 보여주기
	//MemDC.SetTextColor( RGB(225,250,255) );
	//MemDC.TextOut( TipX, TipY, m_BtnMsgTip );
	
	CRect rect;
	GetClientRect(&rect);

	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);
	// 부분 갱신 플래그 해제
//	bSetInvalidate = FALSE;
	MemDC.DeleteDC();

}
