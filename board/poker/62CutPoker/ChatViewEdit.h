#if !defined(AFX_MYRICHEDIT_H__AAF09DEF_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_MYRICHEDIT_H__AAF09DEF_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyRichEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChatViewEdit window

class CChatViewEdit : public CRichEditCtrl
{
public:
	int oldline;
	CChatViewEdit();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatViewEdit)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTextFormat(int kind);
	void AddText(CString *str, COLORREF crNewColor=RGB(0,0,0), BOOL bScroll=TRUE);
	void AddText(char *str, COLORREF crNewColor=RGB(0,0,0), BOOL bScroll=TRUE);
	virtual ~CChatViewEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChatViewEdit)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYRICHEDIT_H__AAF09DEF_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_)
