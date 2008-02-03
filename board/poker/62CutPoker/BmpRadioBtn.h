#if !defined(AFX_BMPRADIOBTN_H__B7F9B5B4_D3C2_4A59_82B4_47EC8C8C63BE__INCLUDED_)
#define AFX_BMPRADIOBTN_H__B7F9B5B4_D3C2_4A59_82B4_47EC8C8C63BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpRadioBtn.h : header file
//

#include "MyBitmap.h"

/////////////////////////////////////////////////////////////////////////////
// CBmpRadioBtn window

class CBmpRadioBtn : public CButton
{
// Construction
public:
	CBmpRadioBtn();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual ~CBmpRadioBtn();

	void Init( int x, int y, LPSTR checked_filename, LPSTR unchecked_filename, CWnd* pParent );
	BOOL GetCheck() const
	{
		return m_bChecked;
	}

	void AddToGroup( CBmpRadioBtn* pBtn );
	void RemoveFromGroup();

	void SetSizeToBmp();
	void SetCheck( BOOL bCheck );

protected:

	void GroupCheck( BOOL bSet );
	

	void SetNext( CBmpRadioBtn* pBtn )
	{
		m_pNext = pBtn;
	}
	void SetPrev( CBmpRadioBtn* pBtn )
	{
		m_pPrev = pBtn;
	}

	CBmpRadioBtn*	m_pPrev;
	CBmpRadioBtn*	m_pNext;

	POINT			m_pt;
	BOOL			m_bChecked;

	CMyBitmap		m_bmp[2];
	CWnd*			m_pParent;

	// Generated message map functions
public:

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

#endif // !defined(AFX_BMPRADIOBTN_H__B7F9B5B4_D3C2_4A59_82B4_47EC8C8C63BE__INCLUDED_)
