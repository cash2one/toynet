#if !defined(AFX_LEVELVIEW_H__A4E2A4D4_7004_48DB_9F38_E314AC9528AC__INCLUDED_)
#define AFX_LEVELVIEW_H__A4E2A4D4_7004_48DB_9F38_E314AC9528AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LevelView.h : header file
//

#include "Graphic.h"


/////////////////////////////////////////////////////////////////////////////
// CLevelView frame
#define LEVELVIEW_TIMER 3234

// [ ###¿Ë∆Ã### ] ≈∏¿Ã∆≤, ∑Œ∫Ò»≠∏È ¿Ë∆Ã ∫∏ø©¡÷±‚
class CLevelView : public CWnd
{

protected:
	

// Attributes
public:

	CPage Page;
	int UseMode;
	INT64 JackPotMoney;

	int		AX, AY;  // ¿˝¥Î¡¬«•
	BOOL	m_bFirst;
	BOOL	m_bSecond;
	int		m_nLength;

	int     m_nFirstXp;
	int     m_nSecondXp;
	

	char    m_jackpot_digit[30];
	int     m_digit_cnt;

	void Create(CWnd *pParentWnd, CRect &rect, int UseMode, int nID);

// Operations
public:
	void set_draw_pos();
	void draw_digit(int startx);
	void get_jackpotmoney_digit(INT64 Money);
	void DrawSecond(INT64 Money);
	void DrawFirst(INT64 Money);
	int DrawJackPot(int sx,int sy, INT64 Money);
	void SetMoney(INT64 money);
	CLevelView();           // protected constructor used by dynamic creation
	virtual ~CLevelView();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLevelView)
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CLevelView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEVELVIEW_H__A4E2A4D4_7004_48DB_9F38_E314AC9528AC__INCLUDED_)
