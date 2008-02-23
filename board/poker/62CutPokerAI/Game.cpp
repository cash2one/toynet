// ㅋcpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "Game.h"
#include "Global.h"

#include "SelCardDlg.h"
#include "SelCardChoiceDlg.h"

//#include "SelectWinCase.h"
#include "ResultHi.h"
//#include "ResultHiLow.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CAST_CHIP_OFFSET_XP 10
#define CAST_CHIP_OFFSET_YP 5

#pragma comment(lib,"winmm.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGame::CGame()
{
	pPage = NULL;

	No1PosPNum = 0;
	MyPosPNum = 0;

	// 타이머 카운트 초기화
	TotalTurnCnt = TOTALTURN_MAX;
	MyTurnCnt = MYTURN_MAX;
	m_GameCount = 0;

	// 게임 정보
	bGameStart = FALSE;

	// 게임 도움말 메세지 카운터 초기화
	HelpMsgCnt = 0;
	
	BackTime = 0;
	CurTime = 0;
	m_JackPotActive = 0;	
	m_nHinddenArrow = 0;
	m_bHiddenCardShow = FALSE;

	bViewTab   = FALSE;
	nChatRuleMessage = 0;
}

CGame::~CGame()
{
}

void CGame::Init(CPage *ppage, CPage *pbatpage)
{
	pPage = ppage;

	// 초기 한번 셋
	for(int i=0; i<MAX_PLAYER; i++) {
		CardDeck[i].Init(pPage, i); // 카드덱
	}
	for( i=0; i<MAX_CHIP; i++) 
	{
		m_Chip[i].Init(ppage, pbatpage);
		m_ChipEx[i].Init(ppage, pbatpage);
	}
}

// 게임 오버시 
void CGame::ResetGame()
{
	g_RI.State = 0;

	bGameStart = FALSE;
	bHaveToResetGame = FALSE;
	ResetGameCnt = 0;
	bCantStartGame = FALSE;
	GameCnt = 0;
	g_bChangeCardStage = FALSE;

	// #### [ 세븐 포커 ] ####
	bOpen = FALSE;
	WinnerPNum = -1;
	LowWinnerPNum = -1;

	CurPlayer = -1;
	FlyWindCard = FALSE;


	m_nHinddenArrow = 0;
	m_bHiddenCardShow = FALSE;

	bBtnClick = FALSE;

	//////////////////
	// 타이머
	BackTime = 0;
	CurTime = 0;

	// 상태
	m_GameState = 0;

	// 게임 도움말 메세지 카운터 초기화
	HelpMsgCnt = 0;
//	DumyCardCnt = 52;
	// 유리 메세지 카운터 초기화
//	GlassMsgCnt = 0;


	
	// ### [ 관전기능 ] ###
	for(int i=0; i<g_Max_NewPlayer; i++) {// 플레이어 리셋
		Play[i].Reset();
		if(i < g_Max_Player)
		{
			PlayScr[i].Reset();
			// 카드덱 초기화
			CardDeck[i].Reset();
		}
	}

	// [ 카드 이벤트 에러시 ]
	bWindCardEnd	= FALSE;
	nWindCardEndCnt = 0;
	StartUserNum	= 0;

	m_discardindex = 0;

	// ###  [ 칩 ] ###
	// 배팅 칩 게임판을 지운다
	g_pGameView->BatPage.PutSprAuto(0, 0, g_pGameView->m_pgamebackspr, 1);
	m_JackPotActive = 0;
	for(i=0; i<MAX_CHIP; i++) 
	{
		m_Chip[i].Reset();
		m_ChipEx[i].Reset();
	}

	ZeroMemory(m_jackpot_winnerid, sizeof(m_jackpot_winnerid));
	ZeroMemory(&g_Poker, sizeof(g_Poker));
	ZeroMemory(m_CardHi,sizeof(m_CardHi));
	ZeroMemory(m_CardLow,sizeof(m_CardLow));
	ZeroMemory(m_CardSwing,sizeof(m_CardSwing));
	memset( ClockFlag, FALSE, sizeof( BOOL ) * 16 );

	// ### [ 관전기능 ] ###
	g_MyInfo.JoinState = 0;
	g_MyInfo.PlayState = 0;

	//  카드 선택
	m_bchoice_card		= FALSE;
	m_nchoice_cardcnt  = 0;
	m_ncur_selectcard = 0;
	m_nchange_card = 0; //카드 바꾸기 카운트 

	m_hpos = NULL;

	for(i=0; i<MAX_CHIPCNT; i++) m_nStackY[i]=MAX_STACK_OFFSETY;
	
	// 시작버튼 활성화 
//	if(strcmp(g_RI.ID,g_MyInfo.UI.ID)==0 && g_RI.UNum == g_MyInfo.UI.UNum 
//		&& g_RI.NowUserNum < 5 && g_RI.NowUserNum >2 && g_RI.State == 0){		
//		g_pGameView->GameStartBtn.Show(TRUE);		
//	}

	g_pGameView->SetBtnState();

	//AI 돈머니 올림 - jeong
	CSV_ASK_MONEYINFO aumsg;
	aumsg.Set(Play[0].UI.UNum, 100000, g_RI.RoomNum);
	SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());

}

// ### [ Frame++ ] ###
void CGame::OnTimer()
{
	GameCnt++;

	// [ 카드 날리기 ]
	if(g_RI.State == 1)
	{
		if( bWindCardEnd )
		{
			nWindCardEndCnt--;
			if(nWindCardEndCnt <= 0)
			{
				bWindCardEnd = FALSE;
				// 서버로 메세지 보낸다.
				// ### [ 관전기능 ] ###
				if(!g_MyObserver && !Play[0].bFold && Play[0].JoinState == 1 && Play[0].PlayState == 1)
				{
					CCL_WINDCARDEND msg2;
					//msg2.Set(g_RI.RoomNum, Play[0].ServPNum, Play[0].UI.UNum, Play[0].UI.ID);//int rnum, int pnum, int unum, char *id)
					msg2.Set(g_RI.RoomNum, g_MyInfo.ServPNum, g_MyInfo.UI.UNum, g_MyInfo.UI.ID);//int rnum, int pnum, int unum, char *id)
					SockMan.SendData(g_MainSrvSID, msg2.pData, msg2.GetTotalSize());
				}
			}
		}

		// 게임시작 카드선택
		if( m_bchoice_card ) 
		{
			m_nchoice_cardcnt--;
			if(m_nchoice_cardcnt <= 0) {
				m_bchoice_card = FALSE;
				m_nchoice_cardcnt = 0;
				g_pGameView->OnBtnFold();
			}
		}

		//[62] 카드 바꾸기 타임 
		if(m_nchange_card > 0){
			m_nchange_card--;

			if(m_nchange_card == 1000 / int(1000.0f/REFRESH_FPS)){

				Play[0].bChangeWhether = 0;
				g_pGameView->ChangeCard_OK.Enable(FALSE);
				g_pGameView->ChangeCard_Cancel.Enable(FALSE);
			}

			if(m_nchange_card <= 0){
				m_nchange_card = 0;				
			//	g_pGameView->OnBtnFold();
				SetChangeCard_Cancel_62Cut(); // 패스로 처리
				g_bChangeCardStage = FALSE;
			}
		}
	}

	for(int i=0; i<g_Max_Player; i++) {			
		CardDeck[i].OnTimer();
	}

	for(i=0; i<MAX_CHIP; i++) {
		m_Chip[i].OnTimer();
		m_ChipEx[i].OnTimer();			//Add ChipEx - jeong
	}
}

void CGame::SetGlassMsg(int cnt, WORD color, CString &msg)
{/*
	GlassMsgCnt = cnt;
	GlassMsgColor = color;
	GlassMsgGrade = 0;
	szGlassMsg = msg;
*/
}

void CGame::SetHelpMsg(int cnt, int kind, CString &msg)
{
	/*
	HelpMsgCnt = cnt;
	HelpMsgKind = kind;
	szHelpMsg = msg;
	*/
}

/*
// 테트리스
int CGame::GetPNum_ByServPN(int onum)
{
	//if(onum == MyPosPNum) return 0
	if(onum < MyPosPNum) return onum+1;
	if(onum > MyPosPNum) return onum;
	return 0; // (onum == MyPosPNum) 
}

int CGame::GetServPNum_ByPN(int onum)
{
	if(onum==0) return MyPosPNum;

	if(onum > MyPosPNum) return onum;
	return onum-1; // (onum <= MyPosPNum)
}
*/

// 훌라, 포커
int CGame::GetPNum_ByServPN(int onum) // 서버인덱스 ->  클라이언트 인덱스
{
	return (abs(onum-MyPosPNum+g_Max_Player))%g_Max_Player;
}

int CGame::GetServPNum_ByPN(int onum) // 클라이언트 -> 서버인덱스 나를 0으로
{
	return (MyPosPNum+onum)%g_Max_Player;
}

// ### [ 관전기능 ] ###
int CGame::GetPlayerPNum(char *id)
{
	int pnum = -1;
	if(strlen(id)==0) return pnum;
	for(int i=0; i<g_Max_NewPlayer; i++) {
		if(strlen(Play[i].UI.ID) > 0)
			if(strcmp(Play[i].UI.ID, id) == 0) {pnum=i; break;}
	}
	return pnum;
}

void CGame::PlayMusic()
{
	
//	if(g_RI.GameKind == 0) Sound.PlayMusic(MUSIC01);
//	else Sound.PlayMusic(MUSIC02);
	Sound.StopMusic();
	if(g_Where == WH_LOBY)
		Sound.PlayOggMusic(MUSIC_OGG01);
	else if(g_Where == WH_GAME)
		Sound.PlayOggMusic(MUSIC_OGG02);
}


void CGame::OnGameKey(int nVKey)
{
	PlayScr[0].OnGameKey(nVKey);
}

BOOL CGame::OnLButtonDown(int x, int y)
{
	//if(g_RI.State == 1 && PlayScr[0].bPractiseMode == FALSE) return FALSE;
	/*
	if(PlayScr[0].OnLButtonDown(x, y)) { PlayScr[0].OnGameKey(Play[0].ServPNum+'1'); return TRUE; }
	if(PlayScr[1].OnLButtonDown(x, y)) { PlayScr[0].OnGameKey(Play[1].ServPNum+'1'); return TRUE; }
	if(PlayScr[2].OnLButtonDown(x, y)) { PlayScr[0].OnGameKey(Play[2].ServPNum+'1'); return TRUE; }
	if(PlayScr[3].OnLButtonDown(x, y)) { PlayScr[0].OnGameKey(Play[3].ServPNum+'1'); return TRUE; }
	if(PlayScr[4].OnLButtonDown(x, y)) { PlayScr[0].OnGameKey(Play[4].ServPNum+'1'); return TRUE; }
	if(PlayScr[5].OnLButtonDown(x, y)) { PlayScr[0].OnGameKey(Play[5].ServPNum+'1'); return TRUE; }
	*/
	if(PlayScr[0].OnLButtonDown(x, y)) { return TRUE; }
	if(PlayScr[1].OnLButtonDown(x, y)) { return TRUE; }
	if(PlayScr[2].OnLButtonDown(x, y)) { return TRUE; }
	if(PlayScr[3].OnLButtonDown(x, y)) { return TRUE; }
	if(PlayScr[4].OnLButtonDown(x, y)) { return TRUE; }
	if(PlayScr[5].OnLButtonDown(x, y)) { return TRUE; }

	g_CvCard.OnLButtonDown(x, y);
	
	return FALSE;
}

BOOL CGame::OnLButtonUp(int x, int y)
{
	if(PlayScr[0].OnLButtonUp(x, y) ) return TRUE;
	g_CvCard.OnLButtonUp(x, y);
	return FALSE;
}

BOOL CGame::OnMouseMove(int x, int y)
{
	if(PlayScr[0].OnMouseMove(x, y) ) return TRUE;
	g_CvCard.OnLButtonMove(x, y);
	return FALSE;
}

BOOL CGame::OnRButtonDown(int xp,int yp)
{
	g_BgMode++;
	if(g_BgMode > 2)g_BgMode = 0;

	//g_pMainDlg->Reload_spritefile(); softpark
	//g_pGameView->m_pgamebackspr = &GameBackSpr;
	//g_pGameView->BatPage.PutSprAuto(0, 0, g_pGameView->m_pgamebackspr, 1);			// 배팅 칩 게임판을 지운다
	
	return TRUE;
}

BOOL CGame::OnLButtonDblclk(int x, int y)
{
		// 카드 바꾸기 일때 [62]
	if(!g_MyObserver &&  g_bChangeCardStage == TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0)
	{
		if(Game.CardDeck[0].PtInCardRect(x,y,1))
		{	
			g_pGameView->SendMessage(WM_COMMAND,IDM_CHANGE_OK, 0);
			return TRUE;
		}
	}
	return FALSE;
}

// ### [ 관전기능 ] ###
void CGame::DoPrepareGame(STARTINFO *pSC)
{
	if(pSC == NULL) return;
	Game.ResetGame();
	g_pGameDlg->ClearResetGameTimer();
	g_RI.State = 1;

	g_pGameView->GameStartBtn.Show(FALSE);

	// 서버, 클라이언트 각각 선언하여 별도로 사용한다.
	ZeroMemory(&g_Poker, sizeof(g_Poker));
	memcpy(g_Poker.PS, pSC->Ps, sizeof(PLAYSTATE)*g_Max_Player);
	// 게임 시작 플래그 세팅
	bGameStart = TRUE;
	BOOL bFalse = FALSE;
	for(int i=0; i<g_Max_Player; i++) {		
		// 플레이어 스크린 초기화
		PlayScr[i].Reset();
		// 카드덱 초기화
		CardDeck[i].Reset(); // pn 값 일치 않함
	}
	StartUserNum = 0;
	int BetMoney = g_RI.BetMoney;
	for( i=0; i<g_Max_Player; i++) 
	{		
		if(strlen(pSC->JoinID[i]) >0)
		{
			int pn = GetPNum_ByServPN(i);
			if(strcmp(Play[pn].UI.ID, pSC->JoinID[i]) != 0) { bFalse = TRUE; break; }
			
			// 플레이어 상태 세팅
			Play[pn].JoinState = pSC->Ps[i].JoinState; //
			Play[pn].PlayState = pSC->Ps[i].PlayState;
			Play[pn].bFold     = FALSE;
			Play[pn].UI.PMoney -=  BetMoney;//100;
			g_Poker.RU[i].nPMoney = Play[pn].UI.PMoney;
			Play[pn].nSndFxKind = pSC->Ps[i].nSndFxKind;
			
			// 유저 카드 셋팅 3장
			if( pn == 0)
			{
				// ### [ 관전기능 ] ###
				if( !g_MyObserver )
				{
					g_MyInfo.JoinState = Play[0].JoinState;
					g_MyInfo.PlayState = Play[0].PlayState;
					g_MyInfo.bFold = Play[0].bFold;
					g_MyInfo.ServPNum = Play[0].ServPNum;
				}
				
				
				CardDeck[0].NewCardSet( pSC->nCard[0]);
				CardDeck[0].NewCardSet( pSC->nCard[1]);
				CardDeck[0].NewCardSet( pSC->nCard[2]);
				// 추가
				if(g_RI.FormKind == 1)
					CardDeck[0].NewCardSet( pSC->nCard[3]);
			}
			else
			{
				CardDeck[pn].NewCardSet( DUMY_CARD); // 53
				CardDeck[pn].NewCardSet( DUMY_CARD);
				CardDeck[pn].NewCardSet( DUMY_CARD);
				// 추가
				if(g_RI.FormKind == 1)
					CardDeck[pn].NewCardSet( DUMY_CARD);
			}

			Play[pn].nCardTotal = 3;
			Play[pn].nOpenTotal = 0; 
			g_Poker.RU[i].bInUse = TRUE;
			g_Poker.RU[i].nRealBat = BetMoney;

			// 기본 배팅 칩을 던짐
			CastChipEx(pn, BetMoney, 1); // 여기서의 10은 단위다!!
			StartUserNum++;
		}
	}

	Sound.Play(SND25);

	if(bFalse == TRUE) {
		// 강퇴 처리
		CString st[MAX_PLAYER], str;
		for(int i=0; i<g_Max_Player; i++) 
		{
			int pn = GetPNum_ByServPN(i);
			st[i].Format("%d: %s, %s\n",i, Play[pn].UI.ID, pSC->JoinID[i]);
			str += st[i];
		}
		g_pGameDlg->ExitGame();
		AfxMessageBox("잘못된 게임 시작정보로 인하여 퇴장합니다");
		AfxMessageBox(str);
		return;
	}

	g_Poker.nState = RSTATE_PREPARESTART; // 서버쪽 셋팅이나
	g_Poker.nDistUser = pSC->nDistUser;
	CurPlayer = pSC->nDistUser;

	// AI 돈 무한	- jeong
	Play[0].UI.PMoney = 100000000000000000;
	g_MyInfo.UI.PMoney  =  Play[0].UI.PMoney; 
	
	// 카드 날리기 => 카드 다날림 서버로
	Card_Distribute();
}



