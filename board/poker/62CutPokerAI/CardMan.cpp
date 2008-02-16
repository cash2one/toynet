// CardMan.cpp: implementation of the CCardMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NMClnt.h"
#include "NMClntDlg.h"
#include "CardMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCardMan::CCardMan()
{
	m_nTotalCard = 16; // ########### 카드데크 갯수 설정 ###########

	m_nSCardW = 33;
	m_nSCardH = 45;
	m_nBCardW = CARD_W;
	m_nBCardH = CARD_H;

//	m_Deck.Init();
//	m_Card.Init();
}

CCardMan::~CCardMan()
{
	Clear();
}

BOOL CCardMan::Init()
{
//	if(!m_Deck.Open(g_AppStartPath+g_strDataDir+"CARDDECK.lst"))  return FALSE;
//	if(!m_Card.Open(g_AppStartPath+g_strDataDir+"CARD.lst"))  return FALSE;

	return TRUE;
}

void CCardMan::Clear()
{
	m_Deck.ClearMem();
	m_Card.ClearMem();
}

void CCardMan::Draw(int nX,int nY,CDC *pDC,int nCard)
{
	if(pDC==NULL) return;

	if(nCard<0 || nCard>=52) return;
	m_Deck.Draw(3,pDC,nX,nY,m_nBCardW,m_nBCardH, 0,0,SRCAND);
	if(nCard<13){
		m_Card.Draw(0,pDC,nX,nY,m_nBCardW,m_nBCardH, m_nBCardW*nCard,0,SRCPAINT);
		return;
	}
	if(nCard<26){
		m_Card.Draw(1,pDC,nX,nY,m_nBCardW,m_nBCardH, m_nBCardW*(nCard-13),0,SRCPAINT);
		return;
	}
	if(nCard<39){
		m_Card.Draw(2,pDC,nX,nY,m_nBCardW,m_nBCardH, m_nBCardW*(nCard-26),0,SRCPAINT);
		return;
	}
	if(nCard<52){
		m_Card.Draw(3,pDC,nX,nY,m_nBCardW,m_nBCardH, m_nBCardW*(nCard-39),0,SRCPAINT);
		return;
	}
}

void CCardMan::DrawDeck(int nX,int nY,CDC *pDC,int nCard, int nStyle/*=2*/)
{
	if(pDC==NULL) return;

	if(nCard<0 || nCard>=m_nTotalCard) return;
	switch(nStyle)
	{
	case 0: // 작은카드
		m_Deck.Draw(0,pDC,nX,nY,m_nSCardW,m_nSCardH, 0,0,SRCAND);
		m_Deck.Draw(1,pDC,nX,nY,m_nSCardW,m_nSCardH, m_nSCardW*nCard,0,SRCPAINT);
		break;
	case 1: // 작은 궁금카드
		m_Deck.Draw(0,pDC,nX,nY,m_nSCardW,m_nSCardH, 0,0,SRCAND);
		m_Deck.Draw(2,pDC,nX,nY,m_nSCardW,m_nSCardH, 0,0,SRCPAINT);
		break;
	case 2: // 기본 큰카드
		m_Deck.Draw(3,pDC,nX,nY,m_nBCardW,m_nBCardH, 0,0,SRCAND);
		m_Deck.Draw(4,pDC,nX,nY,m_nBCardW,m_nBCardH, m_nBCardW*nCard,0,SRCPAINT);
		break;
	case 3: // 궁금 큰카드
		m_Deck.Draw(3,pDC,nX,nY,m_nBCardW,m_nBCardH, 0,0,SRCAND);
		m_Deck.Draw(5,pDC,nX,nY,m_nBCardW,m_nBCardH, 0,0,SRCPAINT);
		break;
	}
}
