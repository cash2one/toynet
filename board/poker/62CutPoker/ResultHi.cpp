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
	
	m_nItemByeUse = 0; // [��ȣõ��] 2004.07.08

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
	
	
	/*
	else if(WinCase == 2)
	{
		m_Str2 = Game.GetLowName(pnum);
		memcpy(m_aCard, Game.m_CardLow, sizeof(int)*5);
	}
	else if(WinCase == 3) // 7 �� �����ش�.
	{
			m_Str1 = Game.GetName(pnum);
			m_Str1 += Game.GetLowName(pnum);
			memcpy(m_aSwing, Game.m_CardSwing, sizeof(int)*7);
	}
	*/

	return TRUE;
}

BOOL CResultHi::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// ���̾�α� ��Ʈ�� �����ǰ� ��ġ ������
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	m_ShowCardBtn.Init(80,226,".\\image\\commonbtn\\Btn_ok.bmp",4);
	m_ShowCardBtn.ShowWindow(SW_HIDE);

	// ��� �׸� �б�
	if( g_MyObserver ) {
		Back.LoadBitmapFile(".\\image\\Gameover\\result_ob.bmp");
		m_winner_kind = 0;
	}
	else if(Play[0].ServPNum == Game.WinnerPNum) {
		Back.LoadBitmapFile(".\\image\\Gameover\\result_mywin.bmp");
		m_winner_kind = 1;

		// ĳ���� �ε���0 ����(���â) - jeong
		m_CharView.CharNum = g_pGameView->CharBox[0].m_nCharIndex[0];

		// ���� Bank Money(����) - jeong
		Play[0].BankMoney = Play[0].BankMoney + pGO.nWinMoney;

		// �̱� �ݾ� ���� - jeong
		Play[0].nWinMoney = pGO.nWinMoney;
	}
	else {
		Back.LoadBitmapFile(".\\image\\Gameover\\result_otherwin.bmp");		// Result design will be changed - jeong
		m_winner_kind = 2;

		// ĳ���� �ε���1 ����(���â) - jeong
		m_CharView.CharNum = g_pGameView->CharBox[0].m_nCharIndex[1];

		// ���� �ݾ� ���� - jeong
		Play[0].LoseMoney = Play[0].LoseMoney + pGO.Ui[Play[0].ServPNum].LoseMoney;

		INT64 MinusMoney = Play[0].PrevMoney-Play[0].LoseMoney;

		if( MinusMoney < 0)
		{
				Play[0].PrevMoney = 0;
				Play[0].LoseMoney = 0;
				Play[0].BankMoney += MinusMoney;
		}
	

		//m_CloseBtn.Init(131, 304,".\\image\\commonbtn\\Btn_ok.bmp",4);
		
		// [��ȣõ��] 2004.07.08 
		// ���� �ð��� �� ������ �ð����� ��ȯ
		CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
		SYSTEMTIME st;
		time.GetAsSystemTime(st);
		BOOL bExist = FALSE;
//				   GameItem_IsHaveItem(g_MyGameItem, ITEM_SCODE_SAFEGUARDANGEL);
		BOOL rtn = GameItem_IsUsingItem(g_MyGameItem, ITEM_SCODE_SAFEGUARDANGEL, &bExist, &st);
		if( bExist ) {
			m_nItemByeUse = 1;
			//m_SafeBtn.Init(238,290,".\\image\\GameOver\\Btn_Use.bmp",4); // ����ϱ�
		}
		else {
			m_nItemByeUse = 2;
			//m_SafeBtn.Init(238,290,".\\image\\GameOver\\Btn_Bye.bmp",4); // �����ϱ�
	
		}
	}

	if(Play[0].ServPNum != Game.WinnerPNum && m_winner_kind > 0){
		m_CloseBtn.Init(130,361,".\\image\\commonbtn\\Btn_ok.bmp",4);
		//m_MiniGame.Init(190,361,".\\image\\commonbtn\\Btn_Yes.bmp",4);
	}
	else{
		m_CloseBtn.Init(110,251,".\\image\\commonbtn\\Btn_ok.bmp",4);
		m_MiniGame.Init(190,251,".\\image\\commonbtn\\Btn_Yes.bmp",4);
	}


