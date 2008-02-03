#if !defined(AFX_PROFILEDLG_H__2C402B81_30AF_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_PROFILEDLG_H__2C402B81_30AF_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfileDlg.h : header file
//

#include "MyButton.h"
#include "CharView.h"
#include "UserStruct.h"
#include "MyBitmap.h"

/////////////////////////////////////////////////////////////////////////////
// CProfileDlg dialog

class CProfileDlg : public CDialog
{
// Construction
public:
	CMyBitmap Back;
	CCharView m_CharView;
	CFont Font1, Font2;
	USERINFO UI;
	BOOL m_bHideBanishBtn;		// [게임 아이템 작업] <-- 추가

	CProfileDlg(CWnd* pParent = NULL);   // standard constructor
//	void Init(USERINFO *pUI);
	void Init(USERINFO *pUI, BOOL bHideBanishBtn=FALSE);	// [게임 아이템 작업] <-- 변경

// Dialog Data
	//{{AFX_DATA(CProfileDlg)
	enum { IDD = IDD_PROFILE_DIALOG };
	CMyButton	m_MyHompyBtn;
	CMyButton	m_BanishBtn;
	CMyButton	m_ProfileCloseBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProfileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonDetailprofile();
	afx_msg void OnButtonBanish();
	afx_msg void OnButtonMyhompy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEDLG_H__2C402B81_30AF_11D4_97A5_0050BF0FBE67__INCLUDED_)
