#if !defined(AFX_OBSERVERDLG_H__C7B7722C_1378_496C_8237_6851CE7549F7__INCLUDED_)
#define AFX_OBSERVERDLG_H__C7B7722C_1378_496C_8237_6851CE7549F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObserverDlg.h : header file
//

//
#include "ListCtrlEx.h"
///////////////////////////////////////////////////////////////////////////
// CObserverDlg dialog

class CObserverDlg : public CDialog
{
// Construction
public:
	void SetObserverViewBtn();
	void User_SetItemReservation(int unum, char *id, int seq);
	void User_DelItem(int unum, char* id);
	void User_SetItem(int unum, char *id, BOOL sex, int level, INT64 money, int seq);
	void User_InsItem(int unum, char *id, BOOL sex, int level, INT64 money, int seq);
	void User_InitList();

	CMyBitmap Back;
	CImageList m_UserImage;

	CRect OrgWindRect, OrgListRect;
	int ViewMode;
	int nUserSel;

	CObserverDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObserverDlg)
	enum { IDD = IDD_DLG_OBSERVER };
	CMyButton	m_OutBtn;
	CMyButton	m_InBtn;
	CMyButton	m_CloseBtn;
	CMyButton	m_ProfileBtn;
	CListCtrlEx	m_UserList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObserverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObserverDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnProfile();
	afx_msg void OnBtnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonIn();
	afx_msg void OnButtonOut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBSERVERDLG_H__C7B7722C_1378_496C_8237_6851CE7549F7__INCLUDED_)
