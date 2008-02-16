// ChatBox.h: interface for the CChatBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATBOX_H__A28A8505_2070_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_CHATBOX_H__A28A8505_2070_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"
#include "ChatViewEdit.h"
#include "SendEdit.h"
#include "GraphButton.h"
#include "xSprite.h"


#define IDM_CHATWND_CLOSE				200			//	채팅윈도 닫기버튼
#define IDM_CHATWND_MINI				201			//	채팅윈도 최소화 버튼

#define IDM_CHATWND_SCROLLUP			202			//	채팅윈도 스크롤 업 버튼
#define IDM_CHATWND_SCROLLDOWN			203			//	채팅윈도 스크롤 다운 버튼
#define IDM_CHATWND_SCROLLTHUMB			204			//	채팅윈도 텀버 버튼


//	스타일 플래그
#define CHATBOX_FLAG_FRAME		((DWORD)1)		// 0000000000000001(b)	- 테두리
#define CHATBOX_FLAG_BACKGROUND	((DWORD)2)		// 0000000000000010(b)	- 백그라인드
#define CHATBOX_FLAG_TITLE		((DWORD)4)		// 0000000000000100(b)	- 타이틀
#define CHATBOX_FLAG_VSCROLL	((DWORD)8)		// 0000000000001000(b)	- 수직 스크롤바
/*
#define CHATBOX_FLAG_VSCROLL	((DWORD)16)		// 0000000000010000(b)
#define CHATBOX_FLAG_GRAYSCL	((DWORD)32)		// 0000000000100000(b)
#define CHATBOX_FLAG_MONO		((DWORD)64)		// 0000000001000000(b)
#define CHATBOX_FLAG_ANTI		((DWORD)128)	// 0000000010000000(b)
#define CHATBOX_FLAG_LE			((DWORD)256)	// 0000000100000000(b)
#define CHATBOX_FLAG_DE			((DWORD)512)	// 0000001000000000(b)
#define CHATBOX_FLAG_RXY		((DWORD)1024)	// 0000010000000000(b)
#define CHATBOX_FLAG_CENTER		((DWORD)2048)	// 0000100000000000(b)
#define CHATBOX_FLAG_PERCENT	((DWORD)4096)	// 0001000000000000(b)
#define CHATBOX_FLAG_MIRROR		((DWORD)8192)	// 0010000000000000(b)
#define CHATBOX_FLAG_TEMPCLIP	((DWORD)16384)	// 0100000000000000(b)
*/

//	채팅 에디트박스
class CChatBox
{
public:
	void sendedit_draw(CDC *pDC);
	CWnd			*m_pParent;
	CPage			*pPage;

	
	xSprite			*m_pBackSpr;

	CSendEdit SendEdit;
	CString m_MsgTitle;
	// 족보등급 보이기
	CGraphButton	BadUserBtn;
	CGraphButton    TabViewChatBtn;
	CGraphButton    TabViewRuleBtn;

	int m_xp[2],		m_yp[2];
	int m_width[2],		m_height[2];
	int m_clickx[2],	m_clicky[2];
	int m_orgwidth[2],	m_orgheight[2];
	
	void DrawRule(CDC *pDC);
	void DrawChat(CDC *pDC);
	void chat_changeinitpos();

//	=========== 퀘스트 설명글 관련 ================
	CChatViewEdit	ChatViewEdit;
	void			ResetChatViewEditPos( void );						//	위치 다시 설정한다
	BOOL			OnMouseWheel( WPARAM wParam, LPARAM lParam );
//	===============================================

	CGraphButton	m_CloseBtn;				//	닫기 버튼
	CGraphButton	m_MiniBtn;				//	최소화 버튼

	CGraphButton	m_ScrollUpBtn;			//	스크롤 업 버튼
	CGraphButton	m_ScrollDownBtn;		//	스크롤 다운 버튼
	CGraphButton	m_ScrollThumbBtn;		//	텀버 버튼
	CRect			m_ScrollBaseRect;		//	스크롤바 영역
	int				m_NowIndex;				//	현재 인덱스
	CPoint			m_LButtonDownThumbPos;
	void			ResetScrollBar( void );	//	스크롤바 리셋하기
	void			CheckScrollBar( void );	//	스크롤바 체크하기
	void			SetScrollPos( int nStartChar, int nEndChar );	//	스크롤바의 위치 셋팅한다


//	=========== 스타일 ================================================
	DWORD			m_Style;			//	스타일
	DWORD			GetStyle( void ) { return m_Style; }
	DWORD			SetStyle( DWORD Style );
	DWORD			AddStyle( DWORD Style );		//	스타일 추가하기
	DWORD			DelStyle( DWORD Style );		//	스타일 빼기
//	===================================================================

