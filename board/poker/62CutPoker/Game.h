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


#define TOTALTURN_MAX 160	// ��
#ifdef _DEBUG
	#define MYTURN_MAX 5		// ��
#else
	#define MYTURN_MAX 12		// ��
#endif

#define MAX_CHIP 100
#define MAX_CHIPCNT 17//13 // Ĩ ����

#define MAX_STACK_OFFSETY 320

enum
{
	RSTATE_NONE = 0,			// �ƹ����µ� �ƴ�(�����)	
	RSTATE_PREPARESTART,		// ���� ������ �غ���
	RSTATE_STARTGAME,			// ������ ���۵Ǿ���
	RSTATE_CHANGECARD,			// ī�� �ٲٱ�
	RSTATE_RESULT  // ���� ���
};


class CGame  
{
public:
	CPage *pPage;
	// #### [ ���� ��Ŀ ] ####
	CCardDeck CardDeck[MAX_PLAYER]; // �� �÷��̾� ������ �ִ� ī�� 7��
	CSevenCard m_SevenCard;			// ���� ��Ŀ �� ����
	BOOL FlyWindCard;				// ī�� ������ ���ΰ�
	int CurPlayer;					// ���� ����
	int WinnerPNum;					// ���� ����
	int LowWinnerPNum;				// �ο� ����
	BOOL bOpen;						// ��� �̰�°�?(��� �׾ Ȧ�� �̰�°� FALSE, ������ �����ߴ°� TRUE)
	
	char m_jackpot_winnerid[16];
	// Ŭ���̾�Ʈ ���� ����
	int m_GameState;				// 0 ����, 1, 2, 3, 4, 5, 6 �°����� 7 �°���(���� ����)
	INT64 m_JackPotActive;

	int No1PosPNum;
	int MyPosPNum;
	BOOL bCreateRoom;		// Create Room - jeong

	// Ÿ�̸� ī��Ʈ
	int TotalTurnCnt;
	int MyTurnCnt;
	int m_GameCount;

	// [ ���� ���� 5�� ]
	int m_CardHi[5]; 
	int m_CardLow[5];
	int m_CardSwing[7];

	// ȭ�鿡 �ð� �׷���
	BOOL ClockFlag[16];
	DWORD CurTime;
	DWORD BackTime;
	void SetBackTime();

	int m_nHinddenArrow;
	BOOL m_bHiddenCardShow;
	BOOL m_bHiddenClick;
	BOOL bBtnClick;

	CPoint m_hpos;

	// [ Ĩ ]
	INT64      m_RMoney;
	CChip m_Chip[MAX_CHIP];
	CChip m_ChipEx[MAX_CHIP];

	// [ ī�� �̺�Ʈ ������ ]
	BOOL bWindCardEnd;
	int  nWindCardEndCnt;
	int  StartUserNum; // ���� ���� ���� �ο�

	// ���� ����
	BOOL bGameStart;		// ������ ���۵Ǿ��°�?(�е����� ����)
	BOOL bHaveToResetGame;	// ������ �ʱ�ȭ �ؾ��ϴ°�?
	int ResetGameCnt;		// �����ʱ�ȭ ���� ī����
	BOOL bCantStartGame;	// ������ ������ �� ���� �����ΰ�?
	int GameCnt;

	int m_discardindex;     // ����ī�� 1~4

	// ���� ����â ���� ����
	int HelpMsgCnt;
	int HelpMsgKind;
	CString szHelpMsg;

	// ���� �޼���â ���� ����
//	int GlassMsgCnt;
//	WORD GlassMsgColor;
//	int GlassMsgGrade;
//	CString szGlassMsg;

	// ���� ��� �����ֱ�
	BOOL bViewTab;   // 0:ä��â, 1:���â
	int MyHighRule; //
	int MyLowRule;	//
	int nChatRuleMessage; // 0: �޼��� ���� 1: ä�� �޼��� 2: ����

	// ī�� ���� Ÿ�̸�
	BOOL m_bchoice_card;
	int  m_nchoice_cardcnt;
	int  m_ncur_selectcard;
	int  m_nchange_card;

	// ������ ���� Y ��ǥ
	int m_nStackY[MAX_CHIPCNT];
	int m_cn[MAX_CHIPCNT];


public:
	void StackChip();
	
	int GetViewMyrule(enum SEVEN_CARD eName);
	void SetChangeViewTabBtn(BOOL bView=FALSE);

	// ### [ ������� ] ###
	void Accept_CreateRoom(CSV_ACCEPT_CREATEROOM *pMsg);	// �游��� �㰡
	void Accept_EnterRoom(CSV_ACCEPT_ENTERROOM *pMsg);		// �濡 ���� �㰡 ����
	void User_EnterRoom(CSV_USERENTERROOM *pMsg);			// ������ �濡 ���� �˸�
	void User_OutRoom(CSV_USEROUTROOM *pMsg);				// ������ �濡�� ���� �˸�
	void SetPlayInfo(int nowpnum, int newpnum,BOOL bParticipation); // ����->����..

	void ChipFunc( USERINFO *pUI);

	CGame();
	virtual ~CGame();

	void Init(CPage *ppage, CPage *pbatpage);
	void ResetGame();
	void OnTimer();

	int GetPNum_ByServPN(int onum);
	int GetServPNum_ByPN(int onum);
	int GetPlayerPNum(char *id);


	// ���� ���� �޼��� ����
	void SetHelpMsg(int cnt, int kind, CString &msg);
	// ���� �޼��� ����
	void SetGlassMsg(int cnt, WORD color, CString &msg);
	// �������
	void PlayMusic();
	// Ű���� ����
	void OnGameKey(int nVKey);

	// ���콺 ����
	BOOL OnLButtonDown(int x, int y);
	BOOL OnMouseMove(int x, int y);
	BOOL OnLButtonDblclk(int x, int y);
	BOOL OnRButtonDown(int x, int y);
	BOOL OnLButtonUp(int x, int y);

	// ���� ���� �Լ���...
	void OnStartButtonActive(int spnum,char * id,int bactive); // ���۹�ư Ȱ��ȭ 
	void DoPrepareGame(STARTINFO *pSC = NULL);
	void OnGameOver(GAMEOVERRESULT *pGOR);
	void DoExitSubscript(int bexit);

	// #### [ ���� ��Ŀ ] ####
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
	
	// [ ���� ���� ]
	void Raise(POKERGAME *pMsg);

	void Charge_Safeangel_money(CCL_CHARGE_SAFEANGEL *pMsg); // [��ȣõ��] 2004.07.08

	// [ ī�� ������ ]
	void EndWindCard();
	void Card_Distribute();
	void OnCardEvent(CARDEVENT *pEvent);

	// [ ���� ���� ]
	CString GetMyName();
	CString GetName(int nPNum, BOOL bRule = FALSE);
	INT64 GetMaxMoney(INT64 nTotal);

	void SoundFxKind(int betkind, int sex, int fxkind);// ### [���� �߰� �۾�] ###

	// [62]	
	void ChangeCard_Init();
	void SetChangeCard_62Cut();
	void SetChangeCard_Cancel_62Cut();
	void SetOtherChangeCard_62Cut(CHANGECARD_62CUT data);
	void SetOtherChangeCard_62Cut_Ex(CHANGECARD_62CUT_EX data);

	CString  GetCardName(int cardnum);//[62] �ٲ� ī�� �̸�

};

#endif // !defined(AFX_GAME_H__3CC8EB42_24E8_11D4_97A5_0050BF0FBE67__INCLUDED_)
