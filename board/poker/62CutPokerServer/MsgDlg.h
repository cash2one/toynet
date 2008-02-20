#if !defined(AFX_MSGDLG_H__9F614787_9F76_47EF_A9DD_DA53729DD7C5__INCLUDED_)
#define AFX_MSGDLG_H__9F614787_9F76_47EF_A9DD_DA53729DD7C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg dialog

class CMsgDlg : public CDialog
{
// Construction
public:
	CMsgDlg(CWnd* pParent = NULL);   // standard constructor

	void SetMsg(LPCTSTR pstrMsg);

// Dialog Data
	//{{AFX_DATA(CMsgDlg)
	enum { IDD = IDD_DIALOG_MSG };
	CString	m_strMsg;
	CMsgDlg*	a;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd, LPCTSTR pstrMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGDLG_H__9F614787_9F76_47EF_A9DD_DA53729DD7C5__INCLUDED_)
