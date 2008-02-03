#if !defined(AFX_ROOMOPTIONDLG_H__43ABACD6_58F5_443D_8506_40C357722579__INCLUDED_)
#define AFX_ROOMOPTIONDLG_H__43ABACD6_58F5_443D_8506_40C357722579__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RoomOptionDlg.h : header file
//

#include "MyButton.h"
#include "Graphic.h"

/////////////////////////////////////////////////////////////////////////////
// CRoomOptionDlg dialog

class CRoomOptionDlg : public CDialog
{
// Construction
public:

	CMyBitmap Back;
	CPage Page;
	CRect CardRectBig, CardRect[16]; // 카드뒷면

	int m_aMyCard[16]; // 카드
	int m_nMyCard;  // 나의 대표카드.


	CRoomOptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRoomOptionDlg)
	enum { IDD = IDD_ROOMOPTIONDLG };
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	CComboBox	m_ctrTitle;
	BOOL	m_bWhis;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoomOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRoomOptionDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckWhis();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROOMOPTIONDLG_H__43ABACD6_58F5_443D_8506_40C357722579__INCLUDED_)
