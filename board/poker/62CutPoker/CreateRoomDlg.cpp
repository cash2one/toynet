// CreateRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "CreateRoomDlg.h"
#include "Global.h"

//#include "

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateRoomDlg dialog


//CCreateRoomDlg::CCreateRoomDlg(CWnd* pParent /*=NULL*/)
CCreateRoomDlg::CCreateRoomDlg(int nIcon,INT64 nMoney,CWnd* pParent /*=NULL*/)
	: CDialog(CCreateRoomDlg::IDD, pParent)
{
	m_HollowBrush.CreateStockObject(HOLLOW_BRUSH);
	//{{AFX_DATA_INIT(CCreateRoomDlg)
	m_RoomTitle = _T("");
	m_Pass = _T("");
	m_nBat = -1;
	m_nOpen = -1;
	m_nStyle = -1;
	m_bSuperMaster = FALSE;
	m_bPlayer = -1;
	//}}AFX_DATA_INIT
	m_bMakeRoom   = FALSE;
	m_nUserMoney  = nMoney;
	m_nUserIcon   = nIcon;
	m_nPlayer	  = 0;
	m_bHaveSupermaster = FALSE;
	m_bSuperRoomMaster = FALSE;
}


void CCreateRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateRoomDlg)
	DDX_Control(pDX, IDC_BTN_IMGCHECK, m_ckSuperMaster);
	DDX_Control(pDX, IDC_COMBO_MINIM, m_MinMCombo);
	DDX_Control(pDX, IDC_COMBO_FORMKIND, m_FormKindCombo);
	DDX_Control(pDX, IDC_EDIT_ROOMTITLE, m_ctrRoomTitle);
	DDX_Control(pDX, IDC_COMBO_GAMEKIND, m_GameKindCombo);
	DDX_Control(pDX, IDC_COMBO_LEVEL, m_LevelCombo);
	DDX_Control(pDX, IDC_COMBO_MAXNUM, m_MaxNum);
	DDX_Control(pDX, IDC_COMBO_SECRET, m_SecretCombo);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OKBtn);
	DDX_Text(pDX, IDC_EDIT_ROOMTITLE, m_RoomTitle);
	DDV_MaxChars(pDX, m_RoomTitle, 30);
	DDX_Text(pDX, IDC_EDIT_ROOMPASS, m_Pass);
	DDV_MaxChars(pDX, m_Pass, 4);
	DDX_Radio(pDX, IDC_BATRADIO1, m_nBat);
	DDX_Radio(pDX, IDC_BATRADIO2, m_nOpen);
	DDX_CBIndex(pDX, IDC_COMBOSTYLE, m_nStyle);
	DDX_Check(pDX, IDC_CHECK_SUPERMASTER, m_bSuperMaster);
	DDX_Radio(pDX, IDC_RADIO_USERCOUNT5, m_bPlayer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateRoomDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateRoomDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_SECRET, OnSelchangeComboSecret)
	ON_CBN_SELCHANGE(IDC_COMBO_MAXNUM, OnSelchangeComboMaxnum)
	ON_CBN_SELCHANGE(IDC_COMBO_LEVEL, OnSelchangeComboLevel)
	ON_CBN_SELCHANGE(IDC_COMBO_FORMKIND, OnSelchangeComboFormkind)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_SUPERMASTER, OnCheckSupermaster)
	ON_CBN_SELCHANGE(IDC_COMBO_MINIM, OnSelchangeComboMinim)
	ON_BN_CLICKED(IDC_RADIO_USERCOUNT5, OnRadioUsercount5)
	ON_BN_CLICKED(IDC_RADIO_USERCOUNT6, OnRadioUsercount6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateRoomDlg message handlers

BOOL CCreateRoomDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	CRect rt;
	// 배경 로드
	Back.LoadBitmapFile(".\\image\\room\\croomback.bmp");
	
	// 버튼 초기화
	m_OKBtn.Init(57, 158, ".\\image\\room\\Btn_create.bmp", 4);
	m_CancelBtn.Init(154, 158, ".\\image\\commonbtn\\Btn_cancel.bmp", 4);

	m_ckSuperMaster.Init(102,104,".\\image\\commonbtn\\btn_check.bmp",NULL,this);	

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
		(LPCTSTR)g_StrMan.Get(_T("DEFAULT_FONT")));

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
		(LPCTSTR)g_StrMan.Get(_T("DEFAULT_FONT")));	

	// 콤보박스 초기화
	bSecret=0;
	m_SecretCombo.SetCurSel(0);
	nMaxNum=g_Max_Player;
	m_MaxNum.SetCurSel(2);
	m_LevelCombo.SetCurSel(0);
	m_Pass.Format("");

	rt.SetRect(207, 98,278,116);
	GetDlgItem(IDC_EDIT_ROOMPASS)->MoveWindow(rt);

	GameKind=0;
	m_GameKindCombo.SetCurSel(0);
	rt.SetRect(74,70,143,88);
	m_GameKindCombo.MoveWindow(rt);
	rt = NULL;
	// 추가 기본 초이스 방
	FormKind = 0;
	m_FormKindCombo.SetCurSel(0);
