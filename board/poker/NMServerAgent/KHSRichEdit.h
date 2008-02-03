#if !defined(AFX_KHSRICHEDIT_H__18BFF667_C46D_11d4_804D_0050BF0FBDAF__INCLUDED_)
#define AFX_KHSRICHEDIT_H__18BFF667_C46D_11d4_804D_0050BF0FBDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKHSRichEdit window

class CKHSRichEdit : public CRichEditCtrl
{
// Construction
public:
	CKHSRichEdit();

	CMenu   m_ctrMenu;  // 팝업 메뉴
	CString m_strSel;   // 선택 문자열

// Attributes
public:

// Operations
public:
	void AddText(LPCTSTR szTextIn, COLORREF &crNewColor);
	void AddText(CString &strTextIn, COLORREF &crNewColor);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKHSRichEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKHSRichEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKHSRichEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuCopy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KHSRICHEDIT_H__18BFF667_C46D_11d4_804D_0050BF0FBDAF__INCLUDED_)
