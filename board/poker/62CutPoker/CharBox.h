// CharBox.h: interface for the CCharBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARBOX_H__F560D681_2133_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_CHARBOX_H__F560D681_2133_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"

class CCharBox  
{
public:
	CWnd *pParent;
	CPage *pPage;
	CFont Font1, Font2;
	BOOL bShow;
	BOOL bLock;

	int Xp, Yp;
	int Width, Height;
	int CharXp, CharYp;
	int CardXp, CardYp;

	int m_xp[2],	 m_yp[2];       // 5, 7 ÁÂÇ¥
	int m_width[2],	 m_height[2];
	int m_charxp[2], m_charyp[2];
	int m_clickx[2], m_clicky[2];
	int m_cardxp[2], m_cardyp[2];

	int PNum;
	BOOL bClick;

	int m_nCharIndex[2];

public:
	CCharBox();
	virtual ~CCharBox();

	void Init(CWnd *pWnd, CPage *ppage, int defpos);
	void charbox_changeinitpos();

	void draw_5user(CDC *pDC);
	void draw_6user(CDC *pDC);
	void DrawDigit(int bKind, int Pos, INT64 money);
	
	void Lock(BOOL bvalLock);
	void SetPlayer(int pnum);
	void ClearPlayer();
	BOOL OnLButtonDown(int x, int y);
	BOOL OnLButtonUp(int x, int y);
	BOOL OnMouseMove(int x, int y);
	void Draw(CDC *pDC);
	void ResultCharDraw(CPage *pRPage, int nPlayerNum);
};

#endif // !defined(AFX_CHARBOX_H__F560D681_2133_11D4_97A5_0050BF0FBE67__INCLUDED_)
