// SevenCard.h: interface for the CSevenCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEVENCARD_H__899A32C9_6D37_11D4_804D_0050BF0FBDAF__INCLUDED_)
#define AFX_SEVENCARD_H__899A32C9_6D37_11D4_804D_0050BF0FBDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 세븐카드 족보 열거형.
enum SEVEN_CARD{
	N7C_DUMMY=0,

	// 망통.
	N7C_NOPAIR,
	// 원페어.
	N7C_ONEPAIR,
	// 투페어.
	N7C_TWOPAIR,
	// 쓰리카드.
	N7C_TRIPLE,
	// 스트레이트
	N7C_STRAIGHT,
	N7C_BACKS,  //백스트레이트
	N7C_MOUNT,  // 마운틴
	// 플러쉬.
	N7C_FLUSH,
	// 풀하우스
	N7C_TITLE,
	// 포카드
	N7C_QUADS,
	// 스트레이트 플러쉬.
	N7C_SFULSH,
	N7C_BSFLUSH, //백스트레이트 플러시.
	// 로얄스트레이트 플러시.
	N7C_CROYAL,
	N7C_HROYAL,
	N7C_DROYAL,
	N7C_SROYAL   
};

#define TOTAL_CARD 7

class CSevenCard  
{
public:
	BOOL IsSpecial(enum SEVEN_CARD eName,int &nVal);
	INT64 GetSecondVal(int pCard[],int n,enum SEVEN_CARD eCard,int nVal);
	CString GetName(enum SEVEN_CARD eName, int nVal, int *nResult);
	int GetWinner(int pCardVal[], INT64 pSecVal[], int pUser[], int n);
	INT64 GetPairNum(int pCard[],int n);
	int GetFlushPic(int pic[]);
	INT64 GetStraightNum(int pCard[], int nVal);
	INT64 GetFlushNum(int pCard[], int n);
	int GetFlushVal(int a[], int b[], int n, int nPic);
	BOOL IsStraightFlush(int a[], int b[], int n, int nPic, int *pRes,BOOL *pRoyal);
	int SameNum(int a[], int *pStart,int n=7);
	BOOL IsPair(int a[],int resV[],int resN[],int *pNum);
	BOOL IsStraight(int a[],int n, int *pVal);
	int BubbleSort(int a[], int b[], int n);
	int GetValue(int pCard[],int nNum,int *pPrime);	

	CSevenCard();
	virtual ~CSevenCard();

	int N[TOTAL_CARD];
	int P[TOTAL_CARD];
};

#endif // !defined(AFX_SEVENCARD_H__899A32C9_6D37_11D4_804D_0050BF0FBDAF__INCLUDED_)
