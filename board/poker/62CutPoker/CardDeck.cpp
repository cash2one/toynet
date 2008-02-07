// CardDeck.cpp: implementation of the CCardDeck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "CardDeck.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCardDeck::CCardDeck()
{
	Reset();
	PNum = 0;

}

CCardDeck::~CCardDeck()
{

}

// 절대 위치 각 사용자 카드 위치
void CCardDeck::Init(CPage *ppage, int pnum)
{
	if(ppage == NULL) return;
	if(pnum < 0 || pnum >= MAX_PLAYER) return;

	pPage = ppage;
	PNum = pnum;

	Xp = 0;
	Yp = 0;

	m_ChangeCardIndex = EMPTY;
	m_ChangeCardNum = EMPTY;
	m_returnChangeCardIndex = EMPTY;			// 보낸 숫자와 비교할때 쓴다
	m_returnChangeCardNum  = EMPTY;		// 보낸 숫자와 비교할때 쓴다
	m_returnCardNum = EMPTY;			// 바꾸기 요청을 해서 새로 받아온 숫자

	// 각 플레이어 카드 좌표(절대)
	switch(PNum)
	{
	case 0:
		{
			m_xp[0] = 341; m_yp[0] = 469;

	//		m_xp[1] = 358; m_yp[1] = 471;
		} break;
	case 1:			// AI 카드 위치 변경 - jeong
	case 2:
	case 3:
	case 4:
	case 5:
		{
			m_xp[0] = 275; m_yp[0] = 60;
			
			//	m_xp[1] = 87;m_yp[1] = 275;
		} break;
	}
	/*
	case 1:
		{
			m_xp[0] = 83; m_yp[0] = 263;

		//	m_xp[1] = 87;m_yp[1] = 275;
		} break;
	

	case 2:
		{
			m_xp[0] = 83; m_yp[0] = 96;

	//		m_xp[1] = 87; m_yp[1] = 101;
		} break;

	case 3:
		{
			m_xp[0] = 563; m_yp[0] = 96;

	//		m_xp[1] = 353; m_yp[1] = 49;
		} break;
	case 4:
		{
			m_xp[0] = 563; m_yp[0] = 263;

	///		m_xp[1] = 543; m_yp[1] = 101;
		} break;


//////////////
	case 5:
		{
			m_xp[0] = 550; m_yp[0] = 210;

	//		m_xp[1] = 543; m_yp[1] = 275;
		} break;
	}
	*/

	carddeck_changeinitpos();
}

void CCardDeck::carddeck_changeinitpos()
{
	if(g_RI.bPlayer==0) {
		Xp = m_xp[0];
		Yp = m_yp[0];

	} else {
		Xp = m_xp[1];
		Yp = m_yp[1];
	}
}


// 초기화
void CCardDeck::Reset()
{
	TotalCardNum = 0;
	TmpTotalCardNum = 0;
	m_ChangeCardIndex = EMPTY;
	m_ChangeCardNum = EMPTY;
	m_SelectCard = EMPTY;		// 선택된 카드 인덱스 
	m_returnChangeCardIndex = EMPTY;			// 보낸 숫자와 비교할때 쓴다
	m_returnChangeCardNum  = EMPTY;		// 보낸 숫자와 비교할때 쓴다
	m_returnCardNum = EMPTY;			// 바꾸기 요청을 해서 새로 받아온 숫자
	bWindCardEnd = FALSE;

	for(int i =0; i < SELECT_TOTALCARD; i++) Card[i].Clear();

	CoverCard.Clear();
}

// 새로운 카드 셋팅
void CCardDeck::NewCardSet(int cardNum)
{
	if(TotalCardNum < 0 || TotalCardNum >= SELECT_TOTALCARD) return;// ###[8포커]###
	
	Card[TotalCardNum].Init(pPage, cardNum, PNum); // PNum 소유자
	Card[TotalCardNum].bShow = TRUE;  // 임시 ( FALSE -> 카드 날리기 받았을때 TRUE)
	
	int NewYp = Yp;
	if(PNum == 0 ) // 첫 3장 받았을 경우는 같은 선상에 그린다.
	{
		if( !g_MyObserver && GetFace(2) )// ### [ 관전기능 ] ###
			NewYp = Yp - 7;
	}	
	
	Card[TotalCardNum].SetPos(Xp+TotalCardNum*16, NewYp);

	TotalCardNum++;
	
}

