// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "MyButton.h"

#pragma comment(lib, "winmm")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
	bEnable = TRUE;
	bCaptured=FALSE;
	bClicked=FALSE;
	nState = 1;

	bOneClip = FALSE;
	memset(Clip, 0, sizeof(Clip));

	bTransColor = FALSE;
	TransColor = 0;

	iUser1 = iUser2 = iUser3 = 0;
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_ENABLE()
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

BOOL CMyButton::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CMyButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	Draw(dc);
}

void CMyButton::Draw(CDC& dc) 
{
	//CPaintDC dc(this); // device context for painting
	CDC cdc;
	CRect rect;
	GetClientRect(&rect);


	if(bEnable == FALSE) // 버튼이 Disable된 경우
	{
		cdc.CreateCompatibleDC(&dc);
		if(cdc.m_hDC == NULL) return;
		if(bOneClip) {
			if(BmpOne.m_hObject==NULL) return;
			cdc.SelectObject(BmpOne.m_hObject);
			int c = 3;
			if(!Clip[c].bInit) c = 0;
			if(Clip[c].bInit) dc.BitBlt(0, 0, Clip[c].cx, Clip[c].cy, &cdc, Clip[c].x, Clip[c].y, SRCCOPY);
		}
		else {
			if(Bmp1.m_hObject==NULL && Bmp4.m_hObject==NULL) return;
			cdc.SelectObject((Bmp4.m_hObject!=NULL) ? &Bmp4 : &Bmp1);
			dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
		}
		return;
	}

	switch(nState)
	{
	case 1:	// 보통 상태
		cdc.CreateCompatibleDC(&dc);
		if(cdc.m_hDC == NULL) break;
		if(bOneClip) {
			if(BmpOne.m_hObject==NULL) return;
			cdc.SelectObject(BmpOne.m_hObject);
			int c = 0;
			if(Clip[c].bInit) dc.BitBlt(0, 0, Clip[c].cx, Clip[c].cy, &cdc, Clip[c].x, Clip[c].y, SRCCOPY);
		}
		else {
			if(Bmp1.m_hObject == NULL) break;
			cdc.SelectObject(&Bmp1);
			dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
		}
		break;

	case 2:	// 마우스 커서가 위를 지나는 상태(혹은 눌려진채로 밖으로 벗어나는경우)
		cdc.CreateCompatibleDC(&dc);
		if(cdc.m_hDC == NULL) break;
		if(bOneClip) {
			if(BmpOne.m_hObject==NULL) return;
			cdc.SelectObject(BmpOne.m_hObject);
			int c = 1;
			if(!Clip[c].bInit) c = 0;
			if(Clip[c].bInit) dc.BitBlt(0, 0, Clip[c].cx, Clip[c].cy, &cdc, Clip[c].x, Clip[c].y, SRCCOPY);
		}
		else {
			if(Bmp1.m_hObject == NULL && Bmp2.m_hObject == NULL) break;
			cdc.SelectObject((Bmp2.m_hObject!=NULL)? &Bmp2 : &Bmp1);
			dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
		}
		break;

	case 3:	// 마우스 커서가 눌린 상태
		cdc.CreateCompatibleDC(&dc);
		if(cdc.m_hDC == NULL) break;

		if(bOneClip) {
			if(BmpOne.m_hObject==NULL) return;
			cdc.SelectObject(BmpOne.m_hObject);
			int c = 2;
			if(!Clip[c].bInit) c = 1;
			if(!Clip[c].bInit) c = 0;
			if(Clip[c].bInit) dc.BitBlt(0, 0, Clip[c].cx, Clip[c].cy, &cdc, Clip[c].x, Clip[c].y, SRCCOPY);
		}
		else {
			if(Bmp3.m_hObject!=NULL) {
				cdc.SelectObject(&Bmp3);
				dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
			}
			else if(Bmp2.m_hObject!=NULL) {
				cdc.SelectObject(&Bmp2);
				dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
			}
			else {
				if(Bmp1.m_hObject == NULL) break;
				cdc.SelectObject(&Bmp1);
				dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
			}
		}
		break;
	}
	
	// Do not call CButton::OnPaint() for painting messages
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(bEnable == FALSE) return;

	if(!bCaptured) {
		if(SetCapture()) bCaptured=TRUE;
	}

	bClicked=TRUE;
	nState=3;
	Invalidate(FALSE);

	if(strWav.GetLength() > 0)
		PlaySound(strWav.operator LPCTSTR(), NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);

	//CButton::OnLButtonDown(nFlags, point);
}


void CMyButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(bEnable == FALSE) return;
	if(bCaptured) {
		ReleaseCapture();
		bCaptured=FALSE;
	}

	if(bClicked)
	{
		bClicked = FALSE;
		if(nState==2)
		{
			nState=1;
		}
		else if(nState==3)
		{
			nState=1;
			//(GetParent())->SendMessage(WM_COMMAND, GetDlgCtrlID(), (long)GetSafeHwnd());
			(GetParent())->PostMessage(WM_COMMAND, GetDlgCtrlID(), (long)GetSafeHwnd());
		}
		Invalidate(FALSE);
	}
	// CButton::OnLButtonUp(nFlags, point);
}


void CMyButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this); // device context for painting

	if(bEnable == FALSE) return;

	if(!bCaptured)
	{
		if(SetCapture()) bCaptured=TRUE;
	}
	
	CRect rc;
	GetClientRect(rc);

	if(!rc.PtInRect(point))
	{
		if(!bClicked)
		{
			if(ReleaseCapture())
			{
				bCaptured=FALSE;
				nState=1;
				Invalidate(FALSE);
			}
		}
		else
		{
			if(nState!=2)
			{
				nState=2;
				Invalidate(FALSE);
			}
		}
	}
	else
	{
		if(nState==1) { nState=2; Invalidate(FALSE);}
		if(nState!=3 && bClicked) { nState=3; Invalidate(FALSE);}
	}

	// CButton::OnMouseMove(nFlags, point);
}


void CMyButton::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//CButton::OnRButtonDown(nFlags, point);
}

void CMyButton::OnRButtonUp(UINT nFlags, CPoint point) 
{
	//CButton::OnRButtonUp(nFlags, point);
}

void CMyButton::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	//CButton::OnRButtonDblClk(nFlags, point);
}

void CMyButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//CButton::OnLButtonDblClk(nFlags, point);
}



void CMyButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	// CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyButton::Init(UINT bmp1, UINT bmp2, UINT bmp3, UINT bmp4, char* wavfile)
{
	if(bCaptured) 
		if(ReleaseCapture()) bCaptured=FALSE;

	bOneClip = FALSE;
	memset(Clip, 0, sizeof(Clip));

	if(Bmp1.m_hObject) { Bmp1.DeleteObject(); Bmp1.m_hObject=NULL;}
	if(Bmp2.m_hObject) { Bmp2.DeleteObject(); Bmp2.m_hObject=NULL;}
	if(Bmp3.m_hObject) { Bmp3.DeleteObject(); Bmp3.m_hObject=NULL;}
	if(Bmp4.m_hObject) { Bmp4.DeleteObject(); Bmp4.m_hObject=NULL;}

	if(bmp1) Bmp1.LoadBitmap(bmp1);
	if(bmp2) Bmp2.LoadBitmap(bmp2);
	if(bmp3) Bmp3.LoadBitmap(bmp3);
	if(bmp4) Bmp4.LoadBitmap(bmp4);

	strWav.Format("");
	if(wavfile != NULL) strWav.Format("%s", wavfile);

	// 첫번째 비트맵 사이즈에 맞춰서 버튼사이즈를 조절함
	if(GetSafeHwnd() && Bmp1.m_hObject) {
		BITMAP bm;
		::GetObject(Bmp1.m_hObject, sizeof(bm), &bm); // BMP의 가로 세로 사이즈를 얻는데만 씀
		if(bm.bmWidth>0 && bm.bmHeight>0) {
			// 윈도우의 위치와 Z오더는 무시하고 사이즈만 변경시킨다
			SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE | SWP_NOZORDER);
		}
	}
}

void CMyButton::Init(LPCTSTR filename, int imagenum, LPCTSTR wavfile)
{
	if(bCaptured) 
		if(ReleaseCapture()) bCaptured=FALSE;

	bOneClip = TRUE;
	memset(Clip, 0, sizeof(Clip));

	if(BmpOne.m_hObject) { BmpOne.DeleteObject(); BmpOne.m_hObject=NULL; }
	BmpOne.LoadBitmapFile(filename);

	strWav.Format("");
	if(wavfile != NULL) strWav.Format("%s", wavfile);

	// 비트맵 사이즈에 맞춰서 버튼사이즈를 조절함
	if(GetSafeHwnd() && BmpOne.m_hObject) {
		BITMAP bm;
		::GetObject(BmpOne.m_hObject, sizeof(bm), &bm); // BMP의 가로 세로 사이즈를 얻는데만 씀
		int h = bm.bmHeight/imagenum;
		if(bm.bmWidth>0 && h>0) {
			// 윈도우의 위치와 Z오더는 무시하고 사이즈만 변경시킨다
			SetWindowPos(NULL, 0, 0, bm.bmWidth, h, SWP_NOMOVE | SWP_NOZORDER);
		}

		for(int i=0; i<imagenum; i++) {
			Clip[i].bInit = TRUE;
			Clip[i].x = 0;
			Clip[i].y = h * i;
			Clip[i].cx = bm.bmWidth;
			Clip[i].cy = h;
		}
	}
}

void CMyButton::Init(int xp, int yp, LPCTSTR filename, int imagenum, LPCTSTR wavfile)
{
	Init(filename, imagenum, wavfile);
	if(GetSafeHwnd()) {
		// 윈도우의 사이즈와 Z오더는 무시하고 위치만 변경시킨다
		SetWindowPos(NULL, xp, yp, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

void CMyButton::SetTransColor(COLORREF color)
{
	bTransColor = TRUE;
	TransColor = color;
}

BOOL CMyButton::EnableWindow(BOOL enable)
{
	bEnable = enable;

	if(bCaptured) {
		ReleaseCapture();
		bCaptured=FALSE;
	}

	CButton::EnableWindow(bEnable);

	nState = 1;
	bClicked = FALSE;
	Invalidate(FALSE);

	return TRUE;
	//return CButton::EnableWindow(bEnable);
}

void CMyButton::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
}

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

	// 드로잉을 무시한다 (이함수를 오버라이딩하지 않으면 
	// Owner Draw 설정시 Debug Assert에 걸림)
}

void CMyButton::OnEnable(BOOL bEnable) 
{
	//CButton::OnEnable(bEnable);
	
	// TODO: Add your message handler code here
	// 무시한다(Enable/Disable시 깜빡임 방지)
}

void CMyButton::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CButton::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!bShow) {
		if(bCaptured) {
			ReleaseCapture();
			bCaptured=FALSE;
		}
	}	
}

BOOL CMyButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	Draw(*pDC);

	return TRUE;
	//return CButton::OnEraseBkgnd(pDC);
}
