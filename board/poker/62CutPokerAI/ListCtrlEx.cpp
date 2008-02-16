/******************************************************************************\
* file   : ListCtrlEx.cpp
* created: 1997.09.22
*
* Zafir Anjum   - Original author.
* Mark Findlay  - IE4 fixes.
* Matthew Bells - Better TitleTips.
*
* description:
* A super CListControl.
* <P>features:
* <UL>
*   <LI>Title Tip item expantion
*   <LI>full row selection<BR>
*       <I>Note:</I> this is also a feature in IE 4.0 with
*       LVS_EX_FULLROWSELECT.
*   <LI>notifies parent selection has changed
*   <LI>supports column dragging (with IE4) which changes the column order
* </UL>
*
\******************************************************************************/
// ListCtrlEx.cpp : implementation of the CListCtrlEx class

#include "stdafx.h"
#include <assert.h>
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx
CHeaderCtrlEx::CHeaderCtrlEx()
{
//	m_colorText		= RGB( 69,  31,   1);
//	m_colorBack		= RGB(255, 228, 207);
//	m_colorBorder	= RGB(209, 185, 155);

	m_colorText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_colorBack = ::GetSysColor(COLOR_WINDOW);
//	m_colorBorder = ::GetSysColor(COLOR_WINDOW);

	m_bDrawShadow = FALSE;
	m_clrShadowColor = RGB(150,150,150);

    BYTE byRed    = GetRValue(m_colorBack);
    BYTE byGreen  = GetGValue(m_colorBack);
    BYTE byBlue   = GetBValue(m_colorBack);
    BYTE byReduce = 50;
    if(byRed >= byReduce)
        byRed = BYTE(byRed - byReduce);
    else
        byRed = 0;
    if(byGreen >= byReduce)
        byGreen = BYTE(byGreen - byReduce);
    else
        byGreen = 0;
    if(byBlue >= byReduce)
        byBlue = BYTE(byBlue - byReduce);
    else
        byBlue = 0;
    m_colorBorder = RGB(byRed, byGreen, byBlue);


	m_pImagePoint = NULL;

	Release();
}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
	Release();
}


BEGIN_MESSAGE_MAP(CHeaderCtrlEx, CHeaderCtrl)
    //{{AFX_MSG_MAP(CHeaderCtrlEx)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx message handlers

void CHeaderCtrlEx::SetColors( COLORREF colorBack, COLORREF colorText)
{
    BYTE byRed    = GetRValue(colorBack);
    BYTE byGreen  = GetGValue(colorBack);
    BYTE byBlue   = GetBValue(colorBack);
    BYTE byReduce = 50;

    if(byRed >= byReduce)
        byRed = BYTE(byRed - byReduce);
    else
        byRed = 0;
    if(byGreen >= byReduce)
        byGreen = BYTE(byGreen - byReduce);
    else
        byGreen = 0;
    if(byBlue >= byReduce)
        byBlue = BYTE(byBlue - byReduce);
    else
        byBlue = 0;

    m_colorBack   = colorBack;
    m_colorText   = colorText;
    m_colorBorder = RGB(byRed, byGreen, byBlue);
}

void CHeaderCtrlEx::SetTextShadow(BOOL bUse)
{
	m_bDrawShadow = bUse;
}

void CHeaderCtrlEx::SetTextShadowColor(COLORREF color)
{
	m_clrShadowColor = color;
}

