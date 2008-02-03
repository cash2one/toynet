#if !defined(AFX_LEADERSMONEYLIMIT_H__7C6B4DEC_2D10_4753_BB46_958D0A32BD8F__INCLUDED_)
#define AFX_LEADERSMONEYLIMIT_H__7C6B4DEC_2D10_4753_BB46_958D0A32BD8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeadersMoneyLimit.h : header file
//
#include "MyButton.h"
#include "MyBitmap.h"
#include "UserStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CLeadersMoneyLimit dialog

class CLeadersMoneyLimit : public CDialog
{
// Construction
public:
	LEADERS_JOIN m_LJ;
	
	int m_LeadersKind;
	
	CLeadersMoneyLimit(CWnd* pParent = NULL);   // standard constructor
	BOOL Init(LEADERS_JOIN *pLJoin);

// Dialog Data
	//{{AFX_DATA(CLeadersMoneyLimit)
	enum { IDD = IDD_DIG_LIMITMONEY };
	CMyButton	m_OkBtn;
	CMyButton	m_CancelBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeadersMoneyLimit)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLeadersMoneyLimit)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEADERSMONEYLIMIT_H__7C6B4DEC_2D10_4753_BB46_958D0A32BD8F__INCLUDED_)
