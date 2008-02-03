// GraphButton.cpp: implementation of the CGraphButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphButton.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphButton::CGraphButton()
{
	m_pParent = NULL;
	m_pPage = NULL;
	m_pSpr = NULL;
	m_CmndID = 0;
	m_bShow = FALSE;
	m_bEnable = TRUE;
	m_Xp = m_Yp = 0;
	m_Width = 0;
	m_Height = 0;
	m_bClick = FALSE;
	m_bOver = FALSE;
	m_iButtonNumber = 0;

	//	마우스 오버됐을때 한번만 나오게 한다
	m_bOverSoundInit = m_bOverSound = false;
	m_SoundNum = SND50;
	m_bShowToolTip = false;

	m_pFont = NULL;
	m_ButtonCnt = 0;
}

CGraphButton::~CGraphButton()
{
}

void CGraphButton::Init(CWnd *pWnd, CPage *ppage, int x, int y, xSprite *pspr, int iButtonNumber, int cmndid, int ButtonCnt, bool bSound )
{
	m_pParent = pWnd;
	m_pPage = ppage;
	m_pSpr = pspr;
	m_CmndID = cmndid;
	m_bEnable = m_bShow = TRUE;
	m_bClick = FALSE;
	m_bOver = FALSE;

	m_iButtonNumber = iButtonNumber;
	m_ButtonCnt = ButtonCnt;

	//	무조건 마우스 오버 사운드 설정된다
	SetMouseOverSound( SND50 );

	m_bOverSoundInit = bSound;
	
	m_Xp = x;
	m_Yp = y;
	m_Width = 0;
	m_Height = 0;
	if( m_pSpr != NULL )
	{
		if(m_pSpr->spr != NULL)
		{
			m_Width = m_pSpr->spr[ m_iButtonNumber * m_ButtonCnt ].xl;
			m_Height = m_pSpr->spr[ m_iButtonNumber * m_ButtonCnt ].yl;
		}
	}



}

void CGraphButton::SetSprite(xSprite *pspr, int iButtonNumber)
{
	m_iButtonNumber = iButtonNumber;
	m_pSpr = pspr;
	if(m_pSpr->spr != NULL)
	{
		m_Width = m_pSpr->spr[ m_iButtonNumber * 3 ].xl;
		m_Height = m_pSpr->spr[ m_iButtonNumber * 3 ].yl;
	}
}

void CGraphButton::Enable(BOOL bVal)
{
	if(bVal==FALSE)
	{
		m_bClick = FALSE;
		m_bOver = FALSE;
	}
	m_bEnable = bVal;
}

void CGraphButton::Show(BOOL bVal)
{
	CGraphButton::Enable( bVal );

	m_bShow = bVal;
}

BOOL CGraphButton::OnMouseMove(int x, int y)
{
	if(m_bEnable==FALSE) return FALSE;

	if(x >= m_Xp && x <= m_Xp+m_Width && y >= m_Yp && y <= m_Yp+m_Height)
	{
		SetCursor(g_pGameView->hCurHand);
		m_bOver = TRUE;
		//	마우스 오버됐을때 한번만 나오게 한다
		if( !m_bOverSound && m_bOverSoundInit )
		{
			m_bOverSound = true;
			Sound.Play( m_SoundNum, false, m_SoundVol );
		}
		return TRUE;
	}
	else
	{
		m_bOverSound = false;
		m_bOver = FALSE;
		return FALSE;
	}
}

BOOL CGraphButton::OnLButtonDown(int x, int y)
{
	if(m_bEnable==FALSE) return FALSE;

	if(x >= m_Xp && x <= m_Xp+m_Width && y >= m_Yp && y <= m_Yp+m_Height) m_bClick = TRUE;
	else { m_bClick = FALSE; return FALSE; }
	return TRUE;
}

BOOL CGraphButton::OnLButtonUp(int x, int y)
{
	if(m_bEnable==FALSE) return FALSE;

	if(m_bClick)
	{
		m_bClick = FALSE;
		if(x >= m_Xp && x <= m_Xp+m_Width && y >= m_Yp && y <= m_Yp+m_Height)
		{
			// 버튼 눌림 메세지를 보냄
			m_pParent->SendMessage(WM_COMMAND, m_CmndID, NULL);
			return TRUE;
		}
	}
	m_bClick = FALSE;
	return FALSE;
}

