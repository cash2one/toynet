// LobbyListWnd.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "LobbyListWnd.h"
#include "Global.h"
#include "RoomPassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define IDM_UP			100
#define IDM_DOWN		101
#define IDM_THUMB		102

#define IDM_ROOMNUM			WM_USER+103			
#define IDM_ROOMMASTER		WM_USER+104
#define IDM_ROOMMTITLE		WM_USER+105
#define IDM_ROOMMKIND		WM_USER+106
#define IDM_ROOMCOUNT		WM_USER+107
#define IDM_ROOMSTATE		WM_USER+108



/////////////////////////////////////////////////////////////////////////////
// CLobbyListWnd

IMPLEMENT_DYNCREATE(CLobbyListWnd, CWnd)

CLobbyListWnd::CLobbyListWnd()
{
	m_hTimer = 0;
	m_UseMode = 0;
	m_pBackSpr = NULL;
	m_pButtonSpr = NULL;

	m_NowIndex = 0;
	m_MouseOverIndex = -1;	//	오버된 인덱스
	m_SelectIndex =-1;		//	선택된 인덱스

	m_TimeCnt = 0;
	bRoomNumSort = true;

	ZeroMemory( m_pRoomInfoList, sizeof( m_pRoomInfoList ) );
}

CLobbyListWnd::~CLobbyListWnd()
{
}


BEGIN_MESSAGE_MAP(CLobbyListWnd, CWnd)
	//{{AFX_MSG_MAP(CLobbyListWnd)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDC_MENU_ADM_INROOM, OnMenuAdmInroom)   // 방
	ON_COMMAND(IDC_MENU_ADM_MAKEROOM, OnMenuAdmMakeroom)
	ON_COMMAND(IDC_MENU_ADM_RENAMEROOM, OnMenuAdmRenameroom)
	ON_COMMAND(IDC_MENU_ADM_ROOMDEL, OnMenuAdmRoomdel)
	ON_COMMAND(IDC_MENU_ADM_ROOMINFO, OnMenuAdmRoominfo)
	ON_COMMAND(IDC_MENU_ADM_ROOMMASTER, OnMenuAdmRoommaster)
	ON_COMMAND(IDC_BUTTON_ROOMINFO, OnButtonRoominfo)
	ON_COMMAND(IDC_BUTTON_ENTERGAME, OnButtonEntergame)
	ON_COMMAND(IDC_BUTTON_CREATEROOM, OnButtonCreateroom)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLobbyListWnd drawing
void CLobbyListWnd::Create( CWnd* pParentWnd, const RECT& rect, UINT nID, int UseMode )
{
	CWnd::Create( NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID );
	ShowWindow(SW_SHOW);

	//	 0 - 룸리스트, 1 - 유저리스트
	m_UseMode = UseMode;
	if( m_UseMode < 0 )			m_UseMode = 0;
	else if( m_UseMode > 1 )	m_UseMode = 1;

	m_Page.Init( rect.right - rect.left, rect.bottom - rect.top, 16 );

	// 팝업 메뉴 초기화
	g_pLobyDlg->m_RoomMenu.LoadMenu(IDR_ROOMMENU);
	m_RoomMenuV.Attach(g_pLobyDlg->m_RoomMenu.GetSafeHmenu());

	if( m_hTimer != 0 )		KillTimer( m_hTimer );
	m_hTimer = SetTimer( nID, 50, NULL );
}

void CLobbyListWnd::Init( xSprite *pBackSpr, xSprite *pButtonSpr )
{

	m_pBackSpr = pBackSpr;
	m_pButtonSpr = pButtonSpr;

	m_NowIndex = 0;
	m_MouseOverIndex = -1;	//	오버된 인덱스
	m_SelectIndex = -1;		//	선택된 인덱스

	if( m_UseMode == 0 )
	{
		m_UpBtn.Init( this, &m_Page, 511, RLIST_SY, m_pButtonSpr, 0, IDM_UP,3 );
		m_DownBtn.Init( this, &m_Page, 511, 219+RLIST_SY, m_pButtonSpr, 1, IDM_DOWN ,3);
		m_ThumbBtn.Init( this, &m_Page, 511, 17+RLIST_SY, m_pButtonSpr, 2, IDM_THUMB ,3);

		m_RoomNunBtn.Init( this, &m_Page, 0, 0, m_pButtonSpr, 3, IDM_ROOMNUM,3 );
		m_RoomMasterBtn.Init( this, &m_Page, 41, 0, m_pButtonSpr, 4, IDM_ROOMMASTER,3 );
		m_RoomTitleBtn.Init( this, &m_Page, 129, 0, m_pButtonSpr, 5, IDM_ROOMMTITLE,3 );
		m_RoomKindBtn.Init( this, &m_Page,  340, 0, m_pButtonSpr, 6, IDM_ROOMMKIND,3 );
		m_RoomStateBtn.Init( this, &m_Page, 395, 0, m_pButtonSpr, 7, IDM_ROOMSTATE,3 );// 최소설정 머니로 변경 사용한다.
		m_RoomCountBtn.Init( this, &m_Page, 471, 0, m_pButtonSpr, 8, IDM_ROOMCOUNT,3 );
		


		m_ScrollBaseRect.SetRect( 0, 0, 16, 201 );	//	스크롤바 영역
		m_ScrollBaseRect.OffsetRect( 511, 17+RLIST_SY );

		//	체크할 위치 설정
		int y = 0;
		for( int i = 0; i < MAX_ROOMCHECK; i++ )
		{
			m_RoomCheckRect_Max[ i ].SetRect( 0, 0, 510, 19 );
			y = i * 19-i+RLIST_SY;
			m_RoomCheckRect_Max[ i ].OffsetRect( 0, y );
		}
	}
	m_UpBtn.Show( false );
	m_DownBtn.Show( false );
	m_ThumbBtn.Show( false );



	//	정렬하기 - 스클로바용으로
	ResetControlls();

	//	리스트 초기화
	DelAllList();
	return;
}


