// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "OptionDlg.h"

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog


COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{
	// NULL브러쉬 생성
	m_HollowBrush.CreateStockObject(HOLLOW_BRUSH);

	//{{AFX_DATA_INIT(COptionDlg)
	//}}AFX_DATA_INIT

	m_ManSndKind = -1;		//남자 사운드 종류
	m_FemaleSndKind = -1;	//여자 사운드 종류
	m_bOffSndFX = FALSE;
	m_bOffMusic = FALSE;
	m_nScreenMode = -1;
	m_bNoWhisper = FALSE;
	m_bNoInvite = FALSE;
}


void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDlg)
	DDX_Control(pDX, IDC_BTN_OPTION_MAN3, m_ManSnd3);
	DDX_Control(pDX, IDC_BTN_OPTION_FEMALE3, m_FemaleSnd3);
	DDX_Control(pDX, IDC_BTN_STARTMODE2, m_StartWindowScreenMode);
	DDX_Control(pDX, IDC_BTN_STARTMODE1, m_StartFullScreenMode);
	DDX_Control(pDX, IDC_BTN_OPTION_MAN2, m_ManSnd2);
	DDX_Control(pDX, IDC_BTN_OPTION_MAN1, m_ManSnd1);
	DDX_Control(pDX, IDC_BTN_OPTION_FEMALE2, m_FemaleSnd2);
	DDX_Control(pDX, IDC_BTN_OPTION_FEMALE1, m_FemaleSnd1);
	DDX_Control(pDX, IDC_BTN_OFFSNDFX, m_OffSndFx);
	DDX_Control(pDX, IDC_BTN_OFFMUSIC, m_OffMusic);
	DDX_Control(pDX, IDC_BTN_NOWHISPER, m_NoWhisper);
	DDX_Control(pDX, IDC_BTN_NOINVITE, m_NoInvite);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OKBtn);
	DDX_Control(pDX, IDC_LIST_SHUTMOUSE, m_IDList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	//{{AFX_MSG_MAP(COptionDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SHUTMOUSE, OnClickListShutmouse)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers

BOOL COptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);	

	// 버튼 초기화
	m_OKBtn.Init(53, 367, ".\\image\\Option\\btn_apply.BMP", 4);
	m_CancelBtn.Init(153,367, ".\\image\\commonbtn\\Btn_cancel.bmp", 4);

