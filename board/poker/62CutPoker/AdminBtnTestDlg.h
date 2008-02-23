#if !defined(AFX_ADMINBTNTESTDLG_H__C35FA918_5AF9_4209_8E70_36668017AC5B__INCLUDED_)
#define AFX_ADMINBTNTESTDLG_H__C35FA918_5AF9_4209_8E70_36668017AC5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminBtnTestDlg.h : header file
//
#include "Graphic.h"
#include "GraphButton.h"

#include "MyBitmap.h"

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAdminBtnTestDlg dialog

class CAdminBtnTestDlg : public CDialog
{
// Construction
public:
	CAdminBtnTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdminBtnTestDlg)
	enum { IDD = IDD_DLG_ADMIN_BTNTEST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminBtnTestDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	xSprite				testBtnSpr_;
	xSprite				testBackSpr_;

	CPage				page_;
	

	BOOL				bLeft_;
	BOOL				bRight_;
	BOOL				bSelect_;

	int					mouse_;

	BOOL				bBtnX_;
	BOOL				bBtnY_;
	BOOL				bBtnZ_;

	BOOL				bBtnA_;
	BOOL				bBtnB_;
	BOOL				bBtnC_;

	//CMyBitmap			myBitmap_;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdminBtnTestDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINBTNTESTDLG_H__C35FA918_5AF9_4209_8E70_36668017AC5B__INCLUDED_)