// 히든 카드를 숨길 카드 셋팅 - jeong
void CCardDeck::CoverCardSet(int cardNum)
{
	if(TotalCardNum < 0 || TotalCardNum >= SELECT_TOTALCARD+1) return;// 숨길카드를 위한 체크 
	
	CoverCard.Init(pPage, cardNum, PNum); // PNum 소유자
	CoverCard.bShow = FALSE;
	CoverCard.bFront = FALSE;
}

// 첫부분에서 카드 위치좌표 셋 !! 폴드시 어둡게 그린다.
void CCardDeck::Draw( BOOL bFold) 
{
	for(int i = 0; i < TotalCardNum; i++)
	{
		Card[i].Draw( bFold );
	}

	if ( TotalCardNum == SELECT_TOTALCARD )
		CoverDraw( g_CvCard.nYpMove );
}

void CCardDeck::CoverDraw( int nYp ) 
{
	CoverCard.Draw( nYp );
}



// 좌표값을 준다
void CCardDeck::DrawXY()
{
	// 나의 카드 3장 부터는 약간 위쪽으로 그린다.
	int cnt = 0;
	int NewYp = Yp;

	for(int i = 0; i < TotalCardNum; i++)
	{
		if(PNum == 0 && cnt > 1) // 첫 3장 받았을 경우는 같은 선상에 그린다.
		{
			if(!g_MyObserver && GetFace(2) )// ### [ 관전기능 ] ###
				NewYp = Yp - 7;
		}
		Card[i].DrawXY(Xp+cnt*14, NewYp);
		cnt++;
	}
}

BOOL CCardDeck::GetFace(int index)
{
	return Card[index].bFront;

}

void CCardDeck::OnTimer()
{
	for(int i = 0; i < SELECT_TOTALCARD; i++) Card[i].OnTimer(); //// ###[8포커]###
}

// 임시 사용
void CCardDeck::CardSet(int cardnum)
{
	if(TotalCardNum < 0 || TotalCardNum >= SELECT_TOTALCARD) return;
	
	Card[TotalCardNum].Init(pPage, cardnum, PNum);
	Card[TotalCardNum].bShow = TRUE;
	TotalCardNum++;
}

// 카드의 앞면
void CCardDeck::SetFaceUp(int index)
{
	if(index < 0 || index >= SELECT_TOTALCARD) return;
	Card[index].bFront = TRUE;
}

// 카드의 뒷면 
void CCardDeck::SetFaceDown(int index)
{
	if(index < 0 || index >= SELECT_TOTALCARD) return;
	Card[index].bFront = FALSE;
}

// 0 ~ 6 총 7장
int CCardDeck::GetCardNo(int index)
{
	if(index < 0 || index >= SELECT_TOTALCARD) return 0;
	return Card[index].GetCardNum();
}

void CCardDeck::SetCardRect()
{
	POINT pt = Card[0].m_move.GetCurPos();
	POINT pt1 = Card[1].m_move.GetCurPos();
	m_SelectCard =-1;

	int xSize = pt1.x - pt.x;
	int ySize = 96;

	for(int i = 0 ;  i < SELECT_TOTALCARD ; i++){		
		POINT pt = Card[i].m_move.GetCurPos();		
		if(5  == i){
			CardRect[i].SetRect(pt.x,pt.y,pt.x+74,pt.y+ySize);
		}else CardRect[i].SetRect(pt.x,pt.y,pt.x+xSize,pt.y+ySize);
	}	
}

