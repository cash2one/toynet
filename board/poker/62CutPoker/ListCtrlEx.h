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


	//	�̹����� ������� ������� Init() ȣ������ ����
	//	bImageUse - ture �̸� �̹����� ���, false �̸� ������� �ʴ´�..
	//	iImageCnt - ����� Į����( = �̹��� ���� )
	//	clrColorKey - Į��Ű
	//	pImagePoint.x - �̹������� X ��ǥ, ImgPoint.y - �̹��� ����( Width ), �̹����� Y �� �׻� 0 �̴�
	//				Height �� 14 ���� ũ�� �ȵȴ�
	void Init( BOOL bImageUse = false, int iImageCnt = 0, COLORREF clrColorKey = 0, POINT *pImagePoint = NULL );

	void Release( void );

protected:
    COLORREF m_colorText, m_colorBack, m_colorBorder;


	//	��¥ ��ſ� �̹��� ��� ����
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
		CListCtrlEx		*pListCtrl;		// ����Ʈ ��Ʈ�� ������
		int				iSubItem;		// ��Ʈ�� ���� ������
	}LISTCTRL_COLUMN;

	typedef CListCtrl inherited;

	DECLARE_DYNCREATE(CListCtrlEx)

// Construction
protected:
	BOOL m_bHeadColumnColorUse;	//	����� �׸� ������
	BOOL m_bSortUse;	//	������ ��� �ϴ��� ����
	int m_UserMode;		//	��� ���
	COLORREF m_crMan, m_crWoman;

	BOOL m_bDrawFocusRect;	//	��Ŀ�� ���� �������� Focus Rectangle�� �׸��� ����
	

public:
    CHeaderCtrlEx		m_HeaderCtrl;

	//char m_UserNum_3000[ 3000 ];
	bool m_bSortType;

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	void	SetUserStep( int Num, char State );

	//	���ݸ�� ���� �Ұ����� false �̸� �⺻ ���
	void	SetScrollBarChange( BOOL bExChange = false );
	//	�׸������ ��� �Ұ�����
	void	SetGridLineUse( BOOL bGridLineUse = false );
	//	��Ŀ�� ���� �������� ������ �׸������� ����
	void	SetDrawFocusRect( BOOL bUse = true );


	//	����� Į�� ������ �ȵ�
//	CHeaderCtrlEx	*GetHeaderCtrl( void );



	//	UserMode : 0 �⺻, 1 �� ����� �۾��� ����, 2�� ĳ���� �ܰ躰 ���,
	//				3�� 4���� ���Į��2�� �۾�Į�� 2�� ���
	//
	//				0 - �⺻ ��Ʈ��.. ���� �۾��� ���� 1���� ���.
	//					Init( bool, bool, 0, BkColor( = TextBkColor ), TextColor ) )
	//					Init() �Լ� ȣ�� ���ϸ� 
	//						- SetBackColors( BkColor, TextBkColor ), SetTextColor() ȣ���ϰų�
	//						- SetBkColor(), SetTextBkColor(), SetTextColor()�� ȣ���Ұ�
	//				1 - ĳġ���ε��, ���� 1, �۾����� 2( ��, �� )�� ��� 
	//						SetBackColors( BkColor, TextBkColor )�� ȣ���ϰų� SetBkColor(), SetTextBkColor() �� ȣ��
	//				2 - ĳġ���ε��, ���� 1, �ܰ躰 Į�� ���( 6���� )
	//					Init( bool, bool, 2, BkColor, TextBkColor, *pColor )
	//						SetBkColor(), SetTextBkColor() �� ȣ��( ���ص� �� )
	//				3 - ������ ��ȣ 0, 1 ���� ������, ������ ������
	//					���� �ִ� ��� ���¹� ����..( ������ ��ȣ 0( ���� ����� )��, �����ܹ�ȣ 1( ���� ����� )���� �������� )
	//					Init( bool, bool, 3, TextColor1, TextColor2 )
	//					SetBackColors( BkColor1, BkColor2 ) �ݵ�� ȣ��
	//				4 - �ΰ��� ������ �ٴ����� �����
	//					Init( bool, bool, 4, TextColor1, TextColor2 )
	//					SetBackColors( BkColor1, BkColor2 ) �ݵ�� ȣ��
	//
	//				pColor �� 2�� ��� �϶��� ���
	//
	//				���Ļ��,                �쵥 ���� ���                �����        ���������ѹ�       �����Ұ����ѹ�
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

	// ������ ����, ������ ���õ� ���� �Լ����� ��� �������̵��Ѵ�
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
