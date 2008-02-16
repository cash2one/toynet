#if !defined(AFX_SENDINVITEDLG_H__503DD5A1_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_SENDINVITEDLG_H__503DD5A1_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendInviteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSendInviteDlg dialog
#include "MyButton.h"

class CSendInviteDlg : public CDialog
{
// Construction
public:
	int TargetUNum;
	CString TargetID;

	CSendInviteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSendInviteDlg)
	enum { IDD = IDD_SENDINVITE_DIALOG };
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	CString	m_InviteMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendInviteDlg)
	public:
	virtual int DoModal(int unum, CString &id);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSendInviteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDINVITEDLG_H__503DD5A1_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_)
