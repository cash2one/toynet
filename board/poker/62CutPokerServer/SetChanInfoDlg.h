#if !defined(AFX_SETCHANINFODLG_H__AED4CB90_097E_4FEF_A132_91222112FA2C__INCLUDED_)
#define AFX_SETCHANINFODLG_H__AED4CB90_097E_4FEF_A132_91222112FA2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetChanInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetChanInfoDlg dialog

class CSetChanInfoDlg : public CDialog
{
// Construction
public:
	CSetChanInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetChanInfoDlg)
	enum { IDD = IDD_SETCHANINFO_DIALOG };
	CString	m_ChanName;
	int		m_GameKind;
	int		m_Limit1;
	int		m_Limit2;
	int		m_Grade;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetChanInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetChanInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCHANINFODLG_H__AED4CB90_097E_4FEF_A132_91222112FA2C__INCLUDED_)
