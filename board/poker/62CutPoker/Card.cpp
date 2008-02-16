// Card.cpp: implementation of the CCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "Card.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int SHADOW_SIZE = 1;
const int SPR_WIDTH = 71;
const int SPR_HEIGHT = 96;

BOOL bShowShadow = FALSE;
CCard::CCard()
{
	Clear();
}

CCard::~CCard()
{
}

void CCard::Init(CPage *ppage, int cardnum, int pnum)
{
	Clear();
	pPage = ppage;
	CardNo = cardnum;
	Where = 0;	//  카드덱 0, 유저 1
	PNum = pnum;
	m_size.x = 62;
	m_size.y = 83;
}

void CCard::Clear()
{
	pPage = NULL;
	ZeroMemory(&Event, sizeof(CARDEVENT));
	ZeroMemory(&Event1, sizeof(CARDEVENT));

	// ### [ 관전기능 ] ###
	m_move.Reset();
	
	CardNo = 0;
	PNum  = -1;
	Where = 0;
	Kind = 0;
	Attrib = 0;
	ShowMode = 0;

	bNotifyEvent = FALSE;
	NotifyString.Format("");

	bShow = FALSE;
	bFront = FALSE;
	bMove = FALSE;
	bAccelMove = FALSE;
	bEvent = FALSE;

	Xp = Yp = 0;
	SXp = SYp = 0;
	TXp = TYp = 0;
	MoveDelay = 0;
	EndMoveDelay = 0;
	MoveSpeed = 65;

	bTwincle = FALSE;
	TwincleCnt = 0;

	bGlass = FALSE;	
}

void CCard::Reset()
{
	ZeroMemory(&Event, sizeof(CARDEVENT));
	ZeroMemory(&Event1, sizeof(CARDEVENT));
	Where = 0;
	ShowMode = 0;

	bNotifyEvent = FALSE;
	NotifyString.Format("");
	bShow = FALSE;
	bFront = FALSE;
	bMove = FALSE;
	bAccelMove = FALSE;
	bEvent = FALSE;
	bEvent1 = FALSE;

	Xp = Yp = 0;
	SXp = SYp = 0;
	TXp = TYp = 0;
	MoveDelay = 0;
	EndMoveDelay = 0;
	MoveSpeed = 50;

	bTwincle = FALSE;
	TwincleCnt = 0;

	bGlass = FALSE;	
}

void CCard::SetPos(int tx, int ty)
{
	SXp = TXp = Xp = tx;
	SYp = TXp = Yp = ty;
}

// 1 번째 이벤트
void CCard::SetEvent(int eventcode, int option, int option1, int option2)
{
	//memcpy(&Event, pEvent, sizeof(CARDEVENT));
	ZeroMemory(&Event, sizeof(CARDEVENT));

	bEvent = TRUE;
	Event.CardNo = CardNo;
	Event.EventCode = eventcode;
	Event.Option = option;
	Event.Option2  = option1;
}

// 2번째 이벤트
void CCard::SetEvent1(int eventcode, int option, int option1, int option2)
{
	//memcpy(&Event, pEvent, sizeof(CARDEVENT));
	ZeroMemory(&Event1, sizeof(CARDEVENT));

	bEvent1 = TRUE;
	Event1.CardNo = CardNo;
	Event1.EventCode = eventcode;
	Event1.Option = option;
	Event1.Option2  = option1;
}


void CCard::ClearEvent()
{
	ZeroMemory(&Event, sizeof(CARDEVENT));
	bEvent = FALSE;
}

void CCard::ClearEvent1()
{
	ZeroMemory(&Event1, sizeof(CARDEVENT));
	bEvent1 = FALSE;
}

void CCard::SetNotifyEvent(char *pstr, COLORREF color)
{
	bNotifyEvent = TRUE;
	NotifyStringColor = color;
	NotifyString.Format("%s", pstr);
}

void CCard::ClearNotifyEvent()
{
	bNotifyEvent = FALSE;
	NotifyStringColor = 0;
	NotifyString.Format("");
}

void CCard::SetMoveStartSnd(SND_ENUM sound)
{
//	MoveStartSnd = sound;
}

void CCard::SetMoveEndSnd(SND_ENUM sound)
{
//	MoveEndSnd = sound;
}

