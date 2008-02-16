/******************************************************************************\
* file   : ListCtrl.h
* created: 1997.09.22
*
* description:
* Implements a CListCtrl which can mark full rows.
\******************************************************************************/

#ifndef _CHEADERCTRLEX_H_
#define _CHEADERCTRLEX_H_


/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx window

class CHeaderCtrlEx : public CHeaderCtrl
{
// Construction
public:
	CHeaderCtrlEx();
	virtual ~CHeaderCtrlEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaderCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL		m_bLButtonDown;

	BOOL		m_bDrawShadow;
	COLORREF	m_clrShadowColor;

	void SetTextShadow(BOOL bUse);
	void SetTextShadowColor(COLORREF color);

	BOOL LoadBitmapFile( LPCTSTR szFilename );
	BOOL LoadBitmapFile( UINT nIDResource );
	BOOL TransDraw( HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc, COLORREF colorTransparent = 0 );

	void SetColors( COLORREF colorBack, COLORREF colorText );


	//	이미지를 사용하지 않을경우 Init() 호출하지 말것
	//	bImageUse - ture 이면 이미지를 사용, false 이면 사용하지 않는다..
	//	iImageCnt - 헤더의 칼럼수( = 이미지 갯수 )
	//	clrColorKey - 칼라키
	//	pImagePoint.x - 이미지에서 X 좌표, ImgPoint.y - 이미지 넓이( Width ), 이미지의 Y 는 항상 0 이다
	//				Height 는 14 보다 크면 안된다
	void Init( BOOL bImageUse = false, int iImageCnt = 0, COLORREF clrColorKey = 0, POINT *pImagePoint = NULL );

	void Release( void );

protected:
    COLORREF m_colorText, m_colorBack, m_colorBorder;


	//	글짜 대신에 이미지 사용 여부
	BOOL		m_bImageUse;
	COLORREF	m_clrColorKey;
	CBitmap		m_Bitmap;
	int			m_ImgWidth;
	int			m_ImgHeight;
	POINT		*m_pImagePoint;
	int			m_ImageCount;

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeaderCtrlEx)
	afx_msg void OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif










/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window
#ifndef _CLISTCTRLEX_H_
#define _CLISTCTRLEX_H_

#include <afxtempl.h>
#include "TitleTip.h"

#define LVNU_SELCHANGED 0x1000

/**
* A super CListControl.
* <P>features:
* <UL>
*   <LI>Title Tip item expantion
*   <LI>full row selection
*   <LI>notifies parent selection has changed
*   <LI>
* </UL>
*/

class CListCtrlEx : public CListCtrl
{
	typedef struct LISTCTRL_COLUMN
	{
		CListCtrlEx		*pListCtrl;		// 리스트 컨트롤 포인터
		int				iSubItem;		// 소트할 서브 아이템
	}LISTCTRL_COLUMN;

	typedef CListCtrl inherited;

	DECLARE_DYNCREATE(CListCtrlEx)

// Construction
protected:
	BOOL m_bHeadColumnColorUse;	//	헤더에 그림 입히기
	BOOL m_bSortUse;	//	정렬을 사용 하는지 여부
	int m_UserMode;		//	사용 모드
	COLORREF m_crMan, m_crWoman;

	BOOL m_bDrawFocusRect;	//	포커스 받은 아이템의 Focus Rectangle을 그릴지 여부
	

public:
    CHeaderCtrlEx		m_HeaderCtrl;

	//char m_UserNum_3000[ 3000 ];
	bool m_bSortType;

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	void	SetUserStep( int Num, char State );

	//	스콜모양 변경 할것인지 false 이면 기본 모양
	void	SetScrollBarChange( BOOL bExChange = false );
	//	그리드라인 사용 할것인지
	void	SetGridLineUse( BOOL bGridLineUse = false );
	//	포커스 받은 아이템의 점선을 그릴것인지 설정
	void	SetDrawFocusRect( BOOL bUse = true );


	//	헤더에 칼라 적용이 안됨
//	CHeaderCtrlEx	*GetHeaderCtrl( void );