//62Cut 카드위치 제조정.
BOOL CCardDeck::SetCardPosition(int index,int pnum,int changenum,int num,int delay)
{	
	if(index < 0 || index > 5)return FALSE;
	if(changenum < 0  || changenum > 52)return FALSE;
	if(num < 0  || num > 52)return FALSE;

	//바꾼다고 보낸 카드랑 서버를 거쳐 온 데이타랑 비교 가지고 있는 카드랑 같은지를 비교
	if(pnum == 0)
	{
		if(m_ChangeCardIndex != index && !g_MyObserver)return FALSE;
		if(m_ChangeCardNum != changenum && !g_MyObserver)return FALSE;
	}		

	m_returnChangeCardIndex = index;
	m_returnChangeCardNum = changenum;
	m_returnCardNum = num;			// 바꾸기 요청을 해서 새로 받아온 숫자	

	Card[index].CardNo = DUMY_CARD;	

	Card[index].bShow = TRUE;  // 임시 ( FALSE -> 카드 날리기 받았을때 TRUE)		
	
	CPoint cpt;
	cpt.x = Card[index].Xp;
	cpt.y = Card[index].Yp;
	Card[index].m_move.SetCurPos(cpt);

	bWindCardEnd = TRUE;

	CPoint pt;
	pt.x = 368;
	pt.y = 40;
	Card[index].StraightMoveTo(pt, 10*3, 18*3, delay);					
	Card[index].SetEvent(EVENT_CHANGEDEL_ONEFLYEND, pnum, index ); // 플레이 넘버,인덱스 값	

	if(pnum == 0)Card[m_returnChangeCardIndex].SetChangeCardCheck(FALSE);

	return TRUE;
}