void CGame::OnGameOver(GAMEOVERRESULT *pGOR)
{
	g_Wait = FALSE;
	g_RI.State = 0;
	bGameStart = FALSE;				// 게임 시작 플래그 세팅
	bHaveToResetGame = TRUE;		// 게임 초기화 플래그 및 카운터 세팅
	ResetGameCnt = 0;

	PlayScr[0].bGameOver = TRUE;	// 게임 종료 상태
//	Sound.AllMusicStop();
	g_pGameView->SetBtnState();		// 버튼 초기화

	bOpen = pGOR->bOpen;
	WinnerPNum= -1;
	LowWinnerPNum = -1;
	memcpy(&g_Poker,&pGOR->Pg,sizeof(g_Poker));

	int bJackpot = pGOR->bJackPot;

	g_TmpJackPotMoney = pGOR->JackPotMoney;

	for(int i=0; i<g_Max_Player; i++)
	{
		if(strlen(pGOR->Ui[i].ID)>0) //  겜참여자.
		{
			int pnum = GetPlayerPNum(pGOR->Ui[i].ID);
			if(pnum == -1) continue;
			Play[pnum].UI.WinNum = pGOR->Ui[i].WinNum;
			Play[pnum].UI.LooseNum = pGOR->Ui[i].LooseNum;
			Play[pnum].UI.DrawNum = pGOR->Ui[i].DrawNum;
			Play[pnum].UI.PMoney = pGOR->Ui[i].PMoney;
			if(	Play[pnum].UI.PMoney < 0) 	Play[pnum].UI.PMoney = 0;
			// 레벨상점 폐쇄
			Play[pnum].UI.nIcon = GetPokerLevel(Play[pnum].UI.PMoney);
			memcpy(&Play[pnum].UI.PI, &pGOR->Ui[i].PI, sizeof(POKERINFO));
			PlayScr[pnum].bGameOver = TRUE;
			
			// 히든 카드 오픈
			if( bOpen && !pGOR->Pg.PS[i].bFold )
			{
				if(pnum == 0) {
				
					if(g_Poker.BC[i].nCard[0] >=0)
						CardDeck[0].Card[0].CardNo = g_Poker.BC[i].nCard[0]; // 값 변경
					if(g_Poker.BC[i].nCard[1] >=0)
						CardDeck[0].Card[1].CardNo = g_Poker.BC[i].nCard[1]; // 값 변경

					/* softpark  62 poker
					if(g_Poker.BC[i].nCard[2] >= 0 && g_Poker.BC[i].nCard[2] < 52)
						CardDeck[0].Card[5].CardNo = g_Poker.BC[i].nCard[2]; // 값 변경[62]
					*/
					if(g_Poker.BC[i].nCard[2] >= 0 && g_Poker.BC[i].nCard[2] < 52)
						CardDeck[0].Card[6].CardNo = g_Poker.BC[i].nCard[2]; // 값 변경[62]
			
					CardDeck[0].SetFaceUp(0);
					CardDeck[0].SetFaceUp(1);
					//CardDeck[0].SetFaceUp(5);//62 poker
					CardDeck[0].SetFaceUp(6);
				}
				else {
					if(g_Poker.BC[i].nCard[0] >=0)
						CardDeck[pnum].Card[0].CardNo = g_Poker.BC[i].nCard[0]; // 값 변경
					if(g_Poker.BC[i].nCard[1] >=0)
						CardDeck[pnum].Card[1].CardNo = g_Poker.BC[i].nCard[1]; // 값 변경

				//	if(Play[pnum].bChangeWhether == 1)CardDeck[pnum].Card[5].CardNo = g_Poker.BC[i].nCard[2]; // 값 변경[62]
					if(g_Poker.BC[i].nCard[2] >= 0 && g_Poker.BC[i].nCard[2] < 52)
						CardDeck[pnum].Card[6/*5*/].CardNo = g_Poker.BC[i].nCard[2]; // 값 변경[62]

					CardDeck[pnum].SetFaceUp(0);
					CardDeck[pnum].SetFaceUp(1);
					CardDeck[pnum].SetFaceUp(6/*5*/);
				}
			}
			else if( !bOpen && bJackpot==1 && !pGOR->Pg.PS[i].bFold) 
			{
				if(g_Poker.BC[i].nCard[0] >=0)
					CardDeck[pnum].Card[0].CardNo = g_Poker.BC[i].nCard[0]; // 값 변경
				if(g_Poker.BC[i].nCard[1] >=0)
					CardDeck[pnum].Card[1].CardNo = g_Poker.BC[i].nCard[1]; // 값 변경
				
				if(g_Poker.BC[i].nCard[2] >= 0 && g_Poker.BC[i].nCard[2] < 52)
					CardDeck[pnum].Card[6/*5*/].CardNo = g_Poker.BC[i].nCard[2]; // 값 변경[62]

				CardDeck[pnum].SetFaceUp(0);
				CardDeck[pnum].SetFaceUp(1);
				CardDeck[pnum].SetFaceUp(6/*5*/);
			}

			// ## 파산 ##		// ### [ 관전기능 ] ###
			if(!g_MyObserver && pnum == 0 && Play[pnum].UI.PMoney <= 0) {
				Play[pnum].UI.PMoney = 0;
				Sound.Play( SND30 );
			}		
		}
	}

	// ### [ 관전기능 ] ###
	if( g_MyObserver != TRUE ) //관전이 아니면
	{
		g_MyInfo.UI.WinNum   = Play[0].UI.WinNum; 
		g_MyInfo.UI.LooseNum = Play[0].UI.LooseNum; 
		g_MyInfo.UI.DrawNum =  Play[0].UI.DrawNum;
		g_MyInfo.UI.PMoney  =  Play[0].UI.PMoney; 
		g_MyInfo.UI.nIcon   =  Play[0].UI.nIcon;
	}
	
	CString str="";
	CString strM="";
	
	if( !bOpen ) // 승자 1명 (나머지 유저 폴드)
	{
		WinnerPNum = pGOR->nNowWinner;
		LowWinnerPNum =-1;
		CurPlayer = WinnerPNum; // 하이 승자를 선으로 한다.
		g_Poker.CurPlayer = CurPlayer;
		
		str.Format(g_StrMan.Get(_T("WIN_MESINFO")), pGOR->ID);		
		g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(247,200,80));
		
		INT64 nTotBat = Game.GetRealBet();
		strM = NumberToOrientalString(nTotBat);					// 베팅액 총 ## 총베팅액
		str.Format(g_StrMan.Get(_T("BET_TOTAL")),strM);		
		g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(247,200,80));
		
		strM = NumberToOrientalString(pGOR->nRealBat); // 하이 머니 
		str.Format(g_StrMan.Get(_T("BET_WIN")), strM); // 하이 승자 아디 ## 하이승:%s(+%s)		
		g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(247,200,80));				
		
		strM = NumberToOrientalString(pGOR->nGory);		
		str.Format(g_StrMan.Get(_T("BET_GORI")),pGOR->nPercent,strM);
		g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(170, 100, 0));		
		
		int pnum = GetPlayerPNum(pGOR->ID);
		if(pnum != -1) {
			// 축하
			// ### [ 관전기능 ] ###
			if( !g_MyObserver )
			{
				if(pnum == 0)
					Sound.Play( SND32 );
				else if(Play[0].JoinState == 1)
					Sound.Play( SND33 );
			}
			
			CResultHi Dlg;
			if( Dlg.Init(&Play[pnum].UI, pGOR, 0, pnum) )
				Dlg.DoModal();
		}

		//카드 렉트 초기화[62]
		CardDeck[0].ReSetCardRect();
	}
	else // (하이:1 로우:2 스윙:3  승자 1명 ) 승자가 2명: 4 (하이로  이겼는가 로우로 이겼는가 승자가 2명인가)
	{
		
		m_GameState = 7; // 게임오버
		
		WinnerPNum = pGOR->nNowWinner;
		LowWinnerPNum = -1;
		CurPlayer = WinnerPNum; // 하이 승자를 선으로 한다.
		g_Poker.CurPlayer = CurPlayer;
			
		str.Format(g_StrMan.Get(_T("WIN_MESINFO")), pGOR->ID);
		//g_pGameDlg->m_GameView.InfoBox.ChatViewEdit.AddText(&str,RGB(247,200,80));
		g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(247,200,80));
		
		INT64 nTotBat = Game.GetRealBet();
		strM = NumberToOrientalString(nTotBat);//pGOR->Pg.nTotBat);					// 베팅액 총 ## 총베팅액
		str.Format(g_StrMan.Get(_T("BET_TOTAL")),strM);
		//		g_pGameDlg->m_GameView.InfoBox.ChatViewEdit.AddText(&str,RGB(247,200,80));
		g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(247,200,80));
		
		strM = NumberToOrientalString(pGOR->nRealBat); // 하이 머니 
		str.Format(g_StrMan.Get(_T("BET_WIN")), strM); // 하이 승자 아디 ## 하이승:%s(+%s)
		//g_pGameDlg->m_GameView.InfoBox.ChatViewEdit.AddText(&str,RGB(247,200,80));
		g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(247,200,80));
		
		strM = NumberToOrientalString(pGOR->nGory);
		//	str.Format("[%s]님 딜러비(1%%) : -%s원\r\n",strM);
		str.Format(g_StrMan.Get(_T("BET_GORI")),pGOR->nPercent,strM);
		g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(170, 100, 0));
		
		
		strncpy(m_jackpot_winnerid, pGOR->ID, 15);
		
		int pnum = GetPlayerPNum(pGOR->ID);
		if(pnum != -1) {
			
			// 축하
			// ### [ 관전기능 ] ###
			if( !g_MyObserver  )
			{
				if(pnum == 0)
					Sound.Play( SND32 );
				else if(Play[0].JoinState == 1)
					Sound.Play( SND33 );
			}
			
			CResultHi Dlg;
			if( Dlg.Init(&Play[pnum].UI, pGOR, 1, pnum) ) 
				Dlg.DoModal();
		}

		//카드 렉트 초기화[62]
		CardDeck[0].ReSetCardRect();
	}
}

void CGame::DoExitSubscript(int bexit)
{
	// ### [ 관전기능 ] ###
	CCL_EXITSUBSCRIPT esmsg;
	//esmsg.Set(g_RI.RoomNum, Play[0].UI.UNum, Play[0].UI.ID, bexit);
	esmsg.Set(g_RI.RoomNum, g_MyInfo.UI.UNum, g_MyInfo.UI.ID, bexit);

	// 메인서버에게 나가기 예약 상태를 알림
	SockMan.SendData(g_MainSrvSID, esmsg.pData, esmsg.GetTotalSize());
}

// ### [ Frame++ ] ### *3
void CGame::Card_Distribute()
{
	int delay=0;// 지연
	
	POINT pt;  // 스타트
	POINT pt1; // 스타트 위치
	pt1.x = 200;
	pt1.y = 240;	// 카드 날리는 위치 변경 - jeong
	//pt1.y = 40;
	FlyWindCard = TRUE;		// 카드 날리는 중
	CCard* pLastCard = NULL;// 마지막 카드

	int pnum = GetPNum_ByServPN( g_Poker.nDistUser ); // 카드 날리는 순서
	if(pnum < 0 || pnum >= g_Max_Player) return;

	if(Play[pnum].nCardTotal == 3) // 1번째 장 줄때 지연 +
		delay = 13*1;//13*3;
	else if(Play[pnum].nCardTotal == 4) // 4번째 장 줄때 지연 +
		delay = 12*1;//12*3;
	else
		delay = 12*1;//12*3;

		
	// 처음 3장
	if(g_Poker.nState == RSTATE_PREPARESTART )
	{	
		int nCardNum = 3; // 카드 장수
		int delaytime = 4*3;
	
		if(g_RI.FormKind == 1) {
			nCardNum = 4; // 초이스 카드
			delaytime = 3*3;
		}

		for( int j = 0; j < nCardNum; j++)	{
			for(int i = pnum; i<g_Max_Player + pnum; i++) {
				int index = i;
				if(index >= g_Max_Player) index = index-g_Max_Player;
				if(strlen(Play[index].UI.ID)>0 && Play[index].JoinState == 1 && Play[index].PlayState == 1)	
				{
					CCard* pCard = &CardDeck[index].Card[j];
					if(pCard != NULL) 
					{
						pCard->m_move.SetCurPos(pt1);
						pt.x = pCard->Xp;
						pt.y = pCard->Yp;
						pCard->StraightMoveTo(pt, 10*3, 18*3, delay);
						delay += delaytime;
						pLastCard = pCard;
					}
				}
			}
			// 인원수에 따라 차등
			delay +=2*3;
		}

		if(pLastCard != NULL)	
		{
			pLastCard->SetEvent(EVENT_THREEFLYEND); // 3장 마지막 까지 날렸으면 서버로
			SetWindCardEnd(TRUE);
		}
	}
	else if(g_Poker.bDistCard)
	{
		int j = 0;
		if(g_RI.FormKind == 0)
			j = Play[pnum].nCardTotal-1; // 선의 카드 수를  따른다. 
		else 
			j = Play[pnum].nCardTotal-1; // 선의 카드 수를  따른다. 초이스

		for(int i=pnum;  i < g_Max_Player + pnum; i++)  {
			int index = i;
			if(index >= g_Max_Player) index = index - g_Max_Player;
			if(strlen(Play[index].UI.ID)>0 && Play[index].JoinState == 1 && Play[index].PlayState == 1 )
			{//&& j<SELECT_TOTALCARD) {//softpark j<SELECT_TOTALCARD 추가 : overflow 방 지. 현재 미 추가시. seven포카로 변경해서 그런지. 7 이 나오면서 overflow 발생
				CCard* pCard = &CardDeck[index].Card[j];
				if(pCard != NULL)  
				{
					pCard->m_move.SetCurPos(pt1);
					pt.x = pCard->Xp;
					pt.y = pCard->Yp;
					pCard->StraightMoveTo(pt, 10*3, 18*3, delay);
					delay += 3*3;
					// 한장씩 도착하면 보여준다
					pCard->SetEvent(EVENT_ONEFLYEND, index, j ); // 플레이 넘버,인덱스 값
					pLastCard = pCard;
				}
			}
		}
	
		if(pLastCard != NULL)
		{
			pLastCard->SetEvent1(EVENT_ONEFLYTOTEND); // 모두 도착하면 서버로
			SetWindCardEnd();
		}
	}



}

void CGame::Raise(POKERGAME *pMsg)
{
//	m_bTurn = FALSE;
	if(pMsg == NULL) return;

	memcpy(&g_Poker,(char*)pMsg, sizeof(g_Poker));	

	// ### [ 사용자머니 변경 ] ### 
	for(int i=0; i<g_Max_Player; i++)
	{
		int pn = GetPNum_ByServPN(i);
		if(strlen(Play[pn].UI.ID)>0 && Play[pn].JoinState == 1 && Play[pn].PlayState == 1) // 겜중이면
		{  
			Play[pn].UI.PMoney = g_Poker.RU[i].nPMoney;// 사용자 머니 셋
			if( g_Poker.PS[i].bFold ) Play[pn].bFold = TRUE;

			// 추가 
			if(Play[pn].UI.PMoney < 0) Play[pn].UI.PMoney = 0;

			// ### [ 관전기능 ] ###
			Play[pn].UI.nIcon = GetPokerLevel(Play[pn].UI.PMoney );
		}	
	}

	for(i=0; i<g_Max_NewPlayer; i++)//[62
	{
		Play[i].m_bSelectCard = 0;			
	}
	
	// ##### [ 정보 창에 메세지 && 사운드 출력] #####
	if(g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < g_Max_Player)
	{
		int LastPnum = g_Poker.LastPlayer;
		int nSNum = GetPNum_ByServPN(LastPnum);
		if( g_Poker.RU[LastPnum].nLastCmd>0 && nSNum >= 0 && nSNum < g_Max_Player) 
		{
			int nLastCmd = g_Poker.RU[LastPnum].nLastCmd;
			switch(nLastCmd)
			{
			case 1: // foldUser 에서 처리
				{
				} break; // 다이
			case 2:
				{
					SoundFxKind(nLastCmd,Play[nSNum].UI.Sex, Play[nSNum].nSndFxKind);
				}break; // 체크
			case 3: 
				{	SoundFxKind(nLastCmd,Play[nSNum].UI.Sex, Play[nSNum].nSndFxKind);
					if(g_Poker.RU[LastPnum].nReCall>0) Sound.Play(SND26);
				} break; // 콜
			case 4: 
				{	
					SoundFxKind(nLastCmd,Play[nSNum].UI.Sex, Play[nSNum].nSndFxKind);
					Sound.Play(SND25);
				}  break; // 삥
			case 5:
				{	
					SoundFxKind(nLastCmd,Play[nSNum].UI.Sex, Play[nSNum].nSndFxKind);
					Sound.Play(SND26);
				}  break; // 따당
			case 6:
				{
					SoundFxKind(nLastCmd,Play[nSNum].UI.Sex, Play[nSNum].nSndFxKind);
					Sound.Play(SND27);
				} break; // 맥스
			case 7:
				{
					SoundFxKind(nLastCmd,Play[nSNum].UI.Sex, Play[nSNum].nSndFxKind);
					Sound.Play(SND27);
					Play[nSNum].bAllIn	= TRUE;
					PlayScr[nSNum].m_AllinAniStartTime = 0;
					PlayScr[nSNum].m_AllinAniStartTime = timeGetTime();
				} break; // 올인
			}
		}
	}


	// 현재 순서가 카드 날리기 인가?
	if( g_Poker.bDistCard ) 
	{ 
		// 장수 증가 할것
		for(int i=0; i<g_Max_Player; i++) {
			int pn = GetPNum_ByServPN(i);
			if(strlen(Play[pn].UI.ID)>0 && Play[pn].JoinState == 1 && Play[pn].PlayState == 1 && !Play[pn].bFold ) // 겜중이면
			{
				int totCnt = g_Poker.PS[i].nCardTotal;
				int openCnt = g_Poker.PS[i].nOpenTotal;				
				
				if(pn == 0)	{
					if(totCnt == 4) {// 1 한번에 4장
						
						// ### [ 관전기능 ] ###
						if( g_MyObserver ) {

							// 2가지 경우 = 3장 나누어준 상태에서 방입장 카드숫자 -1 하지 않는당
							if(bGameStart && g_RI.FormKind == 1 )
							{	// 처음서 부터 관전중 
								CardDeck[pn].Card[3].Clear();
								CardDeck[pn].TotalCardNum -= 1;
							}
							CardDeck[pn].Card[2].CardNo = g_Poker.PS[i].nCard[0]; // 값 변경
							CardDeck[pn].NewCardSet( g_Poker.PS[i].nCard[1]);  // 값 등록
							for(int k = 2; k < totCnt-1; k++) {	CardDeck[pn].SetFaceUp(k); }// 앞면(보이기)
						
						}
						else
						{
							CardDeck[0].Card[2].CardNo = g_Poker.PS[i].nCard[0]; // 값 변경
							for(int k = 0; k < totCnt-1; k++) {	CardDeck[0].SetFaceUp(k);}// 앞면(보이기) 나는 모두
							/// 3번째 장 위쪽으로 그린다
							CPoint pt;
							pt.x = CardDeck[0].Card[2].Xp;
							CardDeck[0].Card[2].Yp -= 7;
							pt.y = CardDeck[0].Card[2].Yp;
							CardDeck[0].Card[2].m_move.SetCurPos(pt);
							CardDeck[0].NewCardSet( g_Poker.PS[i].nCard[1]); // 4번째 카드 등록
							// 3번째 장 오픈시 한번더
							Sound.Play( SND16 );
						}
					}
					else if(totCnt < 7) // 히든 전
					{
						if(openCnt > 0)
							CardDeck[0].NewCardSet( g_Poker.PS[i].nCard[openCnt-1]); // 4번째 카드 등록
					}
					else // hidden
					{
						// ### [ 관전기능 ] ###
						CardDeck[0].NewCardSet( DUMY_CARD);
						/*
						if(openCnt > 0) {
							// ### [ 관전기능 ] ###
							if(g_MyObserver)
								CardDeck[pn].NewCardSet( DUMY_CARD); // 4번째 카드 등록
							else
								CardDeck[0].NewCardSet( g_Poker.PS[i].nCard[openCnt-1]); // 4번째 카드 등록
						}
						*/
					}
				}
				else
				{
					if(totCnt == 4)
					{
	/*///////					// ### [ 관전기능 ] ###
						if( g_MyObserver ) 	{	// 2가지 경우 = 3장 나누어준 상태에서 방입장
							if(bGameStart && g_RI.FormKind == 1 ) {	
								// 처음서 부터 관전중
								CardDeck[pn].Card[3].Clear();
								CardDeck[pn].TotalCardNum -= 1;
							}
						}
						else {						
							if(g_RI.FormKind == 1 && Play[pn].JoinState == 1) {
								// 추가 초이스 카드
								CardDeck[pn].Card[3].Clear();
								CardDeck[pn].TotalCardNum -= 1;
							}
						}
*/
						CardDeck[pn].Card[2].CardNo = g_Poker.PS[i].nCard[0]; // 값 변경
						CardDeck[pn].NewCardSet( g_Poker.PS[i].nCard[1]);  // 값 등록
						
						
						for(int k = 2; k < totCnt-1; k++) {
							CardDeck[pn].SetFaceUp(k);
						}

					}
					else if(totCnt < 7)
					{
						if(openCnt > 0)
							CardDeck[pn].NewCardSet( g_Poker.PS[i].nCard[openCnt-1]); // 4번째 카드 등록
					}
					else if(totCnt == 7) // hidden
					{
						CardDeck[pn].NewCardSet( DUMY_CARD ); // 4번째 카드 등록
						Game.CardDeck[0].CoverCardSet(DUMY_CARD);
						g_CvCard.bAllowControl = TRUE;
					}
				}

				Play[pn].nCardTotal = totCnt; 
				Play[pn].nOpenTotal = openCnt;
			}
		}
		
		// 카드날리기~
		Card_Distribute();
		return;
	}

	// 레이즈모드이다 베팅시작  내턴이면 버튼 활성화
	if(g_Poker.nState==RSTATE_STARTGAME) 
	{
		CurPlayer = g_Poker.CurPlayer;
		
		if(CurPlayer < 0 || CurPlayer >= g_Max_Player) return;
		int pn = GetPNum_ByServPN(CurPlayer);

		if(g_Poker.LastStyle == -1 )
		{
			INT64 nRaiseBat = GetRaiseBat();
			CastChipEx(pn, nRaiseBat);//g_Poker.nRaiseBat);
			g_pGameView->SetBtnState();

// ######   지연  #######
			return;
		}
	
		if(g_Poker.LastStyle >= 2  ) // 베팅시에만
		{
			int LastPlayer = g_Poker.LastPlayer;
			if(LastPlayer < 0 || LastPlayer >= g_Max_Player) return;
			int bet = GetPNum_ByServPN(LastPlayer);
			
		
			if(g_Poker.RU[LastPlayer].nLastCmd == 3)
			{// 콜일때
				//if(g_Poker.RU[LastPlayer].nReCall > 0)
				//{
					INT64 nRaiseBat = GetRaiseBat();
					CastChipEx(bet, nRaiseBat);//g_Poker.nRaiseBat);
				//}
			}
			else
			{
				INT64 nRaiseBat = GetRaiseBat();
				CastChipEx(bet, nRaiseBat);//g_Poker.nRaiseBat);
			}
		}
		SetBackTime();
		// 추가
		FlyWindCard = FALSE;
		// 버튼 셋
		// ### [ 관전기능 ] ###
		if(!g_MyObserver && Play[0].ServPNum == CurPlayer)
		{
			bBtnClick = FALSE;
//			Sound.Play(SND24);
			g_pGameView->SetBtnState(FALSE);
			g_pGameView->AISelectBetMode();

		}
		return;
	}
}