void CCard::SetMove(int tx, int ty, int sdelay, int edelay, int speed)
{
	SXp = Xp; SYp = Yp;
	TXp = tx; TYp = ty;
	bMove = TRUE;
	bAccelMove = FALSE;
	MoveDelay = sdelay;
	EndMoveDelay = edelay;
//	if(speed == -1) MoveSpeed = Game.CardSpeed;
//	else MoveSpeed = speed;
}

void CCard::SetAccelMove()
{
	bAccelMove = TRUE;
	AccelSpeed = 1;
}

void CCard::OnTimer()
{
	m_move.OnTimer( 1 ); // 속도
	if( m_move.IsStopped() )
	{
		// 이벤트가 있다면 이벤트를 처리
	//	
		
		if(bEvent)
		{
			if(Event.EventCode == EVENT_THREEFLYEND)
			{
				Game.OnCardEvent(&Event);
				ClearEvent();
			}
			else if(Event.EventCode == EVENT_ONEFLYEND)
			{
			//	m_move.PlayMoveSound(SND22);
				Game.OnCardEvent(&Event);
				ClearEvent();
			} 
			else if(Event.EventCode == EVENT_CHANGEDEL_ONEFLYEND)
			{
				Game.OnCardEvent(&Event);
			//	ClearEvent();
			}
			else if(Event.EventCode == EVENT_CHANGESET_ONEFLYEND)
			{
				Game.OnCardEvent(&Event);
				ClearEvent();
			}
		}
		
		if(bEvent1)
		{
			if(Event1.EventCode == EVENT_ONEFLYTOTEND)
			{
				Game.OnCardEvent(&Event1);
				ClearEvent1();
			}
		}
	}
}

void CCard::Draw( BOOL bFold)
{
	if(pPage == NULL) return;
	if(bShow == FALSE) return;

	if(m_move.GetDelay() > 0) return;

	int sprno = (bFront)? CardNo : DUMY_CARD;
	POINT pt = m_move.GetCurPos();
	int xp = pt.x;
	int yp = pt.y;

	// 폴드시 흐리게 그린다.
	if( bFold )
		pPage->PutSprAuto(xp, yp, &BigCardSpr, sprno, GLOW, 28);//,RGBmix(0,10,10));
	else
		pPage->PutSprAuto(xp, yp, &BigCardSpr, sprno);
}


void CCard::DrawXY(int x, int y)
{

	if(pPage == NULL) return;
	if(bShow == FALSE) return;

	int sprno = (bFront)? CardNo : DUMY_CARD;
	POINT pt = m_move.GetCurPos();
	int xp = pt.x;
	int yp = pt.y;
	pPage->PutSprAuto(xp, yp, &BigCardSpr, sprno);

}

BOOL CCard::PtInCard(int x, int y)
{
	BOOL bOPen =  FALSE;
	CRect rc;
	POINT cur;//m_move.GetCurPos();
	cur.x = Xp;
	cur.y = Yp;
	rc.SetRect( cur.x, cur.y, cur.x + m_size.x, cur.y + m_size.y );
	
	POINT pt;
	pt.x = x;
	pt.y = y;

	if(rc.PtInRect( pt )) 
	{
		bOPen = TRUE;		
	//	bFront = TRUE;
	}
	
	return bOPen;
}

void CCard::StraightMoveTo( POINT dest, float spd_gamma, float acc_gamma, int delay )
{
	POINT cur = m_move.GetCurPos();
	FPOINT spd;
	spd.x = spd.y = 0;
	spd.x = (float)(dest.x - cur.x);
	spd.y = (float)(dest.y - cur.y);
	FPOINT acc = spd;

	FLOAT temp_spdgamma = 5.0f;

	if( spd_gamma == 0.0f )	spd.x = spd.y = 0;
	else
	{
		spd.x /= max( spd_gamma, temp_spdgamma );
		spd.y /= max( spd_gamma, temp_spdgamma );
	}

	FLOAT temp_accgamma = 5.0f;
	if( acc_gamma == 0.0f )	acc.x = acc.y = 0;
	else
	{
		acc.x /= max( acc_gamma, temp_accgamma );
		acc.y /= max( acc_gamma, temp_accgamma );
	}

	m_move.StraightMoveTo( dest, spd, acc, delay );
}