RegistTrayIcon// 62CutPokerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "62CutPokerDlg.h"
#include "Global.h"
#include "ProfileDlg.h"
#include "RecvInviteDlg.h"
#include "RefuseInviteDlg.h"
#include "RoomInfoDlg.h"
#include "ConfigDlg.h"
#include "MyVoteDlg.h"
#include "LoginDlg.h"
#include "StartDlg.h"
#include "ItemListDlg.h"	// [게임 아이템 작업]
#include "AskBuyItemDlg.h"	// [게임 아이템 작업]
#include "SelCardDlg.h"
#include <atlbase.h>
#include "ShortCut.h"
//#include "Oring.h"			// [ 올인잡업 ]
#include "DesktopIconMan.h"
#include "NMCrypt.h"
#include <assert.h>

#include "ResultFindIdDlg.h"
#include "LeadersMoneyLimit.h"
#include "SysUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


// [SSO 작업] - 기존 함수 교체
char* _StrTok(const char* str, const char sep)
{
	// strtok()와 같은 기능의 함수이지만 separator를 1개만 받고
	// 인자가 비어있는 경우도 인자로 인정함

	static const int TOK_BUFSIZE = 1024*40;

	static char* pnow = NULL;
	static char buf[TOK_BUFSIZE];
	static char token[TOK_BUFSIZE];

	if(str) {
		int slen = strlen(str);
		assert(slen < TOK_BUFSIZE);		// ASSERT
		strcpy(buf, str);
		pnow = buf;
	}

	if(!pnow || *pnow == 0)
		return NULL;

	int tlen = 0;
	char* porg = pnow;
	while(*pnow != 0)
	{
		if(*pnow == sep) {
			++pnow;
			break;
		}

		++tlen;
		++pnow;
	}

	if(tlen > 0) {
		strncpy(token, porg, tlen);
		token[tlen] = 0;
	}
	else {
		token[0] = 0;
	}

	return token;
}



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerDlg dialog

C62CutPokerDlg::C62CutPokerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C62CutPokerDlg::IDD, pParent)
{
	g_pMainDlg = this;
	hTimerBadUser = NULL;
	bNowLogout = FALSE;
	ZeroMemory(&CFG, sizeof(CONFIG));

	m_bExamServer = FALSE;
	m_nItemServer = 0;	

	//{{AFX_DATA_INIT(C62CutPokerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C62CutPokerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C62CutPokerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(C62CutPokerDlg, CDialog)
	//{{AFX_MSG_MAP(C62CutPokerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_PACKET_NOTIFY, OnPacketNotify)
	ON_WM_MOVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_ALLINMESSAGE,OnAllinMessage) // [수호천사] 2004.07.08
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerDlg message handlers

BOOL C62CutPokerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);


	CString str;
	float gamever = (float)VERSION/100; 
	str.Format("Good Boy 7 Poker Ver%2.2f", gamever);
	SetWindowText(str);

	// 아바타 디스플레이 모듈 초기화
	//InitAvatarModule();
	//InitAvatarModule_ForTest("E:\\work\\board\\poker\\_BIN\\Client", "E:\\work\\board\\poker\\_BIN\\Client\\AvatarDisplay.dll");
	InitAvatarModule_ForTest(".\\Avatar", ".\\Avatar\\AvatarDisplay.dll");



	// 접속 메세지 표기
	m_ConnectMsgDlg.MsgBox(0, "메인서버에 접속중입니다");
	
	// 스프라이트 파일들을 읽어들임
	g_BgMode = 0;	// 디폴트 배경 로드 
	LoadSpriteFile();// ### [ 관전기능 ] ###
	load_spritefile_7();
	
	// 게임 윈도우와 방목록 윈도우를 만들어서 적당한 위치로 옮긴다
	CRect rect;
	//GetClientRect(rect);
	rect.SetRect(0,0,800, 600);
	
	if(m_TitleDlg.Create(IDD_TITLE_DIALOG, this) == FALSE) AfxMessageBox("Create failed on create Title Dialog");
	if(m_LobyDlg.Create(IDD_LOBY_DIALOG, this) == FALSE) AfxMessageBox("Create failed on create Loby Dialog");
	if(m_GameDlg.Create(IDD_GAME_DIALOG, this) == FALSE) AfxMessageBox("Create failed on create Game Dialog");
	
	// [관리자 모드 작업] 관리자 메시지 보내기
	if(AMesDlg.GetSafeHwnd()==NULL){
		if(AMesDlg.Create(IDD_ADMIN_MESSAGE,this) == FALSE) AfxMessageBox("Create failed on create Admin Message Dialog");
		else AMesDlg.ShowWindow(SW_HIDE);
	}

	m_TitleDlg.MoveWindow(rect, FALSE);
	m_LobyDlg.MoveWindow(rect, FALSE);
	m_GameDlg.MoveWindow(rect, FALSE);
	m_TitleDlg.ShowWindow(SW_SHOW);	

	// 타이틀 바 제거후 아이콘 보이기
	this->ModifyStyle(0, WS_SYSMENU,0 );//| WS_MINIMIZE | WS_MAXIMIZE,0);

	// 오리지날 사이즈를 구한다
	GetWindowRect(g_OrgMainDlgRect);
#ifndef _DEBUG
	// 릴리즈 모드이면 윈도우 사이즈를 0으로 만들어 숨긴다
	MoveWindow(0,0,0,0);
#endif

	//////////////////////
	// 통신 메세지 처리 클래스 초기화
	int rtn = m_SockCmnd.Init(this);
	if(rtn==FALSE)
	{
		AfxMessageBox("소켓 초기화에 실패하였습니다");
		// 프로그램 종료
		m_ConnectMsgDlg.OnCancel();
	}
	Sound.Init(m_hWnd);// 다이렉트 사운드 초기화
	srand((unsigned)timeGetTime());  // 랜덤 초기화

	// ## 바로가기 아이콘 ##
//	CreateShortCut();

	// 홈페이지 자동 띄우기
	//char strDir[500]={0,};
	//GetCurrentDirectory(500, strDir);
	//CString runstr;
	//runstr.Format("%s\\Homepage.url", strDir);
	//::ShellExecute(NULL, "open", runstr.operator LPCTSTR(), NULL, ".", SW_SHOWNORMAL);

//###버그를잡아라
#ifdef _DEBUG


	CLoginDlg LoginDlg;
	LoginDlg.m_ID = "bbbb";					// 플레이어 로그인 - jeong
	if(LoginDlg.DoModal()==IDOK) {
		g_LoginID = LoginDlg.m_ID;
		g_LoginPass = LoginDlg.m_Pass;
		g_ServIP = LoginDlg.m_ServerIP;
		g_ServPort = LoginDlg.m_ServerPort;
	}
	else {
		// 프로그램 종료
		m_ConnectMsgDlg.OnCancel();
		return FALSE;
	}
	

#endif

	// 욕설 필터링 데이터 읽기
	//char strbuf[1024] = {0,};
	char strbuf[1024] = "Avatar";
	//GetBaseAvatarDir(strbuf, 1023);
	CString filterfile = strbuf;
	filterfile += "\\filter.bcd";
	if(!BadChat.Open(filterfile)) {
		AfxMessageBox("프로그램 실행에 필요한 파일을 읽을 수 없습니다.\n프로그램을 재설치하시기 바랍니다.");
		// 프로그램 종료
		m_ConnectMsgDlg.OnCancel();
		return FALSE;
	}	

	// 메인서버와 접속 시도
	g_MainSrvSID = SockMan.ConnectSocket((char*)g_ServIP.operator LPCTSTR(), g_ServPort);
	g_Where = WH_LOGIN;

	CDesktopIconMan icon;
	// CString ProgName, CString url, CString ShortcutName, CString Desc
	icon.Check( "Good Boy 7 Poker", "http://www.goodboy7poker.com", "NewPoker", "Good Boy 7 Poker" );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void C62CutPokerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void C62CutPokerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR C62CutPokerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void C62CutPokerDlg::OnOK() 
{
	// TODO: Add extra validation here
	//CDialog::OnOK();
}

void C62CutPokerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	if(g_Where==WH_GAME)
	{
		#ifdef _DEBUG
			g_pGameDlg->ExitGame();
			return;
		#else
			//g_pGameDlg->SendMessage(WM_COMMAND, IDC_BUTTON_EXITGAME, 0); // 다이얼 로그버튼 사용시
			g_pGameDlg->SendMessage(WM_COMMAND, IDM_EXIT, 0);

			return;
		#endif
	}

#ifndef _DEBUG
	if(g_Where == WH_LOBY) {
		g_pLobyDlg->PostMessage(WM_COMMAND, IDC_BUTTON_LOGOUT, 0);
		return;
	}
#endif

	//if(g_Where != WH_GAME && g_Where != WH_LOBY) bNowLogout=TRUE;
	if(g_Where == WH_FIRST || g_Where == WH_LOGIN) bNowLogout=TRUE;

	static BOOL bShowMsgDlg = FALSE;
	if(bShowMsgDlg==FALSE && bNowLogout == FALSE)
	{
		bShowMsgDlg = TRUE;
		m_LogoutMsgDlg.MsgBox(2, "프로그램을 종료하는 중입니다.");
		
		g_Wait = TRUE;
		
		// ### [ 관전기능 ] ###
		CSV_ASK_LOGOUT alomsg;
		//alomsg.Set(Play[0].UI.UNum, Play[0].UI.ID);
		alomsg.Set(g_MyInfo.UI.UNum, g_MyInfo.UI.ID);
		SockMan.SendData(g_MainSrvSID, alomsg.pData, alomsg.GetTotalSize());
	}

	KillProcess();

	// 로그아웃이 허용되었을 때만 프로그램을 종료한다
	if(bNowLogout == FALSE) return;

	CDialog::OnCancel();
}
void C62CutPokerDlg::KillProcess()
{
	::ShellExecute(NULL, "open", "end.bat", NULL, ".", SW_HIDE);
}
// [SSO 작업] - 기존 함수 교체
BOOL C62CutPokerDlg::AnalyzeArgument(char *key)
{
//
	// 클립보드 인자 전달 규칙(각각의 인자는 컴마(,)로 구분되며 인자 사이의 공백은 없다)
	// 전달 인자의 순서 : ServerIP,Port,AuthCookie,DataCookie,CpCookie,SpareParam (인자는 암호화되어 있다)
	// - 넷마블 게임에서는 CpCookie를 전달하지 않는다
	// - CP 게임에서는 AuthCookie, DataCookie를 전달하지 않는다(몇몇 CP게임은 예외)
	// - Extra 인자는 생략되기도 한다
	//

	char buf[1024*20] = {0,};	// 버퍼는 충분히 크게 잡는다

	// 클립보드에서 인자 데이터를 읽어온다(한번 얻어오면 삭제됨)
	if(!GetNMClipData(buf, sizeof(buf)-1, key, true))
		return FALSE;

	if(strlen(buf)==0)
		return FALSE;

	char arg[6][1024*2] = {0,};

	int argcnt = 0;
	char* token = _StrTok(buf, ',');
	while(token && argcnt < 6)
	{
		strcpy(arg[argcnt], token);
		argcnt++;
		token = _StrTok(NULL, ',');
	}

	// 인자가 최소한 5개는 되어야 한다
	if(argcnt < 5)
		return FALSE;

	// IP주소가 올바른지 검사
	if(strlen(arg[0]) > 16)
		return FALSE;

	// CP 게임은 아래부분을 고쳐 쓸것
	g_ServIP     = arg[0];					// 서버 IP
	g_ServPort   = (UINT)::atoi(arg[1]);	// 서버 Port
	g_AuthCookie = arg[2];					// 인증 쿠키	- 버퍼 크기를1024*2 정도로 잡아야 한다
	g_DataCookie = arg[3];					// 데이터 쿠키	- 버퍼 크기를1024*2 정도로 잡아야 한다
	g_CpCookie   = arg[4];					// CP쿠키		- 버퍼 크기를1024*2 정도로 잡아야 한다
	g_SpareParam = arg[5];					// 여분의 인자(CP게임등에서 사용)
	
	return TRUE;
}


#include "SysUtil.h"
BOOL C62CutPokerDlg::LoadConfig(LPCTSTR id)
{
	if(id == NULL || strlen(id) == 0) 
		return FALSE;
	
	memset(&CFG, 0, sizeof(CFG));
	
#ifndef _DEBUG
	// 릴리즈 모드인 경우 기본을 풀스크린 모드로 설정한다
	CFG.nScreenMode       = GetProfile_int(id, "ScreenMode",       0);
#else
	// 디버그 모드인 경우 기본을 창모드로 설정한다
	CFG.nScreenMode       = GetProfile_int(id, "ScreenMode",       0);
#endif
	CFG.bOffSndFX         = GetProfile_int(id, "OffSndFX",         0);
	CFG.bOffMusic         = GetProfile_int(id, "OffMusic",         0);
	CFG.bNoInvite         = GetProfile_int(id, "NoInvit",          0);
	CFG.bNoWhisper        = GetProfile_int(id, "NoWhisper",        0);
	CFG.bFailToFullScreen = GetProfile_int(id, "FailToFullScreen", 0);
	CFG.bHideOtherBlock   = GetProfile_int(id, "HideOtherBlock",   0);
	// ### [ 관전기능 ] ###
	if(g_MyInfo.UI.Sex == 1)
		CFG.SndFxKind	  = GetProfile_int(id, "SndFxKind",   0);
	else
		CFG.SndFxKind	  = GetProfile_int(id, "SndFxKind",   3);


	///메크로 
	memset(&g_ChatMacro, 0, sizeof(g_ChatMacro));	
	
	char fname[MAX_PATH];
	//메크로 로드	
	wsprintf(fname, ".\\config\\%s.mcr",id);	

	// 환경설정 파일을 연다
	FILE *fp = fopen( fname, "rb");

	if( fp == NULL ) return FALSE;
	fread(&g_ChatMacro, sizeof(g_ChatMacro), 1, fp);
	fclose(fp);

	return TRUE;
}

BOOL C62CutPokerDlg::SaveConfig(LPCTSTR id)
{
	if(id == NULL || strlen(id) == 0) 
		return FALSE;

	WriteProfile_int(id, "ScreenMode",       CFG.nScreenMode);
	WriteProfile_int(id, "OffSndFX",         CFG.bOffSndFX);
	WriteProfile_int(id, "OffMusic",         CFG.bOffMusic);
	WriteProfile_int(id, "NoInvit",          CFG.bNoInvite);
	WriteProfile_int(id, "NoWhisper",        CFG.bNoWhisper);
	WriteProfile_int(id, "FailToFullScreen", CFG.bFailToFullScreen);
	WriteProfile_int(id, "HideOtherBlock",   CFG.bHideOtherBlock);
	WriteProfile_int(id, "SndFxKind",		 CFG.SndFxKind);

	
	//메크로 로드
	char fname[MAX_PATH];	
	wsprintf(fname, ".\\config\\%s.mcr",id);	

	// 환경설정 파일을 연다
	FILE *fp = fopen( fname, "wb");

	if( fp == NULL ) return FALSE;
	fwrite(&g_ChatMacro, sizeof(g_ChatMacro), 1, fp);
	fclose(fp);

	return TRUE;
}


void C62CutPokerDlg::SetupByConfig()
{
	// 사운드 설정 적용
	if(Sound.bDontUseSndFx != CFG.bOffSndFX) Sound.SetSndFx(CFG.bOffSndFX ^ 1);
	if(Sound.bDontUseMusic != CFG.bOffMusic) {
		Sound.SetMusic(CFG.bOffMusic ^ 1);
	}

	if(CFG.bFailToFullScreen) {
		// 풀스크린이 성공하는지 여부를 체크하기 위함
		CFG.nScreenMode = 0;
		CFG.bFailToFullScreen = FALSE;
		// ### [ 관전기능 ] ###
		SaveConfig(g_MyInfo.UI.ID);
		MessageBox("지난번 실행시 풀스크린 모드로 화면 전환에 실패하였기때문에 창모드로 실행합니다.", "알림", MB_OK);
	} else {
		// 화면 모드 설정 적용
		if(CFG.nScreenMode==1) {
			// 풀스크린이 성공하는지 여부를 체크하기 위함
			CFG.bFailToFullScreen = TRUE;
			// ### [ 관전기능 ] ###
			SaveConfig(g_MyInfo.UI.ID);

			g_pLobyDlg->bDontClickSound = TRUE;
			g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_CHANGEDISPLAY);
			g_Mini.SendMessage(WM_COMMAND, IDC_BUTTON_CHANGEDISPLAY);

			// 풀스크린이 성공하였기때문에 원래값으로 초기화함
			CFG.bFailToFullScreen = FALSE;
			// ### [ 관전기능 ] ###
			SaveConfig(g_MyInfo.UI.ID);
		}
	}

	// 부가 기능 설정 적용
	if(CFG.bNoInvite) {
		CButton* pCheck = (CButton*)g_pLobyDlg->GetDlgItem(IDC_CHECK_REFUSEINVITE);
		pCheck->SetCheck(TRUE);
	}
	if(CFG.bNoWhisper) {
		CButton* pCheck = (CButton*)g_pLobyDlg->GetDlgItem(IDC_CHECK_REFUSEWHISPER);
		pCheck->SetCheck(TRUE);
		g_pLobyDlg->PostMessage(WM_COMMAND, IDC_CHECK_REFUSEWHISPER);
	}


	// ### [사운드 추가 작업] ###
	// ### [ 관전기능 ] ###
	if(g_MyInfo.UI.Sex == 1) // 남자
	{
		if(CFG.SndFxKind >= 3 ) CFG.SndFxKind = 0; // 0 ~ 2
	}
	else 
	{
		if(CFG.SndFxKind < 3 ) CFG.SndFxKind = 3; // 3 ~ 5
	}

	// ### [ 관전기능 ] ###
	g_MyInfo.nSndFxKind = CFG.SndFxKind;
	Play[0].nSndFxKind  = CFG.SndFxKind;
}

void C62CutPokerDlg::ResetBadUserTimer()
{
	KillBadUserTimer();
	hTimerBadUser = SetTimer(BADUSER_TIMER, MAX_BADUSERDELAYTIME, NULL);
}

void C62CutPokerDlg::KillBadUserTimer()
{
	if(hTimerBadUser)
	{
		KillTimer(hTimerBadUser);
		hTimerBadUser = NULL;
	}
}

