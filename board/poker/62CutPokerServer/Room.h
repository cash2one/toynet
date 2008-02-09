// Room.h: interface for the CRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROOM_H__A5112CA1_5E39_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_ROOM_H__A5112CA1_5E39_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "CommMsg.h"
#include "SevenCard.h"
#include "Raise.h"

#define MAX_RAISENUM 4
#define MAX_WINDTIMEOUT 17
//#define MAX_TURNTIMEOUT	19	// ########### 턴 타임아웃
#define MAX_TURNTIMEOUT	11	// ########### 턴 타임아웃		- modifed by jeong
#define MAX_SELECTTIMEOUT 15

#define TOTAL_CARDNUM 52 //총 카드수
#define DUMY_CARD     52
#define EMPTY_CARD    -1
#define MAX_PLAYERCARD 6

#define JACKPOT_FCARD 10
#define JACKPOT_SCARD 8
#define JACKPOT_RCARD 5

#define MIN_PLAYER		2 //3

/*
enum
{
	RSTATE_NONE = 0,			// 아무상태도 아님(대기중)
	RSTATE_DISTRIBUTECARD,		// 패를 돌리는 중
	RSTATE_PREPARESTART,		// 게임 시작을 준비중
	RSTATE_STARTGAME,			// 게임이 시작되었음
	RSTATE_RESULT  // 게임 결과
};
*/

enum
{
	RSTATE_NONE = 0,			// 아무상태도 아님(대기중)
	RSTATE_DISTRIBUTECARD,		// 패를 돌리는 중
	RSTATE_PREPARESTART,		// 게임 시작을 준비중
	RSTATE_STARTGAME,			// 게임이 시작되었음
	RSTATE_CHANGECARD,			// 카드 바꾸기
	RSTATE_RESULT  // 게임 결과
};

//  카드 장수 
enum GAMEPLAY_ROUND_STATE
{
	GAME_ROUND_READY = 0,			// 0 : 아무상태도 아님(대기중)	0
	GAME_ROUND_3,					// 1 : 3장을 줌					1
	GAME_ROUND_CARDCHOICE,			// 2 : 3장중 카드를 선택함 		2
	GAME_ROUND_4,					// 3 : 4장을 줌					3
	GAME_ROUND_5,					// 4 : 5장을 줌					4
	GAME_ROUND_6,					// 5 : 6장을 줌					5
	GAME_ROUND_HIDDEN,				// 6 : 7장 (히든) 
	//GAME_ROUND_CHANGECARD1,			// 6 : 카드를 바꿈 (첫번째)		6	
	//GAME_ROUND_CHANGECARD2,			// 카드를 바꿈 (두번째 히든으로 줌 )		7
	GAME_ROUND_RESULT,				// 승자 패자를 가림			8	
};


class CChannel;

// 사용자 상태 구조체 [게임 아이템 작업]
struct USERSTATE
{
	BOOL bForceBanish;	// 강제 추방 예약
};

// 강퇴자 리스트 클래스 [게임 아이템 작업]
class CBanUserList
{
public:
	enum { MAX_BANUSERNUM = 10 };
	
	int  m_TotNum;
	int  m_NowPos;
	char m_BanID[MAX_BANUSERNUM][16];

	void Clear() {
		m_TotNum = 0;
		m_NowPos = 0;
		memset(m_BanID, 0, sizeof(m_BanID));
	}

	void AddID(char* id) {
		strncpy(m_BanID[m_NowPos], id, 15);
		m_NowPos++;
		if(m_NowPos >= MAX_BANUSERNUM) m_NowPos = 0;
		m_TotNum++;
		if(m_TotNum >= MAX_BANUSERNUM) m_TotNum = MAX_BANUSERNUM;
	}

	BOOL FindID(char* id) {
		for(int i=0; i<m_TotNum; i++) {
			if(strncmp(m_BanID[i], id, 15) == 0)
				return TRUE;
		}
		return FALSE;
	}
};

