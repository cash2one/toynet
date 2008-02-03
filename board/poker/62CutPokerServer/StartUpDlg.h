#if !defined(AFX_STARTUPDLG_H__A588D9B5_B572_4DD0_B3DE_9A3D62E44345__INCLUDED_)
#define AFX_STARTUPDLG_H__A588D9B5_B572_4DD0_B3DE_9A3D62E44345__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StartUpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStartUpDlg dialog

class CStartUpDlg : public CDialog
{
// Construction
public:
	CStartUpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStartUpDlg)
	enum { IDD = IDD_STARTUP_DIALOG };
	CString	m_Info;
	BOOL	m_bInitConnectStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartUpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStartUpDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonConfig();
	afx_msg void OnButtonConfig2();
	afx_msg void OnButtonConfig3();
	afx_msg void OnButtonConfig4();
	afx_msg void OnButtonEvent(); // ### [이벤트 티켓] ###
	afx_msg void OnButtonSa();	  // [ 통계기록 ]
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTUPDLG_H__A588D9B5_B572_4DD0_B3DE_9A3D62E44345__INCLUDED_)
