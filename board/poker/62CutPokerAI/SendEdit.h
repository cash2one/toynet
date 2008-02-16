#if !defined(AFX_SENDEDIT_H__AAF09DFC_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_SENDEDIT_H__AAF09DFC_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendEdit.h : header file
//

#include <mmsystem.h>

/////////////////////////////////////////////////////////////////////////////
// CSendEdit window

class CSendEdit : public CEdit
{
public:

	int iUse;

	int NowHPos;
	CString History[10];

	BOOL bSetTextColor;
	BOOL bSetBkColor;
	CBrush TextBrush;
	CBrush BkBrush;
	COLORREF TextColor;
	COLORREF BkColor;

	CSendEdit();

	void SetTextColor(COLORREF color);
	void SetBkColor(COLORREF color);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendEdit)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSendEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSendEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDEDIT_H__AAF09DFC_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_)
