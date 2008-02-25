// ChatBox.cpp: implementation of the CChatBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatBox.h"
#include "Global.h"
#include "CharCutInArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define GAMEVIEW_INTERPACE_HEIGHT   24      // ���ϴ� 
#define TITLE_HEIGHT				20      // Ÿ��Ʋ�ٸ� ����Ѵٸ� �β�


/*
	���콺 �� �߰� 
	GameDlg.cpp
	BOOL CGameDlg::PreTranslateMessage(MSG* pMsg) 
	{
	  // �߰�
	  ///////////////////////////////////////////////////////////////
	  if(pMsg->message == WM_MOUSEWHEEL) // ä��â���� ���콺 ��
	  {
	  if(GetFocus() == &m_GameView.ChatBox.SendEdit) {
	  m_GameView.ChatBox.OnMouseWheel(pMsg->wParam, pMsg->lParam);
	  }
	  } 
	  ///////////////////////////////////////////////////////////////
	 
	 GameView.cpp
	 switch(message)
	 {
	 case WM_MOUSEWHEEL:
		ChatBox.OnMouseWheel(wParam, lParam);
		return 1;

*/


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatBox::CChatBox()
{
	m_pParent = NULL;
	Release();
}

CChatBox::~CChatBox()
{
	Release();
	m_pParent = NULL;
}

void CChatBox::Release( void )
{
	pPage = NULL;
	m_pBackSpr = NULL;

	
	m_bShow = FALSE;
	m_bShowCaret = TRUE;
	m_Xp = m_Yp = 0;
	m_Width = 0;
	m_Height = 0;
	m_Style = 0;
	m_NowIndex = 0;
	m_ScrollBaseRect = 0;

	m_bMove = FALSE;
	m_ClickX = m_ClickY = 0;

	m_bSize = FALSE;
	m_OrgWidth = m_OrgHeight = 0;
	m_bOnlyNoMove = false;		//	�������� �ʰ� �Ѵ�
	m_bOnlyNoSize = false;		//	������ ������ �ȵȴ�
	m_bMusicRoom = false;

	m_bWidthSize = false;		//	���λ����� ����
	m_bHeightSize = false;		//	���� ������ ��������

	m_clrTitelColor = 0;

	if( ChatViewEdit.GetSafeHwnd() != NULL )	ChatViewEdit.SetWindowText( "" );

	return;
}

void CChatBox::Init( CWnd *pWnd, CPage *ppage, xSprite *pSpr,/* POINT StartPoint, SIZE Size,*/ bool bMusicRoom, DWORD dwExStyle, DWORD dwBasicStyle )
{
	Release();

	m_pParent = pWnd;
	pPage = ppage;
	m_pBackSpr = pSpr;
	m_bMusicRoom = bMusicRoom;
	m_bShow = TRUE;
	 
	//////////////////////////////////////////////////
	// �׷��� ��ǥ, ������
	// 5��
	m_xp[0] = 7;		m_yp[0] = 428;
	m_width[0] = 242;	m_height[0] = 145;

	// 6��
	m_xp[1] = 7;		m_yp[1] = 447;
	m_width[1]  = 242;  m_height[1] = 126;
	//////////////////////////////////////////////////

	//	�׵θ� + ��׶��ε�, Ÿ��Ʋ ( �⺻ ��Ÿ�� - m_Style = CHATBOX_FLAG_FRAME | CHATBOX_FLAG_BACKGROUND | CHATBOX_FLAG_TITLE | CHATBOX_FLAG_VSCROLL )
	m_Style = dwBasicStyle | dwExStyle;
	if( m_Yp > GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - m_Height )		m_Yp = GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - m_Height;

	int xpos = 40;
	m_CloseBtn.Init( m_pParent, pPage, m_Xp + m_Width - 18, m_Yp + 3, m_pBackSpr, 0, IDM_CHATWND_CLOSE );
	m_ScrollUpBtn.Init( m_pParent, pPage, m_Xp + m_Width - xpos, m_Yp + 3, m_pBackSpr, 0, IDM_CHATWND_SCROLLUP);
	m_ScrollDownBtn.Init( m_pParent, pPage, m_Xp + m_Width - xpos, m_Yp + 3, m_pBackSpr, 1, IDM_CHATWND_SCROLLDOWN);
	m_ScrollThumbBtn.Init( m_pParent, pPage, m_Xp + m_Width - xpos, m_Yp + 3, m_pBackSpr, 2, IDM_CHATWND_SCROLLTHUMB);

	if( ChatViewEdit.m_hWnd == NULL )
	{
		int y = 3, width = m_Width - 3;
		if( m_Style & CHATBOX_FLAG_TITLE )		y = 20;
		if( m_Style & CHATBOX_FLAG_VSCROLL )	width -= ( m_ScrollUpBtn.m_Width + 4 );

		// ���� ����Ʈ ����
		CRect rc;
		// ä�� �� ��ġ����Ʈ ��Ʈ�� ����
		rc.SetRect( m_Xp + 3, m_Yp + y, m_Xp + width, m_Yp + m_Height - y );
		ChatViewEdit.Create( ES_READONLY | ES_AUTOVSCROLL | ES_WANTRETURN | ES_MULTILINE, rc, m_pParent, 124 );
		ChatViewEdit.SetFont( &g_pGameView->Font2 );
		// ä�� �� ��ġ����Ʈ�� �Ⱥ��̴� ������ �Űܵд�
		ChatViewEdit.GetClientRect( rc );
		rc.OffsetRect( -1000, 0 );
		ChatViewEdit.MoveWindow( rc );
		ChatViewEdit.ShowWindow( SW_HIDE );
		

		rc.SetRect(m_Xp+3, m_Yp+y, m_Xp+width, m_Yp + m_Height -y);
		SendEdit.iUse = 1;
		SendEdit.Create(ES_WANTRETURN|ES_MULTILINE|ES_AUTOHSCROLL, rc, m_pParent, 123);
		SendEdit.SetFont(&g_pGameView->Font2);
		// ���� ����Ʈ�� �Ⱥ��̴� ������ �Űܵд�
		SendEdit.GetClientRect(rc);
		rc.OffsetRect(-1000, 0);
		SendEdit.MoveWindow(rc);
		SendEdit.ShowWindow(SW_HIDE);
	}

	chat_changeinitpos();

	return;
}