//	m_btnOptionMode.Init(26, 10,NULL,NULL,this);
//	m_btnMacroMode.Init(145,10,NULL,NULL,this);
//	m_btnOptionMode.AddToGroup(&m_btnMacroMode);

	// 이미지 채크 옵션 버튼

	m_ManSnd1.Init(68,68,".\\image\\commonbtn\\btn_radio.BMP",NULL,this);
	m_ManSnd2.Init(131,68,".\\image\\commonbtn\\btn_radio.bmp",NULL,this);
	m_ManSnd3.Init(195,68,".\\image\\commonbtn\\btn_radio.bmp",NULL,this);
	m_ManSnd1.AddToGroup(&m_ManSnd2);
	m_ManSnd1.AddToGroup(&m_ManSnd3);

	m_FemaleSnd1.Init(68,86,".\\image\\commonbtn\\btn_radio.bmp",NULL,this);
	m_FemaleSnd2.Init(131,86,".\\image\\commonbtn\\btn_radio.bmp",NULL,this);	
	m_FemaleSnd3.Init(195,86,".\\image\\commonbtn\\btn_radio.bmp",NULL,this);	
	m_FemaleSnd1.AddToGroup(&m_FemaleSnd2);
	m_FemaleSnd1.AddToGroup(&m_FemaleSnd3);

	m_OffSndFx.Init(44,130,".\\image\\commonbtn\\btn_check.bmp",NULL,this);
	m_OffMusic.Init(143,130,".\\image\\commonbtn\\btn_check.bmp",NULL,this);

	m_StartWindowScreenMode.Init(44,177,".\\image\\commonbtn\\btn_radio.bmp",NULL,this);	
	m_StartFullScreenMode.Init(143,177,".\\image\\commonbtn\\btn_radio.bmp",NULL,this);
	m_StartWindowScreenMode.AddToGroup(&m_StartFullScreenMode);

	m_NoInvite.Init(44,223,".\\image\\commonbtn\\btn_check.bmp",NULL,this);
	m_NoWhisper.Init(143,223,".\\image\\commonbtn\\btn_check.bmp",NULL,this);

	m_ManSndKind = -1;		//남자 사운드 종류
	m_FemaleSndKind = -1;	//여자 사운드 종류
	m_bOffSndFX = FALSE;
	m_bOffMusic = FALSE;
	m_nScreenMode = -1;
	m_bNoWhisper = FALSE;
	m_bNoInvite = FALSE;
		

	m_bOffSndFX = g_pMainDlg->CFG.bOffSndFX;
	m_bOffMusic = g_pMainDlg->CFG.bOffMusic;
	m_nScreenMode = g_pMainDlg->CFG.nScreenMode;
	m_bNoInvite = g_pMainDlg->CFG.bNoInvite;
	m_bNoWhisper = g_pMainDlg->CFG.bNoWhisper;


	m_OffSndFx.SetCheck(g_pMainDlg->CFG.bOffSndFX);
	m_OffMusic.SetCheck(g_pMainDlg->CFG.bOffMusic);

	if(g_pMainDlg->CFG.nScreenMode ==1)m_StartFullScreenMode.OnClicked();
	else m_StartWindowScreenMode.OnClicked();

	m_NoWhisper.SetCheck(g_pMainDlg->CFG.bNoWhisper);
	m_NoInvite.SetCheck(g_pMainDlg->CFG.bNoInvite);	
	
	
	// ### [사운드 추가 작업] ###
	// ### [ 관전기능 ] ###
	if(g_MyInfo.UI.Sex == 1) // 남자
	{
		if(g_pMainDlg->CFG.SndFxKind >= 3) g_pMainDlg->CFG.SndFxKind = 0;

		m_FemaleSnd1.RemoveFromGroup();	
		m_FemaleSnd1.EnableWindow(FALSE);
		m_FemaleSnd2.EnableWindow(FALSE);
		m_FemaleSnd3.EnableWindow(FALSE);

		m_ManSndKind = g_pMainDlg->CFG.SndFxKind;		//남자 사운드 종류
	}
	else // 여자
	{
		if(g_pMainDlg->CFG.SndFxKind < 3 ) g_pMainDlg->CFG.SndFxKind = 3;
		
		m_ManSnd1.RemoveFromGroup();	
		m_ManSnd1.EnableWindow(FALSE);
		m_ManSnd2.EnableWindow(FALSE);
		m_ManSnd3.EnableWindow(FALSE);

		m_FemaleSndKind = g_pMainDlg->CFG.SndFxKind;	//여자 사운드 종류
	}
	

	// ### [사운드 추가 작업] ###
	switch(g_pMainDlg->CFG.SndFxKind)
	{
		case 0:
			m_ManSnd1.SetCheck(TRUE);		
			break;
		case 1:
			m_ManSnd2.SetCheck(TRUE);
			break;
		case 2:
			m_ManSnd3.SetCheck(TRUE);
			break;
		case 3:
			m_FemaleSnd1.SetCheck(TRUE);
			break;
		case 4:
			m_FemaleSnd2.SetCheck(TRUE);
			break;
		case 5:
			m_FemaleSnd3.SetCheck(TRUE);
			break;		
	}

	UpdateData(FALSE);

	InitList();
	if(g_Where == WH_GAME) {
		AddID();
	}

	RECT rc1;
	GetWindowRect( &rc1 );