void CCardDeck::SetChangeCard()
{	
	if(m_returnChangeCardIndex == EMPTY)return;
	if(m_returnChangeCardNum == EMPTY)return;

	if(PNum == 0){
		if(m_ChangeCardIndex != m_returnChangeCardIndex && !g_MyObserver)return;
		if(m_ChangeCardNum != m_returnChangeCardNum && !g_MyObserver)return;
	}

	if(g_RI.ChangeCardStep == 2)
	{	
		int sp = Game.GetServPNum_ByPN(PNum);
		
		for(int i = 0; i < 6; i++){
			if(DUMY_CARD == Card[i].CardNo && i == m_returnChangeCardIndex)
			{
				Card[i].ClearEvent();										
				Card[i].SetPos(Card[i].Xp,Card[i].Yp);
				
				CPoint pt;
				pt.x = Card[i].Xp;
				pt.y = Card[i].Yp;
				Card[i].m_move.SetCurPos(pt);
				
				for(int x = i ; x < 6 ; x ++){
					int num = x+1;
					if(num == 6){
						Card[x].CardNo = -10;					
					}
					else{
						Card[x].CardNo = Card[num].CardNo;											
					}
				}			
				if(m_returnChangeCardIndex <= 1)
				{						
					Card[1].bFront = TRUE;
					if(PNum == 0 && !g_MyObserver){
						CPoint pt;
						pt = Card[1].m_move.GetCurPos();					
						pt.y -= _CARD_YPOS_OFFSET+2;					
						Card[1].m_move.SetCurPos(pt);
					}
				}
				break;
			}			
		}
		
		CPoint pt;
		pt.x = Card[5].Xp;
		pt.y = Card[5].Yp;
		
		Card[5].bShow = TRUE;  // 임시 ( FALSE -> 카드 날리기 받았을때 TRUE)
		
		CPoint cpt;
		cpt.x = 368;
		cpt.y = 40;
		Card[5].m_move.SetCurPos(cpt);
		
		Card[5].bFront = FALSE;
		Card[5].CardNo = m_returnCardNum;		
		Card[5].StraightMoveTo(pt, 10*3, 18*3, 0);			
		// 플레이 넘버,인덱스 값
		Card[5].SetEvent(EVENT_CHANGESET_ONEFLYEND, PNum, 5 );	
		



		for(i = 0 ; i < 4;i++){
			g_Poker.PS[sp].nCard[i] = Card[i+2].CardNo;
		}

		if(m_returnChangeCardIndex < 2)
		{
			g_Poker.BC[sp].nCard[1] = Card[1].CardNo;
		}

		g_Poker.BC[sp].nCard[2] = m_returnCardNum;
		
	}

	if(g_RI.ChangeCardStep == 1)
	{
		if(m_returnChangeCardIndex <= 1){
			CPoint pt;
			pt.x = Card[m_returnChangeCardIndex].Xp;
			pt.y = Card[m_returnChangeCardIndex].Yp;
			
			Card[m_returnChangeCardIndex].bShow = TRUE;  // 임시 ( FALSE -> 카드 날리기 받았을때 TRUE)
			
			CPoint cpt;
			cpt.x = 368;
			cpt.y = 40;
			Card[m_returnChangeCardIndex].m_move.SetCurPos(cpt);		
			Card[m_returnChangeCardIndex].bFront = FALSE;	
			Card[m_returnChangeCardIndex].CardNo = m_returnCardNum;	
			Card[m_returnChangeCardIndex].StraightMoveTo(pt, 10*3, 18*3, 0);		
			// 플레이 넘버,인덱스 값		
			Card[m_returnChangeCardIndex].SetEvent(EVENT_CHANGESET_ONEFLYEND, PNum, m_returnChangeCardIndex );		
		}
		else if(m_returnChangeCardIndex > 1)
		{				
			for(int i = 0; i < TotalCardNum; i++){
				if(DUMY_CARD == Card[i].CardNo && i == m_returnChangeCardIndex){					
					Card[i].ClearEvent();										
					Card[i].SetPos(Card[i].Xp,Card[i].Yp);
					
					CPoint pt;
					pt.x = Card[i].Xp;
					pt.y = Card[i].Yp;
					Card[i].m_move.SetCurPos(pt);
					
					for(int x = i ; x < TotalCardNum ; x ++){
						int num = x+1;
						if(num == 6){
							Card[x].CardNo = -10;					
						}
						else{
							Card[x].CardNo = Card[num].CardNo;											
						}
					}				
				}			
			}
			
			CPoint pt;
			pt.x = Card[5].Xp;
			pt.y = Card[5].Yp;
			
			Card[5].bShow = TRUE;  // 임시 ( FALSE -> 카드 날리기 받았을때 TRUE)
			
			CPoint cpt;
			cpt.x = 368;
			cpt.y = 40;
			Card[5].m_move.SetCurPos(cpt);

			Card[5].bFront = FALSE;
			Card[5].CardNo = m_returnCardNum;		
			Card[5].StraightMoveTo(pt, 10*3, 18*3, 0);			
			// 플레이 넘버,인덱스 값
			Card[5].SetEvent(EVENT_CHANGESET_ONEFLYEND, PNum, 5 );
			int sp = Game.GetServPNum_ByPN(PNum);
			for(i = 0 ; i < 4;i++)
			{
				g_Poker.PS[sp].nCard[i] = Card[i+2].CardNo;
			}		
		}
	}
	
	bWindCardEnd = FALSE;					// 카드 날리기 끝 알림 
	m_ChangeCardIndex =EMPTY;
	m_ChangeCardNum = EMPTY;
	m_returnChangeCardIndex = EMPTY;			// 보낸 숫자와 비교할때 쓴다
	m_returnChangeCardNum = EMPTY;		// 보낸 숫자와 비교할때 쓴다
	m_returnCardNum =EMPTY;			// 바꾸기 요청을 해서 새로 받아온 숫자
}

void CCardDeck::ReSetCardRect()
{	
	if(m_SelectCard > -1){
		POINT pt = Card[m_SelectCard].m_move.GetCurPos();						
		
		if( m_SelectCard < 2){			
			pt.y += _CARD_YPOS_OFFSET*2;				
			Card[m_SelectCard].m_move.SetCurPos(pt);
		}
		else {
			pt.y += _CARD_YPOS_OFFSET;
			Card[m_SelectCard].m_move.SetCurPos(pt);			
		}			
	}

	m_ChangeCardIndex = m_SelectCard;
	m_SelectCard = -1;
//	m_ChangeCardNum = -1;
	g_bChangeCardStage = FALSE;

	for(int i = 0 ; i < SELECT_TOTALCARD ; i ++){	
		CardRect[i] = NULL;
	}
}

