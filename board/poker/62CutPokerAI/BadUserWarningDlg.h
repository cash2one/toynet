#if !defined(AFX_BADUSERWARNINGDLG_H__87DA9FCB_C696_4981_A5BD_9565C27B229A__INCLUDED_)
#define AFX_BADUSERWARNINGDLG_H__87DA9FCB_C696_4981_A5BD_9565C27B229A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BadUserWarningDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBadUserWarningDlg dialog
#include "MyButton.h"

class CBadUserWarningDlg : public CDialog
{
// Construction
public:
	CBadUserWarningDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBadUserWarningDlg)
	enum { IDD = IDD_BADUSER_WARNING };
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBadUserWarningDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBadUserWarningDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BADUSERWARNINGDLG_H__87DA9FCB_C696_4981_A5BD_9565C27B229A__INCLUDED_)
