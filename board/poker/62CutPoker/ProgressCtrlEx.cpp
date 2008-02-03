// ProgressCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ProgressCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressCtrlEx

CProgressCtrlEx::CProgressCtrlEx()
{
	m_nCurrentPosition = 0;
	MaxNum = 0;
}

CProgressCtrlEx::~CProgressCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CProgressCtrlEx, CProgressCtrl)
	//{{AFX_MSG_MAP(CProgressCtrlEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressCtrlEx message handlers

void CProgressCtrlEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
	
	CRect rectFill;			   // Rectangle for filling band
	CRect rectClient;
	CBrush brush,brush1;
	GetClientRect(&rectClient);

	COLORREF tempcolor = m_clrBarColor & 0x00010101;
	
	////////////BackGround
	::SetRect(&rectFill, 0, 0, rectClient.right, rectClient.bottom);
	VERIFY(brush.CreateSolidBrush(m_clrBkGround));
	dc.FillRect(&rectFill, &brush);
	VERIFY(brush.DeleteObject());	
	
	if( MaxNum <= 0)return;
	//progress bar	
	float maxWidth = rectClient.right;
	float block = (rectClient.Width()-((MaxNum-2)*2)) / MaxNum;		
	rectFill.SetRect(0,1,block,rectClient.Height()-1);	
	rectFill.OffsetRect(2,0);
/*
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	bar.LoadBitmapFile(".\\image\\vote\\time.BMP");

	CBitmap *pOldBmp = cdc.SelectObject(&bar);

	int w = rectClient.Width();	
	int num =0;
	num = (block * ((MaxNum - m_nCurrentPosition)+1)); 

//	dc.StretchBlt(0,0,10,13,&cdc,0,0,bar.GetWidth(),13,SRCCOPY);	
	dc.StretchBlt(0,0,w-num,13,&cdc,0,0,bar.GetWidth(),13,SRCCOPY);	

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	*/
	for(int i = 0 ; i < m_nCurrentPosition ; i ++){		
		VERIFY(brush.CreateSolidBrush(m_clrBarColor));
		dc.FillRect(&rectFill,&brush);
		VERIFY(brush.DeleteObject());	
		rectFill.OffsetRect(2+block,0);
	}	
}

void CProgressCtrlEx:: SetRangeEx(int start, int max , int mode)
{
	start = start;MaxNum = max;
	m_nCurrentPosition = start;
	mode = mode;
}

int CProgressCtrlEx::SetPosEx(int npos)
{
	m_nCurrentPosition = npos;
	return (CProgressCtrl::SetPos(npos));
}

BOOL CProgressCtrlEx::OnEraseBkgnd(CDC* pDC) 
{	
	return TRUE;
}
