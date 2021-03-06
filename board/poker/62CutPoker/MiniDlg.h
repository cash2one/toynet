#if !defined(AFX_MINIDLG_H__8B80A5C7_8B18_4AB6_9983_977FFBCDC0F5__INCLUDED_)
#define AFX_MINIDLG_H__8B80A5C7_8B18_4AB6_9983_977FFBCDC0F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiniDlg.h : header file
//

#include "MyButton.h"
#include "MyBitmap.h"

#include "resource.h"
#include "MiniGame.h"

#include "Ime.h"
#include "Graphic.h"
#include "GraphButton.h"

#define MINIGAME_TIMER 100


#define IDM_MINI_START	(WM_USER+6110)
#define IDM_MINI_STOP	(WM_USER+6111)
#define IDM_MINI_INIT	(WM_USER+6112)

#define MINISTART_OFFSET_X 118
#define MINISTART_OFFSET_Y 397

#define MINISTOP_OFFSET_X 241
#define MINISTOP_OFFSET_Y 397

#define MINI_BTN_TOTAL 2


/////////////////////////////////////////////////////////////////////////////
// CMiniDlg dialog

class CMiniDlg : public CDialog
{
public:
	void				InitPage(CPage *ppage);
	void				PushStopBtn();
	void				FucButtonDraw(CDC *pDC);
	void				FocusEffectDraw(CPage *pPage);

public:
	CMiniGame			m_MnGame;
	bool				bInitGame;
	int					X2Cnt;
	
// Construction
public:
	CMiniDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMiniDlg)
	enum { IDD = IDD_DLG_MINIGAME };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
	
	

protected:

	// Generated message map functions
	//{{AFX_MSG(CMiniDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButtonHi();
	afx_msg void OnButtonLow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPage				Page;
	CMyBitmap			Back;
	CFont				Font1, Font2;

	xSprite				InitBtnSpr;
	xSprite				StartBtnSpr;
	xSprite				EndBtnSpr;
	xSprite				GameBackSpr;
	xSprite				FocusSpr;
	xSprite				DrawBonusSpr;

	CGraphButton		X2InitBtn;
	CGraphButton		X2PlayBtn;
	CGraphButton		X2EndBtn;

	int					CloseGameCnt;

	// 시작 종료 버튼 인덱스
	int m_nMiniBtnIndex;
	bool m_bMiniBtnMouseDown;
	int	m_nMiniOffSetX[MINI_BTN_TOTAL];
	int	m_nMiniOffSetY[MINI_BTN_TOTAL];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIDLG_H__8B80A5C7_8B18_4AB6_9983_977FFBCDC0F5__INCLUDED_)





















