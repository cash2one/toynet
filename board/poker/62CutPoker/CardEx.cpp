// CardEx.cpp: implementation of the CCardEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "CardEx.h"
#include "global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCardEx::CCardEx()
{
	Clear();
}

CCardEx::~CCardEx()
{

}

void CCardEx::Init(CPage *ppage, int cardnum, int pnum)
{
	Clear();
	CCard::Init(ppage,cardnum,pnum);	
}

void CCardEx::Clear()
{
	CCard::Clear();
	bSelect = FALSE;
	bChangeCard = FALSE;
}

void CCardEx::Reset()
{
	CCard::Reset();
	bSelect = FALSE;
	bChangeCard = FALSE;
}


void CCardEx::Draw( BOOL bFold)
{
	CCard::Draw();
	POINT pt = m_move.GetCurPos();
	int xp = pt.x;
	int yp = pt.y;
	// 카드 바꾸기 화살표 [62]
	if(g_bChangeCardStage == TRUE && g_Poker.nState == RSTATE_CHANGECARD
		&& Play[0].JoinState == 1 && Play[0].PlayState == 1 && !g_MyObserver ){
		if(PNum == 0){
			if(bSelect){
				pPage->PutSprAuto(xp, yp-20, &FocusSpr, 7);
			}					
		}
	}
	else{
		bSelect = FALSE;
	}		

	if(bChangeCard)pPage->PutSprAuto(xp-3, yp-3, &FocusSpr, 11);
}