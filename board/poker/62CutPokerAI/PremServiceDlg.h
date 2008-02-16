#if !defined(AFX_PREMSERVICEDLG_H__1CA6BC36_56C7_41D5_B9B9_D9E39C2A482A__INCLUDED_)
#define AFX_PREMSERVICEDLG_H__1CA6BC36_56C7_41D5_B9B9_D9E39C2A482A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PremServiceDlg.h : header file
//

#include "MyButton.h"
#include "MyBitmap.h"
#include "BmpToRegion.h"

/////////////////////////////////////////////////////////////////////////////
// CPremServiceDlg dialog

class CPremServiceDlg : public CDialog
{
// Construction
public:
	CPremServiceDlg(CWnd* pParent = NULL);   // standard constructor

	CBmpRegion	m_BmpRegion;
	UINT		m_hTimer;
	int			m_TimerCount;

	BOOL Create(CWnd* pParentWnd);

// Dialog Data
	//{{AFX_DATA(CPremServiceDlg)
	enum { IDD = IDD_PREMSERVICE_DIALOG };
	CMyButton	m_OKBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPremServiceDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPremServiceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREMSERVICEDLG_H__1CA6BC36_56C7_41D5_B9B9_D9E39C2A482A__INCLUDED_)
