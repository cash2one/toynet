#if !defined(AFX_MSGDLG_H__070F5001_1C5C_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_MSGDLG_H__070F5001_1C5C_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgDlg.h : header file
//

#define MSGBOX_WAIT_TIMER 334

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg dialog

class CMsgDlg : public CDialog
{
// Construction
public:
	int iMode;
	CString szMsgText;
	int hTimerWait;

	CMsgDlg(CWnd* pParent = NULL);   // standard constructor
	void MsgBox(int mode, char *msgstr);

// Dialog Data
	//{{AFX_DATA(CMsgDlg)
	enum { IDD = IDD_MSG_DIALOG };
	CMsgDlg*	a;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CMsgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGDLG_H__070F5001_1C5C_11D4_97A5_0050BF0FBE67__INCLUDED_)