void CChatBox::chat_changeinitpos()
{

	if( g_RI.bPlayer == 0  ) {

		m_Xp = m_xp[0];
		m_Yp = m_yp[0];
		m_Width = m_OrgWidth   = m_width[0];
		m_Height = m_OrgHeight = m_height[0];

		// �׷��� ��ư �ʱ�ȭ
		//BadUserBtn.Init(m_pParent, pPage, 214, 570, &BadUserBtnSpr, 0, IDM_BADUSER);
		// ������� ���̱�
		TabViewChatBtn.Init(m_pParent, pPage, 3, 404, &ObserverSpr, 3, IDM_TABVIEWCHAT); //
		TabViewRuleBtn.Init(m_pParent, pPage, 125, 404, &ObserverSpr, 4, IDM_TABVIEWRULE); //

	} else {

		m_Xp = m_xp[1];
		m_Yp = m_yp[1];
		m_Width = m_OrgWidth   = m_width[1];
		m_Height = m_OrgHeight = m_height[1];

		// �׷��� ��ư �ʱ�ȭ
		BadUserBtn.Init(m_pParent, pPage, 214, 570, &BadUserBtnSpr, 0, IDM_BADUSER);
		// ������� ���̱�
		TabViewChatBtn.Init(m_pParent, pPage, 3, 404, &ObserverSpr, 3, IDM_TABVIEWCHAT); //
		TabViewRuleBtn.Init(m_pParent, pPage, 125, 404, &ObserverSpr, 4, IDM_TABVIEWRULE); //
	}

	ResetChat();


	if( m_Yp > GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - m_Height )		m_Yp = GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - m_Height;
	
	int y = 3, width = m_Width - 3;
	if( m_Style & CHATBOX_FLAG_TITLE )		y = 20;
	if( m_Style & CHATBOX_FLAG_VSCROLL )	width -= ( m_ScrollUpBtn.m_Width + 4 );


	CRect rc;

	rc.SetRect(m_Xp+3, m_Yp+y, m_Xp+width-15, m_Yp+y+ 20); // ä�ùڽ� �ٷ� �Ʒ���ġ
	SendEdit.MoveWindow(rc);
	SendEdit.GetClientRect(rc);
	rc.OffsetRect(-1000, 0);
	SendEdit.MoveWindow(rc);

	rc.SetRect( m_Xp + 3, m_Yp + y, m_Xp + width, m_Yp + m_Height - y);
	ChatViewEdit.MoveWindow(rc);
	ChatViewEdit.GetClientRect(rc);
	rc.OffsetRect(-1000, 0);
	ChatViewEdit.MoveWindow(rc);

	//	��ũ�ѹ� �����ϱ�
	ResetScrollBar();
}


//	ä�ó��븸 �ʱ�ȭ
void CChatBox::ResetChat( void )
{
	if( ChatViewEdit.m_hWnd == NULL )		return;

	ChatViewEdit.SetWindowText( "" );
	SetScrollPos( -1, 0 );

	return;
}

//	��ũ�ѹ��� ��ġ �����Ѵ�
void CChatBox::SetScrollPos( int nStartChar, int nEndChar )
{
	if( ChatViewEdit.m_hWnd == NULL )		return;

	ChatViewEdit.SetSel( nStartChar, nEndChar );
	m_NowIndex = nEndChar;
	ResetScrollBar();

	return;
}

//	��ġ����Ʈ ��Ʈ�� ��ġ �ٽ� �����Ѵ�
void CChatBox::ResetChatViewEditPos( void )
{
	if( ChatViewEdit.m_hWnd == NULL )			return;

	int y = 3, width = m_Width - 3;
	if( m_Style & CHATBOX_FLAG_TITLE )		y = 20;
	if( m_Style & CHATBOX_FLAG_VSCROLL )	width -= ( m_ScrollUpBtn.m_Width + 4 );

	// ���� ����Ʈ ����
	CRect rc;
	// ä�� �� ��ġ����Ʈ ��Ʈ�� ����
	rc.SetRect( m_Xp + 3, m_Yp + y, m_Xp + width, m_Yp + m_Height - y );
	// ä�� �� ��ġ����Ʈ�� �Ⱥ��̴� ������ �Űܵд�
	rc.OffsetRect( -1000, 0 );
	ChatViewEdit.MoveWindow( rc );
	ChatViewEdit.ShowWindow( SW_HIDE );

	return;
}

//	��Ÿ�� �����ϱ�
DWORD CChatBox::SetStyle( DWORD Style )
{
	m_Style = Style;

	//	��ũ�ѹ� �����ϱ�
	ResetScrollBar();

	//	��ġ����Ʈ ��Ʈ�� ��ġ �ٽ� �����Ѵ�
	if( m_Style & CHATBOX_FLAG_VSCROLL )	ResetChatViewEditPos();

	return m_Style;
}

//	��Ÿ�� �߰��ϱ�
DWORD CChatBox::AddStyle( DWORD Style )
{
	int style = m_Style;

	style &= ~Style;
	style |= Style;

	m_Style = style;

	//	��ũ�ѹ� �����ϱ�
	ResetScrollBar();

	//	��ġ����Ʈ ��Ʈ�� ��ġ �ٽ� �����Ѵ�
	if( m_Style & CHATBOX_FLAG_VSCROLL )	ResetChatViewEditPos();

	return m_Style;
}

//	��Ÿ�� ����
DWORD CChatBox::DelStyle( DWORD Style )
{
	int style = m_Style;
	style &= ~Style;
	m_Style = style;

	//	��ũ�ѹ� �����ϱ�
	ResetScrollBar();

	//	��ġ����Ʈ ��Ʈ�� ��ġ �ٽ� �����Ѵ�
	if( m_Style & CHATBOX_FLAG_VSCROLL )	ResetChatViewEditPos();

	return m_Style;
}

//	���������� ���μ�
int CChatBox::GetOnePageWriteLine( void )
{
	int wline = ( m_Height - 6 ) / 14;

	//	Ÿ��Ʋ �ٰ� �ִٸ� 1���� �����ش�
	if( m_Style & CHATBOX_FLAG_TITLE )		wline--;
	
	return wline;
}


void CChatBox::Show( bool bShow )
{
//	g_pGameView->m_FastInventoryDlg.m_bChattingBoxView = bShow;

	if( bShow )
	{
		m_CloseBtn.Show( true );
//		m_MiniBtn.Show( true );
		m_bShow = true;
	}
	else
	{
		m_CloseBtn.Show( false );
//		m_MiniBtn.Show( false );
		m_bShow = false;
	}
	return;
}

void CChatBox::UpdateButtonState( int nCmd )
{
	if( nCmd == IDM_CHATWND_CLOSE || nCmd == IDM_CHATWND_MINI )
	{
		Show( false );
	}
	//	ä������ ��ũ�� �� ��ư
	else if( nCmd == IDM_CHATWND_SCROLLUP )
	{
		if( !( m_Style & CHATBOX_FLAG_VSCROLL ) )		return;
	//	Sound.Play(SND00);
		m_NowIndex--;
		if( m_NowIndex < 0 )		m_NowIndex = 0;

		//	��Ŭ�ι� �Һ�� ����
		ResetScrollBar();
	}
	//	ä������ ��ũ�� �ٿ� ��ư
	else if( nCmd == IDM_CHATWND_SCROLLDOWN )
	{
		if( !( m_Style & CHATBOX_FLAG_VSCROLL ) )		return;

	//	Sound.Play(SND00);

		// ä�� ���ڿ� �׸���
		int totline = ChatViewEdit.GetLineCount();
		//	���������� ���μ�
		int wline = GetOnePageWriteLine();
		if( wline <= 0 )		wline = 1;

		if( totline < wline )		return;

		m_NowIndex++;
		if( m_NowIndex >= totline - wline )		m_NowIndex = totline - wline;
		if( m_NowIndex < 0 )		m_NowIndex = 0;

		//	��Ŭ�ι� �Һ�� ����
		ResetScrollBar();
	}
	//	ä������ �ҹ� ��ư
	else if( nCmd == IDM_CHATWND_SCROLLTHUMB )
	{
		if( !( m_Style & CHATBOX_FLAG_VSCROLL ) )		return;
	}

	return;
}

