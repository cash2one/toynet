#if !defined(AFX_ADMINMANAGER_H__8DCA037A_963D_4C38_972E_2855375777E5__INCLUDED_)
#define AFX_ADMINMANAGER_H__8DCA037A_963D_4C38_972E_2855375777E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminManager.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdminManager dialog
//### [관리자 모드 작업] ###
class CAdminManager : public CDialog
{
// Construction
public:
	void DelItem(CString strID);
	void InitList();
	void InsItem(CString strID, int grade, CString strIP);
	CAdminManager(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdminManager)
	enum { IDD = IDD_DLG_ADMINMAN };
	CIPAddressCtrl	m_ctrlIP;
	CListCtrl	m_ctrlAdminList;
	CString	m_strID;
	int		m_nGrade;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminManager)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdminManager)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListAdmin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	virtual void OnOK();
	afx_msg void OnBtnAdmininfosend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINMANAGER_H__8DCA037A_963D_4C38_972E_2855375777E5__INCLUDED_)
