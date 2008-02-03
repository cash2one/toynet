#if !defined(AFX_CREATEROOMDLG_H__801F1DC1_2037_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_CREATEROOMDLG_H__801F1DC1_2037_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateRoomDlg.h : header file
//

#include "MyButton.h"
#include "MyBitmap.h"
#include "CheckButton.h"
#include "BmpCheckButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCreateRoomDlg dialog

class CCreateRoomDlg : public CDialog
{
public:
	CMyBitmap Back;
	BOOL bSecret;
	int nMaxNum;
	int Level;
	int GameKind;
	int FormKind;
	int ComboSelCreateRGrade;      // 최소머니 등급

	BOOL m_bHaveSupermaster;		//슈퍼 방장을 가지고 있을때 
	BOOL m_bSuperRoomMaster;		//슈퍼 방장기능 사용여부.m_bSuperMaster 를 대신한다.	

	int m_nPlayer;

	CBrush		m_HollowBrush;


	CFont Font1, Font2;

	BOOL  m_bMakeRoom;
	INT64 m_nUserMoney;
	int   m_nUserIcon;

	//CCreateRoomDlg(CWnd* pParent = NULL);   // standard constructor
	CCreateRoomDlg(int nIcon,INT64 nMoney,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateRoomDlg)
	enum { IDD = IDD_CREATEROOM_DIALOG };
	CBmpCheckButton	m_ckSuperMaster;
	CComboBox	m_MinMCombo;
	CComboBox	m_FormKindCombo;
	CEdit	m_ctrRoomTitle;
	CComboBox	m_GameKindCombo;
	CComboBox	m_LevelCombo;
	CComboBox	m_MaxNum;
	CComboBox	m_SecretCombo;
	CMyButton	m_CancelBtn;
	CMyButton	m_OKBtn;
	CString	m_RoomTitle;
	CString	m_Pass;
	int		m_nBat;
	int		m_nOpen;
	int		m_nStyle;
	BOOL	m_bSuperMaster;
	int		m_bPlayer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateRoomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateRoomDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboSecret();
	afx_msg void OnSelchangeComboMaxnum();
	afx_msg void OnSelchangeComboLevel();
	afx_msg void OnSelchangeComboFormkind();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheckSupermaster();
	afx_msg void OnSelchangeComboMinim();
	afx_msg void OnRadioUsercount5();
	afx_msg void OnRadioUsercount6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEROOMDLG_H__801F1DC1_2037_11D4_97A5_0050BF0FBE67__INCLUDED_)
