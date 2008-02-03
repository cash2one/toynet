#if !defined(AFX_STARTDLG_H__06F235A2_817F_42F0_B6C7_5C8F15F66D3F__INCLUDED_)
#define AFX_STARTDLG_H__06F235A2_817F_42F0_B6C7_5C8F15F66D3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StartDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStartDlg dialog
#include "MyBitmap.h"
#include "Graphic.h"
#include "MyButton.h"


#define ID_BUTTON 10

class CStartDlg : public CDialog
{
// Construction
public:
	CMyBitmap Back;

	CStartDlg(CWnd* pParent = NULL);   // standard constructor
	CMyButton m_test1;
	CMyButton m_test2;
	CMyButton m_test3;

	UINT ctlID1;
	UINT ctlID2;
	UINT ctlID3;
	
// Dialog Data
	//{{AFX_DATA(CStartDlg)
	enum { IDD = IDD_START_DIALOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStartDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTDLG_H__06F235A2_817F_42F0_B6C7_5C8F15F66D3F__INCLUDED_)
