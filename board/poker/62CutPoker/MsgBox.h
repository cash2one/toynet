#if !defined(AFX_MSGBOX_H__CECF6661_41E2_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_MSGBOX_H__CECF6661_41E2_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgBox dialog

class CMsgBox : public CDialog
{
// Construction
public:
	CString Msg, Title;
	CMsgBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMsgBox)
	enum { IDD = IDD_MSGBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgBox)
	public:
	virtual int DoModal(char* strMsg, char* strTitle);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgBox)
	afx_msg void OnYes();
	afx_msg void OnNo();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGBOX_H__CECF6661_41E2_11D4_97A5_0050BF0FBE67__INCLUDED_)
