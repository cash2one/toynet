// PlayScr.h: interface for the CPlayScr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYSCR_H__4DF90567_2B2D_4E65_8A16_E24DFFE6B594__INCLUDED_)
#define AFX_PLAYSCR_H__4DF90567_2B2D_4E65_8A16_E24DFFE6B594__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Graphic.h"
#include "UserStruct.h"
#include "xAniMan.h"


class CPlayScr  
{
public:
	

	
	CWnd *pParent;
	CPage *pPage;	

	int PNum;				// 플레이어 번호
	int Xp, Yp;				// 플레이어 스크린의 기준좌표
	int RXp, RYp;			// 결과(승, 패, 폴드)
	int TimeX, TimeY;		// 타이머 막대 
	int MgX, MgY;			// 메세지 박스 그리기
	int MsgX, MsgY;
	int JackPotX, JackPotY;
	int JackPotaniX, JackPotaniY;
	BOOL bGameOver;			// 게임 오버 상태인가?

	////////////////////////////////////////////////
	// 올인 관련 변수 
	int m_AllinXp[3];			// 올인 애니 좌표
	int m_AllinYp[3];			// 올인 애니 좌표	
	DWORD m_AllinAniStartTime;			// 올인 애닉 관련 타임 변수 

	int PlayCnt;			// 플레이 카운트
	UINT PlayStartTick;		// 플레이 시작 타임

	int m_xp[2],    m_yp[2];
	int m_rxp[2],   m_ryp[2];
	int m_mgx[2],	m_mgy[2];
	int m_msgx[2],  m_msgy[2];
	int m_jackpotx[2], m_jackpoty[2];
	int m_jackpotanix[2], m_jackpotaniy[2];


public:
	CPlayScr();
	virtual ~CPlayScr();

	void draw_6user(CDC *pDC);
	void draw_5user(CDC *pDC);
	void playscr_changeinitpos();
	void SelectChoiceCard(int index,  int discard);
	void draw_timer_and_soundfx(int PNum);
	void draw_jackpot_digit();

	void Init(CWnd *pWnd, CPage *ppage, int pnum);
	void Reset();
	void OnTimer();
	void OnGameKey(int nVKey);
	BOOL OnLButtonDown(int xp, int yp);	
	BOOL OnMouseMove(int xp, int yp);
	BOOL OnLButtonUp(int x, int y);
	void Draw(CDC *pDC);
	void DrawChip();
};

#endif // !defined(AFX_PLAYSCR_H__4DF90567_2B2D_4E65_8A16_E24DFFE6B594__INCLUDED_)