BOOL CChatBox::OnLButtonDown( int x, int y )
{
	if( !IsWindowVisible() )						return false;

	if(BadUserBtn.OnLButtonDown(x, y)==TRUE) return TRUE;
	if(TabViewChatBtn.OnLButtonDown(x, y)==TRUE) return TRUE;
	if(TabViewRuleBtn.OnLButtonDown(x, y)==TRUE) return TRUE;


	if( m_Style & CHATBOX_FLAG_TITLE )
	{
		//	�ݱ� ��ư
		if( m_CloseBtn.OnLButtonDown( x, y ) )
		{
			UpdateButtonState( m_CloseBtn.m_CmndID );
			return true;
		}
		//	�ּ�ȭ ��ư
		if( m_MiniBtn.OnLButtonDown( x, y ) )
		{
			UpdateButtonState( m_CloseBtn.m_CmndID );
			return true;
		}
	}

	if( m_Style & CHATBOX_FLAG_VSCROLL )
	{
		m_LButtonDownThumbPos = CPoint( x, y );

		//	��ũ�� �� ��ư
		if( m_ScrollUpBtn.OnLButtonDown( x, y ) )
		{
			UpdateButtonState( m_ScrollUpBtn.m_CmndID );
			return true;
		}

		//	��ũ�� �ٿ� ��ư
		if( m_ScrollDownBtn.OnLButtonDown( x, y ) )
		{
			UpdateButtonState( m_ScrollDownBtn.m_CmndID );
			return true;
		}
		//	�ҹ� ��ư
		if( m_ScrollThumbBtn.OnLButtonDown( x, y ) )
		{
			m_LButtonDownThumbPos.x = x - m_ScrollThumbBtn.m_Xp;
			m_LButtonDownThumbPos.y = y - m_ScrollThumbBtn.m_Yp;
			return true;
		}

		m_LButtonDownThumbPos = 0;

		CPoint point( x, y );
		//	���ѹٿ��� ������ �ٿ��̳� ���϶�
		if( m_ScrollBaseRect.PtInRect( point ) )
		{
			int totline = ChatViewEdit.GetLineCount();
			//	���������� ���μ�
			int wline = GetOnePageWriteLine();
			if( totline < wline || totline <= 0 || wline <= 0  )				return true;

			if( point.y < m_ScrollThumbBtn.m_Yp )		m_NowIndex -= wline;
			else		m_NowIndex += wline;

			if( m_NowIndex >= totline - wline )		m_NowIndex = totline - wline;
			if( m_NowIndex < 0 )		m_NowIndex = 0;

			//	��ũ�ѹ� �����ϱ�
			ResetScrollBar();

			return true ;
		}
	}


	//	�������� �ʰ�, ������ ������ �ȵǴ°Ÿ� �ƹ��͵� ���Ѵ�
	if(	m_bOnlyNoMove && m_bOnlyNoSize )			return false;

/*
	if( !m_bOnlyNoMove )
	{
		// �̵� �� ���� üũ
		if( x >= m_Xp && x <= m_Xp + m_Width && y >= m_Yp && y <= m_Yp + m_Height )		m_bMove = TRUE;
		else	m_bMove = FALSE;
	}

	if( !m_bOnlyNoSize )
	{
		// ������ ���� ��ư ���� üũ
		if( x >= m_Xp + m_Width - 10 && x <= m_Xp + m_Width && y >= m_Yp + m_Height - 10 && y <= m_Yp + m_Height )
		{
			m_bSize = TRUE;
			m_bMove = FALSE;
		}
		else	m_bSize = FALSE;
	}

	if( m_bMove == FALSE && m_bSize == FALSE )		return FALSE;
	if( m_bMove ) { m_ClickX = x - m_Xp; m_ClickY = y - m_Yp; }
	if( m_bSize )
	{
		m_ClickX = x;
		m_ClickY = y;
		m_OrgWidth = m_Width;
		m_OrgHeight = m_Height;
	}
*/

	m_bWidthSize = false;		//	���λ����� ����
	m_bHeightSize = false;		//	���� ������ ��������
	m_bSizeTop = false;

	if( !m_bOnlyNoMove )
	{
		// �̵� �� ���� üũ
		CRect rect( 0, 0, m_Width, m_Height - 5 );
		rect.OffsetRect( m_Xp, m_Yp + 5 );
		if( rect.PtInRect( CPoint( x, y ) ) )		m_bMove = TRUE;
		else	m_bMove = FALSE;
	}

	if( !m_bOnlyNoSize )
	{
		CRect rect( 0, 0, m_Width, 5 );
		rect.OffsetRect( m_Xp, m_Yp );
		if( rect.PtInRect( CPoint( x, y ) ) )
		{
			m_bSize = TRUE;
			m_bMove = FALSE;

			m_bWidthSize = false;		//	���λ����� ����
			m_bHeightSize = true;		//	���� ������ ��������
			m_bSizeTop = true;
		}
/*
		// ������ ���� ��ư ���� üũ
		else if( x >= m_Xp + m_Width - 10 && x <= m_Xp + m_Width && y >= m_Yp + m_Height - 10 && y <= m_Yp + m_Height )
		{
			m_bSize = TRUE;
			m_bMove = FALSE;

			m_bWidthSize = true;		//	���λ����� ����
			m_bHeightSize = true;		//	���� ������ ��������
		}
*/
		else	m_bSize = FALSE;
	}

	if( m_bMove == FALSE && m_bSize == FALSE )		return FALSE;
	if( m_bMove ) { m_ClickX = x - m_Xp; m_ClickY = y - m_Yp; }
	if( m_bSize )
	{
		m_ClickX = x;
		m_ClickY = y;
		m_OrgWidth = m_Width;
		m_OrgHeight = m_Height;
	}

	return TRUE;
}

BOOL CChatBox::OnLButtonUp( int x, int y )
{
	if( !IsWindowVisible() )						return false;

	BadUserBtn.OnLButtonUp(x, y);
	TabViewChatBtn.OnLButtonUp(x, y);
	TabViewRuleBtn.OnLButtonUp(x, y);


	if( m_Style & CHATBOX_FLAG_TITLE )
	{
		//	�ݱ� ��ư
		if( m_CloseBtn.OnLButtonUp( x, y ) )			return true;
		//	�ּ�ȭ ��ư
		if( m_MiniBtn.OnLButtonUp( x, y ) )				return true;
	}

	if( m_Style & CHATBOX_FLAG_VSCROLL )
	{
		//	��ũ�� �� ��ư
		if( m_ScrollUpBtn.OnLButtonUp( x, y ) )			return true;
		//	��ũ�� �ٿ� ��ư
		if( m_ScrollDownBtn.OnLButtonUp( x, y ) )		return true;
		//	�ҹ� ��ư
		if( m_ScrollThumbBtn.OnLButtonUp( x, y ) )		return true;
	}


	//	�������� �ʰ�, ������ ������ �ȵǴ°Ÿ� �ƹ��͵� ���Ѵ�
	if(	m_bOnlyNoMove && m_bOnlyNoSize )		return false;
	if( ChatViewEdit.m_hWnd == NULL )			return false;

	if( m_bMove || m_bSize )
	{
		CRect rc;
		// ä�� �� ��ġ����Ʈ�� �Ⱥ��̴� ������ �Űܵд�
		ChatViewEdit.GetClientRect( rc );
		rc.OffsetRect( -1000, 0 );
		ChatViewEdit.MoveWindow( rc );
		ChatViewEdit.ShowWindow( SW_HIDE );
		ChatViewEdit.MoveWindow( rc );

		// ���� 2000�� ������ �ؽ�Ʈ�� �ٽ� ����
		//CString str;
		//ChatViewEdit.GetWindowText( str );
		//ChatViewEdit.SetWindowText( str );

		m_OrgWidth = m_Width;
		m_OrgHeight = m_Height;
	}
	
	m_bMove = FALSE;
	m_bSize = FALSE;

	return false;
}

