// KHSToolTip.cpp : implementation file
//

#include "stdafx.h"
#include "KHSToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKHSToolTip

CKHSToolTip::CKHSToolTip()
{
}

CKHSToolTip::~CKHSToolTip()
{
}


BEGIN_MESSAGE_MAP(CKHSToolTip, CToolTipCtrl)
	//{{AFX_MSG_MAP(CKHSToolTip)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKHSToolTip message handlers

BOOL CKHSToolTip::WndTip(CWnd * pWnd, LPCTSTR pszText)
{
	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_IDISHWND|TTF_SUBCLASS;
	ti.hwnd   = pWnd->GetParent()->GetSafeHwnd();
	ti.uId    = (UINT)pWnd->GetSafeHwnd();
	ti.hinst  = AfxGetInstanceHandle();
	ti.lpszText = (LPSTR)pszText;

	return (BOOL)SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti);
}

BOOL CKHSToolTip::RectTip(CWnd * pWnd, LPCTSTR pszText, LPRECT lpRect, UINT nIDRESOUCE)
{
	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd   = pWnd->GetSafeHwnd();
	ti.uId    = nIDRESOUCE;
	ti.hinst  = AfxGetInstanceHandle();
	ti.lpszText = (LPSTR)pszText;
	::CopyRect(&ti.rect, lpRect);

	return (BOOL)SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti);
}
