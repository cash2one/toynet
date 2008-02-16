#if !defined(AFX_CHANGECHARDLG_H__C49E89E9_30C2_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_CHANGECHARDLG_H__C49E89E9_30C2_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeCharDlg.h : header file
//

#include "MyButton.h"
#include "CharView.h"
#include "MyBitmap.h"

#define IDC_SCHAR1 5301
#define IDC_SCHAR2 5302
#define IDC_SCHAR3 5303
#define IDC_SCHAR4 5304

/////////////////////////////////////////////////////////////////////////////
// CChangeCharDlg dialog

class CChangeCharDlg : public CDialog
{
// Construction
public:
	CMyBitmap Back;
	CCharView m_LCharView;
	CCharView m_SCharView[4];
	int NowPage;
	int NowSel;
	BOOL Sex;

	CChangeCharDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangeCharDlg)
	enum { IDD = IDD_CHANGECAHR_DIALOG };
	CMyButton	m_RightBtn;
	CMyButton	m_CancelBtn;
	CMyButton	m_OKBtn;
	CMyButton	m_LeftBtn;
	CChangeCharDlg*	a;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeCharDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangeCharDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLeftButton();
	afx_msg void OnRightButton();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGECHARDLG_H__C49E89E9_30C2_11D4_97A5_0050BF0FBE67__INCLUDED_)
