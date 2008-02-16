#if !defined(AFX_BANNERWND_H__70B155E4_B2BB_4BCC_9903_EBA6C8F5B53D__INCLUDED_)
#define AFX_BANNERWND_H__70B155E4_B2BB_4BCC_9903_EBA6C8F5B53D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BannerWnd.h : header file
//

#include "MyBitmap.h"
#include "WebCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CBannerWnd window

class CBannerWnd : public CStatic
{
// Construction
public:
	CString strFile;
	CMyBitmap Back;
	CWebCtrl *m_pWebCtrl;
	CString m_strURL;

	CBannerWnd();

	BOOL OpenURL(LPCTSTR strurl);
	void SetBmp(LPCTSTR fname);
	BOOL CreateWebCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBannerWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBannerWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBannerWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANNERWND_H__70B155E4_B2BB_4BCC_9903_EBA6C8F5B53D__INCLUDED_)
