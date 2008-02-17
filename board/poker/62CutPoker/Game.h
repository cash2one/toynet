// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__3CC8EB42_24E8_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_GAME_H__3CC8EB42_24E8_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>
#include "Graphic.h"
#include "62CutPoker.h"
#include "62CutPokerDlg.h"
#include "UserStruct.h"
#include "CardDeck.h"

#include "SevenCard.h"

#include "Chip.h"


#define TOTALTURN_MAX 160	// 초
#ifdef _DEBUG
	#define MYTURN_MAX 5		// 초
#else
	#define MYTURN_MAX 12		// 초
#endif

#define MAX_CHIP 100
#define MAX_CHIPCNT 17//13 // 칩 종류

#define MAX_STACK_OFFSETY 320

enum
{
	RSTATE_NONE = 0,			// 아무상태도 아님(대기중)	
	RSTATE_PREPARESTART,		// 게임 시작을 준비중
	RSTATE_STARTGAME,			// 게임이 시작되었음
	RSTATE_CHANGECARD,			// 카드 바꾸기
	RSTATE_RESULT  // 게임 결과
};


class CGame  
{
public:
	CPage *pPage;
	// #### [ 세븐 포커 ] ####
	CCardDeck CardDeck[MAX_PLAYER]; // 각 플레이어 가지고 있는 카드 7장
	CSevenCard m_SevenCard;			// 세븐 포커 룰 관련
	BOOL FlyWindCard;				// 카드 날리는 중인가
	int CurPlayer;					// 현재 순서
	int WinnerPNum;					// 하이 승자
	int LowWinnerPNum;				// 로우 승자
	BOOL bOpen;						// 어떻게 이겼는가?(모두 죽어서 홀로 이겼는가 FALSE, 끝까지 베팅했는가 TRUE)
	
	char m_jackpot_winnerid[16];
	// 클라이언트 현재 상태
	int m_GameState;				// 0 없음, 1, 2, 3, 4, 5, 6 승결정중 7 승결정(게임 오버)
	INT64 m_JackPotActive;

	int No1PosPNum;
	int MyPosPNum;
	BOOL bCreateRoom;		// Create Room - jeong

	// 타이머 카운트
	int TotalTurnCnt;
	int MyTurnCnt;
	int m_GameCount;

	// [ 족보 성립 5장 ]
	int m_CardHi[5]; 
	int m_CardLow[5];
	int m_CardSwing[7];

	// 화면에 시간 그래프
	BOOL ClockFlag[16];
	DWORD CurTime;
	DWORD BackTime;
	void SetBackTime();

	int m_nHinddenArrow;
	BOOL m_bHiddenCardShow;
	BOOL m_bHiddenClick;
	BOOL bBtnClick;

	CPoint m_hpos;

	// [ 칩 ]
	INT64      m_RMoney;
	CChip m_Chip[MAX_CHIP];
	CChip m_ChipEx[MAX_CHIP];

	// [ 카드 이벤트 에러시 ]
	BOOL bWindCardEnd;
	int  nWindCardEndCnt;
	int  StartUserNum; // 현재 게임 시작 인원

	// 게임 정보
	BOOL bGameStart;		// 게임이 시작되었는가?(패돌리기 포함)
	BOOL bHaveToResetGame;	// 게임을 초기화 해야하는가?
	int ResetGameCnt;		// 게임초기화 시점 카운터
	BOOL bCantStartGame;	// 게임을 시작할 수 없는 상태인가?
	int GameCnt;

	int m_discardindex;     // 버린카드 1~4

	// 게임 도움말창 관련 정보
	int HelpMsgCnt;
	int HelpMsgKind;
	CString szHelpMsg;

	// 유리 메세지창 관련 정보
//	int GlassMsgCnt;
//	WORD GlassMsgColor;
//	int GlassMsgGrade;
//	CString szGlassMsg;

	// 족보 등급 보여주기
	BOOL bViewTab;   // 0:채팅창, 1:등급창
	int MyHighRule; //
	int MyLowRule;	//
	int nChatRuleMessage; // 0: 메세지 없음 1: 채팅 메세지 2: 족보

	// 카드 선택 타이머
	BOOL m_bchoice_card;
	int  m_nchoice_cardcnt;
	int  m_ncur_selectcard;
	int  m_nchange_card;