BOOL CChatBox::OnMouseMove( int x, int y )
{
	if( !IsWindowVisible() )				return false;

	bool bOver = false;

	// �Ű��ư üũ
	if(BadUserBtn.OnMouseMove(x, y)==TRUE	   ||
		TabViewChatBtn.OnMouseMove(x, y)==TRUE ||
		TabViewRuleBtn.OnMouseMove(x, y)==TRUE ) 
	{
		SetCursor(g_pGameView->hCurHand);
		return TRUE;
	}

	

	if( m_Style & CHATBOX_FLAG_TITLE )
	{
		//	�ݱ� ��ư
		if( m_CloseBtn.OnMouseMove( x, y ) )				bOver = true;
		//	�ּ�ȭ ��ư
		if( m_MiniBtn.OnMouseMove( x, y ) )					bOver = true;
	}

	if( m_Style & CHATBOX_FLAG_VSCROLL )
	{
		if( m_ScrollThumbBtn.m_bClick )
		{
			int totcnt = ChatViewEdit.GetLineCount();
			//	���������� ���μ�
			int wline = GetOnePageWriteLine();
			if( wline <= 0 )			wline = 1;
			if( totcnt <= wline )		return true;

			m_ScrollThumbBtn.m_Yp = y - m_LButtonDownThumbPos.y;
			if( m_ScrollThumbBtn.m_Yp < m_ScrollBaseRect.top )		m_ScrollThumbBtn.m_Yp = m_ScrollBaseRect.top;
			else if( m_ScrollThumbBtn.m_Yp > m_ScrollBaseRect.bottom - m_ScrollThumbBtn.m_Height )
				m_ScrollThumbBtn.m_Yp = m_ScrollBaseRect.bottom - m_ScrollThumbBtn.m_Height;

			
			if(m_ScrollThumbBtn.m_Yp == m_ScrollBaseRect.top) m_ScrollThumbBtn.m_Yp++;
			else if(m_ScrollThumbBtn.m_Yp == m_ScrollBaseRect.bottom-m_ScrollThumbBtn.m_Height) {
				m_ScrollThumbBtn.m_Yp--;
			}

			float ftt = ( float )( m_ScrollThumbBtn.m_Yp - m_ScrollBaseRect.top )/ ( m_ScrollBaseRect.Height() - m_ScrollThumbBtn.m_Height );
			int per = ( int )( ftt * 100 );
			m_NowIndex = ( ( totcnt - wline ) * per ) / 100;



			return true;
		}

		//	��ũ�� �� ��ư
		if( m_ScrollUpBtn.OnMouseMove( x, y ) )				bOver = true;
		//	��ũ�� �ٿ� ��ư
		if( m_ScrollDownBtn.OnMouseMove( x, y ) )			bOver = true;
		//	�ҹ� ��ư
		if( m_ScrollThumbBtn.OnMouseMove( x, y ) )			bOver = true;
	}

	if( bOver )
	{
		SetCursor( g_pGameView->hCurHand );
		return true;
	}


	if( m_bMove == FALSE && m_bSize == FALSE )
	{
		if( !m_bOnlyNoSize )
		{
			// ������ ���� ��ư ���� üũ
			if( x >= m_Xp + m_Width - 10 && x <= m_Xp + m_Width && y >= m_Yp + m_Height - 10 && y <= m_Yp + m_Height )
			{
				SetCursor( g_pGameView->hCurHand );
				return TRUE;
			}
			// �̵� �� ���� üũ - Ÿ��Ʋ�� �ʿ��� ũ�� ���� �Ѵ�
			if( x >= m_Xp && x <= m_Xp + m_Width && y >= m_Yp && y <= m_Yp + 5 )
			{
				SetCursor( g_pGameView->hCurHand );
				return TRUE;
			}
		}
		if( !m_bOnlyNoMove )
		{
			// �̵� �� ���� üũ
			if( x >= m_Xp && x <= m_Xp + m_Width && y >= m_Yp && y <= m_Yp + m_Height )
			{
				SetCursor( g_pGameView->hCurHand );
				return TRUE;
			}
		}

		return FALSE;
	}

	if( pPage == NULL ) return FALSE;


	//	�������� �ʰ�, ������ ������ �ȵǴ°Ÿ� �ƹ��͵� ���Ѵ�
	if(	m_bOnlyNoMove && m_bOnlyNoSize )		return false;


	if( !m_bOnlyNoMove )
	{
		if( m_bMove ) // �̵��ϴ� ���̸�
		{
			SetCursor( g_pGameView->hCurHand );

			x -= m_ClickX;
			y -= m_ClickY;

			if( x < 0 )			m_Xp = 0;
			else if( x + m_Width > GAMEVIEW_WIDTH - 1 )
			{
				if( x <= GAMEVIEW_WIDTH )		m_Xp = GAMEVIEW_WIDTH - m_Width - 1;
			}
			else			m_Xp = x;

			//	ĸ�ǹ� ���� �ö󰥼� ����
			if( y < 0 )		m_Yp = 0;
	//		if( y < CAPTIONBAR_HEIGHT )		m_Yp = CAPTIONBAR_HEIGHT;
			else if( y + m_Height > GAMEVIEW_HEIGHT - 36 )
			{
				if( y <= GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT )	m_Yp = GAMEVIEW_HEIGHT - m_Height - 36;
			}
			else			m_Yp = y;

		}
	}
	if( !m_bOnlyNoSize )
	{
		if( m_bSize ) // ����� �����ϴ� ���̸�
		{
	//		SetCursor( g_pGameView->m_hCurSize );
			//	���λ����� ����
			if( m_bWidthSize )
			{
				if( x < m_Xp + 110 )					x = m_Xp + 110;
				else if( x >= GAMEVIEW_WIDTH - 2 )		x = GAMEVIEW_WIDTH - 2;

				int dx = x - m_ClickX;
				m_Width = m_OrgWidth+dx;
				if( m_OrgWidth + dx > 500 )			dx = 500 - m_OrgWidth;
			}

			//	���� ������ ��������
			if( m_bHeightSize )
			{
				int limitheight = 0;
				if( GetStyle() & CHATBOX_FLAG_TITLE )			limitheight = TITLE_HEIGHT;
				if( GetStyle() & CHATBOX_FLAG_VSCROLL )
					limitheight += m_ScrollUpBtn.m_Height + m_ScrollDownBtn.m_Height + m_ScrollThumbBtn.m_Height + 8;

				if(limitheight==0) {
					limitheight = 100;
				}

				if( m_bSizeTop )
				{
					if( y < CAPTIONBAR_HEIGHT )	y = CAPTIONBAR_HEIGHT;
					m_Yp = y;

					int dy = m_ClickY - y;
					m_Height = m_OrgHeight+dy;

					if( m_Height <= limitheight )
					{
						dy = limitheight - m_Height;
						m_Height = limitheight;
						m_Yp -= dy;
					}
				}
				else
				{
					if( y < m_Yp + limitheight )			y = m_Yp + limitheight;
					else if( y >= GAMEVIEW_HEIGHT - 2 )		y = GAMEVIEW_HEIGHT - 2;

					int dy = y - m_ClickY;
					m_Height = m_OrgHeight+dy;
				}

				if( m_Yp + m_Height > GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - 1 )
					m_Yp = GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - m_Height;
			}


			ResetChatViewEditPos();
		}
	}

	//	�ݱ� ��ư
	m_CloseBtn.m_Xp = m_Xp + m_Width - 18;
	m_CloseBtn.m_Yp = m_Yp + 3;
	//	�ּ�ȭ ��ư
	m_MiniBtn.m_Xp = m_Xp + m_Width - 33;
	m_MiniBtn.m_Yp = m_Yp + 3;

	//	��ũ�ѹ� �����ϱ�
	ResetScrollBar();
	return TRUE;
}

