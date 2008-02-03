#if !defined(AFX_SELCARDDLG_H__8B1008D7_B378_4BC3_B9E2_9BF659366BE8__INCLUDED_)
#define AFX_SELCARDDLG_H__8B1008D7_B378_4BC3_B9E2_9BF659366BE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelCardDlg.h : header file
//

#include "MyBitmap.h"
#include "Graphic.h"

#define SELECTDLG_TIMER 1000

/////////////////////////////////////////////////////////////////////////////
// CSelCardDlg dialog

class CSelCardDlg : public CDialog
{
// Construction
public:
	CMyBitmap Back;
	CPage Page;
	UINT hTimer;
	CRect CardRect1, CardRect2, CardRect3;

	CFont Font1, Font2;

	int Select;
	int Index;
	char       m_aCard[3];

	int        m_nTime;

	CSelCardDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelCardDlg)
	enum { IDD = IDD_DLG_SELCARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelCardDlg)
	public:
	virtual int DoModal(int card1, int card2, int card3); 
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelCardDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELCARDDLG_H__8B1008D7_B378_4BC3_B9E2_9BF659366BE8__INCLUDED_)
