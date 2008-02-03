// Raise.h: interface for the CRaise class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAISE_H__1263E3C1_7A22_11D4_804D_0050BF0FBDAF__INCLUDED_)
#define AFX_RAISE_H__1263E3C1_7A22_11D4_804D_0050BF0FBDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ������ ��ȣ.
enum ERAISESIGN{
	ERAISE_NOSIGN=0,
	ERAISE_FOLD, // ����
	ERAISE_BAT,  // ������
	ERAISE_CALL, // ��
	ERAISE_CHECK,// üũ
	ERAISE_DUMMY,
};

enum ROUNDSTATE{
	RAISE_0 =0,	// �ʱ� 0
	RAISE_1,		// 3�� + 1 ���� (���� ����)
	RAISE_4,		// 4��  2   ���� 6�� ������
	RAISE_5,		// Hidden (7��°) ###change 1 raise
	//RAISE_6,		// change 2 raise softpark
	RAISE_END,	// ��� 
};

#define RAISE_USER (5) // �ִ� 5��

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
	INT64 GetRaiseBat(); // �ƽø� ������ ���Ѵ�
	INT64 GetTotalBat(); // �Ҵ�� ��Ż ������ ���Ѵ�
	int GetRound();

	BOOL RaiseCommand(int nNum, int nUNum, enum ERAISESIGN sign, 
						  BOOL bCheck,int nBtn, int &nNext,BOOL &bNextBtnBat,BOOL &bNextBtnCheck);
	void Start(PLAYSTATE *ps);
	void Set(int nRaise,int nBat);
	void Init(int nBat);
	void Reset();
	CRaise();
	virtual ~CRaise();

	RAISEUSER m_User[RAISE_USER]; // �ִ� 6��.

	int    m_nRound;      // 1��,2��,3��,4��...
	int    m_nStyle;      // ������ ��Ÿ��
	int    m_nTotalRaise; // ��Ż ������ Ƚ��.

	int    BetMoney;

	INT64  m_nMaxTotal,m_nMaxBat,m_nDouble;
	INT64  m_nRaiseBat; // ������ ���þ��� �ְ�ġ.
	double m_fBatRate;
};

#endif // !defined(AFX_RAISE_H__1263E3C1_7A22_11D4_804D_0050BF0FBDAF__INCLUDED_)