//	m_FormKindCombo.EnableWindow(FALSE);


	if(g_nchannel_usernum==0) {
		m_bPlayer = 0;
		m_nPlayer = 0;
	} else if(g_nchannel_usernum==1) {
		m_bPlayer = 0;
		m_nPlayer = 0;
		GetDlgItem(IDC_RADIO_USERCOUNT6)->EnableWindow(FALSE);
		
		m_MaxNum.ResetContent();
		m_MaxNum.AddString("3");
		m_MaxNum.AddString("4");
		m_MaxNum.AddString("5");
		
		nMaxNum=MAX_PLAYER-1/*g_Max_Player*/;
		m_MaxNum.SetCurSel(2);
	} else {
		m_bPlayer = 1;
		m_nPlayer = 1;
		GetDlgItem(IDC_RADIO_USERCOUNT5)->EnableWindow(FALSE);
		
		m_MaxNum.ResetContent();
		m_MaxNum.AddString("4");
		m_MaxNum.AddString("5");
		m_MaxNum.AddString("6");
		
		m_MaxNum.SetCurSel(2);
		nMaxNum=MAX_PLAYER/*g_Max_Player*/;
	}


	rt.SetRect(207,70,278,88);
	m_MaxNum.MoveWindow(rt);

	rt = NULL;
	GetDlgItem(IDC_RADIO_USERCOUNT6)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_USERCOUNT5)->EnableWindow(FALSE);
	UpdateData(FALSE);



	// 디폴트 제목
	CString title;

	int nRoomNameNum = (int)rand()%9;
	switch(nRoomNameNum){
	case 0: title = g_StrMan.Get(_T("RNAME1"));break;
	case 1: title =	g_StrMan.Get(_T("RNAME2"));break;
	case 2: title =	g_StrMan.Get(_T("RNAME3"));break;
	case 3: title =	g_StrMan.Get(_T("RNAME4"));break;
	case 4: title =	g_StrMan.Get(_T("RNAME5"));break;
	case 5: title =	g_StrMan.Get(_T("RNAME6"));break;
	case 6: title =	g_StrMan.Get(_T("RNAME7"));break;
	case 7: title =	g_StrMan.Get(_T("RNAME8"));break;
	case 8: title = g_StrMan.Get(_T("RNAME9"));break;
	}

	switch(g_nChannelStyle){
	case 0: // 자유채널
		m_nBat     =  0; // 1회레이즈
//		m_nMaxUser =  2; // 최대인원 6명
		m_nStyle   =  0; // 방스타일 쿼터배팅
		Level = 0;
		GameKind = 0;
		break; 
	case 1: // 초보채널
		m_nBat     =  0; // 1회레이즈
//		m_nMaxUser =  2; // 최대인원 6명
		m_nStyle   =  0; // 방스타일 쿼터배팅
		Level = 0;
		GameKind = 0;
		break; 
	case 2: // 중수 채널
		m_nBat     =  0; // 1회레이즈
//		m_nMaxUser =  2; // 최대인원 6명
		m_nStyle   =  0;//1; // 방스타일 하프배팅
		Level = 0;
		GameKind = 1;
		break; 
	case 3: // 고수채널
		m_nBat     =  0;//1; // 2회레이즈
//		m_nMaxUser =  2; // 최대인원 6명
		m_nStyle   =  0;//1; // 방스타일 하프배팅
		Level = 1;
		GameKind = 1;
		break; 
	case 4: // 도신채널
		m_nBat     =  0;//1; // 2회레이즈
//		m_nMaxUser =  2; // 최대인원 6명
		m_nStyle   =  1; // 방스타일 하프배팅
		Level = 1;
		GameKind = 1;
		break; 
	}

	m_nOpen    =  0; // 공개를 기본으로
	
	m_ctrRoomTitle.SetWindowText(title);
	m_ctrRoomTitle.SetSel(0, title.GetLength());
	m_ctrRoomTitle.SetFocus();

	
	rt.SetRect(75, 42, 278, 60);
	m_ctrRoomTitle.MoveWindow(rt);
	rt = NULL;

	m_bHaveSupermaster = FALSE;
	m_bSuperRoomMaster = FALSE;

	// [게임 아이템 작업]
	// 현재 시간을 얻어서 서버의 시간으로 변환
	CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);
	// 슈퍼방장 아이템이 사용중이면 슈퍼방 만들기 옵션 자동 체크
	BOOL bExist = FALSE;
	if(GameItem_IsUsingItem(g_MyGameItem, ITEM_SCODE_SUPERMASTER, &bExist, &st)) {
	//	m_bSuperMaster = TRUE;
		m_bSuperRoomMaster = TRUE;
		m_bHaveSupermaster = TRUE;
	//	CheckDlgButton(IDC_CHECK_SUPERMASTER, BST_CHECKED);		
		m_ckSuperMaster.SetCheck(TRUE);
	}
	else if(GameItem_IsUsingItem(g_MyGameItem, ITEM_SCODE_INVINCIBLE, &bExist, &st)) { // [천하무적 아이템]
	//	m_bSuperMaster = TRUE;
		m_bSuperRoomMaster = TRUE;
		m_bHaveSupermaster = TRUE;
	//	CheckDlgButton(IDC_CHECK_SUPERMASTER, BST_CHECKED);		
		m_ckSuperMaster.SetCheck(TRUE);
	}
	// [수호천사] 2004.07.08
	else if(GameItem_IsUsingItem(g_MyGameItem, ITEM_SCODE_SAFEGUARDANGEL, &bExist, &st)) {
	//	m_bSuperMaster = TRUE;
		m_bSuperRoomMaster = TRUE;
		m_bHaveSupermaster = TRUE;
	//	CheckDlgButton(IDC_CHECK_SUPERMASTER, BST_CHECKED);
		m_ckSuperMaster.SetCheck(TRUE);
	}
	/////////////////////////////////////////
	// 최소 머니설정
	INT64 Money = GetLevelToUserMoney( m_nUserIcon ); // 유저레벨 -> 머니
	INT64 MinMoney=0;
	CString strM;
	m_MinMCombo.AddString("제한없음");
	for(int i=1; i<6; i++)
	{
		MinMoney = Money * GetCreateRoomGrade(i) / 10; // 방만들기시 최소머니 설정등급
		strM = NumberToOrientalString(MinMoney);
		strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
		m_MinMCombo.AddString(strM.operator LPCTSTR());
	}
	m_MinMCombo.SetCurSel(0);
	ComboSelCreateRGrade = 0; // 제한 없음
		
	rt.SetRect(102,130,278,139);
	m_MinMCombo.MoveWindow(rt);
