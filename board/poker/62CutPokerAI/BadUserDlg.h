#if !defined(AFX_BADUSERDLG_H__F991EC81_C025_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_BADUSERDLG_H__F991EC81_C025_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BadUserDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CBadUserDlg dialog

#include "MyButton.h"
#include "MyBitmap.h"
#include "BmpCheckButton.h"

class CBadUserDlg : public CDialog
{
// Construction
public:
	CBadUserDlg(CWnd* pParent = NULL);   // standard constructor

	static int SubmitCnt;
	CMyBitmap Back;

	BOOL	m_bAnnounce;

// Dialog Data
	//{{AFX_DATA(CBadUserDlg)
	enum { IDD = IDD_BADUSER };
	CBmpCheckButton	m_btn_Announce;
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	CString	m_SubmitReason;
	CString	m_TargetID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBadUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBadUserDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BADUSERDLG_H__F991EC81_C025_11D4_97A4_0050BF0FBE67__INCLUDED_)