void CHeaderCtrlEx::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMCUSTOMDRAW  *pNMCD = (NMCUSTOMDRAW*)pNMHDR;
	CBrush brush, *pOldBrush;
	CRect  rcDraw(pNMCD->rc);
	CPen   pen, *pOldPen;
	CDC    *pDC = CDC::FromHandle(pNMCD->hdc);
	UINT   nJustify;
	HDITEM hdi;
	char   szBuffer[MAX_PATH];
	int    nOffset;

	if(pNMCD->dwDrawStage == CDDS_PREPAINT || pNMCD->dwDrawStage == CDDS_PREERASE)
	{
		//	3D 효과를 내기 위해서
		//	검은색으로 채우기
		brush.CreateSolidBrush( RGB( 66, 65, 65 ) );
		pOldBrush = (CBrush *)pDC->SelectObject(&brush);
		pen.CreatePen(PS_SOLID, 1, RGB( 66, 65, 65 ) );
		pOldPen = (CPen *)pDC->SelectObject(&pen);
		pDC->Rectangle(rcDraw);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);


		//	테투리와 안쪽에 설정된 색으로 그리기
		CPen pen1;
		CBrush brush1;
		brush1.CreateSolidBrush( m_colorBack );
		pOldBrush = (CBrush *)pDC->SelectObject(&brush1);
		pen1.CreatePen(PS_SOLID, 1, m_colorBorder );
		pOldPen = (CPen *)pDC->SelectObject(&pen1);

		rcDraw.bottom -= 1;

		pDC->Rectangle(rcDraw);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if(pNMCD->dwDrawStage == CDDS_ITEMPREPAINT )
	{
		// 바탕 사각형 그리기
		{
			COLORREF clrBack = RGB( 255, 255, 255 );
			CRect rcDraw_1 = rcDraw;
			if( m_bLButtonDown )	clrBack = RGB( 66, 65, 66 );
			else
			{
				rcDraw_1.right -= 1;
				rcDraw_1.bottom -= 1;
			}


			//	흰색으로 테두리 렉트그리기
			CPen pen1;
			CBrush brush1;
			brush1.CreateSolidBrush( m_colorBack );
			pOldBrush = (CBrush *)pDC->SelectObject(&brush1);
			pen1.CreatePen(PS_SOLID, 1, clrBack );
			pOldPen = (CPen *)pDC->SelectObject(&pen1);
			pDC->Rectangle(rcDraw_1);
			pDC->SelectObject( pOldBrush );
			pDC->SelectObject( pOldPen );
			pen1.DeleteObject();
			brush1.DeleteObject();


			if( m_bLButtonDown )
			{
				pen1.CreatePen(PS_SOLID, 1, m_colorBack );
				pOldPen = (CPen *)pDC->SelectObject(&pen1);
				CPoint po( rcDraw_1.right + 1, rcDraw_1.top - 1 );
				pDC->MoveTo( po );
				pDC->LineTo( CPoint( rcDraw_1.right, rcDraw_1.bottom ) );
				pDC->SelectObject( pOldBrush );
				pDC->SelectObject( pOldPen );
				pen1.DeleteObject();
			}
			else
			{
				rcDraw.right -= 1;
				rcDraw.bottom -= 1;

				//	검은색으로 테두리
				pen1.DeleteObject();
				pen1.CreatePen( PS_SOLID, 1, RGB( 66, 65, 66 ) );
				pOldPen = (CPen *)pDC->SelectObject(&pen1);
				CPoint po( rcDraw.right, rcDraw.top );
				pDC->MoveTo( po );
				pDC->LineTo( CPoint( rcDraw.right, rcDraw.bottom + 1 ) );
				pDC->SelectObject( pOldPen );



				//	회색으로 테두리 밑과 오른쪽에 라인으로
				pen1.DeleteObject();
				pen1.CreatePen( PS_SOLID, 1, RGB( 132, 130, 132 ) );
				pOldPen = (CPen *)pDC->SelectObject(&pen1);
				po.x = rcDraw.left + 1;
				po.y = rcDraw.bottom - 1;
				pDC->MoveTo( po );
				pDC->LineTo( CPoint( rcDraw.right, rcDraw.bottom - 1 ) );

				po.x = rcDraw.right - 1;
				po.y = rcDraw.top + 1;
				pDC->MoveTo( po );
				pDC->LineTo( CPoint( po.x, rcDraw.bottom - 1 ) );
				pDC->SelectObject( pOldPen );
			}
		}




		pDC->SetTextColor(m_colorText);
		pDC->SetBkMode   (TRANSPARENT);

		hdi.mask       = HDI_TEXT | HDI_FORMAT;
		hdi.fmt        = HDF_LEFT;
		hdi.pszText    = szBuffer;
		hdi.cchTextMax = MAX_PATH;

		// 문자 정렬에 따라서 텍스트 출력
		if(GetItem(pNMCD->dwItemSpec, &hdi))
		{
			nOffset = (pDC->GetTextExtent(_T(" ")).cx) << 1;
			rcDraw.DeflateRect(nOffset, 2, nOffset, 0);
			switch(hdi.fmt & HDF_JUSTIFYMASK)
			{
			case HDF_CENTER:
				nJustify = DT_CENTER;
				break;
			case HDF_RIGHT:
				nJustify = DT_RIGHT;
				break;
			default:
				nJustify = DT_LEFT;
				break;
			}

			if( m_bImageUse && m_pImagePoint != NULL )
			{
				int y = ( rcDraw.Height() + 1 ) - m_ImgHeight;
				if( y < 0 )
				{
					CString str;
					str.Format( "ListCtrl Header 이미지의 크기가 너무 큽니다.\n\n세로가 %d 픽셀보다 작아야 합니다", rcDraw.Height() + 1 );
					MessageBox( str, "Info" );
					return;
				}
				else if( y == 0 )	y = 2;
				else if( y == 1 )	y = 1;
				else				y = 0;


				rcDraw.right = rcDraw.left + min( rcDraw.Width(), m_pImagePoint[ pNMCD->dwItemSpec ].y );
				TransDraw( pDC->m_hDC, rcDraw.left, rcDraw.top - y, rcDraw.Width(),
									m_ImgHeight, m_pImagePoint[ pNMCD->dwItemSpec ].x, 0, m_clrColorKey );
			}
			else
			{
				if(!m_bDrawShadow) {
					pDC->SetTextColor(m_colorText);
					pDC->DrawText(szBuffer, lstrlen(szBuffer), &rcDraw, nJustify | DT_SINGLELINE | DT_VCENTER);
				}
				else {
					pDC->SetTextColor(m_clrShadowColor);
					rcDraw.OffsetRect(1,1);
					pDC->DrawText(szBuffer, lstrlen(szBuffer), &rcDraw, nJustify | DT_SINGLELINE | DT_VCENTER);

					pDC->SetTextColor(m_colorText);
					rcDraw.OffsetRect(-1,-1);
					pDC->DrawText(szBuffer, lstrlen(szBuffer), &rcDraw, nJustify | DT_SINGLELINE | DT_VCENTER);
				}
			}
		}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		*pResult = CDRF_SKIPDEFAULT;
	}
	else
	{
		*pResult = CDRF_DODEFAULT;
	}
}



//	================== 그림으로 처리 =============================
void CHeaderCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDown = true;
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CHeaderCtrlEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDown = false;
	CHeaderCtrl::OnLButtonUp(nFlags, point);
}

void CHeaderCtrlEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_bLButtonDown )
	{
		CRect rt;
		GetWindowRect( &rt );
		if( !rt.PtInRect( point ) )
		{
			m_bLButtonDown = false;
		}
	}

	CHeaderCtrl::OnMouseMove(nFlags, point);
}



void CHeaderCtrlEx::Release( void )
{
	m_bImageUse = false;
	m_clrColorKey = 0;
	m_ImageCount = 0;
	m_ImgWidth = 0;
	m_ImgHeight = 0;
	m_bLButtonDown = false;

	if( m_pImagePoint != NULL )
	{
		delete m_pImagePoint;
		m_pImagePoint = NULL;
	}

	m_Bitmap.DeleteObject();

	return;
}