void C62CutPokerDlg::ChangeView(int vnum)
{
	(vnum==0)? m_TitleDlg.ShowWindow(SW_SHOW) : m_TitleDlg.ShowWindow(SW_HIDE);
	//(vnum==1)? m_ChannelDlg.ShowWindow(SW_SHOW) : m_ChannelDlg.ShowWindow(SW_HIDE);
	(vnum==2)? m_LobyDlg.ShowWindow(SW_SHOW) : m_LobyDlg.ShowWindow(SW_HIDE);
	(vnum==3)? m_GameDlg.ShowWindow(SW_SHOW) : m_GameDlg.ShowWindow(SW_HIDE);

	CString str, str2;
	static BOOL bFirstLoby=TRUE;
	static INT64 FirstMarble=0;
	static INT64 PrevMarble=0;
	
	// 제거
	//g_pTitleDlg->DestoryWebCtrl();
	g_pTitleDlg->DestroyBanner();	// <- 추가


	switch(vnum)
	{
	case 0:
		{
			float gamever = (float)VERSION/100; 
			str.Format("%s Ver%2.2f - [%s]","Good Boy 7 Poker", gamever, g_ServerName);
			SetWindowText(str);
			bFirstLoby = TRUE;
			g_pTitleDlg->m_TitleMsg = str;

			g_pTitleDlg->CreateBanner();
			if(g_NoticeURL.GetLength()>0) g_pTitleDlg->m_pWebCtrl->Navigate(g_NoticeURL);

		} break;
	case 1: 
		{
			float gamever = (float)VERSION/100; 
			str.Format("%s Ver%2.2f","Good Boy 7 Poker", gamever);
			SetWindowText(str);
		} break;
	case 2: 
		{		
			// 대기실 채팅창 및 채팅 에디트창 초기화
			m_LobyDlg.m_ChatViewEdit.SetWindowText("");
			m_LobyDlg.m_SendEdit.SetWindowText("");

			// 메인 타이틀 수정 - jeong
			float gamever = (float)VERSION/100; 
			//str.Format("%s Ver%2.2f - [%s]", g_StrMan.Get(_T("GAME_TITLE")), gamever, g_ChannelName);
			str.Format("%s Ver%2.2f - [%s]", "Good Boy 7 Poker", gamever, g_ChannelName);
			g_pLobyDlg->m_LobyMsg=str;
			

			SetWindowText(str);

			if(bFirstLoby == TRUE) {
				// ### [ 관전기능 ] ###
				FirstMarble = g_MyInfo.UI.PMoney;
				bFirstLoby = FALSE;
			}
			else
			{
				// ### [ 관전기능 ] ###
				INT64 TotEarn = g_MyInfo.UI.PMoney - FirstMarble;
				INT64 PrevEarn = g_MyInfo.UI.PMoney - PrevMarble;
		
				if(TotEarn >= 0)
				{
					//str.Format("★접속후 총 +%I64d 원을 획득하셨습니다\n", TotEarn);
					str.Format(g_StrMan.Get(_T("STATE_MES1")), g_MakeCommaMoney(TotEarn));
					g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(0,180,0));
				}
				else
				{
					//str.Format("★접속후 총 %I64d 원을 잃으셨습니다\n", TotEarn);
					str.Format(g_StrMan.Get(_T("STATE_MES2")), g_MakeCommaMoney(TotEarn));
					g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(180,0,0));
				}

				if(PrevEarn >= 0)
				{
					//str.Format("★방금 계셨던 방에서 +%I64d 원을 획득하셨습니다\n", PrevEarn);
					str.Format(g_StrMan.Get(_T("STATE_MES3")), g_MakeCommaMoney(PrevEarn));
					g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(0,180,0));
				}
				else
				{
					//str.Format("★방금 계셨던 방에서 %I64d 원을 잃으셨습니다\n", PrevEarn);
					str.Format(g_StrMan.Get(_T("STATE_MES4")), g_MakeCommaMoney(PrevEarn));
					g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(180,0,0));
				}

				ShowItemState();	// [게임 아이템 작업]
			}

			//### [관리자 모드 작업] ###
			if(g_MyInfo.UI.cAdminUser == 1) {
				str.Format("### [%s]님은 관리자모드로 접속하셨습니다. ### \n", g_MyInfo.UI.ID);
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(255,0,0));
			}

			m_LobyDlg.m_SendEdit.SetFocus();
			// 음악 끄기
//			Sound.AllMusicStop();
		} break;
	case 3: 
		// 메인 타이틀 텍스트 수정
		// 0 ~ 4 까지 0 자유 , 1 초보, 2 중수, 3 고수, 4 도신
		CString rlevel,style;
		rlevel = g_StrMan.Get(_T("NUM_1"));
		
		if(g_RI.GameKind == 2) style = g_StrMan.Get(_T("FULL"));
		else if(g_RI.GameKind == 1) style = g_StrMan.Get(_T("HALF"));
		
		else style = g_StrMan.Get(_T("QUATER"));
		CString str;
		str.Format(g_StrMan.Get(_T("ROOM_TITLE")),
			"Good Boy 7 Poker", g_ChannelName, g_RI.RoomNum, g_RI.Title, g_RI.ID,style,
			g_RI.Secret==0?g_StrMan.Get(_T("L_OPEN")):g_StrMan.Get(_T("L_PASS")), g_StrMan.Get(_T("WHISNO")));
		SetWindowText(str);
		/////////////////////////
		str.Format("[%s]-[%d번방][%s][%s][%s]", g_ChannelName, g_RI.RoomNum, g_RI.Title, g_RI.Secret==0?g_StrMan.Get(_T("L_OPEN")):g_StrMan.Get(_T("L_PASS")), g_RI.FormKind==0?"Normal":"Choice");
		g_pGameView->m_titlestr="";//str;


	//	CString str2="";
	//	str2.Format("[%s][%s][%s]", style,  g_RI.Secret==0?g_StrMan.Get(_T("L_OPEN")):g_StrMan.Get(_T("L_PASS")), g_RI.FormKind==0?"Normal":"Choice");
	//	g_pGameView->ChatBox.m_MsgTitle=str2;
		/////////////////////////
		// ### [ 관전기능 ] ###
		PrevMarble = g_MyInfo.UI.PMoney;

		g_pGameView->ChatBox.ChatViewEdit.SetWindowText(""); // 채팅창
		g_pGameView->ChatBox.SendEdit.SetWindowText("");
		// 포커스
		g_pGameView->SetFocus();
		m_GameDlg.m_GameView.Reset();

		if(g_RI.bPlayer==0)
			str.Format("☆도움말☆\n1. 게임창->채팅창 포커스 이동 단축키 - Enter, ESC\n2. 채팅창->게임창 포커스 이동 단축키 - ESC\n3. 채팅창<->족보 이동 단축기 - Tab\n");
		else 
			str.Format("☆도움말☆\n1. 게임창->채팅창 포커스 이동 단축키 - Enter, ESC\n2. 채팅창->게임창 포커스 이동 단축키 - ESC\n");
		
		g_pGameView->ChatBox.AddText(&str, NOTIC_COLOR);//RGB(255,100,200));
		
		// 족보등급 보기
		Game.SetChangeViewTabBtn(); //기본 채팅창
		break;
	}
}

LONG C62CutPokerDlg::OnPacketNotify(UINT wParam, LONG lParam)
{
	GAMENOTIFY *pNotifyData = (GAMENOTIFY*)wParam;
	if(!pNotifyData) return 0;

	int Sid = pNotifyData->Sid;				// 소켓 번호
	int Signal = pNotifyData->Signal;		// 메시지 시그널
	int TotSize = pNotifyData->TotSize;		// 메시지 사이즈
	char *lpData = pNotifyData->lpData;		// 메시지 데이터 포인터

	switch(Signal)
	{
		// [수호천사] 2004.07.08 다른유저 머니복구 알림
		// [수호천사] 2004.07.08 머니복구 알림
	case CL_CHARGE_SAFEANGEL:
		{
			CCL_CHARGE_SAFEANGEL msg;
			msg.Get( lpData, TotSize );

			Game.Charge_Safeangel_money( &msg );
			
		} break;

	case SV_SERVERNOTIC: // 타이틀 공지
		{
			CSV_SERVERNOTIC msg;
			msg.Get(lpData, TotSize);
			
			CString str;
			CString chat;
			chat = (LPCTSTR)msg.StrChat;
			chat = chat.Mid(0, *msg.l_StrChat);
			str.Format("%s", chat.operator LPCTSTR());
			
			if( *msg.Kind==0 ) {
				str += "\n";
				COLORREF tcolor = RGB(255,255,255);
				if(*msg.bColorAssigned) tcolor = *msg.Color;
				
				g_pTitleDlg->m_ChatViewNotic.SetTextFormat(0);
				g_pTitleDlg->m_ChatViewNotic.AddText(&str, tcolor);
							
				int nFirstVisible = g_pTitleDlg->m_ChatViewNotic.GetFirstVisibleLine();
				if (nFirstVisible > 0) 	{
					g_pTitleDlg->m_ChatViewNotic.LineScroll(-nFirstVisible, 0);
				}
				break;
			}
			
		} break;
			// ### [ 관전기능 ] ###
	case SV_OBSERVER_MESSAGE:
		{
			CSV_OBSERVER_MESSAGE msg;
			msg.Get(lpData, TotSize);

			if(g_Where != WH_GAME) break;

			CString str;
			CString chat;
			chat = (LPCTSTR)msg.StrChat;
			chat = chat.Mid(0, *msg.l_StrChat);
			str.Format("%s", chat.operator LPCTSTR());
			str += "\n";
			g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(255,0,0));

		} break;
		
		// ### [ 관전기능 ] ###
	case SV_RESERVATIONINFO:// 예약자 정보
		{
			CSV_RESERVATIONINFO msg;
			msg.Get(lpData, TotSize);

			if(g_Where != WH_GAME) break;

			for(int i=0; i<*msg.TotNum; i++) {
				char *id = msg.ReservedInfo[i].ID;
				int unum = msg.ReservedInfo[i].UNum;
				int seq =  msg.ReservedInfo[i].Seq;
				g_pGameDlg->m_ObserverDlg.User_SetItemReservation(unum, id, seq);
			}
		} break;
		// ### [ 관전기능 ] ###
		// 겜참여 -> 관전
	case SV_ACCEPT_EMERAGE:
		{
			CSV_ACCEPT_EMERAGE msg;
			msg.Get(lpData, TotSize);

			if(g_Where != WH_GAME) break;
			
			int nowpnum = *msg.NowPNum;
			int newpnum = *msg.NewPNum;

			int MyID = -1;
			for(int i=0; i<g_Max_NewPlayer; i++)	{
				if(strlen(Play[i].UI.ID) > 0) {	
					if(strcmp(g_MyInfo.UI.ID, Play[i].UI.ID)==0) { MyID = i; break; }
				}
			}

			BOOL bTrue = TRUE;
			if(newpnum < g_Max_Player || newpnum >= g_Max_NewPlayer) bTrue = FALSE;
			
			if(MyID == -1 || nowpnum != Play[0].ServPNum || !bTrue) 
			{
				CSV_ASK_OUTROOM aormsg;
				aormsg.Set(g_RI.RoomNum, g_MyInfo.UI.UNum, g_MyInfo.UI.ID);
				SockMan.SendData(g_MainSrvSID, aormsg.pData, aormsg.GetTotalSize());
				break;
			}

			// 플레이어 순서에 맞게 초기화한다			
			Game.SetPlayInfo(nowpnum, newpnum, FALSE);
			
			// ### [ 중복된 IP가 ] ###
			for( i=0; i<g_Max_Player; i++) {
				int pn = Game.GetPNum_ByServPN(i);
				if(strlen(Play[pn].UI.ID) > 0)	{
					Play[pn].MyIpCheck = msg.IPA->IpIndex[i];
				}
			}
		
		} break;
		// ### [ 관전기능 ] ###
		// 관전 -> 겜참여
	case SV_ACCEPT_PARTICIPATION:
		{
			CSV_ACCEPT_PARTICIPATION msg;
			msg.Get(lpData, TotSize);

			int nowpnum = *msg.NowPNum;
			int newpnum = *msg.NewPNum;
			
			int MyID = -1;
			for(int i=0; i<g_Max_NewPlayer; i++)	{
				if(strlen(Play[i].UI.ID) > 0) {	
					if(strcmp(g_MyInfo.UI.ID, Play[i].UI.ID)==0) { MyID = i; break; }
				}
			}

			BOOL bTrue = TRUE;
			if(newpnum < 0 || newpnum >= g_Max_Player) bTrue = FALSE;
			if(MyID == -1 || nowpnum != MyID || !bTrue) 
			{
				CSV_ASK_OUTROOM aormsg;
				aormsg.Set(g_RI.RoomNum, g_MyInfo.UI.UNum, g_MyInfo.UI.ID);
				SockMan.SendData(g_MainSrvSID, aormsg.pData, aormsg.GetTotalSize());
				break;
			}

			// ### [사운드 추가 작업] ###
			Play[MyID].nSndFxKind = *msg.SndFxKind;
			//g_MyInfo.nSndFxKind = *msg.SndFxKind;
			
			// 플레이어 순서에 맞게 초기화한다			
			Game.SetPlayInfo(nowpnum, newpnum, TRUE);
			
			// ### [ 중복된 IP가 ] ###
			for( i=0; i<g_Max_Player; i++) {
				int pn = Game.GetPNum_ByServPN(i);
				if(strlen(Play[pn].UI.ID) > 0) {
					Play[pn].MyIpCheck = msg.IPA->IpIndex[i];
				}
			}
			
		} break;
	
		// ### [ 관전기능 ] ###
	case SV_ENTER_EMERAGE:// 겜참여 -> 관전
		{
			CSV_ENTER_EMERAGE msg;
			msg.Get(lpData, TotSize);
			
			int nowpnum = *msg.NowPNum;
			int newpnum = *msg.NewPNum;

			if(nowpnum < 0 || nowpnum >= g_Max_Player) break;
			if(newpnum < g_Max_Player || newpnum >= g_Max_NewPlayer) break;

			int pn = Game.GetPNum_ByServPN(nowpnum);
			if(strcmp( Play[pn].UI.ID, msg.ID) != 0) break;

			Play[newpnum].SetNewPlayer(&Play[pn].UI);
			Play[newpnum].UI.cObserver = 1;
			//Play[newpnum].nSndFxKind = Play[pn].nSndFxKind;

			Play[pn].Clear();
			g_RI.NowUserNum -= 1;
			g_RI.NowObserverNum += 1;
			
			CString str;
			str.Format("## <%s>님이 게임을 관전 합니다 ##\n", msg.ID);
			g_pGameDlg->m_GameView.ChatBox.AddText(&str, INOUT_COLOR);

			// ### [ 중복된 IP가 ] ###
			for(int i=0; i<g_Max_Player; i++)	{
				int pn = Game.GetPNum_ByServPN(i);
				if(strlen(Play[pn].UI.ID) > 0)	{
					Play[pn].MyIpCheck = msg.IPA->IpIndex[i];
				}
			}
	
			int ounum = Play[newpnum].UI.UNum;
			int sex  = Play[newpnum].UI.Sex;
			int level = Play[newpnum].UI.nIcon;
			INT64 Money = Play[newpnum].UI.PMoney;
			int seq = 1;
			g_pGameDlg->m_ObserverDlg.User_InsItem(ounum, Play[newpnum].UI.ID, sex, level, Money ,seq);
		
		} break;
		
		// ### [ 관전기능 ] ###
	case SV_ENTER_PARTICIPATION:// 관전 -> 겜참여
		{
			CSV_ENTER_PARTICIPATION msg;
			msg.Get(lpData, TotSize);

			if(g_Where != WH_GAME) break;
			int nowpnum = *msg.NowPNum;
			int newpnum = *msg.NewPNum;
			if(newpnum < 0 || newpnum >= g_Max_Player) break;
			if(nowpnum < g_Max_Player || nowpnum >= g_Max_NewPlayer) break;

			//관전의 pnum 인덱스는 서버와 일치한다
			if(strcmp( Play[nowpnum].UI.ID, msg.ID) != 0) break; 
			int i=0;
			int pnum = -1; // 플레이어 번호
			if( !g_MyObserver) // 1) 내가 겜참여중이고 겜참여 넘 입장
			{
				for(i=Game.No1PosPNum; i<g_Max_Player+Game.No1PosPNum; i++) {	
					int index = i;
					if(index>=g_Max_Player) index = index - g_Max_Player;
					if(index==0) continue; //나
					if(strlen(Play[index].UI.ID)==0){pnum=index; break;}
				}
			
				if(pnum == -1) break; // 비어있는 곳을 찾지 못했으면
				
				Play[nowpnum].UI.cObserver = 0;
				Play[pnum].SetNewPlayer(&Play[nowpnum].UI);
				PlayScr[pnum].Reset();
				
				Play[nowpnum].Clear();
				g_RI.NowUserNum += 1;
				g_RI.NowObserverNum -= 1;
				
				Play[pnum].ServPNum = Game.GetServPNum_ByPN(pnum);
#ifdef _DEBUG
				if(newpnum != Play[pnum].ServPNum)
				{
					CString str;
					str.Format("newpnum  =%d ,  pnum = %d, ServPNum = %d", newpnum, pnum, Play[pnum].ServPNum);
					AfxMessageBox(str);
				}
#endif
				Play[pnum].PrevMoney = Play[pnum].UI.PMoney;// +- 증가액 표시
				CString str;
				str.Format("<%s>님이 게임에 참여 합니다.\n", msg.ID);
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, INOUT_COLOR);
			}
			// 3)내가 관전중이고  겜참여 하는넘 입장
			else
			{
				for(i=0; i<g_Max_Player; i++)	{
					if(strlen(Play[i].UI.ID)==0){pnum=i; break;}
				}
				if(pnum == -1) break; // 비어있는 곳을 찾지 못했으면
				
				Game.MyPosPNum = 0; // 서버쪽에 인덱스
				Game.No1PosPNum = Game.GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
				
				// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
				Play[nowpnum].UI.cObserver = 0;
				Play[pnum].SetNewPlayer(&Play[nowpnum].UI);
				PlayScr[pnum].Reset();

				Play[nowpnum].Clear();
				g_RI.NowUserNum += 1;
				g_RI.NowObserverNum -= 1;
								
				Play[pnum].ServPNum = Game.GetServPNum_ByPN(pnum);// 서버측 플레이어 번호 저장
				Play[pnum].PrevMoney = Play[pnum].UI.PMoney;// +- 증가액 표시
				
				CString str;
				str.Format("## ## <%s>님 게임참여 ##\n", msg.ID);
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, INOUT_COLOR);
				
			}
			
			// ### [ 중복된 IP가 ] ###
			for( i=0; i<g_Max_Player; i++) {
				int pn = Game.GetPNum_ByServPN(i);
				if(strlen(Play[pn].UI.ID) > 0)	{
					Play[pn].MyIpCheck = msg.IPA->IpIndex[i];
				}
			}

			// ### [사운드 추가 작업] ###
			Play[pnum].nSndFxKind = *msg.SndFxKind;
			g_pGameDlg->m_ObserverDlg.User_DelItem(Play[pnum].UI.UNum, Play[pnum].UI.ID);
	
		} break;


	case SV_CHECKVERSION: // 버전 체크
		{
			CSV_CHECKVERSION msg;
			msg.Get(lpData, TotSize);
			if(g_MainSrvSID != Sid) break;
			int prev = *msg.PreVersion;
			int ver = *msg.Ver;
			if(ver<0) break;

			// 버전이 틀리면 연결종료
			if(ver != VERSION || prev != VERSION+5)// VERSIOM 값 변경 가능하다면 다른 방법
			{
				PostMessage(UM_DATA_CLOSE, NULL, (long)SockMan.pDataSock[g_MainSrvSID]); 
				g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"프로그램의 버전이 일치하지 않습니다. 최신 버전을 다운받으세요");
				m_ConnectMsgDlg.OnCancel();
				break;
			}
			
			// 로그인 중이면	[SSO 작업]
			if(g_Where == WH_LOGIN)
			{
				ASK_LOGININFO al;
				memset(&al, 0, sizeof(ASK_LOGININFO));

			#ifdef _DEBUG
				strcpy(al.ID, g_LoginID);
				strcpy(al.Pass, g_LoginPass);
			#endif

				strcpy(al.SockNameIP, SockMan.pDataSock[g_MainSrvSID]->SockAddr);	// 연결된 소켓의 어드레스
				strcpy(al.HostNameIP, SockMan.GetServerIP());						// 호스트 시스템 어드레스
				al.Port = SockMan.pDataSock[g_MainSrvSID]->SockPort;				// 연결된 소켓의 포트번호
				// 서버용 소켓을 생성하기 위해 20000정도 더해준 포트번호를 사용한다
				// 실제 사용하게될 포트번호는 메인서버에 의하여 결정된다
				al.Port += 20000; 

				al.AuthCookieLen = g_AuthCookie.GetLength();
				al.DataCookieLen = g_DataCookie.GetLength();

				

				// 로그인 메세지를 보낸다
				CSV_ASK_LOGIN lgimsg;
				//al.bAIPlayer = FALSE;
				lgimsg.Set(&al, (char*)(LPCTSTR)g_AuthCookie, (char*)(LPCTSTR)g_DataCookie);
				SockMan.SendData(g_MainSrvSID, lgimsg.pData, lgimsg.GetTotalSize());

				/*
				CSV_ASK_LOGIN lgimsgAI;
				strcpy( al.ID, "cccc" );
				al.bAIPlayer = TRUE;
				lgimsgAI.Set(&al, (char*)(LPCTSTR)g_AuthCookie, (char*)(LPCTSTR)g_DataCookie);
				SockMan.SendData(g_MainSrvSID, lgimsgAI.pData, lgimsgAI.GetTotalSize());
				*/
				
			}

