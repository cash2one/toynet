

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
	void		SetWinMoney(int nMoney) { m_nWinMoney = nMoney; }

	bool		IsPlayGame() { return m_bPlayGame; }

	void		RaiseMoney();
	void		NextCardGame();

	void		ChangeCardPosition();
	void		OnGameKey(int nVKey);

	int			GetWinGame() { return m_nWinGame; }
	void		InitWinGame() { m_nWinGame = -1; }

	void		PlusWinCnt() { m_nWinCnt++; }
	void		PlusLoseCnt() { m_nLoseCnt++; }

	int			GetWinCnt() { return m_nWinCnt; }
	int			GetLoseCnt() { return m_nLoseCnt; }

	void		InitWinLoseCnt() { m_nWinCnt = 0; m_nLoseCnt = 0; }

	int			GetGameContinue() { return m_nGameContinue;}
	bool		GetMoveCard() { return m_bMoveCard; }

	bool		GetBetEnabled() { return m_bBetEnabled; }
	void		SetBetEnalbed( bool b) { m_bBetEnabled = b; }
	
private:
	
	CCardEx Card[MINI_TOTALCARD];		   // ī������ Ŭ���� // ###[8��Ŀ]###	
	CRect CardRect[MINI_TOTALCARD];		  // ī�� �ٲٱ� rect ��

	CPage *pPage;

	bool	m_bWindCardEnd;
	
	int		TotalCardNum;
	int		TmpTotalCardNum;
	int		m_ChangeCardIndex;			// �ٲ� �ε��� 
	int		m_ChangeCardNum;			// �ٲ� ���� 
	int		m_SelectCard;				// ���õ� ī�� �ε��� 

	int		m_returnChangeCardIndex;	// ���� ���ڿ� ���Ҷ� ����
	int		m_returnChangeCardNum;		// ���� ���ڿ� ���Ҷ� ����
	int		m_returnCardNum;			// �ٲٱ� ��û�� �ؼ� ���� �޾ƿ� ����
	
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

	bool	m_bBetEnabled;
};

#endif
