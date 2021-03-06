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

/////////////////////////////////////////////////////////////////////////////
// CResultHi dialog

#define RESULTHI_TIMER 1300
#define MAX_WAIT_RESULT 7;			//Variant For waiting Result - jeong

class CResultHi : public CDialog
{
// Construction
public:
	

	CPage Page;

	CMyBitmap Back;
	CCharView m_CharView;
	CFont Font1, Font2;
	USERINFO UI;
	GAMEOVERRESULT pGO;
	int WinCase;

	int m_winner_kind;

	UINT hTimer;
	UINT TimeCnt;

	int m_aCard[5];
	int m_aSwing[7];

	CString m_Str1, m_Str2;

	BOOL Init(USERINFO *pUI, GAMEOVERRESULT *pGOR, int wicase, int pnum);

	BOOL m_nItemByeUse; // [��ȣõ��] 2004.07.08

	CResultHi(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CResultHi)
	enum { IDD = IDD_DIALOG_RESULTHI };
	CMyButton	m_SafeBtn;
	CMyButton	m_ShowCardBtn;
	CMyButton	m_CloseBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultHi)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTHI_H__612C30A2_53E3_45AB_BD0E_99C6AAC6FF9A__INCLUDED_)
