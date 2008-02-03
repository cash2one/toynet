#if !defined(AFX_BMPCHECKBUTTON_H__BFCA0CBA_0682_4E69_A8A7_599CB4EAAE09__INCLUDED_)
#define AFX_BMPCHECKBUTTON_H__BFCA0CBA_0682_4E69_A8A7_599CB4EAAE09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpCheckButton.h : header file
//
#include "MyBitmap.h"

/////////////////////////////////////////////////////////////////////////////
// CBmpCheckButton window

class CBmpCheckButton : public CButton
{
// Construction
public:
	CBmpCheckButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpCheckButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBmpCheckButton();

	void Init( int x, int y, LPSTR checked_filename, LPSTR unchecked_filename, CWnd* pParent, BOOL bSendDlgCmd = FALSE );
	void SetCheck( BOOL bCheck );
	BOOL GetCheck() const
	{
		return m_bChecked;
	}

	void SetSizeToBmp();

protected:

	POINT			m_pt;
	BOOL			m_bChecked;
	BOOL			m_bSendCmd;

	CMyBitmap		m_bmp[2];
	CWnd*			m_pParent;

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpCheckButton)		
	afx_msg void OnClicked();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPCHECKBUTTON_H__BFCA0CBA_0682_4E69_A8A7_599CB4EAAE09__INCLUDED_)
