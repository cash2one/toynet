// BmpRadioBtn.cpp : implementation file
//

#include "stdafx.h"
#include "BmpRadioBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpRadioBtn

CBmpRadioBtn::CBmpRadioBtn()
{
	m_bChecked = FALSE;
	m_pParent = NULL;

	m_pPrev = NULL;
	m_pNext = NULL;
}

CBmpRadioBtn::~CBmpRadioBtn()
{
}


BEGIN_MESSAGE_MAP(CBmpRadioBtn, CButton)
	//{{AFX_MSG_MAP(CBmpRadioBtn)
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
// CBmpRadioBtn message handlers

void CBmpRadioBtn::Init( int x, int y, LPSTR checked_filename, LPSTR unchecked_filename, CWnd* pParent )
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
}

void CBmpRadioBtn::SetSizeToBmp()
{
	CRect rc;
	GetWindowRect( rc );

	rc.right = rc.left + m_bmp[1].GetWidth();
	rc.bottom = rc.top + m_bmp[1].GetHeight();
	MoveWindow( rc );
}

void CBmpRadioBtn::SetCheck( BOOL bCheck )
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

void CBmpRadioBtn::OnClicked() 
{
	// TODO: Add your control notification handler code here
	GroupCheck( FALSE );
	SetCheck( TRUE );

	if( m_pParent )
	{
		m_pParent->SendMessage( WM_COMMAND, GetDlgCtrlID(), 0 );
	}
}

void CBmpRadioBtn::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	if( m_bmp[m_bChecked].m_hObject == NULL ) return;

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;	

	cdc.SelectObject(m_bmp[m_bChecked]);

	dc.BitBlt( 0, 0, m_bmp[m_bChecked].GetWidth(), m_bmp[m_bChecked].GetHeight(), &cdc, 0, 0, SRCCOPY );

	// Do not call CButton::OnPaint() for painting messages
}

void CBmpRadioBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

    //  this class works only for push buttons

	if( m_bmp[m_bChecked].m_hObject == NULL ) return;

    ASSERT (lpDrawItemStruct->CtlType == ODT_BUTTON);
    
    //  get the device context pointer to draw into
    CDC*	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CDC cdc;
	cdc.CreateCompatibleDC(pDC);
	if(cdc.m_hDC == NULL) return;	

	cdc.SelectObject(m_bmp[m_bChecked]);

	pDC->BitBlt( 0, 0, m_bmp[m_bChecked].GetWidth(), m_bmp[m_bChecked].GetHeight(), &cdc, 0, 0, SRCCOPY );
}

void CBmpRadioBtn::OnLButtonDown(UINT nFlags, CPoint point) 
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

BOOL CBmpRadioBtn::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
//	return CButton::OnEraseBkgnd(pDC);
}

void CBmpRadioBtn::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CButton::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CBmpRadioBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CButton::OnLButtonUp(nFlags, point);
}

void CBmpRadioBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CButton::OnMouseMove(nFlags, point);
}

void CBmpRadioBtn::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CButton::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}

HBRUSH CBmpRadioBtn::CtlColor(CDC* pDC, UINT nCtlColor) 
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

void CBmpRadioBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnLButtonDown( nFlags, point );
	
//	CButton::OnLButtonDblClk(nFlags, point);
}

void CBmpRadioBtn::AddToGroup( CBmpRadioBtn* pBtn )
{
	if( pBtn == NULL ) return;

	pBtn->SetNext( m_pNext );
	pBtn->SetPrev( this );

	if( m_pNext ) m_pNext->SetPrev( pBtn );
	m_pNext = pBtn;
}

void CBmpRadioBtn::RemoveFromGroup()
{
	if( m_pPrev )
	{
		m_pPrev->SetNext( m_pNext );
	}

	if( m_pNext )
	{
		m_pNext->SetPrev( m_pPrev );
	}
}

void CBmpRadioBtn::GroupCheck( BOOL bSet )
{
	// 무한루프에 빠지게될 가능성을 막기 위해....
	int max_group = 1000;
	CBmpRadioBtn* pBtn = m_pPrev;
	for( int i = 0; i < max_group; i++ )
	{
		if( pBtn ) pBtn->SetCheck( bSet );
		else break;

		pBtn = pBtn->m_pPrev;
	}

	pBtn = m_pNext;
	for( i = 0; i < max_group; i++ )
	{
		if( pBtn ) pBtn->SetCheck( bSet );
		else break;

		pBtn = pBtn->m_pNext;
	}
}