//	헤더에 이미지를 사용할경우 
void CHeaderCtrlEx::Init( BOOL bImageUse, int iImageCnt, COLORREF clrColorKey, POINT *pImagePoint )
{
	if( !bImageUse )		return;
	else if( pImagePoint == NULL )
	{
		MessageBox( "pImageRect가 세팅이 안됐어여.. iImageCnt 만큼 세팅하세요", "Info", MB_OK | MB_ICONINFORMATION );
		return;
	}

	Release();

	m_bImageUse = bImageUse;
	m_clrColorKey = clrColorKey;
	m_ImageCount = iImageCnt;

	if( m_pImagePoint != NULL )
	{
		delete m_pImagePoint;
		m_pImagePoint = NULL;
	}
	m_pImagePoint = new POINT[ m_ImageCount ];
	memset( m_pImagePoint, 0x00, sizeof( POINT ) * m_ImageCount );
	memcpy( m_pImagePoint, pImagePoint, sizeof( POINT ) * m_ImageCount );

	return;
}

BOOL CHeaderCtrlEx::LoadBitmapFile( UINT nIDResource )
{
	if( !m_bImageUse )
	{
		MessageBox( "Init() 로 초기화를 먼저 하세요", "Info", MB_OK | MB_ICONINFORMATION );
		return false;
	}

	m_Bitmap.DeleteObject();

	BOOL bRe = m_Bitmap.LoadBitmap( nIDResource );

	if( bRe )
	{
		BITMAP bm;
		::GetObject(m_Bitmap.m_hObject, sizeof(bm), &bm); // BMP의 가로 세로 사이즈를 얻는데만 씀
		m_ImgWidth = bm.bmWidth;
		m_ImgHeight = bm.bmHeight;
	}

	return bRe;
}