/*
			// 로그인 중이면
			if(g_Where == WH_LOGIN)
			{
				char *id = (char*)g_LoginID.operator LPCTSTR();
				char *pass = (char*)g_LoginPass.operator LPCTSTR();

				char *socknameip = (char*)SockMan.pDataSock[g_MainSrvSID]->SockAddr.operator LPCTSTR();	// 연결된 소켓의 어드레스
				char *hostnameip = (char*)SockMan.GetServerIP();	// 호스트 시스템 어드레스
				UINT port = SockMan.pDataSock[g_MainSrvSID]->SockPort;							// 연결된 소켓의 포트번호
				
				// 서버용 소켓을 생성하기 위해 20000정도 더해준 포트번호를 사용한다
				// 실제 사용하게될 포트번호는 메인서버에 의하여 결정된다
				port += 20000; 

				// 로그인 메세지를 보낸다
				CSV_LOGIN lgimsg;
				lgimsg.Set(id, pass, FALSE, socknameip, hostnameip, port);
				SockMan.SendData(g_MainSrvSID, lgimsg.pData, lgimsg.GetTotalSize());
			}
*/
		} break;

	case SV_SERVERINFO: // 서버 정보	[게임 아이템 작업]
		{
			CSV_SERVERINFO msg;
			msg.Get(lpData, TotSize);

			// 접속한 서버 정보 저장
			g_GameCode = msg.SI->GameCode;
			g_ServerName = msg.SI->ServerName;
			// URL 저장
			g_NoticeURL = msg.SI->NoticeURL;		// 공지 사항 URL
			g_BannerURL1 = msg.SI->BannerURL1;		// 로비 배너 URL
		
			// [ 세븐 포커 ] 잭팟
			// [ ###잭팟### ] 
			g_JackPotMoney = msg.SI->JackPot;		// 같은 값으로 셋
			g_TmpJackPotMoney = g_JackPotMoney;		//
		
			//######################################
			CString str ="http://www.goodboy7poker.com";
			g_BannerURL2 = str;
			g_BannerURL2 += msg.SI->BannerURL2;
			//######################################
			g_ItemURL = msg.SI->ItemURL;			// 아이템 상점용 URL			

			g_ItemHelpURL = msg.SI->ItemHelpURL;	// 아이템 도움말 URL

				
			g_strSrvInfo = msg.SI->SrvInfoMsg;  // 로비창 공지 메세지
			g_LocationURL = msg.SI->LocationURL;	// 위치알림(친구찾기)용 URL
			g_CashURL = msg.SI->CashURL;			// 캐쉬충전 URL


		} break;

	case SV_ACCEPT_LOGIN: // 로그인이 허용 되었음
		{
			CSV_ACCEPT_LOGIN msg;
			msg.Get(lpData, TotSize);

			// 접속 메세지창 종료
			m_ConnectMsgDlg.OnOK();

			// 사용자 정보 설정
			//if ( !msg.LI->UI.bAIPlayer )
				Play[0].SetNewPlayer(&msg.LI->UI);
			/*
			else
			{
				Play[1].SetNewPlayer(&msg.LI->UI);
				break;
			}
			*/

			// ### [ 관전기능 ] ###
			g_MyInfo.SetNewPlayer(&msg.LI->UI);
			g_MyIndex = 0;;
			g_MyObserver = FALSE; //관전중인가


			// 메인 대화상자를 원래의 크기로 복원
			ShowWindow(SW_HIDE);
			MoveWindow(g_OrgMainDlgRect);
			CenterWindow();
			ShowWindow(SW_SHOW);

			// Top으로 이동 (Windows 2000의 경우 익스플로러 아래로 프로그램이 가려지는 경우가 있음)
			SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			SetActiveWindow();
			//SetForegroundWindow(); // 액티브한 후 깜빡거림

			// 중복 처리 방지용
			static BOOL bLoginProccessed=FALSE;
			if(bLoginProccessed==TRUE) break;
			bLoginProccessed=TRUE;

			// 전역 정보 초기화
			g_LoginID = msg.LI->UI.ID;		// 로그인 아이디
			g_MyUniqNo = msg.LI->UniqNo;	// 유니크 넘버 

			// [게임 아이템 작업]
			// 메인 빅배너의 주민등록번호에 따른 타게팅 광고 목적으로 주민등록 번호를 인자로 넘김
			//CString strval;
			//strval.Format("?regid=%s", msg.LI->RegID);	// 인자 구성
			//g_NoticeURL += strval;

			// 타이틀로 이동
			g_Where = WH_TITLE;
			ChangeView(0);

			// 전체 채널 목록을 요구함
			g_pTitleDlg->bDontClickSound = TRUE;
			g_pTitleDlg->PostMessage(WM_COMMAND, IDC_BUTTON_REFRESH);


			// ### [ 관전기능 ] ###
			if( g_MyInfo.UI.Sex == 1) {
				g_MyInfo.nSndFxKind = 0;//기본음성
				Play[0].nSndFxKind  = 0;
			}
			else {
				g_MyInfo.nSndFxKind = 3;
				Play[0].nSndFxKind  = 3;
			}

			// 환경설정 파일을 성공적으로 읽었다면 환경설정 적용
			if(LoadConfig(Play[0].UI.ID)) SetupByConfig();

			// 공지사항, 베너 오픈
			if(g_NoticeURL.GetLength()>0) g_pTitleDlg->m_pWebCtrl->Navigate(g_NoticeURL);
			if(g_BannerURL1.GetLength()>0) g_pLobyDlg->m_BannerWnd.OpenURL(g_BannerURL1);
			// 위치알림(친구찾기)용 URL 오픈
			// ### [ 관전기능 ] ###
			if(g_LocationURL.GetLength()>0) {
				CString strurl;
				//strurl.Format("%s?user_id=%s&server=%s&code=%d", g_LocationURL, Play[0].UI.ID, g_ServerName, g_GameCode);
				strurl.Format("%s?user_id=%s&server=%s&code=%d", g_LocationURL, g_MyInfo.UI.ID, g_ServerName, g_GameCode);
				g_pTitleDlg->m_pLocWebCtrl->Navigate(strurl);
			}
			
	//		CString strmsg;//[62] 채널 공지
	//		strmsg.Format("넷마블 뉴포커가 베타 테스트 중입니다.\n넷마블 뉴포커는 기본적으로 포커와 같은 룰의 게임이며  6번째 카드를 받은 뒤 카드를 두 번 교환할 수 있어 더 많은 재미를 느낄 수 있는 게임입니다. 정식서비스 전환 시 게임머니는 초기화 됩니다.\n감사합니다.");
			



	//		m_MsgBoxDlg.MsgBox(1, (char*)strmsg.operator LPCTSTR());

			//순서 중요
			//CStartDlg *pDlg = new CStartDlg;
			//pDlg->DoModal();
			//delete pDlg;

			//### [관리자 모드 작업] ###
			// 프리미엄 PC방 IP라면 대화상자를 띄워 표시 [PC방 작업]
			if(g_MyInfo.UI.bPremIP) m_PremServiceDlg.Create(this);
			if(g_MyInfo.UI.cAdminUser == 1)	g_bAdminUser = TRUE;

			bLoginProccessed=FALSE;		


		} break;
	case SV_REFUSE_LOGIN: // 로그인이 거부 되었음
		{
			CSV_REFUSE_LOGIN msg;
			msg.Get(lpData, TotSize);
			int code = *msg.Code;
			//------------  [SSO 작업]
			int slen = *msg.l_StrMsg; 

			if(slen > 0) {
				char* strmsg = new char[slen+1];
				memset(strmsg, 0, slen+1);
				strncpy(strmsg, msg.StrMsg, slen);
				
				g_pMainDlg->m_MsgBoxDlg.MsgBox(0,strmsg);

				delete strmsg;
			}
			//------------ 
	/*
			if(code==0) AfxMessageBox("사용자가 아닙니다.\n유저ID를 확인하세요");
			if(code==1) AfxMessageBox("비밀번호가 틀렸습니다.\n비밀번호를 다시 확인하세요");
			if(code==2) AfxMessageBox("이미 사용중인 아이디입니다.\n관리자에게 문의하세요");
			if(code==3) AfxMessageBox("최대 수용인원을 초과하였습니다.\n잠시후 다시 접속해주세요");
			if(code==4) AfxMessageBox("인증되지 않은 사용자입니다");
			if(code==6) AfxMessageBox("본 아이디는 현재 사용 정지 상태이므로\n서버에 접속할 수 없습니다.\n관리자에게 문의하세요");
			if(code==7) AfxMessageBox("본 아이디는 삭제된 아이디입니다.\n관리자에게 문의하세요");
			if(code==5)
			{
				static BOOL bProccessed = FALSE;
				if(bProccessed != FALSE) break;

				int rtn = AfxMessageBox("이전 게임에서 아직 접속 종료 처리가 이루어지지 않았습니다.\n[예]를 누르면 강제로 접속할 수 있습니다만 상황에 따라서 \n바로 전에 하셨던 게임의 기록이 반영되지 않을 수 있습니다.\n20분 후에 다시 접속을 시도해보시고 여전히 이 메시지가 나타난다면\n[예]를 눌러서 강제로 접속하시기 바랍니다.\n지금 강제 접속하시겠습니까?", MB_YESNO);
				if(rtn == IDNO) { m_ConnectMsgDlg.OnCancel(); break; }
				bProccessed = FALSE;

				char *id = (char*)g_LoginID.operator LPCTSTR();
				char *pass = (char*)g_LoginPass.operator LPCTSTR();
				char *socknameip = (char*)SockMan.pDataSock[g_MainSrvSID]->SockAddr.operator LPCTSTR();	// 연결된 소켓의 어드레스
				char *hostnameip = (char*)SockMan.GetServerIP();		// 호스트 시스템 어드레스
				UINT port = SockMan.pDataSock[g_MainSrvSID]->SockPort;	// 연결된 소켓의 포트번호

				// 서버용 소켓을 생성하기 위해 20000정도 더해준 포트번호를 사용한다
				// 실제 사용할 포트번호는 메인서버에 의해서 변경될 수 있다
				port += 20000; 
				// 로그인 메세지를 보낸다
				CSV_LOGIN lgimsg;
				lgimsg.Set(id, pass, TRUE, socknameip, hostnameip, port);
				SockMan.SendData(g_MainSrvSID, lgimsg.pData, lgimsg.GetTotalSize());
				break;
			}
			//PostMessage(UM_DATA_CLOSE, NULL, (long)SockMan.pDataSock[g_MainSrvSID]); // 연결종료
	*/
			// 프로그램 종료
			m_ConnectMsgDlg.OnCancel();
		} break;

	case SV_ACCEPT_LOGOUT: // 로그아웃이 허용되었음
		{
			CSV_ACCEPT_LOGOUT msg;
			msg.Get(lpData, TotSize);
			g_Wait = FALSE;
			bNowLogout = TRUE;
			OnCancel();
		} break;


	////////////////////////////// [게임 아이템 작업] //////////////////////////////

	case SV_GAMEITEMLIST: // 게임 아이템 리스트
		{
			CSV_GAMEITEMLIST msg;
			msg.Get(lpData, TotSize);

			// 메시지 박스가 뜨고나면 이미 접속이 끊어져서 모든 데이터가 초기화 되므로 미리 데이터를 따로 보관
			int kind = msg.pIN->Kind;
			int msglen1 = msg.pIN->StrMsgLen1;
			int msglen2 = msg.pIN->StrMsgLen2;
			char strmsg1[2048] = {0,};	// 아이템 사용을 물어보는 문자열
			char strmsg2[2048] = {0,};	// 아이템 설명 문자열
			if(msglen1 > 0) strncpy(strmsg1, msg.StrMsg1, msglen1);
			if(msglen2 > 0) strncpy(strmsg2, msg.StrMsg2, msglen2);

			// 서버의 시간과 클라이언트의 현재시간을 기록
			CTime st(msg.pIN->ServerTime);
			g_ServerTime = st;
			g_ClientTime = CTime::GetCurrentTime();
			g_CSTimeGap = g_ClientTime - g_ServerTime;

			if(msg.pIN->ErrorCode == 0) {
				// 에러가 없다면 내 아이템 리스트에 보관
				g_MyGameItem.nNum = msg.pIN->TotNum;
				for(int i=0; i<g_MyGameItem.nNum; i++)
					g_MyGameItem.Item[i] = msg.ArrayGI[i];
			}

			// 아이템 리스트를 갱신
			m_GameItemDlg.m_ItemListDlg.RefreshList();

			// 내 아이템 리스트인 경우
			if(kind == 0) {
				// 모두 완료 되었음
			}
			// 아이템 리스트 갱신
			else if(kind == 100) {
				// 모두 완료 되었음
			}
			// 로그인 혹은 채널 입장시 점프 입장 아이템 사용을 묻는 경우
			else if(kind == 1 || kind == 2) {

				// 중복 처리 방지용
				static BOOL bProccessed = FALSE;
				if(bProccessed) break;
				bProccessed = TRUE;

				// 다이얼로그 아이템 정보 초기화
				m_GameItemDlg.m_ItemListDlg.m_StrMsg = strmsg2;

				// 내 아이템 리스트 탭을 초기화면으로 보여주고 점프 아이템을 기본으로 선택시킴
				if(m_GameItemDlg.DoModal(1, FALSE, ITEM_SCODE_JUMP) != IDOK) {
					// 아이템을 사용하지 않았을 경우
					if(kind == 1) {
						// 로그인 시 점프 입장 아이템 사용을 묻는 경우
						// 프로그램 종료
						if(m_ConnectMsgDlg.GetSafeHwnd()) m_ConnectMsgDlg.OnCancel();
					}
					else if(kind == 2) {
						// 채널 입장시 점프 입장 아이템 사용을 묻는 경우
						g_Wait = FALSE;
						g_pTitleDlg->GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);
						// 메시지 창 닫음
						if(g_pTitleDlg->m_MsgDlg.GetSafeHwnd()) g_pTitleDlg->m_MsgDlg.OnOK();
					}
				}

				bProccessed = FALSE;
			}
		} break;

	case SV_USEGAMEITEM: // 게임 아이템 사용결과
		{
			CSV_USEGAMEITEM msg;
			msg.Get(lpData, TotSize);

			// 아이템 사용중 플래그 해제
			m_GameItemDlg.m_ItemListDlg.m_bItemUsed = FALSE;

			// 일치하는 아이템을 내목록에서 찾음
			int index = -1;
			for(int i=0; i<g_MyGameItem.nNum; i++) {
				if(g_MyGameItem.Item[i].NO == msg.pUG->Item.NO) {
					index = i;
					break;
				}
			}

			if(index < 0) break;

			if(g_MyGameItem.Item[index].Code != msg.pUG->Item.Code)
				break;

			// 중복 처리 방지용
			static BOOL bProccessed = FALSE;
			if(bProccessed) break;
			bProccessed = TRUE;

			// 게임 아이템 정보를 갱신
			g_MyGameItem.Item[index] = msg.pUG->Item;

			// 게임 아이템 리스트 목록을 갱신(아이템 다이얼로그가 생성되어 있지 않다면 실패)
		//	m_GameItemDlg.m_ItemListDlg.SetItemList(g_MyGameItem.nNum, g_MyGameItem.Item);
			m_GameItemDlg.m_ItemListDlg.RebuildItemList();

			// 사용한 아이템이 점프 입장권 아이템인가?
			if(g_MyGameItem.Item[index].Code >= ITEM_SCODE_JUMP && g_MyGameItem.Item[index].Code <= ITEM_ECODE_JUMP) {
				// 점프 입장권 아이템을 사용한 경우

				if(g_Where == WH_LOGIN || g_Where == WH_TITLE) {
					// 게임 아이템 대화상자를 닫음
					m_GameItemDlg.SafeClose();
				}

				if(g_Where == WH_TITLE) {
					// 채널 선택창에서 점프 입장권 아이템을 사용한 경우
					g_Wait = FALSE;
					g_pTitleDlg->GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);
					// 메시지 창 닫음
					if(g_pTitleDlg->m_MsgDlg.GetSafeHwnd()) g_pTitleDlg->m_MsgDlg.OnOK();

					if(!msg.pUG->bUseError) {
						// 아이템 사용에 성공하였으므로 채널 입장을 다시 시도
						g_pTitleDlg->PostMessage(WM_COMMAND, IDC_BUTTON_ENTER);
					}
				}
			}

			// 아이템 사용에 관련된 메시지가 존재한다면 메시지를 보여줌
			if(msg.pUG->bMessage) {
				if(strlen(msg.pUG->StrMsg) > 0) {
					g_pMainDlg->m_MsgBoxDlg.MsgBox(0, msg.pUG->StrMsg);
				}
			}

			// 중복 처리 방지용
			bProccessed = FALSE;

		} break;

	case SV_BUYGAMEITEM: // 게임 아이템 구입 문의
		{
		CSV_BUYGAMEITEM msg;
			msg.Get(lpData, TotSize);

			// 중복 처리 방지용
			static BOOL bProccessed = FALSE;
			if(bProccessed) break;
			bProccessed = TRUE;

			int msglen = msg.pBI->StrMsgLen;
			char strmsg[1024] = {0,};

			if(msglen > 0) {
				// 메시지 박스가 뜨고나면 이미 접속이 끊어져서 모든 데이터가 초기화 되므로 미리 데이터를 따로 보관
				strncpy(strmsg, msg.StrMsg, msglen);

				int msgkind = msg.pBI->Kind;
				int itemcode = msg.pBI->ItemCode;

				BOOL bCancelBuyItem = FALSE;

				CAskBuyItemDlg dlg;
				dlg.m_strMsg = strmsg;
				dlg.m_ItemCode = itemcode;

				int rtn = dlg.DoModal();
				if(rtn == IDOK) {
					// 아이템 구입에 동의했다면...
					m_GameItemDlg.m_ItemListDlg.m_StrMsg = "";

					int firsttab = 0;

					if(msgkind == 1 || msgkind == 2) {
						// 구입을 물어본 아이템이 점프 입장권 아이템인 경우는 
						// 사용자가 아이템을 가지고 있지 않음이 확실하기 때문에 바로 구매 페이지로 이동
						firsttab = 2;
					}
					else {
						// 대부분의 경우 이 부분에 진입할 때 아이템 리스트를 서버로부터 받은 상태이나
						// 로그인 되기 전에 처리되어 진다면 아이템 리스트가 없는 상황일 수 있으니 주의

						// 현재 시간을 얻어서 서버의 시간으로 변환
						CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
						SYSTEMTIME st;
						time.GetAsSystemTime(st);

						// 사용 가능한 아이템이 있는지 검사
						BOOL bItemUsing = FALSE;
						BOOL bItemExist = FALSE;
						bItemUsing = GameItem_IsUsingItem(g_MyGameItem, itemcode, &bItemExist, &st);
						if(bItemExist)
							firsttab = 1;	// 아이템을 사용할 수 있도록 아이템 리스트 탭으로 이동
						else
							firsttab = 2;	// 사용 가능한 아이템이 없으므로 구입할 수 있는 상점 페이지로 이동
					}

					if(m_GameItemDlg.DoModal(firsttab, FALSE, itemcode) != IDOK) 
						bCancelBuyItem = TRUE;
				}
				else {
					// 아이템 구입을 거부했다면 
					bCancelBuyItem = TRUE;
				}

				if( bCancelBuyItem ) {
					// 아이템 구입을 취소하였다면
					if(msgkind == 1) {
						// 로그인중에 점프 아이템의 구입을 물어본 경우였다면 즉시 프로그램 종료
						if(m_ConnectMsgDlg.GetSafeHwnd() && m_ConnectMsgDlg.IsWindowVisible()) {
							// 프로그램 종료
							m_ConnectMsgDlg.OnCancel();
						}
					}
					else if(msgkind == 2) {
						// 채널 선택창에서 점프 입장권 아이템을 사용한 경우
						g_Wait = FALSE;
						g_pTitleDlg->GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);
						// 메시지 창 닫음
						if(g_pTitleDlg->m_MsgDlg.GetSafeHwnd()) g_pTitleDlg->m_MsgDlg.OnOK();
					}
					else if(msgkind == 3) {
						// 슈퍼방장 아이템의 사용을 물어본 경우
					}
				}
			}

			bProccessed = FALSE;
			/*
			CSV_BUYGAMEITEM msg;
			msg.Get(lpData, TotSize);

			// 중복 처리 방지용
			static BOOL bProccessed = FALSE;
			if(bProccessed) break;
			bProccessed = TRUE;

			int msglen = msg.pBI->StrMsgLen;
			char strmsg[1024] = {0,};

			if(msglen > 0) {
				// 메시지 박스가 뜨고나면 이미 접속이 끊어져서 모든 데이터가 초기화 되므로 미리 데이터를 따로 보관
				strncpy(strmsg, msg.StrMsg, msglen);

				int msgkind = msg.pBI->Kind;

				BOOL bCancelBuyItem = FALSE;

				CAskBuyItemDlg dlg;
				dlg.m_strMsg = strmsg;
				dlg.m_ItemCode = msg.pBI->ItemCode;

				int rtn = dlg.DoModal();
				if(rtn == IDOK) {
					// 아이템 구입에 동의했다면...
					m_GameItemDlg.m_ItemListDlg.m_StrMsg = "";

					int firsttab = 0;

					if(msgkind == 1 || msgkind == 2) {
						// 구입을 물어본 아이템이 점프 입장권 아이템인 경우는 
						// 사용자가 아이템을 가지고 있지 않음이 확실하기 때문에 바로 구매 페이지로 이동
						firsttab = 2;
					}

					if(m_GameItemDlg.DoModal(firsttab, TRUE) != IDOK) 
						bCancelBuyItem = TRUE;
				}
				else {
					// 아이템 구입을 거부했다면 
					bCancelBuyItem = TRUE;
				}

				if( bCancelBuyItem ) {
					// 아이템 구입을 취소하였다면
					if(msgkind == 1) {
						// 로그인중에 점프 아이템의 구입을 물어본 경우였다면 즉시 프로그램 종료
						if(m_ConnectMsgDlg.GetSafeHwnd() && m_ConnectMsgDlg.IsWindowVisible()) {
							// 프로그램 종료
							m_ConnectMsgDlg.OnCancel();
						}
					}
					else if(msgkind == 2) {
						// 채널 선택창에서 점프 입장권 아이템을 사용한 경우
						g_Wait = FALSE;
						g_pTitleDlg->GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);
						// 메시지 창 닫음
						if(g_pTitleDlg->m_MsgDlg.GetSafeHwnd()) g_pTitleDlg->m_MsgDlg.OnOK();
					}
					else if(msgkind == 3) {
						// 슈퍼방장 아이템의 사용을 물어본 경우
					}
				}
			}

			bProccessed = FALSE;
			*/
		} break;

	////////////////////////////////////////////////////////////////////////////////

	// 2003 06 23
	case SV_ALLCHANINFO: // 모든 채널 정보
		{
			
			/*
			CSV_ALLCHANINFO msg;
			msg.Get(lpData, TotSize);

			g_Wait = FALSE;

			g_pTitleDlg->GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(TRUE);

			g_pTitleDlg->Chan_DeleteList();
			// ### [ 채널 입장 구분 ] ###
			for(int i=0; i<*msg.TotNum; i++) 
				g_pTitleDlg->Chan_InsItem(msg.ArrayCI[i].ChNum, msg.ArrayCI[i].Title, msg.ArrayCI[i].NowUserNum, msg.ArrayCI[i].MaxUserNum, msg.ArrayCI[i].ChannelStyle);

			// 마지막에 선택되어있던 아이템을 선택하게 한다(마지막에 접속한 채널을 알아볼 수 있도록...)
			g_pTitleDlg->Chan_SelectLast();

			// 메시지 창 닫음
			g_pTitleDlg->m_MsgDlg.OnOK();
			*/
			
			CSV_ASK_ENTERCHAN aemsg;
			aemsg.Set(0);
			SockMan.SendData(g_MainSrvSID, aemsg.pData, aemsg.GetTotalSize());

		} break;

	case SV_ACCEPT_ENTERCHAN: // 채널 입장 허용
		{
			CSV_ACCEPT_ENTERCHAN msg;
			msg.Get(lpData, TotSize);


			// [게임 아이템 작업]
			// 에러 코드가 -100보다 작은 경우는 아무런 메시지도 출력하지 않는다
			// 메시지창과 버튼의 활성화는 게임 아이템의 구입 및 사용 메시지에서 처리함
			if(msg.EC->ChNum <= -100) break;

			g_Wait = FALSE;
			g_pTitleDlg->GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);

			// 메시지 창 닫음
			g_pTitleDlg->m_MsgDlg.OnOK();

			if(msg.EC->ChNum < 0) {
				static BOOL bProcessed = FALSE;
				if(bProcessed) break;
				bProcessed = TRUE;
				switch(msg.EC->ChNum) 
				{
				//case -1: AfxMessageBox("요청한 채널은 최대 수용인원을 초과하였습니다."); break;
				//case -2: AfxMessageBox("잘못된 사용자 정보로 인하여 채널에 입장할 수 없습니다."); break;
				//case -3: AfxMessageBox("요청한 채널 번호가 올바르지 않습니다."); break;
				//case -4: AfxMessageBox("입장 가능한 등급이 아닙니다"); break;
				//default : AfxMessageBox("채널에 입장할 수 없습니다."); break;
				case -1: g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("CHAN_FULL"))); break;
				case -2: g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("CHAN_ERROR"))); break;
				case -3: g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("CHAN_NUM_ERR"))); break;
				case -4: g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("NO_LEVEL"))); break; //[세븐포커]
				case -5: g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("NO_MONEY"))); break; //[세븐포커]
				case -10: break; // 버튼 활성화 위해 임시
				default : g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("ENT_CHAN_ERR"))); break;
				}
				bProcessed = FALSE;
				break;
			}
			// 입장한 채널 이름을 저장
			g_ChannelName = msg.EC->ChanName;
			// 입장한 채널스타일을 저장
			g_nChannelStyle = msg.EC->ChanStyle;
			// 입장한 채널이 5, 7인용 
			g_nchannel_usernum = msg.EC->channel_usernum; // 0: 방만들기 우선, 1 : 5인용 2 : 7인용

			// 대기실로 이동
			g_Where = WH_LOBY;
			ChangeView(2);
			return 1;
			//m_LobyDlg.m_SendEdit.SetFocus();
			m_LobyDlg.SetFocus();
			CString str;
			str.Format(g_StrMan.Get(_T("WELCOME")), Play[0].UI.ID);
			str+=g_StrMan.Get(_T("USE_WHIS"));
			str+=g_StrMan.Get(_T("USE_WHIS2"));
			
			g_pLobyDlg->m_ChatViewEdit.AddText(&str);
			str="";
			str+=g_StrMan.Get(_T("BAD_112"));
			g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(0,0,200));
			
			if(g_strSrvInfo.GetLength() >= 2)
			{
				str=g_StrMan.Get(_T("SRV_MES"));//"##<서버공지>##   ";
				//g_strSrvInfo+="\n";
				str+=g_strSrvInfo;// 채널 공지
				str+="\n";
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(255,0,0));
			}
			
			if(Sound.bInited == FALSE)
			{
				str = "※ 다이렉트 사운드 초기화에 실패하여 다중 사운드 출력이 지원되지 않습니다.\n";
				str += "  다른 응용 프로그램에서 사운드 장치를 사용중인지 점검하시고 \n ";
				str += "  시스템에 다이렉트X가 설치되어있지 않다면 최신 버전을 설치하시기 바랍니다.\n";
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(0,200,50));
			}

			ShowItemState();	// [게임 아이템 작업]
			// 방 목록 및 사용자 목록 리스트를 요구
			g_pLobyDlg->RefreshList();
		} break;
	case SV_ACCEPT_OUTCHAN: // 채널 퇴장 허용
		{
			CSV_ACCEPT_OUTCHAN msg;
			msg.Get(lpData, TotSize);

			g_Wait = FALSE;

			// 타이틀로 이동
			g_Where = WH_TITLE;
			ChangeView(0);
			
			// 채널에서 나가면 초기화
			g_nChannelStyle = 0;

			// 채널 리프레쉬
			g_pTitleDlg->bDontClickSound = TRUE;
			g_pTitleDlg->PostMessage(WM_COMMAND, IDC_BUTTON_REFRESH);
		} break;

	case SV_RESULT_FINDID: // 사용자 찾기 결과
		{
			CSV_RESULT_FINDID msg;
			msg.Get(lpData, TotSize);

			g_pTitleDlg->GetDlgItem(IDC_EDIT_FINDID)->EnableWindow(TRUE);
			g_pTitleDlg->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(TRUE);

			CResultFindIdDlg Dlg;

			if(strlen(msg.RF->ID) <= 0) {
				//::MessageBox(m_hWnd, "현재 서버에 접속하지 않았거나 존재하지 않는 사용자입니다.", "ID 찾기 실패", MB_OK);
				Dlg.DoModal( g_StrMan.Get(_T("FIND_ERROR")) );
				//::MessageBox(m_hWnd, g_StrMan.Get(_T("FIND_ERROR")), "SevenPoker Hi/Low", MB_OK);
				//AfxMessageBox(g_StrMan.Get(_T("FIND_ERROR")));
			}
			else {
				CString str;
				if(msg.RF->Position == 0 || msg.RF->ChNum < 0) {
					//str.Format("[%s]님은 서버에 접속하였으며 채널 선택중입니다.", msg.RF->ID); 
					str.Format(g_StrMan.Get(_T("FIND_CHAN")), msg.RF->ID); 
				}
				else {
				
					CString chname = g_pTitleDlg->GetChannelName(msg.RF->ChNum);
					if(msg.RF->Position == 1) {
						str.Format(g_StrMan.Get(_T("FIND_LOBBY")), msg.RF->ID, chname); 
					}
					else if(msg.RF->Position == 2){
						str.Format(g_StrMan.Get(_T("FIND_SHOP")), msg.RF->ID, chname, msg.RF->RoomNum); 
					}
					else if(msg.RF->Position == 3){
						str.Format(g_StrMan.Get(_T("FIND_ROOM")), msg.RF->ID, chname, msg.RF->RoomNum); 
					}
					else {
						str.Format(g_StrMan.Get(_T("FIND_USER")), msg.RF->ID, chname); 
					}
				}
				//::MessageBox(m_hWnd, str, "ID 찾기 결과", MB_OK);g_StrMan._Get(_T("FIND_RESULT"))
				//::MessageBox(m_hWnd, str, g_StrMan._Get(_T("FIND_RESULT")), MB_OK);
				Dlg.DoModal( str );
			}
			
		} break;

	case SV_ALLUSERINFO: // 모든 사용자 정보
		{
			CSV_ALLUSERINFO msg;
			msg.Get(lpData, TotSize);

			//#####################################
			// 메시지 창 닫음
			//g_pLobyDlg->m_MsgDlg.OnOK();
			//#####################################

			//m_LobyDlg.m_UserList.LockWindowUpdate();
			m_LobyDlg.m_UserList.DeleteAllItems();
			for(int i=0; i<*msg.TotNum; i++)
			{
				int unum = msg.ArraySUI[i].UNum;
				char *id = msg.ArraySUI[i].ID;
				BOOL sex = msg.ArraySUI[i].Sex;
				int winnum = 0;//msg.ArraySUI[i].WinNum;
				int loosenum = 0;// msg.ArraySUI[i].LooseNum;
				int position = msg.ArraySUI[i].Position;
				int roomnum = msg.ArraySUI[i].RoomNum;
				int drawnum = 0;//msg.ArraySUI[i].DrawNum;
				int level = msg.ArraySUI[i].nIcon;
				int point = 0;
				//### [관리자 모드 작업] ###
				if(msg.ArraySUI[i].cAdminUser != 1 ||  strcmp(g_MyInfo.UI.ID,msg.ArraySUI[i].ID)==0)
					m_LobyDlg.User_InsItem(unum, id, sex, winnum, loosenum, drawnum, point, position, roomnum, level);
			}
			//m_LobyDlg.m_UserList.UnlockWindowUpdate();
			g_pGameDlg->m_InviteDlg.User_ReloadList();// 대기실 초대창 갱신


		} break;

	case SV_USERINFO: // 특정 사용자 정보
		{
			CSV_USERINFO msg;
			msg.Get(lpData, TotSize);
			
			g_Wait = FALSE;

			// 아이디 길이가 0이면 현재 명단에 없는 사용자임
			if(strlen(msg.UI->ID) == 0) break;

			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
			CProfileDlg dlg;
			dlg.Init(msg.UI, TRUE);	// [게임 아이템 작업]	<-- 변경
			dlg.DoModal();
			bProccessed = FALSE;
		} break;

	case SV_NEWUSER: // 새로운 사용자 접속
		{
			CSV_NEWUSER msg;
			msg.Get(lpData, TotSize);
			int unum = msg.SUI->UNum;
			char *id = msg.SUI->ID;
			BOOL sex = msg.SUI->Sex;
			int winnum = 0;//msg.SUI->WinNum;
			int loosenum = 0;//msg.SUI->LooseNum;
			int position = msg.SUI->Position;
			int roomnum = msg.SUI->RoomNum;
			int drawnum = 0;//msg.SUI->DrawNum;
			int level = msg.SUI->nIcon;
			int point = 0;
			//### [관리자 모드 작업] ###
			if(msg.SUI->cAdminUser != 1)
				m_LobyDlg.User_InsItem(unum, id, sex, winnum, loosenum, drawnum, point, position, roomnum, level);
		} break;

	case SV_OUTUSER: // 사용자 접속 종료
		{
			CSV_OUTUSER msg;
			msg.Get(lpData, TotSize);
			int unum = msg.SUI->UNum;
			char *id = msg.SUI->ID;
			m_LobyDlg.User_DelItem(unum, id);
		} break;

	case SV_CHANGEUSER: // 사용자 접속정보 변경
		{
			CSV_CHANGEUSER msg;
			msg.Get(lpData, TotSize);
			int unum = msg.SUI->UNum;
			char *id = msg.SUI->ID;
			int winnum = 0;//msg.SUI->WinNum;
			int loosenum = 0;//msg.SUI->LooseNum;
			//int point = msg.SUI->Point;
			int position = msg.SUI->Position;
			int roomnum = msg.SUI->RoomNum;
			int drawnum = 0;//msg.SUI->DrawNum;
			int level = msg.SUI->nIcon;
			//int level   = GetPokerLevel( msg.SUI->PMoney );
			int point = 0;
			m_LobyDlg.User_SetItem(unum, id, winnum, loosenum, drawnum, point, position, roomnum, level);
		} break;

	case SV_ALLROOMINFO: // 모든 방정보 목록 도착
		{
			CSV_ALLROOMINFO msg;
			msg.Get(lpData, TotSize);

			////////////////////////////////////
			// ### [ 대기실 리스트 관련 ] ###
			g_pLobyDlg->m_RoomView.DelAllList();
			g_pLobyDlg->m_MsgDlg.OnOK();// 메시지 창 닫음
			for(int i=0; i<*msg.TotNum; i++)
			{
				// ### [ 관전기능 ] ###
				m_LobyDlg.Room_InsItem( &msg.ArrayRI[i] );
			}
			////////////////////////////////////


			/*
			m_LobyDlg.m_RoomList.DeleteAllItems();
			g_pLobyDlg->m_MsgDlg.OnOK();// 메시지 창 닫음
			for(int i=0; i<*msg.TotNum; i++)
			{
				int roomnum = msg.ArrayRI[i].RoomNum;
				int gamekind = msg.ArrayRI[i].GameKind;
				char *title = msg.ArrayRI[i].Title;
				char *master = msg.ArrayRI[i].ID;
				int nowuser = msg.ArrayRI[i].NowUserNum;
				int maxuser = msg.ArrayRI[i].MaxUserNum;
				int secret = msg.ArrayRI[i].Secret;
				int state = msg.ArrayRI[i].State;
				int level = msg.ArrayRI[i].FormKind;
				int observer = msg.ArrayRI[i].NowObserverNum;// ### [ 관전기능 ] ###
				BOOL superroom = msg.ArrayRI[i].bSuperMaster;	// [게임 아이템 작업]
			
				// ### [ 관전기능 ] ###
				m_LobyDlg.Room_InsItem(roomnum, master, title, level, nowuser, maxuser, observer, secret, state, gamekind,superroom);
			}
			*/

			
		} break;

	case SV_ROOMINFO: // 방정보 도착(참여자 정보)
		{
			CSV_ROOMINFO msg;
			msg.Get(lpData, TotSize);
			g_Wait = FALSE;

			if(*msg.ErrCode != 0) break;

			// 메세지 박스가 뜬 상태에서 다른 메세지가 들어오면 또다시 박스가 뜨는것을 막음
			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
			CRoomInfoDlg Dlg;
			Dlg.DoModal(msg.GI);
			bProccessed = FALSE;
		} break;

	case SV_ACCEPT_CHANGECHAR: // 캐릭터 변경 요청 허락
		{
			CSV_ACCEPT_CHANGECHAR msg;
			msg.Get(lpData, TotSize);
			int charnum = *msg.CharNum;
			if(charnum < 0 || charnum > 72) break;
//			Play[0].UI.Character = charnum;
			//g_pLobyDlg->m_CharView.SetCharacter(charnum);
			g_pLobyDlg->m_CharView.SetCharacter(&Play[0].UI);
			g_Wait = FALSE;
			// 메세지 박스가 뜬 상태에서 다른 메세지가 들어오면 또다시 박스가 뜨는것을 막음
			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
			AfxMessageBox("캐릭터가 변경되었습니다"); 
			bProccessed = FALSE;
		} break;


	case SV_ASK_CHANGESNDFX: // ### [사운드 추가 작업] ###
		{
			if(g_Where != WH_GAME) break;

			CSV_ASK_CHANGESNDFX msg;
			msg.Get(lpData, TotSize);

			if(g_RI.RoomNum	!= *msg.RoomNum) break;

			int spnum = *msg.ServPNum;
			if(spnum < 0 || spnum >= g_Max_NewPlayer) break;// ### [ 관전기능 ] ###

			int pn = Game.GetPlayerPNum(msg.ID);
			if(pn == -1) break;

			// ### [사운드 추가 작업] ###
			Play[pn].nSndFxKind = *msg.SndFX;

			// ### [ 관전기능 ] ###
			if(strcmp(msg.ID, g_MyInfo.UI.ID)== 0) {
				g_MyInfo.nSndFxKind = *msg.SndFX;
			}
			
		} break;

	case SV_REFUSE_CREATEROOM: // 방 개설이 거부되었음
		{
			CSV_REFUSE_CREATEROOM msg;

			msg.Get(lpData, TotSize);

			g_Wait = FALSE;
			g_pLobyDlg->iAccessRoomState=0;

			// 메세지 박스가 뜬 상태에서 다른 메세지가 들어오면 또다시 박스가 뜨는것을 막음
			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			switch(*msg.Code)
			{
			case 0: bProccessed = TRUE;  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("OVER_ROOM")));     bProccessed = FALSE; break;
			case 1: bProccessed = TRUE;  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("MAKEROOM_ERR")));  bProccessed = FALSE; break;
			case 2: bProccessed = TRUE;  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("LEVEL_ERROR")));   bProccessed = FALSE; break;
			case 3: bProccessed = TRUE;  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("LECK_PMONEY1")));  bProccessed = FALSE; break;
			case 999: break;
			default: bProccessed = TRUE; g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("MAKE_ROOM_ERR"))); bProccessed = FALSE; 
			}
		} break;

	case SV_ACCEPT_CREATEROOM: // 방 개설이 허용되었음
		{
			CSV_ACCEPT_CREATEROOM msg;
			msg.Get(lpData, TotSize);

			// ### [ 관전기능 ] ###
			Game.Accept_CreateRoom(&msg);
			
		} break;

	case SV_REFUSE_ENTERROOM: // 방에 접속 거부됨
		{
			CSV_REFUSE_ENTERROOM msg;
			msg.Get(lpData, TotSize);
			
			g_Wait=FALSE;
			g_pLobyDlg->iAccessRoomState=0;

			// ### [ 관전기능 ] ###
			if(*msg.Code < 50)
			{
				g_Wait=FALSE;
				g_pLobyDlg->iAccessRoomState=0;

				// 접속 대화상자 닫음
				g_pLobyDlg->m_MsgDlg.OnCancel();
			}

			// 메세지 박스가 뜬 상태에서 다른 메세지가 들어오면 또다시 박스가 뜨는것을 막음
			static BOOL bProccessed = FALSE;
			if( bProccessed ) break;
			
			bProccessed = TRUE;
			
			switch(*msg.Code)
			{
			case 1: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("RI_ERR")));  break;
			case 2:  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("NOTEXIST_ROOM")));  break;
			case 3:  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("PLAYING_ROOM")));  break;
			case 4:  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("OVER_MAXUSER")));  break;
			case 5:  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("ALREADY_INROOM")));  break;
			case 6:  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("WRONG_PASS")));  break;
			case 7:  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("WRONG_ID")));  break;
			case 8:  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("ROOMID_ERROR")));  break;
			case 9:  
				{
					// [수호천사] 2004.07.08
					CNewAskChargeDlg BigDlg;
					BigDlg.DoModal();	
				//g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("LECK_PMONEY1")));  break;
				} break;
			case 10: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[알림]\n강제 추방된 방에는 다시 입장할 수 없습니다");break;// [게임 아이템 작업]
			case 15: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[알림]\n중복된 IP가 존재하여 입장할 수 없습니다"); break;// ### [ 중복된 IP가 ] ###
			case 16:  break;// [ 최소 머니 설정 ]
			// ### [ 관전기능 ] ###
			case 50: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[알림]\n게임에 참여예약 되었습니다."); break;
			case 51: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[알림]\n중복된 IP가 존재합니다"); break;
			case 52: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[알림]\n관전으로 이동할 수 없습니다"); break;
			default:  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[알림]\n요청하신 방에 입장할 수 없습니다"); 
			
			}
			bProccessed = FALSE;
		} break;

	case SV_ACCEPT_ENTERROOM: // 방에 접속 허가받음
		{
			CSV_ACCEPT_ENTERROOM msg;
			msg.Get(lpData, TotSize);

			static BOOL bProccessed = FALSE;
			if( bProccessed ) break;
			
			bProccessed = TRUE;
			// ### [ 관전기능 ] ###
			Game.Accept_EnterRoom(&msg);
			bProccessed = FALSE;

	
		} break;

	case SV_NEWROOM: // 새로운 방이 개설되었음
		{
			CSV_NEWROOM msg;
			msg.Get(lpData, TotSize);

			// ### [ 대기실 리스트 관련 ] ###
			m_LobyDlg.Room_InsItem(msg.RI);

			/*
			int roomnum = msg.RI->RoomNum;
			int gamekind = msg.RI->GameKind;
			char *title = msg.RI->Title;
			char *master = msg.RI->ID;
			int nowuser = msg.RI->NowUserNum;
			int maxuser = msg.RI->MaxUserNum;
			int secret = msg.RI->Secret;
			int state = msg.RI->State;
			int level = msg.RI->FormKind;
			int observer = msg.RI->NowObserverNum;// ### [ 관전기능 ] ### 
			BOOL superroom = msg.RI->bSuperMaster;	// [게임 아이템 작업]
			
			// ### [ 관전기능 ] ### 
			m_LobyDlg.Room_InsItem(roomnum, master, title, level, nowuser, maxuser, observer, secret, state, gamekind,superroom);	// [게임 아이템 작업]);
			*/
		} break;

	case SV_OUTROOM: // 방이 삭제되었음
		{
			CSV_OUTROOM msg;
			msg.Get(lpData, TotSize);

			int roomnum = msg.SRI->RoomNum;
			m_LobyDlg.Room_DelItem(roomnum);
		} break;

	case SV_CHANGEROOM: // 방정보가 갱신되었음
		{
			CSV_CHANGEROOM msg;
			msg.Get(lpData, TotSize);

			// ### [ 대기실 리스트 관련 ] ###
			m_LobyDlg.Room_SetItem(msg.SRI);


		/*
			int roomnum = msg.SRI->RoomNum;
			int gamekind = msg.SRI->GameKind;
			char *master = msg.SRI->ID;
			int nowuser = msg.SRI->NowUserNum;
			int maxuser = msg.SRI->MaxUserNum;
			int state = msg.SRI->State;
			int observer = msg.SRI->NowObserverNum;// ### [ 관전기능 ] ### 
			// ### [ 관전기능 ] ### 
			m_LobyDlg.Room_SetItem(roomnum, gamekind, master, nowuser, maxuser, observer, state);
		*/
			
			// [ ###잭팟### ] 
			g_TmpJackPotMoney = msg.SRI->JackPotMoney; // 갱신
		} break;

	case SV_ACCEPT_OUTROOM: // 퇴실 요청 허가됨
		{
			if(g_Where != WH_GAME) break;
			g_Wait=FALSE;

			// ### [ 관전기능 ] ###
			g_MyInfo.UI.cObserver = 0;
			g_MyObserver = FALSE;
			g_MyInfo.JoinState = 0;
			g_MyInfo.PlayState = 0;
			Play[0].SetNewPlayer(&g_MyInfo.UI);
			Play[0].nSndFxKind = g_MyInfo.nSndFxKind;

			g_pGameDlg->OnExitGame();
		} break;

	case SV_CHANGEMYINFO: // 내 정보가 변경됨
		{
			CSV_CHANGEMYINFO msg;
			msg.Get(lpData, TotSize);
			
			// ### [ 관전기능 ] ###
			g_MyInfo.UI.WinNum   = Play[0].UI.WinNum = msg.pCMI->WinNum;
			g_MyInfo.UI.LooseNum = Play[0].UI.LooseNum = msg.pCMI->LooseNum;
			g_MyInfo.UI.DrawNum = Play[0].UI.DrawNum = msg.pCMI->DrawNum;
			Play[0].UI.PMoney = msg.pCMI->PMoney;
			if(Play[0].UI.PMoney < 0) Play[0].UI.PMoney = 0;
			g_MyInfo.UI.PMoney   = Play[0].UI.PMoney;
			g_MyInfo.UI.nIcon = Play[0].UI.nIcon = GetPokerLevel(Play[0].UI.PMoney);

			g_pLobyDlg->Invalidate(FALSE);
			
		} break;
		
	case SV_USERENTERROOM:  // 새로운 플레이어가 입실하였음
		{
			CSV_USERENTERROOM msg;
			msg.Get(lpData, TotSize);

			// ### [ 관전기능 ] ###
			Game.User_EnterRoom(&msg);
		} break;
			
	case SV_USEROUTROOM: // 사용자가 방에서 퇴장했음
		{
			CSV_USEROUTROOM msg;
			msg.Get(lpData, TotSize);
			
			// ### [ 관전기능 ] ###
			Game.User_OutRoom(&msg);
		} break;

	case SV_CHANGEROOMMASTER: // 방장이 바뀌어야함
		{
			CSV_CHANGEROOMMASTER msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_GAME) break;
			if(strlen(msg.ID)==0) break;

			BOOL bBreak = TRUE;
			for(int i=0; i<g_Max_Player; i++)
			{
				if(strcmp(msg.ID, Play[i].UI.ID) == 0) { bBreak = FALSE; break; }
			}
			if(bBreak == TRUE) break;
			
			// 방 정보를 변경한다
			strcpy(g_RI.ID, msg.ID);
			g_RI.UNum = *msg.UNum;
			
			// 메인 타이틀 텍스트를 갱신한다
			CString rlevel,style;
			if(g_RI.Level == 2) rlevel = g_StrMan.Get(_T("NUM_3"));
			else if(g_RI.Level == 1) rlevel = g_StrMan.Get(_T("NUM_2"));
			else rlevel = g_StrMan.Get(_T("NUM_1"));

			
			if(g_RI.GameKind == 2) style = g_StrMan.Get(_T("FULL"));
			else if(g_RI.GameKind == 1) style = g_StrMan.Get(_T("HALF"));
			else style = g_StrMan.Get(_T("QUATER"));

			// 후에 변경할것
			CString str;
			str.Format(g_StrMan.Get(_T("ROOM_TITLE")),
				"Good Boy 7 Poker", g_ChannelName, g_RI.RoomNum, g_RI.Title, g_RI.ID,style,rlevel,
				g_RI.Secret==0?g_StrMan.Get(_T("L_OPEN")):g_StrMan.Get(_T("L_PASS")),g_StrMan.Get(_T("WHISNO")) );
			SetWindowText(str);

			// ### [ 관전기능 ] ###
			if(strcmp(msg.ID, g_MyInfo.UI.ID) == 0) // 내가 방장이 된다
			{
				g_MastPN = 0;

				CString str;
				str.Format("## 당신은 방장입니다 ##\n");
				g_pGameView->ChatBox.AddText(&str);				
				g_pGameView->MasterBtn.Enable(TRUE); // 방장옵션 비활성 여부 [62]
			}
			else
			{
				int newMastPN = -1;
				for(int i=1; i<g_Max_Player; i++) 
					if(strcmp(Play[i].UI.ID, msg.ID)==0) {newMastPN=i; break;}

				if(newMastPN == -1) break;

				g_MastPN = newMastPN;
				
				CString str;
				str.Format("## [%s]님이 방장입니다 ##\n", msg.ID);
				g_pGameView->ChatBox.AddText(&str);
			}

		} break;

	case SV_ABNORMALOUTROOM: // 비정상적으로 퇴장함
		{
			CSV_ABNORMALOUTROOM msg;
			msg.Get(lpData, TotSize);
			
			g_Wait=FALSE;
			if(g_Where != WH_GAME) break;
			
			// 비정상적인 퇴실임을 설정하고 게임을 종료
			m_GameDlg.bAbnormalOutRoom = TRUE;
			m_GameDlg.ExitGame();
		} break;

	case SV_ASK_INVITE: // 플레이어가 초청 메세지를 보냄
		{
			CSV_ASK_INVITE msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_LOBY) break;

			int tarunum = msg.MyUI->UNum;
			char *tarid = msg.MyUI->ID;
			int roomnum = *msg.RoomNum;
			char strmsg[256]={0,};
			strncpy(strmsg, msg.StrMsg, *msg.l_StrMsg);

			if(strlen(tarid)==0) break;
			
			// 초대 거부중이면 자동으로 거부한다
			g_pLobyDlg->UpdateData(TRUE);
			if(g_pLobyDlg->m_RefuseInviteCheck==TRUE)
			{
				CString str;
				str.Format("[%s]님은 <초대 거부>중입니다.", Play[0].UI.ID);
				CSV_REFUSE_INVITE rimsg;
				rimsg.Set(tarunum, tarid, Play[0].UI.ID, (char*)str.operator LPCTSTR());
				SockMan.SendData(g_MainSrvSID, rimsg.pData, rimsg.GetTotalSize());
				break;
			}
			
			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
			
			CRecvInviteDlg Dlg;
			int rtn = Dlg.DoModal(tarunum, tarid, roomnum, strmsg);
			if(g_Where == WH_LOBY && g_MainSrvSID != -1) {
				if(rtn == IDOK)
				{
					// ### [ 대기실 리스트 관련 ] ###
					g_pLobyDlg->m_RoomView.m_SelectIndex = roomnum;
					g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_ENTERGAME);
					
					/*
					// 방 번호를 문자열로 변환
					char strRoom[10]={0,};
					wsprintf(strRoom, "%d", roomnum);
					int num=-1;
					CString szRN;
					char szName[256]={0,};
					for(;;)
					{
					num = g_pLobyDlg->m_RoomList.GetNextItem(num, LVNI_ALL);
					if(num == -1) break;
					szRN = g_pLobyDlg->m_RoomList.GetItemText(num, 0);
					if(strcmp(strRoom, szRN.operator LPCTSTR())==0)
					{
					// 초대받은 방이 선택되게 한다
					g_pLobyDlg->nRoomSel = num;
					g_pLobyDlg->m_RoomList.SetItemState(num, LVIS_SELECTED, LVIS_SELECTED);
					g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_ENTERGAME);
					break;
					}
					}
					*/
				}
				else
				{
					CSV_REFUSE_INVITE rimsg;
					rimsg.Set(tarunum, tarid, Play[0].UI.ID, (char*)Dlg.m_RefuseMsg.operator LPCTSTR());
					SockMan.SendData(g_MainSrvSID, rimsg.pData, rimsg.GetTotalSize());
				}
			}

			bProccessed = FALSE;
			
		} break;

	case SV_REFUSE_INVITE: // 플레이어가 초청 메세지를 거부함
		{
			CSV_REFUSE_INVITE msg;
			msg.Get(lpData, TotSize);

			char strmsg[256]={0,};
			strncpy(strmsg, msg.StrMsg, *msg.l_StrMsg);

			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
	
			CRefuseInviteDlg Dlg;
			Dlg.DoModal(msg.MyID, strmsg);

			bProccessed = FALSE;
		} break;

	case SV_ASK_BANISHVOTE: // 추방 투표가 시작됨
		{
			CSV_ASK_BANISHVOTE msg;
			msg.Get(lpData, TotSize);

			if(g_Where!=WH_GAME) break;
			
			CString str;
			str.Format("[%s]님에대한 추방 투표가 진행중입니다\n", msg.pBV->TarID);
//			g_pGameDlg->m_ChatViewEdit.AddText(&str, RGB(255,0,0));
			g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(255,0,0));

			// 투표 요청자 혹은 대상자면 메시지만 출력
			if(strcmp(Play[0].UI.ID, msg.pBV->MyID)==0 || strcmp(Play[0].UI.ID, msg.pBV->TarID)==0) break;

			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
	
			CMyVoteDlg Dlg;
			Dlg.m_TargetID = msg.pBV->TarID;
			Dlg.m_BanishReson = msg.pBV->Reson;
			Dlg.DoModal();

			bProccessed = FALSE;
		} break;

	case SV_GAMEOVERRESULT: // 게임이 종료되었음
		{
			CSV_GAMEOVERRESULT msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_GAME) break;
			g_Wait = FALSE;
			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
			Game.BackTime = 0;
			Game.OnGameOver(msg.GOR);
			
			bProccessed = FALSE;
		} break;


	// ### [이벤트 티켓] ###
	case SV_EVENTPRIZE: // 이벤트 당첨 알림
		{
			CSV_EVENTPRIZE msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_GAME) break;
		//	Game.OnEventPrize(&msg);
		} break;
	case SV_EVENTNOTICE: // 이벤트 공지가 도착했음
		{
			CSV_EVENTNOTICE msg;
			msg.Get(lpData, TotSize);

			CString str;
			CString chat;
			chat = (LPCTSTR)msg.StrChat;
			chat = chat.Mid(0, *msg.l_StrChat);

			if(*msg.Kind==1) {
				str = chat;
				str += "\n";
				
				// text color
				COLORREF tcolor = *msg.Color;

				if(g_Where==WH_LOBY) {
					g_pLobyDlg->m_ChatViewEdit.SetTextFormat(0);
					g_pLobyDlg->m_ChatViewEdit.AddText(&str, tcolor);
				}
				if(g_Where==WH_GAME) {
					g_pGameDlg->m_GameView.ChatBox.AddText(&str, tcolor);
				}

			//	PlayScr[0].bEventLogo = TRUE;
			}

		} break;

	case SV_SERVERMSG: // 서버 알림 메세지가 도착했음
		{
			CSV_SERVERMSG msg;
			msg.Get(lpData, TotSize);

			CString str;
			CString chat;
			chat = (LPCTSTR)msg.StrChat;
			chat = chat.Mid(0, *msg.l_StrChat);

			if(*msg.Kind==11)
				str.Format("[개인공지]\n%s", chat.operator LPCTSTR());
			else
				str.Format("%s", chat.operator LPCTSTR());			// 메세지 변경 - jeong
				//str.Format("[서버 알림]\n%s", chat.operator LPCTSTR());

			int Kind = *msg.Kind;

			if(*msg.Kind==0 || *msg.Kind==2 || *msg.Kind==11) { // 머니제한공지
				str += "\n";
				COLORREF tcolor = RGB(255,0,0);
				if(*msg.bColorAssigned) tcolor = *msg.Color;

				if(g_Where==WH_LOBY)
				{
					g_pLobyDlg->m_ChatViewEdit.SetTextFormat(0);
					g_pLobyDlg->m_ChatViewEdit.AddText(&str, tcolor);
				}
				if(g_Where==WH_GAME)
				{
					g_pGameDlg->m_GameView.ChatBox.AddText(&str, tcolor);
					// 채팅 버튼 깜빡임
					Game.nChatRuleMessage = 1;
					//시작버튼 
					if(strcmp(g_RI.ID,g_MyInfo.UI.ID)==0 && g_RI.UNum == g_MyInfo.UI.UNum
						&& g_RI.NowUserNum <3 && g_RI.State == 0){
						g_pGameView->GameStartBtn.Show(FALSE);
						g_pGameView->X2StartBtn.Show(FALSE);
					}
				}
				break;
			}

			if(*msg.Kind==1)
			{
				//// 메시지 창 닫음
				//g_pTitleDlg->m_MsgDlg.OnOK();
				//::MessageBox(m_hWnd, str.operator LPCTSTR(), "서버로부터 메세지가 도착하였습니다.", MB_OK);
				m_MsgBoxDlg.MsgBox(1, str);
				break;
			}


			// 채널 입장 제한 메세지
			if(*msg.Kind==3)
			{
				// 메시지 창 닫음
				g_pTitleDlg->m_MsgDlg.OnOK();
				break;
			}

			// 서버 입장 제한 메세지
			if(*msg.Kind==5)
			{
				// 메시지 창 닫음
				// 다이얼 로그 박스로 보여준다. 아템 사라고 흐미(* 레벨이 한다계 위만 보여준다)
				m_ConnectMsgDlg.OnCancel();
				break;
			}

			if(*msg.Kind==4)
			{
				// 메시지 창 닫음
				g_pTitleDlg->m_MsgDlg.OnOK();
				//::MessageBox(m_hWnd, str.operator LPCTSTR(), "서버로부터 메세지가 도착하였습니다.", MB_OK);
				m_MsgBoxDlg.MsgBox(0, (char*)str.operator LPCTSTR());
				break;
			}

			// 채널 입장 제한 메세지
			if(*msg.Kind==8)
			{
				// 메시지 창 닫음
				g_pTitleDlg->m_MsgDlg.OnOK();
				//::MessageBox(m_hWnd, str.operator LPCTSTR(), "서버로부터 메세지가 도착하였습니다.", MB_OK);
				m_MsgBoxDlg.MsgBox(0, (char*)str.operator LPCTSTR());
				break;
			}

			// 파산창
			if(*msg.Kind == 9 ) // 오링되었슴.
			{
			//	str.Format("게임머니가 부족하여, 게임을 계속 진행할 수 없습니다. 
			//	뉴포커(62Cut)페이지 상단의 충전소 버튼을 클릭하여 게임머니를 충전 받으시기 바랍니다.");			
				/*
				if(m_Oringdlg.GetSafeHwnd() == NULL && m_Oringdlg.m_ShowDlg == FALSE){	
					m_Oringdlg.m_ShowDlg = TRUE;												
					if(m_Oringdlg.DoModal() == IDOK){
						g_pMainDlg->OnCancel();
					}
				}
				*/


				m_GameItemDlg.SafeClose();
				// [수호천사] 2004.07.08
				CNewAskChargeDlg BigDlg;
				//BigDlg.DoModal();				- 오링창 deleted by jeong
			}
			if(*msg.Kind == 30)//게임 시작 카운트를 보내줌
			{
				if(g_Where==WH_GAME &&  g_RI.State == 0){
					COLORREF tcolor = RGB(255,255,0);
					g_pGameDlg->m_GameView.ChatBox.AddText(&str, tcolor);
					// 채팅 버튼 깜빡임
					Game.nChatRuleMessage = 1;
				}
			}

		} break;

	case SV_CHATDATA: // 대기실 채팅데이타가 도착했음
		{
			CSV_CHATDATA msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_LOBY) break;

			CString str;
			CString chat;
			chat = (LPCTSTR)msg.StrChat;
			chat = chat.Mid(0, *msg.l_StrChat);

			//(추가)
			// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
			for(int s=0; s<chat.GetLength()-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13) {
					chat.SetAt(s, ' ');
				}
			}

			str.Format("%s> %s\n", msg.ID, chat.operator LPCTSTR());
			g_pLobyDlg->m_ChatViewEdit.SetTextFormat(0);
			
			if( strcmp("관리자",msg.ID)==0 )// ### [관리자 모드 작업] ###
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(255,0,0));
			else
				g_pLobyDlg->m_ChatViewEdit.AddText(&str);

			
		} break;

	case SV_WHISPERCHAT: // 귓속말
		{
			CSV_WHISPERCHAT msg;
			msg.Get(lpData, TotSize);

			// 대화 수신 거부중인 아이디이면 무시
			if(g_Where == WH_GAME) {
				int pnum = Game.GetPlayerPNum(msg.ID);
				if(pnum != -1) {
					if(Play[pnum].bShutMouse) break;
				}
			}
			
			CString str;
			CString chat;
			chat = (LPCTSTR)msg.StrChat;
			chat = chat.Mid(0, *msg.l_StrChat);

			//(추가)
			// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
			for(int s=0; s<chat.GetLength()-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13) {
					chat.SetAt(s, ' ');
				}
			}

			str.Format("[귓말:%s] %s\n", msg.ID, chat.operator LPCTSTR());

			// 귓말 알림 효과음 출력
			Sound.Play(SND50);

			if(g_Where==WH_LOBY)
			{
				g_pLobyDlg->m_ChatViewEdit.SetTextFormat(0);
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(100,100,100));
			}
			if(g_Where==WH_GAME)
			{
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(250,250,250));
				// 채팅 버튼 깜빡임
				Game.nChatRuleMessage = 1;

			}
		} break;

	// 게임방 채팅데이타
	case CL_CHATDATA:
		{
			CCL_CHATDATA msg;
			msg.Get(lpData, TotSize);

			if(g_Where != WH_GAME) break;
			if(*msg.RoomNum != g_RI.RoomNum) break;

			// 대화 수신 거부중인 아이디이면 무시
			int pnum = Game.GetPlayerPNum(msg.ID);
			if(pnum != -1) {
				if(Play[pnum].bShutMouse) break;
			}

			CString str;
			char chat[256]={0,};
			strncpy(chat, msg.StrChat, min(*msg.l_StrChat, 255));

			//(추가)
			// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
			int slen = strlen(chat);
			for(int s=0; s<slen-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(chat[s] == 8 || chat[s] == 9 || chat[s] == 10 || chat[s] == 13) {
					chat[s] = ' ';
				}
			}

			str.Format("%s> %s\n", msg.ID, chat);
			if( strcmp("관리자",msg.ID)==0 )// ### [관리자 모드 작업] ###
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, SERVERMSG_COLOR);
			else
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, CHATTING_COLOR);

			// 채팅 버튼 깜빡임
			Game.nChatRuleMessage = 1;

		} break;

			//2004.05.07 리더스총액 제한
	case SV_LEADERSJOIN:
		{
			CSV_LEADERSJOIN msg;
			msg.Get(lpData, TotSize);

			g_MyUniqNo = msg.LJ->UniqNo;

			CLeadersMoneyLimit dlg;
			dlg.Init(msg.LJ);
			
			if(dlg.DoModal() == IDOK) {
				//	g_pMainDlg->OnCancel();
				g_Wait = FALSE;
				bNowLogout = TRUE;
				OnCancel();
			}	
			else
			{
				ASK_LOGININFO al;
				memset(&al, 0, sizeof(ASK_LOGININFO));
				
#ifdef _DEBUG
				strcpy(al.ID, g_LoginID);
				strcpy(al.Pass, g_LoginPass);
#endif
				
				strcpy(al.SockNameIP, SockMan.pDataSock[g_MainSrvSID]->SockAddr);	// 연결된 소켓의 어드레스
				strcpy(al.HostNameIP, SockMan.GetServerIP());						// 호스트 시스템 어드레스
				al.Port = SockMan.pDataSock[g_MainSrvSID]->SockPort;				// 연결된 소켓의 포트번호
				// 서버용 소켓을 생성하기 위해 20000정도 더해준 포트번호를 사용한다
				// 실제 사용하게될 포트번호는 메인서버에 의하여 결정된다
				al.Port += 20000; 
				
				al.AuthCookieLen = g_AuthCookie.GetLength();
				al.DataCookieLen = g_DataCookie.GetLength();
				al.bForceLogin = TRUE;
				
				// 로그인 메세지를 보낸다
				CSV_ASK_LOGIN lgimsg;
				lgimsg.Set(&al, (char*)(LPCTSTR)g_AuthCookie, (char*)(LPCTSTR)g_DataCookie);
				SockMan.SendData(g_MainSrvSID, lgimsg.pData, lgimsg.GetTotalSize());
			}
			
		} break;

	/////////////////////////// 클라이언트용 메세지  ////////////////////////

	// ### [ 관전기능 ] ### 
	case CL_EXITSUBSCRIPT:
		{
			CCL_EXITSUBSCRIPT msg;
			msg.Get(lpData, TotSize);
			if(g_Where != WH_GAME) break;

			int pnum = Game.GetPlayerPNum(msg.ID);
			if(pnum==-1) break;

			int bexit = *msg.bExit;

			CString str;
			// ### [ 관전기능 ] ###
			if(bexit == 0)
			{
				if(strcmp(g_MyInfo.UI.ID, msg.ID) == 0)
				{// 나가기
					g_pGameView->bExitSubscript = FALSE;
					g_pGameDlg->ExitGame();
				}
			}
			else if(bexit==1)
			{
				if(strcmp(g_MyInfo.UI.ID, msg.ID) == 0)
				{// 나가기
					g_pGameView->bExitSubscript = TRUE;
				}
				str.Format("[%s]님 나가기 예약\n", Play[pnum].UI.ID);
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(150,150,0));
			}
			else if(bexit == 2)
			{
				if(strcmp(g_MyInfo.UI.ID, msg.ID) == 0)
				{// 나가기
					g_pGameView->bExitSubscript = FALSE;
				}
				str.Format("[%s]님 나가기 취소\n", Play[pnum].UI.ID);
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(150,150,0));
			}
		} break;

	/////////////////////////// 게임 진행용 메세지  ////////////////////////

	case CL_CHANGEGAMEKIND: // 게임 방식이 변경되었음
		{
		} break;

	case CL_STARTINFO: // 게임 시작 정보
		{
		} break;

	
	// 게임 시작
	case SV_STARTCARD:
		{
			CSV_STARTCARD msg;
			msg.Get(lpData, TotSize);
			if(g_Where != WH_GAME) break;
			if(g_RI.State != 0) break;

			int pnum = Game.GetPlayerPNum(msg.ID);
			if(pnum == -1) break;
			
			// ### [ 관전기능 ] ###
			if(strcmp(g_MyInfo.UI.ID, msg.ID) != 0) break;
			
			
			static BOOL bProccessed = FALSE;
			if(bProccessed) break;
			bProccessed = TRUE;
			
			g_RI.State = 1;
			g_RI.ChangeCardStep = 0;

			Game.DoPrepareGame(msg.pSC);

			bProccessed = FALSE;

			CString str;
			str.Format("%s\r\n", g_StrMan.Get(_T("NEW_GAMEMES")) );
			g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(255,255,0));

		}break;

	case SV_SELECTCARD:
		{
			CSV_SELECTCARD msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_GAME) break;
			if(g_RI.State != 1) break;

			int pnum = Game.GetPlayerPNum(msg.ID);
			if(pnum == -1) break;

			if(pnum < 0  || pnum >= g_Max_Player) break;
			if( !Play[pnum].m_bSelectCard ) break;
			
			
			if(g_RI.FormKind == 1 && Game.CardDeck[pnum].TotalCardNum == 3) {
					Play[pnum].m_bSelectCard = FALSE;
					break;
			}			
		
			/////////////////////////////////////////////////////////////////////
			// 오픈카드 선택시 각각 타이머 보여준다.
			// ### [ 관전기능 ] ###
			if( g_MyObserver ) {
				if(Game.bGameStart && pnum != 0 && g_RI.FormKind == 1 && Play[pnum].PlayState == 1) {
					// 추가 초이스 카드
					Game.CardDeck[pnum].Card[3].Clear();
					Game.CardDeck[pnum].TotalCardNum -= 1;
				}
			}
			else
			{
				if(Game.bGameStart && pnum != 0 && g_RI.FormKind == 1 && Play[pnum].PlayState == 1) {
					// 추가 초이스 카드
					Game.CardDeck[pnum].Card[3].Clear();
					Game.CardDeck[pnum].TotalCardNum -= 1;
				}
			}
			/////////////////////////////////////////////////////////////////////
			Play[pnum].m_bSelectCard = FALSE;

		} break;

	// 유저 모두 카드날리기 종료
	case SV_WINDCARDEND : 
		{
			CSV_WINDCARDEND msg;
			msg.Get(lpData, TotSize);
			if(g_Where != WH_GAME) break;
			if(g_RI.State != 1) break;

			int pnum = *msg.Reserve;
			if(pnum < 0 || pnum >= g_Max_Player) break;
			
			static BOOL bProccessed = FALSE;
			if(bProccessed) break;
			bProccessed = TRUE;
			
			Game.EndWindCard();

			bProccessed = FALSE;

		}
		break;

	case SV_RAISE:
		{
			CSV_RAISE msg;
			msg.Get(lpData, TotSize);
			if(g_Where != WH_GAME) break;
			if(g_RI.State != 1) break;			

			//[62]
			g_bChangeCardStage = FALSE;
			g_pGameView->ChangeCard_OK.Enable(FALSE);
			g_pGameView->ChangeCard_Cancel.Enable(FALSE);
			Game.BackTime = 0;			
			Game.Raise(msg.pPG);
		
		}
		break;

	// 유저가 폴드
	case SV_FOLDUSER:
		{
			CSV_FOLDUSER msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_GAME) break;
			if(g_RI.State != 1) break;
			if(strlen(msg.ID)==0) break;
			int pnum  = -1;
			for(int i=0; i<g_Max_Player; i++) {
				if(strcmp(msg.ID, Play[i].UI.ID) == 0) { pnum = i; break; }
			}
			if(pnum == -1) break;
			g_TmpJackPotMoney = *msg.JackPotMoney; // [ ###잭팟### ] 

			int k=0;
			if(Play[pnum].PlayState == 1 && Play[pnum].JoinState == 1) { // 현재 겜참여 중인가
				Play[pnum].bFold = TRUE;
				Play[pnum].PlayState = 0;
				
				if(Play[pnum].PNum == Play[0].PNum)g_bChangeCardStage = FALSE; // 62cut mode reset[62]

				if( !g_MyObserver && pnum== 0) { // ### [ 관전기능 ] ###
					g_MyInfo.bFold = TRUE;
					g_MyInfo.PlayState = 0;
				}
				//현재 겜중인 넘 나갔다.
				Game.StartUserNum--;
				if(Game.StartUserNum <0) Game.StartUserNum = 0;
			
				if(pnum == 0 && Play[0].nCardTotal>3) {// ########### 게임 포기시 카드를 다시 그린다
					for( k = 0; k < Play[0].nCardTotal; k++) {
						CPoint pt;
						pt.x = Game.CardDeck[0].Card[k].Xp;
						if( g_MyObserver ) pt.y = Game.CardDeck[0].Card[k].Yp;	// ### [ 관전기능 ] ###
						else pt.y = Game.CardDeck[0].Card[0].Yp;
						Game.CardDeck[0].Card[k].m_move.SetCurPos(pt);
					}					
					g_pGameView->SetBtnState();
				}
				
				if(Play[pnum].nCardTotal > 0) {
					for( k = 0; k < Play[pnum].nCardTotal; k++) {
						Game.CardDeck[pnum].SetFaceDown(k);
					}
				}
				Game.SoundFxKind(1, Play[pnum].UI.Sex, Play[pnum].nSndFxKind);// ### [사운드 추가 작업] ### 다이
				CString str;
				str.Format(g_StrMan.Get(_T("FOLD_USER")),Play[pnum].UI.ID);
				g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(220,220,0));
			}

		} break;
	// 마지막 히든 카드
	case SV_HIDDENCARD:
		{
			CSV_HIDDENCARD msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_GAME) break;
			if(g_RI.State != 1) break;

			if(strlen(msg.ID)==0) break;
			if( g_MyObserver ) break;

			int pnum = -1;
			for(int i=0; i<g_Max_Player; i++) {
				if(strcmp(msg.ID, Play[i].UI.ID) == 0) { pnum = i; break; }
			}
			if(pnum != 0) {
#ifdef _DEBUG
			AfxMessageBox("Hidden Error");			
#endif			break;
			}
			
			if(Play[pnum].PlayState == 1 && Play[pnum].JoinState == 1)
			{
				Game.CardDeck[0].Card[6].CardNo = *msg.nCard; // 값 변경
			}

		} break;
		
	case SV_ASK_CHANGECARD_62CUT://[62]
		{
			CSV_ASK_CHANGECARD_62CUT msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_GAME) break;
			if(g_RI.State != 1) break;
			
			g_Poker.CurPlayer = -1;
			Game.CurPlayer = -1;
			
			if(msg.pCC->nFlag == 6){// 바꾸기 시작
				g_RI.ChangeCardStep = msg.pCC->nChangeStep;	
				Game.ChangeCard_Init();
			}		
			else if(msg.pCC->nFlag == 100){//바꾼 카드 정보를 받아서 카드를 바꾼다							
				Game.SetOtherChangeCard_62Cut(*msg.pCC);
			}
		}break;
	case SV_ASK_CHANGECARD_62CUT_EX://[62]
		{
			CSV_ASK_CHANGECARD_62CUT_EX msg;
			msg.Get(lpData, TotSize);			
			
			if(g_Where != WH_GAME) break;
			if(g_RI.State != 1) break;
			if(g_RI.RoomNum != msg.pCC->nRoomNum)break;

			g_Poker.CurPlayer = -1;
			Game.CurPlayer = -1;
			
			//바꾼 카드 정보를 받아서 카드를 바꾼다					
			Game.SetOtherChangeCard_62Cut_Ex(*msg.pCC);
						
		}break;
	case SV_ACTIVE_STARTBUTTON:
		{
			CSV_ACTIVE_STARTBUTTON msg;			
			msg.Get(lpData, TotSize);			
			
			//softpark
			//if(g_Where != WH_GAME) break;
			//if(g_RI.State != 0) break;
			//if(g_RI.RoomNum != *msg.RoomNum)break;
			
			Game.OnStartButtonActive(*msg.WinnerServPNum,msg.WinnerID,*msg.bActive);

		}break;

	// 나의 게임 결과 == > 사용 않함
	case CL_USERRESULT:
		{
			CCL_USERRESULT msg;
			msg.Get(lpData, TotSize);
			if(g_Where != WH_GAME) break;

			if(strlen(msg.pRS->ID)==0) break;

			int pnum = -1;
			for(int i=0; i<g_Max_Player; i++)	{
				if(strcmp(msg.pRS->ID, Play[i].UI.ID) == 0) { pnum = i; break; }
			}
			if(pnum == TRUE) break;

			memcpy(&Play[pnum].UI, &msg.pRS->UI, sizeof(USERINFO));
		
		} 
		break;

	case SV_ENDGAME:
		{
			CSV_ENDGAME msg;
			msg.Get(lpData, TotSize);
			if(g_Where != WH_GAME) break;

			
			// 나가기 예약상태면 게임 종료
			if(g_pGameView->bExitSubscript) {
				g_pGameDlg->ExitGame();
			}
			else {
				// 게임 리셋 예약
				Game.ResetGame();
				Game.CurPlayer = g_Poker.CurPlayer;
			}
		}
		break;

	// [ ###바로시작### ]
	case SV_REFUSE_QUICKSTART:
			{
				static BOOL bProcessed = FALSE;
				if(bProcessed) break;
				bProcessed = TRUE;
				g_pLobyDlg->CreateRoom();
				bProcessed = FALSE;
			}
			break;
	case SV_ENDRAISE: //하이로우
		{
		}
		break;
	
	// [ ###잭팟### ] 
	case SV_SPECIALCARD :
		{
			CSV_SPECIALCARD msg;
			msg.Get(lpData, TotSize);
		

			CString spec("");
			if(msg.pSC->nCard==1) spec = g_StrMan.Get(_T("SPECIAL0"));
			else if(msg.pSC->nCard==2) spec = g_StrMan.Get(_T("SPECIAL1"));
			else if(msg.pSC->nCard==3) spec = g_StrMan.Get(_T("SPECIAL2"));

			CString str("");
			if(msg.pSC->JackPotPlus>0)
			{
				CString strM = NumberToOrientalString(msg.pSC->JackPotPlus);
				strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
				str.Format(g_StrMan.Get(_T("MES_SPE2")),
				msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, spec, strM);

				if(g_Where != WH_GAME)
				{
					Sound.Play(SND36); // 팡파레
				}
		
			}
			else
			{
				str.Format(g_StrMan.Get(_T("MES_SPE3")),
				msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, spec);
			}
		/*
			switch (msg.pSC->nCard)
			{
			case 1:
			//	str.Format("%s의 %d번방 %s님이 포카드 하셨습니다. 잭팟 상금 %I64d \n", msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, msg.pSC->JackPotPlus );
				str.Format(g_StrMan.Get(_T("MES_SPE1")),
				msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, msg.pSC->JackPotPlus, spec);
				break;
			case 2:
			//	str.Format("%s의 %d번방 %s님이 스트레이트 플러쉬 하셨습니다. 잭팟 상금 %I64d \n", msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID , msg.pSC->JackPotPlus);
				str.Format(g_StrMan.Get(_T("MES_SPE1")),
				msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, msg.pSC->JackPotPlus, spec);
				break;
			case 3:
			//	str.Format("%s의 %d번방 %s님이 로얄스트레이트 플러시 하셨습니다. 잭팟 상금 %I64d \n", msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, msg.pSC->JackPotPlus );
				str.Format(g_StrMan.Get(_T("MES_SPE1")),
				msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, msg.pSC->JackPotPlus, spec);
				break;
			}
		*/
		
			// 같은방에서 나왔다
			if(msg.pSC->JackPotPlus > 0 && strcmp(msg.pSC->ChanName, g_ChannelName)==0 && msg.pSC->nRoom == g_RI.RoomNum )
			{
				Sound.Play(SND36); // 팡파레
				for(int i=0; i < g_Max_Player; i++)
				{
					if(strlen(Play[i].UI.ID) == 0) continue;
					if(strcmp(msg.pSC->ID, Play[i].UI.ID)==0)
					{
						Play[i].UI.PMoney += msg.pSC->JackPotPlus;
						Play[i].UI.nIcon = GetPokerLevel(Play[i].UI.PMoney);
						break;
					}
				}
				
				Game.m_JackPotActive = msg.pSC->JackPotPlus; // 나의 게임방에서 나왔다.
				Sound.Play(SND31); // 잭팟 칩소리

			}

			g_TmpJackPotMoney = msg.pSC->NowJackPot;
			/*
			if(g_Where != WH_GAME)
			{							
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(0,0,255));
			}
			else
			{				
				g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(220,220,0));
			}
			*/
		}
		break;
			//### [관리자 모드 작업] ###
	case NM_ADMIN_WHISPERCHAT:
		{
			CSV_WHISPERCHAT msg;
			msg.Get(lpData, TotSize);
			
			
			CString str;
			CString chat;
			chat = (LPCTSTR)msg.StrChat;
			chat = chat.Mid(0, *msg.l_StrChat);
			
			//(추가)
			// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
			for(int s=0; s<chat.GetLength()-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13) {
					chat.SetAt(s, ' ');
				}
			}
			str.Format("[귓말:%s] %s\n", "관리자", chat.operator LPCTSTR());
			// 귓말 알림 효과음 출력
			Sound.Play(SND50);
			
			if(g_Where==WH_LOBY) {
				g_pLobyDlg->m_ChatViewEdit.SetTextFormat(0);
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(255,50,100));
			}

			if(g_Where==WH_GAME)
			{
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(255,50,100));
				
			}
		} break;

		//### [관리자 모드 작업] ###  방정보 변경
	case SV_CHANGESTYLEROOMINFO:
		{
			// 현재 위치가 겜방인가, 대기실 인가
			CSV_CHANGESTYLEROOMINFO msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where == WH_GAME)
			{
				strncpy(g_RI.Title, msg.SRI->Title, 30);
				
				CString rlevel,style;
				rlevel = g_StrMan.Get(_T("NUM_1"));
				
				if(g_RI.GameKind == 2) style = g_StrMan.Get(_T("FULL"));
				else if(g_RI.GameKind == 1) style = g_StrMan.Get(_T("HALF"));
				else style = g_StrMan.Get(_T("QUATER"));
				
				CString str;
				str.Format(g_StrMan.Get(_T("ROOM_TITLE")),
					"Good Boy 7 Poker", g_ChannelName, g_RI.RoomNum, g_RI.Title, g_RI.ID,style,
					g_RI.Secret==0?g_StrMan.Get(_T("L_OPEN")):g_StrMan.Get(_T("L_PASS")), g_StrMan.Get(_T("WHISNO") ));
				SetWindowText(str);
				
				// ### 타이틀 ###
				
				
				str.Format("[%s] - [%d번방] [방제목: %s]", g_ChannelName, g_RI.RoomNum, g_RI.Title);
				g_pGameView->m_titlestr=str;
				
				str.Format("방정보가  변경되었습니다.\n");
				g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(220,220,0));
			}
			else if(g_Where == WH_LOBY)
			{
				// ### [ 대기실 리스트 관련 ] ###
				m_LobyDlg.Room_SetItem(msg.SRI, TRUE);
/*
				int roomnum = msg.SRI->RoomNum;
				m_LobyDlg.Room_SetItem(roomnum, 0, "", 0, 0, 0,0, msg.SRI->Title, TRUE);// ### [ 관전기능 ] ###
*/
			}
		} break;

	case NM_SPECIALMESSAGE:
		{
			CNM_SPECIALMESSAGE msg;
			msg.Get(lpData, TotSize);

			while(1) {
			}

		} break;
	}

	return 1;
}

