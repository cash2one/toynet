// Raise.h: interface for the CRaise class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAISE_H__1263E3C1_7A22_11D4_804D_0050BF0FBDAF__INCLUDED_)
#define AFX_RAISE_H__1263E3C1_7A22_11D4_804D_0050BF0FBDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// 레이즈 신호.
enum ERAISESIGN{
	ERAISE_NOSIGN=0,
	ERAISE_FOLD, // 폴드
	ERAISE_BAT,  // 레이즈
	ERAISE_CALL, // 콜
	ERAISE_CHECK,// 체크
	ERAISE_DUMMY,
};

enum ROUNDSTATE{
	RAISE_0 =0,	// 초기 0
	RAISE_1,		// 3장 + 1 라운드 (최초 배팅)
	RAISE_4,		// 4장  2   현재 6장 배팅중
	RAISE_5,		// Hidden (7장째) ###change 1 raise
	//RAISE_6,		// change 2 raise softpark
	RAISE_END,	// 결과 
};

#define RAISE_USER (5) // 최대 5명

class CRaise  
{
public:
	INT64 GetWinnerHiLowMoney(BOOL bHigh, int nWinner, int nLowWinner);
	INT64 GetFailMoneyLow(int nHighWin, int nLowWin, int nUser);
	INT64 GetRealBet();
	INT64 GetFailMoney(int nWinner,int nUser);
	INT64 GetWinnerBet(int nWinner);
	INT64 GetWinnerMoney(int nWinner);
	BOOL EndRaise();
	BOOL AnalNum(int nNum);
	void SetUser(int nNum,INT64 nSchool,USERINFO &ui);
	INT64 GetRaiseBat(); // 맥시멈 배팅을 구한다
	INT64 GetTotalBat(); // 할당된 토탈 배팅을 구한다
	int GetRound();

	BOOL RaiseCommand(int nNum, int nUNum, enum ERAISESIGN sign, 
						  BOOL bCheck,int nBtn, int &nNext,BOOL &bNextBtnBat,BOOL &bNextBtnCheck);
	void Start(PLAYSTATE *ps);
	void Set(int nRaise,int nBat);
	void Init(int nBat);
	void Reset();
	CRaise();
	virtual ~CRaise();

	RAISEUSER m_User[RAISE_USER]; // 최대 6명.

	int    m_nRound;      // 1구,2구,3구,4구...
	int    m_nStyle;      // 레이즈 스타일
	int    m_nTotalRaise; // 토탈 레이즈 횟수.

	int    BetMoney;

	INT64  m_nMaxTotal,m_nMaxBat,m_nDouble;
	INT64  m_nRaiseBat; // 레이즈 배팅액의 최고치.
	double m_fBatRate;
};

#endif // !defined(AFX_RAISE_H__1263E3C1_7A22_11D4_804D_0050BF0FBDAF__INCLUDED_)
