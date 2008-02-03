// CardDeck.h: interface for the CCardDeck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARDDECK_H__BA644A7F_D5BD_4F61_B013_B0BCFC25AF53__INCLUDED_)
#define AFX_CARDDECK_H__BA644A7F_D5BD_4F61_B013_B0BCFC25AF53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CardEx.h"

#define SELECT_TOTALCARD 8  //6 by softpark  // ###[8포커]###
#define EMPTY -1 // 빈상태 (62Cut)
#define _CARD_YPOS_OFFSET 5

// 7명의 카드덱
class CCardDeck  
{
public:
	
	int PNum;			   // 플레이어 넘버
	CCardEx Card[SELECT_TOTALCARD];		   // 카드저장 클래스 // ###[8포커]###	
	CRect CardRect[SELECT_TOTALCARD];		// 카드 바꾸기 rect 값

	CPage *pPage;
	
	int TotalCardNum;
	// 나의 카드덱 위치
	int TmpTotalCardNum;
	int m_ChangeCardIndex;			// 바꾼 인덱스 
	int m_ChangeCardNum;		// 바꾼 숫자 

	int m_SelectCard;		// 선택된 카드 인덱스 
	BOOL bWindCardEnd;
	
	int m_returnChangeCardIndex;			// 보낸 숫자와 비교할때 쓴다
	int m_returnChangeCardNum;		// 보낸 숫자와 비교할때 쓴다
	int m_returnCardNum;			// 바꾸기 요청을 해서 새로 받아온 숫자

	int Xp;
	int Yp;

	int m_xp[2], m_yp[2];

public:
	CCardDeck();
	virtual ~CCardDeck();

	void carddeck_changeinitpos();

	void Init(CPage *ppage, int pnum);

	void Reset();
	void NewCardSet(int cardNum);
	int GetCardNo(int index);
	void DrawXY();
	void SetFaceUp(int index);
	void SetFaceDown(int index);
	void CardSet(int cardnum);
	void OnTimer();
	void Draw( BOOL bFold = FALSE);

	BOOL GetFace(int index);

	//62cut
	void ReSetCardRect();
	void SetCardRect();					
	BOOL SetCardPosition(int index,int pnum,int changenum,int num,int delay=0);	
	void SetChangeCard();			//바꾼카드 셋팅
	BOOL PtInCardRect(int xp,int yp,int flg=0);//0 onclick 1 dbclick
	void SetChangeSelectCard();		//바꾸기 요청한카드 표시

};

#endif // !defined(AFX_CARDDECK_H__BA644A7F_D5BD_4F61_B013_B0BCFC25AF53__INCLUDED_)
