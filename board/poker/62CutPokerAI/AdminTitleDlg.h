#if !defined(AFX_ADMINTITLEDLG_H__5737BEFF_2345_47C5_A052_BE3BFC486075__INCLUDED_)
#define AFX_ADMINTITLEDLG_H__5737BEFF_2345_47C5_A052_BE3BFC486075__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminTitleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdminTitleDlg dialog

class CAdminTitleDlg : public CDialog
{
// Construction
public:
	CAdminTitleDlg(CString strName, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdminTitleDlg)
	enum { IDD = IDD_ADMIN_TITLE };
	CComboBox	m_ctrlTitle;
	CString	m_strPrevTitle;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminTitleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdminTitleDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINTITLEDLG_H__5737BEFF_2345_47C5_A052_BE3BFC486075__INCLUDED_)