//	if(WinCase == 0 )
//		Back.LoadBitmapFile(".\\image\\resulthi.bmp");
//	else //if(WinCase == 1)
//		Back.LoadBitmapFile(".\\image\\resulthi.bmp");
//	else if(WinCase == 2)
//		Back.LoadBitmapFile(".\\image\\resultLow.bmp");
//	else
//		Back.LoadBitmapFile(".\\image\\resultSwing.bmp");

	// ��ư �ʱ�ȭ
	//m_CloseBtn.Init(IDB_BTN_RESULTOK1, IDB_BTN_RESULTOK2, IDB_BTN_RESULTOK3, IDB_BTN_RESULTOK4);
	//m_CloseBtn.Init(268,176,".\\image\\GBtn_result_mywin.bmp",4);

		// ��Ʈ ����
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
		"����");

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
		"����");

	int xSize = 59;
	int ySize = 99;

	Page.Init(146, ySize, 16);

	// ĳ���� �� �ʱ�ȭ
	CRect rc;
	rc.SetRect(0,0,75,125);
	rc.OffsetRect(22,61);
	m_CharView.Create(this, rc, 5, 3224);

	hTimer = SetTimer(RESULTHI_TIMER, 1000, NULL);
	TimeCnt = 0;

	// [��ȣõ��] 2004.07.08 
	//if(m_nItemByeUse == 0) TimeCnt = 3 ;
	//else TimeCnt = 5;

	// ���а��â ���ð� - jeong
	TimeCnt = MAX_WAIT_RESULT;
	
	/*

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 346;
//	rc1.bottom = rc1.top + 269;
	rc1.bottom = rc1.top + Back.GetHeight();

	MoveWindow( &rc1 );
	*/

	WindowResize2Bitmap(m_hWnd, Back);

	CenterWindow(g_pGameView->GetOwner());	

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
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// �����

	cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	::DeleteObject(h);
	h = NULL;

	int ax = 12, ay = 182;

	// ���� ���
	int level =  UI.nIcon;// GetLevel( UI.WinNum, UI.LooseNum, UI.DrawNum);