void CGame::EndWindCard()
{
	SetBackTime();
	//// 카드 선택시 시간보이기
	for(int i=0; i<g_Max_Player;i++) {
		if(g_Poker.nState==RSTATE_PREPARESTART && strlen(Play[i].UI.ID)>0 && Play[i].JoinState != 0) {
			Play[i].m_bSelectCard = TRUE;
		}
	}

	// ### [ 관전기능 ] ###
	if( !g_MyObserver && g_Poker.nState==RSTATE_PREPARESTART &&
		strlen(Play[0].UI.ID)>0 && Play[0].JoinState != 0)	// 시작준비 모드이다(카드선택모드)
	{
		m_bchoice_card		= TRUE;
		if(g_RI.FormKind==0)
		//	m_nchoice_cardcnt  = 400;
		m_nchoice_cardcnt = 10000 / int(1000.0f/REFRESH_FPS); // 8초인데 여유시간 2초 더줌 
		else
		//	m_nchoice_cardcnt  = 500;
		m_nchoice_cardcnt = 12000/ int(1000.0f/REFRESH_FPS); // 10초인데 여유시간 2초 더줌 
		return;
		/*
		int aBuf[4] ={0,};
		aBuf[0] = CardDeck[0].GetCardNo(0);
		aBuf[1] = CardDeck[0].GetCardNo(1);
		aBuf[2] = CardDeck[0].GetCardNo(2);

		int select = 0;
		int index = 0;
		int DisCard = 0;
		int DisCardIndex = 0;
	
		SetBackTime();

		if(g_RI.FormKind == 0) {
			//// 카드 선택시 시간보이기
			bSelectCard = 1;
			CSelCardDlg dlg;
			dlg.DoModal( aBuf[0], aBuf[1], aBuf[2] );

			select = dlg.Select;
			index = dlg.Index;
		}
		else {
			//// 카드 선택시 시간보이기
			bSelectCard = 2;
			aBuf[3] = CardDeck[0].GetCardNo(3);
			CSelCardChoiceDlg dlg;
			dlg.DoModal( aBuf[0], aBuf[1], aBuf[2], aBuf[3] );

			select = dlg.Select; // 선택된 카드값(0~51)
			index = dlg.Index;   // 인덱스
			DisCard = dlg.DisCard;			 // 버린 카드값(0~51)
			DisCardIndex = dlg.DisCardIndex; // 버린카드 인덱스(1~4)
		}
		// 해제
		bSelectCard = 0;
	
		if( select < 0	)  {// Fold
			g_pGameView->OnBtnFold();

		} else {	
			int i = 0;
			if(g_RI.FormKind == 0) {
				// 사용자카드재배치
				int cnt = 0;
				for( i=0;i<3;i++) {
					if(index!=i+1){
						CardDeck[0].Card[cnt].CardNo = aBuf[i];
						cnt++;
					}
				}
			} else {//초이스
				if(DisCardIndex != 4) {
					for( i = DisCardIndex-1; i < 3; i++) {
						aBuf[i] = aBuf[i+1];
						CardDeck[0].Card[i].CardNo = aBuf[i];
						aBuf[i+1] = 0;
					}
				}
				for(i = 0; i < 3; i++) {
					if(aBuf[i] == select) // 카드값
					{	
						index = i+1;
						break;
					}
				}

				// 기존 방법
				int cnt = 0;
				for( i=0;i<3;i++) {
					if(index!=i+1){
						CardDeck[0].Card[cnt].CardNo = aBuf[i];
						cnt++;
					}
				}

				//#################################################
				// 카드덱에서 제거
				CardDeck[0].Card[3].Clear();
				CardDeck[0].TotalCardNum -= 1;
				//#################################################
			}
			Sound.Play( SND16 );
			//서버로 내가 선택한 그림 보내준다.
			POKERCLNT pc={0,}; 
			pc.nStyle = 3; // 배팅
			pc.nBtn   = index; // 선택카드 (인덱스 1 , 2, 3)
			
			CCL_ASK_SELECTCARD scard;
			scard.Set(g_RI.RoomNum,  Play[0].UI.UNum, Play[0].ServPNum, Play[0].UI.ID, &pc, DisCardIndex);
			SockMan.SendData(g_MainSrvSID, scard.pData, scard.GetTotalSize());
		}
		return;
	*/
	}

	if(g_Poker.nState==RSTATE_STARTGAME){ // 레이즈모드이다
		// 베팅시작 (카드 다 날리고 베팅 시작)
		// 현재 턴 셋팅
		CurPlayer = g_Poker.CurPlayer;
		//  타이머 보이기
		SetBackTime();
		if(g_Poker.LastStyle >= 2  ) {
			if(g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < g_Max_Player) 
			{
				int bet = GetPNum_ByServPN(g_Poker.LastPlayer);
				
				INT64 nRaiseBat = GetRaiseBat();
				CastChipEx(bet, nRaiseBat);//g_Poker.nRaiseBat);
			}
		}

		// 버튼 셋
		// ### [ 관전기능 ] ###
		if(!g_MyObserver && Play[0].ServPNum == CurPlayer)
		{
			bBtnClick = FALSE;
//			Sound.Play(SND24);
			g_pGameView->SetBtnState(FALSE);
			g_pGameView->AISelectBetMode();
		}
		FlyWindCard = FALSE;
		return;
	}
}


INT64 CGame::GetMaxMoney(INT64 nTotal)
{
	double fBatRate = 0.25;
	if(g_RI.GameKind==0)      fBatRate = 0.25; // 쿼터방
	else if(g_RI.GameKind==1) fBatRate = 0.5;  // 하프방
//////////[풀]	else if(g_RI.GameKind==2) fBatRate = 1.0;  // 풀방
	else                      fBatRate = 0.25; // 쿼터방

	INT64 nCalcBat  = 0;
	nCalcBat = (INT64)(nTotal*fBatRate);
	return nCalcBat;

}

CString CGame::GetName(int nPNum, BOOL bRule)
{
		
	if(nPNum < 0 || nPNum >= g_Max_Player) return "";
	if(strlen(Play[nPNum].UI.ID)==0 || Play[nPNum].JoinState == 0 || Play[nPNum].PlayState == 0 ) return "";
	
	int nSNum = Play[nPNum].ServPNum;
	if( g_Poker.PS[nSNum].bFold ) return "";	
	
	enum SEVEN_CARD eName;
	int nVal=0,i=0;
	int aCard[10]={0,};
	int nCard = g_Poker.PS[nSNum].nCardTotal;
	if(nCard<6) return ""; //  마지막
	
	if(g_RI.ChangeCardStep ==2){//[62]
		for(i=0;i<2;i++){
			aCard[i] = g_Poker.BC[nSNum].nCard[i];
		}
		for(i=2;i<min(nCard,TOTAL_CARD);i++){
			if(i==5){
				if(g_Poker.BC[nSNum].nCard[2] == -1){
					aCard[i] = g_Poker.PS[nSNum].nCard[3];
				}
				else{
					aCard[i] = g_Poker.BC[nSNum].nCard[2];
				}				
			}
			else{
				aCard[i] = g_Poker.PS[nSNum].nCard[i-2];
			}
		}
	}
	else{
		//for(i=0;i<2;i++){
		for(i=0;i<3;i++){
			aCard[i] = g_Poker.BC[nSNum].nCard[i];
		}
		for(i=3;i<min(nCard,TOTAL_CARD);i++){			
			aCard[i] = g_Poker.PS[nSNum].nCard[i-3];
		}
	}	

//	aCard[7] = g_Poker.PS[nSNum].nCard[4]; // ###[8포커] ### 

	eName    = (SEVEN_CARD)m_SevenCard.GetValue(aCard,nCard, &nVal);
	CString strName("");
	int nMainResult = 0;
	strName  = m_SevenCard.GetName(eName,nVal,&nMainResult);
	
	if(bRule)
	{
		if(nCard >= TOTAL_CARD)
			GetCardNum(aCard, eName, nVal );
	}

	return strName;

}


CString CGame::GetMyName()
{
	// ### [ 관전기능 ] ###
	if(g_MyObserver || strlen(Play[0].UI.ID)==0 || Play[0].JoinState == 0 || Play[0].PlayState == 0 ) return "";
	int nSNum = g_MyInfo.ServPNum;
	
	if(nSNum < 0 || nSNum >= g_Max_Player) return "";
	if( g_Poker.PS[nSNum].bFold ) return "";

	enum SEVEN_CARD eName;
	int nVal=0,i=0;
	int aCard[10]={0,};
	int nCard = g_Poker.PS[nSNum].nCardTotal;
	if(nCard<3) return "";
	
 	if(g_RI.ChangeCardStep == 2){//히든카드다.[62]
		for(i=0;i<2;i++){
			aCard[i] = CardDeck[0].GetCardNo(i);
		}
		
		for(i=2;i<min(nCard,TOTAL_CARD);i++){
		//	if(i==5) aCard[i] = g_Poker.BC[nSNum].nCard[2];
			if(i==5)aCard[i] = CardDeck[0].GetCardNo(i);
			else  aCard[i] = g_Poker.PS[nSNum].nCard[i-2];
		}
	}
	else
	{		
		/*softpark
		for(i=0;i<3;i++){
			aCard[i] = CardDeck[0].GetCardNo(i);
		}		
		for(i=3;i<min(nCard,TOTAL_CARD);i++){			
			aCard[i] = g_Poker.PS[nSNum].nCard[i-2];
		}*/		
		for(i=0;i<min(nCard,TOTAL_CARD);i++){
			aCard[i] = CardDeck[0].GetCardNo(i);
		}
	}	

	
	eName    = (SEVEN_CARD)m_SevenCard.GetValue(aCard,nCard, &nVal);
	CString strName="";
	int nMainResult = 0;
	strName  = m_SevenCard.GetName(eName,nVal,&nMainResult);
	MyHighRule = eName;//GetViewMyrule(eName); // 족보 등급 보여주기

	return strName;

}

// ######### 카드 날리는 이벤트 설정 유저가 나갔을 경우.... 이벤트 호출 에러 발생..
// 서버에서 일정 시간 동안 클라이언트 메세지 없으면 자동 시작...
// 클라이언트 타이머 일정 시간대기후 서버로 
void CGame::OnCardEvent(CARDEVENT *pEvent)
{
	if(pEvent == NULL) return;

	switch( pEvent->EventCode )
	{
	case EVENT_THREEFLYEND: // 처음 3장
		{
			// ### [ 관전기능 ] ###
			if(!g_MyObserver && bWindCardEnd  && Play[0].JoinState == 1 && Play[0].PlayState == 1)
			//if(bWindCardEnd  && Play[0].JoinState == 1 && Play[0].PlayState == 1)
			{
				CCL_WINDCARDEND msg2;
				msg2.Set(g_RI.RoomNum, Play[0].ServPNum, Play[0].UI.UNum, Play[0].UI.ID);//int rnum, int pnum, int unum, char *id)
				SockMan.SendData(g_MainSrvSID, msg2.pData, msg2.GetTotalSize());
			}
			
			bWindCardEnd = FALSE;
		} break;

	case EVENT_ONEFLYEND: // 한장씩 날리고 도착하면 보여준다.
		{
			int pnum = pEvent->Option;
			int Index = pEvent->Option2;

			if(pnum < 0 || pnum >= g_Max_Player) break;

			if(Index <= 5)	// modefied by jeong - 처음 6장을 보여준다
				CardDeck[pnum].SetFaceUp(Index);
			else			// 마지막 장은 숨긴다
			{
				//CardDeck[pnum].SetFaceUp(Index);
				CardDeck[pnum].SetFaceDown(Index);
			}
		}
		break;
	
	case EVENT_ONEFLYTOTEND: // 한장씩 모두 날렸다면 서버로
		{
			// ### [ 관전기능 ] ###
			if(!g_MyObserver && bWindCardEnd && Play[0].JoinState == 1 && Play[0].PlayState == 1)
			{
				CCL_WINDCARDEND msg2;
				msg2.Set(g_RI.RoomNum, Play[0].ServPNum, Play[0].UI.UNum, Play[0].UI.ID);//int rnum, int pnum, int unum, char *id)
				SockMan.SendData(g_MainSrvSID, msg2.pData, msg2.GetTotalSize());
			}

			bWindCardEnd = FALSE;
		} break;
	case EVENT_CHANGEDEL_ONEFLYEND: // 바꿀 이미지가 도착 했을때 
		{
			int pnum = pEvent->Option;
			int Index = pEvent->Option2;

			if(pnum < 0 || pnum >= g_Max_Player) break;						
			
			CardDeck[pnum].SetChangeCard();	
			Sound.Play( SND16 );

		}break;
	case EVENT_CHANGESET_ONEFLYEND: // 바꿀 이미지가 도착 했을때 
		{
			int pnum = pEvent->Option;
			int Index = pEvent->Option2;

			if(pnum < 0 || pnum >= g_Max_Player) break;	
			if(g_RI.ChangeCardStep == 1 )//1컷
			{
				if(Index <= 1 ){
					if(!Play[pnum].bFold)CardDeck[pnum].SetFaceUp(Index);
				}else{
					if(!Play[pnum].bFold)CardDeck[pnum].SetFaceUp(5);
				}
			}			
			
			if(pnum == 0 && !g_MyObserver && Play[0].PlayState == 1 && !Play[0].bFold)
			{				
				Game.FlyWindCard = FALSE;
				CCL_CARDCHANGE_END_62CUT cend;
				cend.Set(g_RI.RoomNum,Play[0].ServPNum,Play[0].UI.ID);
				SockMan.SendData(g_MainSrvSID,cend.pData,cend.GetTotalSize());					
			}

			/*
			BOOL  bEndCardWind= TRUE;
			for(int i = 0 ; i < g_Max_Player;i++){
				if(CardDeck[i].bWindCardEnd == TRUE){
					bEndCardWind = FALSE;					
				}
			}
			
			if(bEndCardWind){				
				Game.FlyWindCard = FALSE;
				CCL_CARDCHANGE_END_62CUT cend;
				cend.Set(g_RI.RoomNum,Play[0].ServPNum,Play[0].UI.ID);
				SockMan.SendData(g_MainSrvSID,cend.pData,cend.GetTotalSize());					
			}
			*/
				
		}break;
	}
}


void CGame::AddChip(INT64 nMoney)
{
	if(m_RMoney>=nMoney) return;
	INT64 nAddMoney = nMoney - m_RMoney;
	m_RMoney=nMoney;
	INT64 n100Money = (INT64)(nAddMoney/100);
	if(n100Money<=0 && nAddMoney<50) return;
	if(n100Money<=0) n100Money = 1;
	if(n100Money>10000) n100Money = 10000;
}

void CGame::PreDrawCastedChip(BYTE *pChipCnt)
{
	if(pChipCnt == NULL) return;

	int cnt[MAX_CHIPCNT]={0,};
	for(int i=0; i<MAX_CHIPCNT; i++) cnt[i] = pChipCnt[i];

	int nowchip=0;
	for(;;)
	{
		int zcnt = 0;
		for(i=0; i<MAX_CHIPCNT; i++) 
		{
			if(cnt[i] > 0) {
				cnt[i]--;
				int kind = abs(rand()%3);

				int tx;
				int ty;
			
				if(g_RI.bPlayer == 0) {

					int stx = 0;		 // 위치 
					int sty = 0;
					
					if(g_RI.bPlayer == 0) {
						stx = g_pGameView->m_betpage_x+CAST_CHIP_OFFSET_XP;		 // 위치 
						sty = g_pGameView->m_betpage_y+CAST_CHIP_OFFSET_YP;
					}
					else {
						stx = g_pGameView->m_betpage_x+25;		 // 위치 
						sty = g_pGameView->m_betpage_y+22;
					}

					int w =   g_pGameView->m_diameter;		 // 지름
					int radius = w/2;
					int erad = radius-30;// 효과
					
					int cx = stx + radius;
					int cy = sty + radius;
					
				//	int tx, ty;
					
					for(;;) {
						tx = stx+rand()%w;
						ty = sty+rand()%w;							
						
						int xp = tx - cx;
						int yp = ty - cy;
						
						if(xp*xp + yp*yp < erad*erad) {
							break;
						}
						
						if(xp*xp + yp*yp < radius*radius) {
							if(rand()%5 == 0) break;
						}
					}
				}
				else
				{
				/*
					int tx = BETPAGE_X+15+rand()%220; // 259 크기
					int ty = BETPAGE_Y+20+rand()%135; // 226 
				*/
					tx = g_pGameView->m_betpage_x+CAST_CHIP_OFFSET_XP+rand()%(g_pGameView->m_betpagesize_x-45); // 259 크기
					ty = g_pGameView->m_betpage_y+CAST_CHIP_OFFSET_YP+rand()%(g_pGameView->m_betpagesize_y-45); // 226 
				}

			    tx = tx-(g_pGameView->m_betpage_x); // 작은 페이지 좌표로
			    ty = ty-(g_pGameView->m_betpage_y); //
			

				int sprno = i*3+kind;
				g_pGameView->BatPage.PutSprAuto(tx, ty, &ChipSpr, sprno);
			}
			else zcnt++;
		}
		if(zcnt>=MAX_CHIPCNT) break;//7 칩 제한 걸기
	}

}

