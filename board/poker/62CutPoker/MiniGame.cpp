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

	m_nGameContinue = -1;

	m_nWinGame = -1;

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
	if(TotalCardNum < 0 ) return;// ###[8포커]###
	
	Card[TotalCardNum].Init(pPage, cardNum, 0);
	Card[TotalCardNum].bShow = TRUE;

	Card[TotalCardNum].SetPos(m_nXp+TotalCardNum*CARD_GAP_SIZE, m_nYp);

	POINT pt;
	pt.x = 640;
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
	if(index < 0 || index >= TotalCardNum) return;
	Card[index].bFront = TRUE;
}

// 카드의 뒷면 
void CMiniGame::SetFaceDown(int index)
{
	if(index < 0 || index >= TotalCardNum) return;
	Card[index].bFront = FALSE;
}

void CMiniGame::OnTimer()
{
	if( m_bPlayGame || m_bMoveCard )
	{
		if( m_nMoveIndex >= TotalCardNum )
			m_nMoveIndex = m_nNextGame;

			MoveCard(m_nMoveIndex);
	}
}

void CMiniGame::MoveCard(int nIndex )
{
	POINT pt;
	
	pt = GetCardCurPos(nIndex);

	if( !CheckCardOutRage(nIndex) )
		pt.x -= MOVE_RAGE_X;
	else
	{
		pt.x = m_nLimitXp[nIndex-m_nNextGame];
		SetCardCurPos(nIndex, pt);

		if( nIndex != (TotalCardNum-1) )
			SetFaceUp(nIndex);

		m_nMoveIndex++;

		if( m_bMoveCard )
			Sound.Play(SND16);

		if( TotalCardNum == m_nMoveIndex )
			m_bMoveCard = FALSE;
		
		return;
	}
	
	SetCardCurPos(nIndex, pt);
}

bool CMiniGame::CheckCardOutRage(int nIndex)
{
	POINT pt;
	pt = GetCardCurPos(nIndex);

	if( m_nLimitXp[nIndex-m_nNextGame] <= (pt.x-MOVE_RAGE_X) )
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

	m_bPlayGame = FALSE;
	m_nMoveIndex = 0;
	m_nNextGame = 0;

	g_Mini.bInitGame = TRUE;

	m_bBetEnabled = TRUE;
}

void CMiniGame::PreCardGame()
{
	Reset();
	PreCardSet();	
	m_bMoveCard = TRUE;
}

void CMiniGame::NextCardGame()
{
	Sound.Play(SND_ONEMORE);
	InitWinGame();
	
	int nCardNum;
	
	nCardNum = LoopToCheckCardNum(TotalCardNum);
	NewCardSet(nCardNum);

	m_nNextGame++;
	ChangeCardPosition();
	m_bMoveCard = TRUE;
	m_nMoveIndex = m_nNextGame;
	m_nGameContinue = -1;
	m_bBetEnabled = TRUE;
	m_bPlayGame = TRUE;
}

void CMiniGame::ChangeCardPosition()
{
	int nChangedXp;
	for(int i = 0; i<TotalCardNum; i++)
	{
		nChangedXp = Card[i].SXp - m_nNextGame*CARD_GAP_SIZE;

		Card[i].SetPos(nChangedXp, m_nYp);
	}
}

void CMiniGame::PreCardSet()
{
	srand(time(NULL));
	int nCardNum = rand()%DUMY_CARD;
	for( int i=0; i<4; i++ )
	{
		NewCardSet(nCardNum);
		nCardNum = LoopToCheckCardNum(i+1);
	}
}

int CMiniGame::LoopToCheckCardNum(int nIndex)
{
	//return 6;
	int nCardNum;
	for(int i=0; i<DUMY_CARD; i++)
	{
		srand(i*time(NULL));
		nCardNum = CheckDuplicatedCardNum(nIndex);

		if( nCardNum != DUMY_CARD )
		{
			return nCardNum;
		}
		else
			continue;
		
	}

	return nCardNum;
}

int CMiniGame::CheckDuplicatedCardNum(int nIndex)
{

	int nCardNum = rand()%DUMY_CARD;

	for( int i=0; i<TotalCardNum; i++)
	{
		if( CompareCardNum( i, nCardNum) )
			return DUMY_CARD;	
	}

	return nCardNum;
}

bool CMiniGame::CompareCardNum(int nIndex, int nCardNum)
{
	if( Card[nIndex].CardNo == nCardNum )
		return TRUE;
	else
		return FALSE;

	return TRUE;
}


// 0: Low
// 1: Hi
// 2: Draw
int CMiniGame::CheckCardRank()
{
	int nCardNum = Card[TotalCardNum-1].CardNo;

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

	SetFaceUp(TotalCardNum-1);

	m_bPlayGame = FALSE;

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

	m_nWinCnt = 0;
	m_nLoseCnt = 0;
	m_nDrawCnt = 0;

	m_bBetEnabled = FALSE; 

	if( m_nGameContinue == 0 )
	{
		//승리
		RaiseMoney();
		m_nWinGame = 0;
		//Sound.Play(SND32);
		Sound.Play(SND_FWIN);
	}
	else if( m_nGameContinue == 1 ){
		//패배
		m_nWinGame = 1;
		//Sound.Play(SND33);
		Sound.Play(SND_FLOSE);
		DefeatGame();
	}
	else if( m_nGameContinue == 2 ){
		//드로우
		m_nWinGame = 2;
		Sound.Play(SND_DRAWBONUS);
	}

	
}

int CMiniGame::GetGameResult()
{
	return m_nGameContinue;
}

void CMiniGame::RaiseMoney()
{
	m_nBankMoney = m_nWinMoney + m_nBankMoney;
	m_nWinMoney = m_nWinMoney * 2;
	
}
void CMiniGame::IsGame()
{
	if( m_bPlayGame == TRUE )
		return;

	if( m_nGameContinue == 1)			//Lose
		DefeatGame();
	else
		NextCardGame();
}

void CMiniGame::StopGame()
{
	if( m_nGameContinue == 0 || m_nGameContinue == 2 )
	{
		Play[0].BankMoney += m_nBankMoney;

		CSV_ASK_MONEYINFO aumsg;
		aumsg.Set(Play[0].UI.UNum, m_nBankMoney, g_RI.RoomNum);
		SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());
	}

	g_Mini.SendMessage(WM_CLOSE,0,0);
}

void CMiniGame::DefeatGame()
{
	Play[0].BankMoney -= m_nPrevWinMoney;
			
	CSV_ASK_MONEYINFO aumsg;
	aumsg.Set(Play[0].UI.UNum, -m_nPrevWinMoney, g_RI.RoomNum);
	SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());
}

void CMiniGame::OnGameKey(int nVKey)
{
	int nResult;

	if( m_bMoveCard )
		return;

	if( !m_bBetEnabled )
		return;

	switch(nVKey)
	{
		case VK_INSERT:
		{
			nResult = CheckWinGame( 1 );
			SetGameResult( nResult );
			//Sound.Play(SND45);
		}
		break;
		case VK_DELETE:
		{
			nResult = CheckWinGame( 0 );
			SetGameResult( nResult );
			//Sound.Play(SND45);
		}
		break;
	}
}
















