BOOL CHeaderCtrlEx::LoadBitmapFile(LPCTSTR szFilename) 
{ 
	if( !m_bImageUse )
	{
		MessageBox( "Init() 로 초기화를 먼저 하세요", "Info", MB_OK | MB_ICONINFORMATION );
		return false;
	}

	if(szFilename==NULL) return FALSE;
	if(strlen(szFilename)==0) return FALSE;

	m_Bitmap.DeleteObject();

	HBITMAP hBitmap = NULL; 
	hBitmap = (HBITMAP)LoadImage(NULL, szFilename, IMAGE_BITMAP, 0, 0, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 

	BOOL bRe = m_Bitmap.Attach( hBitmap );

	if( bRe )
	{
		BITMAP bm;
		::GetObject(m_Bitmap.m_hObject, sizeof(bm), &bm); // BMP의 가로 세로 사이즈를 얻는데만 씀
		m_ImgWidth = bm.bmWidth;
		m_ImgHeight = bm.bmHeight;
	}

	return bRe;
}

BOOL CHeaderCtrlEx::TransDraw(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc, COLORREF colorTransparent)
{
	if( !m_bImageUse )
	{
		MessageBox( "Init() 로 초기화를 먼저 하세요", "Info", MB_OK | MB_ICONINFORMATION );
		return false;
	}

	if(!m_Bitmap.m_hObject)
	{
		MessageBox( "m_Bitmap 초기화를 먼저 하세요", "Info", MB_OK | MB_ICONINFORMATION );
		return FALSE;
	}

	if(nWidth == 0 || nHeight == 0) {
		// 비트맵 사이즈를 얻어옴
		if(m_Bitmap.m_hObject) {
			BITMAP bm;
			::GetObject(m_Bitmap.m_hObject, sizeof(bm), &bm); // BMP의 가로 세로 사이즈를 얻는데만 씀
			nWidth = bm.bmWidth;
			nHeight = bm.bmHeight;
		}
	}

	CDC dc, memDC, maskDC, tempDC;
	dc.Attach( hdcDest );
	maskDC.CreateCompatibleDC(&dc);
	CBitmap maskBitmap;
	
	// 이전 오브젝트 저장
	CBitmap* pOldMemBmp = NULL;
	CBitmap* pOldMaskBmp = NULL;
	HBITMAP hOldTempBmp = NULL;
	
	memDC.CreateCompatibleDC(&dc);
	tempDC.CreateCompatibleDC(&dc);
	CBitmap bmpImage;
	bmpImage.CreateCompatibleBitmap( &dc, nWidth, nHeight );
	pOldMemBmp = memDC.SelectObject( &bmpImage );
	
	hOldTempBmp = (HBITMAP) ::SelectObject( tempDC.m_hDC, m_Bitmap.m_hObject );
	
	memDC.BitBlt( 0,0,nWidth, nHeight, &tempDC, nXSrc, nYSrc, SRCCOPY );
	
	// 마스크를 위한 흑백 비트맵 생성
	maskBitmap.CreateBitmap( nWidth, nHeight, 1, 1, NULL );
	pOldMaskBmp = maskDC.SelectObject( &maskBitmap );
	memDC.SetBkColor( colorTransparent );
	
	// 메모리 DC로부터 마스크 생성
	maskDC.BitBlt( 0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY );
	
	// memDC의 백그라운드를 검은색으로 세팅한다.
	// 검은색으로 SRCPAINT를 사용하여 다른색은 나타나도록 만들어 검은색이 투명색이된다.
	memDC.SetBkColor(RGB(0,0,0));
	memDC.SetTextColor(RGB(255,255,255));
	memDC.BitBlt(0, 0, nWidth, nHeight, &maskDC, 0, 0, SRCAND);
	
	// 포그라운드를 검은색으로 세팅한다
	dc.SetBkColor(RGB(255,255,255));
	dc.SetTextColor(RGB(0,0,0));
	dc.BitBlt(nXDest, nYDest, nWidth, nHeight, &maskDC, 0, 0, SRCAND);
	
	// 포그라운드로 백그라운드를 융합
	dc.BitBlt(nXDest, nYDest, nWidth, nHeight, &memDC, 0, 0, SRCPAINT);

	if (hOldTempBmp) ::SelectObject( tempDC.m_hDC, hOldTempBmp);
	if (pOldMaskBmp) maskDC.SelectObject( pOldMaskBmp );
	if (pOldMemBmp) memDC.SelectObject( pOldMemBmp );

	// 사용이 끝난 DC 및 비트맵 해제
	maskDC.DeleteDC();
	memDC.DeleteDC();
	bmpImage.DeleteObject();
	
	dc.Detach();

	return TRUE;
}

//===============================================================================================

















//===============================================================================================//
/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

IMPLEMENT_DYNCREATE(CListCtrlEx, CListCtrl)

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	//}}AFX_MSG_MAP
	ON_MESSAGE(LVM_SETTEXTCOLOR, OnSetTextColor)
	ON_MESSAGE(LVM_SETTEXTBKCOLOR, OnSetTextBkColor)
	ON_MESSAGE(LVM_SETBKCOLOR, OnSetBkColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx construction/destruction

CListCtrlEx::CListCtrlEx()
{
	m_bFullRowSel = FALSE;
	m_bClientWidthSel = TRUE;

	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrTextBk = ::GetSysColor(COLOR_WINDOW);
	m_clrBkgnd = ::GetSysColor(COLOR_WINDOW);
	m_clrBackOdd = m_clrBkgnd;
	m_clrBackEven = m_clrBkgnd;

	m_bHeadColumnColorUse = false;
	m_bSortUse = false;
	m_bSortType = true;
	m_bDrawFocusRect = TRUE;

	m_colorTextHighlight = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_colorBackHighlight = ::GetSysColor(COLOR_HIGHLIGHT);

	Init();
}

CListCtrlEx::~CListCtrlEx()
{
	m_ItemInfo.RemoveAll();
}

// Make sure the control is owner drawn
BOOL CListCtrlEx::PreCreateWindow(CREATESTRUCT& cs)
{
	// default is report view and full row selection
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT | LVS_OWNERDRAWFIXED;
	m_bFullRowSel = TRUE;

	return(CListCtrl::PreCreateWindow(cs));
}

BOOL CListCtrlEx::SetFullRowSel(BOOL bFullRowSel)
{
	// no painting during change
	LockWindowUpdate();

	m_bFullRowSel = bFullRowSel;

	BOOL bRet;

	if (m_bFullRowSel)
		bRet = ModifyStyle(0L, LVS_OWNERDRAWFIXED);
	else
		bRet = ModifyStyle(LVS_OWNERDRAWFIXED, 0L);

	// repaint window if we are not changing view type
	if (bRet && (GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
		Invalidate();

	// repaint changes
	UnlockWindowUpdate();

	return(bRet);
}

BOOL CListCtrlEx::GetFullRowSel()
{
	return(m_bFullRowSel);
}

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx drawing

/*
* DrawItem() is called by the framework whenever an item needs to be drawn
* for owner drawn controls.
* Note:
* <UL>
*   <LI>LVS_SHOWSELALWAYS: non owner drawn controls show an item is
*     highlighted when the control does not have focus with a different
*     highlight color is (usually gray). This is not supported for
*     this control.
* </UL>
*/

void CListCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int iSavedDC = pDC->SaveDC();             // Save DC state
        
	int iItem = lpDrawItemStruct->itemID;

	// Get item image and state info
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = iItem;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);

	bool bHighlight = (
		(lvi.state & LVIS_DROPHILITED) ||
		((lvi.state & LVIS_SELECTED) && ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS)))
		);

	// Get rectangles for drawing
	CRect rcBounds;
	CRect rcLabel;
	CRect rcIcon;
	GetItemRect(iItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(iItem, rcLabel, LVIR_LABEL);
	GetItemRect(iItem, rcIcon, LVIR_ICON);
	CRect rcItem(rcBounds);

	CString sLabel = GetItemText(iItem, 0);

	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character
	int offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2;

	// rcBounds.left = rcLabel.left;


	CRect rcWnd;
	GetClientRect(&rcWnd);
	if(m_bClientWidthSel && rcBounds.right<rcWnd.right)
		rcBounds.right = rcWnd.right;

	COLORREF crTextColor = m_colorTextHighlight;

	// Draw the background
    pDC->SetBkMode(TRANSPARENT);
	if(bHighlight)
	{
		pDC->SetBkColor( m_colorBackHighlight );
        //pDC->SetTextColor( m_colorTextHighlight );
		crTextColor = m_colorTextHighlight;
		pDC->FillRect( rcBounds, &CBrush( m_colorBackHighlight ) );
	}
	else
	{
		//	배경 그리기
		if( m_UserMode == 3 ) {
			//	방에 들어갈수 있는 줄의 배경
			if( lvi.iImage < 2 )	pDC->FillRect(rcBounds, &CBrush( m_clrBackOdd ) );
			else					pDC->FillRect(rcBounds, &CBrush( m_clrBackEven ) );
		}
		else if( m_UserMode == 4 ) {
			pDC->FillRect(rcBounds, &CBrush( iItem % 2 ? m_clrBackOdd : m_clrBackEven ) );
		}
		else {
			pDC->FillRect(rcBounds, &CBrush( m_ItemInfo[iItem].BackColor ) );
		}


		if( m_UserMode == 0 ) {
			crTextColor = m_ItemInfo[iItem].TextColor;
		}
		//	남녀 색깔
		else if( m_UserMode == 1 ) {
			crTextColor = m_ItemInfo[iItem].TextColor;
		}
		//	캐릭터 단계별 색깔
		else if( m_UserMode == 2 ) {
			crTextColor = m_ItemInfo[iItem].TextColor;
		}
		//	배경칼라 2개와 글씨칼라 2개 사용
		else if( m_UserMode == 3 ) {
			//	공개 대기중과, 공개 진행방
			if( lvi.iImage < 2 )	crTextColor = m_crWoman;
			//	공개 풀, 비공개 대기, 비공개 진행, 비공개 풀 일경우
			else					crTextColor = m_crMan;
		}
		//	배경칼라 2개와 글씨칼라 2개 사용
		else if( m_UserMode == 4 ) {	
			//	칸건너뛰어서 같은색으로
			if( iItem%2 )	crTextColor = m_crWoman;
			else		crTextColor = m_crMan;
		}
	}

	pDC->SetTextColor( crTextColor );

	// Set clip region
	rcItem.right = rcItem.left + GetColumnWidth(0);

	// Draw state icon
	if(lvi.state & LVIS_STATEIMAGEMASK)
	{
		int nImage = ((lvi.state & LVIS_STATEIMAGEMASK)>>12) - 1;
		CImageList* pImageList = GetImageList(LVSIL_STATE);
		if(pImageList)
		{
			pImageList->Draw(pDC, nImage,
				CPoint(rcItem.left, rcItem.top), ILD_TRANSPARENT);
		}
	}

	// Draw normal and overlay icon
	CImageList* pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
	{
		UINT nOvlImageMask = lvi.state & LVIS_OVERLAYMASK;
		pImageList->Draw(pDC, lvi.iImage, 
			CPoint(rcIcon.left, rcIcon.top),
			(bHighlight?ILD_BLEND50:0) | ILD_TRANSPARENT | nOvlImageMask );
	}

	// Draw item label - Column 0
	rcLabel.left += offset/2-1;
	rcLabel.right -= offset;

	if(!m_ItemInfo[iItem].bDrawShadow) {
		pDC->DrawText(sLabel,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	}
	else {
		pDC->SetTextColor( m_ItemInfo[iItem].ShadowColor );

		rcLabel.OffsetRect(1,1);
		pDC->DrawText(sLabel, -1, rcLabel, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);

		pDC->SetTextColor( crTextColor );

		rcLabel.OffsetRect(-1,-1);
		pDC->DrawText(sLabel, -1, rcLabel, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	}

	// Draw labels for remaining columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	for(int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++)
	{
		rcItem.left = rcItem.right;
		rcItem.right += lvc.cx;

		sLabel = GetItemText(iItem, nColumn);

		// Get the text justification
		UINT nJustify = DT_LEFT;
		switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
		{
		case LVCFMT_RIGHT:
			nJustify = DT_RIGHT;
			break;
		case LVCFMT_CENTER:
			nJustify = DT_CENTER;
			break;
		default:
			break;
		}

		rcLabel = rcItem;
		rcLabel.left += offset;
		rcLabel.right -= offset;

		if(!m_ItemInfo[iItem].bDrawShadow) {
			pDC->DrawText(sLabel, -1, rcLabel,
				nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
		}
		else {
			pDC->SetTextColor( m_ItemInfo[iItem].ShadowColor );

			rcLabel.OffsetRect(1,1);
			pDC->DrawText(sLabel, -1, rcLabel,
				nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);

			pDC->SetTextColor( crTextColor );

			rcLabel.OffsetRect(-1,-1);
			pDC->DrawText(sLabel, -1, rcLabel,
				nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
		}
	}

	if(m_bDrawFocusRect) {
		// draw focus rectangle if item has focus
		if ((lvi.state & LVIS_FOCUSED) && (GetFocus() == this))
			pDC->DrawFocusRect(rcBounds);
	}

	pDC->RestoreDC(iSavedDC);                 // Restore DC.
}

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx diagnostics

#ifdef _DEBUG

void CListCtrlEx::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);

	dc << "m_bFullRowSel = " << m_bFullRowSel;
	dc << "\n";
}

#endif //_DEBUG


/**
* @param iRow    [in] row of cell
* @param iColunm [in] column of cell
* @return Rectangle corresponding to the given cell.
*/

CRect CListCtrlEx::GetCellRect(int iRow, int iColumn)const
{
	// Make sure that the ListView is in LVS_REPORT
	if((GetStyle() & LVS_TYPEMASK) != LVS_REPORT)
		return CRect(0,0,0,0);

	// Get the number of columns
	{
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int iColumnCount = pHeader->GetItemCount();
		assert(iColumn < iColumnCount);
	}

	CRect rect;
	GetItemRect(iRow, &rect, LVIR_BOUNDS);
	// Now find the column
	for(int colnum = 0; colnum < iColumn; colnum++)
	{
		rect.left += GetTrueColumnWidth(colnum);
	}

	// Found the column
	rect.right = rect.left + GetTrueColumnWidth(iColumn);

	RECT rectClient;
	GetClientRect(&rectClient);
	if(rect.right > rectClient.right)
		rect.right = rectClient.right;

	return rect;
}

/**
* @author Mark Findlay
*/

CString CListCtrlEx::GetTrueItemText(int row, int col)const
{
	// Get the header control 
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	_ASSERTE(pHeader);
	
	// get the current number of columns 
	int nCount = pHeader->GetItemCount();
	
	// find the actual column requested. We will compare
	// against hi.iOrder
	for (int x=0; x< nCount; x++)
	{
		HD_ITEM hi = {0};
		hi.mask = HDI_ORDER;
		
		BOOL bRet = pHeader->GetItem(x,&hi);
		_ASSERTE(bRet);
		if (hi.iOrder == col)
		{
			// Found it, get the associated text
			return GetItemText(row,x);
		}
	}
	
	_ASSERTE(FALSE);
	return "We better never fall through to here!";
}

/**
* @author Mark Findlay
*/

int CListCtrlEx::GetTrueColumnWidth(int nCurrentPosition)const
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	_ASSERTE(pHeader);
	
	int nCount = pHeader->GetItemCount();
	
	for (int x=0; x< nCount; x++)
	{
		HD_ITEM hi = {0};
		hi.mask = HDI_WIDTH | HDI_ORDER;
		
		BOOL bRet = pHeader->GetItem(x,&hi);
		_ASSERTE(bRet);
		if (hi.iOrder == nCurrentPosition)
			return hi.cxy;
	}
	
	_ASSERTE(FALSE);
	return 0; // We would never fall through to here!
}

void CListCtrlEx::HideTitleTip()
{
	m_titletip.ShowWindow(SW_HIDE);
}

/**
* @param point   [in]  point in client coordinates
* @param iRow    [out] row containing the point
* @param iColunm [out] column containing the point
*
* @author Matthew Bells
*/

bool CListCtrlEx::HitTestRowCol(CPoint& point, int& iRow, int& iColumn)const
{
	// Make sure that the ListView is in LVS_REPORT
	if((GetStyle() & LVS_TYPEMASK) != LVS_REPORT)
		return false;

	int iPosX = point.x;
	iRow = HitTest(point);

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int iColumnCount = pHeader->GetItemCount();

	for(iColumn = 0; iColumn < iColumnCount; ++iColumn)
	{
		iPosX -= GetTrueColumnWidth(iColumn);
		if(iPosX < 0)
			break;
	}
	if(iColumn == iColumnCount)
		iColumn = -1;

	return (iRow != -1 && iColumn != -1);
}

void CListCtrlEx::RepaintSelectedItems()
{
	CRect rcItem;
	CRect rcLabel;

	// Invalidate focused item so it can repaint

	int iItem = GetNextItem(-1, LVNI_FOCUSED);

	if(iItem != -1)
	{
		GetItemRect(iItem, rcItem, LVIR_BOUNDS);
		GetItemRect(iItem, rcLabel, LVIR_LABEL);
		rcItem.left = rcLabel.left;

		InvalidateRect(rcItem, FALSE);
	}

	// Invalidate selected items depending on LVS_SHOWSELALWAYS

	if(!(GetStyle() & LVS_SHOWSELALWAYS))
	{
		for(iItem = GetNextItem(-1, LVNI_SELECTED);
			iItem != -1; iItem = GetNextItem(iItem, LVNI_SELECTED))
		{
			GetItemRect(iItem, rcItem, LVIR_BOUNDS);
			GetItemRect(iItem, rcLabel, LVIR_LABEL);
			rcItem.left = rcLabel.left;

			InvalidateRect(rcItem, FALSE);
		}
	}

	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

void CListCtrlEx::OnDestroy() 
{
	m_titletip.DestroyWindow();

	if( m_HeaderCtrl.m_hWnd != NULL )	m_HeaderCtrl.UnsubclassWindow();

	CListCtrl::OnDestroy();
}

void CListCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	inherited::OnChar(nChar, nRepCnt, nFlags);
	HideTitleTip();
	SendSelChangedNotification();
}

void CListCtrlEx::OnKillFocus(CWnd* pNewWnd) 
{
	CListCtrl::OnKillFocus(pNewWnd);

	// This should be hidden no matter if another control is getting focus
	// or the edit box.
	HideTitleTip();

	// this really still has focus if one of its chilren (ie. the edit box)
	// has focus
	if(pNewWnd != NULL && pNewWnd->GetParent() == this)
		return;

	// repaint items that should change appearance
	if(m_bFullRowSel && (GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
		RepaintSelectedItems();
}

void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	int iTest = GetKeyState(VK_LMENU);
	// Shortcut to editing.
	if((GetKeyState(VK_LMENU) & 0x8000) || (GetKeyState(VK_RMENU) & 0x8000))
	{
		int iRow;
		int iColumn;
		if(HitTestRowCol(point, iRow, iColumn))
		{
			SetFocus();
			PostMessage(LVM_EDITLABEL, (WPARAM)iRow, 0);
		}
	}
	else
	{
		inherited::OnLButtonDown(nFlags, point);
		ShowTitleTip(point);                    // Make sure TitleTip changes if needed.
		SendSelChangedNotification();
	}
}

void CListCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	if( nFlags == 0 )
	{
		ShowTitleTip(point);                    // Make sure TitleTip changes if needed.
	}
	
	CListCtrl::OnMouseMove(nFlags, point);
}

/*
* When the regular list view control repaints an item, it repaints only the
* area occupied by defined columns. If the last column does not extend to the
* end of the client area, then the space to the right of the last column is
* not repainted. If we are highlighting the full row then this area also needs
* to be invalidated so that the code in DrawItem() can add or remove the
* highlighting from this area.
*/

void CListCtrlEx::OnPaint() 
{
	// in full row select mode, we need to extend the clipping region
	// so we can paint a selection all the way to the right
    // 아이템이 없는 경우 표시
    if(GetItemCount() < 1)
    {
	    Default();
		return;
    }


	if( m_bClientWidthSel &&
		(GetStyle() & LVS_TYPEMASK) == LVS_REPORT && GetFullRowSel())
	{
		CRect rcAllLabels;
		GetItemRect(0, rcAllLabels, LVIR_BOUNDS);

		CRect rcClient;
		GetClientRect(&rcClient);

		if(rcAllLabels.right < rcClient.right)
		{
			// need to call BeginPaint (in CPaintDC c-tor)
			// to get correct clipping rect
			CPaintDC dc(this);

			CRect rcClip;
			dc.GetClipBox(rcClip);

			rcClip.left = min(rcAllLabels.right-1, rcClip.left);
			rcClip.right = rcClient.right;

			InvalidateRect(rcClip, FALSE);
			// EndPaint will be called in CPaintDC d-tor
		}
	}

	CListCtrl::OnPaint();
}

/*
* This is another step to mimic the default behaviour of the list view
* control. When the control loses focus, the focus rectangle around the
* selected (focus) item has to be removed. When the control gets back
* focus, then the focus rectangle has to be redrawn. Both these handlers
* call the RepaintSelectedItems() helper function. 
*/

void CListCtrlEx::OnSetFocus(CWnd* pOldWnd) 
{
	CListCtrl::OnSetFocus(pOldWnd);

	// check if we are getting focus from label edit box
//	if(pOldWnd!=NULL && pOldWnd->GetParent()==this)
//		return;

	// repaint items that should change appearance
	if(m_bFullRowSel && (GetStyle() & LVS_TYPEMASK)==LVS_REPORT)
		RepaintSelectedItems();
}

LRESULT CListCtrlEx::OnSetBkColor(WPARAM wParam, LPARAM lParam)
{
	m_clrBkgnd = (COLORREF)lParam;

	return(Default());
}

LRESULT CListCtrlEx::OnSetTextBkColor(WPARAM wParam, LPARAM lParam)
{
	m_clrTextBk = (COLORREF)lParam;

	return(Default());
}

LRESULT CListCtrlEx::OnSetTextColor(WPARAM wParam, LPARAM lParam)
{
	m_clrText = (COLORREF)lParam;

	return(Default());
}

void CListCtrlEx::PreSubclassWindow() 
{
//	m_HeaderCtrl.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() );
//	EnableToolTips( TRUE );

	CListCtrl::PreSubclassWindow();

	m_titletip.Create(this);
	m_titletip.SetBackground(CBrush(GetBkColor()));
}

void CListCtrlEx::SendSelChangedNotification()
{
	NMHDR nmh;
	nmh.hwndFrom = *this;
	nmh.idFrom = GetDlgCtrlID();
	nmh.code = LVNU_SELCHANGED;
	GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmh);
}

void CListCtrlEx::ShowTitleTip(CPoint point)
{
	int iRow;
	int iCol;

	if(HitTestRowCol(point, iRow, iCol))
	{
		CRect cellrect = GetCellRect(iRow, iCol);
		// offset is equal to TextExtent of 2 space characters.
		// Make sure you have the right font selected into the
		// device context before calling GetTextExtent.
		// You can save this value as a member variable.
		// offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2;
		int offset = 6;
		/*if(iCol == 0)
		{
			CRect rcLabel;
			GetItemRect(iRow, &rcLabel, LVIR_LABEL );
			offset = rcLabel.left - cellrect.left + offset / 2 - 1;
		}*/

		if(iCol == 0)					// TBD: test this with IE4
			cellrect.left -= 2;	// Does it also move the first column???

		cellrect.top--;

		if(GetItemState(iRow, LVIS_SELECTED))
		{
			m_titletip.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			m_titletip.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			m_titletip.SetBkColor(m_clrTextBk);
			m_titletip.SetTextColor(m_clrText);
		}

		m_titletip.Show(cellrect, GetTrueItemText(iRow, iCol), offset-1);
	}
}

void CListCtrlEx::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

/*
	//	선택바를 클라이언트 영역을 모두 사용하면, 맨밑에 아이템 삭제했을때
	//	전체 영역에서 사용하지 않는 영역이 지워지지 않는것을 지운다
	if( m_bClientWidthSel )
	{
		CRect rcBounds, rcReDraw;
		GetItemRect( pNMListView->iItem, rcBounds, LVIR_BOUNDS );

		//	지운곳의 영역만 다시 그린다
		GetClientRect( &rcReDraw );
		rcReDraw.top = rcBounds.top;
		rcReDraw.bottom = rcBounds.bottom;

		InvalidateRect( rcReDraw, false );
	}
*/
	Invalidate( false );

	*pResult = 0;
}