//	if( m_bSuperMaster ) GetDlgItem(IDC_COMBO_MINIM)->EnableWindow(TRUE);
	if(m_bSuperRoomMaster) GetDlgItem(IDC_COMBO_MINIM)->EnableWindow(TRUE);
	
	/////////////////////////////////////////


	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 296;
	rc1.bottom = rc1.top + 231;
	MoveWindow( &rc1 );

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateRoomDlg::OnOK() 
{
	// TODO: Add extra validation here
	Sound.Play(SND45);

	UpdateData(TRUE);

	// 2004_01_27 비공개방 콤보 박스 제거
	if( m_Pass.GetLength() > 0 ) {bSecret  = TRUE;}

	if(m_RoomTitle.GetLength()==0) { g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"제목을 입력하세요"); return; }
	if(bSecret && m_Pass.GetLength()==0) { g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"비밀번호를 입력하세요"); return; }
	if(bSecret && m_Pass.GetLength()>4) { g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"비밀번호가 너무 깁니다"); return; }


		// 욕설 검사
	if(BadChat.IsBad(m_RoomTitle)==TRUE){
		g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)g_StrMan.Get(_T("BAD_CHAT2")).operator LPCTSTR() );
		GetDlgItem(IDC_EDIT_ROOMTITLE)->SetFocus();
		return;
	}


//	Level = m_LevelCombo.GetCurSel();
//	GameKind = m_GameKindCombo.GetCurSel();
	
	CDialog::OnOK();
}

void CCreateRoomDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	Sound.Play(SND45);
	CDialog::OnCancel();
}

void CCreateRoomDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_HollowBrush.DeleteObject();
	Back.DeleteObject();
	Font1.DeleteObject();
	Font2.DeleteObject();
}

void CCreateRoomDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	
	cdc.DeleteDC();
	::DeleteObject(h);
	h = NULL;
