#if !defined(AFX_ITEMWEBCTRL_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_ITEMWEBCTRL_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemWebCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemWebCtrl html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CItemWebCtrl : public CHtmlView
{
//protected:
public:
	CItemWebCtrl();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CItemWebCtrl)

// html Data
public:
	//{{AFX_DATA(CItemWebCtrl)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

	BOOL m_bHideBorder;
	BOOL m_bNavigateStart;
	int	m_NavigatedCount;

// Operations
public:

	void SetOption(BOOL bHideBorder);
	BOOL PutCharSet(LPCTSTR strCharset);
	BOOL InsertHTML(LPCTSTR strHtml);
	BOOL ExecJavascript(LPCTSTR strScript);

protected:

	CString m_strCharset;
	CString m_strInsHtml;
	CString m_strRunScript;

	void SetBorderNone();
	BOOL _PutCharSet();
	BOOL _InsertHTML();
	BOOL _ExecJavascript();

public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemWebCtrl)
	public:
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnNavigateComplete2( LPCTSTR strURL );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
//protected:
	virtual ~CItemWebCtrl();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CItemWebCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMWEBCTRL_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_)
