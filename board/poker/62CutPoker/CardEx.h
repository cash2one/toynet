// CardEx.h: interface for the CCardEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARDEX_H__44578D41_E3A7_4A5A_B136_8627AC83F0F5__INCLUDED_)
#define AFX_CARDEX_H__44578D41_E3A7_4A5A_B136_8627AC83F0F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Card.h"

class CCardEx  : public CCard
{
public:
	CCardEx();
	virtual ~CCardEx();

	//62cut 선택시 상태 변수
	BOOL bSelect;
	BOOL bChangeCard;

	void Init(CPage *ppage, int cardnum, int pnum);	// 카드 셋팅
	void Clear();
	void Reset();

	void SetChangeCardCheck(BOOL flg){bChangeCard = flg;}

	void Draw( BOOL bFold = FALSE );
	void DrawForMini( BOOL bFold);

};

#endif // !defined(AFX_CARDEX_H__44578D41_E3A7_4A5A_B136_8627AC83F0F5__INCLUDED_)
