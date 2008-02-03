#if !defined(AFX_SELCARDCHOICEDLG_H__1DB79FA1_E7FD_4DEC_9911_7231CB7CAD5F__INCLUDED_)
#define AFX_SELCARDCHOICEDLG_H__1DB79FA1_E7FD_4DEC_9911_7231CB7CAD5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelCardChoiceDlg.h : header file
//

#include "MyBitmap.h"
#include "Graphic.h"

/////////////////////////////////////////////////////////////////////////////
// CSelCardChoiceDlg dialog
#define SELECTDLGCHOICE_TIMER 1000

class CSelCardChoiceDlg : public CDialog
{
// Construction
public:

	CMyBitmap Back;
	CPage Page;
	UINT hTimer;
	CRect CardRect1, CardRect2, CardRect3, CardRect4;

	CFont Font1, Font2;

	int Select;
	int Index;

	int DisCard;    // ###초이스 카드###
	int DisCardIndex;
	int bChoice;	// 카드 선택중인가


	char       m_aCard[4];

	int        m_nTime;


	CSelCardChoiceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelCardChoiceDlg)
	enum { IDD = IDD_DLG_SELCARDCHOICE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelCardChoiceDlg)
	public:
	virtual int DoModal(int card1, int card2, int card3, int card4); 
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelCardChoiceDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELCARDCHOICEDLG_H__1DB79FA1_E7FD_4DEC_9911_7231CB7CAD5F__INCLUDED_)
