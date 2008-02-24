// CardEx.cpp: implementation of the CCardEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "CoverCard.h"
#include "global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int nLimitYp = 462;

CCoverCard::CCoverCard()
{
	Clear();
}

CCoverCard::~CCoverCard()
{

}

void CCoverCard::Init(CPage *ppage, int cardnum, int pnum)
{
	Clear();
	CCard::Init(ppage,cardnum,pnum);	
}

void CCoverCard::Clear()
{
	CCard::Clear();
	bSelect = FALSE;
	bChangeCard = FALSE;
	bButtonDown = FALSE;
	bButtonMove = FALSE;
	bBUttonUp = FALSE;
	bDraw = FALSE;

	nMouseOffSetX = 470;
	nMouseOffSetY = 470;
	nKeyDownTimes = 0;
	bKeyDown = TRUE;
}

void CCoverCard::Reset()
{
	CCard::Reset();
	bSelect = FALSE;
	bChangeCard = FALSE;
	bAllowControl = FALSE;
	bButtonDown = FALSE;
	bButtonMove = FALSE;
	bBUttonUp = FALSE;
	bDraw = FALSE;
}


void CCoverCard::Draw( int nYp )
{
	if( Game.FlyWindCard == FALSE )
	{
		const int nLimitXp = 437;

		if ( nYp < nLimitYp )
			nYp = nLimitYp;
		
		bShow = TRUE;
		CPoint pt( nLimitXp, nYp );
		m_move.SetCurPos(pt);
		CCard::Draw();
	}
}

BOOL CCoverCard::OnLButtonUp(int xp, int yp)
{
	const int nOopenGap = 40;
	if( bAllowControl && Game.FlyWindCard == FALSE && bButtonMove == TRUE )
	{
		if ( nYpMove > nLimitYp + nOopenGap)
		{
			nYpMove = 700;
			bAllowControl = FALSE;
		}
		else if ( nYpMove <= nLimitYp + nOopenGap)
			nYpMove = nLimitYp;

		Game.CardDeck[0].CoverDraw(nYpMove);

		bButtonDown = FALSE;
		bButtonMove = FALSE;
	}

	return TRUE;
}

BOOL CCoverCard::OnLButtonDown(int xp, int yp)
{
	if( bAllowControl && Game.FlyWindCard == FALSE )
	{
		if( Game.CardDeck[0].Card[6].PtInCard(xp, yp) )
		{	
			nYpDown = yp;
			bButtonDown = TRUE;
			Game.CardDeck[0].Draw(TRUE);
			 
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCoverCard::OnLButtonMove(int xp, int yp)
{
	if( bAllowControl && Game.FlyWindCard == FALSE )
	{
		if( bButtonDown )
		{	
			int nGapMove = yp - nYpDown;
			nYpMove = nGapMove+nLimitYp;
			bButtonMove = TRUE;
			Game.CardDeck[0].SetFaceUp(6);
			Game.CardDeck[0].CoverDraw(nYpMove);
			
			return TRUE;
		}
	}

	return FALSE;
}

void CCoverCard::OnGameKey(int nVKey)
{
	switch(nVKey)
	{
		case 55:				// 7번 키
		{
			if( bAllowControl && Game.FlyWindCard == FALSE )
			{
				nMouseOffSetY = nMouseOffSetY + nKeyDownTimes;
				if( bKeyDown )
				{
					OnLButtonDown(nMouseOffSetX, nMouseOffSetY);
					bKeyDown = FALSE;
				}
	
				OnLButtonMove(nMouseOffSetX, nMouseOffSetY);

				nKeyDownTimes++;
			}	
		}
		break;
		case 56:				// 8번 키
		{

		}
		break;
	}
}

void CCoverCard::OnGameKeyUp(int nVKey)
{
	switch(nVKey)
	{
		case 55:				// 7번 키
		{
			if( bAllowControl && Game.FlyWindCard == FALSE )
			{
				OnLButtonUp(nMouseOffSetX, nMouseOffSetY);
				bKeyDown = TRUE;
				nKeyDownTimes = 0;
				nMouseOffSetY = 470;
			}
		}
		break;
		case 56:				// 8번 키
		{
				nMouseOffSetY = 550;
				bButtonDown = TRUE;
				//bButtonMove = TRUE;
				OnLButtonMove(nMouseOffSetX, nMouseOffSetY);
				OnLButtonUp(nMouseOffSetX, nMouseOffSetY);
		}
		break;
	}
}