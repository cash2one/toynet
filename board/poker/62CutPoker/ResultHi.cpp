// ResultHi.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "ResultHi.h"
#include "SysUtil.h"

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultHi dialog


CResultHi::CResultHi(CWnd* pParent /*=NULL*/)
	: CDialog(CResultHi::IDD, pParent)
{
	ZeroMemory(&UI, sizeof(UI));
	ZeroMemory(&pGO, sizeof(GAMEOVERRESULT));
	TimeCnt = 0;
	hTimer = NULL;
	WinCase = 0;
	m_Str1 = "";
	m_Str2 = "";
	m_winner_kind = 0;

	m_nResultBtnIndex = 0;
	
	m_nItemByeUse = 0; // [수호천사] 2004.07.08

	ZeroMemory(m_aCard, sizeof(m_aCard));
	ZeroMemory(m_aSwing, sizeof(m_aSwing));
	
	//{{AFX_DATA_INIT(CResultHi)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CResultHi::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResultHi)
	DDX_Control(pDX, ID_MINIGAME, m_MiniGame);
	DDX_Control(pDX, IDC_BUTTON_SAFEANGEL, m_SafeBtn);
	DDX_Control(pDX, IDC_BUTTON_SHOWCARD, m_ShowCardBtn);
	DDX_Control(pDX, IDOK, m_CloseBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResultHi, CDialog)
	//{{AFX_MSG_MAP(CResultHi)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SHOWCARD, OnButtonShowcard)
	ON_BN_CLICKED(IDC_BUTTON_SAFEANGEL, OnButtonSafeangel)
	ON_BN_CLICKED(ID_MINIGAME, OnMinigame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultHi message handlers

BOOL CResultHi::Init(USERINFO *pUI, GAMEOVERRESULT *pGOR, int wincase, int pnum)
{
	if(pUI == NULL || pGOR == NULL) return FALSE;

	memcpy(&UI, pUI, sizeof(UI));
	memcpy(&pGO, pGOR, sizeof(GAMEOVERRESULT));
	WinCase = wincase;
	if(WinCase == 1)
	{
		m_Str1 = Game.GetName(pnum, TRUE);
		memcpy(m_aCard, Game.m_CardHi, sizeof(int)*5);
	}
	
	
	return TRUE;
}

BOOL CResultHi::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	//m_ShowCardBtn.Init(80,226,".\\image\\commonbtn\\Btn_ok.bmp",4);
	//m_ShowCardBtn.ShowWindow(SW_HIDE);

	// 배경 그림 읽기
	if(Play[0].ServPNum == Game.WinnerPNum) {
		Back.LoadBitmapFile(".\\image\\Gameover\\result_mywin.bmp");
		m_winner_kind = 1;

		// 캐릭터 인덱스0 저장(결과창) - jeong
		m_CharView.CharNum = g_pGameView->CharBox[0].m_nCharIndex[0];

		// 저장 Bank Money(증감) - jeong
		Play[0].BankMoney = Play[0].BankMoney + pGO.nWinMoney;

		// 이긴 금액 세팅 - jeong
		Play[0].nWinMoney = pGO.nWinMoney;

		m_nResultOffSetX[0] = 100;
		m_nResultOffSetX[1] = 190;

		m_nResultOffSetY[0] = 255;
		m_nResultOffSetY[1] = 255;

		m_bWin = TRUE;
	}
	else {
		Back.LoadBitmapFile(".\\image\\Gameover\\result_otherwin.bmp");		// Result design will be changed - jeong
		m_winner_kind = 2;

		// 캐릭터 인덱스1 저장(결과창) - jeong
		m_CharView.CharNum = g_pGameView->CharBox[0].m_nCharIndex[1];

		// 잃은 금액 저장 - jeong
		Play[0].LoseMoney = 0;
		int pnum = Play[0].ServPNum;
		int nBetMoney = g_Poker.RU[pnum].nRealBat;
		int nTotalMoney = Play[0].PrevMoney+ Play[0].BankMoney;
		if( nBetMoney > 0 )
		{
			if( nTotalMoney > 0)
			{
				CSV_ASK_MONEYINFO aumsg;
				aumsg.Set(Play[0].UI.UNum, nBetMoney, g_RI.RoomNum);
				SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());
			}
		}
		else if( nBetMoney == 0 )
		{
			if ( Play[0].UI.PMoney < 0 )
			{
				CSV_ASK_MONEYINFO aumsg;
				aumsg.Set(Play[0].UI.UNum, -Play[0].UI.PMoney, g_RI.RoomNum);
				SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());
				
			}
		}



		// [수호천사] 2004.07.08 
		// 현재 시간을 얻어서 서버의 시간으로 변환
		CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
		SYSTEMTIME st;
		time.GetAsSystemTime(st);
		BOOL bExist = FALSE;
//				   GameItem_IsHaveItem(g_MyGameItem, ITEM_SCODE_SAFEGUARDANGEL);
		BOOL rtn = GameItem_IsUsingItem(g_MyGameItem, ITEM_SCODE_SAFEGUARDANGEL, &bExist, &st);
		if( bExist ) {
			m_nItemByeUse = 1;
			//m_SafeBtn.Init(238,290,".\\image\\GameOver\\Btn_Use.bmp",4); // 사용하기
		}
		else {
			m_nItemByeUse = 2;
			//m_SafeBtn.Init(238,290,".\\image\\GameOver\\Btn_Bye.bmp",4); // 구매하기
	
		}

		m_nResultOffSetX[0] = 140;
		m_nResultOffSetX[1] = 140;

		m_nResultOffSetY[0] = 355;
		m_nResultOffSetY[1] = 355;

		m_bWin = FALSE;
	}

	// 폰트 생성
	if(Font1.m_hObject == NULL)
	Font1.CreateFont(12,0,0,0,
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

	if(Font2.m_hObject == NULL)
	Font2.CreateFont(12,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	int xSize = 59;
	int ySize = 99;

	if(Play[0].ServPNum == Game.WinnerPNum)
		Page.Init(347, 297, 16);
	else
		Page.Init(347, 406, 16);


	// 버튼 초기화
	if(Play[0].ServPNum == Game.WinnerPNum)
	{
		OkBtn.Init(this, &Page, 90, 251, &OkBtnSpr, 0,IDM_RESULT_OK);
		OkBtn.Show(TRUE);
		OkBtn.m_Width = 20;
		OkBtn.m_Height = 44;
	
		BonusGBtn.Init(this, &Page, 180, 251, &BonusGBtnSpr, 0,IDM_RESULT_BONUS);
		BonusGBtn.Show(TRUE);
		BonusGBtn.m_Width = 100;
		BonusGBtn.m_Height = 44;
	}
	else
	{
		OkBtn.Init(this, &Page, 135, 350, &OkBtnSpr, 0,IDM_RESULT_OK);
		OkBtn.Show(TRUE);
		OkBtn.m_Width = 20;
		OkBtn.m_Height = 44;
	}

	m_bResultBtnMouseDown = FALSE;


	// 캐릭터 뷰 초기화
	CRect rc;
	rc.SetRect(0,0,75,125);
	rc.OffsetRect(22,61);
	//m_CharView.Create(this, rc, 5, 3224);

	hTimer = SetTimer(RESULTHI_TIMER, 55, NULL);
	TimeCnt = 0;

	// [수호천사] 2004.07.08 
	//if(m_nItemByeUse == 0) TimeCnt = 3 ;
	//else TimeCnt = 5;

	// 승패결과창 대기시간 - jeong
	TimeCnt = MAX_WAIT_RESULT*20;


	WindowResize2Bitmap(m_hWnd, Back);

	CenterWindow(g_pGameView->GetOwner());	

	
	((C62CutPokerDlg *)AfxGetMainWnd())->m_hSubWindow = m_hWnd;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResultHi::OnOK() 
{
	// TODO: Add extra validation here
	Sound.Play(SND45);
	
	CDialog::OnOK();
}

void CResultHi::OnCancel() 
{
	// TODO: Add extra cleanup here
	OnOK();
	//CDialog::OnCancel();
}

void CResultHi::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	
	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	MemDC.SelectObject(Page.hBmp);
	
	// 버튼 출력 및 배경 출력
	if(!m_bWin)
	{
		Page.PutSprAuto(0, 0, &ResultLoseSpr, 0);
		OkBtn.Draw(&MemDC);
	}
	else
	{
		Page.PutSprAuto(0, 0, &ResultWinSpr, 0);
		OkBtn.Draw(&MemDC);
		BonusGBtn.Draw(&MemDC);
	}

	// 아바타 출력
	Page.PutSprAuto(22, 61, &AvaSpr, m_CharView.CharNum);

	// 텍스트 출력
	CString str ="";
	CString strM="";
	int ax, ay;

	MemDC.SelectObject(&Font2);
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(0,0,0));
	
	if(WinCase == 0)
	{
		ax = 109, ay = 183;
		strM = NumberToOrientalString(pGO.nWinMoney);
		str.Format("+%s",strM );
		CRect rt;

		if(!m_bWin)
			rt.SetRect(117,217,117+210,217+17);
		else
			rt.SetRect(114,207,114+210,207+17);
		
		
		MemDC.DrawText(str, &rt, DT_RIGHT | DT_WORDBREAK);

		if(!m_bWin)
		{
			CRect rt;
			CString str1,str2;
			rt.SetRect(117,244,117+210,244+17);

			INT64 roundingoff = 200;

			str2 = NumberToOrientalString(roundingoff);
			str1.Format("-%s",str2);
			MemDC.DrawText(str1, &rt, DT_RIGHT | DT_WORDBREAK);
		}


		// 카드 5장 그리기
		int px = 149;
		int py = 88;

		for(int i = 0; i < 5; i++) 
			Page.PutSprAuto(px+i*18, py, &BigCardSpr, 52);
	}
	else if(WinCase == 1)
	{
		ax = 109, ay = 183;
		strM = NumberToOrientalString(pGO.nWinMoney);
		str.Format("+%s",strM );
		CRect rt;

		if(!m_bWin)
			rt.SetRect(117,217,117+210,217+17);
		else
			rt.SetRect(114,207,114+210,207+17);
		
		

		MemDC.DrawText(str, &rt, DT_RIGHT | DT_WORDBREAK);

		if(!m_bWin)
		{
			CRect rt;
			CString str1,str2;
			rt.SetRect(117,244,117+210,244+17);

			INT64 roundingoff = 200;

			str2 = NumberToOrientalString(roundingoff);
			str1.Format("-%s",str2);
			MemDC.DrawText(str1, &rt, DT_RIGHT | DT_WORDBREAK);


		}

		// 카드 5장 그리기
		int px = 149;
		int py = 88;

		for(int i = 0; i < 5; i++) 
		{
			Page.PutSprAuto(px+i*18, py, &BigCardSpr, m_aCard[i]);
		}
		
	//	m_Str1 = "로얄 스트레이트 양아치 2";
		ax = 139;
		ay = 68;			// 족보 결과출력 좌표 - modifed by jeong
		CRect rect;
		rect.SetRect(0,0,160,14);
		if(m_Str1!="")
		{
			MemDC.SetTextColor(RGB(0,0,0));
			rect.OffsetRect(ax, ay);
			MemDC.DrawText(m_Str1, &rect, DT_CENTER | DT_WORDBREAK);

		}
	}


	CRect rect;
	GetClientRect(&rect);
	
	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);

	MemDC.DeleteDC();

	OkBtn.OnMouseMove( m_nResultOffSetX[m_nResultBtnIndex], m_nResultOffSetY[m_nResultBtnIndex] );
	BonusGBtn.OnMouseMove( m_nResultOffSetX[m_nResultBtnIndex], m_nResultOffSetY[m_nResultBtnIndex] );
	
	short mxp, myp;
	if( m_bResultBtnMouseDown )
	{
		m_bResultBtnMouseDown = FALSE;
		
		mxp = m_nResultOffSetX[m_nResultBtnIndex];
		myp = m_nResultOffSetY[m_nResultBtnIndex];
		
		OkBtn.OnLButtonDown(mxp, myp);
		BonusGBtn.OnLButtonDown(mxp, myp);
		
		OkBtn.OnLButtonUp(mxp, myp);
		BonusGBtn.OnLButtonUp(mxp, myp);
	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CResultHi::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
	Font1.DeleteObject();
	Font2.DeleteObject();
	
	if(hTimer)
	{
		KillTimer(hTimer);
		hTimer = NULL;
	}
	
}


