// GraphButton.h: interface for the CGraphButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHBUTTON_H__F560D682_2133_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_GRAPHBUTTON_H__F560D682_2133_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"

class CGraphButton  
{
public:

//	========= ���콺 ���������� �ѹ��� ������ �Ѵ� =============
	//	���� ��ȣ�� �����Ǿ���
	bool	m_bOverSoundInit;
	bool	m_bOverSound;
	//	����
	int		m_SoundVol;

	enum SND_ENUM		m_SoundNum;

	void	SetMouseOverSound( enum SND_ENUM snd, int vol = 100 );
	void	MouseOverSoundNoUse( void );
	POINT	GetButtonPoint( void );
//	============================================================

	CWnd *m_pParent;
	CPage *m_pPage;
	xSprite *m_pSpr;
	int m_CmndID;
	BOOL m_bShow;
	BOOL m_bEnable;
	BOOL m_bClick;
	BOOL m_bOver;
	int m_Xp, m_Yp;
	int m_Width, m_Height;

	int		m_iButtonNumber;
	int		m_nAniButtonIndex;
	int		m_nMouseMoveCnt;

//	===================== ���� �̵� ===========================================
	void	MoveWindow( int x, int y ) { m_Xp = x; m_Yp = y; }
	void	MoveWindow( POINT point ) { MoveWindow( point.x, point.y ); }
//	===========================================================================

//	===================== ���� ���� ===========================================
	bool	m_bShowToolTip;
	CString m_strToolTip;
	CFont	*m_pFont;
	void	SetToolTip( char *pTooltip = NULL, CFont *pFont = NULL );		//	�����ֱ�
	void	ShowToolTip( bool bShow = true );
	void	DrawToolTip( CDC *pDC, int Alpha = 20, COLORREF clrBoxFillBack = RGB( 0, 0, 0 ), COLORREF clrText = RGB( 255, 255, 255 ), bool bFixing = false );
//	===========================================================================

	int		m_ButtonCnt;

	CGraphButton();
	virtual ~CGraphButton();
	void Init(CWnd *pWnd, CPage *ppage, int x, int y, xSprite *pspr, int iButtonNumber, int cmndid, int ButtonCnt = 4, bool bSound = false);
	void SetSprite(xSprite *pspr, int iButtonNumber);
	void Enable(BOOL bVal);
	void Show(BOOL bVal);
	BOOL OnMouseMove(int x, int y);
	BOOL OnLButtonDown(int x, int y);
	BOOL OnLButtonUp(int x, int y);
	void Draw( CDC *pDC, int opt = 0, int grade = 0 );
	int GetButtonState( void );
};

//////////////////////////// �����̴� ��ư Ŭ���� //////////////////////////////

class CSliderButton  
{
public:
	CWnd *m_pParent;
	CPage *m_pPage;
	xSprite *m_pSpr;
	int CmndID;
	int MovCmndID;

	BOOL bShow;
	BOOL bEnable;
	BOOL bClick;
	BOOL bOver;
	int m_Xp, m_Yp, m_FirstXp, m_FirstYp;
	int m_Width, m_Height;

	int TotGrade;
	int NowGrade;
	int m_MStartPos;

	int	m_iButtonNum;
	int m_ButtonCnt;


	int		m_BodyLength;		//	�ٵ� ����
	void	SetBodyLength( int BodyLength );
	int		m_NowPage;			//	���� ������
	int		m_TotPage;			//	�� ������ ����
	void	SetNowPage( int NowPage );
	void	SetTotPage( int TotPage );
	int		GetNowPageNum( void ) { return m_NowPage; }

	bool	m_bVertical;		//	���γ�?
	bool	IsStyleVertical( void ) { return m_bVertical; }

//	===================== ���� �̵� ===========================================
	void	MoveWindow( int x, int y ) { m_Xp = x; m_Yp = y; }
	void	MoveWindow( POINT point ) { MoveWindow( point.x, point.y ); }
//	===========================================================================

	CSliderButton();
	virtual ~CSliderButton();
	void Init(CWnd *pWnd, CPage *pPage, int x, int y, xSprite *pSpr, int cmndid, int movcmndid, int iButtonNum, int ButtonCnt = 3, bool bVertical = false );
	void SetSprite(xSprite *pspr);
	void Enable(BOOL bVal);
	BOOL OnMouseMove(int x, int y);
	BOOL OnLButtonDown(int x, int y);
	BOOL OnLButtonUp(int x, int y);
	void Draw(CDC *pDC);
};


#endif // !defined(AFX_GRAPHBUTTON_H__F560D682_2133_11D4_97A5_0050BF0FBE67__INCLUDED_)
