// LevelView.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "LevelView.h"

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLevelView

CLevelView::CLevelView()
{
	UseMode = 0;
	JackPotMoney = 0;
//	AX =204; +10
//	AY = 3; +15
	AX =214;
	AY = 18;
	m_bFirst = FALSE;
	m_bSecond = FALSE;
	m_nLength = 0;
	m_nFirstXp	= 0;
	m_nSecondXp = 0;

	ZeroMemory(m_jackpot_digit, sizeof(m_jackpot_digit));
	m_digit_cnt = 0;
}

CLevelView::~CLevelView()
{
}


BEGIN_MESSAGE_MAP(CLevelView, CWnd)
	//{{AFX_MSG_MAP(CLevelView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLevelView message handlers

void CLevelView::Create(CWnd *pParentWnd, CRect &rect, int usemode, int nID)
{
	Page.Init(rect.Width(), rect.Height(), 16);
	UseMode = usemode;
	CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID);
	ShowWindow(SW_SHOW);
	SetTimer( LEVELVIEW_TIMER, 50, NULL );
}

void CLevelView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	Invalidate( FALSE );
	CWnd::OnTimer(nIDEvent);
}

void CLevelView::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer( LEVELVIEW_TIMER );	
	
}

void CLevelView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(Page.lpBit == NULL) return;
	// 더블 버퍼링용 메모리 DC 생성
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	MemDC.SelectObject(Page.hBmp);
	MemDC.SetBkMode(TRANSPARENT);
	

	Page.PutSprAuto(0, 0, &JackPotSpr, 14);
	Page.PutSprAuto(10,15, &JackPotBackSpr, 0);

	// [ ###잭팟### ] 
	INT64 minus = g_TmpJackPotMoney - g_JackPotMoney;
	INT64 plus = 0;
	if(g_TmpJackPotMoney > g_JackPotMoney) {
		if(		minus > 10000000000)plus = 987654321;
		else if(minus > 1000000000) plus = 98765431;
		else if(minus > 100000000)  plus = 9876541;
		else if(minus > 10000000)   plus = 987651;
		else if(minus > 1000000)    plus = 98761;
		else if(minus > 100000)	    plus = 9871;
		else if(minus > 10000)	    plus = 981;
		else if(minus > 1000)       plus = 501; 
		else if(minus > 50 )        plus = 49;
		else plus = 1;

		g_JackPotMoney+=plus; // 증가 보여주기
	}
	else  if(g_TmpJackPotMoney < g_JackPotMoney) {
		if(		minus < -100000000000000000) plus = 9999999987654321;
		else if(minus < -10000000000000000)  plus = 999999987654321;
		else if(minus < -1000000000000000)   plus = 99999987654321;
		else if(minus < -100000000000000)    plus = 9999987654321;
		else if(minus < -10000000000000)     plus = 999987654321;
		else if(minus < -1000000000000)      plus = 99987654321;
		else if(minus < -100000000000)       plus = 9987654321;
		else if(minus < -10000000000)        plus = 997654321;
		else if(minus < -1000000000)         plus = 99765431;
		else if(minus < -100000000)	         plus = 9976541;
		else if(minus < -10000000)           plus = 997651;
		else if(minus < -1000000)            plus = 99761;
		else if(minus < -100000)	         plus = 9971;
		else if(minus < -10000)	             plus = 991;
		else if(minus < -1000)               plus = 91; 
		else if(minus < -50 )                plus = 9;
		else plus = 1;
		g_JackPotMoney-=plus; // 증가 보여주기
	}
	INT64 tmp = g_JackPotMoney;


	get_jackpotmoney_digit(tmp);
	draw_digit(m_nFirstXp);

//	set_draw_pos();
//	draw_digit(m_nFirstXp);
//	draw_digit(m_nFirstXp+m_digit_cnt*10);
	//DrawFirst(tmp);
	//DrawSecond(tmp);



	//MemDC.SelectObject(&g_pGameView->Font2);
	//MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetTextColor(RGB(255,255,0));
	//MemDC.TextOut(Xp+30, Yp, "☆잭팟 상금☆");
	//CString str;
	//str.Format("blueiimarble 님이 잭팟 %I64d 당첨되셨습니다",54515151);
	//MemDC.TextOut(Xp+150, Yp, str);


	
	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);
		
	MemDC.DeleteDC();

	//	Page.FlipToGDI(dc.m_hDC);
	// Do not call CWnd::OnPaint() for painting messages
}

void CLevelView::SetMoney(INT64 money)
{
	JackPotMoney = money;
	Invalidate(FALSE);
}

int CLevelView::DrawJackPot(int sx, int sy,INT64 Money)
{

	return 0;
}