//	단계별로 칼라 적용
void CListCtrlEx::Init( BOOL bSortUse, BOOL bHeadColumnColorUse, int UserMode, COLORREF crMan, COLORREF crWoman, COLORREF *pColor )
{
	if( GetSafeHwnd() == NULL )		return;

	m_bHeadColumnColorUse = bHeadColumnColorUse;
	m_bSortUse = bSortUse;
	m_UserMode = UserMode;


	m_crMan = crMan;
	m_crWoman = crWoman;

	if( UserMode == 0 )
	{
		SetBkColor( crMan );
		SetTextBkColor( crMan );
		SetTextColor( crWoman );
	}
	else if( UserMode == 2 )
	{
		if( pColor == NULL )
		{
			MessageBox( "pColor 가 NULL 입니다. pColor 값을 설정하시구 사용하세요", "Error", MB_OK | MB_ICONINFORMATION );
			return;
		}


		SetBackColors( crMan, crWoman );
	}



	if( m_bHeadColumnColorUse )
	{
		if( m_HeaderCtrl.m_hWnd != NULL )
		{
			m_HeaderCtrl.UnsubclassWindow();
		}

		m_HeaderCtrl.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() );
		EnableToolTips( TRUE );
	}

	m_ItemInfo.RemoveAll();

	return;
}


