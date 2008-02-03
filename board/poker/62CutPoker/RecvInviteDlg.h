#if !defined(AFX_RECVINVITEDLG_H__503DD5A2_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_RECVINVITEDLG_H__503DD5A2_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecvInviteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecvInviteDlg dialog
#include "MyButton.h"
#include "MyBitmap.h"


class CRecvInviteDlg : public CDialog
{
// Construction
public:
	int TargetUNum;
	CString TargetID;
	int RoomNum;
	CString szMsg;

	CRecvInviteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecvInviteDlg)
	enum { IDD = IDD_RECVINVITE_DIALOG };
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	CString	m_RefuseMsg;
	CString	m_RoomInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecvInviteDlg)
	public:
	virtual int DoModal(int tarunum, char *tarid, int roomnum, char *strmsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecvInviteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECVINVITEDLG_H__503DD5A2_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_)
