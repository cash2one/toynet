// AskBuyItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "AskBuyItemDlg.h"

#include "MyBitmap.h"
#include "SysUtil.h"

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 배경 이미지 파일명
static const LPCTSTR BACKIMAGE_NAME = "image\\gameitem\\AskBuyItemBack.bmp";

/////////////////////////////////////////////////////////////////////////////
// CAskBuyItemDlg dialog


CAskBuyItemDlg::CAskBuyItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAskBuyItemDlg::IDD, pParent)
{
	// NULL브러쉬 생성
	m_HollowBrush.CreateStockObject(HOLLOW_BRUSH);

	m_ItemCode = 0;

	//{{AFX_DATA_INIT(CAskBuyItemDlg)
	m_strMsg = _T("");
	//}}AFX_DATA_INIT
}


void CAskBuyItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAskBuyItemDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Text(pDX, IDC_STATIC_MSG, m_strMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAskBuyItemDlg, CDialog)
	//{{AFX_MSG_MAP(CAskBuyItemDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAskBuyItemDlg message handlers

BOOL CAskBuyItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 배경 비트맵 사이즈에 맞게 윈도우 사이즈 조정
	WindowResize2Bitmap(m_hWnd, BACKIMAGE_NAME);

	// 버튼 초기화
	m_OkBtn.Init("image\\gameitem\\btn_yes.bmp", 4);
	m_CancelBtn.Init("image\\gameitem\\btn_no.bmp", 4);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAskBuyItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CAskBuyItemDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CAskBuyItemDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CMyBitmap Back;
	Back.LoadBitmapFile(BACKIMAGE_NAME);
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	// 폰트 생성
	CFont Font1;
	Font1.CreateFont(12,0,0,0,
		FW_NORMAL,
		//FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	dc.SetBkMode(TRANSPARENT);

	CFont* pOldFont = dc.SelectObject(&Font1);

	// 아이템과 기능설명을 그림
	DrawGameItem(dc);
	
	cdc.SelectObject(pOldFont);
	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Font1.DeleteObject();
	Back.DeleteObject();

	// Do not call CDialog::OnPaint() for painting messages
}

void CAskBuyItemDlg::DrawGameItem(CDC& dc)
{
	CFont Font1;
	Font1.CreateFont(12,0,0,0,
		FW_NORMAL,
		//FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	CFont Font2;
	Font2.CreateFont(12,0,0,0,
		//FW_NORMAL,
		FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	// 아이템 이미지 찍기
	CMyBitmap bmp;
	bmp.LoadBitmapFile(GameItem_GetItemImageFile(m_ItemCode));
	CSize bmpsize = bmp.GetBitmapDimension();
	bmp.TransDraw(dc.m_hDC, 22+6, 35+8, bmpsize.cx, bmpsize.cy, 0, 0, RGB(255,0,255));
	bmp.DeleteObject();

	CString str;
	CRect rect;

	// 아이템 상태 찍기
	dc.SelectObject(&Font2);
	dc.SetTextColor(RGB(0,80,100));

	int ax = 127, ay = 40;

	// 아이템 이름 찍기
	str.Format("◆ %s", GameItem_GetItemName(m_ItemCode));
	dc.TextOut(ax, ay, str); ay += 20;

	// 아이템 설명 찍기
	dc.SelectObject(&Font1);
	rect.SetRect(0,0,225,70);
	rect.OffsetRect(ax, ay);
	str.Format("◆ %s", GameItem_GetItemHelp(m_ItemCode));
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK); ay += 20;
}


HBRUSH CAskBuyItemDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
		HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) {
		case CTLCOLOR_STATIC:
			// The Slider Control has CTLCOLOR_STATIC, but doesn't let
			// the background shine through,
			pDC->SetBkMode(TRANSPARENT);
			//pDC->SetTextColor(RGB(255,255,255));
			pDC->SetTextColor(RGB(0,80,100));
			TCHAR lpszClassName[255];
			GetClassName(pWnd->m_hWnd, lpszClassName, 255);
			if(_tcscmp(lpszClassName, TRACKBAR_CLASS) == 0)
				return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

		case CTLCOLOR_BTN:
			// let static controls shine through
			pDC->SetBkMode(TRANSPARENT);
			return HBRUSH(m_HollowBrush);

		default:
			break;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CAskBuyItemDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}