void CLobbyListWnd::OnDestroy() 
{
	if( m_hTimer != 0 )		KillTimer( m_hTimer );

	m_RoomMenuV.Detach();
	//	리시트 지우기
	DelAllList();
	CWnd::OnDestroy();
}


LRESULT CLobbyListWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch( message )
	{
		case WM_COMMAND:
		{
			switch( wParam )
			{
				case IDM_UP:
				{
					m_NowIndex--;
					if( m_NowIndex < 0 )		m_NowIndex = 0;
					m_MouseOverIndex = -1;	//	오버된 인덱스

					//	스클로바 텀브바 설정
					ResetControlls();

					return 1;
				}
				case IDM_DOWN:
				{
					//int totcnt = m_List.GetCount();
					int totcnt = m_deque_room.size();
					if( m_UseMode == 0 )
					{
						if( totcnt <= MAX_ROOMCHECK )		return 1;
						m_NowIndex++;
						if( m_NowIndex >= totcnt - MAX_ROOMCHECK )		m_NowIndex = totcnt - MAX_ROOMCHECK;
					}
					
					if( m_NowIndex < 0 )		m_NowIndex = 0;
					m_MouseOverIndex = -1;	//	오버된 인덱스

					//	스클로바 텀브바 설정
					ResetControlls();

					return 1;
				}
				case IDM_THUMB:
				{
					return 1;
				}

				case IDM_ROOMNUM:
					{
						bRoomNumSort = !bRoomNumSort;
						sort(m_deque_room.begin(), m_deque_room.end(), RoomInfornumSort(bRoomNumSort) );
						RenewalShortList();
						return 1;
					}
				case IDM_ROOMMASTER:
					{
						bRoomNumSort = !bRoomNumSort;
						sort(m_deque_room.begin(), m_deque_room.end(), RoomInformasterSort(bRoomNumSort) );
						RenewalShortList();
						return 1;
					}
				case IDM_ROOMMTITLE:
					{
						bRoomNumSort = !bRoomNumSort;
						sort(m_deque_room.begin(), m_deque_room.end(), RoomInfortitleSort(bRoomNumSort) );
						RenewalShortList();
						return 1;
					}
				case IDM_ROOMMKIND:
					{
						bRoomNumSort = !bRoomNumSort;
						sort(m_deque_room.begin(), m_deque_room.end(), RoomInforkindSort(bRoomNumSort) );
						RenewalShortList();
						return 1;
					}
				case IDM_ROOMCOUNT:
					{
						bRoomNumSort = !bRoomNumSort;
						sort(m_deque_room.begin(), m_deque_room.end(), RoomInforcountSort(bRoomNumSort) );
						RenewalShortList();
						return 1;
					}
				case IDM_ROOMSTATE: // 최소금액 설정
					{	
						bRoomNumSort = !bRoomNumSort;
						sort(m_deque_room.begin(), m_deque_room.end(), RoomInforstateSort(bRoomNumSort) );
						RenewalShortList();
						return 1;
					}
				
			}

			break;
		}

		case WM_SETCURSOR:			return 1;
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:			return 1;
								
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CLobbyListWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bOver = false;
	if( m_UpBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_DownBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_ThumbBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	SetCursor( g_pGameView->hCurHand );
	if( bOver )		return;


	SetCursor( g_pGameView->hCurSel );

	if( m_UseMode == 0 )
	{
		for( int i = 0; i < MAX_ROOMCHECK; i++ )
		{
			if( !m_RoomCheckRect_Max[ i ].PtInRect( point ) )		continue;
			int index = m_NowIndex + i;
			if( m_pRoomInfoList[ index ] == NULL )		return;
			if( m_pRoomInfoList[ index ]->RoomNum < 0 )		return;

						
			m_SelectIndex = m_pRoomInfoList[ index ]->RoomNum;
			
			//방입장
			g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_ENTERGAME, 0);
			return;
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CLobbyListWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	SetCapture();

	m_LButtonDownThumbPos = point;
	SetCursor( g_pGameView->hCurHand );

	if( m_RoomNunBtn.OnLButtonDown( point.x, point.y ) )			return;
	if( m_RoomMasterBtn.OnLButtonDown( point.x, point.y ) )			return;
	if( m_RoomTitleBtn.OnLButtonDown( point.x, point.y ) )			return;
	if( m_RoomKindBtn.OnLButtonDown( point.x, point.y ) )			return;
	if( m_RoomCountBtn.OnLButtonDown( point.x, point.y ) )			return;
	if( m_RoomStateBtn.OnLButtonDown( point.x, point.y ) )			return;
	if( m_UpBtn.OnLButtonDown( point.x, point.y ) )			return;
	if( m_DownBtn.OnLButtonDown( point.x, point.y ) )		return;
	if( m_ThumbBtn.OnLButtonDown( point.x, point.y ) ) {
		m_LButtonDownThumbPos.x = point.x - m_ThumbBtn.m_Xp;
		m_LButtonDownThumbPos.y = point.y - m_ThumbBtn.m_Yp;
		return;
	}

	m_LButtonDownThumbPos = 0;
	SetCursor( g_pGameView->hCurSel );
	
	for( int i = 0; i < MAX_ROOMCHECK; i++ ) {
		if( !m_RoomCheckRect_Max[ i ].PtInRect( point ) )		continue;
		int index = m_NowIndex + i;
		if( m_pRoomInfoList[ index ] == NULL || m_pRoomInfoList[ index ]->RoomNum <= 0 ) {
			m_SelectIndex = -1;
			return;
		}
		
		if( m_SelectIndex == m_pRoomInfoList[ index ]->RoomNum )		m_SelectIndex = -1;
		else	m_SelectIndex = m_pRoomInfoList[ index ]->RoomNum;
		return;
	}
	
	//	스롤바에서 페이지 다운이나 업일때
	if( m_ScrollBaseRect.PtInRect( point ) ) {
		if( point.y < m_ThumbBtn.m_Yp ) {
			m_NowIndex -= MAX_ROOMCHECK;
		}
		else {
			int totcnt = m_deque_room.size();
			if( totcnt <= MAX_ROOMCHECK )		return;
			m_NowIndex += MAX_ROOMCHECK;
			if( m_NowIndex >= totcnt - MAX_ROOMCHECK )		m_NowIndex = totcnt - MAX_ROOMCHECK;
		}

		if( m_NowIndex < 0 )		m_NowIndex = 0;
		m_MouseOverIndex = -1;	//	오버된 인덱스
	}
	//	스클로바 위치조정
	ResetControlls();
	CWnd::OnLButtonDown(nFlags, point);
}

void CLobbyListWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();

	if( m_RoomNunBtn.OnLButtonUp( point.x, point.y ) )		return;
	if( m_RoomMasterBtn.OnLButtonUp( point.x, point.y ) )		return;
	if( m_RoomTitleBtn.OnLButtonUp( point.x, point.y ) )		return;
	if( m_RoomKindBtn.OnLButtonUp( point.x, point.y ) )		return;
	if( m_RoomCountBtn.OnLButtonUp( point.x, point.y ) )		return;
	if( m_RoomStateBtn.OnLButtonUp( point.x, point.y ) )		return;

	if( m_UpBtn.OnLButtonUp( point.x, point.y ) )		return;
	if( m_DownBtn.OnLButtonUp( point.x, point.y ) )		return;
	if( m_ThumbBtn.OnLButtonUp( point.x, point.y ) )	return;

	SetCursor( g_pGameView->hCurSel );
	CWnd::OnLButtonUp(nFlags, point);
}

void CLobbyListWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd *pWnd = GetFocus();
	//SetFocus();
	if( m_ThumbBtn.m_bClick )
	{
		//int totcnt = m_List.GetCount();
		int totcnt = m_deque_room.size();
		int index = 0;
		
		if( totcnt <= MAX_ROOMCHECK )	return;
		
		m_ThumbBtn.m_Yp = point.y - m_LButtonDownThumbPos.y;
		if( m_ThumbBtn.m_Yp < m_ScrollBaseRect.top )		m_ThumbBtn.m_Yp = m_ScrollBaseRect.top;
		else if( m_ThumbBtn.m_Yp > m_ScrollBaseRect.bottom - m_ThumbBtn.m_Height )
			m_ThumbBtn.m_Yp = m_ScrollBaseRect.bottom - m_ThumbBtn.m_Height;
		
		//	인덱스 구해오기
		index = MAX_ROOMCHECK;
		
		
		float ftt = ( float )( m_ThumbBtn.m_Yp - m_ScrollBaseRect.top )/ ( m_ScrollBaseRect.Height() - m_ThumbBtn.m_Height );
		int per = ( int )( ftt * 100 );
		m_NowIndex = ( ( totcnt - index ) * per ) / 100;
		
		return;
	}
	
	BOOL bOver = false;

	if(	m_RoomNunBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_RoomMasterBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_RoomTitleBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_RoomKindBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_RoomCountBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_RoomStateBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	

	if( m_UpBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_DownBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_ThumbBtn.OnMouseMove( point.x, point.y ) )	bOver = true;

	if( bOver )		return;

	m_MouseOverIndex = -1;
	
	for( int i = 0; i < MAX_ROOMCHECK; i++ )
	{
		if( !m_RoomCheckRect_Max[ i ].PtInRect( point ) )		continue;
		m_MouseOverIndex = i;
		break;
	}

	if( m_MouseOverIndex < 0 && pWnd != NULL )	pWnd->SetFocus();
	SetCursor( g_pGameView->hCurSel );
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CLobbyListWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	int nRowsScrolled = zDelta / 120;
	int cnt = 1;
	if( m_UseMode == 1 )		cnt = 4;
	for( int i = 0; i < cnt; i++ )
	{
		if( nRowsScrolled > 0 )	PostMessage( WM_COMMAND, IDM_UP, 0 );
		else		PostMessage( WM_COMMAND, IDM_DOWN, 0 );
	}
	SetCursor( g_pGameView->hCurSel );
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

//	메뉴 띄우기
void CLobbyListWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor( g_pGameView->hCurSel );
	//	방리스트일경우는 메뉴 안뜨운다
	if( m_UseMode == 0 )
	{
		for( int i = 0; i < MAX_ROOMCHECK; i++ )
		{
			if( !m_RoomCheckRect_Max[ i ].PtInRect( point ) )		continue;
			int index = m_NowIndex + i;
			if( m_pRoomInfoList[ index ] == NULL || m_pRoomInfoList[ index ]->RoomNum <= 0 )		return;
			m_SelectIndex = m_pRoomInfoList[ index ]->RoomNum;
				
			if(m_SelectIndex < 0)
			{
				m_RoomMenuV.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_ROOMINFO, MF_GRAYED);
				m_RoomMenuV.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_ENTERGAME, MF_GRAYED);
			}
			else
			{
				m_RoomMenuV.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_ROOMINFO, MF_ENABLED);
				m_RoomMenuV.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_ENTERGAME, MF_ENABLED);
			}
			
			// 팝업 메뉴
			CPoint pnt;
			GetCursorPos(&pnt);
			
			//### [관리자 모드 작업] ###
			if(g_bAdminUser){
				if(m_SelectIndex < 0){
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMINFO	, MF_GRAYED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_INROOM	, MF_GRAYED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_WARN		, MF_GRAYED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_RENAMEROOM, MF_GRAYED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMDEL	, MF_GRAYED);
					//	m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_MAKEROOM	, MF_GRAYED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMMASTER, MF_GRAYED);
					
				}
				else{
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMINFO	, MF_GRAYED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_INROOM	, MF_GRAYED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_WARN		, MF_GRAYED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_RENAMEROOM, MF_ENABLED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMDEL	, MF_ENABLED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_MAKEROOM	, MF_GRAYED);
					m_RoomMenuV.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMMASTER, MF_GRAYED);
				}
				m_RoomMenuV.GetSubMenu(3)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
			}
			else{
				m_RoomMenuV.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
			}

			break;
		}

		return;
	}

	m_MouseOverIndex = -1;

