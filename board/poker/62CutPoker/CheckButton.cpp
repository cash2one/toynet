// CheckButton.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "CheckButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckButton

CCheckButton::CCheckButton()
{
	m_bChecked = FALSE;
	m_pParent = NULL;
}

CCheckButton::~CCheckButton()
{
}


BEGIN_MESSAGE_MAP(CCheckButton, CButton)
	//{{AFX_MSG_MAP(CCheckButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckButton message handlers
BOOL CCheckButton::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}



void CCheckButton::Init( int x, int y, LPSTR checked_filename, LPSTR unchecked_filename, CWnd* pParent )
{
	if( pParent == NULL ) return;

	m_pt.x = x;
	m_pt.y = y;

	m_bmp[0].LoadBitmapFile( unchecked_filename );
	m_bmp[1].LoadBitmapFile( checked_filename );

	m_bChecked = FALSE;

	CRect rc;
	GetClientRect( rc );
	rc.OffsetRect( x, y );

	MoveWindow( rc );

	m_pParent = pParent;
}

void CCheckButton::Init( LPCSTR checked_filename, LPSTR unchecked_filename, CWnd* pParent ) 
{
	if( pParent == NULL ) return;

	if(m_bmp[0].m_hObject) { m_bmp[0].DeleteObject(); m_bmp[0].m_hObject=NULL; }
	if(m_bmp[1].m_hObject) { m_bmp[1].DeleteObject(); m_bmp[1].m_hObject=NULL; }
	
	m_bmp[0].LoadBitmapFile( unchecked_filename );
	m_bmp[1].LoadBitmapFile( checked_filename );

	int imagenum = 1;
	// 비트맵 사이즈에 맞춰서 버튼사이즈를 조절함
	if(GetSafeHwnd() && m_bmp[0].m_hObject) {
		BITMAP bm;
		::GetObject(m_bmp[0].m_hObject, sizeof(bm), &bm); // BMP의 가로 세로 사이즈를 얻는데만 씀
		int h = bm.bmHeight/imagenum;
		if(bm.bmWidth>0 && h>0) {
			// 윈도우의 위치와 Z오더는 무시하고 사이즈만 변경시킨다
			SetWindowPos(NULL, 0, 0, bm.bmWidth, h, SWP_NOMOVE | SWP_NOZORDER);
		}
	}

	m_bChecked = FALSE;
	m_pParent = pParent;
}

void CCheckButton::SetCheck( BOOL bCheck )
{
	m_bChecked = bCheck;

	CRect rc;
	GetWindowRect( rc );
	rc.left -= 5; rc.top -= 5;
	rc.right += 5; rc.bottom += 5;

	if( m_pParent )
	{
		m_pParent->ScreenToClient( rc );
		m_pParent->InvalidateRect( rc, FALSE );
	}
}

void CCheckButton::OnClicked() 
{
	// TODO: Add your control notification handler code here

	m_bChecked = !m_bChecked;

	CRect rc;
	GetWindowRect( rc );
	rc.left -= 5; rc.top -= 5;
	rc.right += 5; rc.bottom += 5;

	

	if( m_pParent )
	{
		m_pParent->ScreenToClient( rc );
		m_pParent->InvalidateRect( rc, FALSE );

	}

//	m_pParent->PostMessage(WM_COMMAND, GetDlgCtrlID(), (long)GetSafeHwnd());
//	(GetParent())->PostMessage(WM_COMMAND, GetDlgCtrlID(), (long)GetSafeHwnd());
	Invalidate(FALSE);
}

void CCheckButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	if( m_bmp[m_bChecked].m_hObject == NULL ) return;

	cdc.SelectObject(m_bmp[m_bChecked]);

	dc.BitBlt( 0, 0, m_bmp[m_bChecked].GetWidth(), m_bmp[m_bChecked].GetHeight(), &cdc, 0, 0, SRCCOPY );

	cdc.DeleteDC();

	// Do not call CButton::OnPaint() for painting messages
}

void CCheckButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

    //  this class works only for push buttons
    ASSERT (lpDrawItemStruct->CtlType == ODT_BUTTON);
    
    //  get the device context pointer to draw into
    CDC*	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CDC cdc;
	cdc.CreateCompatibleDC(pDC);
	if(cdc.m_hDC == NULL) return;
	if( m_bmp[m_bChecked].m_hObject == NULL ) return;

	cdc.SelectObject(m_bmp[m_bChecked]);

	pDC->BitBlt( 0, 0, m_bmp[m_bChecked].GetWidth(), m_bmp[m_bChecked].GetHeight(), &cdc, 0, 0, SRCCOPY );
}

void CCheckButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect( rc );
	if( rc.PtInRect( point ) )
	{
		OnClicked();
	}

//	CButton::OnLButtonDown(nFlags, point);
}

BOOL CCheckButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
//	return CButton::OnEraseBkgnd(pDC);
}

void CCheckButton::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CButton::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CCheckButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CButton::OnLButtonUp(nFlags, point);
}

void CCheckButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CButton::OnMouseMove(nFlags, point);
}

void CCheckButton::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CButton::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}

HBRUSH CCheckButton::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

	CDC cdc;
	cdc.CreateCompatibleDC(pDC);
	if(cdc.m_hDC == NULL) return NULL;
	if( m_bmp[m_bChecked].m_hObject == NULL ) return NULL;

	cdc.SelectObject(m_bmp[m_bChecked]);

	pDC->BitBlt( 0, 0, m_bmp[m_bChecked].GetWidth(), m_bmp[m_bChecked].GetHeight(), &cdc, 0, 0, SRCCOPY );
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

void CCheckButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnLButtonDown( nFlags, point );
	
//	CButton::OnLButtonDblClk(nFlags, point);
}
