#if !defined(AFX_LIMITMONEYCONFIRMDLG_H__B3E94BC7_277C_4654_BE78_E9F1C481B417__INCLUDED_)
#define AFX_LIMITMONEYCONFIRMDLG_H__B3E94BC7_277C_4654_BE78_E9F1C481B417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LimitMoneyConfirmDlg.h : header file
//
#include "MyButton.h"
#include "MyBitmap.h"

/////////////////////////////////////////////////////////////////////////////
// CLimitMoneyConfirmDlg dialog

class CLimitMoneyConfirmDlg : public CDialog
{
// Construction
public:
	CLimitMoneyConfirmDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLimitMoneyConfirmDlg)
	enum { IDD = IDD_DIALOG_LIMITCONFIRM };
	CMyButton	m_OkBtn;
	CMyButton	m_CancelBtn;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLimitMoneyConfirmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLimitMoneyConfirmDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIMITMONEYCONFIRMDLG_H__B3E94BC7_277C_4654_BE78_E9F1C481B417__INCLUDED_)
