#if !defined(AFX_RESULTFINDIDDLG_H__D5F15B4D_A8A2_481E_BC5B_7894B3AE3181__INCLUDED_)
#define AFX_RESULTFINDIDDLG_H__D5F15B4D_A8A2_481E_BC5B_7894B3AE3181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultFindIdDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResultFindIdDlg dialog
#include "MyButton.h"

class CResultFindIdDlg : public CDialog
{
// Construction
public:
	CResultFindIdDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_Str;

// Dialog Data
	//{{AFX_DATA(CResultFindIdDlg)
	enum { IDD = IDD_RESULT_FINDID };
	CMyButton	m_OkBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultFindIdDlg)
	public:
	virtual int DoModal(CString &str);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResultFindIdDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTFINDIDDLG_H__D5F15B4D_A8A2_481E_BC5B_7894B3AE3181__INCLUDED_)