void CGraphButton::Draw(CDC *pDC, int opt, int grade )
{
	if(m_pPage == NULL) return;
	if(m_pSpr == NULL) return;
	if(m_bShow == FALSE) return;

	// 버튼 그리기
	if(m_bEnable==FALSE)
	{
//		if(m_pSpr->sfhead.TotalSprNum <4) m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * 3 ) + 2, opt, grade );
		if(m_ButtonCnt <4) m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, opt, grade );
		else m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 3, opt, grade);		//	다운된그림
	}
	else if(m_bOver==TRUE && m_bClick==FALSE)	m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 1, opt, grade);
	else if(m_bClick==FALSE) m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 0, opt, grade);
	else if(m_bClick==TRUE) m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, opt, grade);

	return;
}

int CGraphButton::GetButtonState( void )
{
	if(m_bShow == FALSE) return -1;

	// 버튼 그리기
	if( m_bOver && !m_bClick )		return 1;
	else if( !m_bClick )			return 0;
	else if( m_bClick )				return 2;

	return -1;
}

void CGraphButton::SetMouseOverSound( SND_ENUM snd, int vol )
{
	m_SoundNum = snd;
	m_SoundVol = vol;
	m_bOverSoundInit = true;
	return;
}

void CGraphButton::MouseOverSoundNoUse( void )
{
	m_bOverSoundInit = false;
	return;
}

POINT CGraphButton::GetButtonPoint( void )
{
	return CPoint( m_Xp, m_Yp );
}

//	===========================================================================
//	툴팁 사용하기
void CGraphButton::SetToolTip( char *pTooltip, CFont *pFont )
{
	m_pFont = pFont;

	CString strTooltip = pTooltip;
	if( strTooltip.GetLength() == 0 || m_pFont == NULL )
	{
		m_bShowToolTip = false;
		m_strToolTip.Empty();
		pFont = NULL;
		return;
	}

	m_bShowToolTip = true;
	m_strToolTip = strTooltip;

	return;
}

void CGraphButton::ShowToolTip( bool bShow )
{
	m_bShowToolTip = bShow;

	if( m_strToolTip.GetLength() == 0 || m_pFont == NULL )		m_bShowToolTip = false;

	return;
}

#define RGBMake555( color )			(   ( ( ( (WORD)( GetRValue(color) ) )>>3)<<10 ) + ( ( ( (WORD)( GetGValue(color) ) )>>3)<<5 ) + ( ( ( (WORD)( GetBValue(color) ) )>>3) ) )

void CGraphButton::DrawToolTip( CDC *pDC, int Alpha, COLORREF clrBoxFillBack, COLORREF clrText, bool bFixing )
{
	if( m_bShow == FALSE )			return;
	if( !m_bShowToolTip )			return;
	if( m_pPage == NULL )			return;
	if( m_pSpr == NULL )			return;
	if( pDC == NULL )				return;
	if( !m_bOver || m_bClick )		return;
	int len = m_strToolTip.GetLength();
	if( len == 0 )					return;

	CFont *pFont = ( CFont * )pDC->SelectObject( m_pFont );

	int width = len * 12 + 12;
	int height = 12 + 3;

	CRect tr;
	tr.SetRect( 0 , 0, width, height );
	int dh = pDC->DrawText( m_strToolTip, len, &tr, DT_LEFT | DT_WORDBREAK| DT_CALCRECT );

	POINT pt;
	GetCursorPos(&pt);
	m_pParent->ScreenToClient(&pt);
	tr.OffsetRect( pt.x + 10, pt.y + 25 );

	tr.left -= 5;
	tr.right += 5;
	tr.bottom += 2;
	tr.top -= 2;

	if( tr.left < 0 )	tr.OffsetRect( -tr.left, 0 );
	if( tr.right > m_pPage->width ) tr.OffsetRect( m_pPage->width - tr.right, 0 );
	if( tr.top < 0 )	tr.OffsetRect( 0, -tr.top );
	if( tr.bottom > m_pPage->height )
	{
		int left = tr.left;
		width = tr.Width();
		tr.SetRect( 0 , 0, width, height );
		if( bFixing )		tr.OffsetRect( left, m_Yp - 20 );
		else				tr.OffsetRect( left, pt.y - 25 );
	}


	m_pPage->DrawFillBoxGB( tr.left , tr.top -1, tr.right , tr.bottom , RGBMake555( clrBoxFillBack ), Alpha );


	COLORREF crColor = pDC->SetTextColor( clrText );
	tr.left += 5;
	tr.right -= 5;
	tr.bottom -= 2;
	tr.top += 2;
	pDC->DrawText( m_strToolTip, len, &tr, DT_LEFT |DT_WORDBREAK );
	pDC->SetTextColor( crColor );


	pDC->SelectObject( pFont );

	return;
}
//	===========================================================================








