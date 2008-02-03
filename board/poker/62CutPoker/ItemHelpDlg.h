#if !defined(AFX_ITEMHELPDLG_H__EB2B0C50_6583_4ED5_8301_3A3FCBF24119__INCLUDED_)
#define AFX_ITEMHELPDLG_H__EB2B0C50_6583_4ED5_8301_3A3FCBF24119__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemHelpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemHelpDlg dialog

class CItemHelpDlg : public CDialog
{
// Construction
public:
	CItemHelpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CItemHelpDlg)
	enum { IDD = IDD_ITEMHELP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemHelpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemHelpDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMHELPDLG_H__EB2B0C50_6583_4ED5_8301_3A3FCBF24119__INCLUDED_)