void C62CutPokerDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}

void C62CutPokerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillBadUserTimer();
	Sound.Destroy();
}

void C62CutPokerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(nIDEvent == BADUSER_TIMER)
	{
		KillBadUserTimer();
	}

	CDialog::OnTimer(nIDEvent);
}

void C62CutPokerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( g_pLobyDlg->m_bFullScr == TRUE ) return;

	SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}



// ### 아이콘 생성 ##
void C62CutPokerDlg::CreateShortCut()
{
	// 프로그램 이름
	LPCTSTR strProgName = "Good Boy 7 Poker";
	// URL
	LPCSTR pszURL = "http://www.goodboy7poker.com";//"http://NewPoker.goodboy7poker.com";
	// 바로가기 이름
	LPCSTR pszlnkName = "NewPoker";
	// 바로가기 설명
	LPCSTR szDescription = "Good Boy 7 Poker (NewPoker)\n홈페이지로 이동합니다.";
	

	CRegKey Key;
	int ShortCutVersion = 0;

	// 바로가기 아이콘의 버전을 읽어옴
	char strkey[500]={0,};
	::wsprintf(strkey, "Software\\Netmarble\\%s", strProgName);
	int rtn = Key.Open(HKEY_LOCAL_MACHINE, strkey); 
if(rtn == ERROR_SUCCESS) {
		char verstr[100]={0,};
		unsigned long vstrlen = 100;
		rtn = Key.QueryValue(verstr, "ShortCutVersion", &vstrlen);
		if(rtn == ERROR_SUCCESS) {
			ShortCutVersion = ::atoi(verstr);
			if(ShortCutVersion < VERSION) {
				// 레지스트리에 바로가기 아이콘의 버전을 설정(키가 없을땐 키를 생성)
				char verstr[100]={0,};
				wsprintf(verstr, "%d", VERSION);
				Key.SetValue(verstr, "ShortCutVersion");

				// 실행파일명을 얻음(실행파일의 아이콘을 사용하기 위함)
				char szFileName[MAX_PATH]={0,};
				GetModuleFileName(AfxGetInstanceHandle(), szFileName, MAX_PATH-1);
				// 인터넷 바로가기 아이콘을 바탕화면에 생성
				CreateInternetShortCutToDesktopFolder(pszURL, pszlnkName, szDescription, szFileName, 0);
			}
		}
	}
}



