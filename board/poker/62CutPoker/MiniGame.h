

#ifndef __MINI_GAME__
#define __MINI_GAME__

#include "CardEx.h"

#define MINI_TOTALCARD 20
#define MOVE_RAGE_X 40
#define DRAW_CARD_INDEX 6
#define CARD_GAP_SIZE 123

class CMiniGame
{

public:
	CMiniGame();
	virtual ~CMiniGame();

	void		Init(CPage *ppage);
	void		InitGame();
	void		Reset();
	void		ResetMoney();
	void		NewCardSet(int cardNum);
	int			GetCardNo(int index);
	void		SetFaceUp(int index);
	void		SetFaceDown(int index);


	void		MoveCard(int nIndex);
	bool		CheckCardOutRage(int nIndex);
	

	void		PreCardGame();
	void		PreCardSet();
	int			LoopToCheckCardNum(int nIndex);
	int 		CheckDuplicatedCardNum(int nIndex);
	bool		CompareCardNum(int nIndex, int nCardNum);

	void		OnTimer();
	void		Draw( BOOL bFold = FALSE);
	
	CCardEx		GetCardEx( int nIndex ) { return Card[nIndex]; }
	void		SetCardCurPos(int nIndex, POINT pt);
	POINT		GetCardCurPos(int nIndex );	
	BOOL		GetFace(int index);

	void		SetGameResult(int nGameContinue);
	int			GetGameResult();

	int			CheckCardRank();
	int			CheckWinGame(int nSelectRank);

	void		IsGame();
	void		StopGame();
	void		DefeatGame();

	int			GetBankMoney() { return m_nBankMoney; }
	int			GetWinMoney() { return m_nWinMoney; }
	void		SetWinMoney(int nMoney) { m_nWinMoney = m_nPrevWinMoney = nMoney; }

	bool		IsPlayGame() { return m_bPlayGame; }

	void		RaiseMoney();
	void		NextCardGame();

	void		ChangeCardPosition();
	void		OnGameKey(int nVKey);

	int			GetWinGame() { return m_nWinGame; }
	void		InitWinGame() { m_nWinGame = -1; }

	void		PlusWinCnt() { m_nWinCnt++; }
	void		PlusLoseCnt() { m_nLoseCnt++; }
	void		PlusDrawCnt() { m_nDrawCnt++; }

	int			GetWinCnt() { return m_nWinCnt; }
	int			GetLoseCnt() { return m_nLoseCnt; }
	int			GetDrawCnt() { return m_nDrawCnt; }

	void		InitWinLoseCnt() { m_nWinCnt = 0; m_nLoseCnt = 0; m_nDrawCnt = 0;}

	int			GetGameContinue() { return m_nGameContinue;}
	bool		GetMoveCard() { return m_bMoveCard; }

	bool		GetBetEnabled() { return m_bBetEnabled; }
	void		SetBetEnalbed( bool b) { m_bBetEnabled = b; }
	
private:
	
	CCardEx Card[MINI_TOTALCARD];		   // 카드저장 클래스 // ###[8포커]###	
	CRect CardRect[MINI_TOTALCARD];		  // 카드 바꾸기 rect 값

	CPage *pPage;

	bool	m_bWindCardEnd;
	
	int		TotalCardNum;
	int		TmpTotalCardNum;
	int		m_ChangeCardIndex;			// 바꾼 인덱스 
	int		m_ChangeCardNum;			// 바꾼 숫자 
	int		m_SelectCard;				// 선택된 카드 인덱스 

	int		m_returnChangeCardIndex;	// 보낸 숫자와 비교할때 쓴다
	int		m_returnChangeCardNum;		// 보낸 숫자와 비교할때 쓴다
	int		m_returnCardNum;			// 바꾸기 요청을 해서 새로 받아온 숫자
	
	int		m_nXp;
	int		m_nYp;

	int		m_nLimitXp[MINI_TOTALCARD];


	int		m_nBankMoney;
	int		m_nWinMoney;

	int		m_nGameContinue;
	bool	m_bPlayGame;

	int		m_nMoveIndex;
	bool	m_bMoveCard;

	int		m_nNextGame;

	int		m_nWinGame;					// -1:None 0:Win 1:Lose 

	// Grphic Count
	int		m_nWinCnt;
	int		m_nLoseCnt;
	int		m_nDrawCnt;

	bool	m_bBetEnabled;

	int		m_nPrevWinMoney;
};

#endif