void CListCtrlEx::SetUserStep( int Num, char State )
{
	if( m_UserMode == 0 )		return;

	//m_UserNum_3000[ Num ] = State;

	return;
}

void CListCtrlEx::SortByColumn(int nColumn, BOOL bAscent)
{
	if(!GetSafeHwnd()) return;

	// 소트 방식 결정
	if(bAscent) m_bSortType = TRUE;
	else m_bSortType = FALSE;

	int count = GetItemCount();
	if(count == 0) return;

	int* buffer = new int[count];
	CArray<ITEMINFO, ITEMINFO> oldItemInfo;

	oldItemInfo.SetSize(count);

	int i;
	// item data를 임시 보관
	for( i = 0; i< count; i++ ) {
		buffer[i] = GetItemData(i);
		oldItemInfo[i] = m_ItemInfo[i];
	}

	// sort를 위해 item data를 세팅
	for( i = 0; i< count; i++ ) {
		SetItemData( i, i );
	}

	LISTCTRL_COLUMN ltex;
	memset( &ltex, 0x00, sizeof( LISTCTRL_COLUMN ) );
	ltex.pListCtrl = this;
	ltex.iSubItem = nColumn;

	SortItems( CompareFunc, (LPARAM)&ltex );

	// sort가 끝난후 원래의 item data를 복원
	for( i = 0; i< count; i++ ) {
		int item = GetItemData(i);
		SetItemData( i, buffer[item] );
		m_ItemInfo[i] = oldItemInfo[item];
	}

	delete buffer;

	return;
}