///////////////////// 슬라이더 버튼 클래스 //////////////////////

CSliderButton::CSliderButton()
{
	m_pParent = NULL;
	m_pPage = NULL;
	m_pSpr = NULL;
	CmndID = 0;
	bShow = FALSE;
	bEnable = TRUE;
	m_Xp = m_Yp = 0;
	m_Width = 0;
	m_Height = 0;
	bClick = FALSE;
	bOver = FALSE;

	m_FirstXp = m_FirstYp = 0;

	TotGrade = 0;
	NowGrade = 0;
	m_MStartPos = 0;

	m_iButtonNum = 0;
	m_ButtonCnt = 3;

	m_bVertical = false;

	m_NowPage = 0;
	m_TotPage = 1;
}

CSliderButton::~CSliderButton()
{

}

void CSliderButton::Init(CWnd *pWnd, CPage *pPage, int x, int y, xSprite *pSpr, int cmndid, int movcmndid, int iButtonNum, int ButtonCnt, bool bVertical )
{
	m_pParent = pWnd;
	m_pPage = pPage;
	m_pSpr = pSpr;
	CmndID = cmndid;
	MovCmndID = movcmndid;
	bShow = TRUE;
	bClick = FALSE;
	bOver = FALSE;

	m_iButtonNum = iButtonNum;
	m_ButtonCnt = ButtonCnt;

	m_bVertical = bVertical;

	m_FirstXp = m_Xp = x;
	m_FirstYp = m_Yp = y;

	m_Width = 0;
	m_Height = 0;
	if( m_pSpr->spr != NULL )
	{
		m_Width = m_pSpr->spr[m_iButtonNum * m_ButtonCnt].xl;
		m_Height = m_pSpr->spr[m_iButtonNum * m_ButtonCnt].yl;
	}

	//	0개이므로 슬라이더 버튼은 움직이지 않는다
	SetNowPage( 0 );
	SetTotPage( 0 );
}

void CSliderButton::SetSprite(xSprite *pSpr)
{
	m_pSpr = pSpr;
	if( m_pSpr->spr != NULL )
	{
		m_Width = m_pSpr->spr[m_iButtonNum * m_ButtonCnt].xl;
		m_Height = m_pSpr->spr[m_iButtonNum * m_ButtonCnt].yl;
	}
}

void CSliderButton::Enable(BOOL bVal)
{
	if(bVal==FALSE)
	{
		bClick = FALSE;
		bOver = FALSE;
	}
	bEnable = bVal;

	m_MStartPos = 0;
}

int		GetPageNum( int x, int y );

BOOL CSliderButton::OnMouseMove(int x, int y)
{
	if(bEnable==FALSE) return FALSE;

	if(bClick == FALSE)
	{
		if( x >= m_Xp && x <= m_Xp + m_Width && y >= m_Yp && y <= m_Yp + m_Height )
		{
			SetCursor(g_pGameView->hCurHand);
			bOver = TRUE;
			return TRUE;
		}
		else { bOver = FALSE; return FALSE; }
	}
	else
	{
#ifndef _DEBUG
		//	아직 완성 되지 않았다
		return true;
#endif

		int pos = 0;
		if( IsStyleVertical() )
		{
			pos = y - m_MStartPos;
			if( m_Yp + pos < m_FirstYp )					return true;
			if( m_Yp + pos > m_FirstYp + m_BodyLength )		return true;

			m_MStartPos = y;
			m_Yp += pos;

			pos = m_Yp;
			pos -= m_FirstYp;
		}
		else
		{
			pos = x - m_MStartPos;
			if( m_Xp + pos < m_FirstXp )					return true;
			if( m_Xp + pos > m_FirstXp + m_BodyLength )		return true;

			m_MStartPos = x;
			m_Xp += pos;
	
			pos = m_Xp;
			pos -= m_FirstXp;
		}

		int totpage = m_TotPage - 1;
		if( totpage <= 0 )			totpage = 1;

		int page = 0;
		int per = ( int )( ( ( float )pos / ( float )m_BodyLength ) * 100 );
		int nowper = 100 / totpage;
		page = per / nowper;

		if( page == m_NowPage )		return true;

		m_NowPage = page;
		if( m_NowPage >= m_TotPage )		m_NowPage = m_TotPage - 1;
		m_MStartPos = pos;

		// 마우스 드래깅 메세지를 보냄
		if( pos != 0 ) m_pParent->SendMessage( WM_COMMAND, MovCmndID, m_NowPage );
		return TRUE;
	}
}