	BOOL			m_bShow;
	BOOL			m_bShowCaret;
	int				m_Xp, m_Yp;
	int				m_Width, m_Height;

	bool			m_bMusicRoom;		//	음악방송방인지

	// 이동 관련 변수
	BOOL			m_bMove;
	int				m_ClickX, m_ClickY;

	// 박스 사이즈 바꾸기 관련 변수
	BOOL			m_bSize;
	int				m_OrgWidth, m_OrgHeight;


	void			Show( bool bShow = true );
	BOOL			IsWindowVisible( void ) { return m_bShow; }

	void			SetMusicRoomIcon( bool bMusicRoom = true ) { m_bMusicRoom = bMusicRoom; }	//	음악방인지 아닌지

//	=====================================================================
	COLORREF		m_clrTitelColor;
	CString			m_strTitle;
	void			SetTitle( CString &strTitle, COLORREF clrColor = RGB( 182, 255, 118 ) ) { SetTitle( ( char * )strTitle.operator LPCTSTR(), clrColor ); }
	void			SetTitle( char *pTitle, COLORREF clrColor = RGB( 182, 255, 118 ) ) { m_strTitle = pTitle; m_clrTitelColor = clrColor; }
	void			DrawTitle( CDC *pDC );
//	=====================================================================

//	=====================================================================
	bool			m_bOnlyNoMove;		//	움직이지 않게 한다
	bool			m_bOnlyNoSize;		//	사이즈 조정이 안된다
	bool			m_bWidthSize;		//	가로사이즈 조정
	bool			m_bHeightSize;		//	세로 사이즈 조정가능
	bool			m_bSizeTop;			//	위로 사이즈 조정

	void			SetWndMoveState( bool bMoveState = true );
	void			SetWndSizeState( bool bSizeState = true );
//	=====================================================================

	void			AddText( char *pStr, COLORREF crNewColor = RGB( 254, 255, 170 ), bool bScroll = true );
	void			AddText(CString *str, COLORREF crNewColor = RGB( 254, 255, 170 ), bool bScroll=TRUE);
	void			SetTextFormat(int kind);

	void			SetWindowText( LPCTSTR lpszString );
	void			GetWindowText( CString &strString );

	int				GetLineCount( void );

	void			UpdateButtonState( int nCmd );

	void			MoveWindowPlusPos( CPoint *lpPt );
	void			MoveWindow( int x, int y, int width, int height );
	void			MoveWindow( CRect *lpRect );
	void			MoveWindow( CPoint *lpPt );

	BOOL			PtInRect( POINT point );


	void			Release( void );
	void			ResetChat( void );							//	채팅내용만 초기화


	void			DrawScrollBarUse( CDC *pDC );				//	스크롤바 사용할경우

	int				GetTotalLine( void ) { return ChatViewEdit.GetLineCount(); }
	int				GetOnePageWriteLine( void );				//	한페이지당 라인수

	CChatBox();
	virtual ~CChatBox();

	void Init( CWnd *pWnd, CPage *ppage, xSprite *pSpr,/* POINT StartPoint, SIZE Size,*/ bool bMusicRoom = false, DWORD dwExStyle = 0, DWORD dwBasicStyle = CHATBOX_FLAG_FRAME | CHATBOX_FLAG_BACKGROUND | CHATBOX_FLAG_TITLE );
	BOOL OnLButtonDown( int x, int y);
	BOOL OnLButtonUp( int x, int y );
	BOOL OnMouseMove( int x, int y );
	void Draw( CDC *pDC );

	void OnTimer();
};

#endif // !defined(AFX_CHATBOX_H__A28A8505_2070_11D4_97A5_0050BF0FBE67__INCLUDED_)