void CResultHi::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CResultHi::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == hTimer)
	{
		TimeCnt--;
	
		if(TimeCnt < 1) {
			
				KillTimer(hTimer);
				OnCancel();
		}
		
		if(Game.bGameStart || g_RI.State == 1 || g_Where != WH_GAME)
		{
			KillTimer(hTimer);
			OnCancel();
		}
	}

	Invalidate(FALSE);

	CDialog::OnTimer(nIDEvent);
}

void CResultHi::OnButtonShowcard() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);


		
	CDialog::OnOK();
}

void CResultHi::OnButtonSafeangel() 
{
	// TODO: Add your control notification handler code here
	if( m_nItemByeUse == 1)
	{
		/*
		g_pMainDlg->m_GameItemDlg.m_ItemListDlg.SetItemList( g_MyGameItem.nNum, g_MyGameItem.Item );
		g_pMainDlg->m_GameItemDlg.DoModaless(1, FALSE, ITEM_SCODE_SAFEGUARDANGEL);
		if( g_pMainDlg->m_GameItemDlg.m_ItemListDlg.SelectUsableGameItem( ITEM_SCODE_SAFEGUARDANGEL ) ) {
			g_pMainDlg->m_GameItemDlg.m_ItemListDlg.UseSelectedGameItem();
		}
		*/
		// 현재 시간을 얻어서 서버의 시간으로 변환
		CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
		SYSTEMTIME st;
		time.GetAsSystemTime(st);
		for(int i=0; i<g_MyGameItem.nNum; i++) {
			ItemState state = GameItem_GetItemState(g_MyGameItem.Item[i], &st);
			if(state.iState == 1) {
				if(ITEM_SCODE_SAFEGUARDANGEL == GameItem_GetItemKind(g_MyGameItem.Item[i].Code) ) {		
					CSV_USEGAMEITEM ugmsg;
					USEGAMEITEMINFO ug;
					strncpy(ug.ID, (char*)(LPCTSTR)g_MyInfo.UI.ID, 15);
					ug.Item = g_MyGameItem.Item[i];
					ugmsg.Set(&ug);
					SockMan.SendData(g_MainSrvSID, ugmsg.pData, ugmsg.GetTotalSize());
					break;
				}
			}
		}
	} 
	else if(m_nItemByeUse == 2)
	// ( 구입하기면 )
	{
	//	g_pMainDlg->m_GameItemDlg.m_ItemListDlg.SetItemList( g_MyGameItem.nNum, g_MyGameItem.Item );					
		g_pMainDlg->m_GameItemDlg.m_ItemListDlg.m_StrMsg = "";
//		g_pMainDlg->m_GameItemDlg.m_ItemListDlg.SetItemList( g_MyGameItem.nNum, g_MyGameItem.Item );
		g_pMainDlg->m_GameItemDlg.DoModaless( 2, TRUE, ITEM_SCODE_SAFEGUARDANGEL );
	}
	CDialog::OnOK();
}

void CResultHi::OnMinigame() 
{
	// TODO: Add your control notification handler code here
	//Sound.Play(SND45);	
	Sound.Play(SND_FBONUS);	

	//if( Game.m_bShowMiniGame )
	if( Play[0].ServPNum == Game.WinnerPNum )
		g_Mini.DoModal();

	OnOK();
}

BOOL CResultHi::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(pMsg->message == WM_KEYDOWN) 
	{

		if(pMsg->wParam == VK_LEFT)
		{
			m_nResultBtnIndex--;
			if( m_nResultBtnIndex < 0)
				m_nResultBtnIndex = 1;
		}
		else if(pMsg->wParam == VK_RIGHT)
		{
			m_nResultBtnIndex++;
			if( m_nResultBtnIndex > 1 )
				m_nResultBtnIndex = 0;

		}
		else if(pMsg->wParam == VK_DOWN)
			m_bResultBtnMouseDown = TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CResultHi::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(wParam)
	{
		case IDM_RESULT_OK:
		{
			OnOK();
		}
		break;
			
		case IDM_RESULT_BONUS:
		{
			OnMinigame();
		}
		break;
	}
		
	return CDialog::OnCommand(wParam, lParam);
}





















