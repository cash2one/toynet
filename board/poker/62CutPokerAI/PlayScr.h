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

	int PNum;				// �÷��̾� ��ȣ
	int Xp, Yp;				// �÷��̾� ��ũ���� ������ǥ
	int RXp, RYp;			// ���(��, ��, ����)
	int TimeX, TimeY;		// Ÿ�̸� ���� 
	int MgX, MgY;			// �޼��� �ڽ� �׸���
	int MsgX, MsgY;
	int JackPotX, JackPotY;
	int JackPotaniX, JackPotaniY;
	BOOL bGameOver;			// ���� ���� �����ΰ�?

	////////////////////////////////////////////////
	// ���� ���� ���� 
	int m_AllinXp[3];			// ���� �ִ� ��ǥ
	int m_AllinYp[3];			// ���� �ִ� ��ǥ	
	DWORD m_AllinAniStartTime;			// ���� �ִ� ���� Ÿ�� ���� 

	int PlayCnt;			// �÷��� ī��Ʈ
	UINT PlayStartTick;		// �÷��� ���� Ÿ��

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
