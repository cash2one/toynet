#if !defined(AFX_NOTICEDLG_H__198CD483_C001_4AA0_903E_88D915DBF814__INCLUDED_)
#define AFX_NOTICEDLG_H__198CD483_C001_4AA0_903E_88D915DBF814__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoticeDlg.h : header file
//

#include "MyButton.h"
#include "MyBitmap.h"
#include "BmpToRegion.h"

#define MAX_NOTICE_STR_NUM		 1024


/////////////////////////////////////////////////////////////////////////////
// CNoticeDlg dialog

class CNoticeDlg : public CDialog
{
// Construction
public:
	CNoticeDlg(CWnd* pParent = NULL);   // standard constructor
	void				SetNoticeStr(CString clNoticeStr);
	
	CBmpRegion	m_BmpRegion;
	CMyBitmap	m_clBack;
	CMyBitmap	m_clTitle;

	CFont		m_clFont;


	

	
	int			m_iNoticeCharNum;		// 공지 한줄에 글자수
	int			m_iNoticeLineNum;		// 공지 줄수
	
	char		m_cNoticeStr[50][MAX_NOTICE_STR_NUM];
	
// Dialog Data
	//{{AFX_DATA(CNoticeDlg)
	enum { IDD = IDD_NOTICE_DIALOG };
	CMyButton	m_clOKBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoticeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNoticeDlg)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTICEDLG_H__198CD483_C001_4AA0_903E_88D915DBF814__INCLUDED_)
