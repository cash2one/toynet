// CardDeck.cpp: implementation of the CMiniGame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "MiniGame.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiniGame::CMiniGame()
{
	Reset();
}

CMiniGame::~CMiniGame()
{

}

// 절대 위치 각 사용자 카드 위치
void CMiniGame::Init(CPage *ppage)
{
	if(ppage == NULL) return;
	
	pPage = ppage;

	Reset();
}


// 초기화
void CMiniGame::Reset()
{
	TotalCardNum = 0;

	m_nXp= 122;
	m_nYp = 250;
	
	m_nLimitXp[0] = 122;
	m_nLimitXp[1] = 245;
	m_nLimitXp[2] = 368;
	m_nLimitXp[3] = 491;

	m_nGameContinue = 0;

	for(int i =0; i < MINI_TOTALCARD; i++) Card[i].Clear();
}

void CMiniGame::ResetMoney()
{
	//m_nWinMoney = 100;
	m_nBankMoney = 0;
}

// 새로운 카드 셋팅
void CMiniGame::NewCardSet(int cardNum)
{
	if(TotalCardNum < 0 || TotalCardNum >= MINI_TOTALCARD) return;// ###[8포커]###
	
	Card[TotalCardNum].Init(pPage, cardNum, 0);
	Card[TotalCardNum].bShow = TRUE;

	Card[TotalCardNum].SetPos(m_nXp+TotalCardNum*123, m_nYp);

	POINT pt;
	pt.x = 700;
	pt.y = 250;
	SetCardCurPos(TotalCardNum, pt);

	TotalCardNum++;
}

void CMiniGame::Draw( BOOL bFold) 
{
	for(int i = 0; i < TotalCardNum; i++)
		Card[i].DrawForMini( bFold );

}

BOOL CMiniGame::GetFace(int index)
{
	return Card[index].bFront;

}

// 카드의 앞면
void CMiniGame::SetFaceUp(int index)
{
	if(index < 0 || index >= MINI_TOTALCARD) return;
	Card[index].bFront = TRUE;
}

// 카드의 뒷면 
void CMiniGame::SetFaceDown(int index)
{
	if(index < 0 || index >= MINI_TOTALCARD) return;
	Card[index].bFront = FALSE;
}

void CMiniGame::OnTimer()
{
	MoveCard(0);
	MoveCard(1);
	MoveCard(2);
	MoveCard(3);
}

void CMiniGame::MoveCard(int nIndex )
{
	POINT pt;
	
	pt = GetCardCurPos(nIndex);

	if( !CheckCardOutRage(nIndex) )
		pt.x -= MOVE_RAGE_X;
	else
	{
		pt.x = m_nLimitXp[nIndex];
		SetCardCurPos(nIndex, pt);
		if( nIndex != 3)
			SetFaceUp(nIndex);
		return;
	}
	
	SetCardCurPos(nIndex, pt);
}

bool CMiniGame::CheckCardOutRage(int nIndex)
{
	POINT pt;
	pt = GetCardCurPos(nIndex);

	if( m_nLimitXp[nIndex] <= (pt.x-MOVE_RAGE_X) )
		return FALSE;
	else
		return TRUE;

	return FALSE;
}

void CMiniGame::SetCardCurPos(int nIndex, POINT pt)
{
	Card[nIndex].m_move.SetCurPos(pt);
}

POINT CMiniGame::GetCardCurPos(int nIndex )
{
	return Card[nIndex].m_move.GetCurPos();
}

void CMiniGame::InitGame()
{
	ResetMoney();
	PreCardGame();
}

void CMiniGame::PreCardGame()
{
	Reset();
	PreCardSet();	
}

void CMiniGame::PreCardSet()
{
	int nCardNum;
	for( int i=0; i<MINI_TOTALCARD; i++ )
	{
		nCardNum = LoopToCheckCardNum(i);
		NewCardSet(nCardNum);
	}
}

int CMiniGame::LoopToCheckCardNum(int nIndex)
{
	int nCardNum;
	while( true )
	{
		nCardNum = CheckDuplicatedCardNum(nIndex);

		if( nCardNum != DUMY_CARD )
		{
				return nCardNum;
		}
		else
			continue;
		
	}
}

int CMiniGame::CheckDuplicatedCardNum(int nIndex)
{
	int nCardNum = rand()%(DUMY_CARD-1);

	for( int i=0; i<TotalCardNum; i++)
	{
		if( nIndex != i )
		{
			if( CompareCardNum( i, nCardNum) )
				return DUMY_CARD;
			else
				return nCardNum;
		}
	}

	return nCardNum;
}

bool CMiniGame::CompareCardNum(int nIndex, int nCardNum)
{
	if( Card[nIndex].CardNo == nCardNum )
		return TRUE;
	else
		return FALSE;

	return FALSE;
}


// 0: Low
// 1: Hi
// 2: Draw
int CMiniGame::CheckCardRank()
{
	int nCardNum = Card[3].CardNo;

	nCardNum = nCardNum%13;

	if( nCardNum < DRAW_CARD_INDEX )
		return 0;
	else if( nCardNum > DRAW_CARD_INDEX )
		return 1;
	else if ( nCardNum == DRAW_CARD_INDEX )
		return 2;

	return 2;
}

// 0: Win
// 1: Lose
// 2: Draw
int CMiniGame::CheckWinGame(int nSelectRank)
{
	int nResultRank = CheckCardRank();

	SetFaceUp(3); 

	if(nResultRank == 2)
		return 2;

	if( nSelectRank == nResultRank)
		return 0;
	else
		return 1;
}

void CMiniGame::SetGameResult(int nGameContinue)
{
	m_nGameContinue = nGameContinue;
}

int CMiniGame::GetGameResult()
{
	return m_nGameContinue;
}


// Save Money
void CMiniGame::IsGame()
{
	if( m_nGameContinue == 0 )				//Win
	{
		m_nBankMoney = m_nWinMoney + m_nBankMoney;
		m_nWinMoney = m_nWinMoney * 2;
		PreCardGame();
	}
	else if( m_nGameContinue == 1)			//Lose
		DefeatGame();
	else if( m_nGameContinue == 2)			//Draw
		PreCardGame();
}

void CMiniGame::StopGame()
{
	if( m_nGameContinue == 0 )
	{
		m_nBankMoney = m_nWinMoney + m_nBankMoney;

		// 개인정보의 캐쉬에 저장 - jeong
		g_MyInfo.UI.PMoney += m_nBankMoney;
	}



	g_Mini.SendMessage(WM_CLOSE,0,0);
}

void CMiniGame::DefeatGame()
{
	g_Mini.SendMessage(WM_CLOSE,0,0);
}