///////////////////////////////////////////////////////////////////////////////
// ### [ 관전기능 ] ###
typedef struct 
{
	int PNum;
	int UNum;
	int nSndFxKind;
	USERINFO *pUI;
} RESERVEDINFO, *LPRLIST;


class CRoom  
{
private:
	// 5, 7인용 작업
	
	int			m_Max_NewPlayer;		//실제 게임을 할수 있는 인원수와 관전자 포함 현재 10명이다.

public:
	char		m_DealerID[16];		// 전번 우승자
	int			m_Max_Player;			//실제 게임을 하는 인원수 현재 5명이다
	// 5, 7인용 작업
inline	const int get_maxplayer() const	  { return m_Max_Player; }
inline	const int get_maxnewplayer() const { return m_Max_NewPlayer; }
	void set_player_usernum(const int channel_usernum);

public:
	CChannel *pChan;	// 룸이 속한 채널의 포인터
//	char* SndBuf;		// 룸이 속한 채널의 센드 임시 버퍼의 포인터
	BOOL bValid;

	// ### [ 관전기능 ] ###
	ROOMINFO Ri;
	USERINFO Ui[MAX_NEWPLAYER];
	PLAYSTATE2 Ps2[MAX_NEWPLAYER];
	USERSTATE	Us[MAX_NEWPLAYER];		// 유저 상태 정보(자동으로 Reset되지 않음)	[게임 아이템 작업]

	PLAYSTATE Ps[MAX_NEWPLAYER];
	BACKCARD  Bc[MAX_NEWPLAYER];   // 백업 카드
	CSevenCard m_Seven;
	CRaise    m_Raise;

	//CHANGECARD_62CUT m_PlayerChangeCradDeck[MAX_PLAYER-1];//바꾼 카드 임시 저장 장소

	// ### [ 관전기능 ] ###
	BOOL bNowUserEmptyDestory;
	CPtrList m_ReservedList;  //관전->참여예약 리스트
	CPtrList m_ObservedList;  //참여->관전예약 리스트
	int m_AdminCount;  // ### [관리자 모드 작업] ###  현재 접속한 관리자 수

	////// [게임 아이템 작업]
	BOOL		bSuperRoomDestory;	// 슈퍼방장의 퇴장으로 인한 슈퍼방 파괴 예약
	CBanUserList BanUserList;		// 강퇴자 리스트 클래스(슈퍼방
	CBanUserList VoteBanUserList;		// 강퇴자 리스트 클래스(일반방 


	int m_CurPlayer;
	int m_GameState;		// 0: 1:카드 3장, 2: 카드 선택, 3: 4장, 4: 5장, 5: 6장, 6: 히든, 7: 승결정
	int m_CardWindState;
	int m_JackPotActive;
	
	////// 게임 진행 관련 변수들
	int StartUserNum;
	BOOL bRestartCnt;
	int RestartCnt;
	BOOL bWaitGameOver;
	int WaitGameOverCnt;

	int GameState;	// 0:아무상태도아님 1:게임시작
	// ### [ 중복된 IP가 ] ###
	int m_aIpIndex[MAX_PLAYER+1]; 
	int m_nIpCnt;
	
			
	////// 7card 게임 진행 관련 함수들
	char m_CardDeck[52];
	int m_CurCardIndex;

	////// 추방 투표 관련 변수들
	int BanishVoteState;
	int BanishVoteCnt;
	int BanishVoteAgreeNum;
	int BanishVoteDisgreeNum;
	BANISHVOTEINFO BVI;
	SMALLROOMINFO m_SRi;		   // 방정보 변경용

public:
	void LogRecordFunc(int unum, int pnum);

	CRoom();
	virtual ~CRoom();

	// ### [관리자 모드 작업] ###
	void Admin_Room_Remove();
	void ChangeRoomOption_ADMIN(CNM_ADMIN_CHANGEROOM *pMsg);
	void ChangeRoomOption(CCL_ASK_CHSTYLEROOMINFO *pMsg);
	int Room_MyIp_Check(int unum);
	
