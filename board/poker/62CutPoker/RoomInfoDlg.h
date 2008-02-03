#if !defined(AFX_ROOMINFODLG_H__CD9E5744_3ACD_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_ROOMINFODLG_H__CD9E5744_3ACD_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RoomInfoDlg.h : header file
//

#include "MyButton.h"
#include "CharView.h"
#include "UserStruct.h"
#include "MyBitmap.h"

#define ID_CHARVIEW0 1000
#define ID_CHARVIEW1 1001
#define ID_CHARVIEW2 1002

/////////////////////////////////////////////////////////////////////////////
// CRoomInfoDlg dialog
#include "ListCtrlEx.h"

class CRoomInfoDlg : public CDialog
{
// Construction
public:
	void User_InsItem(int unum, char* id, BOOL bsex, int level);
	void User_InitList();
	
	CImageList m_UserImage;

	CMyBitmap Back;
	CFont Font1, Font2;

	GAMEINFO m_GI;
	CCharView CharView[MAX_PLAYER];
	CRoomInfoDlg(CWnd* pParent = NULL);   // standard constructor

/*
	int m_CurViewIndex;
	int m_CurUserNumCnt;
	USERINFO m_UserView[2];
*/


// Dialog Data
	//{{AFX_DATA(CRoomInfoDlg)
	enum { IDD = IDD_ROOMINFO_DIALOG };
	CListCtrlEx	m_UserList;
	CMyButton	m_RightBtn;
	CMyButton	m_LeftBtn;
	CMyButton	m_OkBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoomInfoDlg)
	public:
	virtual int DoModal(GAMEINFO *pGI);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRoomInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnButtonLeft();
	afx_msg void OnButtonRight();
	afx_msg void OnDblclkListObr(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROOMINFODLG_H__CD9E5744_3ACD_11D4_97A5_0050BF0FBE67__INCLUDED_)
