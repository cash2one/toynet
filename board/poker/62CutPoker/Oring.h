#if !defined(AFX_ORING_H__149E8A68_4CE8_4E0E_8538_3E0C8FC06404__INCLUDED_)
#define AFX_ORING_H__149E8A68_4CE8_4E0E_8538_3E0C8FC06404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Oring.h : header file
//

#include "MyButton.h"
#include "MyBitmap.h"
#include "CharView.h"

/////////////////////////////////////////////////////////////////////////////
// COring dialog

class COring : public CDialog
{
// Construction
public:
	CMyBitmap	Back;
//	CCharView	m_CharView;

	COring(CWnd* pParent = NULL);   // standard constructor
	BOOL    m_ShowDlg;

// Dialog Data
	//{{AFX_DATA(COring)
	enum { IDD = IDD_ORING };
	CMyButton	m_Ok;
	CMyButton	m_Cancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COring)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COring)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnButtonLevel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORING_H__149E8A68_4CE8_4E0E_8538_3E0C8FC06404__INCLUDED_)