	// ### [ 관전기능 ] ###
	void Emerge_Out(int unum, int pnum);
	void Participation(int unum, int pnum, int sndFxKind);
	// 자리예약 
	void Make_a_Reservaiton(int pnum, int unum, int nSndFxKind);
	void Cancel_a_Reservation(char *id);
	void Reserve_takeaSeat();
	void ReservedRemoveAll();
	void Want_Observer_Send();
	int GetReservationSequential();
	void Observer_takeaSeat(char *id);
	void ObserverRemoveAll();
	BOOL Observer_FindID(char *id);
	void Cancel_a_Observer(char *id);
	void Make_a_Observer(int pnum, int unum);
	BOOL ReservationFindID(char *id);
	void GetReservaitonInfo();


	void Clear();
	int GetPNum(char* id);

	SMALLROOMINFO &GetSmallRoomInfo();
	const ENTERROOMINFO GetEnterRoomInfo();


	BOOL SendMsgTo(int pnum, char *pMsgData, int nMsgSize);
	BOOL SendMsgTo(char* id, char *pMsgData, int nMsgSize);
	BOOL SendMsgExept(char* id, char *pMsgData, int nMsgSize);
	BOOL SendMsgExept(int pnum, char *pMsgData, int nMsgSize);
	BOOL SendMsgToAll(char *pMsgData, int nMsgSize);

	// ### [ 중복된 IP가 ] ### // ### [사운드 추가 작업] ###
	BOOL OnUserEnterRoom(USERINFO *pUI, int pnum, int index=0, int sndFxKind=0);
	BOOL OnUserOutRoom(int unum, char *id, BOOL bDisconnect);

	void OnForceBanish(CSV_ASK_FORCEBANISH *pMsg);	// [게임 아이템 작업]
	void DoForceBanish(int pnum);					// [게임 아이템 작업]

	void SendRoomEventNotice(int pnum); // ### [이벤트 티켓] ###
	void ExitSubScript(CCL_EXITSUBSCRIPT *pMsg, int pnum, int unum);
	int GetPercentGory();
	INT64 CalcGory(INT64 nRealTotal);

	void CheckSelectWinCase();
	void SelectWinCase(int pnum, int winCase);
	void GetLowHandWinner();
	int Random_Num(int min, int max);
	void CardShuffle();


	void StartRaise();
	void AnalRaiseOn();
	void EndWind(int pnum);//, POKERCLNT *pc);
	void SendTurn(int nPNum,POKERCLNT *pc);
	int GetNextUser(int nPrev);
	
//	int GetWinner(BOOL bEnd,int &nSpecialVal);
	int GetHiLowWinner(BOOL bEnd,int nSpecialVal[]);
	int GetWinner(BOOL bEnd,int nSpecialVal[]);

	void SendReset();
	POKERGAME MakeInfo();
	
	
	//  ####################  메인게임 루틴 ###################

	
	int       m_nPrimeUser;
	int       GetPrimer();
	
	int  GiveCard();
	int  GetRand(int nMod);
	BOOL FoldUser(int nNum, BOOL bOut = FALSE);// ### [ 관전기능 ] ###
	void OnReadyToStart(int pnum, POKERCLNT *pc, int discard);

	////// 게임 진행 관련 함수들

	void OnGlobalGameTimer();
	void ResetGame();
	void StartGame();

	int GetTotLivePlayer();
	int GetTotLiveTeam();
	BOOL CheckGameOver(BOOL &bOpen);

	//62cut
	//void ChangeCard_62cut(CHANGECARD_62CUT data);
	//void ChangeCardEnd_62Cut(int pnum,char *id);
	void ChangeCardRaiseCheck();

	void OnAskChangeGameKind(CCL_ASK_CHANGEGAMEKIND *pMsg);
	void SetWaitGameOver();
	void DoGameOver(BOOL bOpen);

	////// 기타 함수들

	void OnAskBanishVote(BANISHVOTEINFO *pbv);
	void DoBanishVote();
	void OnMyBanishVote(char *id, BOOL bAgree);
};

#endif // !defined(AFX_ROOM_H__A5112CA1_5E39_11D4_97A4_0050BF0FBE67__INCLUDED_)