//	rc1.right = rc1.left + 225;
//	rc1.bottom = rc1.top + 390;

	rc1.right = rc1.left + 276;
	rc1.bottom = rc1.top + 413;

	MoveWindow( &rc1 );

	
	// 매크로 페이지..
	CRect mrect;
	mrect.SetRect(0,0,175,16);
	mrect.OffsetRect(77 ,48);
	for( int i=0; i< MAX_MACRO_NUM; i++ ){
		m_Macroedit[i].Create(WS_CHILD| WS_TABSTOP| ES_WANTRETURN | ES_LEFT | ES_AUTOHSCROLL , mrect, this, IDC_EDIT_MACRO + i);
		m_Macroedit[i].SetFont(GetFont());
		m_Macroedit[i].LimitText(39);				
		m_Macroedit[i].SetWindowText(g_ChatMacro.Macrostr[i]);	
		mrect.OffsetRect(0,28);
	}


	m_DisPlayMode = MODE_OPTION;	
	
	OnChangeView(m_DisPlayMode);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptionDlg::OnChangeView(char mode)//메크로
{	
	if(mode == MODE_OPTION && mode != m_DisPlayMode)
	{	
		m_ManSnd3.ShowWindow(SW_HIDE);
		m_FemaleSnd3.ShowWindow(SW_HIDE);
		m_StartWindowScreenMode.ShowWindow(SW_HIDE);
		m_StartFullScreenMode.ShowWindow(SW_HIDE);
		m_ManSnd2.ShowWindow(SW_HIDE);
		m_ManSnd1.ShowWindow(SW_HIDE);
		m_FemaleSnd2.ShowWindow(SW_HIDE);
		m_FemaleSnd1.ShowWindow(SW_HIDE);
		m_OffSndFx.ShowWindow(SW_HIDE);
		m_OffMusic.ShowWindow(SW_HIDE);
		m_NoWhisper.ShowWindow(SW_HIDE);
		m_NoInvite.ShowWindow(SW_HIDE);	
		m_IDList.ShowWindow(SW_HIDE);

		for( int i=0; i< MAX_MACRO_NUM; i++){
			this->m_Macroedit[i].ShowWindow(SW_HIDE);	
		}

		m_DisPlayMode = mode;

		RECT rc1;
		GetClientRect(&rc1);
		rc1.right = rc1.left + 276;
		rc1.bottom = rc1.top + 360;	
		InvalidateRect(&rc1,FALSE);
	}
	if(mode == MODE_MACRO && mode != m_DisPlayMode)
	{		
		m_ManSnd3.ShowWindow(SW_HIDE);
		m_FemaleSnd3.ShowWindow(SW_HIDE);
		m_StartWindowScreenMode.ShowWindow(SW_HIDE);
		m_StartFullScreenMode.ShowWindow(SW_HIDE);
		m_ManSnd2.ShowWindow(SW_HIDE);
		m_ManSnd1.ShowWindow(SW_HIDE);
		m_FemaleSnd2.ShowWindow(SW_HIDE);
		m_FemaleSnd1.ShowWindow(SW_HIDE);
		m_OffSndFx.ShowWindow(SW_HIDE);
		m_OffMusic.ShowWindow(SW_HIDE);
		m_NoWhisper.ShowWindow(SW_HIDE);
		m_NoInvite.ShowWindow(SW_HIDE);	
		m_IDList.ShowWindow(SW_HIDE);
	
		for( int i=0; i< MAX_MACRO_NUM; i++){
			this->m_Macroedit[i].ShowWindow(SW_HIDE);	
		}
		
		RECT rc1;	
		GetClientRect(&rc1);
		rc1.right = rc1.left + 276;
		rc1.bottom = rc1.top + 360;	
		InvalidateRect(&rc1,FALSE);
		m_DisPlayMode = mode;
	} 	
}

void COptionDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(Sound.bDontUseSndFx != m_bOffSndFX) Sound.SetSndFx(m_bOffSndFX ^ 1);
	if(Sound.bDontUseMusic != m_bOffMusic) {
		Sound.SetMusic(m_bOffMusic ^ 1);
		//
		Game.PlayMusic();
	}

	if(g_Where == WH_GAME) {
		int num = -1;
		for(;;)
		{
			num = m_IDList.GetNextItem(num, LVNI_ALL);
			if(num == -1) break;

			LVITEM Item = {0,};
			Item.mask = LVIF_IMAGE;
			m_IDList.GetItem(&Item);
			int image = Item.iImage;

			CString id = m_IDList.GetItemText(num, 0);
			int pnum = Game.GetPlayerPNum((char*)(LPCTSTR)id);
			if(pnum != -1) {
				if(image) Play[pnum].bShutMouse = TRUE;
				else Play[pnum].bShutMouse = FALSE;
			}
		}
	}


	// 초대거부와 귓말거부에 대하여 새로운 환경설정을 적용
	CButton* pCheck;
	pCheck = (CButton*)g_pLobyDlg->GetDlgItem(IDC_CHECK_REFUSEINVITE);
	if(pCheck) {
		if(m_bNoInvite != pCheck->GetCheck()) {
			pCheck->SetCheck(m_bNoInvite);
		}
	}
	pCheck = (CButton*)g_pLobyDlg->GetDlgItem(IDC_CHECK_REFUSEWHISPER);
	if(pCheck) {
		if(m_bNoWhisper != pCheck->GetCheck()) {
			pCheck->SetCheck(m_bNoWhisper);
			// 상태에 따라 귓말 거부/허용 메시지를 보냄
			CSV_NOWHISPER nwmsg;
			nwmsg.Set(m_bNoWhisper);
			SockMan.SendData(g_MainSrvSID, nwmsg.pData, nwmsg.GetTotalSize());
		}
	}


	g_pMainDlg->CFG.bOffSndFX = m_bOffSndFX;
	g_pMainDlg->CFG.bOffMusic = m_bOffMusic;
	g_pMainDlg->CFG.nScreenMode = m_nScreenMode;
	g_pMainDlg->CFG.bNoInvite = m_bNoInvite;
	g_pMainDlg->CFG.bNoWhisper = m_bNoWhisper;

	// ### [사운드 추가 작업] ###
	if(g_MyInfo.UI.Sex == 1) // 남자
	{
		g_pMainDlg->CFG.SndFxKind= m_ManSndKind;
	}
	else
	{
		g_pMainDlg->CFG.SndFxKind= m_FemaleSndKind;
	}
	

	// ### [ 관전기능 ] ###
	if( !g_MyObserver ) Play[0].nSndFxKind = g_pMainDlg->CFG.SndFxKind;
	g_MyInfo.nSndFxKind = g_pMainDlg->CFG.SndFxKind;
	
	//메크로 저장

	memset(&g_ChatMacro,0,sizeof(g_ChatMacro));

	for( int i=0; i< MAX_MACRO_NUM; i++ ){		
		m_Macroedit[i].GetWindowText(g_ChatMacro.Macrostr[i],39);			
	}

	g_pMainDlg->SaveConfig(g_MyInfo.UI.ID);// 환경 설정 파일을 저장한다

	Sound.Play(SND45);
	CDialog::OnOK();
}

void COptionDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	Sound.Play(SND45);
	CDialog::OnCancel();
}

void COptionDlg::InitList()
{
	// 비트맵 읽기
	CBitmap Bmp;
	Bmp.LoadBitmap(IDB_IMAGE_CHECK);

	// 이미지 리스트 생성
	m_CheckImage.Create(15, 15, ILC_COLOR8 | ILC_MASK, 6, 0);
	m_CheckImage.Add(&Bmp, RGB(255,255,255));

	m_IDList.SetImageList(&m_CheckImage, LVSIL_SMALL);

	// 전체행 셀렉트 설정
	m_IDList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// 컬러 설정
	m_IDList.SetBkColor(RGB(122,146,219));
	m_IDList.SetTextBkColor(RGB(122,146,219));

	//// Column S삽입부
	LV_COLUMN col;
	char szName[256]={0,};

	wsprintf(szName, "ID");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 130;
	col.pszText = szName;
	col.iSubItem = 0;
	m_IDList.InsertColumn(0, &col);

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 225;
	rc1.bottom = rc1.top + 390;
	MoveWindow( &rc1 );
}