BOOL CSliderButton::OnLButtonDown(int x, int y)
{
	if( !bEnable ) return false;

	if( x >= m_Xp && x <= m_Xp + m_Width && y >= m_Yp && y <= m_Yp + m_Height )
	{
		bClick = TRUE;
		if( IsStyleVertical() )		m_MStartPos = y;
		else						m_MStartPos = x;
		// 버튼 눌림 메세지를 보냄
		m_pParent->SendMessage(WM_COMMAND, CmndID, 0);
	}
	else
	{
		bClick = FALSE;
		return false;
	}

	return true;
}

BOOL CSliderButton::OnLButtonUp(int x, int y)
{
	if( !bEnable ) return false;

	m_MStartPos = 0;

	if(bClick)
	{
		// 버튼 떨어짐 메세지를 보냄
		m_pParent->SendMessage(WM_COMMAND, CmndID, 1);
	}
	bClick = FALSE;

	if( x >= m_Xp && x <= m_Xp + m_Width && y >= m_Yp && y <= m_Yp + m_Height )		return true;

	return false;
}

void CSliderButton::Draw(CDC *pDC)
{
	if( m_pPage == NULL ) return;
	if( m_pSpr == NULL ) return;
	if( !bShow ) return;

	// 버튼 그리기
	if( bEnable )
	{
		if( m_ButtonCnt < 4 ) m_pPage->PutSprAuto( m_Xp, m_Yp, m_pSpr, m_iButtonNum * m_ButtonCnt + 2 );
		else m_pPage->PutSprAuto( m_Xp, m_Yp, m_pSpr, m_iButtonNum * m_ButtonCnt + 3 );
	}
	else if( bOver && !bClick ) m_pPage->PutSprAuto( m_Xp, m_Yp, m_pSpr, m_iButtonNum * m_ButtonCnt + 1 );
	else if( !bClick ) m_pPage->PutSprAuto( m_Xp, m_Yp, m_pSpr, m_iButtonNum * m_ButtonCnt + 0 );
	else if( bClick ) m_pPage->PutSprAuto( m_Xp, m_Yp, m_pSpr, m_iButtonNum * m_ButtonCnt + 2 );
}

//	슬라이더바가 움질일수 있는 최대 길이
void CSliderButton::SetBodyLength( int BodyLength )
{
	if( m_pPage == NULL )		return;
	if( m_pSpr == NULL )		return;

	if( BodyLength <= 0 )
	{
		BodyLength = m_Width;
		if( IsStyleVertical() )		BodyLength = m_Height;
	}

	if( IsStyleVertical() )		m_BodyLength = BodyLength - m_Height;
	else						m_BodyLength = BodyLength - m_Width;

	return;
}

void CSliderButton::SetTotPage( int TotPage )
{
	if( TotPage <= 0 )		TotPage = 1;
	m_TotPage = TotPage;
}

void CSliderButton::SetNowPage( int NowPage )
{
	if( m_pPage == NULL )		return;
	if( m_pSpr == NULL )		return;
	if( m_TotPage <= 0 )			return;
	if( NowPage < 0 )			NowPage = 0;
	else if( NowPage >= m_TotPage )		NowPage = m_TotPage - 1;
	if( NowPage < 0 )		NowPage = 0;

	int totpage = m_TotPage - 1;
	if( totpage <= 0 )		totpage = 1;

	m_NowPage = NowPage;

	int per = ( int )( ( ( float )m_NowPage / ( float )totpage ) * 100 );

	//	세로일때
	if( IsStyleVertical() )
	{
		m_Yp = m_FirstYp + ( m_BodyLength * per ) / 100;

		if( m_MStartPos != 0 )		m_MStartPos += ( m_Yp + m_MStartPos );
	}
	//	가로일때
	else
	{
		m_Xp = m_FirstXp + ( m_BodyLength * per ) / 100;

		if( m_MStartPos != 0 )		m_MStartPos += ( m_Xp + m_MStartPos );
	}

	return;
}