BOOL CCardDeck::PtInCardRect(int xp,int yp,int flg)
{		
	CPoint pt;
	pt.x = xp;
	pt.y = yp;
	for(int i = 0 ; i < SELECT_TOTALCARD; i ++){
		if(CardRect[i].PtInRect(pt) > 0 )
	//	if(pt.x > CardRect[i].left && pt.x < CardRect[i].right
	//		&& pt.y > CardRect[i].top && pt.y < CardRect[i].bottom)
		{			
			if(m_SelectCard == i){
				POINT pt = Card[i].m_move.GetCurPos();				

				//더블 클릭이면 리턴 
				if(flg == 0){				
					Card[i].bSelect = FALSE;			
					m_SelectCard =-1;				
					m_ChangeCardNum = -1;							
					
					if( i < 2){
						pt.y += _CARD_YPOS_OFFSET*2;
						CardRect[i].OffsetRect(0,_CARD_YPOS_OFFSET*2);			
					}
					else {
						pt.y += _CARD_YPOS_OFFSET;
						CardRect[i].OffsetRect(0,_CARD_YPOS_OFFSET);			
					}		
				}
				
				Card[i].m_move.SetCurPos(pt);				
				Sound.Play( SND23 );
				return 1;
			}

			if(m_SelectCard == -1){
				POINT pt = Card[i].m_move.GetCurPos();				
				
			//	if(flg == 0)
				{
					Card[i].bSelect = TRUE;			
					m_SelectCard = i;		
					m_ChangeCardNum = Card[i].CardNo;
					
					if( i < 2){
						pt.y -= _CARD_YPOS_OFFSET*2;
						CardRect[i].OffsetRect(0,-(_CARD_YPOS_OFFSET*2));			
					}
					else {
						pt.y -= _CARD_YPOS_OFFSET;
						CardRect[i].OffsetRect(0,-_CARD_YPOS_OFFSET);			
					}
				}

				Card[i].m_move.SetCurPos(pt);					
				Sound.Play( SND23 );
				return 1;
			}

			if(m_SelectCard != i && m_SelectCard != -1)
			{
				POINT pt = Card[m_SelectCard].m_move.GetCurPos();				
				
				if( m_SelectCard < 2){
					pt.y += _CARD_YPOS_OFFSET*2;
					CardRect[m_SelectCard].OffsetRect(0,_CARD_YPOS_OFFSET*2);			
				}
				else {
					pt.y += _CARD_YPOS_OFFSET;
					CardRect[m_SelectCard].OffsetRect(0,_CARD_YPOS_OFFSET);			
				}
				
				Card[m_SelectCard].m_move.SetCurPos(pt);	
				Card[m_SelectCard].bSelect = FALSE;			

				pt = Card[i].m_move.GetCurPos();				
				if( i < 2){
					pt.y -= _CARD_YPOS_OFFSET*2;
					CardRect[i].OffsetRect(0,-(_CARD_YPOS_OFFSET*2));			
				}
				else{
					pt.y -= _CARD_YPOS_OFFSET;
					CardRect[i].OffsetRect(0,-_CARD_YPOS_OFFSET);			
				}								
				Card[i].m_move.SetCurPos(pt);				
				Card[i].bSelect = TRUE;	
				m_SelectCard = i;												
				m_ChangeCardNum = Card[i].CardNo;
				Sound.Play( SND23 );
				return 1;
			}							
		}
	}	

	return FALSE;
}

void CCardDeck::SetChangeSelectCard()		//바꾸기 요청한카드 표시
{
	POINT pt = Card[m_SelectCard].m_move.GetCurPos();
	
	if( m_SelectCard < 2){
		pt.y -= _CARD_YPOS_OFFSET*2;
		CardRect[m_SelectCard].OffsetRect(0,-(_CARD_YPOS_OFFSET*2));			
	}
	else{
		pt.y -= _CARD_YPOS_OFFSET;
		CardRect[m_SelectCard].OffsetRect(0,-_CARD_YPOS_OFFSET);			
	}			
	Card[m_SelectCard].m_move.SetCurPos(pt);
	Card[m_SelectCard].SetChangeCardCheck(TRUE);
}