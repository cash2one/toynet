#if !defined(AFX_ROOMPASSDLG_H__239A4D24_4370_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_ROOMPASSDLG_H__239A4D24_4370_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RoomPassDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRoomPassDlg dialog

class CRoomPassDlg : public CDialog
{
// Construction
public:
	CRoomPassDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRoomPassDlg)
	enum { IDD = IDD_ROOMPASS_DIALOG };
	CString	m_Pass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoomPassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRoomPassDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROOMPASSDLG_H__239A4D24_4370_11D4_97A5_0050BF0FBE67__INCLUDED_)