int CALLBACK CListCtrlEx::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	LISTCTRL_COLUMN *ltex = ( LISTCTRL_COLUMN * )lParamSort;

	if( ltex->iSubItem == 0 )
	{
		int img_1 = 0, img_2 = 0;
		LV_ITEM lvi;
		lvi.mask = LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = lParam1;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;		// get all state flags
		ltex->pListCtrl->GetItem( &lvi );
		img_1 = lvi.iImage;

		lvi.iItem = lParam2;
		ltex->pListCtrl->GetItem( &lvi );
		img_2 = lvi.iImage;

		if( ltex->pListCtrl->m_bSortType )
		{
			if( img_1 > img_2 )		return 1;
			else		return -1;
		}
		else
		{
			if( img_1 < img_2 )		return 1;
			else		return -1;
		}
	}
	else
	{
		CString str1,str2;
		str1 = ltex->pListCtrl->GetItemText( lParam1, ltex->iSubItem );
		str2 = ltex->pListCtrl->GetItemText( lParam2, ltex->iSubItem );
		if( ltex->pListCtrl->m_bSortType )		return strcmp( str1, str2 );
		else									return strcmp( str2, str1 );
	}
}


void CListCtrlEx::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if( !m_bSortUse ) return;

	//static int count = 0;
	//int lastc = count;
	//count = GetItemCount();
	//if(count == 0) return;
	//if( lastc == count )	m_bSortType = !m_bSortType;

	m_bSortType = !m_bSortType;

	SortByColumn(pNMListView->iSubItem, m_bSortType);

	*pResult = 0;
}