// [게임 아이템 작업]
void C62CutPokerDlg::BrowserMove(LPCTSTR targeturl) 
{ 	
	if(!targeturl) return;
	if(strlen(targeturl)==0) return;

	#if(0) // 기존 방식(사용 안함)				
		// 아바타 모듈 기본 폴더 얻기
		//char strbuf[1024] = {0,}; 
		char strbuf[1024] = "Avatar"; 
		//GetBaseAvatarDir(strbuf, 1023); 

		// 브라우저 제어 프로그램
		CString exeName = strbuf; 
		exeName += "\\NMIEMoveMFC.exe"; 

		// 넷마블 웹페이지 이동
		::ShellExecute( NULL, "open", exeName, targeturl, NULL, SW_HIDE );
	#else // 새로운 방식
		OpenNetmarbleWebpage(g_MyUniqNo, targeturl);
	#endif
} 

// [게임 아이템 작업]
void C62CutPokerDlg::ShowItemState()
{
	CString str;

	// 현재 시간을 얻어서 서버의 시간으로 변환
	CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);

	CTimeSpan tmptime=0; // [수호천사] 2004.07.09
	int tmp = -1;

	// 아이템의 상태를 출력
	for(int i=0; i<g_MyGameItem.nNum; i++) {
		ItemState state = GameItem_GetItemState(g_MyGameItem.Item[i], &st);
		if(state.iState == 2) {

				// [수호천사] 2004.07.09
			if( ITEM_SCODE_SAFEGUARDANGEL == GameItem_GetItemKind(g_MyGameItem.Item[i].Code) ) {		
				if(tmptime < state.LeftTime) {
					tmptime = state.LeftTime;
					tmp = i;
				}
				continue;
			}

			if(state.LeftTime.GetDays() > 0) {
				str.Format("◆ [%s] 사용중 (종료까지 %d일 %d시간 %d분 남았습니다.)\n", 
					GameItem_GetItemName(g_MyGameItem.Item[i]), 
					state.LeftTime.GetDays(), state.LeftTime.GetHours(), state.LeftTime.GetMinutes());
			}
			else {
				str.Format("◆ [%s] 사용중 (종료까지 %d시간 %d분 남았습니다.)\n", 
					GameItem_GetItemName(g_MyGameItem.Item[i]), 
					state.LeftTime.GetHours(), state.LeftTime.GetMinutes());
			}

			g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(200,0,0));
		}
	}

	// [수호천사] 2004.07.09
	if(tmp != -1) {
		if(tmptime.GetDays() > 0) {
			str.Format("◆ [%s] 사용중 (종료 까지 %d일 %d시간 %d분 남았습니다.)\n", 
				GameItem_GetItemName(g_MyGameItem.Item[tmp]), 
				tmptime.GetDays(), tmptime.GetHours(), tmptime.GetMinutes());
		}
		else {
			str.Format("◆ [%s] 사용중 (종료 까지 %d시간 %d분 남았습니다.)\n", 
				GameItem_GetItemName(g_MyGameItem.Item[tmp]), 
				tmptime.GetHours(), tmptime.GetMinutes());
		}
		
		g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(200,0,0));
	}
}