void CGame::CastChip(int pnum, INT64 nMarble, int sdelay)
{
	if(pnum < 0 || pnum >= g_Max_Player) return;

	INT64 marble = nMarble;
	int cn[MAX_CHIPCNT]={0,};//7


	if(marble >= 50000000 && marble <= 2000000000) // 5천만 ~ 20억
	{
		if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}
	else if( marble >= 2000000000 && marble <= 50000000000) // 20억 ~ 500억
	{
		if(marble>=1000000000) {cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
		if(marble>=500000000) {cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
		
		if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}
	else if( marble >= 50000000000 && marble <= 200000000000) // 500억 ~ 2000억
	{
		if(marble>=10000000000) {cn[15]=(int)(marble/10000000000); marble%=10000000000;} // 100억
		if(marble>=1000000000) {cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
		if(marble>=500000000) {cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
		
		if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}
	else if(marble > 200000000000 ) // 2000억 ~
	{
		if(marble>=100000000000) {cn[16]=(int)(marble/100000000000); marble%=100000000000;} // 1000억
		
		if(marble>=10000000000) {cn[15]=(int)(marble/10000000000); marble%=10000000000;} // 100억
		if(marble>=1000000000) {cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
		if(marble>=500000000) {cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
		if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}

	if(marble>=1000000) {cn[10]=(int)(marble/1000000); marble%=1000000;} //100 만
	if(marble>=100000) {cn[9]=(int)(marble/100000); marble%=100000;}
	if(marble>=10000) {cn[8]=(int)(marble/10000); marble%=10000;}
	if(marble>=5000) {cn[7]=(int)(marble/5000); marble%=5000;}
	if(marble>=1000) {cn[6]=(int)(marble/1000); marble%=1000;}
	if(marble>=500) {cn[5]=(int)(marble/500); marble%=500;}
	if(marble>=100) {cn[4]=(int)(marble/100); marble%=100;}
	if(marble>=50) {cn[3]=(int)(marble/50); marble%=50;}
	if(marble>=10) {cn[2]=(int)(marble/10); marble%=10;}
	if(marble>=5) {cn[1]=(int)(marble/5); marble%=5;}
	if(marble>=1) {cn[0]=(int)(marble/1); marble%=1;}

	int sx=0, sy=0;


	if(g_RI.bPlayer == 0) {
		
		switch(pnum)
		{
		case 0: sx=400; sy=450; break; 
		case 1: sx=252; sy=305; break; 
		case 2: sx=252; sy=138; break; 
		case 3: sx=546; sy=138;  break; 
		case 4: sx=546; sy=305;  break; 
		default : return;
		}
	}
	else {
		switch(pnum)
		{
		case 0: sx=400; sy=450; break; 
		case 1: sx=252; sy=324; break; 
		case 2: sx=246; sy=181; break; 
		case 3: sx=389; sy=140;  break; 
		case 4: sx=546; sy=181;  break; 
		case 5: sx=546; sy=324;  break; 
		default : return;
		}
	}

	int nowchip=0;
	for(int i=MAX_CHIPCNT-1; i>=0; i--)//6
	{
		for(int j=0; j<cn[i]; j++) 
		{
			for(int n=nowchip; n<MAX_CHIP; n++)
			{
				nowchip = n;
				if(m_Chip[n].bLive == FALSE)
				{
					int kind = abs(rand()%3);
					

					int stx = 0;		 // 위치 
					int sty = 0;
					int tx, ty;

					if(g_RI.bPlayer == 0) {
						if(g_RI.bPlayer == 0) {
							stx = g_pGameView->m_betpage_x+CAST_CHIP_OFFSET_XP;		 // 위치 
							sty = g_pGameView->m_betpage_y+CAST_CHIP_OFFSET_YP;
						}
						else {
							stx = g_pGameView->m_betpage_x+25;		 // 위치 
							sty = g_pGameView->m_betpage_y+22;
						}

						
						int w =	  g_pGameView->m_diameter;		 // 지름
						int radius = w/2;
						int erad = radius-30;// 효과
						
						int cx = stx + radius;
						int cy = sty + radius;
						
						
						
						for(;;) {
							tx = stx+rand()%w;
							ty = sty+rand()%w;
							
							int xp = tx - cx;
							int yp = ty - cy;
							
							if(xp*xp + yp*yp < erad*erad) {
								break;
							}
							
							if(xp*xp + yp*yp < radius*radius) {
								if(rand()%5 == 0) break;
							}
						}
									
					} else {
					//	int tx = BETPAGE_X+15+rand()%220; // 259 크기
					//	int ty = BETPAGE_Y+20+rand()%135; // 226 
						tx = g_pGameView->m_betpage_x+CAST_CHIP_OFFSET_XP+rand()%(g_pGameView->m_betpagesize_x-45); // 259 크기
						ty = g_pGameView->m_betpage_y+CAST_CHIP_OFFSET_YP+rand()%(g_pGameView->m_betpagesize_y-45); // 226 
						
					}

					m_Chip[n].SetChip(i, kind);
					m_Chip[n].SetPos(sx, sy);
					m_Chip[n].SetMove(tx, ty, sdelay);
					break;
				}
			}
			if(n == MAX_CHIP) return;//100
		}
	}

}

void CGame::CastChipEx(int pnum, INT64 nMarble, int sdelay)		// CastChipEx - created by jeong
{
	if(pnum < 0 || pnum >= g_Max_Player) return;

	INT64 marble = nMarble;

	for( int c=0; c<MAX_CHIPCNT; c++)
		m_cn[c]=0;

	if(marble >= 50000000 && marble <= 2000000000) // 5천만 ~ 20억
	{
		if(marble>=100000000) {m_cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {m_cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}
	else if( marble >= 2000000000 && marble <= 50000000000) // 20억 ~ 500억
	{
		if(marble>=1000000000) {m_cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
		if(marble>=500000000) {m_cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
		
		if(marble>=100000000) {m_cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {m_cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}
	else if( marble >= 50000000000 && marble <= 200000000000) // 500억 ~ 2000억
	{
		if(marble>=10000000000) {m_cn[15]=(int)(marble/10000000000); marble%=10000000000;} // 100억
		if(marble>=1000000000) {m_cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
		if(marble>=500000000) {m_cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
		
		if(marble>=100000000) {m_cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {m_cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}
	else if(marble > 200000000000 ) // 2000억 ~
	{
		if(marble>=100000000000) {m_cn[16]=(int)(marble/100000000000); marble%=100000000000;} // 1000억
		
		if(marble>=10000000000) {m_cn[15]=(int)(marble/10000000000); marble%=10000000000;} // 100억
		if(marble>=1000000000) {m_cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
		if(marble>=500000000) {m_cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
		if(marble>=100000000) {m_cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {m_cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}

	if(marble>=1000000) {m_cn[10]=(int)(marble/1000000); marble%=1000000;} //100 만
	if(marble>=100000) {m_cn[9]=(int)(marble/100000); marble%=100000;}
	if(marble>=10000) {m_cn[8]=(int)(marble/10000); marble%=10000;}
	if(marble>=5000) {m_cn[7]=(int)(marble/5000); marble%=5000;}
	if(marble>=1000) {m_cn[6]=(int)(marble/1000); marble%=1000;}
	if(marble>=500) {m_cn[5]=(int)(marble/500); marble%=500;}
	if(marble>=100) {m_cn[4]=(int)(marble/100); marble%=100;}
	if(marble>=50) {m_cn[3]=(int)(marble/50); marble%=50;}
	if(marble>=10) {m_cn[2]=(int)(marble/10); marble%=10;}
	if(marble>=5) {m_cn[1]=(int)(marble/5); marble%=5;}
	if(marble>=1) {m_cn[0]=(int)(marble/1); marble%=1;}

	int sx=0, sy=0;

	// AI 배팅 시작점
	const int nAIBetOffsetX = 400;
	const int nAIBetOffsetY = 150;

	if(g_RI.bPlayer == 0) {
		
		switch(pnum)
		{
			case 0: sx=400; sy=450; break; 
			default: 
				sx=nAIBetOffsetX; sy=nAIBetOffsetY;
			break;
		}
	}
	else {
		switch(pnum)
		{
			case 0: sx=400; sy=450; break; 
			default: 
				sx=nAIBetOffsetX; sy=nAIBetOffsetY;
			break;
		}
	}
	
	const int nbetpage_x = 320;
	const int nbetpage_y = 190;

	int nowchip=0;
	for(int i=MAX_CHIPCNT-1; i>=0; i--)//6
	{
		for(int j=0; j<m_cn[i]; j++) 
		{
			for(int n=nowchip; n<MAX_CHIP; n++)
			{
				nowchip = n;
				if(m_Chip[n].bLive == FALSE)
				{
					int kind = abs(rand()%3);
					

					int stx = 0;		 // 위치 
					int sty = 0;
					int tx, ty;

					if(g_RI.bPlayer == 0) {
						if(g_RI.bPlayer == 0) {
							stx = nbetpage_x;		 // 위치 
							sty = nbetpage_y;
						}
						else {
							stx = nbetpage_x;		 // 위치 
							sty = nbetpage_y;
						}
						
						const int nDiameter = 80;
						int w =	  g_pGameView->m_diameter-nDiameter;		 // 지름
						int radius = w/2;
						int erad = radius-20;// 효과
						
						int cx = stx + radius;
						int cy = sty + radius;
						
						
						
						for(;;) {
							tx = stx+rand()%w;
							ty = sty+rand()%w;
							
							int xp = tx - cx;
							int yp = ty - cy;
							
							if(xp*xp + yp*yp < erad*erad) {
								break;
							}
							
							if(xp*xp + yp*yp < radius*radius) {
								if(rand()%5 == 0) break;
							}
						}
						
									
					} else {
					//	int tx = BETPAGE_X+15+rand()%220; // 259 크기
					//	int ty = BETPAGE_Y+20+rand()%135; // 226 
						tx = nbetpage_x+rand()%(nbetpage_x-10); // 259 크기
						ty = nbetpage_y+rand()%(nbetpage_y-10); // 226 
						
					}
					
					m_Chip[n].SetChip(i, kind);
					m_Chip[n].SetPos(sx, sy);
					m_Chip[n].SetMove(tx, ty, sdelay);
					m_Chip[n].SetStack( TRUE );
					break;
				}
			}
			if(n == MAX_CHIP) return;//100
		}
	}

	//StackChip();
}

void CGame::StackChip()	
{
	// stack money after betting - jeong
	int nowchipEx=0;
	int nStackX=0;
	const int nStartX=295;
	for(int i=MAX_CHIPCNT-1; i>=0; i--)//6
	{
		for(int j=0; j<m_cn[i]; j++) 
		{
			for(int n=nowchipEx; n<MAX_CHIP; n++)
			{
				if(m_Chip[n].bStack == TRUE)
				{
					int kind = abs(rand()%3);

					nStackX = nStartX + (i*35);				
					m_nStackY[i] = m_nStackY[i] - 5;
					
					m_Chip[n].SetStack( FALSE );
					//m_ChipEx[n].SetChip(i, kind);
					m_Chip[n].SetPos(nStackX, m_nStackY[i]);
					m_Chip[n].SetMove(nStackX, m_nStackY[i], 1);
					break;
				}
			}
			if(n == MAX_CHIP) return;//100
		}
	}
}


// 잭팟용 추가
void CGame::CastChipJackPot(int pnum, INT64 nMarble, int sdelay)
{
	if(pnum < 0 || pnum >= 8) return;

	INT64 marble = nMarble;
	int cn[MAX_CHIPCNT]={0,};//7

	if(marble >= 50000000 && marble <= 2000000000) // 5천만 ~ 20억
	{
		if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}
	else if( marble >= 2000000000 && marble <= 50000000000) // 20억 ~ 500억
	{
		if(marble>=1000000000) {cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
		if(marble>=500000000) {cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
		
		if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}
	else if( marble >= 50000000000 && marble <= 200000000000) // 500억 ~ 2000억
	{
		if(marble>=10000000000) {cn[15]=(int)(marble/10000000000); marble%=10000000000;} // 100억
		if(marble>=1000000000) {cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
		if(marble>=500000000) {cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
		
		if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}
	else if(marble > 200000000000 ) // 2000억 ~
	{
		if(marble>=100000000000) {cn[16]=(int)(marble/100000000000); marble%=100000000000;} // 1000억
		
		if(marble>=10000000000) {cn[15]=(int)(marble/10000000000); marble%=10000000000;} // 100억
		if(marble>=1000000000) {cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
		if(marble>=500000000) {cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
		if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
		if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
	}

	if(marble>=1000000) {cn[10]=(int)(marble/1000000); marble%=1000000;} //100 만
	if(marble>=100000) {cn[9]=(int)(marble/100000); marble%=100000;}
	if(marble>=10000) {cn[8]=(int)(marble/10000); marble%=10000;}
	if(marble>=5000) {cn[7]=(int)(marble/5000); marble%=5000;}
	if(marble>=1000) {cn[6]=(int)(marble/1000); marble%=1000;}
	if(marble>=500) {cn[5]=(int)(marble/500); marble%=500;}
	if(marble>=100) {cn[4]=(int)(marble/100); marble%=100;}
	if(marble>=50) {cn[3]=(int)(marble/50); marble%=50;}
	if(marble>=10) {cn[2]=(int)(marble/10); marble%=10;}
	if(marble>=5) {cn[1]=(int)(marble/5); marble%=5;}
	if(marble>=1) {cn[0]=(int)(marble/1); marble%=1;}

	int sx=0, sy=0;
/*
	switch(pnum)
	{
	case 0: sx=55; sy=50; break; 
	case 1: sx=165; sy=50; break; 
	case 2: sx=275; sy=50; break; 
	case 3: sx=385; sy=50; break; 
	case 4: sx=495; sy=50; break; 
	case 5: sx=505; sy=50; break; 
	case 6: sx=555; sy=50; break; 
	case 7: sx=425; sy=50; break; 
	default : return;
	}
	*/
	if(g_RI.bPlayer==0 ){
		sx = 390;
		sy = 20;
	} else {
		sx = 554;
		sy = 55;
	}

	int nowchip=0;
	for(int i=MAX_CHIPCNT-1; i>=0; i--)//6
	{
		for(int j=0; j<cn[i]; j++) 
		{
			for(int n=nowchip; n<MAX_CHIP; n++)
			{
				nowchip = n;
				if(m_Chip[n].bLive == FALSE)
				{
					int kind = abs(rand()%3);

					
					int stx = 0;		 // 위치 
					int sty = 0;
					int tx, ty;

					if(g_RI.bPlayer == 0) {
						if(g_RI.bPlayer == 0) {
							stx = g_pGameView->m_betpage_x+CAST_CHIP_OFFSET_XP;		 // 위치 
							sty = g_pGameView->m_betpage_y+CAST_CHIP_OFFSET_YP;
						}
						else {
							stx = g_pGameView->m_betpage_x+25;		 // 위치 
							sty = g_pGameView->m_betpage_y+22;
						}
						
						int w =	  g_pGameView->m_diameter;		 // 지름
						int radius = w/2;
						int erad = radius-30;// 효과
						
						int cx = stx + radius;
						int cy = sty + radius;
						
						
						
						for(;;) {
							tx = stx+rand()%w;
							ty = sty+rand()%w;
							
							int xp = tx - cx;
							int yp = ty - cy;
							
							if(xp*xp + yp*yp < erad*erad) {
								break;
							}
							
							if(xp*xp + yp*yp < radius*radius) {
								if(rand()%5 == 0) break;
							}
						}
									
					} else {
					//	int tx = BETPAGE_X+15+rand()%220; // 259 크기
					//	int ty = BETPAGE_Y+20+rand()%135; // 226 
						tx = g_pGameView->m_betpage_x+CAST_CHIP_OFFSET_XP+rand()%(g_pGameView->m_betpagesize_x-45); // 259 크기
						ty = g_pGameView->m_betpage_y+CAST_CHIP_OFFSET_YP+rand()%(g_pGameView->m_betpagesize_y-45); // 226 
					}

					m_Chip[n].SetChip(i, kind);
					m_Chip[n].SetPos(sx, sy);
					m_Chip[n].SetMove(tx, ty, sdelay);
					break;
				}
			}
			if(n == MAX_CHIP) return;//100
		}
	}

}



// 타이머 그래프
void CGame::SetBackTime()
{
	BackTime = timeGetTime();
	memset( ClockFlag, FALSE, sizeof( BOOL ) * 16 );
}

// 5 장의 카드를 리턴 한다.
void CGame::GetCardNum(int pCard[], enum SEVEN_CARD eName, int nVal)
{
	if(pCard == NULL) return;
	
	ZeroMemory(m_CardHi,sizeof(m_CardHi));
	
	int N[7];
	int P[7];
	int nNum = 7;//6

	int i=0;
	for(i=0; i<7; i++){ N[i] = 0; P[i] = 0; }//먼저 카드데이타를 클리어하고..
	int  nResult = 0; int nFlushPic =-1;
	int  nSpade=0,    nHeart=0,    nDia=0,    nClov=0;
	int n2=0,n3=0,n4=0,n5=0,n6=0,n7=0,n8=0,n9=0,n10=0,n11=0,n12=0,n13=0,n14=0;
	for(i=0; i<nNum; i++){
		int nCard = pCard[i];
		if(nCard == 0 || nCard == 13 || nCard == 26 || nCard == 39) 
			N[i] = 14;
		else
			N[i] = nCard - 13*(nCard/13)+1;
		if(0<=nCard && nCard<13) 
			P[i] = 0;//클로바.
		else if(13<=nCard && nCard<26) 
			P[i] = 1;//하트.
		else if(26<=nCard && nCard<39) 
			P[i] = 2;//다이아.
		else if(39<=nCard && nCard<52) 
			P[i] = 3;//스페이드.

		if(nCard==2) n2++;        else if(nCard==3) n3++;   else if(nCard==4) n4++;
		else if(nCard==5) n5++;   else if(nCard==6) n6++;   else if(nCard==7) n7++;
		else if(nCard==8) n8++;   else if(nCard==9) n9++;   else if(nCard==10) n10++;
		else if(nCard==11) n11++; else if(nCard==12) n12++; else if(nCard==13) n13++;
		else if(nCard==14) n14++;

		if(P[i]==3) nSpade++;       else if(P[i]==2) nDia++;
		else if(P[i]==1) nHeart++;  else if(P[i]==0) nClov++;
		if(nSpade>=5){ nFlushPic = 3; }
		if(nDia>=5)  { nFlushPic = 2; }
		if(nHeart>=5){ nFlushPic = 1; }
		if(nClov>=5) { nFlushPic = 0; }
	}

	m_SevenCard.BubbleSort(N,P,nNum);

	if(eName==N7C_SROYAL){
		//strName = "로얄 스트레이트 플러시";
		
		m_CardHi[0] = 39;
		int card = 51;
		for( i = 1; i < 5; i++)
			m_CardHi[i]= card--;
	}
	else if(eName==N7C_DROYAL){
		//strName = "다이아 로티플";
		m_CardHi[0] = 26;
		int card = 38;
		for( i = 1; i < 5; i++)
			m_CardHi[i]= card--;

	}
	else if(eName==N7C_HROYAL){
		//strName = "하트 로티플";
		m_CardHi[0] = 13;
		int card = 25;
		for( i = 1; i < 5; i++)
			m_CardHi[i]= card--;
		
	}
	else if(eName==N7C_CROYAL){
		//strName = "크로바 로티플";
	
		m_CardHi[0] = 0;
		int card = 12;
		for( i = 1; i < 5; i++)
			m_CardHi[i]= card--;
		
	}

	else if(eName==N7C_BSFLUSH){
		//strName = "백스트레이트 플러시";

		int pic[4] = { 0,};
		int curPic = 0;
		for(i = 0; i < TOTAL_CARD; i++)
		{
			if( pic[ P[i] ]++ > 2) 
			{
				curPic = P[i];
				break; // 가장 많은 무늬
			}
		}
		
		int card = 13*curPic;

		for( i = 0; i < 5; i++)
		{
			m_CardHi[i]= card++;
		}
	}
	else if(eName==N7C_SFULSH){ 
	
		//strName = "스트레이트 플러쉬";
		int pic[4] = { 0,};
		int curPic = 0;
		for(i = 0; i < TOTAL_CARD; i++)
		{
			if( pic[ P[i] ]++ > 2) 
			{
				curPic = P[i];
				break; // 가장 많은 무늬
			}
		}

		if(nVal==14) // A
		{
			m_CardHi[0] = 13*curPic;
			int card = 13*curPic+12;
			for( i = 1; i < 5; i++)
			{
				m_CardHi[i]= card--;
			}
		}
		else
		{
			int card = 13*curPic+(nVal-1);
				
			for( i = 0; i < 5; i++)
			{
				m_CardHi[i]= card--;
			}
		}
/*
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
*/
	}
	else if(eName==N7C_QUADS){
	
		//strName = "포카드";
		if(nVal==14)
		{
			int card = 39;
			for(i = 0; i<4; i++)
			{
				m_CardHi[i] = card;
				card -= 13;
			}
			
			for(i = TOTAL_CARD-1; i >=0 ; i--)
			{
				if(N[i] != 14)
				{
					m_CardHi[4]= (N[i]-1) + P[i]*13;
					break;
				}
			}
		}
		else
		{
			int card = nVal-1; //12
			for(i = 0; i<4; i++)
			{
				m_CardHi[i] = card;
				card += 13;
			}

			for(i = TOTAL_CARD -1; i >=0 ; i--)
			{
				if(N[i] != nVal)
				{
					if(N[i] == 14)
						m_CardHi[4] = P[i]*13;
					else
						m_CardHi[4]= (N[i]-1) + P[i]*13;
					break;
				}
			}
		}
/*

		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
*/
	}
	else if(eName==N7C_TITLE){
		//strName = "집(풀하우스)";
		int nV1 = (int)nVal/100;
		int nV2 = (int)nVal%100;

		int count = 0;
		if(nV1==14) // 3장 
		{			
			for(i = 0; i<TOTAL_CARD; i++)
			{
				if(N[i] == 14)
						m_CardHi[count++] =  P[i]*13;
				if(count >= 3) break;
				
			}
		}
		else
		{
			for(i = 0; i<TOTAL_CARD; i++)
			{
				if(N[i] == nV1)
						m_CardHi[count++] = (N[i]-1) + P[i]*13;
				if(count >= 3) break;
				
			}
		}

		if(nV2==14) // 2장 
		{
			for(i = 0; i<TOTAL_CARD; i++)
			{
				if(N[i] == 14)
						m_CardHi[count++] = P[i]*13;
				if(count >= 5) break;
			}
		}
		else
		{
			for(i = 0; i<TOTAL_CARD; i++)
			{
				if(N[i] == nV2)
						m_CardHi[count++] = (N[i]-1) + (P[i]*13);
				if(count >= 5) break;
				
			}
		}
/*
		CString s1="",s2="";
		if(nV1==14)      s1 = "Ace";
		else if(nV1==13) s1="K";
		else if(nV1==12) s1="Q";
		else if(nV1==11) s1="J";
		else s1.Format("%d",nV1);

		if(nV2==14)      s2 = "Ace";
		else if(nV2==13) s2="K";
		else if(nV2==12) s2="Q";
		else if(nV2==11) s2="J";
		else s2.Format("%d",nV2);

		strVal.Format("%s,%s",s1,s2);
*/
	}
	else if(eName==N7C_FLUSH){
		//strName = "플러시";

		int pic[4] = { 0,};
		int curPic = 0;
		for(i = 0; i < TOTAL_CARD; i++)
		{
			if( pic[ P[i] ]++ > 2) 
			{
				curPic = P[i];
				break; // 가장 많은 무늬
			}
		}

		int count = 0;

		for(i = TOTAL_CARD-1; i >=0 ; i--)
		{
			if( curPic == P[i])
			{
				if(N[i] == 14)
					m_CardHi[count++] = P[i]*13;
				else
					m_CardHi[count++] = (N[i]-1) + P[i]*13;

			}
			if(count >=5) break;
		}
	
/*

		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
*/
	}
	else if(eName==N7C_MOUNT) 
	{
		//strName = "마운틴(스트레이트)";
		int count = 0;
		int flag = 0;
		for(i = TOTAL_CARD-1; i >=0 ; i--)
		{
			if(flag == 0)
			{
				if(N[i] == 14)
					m_CardHi[count++] = P[i]*13;
				else
					m_CardHi[count++] = (N[i]-1) + P[i]*13;
				flag = 1;
			}
			else
			{
				if(m_CardHi[count-1]%13+1 == N[i]) continue;
				m_CardHi[count++] = (N[i]-1) + P[i]*13;
			}

			if(count >=5) break;
		}


	}
	else if(eName==N7C_BACKS)
	{
		//strName = "백스트레이트";  A 1 2 3 4 5 
		m_CardHi[0] = P[6]*13;
		
		int Cnt = 4; // 5 
		int count = 4;
		for(i = TOTAL_CARD-1; i >=0 ; i--)
		{
			if(N[i]-1 == Cnt) 
			{
				m_CardHi[count--] = Cnt + P[i]*13; // 순서 주의
				Cnt--;
				if(count < 1 || Cnt < 0) break;
			}
		}
 
	}
	else if(eName==N7C_STRAIGHT){
	//	strName = "스트레이트";

		int count = 0;
		int flag = 0;

		for(i = TOTAL_CARD-1; i >=0 ; i--)
		{
			if(N[i] == nVal)
			{
				if(N[i] == 14)
					m_CardHi[count++] = P[i]*13;
				else
					m_CardHi[count++] = (N[i]-1) + P[i]*13;
				
				for(int j = i; j >= 0; j--)
				{
					if(m_CardHi[count-1]%13+1 == N[j]) continue;
					m_CardHi[count++] = (N[j]-1) + P[j]*13;
					if(count >=5) break;
				}
				break;
			}
		}
/*
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
*/
	}
	else if(eName==N7C_TRIPLE){
		//strName = "봉(쓰리카드)";

		int count = 0; // 3장 
		for(i = 0; i < TOTAL_CARD; i++)
		{
			if(nVal == N[i])
			{
				if(nVal == 14)
					m_CardHi[count++] = P[i]*13;
				else
					m_CardHi[count++] = (N[i]-1) + P[i]*13;
				
				if(count >= 3) break;
			}
		}

		for(i = TOTAL_CARD-1; i >=0 ; i--) // 나머지 2장
		{
			if(N[i] != nVal)
			{
				if(N[i] == 14)
					m_CardHi[count++] = P[i]*13;
				else
					m_CardHi[count++]= (N[i]-1) + P[i]*13;
				
				if(count >= 5) break;
			}
		}
/*
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
*/
	}
	else if(eName==N7C_TWOPAIR){
	//	strName = "투페어";
		int nV1 = (int)nVal/100;
		int nV2 = (int)nVal%100;

		int count = 0; // 3장 
		for(i = TOTAL_CARD-1; i >= 0 ; i--)
		{
			if(nV1 == N[i])
			{
				if(N[i] == 14)
					m_CardHi[count++] = P[i]*13;
				else
					m_CardHi[count++]= (N[i]-1) + P[i]*13;
			}
			else if(nV2 == N[i])
			{
				if(N[i] == 14)
					m_CardHi[count++] = P[i]*13;
				else
					m_CardHi[count++]= (N[i]-1) + P[i]*13;
			}

			if(count >= 4) break;
		}

		for(i = TOTAL_CARD-1; i >=0 ; i--) // 나머지 2장
		{
			if(N[i] != nV1 && N[i] != nV2 )
			{
				if(N[i] == 14)
					m_CardHi[count++] = P[i]*13;
				else
					m_CardHi[count++]= (N[i]-1) + P[i]*13;
				
				if(count >= 5) break;
			}
		}

/*
		CString s1="",s2="";
		if(nV1==14)      s1 = "Ace";
		else if(nV1==13) s1="K";
		else if(nV1==12) s1="Q";
		else if(nV1==11) s1="J";
		else s1.Format("%d",nV1);

		if(nV2==14)      s2 = "Ace";
		else if(nV2==13) s2="K";
		else if(nV2==12) s2="Q";
		else if(nV2==11) s2="J";
		else s2.Format("%d",nV2);

		strVal.Format("%s,%s",s1,s2);
*/
	}
	else if(eName==N7C_ONEPAIR){
		//strName = "원페어";

		int count = 0;
		for(i = TOTAL_CARD-1; i >= 0 ; i--) // 2장
		{
			if(nVal == N[i])
			{
				if(N[i] == 14)
					m_CardHi[count++] = P[i]*13;
				else
					m_CardHi[count++]= (N[i]-1) + P[i]*13;
			}
		
			if(count >= 2) break;
		}

		for(i = TOTAL_CARD-1; i >=0 ; i--) // 나머지 3장
		{
			if(N[i] != nVal)
			{
				if(N[i] == 14)
					m_CardHi[count++] = P[i]*13;
				else
					m_CardHi[count++]= (N[i]-1) + P[i]*13;
				
				if(count >= 5) break;
			}
		}

/*
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
*/
	}
	else if(eName==N7C_NOPAIR){
	//	strName = "";

		int count = 0;
		for(i = TOTAL_CARD-1; i >= 0 ; i--) // 
		{
			if(N[i] == 14)
				m_CardHi[count++] = P[i]*13;
			else
				m_CardHi[count++]= (N[i]-1) + P[i]*13;
		
			if(count >= 5) break;
		}



	}
}

// ### [ Frame++ ] ###
void CGame::SetWindCardEnd(BOOL bFirst)
{
	if(bWindCardEnd == FALSE)
	{
		
		if( bFirst ) // 3장 3명 => 61초 1사람당 20초
		{
			nWindCardEndCnt = 3*50+(22*StartUserNum);
		}
		else // 1장 7초+
		{
			nWindCardEndCnt = 3*30+(9*StartUserNum);
		}
		bWindCardEnd = TRUE;
	}
}



//pg.nRaiseBat = m_Raise.GetRaiseBat();
INT64 CGame::GetRaiseBat()
{
	INT64 nRaiseBat = 0;

	for(int i=0;i<g_Max_Player;i++)
		nRaiseBat = max(nRaiseBat,g_Poker.RU[i].nNowBat);

	return nRaiseBat;
}

//pg.nTotBat   = m_Raise.GetRealBet();
INT64 CGame::GetRealBet()
{
	INT64 nRealBet = 0;

	for(int i=0;i<g_Max_Player;i++)
		if(g_Poker.RU[i].bInUse) nRealBet+= g_Poker.RU[i].nRealBat;

	return nRealBet;
}


INT64 CGame::GetTotalBet()
{
	INT64 nRealBet = 0;

	for(int i=0;i<g_Max_Player;i++)
		if(g_Poker.RU[i].bInUse) nRealBet+= g_Poker.RU[i].nTotBat;

	return nRealBet;
}

// ### [사운드 추가 작업] ###
void CGame::SoundFxKind(int betkind, int sex, int fxkind)
{
	switch(betkind)
	{
	case 1: //다이
		{
			switch(fxkind)
			{
			case 0: 	Sound.Play( SNDM0_0 ); break;
			case 1: 	Sound.Play( SNDM1_0 ); break;
			case 2: 	Sound.Play( SNDM2_0 ); break;
			case 3: 	Sound.Play( SNDF0_0 ); break;
			case 4: 	Sound.Play( SNDF1_0 ); break;
			case 5: 	Sound.Play( SNDF2_0 ); break;
			default :	Sound.Play( SNDM0_0 ); break;
			}
		} break;
	case 2:// 체크
		{
			switch(fxkind)
			{
			case 0: 	Sound.Play( SNDM0_1 ); break;
			case 1: 	Sound.Play( SNDM1_1 ); break;
			case 2: 	Sound.Play( SNDM2_1 ); break;
			case 3: 	Sound.Play( SNDF0_1 ); break;
			case 4: 	Sound.Play( SNDF1_1 ); break;
			case 5: 	Sound.Play( SNDF2_1 ); break;
			default :	Sound.Play( SNDM0_1 ); break;
			}
		} break;
	case 3: // 콜
		{	
			switch(fxkind)
			{
			case 0: 	Sound.Play( SNDM0_2 ); break;
			case 1: 	Sound.Play( SNDM1_2 ); break;
			case 2: 	Sound.Play( SNDM2_2 ); break;
			case 3: 	Sound.Play( SNDF0_2 ); break;
			case 4: 	Sound.Play( SNDF1_2 ); break;
			case 5: 	Sound.Play( SNDF2_2 ); break;
			default :	Sound.Play( SNDM0_2 ); break;
			}
		} break;

	case 4: // 삥
		{	
			switch(fxkind)
			{
			case 0: 	Sound.Play( SNDM0_3 ); break;
			case 1: 	Sound.Play( SNDM1_3 ); break;
			case 2: 	Sound.Play( SNDM2_3 ); break;
			case 3: 	Sound.Play( SNDF0_3 ); break;
			case 4: 	Sound.Play( SNDF1_3 ); break;
			case 5: 	Sound.Play( SNDF2_3 ); break;
			default :	Sound.Play( SNDM0_3 ); break;
			}
		} break;

	case 5: // 따당
		{	
			switch(fxkind)
			{
			case 0: 	Sound.Play( SNDM0_4 ); break;
			case 1: 	Sound.Play( SNDM1_4 ); break;
			case 2: 	Sound.Play( SNDM2_4 ); break;
			case 3: 	Sound.Play( SNDF0_4 ); break;
			case 4: 	Sound.Play( SNDF1_4 ); break;
			case 5: 	Sound.Play( SNDF2_4 ); break;
			default :	Sound.Play( SNDM0_4 ); break;
			}
		} break;
	case 6: // 맥스
		{	
			if(g_RI.GameKind == 1) {
				switch(fxkind)
				{
				case 0: 	Sound.Play( SNDM0_5 ); break;
				case 1: 	Sound.Play( SNDM1_5 ); break;
				case 2: 	Sound.Play( SNDM2_5 ); break;
				case 3: 	Sound.Play( SNDF0_5 ); break;
				case 4: 	Sound.Play( SNDF1_5 ); break;
				case 5: 	Sound.Play( SNDF2_5 ); break;
				default :	Sound.Play( SNDM0_5 ); break;
				}
			} else {
				switch(fxkind)
				{
				case 0: 	Sound.Play( SNDM0_5_Q ); break;
				case 1: 	Sound.Play( SNDM1_5_Q ); break;
				case 2: 	Sound.Play( SNDM2_5_Q ); break;
				case 3: 	Sound.Play( SNDF0_5_Q ); break;
				case 4: 	Sound.Play( SNDF1_5_Q ); break;
				case 5: 	Sound.Play( SNDF2_5_Q ); break;
				default :	Sound.Play( SNDM0_5_Q ); break;
				}
			}
		} break;
	case 7: // 따당
		{	
			switch(fxkind)
			{
			case 0: 	Sound.Play( SNDM0_6 ); break;
			case 1: 	Sound.Play( SNDM1_6 ); break;
			case 2: 	Sound.Play( SNDM2_6 ); break;
			case 3: 	Sound.Play( SNDF0_6 ); break;
			case 4: 	Sound.Play( SNDF1_6 ); break;
			case 5: 	Sound.Play( SNDF2_6 ); break;
			default :	Sound.Play( SNDM0_6 ); break;
			}
		} break;
	}
}

void CGame::ChipFunc(USERINFO *pUI)
{
	BYTE ChipCnt[MAX_CHIPCNT] = { 0,};
	INT64 RealBet = GetRealBet();
	
	if( RealBet > 5000000)
	{
		for(int i = 0; i < g_Max_Player; i++)	{
			int pn = GetPNum_ByServPN(i);			
			if(Play[pn].JoinState==1) {
				INT64 marble = g_Poker.RU[i].nRealBat;
				int cn[MAX_CHIPCNT]={0,};//7
				if(g_RI.Level == 2) { //중수
					if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
					if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
				}
				else if(g_RI.Level == 3) //고수
				{
					if(marble>=10000000000) {cn[15]=(int)(marble/10000000000); marble%=10000000000;} // 100억
					if(marble>=1000000000) {cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
					if(marble>=500000000) {cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
					if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
					if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
				}
				else if(g_RI.Level == 4) // 도신
				{
					//########################################
					// 추가 예정
					if(marble>=100000000000) {cn[16]=(int)(marble/100000000000); marble%=100000000000;} // 1000억
					if(marble>=10000000000) {cn[15]=(int)(marble/10000000000); marble%=10000000000;} // 100억
					if(marble>=1000000000) {cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
					if(marble>=500000000) {cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
					if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
					if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
				}
				
				if(marble>=1000000) {cn[10]=(int)(marble/1000000); marble%=1000000;} //100 만
				if(marble>=100000) {cn[9]=(int)(marble/100000); marble%=100000;}
				if(marble>=10000) {cn[8]=(int)(marble/10000); marble%=10000;}
				if(marble>=5000) {cn[7]=(int)(marble/5000); marble%=5000;}
				if(marble>=1000) {cn[6]=(int)(marble/1000); marble%=1000;}
				if(marble>=500) {cn[5]=(int)(marble/500); marble%=500;}
				if(marble>=100) {cn[4]=(int)(marble/100); marble%=100;}
				if(marble>=50) {cn[3]=(int)(marble/50); marble%=50;}
				if(marble>=10) {cn[2]=(int)(marble/10); marble%=10;}
				if(marble>=5) {cn[1]=(int)(marble/5); marble%=5;}
				if(marble>=1) {cn[0]=(int)(marble/1); marble%=1;}
				int nowchip=0;
				for(int k=0; k<MAX_CHIPCNT; k++) {
					if(ChipCnt[k]+cn[k] > 125) ChipCnt[k]=125;
					else ChipCnt[k] += cn[k];
				}
			}
		}
		
	}
	else
	{	
		INT64 marble = RealBet;//(int)g_Poker.nTotBat;
		int cn[MAX_CHIPCNT]={0,};//7
		if(g_RI.Level == 2) //중수
		{
			if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
			if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
		}
		else if(g_RI.Level == 3) //고수
		{
			if(marble>=10000000000) {cn[15]=(int)(marble/10000000000); marble%=10000000000;} // 100억
			if(marble>=1000000000) {cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
			if(marble>=500000000) {cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
			if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
			if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
		}
		else if(g_RI.Level == 4) // 도신
		{
			//########################################
			// 추가 예정
			if(marble>=100000000000) {cn[16]=(int)(marble/100000000000); marble%=100000000000;} // 1000억
			if(marble>=10000000000) {cn[15]=(int)(marble/10000000000); marble%=10000000000;} // 100억
			if(marble>=1000000000) {cn[14]=(int)(marble/1000000000); marble%=1000000000;} // 10억
			if(marble>=500000000) {cn[13]=(int)(marble/500000000); marble%=500000000;} // 5억
			if(marble>=100000000) {cn[12]=(int)(marble/100000000); marble%=100000000;} // 1억
			if(marble>=10000000) {cn[11]=(int)(marble/10000000); marble%=10000000;} // 1000만
		}
		if(marble>=1000000) {cn[10]=(int)(marble/1000000); marble%=1000000;} //100 만
		if(marble>=100000) {cn[9]=(int)(marble/100000); marble%=100000;}
		if(marble>=10000) {cn[8]=(int)(marble/10000); marble%=10000;}
		if(marble>=5000) {cn[7]=(int)(marble/5000); marble%=5000;}
		if(marble>=1000) {cn[6]=(int)(marble/1000); marble%=1000;}
		if(marble>=500) {cn[5]=(int)(marble/500); marble%=500;}
		if(marble>=100) {cn[4]=(int)(marble/100); marble%=100;}
		if(marble>=50) {cn[3]=(int)(marble/50); marble%=50;}
		if(marble>=10) {cn[2]=(int)(marble/10); marble%=10;}
		if(marble>=5) {cn[1]=(int)(marble/5); marble%=5;}
		if(marble>=1) {cn[0]=(int)(marble/1); marble%=1;}
		
		int nowchip=0;
		for(int k=0; k<MAX_CHIPCNT; k++)
		{
			if(ChipCnt[k]+cn[k] > 125) ChipCnt[k]=125;
			else ChipCnt[k] += cn[k];
		}
	}
	// 이미 던져진 칩들을 그려준다
	PreDrawCastedChip(ChipCnt);
}




////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// ### [ 관전기능 ] ###       ==============> 시작
void CGame::Accept_CreateRoom(CSV_ACCEPT_CREATEROOM *pMsg) // 방만들기 허가
{
	if(g_Where!=WH_LOBY) return;
	
	g_RI.GameKind = pMsg->RI->GameKind;
	g_pGameView->all_change_initpos(pMsg->RI->bPlayer);// 5,7 인용
	// 초기화
	Game.ResetGame();

	g_Wait = FALSE;
	g_pLobyDlg->iAccessRoomState=0;
	
	// 게임 정보 구조체를 초기화 한다
	for(int i=1; i<g_Max_Player; i++) Play[i].Clear();
	Play[0].JoinState = 0;
	// ### [ 중복된 IP가 ] ###
	Play[0].MyIpCheck = -1;
	
	memcpy(&g_RI, pMsg->RI, sizeof(ROOMINFO));
	
	Game.No1PosPNum = 0;
	Game.MyPosPNum = 0;
	
	// 서버측 플레이어 번호 지정
	for(i=0; i<g_Max_Player; i++)	{ Play[i].ServPNum = i; }
	
	// ### [ 관전기능 ] ###
	g_MyInfo.JoinState = 0;
	g_MyInfo.MyIpCheck = -1;
	g_MyInfo.ServPNum = Play[0].ServPNum;
	
	g_MyPN = g_MastPN = 0;	// 내가 방장
	g_Where = WH_GAME;
	Play[0].PrevMoney = Play[0].UI.PMoney;// +- 증가액 표시
	
	// 게임 창
	g_pMainDlg->ChangeView(3);
	Game.CurPlayer = 0;// 방처음 개설자가 선
	
	// 캐릭터창 초기화
	if(g_RI.MaxUserNum > g_Max_Player) g_RI.MaxUserNum=g_Max_Player;
	if(g_RI.MaxUserNum < 4) g_RI.MaxUserNum=4;
	
	
	CString str;
	str.Format("## <%s>님 입장 ##\n", Play[0].UI.ID);
	g_pGameDlg->m_GameView.ChatBox.AddText(&str,  INOUT_COLOR);
	// 버튼 활성화
	g_pGameView->ExitBtn.Enable(TRUE);
	// ### [ 관전기능 ] ###
	g_pGameView->SetObserverBtn();
}


// 방에 입장 허가 받음
void CGame::Accept_EnterRoom(CSV_ACCEPT_ENTERROOM *pMsg) 
{
	char *strMast = pMsg->EI->Ri.ID;
	g_RI.GameKind = pMsg->EI->Ri.GameKind;
	g_pGameView->all_change_initpos(pMsg->EI->Ri.bPlayer);// 5,7 인용

	g_MyPN = 0;
	int MyID=-1, MastID=-1;
	int i, k;

	for(i=0; i<g_Max_NewPlayer; i++)Play[i].m_bSelectCard = 0;

	// ### [ 관전기능 ] ###
	for(i=0; i<g_Max_NewPlayer; i++) {
		if(strlen(pMsg->EI->Ui[i].ID) > 0) {
			if(strcmp(strMast, pMsg->EI->Ui[i].ID)==0){
				MastID = i; // 방장 아이디 찾기
				g_pGameView->MasterBtn.Enable(TRUE); // 방장옵션 비활성 여부 [62]
			}
			else 
			{
				g_pGameView->MasterBtn.Enable(FALSE);
			}
			if(strcmp(g_MyInfo.UI.ID, pMsg->EI->Ui[i].ID)==0) MyID = i; // 나의 아이디
		}
	}
	g_Wait = FALSE;
	g_pLobyDlg->iAccessRoomState=0;
	
	// 방장 혹은 나의 아이디를 찾을 수 없거나 방장과 내 아이디가 동일하다?
	if(MastID == -1 || MyID == -1 || MastID == MyID) {
		// ### [ 관전기능 ] ###
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, g_MyInfo.UI.UNum, g_MyInfo.UI.ID);
		SockMan.SendData(g_MainSrvSID, aormsg.pData, aormsg.GetTotalSize());
		// 연결종료
		g_MastPN = 0;
		g_pLobyDlg->m_MsgDlg.OnCancel();
		return;
	}

	if(pMsg->EI->Ui[MyID].cObserver == 1)  { g_MyObserver = TRUE; }
	Game.ResetGame();
	g_MastPN = MastID;
	
	// 방 정보 복사
	memcpy(&g_RI, &pMsg->EI->Ri, sizeof(ROOMINFO));
		
	if(g_RI.MaxUserNum > g_Max_Player) g_RI.MaxUserNum=g_Max_Player;
	if(g_RI.bPlayer==0) {
		if(g_RI.MaxUserNum < 3) g_RI.MaxUserNum=3;//3,4,5
	}
	else {
		if(g_RI.MaxUserNum < 4) g_RI.MaxUserNum=4;//4,5,6
	}


	
	// 접속 메세지 대화창 닫기
	g_pLobyDlg->m_MsgDlg.OnOK();
	
	// 게임화면으로 변경
	g_Where = WH_GAME;
	g_pMainDlg->ChangeView(3);
	
	memcpy(&g_Poker, &pMsg->EI->Pg,sizeof(g_Poker));


	if(g_RI.ChangeCardStep > 0 && g_Poker.nState == RSTATE_CHANGECARD)
	{
		g_bChangeCardStage = TRUE;
	}
	
	if( !g_MyObserver ) // 관전이 아니라면
	{
		Play[0].JoinState = 0;
		g_MyInfo.JoinState = 0;
		Game.MyPosPNum = MyID; // 서버쪽에 인덱스
		Game.No1PosPNum = Game.GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
		
		// 게임 정보 초기화
		for(i=1; i<g_Max_NewPlayer; i++) Play[i].Clear();
		// 서버측 플레이어 번호 지정
		for(i=0; i<g_Max_NewPlayer; i++) {
			if(i<g_Max_Player)
				Play[i].ServPNum = Game.GetServPNum_ByPN(i);
			else
				Play[i].ServPNum = i;
		}
		
		g_MyInfo.ServPNum = Play[0].ServPNum;
		
		// 플레이어 순서에 맞게 초기화한다
		for(i=0; i<g_Max_NewPlayer; i++) {
			if(i>=g_Max_Player) { //관전자 아디셋팅
				if(strlen(pMsg->EI->Ui[i].ID) > 0) {
					Play[i].SetNewPlayer(&pMsg->EI->Ui[i]);
					if(pMsg->EI->Ui[i].cAdminUser == 0) {
						// 관전자 리스트 출력
						int ounum = Play[i].UI.UNum;
						int sex  = Play[i].UI.Sex;
						int level = Play[i].UI.nIcon;
						INT64 Money = Play[i].UI.PMoney;
						int seq = 1;
						g_pGameDlg->m_ObserverDlg.User_InsItem(ounum, Play[i].UI.ID, sex, level, Money, seq);
					}
				}
				continue;
			}
			int pn = Game.GetPNum_ByServPN(i);
			if(strlen(pMsg->EI->Ui[i].ID) > 0) {
				if(pn!=0) Play[pn].SetNewPlayer(&pMsg->EI->Ui[i]);
				
				Play[pn].JoinState = pMsg->EI->Pg.PS[i].JoinState;
				Play[pn].PlayState = pMsg->EI->Pg.PS[i].PlayState;
				Play[pn].MyIpCheck = pMsg->EI->Pg.PS[i].nCheckIp;   // ### [ 중복된 IP가 ] ###
				Play[pn].nSndFxKind = pMsg->EI->Pg.PS[i].nSndFxKind; // ### [사운드 추가 작업] ###
				Play[pn].UI.nIcon = GetPokerLevel(pMsg->EI->Ui[i].PMoney);
				Play[pn].PrevMoney = pMsg->EI->Ui[i].PMoney; // +- 증가액 표시
				
				// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
				if(Play[pn].JoinState==1) {
					int nTotCnt = pMsg->EI->Pg.PS[i].nCardTotal;
					int nOpCnt =  pMsg->EI->Pg.PS[i].nOpenTotal;
					Play[pn].bFold = pMsg->EI->Pg.PS[i].bFold;
					Play[pn].nCardTotal = nTotCnt;
					Play[pn].nOpenTotal = nOpCnt;
					for(k = 0; k < nTotCnt; k++) {
						Game.CardDeck[pn].NewCardSet(DUMY_CARD); // 카운트 자동증가
						CPoint pt;
						pt.x = Game.CardDeck[pn].Card[k].Xp;
						pt.y = Game.CardDeck[pn].Card[k].Yp;
						Game.CardDeck[pn].Card[k].m_move.SetCurPos(pt);
					}
					
					if(g_Poker.nState > RSTATE_PREPARESTART && nOpCnt > 0 && !Play[pn].bFold) {
						if(nOpCnt <=  4) {							
							if(g_RI.ChangeCardStep == 2)//[62
							{
								if(g_Poker.PS[i].nCard[3] < 0 && nOpCnt == 4)//히든 카드가 2장이란 소리 0 히든 5 히든
								{
									Game.CardDeck[pn].Card[1].CardNo = g_Poker.BC[i].nCard[1];
									Game.CardDeck[pn].SetFaceUp(1);

									for(k = 0; k < nOpCnt; k++)	{
										Game.CardDeck[pn].Card[k+2].CardNo = g_Poker.PS[i].nCard[k]; // 값 변경

										if(Game.CardDeck[pn].Card[k+2].CardNo >= 0 &&
											Game.CardDeck[pn].Card[k+2].CardNo < 52){
											Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
										}
										
										CPoint pt;
										pt.x = Game.CardDeck[pn].Card[k+2].Xp;
										pt.y = Game.CardDeck[pn].Card[k+2].Yp;
										Game.CardDeck[pn].Card[k+2].m_move.SetCurPos(pt);
									}
									Game.CardDeck[pn].Card[5].CardNo = 52;									
								}
								else if(g_Poker.PS[i].nCard[3] < 0 && nOpCnt == 3)//히든 카드가 3장이란 소리
								{
									Game.CardDeck[pn].Card[0].CardNo = 52;
									Game.CardDeck[pn].Card[1].CardNo = 52;

									for(k = 0; k < nOpCnt; k++)	{
										Game.CardDeck[pn].Card[k+2].CardNo = g_Poker.PS[i].nCard[k]; // 값 변경

										if(Game.CardDeck[pn].Card[k+2].CardNo >= 0 &&
											Game.CardDeck[pn].Card[k+2].CardNo < 52){
											Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
										}
										
										CPoint pt;
										pt.x = Game.CardDeck[pn].Card[k+2].Xp;
										pt.y = Game.CardDeck[pn].Card[k+2].Yp;
										Game.CardDeck[pn].Card[k+2].m_move.SetCurPos(pt);
									}
									Game.CardDeck[pn].Card[5].CardNo = 52;									
								}
								else if(g_Poker.PS[i].nCard[3] >= 0 && nOpCnt == 4)//아무것도 바꾸지 않았을때 기본 0,1  히든
								{
									for(k = 0; k < nOpCnt; k++)	{
										Game.CardDeck[pn].Card[k+2].CardNo = g_Poker.PS[i].nCard[k]; // 값 변경
										Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
										
										CPoint pt;
										pt.x = Game.CardDeck[pn].Card[k+2].Xp;
										pt.y = Game.CardDeck[pn].Card[k+2].Yp;
										Game.CardDeck[pn].Card[k+2].m_move.SetCurPos(pt);
									}
								}
							}
							else if(g_RI.ChangeCardStep < 2)
							{
								for(k = 0; k < nOpCnt; k++)	{
									Game.CardDeck[pn].Card[k+2].CardNo = g_Poker.PS[i].nCard[k]; // 값 변경
									Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
									
									CPoint pt;
									pt.x = Game.CardDeck[pn].Card[k+2].Xp;
									pt.y = Game.CardDeck[pn].Card[k+2].Yp;
									Game.CardDeck[pn].Card[k+2].m_move.SetCurPos(pt);
								}
							}
						}
						else {// 모두 카드 오픈
							// 히든 카드
							Game.CardDeck[pn].Card[0].CardNo = g_Poker.BC[i].nCard[0];
							Game.CardDeck[pn].Card[1].CardNo = g_Poker.BC[i].nCard[1];											
							
							for( k = 0; k < nOpCnt; k++)	{
								Game.CardDeck[pn].Card[k+2].CardNo = g_Poker.PS[i].nCard[k]; // 값 변경
								Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
							}

							Game.CardDeck[pn].Card[5].CardNo = g_Poker.BC[i].nCard[2];//[62
							Game.CardDeck[pn].SetFaceUp(0);
							Game.CardDeck[pn].SetFaceUp(1);
							Game.CardDeck[pn].SetFaceUp(5);
							// 위치셋
							for(k = 0; k < 6; k++)	{
								CPoint pt;
								pt.x = Game.CardDeck[pn].Card[k].Xp;
								pt.y = Game.CardDeck[pn].Card[k].Yp;
								Game.CardDeck[pn].Card[k].m_move.SetCurPos(pt);
							}
						}
						
					}
				}
			}
		}
		// 버튼 상태 설정
		Game.CurPlayer = g_Poker.CurPlayer;
		g_pGameView->SetBtnState();
		
		CString str;
		str.Format(g_StrMan.Get(_T("ENTER_ROOM")), g_MyInfo.UI.ID);
		g_pGameDlg->m_GameView.ChatBox.AddText(&str, INOUT_COLOR);
		
	}
	else //관전
	{
		// 서버 클라이언트 1 : 1
		g_MyIndex = MyID;
		Game.MyPosPNum = 0;						    // 서버쪽에 인덱스
		Game.No1PosPNum = Game.GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
		for(i=0; i<g_Max_NewPlayer; i++) {
			Play[i].Clear(); // 게임 정보 초기화
			Play[i].ServPNum = i;
		}
		g_MyInfo.ServPNum = MyID;
		
		// 플레이어 순서에 맞게 초기화한다
		for(i=0; i<g_Max_NewPlayer; i++) {
			int pn = i;
			if(strlen(pMsg->EI->Ui[i].ID) > 0)	{
				Play[pn].SetNewPlayer(&pMsg->EI->Ui[i]);
				if(i>=g_Max_Player) // 관전자 리스트 출력
				{
					if(pMsg->EI->Ui[i].cAdminUser == 0) 
					{
						int ounum = Play[i].UI.UNum;
						int sex  = Play[i].UI.Sex;
						int level = Play[i].UI.nIcon;
						INT64 Money = Play[i].UI.PMoney;
						int seq = 1;
						g_pGameDlg->m_ObserverDlg.User_InsItem(ounum, Play[i].UI.ID, sex, level,Money, seq);
					}
					Play[pn].UI.nIcon = GetPokerLevel(pMsg->EI->Ui[i].PMoney);
					Play[pn].PrevMoney = pMsg->EI->Ui[i].PMoney; // +- 증가액 표시
					continue;
				}
				
				Play[pn].JoinState = pMsg->EI->Pg.PS[i].JoinState;
				Play[pn].PlayState = pMsg->EI->Pg.PS[i].PlayState;
				Play[pn].MyIpCheck = pMsg->EI->Pg.PS[i].nCheckIp; // ### [ 중복된 IP가 ] ###
				Play[pn].nSndFxKind = pMsg->EI->Pg.PS[i].nSndFxKind;// ### [사운드 추가 작업] ###
				Play[pn].UI.nIcon = GetPokerLevel(pMsg->EI->Ui[i].PMoney);
				Play[pn].PrevMoney = pMsg->EI->Ui[i].PMoney; // +- 증가액 표시
				
				
				// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
				if(Play[pn].JoinState==1)
				{
					int nTotCnt = pMsg->EI->Pg.PS[i].nCardTotal;
					int nOpCnt =  pMsg->EI->Pg.PS[i].nOpenTotal;
					
					Play[pn].bFold = pMsg->EI->Pg.PS[i].bFold;
					Play[pn].nCardTotal = nTotCnt;
					Play[pn].nOpenTotal = nOpCnt;
					for(k = 0; k < nTotCnt; k++)
					{
						Game.CardDeck[pn].NewCardSet(DUMY_CARD); // 카운트 자동증가
						CPoint pt;
						pt.x = Game.CardDeck[pn].Card[k].Xp;
						pt.y = Game.CardDeck[pn].Card[k].Yp;
						Game.CardDeck[pn].Card[k].m_move.SetCurPos(pt);
					}
					
					if(g_Poker.nState > RSTATE_PREPARESTART && nOpCnt > 0 && !Play[pn].bFold)
					{
						if(nOpCnt <=  4)
						{
							if(g_RI.ChangeCardStep == 2)//[62
							{
								if(g_Poker.PS[i].nCard[3] < 0 && nOpCnt == 4)//히든 카드가 2장이란 소리
								{
									Game.CardDeck[pn].Card[1].CardNo = g_Poker.BC[i].nCard[1];
									Game.CardDeck[pn].SetFaceUp(1);

									for(k = 0; k < nOpCnt; k++)	{
										Game.CardDeck[pn].Card[k+2].CardNo = g_Poker.PS[i].nCard[k]; // 값 변경

										if(Game.CardDeck[pn].Card[k+2].CardNo >= 0 &&
											Game.CardDeck[pn].Card[k+2].CardNo < 52){
											Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
										}
										
										CPoint pt;
										pt.x = Game.CardDeck[pn].Card[k+2].Xp;
										pt.y = Game.CardDeck[pn].Card[k+2].Yp;
										Game.CardDeck[pn].Card[k+2].m_move.SetCurPos(pt);
									}
									Game.CardDeck[pn].Card[5].CardNo = 52;									
								}
								else if(g_Poker.PS[i].nCard[3] < 0 && nOpCnt == 3)//히든 카드가 2장이란 소리
								{
									Game.CardDeck[pn].Card[0].CardNo = 52;
									Game.CardDeck[pn].Card[1].CardNo = 52;

									for(k = 0; k < nOpCnt; k++)	{
										Game.CardDeck[pn].Card[k+2].CardNo = g_Poker.PS[i].nCard[k]; // 값 변경

										if(Game.CardDeck[pn].Card[k+2].CardNo >= 0 &&
											Game.CardDeck[pn].Card[k+2].CardNo < 52){
											Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
										}
										
										CPoint pt;
										pt.x = Game.CardDeck[pn].Card[k+2].Xp;
										pt.y = Game.CardDeck[pn].Card[k+2].Yp;
										Game.CardDeck[pn].Card[k+2].m_move.SetCurPos(pt);
									}
									Game.CardDeck[pn].Card[5].CardNo = 52;									
								}
								else if(g_Poker.PS[i].nCard[3] >= 0 && nOpCnt == 4)//아무것도 바꾸지 않았을때 기본 0,1  히든
								{
									for(k = 0; k < nOpCnt; k++)	{
										Game.CardDeck[pn].Card[k+2].CardNo = g_Poker.PS[i].nCard[k]; // 값 변경
										Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
										
										CPoint pt;
										pt.x = Game.CardDeck[pn].Card[k+2].Xp;
										pt.y = Game.CardDeck[pn].Card[k+2].Yp;
										Game.CardDeck[pn].Card[k+2].m_move.SetCurPos(pt);
									}
								}
							}
							else if(g_RI.ChangeCardStep < 2)
							{
								for(k = 0; k < nOpCnt; k++)	{
									Game.CardDeck[pn].Card[k+2].CardNo = g_Poker.PS[i].nCard[k]; // 값 변경
									Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
									
									CPoint pt;
									pt.x = Game.CardDeck[pn].Card[k+2].Xp;
									pt.y = Game.CardDeck[pn].Card[k+2].Yp;
									Game.CardDeck[pn].Card[k+2].m_move.SetCurPos(pt);
								}
							}							
						}
						else // 모두 카드 오픈
						{
							Game.CardDeck[pn].Card[0].CardNo = g_Poker.BC[i].nCard[0];
							Game.CardDeck[pn].Card[1].CardNo = g_Poker.BC[i].nCard[1];											
							
							for( k = 0; k < nOpCnt; k++)	{
								Game.CardDeck[pn].Card[k+2].CardNo = g_Poker.PS[i].nCard[k]; // 값 변경
								Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
							}

							Game.CardDeck[pn].Card[5].CardNo = g_Poker.BC[i].nCard[2];//[62
							Game.CardDeck[pn].SetFaceUp(0);
							Game.CardDeck[pn].SetFaceUp(1);
							Game.CardDeck[pn].SetFaceUp(5);
							// 위치셋
							for(k = 0; k < 6; k++)	{
								CPoint pt;
								pt.x = Game.CardDeck[pn].Card[k].Xp;
								pt.y = Game.CardDeck[pn].Card[k].Yp;
								Game.CardDeck[pn].Card[k].m_move.SetCurPos(pt);
							}
						}						
					}					
				}
			}
		}
		
		// 버튼 상태 설정
		Game.CurPlayer = g_Poker.CurPlayer;
		g_pGameView->SetBtnState();
		
		CString str;
		str.Format("## <%s>님 입장[관전중]##\n", g_MyInfo.UI.ID);
		g_pGameDlg->m_GameView.ChatBox.AddText(&str, OBINOUT_COLOR);
		
	}
	
	g_pGameView->SetObserverBtn();// ### [ 관전기능 ] ###
	Game.ChipFunc(pMsg->EI->Ui);//칩 그리기

	
}


// 유저가 방에 입장 알림
void CGame::User_EnterRoom(CSV_USERENTERROOM *pMsg)
{
	if(g_Where != WH_GAME) return;
	int i=0;
	int pnum = -1; // 플레이어 번호
	
	if( !g_MyObserver && pMsg->UI->cObserver!=1 ) // 1) 내가 겜참여중이고 겜참여 넘 입장
	{
		for(i=Game.No1PosPNum; i<g_Max_Player+Game.No1PosPNum; i++) {// 서버쪽의 0번인덱스 클라이언트 인덱스 부터
			int index = i;
			if(index>=g_Max_Player) index = index - g_Max_Player;
			if(index==0) continue; //나
			if(strlen(Play[index].UI.ID)==0){pnum=index; break;}
		}
		if(pnum == -1) return; // 비어있는 곳을 찾지 못했으면
		
		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_RI.NowUserNum += 1;
		Play[pnum].SetNewPlayer(pMsg->UI);
		PlayScr[pnum].Reset();
		
		Play[pnum].ServPNum = Game.GetServPNum_ByPN(pnum);	// 서버측 플레이어 번호 저장
		Play[pnum].PrevMoney = Play[pnum].UI.PMoney;		// +- 증가액 표시
		
		CString str;
		str.Format(g_StrMan.Get(_T("ENTER_ROOM")), pMsg->UI->ID);
		g_pGameDlg->m_GameView.ChatBox.AddText(&str, INOUT_COLOR);
	}
	//2) 내가 겜중이고 관전인넘 입장
	//else if(!g_MyObserver) 
	//{
	//}
	// 3)내가 관전중이고  겜참여 하는넘 입장
	else if(pMsg->UI->cObserver!=1)
	{
		for(i=0; i<g_Max_Player; i++) {
			if(strlen(Play[i].UI.ID)==0){pnum=i; break;}
		}
		if(pnum == -1) return; // 비어있는 곳을 찾지 못했으면
		
		Game.MyPosPNum = 0; // 서버쪽에 인덱스
		Game.No1PosPNum = Game.GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
		
		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_RI.NowUserNum += 1;
		Play[pnum].SetNewPlayer(pMsg->UI);
		PlayScr[pnum].Reset();
		Play[pnum].ServPNum = Game.GetServPNum_ByPN(pnum);// 서버측 플레이어 번호 저장
		Play[pnum].PrevMoney = Play[pnum].UI.PMoney;	  // +- 증가액 표시
		
		CString str;
		str.Format(g_StrMan.Get(_T("ENTER_ROOM")), pMsg->UI->ID);
		g_pGameDlg->m_GameView.ChatBox.AddText(&str, INOUT_COLOR);
		
	}
	// 2)내가 겜중이고 관전인넘 입장
	// 4)내가 관전 이고 관전인넘 입장
	else
	{
		if(pMsg->UI->cAdminUser == 1) {
			pnum = g_Max_NewPlayer-1;//관리자
		}
		else {
			for(i=g_Max_Player; i<g_Max_NewPlayer-1; i++) {
				if(strlen(Play[i].UI.ID)==0){pnum=i; break;}
			}
		}
		if(pnum == -1) return;			

		Play[pnum].SetNewPlayer(pMsg->UI);// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		Play[pnum].ServPNum = pnum;		// 서버측 플레이어 번호 저장
		Play[pnum].PrevMoney = Play[pnum].UI.PMoney;// +- 증가액 표시
		
		if( pMsg->UI->cAdminUser==0) {// 관리자가 아니면 
			g_RI.NowObserverNum += 1;
			CString str;
			str.Format("## <%s>님 입장[관전중] ##\n", pMsg->UI->ID);
			g_pGameDlg->m_GameView.ChatBox.AddText(&str, OBINOUT_COLOR);
			
			// 관전자 리스트 출력
			int ounum = Play[pnum].UI.UNum;
			int sex  = Play[pnum].UI.Sex;
			int level = Play[pnum].UI.nIcon;
			INT64 Money = Play[pnum].UI.PMoney;
			int seq = 1;
			g_pGameDlg->m_ObserverDlg.User_InsItem(ounum, Play[pnum].UI.ID, sex, level, Money, seq);
		}
	}
	
	// ### [ 중복된 IP가 ] ###
	for(i=0; i<g_Max_Player; i++) {
		int pn = Game.GetPNum_ByServPN(i);
		if(strlen(Play[pn].UI.ID) > 0) {
			Play[pn].MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}
	Play[pnum].nSndFxKind = *pMsg->SndFxKind;// ### [사운드 추가 작업] ###
	
	
	// 시작버튼 활성화 
//	if(strcmp(g_RI.ID,g_MyInfo.UI.ID)==0 && g_RI.UNum == g_MyInfo.UI.UNum 
//		&& g_RI.NowUserNum < 5 && g_RI.NowUserNum >2 && g_RI.State == 0){		
//			g_pGameView->GameStartBtn.Show(TRUE);				
//	}
}

void CGame::User_OutRoom(CSV_USEROUTROOM *pMsg) // 유저가 방에서 나감 알림
{
	if(g_Where != WH_GAME) return;
	
	int pnum=-1;
	for(int i=0; i<g_Max_NewPlayer; i++) {
		if(strcmp(Play[i].UI.ID, pMsg->ID)==0) {pnum=i; break;}
	}
	if(pnum == -1) return;
	
	if( pnum<g_Max_Player ) //겜중인넘
	{
		// [ 카드 날리기 ]
		if(Play[pnum].bFold == FALSE && Play[pnum].JoinState == 1 && Play[pnum].PlayState == 1)
		{	//현재 겜중인 넘 나갔다.
			Game.StartUserNum--;
			if(Game.StartUserNum <0) Game.StartUserNum = 0;
		}
		
		Play[pnum].Clear(); // UI 최기화
		PlayScr[pnum].Reset();
		// ##### 카드 정보 클리어 ##### 
		Game.CardDeck[pnum].Reset(); // 이벤트를 알수 있다면 클리어 하지 않는다.
		
		if(g_RI.NowUserNum > 0) g_RI.NowUserNum -= 1;
		
		CString str;
		str.Format(g_StrMan.Get(_T("OUT_ROOM")), pMsg->ID);
		g_pGameDlg->m_GameView.ChatBox.AddText(&str, INOUT_COLOR);
		
	}
	else // 관전인넘
	{
		if(Play[pnum].UI.cAdminUser == 0) {
			g_pGameDlg->m_ObserverDlg.User_DelItem(Play[pnum].UI.UNum, Play[pnum].UI.ID);
			
			if(g_RI.NowObserverNum > 0) g_RI.NowObserverNum -= 1;
			CString str;
			str.Format("## <%s>님 퇴장[관전중] ##\n", pMsg->ID);
			g_pGameDlg->m_GameView.ChatBox.AddText(&str, OBINOUT_COLOR);
		}
		Play[pnum].Clear(); // UI 최기화
	}
	// ### [ 중복된 IP가 ] ###
	for(i=0; i<g_Max_Player; i++)	{
		int pn = Game.GetPNum_ByServPN(i);
		if(strlen(Play[pn].UI.ID) > 0) {
			Play[pn].MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}

	//시작버튼 
	if(strcmp(g_RI.ID,g_MyInfo.UI.ID)==0 && g_RI.UNum == g_MyInfo.UI.UNum 
		&& g_RI.NowUserNum <3 && g_RI.State == 0 ){
		g_pGameView->GameStartBtn.Show(FALSE);		
	}
	/*
	if(g_RI.NowUserNum <3 && g_RI.State == 0)
	{
		CString str;
		str.Format("[알림] 게임 시작 카운트가 중지되었습니다.");
		g_pGameDlg->m_GameView.ChatBox.AddText(&str, OBINOUT_COLOR);
	}
	*/
}




// bPaticipation = TRUE  관전 -> 참여
// bPaticipation = FALSE 참여 -> 관전
void CGame::SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation )
{
	//백업용 - 포인터 사용할까?
	USERINFO tmpUi[MAX_PLAYER] = {0,};
	PLAYSTATE tmpPs[MAX_PLAYER] = { 0,};
	BACKCARD  tmpBC[MAX_PLAYER] = {0,};


	struct tempcarddeck{
		char nCard[6];
	};

	int       tmpSelectCard[MAX_PLAYER] = {0,};

	tempcarddeck tCardDeck[MAX_PLAYER] = {0,};
	
	if( !bParticipation ) {
		//현재 겜참여 중이기 때문에 0 번 인덱스
		//기본 정보 초기화
		Play[0].Clear();
	}

	// 기존 플레이 정보 backup
	int pn = 0, k;
	for(int i=0; i<g_Max_Player; i++) {
		if(strlen(Play[i].UI.ID)==0) continue;
		if(bParticipation )// 관전 -> 참여
			pn = i;
		else
			pn = Game.GetServPNum_ByPN(i);
		memcpy(&tmpUi[pn], &Play[i].UI, sizeof(USERINFO));
		tmpPs[pn].JoinState = Play[i].JoinState;
		tmpPs[pn].PlayState = Play[i].PlayState;
			
		tmpPs[pn].bFold = Play[i].bFold;
		tmpPs[pn].nCardTotal= Play[i].nCardTotal;
		tmpPs[pn].nOpenTotal= Play[i].nOpenTotal;
		
		tmpSelectCard[pn] = Play[i].m_bSelectCard;

		//서버로부터 다시 받는다
		// ### [ 중복된 IP가 ] ###
		//tmpPs[pn].nCheckIp = Play[i].MyIpCheck;
		// ### [사운드 추가 작업] ###
		tmpPs[pn].nSndFxKind = Play[i].nSndFxKind;

		for(k = 0; k < 6 ; k++) {
			tCardDeck[pn].nCard[k] = Game.CardDeck[i].Card[k].CardNo;
		}			
		/*
		for(k = 0; k<Play[i].nOpenTotal; k++) {
			tmpPs[pn].nCard[k] = Game.CardDeck[i].Card[k+2].CardNo;
		}	
		
		tmpBC[pn].nCard[0] = Game.CardDeck[i].Card[0].CardNo;
		//히든카드 일때만 넣는다.
	//	if(Game.CardDeck[i].Card[1].CardNo == 52)
		tmpBC[pn].nCard[1] = Game.CardDeck[i].Card[1].CardNo;		
	//	if(Game.CardDeck[i].Card[5].CardNo == 52)
		tmpBC[pn].nCard[2] = Game.CardDeck[i].Card[5].CardNo;//[62]
		*/
	}

	//기본 정보 초기화
	if(bParticipation) {   // 관전 -> 참여
		g_MyInfo.JoinState = 0;
		Game.MyPosPNum = newpnum; // 서버쪽에 새로운 인덱스
		Game.No1PosPNum = Game.GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		Play[nowpnum].Clear();
	}
	else {	//참여 -> 관전
		g_MyInfo.JoinState = 0;
		Game.MyPosPNum = 0; // 서버쪽에 새로운 인덱스
		Game.No1PosPNum = Game.GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
	}
	
	// 게임 정보 모두 초기화
	for(i=0; i<g_Max_Player; i++) {
		Play[i].Clear();
		PlayScr[i].Reset();
		Game.CardDeck[i].Reset(); 
		Play[i].ServPNum = Game.GetServPNum_ByPN(i); // 초기화 되지 않는다.
	}
	
	
	if(bParticipation ) {	// 관전 -> 참여
		g_MyInfo.UI.cObserver = 0;
		g_MyObserver = FALSE;
		g_MyInfo.Reset();
		Play[0].SetNewPlayer(&g_MyInfo.UI);
		Play[0].nSndFxKind = g_MyInfo.nSndFxKind;
					
		g_RI.NowUserNum++;
		if(g_RI.NowObserverNum > 0) g_RI.NowObserverNum--;
		
		g_MyInfo.ServPNum = Play[0].ServPNum;
		Play[0].PrevMoney = Play[0].UI.PMoney;// +- 증가액 표시
		
	}
	else {   //참여 -> 관전
		g_MyInfo.UI.cObserver = 1;
		g_MyObserver = TRUE;
		g_MyInfo.Reset();
		Play[newpnum].SetNewPlayer(&g_MyInfo.UI);
		
		g_MyInfo.ServPNum = newpnum;
		Play[newpnum].ServPNum = newpnum;

		g_RI.NowObserverNum++;
		if(g_RI.NowUserNum > 0) g_RI.NowUserNum--;
	}
	
	// 플레이어 순서에 맞게 초기화한다
	for( i=0; i<g_Max_Player; i++) {
		int pn = Game.GetPNum_ByServPN(i);
		if(strlen(tmpUi[i].ID) > 0)	{
			if(bParticipation ) {
				if(pn!=0) Play[pn].SetNewPlayer(&tmpUi[i]);
			}
			else Play[pn].SetNewPlayer(&tmpUi[i]);
			
			Play[pn].JoinState = tmpPs[i].JoinState;
			Play[pn].PlayState = tmpPs[i].PlayState;
			//Play[pn].MyIpCheck = tmpPs[i].nCheckIp;  // ### [ 중복된 IP가 ] ###
			Play[pn].nSndFxKind = tmpPs[i].nSndFxKind; // ### [사운드 추가 작업] ###

			Play[pn].m_bSelectCard  = tmpSelectCard[i];

			// 레벨상점 폐쇄
			Play[pn].UI.nIcon = GetPokerLevel(tmpUi[i].PMoney);
			Play[pn].PrevMoney = tmpUi[i].PMoney;// +- 증가액 표시
			
			// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
			if(Play[pn].JoinState==1) {
				int nTotCnt = tmpPs[i].nCardTotal;
				int nOpCnt =  tmpPs[i].nOpenTotal;
				
				Play[pn].bFold = tmpPs[i].bFold;
				Play[pn].nCardTotal = nTotCnt;
				Play[pn].nOpenTotal = nOpCnt;
				for(k = 0; k < nTotCnt; k++) {
					Game.CardDeck[pn].NewCardSet(DUMY_CARD); // 카운트 자동증가
					CPoint pt;
					pt.x = Game.CardDeck[pn].Card[k].Xp;
					pt.y = Game.CardDeck[pn].Card[k].Yp;
					Game.CardDeck[pn].Card[k].m_move.SetCurPos(pt);
				}
				
				if(g_Poker.nState > RSTATE_PREPARESTART && nOpCnt > 0 && !Play[pn].bFold)	{
					if(nOpCnt <=  4) {		
						
						for(k = 0; k < 6; k++) {
							Game.CardDeck[pn].Card[k].CardNo = tCardDeck[i].nCard[k];
							if(Game.CardDeck[pn].Card[k].CardNo < 52){
								Game.CardDeck[pn].SetFaceUp(k);
							}
						}						
					}
					else // 모두 카드 오픈
					{
						// 히든 카드
						/*
						Game.CardDeck[pn].Card[0].CardNo = tmpBC[i].nCard[0];
						Game.CardDeck[pn].Card[1].CardNo = tmpBC[i].nCard[1];						
						
						for( k = 0; k < nOpCnt; k++) {
							Game.CardDeck[pn].Card[k+2].CardNo = tmpPs[i].nCard[k]; // 값 변경
							Game.CardDeck[pn].SetFaceUp(k+2); // 보이기
						}

						Game.CardDeck[pn].Card[5].CardNo = tmpBC[i].nCard[2];//[62
						Game.CardDeck[pn].SetFaceUp(0);
						Game.CardDeck[pn].SetFaceUp(1);
						Game.CardDeck[pn].SetFaceUp(5);
						*/
						for(k = 0; k < nTotCnt; k++) {
							Game.CardDeck[pn].Card[k].CardNo = tCardDeck[i].nCard[k];							
							Game.CardDeck[pn].SetFaceUp(k);							
						}						

						// 위치셋
						for(k = 0; k < 6; k++) {
							CPoint pt;
							pt.x = Game.CardDeck[pn].Card[k].Xp;
							pt.y = Game.CardDeck[pn].Card[k].Yp;
							Game.CardDeck[pn].Card[k].m_move.SetCurPos(pt);
						}
					}					
				}				
			}
		}
	}


	// bPaticipation = TRUE  관전 -> 참여
	// bPaticipation = FALSE 참여 -> 관전
	if(bParticipation) {
		g_pGameDlg->m_ObserverDlg.User_DelItem(Play[0].UI.UNum, Play[0].UI.ID);
	}
	else {
		int ounum = Play[newpnum].UI.UNum;
		int sex  = Play[newpnum].UI.Sex;
		int level = Play[newpnum].UI.nIcon;
		INT64 Money = Play[newpnum].UI.PMoney;
		int seq = 1;
		g_pGameDlg->m_ObserverDlg.User_InsItem(ounum, Play[newpnum].UI.ID, sex, level, Money, seq);
	}
	g_pGameView->SetObserverBtn();
	CString str;
	if(bParticipation) {
		str.Format("참여하기를 선택 하였습니다.\n");
		Sound.Play(SND34);

	}
	else {
		str.Format("관전하기를 선택 하였습니다.\n");
		Sound.Play(SND35);
	}
	g_pGameDlg->m_GameView.ChatBox.AddText(&str, RESERVER_COLOR);
}
// ### [ 관전기능 ] ###       ==============> 끝
////////////////////////////////////////////////////////////////////////////
//카드 바꾸기 시작 초기 [62]
void CGame::ChangeCard_Init()
{
	Game.SetBackTime();					
	g_bChangeCardStage = TRUE;
	g_Poker.nState = RSTATE_CHANGECARD;	
	Sound.Play(SND51);	
	
	Game.FlyWindCard = TRUE;
	
	for(int i = 0 ; i < g_Max_NewPlayer ; i++){
		if(Play[i].JoinState == 1 && Play[i].PlayState == 1){
			Play[i].m_bSelectCard = 1;						
			int nSNum = Play[i].ServPNum;
			g_Poker.RU[nSNum].nLastCmd = 0;
			Play[i].bChangeWhether = -1;
		}
	}
	
	if(Play[0].JoinState ==1 && Play[0].PlayState == 1 && !Play[0].bFold && !g_MyObserver){					
		g_pGameView->ChangeCard_OK.Enable(TRUE);
		g_pGameView->ChangeCard_Cancel.Enable(TRUE);
		Game.CardDeck[0].SetCardRect();		
		Play[0].m_KD_SeleteCard =-1; // 방향키 값 reset
		Game.m_nchange_card = 13000 / int(1000.0f/REFRESH_FPS);//13초
		//	Game.m_nchange_card = 600;
	}
}

// 62cut [62] // 바꾼패 처리 부분 한넘씩 보낼때 
void CGame::SetOtherChangeCard_62Cut(CHANGECARD_62CUT data)
{			
	if(data.nServerPNum < 0 || data.nServerPNum > g_Max_Player)return;

	int pnum = GetPNum_ByServPN(data.nServerPNum);

	if(pnum < 0 || pnum > g_Max_NewPlayer)return;
	if(strcmp(Play[pnum].UI.ID,data.ID) != 0 )return;	

	if(pnum == 0)m_nchange_card = 0;//카드 선택  타임 리셋
	// 바꾸기 카드를 받았당.	
	Play[pnum].m_bSelectCard = FALSE;	
	
	if(data.nCardIndex == -1 && data.nChangeCardNum == -1 && data.nCardNum == -1){
		Play[pnum].bChangeWhether = 0;
		
		if(pnum == 0 && !g_MyObserver && strcmp(Play[0].UI.ID,data.ID) == 0 )
		{				
			Game.FlyWindCard = FALSE;
			CCL_CARDCHANGE_END_62CUT cend;
			cend.Set(g_RI.RoomNum,Play[0].ServPNum,Play[0].UI.ID);
			SockMan.SendData(g_MainSrvSID,cend.pData,cend.GetTotalSize());					
		}
	}

	if(data.nCardIndex < 0 || data.nCardIndex >= 6)return;	
	if(data.nCardNum < 0  || data.nCardNum > 52)return;	
	
	CardDeck[pnum].SetCardPosition(data.nCardIndex,pnum,data.nChangeCardNum,data.nCardNum);	

	Play[pnum].bChangeWhether = 1;	
	
	if(pnum == 0)Sound.Play( SND16 );	
}

// 62cut [62] // 바꾼패 처리 부분 동시에 뿌림
void CGame::SetOtherChangeCard_62Cut_Ex(CHANGECARD_62CUT_EX pCC)
{			
	if(pCC.nDistUser == -1)return;
	
	int pnum = GetPNum_ByServPN( pCC.nDistUser ); // 카드 날리는 순서

	int delay = 3*3;

	BOOL bChangeEndFlg = TRUE; // 한사람도 바꾼 사람이 없을때를 위한 플래그

	for(int i = pnum; i<g_Max_Player + pnum; i++) {
		
		int index = i;
		if(index >= g_Max_Player) index = index-g_Max_Player;

		int spnum = Play[index].ServPNum;

		if(pCC.data[spnum].nServerPNum < 0 || pCC.data[spnum].nServerPNum > g_Max_Player)continue;				
		if(strcmp(Play[index].UI.ID,pCC.data[spnum].ID) != 0 )continue;				
		if(pCC.data[spnum].nCardIndex < 0 || pCC.data[spnum].nCardIndex >= 7)continue;	
		if(pCC.data[spnum].nCardNum < 0  || pCC.data[spnum].nCardNum > 52)continue;	
		
		//바꾼 카드 셋팅
		CardDeck[index].SetCardPosition(pCC.data[spnum].nCardIndex,index,
										pCC.data[spnum].nChangeCardNum,pCC.data[spnum].nCardNum,delay);		
		
		bChangeEndFlg = FALSE;
		if(index == pnum)Sound.Play( SND16 );
		// 인원수에 따라 차등
		delay +=2*2;		
	}		

	if(bChangeEndFlg)//한사람도 바꾼 카드가 없다
	{
		Game.FlyWindCard = FALSE;
		CCL_CARDCHANGE_END_62CUT cend;
		cend.Set(g_RI.RoomNum,Play[0].ServPNum,Play[0].UI.ID);
		SockMan.SendData(g_MainSrvSID,cend.pData,cend.GetTotalSize());				
	}
}

//  바꾸기 버튼 처리// 62cut [62]
void CGame::SetChangeCard_62Cut()
{	
	if(!g_MyObserver && Play[0].bFold == TRUE || Play[0].JoinState != 1 || Play[0].PlayState !=1 )return;

	if(g_pGameView->ChangeCard_OK.m_bEnable == FALSE)return;
	if(g_pGameView->ChangeCard_Cancel.m_bEnable == FALSE)return;
	if(g_bChangeCardStage == FALSE)return;	

	if(CardDeck[0].m_ChangeCardNum > -1 && CardDeck[0].m_SelectCard > -1){

		//바꾼 카드 표시
		CardDeck[0].SetChangeSelectCard();
		Play[0].bChangeWhether =1;//내가 바꾸기를 했다

		CCL_CARDCHANGE_62CUT_RESULT  ccardre;
		CHANGECARD_62CUT data;
		
		ZeroMemory(&data,sizeof(CHANGECARD_62CUT));
		strcpy(data.ID,g_MyInfo.UI.ID);
		data.nFlag = CardDeck[0].m_ChangeCardNum;
		data.nUNum = Play[0].UI.UNum;
		data.nChangeCardNum = CardDeck[0].m_ChangeCardNum;
		data.nCardNum = 0;
		data.nRoomNum = g_RI.RoomNum;
		data.nCardIndex = CardDeck[0].m_SelectCard;
		data.nServerPNum = Play[0].ServPNum;

		ccardre.Set(&data);
		SockMan.SendData(g_MainSrvSID, ccardre.pData, ccardre.GetTotalSize());		
		
		g_bChangeCardStage = FALSE;	
		CardDeck[0].ReSetCardRect();
		Play[0].m_bSelectCard = FALSE;		
		m_nchange_card = 0;
		Sound.Play( SND16 );		
	}	
}

// 바꾸기 취소 버튼처리 // 62cut [62]
void CGame::SetChangeCard_Cancel_62Cut()
{	
	if(!g_MyObserver && Play[0].bFold == TRUE || Play[0].JoinState != 1 || Play[0].PlayState !=1 )return;

	if(g_bChangeCardStage == FALSE)return;

	Play[0].bChangeWhether  = 0;
	CCL_CARDCHANGE_62CUT_RESULT  ccardre;
	CHANGECARD_62CUT data;	

	memset(&data,-1,sizeof(CHANGECARD_62CUT));
	strcpy(data.ID,g_MyInfo.UI.ID);
	
	data.nUNum = Play[0].UI.UNum;
	data.nRoomNum = g_RI.RoomNum;	
	data.nServerPNum = Play[0].ServPNum;
	
	ccardre.Set(&data);
	SockMan.SendData(g_MainSrvSID, ccardre.pData, ccardre.GetTotalSize());	

	CardDeck[0].ReSetCardRect();
	g_bChangeCardStage = FALSE;		
	Play[0].m_bSelectCard = FALSE;
	m_nchange_card = 0;
	Sound.Play( SND16 );
	Game.FlyWindCard = FALSE;	
	if(g_RI.ChangeCardStep == 2)
	{
		Play[0].bHiddenOpen = 1;
		Game.m_bHiddenCardShow = 1;		
	}
}

////////////////////////////////////////////////////////////////////////////

// 족보등급 보기
void CGame::SetChangeViewTabBtn(BOOL bView)
{
	bViewTab = bView;
		
	if(bView)  {// 족보 보기
		g_pGameView->ChatBox.BadUserBtn.Enable(FALSE);
		g_pGameView->ChatBox.TabViewChatBtn.Enable(TRUE);
		g_pGameView->ChatBox.TabViewRuleBtn.Enable(FALSE);
	}
	else { // 채팅창
		g_pGameView->ChatBox.BadUserBtn.Enable(TRUE);
		g_pGameView->ChatBox.TabViewChatBtn.Enable(FALSE);
		g_pGameView->ChatBox.TabViewRuleBtn.Enable(TRUE);
	}

	nChatRuleMessage = 0; // 초기화
}

CString  CGame::GetCardName(int cardnum)
{
	CString cardname;	
	cardname= _T("");
	int kind = cardnum / 13;
	int num = cardnum - (kind * 13) +1;

	if(cardnum < 0)kind = 5;
	
	switch(kind)
	{
		case 0:
			cardname = "바꾼 카드가 ";
			if(num == 1)
				cardname += " A♣ 입니다.";
			else if(num == 11)
				cardname += " J♣ 입니다.";			
			else if(num == 12)
				cardname += " Q♣ 입니다.";			
			else if(num == 13)
				cardname += " K♣ 입니다.";			
			else
				cardname.Format("바꾼 카드가 %d♣ 입니다.",num);
		
			break;
		case 1:			
			cardname = "바꾼 카드가 ";
			if(num == 1)
				cardname += " A◆ 입니다.";
			else if(num == 11)
				cardname += " J◆ 입니다.";			
			else if(num == 12)
				cardname += " Q◆ 입니다.";			
			else if(num == 13)
				cardname += " K◆ 입니다.";			
			else 
				cardname.Format("바꾼 카드가 %d◆ 입니다.",num);

			break;
		case 2:
			cardname = "바꾼 카드가 ";
			if(num == 1)
				cardname += " A♥ 입니다.";
			else if(num == 11)
				cardname += " J♥ 입니다.";			
			else if(num == 12)
				cardname += " Q♥ 입니다.";			
			else if(num == 13)
				cardname += " K♥ 입니다.";			
			else 				
				cardname.Format("바꾼 카드가 %d♥ 입니다.",num);

			break;
		case 3:
			cardname = "바꾼 카드가 ";
			if(num == 1)
				cardname += " A♠ 입니다.";
			else if(num == 11)
				cardname += " J♠ 입니다.";			
			else if(num == 12)
				cardname += " Q♠ 입니다.";			
			else if(num == 13)
				cardname += " K♠ 입니다.";			
			else 				
				cardname.Format("바꾼 카드가 %d♠ 입니다.",num);

			break;
		default:
			cardname= "바꾼 카드가 없습니다";
			break;
	}
	return cardname;
}

// 현재 나의 하이룰 보기
int CGame::GetViewMyrule(enum SEVEN_CARD eName)
{
	int myValue = 0;
	switch(eName)
	{
	case N7C_SROYAL:
	case N7C_DROYAL:
	case N7C_HROYAL:
	case N7C_CROYAL:  // 로얄스트레이트플러쉬
		{
			myValue = 1;
		} break;
	case N7C_BSFLUSH: //"백스트레이트 플러시";
		{
			myValue = 2;
		} break;
	case N7C_SFULSH:  //"스트레이트 플러쉬";
		{
			myValue = 3;
		}break;
		
	case N7C_QUADS: //"포카드";
		{
			myValue = 4;
		}break;
	case N7C_TITLE: //"집(풀하우스)";
		{
			myValue = 5;
		}break;
	case N7C_FLUSH: //"플러시";
		{
			myValue = 6;
		}break;
	case N7C_MOUNT: //"마운틴(스트레이트)";
		{
			myValue = 7;
		} break;
	case N7C_BACKS: //"백스트레이트";
		{
			myValue = 8;
		} break;
		
	case N7C_STRAIGHT: //"스트레이트";
		{
			myValue = 9;
		} break;
	case N7C_TRIPLE:  //"봉(쓰리카드)";
		{
			myValue = 10;
		} break;
	case N7C_TWOPAIR: //"투페어";
		{
			myValue = 11;
		}
		break;
	case N7C_ONEPAIR: //"원페어";
		{
			myValue = 12;
		}
		break;
	}
	return myValue;	
}

void CGame::OnStartButtonActive(int spnum,char * id,int bactive) // 시작버튼 활성화 
{
	if(spnum < 0 || spnum > g_Max_Player)return;
	if(strlen(id) <= 0)return;
	
	if(bactive == 1){
		if(g_MyInfo.ServPNum != spnum) return;
		if(strcmp(g_MyInfo.UI.ID,id) != 0)return;
		
		//g_pGameView->GameStartBtn.Show(TRUE);	//시작버튼 비활성화 - jeong
		g_pGameView->GameStartBtn.Show(FALSE);
	}
	else 
	{
		g_pGameView->GameStartBtn.Show(FALSE);
	}
}

// [수호천사] 2004.07.09
void CGame::Charge_Safeangel_money(CCL_CHARGE_SAFEANGEL *pMsg)
{
	INT64 addmoney = (INT64)*pMsg->money;
	int kind = *pMsg->kind;
	
	CString sysmsg = _T("");
	if(strcmp(pMsg->ID, g_MyInfo.UI.ID) == 0) {
		if( g_MyObserver ) {
			g_MyInfo.UI.PMoney += addmoney;	
		}
		else {
			Play[0].UI.PMoney += addmoney;
			g_MyInfo.UI.PMoney = Play[0].UI.PMoney;
		}
			
		switch(kind)
		{
		case CCL_CHARGE_SAFEANGEL::CHARGE_ANGEL:
			{
			//	sysmsg.Format("%s원을 [수호천사] 아이템으로 충전합니다.\n", g_MoneyMark(addmoney));
				sysmsg.Format("수호천사 아이템을 사용하여 %s원이 복구되었습니다.\n", g_MoneyMark(addmoney));
			}break;
		}
	}
	else {
		int pnum = Game.GetPlayerPNum(pMsg->ID);
		if(pnum != -1) {
			switch(kind)
			{
			case CCL_CHARGE_SAFEANGEL::CHARGE_ANGEL:
				{
					sysmsg.Format("[%s]님이 수호천사 아이템을 사용하여 %s원이 복구되었습니다.\n", pMsg->ID, g_MoneyMark( addmoney ));
				}break;
			}
			Play[pnum].UI.PMoney += addmoney;
		}
	}

	if(sysmsg != "") {
		if(g_Where==WH_LOBY)
		{
			g_pLobyDlg->m_ChatViewEdit.SetTextFormat(0);
			g_pLobyDlg->m_ChatViewEdit.AddText(&sysmsg, RGB(96,112,254));
			CRect rt;
			rt.SetRect(574,395,574+209,395+145);
			g_pLobyDlg->InvalidateRect(&rt,FALSE);
		}
		if(g_Where==WH_GAME)
		{
			g_pGameView->ChatBox.AddText( &sysmsg, RGB(250,250,0) );
		}
		Sound.Play( SND50 );	//사운드는 맘에 드는걸로다가
	}

}