	// 축적된 배팅 Y 좌표
	int m_nStackY[MAX_CHIPCNT];
	int m_cn[MAX_CHIPCNT];


public:
	void StackChip();
	
	int GetViewMyrule(enum SEVEN_CARD eName);
	void SetChangeViewTabBtn(BOOL bView=FALSE);

	// ### [ 관전기능 ] ###
	void Accept_CreateRoom(CSV_ACCEPT_CREATEROOM *pMsg);	// 방만들기 허가
	void Accept_EnterRoom(CSV_ACCEPT_ENTERROOM *pMsg);		// 방에 입장 허가 받음
	void User_EnterRoom(CSV_USERENTERROOM *pMsg);			// 유저가 방에 입장 알림
	void User_OutRoom(CSV_USEROUTROOM *pMsg);				// 유저가 방에서 나감 알림
	void SetPlayInfo(int nowpnum, int newpnum,BOOL bParticipation); // 관전->참여..

	void ChipFunc( USERINFO *pUI);

	CGame();
	virtual ~CGame();

	void Init(CPage *ppage, CPage *pbatpage);
	void ResetGame();
	void OnTimer();

	int GetPNum_ByServPN(int onum);
	int GetServPNum_ByPN(int onum);
	int GetPlayerPNum(char *id);


	// 게임 도움말 메세지 세팅
	void SetHelpMsg(int cnt, int kind, CString &msg);
	// 유리 메세지 세팅
	void SetGlassMsg(int cnt, WORD color, CString &msg);
	// 배경음악
	void PlayMusic();
	// 키보드 관련
	void OnGameKey(int nVKey);

	// 마우스 관련
	BOOL OnLButtonDown(int x, int y);
	BOOL OnMouseMove(int x, int y);
	BOOL OnLButtonDblclk(int x, int y);
	BOOL OnRButtonDown(int x, int y);
	BOOL OnLButtonUp(int x, int y);

	// 게임 진행 함수들...
	void OnStartButtonActive(int spnum,char * id,int bactive); // 시작버튼 활성화 
	void DoPrepareGame(STARTINFO *pSC = NULL);
	void OnGameOver(GAMEOVERRESULT *pGOR);
	void DoExitSubscript(int bexit);

	// #### [ 세븐 포커 ] ####
	INT64 GetRealBet();
	INT64 GetRaiseBat();
	INT64 GetTotalBet();
	void SetWindCardEnd(BOOL bFirst=FALSE);
	void GetCardNum(int pCard[],enum SEVEN_CARD eName, int nVal);
	void CastChip(int pnum, INT64 nMarble, int sdelay = 0);
	void CastChipEx(int pnum, INT64 nMarble, int sdelay = 0);
	
	void CastChipJackPot(int pnum, INT64 nMarble, int sdelay = 0);
	
	
	void PreDrawCastedChip(BYTE *pChipCnt);
	void AddChip(INT64 nMoney);
	
	// [ 베팅 관련 ]
	void Raise(POKERGAME *pMsg);

	void Charge_Safeangel_money(CCL_CHARGE_SAFEANGEL *pMsg); // [수호천사] 2004.07.08

	// [ 카드 날리기 ]
	void EndWindCard();
	void Card_Distribute();
	void OnCardEvent(CARDEVENT *pEvent);

	// [ 하이 족보 ]
	CString GetMyName();
	CString GetName(int nPNum, BOOL bRule = FALSE);
	INT64 GetMaxMoney(INT64 nTotal);

	void SoundFxKind(int betkind, int sex, int fxkind);// ### [사운드 추가 작업] ###

	// [62]	
	void ChangeCard_Init();
	void SetChangeCard_62Cut();
	void SetChangeCard_Cancel_62Cut();
	void SetOtherChangeCard_62Cut(CHANGECARD_62CUT data);
	void SetOtherChangeCard_62Cut_Ex(CHANGECARD_62CUT_EX data);

	CString  GetCardName(int cardnum);//[62] 바꾼 카드 이름

};

#endif // !defined(AFX_GAME_H__3CC8EB42_24E8_11D4_97A5_0050BF0FBE67__INCLUDED_)