//	CMyBitmap bmp;
//	bmp.LoadBitmap(IDB_LEVEL15);
//	bmp.TransDraw(dc.m_hDC, ax-4, ay, 15, 15, 15*level, 0, RGB(255,0,255));
//	bmp.DeleteObject();
	dc.SetBkMode(TRANSPARENT);
	/*
	// ���̵� ��� deleted by jeong
	dc.SelectObject(&Font1);
	dc.SetTextColor(RGB(0,0,0));
	//dc.TextOut(ax, ay+20, UI.ID);
	*/

	CRect rt;

	if(Play[0].ServPNum != Game.WinnerPNum && m_winner_kind > 0 ){
		rt.SetRect(13,217,13+92,217+21);
	}
	else{
		rt.SetRect(12,206,12+92,206+21);
	}

	
	// ���̵� ���
	//dc.DrawText(UI.ID, &rt, DT_CENTER | DT_WORDBREAK);

	
	CString str ="";
	CString strM="";


	if(WinCase == 0)
	{	
		/*
		ax = 115 , ay = 60;
		INT64 nTotBat = Game.GetRealBet();
		strM = NumberToOrientalString(nTotBat);//pGO.Pg.nTotBat);					// ���̽��� �Ӵ� ��
		str.Format("�� �ѹ��þ�:%s��",strM);
		dc.TextOut(ax, ay+19, str);
	
		strM = NumberToOrientalString(pGO.nWinMoney); // ���� �Ӵ�	
		str.Format("�� ȹ��ݾ�:%s��",strM);
		dc.TextOut(ax, ay+38, str);

			//#####  ���ݾ� ǥ�� #####
		strM = NumberToOrientalString(pGO.nGory);	
		str.Format(g_StrMan.Get(_T("BET_GORIWINDOW")),pGO.nPercent,strM);
		dc.TextOut(ax, ay+57, str);

		strM = NumberToOrientalString(UI.PMoney);
		str.Format(g_StrMan.Get(_T("USER_PMONEY")),strM);//�� �ݾ�:%s��	
		str.Format("�� �ݾ�:%s��",strM);//
		dc.TextOut(ax, ay+76, str);
		*/

		ax = 109, ay = 183;
		strM = NumberToOrientalString(pGO.nWinMoney); // ���� �Ӵ�		
		//str.Format("��ȹ��ݾ�(+%s)��",strM );//, pGO.nPercent); // ���̽� (+%s) ���Ӻ�(%d%%):-%s��
		str.Format("+%s",strM );
		CRect rt;

		if(Play[0].ServPNum != Game.WinnerPNum && m_winner_kind > 0){
			rt.SetRect(117,217,117+210,217+17);
		}
		else{
			rt.SetRect(114,207,114+210,207+17);
		}
		

		dc.DrawText(str, &rt, DT_RIGHT | DT_WORDBREAK);

		if(Play[0].ServPNum != Game.WinnerPNum && m_winner_kind > 0)//���� ���� �ݾ� ǥ�� 
		{
			CRect rt;
			CString str1,str2;
			rt.SetRect(117,244,117+210,244+17);

		//	INT64 roundingoff = g_Poker.RU[Play[0].ServPNum].nRealBat;
			INT64 roundingoff = pGO.Ui[Play[0].ServPNum].LoseMoney;
			//if(roundingoff > 100)
			//roundingoff = (roundingoff/100)*100;

			str2 = NumberToOrientalString(roundingoff);
			str1.Format("-%s",str2);
			dc.DrawText(str1, &rt, DT_RIGHT | DT_WORDBREAK);

			/*
			//��ȣõ�� ���� �Ӵ�
			CString limitm;
			CRect limitrt;
			dc.SetTextColor(RGB(255,0,0));
			limitm = "�⺻ 18��� ~ �ִ� 45���";
			limitrt.SetRect(35,333,180+35,333+13);
			dc.DrawText(limitm, &limitrt, DT_CENTER | DT_WORDBREAK);
			*/
		}


		// ī�� 5�� �׸���
		int px = 149;
		int py = 88;

		for(int i = 0; i < 5; i++) {
			Page.PutSprAuto(i*18, 0, &BigCardSpr, 52);
		}
		Page.FlipToGDI(dc.m_hDC, px, py ,1);

	}
	else if(WinCase == 1)
	{
		ax = 109, ay = 183;
		strM = NumberToOrientalString(pGO.nWinMoney); // ���� �Ӵ�		
		//str.Format("��ȹ��ݾ�(+%s)��",strM );//, pGO.nPercent); // ���̽� (+%s) ���Ӻ�(%d%%):-%s��
		str.Format("+%s",strM );
		CRect rt;

		if(Play[0].ServPNum != Game.WinnerPNum && m_winner_kind > 0){
			rt.SetRect(117,217,117+210,217+17);
		}
		else{
			rt.SetRect(114,207,114+210,207+17);
		}
		

		dc.DrawText(str, &rt, DT_RIGHT | DT_WORDBREAK);

		if(Play[0].ServPNum != Game.WinnerPNum && m_winner_kind > 0)//���� ���� �ݾ� ǥ�� 
		{
			CRect rt;
			CString str1,str2;
			rt.SetRect(117,244,117+210,244+17);

		//	INT64 roundingoff = g_Poker.RU[Play[0].ServPNum].nRealBat;
			INT64 roundingoff = pGO.Ui[Play[0].ServPNum].LoseMoney;

			//if(roundingoff > 100)
			//roundingoff = (roundingoff/100)*100;

			str2 = NumberToOrientalString(roundingoff);
			str1.Format("-%s",str2);
			dc.DrawText(str1, &rt, DT_RIGHT | DT_WORDBREAK);

			/*
			//��ȣõ�� ���� �Ӵ�
			CString limitm;
			CRect limitrt;
			dc.SetTextColor(RGB(255,0,0));
			limitm = "�⺻ 18��� ~ �ִ� 45���";
			limitrt.SetRect(35,333,180+35,333+13);
			dc.DrawText(limitm, &limitrt, DT_CENTER | DT_WORDBREAK);
			*/

		}

		// ī�� 5�� �׸���
		int px = 149;
		int py = 88;

		for(int i = 0; i < 5; i++) {
			Page.PutSprAuto(i*18, 0, &BigCardSpr, m_aCard[i]);
		}
		Page.FlipToGDI(dc.m_hDC, px, py ,1);
	
	//	m_Str1 = "�ξ� ��Ʈ����Ʈ ���ġ 2";
		ax = 139;
		ay = 68;			// ���� ������ ��ǥ - modifed by jeong
		CRect rect;
		rect.SetRect(0,0,160,14);
		if(m_Str1!=""){
			dc.SetTextColor(RGB(0,0,0));
			rect.OffsetRect(ax, ay);
			dc.DrawText(m_Str1, &rect, DT_CENTER | DT_WORDBREAK);

		}
	}

	

//	str.Format("ȹ���� �ݾ��� %d%%�� ���Ӻ�� �����˴ϴ�.", pGO.nPercent);
//	dc.SetTextColor(RGB(43,140,185));
//	dc.TextOut(55, 208, str.operator LPCTSTR());

/*	
	if(m_winner_kind==0)
		dc.SetTextColor(RGB(43,140,185));
	else if(m_winner_kind==1)
		dc.SetTextColor(RGB(255,131,7));
	else
		dc.SetTextColor(RGB(103,130,160));
*/
	

	//m_CharView.SetCharacter(&UI);
	// ĳ���� �ε��� ����(���â) - jeong
	/*
	if ( Play[0].ServPNum == Game.WinnerPNum )
		m_CharView.CharNum = g_pGameView->CharBox[0].m_nCharIndex[0];
	else
		m_CharView.CharNum = g_pGameView->CharBox[0].m_nCharIndex[1];
	*/
	
	
	
	cdc.DeleteDC();
	
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
		// ���� �ð��� �� ������ �ð����� ��ȯ
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
	// ( �����ϱ�� )
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
	Sound.Play(SND45);	

	//if( Game.m_bShowMiniGame )
	if( Play[0].ServPNum == Game.WinnerPNum )
		g_Mini.DoModal();

	OnOK();
}





