// �ڵ���ũ��
void CChatBox::OnTimer()
{
	if( m_ScrollUpBtn.m_bClick || m_ScrollDownBtn.m_bClick )
	{
		if( Game.GameCnt % 4 == 0 )
		{
			if( m_ScrollUpBtn.m_bClick )	UpdateButtonState( m_ScrollUpBtn.m_CmndID );
			if( m_ScrollDownBtn.m_bClick )	UpdateButtonState( m_ScrollDownBtn.m_CmndID );
		}
	}
}
// ���콺 �� ó��
BOOL CChatBox::OnMouseWheel( WPARAM wParam, LPARAM lParam )
{
	// lParam ��ǥ
	// LOWORD(wParam) Ű�������
	// HIWORD(wParam) ���� �������Ÿ�

	if( !IsWindowVisible() )						return false;
	if( !( m_Style & CHATBOX_FLAG_VSCROLL ) )		return false;

	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	g_pGameView->ScreenToClient(&pt);

	short status = LOWORD(wParam);
	short lenth  = HIWORD(wParam);

	//////////////////////
	// ä�ÿ��� ������
	//CRect rc;
	//rc.SetRect( m_Xp + 3, m_Yp, m_Xp +m_Width, m_Yp +  m_Height + 14 );
	//if( !rc.PtInRect(pt) ) return false;
	//////////////////////
/*
	int nRowsScrolled = lenth / 120;
	if( nRowsScrolled == 1 || nRowsScrolled == 8 )	{
		//m_pParent->SendMessage( WM_COMMAND, IDM_CHATWND_SCROLLUP );
		UpdateButtonState( m_ScrollUpBtn.m_CmndID );
	}
	else {
		//m_pParent->SendMessage( WM_COMMAND, IDM_CHATWND_SCROLLDOWN );
		UpdateButtonState( m_ScrollDownBtn.m_CmndID );

	}
*/
	if( lenth >= 120 ) {
		UpdateButtonState( m_ScrollUpBtn.m_CmndID );
	}
	else {
		UpdateButtonState( m_ScrollDownBtn.m_CmndID );
	}
	return true;
	//return ChatViewEdit.SendMessage( WM_MOUSEWHEEL, wParam, lParam );
}

void CChatBox::DrawChat(CDC *pDC)
{

	DWORD style = m_Style;

	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( RGB( 200, 255, 230 ) );
	pDC->SelectObject( &g_pGameView->Font2 );


	// �Ű� ��ư �׸���
	BadUserBtn.Draw(pDC);

	//	��׶��� �׸���
//	if( style & CHATBOX_FLAG_BACKGROUND )
//	{
//		pPage->DrawFillBoxGB( m_Xp, m_Yp, min( GAMEVIEW_WIDTH - 2, m_Xp + m_Width - 2 ),
//			min( GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - 2, m_Yp + m_Height - 1 ), RGBmix( 0, 2, 5 ), 12 );
//	}
/*
	//	�׵θ� �׸���
	if( style & CHATBOX_FLAG_FRAME )
	{
//		pPage->DrawEmptyBox( m_Xp, m_Yp, m_Xp + m_Width - 1, m_Yp + m_Height - 1, RGBMake555( RGB( 221, 221, 221 ) ) );
//		SetClipArea( m_Xp, m_Yp, min( GAMEVIEW_WIDTH - 2, m_Xp + m_Width - 1 ),	min( GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - 2, m_Yp + y ) );

		//	���� ����
		pPage->DrawFillBoxGB( m_Xp, m_Yp, m_Xp + m_Width - 1, m_Yp, RGBmix( 31, 31, 31 ), 18 );
		//	�Ʒ��� ����
		pPage->DrawFillBoxGB( m_Xp + 1, m_Yp + m_Height - 1, m_Xp + m_Width - 2, m_Yp + m_Height - 1, RGBmix( 31, 31, 31 ), 18 );
		//	���� ����
		pPage->DrawFillBoxGB( m_Xp, m_Yp + 1, m_Xp, m_Yp + m_Height - 1, RGBmix( 31, 31, 31 ), 18 );
		//	������ ����
		pPage->DrawFillBoxGB( m_Xp + m_Width - 1, m_Yp + 1, m_Xp + m_Width - 1, m_Yp + m_Height - 1, RGBmix( 31, 31, 31 ), 18 );
	}
*/
	// ���� ���ڿ� ��� �׸���
	// ä�� ���ڿ� �׸���
	sendedit_draw( pDC );

	//	��ũ�ѹ� ����Ұ�� ���� �׷��ش�
	if( style & CHATBOX_FLAG_VSCROLL )
	{
		DrawScrollBarUse( pDC );

		return;
	}


	int y = 0, width = m_Width, height = m_Height;

	//	Ÿ��Ʋ �׸���
	if( style & CHATBOX_FLAG_TITLE )
	{
		y = 20;
		//	Ÿ��Ʋ ���ֱ�
		DrawTitle( pDC );

		//	�ݱ� ��ư
		m_CloseBtn.Draw(pDC);
		//	�ּ�ȭ ��ư
		m_MiniBtn.Draw(pDC);

		//	���ǹ��̸� Ȱ��ȭ ������
		if( m_bMusicRoom )		pPage->PutSprAuto( m_Xp + 3, m_Yp + 3, m_pBackSpr, 9 );
		else					pPage->PutSprAuto( m_Xp + 3, m_Yp + 3, m_pBackSpr, 10 );
//		pDC->TextOut( m_Xp + 3 + 23, m_Yp + 5, "ä��â" );
	}


	//	������ �����׸�
//	pPage->PutSprAuto( m_Xp + width - 12, m_Yp + height-12, m_pBackSpr, 11 );


	if( ChatViewEdit.m_hWnd == NULL )		return;

	
	CRect rc;
	
	int totline = ChatViewEdit.GetLineCount();
	if( totline == 0 )		return;

	pDC->SetTextColor( RGB( 200, 255, 230 ) );


	rc.SetRect( m_Xp + 3, m_Yp + y, m_Xp + width, m_Yp + y + 14 );
	CString str;
	//	���������� ���μ�
	int wline = GetOnePageWriteLine();
	if( wline <= 0 )		return;

	int first = 0;
	if( totline - wline >= 0 )		first = totline - wline;

	for( int i = first; i < totline; i++ )
	{
		if( i < 0 )			return;

		// ����2000�� �Ϻ� 98 �ý��ۿ��� ���ڰ� �����°� �����ϱ� ���� ó��
		char buf[ 256 ] = { 0, };
		ChatViewEdit.GetLine( i, buf, 255 );
		buf[ ChatViewEdit.LineLength( ChatViewEdit.LineIndex( i ) ) ] = 0;

		int slen = strlen( buf );
		if( slen > 0 )
		{
			// ���� ���� �ٴ� �������� �ڵ带 ����
			// ���� 2000�� ��� ������ ���� ����� �������� ����
			// if(buf[slen-2]==13) buf[slen-2]=0;

			for( int k = 0; k <= slen; k++ )
			{
				if( buf[ k ] == 10 ) buf[ k ] = 0;
				if( buf[ k ] == -1 ) buf[ k ] = 0;
				if( buf[ k ] == 13 ) buf[ k ] = 0;

				// �����ڵ尡 �ƴϸ� ������ ���������
				//if(buf[k]>=0 && buf[k]<=31) buf[k]=0;
			}

			// ù��° ���ڿ� ���� ���δ����� ������ �����Ѵ�
			int nindex = ChatViewEdit.LineIndex( i );
			CHARFORMAT cf;
			cf.cbSize = sizeof( CHARFORMAT );
			cf.dwMask = CFM_COLOR;
			ChatViewEdit.SetSel( nindex, nindex );
			ChatViewEdit.GetSelectionCharFormat( cf );
			
			// ���� �������̸� ���� ������� ����ְ� �ٸ����̶�� �׻� �״�� ����
			if( cf.crTextColor == 0 )	pDC->SetTextColor( RGB( 200, 255, 230 ) );
			else pDC->SetTextColor( cf.crTextColor );

			str.Format( "%s", buf );
			pDC->DrawText( str, &rc, DT_LEFT );
		}

		rc.OffsetRect( 0, 14 );
	}


}

