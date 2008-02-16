#if !defined(AFX_MSGBOXDLG_H__108F2455_B6B4_49F2_92C8_B7C8310697DA__INCLUDED_)
#define AFX_MSGBOXDLG_H__108F2455_B6B4_49F2_92C8_B7C8310697DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgBoxDlg.h : header file
//
#include "MyButton.h"
#include "ChatViewEdit.h"

#define MSGBOXDLG_WAIT_TIMER 339
#define MAX_NOTICE_STR_NUM		 1024

/////////////////////////////////////////////////////////////////////////////
// CMsgBoxDlg dialog

class CMsgBoxDlg : public CDialog
{
// Construction
public:
	void SetLineCount( CString clNoticeStr );
	int iMode;
	CString szMsgText;
	int hTimerWait;

	int			m_iNoticeCharNum;		// 공지 한줄에 글자수
	int			m_iNoticeLineNum;		// 공지 줄수
	
	char		m_cNoticeStr[50][MAX_NOTICE_STR_NUM];

	void MsgBox(int mode, CString &msgstr);
	void SetWindowResize();

	void MsgBox(int mode, char *msgstr);
	CMsgBoxDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMsgBoxDlg)
	enum { IDD = IDD_MSG_BOX };
	CChatViewEdit	m_ctrlNotic;
	CMyButton	m_OkBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgBoxDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgBoxDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGBOXDLG_H__108F2455_B6B4_49F2_92C8_B7C8310697DA__INCLUDED_)
