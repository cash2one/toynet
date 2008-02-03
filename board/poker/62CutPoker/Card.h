// Card.h: interface for the CCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"
#include "Sound.h"
#include "CardDefine.h"
#include "MoveMent.h"

class CCard  
{
public:
	BOOL PtInCard( int x, int y );

	CPage *pPage;

	POINT m_size;		// 스프라이트 크기 (카드 크기)

	CARDEVENT Event;
	CARDEVENT Event1;
//	BOOL bShowShadow;
	// 카드 속성
	int CardNo;			// 카드번호(0 ~ 51, 0 ~ 12: 클로버  13 ~ 25:다이아몬드 26~38 :하트  39 ~ 51:스페이드)
	int PNum;			// 카드의 소유자(0-7:플레이어)
	int Where;			// 카드가 현재 있는 위치 (  카드덱 0, 유저 1)
	int Kind;			// 패의 분류 (1-12월... (13=서비스패))
	int Attrib;			// 카드의 속성(1:광 2:열 3:띠 4:홑피 )
	
	int ShowMode;		// 보여주기 방법(0:세팅 없음 1:딜레이 후 이동시 보여줌)

	// 사운드
//	SND_ENUM MoveStartSnd;	// 이동이 시작될때의 사운드
//	SND_ENUM MoveEndSnd;	// 이동이 끝났을때의 사운드

	// 알림 이벤트
	BOOL bNotifyEvent;	// 알림 이벤트가 있는가?
	COLORREF NotifyStringColor;	// 알림 이벤트 스트링 색상
	CString NotifyString;// 알림 이벤트 스트링


	// 카드 상태
	BOOL bShow;			// 화면에 보여지는가?
	BOOL bFront;		// 앞면이 보이는 상태인가?
	BOOL bMove;			// 이동중인가?
	BOOL bAccelMove;	// 가속 이동중인가?
	// 이벤트에 관련된 변수.
	BOOL bEvent;		// 카드 이벤트가 있는가?
	BOOL bEvent1;		// 카드 이벤트가 있는가?

	// 카드 이동 관련 변수
	int Xp, Yp;			// 카드가 그려질 좌표
	int SXp, SYp;		// 카드 이동시 출발 좌표
	int TXp, TYp;		// 카드 이동시 목표 좌표
	int MoveDelay;		// 이동 시작 지연치
	int EndMoveDelay;	// 이동 완료 지연치
	int MoveSpeed;		// 이동 속도
	int AccelSpeed;		// 가속 이동 속도

	// 반짝임 관련 변수
	BOOL bTwincle;
	int TwincleCnt;		// 반짝임 카운트

	// 반투명 출력 변수
	BOOL bGlass;	

	int  GetCardNum()	{ return CardNo; }



	// 이동에 관련된 변수.
	CMovement	m_move;
	void StraightMoveTo( POINT dest, float spd_gamma, float acc_gamma, int delay = 0 );


	
	CCard();
	virtual ~CCard();

	void Init(CPage *ppage, int cardnum, int pnum);	// 카드 셋팅
	void Clear();			// 모두 클리어
	void Reset();			// 한판 끝나고 
	void OnTimer();
	
	void SetEvent(int eventcode, int option=0, int option1 = 0, int option2 = 0);
	void SetEvent1(int eventcode, int option=0, int option1 = 0, int option2 = 0);
	
	void ClearEvent();
	void ClearEvent1();
	void SetNotifyEvent(char *pstr, COLORREF color=RGB(0,0,0));
	void ClearNotifyEvent();
	void SetMoveStartSnd(SND_ENUM sound);
	void SetMoveEndSnd(SND_ENUM sound);
	void SetPos(int tx, int ty);
	void SetMove(int tx, int ty, int sdelay=0, int edelay=0, int speed=-1);
	void SetAccelMove();


	// 좌표 값 
	void DrawXY(int x, int y);
	void Draw( BOOL bFold = FALSE );


};

#endif // !defined(AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_)
