// InfoBox.h: interface for the CInfoBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOBOX_H__8D0F71E1_25F2_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_INFOBOX_H__8D0F71E1_25F2_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"
#include "ChatViewEdit.h"
#include "SendEdit.h"

class CInfoBox  
{
public:
	CWnd *pParent;
	CPage *pPage;
	CFont Font, Font2;
	CChatViewEdit ChatViewEdit;
	CSendEdit SendEdit;


	INT64 NowMarble;

	CString NowStr;
	
	BOOL bShow;
	int Xp, Yp;
	int Width, Height;

	// 이동 관련 변수
	BOOL bMove;
	int ClickX, ClickY;

	// 박스 사이즈 바꾸기 관련 변수
	BOOL bSize;
	int OrgWidth, OrgHeight;

	CInfoBox();
	virtual ~CInfoBox();

	void Init(CWnd *pWnd, CPage *ppage);
	BOOL OnLButtonDown(int x, int y);
	void OnLButtonUp();
	void OnMouseMove(int x, int y);
	void Draw(CDC *pDC);
};

#endif // !defined(AFX_INFOBOX_H__8D0F71E1_25F2_11D4_97A5_0050BF0FBE67__INCLUDED_)