//	CWnd::OnRButtonDown(nFlags, point);
}

void CLobbyListWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_UpBtn.m_bClick || m_DownBtn.m_bClick )
	{
		m_TimeCnt++;
		if( m_TimeCnt % 4 == 0 )
		{
			if( m_UpBtn.m_bClick )		SendMessage( WM_COMMAND, IDM_UP );
			if( m_DownBtn.m_bClick )	SendMessage( WM_COMMAND, IDM_DOWN );
			m_TimeCnt = 0;
		}
	}

	POINT mpos;
	GetCursorPos(&mpos);
	ScreenToClient(&mpos);

	if( GetFocus() != this || mpos.y<0 || mpos.x<0 || mpos.y>253 || mpos.x> 528)
	{
		m_MouseOverIndex = -1;	//	오버된 인덱스
		m_UpBtn.m_bOver = false;
		m_DownBtn.m_bOver = false;
		m_ThumbBtn.m_bOver = false;
		
		m_RoomNunBtn.m_bOver = false;
		m_RoomMasterBtn.m_bOver = false;
		m_RoomTitleBtn.m_bOver = false;
		m_RoomKindBtn.m_bOver = false;
		m_RoomCountBtn.m_bOver = false;
		m_RoomStateBtn.m_bOver = false;
	}

	Invalidate( false );

	CWnd::OnTimer(nIDEvent);
}

void CLobbyListWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC MemDC;
	MemDC.CreateCompatibleDC( &dc );
	if( MemDC.m_hDC == NULL )		return;
	MemDC.SelectObject( m_Page.hBmp );
	MemDC.SetBkMode( TRANSPARENT );

	Draw( &MemDC );

	CRect rect;
	GetWindowRect( &rect );
	dc.BitBlt( 0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY );

	SetFocus();
}

void CLobbyListWnd::Draw( CDC *pDC )
{
	if( m_pBackSpr == NULL )		return;

	//	룸일때
	DrawRoom( pDC );
	DrawButton( pDC );

	return;
}

//	룸일때
void CLobbyListWnd::DrawRoom( CDC *pDC )
{
	//	배경 그리기
	m_Page.PutSprAuto( 0, 0, m_pBackSpr, 0 );

	pDC->SelectObject( &g_pGameView->Font2);

	int imgnum = 0;

	CString strTemp;
	int index = m_NowIndex;
	CRect rect;
	ROOMINFO *pRi = NULL;
	for( int i = 0; i < MAX_ROOMCHECK; i++ )
	{
		index = m_NowIndex + i;
		if( m_pRoomInfoList[ index ] == NULL )		continue;
		pRi = m_pRoomInfoList[ index ];
		if(pRi == NULL )
		{
			TRACE("pRI == NULL");
			continue;
		}
		pDC->SetTextColor( RGB( 0, 0, 0 ) );

		//	선택된것 그려주기
		if( m_SelectIndex >=0  && m_SelectIndex == m_pRoomInfoList[ index ]->RoomNum )
		{
			m_Page.PutSprAuto( m_RoomCheckRect_Max[ i ].left+1, m_RoomCheckRect_Max[ i ].top, m_pBackSpr, 1 );
			pDC->SetTextColor( RGB( 200, 100, 100 ) );
		}
		else if( m_MouseOverIndex >= 0 && m_MouseOverIndex == i )	//	오버된것 그려주기
		{
			m_Page.PutSprAuto( m_RoomCheckRect_Max[ i ].left+1 , m_RoomCheckRect_Max[ i ].top, m_pBackSpr, 2 );
		}


		//	방정보 이미지
		int image;
		if(pRi->NowUserNum>=pRi->MaxUserNum) image=2;// ### [ 관전기능 ] ### 
		else if(pRi->State == 1) image=1;
		else image=0;
		if(pRi->NowUserNum+pRi->NowObserverNum>=pRi->MaxUserNum+MAX_OBSERVER-1) image=3;
		if(pRi->Secret==1) image += 4;
		
		m_Page.PutSprAuto( m_RoomCheckRect_Max[ i ].left+3, m_RoomCheckRect_Max[ i ].top+2, &LobyRoomImgSpr, image );

		
		//방번호
		rect.SetRect( 0, 0, 20, 19 );
		rect.OffsetRect( m_RoomCheckRect_Max[ i ].left + 2 + 15, m_RoomCheckRect_Max[ i ].top + 2 );
		strTemp.Format( "%d", pRi->RoomNum );
		if(pRi->bSuperMaster) pDC->SetTextColor( RGB( 255, 0, 0 ) );
		pDC->DrawText( strTemp, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
		

		//방장 
		rect.SetRect( 0, 0, 86, 19 );
		rect.OffsetRect( 41, m_RoomCheckRect_Max[ i ].top+1 );
		pDC->DrawText( pRi->ID, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );

		//	방제목 쓰기
		rect.SetRect( 0, 0, 209, 19 );
		rect.OffsetRect( 131, m_RoomCheckRect_Max[ i ].top +1 );
		pDC->DrawText( pRi->Title, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
	
		// 게임 종류
		if(pRi->FormKind == 0) {
			//	strTemp.Format( "%s / 5인", g_StrMan._Get(_T("GAMETYPE_NORMAL")));
				strTemp.Format( "Normal");					
		}
		else
		{
				strTemp.Format( "Choice");	
				pDC->SetTextColor( RGB( 0, 0, 255 ) );
		}
		rect.SetRect( 0, 0, 55, 19 );
		rect.OffsetRect( 341, m_RoomCheckRect_Max[ i ].top+1 );
		pDC->DrawText( strTemp, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
		
		if(pRi->bSuperMaster) pDC->SetTextColor( RGB( 255, 0, 0 ) );
		else pDC->SetTextColor( RGB( 0, 0, 0 ) );

		// 최소금액 설정
		if(pRi->CreateRGrade > 0)
		{	
			INT64 Money = GetLevelToUserMoney( pRi->UsernIcon );
			Money = Money * GetCreateRoomGrade(pRi->CreateRGrade) / 10; // 방만들기시 최소머니 설정등급
			strTemp = NumberToOrientalString(Money);
			strTemp +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
		}
		else 
			strTemp.Format("-");

		rect.SetRect( 0, 0, 76, 19 );
		rect.OffsetRect( 395, m_RoomCheckRect_Max[ i ].top +1);
		pDC->DrawText( strTemp, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );

		//	인원수
		rect.SetRect( 0, 0, 40, 19 );
		rect.OffsetRect( 471, m_RoomCheckRect_Max[ i ].top+1 );
		strTemp.Format( "%d/%d", pRi->NowUserNum, pRi->MaxUserNum );
		pDC->DrawText( strTemp, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
		

		// 상태
	//	strTemp.Format("%s", (pRi->State==0)?g_StrMan._Get(_T("L_WAIT")):g_StrMan._Get(_T("L_START")));
	//	rect.SetRect( 0, 0, 45, 19 );
	//	rect.OffsetRect( 465, m_RoomCheckRect_Max[ i ].top +1);
	//	pDC->DrawText( strTemp, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );

		// 베팅룰
	//	switch(pRi->GameKind){
	//	case 0:  strTemp.Format( "%s", g_StrMan._Get(_T("QUATER")));break;
	//	case 1:  strTemp.Format( "%s", g_StrMan._Get(_T("HALF")));break;
	//	default: strTemp.Format( "%s", g_StrMan._Get(_T("QUATER"))); break;
	//	}
	//	rect.SetRect( 0, 0, 40, 19 );
	//	rect.OffsetRect( 468, m_RoomCheckRect_Max[ i ].top+1 );
	//	pDC->DrawText( strTemp, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
	}

	return;
}


void CLobbyListWnd::DrawButton( CDC *pDC )
{
	m_UpBtn.Draw(pDC);
	m_DownBtn.Draw(pDC);
	m_ThumbBtn.Draw(pDC);

	m_RoomNunBtn.Draw(pDC);
	m_RoomMasterBtn.Draw(pDC);
	m_RoomTitleBtn.Draw(pDC);
	m_RoomKindBtn.Draw(pDC);
	m_RoomCountBtn.Draw(pDC);
	m_RoomStateBtn.Draw(pDC);

	if( !m_ThumbBtn.m_bShow )
	{
//		m_Page.PutSprAuto( m_ThumbBtn.m_Xp, m_ThumbBtn.m_Yp, m_pButtonSpr, 6 );
	}

	return;
}

//	============= 리스트 관련 ========================//
//	추가하기
bool CLobbyListWnd::InsertList( void *pList )
{


	deque<ROOMINFO>::iterator iter;
	int count = m_deque_room.size();
	for(iter = m_deque_room.begin(); iter != m_deque_room.end(); ++iter) { //중복검사
		if((*iter).RoomNum ==  ( ( ROOMINFO * )pList )->RoomNum )		return false;;
	}
	m_deque_room.push_back( *((ROOMINFO*)pList) );
	//	정보 갱신 정렬하기
	RenewalShortList();
	return true;
}

//	수정하기
bool CLobbyListWnd::SetList( void *pList, BOOL bTitle)
{
	int rnum = 0;

	if(!bTitle)
		rnum =  (( SMALLROOMINFO * )pList )->RoomNum;
	else
		rnum = ( ( CHANGESTYLEROOMINFO * )pList )->RoomNum;
	
	deque<ROOMINFO>::iterator iter;
	int count = m_deque_room.size();
	for(iter = m_deque_room.begin(); iter != m_deque_room.end(); iter++) { //중복검사
		if((*iter).RoomNum !=  rnum ) continue;
		if(!bTitle) {
			SMALLROOMINFO *pSR = (SMALLROOMINFO * )pList;
			(*iter).GameKind   = pSR->GameKind;
			strncpy((*iter).ID, pSR->ID, 15);
			(*iter).NowUserNum = pSR->NowUserNum;
			(*iter).MaxUserNum = pSR->MaxUserNum;
			(*iter).NowObserverNum = pSR->NowObserverNum;
			(*iter).State = pSR->State;
			
		} else {
			CHANGESTYLEROOMINFO *pCR = (CHANGESTYLEROOMINFO * )pList;
			strncpy((*iter).Title, pCR->Title, 32);
		}
		break;
	}
	
	//	정보 갱신 정렬하기
	RenewalShortList();
	return true;
}

//	지우기
void CLobbyListWnd::DelList( int DelIndex, char *pID )
{
	if(m_deque_room.size() == 0) return;

	deque<ROOMINFO>::iterator iter;
	int count = m_deque_room.size();
	for(iter = m_deque_room.begin(); iter != m_deque_room.end(); iter++) { //중복검사
		if((*iter).RoomNum ==  DelIndex ) {
			m_deque_room.erase(iter);
			break;
		}
	}
			
	//	정보 갱신 정렬하기
	RenewalShortList();
	return;
}

//	모두 초기화
void CLobbyListWnd::DelAllList( void )
{
	m_deque_room.clear();
	ZeroMemory( m_pRoomInfoList, sizeof( m_pRoomInfoList ) );		//	방리스트
	m_NowIndex = 0;			//	현재 인덱스
	m_MouseOverIndex = -1;	//	오버된 인덱스
	m_SelectIndex = -1;		//	선택된 인덱스

	m_ThumbBtn.m_Yp = 17 + RLIST_SY;
	return;

}

ROOMINFO *CLobbyListWnd::GetRoomInfo( int RoomNum )
{
	if( RoomNum < 0 )		return NULL;
	
	deque<ROOMINFO>::iterator iter;
	for(iter=m_deque_room.begin(); iter!=m_deque_room.end(); iter++) {
		if( (*iter).RoomNum == RoomNum)	{
			return &(*iter);
		}
	}
	return NULL;
}


//	인덱스 설정
void CLobbyListWnd::SetIndex( int Index )
{
	m_NowIndex = Index;
	ResetControlls();
	return;
}

//	아이디 검색
int	CLobbyListWnd::GetFindUserIndex( char *pID )
{
	if( m_UseMode != 1 )		return -1;

	if( pID == NULL || strlen( pID ) == 0 )		return -1;
/*
	for( int i = 0; i < MAX_USER; i++ )
	{
		if( m_pUserInfoList[ i ] == NULL )		continue;
		if( strcmp( m_pUserInfoList[ i ]->ID, pID ) == 0 )		return i;
	}
*/
	return -1;
}

//	찾기로 해서 존재하면 그유저에게 포커스 가게한다
void CLobbyListWnd::SetFindSelectUser( SMALLUSERINFO *pUi )
{
	if( pUi == NULL )		return;
	int index = GetFindUserIndex( pUi->ID );
	if( index < 0 )		return;

	m_SelectIndex = pUi->UNum;

	m_NowIndex = index;
	//	스클로바 텀브바 설정
	ResetControlls();
	return;
}


//	선택된 인덱스 찾아오기
int	CLobbyListWnd::GetSelectIndex( void )
{
	return m_SelectIndex;
}

//	정보 갱신
void CLobbyListWnd::RenewalShortList( void )
{
	BOOL bScrollBarShow = false;

	if(m_deque_room.size()> MAX_ROOMCHECK) bScrollBarShow = true;
	ZeroMemory( m_pRoomInfoList, sizeof( m_pRoomInfoList ) );

	deque<ROOMINFO>::iterator iter;
	int nIndex = 0;
	for(iter = m_deque_room.begin(); iter!=m_deque_room.end(); ++iter) {
		m_pRoomInfoList[nIndex++] = &(*iter);
	}

	if( m_NowIndex >= m_deque_room.size() - MAX_ROOMCHECK )	m_NowIndex = m_deque_room.size() - MAX_ROOMCHECK;

	if( m_NowIndex < 0 )		m_NowIndex = 0;
	m_UpBtn.Show( bScrollBarShow );
	m_DownBtn.Show( bScrollBarShow );
	m_ThumbBtn.Show( bScrollBarShow );

	if( !bScrollBarShow )
	{
		m_NowIndex = 0;
		m_MouseOverIndex = -1;	//	오버된 인덱스
	}
	return;
}

//	==================================================

//	정렬하기 - 스크롤기능
void CLobbyListWnd::ResetControlls( void )
{
	int totcnt = m_deque_room.size();
	int index = 0;
	index = MAX_ROOMCHECK;

	int tt = totcnt - index;
	if( tt <= 0 )	tt = 1;

	float ftt = ( float )m_NowIndex / tt;
	int per = ( int )( ftt * 100 );
	int bb = m_ScrollBaseRect.Height() - m_ThumbBtn.m_Height;
	int dd = ( bb * per ) / 100;
	m_ThumbBtn.m_Yp = m_ScrollBaseRect.top + dd;

	return;
}




////////////////////////////////////////////////////////////
//					방관련
///////////////////////////////////////////////////////////
void CLobbyListWnd::OnMenuAdmRenameroom() // 방제 변경하기
{
	// TODO: Add your command handler code here
	// 선택된 아이템중 첫번째 아이템을 얻는다

	if(m_SelectIndex<0) return;
	int rnum = GetSelectIndex();
	if(rnum < 0) return;

	STYLEROOMINFO sri; memset(&sri,0,sizeof(sri));

	if(!GetStyleRoomInfo(rnum,sri)) return;

	CAdminTitleDlg dlg(sri.Title);

	if(dlg.DoModal()==IDCANCEL) return;

	// 다시한번 방정보를 확인하자!!
	if(!GetStyleRoomInfo(rnum,sri)) return;

	strcpy(sri.Title,dlg.m_strName.operator LPCTSTR());

	ADMINMESSAGE amsg={0,};
	amsg.AdminUNum = g_MyInfo.UI.UNum;
	strncpy(amsg.AdminID, g_MyInfo.UI.ID, 15);


	CNM_ADMIN_CHANGEROOM croom;
	croom.Set(&amsg, &sri);
	SockMan.SendData(g_MainSrvSID, croom.pData, croom.GetTotalSize());


/*
	POSITION pos = m_RoomList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	nRoomSel = m_RoomList.GetNextSelectedItem(pos);

	if(nRoomSel < 0) return;
	if(nRoomSel >= m_RoomList.GetItemCount()) return;

	CString rstr;
	rstr = m_RoomList.GetItemText(nRoomSel, 0);
	if(rstr.GetLength()==0) return;
	int rnum = ::atoi(rstr.operator LPCTSTR());
	if(rnum < 0) return;

	STYLEROOMINFO sri; memset(&sri,0,sizeof(sri));

	if(!GetStyleRoomInfo(rnum,sri)) return;

	CAdminTitleDlg dlg(sri.Title);

	if(dlg.DoModal()==IDCANCEL) return;

	// 다시한번 방정보를 확인하자!!
	if(!GetStyleRoomInfo(rnum,sri)) return;

	strcpy(sri.Title,dlg.m_strName.operator LPCTSTR());

	ADMINMESSAGE amsg={0,};
	amsg.AdminUNum = g_MyInfo.UI.UNum;
	strncpy(amsg.AdminID, g_MyInfo.UI.ID, 15);


	CNM_ADMIN_CHANGEROOM croom;
	croom.Set(&amsg, &sri);
	SockMan.SendData(g_MainSrvSID, croom.pData, croom.GetTotalSize());
*/

	
}

// 방스타일을 구한다

void CLobbyListWnd::OnMenuAdmRoomdel() // 방제거하기
{
	if(m_SelectIndex<0) return;
	int rnum = GetSelectIndex();
	if(rnum < 0) return;

	
	STYLEROOMINFO sri; memset(&sri,0,sizeof(sri));
	if(!GetStyleRoomInfo(rnum,sri)) return;
	CString strMess; strMess.Format(g_StrMan.Get(_T("ADMIN_DELROOM")), rnum, sri.Title);

	int result=AfxMessageBox(strMess, MB_YESNO);
	if(result==IDNO) return;

	// 다시한번 방정보를 확인하자!!
	if(!GetStyleRoomInfo(rnum,sri)) return;

	ADMINMESSAGE amsg={0,};
	amsg.AdminUNum = g_MyInfo.UI.UNum;
	strncpy(amsg.AdminID, g_MyInfo.UI.ID, 15);

	amsg.TarUNum = sri.RoomNum;			//방번호
	strncpy(amsg.TarID, sri.ID, 15);	// 방장 아이디


	CNM_ADMIN_DELROOM droom;
	droom.Set(&amsg);
	SockMan.SendData(g_MainSrvSID, droom.pData, droom.GetTotalSize());


	/*
	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_RoomList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	nRoomSel = m_RoomList.GetNextSelectedItem(pos);

	if(nRoomSel < 0) return;
	if(nRoomSel >= m_RoomList.GetItemCount()) return;

	CString rstr;
	rstr = m_RoomList.GetItemText(nRoomSel, 0);
	if(rstr.GetLength()==0) return;
	int rnum = ::atoi(rstr.operator LPCTSTR());
	if(rnum < 0) return;

	STYLEROOMINFO sri; memset(&sri,0,sizeof(sri));
	if(!GetStyleRoomInfo(rnum,sri)) return;
	CString strMess; strMess.Format(g_StrMan.Get(_T("ADMIN_DELROOM")), rnum, sri.Title);

	int result=AfxMessageBox(strMess, MB_YESNO);
	if(result==IDNO) return;

	// 다시한번 방정보를 확인하자!!
	if(!GetStyleRoomInfo(rnum,sri)) return;

	ADMINMESSAGE amsg={0,};
	amsg.AdminUNum = g_MyInfo.UI.UNum;
	strncpy(amsg.AdminID, g_MyInfo.UI.ID, 15);

	amsg.TarUNum = sri.RoomNum;			//방번호
	strncpy(amsg.TarID, sri.ID, 15);	// 방장 아이디


	CNM_ADMIN_DELROOM droom;
	droom.Set(&amsg);
	SockMan.SendData(g_MainSrvSID, droom.pData, droom.GetTotalSize());
	*/
}


void CLobbyListWnd::OnMenuAdmRoommaster()  // 방장에게 경고하기
{
	// TODO: Add your command handler code here

	
}


void CLobbyListWnd::OnMenuAdmInroom()  //방입장
{
	g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_ENTERGAME, 0);
	
}
void CLobbyListWnd::OnMenuAdmMakeroom() //방만들기
{
	g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_CREATEROOM, 0);
	
}

void CLobbyListWnd::OnMenuAdmRoominfo() //방정보 보기
{
	g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_ROOMINFO, 0);
}


BOOL CLobbyListWnd::GetStyleRoomInfo(int rnum, STYLEROOMINFO &sri)
{
	if(rnum<1 || rnum>=MAX_ROOM) return FALSE;
	ROOMINFO *pRi = GetRoomInfo( rnum );
	if( pRi == NULL )		return FALSE;

	memset(&sri,0,sizeof(STYLEROOMINFO));
	STYLEROOMINFO tmpri ={ 0,};


	tmpri.RoomNum = pRi->RoomNum;
	strncpy(tmpri.ID, pRi->ID,15);       // 방장아이디
	strncpy(tmpri.Title, pRi->Title, 31); // 방타이틀
	
	
	if(tmpri.RoomNum != rnum || strlen(tmpri.ID)==0) return FALSE; // 방번호가 틀리거나, 방장이 없다면
	memcpy(&sri, &tmpri, sizeof(STYLEROOMINFO));
	return TRUE;
}



void CLobbyListWnd::OnButtonRoominfo() 
{
	// TODO: Add your command handler code here
	g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_ROOMINFO, 0);
	
}

void CLobbyListWnd::OnButtonEntergame() 
{
	// TODO: Add your command handler code here
	g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_ENTERGAME, 0);
	
}

void CLobbyListWnd::OnButtonCreateroom() 
{
	// TODO: Add your command handler code here
	g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_CREATEROOM, 0);
	
}



void CLobbyListWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}




BOOL CLobbyListWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == 'M' || pMsg->wParam == 'm')
		{
			// 서버에 플레이어정보 돈 추가  - jeong
			/*
			Play[0].UI.PMoney += 100;
			Play[0].PrevMoney += 100;
			g_MyInfo.UI.PMoney  =  Play[0].UI.PMoney; 
			*/

			
			CSV_ASK_MONEYINFO aumsg;
			aumsg.Set(Play[0].UI.UNum, 100, 1);
			SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());
		}
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}