// [게임 아이템 작업]
void C62CutPokerDlg::ShowMyItemBag()
{
	// 다이얼로그 아이템 정보 초기화
	m_GameItemDlg.m_ItemListDlg.m_StrMsg = "";
//	m_GameItemDlg.m_ItemListDlg.SetItemList(g_MyGameItem.nNum, g_MyGameItem.Item);
	// 게임 아이템 대화상자
	m_GameItemDlg.DoModaless();
}

// [PC방 작업]
void C62CutPokerDlg::URLEncode(char* output, char* input)
{
    int opt_inx=0, ipt_inx=0;

	if(strlen(input) > 7) {
		if(strnicmp(input, "http://", 7) == 0) {
			ipt_inx = 7;
			opt_inx = 7;
			strncpy(output, input, 7);
		}
	}

    for ( ; input[ipt_inx]; ipt_inx++,opt_inx++)
    {
		int char_val = input[ipt_inx]; 
        if ( char_val < 0 ) char_val +=256; 


        if (
            char_val <= 0x1F || 
            char_val == 0x7F ||
            char_val >=0x80 ||
            char_val == ' ' ||
            char_val == '{' ||
            char_val == '}' ||
            char_val == '[' ||
            char_val == ']' ||
            char_val == '|' ||
            char_val == '\\' ||
            char_val == '^' ||
            char_val == '~' ||
            char_val == '`' ||
            char_val == '#' ||
            char_val == ';' ||
            char_val == '/' ||
            char_val == '?' ||
            char_val == '@' ||
            char_val == '=' ||
            char_val == '&'     )
        {
            output[opt_inx]='%';
            
            int UpperBit = char_val / 0x10;

            if (UpperBit >=0 && UpperBit <=9 )
                output[++opt_inx]= UpperBit+'0';
            else 
                output[++opt_inx]= UpperBit+'A'-10;
            

            int LowerBit = char_val % 0x10;
            if (LowerBit >=0 && LowerBit <=9 )
                output[++opt_inx]= LowerBit+'0';
            else 
                output[++opt_inx]= LowerBit+'A'-10;
        }
        else
            output[opt_inx]=char_val;    
    
    }

    output[opt_inx]=0;
}