void CChatBox::DrawRule(CDC *pDC)
{
	if(g_RI.bPlayer==0)
		pPage->PutSprAuto(3, 425, g_pGameView->m_pgamebackspr, 2);
	else
		pPage->PutSprAuto(3, 425, g_pGameView->m_pgamebackspr, 2);
}


void CChatBox::Draw( CDC *pDC )
{
	// ä�ùڽ� ��� �׸���
	if( pPage == NULL )		return;
	if( m_bShow == FALSE )		return;
	
	if( Game.bViewTab ) {
		//DrawRule(pDC); // ���� ����
	}
	else {
		DrawChat(pDC); // ä�� ����
	}
	

//	if(g_RI.bPlayer==0) {
		//TabViewChatBtn.Draw(pDC);
		//TabViewRuleBtn.Draw(pDC);
//	}
	
	if( Game.bViewTab ) {
		if(Game.nChatRuleMessage == 1) { // ä�� ������
			if(Game.GameCnt/12%5== 0) 
				pPage->PutSprAuto(2, TabViewRuleBtn.m_Yp, &EtcSpr, 16);
		}
		
	} else {
		if(Game.nChatRuleMessage == 2) { // ����������
			if(Game.GameCnt%2== 0)
				pPage->PutSprAuto(129, TabViewRuleBtn.m_Yp, &EtcSpr, 17);
		}
	}

	// 1) ������� : �������� ä�ñ� ä�� ����
	// 2)          : ä�ú��� ����   ���� ����

	return;
}

//	=====================================================================
//	Ÿ��Ʋ ���ֱ�
void CChatBox::DrawTitle( CDC *pDC )
{
	CRect rect( 0, 0, m_Width - 25, 14 );
	rect.OffsetRect( m_Xp + 25, m_Yp + 3 );

	pDC->SetTextColor( m_clrTitelColor );
	m_strTitle.Format( "[%d����] %d/%d��", g_RI.RoomNum, g_RI.NowUserNum, g_RI.MaxUserNum );
	pDC->DrawText( m_strTitle, &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER );
	return;
}
//	=====================================================================

//	=========================================================================
void CChatBox::SetWndMoveState( bool bMoveState )
{
	m_bOnlyNoMove = bMoveState;		//	�������� �ʰ� �Ѵ�
	return;
}

void CChatBox::SetWndSizeState( bool bSizeState )
{
	m_bOnlyNoSize = bSizeState;		//	������ ������ �ȵȴ�

	return;
}
//	=========================================================================

void CChatBox::SetTextFormat(int kind)
{
	if( ChatViewEdit.m_hWnd == NULL )		return;
	ChatViewEdit.SetTextFormat( kind );
	return;
}

void CChatBox::SetWindowText( LPCTSTR lpszString )
{
	if( ChatViewEdit.m_hWnd == NULL )		return;
	ChatViewEdit.SetWindowText( lpszString );
	return;
}

void CChatBox::GetWindowText( CString &strString )
{
	if( ChatViewEdit.m_hWnd == NULL )		return;
	ChatViewEdit.GetWindowText( strString );
	return;
}

int CChatBox::GetLineCount( void )
{
	if( ChatViewEdit.m_hWnd == NULL )		return 0;
	return ChatViewEdit.GetLineCount();
}

void CChatBox::AddText( char *pStr, COLORREF crNewColor, bool bScroll )
{
	//	���̰� 0�ΰ��� �߰� �Ұ���
	if( pStr == NULL || strlen( pStr ) == 0 )			return;
	if( ChatViewEdit.m_hWnd == NULL )					return;

	int oldtotline = ChatViewEdit.GetLineCount();
	ChatViewEdit.AddText( pStr, crNewColor, bScroll );

	//	��Ŭ�ιٰ� ���ٸ� ó�� ���Ѵ�
	if( !( m_Style & CHATBOX_FLAG_VSCROLL ) )			return;

	//	���������� ���μ�
	int wline = GetOnePageWriteLine();
	int totline = ChatViewEdit.GetLineCount();
	//	�Ѷ��ε� ���������� ���μ����� ���ٸ�
	if( totline < wline )								return;

	//	�߰��ϱ����� ������ ���ΰ� ���� ��Ŀ���� �ִ� ������ ���ٸ�
	if( m_NowIndex >= oldtotline - wline - 1 )		m_NowIndex = totline - wline - 1;


	if( totline < wline )		return;

	if( m_NowIndex < totline - wline - 1 )
	{
		ResetScrollBar();
		return;
	}

	m_NowIndex = totline - wline;

	ResetScrollBar();

	return;
}

