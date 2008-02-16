#if !defined(AFX_ADMINMESSAGEDLG_H__13EF9BBF_185F_4ED6_B66B_36FBF84E8076__INCLUDED_)
#define AFX_ADMINMESSAGEDLG_H__13EF9BBF_185F_4ED6_B66B_36FBF84E8076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminMessageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdminMessageDlg dialog

class CAdminMessageDlg : public CDialog
{
// Construction
public:
	CAdminMessageDlg(CWnd* pParent = NULL);   // standard constructor

	int  m_UNum;
	BOOL m_bShow;
	BOOL IsShow(){ return m_bShow; }
	void Hide();
	void Show(int unum, CString strID = _T(""));

	void OnChatSend();

// Dialog Data
	//{{AFX_DATA(CAdminMessageDlg)
	enum { IDD = IDD_ADMIN_MESSAGE };
	CString	m_strMessChat;
	CString	m_strMessID;
	int		m_nTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdminMessageDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnHide();
	afx_msg void OnBtnSend();
	afx_msg void OnButtonWar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINMESSAGEDLG_H__13EF9BBF_185F_4ED6_B66B_36FBF84E8076__INCLUDED_)