void CLevelView::DrawFirst(INT64 Money)
{

	int sx = AX;
	int sy = AY;

	
	int kk = 0;
	int count = 0;
	int addx = 0; // 컴마
	for( int j = 0; j < 20 ; j ++ ) {
		if( Money <= 0 ) {
			Page.PutSprAuto( sx - ((j * 9)+(count*8)+9), sy, &JackPotSpr,11 );
			m_nLength = ((j * 9)+(count*8)+9);
			break;
		}
		if(kk >= 3)	{
			kk = 0;
			Page.PutSprAuto( sx + addx - j * 9, sy, &JackPotSpr,10 );
			addx -= 8;
			count++;
		}
		Page.PutSprAuto( sx + addx - j * 9, sy, &JackPotSpr,(int)(Money%(INT64)10) );
		Money = Money / 10;
		kk++;
	}
}

void CLevelView::DrawSecond(INT64 Money)
{
	int sx = AX;
	int sy = AY;
	int value = 0;
	int kk = 0;
	int count = 0;
	int addx = 0; // 컴마
	for( int j = 0; j < 20 ; j ++ ) {
		if( Money <= 0 ) {
			Page.PutSprAuto( sx - ((j * 9)+(count*8)+9), sy, &JackPotSpr,11 );
			break;
		}
		if(kk >= 3)	{
			kk = 0;
			Page.PutSprAuto( sx + addx - j * 9, sy, &JackPotSpr,10 );
			addx -= 8;
			count++;
		}
		Page.PutSprAuto( sx + addx - j * 9, sy, &JackPotSpr,(int)(Money%(INT64)10) );
		Money = Money / 10;
		kk++;
	}
}

void CLevelView::get_jackpotmoney_digit(INT64 Money)
{
	ZeroMemory(m_jackpot_digit, sizeof(m_jackpot_digit));
	m_digit_cnt = 0;

	int kk = 0;
	int addx = 0; // 컴마
	for( int j = 0; j < 20 ; j ++ ) {
		if( Money <= 0 ) {
			m_jackpot_digit[m_digit_cnt]=-2;
			break;
		}
		if(kk >= 3)	{
			kk = 0;
			m_jackpot_digit[m_digit_cnt] = -1;
			m_digit_cnt++;
		}
		m_jackpot_digit[m_digit_cnt]=(int)(Money%(INT64)10);
		m_digit_cnt++;
		Money = Money / 10;
		kk++;
	}
}

void CLevelView::draw_digit(int startx)
{

	int sx = AX;startx;
	int sy = AY;
	int tmp = 0;
	int i=0;
	static int cnt = 0;

	
	Page.PutSprAuto(0, 0, &JackPotSpr, 14+ (cnt%7));
	Page.PutSprAuto(10,15, &JackPotBackSpr, 0);

	cnt++;
	if(cnt >= 7)cnt = 0;

	for( i=0; i<m_digit_cnt+1;i++)
	{
		tmp = m_jackpot_digit[i];
		if(tmp == -1)
			Page.PutSprAuto( sx  - i * 9, sy, &JackPotSpr, 10 );
		else if(tmp == -2)
			Page.PutSprAuto( sx  - 10 - i * 9, sy, &JackPotSpr, 11 );
		else
			Page.PutSprAuto( sx  - i * 9, sy, &JackPotSpr, tmp );
	}
/*

	int cnt=0;
	for( i=m_digit_cnt; i>=0;i--)
	{
		tmp = m_jackpot_digit[i];
		if(tmp == -1)
			Page.PutSprAuto( sx  + cnt * 9, sy, &JackPotSpr, 10 );		// ,
		else if(tmp == -2)
			Page.PutSprAuto( sx  - 10 + cnt * 9, sy, &JackPotSpr, 11 ); // $
		else
			Page.PutSprAuto( sx  + cnt * 9, sy, &JackPotSpr, tmp );     // NUMBER

		cnt++;
	}
*/
	/*
	int sx = AX;
	int sy = AY;
	
	int kk = 0;
	int count = 0;
	int addx = 0; // 컴마
	for( int j = 0; j < 20 ; j ++ ) {
		if( Money <= 0 ) {
			Page.PutSprAuto( sx - ((j * 9)+(count*8)+9), sy, &JackPotSpr,11 );
			m_nLength = ((j * 9)+(count*8)+9);
			break;
		}
		if(kk >= 3)	{
			kk = 0;
			Page.PutSprAuto( sx + addx - j * 9, sy, &JackPotSpr,10 );
			addx -= 8;
			count++;
		}
		Page.PutSprAuto( sx + addx - j * 9, sy, &JackPotSpr,(int)(Money%(INT64)10) );
		Money = Money / 10;
		kk++;
	}
*/
}

void CLevelView::set_draw_pos()
{
	m_nFirstXp--;
	m_nSecondXp--;
	if(m_nFirstXp < -222) m_nFirstXp=222+m_digit_cnt*10;
	if(m_nSecondXp < -222-m_digit_cnt*10) m_nSecondXp=222+222+m_digit_cnt*10;
	
}
