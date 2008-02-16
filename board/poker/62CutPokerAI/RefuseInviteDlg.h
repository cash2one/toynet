#if !defined(AFX_REFUSEINVITEDLG_H__503DD5A3_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_REFUSEINVITEDLG_H__503DD5A3_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RefuseInviteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRefuseInviteDlg dialog
#include "MyButton.h"
#include "MyBitmap.h"

class CRefuseInviteDlg : public CDialog
{
// Construction
public:
	CString szID, szMsg;
	CRefuseInviteDlg(CWnd* pParent = NULL);   // standard constructor

	CMyBitmap Back;

// Dialog Data
	//{{AFX_DATA(CRefuseInviteDlg)
	enum { IDD = IDD_REFUSE_DIALOG };
	CMyButton	m_OkBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRefuseInviteDlg)
	public:
	virtual int DoModal(char *id, char *strmsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRefuseInviteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REFUSEINVITEDLG_H__503DD5A3_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_)