/*
	int nX =  24;
	int nY = 110;
	
	
	dc.SetBkMode(TRANSPARENT);

	switch(g_nChannelStyle)
	{
	case 0: // 자유채널
		{
			CString str;
			dc.SelectObject(&Font2);
			dc.SetTextColor(RGB(255,255,255));
			str = g_StrMan.Get(_T("RS_FREE"));

			// [게임 아이템 작업] [점프 아이템 작업]
			if(g_pMainDlg->m_nItemServer) str = g_StrMan.Get(_T("SPECIAL_SERVER"));
			
			dc.TextOut(nX+17, nY, str.operator LPCTSTR());
			
			str = g_StrMan.Get(_T("RS_QUATERBET"));
			dc.TextOut(nX+8, nY+30, str.operator LPCTSTR());

			str = g_StrMan.Get(_T("RS_1RAISE"));
			dc.TextOut(nX+8, nY+52, str.operator LPCTSTR());
		//	rect.SetRect(0,0,170,70);
		//	rect.OffsetRect(180, 45);
		//	str.Format("%s", "UI.Desc");
		//	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);
		}
		break; 

	case 1: // 초보채널
		{
			CString str;
			dc.SelectObject(&Font2);
			//dc.SetTextColor(RGB(255,255,255));
			dc.SetTextColor(RGB(0,0,0));
			str = g_StrMan.Get(_T("RS_BASIC"));
			dc.TextOut(nX+17, nY, str.operator LPCTSTR());
			
			str = g_StrMan.Get(_T("RS_QUATERBET"));
			dc.TextOut(nX+8, nY+30, str.operator LPCTSTR());

			str = g_StrMan.Get(_T("RS_RAISE1"));
			dc.TextOut(nX+8, nY+52, str.operator LPCTSTR());

			str = g_StrMan.Get(_T("SCHOOL"));

			dc.TextOut(nX+8, nY+74, str.operator LPCTSTR());

		}
		break; 
	
	case 2: // 중수 채널
		{
			CString str;
			dc.SelectObject(&Font2);
			//dc.SetTextColor(RGB(255,255,255));
			dc.SetTextColor(RGB(0,0,0));
			str = g_StrMan.Get(_T("RS_MIDDLE"));
			dc.TextOut(nX+17, nY, str.operator LPCTSTR());
			
			//str = g_StrMan.Get(_T("RS_HALFBET"));
			str = g_StrMan.Get(_T("RS_QUATERBET"));
			dc.TextOut(nX+8, nY+30, str.operator LPCTSTR());

			str = g_StrMan.Get(_T("RS_RAISE2"));
			dc.TextOut(nX+8, nY+52, str.operator LPCTSTR());

			str = g_StrMan.Get(_T("SCHOOL_1000"));
			dc.TextOut(nX+8, nY+74, str.operator LPCTSTR());
		
		}
		break; 
	
	case 3: // 고수채널
		{
			CString str;
			dc.SelectObject(&Font2);
			//dc.SetTextColor(RGB(255,255,255));
			dc.SetTextColor(RGB(0,0,0));
			str = g_StrMan.Get(_T("RS_HIGH"));
			dc.TextOut(nX+17, nY, str.operator LPCTSTR());
			
			//str = g_StrMan.Get(_T("RS_HALFBET"));
			str = g_StrMan.Get(_T("RS_HALFBET"));
			dc.TextOut(nX+8, nY+30, str.operator LPCTSTR());

			//str = g_StrMan.Get(_T("RS_2RAISE"));
			str = g_StrMan.Get(_T("RS_RAISE3"));
			dc.TextOut(nX+8, nY+52, str.operator LPCTSTR());

			str = g_StrMan.Get(_T("SCHOOL"));
			dc.TextOut(nX+8, nY+74, str.operator LPCTSTR());
		
		}
		break; 
	
	case 4: // 도신채널
		{
			CString str;
			dc.SelectObject(&Font2);
			//dc.SetTextColor(RGB(255,255,255));
			dc.SetTextColor(RGB(0,0,0));
			str = g_StrMan.Get(_T("RS_GOD"));
			dc.TextOut(nX+17, nY, str.operator LPCTSTR());
			
			str = g_StrMan.Get(_T("RS_HALFBET"));
			dc.TextOut(nX+8, nY+30, str.operator LPCTSTR());

			//str = g_StrMan.Get(_T("RS_2RAISE"));
			str = g_StrMan.Get(_T("RS_RAISE4"));
			dc.TextOut(nX+8, nY+52, str.operator LPCTSTR());

			str = g_StrMan.Get(_T("SCHOOL_1000"));
			dc.TextOut(nX+8, nY+74, str.operator LPCTSTR());

		}
		break; 

	}
	*/

	// Do not call CDialog::OnPaint() for painting messages
}

void CCreateRoomDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CCreateRoomDlg::OnSelchangeComboSecret() 
{
	// TODO: Add your control notification handler code here
	if(m_SecretCombo.GetCurSel()==0)
	{
		bSecret = FALSE;
		GetDlgItem(IDC_EDIT_ROOMPASS)->EnableWindow(FALSE);
	}
	else
	{
		bSecret = TRUE;
		GetDlgItem(IDC_EDIT_ROOMPASS)->EnableWindow(TRUE);
	}
}

void CCreateRoomDlg::OnSelchangeComboMaxnum() 
{
	// TODO: Add your control notification handler code here
	if(m_nPlayer==0) {
		if(m_MaxNum.GetCurSel()==0) nMaxNum = 3;
		else if(m_MaxNum.GetCurSel()==1) nMaxNum = 4;
		else if(m_MaxNum.GetCurSel()==2) nMaxNum = 5;
		else nMaxNum = 5;
	}
	else {
		if(m_MaxNum.GetCurSel()==0) nMaxNum = 4;
		else if(m_MaxNum.GetCurSel()==1) nMaxNum = 5;
		else if(m_MaxNum.GetCurSel()==2) nMaxNum = 6;
		else nMaxNum = 6;
	}
}

void CCreateRoomDlg::OnSelchangeComboLevel() 
{
	// TODO: Add your control notification handler code here
}


void CCreateRoomDlg::OnSelchangeComboFormkind() 
{
	// TODO: Add your control notification handler code here
	if(m_FormKindCombo.GetCurSel() == 0) 
		FormKind = 0;
	else
		FormKind = 1;
	
}

HBRUSH CCreateRoomDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CCreateRoomDlg::OnCheckSupermaster() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if( m_bSuperRoomMaster )
		GetDlgItem(IDC_COMBO_MINIM)->EnableWindow(TRUE);
	else {
		GetDlgItem(IDC_COMBO_MINIM)->EnableWindow(FALSE);
		ComboSelCreateRGrade = 0;
		m_MinMCombo.SetCurSel(0);
	}
/*
	if( m_bSuperMaster )
		GetDlgItem(IDC_COMBO_MINIM)->EnableWindow(TRUE);
	else {
		GetDlgItem(IDC_COMBO_MINIM)->EnableWindow(FALSE);
		ComboSelCreateRGrade = 0;
		m_MinMCombo.SetCurSel(0);
	}
*/	
}

void CCreateRoomDlg::OnSelchangeComboMinim() 
{
	// TODO: Add your control notification handler code here
	
	ComboSelCreateRGrade = m_MinMCombo.GetCurSel(); // 제한 없음
	
}

void CCreateRoomDlg::OnRadioUsercount5() 
{
	// TODO: Add your control notification handler code here
	m_nPlayer = 0;
	
	m_MaxNum.ResetContent();
	m_MaxNum.AddString("3");
	m_MaxNum.AddString("4");
	m_MaxNum.AddString("5");

	nMaxNum=5/*g_Max_Player*/;
	m_MaxNum.SetCurSel(2);
}

void CCreateRoomDlg::OnRadioUsercount6() 
{
	// TODO: Add your control notification handler code here
	m_nPlayer = 1;
	
	m_MaxNum.ResetContent();
	m_MaxNum.AddString("4");
	m_MaxNum.AddString("5");
	m_MaxNum.AddString("6");

	nMaxNum=6/*g_Max_Player*/;
	m_MaxNum.SetCurSel(2);
}

BOOL CCreateRoomDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(wParam == IDC_BTN_IMGCHECK){
		
	//	if(m_bHaveSupermaster == TRUE)
		{
			if(m_ckSuperMaster.GetCheck() == TRUE)
			{
			//	m_bSuperMaster = TRUE;
				m_bSuperRoomMaster = TRUE;		
				
			}
			else 
			{
			//	m_bSuperMaster = FALSE;
				m_bSuperRoomMaster = FALSE;
			}
		}
	//	else 
		{
		//	if(m_bSuperRoomMaster == FALSE)
			{
		//		m_ckSuperMaster.SetCheck(FALSE);
		//		m_bSuperRoomMaster = FALSE;
			//	m_bSuperMaster = FALSE;
			}
		}
	}
	
	return CDialog::OnCommand(wParam, lParam);
}
