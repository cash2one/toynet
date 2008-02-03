#if !defined(AFX_CONFIG3DLG_H__9DB97E02_ABCF_41E9_9A63_BDF85BEF2018__INCLUDED_)
#define AFX_CONFIG3DLG_H__9DB97E02_ABCF_41E9_9A63_BDF85BEF2018__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Config3Dlg.h : header file
//

#include "Global.h"

/////////////////////////////////////////////////////////////////////////////
// CConfig3Dlg dialog

void CipherFormat(CString& strout, int val, int cipher);

class CConfig3Dlg : public CDialog
{
// Construction
public:
	CConfig3Dlg(CWnd* pParent = NULL);   // standard constructor

	void SetControlState();
	void InitList();
	void InsertItem(int no, CHANINFO_CFG *pCI);
	void SetItem(int no, CHANINFO_CFG *pCI);
	CHANINFO_CFG GetItemInfo(int no);

// Dialog Data
	//{{AFX_DATA(CConfig3Dlg)
	enum { IDD = IDD_CONFIG3_DIALOG };
	CListCtrl	m_ChanInfoList;
	int		m_EnterLimitMode;
	BOOL	m_bLoginLimit;
	BOOL	m_bChanLimit;
	int		m_LoginLimit1;
	int		m_LoginLimit2;
	CString	m_strFM;
	CString	m_strGM;
	CString	m_strHM;
	UINT	m_nHL;
	UINT	m_nFL;
	UINT	m_nGL;
	BOOL	m_bExamServer;
	int		m_LoginGrade;
	CString	m_strCM;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfig3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfig3Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDblclkListChaninfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSeledit();
	afx_msg void OnColumnclickListChaninfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioEinterlimitmode0();
	afx_msg void OnRadioEinterlimitmode1();
	afx_msg void OnCheckLoginlimit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIG3DLG_H__9DB97E02_ABCF_41E9_9A63_BDF85BEF2018__INCLUDED_)
