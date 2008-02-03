#if !defined(AFX_SETCOLUMNDLG_H__3FF1F00A_E3DD_4B5C_A22F_14EC58D33F48__INCLUDED_)
#define AFX_SETCOLUMNDLG_H__3FF1F00A_E3DD_4B5C_A22F_14EC58D33F48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetColumnDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetColumnDlg dialog

class CSetColumnDlg : public CDialog
{
// Construction
public:
	CSetColumnDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL m_bDontEnableAutoIndexCheck;
	void EnableAutoIndex(BOOL bEnable);

// Dialog Data
	//{{AFX_DATA(CSetColumnDlg)
	enum { IDD = IDD_SETCOLUMN_DIALOG };
	CString	m_Value;
	CString	m_ColumnName;
	BOOL	m_bAutoIndex;
	int		m_StartVal;
	int		m_Cipher;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetColumnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetColumnDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCheckAutoindex();	// <- Ãß°¡
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCOLUMNDLG_H__3FF1F00A_E3DD_4B5C_A22F_14EC58D33F48__INCLUDED_)
