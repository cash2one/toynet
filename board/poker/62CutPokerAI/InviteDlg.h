#if !defined(AFX_INVITEDLG_H__F8F83B77_21B2_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_INVITEDLG_H__F8F83B77_21B2_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InviteDlg.h : header file
//


#include "ListCtrlEx.h"
#include "MyButton.h"
#include "MyBitmap.h"

/////////////////////////////////////////////////////////////////////////////
// CInviteDlg dialog

class CInviteDlg : public CDialog
{
// Construction
public:
	void SetBtnStat();
	CMyBitmap Back;
	CImageList m_UserImage;
	CMenu m_InviteMenu;

	CRect OrgWindRect, OrgListRect;

	int ViewMode;
	int nUserSel;

	CInviteDlg(CWnd* pParent = NULL);   // standard constructor

	void MoveToSide();
	void User_InitList();
	void User_InsItem(int unum, char *id, BOOL sex, int winnum, int loosenum, int drawnum, int point, int position, int roomnum, int level);
	void User_SetItem(int unum, char *id, int winnum, int loosenum, int point, int position, int roomnum, int level);
	void User_DelItem(int unum, char *id);
	void User_ReloadList();

// Dialog Data
	//{{AFX_DATA(CInviteDlg)
	enum { IDD = IDD_INVITE_DIALOG };
	CMyButton	m_BtnAllUser;
	CMyButton	m_BtnLoby;
	CMyButton	m_CloseBtn;
	CMyButton	m_InviteBtn;
	CMyButton	m_ProfileBtn;
	CListCtrlEx	m_UserList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInviteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInviteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonProfile();
	afx_msg void OnButtonInvite();
	afx_msg void OnRadioAlluser();
	afx_msg void OnRadioWaituser();
	afx_msg void OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnRclickListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonWhisper();
	afx_msg void OnButtonClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonLoby();
	afx_msg void OnButtonAlluser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INVITEDLG_H__F8F83B77_21B2_11D4_97A5_0050BF0FBE67__INCLUDED_)
