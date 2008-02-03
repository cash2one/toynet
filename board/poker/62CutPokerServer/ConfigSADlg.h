#if !defined(AFX_CONFIGSADLG_H__8C023659_75F9_47DD_9556_1CEC7F7F5187__INCLUDED_)
#define AFX_CONFIGSADLG_H__8C023659_75F9_47DD_9556_1CEC7F7F5187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigSADlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigSADlg dialog

class CConfigSADlg : public CDialog
{
// Construction
public:
	void SetControlState();
	CString MoneyToString(INT64 number);
	CConfigSADlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigSADlg)
	enum { IDD = IDD_DIALOG_CONFIGSA };
	BOOL	m_bSaveSetLog;
	CString	m_LeadersMoney;
	CString	m_LeadersPlusMoney;
	CString	m_NormalMoney;
	BOOL	m_bTotalMoneyLimit;
	BOOL	m_bSafeAngel;
	CString	m_strSafeMax;
	CString	m_strSafeMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigSADlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigSADlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnCheckLimit();
	afx_msg void OnChangeEditNormal();
	afx_msg void OnChangeEditLeaders();
	afx_msg void OnChangeEditLeadersplus();
	afx_msg void OnChangeEditSafemin();
	afx_msg void OnChangeEditSafemax();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGSADLG_H__8C023659_75F9_47DD_9556_1CEC7F7F5187__INCLUDED_)
