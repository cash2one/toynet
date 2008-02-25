#if !defined(AFX_RESULTHI_H__612C30A2_53E3_45AB_BD0E_99C6AAC6FF9A__INCLUDED_)
#define AFX_RESULTHI_H__612C30A2_53E3_45AB_BD0E_99C6AAC6FF9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultHi.h : header file
//
#include "MyButton.h"
#include "CharView.h"
#include "UserStruct.h"
#include "MyBitmap.h"

#include "Graphic.h"
#include "GraphButton.h"

/////////////////////////////////////////////////////////////////////////////
// CResultHi dialog

#define RESULTHI_TIMER 1300
#define MAX_WAIT_RESULT 7			//Variant For waiting Result - jeong

#define IDM_RESULT_OK		(WM_USER+6114)
#define IDM_RESULT_BONUS	(WM_USER+6115)

class CResultHi : public CDialog
{
// Construction
public:
	

	CPage Page;
	CGraphButton OkBtn;
	CGraphButton BonusGBtn;

	CMyBitmap Back;
	CCharView m_CharView;
	CFont Font1, Font2;
	USERINFO UI;
	GAMEOVERRESULT pGO;
	int WinCase;

	BOOL m_bResultBtnMouseDown;
	int m_nResultBtnIndex;
	int	m_nResultOffSetX[2];
	int	m_nResultOffSetY[2];

	int m_winner_kind;

	UINT hTimer;
	UINT TimeCnt;

	int m_aCard[5];
	int m_aSwing[7];

	CString m_Str1, m_Str2;

	BOOL Init(USERINFO *pUI, GAMEOVERRESULT *pGOR, int wicase, int pnum);

	BOOL m_nItemByeUse; // [수호천사] 2004.07.08

	CResultHi(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CResultHi)
	enum { IDD = IDD_DIALOG_RESULTHI };
	CMyButton	m_MiniGame;
	CMyButton	m_SafeBtn;
	CMyButton	m_ShowCardBtn;
	CMyButton	m_CloseBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultHi)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResultHi)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonShowcard();
	afx_msg void OnButtonSafeangel();
	afx_msg void OnMinigame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTHI_H__612C30A2_53E3_45AB_BD0E_99C6AAC6FF9A__INCLUDED_)





