void CListCtrlEx::SetBackColors( COLORREF colorOdd, COLORREF colorEven )
{
	m_clrBackOdd = colorOdd;
	m_clrBackEven = colorEven;

	//	X
	SetBkColor( colorOdd );
	SetTextBkColor( colorEven );
}

void CListCtrlEx::SetHeaderColors( COLORREF colorBack, COLORREF colorText )
{
	m_HeaderCtrl.SetColors( colorBack, colorText );
	return;
}

void CListCtrlEx::SetHighlightColors( COLORREF colorBack, COLORREF colorText)
{
    m_colorTextHighlight = colorText;
    m_colorBackHighlight = colorBack;
}

void CListCtrlEx::SetScrollBarChange( BOOL bExChange )
{
	if( bExChange )		SetExtendedStyle( GetExtendedStyle() | LVS_EX_FLATSB );
	else				SetExtendedStyle( GetExtendedStyle() & ~LVS_EX_FLATSB );
	return;
}

void CListCtrlEx::SetGridLineUse( BOOL bGridLineUse )
{
	if( bGridLineUse )		SetExtendedStyle( GetExtendedStyle() | LVS_EX_GRIDLINES );
	else					SetExtendedStyle( GetExtendedStyle() & ~LVS_EX_GRIDLINES );
	return;
}

void CListCtrlEx::SetDrawFocusRect( BOOL bUse )
{
	m_bDrawFocusRect = bUse;
	return;
}

///////////////////////////////////////

BOOL CListCtrlEx::SetItemTextColor(int nItem, COLORREF color)
{
	if(nItem<0 || nItem>=m_ItemInfo.GetSize()) 
		return FALSE;

	m_ItemInfo[nItem].TextColor = color;

	return TRUE;
}

BOOL CListCtrlEx::SetItemTextBkColor(int nItem, COLORREF color)
{
	if(nItem<0 || nItem>=m_ItemInfo.GetSize()) 
		return FALSE;

	m_ItemInfo[nItem].BackColor = color;

	return TRUE;
}

BOOL CListCtrlEx::SetItemShadow(int nItem, BOOL bUse)
{
	if(nItem<0 || nItem>=m_ItemInfo.GetSize()) 
		return FALSE;

	m_ItemInfo[nItem].bDrawShadow = bUse;

	return TRUE;
}

BOOL CListCtrlEx::SetItemShadowColor(int nItem, COLORREF color)
{
	if(nItem<0 || nItem>=m_ItemInfo.GetSize()) 
		return FALSE;

	m_ItemInfo[nItem].ShadowColor = color;

	return TRUE;
}

void CListCtrlEx::OnInsertItem( int nItem )
{
	// 메모리를 미리 확보
	if(m_ItemInfo.GetSize() < nItem+1) {
		int nowsize = m_ItemInfo.GetSize();
		m_ItemInfo.SetSize(max(1, nowsize) * 2);
	}

	ITEMINFO info = {0,};
	info.TextColor = m_clrText;
	info.BackColor = m_clrTextBk;
	info.bDrawShadow = FALSE;
	info.ShadowColor = RGB(100, 100, 100);
	
	m_ItemInfo.InsertAt(nItem, info);
}

void CListCtrlEx::OnDeleteItem( int nItem )
{
	m_ItemInfo.RemoveAt(nItem);
}

void CListCtrlEx::OnDeleteAllItems()
{
	m_ItemInfo.RemoveAll();
}

int CListCtrlEx::InsertItem( const LVITEM* pItem )
{
	int index = CListCtrl::InsertItem( pItem );
	if(index >= 0) OnInsertItem( index );
	return index;
}

int CListCtrlEx::InsertItem( int nItem, LPCTSTR lpszItem )
{
	int index = CListCtrl::InsertItem( nItem, lpszItem );
	if(index >= 0) OnInsertItem( index );
	return index;
}

int CListCtrlEx::InsertItem( int nItem, LPCTSTR lpszItem, int nImage )
{
	int index = CListCtrl::InsertItem( nItem, lpszItem, nImage );
	if(index >= 0) OnInsertItem( index );
	return index;
}

int CListCtrlEx::InsertItem( UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam )
{
	int index = CListCtrl::InsertItem( nMask, nItem, lpszItem, nState, nStateMask, nImage, lParam );
	if(index >= 0) OnInsertItem( index );
	return index;
}

BOOL CListCtrlEx::DeleteItem( int nItem )
{
	BOOL bRtn = CListCtrl::DeleteItem( nItem );
	if(bRtn) OnDeleteItem( nItem );
	return bRtn;
}

BOOL CListCtrlEx::DeleteAllItems()
{
	BOOL bRtn = CListCtrl::DeleteAllItems();
	if(bRtn) OnDeleteAllItems();
	return bRtn;
}
