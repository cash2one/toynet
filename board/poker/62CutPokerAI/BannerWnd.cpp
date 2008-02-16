// BannerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BannerWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBannerWnd

CBannerWnd::CBannerWnd()
{
	m_pWebCtrl = NULL;
}

CBannerWnd::~CBannerWnd()
{
	if(m_pWebCtrl) {
		delete m_pWebCtrl;
		m_pWebCtrl = NULL;
	}
}


BEGIN_MESSAGE_MAP(CBannerWnd, CStatic)
	//{{AFX_MSG_MAP(CBannerWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBannerWnd message handlers

void CBannerWnd::SetBmp(LPCTSTR fname)
{
	strFile = fname;
}

void CBannerWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	Back.LoadBitmapFile(strFile);

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.DeleteDC();

	Back.DeleteObject();

	// Do not call CStatic::OnPaint() for painting messages
}

int CBannerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if(m_strURL.GetLength() > 0) OpenURL(m_strURL);

	return 0;
}

void CBannerWnd::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pWebCtrl) {
		delete m_pWebCtrl;
		m_pWebCtrl = NULL;
	}
}

BOOL CBannerWnd::CreateWebCtrl()
{
	if(!GetSafeHwnd()) return FALSE;

	CRect rc;
	GetClientRect(rc);

	if(!m_pWebCtrl) {
		m_pWebCtrl= new CWebCtrl();
	}

	// 웹 컨트롤 생성
	if(!m_pWebCtrl->GetSafeHwnd()) {
		if(m_pWebCtrl->Create(NULL, "WebControl", WS_VISIBLE|WS_CHILD, rc, this, 61321)) {
			if(m_strURL.GetLength() > 0) m_pWebCtrl->Navigate(m_strURL);
		}
		else {
			delete m_pWebCtrl;
			m_pWebCtrl = NULL;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CBannerWnd::OpenURL(LPCTSTR strurl)
{
	m_strURL = strurl;

	if(!m_pWebCtrl) {
		if(!CreateWebCtrl()) return FALSE;
	}
	else {
		if(m_strURL.GetLength() == 0) return FALSE;
		if(m_pWebCtrl && m_pWebCtrl->GetSafeHwnd()) m_pWebCtrl->Navigate2(m_strURL);
	}

	return TRUE;
}

void CBannerWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CStatic::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
//	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
}
