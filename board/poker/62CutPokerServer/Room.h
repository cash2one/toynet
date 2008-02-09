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
//#define MAX_TURNTIMEOUT	19	// ########### �� Ÿ�Ӿƿ�
#define MAX_TURNTIMEOUT	11	// ########### �� Ÿ�Ӿƿ�		- modifed by jeong
#define MAX_SELECTTIMEOUT 15

#define TOTAL_CARDNUM 52 //�� ī���
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
	RSTATE_NONE = 0,			// �ƹ����µ� �ƴ�(�����)
	RSTATE_DISTRIBUTECARD,		// �и� ������ ��
	RSTATE_PREPARESTART,		// ���� ������ �غ���
	RSTATE_STARTGAME,			// ������ ���۵Ǿ���
	RSTATE_RESULT  // ���� ���
};
*/

enum
{
	RSTATE_NONE = 0,			// �ƹ����µ� �ƴ�(�����)
	RSTATE_DISTRIBUTECARD,		// �и� ������ ��
	RSTATE_PREPARESTART,		// ���� ������ �غ���
	RSTATE_STARTGAME,			// ������ ���۵Ǿ���
	RSTATE_CHANGECARD,			// ī�� �ٲٱ�
	RSTATE_RESULT  // ���� ���
};

//  ī�� ��� 
enum GAMEPLAY_ROUND_STATE
{
	GAME_ROUND_READY = 0,			// 0 : �ƹ����µ� �ƴ�(�����)	0
	GAME_ROUND_3,					// 1 : 3���� ��					1
	GAME_ROUND_CARDCHOICE,			// 2 : 3���� ī�带 ������ 		2
	GAME_ROUND_4,					// 3 : 4���� ��					3
	GAME_ROUND_5,					// 4 : 5���� ��					4
	GAME_ROUND_6,					// 5 : 6���� ��					5
	GAME_ROUND_HIDDEN,				// 6 : 7�� (����) 
	//GAME_ROUND_CHANGECARD1,			// 6 : ī�带 �ٲ� (ù��°)		6	
	//GAME_ROUND_CHANGECARD2,			// ī�带 �ٲ� (�ι�° �������� �� )		7
	GAME_ROUND_RESULT,				// ���� ���ڸ� ����			8	
};


class CChannel;

// ����� ���� ����ü [���� ������ �۾�]
struct USERSTATE
{
	BOOL bForceBanish;	// ���� �߹� ����
};

// ������ ����Ʈ Ŭ���� [���� ������ �۾�]
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
// ### [ ������� ] ###
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
	// 5, 7�ο� �۾�
	
	int			m_Max_NewPlayer;		//���� ������ �Ҽ� �ִ� �ο����� ������ ���� ���� 10���̴�.

public:
	char		m_DealerID[16];		// ���� �����
	int			m_Max_Player;			//���� ������ �ϴ� �ο��� ���� 5���̴�
	// 5, 7�ο� �۾�
inline	const int get_maxplayer() const	  { return m_Max_Player; }
inline	const int get_maxnewplayer() const { return m_Max_NewPlayer; }
	void set_player_usernum(const int channel_usernum);

public:
	CChannel *pChan;	// ���� ���� ä���� ������
