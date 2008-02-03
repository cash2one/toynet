// BmpCheckButton.cpp : implementation file
//

#include "stdafx.h"
#include "BmpCheckButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpCheckButton

CBmpCheckButton::CBmpCheckButton()
{
	m_bChecked = FALSE;
	m_pParent = NULL;
	m_bSendCmd = FALSE;
}

CBmpCheckButton::~CBmpCheckButton()
{
}


BEGIN_MESSAGE_MAP(CBmpCheckButton, CButton)
	//{{AFX_MSG_MAP(CBmpCheckButton)
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
// CBmpCheckButton message handlers
void CBmpCheckButton::Init( int x, int y, LPSTR checked_filename, LPSTR unchecked_filename, CWnd* pParent, BOOL bSendCmd )
{
	if( pParent == NULL ) return;

	m_pt.x = x;
	m_pt.y = y;

//	if( unchecked_filename )
	m_bmp[0].LoadBitmapFile( unchecked_filename );

//	if( checked_filename )
	m_bmp[1].LoadBitmapFile( checked_filename );

	m_bChecked = FALSE;

	CRect rc;
	GetClientRect( rc );
	rc.OffsetRect( x, y );

	MoveWindow( rc );

	m_pParent = pParent;
	m_bSendCmd = bSendCmd;
}

void CBmpCheckButton::SetSizeToBmp()
{
	CRect rc;
	GetWindowRect( rc );

	rc.right = rc.left + m_bmp[1].GetWidth();
	rc.bottom = rc.top + m_bmp[1].GetHeight();
	MoveWindow( rc );
}

void CBmpCheckButton::SetCheck( BOOL bCheck )
{
	m_bChecked = bCheck;

	CRect rc;
	GetWindowRect( rc );
	rc.left -= 5; rc.top -= 5;
	rc.right = rc.left + m_bmp[1].GetWidth() + 5; 
	rc.bottom = rc.top + m_bmp[1].GetHeight() + 5;

	if( m_pParent )
	{
		m_pParent->ScreenToClient( rc );
		m_pParent->InvalidateRect( rc, FALSE );
	}

	CButton::SetCheck( bCheck );
}

void CBmpCheckButton::OnClicked() 
{
	// TODO: Add your control notification handler code here
	m_bChecked = !m_bChecked;
	SetCheck( m_bChecked );

	if( m_pParent )
	{
		m_pParent->SendMessage( WM_COMMAND, GetDlgCtrlID(), 0 );
	}

}

void CBmpCheckButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	if( m_bmp[m_bChecked].m_hObject == NULL ) return;

	cdc.SelectObject(m_bmp[m_bChecked]);

	dc.BitBlt( 0, 0, m_bmp[m_bChecked].GetWidth(), m_bmp[m_bChecked].GetHeight(), &cdc, 0, 0, SRCCOPY );

	// Do not call CButton::OnPaint() for painting messages
}

void CBmpCheckButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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

void CBmpCheckButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	CRect rc;
//	GetClientRect( rc );

	CRect rc;
	GetClientRect( rc );	
	rc.right = rc.left + m_bmp[1].GetWidth(); 	

	if( rc.PtInRect( point ) )
	{
		OnClicked();
	}

//	CButton::OnLButtonDown(nFlags, point);
}

BOOL CBmpCheckButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
//	return CButton::OnEraseBkgnd(pDC);
}

void CBmpCheckButton::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CButton::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CBmpCheckButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CButton::OnLButtonUp(nFlags, point);
}

void CBmpCheckButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CButton::OnMouseMove(nFlags, point);
}

void CBmpCheckButton::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CButton::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}

HBRUSH CBmpCheckButton::CtlColor(CDC* pDC, UINT nCtlColor) 
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

void CBmpCheckButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnLButtonDown( nFlags, point );
	
//	CButton::OnLButtonDblClk(nFlags, point);
}
