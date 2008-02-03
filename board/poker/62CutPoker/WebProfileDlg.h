#if !defined(AFX_WEBPROFILEDLG_H__30D7977B_4722_4744_97CF_2DDE0BC40419__INCLUDED_)
#define AFX_WEBPROFILEDLG_H__30D7977B_4722_4744_97CF_2DDE0BC40419__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebProfileDlg.h : header file
//

#include "WebCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CWebProfileDlg dialog

class CWebProfileDlg : public CDialog
{
// Construction
public:
	static CString m_strProfileURL;
	CWebCtrl *m_pWebCtrl;
	CString m_strID;
	CString m_strURL;

	CWebProfileDlg(CWnd* pParent = NULL);   // standard constructor

	static void SetProfileURL(LPCTSTR strurl);
	BOOL OpenProfile(LPCTSTR struserid);

// Dialog Data
	//{{AFX_DATA(CWebProfileDlg)
	enum { IDD = IDD_WEBPROFILE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebProfileDlg)
	public:
	virtual BOOL Create();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWebProfileDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBPROFILEDLG_H__30D7977B_4722_4744_97CF_2DDE0BC40419__INCLUDED_)
