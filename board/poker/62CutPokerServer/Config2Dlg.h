#if !defined(AFX_CONFIG2DLG_H__5AE76B55_7885_42FB_89BD_2430B1FB02F6__INCLUDED_)
#define AFX_CONFIG2DLG_H__5AE76B55_7885_42FB_89BD_2430B1FB02F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Config2Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfig2Dlg dialog

class CConfig2Dlg : public CDialog
{
// Construction
public:
	CConfig2Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfig2Dlg)
	enum { IDD = IDD_CONFIG2_DIALOG };
	CString	m_NoticeURL;
	int		m_MaxUser;
	int		m_MaxDBThread;
	int		m_LoginDBQueNum;
	BOOL	m_bDBClose;
	CString	m_BannerURL1;
	CString	m_BannerURL2;
	CString	m_ProfileURL;
	CString	m_strSeverInfoMsg;
	CString	m_LocationURL;
	CString	m_ItemURL;
	CString	m_ItemHelpURL;
	CString	m_CashURL;		// 캐쉬 충전URL
	BOOL	m_bDBClose2;  //### [로그 기록용] ###
	int		m_MaxDBThread2;
	int		m_LoginDBQueNum2;
	int		m_SameIPLimit;
	CString	m_strTitleNoticMsg;
	BOOL	m_bUseMacAddr;
	CString	m_LeadersJoinURL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfig2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfig2Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIG2DLG_H__5AE76B55_7885_42FB_89BD_2430B1FB02F6__INCLUDED_)
