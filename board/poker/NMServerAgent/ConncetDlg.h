#if !defined(AFX_CONNCETDLG_H__BE3FD982_F6D4_487D_ACDA_B57F86B8ECBD__INCLUDED_)
#define AFX_CONNCETDLG_H__BE3FD982_F6D4_487D_ACDA_B57F86B8ECBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConncetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConncetDlg dialog

class CConncetDlg : public CDialog
{
// Construction
public:
	
	CString m_strServerName;
	CString m_strIP;
	UINT    m_nPort;

	void ShowInfo(CString strName);
	void ReListComboData();
	CConncetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConncetDlg)
	enum { IDD = IDD_CONNECT };
	CComboBox	m_comboList;
	CString	m_strSrvInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConncetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConncetDlg)
	afx_msg void OnButtonMakeList();
	afx_msg void OnConnect();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNCETDLG_H__BE3FD982_F6D4_487D_ACDA_B57F86B8ECBD__INCLUDED_)