void CChatBox::AddText(CString *str, COLORREF crNewColor, bool bScroll)
{
	AddText( ( char * )( LPCTSTR )( *str ), crNewColor, bScroll );
	return;
}

void CChatBox::MoveWindowPlusPos( CPoint *lpPt )
{
	m_Xp += lpPt->x;
	m_Yp += lpPt->y;

	if( m_Xp < 0 )		m_Xp = 0;
	else if( m_Xp > GAMEVIEW_WIDTH - m_Width )		m_Xp = GAMEVIEW_WIDTH - m_Width;

	if( m_Yp < 0 )		m_Yp = 0;
	else if( m_Yp > GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - m_Height )		m_Yp = GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - m_Height;


	//	�ݱ� ��ư
	m_CloseBtn.MoveWindow( m_Xp + m_Width - 18, m_Yp + 3 );
	//	�ּ�ȭ ��ư
	m_MiniBtn.MoveWindow( m_Xp + m_Width - 33, m_Yp + 3 );

	//	��ũ�ѹ� ����
	ResetScrollBar();

	return;
}

void CChatBox::MoveWindow( CPoint *lpPt )
{
	m_Xp = lpPt->x;
	m_Yp = lpPt->y;

	if( m_Xp < 0 )		m_Xp = 0;
	else if( m_Xp > GAMEVIEW_WIDTH - m_Width )		m_Xp = GAMEVIEW_WIDTH - m_Width;

	if( m_Yp < 0 )		m_Yp = 0;
	else if( m_Yp > GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - m_Height )		m_Yp = GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT - m_Height;

	//	�ݱ� ��ư
	m_CloseBtn.MoveWindow( m_Xp + m_Width - 18, m_Yp + 3 );
	//	�ּ�ȭ ��ư
	m_MiniBtn.MoveWindow( m_Xp + m_Width - 33, m_Yp + 3 );

	//	��ũ�ѹ� ����
	ResetScrollBar();

	return;
}

void CChatBox::MoveWindow( int x, int y, int width, int height )
{
	m_Width = width;
	m_Height = height;

	MoveWindow( &CPoint( x, y ) );

	return;
}

void CChatBox::MoveWindow( CRect *lpRect )
{
	MoveWindow( lpRect->left, lpRect->top, lpRect->Width(), lpRect->Height() );
	return;
}

BOOL CChatBox::PtInRect( POINT point )
{
	CRect rc( 0, 0, m_Width, m_Height );
	rc.OffsetRect( m_Xp, m_Yp );
	return rc.PtInRect( point );
}

//	��ũ�ѹ� �����ϱ�
void CChatBox::ResetScrollBar( void )
{
	if( !( m_Style & CHATBOX_FLAG_VSCROLL ) )		return;

	int y = 0, width = m_Width - m_ScrollUpBtn.m_Width, height = m_Height;

	//	��ũ�ѹٰ� �ִ°��� ��������������������..10 �ȼ��� ����üũ�� �վ.. ��������
//	if( m_bHeightSize )				height -= 10;

	int plusx = 0;
	//	�׵θ��� ������
	if( m_Style & CHATBOX_FLAG_FRAME )	plusx = 1;

	//	ĸ�ǹٰ� ������
	if( m_Style & CHATBOX_FLAG_TITLE )	y = 20;


	m_ScrollBaseRect.SetRect( 0, 0, m_ScrollUpBtn.m_Width, height - y - ( m_ScrollUpBtn.m_Height * 2 ) - 6 );	//	��ũ�ѹ� ����
	m_ScrollBaseRect.OffsetRect( min( GAMEVIEW_WIDTH, m_Xp + width ) - (plusx + 8), min( GAMEVIEW_HEIGHT - GAMEVIEW_INTERPACE_HEIGHT, m_Yp + y ) + m_ScrollUpBtn.m_Height);

	m_ScrollUpBtn.MoveWindow( m_ScrollBaseRect.left, m_ScrollBaseRect.top - m_ScrollUpBtn.m_Height -1);
	m_ScrollDownBtn.MoveWindow( m_ScrollBaseRect.left, m_ScrollBaseRect.bottom +1 );
	int thumbx = ( m_ScrollBaseRect.Width() - m_ScrollThumbBtn.m_Width ) / 2;
	m_ScrollThumbBtn.MoveWindow( m_ScrollBaseRect.left + thumbx, m_ScrollUpBtn.m_Yp + m_ScrollUpBtn.m_Height + 1 );


	int totline = 0;
	if( ChatViewEdit.m_hWnd != NULL )		totline = ChatViewEdit.GetLineCount();
	//	���������� ���μ�
	int wline = GetOnePageWriteLine();
	if( totline < wline || totline <= 0 || wline <= 0  )
	{
		m_ScrollThumbBtn.MoveWindow( m_ScrollBaseRect.left + thumbx, m_ScrollBaseRect.top+1 );
		return;
	}

	int tt = totline - wline;
	if( tt <= 0 )	tt = 1;

	float ftt = ( float )m_NowIndex / tt;
	int per = ( int )( ftt * 100 );
	if( per > 100 )
	{
		m_NowIndex = tt;
		per = 100;
	}

	int bb = m_ScrollBaseRect.Height() - m_ScrollThumbBtn.m_Height;
	int dd = ( bb * per ) / 100;
	m_ScrollThumbBtn.m_Yp = m_ScrollBaseRect.top + dd;

	if(m_ScrollThumbBtn.m_Yp == m_ScrollBaseRect.top) m_ScrollThumbBtn.m_Yp++;
	else if(m_ScrollThumbBtn.m_Yp == m_ScrollBaseRect.bottom-m_ScrollThumbBtn.m_Height) {
		m_ScrollThumbBtn.m_Yp--;
	}


	return;
}

//	��ũ�ѹ� üũ�ϱ�
void CChatBox::CheckScrollBar( void )
{
	if( !( m_Style & CHATBOX_FLAG_VSCROLL ) )		return;
	if( !m_ScrollUpBtn.m_bClick && !m_ScrollDownBtn.m_bClick )	return;

	if( Game.GameCnt % 8 != 0 )			return;

	if( m_ScrollUpBtn.m_bClick )		m_pParent->SendMessage( WM_COMMAND, IDM_CHATWND_SCROLLUP );
	if( m_ScrollDownBtn.m_bClick )		m_pParent->SendMessage( WM_COMMAND, IDM_CHATWND_SCROLLDOWN );

	return;
}

