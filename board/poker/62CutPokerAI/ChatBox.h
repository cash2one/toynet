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


#define IDM_CHATWND_CLOSE				200			//	ä������ �ݱ��ư
#define IDM_CHATWND_MINI				201			//	ä������ �ּ�ȭ ��ư

#define IDM_CHATWND_SCROLLUP			202			//	ä������ ��ũ�� �� ��ư
#define IDM_CHATWND_SCROLLDOWN			203			//	ä������ ��ũ�� �ٿ� ��ư
#define IDM_CHATWND_SCROLLTHUMB			204			//	ä������ �ҹ� ��ư


//	��Ÿ�� �÷���
#define CHATBOX_FLAG_FRAME		((DWORD)1)		// 0000000000000001(b)	- �׵θ�
#define CHATBOX_FLAG_BACKGROUND	((DWORD)2)		// 0000000000000010(b)	- ��׶��ε�
#define CHATBOX_FLAG_TITLE		((DWORD)4)		// 0000000000000100(b)	- Ÿ��Ʋ
#define CHATBOX_FLAG_VSCROLL	((DWORD)8)		// 0000000000001000(b)	- ���� ��ũ�ѹ�
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

//	ä�� ����Ʈ�ڽ�
class CChatBox
{
public:
	void sendedit_draw(CDC *pDC);
	CWnd			*m_pParent;
	CPage			*pPage;

	
	xSprite			*m_pBackSpr;

	CSendEdit SendEdit;
	CString m_MsgTitle;
	// ������� ���̱�
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

//	=========== ����Ʈ ����� ���� ================
	CChatViewEdit	ChatViewEdit;
	void			ResetChatViewEditPos( void );						//	��ġ �ٽ� �����Ѵ�
	BOOL			OnMouseWheel( WPARAM wParam, LPARAM lParam );
//	===============================================

	CGraphButton	m_CloseBtn;				//	�ݱ� ��ư
	CGraphButton	m_MiniBtn;				//	�ּ�ȭ ��ư

	CGraphButton	m_ScrollUpBtn;			//	��ũ�� �� ��ư
	CGraphButton	m_ScrollDownBtn;		//	��ũ�� �ٿ� ��ư
	CGraphButton	m_ScrollThumbBtn;		//	�ҹ� ��ư
	CRect			m_ScrollBaseRect;		//	��ũ�ѹ� ����
	int				m_NowIndex;				//	���� �ε���
	CPoint			m_LButtonDownThumbPos;
	void			ResetScrollBar( void );	//	��ũ�ѹ� �����ϱ�
	void			CheckScrollBar( void );	//	��ũ�ѹ� üũ�ϱ�
	void			SetScrollPos( int nStartChar, int nEndChar );	//	��ũ�ѹ��� ��ġ �����Ѵ�


//	=========== ��Ÿ�� ================================================
	DWORD			m_Style;			//	��Ÿ��
	DWORD			GetStyle( void ) { return m_Style; }
	DWORD			SetStyle( DWORD Style );
	DWORD			AddStyle( DWORD Style );		//	��Ÿ�� �߰��ϱ�
	DWORD			DelStyle( DWORD Style );		//	��Ÿ�� ����
//	===================================================================

	BOOL			m_bShow;
	BOOL			m_bShowCaret;
	int				m_Xp, m_Yp;
	int				m_Width, m_Height;

	bool			m_bMusicRoom;		//	���ǹ�۹�����

	// �̵� ���� ����
	BOOL			m_bMove;
	int				m_ClickX, m_ClickY;

	// �ڽ� ������ �ٲٱ� ���� ����
	BOOL			m_bSize;
	int				m_OrgWidth, m_OrgHeight;


	void			Show( bool bShow = true );
	BOOL			IsWindowVisible( void ) { return m_bShow; }

	void			SetMusicRoomIcon( bool bMusicRoom = true ) { m_bMusicRoom = bMusicRoom; }	//	���ǹ����� �ƴ���

//	=====================================================================
	COLORREF		m_clrTitelColor;
	CString			m_strTitle;
	void			SetTitle( CString &strTitle, COLORREF clrColor = RGB( 182, 255, 118 ) ) { SetTitle( ( char * )strTitle.operator LPCTSTR(), clrColor ); }
	void			SetTitle( char *pTitle, COLORREF clrColor = RGB( 182, 255, 118 ) ) { m_strTitle = pTitle; m_clrTitelColor = clrColor; }
	void			DrawTitle( CDC *pDC );
//	=====================================================================

//	=====================================================================
	bool			m_bOnlyNoMove;		//	�������� �ʰ� �Ѵ�
	bool			m_bOnlyNoSize;		//	������ ������ �ȵȴ�
	bool			m_bWidthSize;		//	���λ����� ����
	bool			m_bHeightSize;		//	���� ������ ��������
	bool			m_bSizeTop;			//	���� ������ ����

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
	void			ResetChat( void );							//	ä�ó��븸 �ʱ�ȭ


	void			DrawScrollBarUse( CDC *pDC );				//	��ũ�ѹ� ����Ұ��

	int				GetTotalLine( void ) { return ChatViewEdit.GetLineCount(); }
	int				GetOnePageWriteLine( void );				//	���������� ���μ�

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