//	char* SndBuf;		// ���� ���� ä���� ���� �ӽ� ������ ������
	BOOL bValid;

	// ### [ ������� ] ###
	ROOMINFO Ri;
	USERINFO Ui[MAX_NEWPLAYER];
	PLAYSTATE2 Ps2[MAX_NEWPLAYER];
	USERSTATE	Us[MAX_NEWPLAYER];		// ���� ���� ����(�ڵ����� Reset���� ����)	[���� ������ �۾�]

	PLAYSTATE Ps[MAX_NEWPLAYER];
	BACKCARD  Bc[MAX_NEWPLAYER];   // ��� ī��
	CSevenCard m_Seven;
	CRaise    m_Raise;

	//CHANGECARD_62CUT m_PlayerChangeCradDeck[MAX_PLAYER-1];//�ٲ� ī�� �ӽ� ���� ���

	// ### [ ������� ] ###
	BOOL bNowUserEmptyDestory;
	CPtrList m_ReservedList;  //����->�������� ����Ʈ
	CPtrList m_ObservedList;  //����->�������� ����Ʈ
	int m_AdminCount;  // ### [������ ��� �۾�] ###  ���� ������ ������ ��

	////// [���� ������ �۾�]
	BOOL		bSuperRoomDestory;	// ���۹����� �������� ���� ���۹� �ı� ����
	CBanUserList BanUserList;		// ������ ����Ʈ Ŭ����(���۹�
	CBanUserList VoteBanUserList;		// ������ ����Ʈ Ŭ����(�Ϲݹ� 


	int m_CurPlayer;
	int m_GameState;		// 0: 1:ī�� 3��, 2: ī�� ����, 3: 4��, 4: 5��, 5: 6��, 6: ����, 7: �°���
	int m_CardWindState;
	int m_JackPotActive;
	
	////// ���� ���� ���� ������
	int StartUserNum;
	BOOL bRestartCnt;
	int RestartCnt;
	BOOL bWaitGameOver;
	int WaitGameOverCnt;

	int GameState;	// 0:�ƹ����µ��ƴ� 1:���ӽ���
	// ### [ �ߺ��� IP�� ] ###
	int m_aIpIndex[MAX_PLAYER+1]; 
	int m_nIpCnt;
	
			
	////// 7card ���� ���� ���� �Լ���
	char m_CardDeck[52];
	int m_CurCardIndex;

	////// �߹� ��ǥ ���� ������
	int BanishVoteState;
	int BanishVoteCnt;
	int BanishVoteAgreeNum;
	int BanishVoteDisgreeNum;
	BANISHVOTEINFO BVI;
	SMALLROOMINFO m_SRi;		   // ������ �����

public:
	void LogRecordFunc(int unum, int pnum);

	CRoom();
	virtual ~CRoom();

	// ### [������ ��� �۾�] ###
	void Admin_Room_Remove();
	void ChangeRoomOption_ADMIN(CNM_ADMIN_CHANGEROOM *pMsg);
	void ChangeRoomOption(CCL_ASK_CHSTYLEROOMINFO *pMsg);
	int Room_MyIp_Check(int unum);
	
	// ### [ ������� ] ###
	void Emerge_Out(int unum, int pnum);
	void Participation(int unum, int pnum, int sndFxKind);
	// �ڸ����� 
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

	// ### [ �ߺ��� IP�� ] ### // ### [���� �߰� �۾�] ###
	BOOL OnUserEnterRoom(USERINFO *pUI, int pnum, int index=0, int sndFxKind=0);
	BOOL OnUserOutRoom(int unum, char *id, BOOL bDisconnect);

	void OnForceBanish(CSV_ASK_FORCEBANISH *pMsg);	// [���� ������ �۾�]
	void DoForceBanish(int pnum);					// [���� ������ �۾�]

	void SendRoomEventNotice(int pnum); // ### [�̺�Ʈ Ƽ��] ###
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
	
	
	//  ####################  ���ΰ��� ��ƾ ###################

	
	int       m_nPrimeUser;
	int       GetPrimer();
	
	int  GiveCard();
	int  GetRand(int nMod);
	BOOL FoldUser(int nNum, BOOL bOut = FALSE);// ### [ ������� ] ###
	void OnReadyToStart(int pnum, POKERCLNT *pc, int discard);

	////// ���� ���� ���� �Լ���

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

	////// ��Ÿ �Լ���

	void OnAskBanishVote(BANISHVOTEINFO *pbv);
	void DoBanishVote();
	void OnMyBanishVote(char *id, BOOL bAgree);
};

#endif // !defined(AFX_ROOM_H__A5112CA1_5E39_11D4_97A4_0050BF0FBE67__INCLUDED_)