void COptionDlg::AddID()
{
	char szName[256]={0,};
	for(int i=1; i<g_Max_NewPlayer; i++) // ### [관리자 모드 작업] ###
	{
		if(Play[i].UI.cAdminUser != 1 && strlen(Play[i].UI.ID)>0) {
			// 레코드를 리스트 뷰에 삽입
			LV_ITEM item = {0,};
			// 이미지
			int image = Play[i].bShutMouse;
			// 아이디
			wsprintf(szName, "%s", Play[i].UI.ID);
			item.mask = LVIF_TEXT | LVIF_IMAGE;
			item.pszText = szName;
			item.iItem = m_IDList.GetItemCount();
			item.iSubItem = 0;
			item.iImage = image;
			m_IDList.InsertItem(&item);
		}
	}
}

void COptionDlg::OnClickListShutmouse(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pnt;
	GetCursorPos(&pnt);
	m_IDList.ScreenToClient(&pnt);

	UINT flag;
	int num = m_IDList.HitTest(pnt, &flag);
	if(num != -1) {
		LVITEM Item = {0,};
		Item.mask = LVIF_IMAGE;
		m_IDList.GetItem(&Item);
		int image = Item.iImage;
		image ^= 1;
		m_IDList.SetItem( num, 0, LVIF_IMAGE, NULL, image, NULL, NULL, NULL);
	}

	*pResult = 0;
}

HBRUSH COptionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) {
		case CTLCOLOR_STATIC:
			// The Slider Control has CTLCOLOR_STATIC, but doesn't let
			// the background shine through,
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255,255,255));
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

void COptionDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	if(m_DisPlayMode == MODE_OPTION)//메크로
	{
		if(Back.m_hObject) Back.DeleteObject();
		Back.LoadBitmapFile(".\\image\\Option\\optionback.bmp");
	}
	else 
	{
		if(Back.m_hObject) Back.DeleteObject();
		Back.LoadBitmapFile(".\\image\\Option\\optmacroback.BMP");
	}

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법
	
	CBitmap *pOldBmp = cdc.SelectObject(&Back);
	

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	
	::DeleteObject(h);
	h = NULL;
	cdc.SelectObject(pOldBmp);	
	cdc.DeleteDC();	
	Back.DeleteObject();
	// Do not call CDialog::OnPaint() for painting messages
}

void COptionDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CRect rt[2];
	rt[0].SetRect(26,10,131,30);
	rt[1].SetRect(145,10,131+145,30);

	if(rt[0].PtInRect(point)){
		Sound.Play(SND45);
		OnChangeView(MODE_OPTION);
		return;
	}
	if(rt[1].PtInRect(point)){
		Sound.Play(SND45);
		OnChangeView(MODE_MACRO);
		return;
	}

	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

BOOL COptionDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	

	switch(wParam)
	{
	case IDC_BTN_OPTION_MAN1:
		m_ManSndKind = 0;
		break;
	case IDC_BTN_OPTION_MAN2:
		m_ManSndKind = 1;
		break;
	case IDC_BTN_OPTION_MAN3:
		m_ManSndKind = 2;
		break;
	case IDC_BTN_OPTION_FEMALE1:
		m_FemaleSndKind = 3;
		break;
	case IDC_BTN_OPTION_FEMALE2:
		m_FemaleSndKind = 4;
		break;
	case IDC_BTN_OPTION_FEMALE3:
		m_FemaleSndKind = 5;
	case IDC_BTN_OFFSNDFX:
		m_bOffSndFX = m_OffSndFx.GetCheck();		
		break;
	case IDC_BTN_OFFMUSIC:
		m_bOffMusic = m_OffMusic.GetCheck();		
		break;
	case IDC_BTN_STARTMODE1:	//full			
		m_nScreenMode = m_StartFullScreenMode.GetCheck();
		break;
	case IDC_BTN_STARTMODE2://win		
		if(m_StartWindowScreenMode.GetCheck() == TRUE)
		m_nScreenMode = 0;
		break;
	case IDC_BTN_NOWHISPER:
		m_bNoWhisper = m_NoWhisper.GetCheck();
		break;
	case IDC_BTN_NOINVITE:
		m_bNoInvite = m_NoInvite.GetCheck();
		break;
	}
	
	return CDialog::OnCommand(wParam, lParam);
}

