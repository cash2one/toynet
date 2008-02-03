#if !defined(AFX_ASKBUYITEMDLG_H__99140DF9_A59C_4DFF_BB16_5366137225E9__INCLUDED_)
#define AFX_ASKBUYITEMDLG_H__99140DF9_A59C_4DFF_BB16_5366137225E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AskBuyItemDlg.h : header file
//

#include "MyButton.h"
//#include "UserStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CAskBuyItemDlg dialog

class CAskBuyItemDlg : public CDialog
{
// Construction
public:
	CAskBuyItemDlg(CWnd* pParent = NULL);   // standard constructor

	CBrush		m_HollowBrush;

	int m_ItemCode;

	void DrawGameItem(CDC& dc);

// Dialog Data
	//{{AFX_DATA(CAskBuyItemDlg)
	enum { IDD = IDD_ASKBUYITEM_DIALOG };
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	CString	m_strMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAskBuyItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAskBuyItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASKBUYITEMDLG_H__99140DF9_A59C_4DFF_BB16_5366137225E9__INCLUDED_)
