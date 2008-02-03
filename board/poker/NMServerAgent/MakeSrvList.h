#if !defined(AFX_MAKESRVLIST_H__32991008_4C9C_44F8_B810_BE2CC8E73343__INCLUDED_)
#define AFX_MAKESRVLIST_H__32991008_4C9C_44F8_B810_BE2CC8E73343__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MakeSrvList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeSrvList dialog

class CMakeSrvList : public CDialog
{
// Construction
public:
	void Selection();
	CMakeSrvList(CWnd* pParent = NULL);   // standard constructor

	CMenu m_ctrMenu;  // 팝업 메뉴
	int   m_nSelNum;

	void InitList();
	void InsItem(char *Name, char *IP, UINT nPort);
	void DelItem(char *Name);
	// 테스트
	void SetItem(char *Name, char *IP, UINT nPort);


// Dialog Data
	//{{AFX_DATA(CMakeSrvList)
	enum { IDD = IDD_SRV_LIST };
	CListCtrl	m_ctrList;
	CString	m_strName;
	CString	m_strIP;
	UINT	m_nPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakeSrvList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMakeSrvList)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButtonMake();
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickListSrv(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListSrv(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuDel();
	afx_msg void OnDblclkListSrv(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKESRVLIST_H__32991008_4C9C_44F8_B810_BE2CC8E73343__INCLUDED_)
