// CoverCard.h: interface for the CCoverCard class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COVER_CARD__
#define __COVER_CARD__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Card.h"

class CCoverCard  : public CCard
{
public:
	CCoverCard();
	virtual ~CCoverCard();

	//62cut 선택시 상태 변수
	BOOL bSelect;
	BOOL bChangeCard;
	
	BOOL bAllowControl;
	BOOL bButtonDown;
	BOOL bButtonMove;
	BOOL bBUttonUp;
	BOOL bDraw;
	int	nYpDown;
	int nYpMove;

	void Init(CPage *ppage, int cardnum, int pnum);	// 카드 셋팅
	void Clear();
	void Reset();

	void SetChangeCardCheck(BOOL flg){bChangeCard = flg;}
	BOOL OnLButtonUp(int xp, int yp);
	BOOL OnLButtonDown(int xp, int yp);
	BOOL OnLButtonMove(int xp, int yp);

	void Draw( int nYp );
};

#endif // !defined(AFX_CARDEX_H__44578D41_E3A7_4A5A_B136_8627AC83F0F5__INCLUDED_)
