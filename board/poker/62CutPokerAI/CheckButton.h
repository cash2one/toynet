#if !defined(AFX_CHECKBUTTON_H__C8FDD785_EAC1_48BB_88BA_619DC72681EF__INCLUDED_)
#define AFX_CHECKBUTTON_H__C8FDD785_EAC1_48BB_88BA_619DC72681EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckButton window

#include "MyBitmap.h"

class CCheckButton : public CButton
{
// Construction
public:
	CCheckButton();
	virtual ~CCheckButton();
// Attributes
public:
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckButton)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init( int x, int y, LPSTR checked_filename, LPSTR unchecked_filename, CWnd* pParent );
	void Init( LPCSTR checked_filename, LPSTR unchecked_filename, CWnd* pParent ); 
	void SetCheck( BOOL bCheck );
	const BOOL GetCheck() const
	{
		return m_bChecked;
	}

protected:

	CWnd*			m_pParent;
	POINT			m_pt;
	BOOL			m_bChecked;

	CMyBitmap		m_bmp[2];
	
	// Generated message map functions
protected:

	//{{AFX_MSG(CCheckButton)
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

#endif // !defined(AFX_CHECKBUTTON_H__C8FDD785_EAC1_48BB_88BA_619DC72681EF__INCLUDED_)