void C62CutPokerDlg::DoScreenCapture()
{
	CTime time = CTime::GetCurrentTime();
	CString fname;

	fname.Format("SevenPoker2_%d_%d_%d_%d_%d.jpg", time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	if(ScreenCapture(GetSafeHwnd(), fname)) {
		g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"스크린 샷을 \'내 문서\'폴더에 저장하였습니다.");
	}
}


void C62CutPokerDlg::LoadSpriteFile()
{
	if(GameBackSpr.Load(".\\data\\GameBack.spr", 555)==FALSE) AfxMessageBox("GameBack.spr 파일을 읽을 수 없습니다");
//	if(GameBackSpr[1].Load(".\\data\\skin1\\GameBack1.spr", 555)==FALSE) AfxMessageBox("GameBack1.spr 파일을 읽을 수 없습니다");
//	if(GameBackSpr[2].Load(".\\data\\skin2\\GameBack2.spr", 555)==FALSE) AfxMessageBox("GameBack2.spr 파일을 읽을 수 없습니다");

	if(LevelSpr.Load(".\\data\\Level.spr", 555)==FALSE) AfxMessageBox("Level.spr 파일을 읽을 수 없습니다");
	if(CharBackSpr.Load(".\\data\\CharBack.spr", 555)==FALSE) AfxMessageBox("CharBack.spr 파일을 읽을 수 없습니다");
	if(InfoBoxSpr.Load(".\\data\\InfoBox.spr", 555)==FALSE) AfxMessageBox("InfoBox.spr 파일을 읽을 수 없습니다");
	if(ChatBoxSpr.Load(".\\data\\ChatBox.spr", 555)==FALSE) AfxMessageBox("ChatBox.spr 파일을 읽을 수 없습니다");
	if(ExitBtnSpr.Load(".\\data\\ExitBtn.spr", 555)==FALSE) AfxMessageBox("ExitBtn.spr 파일을 읽을 수 없습니다");
	//if(InviteBtnSpr.Load(".\\data\\InviteBtn.spr", 555)==FALSE) AfxMessageBox("InviteBtn.spr 파일을 읽을 수 없습니다");
	if(FullScreenBtnSpr.Load(".\\data\\FullScreenBtn.spr", 555)==FALSE) AfxMessageBox("FullScreenBtn.spr 파일을 읽을 수 없습니다");
	if(WindowBtnSpr.Load(".\\data\\WindowBtn.spr", 555)==FALSE) AfxMessageBox("WindowBtn.spr 파일을 읽을 수 없습니다");
	//if(OptionBtnSpr.Load(".\\data\\OptionBtn.spr", 555)==FALSE) AfxMessageBox("OptionBtn.spr 파일을 읽을 수 없습니다");
	//if(BadUserBtnSpr.Load(".\\data\\BadUserBtn.spr", 555)==FALSE) AfxMessageBox("BadUserBtn.spr 파일을 읽을 수 없습니다");
	//if(MasterBtnSpr.Load(".\\data\\MasterBtn.spr", 555)==FALSE) AfxMessageBox("MasterBtn.spr 파일을 읽을 수 없습니다");
	//if(MinimizeBtnSpr.Load(".\\data\\MinimizeBtn.spr", 555)==FALSE) AfxMessageBox("MinimizeBtn.spr 파일을 읽을 수 없습니다");
	////
	

	// 게임 진행 관련 이미지 
	//if(GameStartBtnSpr.Load(".\\data\\MainStartBtn.spr", 555)==FALSE) AfxMessageBox("MainStartBtn.spr 파일을 읽을 수 없습니다");
	if(GameStartBtnSpr.Load(".\\data\\StartBtn.spr", 555)==FALSE) AfxMessageBox("MainStartBtn.spr 파일을 읽을 수 없습니다");
	if(GameQuitBtnSpr.Load(".\\data\\Mini\\endBtn.spr", 555)==FALSE) AfxMessageBox("endBtn.spr 파일을 읽을 수 없습니다");
	if(X2StartBtnSpr.Load(".\\data\\Mini\\bonusG.spr", 555)==FALSE) AfxMessageBox("bonusG.spr 파일을 읽을 수 없습니다");
	if(AvaSpr.Load(".\\data\\aba.spr", 555)==FALSE) AfxMessageBox("aba.spr 파일을 읽을 수 없습니다");
	if(AllinBtnSpr.Load(".\\data\\Allinbtn.spr", 555)==FALSE) AfxMessageBox("AllinBtn.spr 파일을 읽을 수 없습니다");
	if(DieBtnSpr.Load(".\\data\\FoldBtn.spr", 555)==FALSE) AfxMessageBox("DieBtn.spr 파일을 읽을 수 없습니다");
	if(CheckBtnSpr.Load(".\\data\\CheckBtn.spr", 555)==FALSE) AfxMessageBox("CheckBtn.spr 파일을 읽을 수 없습니다");
	if(CallBtnSpr.Load(".\\data\\CallBtn.spr", 555)==FALSE) AfxMessageBox("CallBtn.spr 파일을 읽을 수 없습니다");
	if(PingBtnSpr.Load(".\\data\\PingBtn.spr", 555)==FALSE) AfxMessageBox("PingBtn.spr 파일을 읽을 수 없습니다");
	if(DadangkBtnSpr.Load(".\\data\\DadangkBtn.spr", 555)==FALSE) AfxMessageBox("DadangkBtn.spr 파일을 읽을 수 없습니다");
	if(MaxBtnSpr.Load(".\\data\\MaxBtn.spr", 555)==FALSE) AfxMessageBox("MaxBtn.spr 파일을 읽을 수 없습니다");

	//[62]
	if(ChangeOKBtnSpr.Load(".\\data\\changeokBtn.spr", 555)==FALSE) AfxMessageBox("changeokBtn.spr 파일을 읽을 수 없습니다");
	if(ChangeCancelBtnSpr.Load(".\\data\\changecancelBtn.spr", 555)==FALSE) AfxMessageBox("changecancelBtn.spr 파일을 읽을 수 없습니다");
	if(ChangeCardInfoSpr.Load(".\\data\\changecardinfo.spr", 555)==FALSE) AfxMessageBox("changecardinfo.spr 파일을 읽을 수 없습니다");	

	if(RoomMasterSpr.Load(".\\data\\roommaster.spr", 555)==FALSE) AfxMessageBox("roommaster.spr 파일을 읽을 수 없습니다");
	if(BanishVoteSpr.Load(".\\data\\banishvote.spr", 555)==FALSE) AfxMessageBox("banishvote.spr 파일을 읽을 수 없습니다");
//	if(SortSpr.Load(".\\data\\sortbtn.spr", 555)==FALSE) AfxMessageBox("sortbtn.spr.spr 파일을 읽을 수 없습니다");

	///	
	//if(EtcBtnSpr.Load(".\\data\\EtcBtn.spr", 555)==FALSE) AfxMessageBox("EtcBtn.spr 파일을 읽을 수 없습니다");
	if(EtcSpr.Load(".\\data\\Etc.spr", 555)==FALSE) AfxMessageBox("Etc.spr 파일을 읽을 수 없습니다");
	///////////////////////////////////////////////////////////////////////////
	if(BigCardSpr.Load(".\\data\\BigCard.spr", 555)==FALSE) AfxMessageBox("BigCard.spr 파일을 읽을 수 없습니다");
	if(SmallCardSpr.Load(".\\data\\SmallCard.spr", 555)==FALSE) AfxMessageBox("SmallCard.spr 파일을 읽을 수 없습니다");

	if(FocusSpr.Load(".\\data\\Focus.spr", 555)==FALSE) AfxMessageBox("Focus.spr 파일을 읽을 수 없습니다");
	if(TimerSpr.Load(".\\data\\Timer.spr", 555)==FALSE) AfxMessageBox("Timer.spr 파일을 읽을 수 없습니다");
	
	// [ ###잭팟### ] 
	if(JackPotBackSpr.Load(".\\data\\JackPotBack.spr", 555)==FALSE) AfxMessageBox("JackPotBack.spr 파일을 읽을 수 없습니다");
	if(JackPotSpr.Load(".\\data\\JackPot.spr", 555)==FALSE) AfxMessageBox("JackPot.spr 파일을 읽을 수 없습니다");
	if(ChipSpr.Load(".\\data\\Chip.spr", 555)==FALSE) AfxMessageBox("Chip.spr 파일을 읽을 수 없습니다");
	// ### [ 파산 ] ###
	if(OringCharSpr.Load(".\\data\\OringChar.spr", 555)==FALSE) AfxMessageBox("OringCharSpr.spr 파일을 읽을 수 없습니다");
	//if(ItemBagSpr.Load(".\\data\\ItemBagBtn.spr", 555)==FALSE) AfxMessageBox("ItemBagBtn.spr 파일을 읽을 수 없습니다");
	// ### [ 중복된 IP가 ] ###
	if(WarningSpr.Load(".\\data\\Warning.spr", 555)==FALSE) AfxMessageBox("Warning.spr 파일을 읽을 수 없습니다");
	// [화면캡쳐]
	//if(CaptureSpr.Load(".\\data\\CaptureBtn.spr", 555)==FALSE) AfxMessageBox("CaptureBtn.spr 파일을 읽을 수 없습니다");
	// ### [ 관전기능 ] ###
	if(ObserverSpr.Load(".\\data\\ObserverBtn.spr", 555)==FALSE) AfxMessageBox("ObserverBtn.spr 파일을 읽을 수 없습니다");

	//////////////////////////////////////////////////
	// ### [ 대기실 리스트 관련 ] ###

	if(LobyListBackSpr.Load(".\\data\\LobyListBack.spr", 555)==FALSE) AfxMessageBox("LobyListBack.spr 파일을 읽을 수 없습니다");
	if(LobyListButtonSpr.Load(".\\data\\LobyListBackBtn.spr", 555)==FALSE) AfxMessageBox("LobyListBackBtn.spr 파일을 읽을 수 없습니다");
	if(LobyRoomImgSpr.Load(".\\data\\LobyRoomImg.spr", 555)==FALSE) AfxMessageBox("LobyRoomImg.spr 파일을 읽을 수 없습니다");

	//////////////////////////////////////////////////

	//애니파일 (올인)
	
	if(AllinAniSpr.Load(".\\data\\AllinAni.spr", 555)==FALSE) AfxMessageBox("AllinAni.spr 파일을 읽을 수 없습니다");
	
}

