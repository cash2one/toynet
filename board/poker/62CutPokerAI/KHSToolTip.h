#if !defined(AFX_KHSTOOLTIP_H__6B65E761_81F8_11D4_9478_E838D8685B59__INCLUDED_)
#define AFX_KHSTOOLTIP_H__6B65E761_81F8_11D4_9478_E838D8685B59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KHSToolTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKHSToolTip window

class CKHSToolTip : public CToolTipCtrl
{
// Construction
public:
	CKHSToolTip();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKHSToolTip)
	//}}AFX_VIRTUAL

// Implementation
public:
	// 주어진 사각형 영역에 툴팁을 보인다.
	BOOL RectTip(CWnd *pWnd, LPCTSTR pszText, LPRECT lpRect, UINT nIDRESURCE);
	// 주어진 자식윈도우에 툴팁을 보인다.
	BOOL WndTip(CWnd *pWnd, LPCTSTR pszText);
	virtual ~CKHSToolTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKHSToolTip)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KHSTOOLTIP_H__6B65E761_81F8_11D4_9478_E838D8685B59__INCLUDED_)
