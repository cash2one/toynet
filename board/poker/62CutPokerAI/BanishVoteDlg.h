#if !defined(AFX_BANISHVOTEDLG_H__F31493DA_EEA9_4BE7_8973_9CCB96095E3E__INCLUDED_)
#define AFX_BANISHVOTEDLG_H__F31493DA_EEA9_4BE7_8973_9CCB96095E3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BanishVoteDlg.h : header file
//

#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CBanishVoteDlg dialog

class CBanishVoteDlg : public CDialog
{
// Construction
public:
	CString TarID[30];
	CBanishVoteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBanishVoteDlg)
	enum { IDD = IDD_BANISHVOTE };
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	CComboBox	m_TargetIDCombo;
	CString	m_Reson;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBanishVoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBanishVoteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANISHVOTEDLG_H__F31493DA_EEA9_4BE7_8973_9CCB96095E3E__INCLUDED_)
