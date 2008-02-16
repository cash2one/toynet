#if !defined(AFX_WHISPERDLG_H__66883C61_413A_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_WHISPERDLG_H__66883C61_413A_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WhisperDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWhisperDlg dialog

class CWhisperDlg : public CDialog
{
// Construction
public:
	BOOL bCreated;
	CString TarID;
	CWhisperDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWhisperDlg)
	enum { IDD = IDD_WHISPER_DIALOG };
	CString	m_strChat;
	CString	m_strWho;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWhisperDlg)
	public:
	virtual BOOL Create(char *id);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWhisperDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHISPERDLG_H__66883C61_413A_11D4_97A5_0050BF0FBE67__INCLUDED_)