void C62CutPokerDlg::Reload_spritefile()
{
	CString str;		
	if(g_BgMode >0)str.Format(".\\data\\skin%d\\",g_BgMode);
	else str = ".\\data\\";
	
	CString temp,temp2;	
	temp2= "파일을 읽을수 없습니다.";	
	

	temp = str+"GameBack.spr";	
	if(GameBackSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"ChatBox.spr";
	if(ChatBoxSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"BadUserBtn.spr";
	if(BadUserBtnSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"FoldBtn.spr";
	if(DieBtnSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"Allinbtn.spr";
	if(AllinBtnSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"CheckBtn.spr";
	if(CheckBtnSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"CallBtn.spr";
	if(CallBtnSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"PingBtn.spr";
	if(PingBtnSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"DadangkBtn.spr";
	if(DadangkBtnSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"MaxBtn.spr";
	if(MaxBtnSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"changeokBtn.spr";
	if(ChangeOKBtnSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"changecancelBtn.spr";
	if(ChangeCancelBtnSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"roommaster.spr";
	if(RoomMasterSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"banishvote.spr";
	if(BanishVoteSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	
	temp = str+"ObserverBtn.spr";
	if(ObserverSpr.Load((char*)temp.operator LPCTSTR(), 555)==FALSE) AfxMessageBox(temp + temp2);	

	g_pGameView->BatPage.PutSprAuto(0, 0, g_pGameView->m_pgamebackspr, 1);			// 배팅 칩 게임판을 지운다
	Game.ChipFunc(NULL);//칩 그리기

}

void C62CutPokerDlg::load_spritefile_7()
{
	// 7인용 이미지
	if(GameBackSpr_7.Load(".\\data\\new_7\\GameBack_7.spr", 555)==FALSE) AfxMessageBox("GameBack_7.spr 파일을 읽을 수 없습니다");
}

void C62CutPokerDlg::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return;
//	CDialog::WinHelp(dwData, nCmd);
}


LONG C62CutPokerDlg::OnAllinMessage(UINT wParam, LONG lParam)
{
	switch(wParam)
	{
	case ACD_GO_CHARGE: // 충전 페이지 이동
		{
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			g_pMainDlg->bNowLogout = FALSE; // 서버에 알리고 죽는다.
			g_Where = WH_TITLE;
			Sound.Play(SND45);
			g_pMainDlg->BrowserMove( g_BannerURL2 );
			
			PostQuitMessage( 0 );
			SockMan.Destroy();
			/*
			g_Wait = FALSE;
			if(g_Where == WH_LOBY)
			g_Where = WH_TITLE;
			bNowLogout = TRUE;
			OnCancel();
			*/
		} break;
		
	case ACD_GO_LEADERS: // 리더스 페이지 이동
		{
			
			CString str ="http://www.goodboy7poker.com/Premium/Club.asp?pgamode=netpoker";
			//		if(m_LJ.PremLType==0)
			//			g_pMainDlg->BrowserMove( str + m_LJ.LeadersJoinURL ); // 가입
			//		else
			//			g_pMainDlg->BrowserMove( str + m_LJ.LeadersJoinURL2 );// 변경
			
			BrowserMove(str);
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			g_Where = WH_LOBY;
			PostQuitMessage( 0 );
			SockMan.Destroy();
			
		} break;

	case ACD_INVINCIBLE_BUY: // 천하무적 아이템 구매
		{
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			m_GameItemDlg.DoModaless(2,FALSE,ITEM_SCODE_INVINCIBLE);
		} break;

	case ACD_INVINCIBLE_DETAILS : // 천하무적 아이템 자세히 보기 
		{
			g_Where = WH_LOBY;
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			CString url;
			url = _T("http://www.goodboy7poker.com/ItemShop/?targetUrl=game/gameitem_info.asp?item_no=385");
			BrowserMove(url);
			PostQuitMessage( 0 );
			SockMan.Destroy();			
		} break;

	case ACD_CLOSEWINDOW:	// 수호천사 창닫기 
		{
		//	::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			g_Where = WH_LOBY;
			PostQuitMessage( 0 );
			SockMan.Destroy();
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
		} break;

	case ACD_ALLIN_INSURANCE_BUY:	//올인 보험 사기 
		{
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			if(g_Where==WH_LOBY)
			{
				m_GameItemDlg.DoModaless(2,FALSE,ITEM_SCODE_SAFEGUARDANGEL);
			}
			if(g_Where==WH_GAME)
			{
				m_GameItemDlg.DoModal(2,FALSE,ITEM_SCODE_SAFEGUARDANGEL);
			}
		} break;

	case ACD_ALLIN_INSURANCE_USE:	//올인 보험 사용하기 
		{
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			if(g_Where==WH_LOBY)
			{
				m_GameItemDlg.DoModaless(1,TRUE,ITEM_SCODE_SAFEGUARDANGEL);
			}
			if(g_Where==WH_GAME)
			{
				m_GameItemDlg.DoModal(1,TRUE,ITEM_SCODE_SAFEGUARDANGEL);
			}
		} break;

		
	case ACD_ALLIN_INSURANCE: // 올인 보험 자세히 보기
		{
			// [수호천사] 2004.07.09

			g_Where = WH_LOBY;
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			CString url;
			url = _T("http://www.goodboy7poker.com/ItemShop/?targetUrl=game/gameitem_info.asp?item_no=970");
			BrowserMove(url);
			PostQuitMessage( 0 );
			SockMan.Destroy();

			/*
			BOOL bExist = FALSE;
			GameItem_IsUsingItem(g_MyGameItem, ITEM_SCODE_SAFEGUARDANGEL, &bExist);
			
			if( bExist ) 
			{
				if(g_Where==WH_LOBY) {
					g_pMainDlg->m_GameItemDlg.m_ItemListDlg.SetItemList( g_MyGameItem.nNum, g_MyGameItem.Item );
					g_pMainDlg->m_GameItemDlg.DoModaless(1, FALSE, ITEM_SCODE_SAFEGUARDANGEL);
					if( g_pMainDlg->m_GameItemDlg.m_ItemListDlg.SelectUsableGameItem( ITEM_SCODE_SAFEGUARDANGEL ) ) {
						g_pMainDlg->m_GameItemDlg.m_ItemListDlg.UseSelectedGameItem();
					}
					//m_GameItemDlg.DoModaless(1,FALSE);
				}
				if(g_Where==WH_GAME) {
					g_pMainDlg->m_GameItemDlg.m_ItemListDlg.SetItemList( g_MyGameItem.nNum, g_MyGameItem.Item );
					g_pMainDlg->m_GameItemDlg.DoModal(1, FALSE, ITEM_SCODE_SAFEGUARDANGEL);
					if( g_pMainDlg->m_GameItemDlg.m_ItemListDlg.SelectUsableGameItem( ITEM_SCODE_SAFEGUARDANGEL ) ) {
						g_pMainDlg->m_GameItemDlg.m_ItemListDlg.UseSelectedGameItem();
					}
					//m_GameItemDlg.DoModal(1,FALSE);
				}
			} else {
				if(g_Where==WH_LOBY) {
					m_GameItemDlg.DoModaless(2,FALSE);
				}
				if(g_Where==WH_GAME) {
					m_GameItemDlg.DoModal(2,FALSE);
				}
			}
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			*/
		} break;
	}
	return TRUE;
}



