//	��ũ�ѹ� ����Ұ��
void CChatBox::DrawScrollBarUse( CDC *pDC )
{
	int y = 3, width = m_Width, height = m_Height;

	//	Ÿ��Ʋ �׸���
	if( m_Style & CHATBOX_FLAG_TITLE )
	{
		y = 20;
		DrawTitle( pDC );
		m_CloseBtn.Draw( pDC);
		m_MiniBtn.Draw( pDC);

		//	���ǹ��̸� Ȱ��ȭ ������
		if( m_bMusicRoom )		pPage->PutSprAuto( m_Xp + 3, m_Yp + 3, m_pBackSpr, 9 );
		else					pPage->PutSprAuto( m_Xp + 3, m_Yp + 3, m_pBackSpr, 10 );
	}

	//	��Ŭ�ι� �׸���
	if( m_Style & CHATBOX_FLAG_VSCROLL )
	{
		width -= ( m_ScrollUpBtn.m_Width + 4 );
/*
		//	���� ����
		pPage->DrawFillBoxGB( m_ScrollBaseRect.left, m_ScrollBaseRect.top, m_ScrollBaseRect.left,
			m_ScrollBaseRect.bottom - 1, RGBmix( 31, 31, 31 ), 18 );

		//	������ ����
		pPage->DrawFillBoxGB( m_ScrollBaseRect.right - 1, m_ScrollBaseRect.top, m_ScrollBaseRect.right - 1,
			m_ScrollBaseRect.bottom - 1, RGBmix( 31, 31, 31 ), 18 );


		//	���� ����
		pPage->DrawFillBoxGB( m_ScrollBaseRect.left + 1, m_ScrollBaseRect.top, m_ScrollBaseRect.right - 2,
			m_ScrollBaseRect.top, RGBmix( 31, 31, 31 ), 18 );

		//	�Ʒ��� ����
		pPage->DrawFillBoxGB( m_ScrollBaseRect.left + 1, m_ScrollBaseRect.bottom - 1, m_ScrollBaseRect.right - 2,
			m_ScrollBaseRect.bottom - 1, RGBmix( 31, 31, 31 ), 18 );
*/

		//m_ScrollUpBtn.Draw( pDC );			//	��ũ�� �� ��ư
		//m_ScrollDownBtn.Draw( pDC );			//	��ũ�� �ٿ� ��ư
		//m_ScrollThumbBtn.Draw( pDC );		//	�ҹ� ��ư
	}
	else	m_ScrollBaseRect = 0;


	if( ChatViewEdit.m_hWnd == NULL )		return;

	
	// ���� ���ڿ� ��� �׸���
	CRect rc;
	
	// ä�� ���ڿ� �׸���
	int totline = ChatViewEdit.GetLineCount();
	if( totline == 0 )		return;

	//	��ũ�ѹ� üũ�ϱ�
	CheckScrollBar();


	pDC->SetTextColor( RGB( 200, 255, 230 ) );


	rc.SetRect( m_Xp + 3, m_Yp + y, m_Xp + width, m_Yp + y + 14 );
	CString str;
	//	���������� ���μ�
	int wline = GetOnePageWriteLine();
	if( wline <= 0 )		return;

	int line = 0;
	for( int i = 0; i < wline; i++ )
	{
		line = i + m_NowIndex;
		if( line >= totline )		return;

		// ����2000�� �Ϻ� 98 �ý��ۿ��� ���ڰ� �����°� �����ϱ� ���� ó��
		char buf[ 256 ] = { 0, };
		ChatViewEdit.GetLine( line, buf, 255 );
		buf[ ChatViewEdit.LineLength( ChatViewEdit.LineIndex( line ) ) ] = 0;

		int slen = strlen( buf );
		if( slen > 0 )
		{
			// ���� ���� �ٴ� �������� �ڵ带 ����
			// ���� 2000�� ��� ������ ���� ����� �������� ����
			// if(buf[slen-2]==13) buf[slen-2]=0;

			for( int k = 0; k <= slen; k++ )
			{
				if( buf[ k ] == 10 ) buf[ k ] = 0;
				if( buf[ k ] == -1 ) buf[ k ] = 0;
				if( buf[ k ] == 13 ) buf[ k ] = 0;

				// �����ڵ尡 �ƴϸ� ������ ���������
				//if(buf[k]>=0 && buf[k]<=31) buf[k]=0;
			}

			// ù��° ���ڿ� ���� ���δ����� ������ �����Ѵ�
			int nindex = ChatViewEdit.LineIndex( line );
			CHARFORMAT cf;
			cf.cbSize = sizeof( CHARFORMAT );
			cf.dwMask = CFM_COLOR;
			ChatViewEdit.SetSel( nindex, nindex );
			ChatViewEdit.GetSelectionCharFormat( cf );
			
			// ���� �������̸� ���� ������� ����ְ� �ٸ����̶�� �׻� �״�� ����
			if( cf.crTextColor == 0 )	pDC->SetTextColor( RGB( 200, 255, 230 ) );
			else pDC->SetTextColor( cf.crTextColor );

			str.Format( "%s", buf );
			pDC->DrawText( str, &rc, DT_LEFT );
		}

		rc.OffsetRect( 0, 14 );
	}

	return;
}

void CChatBox::sendedit_draw(CDC *pDC)
{
	CRect rc;
	SendEdit.GetClientRect(rc);
	int ypos = 3;
//	rc.OffsetRect(m_Xp+2, m_Yp+m_Height+ypos);
//	pPage->DrawFillBoxGB(rc.left, rc.top, min(GAMEVIEW_WIDTH - 2,rc.right), min(GAMEVIEW_HEIGHT - 2,rc.bottom), RGBmix(50,155,155), 12);

	// ���� ä�� ���ڿ� ���
	char buf[256]={0,};
	//SendEdit.GetLine(SendEdit.LineFromChar(-1), buf, 255);	// ���� ĳ�� ��ġ�� ���ڿ��� ����
	SendEdit.GetWindowText(buf, 255);
	CString str;
	str.Format("%s", buf);
	
	// ����Ʈ ��Ʈ�ѳ��� ĳ���� ��ġ�� ��� ĳ���� ����� ��ǥ�� ���
	CPoint pnt = SendEdit.GetCaretPos();
	pnt.x += m_Xp+3;
	pnt.y += m_Yp+m_Height+ypos;
	
	pDC->SetTextColor(RGB(200, 255, 230));


	// ���� ���ڿ��� ������ �ƴѰ�츸 �����
	SendEdit.GetClientRect(rc);
//	int user_5_7_num =0;
//	if(g_RI.bPlayer!=0) user_5_7_num=2;
	rc.OffsetRect(m_Xp+3, m_Yp+m_Height+ypos );
/*
	if( str.GetLength() * 6 >= rc.Width())
	{
		CCharCutInArray cchar;
		str = cchar.GetStrMid( str, 36 ); // ���� ������ �� ���� ��
	}
*/

	CPoint cpnt = SendEdit.PosFromChar(0);
	if(str.GetLength()>0 && str.GetAt(0)!=-1)
	{
		pDC->IntersectClipRect( rc );
		rc.left += cpnt.x;
		pDC->DrawText(str, &rc, DT_LEFT);
		pDC->SelectClipRgn( NULL );//rc������ ��ŭ Ŭ���� 
	}

	
	// ĳ���� �׷��ش�
	static int caretCnt = 0;
	caretCnt++;
	if(caretCnt>7) {m_bShowCaret ^= 1; caretCnt = 0; }
	if(SendEdit.GetFocus() != NULL) {
		if(SendEdit.GetFocus()->m_hWnd == SendEdit.m_hWnd)	// ��Ŀ���� �������ȸ� �׷��ش�
		{
			if(m_bShowCaret) DrawBoxXOR( pnt.x, pnt.y , pnt.x+1, pnt.y+12, (WORD*)pPage->lpBit);
		}
	}
}
