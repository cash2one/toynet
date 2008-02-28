#if !defined(AFX_SYSTEMDLG_H__6BBDB398_2A16_4BC1_9412_EB58773F44B5__INCLUDED_)
#define AFX_SYSTEMDLG_H__6BBDB398_2A16_4BC1_9412_EB58773F44B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemDlg dialog

class CSystemDlg : public CDialog
{
// Construction
public:
	CSystemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSystemDlg)
	enum { IDD = IDD_SYSTEM_DIALOG };
	CDateTimeCtrl	m_dtcDate;
	CDateTimeCtrl	m_dtcTime;
	CComboBox	m_cbDay;
	CComboBox	m_cbMonth;
	CComboBox	m_cbYear;
	CListCtrl	m_lcData;
	//}}AFX_DATA

	void ShowToday();
	void ShowDay();
	void ShowMonth();
	void ShowYear();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnShowtoday();
	afx_msg void OnBtnShowday();
	afx_msg void OnBtnShowmonth();
	afx_msg void OnBtnShowyear();
	afx_msg void OnBtnToday();
	afx_msg void OnBtnApplytime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMDLG_H__6BBDB398_2A16_4BC1_9412_EB58773F44B5__INCLUDED_)