	//	UserMode : 0 기본, 1 번 남녀로 글씨색 구별, 2번 캐릭터 단계별 사용,
	//				3번 4번은 배경칼라2개 글씨칼라 2개 사용
	//
	//				0 - 기본 콘트롤.. 배경과 글씨색 각각 1개씩 사용.
	//					Init( bool, bool, 0, BkColor( = TextBkColor ), TextColor ) )
	//					Init() 함수 호출 안하면 
	//						- SetBackColors( BkColor, TextBkColor ), SetTextColor() 호출하거나
	//						- SetBkColor(), SetTextBkColor(), SetTextColor()를 호출할것
	//				1 - 캐치마인드용, 배경색 1, 글씨색은 2( 남, 여 )개 사용 
	//						SetBackColors( BkColor, TextBkColor )를 호출하거나 SetBkColor(), SetTextBkColor() 를 호출
	//				2 - 캐치마인드용, 배경색 1, 단계별 칼라 사용( 6가지 )
	//					Init( bool, bool, 2, BkColor, TextBkColor, *pColor )
	//						SetBkColor(), SetTextBkColor() 를 호출( 안해도 됨 )
	//				3 - 아이콘 번호 0, 1 번은 같은색, 나머지 같은색
	//					들어갈수 있는 방과 없는방 구분..( 아이콘 번호 0( 공개 대기중 )과, 아이콘번호 1( 공개 진행방 )에만 참가가능 )
	//					Init( bool, bool, 3, TextColor1, TextColor2 )
	//					SetBackColors( BkColor1, BkColor2 ) 반드시 호출
	//				4 - 두가지 색으로 줄단위로 교대로
	//					Init( bool, bool, 4, TextColor1, TextColor2 )
	//					SetBackColors( BkColor1, BkColor2 ) 반드시 호출
	//
	//				pColor 는 2번 모드 일때만 사용
	//
	//				정렬사용,                헤데 색깔 사용                사용모드        참가가능한방       참가불가능한방
	void Init( BOOL bSortUse = false, BOOL bHeadColumnColorUse = false, int UserMode = 0, COLORREF crMan = 0, COLORREF crWoman = 0, COLORREF *pColor = NULL );

	void SortByColumn(int nColumn, BOOL bAscent);


	/////////////////

	struct ITEMINFO
	{
		COLORREF TextColor;
		COLORREF BackColor;
		BOOL	 bDrawShadow;
		COLORREF ShadowColor;
	};

	CArray<ITEMINFO, ITEMINFO>	m_ItemInfo;

	BOOL SetItemTextColor(int nItem, COLORREF color);
	BOOL SetItemTextBkColor(int nItem, COLORREF color);
	
	BOOL SetItemShadow(int nItem, BOOL bUse);
	BOOL SetItemShadowColor(int nItem, COLORREF color);

	void OnInsertItem( int nItem );
	void OnDeleteItem( int nItem );
	void OnDeleteAllItems();

	// 아이템 삽입, 삭제에 관련된 기존 함수들을 모두 오버라이딩한다
	int InsertItem( const LVITEM* pItem );
	int InsertItem( int nItem, LPCTSTR lpszItem );
	int InsertItem( int nItem, LPCTSTR lpszItem, int nImage );
	int InsertItem( UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam );
	BOOL DeleteItem( int nItem );
	BOOL DeleteAllItems( );

	///////////////////

	CListCtrlEx();
	virtual ~CListCtrlEx();

// Attributes
protected:
	BOOL m_bFullRowSel;
	BOOL m_bClientWidthSel;

public:

	void SetBackColors( COLORREF colorOdd, COLORREF colorEven );
	void SetHighlightColors( COLORREF colorBack, COLORREF colorText);
	void SetHeaderColors( COLORREF colorBack, COLORREF colorText );

	BOOL SetFullRowSel(BOOL bFillRowSel);
	BOOL GetFullRowSel();

// Overrides
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void RepaintSelectedItems();

// Implementation - list view colors
	COLORREF m_clrText, m_clrTextBk;
	COLORREF m_clrBkgnd;
	COLORREF m_clrBackOdd, m_clrBackEven;

    COLORREF m_colorTextHighlight, m_colorBackHighlight;


	CTitleTip m_titletip;

	afx_msg LRESULT OnSetTextColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetTextBkColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetBkColor(WPARAM wParam, LPARAM lParam);

	bool HitTestRowCol(CPoint& point, int& iRow, int& iColumn)const;
	CRect GetCellRect(int iRow, int iColumn)const;

	int  GetTrueColumnWidth(int nCurrentPosition)const;
	CString GetTrueItemText(int row, int col)const;
	void HideTitleTip();
	void SendSelChangedNotification();
	void ShowTitleTip(CPoint point);


// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
