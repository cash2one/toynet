#if !defined(AFX_WEBCTRL_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_WEBCTRL_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWebCtrl html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CWebCtrl : public CHtmlView
{
//protected:
public:
	CWebCtrl();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWebCtrl)

// html Data
public:
	//{{AFX_DATA(CWebCtrl)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

	//BOOL m_bHideBorder;

// Operations
public:

	//void SetOption(BOOL bHideBorder);
	//BOOL InsertHTML(LPCTSTR strHtml);
	//BOOL ExecJavascript(LPCTSTR strScript);
	//BOOL PutCharSet(LPCTSTR strCharset);

protected:
	//void SetBorderNone();

public:

	//virtual void OnBeforeNavigate2( LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel );
	virtual void OnNavigateComplete2( LPCTSTR strURL );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
//protected:
	virtual ~CWebCtrl();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWebCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBCTRL_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_)
