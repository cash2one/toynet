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
#include "ItemListDlg.h"	// [���� ������ �۾�]
#include "AskBuyItemDlg.h"	// [���� ������ �۾�]
#include "SelCardDlg.h"
#include <atlbase.h>
#include "ShortCut.h"
//#include "Oring.h"			// [ ������� ]
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


// [SSO �۾�] - ���� �Լ� ��ü
char* _StrTok(const char* str, const char sep)
{
	// strtok()�� ���� ����� �Լ������� separator�� 1���� �ް�
	// ���ڰ� ����ִ� ��쵵 ���ڷ� ������

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
	ON_MESSAGE(WM_ALLINMESSAGE,OnAllinMessage) // [��ȣõ��] 2004.07.08
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

	// ���̾�α� ��Ʈ�� �����ǰ� ��ġ ������
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);


	CString str;
	float gamever = (float)VERSION/100; 
	str.Format("Good Boy 7 Poker Ver%2.2f", gamever);
	SetWindowText(str);

	// �ƹ�Ÿ ���÷��� ��� �ʱ�ȭ
	//InitAvatarModule();
	//InitAvatarModule_ForTest("E:\\work\\board\\poker\\_BIN\\Client", "E:\\work\\board\\poker\\_BIN\\Client\\AvatarDisplay.dll");
	InitAvatarModule_ForTest(".\\Avatar", ".\\Avatar\\AvatarDisplay.dll");



	// ���� �޼��� ǥ��
	m_ConnectMsgDlg.MsgBox(0, "���μ����� �������Դϴ�");
	
	// ��������Ʈ ���ϵ��� �о����
	g_BgMode = 0;	// ����Ʈ ��� �ε� 
	LoadSpriteFile();// ### [ ������� ] ###
	load_spritefile_7();
	
	// ���� ������� ���� �����츦 ���� ������ ��ġ�� �ű��
	CRect rect;
	//GetClientRect(rect);
	rect.SetRect(0,0,800, 600);
	
	if(m_TitleDlg.Create(IDD_TITLE_DIALOG, this) == FALSE) AfxMessageBox("Create failed on create Title Dialog");
	if(m_LobyDlg.Create(IDD_LOBY_DIALOG, this) == FALSE) AfxMessageBox("Create failed on create Loby Dialog");
	if(m_GameDlg.Create(IDD_GAME_DIALOG, this) == FALSE) AfxMessageBox("Create failed on create Game Dialog");
	
	// [������ ��� �۾�] ������ �޽��� ������
	if(AMesDlg.GetSafeHwnd()==NULL){
		if(AMesDlg.Create(IDD_ADMIN_MESSAGE,this) == FALSE) AfxMessageBox("Create failed on create Admin Message Dialog");
		else AMesDlg.ShowWindow(SW_HIDE);
	}

	m_TitleDlg.MoveWindow(rect, FALSE);
	m_LobyDlg.MoveWindow(rect, FALSE);
	m_GameDlg.MoveWindow(rect, FALSE);
	m_TitleDlg.ShowWindow(SW_SHOW);	

	// Ÿ��Ʋ �� ������ ������ ���̱�
	this->ModifyStyle(0, WS_SYSMENU,0 );//| WS_MINIMIZE | WS_MAXIMIZE,0);

	// �������� ����� ���Ѵ�
	GetWindowRect(g_OrgMainDlgRect);
#ifndef _DEBUG
	// ������ ����̸� ������ ����� 0���� ����� �����
	MoveWindow(0,0,0,0);
#endif

	//////////////////////
	// ��� �޼��� ó�� Ŭ���� �ʱ�ȭ
	int rtn = m_SockCmnd.Init(this);
	if(rtn==FALSE)
	{
		AfxMessageBox("���� �ʱ�ȭ�� �����Ͽ����ϴ�");
		// ���α׷� ����
		m_ConnectMsgDlg.OnCancel();
	}
	Sound.Init(m_hWnd);// ���̷�Ʈ ���� �ʱ�ȭ
	srand((unsigned)timeGetTime());  // ���� �ʱ�ȭ

	// ## �ٷΰ��� ������ ##
//	CreateShortCut();

	// Ȩ������ �ڵ� ����
	//char strDir[500]={0,};
	//GetCurrentDirectory(500, strDir);
	//CString runstr;
	//runstr.Format("%s\\Homepage.url", strDir);
	//::ShellExecute(NULL, "open", runstr.operator LPCTSTR(), NULL, ".", SW_SHOWNORMAL);

//###���׸���ƶ�
#ifdef _DEBUG


	CLoginDlg LoginDlg;
	LoginDlg.m_ID = "bbbb";					// �÷��̾� �α��� - jeong
	if(LoginDlg.DoModal()==IDOK) {
		g_LoginID = LoginDlg.m_ID;
		g_LoginPass = LoginDlg.m_Pass;
		g_ServIP = LoginDlg.m_ServerIP;
		g_ServPort = LoginDlg.m_ServerPort;
	}
	else {
		// ���α׷� ����
		m_ConnectMsgDlg.OnCancel();
		return FALSE;
	}
	

#endif

	// �弳 ���͸� ������ �б�
	//char strbuf[1024] = {0,};
	char strbuf[1024] = "Avatar";
	//GetBaseAvatarDir(strbuf, 1023);
	CString filterfile = strbuf;
	filterfile += "\\filter.bcd";
	if(!BadChat.Open(filterfile)) {
		AfxMessageBox("���α׷� ���࿡ �ʿ��� ������ ���� �� �����ϴ�.\n���α׷��� �缳ġ�Ͻñ� �ٶ��ϴ�.");
		// ���α׷� ����
		m_ConnectMsgDlg.OnCancel();
		return FALSE;
	}	

	// ���μ����� ���� �õ�
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
			//g_pGameDlg->SendMessage(WM_COMMAND, IDC_BUTTON_EXITGAME, 0); // ���̾� �α׹�ư ����
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
		m_LogoutMsgDlg.MsgBox(2, "���α׷��� �����ϴ� ���Դϴ�.");
		
		g_Wait = TRUE;
		
		// ### [ ������� ] ###
		CSV_ASK_LOGOUT alomsg;
		//alomsg.Set(Play[0].UI.UNum, Play[0].UI.ID);
		alomsg.Set(g_MyInfo.UI.UNum, g_MyInfo.UI.ID);
		SockMan.SendData(g_MainSrvSID, alomsg.pData, alomsg.GetTotalSize());
	}

	KillProcess();

	// �α׾ƿ��� ���Ǿ��� ���� ���α׷��� �����Ѵ�
	if(bNowLogout == FALSE) return;

	CDialog::OnCancel();
}
void C62CutPokerDlg::KillProcess()
{
	::ShellExecute(NULL, "open", "end.bat", NULL, ".", SW_HIDE);
}
// [SSO �۾�] - ���� �Լ� ��ü
BOOL C62CutPokerDlg::AnalyzeArgument(char *key)
{
//
	// Ŭ������ ���� ���� ��Ģ(������ ���ڴ� �ĸ�(,)�� ���еǸ� ���� ������ ������ ����)
	// ���� ������ ���� : ServerIP,Port,AuthCookie,DataCookie,CpCookie,SpareParam (���ڴ� ��ȣȭ�Ǿ� �ִ�)
	// - �ݸ��� ���ӿ����� CpCookie�� �������� �ʴ´�
	// - CP ���ӿ����� AuthCookie, DataCookie�� �������� �ʴ´�(��� CP������ ����)
	// - Extra ���ڴ� �����Ǳ⵵ �Ѵ�
	//

	char buf[1024*20] = {0,};	// ���۴� ����� ũ�� ��´�

	// Ŭ�����忡�� ���� �����͸� �о�´�(�ѹ� ������ ������)
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

	// ���ڰ� �ּ��� 5���� �Ǿ�� �Ѵ�
	if(argcnt < 5)
		return FALSE;

	// IP�ּҰ� �ùٸ��� �˻�
	if(strlen(arg[0]) > 16)
		return FALSE;

	// CP ������ �Ʒ��κ��� ���� ����
	g_ServIP     = arg[0];					// ���� IP
	g_ServPort   = (UINT)::atoi(arg[1]);	// ���� Port
	g_AuthCookie = arg[2];					// ���� ��Ű	- ���� ũ�⸦1024*2 ������ ��ƾ� �Ѵ�
	g_DataCookie = arg[3];					// ������ ��Ű	- ���� ũ�⸦1024*2 ������ ��ƾ� �Ѵ�
	g_CpCookie   = arg[4];					// CP��Ű		- ���� ũ�⸦1024*2 ������ ��ƾ� �Ѵ�
	g_SpareParam = arg[5];					// ������ ����(CP���ӵ�� ���)
	
	return TRUE;
}


#include "SysUtil.h"
BOOL C62CutPokerDlg::LoadConfig(LPCTSTR id)
{
	if(id == NULL || strlen(id) == 0) 
		return FALSE;
	
	memset(&CFG, 0, sizeof(CFG));
	
#ifndef _DEBUG
	// ������ ����� ��� �⺻�� Ǯ��ũ�� ���� �����Ѵ�
	CFG.nScreenMode       = GetProfile_int(id, "ScreenMode",       0);
#else
	// ����� ����� ��� �⺻�� â���� �����Ѵ�
	CFG.nScreenMode       = GetProfile_int(id, "ScreenMode",       0);
#endif
	CFG.bOffSndFX         = GetProfile_int(id, "OffSndFX",         0);
	CFG.bOffMusic         = GetProfile_int(id, "OffMusic",         0);
	CFG.bNoInvite         = GetProfile_int(id, "NoInvit",          0);
	CFG.bNoWhisper        = GetProfile_int(id, "NoWhisper",        0);
	CFG.bFailToFullScreen = GetProfile_int(id, "FailToFullScreen", 0);
	CFG.bHideOtherBlock   = GetProfile_int(id, "HideOtherBlock",   0);
	// ### [ ������� ] ###
	if(g_MyInfo.UI.Sex == 1)
		CFG.SndFxKind	  = GetProfile_int(id, "SndFxKind",   0);
	else
		CFG.SndFxKind	  = GetProfile_int(id, "SndFxKind",   3);


	///��ũ�� 
	memset(&g_ChatMacro, 0, sizeof(g_ChatMacro));	
	
	char fname[MAX_PATH];
	//��ũ�� �ε�	
	wsprintf(fname, ".\\config\\%s.mcr",id);	

	// ȯ�漳�� ������ ����
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

	
	//��ũ�� �ε�
	char fname[MAX_PATH];	
	wsprintf(fname, ".\\config\\%s.mcr",id);	

	// ȯ�漳�� ������ ����
	FILE *fp = fopen( fname, "wb");

	if( fp == NULL ) return FALSE;
	fwrite(&g_ChatMacro, sizeof(g_ChatMacro), 1, fp);
	fclose(fp);

	return TRUE;
}


void C62CutPokerDlg::SetupByConfig()
{
	// ���� ���� ����
	if(Sound.bDontUseSndFx != CFG.bOffSndFX) Sound.SetSndFx(CFG.bOffSndFX ^ 1);
	if(Sound.bDontUseMusic != CFG.bOffMusic) {
		Sound.SetMusic(CFG.bOffMusic ^ 1);
	}

	if(CFG.bFailToFullScreen) {
		// Ǯ��ũ���� �����ϴ��� ���θ� üũ�ϱ� ����
		CFG.nScreenMode = 0;
		CFG.bFailToFullScreen = FALSE;
		// ### [ ������� ] ###
		SaveConfig(g_MyInfo.UI.ID);
		MessageBox("������ ����� Ǯ��ũ�� ���� ȭ�� ��ȯ�� �����Ͽ��⶧���� â���� �����մϴ�.", "�˸�", MB_OK);
	} else {
		// ȭ�� ��� ���� ����
		if(CFG.nScreenMode==1) {
			// Ǯ��ũ���� �����ϴ��� ���θ� üũ�ϱ� ����
			CFG.bFailToFullScreen = TRUE;
			// ### [ ������� ] ###
			SaveConfig(g_MyInfo.UI.ID);

			g_pLobyDlg->bDontClickSound = TRUE;
			g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_CHANGEDISPLAY);
			g_Mini.SendMessage(WM_COMMAND, IDC_BUTTON_CHANGEDISPLAY);

			// Ǯ��ũ���� �����Ͽ��⶧���� ���������� �ʱ�ȭ��
			CFG.bFailToFullScreen = FALSE;
			// ### [ ������� ] ###
			SaveConfig(g_MyInfo.UI.ID);
		}
	}

	// �ΰ� ��� ���� ����
	if(CFG.bNoInvite) {
		CButton* pCheck = (CButton*)g_pLobyDlg->GetDlgItem(IDC_CHECK_REFUSEINVITE);
		pCheck->SetCheck(TRUE);
	}
	if(CFG.bNoWhisper) {
		CButton* pCheck = (CButton*)g_pLobyDlg->GetDlgItem(IDC_CHECK_REFUSEWHISPER);
		pCheck->SetCheck(TRUE);
		g_pLobyDlg->PostMessage(WM_COMMAND, IDC_CHECK_REFUSEWHISPER);
	}


	// ### [���� �߰� �۾�] ###
	// ### [ ������� ] ###
	if(g_MyInfo.UI.Sex == 1) // ����
	{
		if(CFG.SndFxKind >= 3 ) CFG.SndFxKind = 0; // 0 ~ 2
	}
	else 
	{
		if(CFG.SndFxKind < 3 ) CFG.SndFxKind = 3; // 3 ~ 5
	}

	// ### [ ������� ] ###
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
	
	// ����
	//g_pTitleDlg->DestoryWebCtrl();
	g_pTitleDlg->DestroyBanner();	// <- �߰�


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
			// ���� ä��â �� ä�� ����Ʈâ �ʱ�ȭ
			m_LobyDlg.m_ChatViewEdit.SetWindowText("");
			m_LobyDlg.m_SendEdit.SetWindowText("");

			// ���� Ÿ��Ʋ ���� - jeong
			float gamever = (float)VERSION/100; 
			//str.Format("%s Ver%2.2f - [%s]", g_StrMan.Get(_T("GAME_TITLE")), gamever, g_ChannelName);
			str.Format("%s Ver%2.2f - [%s]", "Good Boy 7 Poker", gamever, g_ChannelName);
			g_pLobyDlg->m_LobyMsg=str;
			

			SetWindowText(str);

			if(bFirstLoby == TRUE) {
				// ### [ ������� ] ###
				FirstMarble = g_MyInfo.UI.PMoney;
				bFirstLoby = FALSE;
			}
			else
			{
				// ### [ ������� ] ###
				INT64 TotEarn = g_MyInfo.UI.PMoney - FirstMarble;
				INT64 PrevEarn = g_MyInfo.UI.PMoney - PrevMarble;
		
				if(TotEarn >= 0)
				{
					//str.Format("�������� �� +%I64d ���� ȹ���ϼ̽��ϴ�\n", TotEarn);
					str.Format(g_StrMan.Get(_T("STATE_MES1")), g_MakeCommaMoney(TotEarn));
					g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(0,180,0));
				}
				else
				{
					//str.Format("�������� �� %I64d ���� �����̽��ϴ�\n", TotEarn);
					str.Format(g_StrMan.Get(_T("STATE_MES2")), g_MakeCommaMoney(TotEarn));
					g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(180,0,0));
				}

				if(PrevEarn >= 0)
				{
					//str.Format("�ڹ�� ��̴� �濡�� +%I64d ���� ȹ���ϼ̽��ϴ�\n", PrevEarn);
					str.Format(g_StrMan.Get(_T("STATE_MES3")), g_MakeCommaMoney(PrevEarn));
					g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(0,180,0));
				}
				else
				{
					//str.Format("�ڹ�� ��̴� �濡�� %I64d ���� �����̽��ϴ�\n", PrevEarn);
					str.Format(g_StrMan.Get(_T("STATE_MES4")), g_MakeCommaMoney(PrevEarn));
					g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(180,0,0));
				}

				ShowItemState();	// [���� ������ �۾�]
			}

			//### [������ ��� �۾�] ###
			if(g_MyInfo.UI.cAdminUser == 1) {
				str.Format("### [%s]���� �����ڸ��� �����ϼ̽��ϴ�. ### \n", g_MyInfo.UI.ID);
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(255,0,0));
			}

			m_LobyDlg.m_SendEdit.SetFocus();
			// ���� ����
//			Sound.AllMusicStop();
		} break;
	case 3: 
		// ���� Ÿ��Ʋ �ؽ�Ʈ ����
		// 0 ~ 4 ���� 0 ���� , 1 �ʺ�, 2 �߼�, 3 ���, 4 ����
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
		str.Format("[%s]-[%d����][%s][%s][%s]", g_ChannelName, g_RI.RoomNum, g_RI.Title, g_RI.Secret==0?g_StrMan.Get(_T("L_OPEN")):g_StrMan.Get(_T("L_PASS")), g_RI.FormKind==0?"Normal":"Choice");
		g_pGameView->m_titlestr="";//str;


	//	CString str2="";
	//	str2.Format("[%s][%s][%s]", style,  g_RI.Secret==0?g_StrMan.Get(_T("L_OPEN")):g_StrMan.Get(_T("L_PASS")), g_RI.FormKind==0?"Normal":"Choice");
	//	g_pGameView->ChatBox.m_MsgTitle=str2;
		/////////////////////////
		// ### [ ������� ] ###
		PrevMarble = g_MyInfo.UI.PMoney;

		g_pGameView->ChatBox.ChatViewEdit.SetWindowText(""); // ä��â
		g_pGameView->ChatBox.SendEdit.SetWindowText("");
		// ��Ŀ��
		g_pGameView->SetFocus();
		m_GameDlg.m_GameView.Reset();

		if(g_RI.bPlayer==0)
			str.Format("�ٵ��򸻡�\n1. ����â->ä��â ��Ŀ�� �̵� ����Ű - Enter, ESC\n2. ä��â->����â ��Ŀ�� �̵� ����Ű - ESC\n3. ä��â<->���� �̵� ����� - Tab\n");
		else 
			str.Format("�ٵ��򸻡�\n1. ����â->ä��â ��Ŀ�� �̵� ����Ű - Enter, ESC\n2. ä��â->����â ��Ŀ�� �̵� ����Ű - ESC\n");
		
		g_pGameView->ChatBox.AddText(&str, NOTIC_COLOR);//RGB(255,100,200));
		
		// ������� ����
		Game.SetChangeViewTabBtn(); //�⺻ ä��â
		break;
	}
}

LONG C62CutPokerDlg::OnPacketNotify(UINT wParam, LONG lParam)
{
	GAMENOTIFY *pNotifyData = (GAMENOTIFY*)wParam;
	if(!pNotifyData) return 0;

	int Sid = pNotifyData->Sid;				// ���� ��ȣ
	int Signal = pNotifyData->Signal;		// �޽��� �ñ׳�
	int TotSize = pNotifyData->TotSize;		// �޽��� ������
	char *lpData = pNotifyData->lpData;		// �޽��� ������ ������

	switch(Signal)
	{
		// [��ȣõ��] 2004.07.08 �ٸ����� �ӴϺ��� �˸�
		// [��ȣõ��] 2004.07.08 �ӴϺ��� �˸�
	case CL_CHARGE_SAFEANGEL:
		{
			CCL_CHARGE_SAFEANGEL msg;
			msg.Get( lpData, TotSize );

			Game.Charge_Safeangel_money( &msg );
			
		} break;

	case SV_SERVERNOTIC: // Ÿ��Ʋ ����
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
			// ### [ ������� ] ###
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
		
		// ### [ ������� ] ###
	case SV_RESERVATIONINFO:// ������ ����
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
		// ### [ ������� ] ###
		// ������ -> ����
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

			// �÷��̾� ������ �°� �ʱ�ȭ�Ѵ�			
			Game.SetPlayInfo(nowpnum, newpnum, FALSE);
			
			// ### [ �ߺ��� IP�� ] ###
			for( i=0; i<g_Max_Player; i++) {
				int pn = Game.GetPNum_ByServPN(i);
				if(strlen(Play[pn].UI.ID) > 0)	{
					Play[pn].MyIpCheck = msg.IPA->IpIndex[i];
				}
			}
		
		} break;
		// ### [ ������� ] ###
		// ���� -> ������
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

			// ### [���� �߰� �۾�] ###
			Play[MyID].nSndFxKind = *msg.SndFxKind;
			//g_MyInfo.nSndFxKind = *msg.SndFxKind;
			
			// �÷��̾� ������ �°� �ʱ�ȭ�Ѵ�			
			Game.SetPlayInfo(nowpnum, newpnum, TRUE);
			
			// ### [ �ߺ��� IP�� ] ###
			for( i=0; i<g_Max_Player; i++) {
				int pn = Game.GetPNum_ByServPN(i);
				if(strlen(Play[pn].UI.ID) > 0) {
					Play[pn].MyIpCheck = msg.IPA->IpIndex[i];
				}
			}
			
		} break;
	
		// ### [ ������� ] ###
	case SV_ENTER_EMERAGE:// ������ -> ����
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
			str.Format("## <%s>���� ������ ���� �մϴ� ##\n", msg.ID);
			g_pGameDlg->m_GameView.ChatBox.AddText(&str, INOUT_COLOR);

			// ### [ �ߺ��� IP�� ] ###
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
		
		// ### [ ������� ] ###
	case SV_ENTER_PARTICIPATION:// ���� -> ������
		{
			CSV_ENTER_PARTICIPATION msg;
			msg.Get(lpData, TotSize);

			if(g_Where != WH_GAME) break;
			int nowpnum = *msg.NowPNum;
			int newpnum = *msg.NewPNum;
			if(newpnum < 0 || newpnum >= g_Max_Player) break;
			if(nowpnum < g_Max_Player || nowpnum >= g_Max_NewPlayer) break;

			//������ pnum �ε����� ������ ��ġ�Ѵ�
			if(strcmp( Play[nowpnum].UI.ID, msg.ID) != 0) break; 
			int i=0;
			int pnum = -1; // �÷��̾� ��ȣ
			if( !g_MyObserver) // 1) ���� ���������̰� ������ �� ����
			{
				for(i=Game.No1PosPNum; i<g_Max_Player+Game.No1PosPNum; i++) {	
					int index = i;
					if(index>=g_Max_Player) index = index - g_Max_Player;
					if(index==0) continue; //��
					if(strlen(Play[index].UI.ID)==0){pnum=index; break;}
				}
			
				if(pnum == -1) break; // ����ִ� ���� ã�� ��������
				
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
				Play[pnum].PrevMoney = Play[pnum].UI.PMoney;// +- ������ ǥ��
				CString str;
				str.Format("<%s>���� ���ӿ� ���� �մϴ�.\n", msg.ID);
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, INOUT_COLOR);
			}
			// 3)���� �������̰�  ������ �ϴ³� ����
			else
			{
				for(i=0; i<g_Max_Player; i++)	{
					if(strlen(Play[i].UI.ID)==0){pnum=i; break;}
				}
				if(pnum == -1) break; // ����ִ� ���� ã�� ��������
				
				Game.MyPosPNum = 0; // �����ʿ� �ε���
				Game.No1PosPNum = Game.GetPNum_ByServPN(0); // �������� 0 �� �ε���
				
				// �� ������ �����ϰ� ���� ������ �÷��̾��� ������ ����
				Play[nowpnum].UI.cObserver = 0;
				Play[pnum].SetNewPlayer(&Play[nowpnum].UI);
				PlayScr[pnum].Reset();

				Play[nowpnum].Clear();
				g_RI.NowUserNum += 1;
				g_RI.NowObserverNum -= 1;
								
				Play[pnum].ServPNum = Game.GetServPNum_ByPN(pnum);// ������ �÷��̾� ��ȣ ����
				Play[pnum].PrevMoney = Play[pnum].UI.PMoney;// +- ������ ǥ��
				
				CString str;
				str.Format("## ## <%s>�� �������� ##\n", msg.ID);
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, INOUT_COLOR);
				
			}
			
			// ### [ �ߺ��� IP�� ] ###
			for( i=0; i<g_Max_Player; i++) {
				int pn = Game.GetPNum_ByServPN(i);
				if(strlen(Play[pn].UI.ID) > 0)	{
					Play[pn].MyIpCheck = msg.IPA->IpIndex[i];
				}
			}

			// ### [���� �߰� �۾�] ###
			Play[pnum].nSndFxKind = *msg.SndFxKind;
			g_pGameDlg->m_ObserverDlg.User_DelItem(Play[pnum].UI.UNum, Play[pnum].UI.ID);
	
		} break;


	case SV_CHECKVERSION: // ���� üũ
		{
			CSV_CHECKVERSION msg;
			msg.Get(lpData, TotSize);
			if(g_MainSrvSID != Sid) break;
			int prev = *msg.PreVersion;
			int ver = *msg.Ver;
			if(ver<0) break;

			// ������ Ʋ���� ��������
			if(ver != VERSION || prev != VERSION+5)// VERSIOM �� ���� �����ϴٸ� �ٸ� ���
			{
				PostMessage(UM_DATA_CLOSE, NULL, (long)SockMan.pDataSock[g_MainSrvSID]); 
				g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"���α׷��� ������ ��ġ���� �ʽ��ϴ�. �ֽ� ������ �ٿ��������");
				m_ConnectMsgDlg.OnCancel();
				break;
			}
			
			// �α��� ���̸�	[SSO �۾�]
			if(g_Where == WH_LOGIN)
			{
				ASK_LOGININFO al;
				memset(&al, 0, sizeof(ASK_LOGININFO));

			#ifdef _DEBUG
				strcpy(al.ID, g_LoginID);
				strcpy(al.Pass, g_LoginPass);
			#endif

				strcpy(al.SockNameIP, SockMan.pDataSock[g_MainSrvSID]->SockAddr);	// ����� ������ ��巹��
				strcpy(al.HostNameIP, SockMan.GetServerIP());						// ȣ��Ʈ �ý��� ��巹��
				al.Port = SockMan.pDataSock[g_MainSrvSID]->SockPort;				// ����� ������ ��Ʈ��ȣ
				// ������ ������ �����ϱ� ���� 20000���� ������ ��Ʈ��ȣ�� ����Ѵ�
				// ���� ����ϰԵ� ��Ʈ��ȣ�� ���μ����� ���Ͽ� �����ȴ�
				al.Port += 20000; 

				al.AuthCookieLen = g_AuthCookie.GetLength();
				al.DataCookieLen = g_DataCookie.GetLength();

				

				// �α��� �޼����� ������
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
			// �α��� ���̸�
			if(g_Where == WH_LOGIN)
			{
				char *id = (char*)g_LoginID.operator LPCTSTR();
				char *pass = (char*)g_LoginPass.operator LPCTSTR();

				char *socknameip = (char*)SockMan.pDataSock[g_MainSrvSID]->SockAddr.operator LPCTSTR();	// ����� ������ ��巹��
				char *hostnameip = (char*)SockMan.GetServerIP();	// ȣ��Ʈ �ý��� ��巹��
				UINT port = SockMan.pDataSock[g_MainSrvSID]->SockPort;							// ����� ������ ��Ʈ��ȣ
				
				// ������ ������ �����ϱ� ���� 20000���� ������ ��Ʈ��ȣ�� ����Ѵ�
				// ���� ����ϰԵ� ��Ʈ��ȣ�� ���μ����� ���Ͽ� �����ȴ�
				port += 20000; 

				// �α��� �޼����� ������
				CSV_LOGIN lgimsg;
				lgimsg.Set(id, pass, FALSE, socknameip, hostnameip, port);
				SockMan.SendData(g_MainSrvSID, lgimsg.pData, lgimsg.GetTotalSize());
			}
*/
		} break;

	case SV_SERVERINFO: // ���� ����	[���� ������ �۾�]
		{
			CSV_SERVERINFO msg;
			msg.Get(lpData, TotSize);

			// ������ ���� ���� ����
			g_GameCode = msg.SI->GameCode;
			g_ServerName = msg.SI->ServerName;
			// URL ����
			g_NoticeURL = msg.SI->NoticeURL;		// ���� ���� URL
			g_BannerURL1 = msg.SI->BannerURL1;		// �κ� ��� URL
		
			// [ ���� ��Ŀ ] ����
			// [ ###����### ] 
			g_JackPotMoney = msg.SI->JackPot;		// ���� ������ ��
			g_TmpJackPotMoney = g_JackPotMoney;		//
		
			//######################################
			CString str ="http://www.goodboy7poker.com";
			g_BannerURL2 = str;
			g_BannerURL2 += msg.SI->BannerURL2;
			//######################################
			g_ItemURL = msg.SI->ItemURL;			// ������ ������ URL			

			g_ItemHelpURL = msg.SI->ItemHelpURL;	// ������ ���� URL

				
			g_strSrvInfo = msg.SI->SrvInfoMsg;  // �κ�â ���� �޼���
			g_LocationURL = msg.SI->LocationURL;	// ��ġ�˸�(ģ��ã��)�� URL
			g_CashURL = msg.SI->CashURL;			// ĳ������ URL


		} break;

	case SV_ACCEPT_LOGIN: // �α����� ��� �Ǿ���
		{
			CSV_ACCEPT_LOGIN msg;
			msg.Get(lpData, TotSize);

			// ���� �޼���â ����
			m_ConnectMsgDlg.OnOK();

			// ����� ���� ����
			//if ( !msg.LI->UI.bAIPlayer )
				Play[0].SetNewPlayer(&msg.LI->UI);
			/*
			else
			{
				Play[1].SetNewPlayer(&msg.LI->UI);
				break;
			}
			*/

			// ### [ ������� ] ###
			g_MyInfo.SetNewPlayer(&msg.LI->UI);
			g_MyIndex = 0;;
			g_MyObserver = FALSE; //�������ΰ�


			// ���� ��ȭ���ڸ� ������ ũ��� ����
			ShowWindow(SW_HIDE);
			MoveWindow(g_OrgMainDlgRect);
			CenterWindow();
			ShowWindow(SW_SHOW);

			// Top���� �̵� (Windows 2000�� ��� �ͽ��÷η� �Ʒ��� ���α׷��� �������� ��찡 ����)
			SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			SetActiveWindow();
			//SetForegroundWindow(); // ��Ƽ���� �� �����Ÿ�

			// �ߺ� ó�� ������
			static BOOL bLoginProccessed=FALSE;
			if(bLoginProccessed==TRUE) break;
			bLoginProccessed=TRUE;

			// ���� ���� �ʱ�ȭ
			g_LoginID = msg.LI->UI.ID;		// �α��� ���̵�
			g_MyUniqNo = msg.LI->UniqNo;	// ����ũ �ѹ� 

			// [���� ������ �۾�]
			// ���� ������ �ֹε�Ϲ�ȣ�� ���� Ÿ���� ���� �������� �ֹε�� ��ȣ�� ���ڷ� �ѱ�
			//CString strval;
			//strval.Format("?regid=%s", msg.LI->RegID);	// ���� ����
			//g_NoticeURL += strval;

			// Ÿ��Ʋ�� �̵�
			g_Where = WH_TITLE;
			ChangeView(0);

			// ��ü ä�� ����� �䱸��
			g_pTitleDlg->bDontClickSound = TRUE;
			g_pTitleDlg->PostMessage(WM_COMMAND, IDC_BUTTON_REFRESH);


			// ### [ ������� ] ###
			if( g_MyInfo.UI.Sex == 1) {
				g_MyInfo.nSndFxKind = 0;//�⺻����
				Play[0].nSndFxKind  = 0;
			}
			else {
				g_MyInfo.nSndFxKind = 3;
				Play[0].nSndFxKind  = 3;
			}

			// ȯ�漳�� ������ ���������� �о��ٸ� ȯ�漳�� ����
			if(LoadConfig(Play[0].UI.ID)) SetupByConfig();

			// ��������, ���� ����
			if(g_NoticeURL.GetLength()>0) g_pTitleDlg->m_pWebCtrl->Navigate(g_NoticeURL);
			if(g_BannerURL1.GetLength()>0) g_pLobyDlg->m_BannerWnd.OpenURL(g_BannerURL1);
			// ��ġ�˸�(ģ��ã��)�� URL ����
			// ### [ ������� ] ###
			if(g_LocationURL.GetLength()>0) {
				CString strurl;
				//strurl.Format("%s?user_id=%s&server=%s&code=%d", g_LocationURL, Play[0].UI.ID, g_ServerName, g_GameCode);
				strurl.Format("%s?user_id=%s&server=%s&code=%d", g_LocationURL, g_MyInfo.UI.ID, g_ServerName, g_GameCode);
				g_pTitleDlg->m_pLocWebCtrl->Navigate(strurl);
			}
			
	//		CString strmsg;//[62] ä�� ����
	//		strmsg.Format("�ݸ��� ����Ŀ�� ��Ÿ �׽�Ʈ ���Դϴ�.\n�ݸ��� ����Ŀ�� �⺻������ ��Ŀ�� ���� ���� �����̸�  6��° ī�带 ���� �� ī�带 �� �� ��ȯ�� �� �־� �� ���� ��̸� ���� �� �ִ� �����Դϴ�. ���ļ��� ��ȯ �� ���ӸӴϴ� �ʱ�ȭ �˴ϴ�.\n�����մϴ�.");
			



	//		m_MsgBoxDlg.MsgBox(1, (char*)strmsg.operator LPCTSTR());

			//���� �߿�
			//CStartDlg *pDlg = new CStartDlg;
			//pDlg->DoModal();
			//delete pDlg;

			//### [������ ��� �۾�] ###
			// �����̾� PC�� IP��� ��ȭ���ڸ� ��� ǥ�� [PC�� �۾�]
			if(g_MyInfo.UI.bPremIP) m_PremServiceDlg.Create(this);
			if(g_MyInfo.UI.cAdminUser == 1)	g_bAdminUser = TRUE;

			bLoginProccessed=FALSE;		


		} break;
	case SV_REFUSE_LOGIN: // �α����� �ź� �Ǿ���
		{
			CSV_REFUSE_LOGIN msg;
			msg.Get(lpData, TotSize);
			int code = *msg.Code;
			//------------  [SSO �۾�]
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
			if(code==0) AfxMessageBox("����ڰ� �ƴմϴ�.\n����ID�� Ȯ���ϼ���");
			if(code==1) AfxMessageBox("��й�ȣ�� Ʋ�Ƚ��ϴ�.\n��й�ȣ�� �ٽ� Ȯ���ϼ���");
			if(code==2) AfxMessageBox("�̹� ������� ���̵��Դϴ�.\n�����ڿ��� �����ϼ���");
			if(code==3) AfxMessageBox("�ִ� �����ο��� �ʰ��Ͽ����ϴ�.\n����� �ٽ� �������ּ���");
			if(code==4) AfxMessageBox("�������� ���� ������Դϴ�");
			if(code==6) AfxMessageBox("�� ���̵�� ���� ��� ���� �����̹Ƿ�\n������ ������ �� �����ϴ�.\n�����ڿ��� �����ϼ���");
			if(code==7) AfxMessageBox("�� ���̵�� ������ ���̵��Դϴ�.\n�����ڿ��� �����ϼ���");
			if(code==5)
			{
				static BOOL bProccessed = FALSE;
				if(bProccessed != FALSE) break;

				int rtn = AfxMessageBox("���� ���ӿ��� ���� ���� ���� ó���� �̷������ �ʾҽ��ϴ�.\n[��]�� ������ ������ ������ �� �ֽ��ϴٸ� ��Ȳ�� ���� \n�ٷ� ���� �ϼ̴� ������ ����� �ݿ����� ���� �� �ֽ��ϴ�.\n20�� �Ŀ� �ٽ� ������ �õ��غ��ð� ������ �� �޽����� ��Ÿ���ٸ�\n[��]�� ������ ������ �����Ͻñ� �ٶ��ϴ�.\n���� ���� �����Ͻðڽ��ϱ�?", MB_YESNO);
				if(rtn == IDNO) { m_ConnectMsgDlg.OnCancel(); break; }
				bProccessed = FALSE;

				char *id = (char*)g_LoginID.operator LPCTSTR();
				char *pass = (char*)g_LoginPass.operator LPCTSTR();
				char *socknameip = (char*)SockMan.pDataSock[g_MainSrvSID]->SockAddr.operator LPCTSTR();	// ����� ������ ��巹��
				char *hostnameip = (char*)SockMan.GetServerIP();		// ȣ��Ʈ �ý��� ��巹��
				UINT port = SockMan.pDataSock[g_MainSrvSID]->SockPort;	// ����� ������ ��Ʈ��ȣ

				// ������ ������ �����ϱ� ���� 20000���� ������ ��Ʈ��ȣ�� ����Ѵ�
				// ���� ����� ��Ʈ��ȣ�� ���μ����� ���ؼ� ����� �� �ִ�
				port += 20000; 
				// �α��� �޼����� ������
				CSV_LOGIN lgimsg;
				lgimsg.Set(id, pass, TRUE, socknameip, hostnameip, port);
				SockMan.SendData(g_MainSrvSID, lgimsg.pData, lgimsg.GetTotalSize());
				break;
			}
			//PostMessage(UM_DATA_CLOSE, NULL, (long)SockMan.pDataSock[g_MainSrvSID]); // ��������
	*/
			// ���α׷� ����
			m_ConnectMsgDlg.OnCancel();
		} break;

	case SV_ACCEPT_LOGOUT: // �α׾ƿ��� ���Ǿ���
		{
			CSV_ACCEPT_LOGOUT msg;
			msg.Get(lpData, TotSize);
			g_Wait = FALSE;
			bNowLogout = TRUE;
			OnCancel();
		} break;


	////////////////////////////// [���� ������ �۾�] //////////////////////////////

	case SV_GAMEITEMLIST: // ���� ������ ����Ʈ
		{
			CSV_GAMEITEMLIST msg;
			msg.Get(lpData, TotSize);

			// �޽��� �ڽ��� �߰��� �̹� ������ �������� ��� �����Ͱ� �ʱ�ȭ �ǹǷ� �̸� �����͸� ���� ����
			int kind = msg.pIN->Kind;
			int msglen1 = msg.pIN->StrMsgLen1;
			int msglen2 = msg.pIN->StrMsgLen2;
			char strmsg1[2048] = {0,};	// ������ ����� ����� ���ڿ�
			char strmsg2[2048] = {0,};	// ������ ���� ���ڿ�
			if(msglen1 > 0) strncpy(strmsg1, msg.StrMsg1, msglen1);
			if(msglen2 > 0) strncpy(strmsg2, msg.StrMsg2, msglen2);

			// ������ �ð��� Ŭ���̾�Ʈ�� ����ð��� ���
			CTime st(msg.pIN->ServerTime);
			g_ServerTime = st;
			g_ClientTime = CTime::GetCurrentTime();
			g_CSTimeGap = g_ClientTime - g_ServerTime;

			if(msg.pIN->ErrorCode == 0) {
				// ������ ���ٸ� �� ������ ����Ʈ�� ����
				g_MyGameItem.nNum = msg.pIN->TotNum;
				for(int i=0; i<g_MyGameItem.nNum; i++)
					g_MyGameItem.Item[i] = msg.ArrayGI[i];
			}

			// ������ ����Ʈ�� ����
			m_GameItemDlg.m_ItemListDlg.RefreshList();

			// �� ������ ����Ʈ�� ���
			if(kind == 0) {
				// ��� �Ϸ� �Ǿ���
			}
			// ������ ����Ʈ ����
			else if(kind == 100) {
				// ��� �Ϸ� �Ǿ���
			}
			// �α��� Ȥ�� ä�� ����� ���� ���� ������ ����� ���� ���
			else if(kind == 1 || kind == 2) {

				// �ߺ� ó�� ������
				static BOOL bProccessed = FALSE;
				if(bProccessed) break;
				bProccessed = TRUE;

				// ���̾�α� ������ ���� �ʱ�ȭ
				m_GameItemDlg.m_ItemListDlg.m_StrMsg = strmsg2;

				// �� ������ ����Ʈ ���� �ʱ�ȭ������ �����ְ� ���� �������� �⺻���� ���ý�Ŵ
				if(m_GameItemDlg.DoModal(1, FALSE, ITEM_SCODE_JUMP) != IDOK) {
					// �������� ������� �ʾ��� ���
					if(kind == 1) {
						// �α��� �� ���� ���� ������ ����� ���� ���
						// ���α׷� ����
						if(m_ConnectMsgDlg.GetSafeHwnd()) m_ConnectMsgDlg.OnCancel();
					}
					else if(kind == 2) {
						// ä�� ����� ���� ���� ������ ����� ���� ���
						g_Wait = FALSE;
						g_pTitleDlg->GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);
						// �޽��� â ����
						if(g_pTitleDlg->m_MsgDlg.GetSafeHwnd()) g_pTitleDlg->m_MsgDlg.OnOK();
					}
				}

				bProccessed = FALSE;
			}
		} break;

	case SV_USEGAMEITEM: // ���� ������ �����
		{
			CSV_USEGAMEITEM msg;
			msg.Get(lpData, TotSize);

			// ������ ����� �÷��� ����
			m_GameItemDlg.m_ItemListDlg.m_bItemUsed = FALSE;

			// ��ġ�ϴ� �������� ����Ͽ��� ã��
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

			// �ߺ� ó�� ������
			static BOOL bProccessed = FALSE;
			if(bProccessed) break;
			bProccessed = TRUE;

			// ���� ������ ������ ����
			g_MyGameItem.Item[index] = msg.pUG->Item;

			// ���� ������ ����Ʈ ����� ����(������ ���̾�αװ� �����Ǿ� ���� �ʴٸ� ����)
		//	m_GameItemDlg.m_ItemListDlg.SetItemList(g_MyGameItem.nNum, g_MyGameItem.Item);
			m_GameItemDlg.m_ItemListDlg.RebuildItemList();

			// ����� �������� ���� ����� �������ΰ�?
			if(g_MyGameItem.Item[index].Code >= ITEM_SCODE_JUMP && g_MyGameItem.Item[index].Code <= ITEM_ECODE_JUMP) {
				// ���� ����� �������� ����� ���

				if(g_Where == WH_LOGIN || g_Where == WH_TITLE) {
					// ���� ������ ��ȭ���ڸ� ����
					m_GameItemDlg.SafeClose();
				}

				if(g_Where == WH_TITLE) {
					// ä�� ����â���� ���� ����� �������� ����� ���
					g_Wait = FALSE;
					g_pTitleDlg->GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);
					// �޽��� â ����
					if(g_pTitleDlg->m_MsgDlg.GetSafeHwnd()) g_pTitleDlg->m_MsgDlg.OnOK();

					if(!msg.pUG->bUseError) {
						// ������ ��뿡 �����Ͽ����Ƿ� ä�� ������ �ٽ� �õ�
						g_pTitleDlg->PostMessage(WM_COMMAND, IDC_BUTTON_ENTER);
					}
				}
			}

			// ������ ��뿡 ���õ� �޽����� �����Ѵٸ� �޽����� ������
			if(msg.pUG->bMessage) {
				if(strlen(msg.pUG->StrMsg) > 0) {
					g_pMainDlg->m_MsgBoxDlg.MsgBox(0, msg.pUG->StrMsg);
				}
			}

			// �ߺ� ó�� ������
			bProccessed = FALSE;

		} break;

	case SV_BUYGAMEITEM: // ���� ������ ���� ����
		{
		CSV_BUYGAMEITEM msg;
			msg.Get(lpData, TotSize);

			// �ߺ� ó�� ������
			static BOOL bProccessed = FALSE;
			if(bProccessed) break;
			bProccessed = TRUE;

			int msglen = msg.pBI->StrMsgLen;
			char strmsg[1024] = {0,};

			if(msglen > 0) {
				// �޽��� �ڽ��� �߰��� �̹� ������ �������� ��� �����Ͱ� �ʱ�ȭ �ǹǷ� �̸� �����͸� ���� ����
				strncpy(strmsg, msg.StrMsg, msglen);

				int msgkind = msg.pBI->Kind;
				int itemcode = msg.pBI->ItemCode;

				BOOL bCancelBuyItem = FALSE;

				CAskBuyItemDlg dlg;
				dlg.m_strMsg = strmsg;
				dlg.m_ItemCode = itemcode;

				int rtn = dlg.DoModal();
				if(rtn == IDOK) {
					// ������ ���Կ� �����ߴٸ�...
					m_GameItemDlg.m_ItemListDlg.m_StrMsg = "";

					int firsttab = 0;

					if(msgkind == 1 || msgkind == 2) {
						// ������ ��� �������� ���� ����� �������� ���� 
						// ����ڰ� �������� ������ ���� ������ Ȯ���ϱ� ������ �ٷ� ���� �������� �̵�
						firsttab = 2;
					}
					else {
						// ��κ��� ��� �� �κп� ������ �� ������ ����Ʈ�� �����κ��� ���� �����̳�
						// �α��� �Ǳ� ���� ó���Ǿ� ���ٸ� ������ ����Ʈ�� ���� ��Ȳ�� �� ������ ����

						// ���� �ð��� �� ������ �ð����� ��ȯ
						CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
						SYSTEMTIME st;
						time.GetAsSystemTime(st);

						// ��� ������ �������� �ִ��� �˻�
						BOOL bItemUsing = FALSE;
						BOOL bItemExist = FALSE;
						bItemUsing = GameItem_IsUsingItem(g_MyGameItem, itemcode, &bItemExist, &st);
						if(bItemExist)
							firsttab = 1;	// �������� ����� �� �ֵ��� ������ ����Ʈ ������ �̵�
						else
							firsttab = 2;	// ��� ������ �������� �����Ƿ� ������ �� �ִ� ���� �������� �̵�
					}

					if(m_GameItemDlg.DoModal(firsttab, FALSE, itemcode) != IDOK) 
						bCancelBuyItem = TRUE;
				}
				else {
					// ������ ������ �ź��ߴٸ� 
					bCancelBuyItem = TRUE;
				}

				if( bCancelBuyItem ) {
					// ������ ������ ����Ͽ��ٸ�
					if(msgkind == 1) {
						// �α����߿� ���� �������� ������ ��� ��쿴�ٸ� ��� ���α׷� ����
						if(m_ConnectMsgDlg.GetSafeHwnd() && m_ConnectMsgDlg.IsWindowVisible()) {
							// ���α׷� ����
							m_ConnectMsgDlg.OnCancel();
						}
					}
					else if(msgkind == 2) {
						// ä�� ����â���� ���� ����� �������� ����� ���
						g_Wait = FALSE;
						g_pTitleDlg->GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);
						// �޽��� â ����
						if(g_pTitleDlg->m_MsgDlg.GetSafeHwnd()) g_pTitleDlg->m_MsgDlg.OnOK();
					}
					else if(msgkind == 3) {
						// ���۹��� �������� ����� ��� ���
					}
				}
			}

			bProccessed = FALSE;
			/*
			CSV_BUYGAMEITEM msg;
			msg.Get(lpData, TotSize);

			// �ߺ� ó�� ������
			static BOOL bProccessed = FALSE;
			if(bProccessed) break;
			bProccessed = TRUE;

			int msglen = msg.pBI->StrMsgLen;
			char strmsg[1024] = {0,};

			if(msglen > 0) {
				// �޽��� �ڽ��� �߰��� �̹� ������ �������� ��� �����Ͱ� �ʱ�ȭ �ǹǷ� �̸� �����͸� ���� ����
				strncpy(strmsg, msg.StrMsg, msglen);

				int msgkind = msg.pBI->Kind;

				BOOL bCancelBuyItem = FALSE;

				CAskBuyItemDlg dlg;
				dlg.m_strMsg = strmsg;
				dlg.m_ItemCode = msg.pBI->ItemCode;

				int rtn = dlg.DoModal();
				if(rtn == IDOK) {
					// ������ ���Կ� �����ߴٸ�...
					m_GameItemDlg.m_ItemListDlg.m_StrMsg = "";

					int firsttab = 0;

					if(msgkind == 1 || msgkind == 2) {
						// ������ ��� �������� ���� ����� �������� ���� 
						// ����ڰ� �������� ������ ���� ������ Ȯ���ϱ� ������ �ٷ� ���� �������� �̵�
						firsttab = 2;
					}

					if(m_GameItemDlg.DoModal(firsttab, TRUE) != IDOK) 
						bCancelBuyItem = TRUE;
				}
				else {
					// ������ ������ �ź��ߴٸ� 
					bCancelBuyItem = TRUE;
				}

				if( bCancelBuyItem ) {
					// ������ ������ ����Ͽ��ٸ�
					if(msgkind == 1) {
						// �α����߿� ���� �������� ������ ��� ��쿴�ٸ� ��� ���α׷� ����
						if(m_ConnectMsgDlg.GetSafeHwnd() && m_ConnectMsgDlg.IsWindowVisible()) {
							// ���α׷� ����
							m_ConnectMsgDlg.OnCancel();
						}
					}
					else if(msgkind == 2) {
						// ä�� ����â���� ���� ����� �������� ����� ���
						g_Wait = FALSE;
						g_pTitleDlg->GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);
						// �޽��� â ����
						if(g_pTitleDlg->m_MsgDlg.GetSafeHwnd()) g_pTitleDlg->m_MsgDlg.OnOK();
					}
					else if(msgkind == 3) {
						// ���۹��� �������� ����� ��� ���
					}
				}
			}

			bProccessed = FALSE;
			*/
		} break;

	////////////////////////////////////////////////////////////////////////////////

	// 2003 06 23
	case SV_ALLCHANINFO: // ��� ä�� ����
		{
			
			/*
			CSV_ALLCHANINFO msg;
			msg.Get(lpData, TotSize);

			g_Wait = FALSE;

			g_pTitleDlg->GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(TRUE);

			g_pTitleDlg->Chan_DeleteList();
			// ### [ ä�� ���� ���� ] ###
			for(int i=0; i<*msg.TotNum; i++) 
				g_pTitleDlg->Chan_InsItem(msg.ArrayCI[i].ChNum, msg.ArrayCI[i].Title, msg.ArrayCI[i].NowUserNum, msg.ArrayCI[i].MaxUserNum, msg.ArrayCI[i].ChannelStyle);

			// �������� ���õǾ��ִ� �������� �����ϰ� �Ѵ�(�������� ������ ä���� �˾ƺ� �� �ֵ���...)
			g_pTitleDlg->Chan_SelectLast();

			// �޽��� â ����
			g_pTitleDlg->m_MsgDlg.OnOK();
			*/
			
			CSV_ASK_ENTERCHAN aemsg;
			aemsg.Set(0);
			SockMan.SendData(g_MainSrvSID, aemsg.pData, aemsg.GetTotalSize());

		} break;

	case SV_ACCEPT_ENTERCHAN: // ä�� ���� ���
		{
			CSV_ACCEPT_ENTERCHAN msg;
			msg.Get(lpData, TotSize);


			// [���� ������ �۾�]
			// ���� �ڵ尡 -100���� ���� ���� �ƹ��� �޽����� ������� �ʴ´�
			// �޽���â�� ��ư�� Ȱ��ȭ�� ���� �������� ���� �� ��� �޽������� ó����
			if(msg.EC->ChNum <= -100) break;

			g_Wait = FALSE;
			g_pTitleDlg->GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);

			// �޽��� â ����
			g_pTitleDlg->m_MsgDlg.OnOK();

			if(msg.EC->ChNum < 0) {
				static BOOL bProcessed = FALSE;
				if(bProcessed) break;
				bProcessed = TRUE;
				switch(msg.EC->ChNum) 
				{
				//case -1: AfxMessageBox("��û�� ä���� �ִ� �����ο��� �ʰ��Ͽ����ϴ�."); break;
				//case -2: AfxMessageBox("�߸��� ����� ������ ���Ͽ� ä�ο� ������ �� �����ϴ�."); break;
				//case -3: AfxMessageBox("��û�� ä�� ��ȣ�� �ùٸ��� �ʽ��ϴ�."); break;
				//case -4: AfxMessageBox("���� ������ ����� �ƴմϴ�"); break;
				//default : AfxMessageBox("ä�ο� ������ �� �����ϴ�."); break;
				case -1: g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("CHAN_FULL"))); break;
				case -2: g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("CHAN_ERROR"))); break;
				case -3: g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("CHAN_NUM_ERR"))); break;
				case -4: g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("NO_LEVEL"))); break; //[������Ŀ]
				case -5: g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("NO_MONEY"))); break; //[������Ŀ]
				case -10: break; // ��ư Ȱ��ȭ ���� �ӽ�
				default : g_pMainDlg->m_MsgBoxDlg.MsgBox(0, (char*)(LPCSTR)g_StrMan.Get(_T("ENT_CHAN_ERR"))); break;
				}
				bProcessed = FALSE;
				break;
			}
			// ������ ä�� �̸��� ����
			g_ChannelName = msg.EC->ChanName;
			// ������ ä�ν�Ÿ���� ����
			g_nChannelStyle = msg.EC->ChanStyle;
			// ������ ä���� 5, 7�ο� 
			g_nchannel_usernum = msg.EC->channel_usernum; // 0: �游��� �켱, 1 : 5�ο� 2 : 7�ο�

			// ���Ƿ� �̵�
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
				str=g_StrMan.Get(_T("SRV_MES"));//"##<��������>##   ";
				//g_strSrvInfo+="\n";
				str+=g_strSrvInfo;// ä�� ����
				str+="\n";
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(255,0,0));
			}
			
			if(Sound.bInited == FALSE)
			{
				str = "�� ���̷�Ʈ ���� �ʱ�ȭ�� �����Ͽ� ���� ���� ����� �������� �ʽ��ϴ�.\n";
				str += "  �ٸ� ���� ���α׷����� ���� ��ġ�� ��������� �����Ͻð� \n ";
				str += "  �ý��ۿ� ���̷�ƮX�� ��ġ�Ǿ����� �ʴٸ� �ֽ� ������ ��ġ�Ͻñ� �ٶ��ϴ�.\n";
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(0,200,50));
			}

			ShowItemState();	// [���� ������ �۾�]
			// �� ��� �� ����� ��� ����Ʈ�� �䱸
			g_pLobyDlg->RefreshList();
		} break;
	case SV_ACCEPT_OUTCHAN: // ä�� ���� ���
		{
			CSV_ACCEPT_OUTCHAN msg;
			msg.Get(lpData, TotSize);

			g_Wait = FALSE;

			// Ÿ��Ʋ�� �̵�
			g_Where = WH_TITLE;
			ChangeView(0);
			
			// ä�ο��� ������ �ʱ�ȭ
			g_nChannelStyle = 0;

			// ä�� ��������
			g_pTitleDlg->bDontClickSound = TRUE;
			g_pTitleDlg->PostMessage(WM_COMMAND, IDC_BUTTON_REFRESH);
		} break;

	case SV_RESULT_FINDID: // ����� ã�� ���
		{
			CSV_RESULT_FINDID msg;
			msg.Get(lpData, TotSize);

			g_pTitleDlg->GetDlgItem(IDC_EDIT_FINDID)->EnableWindow(TRUE);
			g_pTitleDlg->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(TRUE);

			CResultFindIdDlg Dlg;

			if(strlen(msg.RF->ID) <= 0) {
				//::MessageBox(m_hWnd, "���� ������ �������� �ʾҰų� �������� �ʴ� ������Դϴ�.", "ID ã�� ����", MB_OK);
				Dlg.DoModal( g_StrMan.Get(_T("FIND_ERROR")) );
				//::MessageBox(m_hWnd, g_StrMan.Get(_T("FIND_ERROR")), "SevenPoker Hi/Low", MB_OK);
				//AfxMessageBox(g_StrMan.Get(_T("FIND_ERROR")));
			}
			else {
				CString str;
				if(msg.RF->Position == 0 || msg.RF->ChNum < 0) {
					//str.Format("[%s]���� ������ �����Ͽ����� ä�� �������Դϴ�.", msg.RF->ID); 
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
				//::MessageBox(m_hWnd, str, "ID ã�� ���", MB_OK);g_StrMan._Get(_T("FIND_RESULT"))
				//::MessageBox(m_hWnd, str, g_StrMan._Get(_T("FIND_RESULT")), MB_OK);
				Dlg.DoModal( str );
			}
			
		} break;

	case SV_ALLUSERINFO: // ��� ����� ����
		{
			CSV_ALLUSERINFO msg;
			msg.Get(lpData, TotSize);

			//#####################################
			// �޽��� â ����
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
				//### [������ ��� �۾�] ###
				if(msg.ArraySUI[i].cAdminUser != 1 ||  strcmp(g_MyInfo.UI.ID,msg.ArraySUI[i].ID)==0)
					m_LobyDlg.User_InsItem(unum, id, sex, winnum, loosenum, drawnum, point, position, roomnum, level);
			}
			//m_LobyDlg.m_UserList.UnlockWindowUpdate();
			g_pGameDlg->m_InviteDlg.User_ReloadList();// ���� �ʴ�â ����


		} break;

	case SV_USERINFO: // Ư�� ����� ����
		{
			CSV_USERINFO msg;
			msg.Get(lpData, TotSize);
			
			g_Wait = FALSE;

			// ���̵� ���̰� 0�̸� ���� ��ܿ� ���� �������
			if(strlen(msg.UI->ID) == 0) break;

			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
			CProfileDlg dlg;
			dlg.Init(msg.UI, TRUE);	// [���� ������ �۾�]	<-- ����
			dlg.DoModal();
			bProccessed = FALSE;
		} break;

	case SV_NEWUSER: // ���ο� ����� ����
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
			//### [������ ��� �۾�] ###
			if(msg.SUI->cAdminUser != 1)
				m_LobyDlg.User_InsItem(unum, id, sex, winnum, loosenum, drawnum, point, position, roomnum, level);
		} break;

	case SV_OUTUSER: // ����� ���� ����
		{
			CSV_OUTUSER msg;
			msg.Get(lpData, TotSize);
			int unum = msg.SUI->UNum;
			char *id = msg.SUI->ID;
			m_LobyDlg.User_DelItem(unum, id);
		} break;

	case SV_CHANGEUSER: // ����� �������� ����
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

	case SV_ALLROOMINFO: // ��� ������ ��� ����
		{
			CSV_ALLROOMINFO msg;
			msg.Get(lpData, TotSize);

			////////////////////////////////////
			// ### [ ���� ����Ʈ ���� ] ###
			g_pLobyDlg->m_RoomView.DelAllList();
			g_pLobyDlg->m_MsgDlg.OnOK();// �޽��� â ����
			for(int i=0; i<*msg.TotNum; i++)
			{
				// ### [ ������� ] ###
				m_LobyDlg.Room_InsItem( &msg.ArrayRI[i] );
			}
			////////////////////////////////////


			/*
			m_LobyDlg.m_RoomList.DeleteAllItems();
			g_pLobyDlg->m_MsgDlg.OnOK();// �޽��� â ����
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
				int observer = msg.ArrayRI[i].NowObserverNum;// ### [ ������� ] ###
				BOOL superroom = msg.ArrayRI[i].bSuperMaster;	// [���� ������ �۾�]
			
				// ### [ ������� ] ###
				m_LobyDlg.Room_InsItem(roomnum, master, title, level, nowuser, maxuser, observer, secret, state, gamekind,superroom);
			}
			*/

			
		} break;

	case SV_ROOMINFO: // ������ ����(������ ����)
		{
			CSV_ROOMINFO msg;
			msg.Get(lpData, TotSize);
			g_Wait = FALSE;

			if(*msg.ErrCode != 0) break;

			// �޼��� �ڽ��� �� ���¿��� �ٸ� �޼����� ������ �Ǵٽ� �ڽ��� �ߴ°��� ����
			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
			CRoomInfoDlg Dlg;
			Dlg.DoModal(msg.GI);
			bProccessed = FALSE;
		} break;

	case SV_ACCEPT_CHANGECHAR: // ĳ���� ���� ��û ���
		{
			CSV_ACCEPT_CHANGECHAR msg;
			msg.Get(lpData, TotSize);
			int charnum = *msg.CharNum;
			if(charnum < 0 || charnum > 72) break;
//			Play[0].UI.Character = charnum;
			//g_pLobyDlg->m_CharView.SetCharacter(charnum);
			g_pLobyDlg->m_CharView.SetCharacter(&Play[0].UI);
			g_Wait = FALSE;
			// �޼��� �ڽ��� �� ���¿��� �ٸ� �޼����� ������ �Ǵٽ� �ڽ��� �ߴ°��� ����
			static BOOL bProccessed = FALSE;
			if(bProccessed != FALSE) break;
			bProccessed = TRUE;
			AfxMessageBox("ĳ���Ͱ� ����Ǿ����ϴ�"); 
			bProccessed = FALSE;
		} break;


	case SV_ASK_CHANGESNDFX: // ### [���� �߰� �۾�] ###
		{
			if(g_Where != WH_GAME) break;

			CSV_ASK_CHANGESNDFX msg;
			msg.Get(lpData, TotSize);

			if(g_RI.RoomNum	!= *msg.RoomNum) break;

			int spnum = *msg.ServPNum;
			if(spnum < 0 || spnum >= g_Max_NewPlayer) break;// ### [ ������� ] ###

			int pn = Game.GetPlayerPNum(msg.ID);
			if(pn == -1) break;

			// ### [���� �߰� �۾�] ###
			Play[pn].nSndFxKind = *msg.SndFX;

			// ### [ ������� ] ###
			if(strcmp(msg.ID, g_MyInfo.UI.ID)== 0) {
				g_MyInfo.nSndFxKind = *msg.SndFX;
			}
			
		} break;

	case SV_REFUSE_CREATEROOM: // �� ������ �źεǾ���
		{
			CSV_REFUSE_CREATEROOM msg;

			msg.Get(lpData, TotSize);

			g_Wait = FALSE;
			g_pLobyDlg->iAccessRoomState=0;

			// �޼��� �ڽ��� �� ���¿��� �ٸ� �޼����� ������ �Ǵٽ� �ڽ��� �ߴ°��� ����
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

	case SV_ACCEPT_CREATEROOM: // �� ������ ���Ǿ���
		{
			CSV_ACCEPT_CREATEROOM msg;
			msg.Get(lpData, TotSize);

			// ### [ ������� ] ###
			Game.Accept_CreateRoom(&msg);
			
		} break;

	case SV_REFUSE_ENTERROOM: // �濡 ���� �źε�
		{
			CSV_REFUSE_ENTERROOM msg;
			msg.Get(lpData, TotSize);
			
			g_Wait=FALSE;
			g_pLobyDlg->iAccessRoomState=0;

			// ### [ ������� ] ###
			if(*msg.Code < 50)
			{
				g_Wait=FALSE;
				g_pLobyDlg->iAccessRoomState=0;

				// ���� ��ȭ���� ����
				g_pLobyDlg->m_MsgDlg.OnCancel();
			}

			// �޼��� �ڽ��� �� ���¿��� �ٸ� �޼����� ������ �Ǵٽ� �ڽ��� �ߴ°��� ����
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
					// [��ȣõ��] 2004.07.08
					CNewAskChargeDlg BigDlg;
					BigDlg.DoModal();	
				//g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("LECK_PMONEY1")));  break;
				} break;
			case 10: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[�˸�]\n���� �߹�� �濡�� �ٽ� ������ �� �����ϴ�");break;// [���� ������ �۾�]
			case 15: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[�˸�]\n�ߺ��� IP�� �����Ͽ� ������ �� �����ϴ�"); break;// ### [ �ߺ��� IP�� ] ###
			case 16:  break;// [ �ּ� �Ӵ� ���� ]
			// ### [ ������� ] ###
			case 50: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[�˸�]\n���ӿ� �������� �Ǿ����ϴ�."); break;
			case 51: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[�˸�]\n�ߺ��� IP�� �����մϴ�"); break;
			case 52: g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[�˸�]\n�������� �̵��� �� �����ϴ�"); break;
			default:  g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"[�˸�]\n��û�Ͻ� �濡 ������ �� �����ϴ�"); 
			
			}
			bProccessed = FALSE;
		} break;

	case SV_ACCEPT_ENTERROOM: // �濡 ���� �㰡����
		{
			CSV_ACCEPT_ENTERROOM msg;
			msg.Get(lpData, TotSize);

			static BOOL bProccessed = FALSE;
			if( bProccessed ) break;
			
			bProccessed = TRUE;
			// ### [ ������� ] ###
			Game.Accept_EnterRoom(&msg);
			bProccessed = FALSE;

	
		} break;

	case SV_NEWROOM: // ���ο� ���� �����Ǿ���
		{
			CSV_NEWROOM msg;
			msg.Get(lpData, TotSize);

			// ### [ ���� ����Ʈ ���� ] ###
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
			int observer = msg.RI->NowObserverNum;// ### [ ������� ] ### 
			BOOL superroom = msg.RI->bSuperMaster;	// [���� ������ �۾�]
			
			// ### [ ������� ] ### 
			m_LobyDlg.Room_InsItem(roomnum, master, title, level, nowuser, maxuser, observer, secret, state, gamekind,superroom);	// [���� ������ �۾�]);
			*/
		} break;

	case SV_OUTROOM: // ���� �����Ǿ���
		{
			CSV_OUTROOM msg;
			msg.Get(lpData, TotSize);

			int roomnum = msg.SRI->RoomNum;
			m_LobyDlg.Room_DelItem(roomnum);
		} break;

	case SV_CHANGEROOM: // �������� ���ŵǾ���
		{
			CSV_CHANGEROOM msg;
			msg.Get(lpData, TotSize);

			// ### [ ���� ����Ʈ ���� ] ###
			m_LobyDlg.Room_SetItem(msg.SRI);


		/*
			int roomnum = msg.SRI->RoomNum;
			int gamekind = msg.SRI->GameKind;
			char *master = msg.SRI->ID;
			int nowuser = msg.SRI->NowUserNum;
			int maxuser = msg.SRI->MaxUserNum;
			int state = msg.SRI->State;
			int observer = msg.SRI->NowObserverNum;// ### [ ������� ] ### 
			// ### [ ������� ] ### 
			m_LobyDlg.Room_SetItem(roomnum, gamekind, master, nowuser, maxuser, observer, state);
		*/
			
			// [ ###����### ] 
			g_TmpJackPotMoney = msg.SRI->JackPotMoney; // ����
		} break;

	case SV_ACCEPT_OUTROOM: // ��� ��û �㰡��
		{
			if(g_Where != WH_GAME) break;
			g_Wait=FALSE;

			// ### [ ������� ] ###
			g_MyInfo.UI.cObserver = 0;
			g_MyObserver = FALSE;
			g_MyInfo.JoinState = 0;
			g_MyInfo.PlayState = 0;
			Play[0].SetNewPlayer(&g_MyInfo.UI);
			Play[0].nSndFxKind = g_MyInfo.nSndFxKind;

			g_pGameDlg->OnExitGame();
		} break;

	case SV_CHANGEMYINFO: // �� ������ �����
		{
			CSV_CHANGEMYINFO msg;
			msg.Get(lpData, TotSize);
			
			// ### [ ������� ] ###
			g_MyInfo.UI.WinNum   = Play[0].UI.WinNum = msg.pCMI->WinNum;
			g_MyInfo.UI.LooseNum = Play[0].UI.LooseNum = msg.pCMI->LooseNum;
			g_MyInfo.UI.DrawNum = Play[0].UI.DrawNum = msg.pCMI->DrawNum;
			Play[0].UI.PMoney = msg.pCMI->PMoney;
			if(Play[0].UI.PMoney < 0) Play[0].UI.PMoney = 0;
			g_MyInfo.UI.PMoney   = Play[0].UI.PMoney;
			g_MyInfo.UI.nIcon = Play[0].UI.nIcon = GetPokerLevel(Play[0].UI.PMoney);

			g_pLobyDlg->Invalidate(FALSE);
			
		} break;
		
	case SV_USERENTERROOM:  // ���ο� �÷��̾ �Խ��Ͽ���
		{
			CSV_USERENTERROOM msg;
			msg.Get(lpData, TotSize);

			// ### [ ������� ] ###
			Game.User_EnterRoom(&msg);
		} break;
			
	case SV_USEROUTROOM: // ����ڰ� �濡�� ��������
		{
			CSV_USEROUTROOM msg;
			msg.Get(lpData, TotSize);
			
			// ### [ ������� ] ###
			Game.User_OutRoom(&msg);
		} break;

	case SV_CHANGEROOMMASTER: // ������ �ٲ�����
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
			
			// �� ������ �����Ѵ�
			strcpy(g_RI.ID, msg.ID);
			g_RI.UNum = *msg.UNum;
			
			// ���� Ÿ��Ʋ �ؽ�Ʈ�� �����Ѵ�
			CString rlevel,style;
			if(g_RI.Level == 2) rlevel = g_StrMan.Get(_T("NUM_3"));
			else if(g_RI.Level == 1) rlevel = g_StrMan.Get(_T("NUM_2"));
			else rlevel = g_StrMan.Get(_T("NUM_1"));

			
			if(g_RI.GameKind == 2) style = g_StrMan.Get(_T("FULL"));
			else if(g_RI.GameKind == 1) style = g_StrMan.Get(_T("HALF"));
			else style = g_StrMan.Get(_T("QUATER"));

			// �Ŀ� �����Ұ�
			CString str;
			str.Format(g_StrMan.Get(_T("ROOM_TITLE")),
				"Good Boy 7 Poker", g_ChannelName, g_RI.RoomNum, g_RI.Title, g_RI.ID,style,rlevel,
				g_RI.Secret==0?g_StrMan.Get(_T("L_OPEN")):g_StrMan.Get(_T("L_PASS")),g_StrMan.Get(_T("WHISNO")) );
			SetWindowText(str);

			// ### [ ������� ] ###
			if(strcmp(msg.ID, g_MyInfo.UI.ID) == 0) // ���� ������ �ȴ�
			{
				g_MastPN = 0;

				CString str;
				str.Format("## ����� �����Դϴ� ##\n");
				g_pGameView->ChatBox.AddText(&str);				
				g_pGameView->MasterBtn.Enable(TRUE); // ����ɼ� ��Ȱ�� ���� [62]
			}
			else
			{
				int newMastPN = -1;
				for(int i=1; i<g_Max_Player; i++) 
					if(strcmp(Play[i].UI.ID, msg.ID)==0) {newMastPN=i; break;}

				if(newMastPN == -1) break;

				g_MastPN = newMastPN;
				
				CString str;
				str.Format("## [%s]���� �����Դϴ� ##\n", msg.ID);
				g_pGameView->ChatBox.AddText(&str);
			}

		} break;

	case SV_ABNORMALOUTROOM: // ������������ ������
		{
			CSV_ABNORMALOUTROOM msg;
			msg.Get(lpData, TotSize);
			
			g_Wait=FALSE;
			if(g_Where != WH_GAME) break;
			
			// ���������� ������� �����ϰ� ������ ����
			m_GameDlg.bAbnormalOutRoom = TRUE;
			m_GameDlg.ExitGame();
		} break;

	case SV_ASK_INVITE: // �÷��̾ ��û �޼����� ����
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
			
			// �ʴ� �ź����̸� �ڵ����� �ź��Ѵ�
			g_pLobyDlg->UpdateData(TRUE);
			if(g_pLobyDlg->m_RefuseInviteCheck==TRUE)
			{
				CString str;
				str.Format("[%s]���� <�ʴ� �ź�>���Դϴ�.", Play[0].UI.ID);
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
					// ### [ ���� ����Ʈ ���� ] ###
					g_pLobyDlg->m_RoomView.m_SelectIndex = roomnum;
					g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_ENTERGAME);
					
					/*
					// �� ��ȣ�� ���ڿ��� ��ȯ
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
					// �ʴ���� ���� ���õǰ� �Ѵ�
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

	case SV_REFUSE_INVITE: // �÷��̾ ��û �޼����� �ź���
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

	case SV_ASK_BANISHVOTE: // �߹� ��ǥ�� ���۵�
		{
			CSV_ASK_BANISHVOTE msg;
			msg.Get(lpData, TotSize);

			if(g_Where!=WH_GAME) break;
			
			CString str;
			str.Format("[%s]�Կ����� �߹� ��ǥ�� �������Դϴ�\n", msg.pBV->TarID);
//			g_pGameDlg->m_ChatViewEdit.AddText(&str, RGB(255,0,0));
			g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(255,0,0));

			// ��ǥ ��û�� Ȥ�� ����ڸ� �޽����� ���
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

	case SV_GAMEOVERRESULT: // ������ ����Ǿ���
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


	// ### [�̺�Ʈ Ƽ��] ###
	case SV_EVENTPRIZE: // �̺�Ʈ ��÷ �˸�
		{
			CSV_EVENTPRIZE msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_GAME) break;
		//	Game.OnEventPrize(&msg);
		} break;
	case SV_EVENTNOTICE: // �̺�Ʈ ������ ��������
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

	case SV_SERVERMSG: // ���� �˸� �޼����� ��������
		{
			CSV_SERVERMSG msg;
			msg.Get(lpData, TotSize);

			CString str;
			CString chat;
			chat = (LPCTSTR)msg.StrChat;
			chat = chat.Mid(0, *msg.l_StrChat);

			if(*msg.Kind==11)
				str.Format("[���ΰ���]\n%s", chat.operator LPCTSTR());
			else
				str.Format("%s", chat.operator LPCTSTR());			// �޼��� ���� - jeong
				//str.Format("[���� �˸�]\n%s", chat.operator LPCTSTR());

			int Kind = *msg.Kind;

			if(*msg.Kind==0 || *msg.Kind==2 || *msg.Kind==11) { // �Ӵ����Ѱ���
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
					// ä�� ��ư ������
					Game.nChatRuleMessage = 1;
					//���۹�ư 
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
				//// �޽��� â ����
				//g_pTitleDlg->m_MsgDlg.OnOK();
				//::MessageBox(m_hWnd, str.operator LPCTSTR(), "�����κ��� �޼����� �����Ͽ����ϴ�.", MB_OK);
				m_MsgBoxDlg.MsgBox(1, str);
				break;
			}


			// ä�� ���� ���� �޼���
			if(*msg.Kind==3)
			{
				// �޽��� â ����
				g_pTitleDlg->m_MsgDlg.OnOK();
				break;
			}

			// ���� ���� ���� �޼���
			if(*msg.Kind==5)
			{
				// �޽��� â ����
				// ���̾� �α� �ڽ��� �����ش�. ���� ���� ���(* ������ �Ѵٰ� ���� �����ش�)
				m_ConnectMsgDlg.OnCancel();
				break;
			}

			if(*msg.Kind==4)
			{
				// �޽��� â ����
				g_pTitleDlg->m_MsgDlg.OnOK();
				//::MessageBox(m_hWnd, str.operator LPCTSTR(), "�����κ��� �޼����� �����Ͽ����ϴ�.", MB_OK);
				m_MsgBoxDlg.MsgBox(0, (char*)str.operator LPCTSTR());
				break;
			}

			// ä�� ���� ���� �޼���
			if(*msg.Kind==8)
			{
				// �޽��� â ����
				g_pTitleDlg->m_MsgDlg.OnOK();
				//::MessageBox(m_hWnd, str.operator LPCTSTR(), "�����κ��� �޼����� �����Ͽ����ϴ�.", MB_OK);
				m_MsgBoxDlg.MsgBox(0, (char*)str.operator LPCTSTR());
				break;
			}

			// �Ļ�â
			if(*msg.Kind == 9 ) // �����Ǿ���.
			{
			//	str.Format("���ӸӴϰ� �����Ͽ�, ������ ��� ������ �� �����ϴ�. 
			//	����Ŀ(62Cut)������ ����� ������ ��ư�� Ŭ���Ͽ� ���ӸӴϸ� ���� �����ñ� �ٶ��ϴ�.");			
				/*
				if(m_Oringdlg.GetSafeHwnd() == NULL && m_Oringdlg.m_ShowDlg == FALSE){	
					m_Oringdlg.m_ShowDlg = TRUE;												
					if(m_Oringdlg.DoModal() == IDOK){
						g_pMainDlg->OnCancel();
					}
				}
				*/


				m_GameItemDlg.SafeClose();
				// [��ȣõ��] 2004.07.08
				CNewAskChargeDlg BigDlg;
				//BigDlg.DoModal();				- ����â deleted by jeong
			}
			if(*msg.Kind == 30)//���� ���� ī��Ʈ�� ������
			{
				if(g_Where==WH_GAME &&  g_RI.State == 0){
					COLORREF tcolor = RGB(255,255,0);
					g_pGameDlg->m_GameView.ChatBox.AddText(&str, tcolor);
					// ä�� ��ư ������
					Game.nChatRuleMessage = 1;
				}
			}

		} break;

	case SV_CHATDATA: // ���� ä�õ���Ÿ�� ��������
		{
			CSV_CHATDATA msg;
			msg.Get(lpData, TotSize);
			
			if(g_Where != WH_LOBY) break;

			CString str;
			CString chat;
			chat = (LPCTSTR)msg.StrChat;
			chat = chat.Mid(0, *msg.l_StrChat);

			//(�߰�)
			// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
			for(int s=0; s<chat.GetLength()-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13) {
					chat.SetAt(s, ' ');
				}
			}

			str.Format("%s> %s\n", msg.ID, chat.operator LPCTSTR());
			g_pLobyDlg->m_ChatViewEdit.SetTextFormat(0);
			
			if( strcmp("������",msg.ID)==0 )// ### [������ ��� �۾�] ###
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(255,0,0));
			else
				g_pLobyDlg->m_ChatViewEdit.AddText(&str);

			
		} break;

	case SV_WHISPERCHAT: // �ӼӸ�
		{
			CSV_WHISPERCHAT msg;
			msg.Get(lpData, TotSize);

			// ��ȭ ���� �ź����� ���̵��̸� ����
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

			//(�߰�)
			// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
			for(int s=0; s<chat.GetLength()-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13) {
					chat.SetAt(s, ' ');
				}
			}

			str.Format("[�Ӹ�:%s] %s\n", msg.ID, chat.operator LPCTSTR());

			// �Ӹ� �˸� ȿ���� ���
			Sound.Play(SND50);

			if(g_Where==WH_LOBY)
			{
				g_pLobyDlg->m_ChatViewEdit.SetTextFormat(0);
				g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(100,100,100));
			}
			if(g_Where==WH_GAME)
			{
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(250,250,250));
				// ä�� ��ư ������
				Game.nChatRuleMessage = 1;

			}
		} break;

	// ���ӹ� ä�õ���Ÿ
	case CL_CHATDATA:
		{
			CCL_CHATDATA msg;
			msg.Get(lpData, TotSize);

			if(g_Where != WH_GAME) break;
			if(*msg.RoomNum != g_RI.RoomNum) break;

			// ��ȭ ���� �ź����� ���̵��̸� ����
			int pnum = Game.GetPlayerPNum(msg.ID);
			if(pnum != -1) {
				if(Play[pnum].bShutMouse) break;
			}

			CString str;
			char chat[256]={0,};
			strncpy(chat, msg.StrChat, min(*msg.l_StrChat, 255));

			//(�߰�)
			// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
			int slen = strlen(chat);
			for(int s=0; s<slen-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(chat[s] == 8 || chat[s] == 9 || chat[s] == 10 || chat[s] == 13) {
					chat[s] = ' ';
				}
			}

			str.Format("%s> %s\n", msg.ID, chat);
			if( strcmp("������",msg.ID)==0 )// ### [������ ��� �۾�] ###
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, SERVERMSG_COLOR);
			else
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, CHATTING_COLOR);

			// ä�� ��ư ������
			Game.nChatRuleMessage = 1;

		} break;

			//2004.05.07 �������Ѿ� ����
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
				
				strcpy(al.SockNameIP, SockMan.pDataSock[g_MainSrvSID]->SockAddr);	// ����� ������ ��巹��
				strcpy(al.HostNameIP, SockMan.GetServerIP());						// ȣ��Ʈ �ý��� ��巹��
				al.Port = SockMan.pDataSock[g_MainSrvSID]->SockPort;				// ����� ������ ��Ʈ��ȣ
				// ������ ������ �����ϱ� ���� 20000���� ������ ��Ʈ��ȣ�� ����Ѵ�
				// ���� ����ϰԵ� ��Ʈ��ȣ�� ���μ����� ���Ͽ� �����ȴ�
				al.Port += 20000; 
				
				al.AuthCookieLen = g_AuthCookie.GetLength();
				al.DataCookieLen = g_DataCookie.GetLength();
				al.bForceLogin = TRUE;
				
				// �α��� �޼����� ������
				CSV_ASK_LOGIN lgimsg;
				lgimsg.Set(&al, (char*)(LPCTSTR)g_AuthCookie, (char*)(LPCTSTR)g_DataCookie);
				SockMan.SendData(g_MainSrvSID, lgimsg.pData, lgimsg.GetTotalSize());
			}
			
		} break;

	/////////////////////////// Ŭ���̾�Ʈ�� �޼���  ////////////////////////

	// ### [ ������� ] ### 
	case CL_EXITSUBSCRIPT:
		{
			CCL_EXITSUBSCRIPT msg;
			msg.Get(lpData, TotSize);
			if(g_Where != WH_GAME) break;

			int pnum = Game.GetPlayerPNum(msg.ID);
			if(pnum==-1) break;

			int bexit = *msg.bExit;

			CString str;
			// ### [ ������� ] ###
			if(bexit == 0)
			{
				if(strcmp(g_MyInfo.UI.ID, msg.ID) == 0)
				{// ������
					g_pGameView->bExitSubscript = FALSE;
					g_pGameDlg->ExitGame();
				}
			}
			else if(bexit==1)
			{
				if(strcmp(g_MyInfo.UI.ID, msg.ID) == 0)
				{// ������
					g_pGameView->bExitSubscript = TRUE;
				}
				str.Format("[%s]�� ������ ����\n", Play[pnum].UI.ID);
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(150,150,0));
			}
			else if(bexit == 2)
			{
				if(strcmp(g_MyInfo.UI.ID, msg.ID) == 0)
				{// ������
					g_pGameView->bExitSubscript = FALSE;
				}
				str.Format("[%s]�� ������ ���\n", Play[pnum].UI.ID);
				g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(150,150,0));
			}
		} break;

	/////////////////////////// ���� ����� �޼���  ////////////////////////

	case CL_CHANGEGAMEKIND: // ���� ����� ����Ǿ���
		{
		} break;

	case CL_STARTINFO: // ���� ���� ����
		{
		} break;

	
	// ���� ����
	case SV_STARTCARD:
		{
			CSV_STARTCARD msg;
			msg.Get(lpData, TotSize);
			if(g_Where != WH_GAME) break;
			if(g_RI.State != 0) break;

			int pnum = Game.GetPlayerPNum(msg.ID);
			if(pnum == -1) break;
			
			// ### [ ������� ] ###
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
			// ����ī�� ���ý� ���� Ÿ�̸� �����ش�.
			// ### [ ������� ] ###
			if( g_MyObserver ) {
				if(Game.bGameStart && pnum != 0 && g_RI.FormKind == 1 && Play[pnum].PlayState == 1) {
					// �߰� ���̽� ī��
					Game.CardDeck[pnum].Card[3].Clear();
					Game.CardDeck[pnum].TotalCardNum -= 1;
				}
			}
			else
			{
				if(Game.bGameStart && pnum != 0 && g_RI.FormKind == 1 && Play[pnum].PlayState == 1) {
					// �߰� ���̽� ī��
					Game.CardDeck[pnum].Card[3].Clear();
					Game.CardDeck[pnum].TotalCardNum -= 1;
				}
			}
			/////////////////////////////////////////////////////////////////////
			Play[pnum].m_bSelectCard = FALSE;

		} break;

	// ���� ��� ī�峯���� ����
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

	// ������ ����
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
			g_TmpJackPotMoney = *msg.JackPotMoney; // [ ###����### ] 

			int k=0;
			if(Play[pnum].PlayState == 1 && Play[pnum].JoinState == 1) { // ���� ������ ���ΰ�
				Play[pnum].bFold = TRUE;
				Play[pnum].PlayState = 0;
				
				if(Play[pnum].PNum == Play[0].PNum)g_bChangeCardStage = FALSE; // 62cut mode reset[62]

				if( !g_MyObserver && pnum== 0) { // ### [ ������� ] ###
					g_MyInfo.bFold = TRUE;
					g_MyInfo.PlayState = 0;
				}
				//���� ������ �� ������.
				Game.StartUserNum--;
				if(Game.StartUserNum <0) Game.StartUserNum = 0;
			
				if(pnum == 0 && Play[0].nCardTotal>3) {// ########### ���� ����� ī�带 �ٽ� �׸���
					for( k = 0; k < Play[0].nCardTotal; k++) {
						CPoint pt;
						pt.x = Game.CardDeck[0].Card[k].Xp;
						if( g_MyObserver ) pt.y = Game.CardDeck[0].Card[k].Yp;	// ### [ ������� ] ###
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
				Game.SoundFxKind(1, Play[pnum].UI.Sex, Play[pnum].nSndFxKind);// ### [���� �߰� �۾�] ### ����
				CString str;
				str.Format(g_StrMan.Get(_T("FOLD_USER")),Play[pnum].UI.ID);
				g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(220,220,0));
			}

		} break;
	// ������ ���� ī��
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
				Game.CardDeck[0].Card[6].CardNo = *msg.nCard; // �� ����
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
			
			if(msg.pCC->nFlag == 6){// �ٲٱ� ����
				g_RI.ChangeCardStep = msg.pCC->nChangeStep;	
				Game.ChangeCard_Init();
			}		
			else if(msg.pCC->nFlag == 100){//�ٲ� ī�� ������ �޾Ƽ� ī�带 �ٲ۴�							
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
			
			//�ٲ� ī�� ������ �޾Ƽ� ī�带 �ٲ۴�					
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

	// ���� ���� ��� == > ��� ����
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

			
			// ������ ������¸� ���� ����
			if(g_pGameView->bExitSubscript) {
				g_pGameDlg->ExitGame();
			}
			else {
				// ���� ���� ����
				Game.ResetGame();
				Game.CurPlayer = g_Poker.CurPlayer;
			}
		}
		break;

	// [ ###�ٷν���### ]
	case SV_REFUSE_QUICKSTART:
			{
				static BOOL bProcessed = FALSE;
				if(bProcessed) break;
				bProcessed = TRUE;
				g_pLobyDlg->CreateRoom();
				bProcessed = FALSE;
			}
			break;
	case SV_ENDRAISE: //���̷ο�
		{
		}
		break;
	
	// [ ###����### ] 
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
					Sound.Play(SND36); // ���ķ�
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
			//	str.Format("%s�� %d���� %s���� ��ī�� �ϼ̽��ϴ�. ���� ��� %I64d \n", msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, msg.pSC->JackPotPlus );
				str.Format(g_StrMan.Get(_T("MES_SPE1")),
				msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, msg.pSC->JackPotPlus, spec);
				break;
			case 2:
			//	str.Format("%s�� %d���� %s���� ��Ʈ����Ʈ �÷��� �ϼ̽��ϴ�. ���� ��� %I64d \n", msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID , msg.pSC->JackPotPlus);
				str.Format(g_StrMan.Get(_T("MES_SPE1")),
				msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, msg.pSC->JackPotPlus, spec);
				break;
			case 3:
			//	str.Format("%s�� %d���� %s���� �ξ⽺Ʈ����Ʈ �÷��� �ϼ̽��ϴ�. ���� ��� %I64d \n", msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, msg.pSC->JackPotPlus );
				str.Format(g_StrMan.Get(_T("MES_SPE1")),
				msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, msg.pSC->JackPotPlus, spec);
				break;
			}
		*/
		
			// �����濡�� ���Դ�
			if(msg.pSC->JackPotPlus > 0 && strcmp(msg.pSC->ChanName, g_ChannelName)==0 && msg.pSC->nRoom == g_RI.RoomNum )
			{
				Sound.Play(SND36); // ���ķ�
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
				
				Game.m_JackPotActive = msg.pSC->JackPotPlus; // ���� ���ӹ濡�� ���Դ�.
				Sound.Play(SND31); // ���� Ĩ�Ҹ�

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
			//### [������ ��� �۾�] ###
	case NM_ADMIN_WHISPERCHAT:
		{
			CSV_WHISPERCHAT msg;
			msg.Get(lpData, TotSize);
			
			
			CString str;
			CString chat;
			chat = (LPCTSTR)msg.StrChat;
			chat = chat.Mid(0, *msg.l_StrChat);
			
			//(�߰�)
			// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
			for(int s=0; s<chat.GetLength()-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(chat.GetAt(s) == 8 || chat.GetAt(s) == 9 || chat.GetAt(s) == 10 || chat.GetAt(s) == 13) {
					chat.SetAt(s, ' ');
				}
			}
			str.Format("[�Ӹ�:%s] %s\n", "������", chat.operator LPCTSTR());
			// �Ӹ� �˸� ȿ���� ���
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

		//### [������ ��� �۾�] ###  ������ ����
	case SV_CHANGESTYLEROOMINFO:
		{
			// ���� ��ġ�� �׹��ΰ�, ���� �ΰ�
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
				
				// ### Ÿ��Ʋ ###
				
				
				str.Format("[%s] - [%d����] [������: %s]", g_ChannelName, g_RI.RoomNum, g_RI.Title);
				g_pGameView->m_titlestr=str;
				
				str.Format("��������  ����Ǿ����ϴ�.\n");
				g_pGameDlg->m_GameView.ChatBox.AddText(&str,RGB(220,220,0));
			}
			else if(g_Where == WH_LOBY)
			{
				// ### [ ���� ����Ʈ ���� ] ###
				m_LobyDlg.Room_SetItem(msg.SRI, TRUE);
/*
				int roomnum = msg.SRI->RoomNum;
				m_LobyDlg.Room_SetItem(roomnum, 0, "", 0, 0, 0,0, msg.SRI->Title, TRUE);// ### [ ������� ] ###
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



// ### ������ ���� ##
void C62CutPokerDlg::CreateShortCut()
{
	// ���α׷� �̸�
	LPCTSTR strProgName = "Good Boy 7 Poker";
	// URL
	LPCSTR pszURL = "http://www.goodboy7poker.com";//"http://NewPoker.goodboy7poker.com";
	// �ٷΰ��� �̸�
	LPCSTR pszlnkName = "NewPoker";
	// �ٷΰ��� ����
	LPCSTR szDescription = "Good Boy 7 Poker (NewPoker)\nȨ�������� �̵��մϴ�.";
	

	CRegKey Key;
	int ShortCutVersion = 0;

	// �ٷΰ��� �������� ������ �о��
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
				// ������Ʈ���� �ٷΰ��� �������� ������ ����(Ű�� ������ Ű�� ����)
				char verstr[100]={0,};
				wsprintf(verstr, "%d", VERSION);
				Key.SetValue(verstr, "ShortCutVersion");

				// �������ϸ��� ����(���������� �������� ����ϱ� ����)
				char szFileName[MAX_PATH]={0,};
				GetModuleFileName(AfxGetInstanceHandle(), szFileName, MAX_PATH-1);
				// ���ͳ� �ٷΰ��� �������� ����ȭ�鿡 ����
				CreateInternetShortCutToDesktopFolder(pszURL, pszlnkName, szDescription, szFileName, 0);
			}
		}
	}
}



// [���� ������ �۾�]
void C62CutPokerDlg::BrowserMove(LPCTSTR targeturl) 
{ 	
	if(!targeturl) return;
	if(strlen(targeturl)==0) return;

	#if(0) // ���� ���(��� ����)				
		// �ƹ�Ÿ ��� �⺻ ���� ���
		//char strbuf[1024] = {0,}; 
		char strbuf[1024] = "Avatar"; 
		//GetBaseAvatarDir(strbuf, 1023); 

		// ������ ���� ���α׷�
		CString exeName = strbuf; 
		exeName += "\\NMIEMoveMFC.exe"; 

		// �ݸ��� �������� �̵�
		::ShellExecute( NULL, "open", exeName, targeturl, NULL, SW_HIDE );
	#else // ���ο� ���
		OpenNetmarbleWebpage(g_MyUniqNo, targeturl);
	#endif
} 

// [���� ������ �۾�]
void C62CutPokerDlg::ShowItemState()
{
	CString str;

	// ���� �ð��� �� ������ �ð����� ��ȯ
	CTime time = CTime::GetCurrentTime() - g_CSTimeGap;
	SYSTEMTIME st;
	time.GetAsSystemTime(st);

	CTimeSpan tmptime=0; // [��ȣõ��] 2004.07.09
	int tmp = -1;

	// �������� ���¸� ���
	for(int i=0; i<g_MyGameItem.nNum; i++) {
		ItemState state = GameItem_GetItemState(g_MyGameItem.Item[i], &st);
		if(state.iState == 2) {

				// [��ȣõ��] 2004.07.09
			if( ITEM_SCODE_SAFEGUARDANGEL == GameItem_GetItemKind(g_MyGameItem.Item[i].Code) ) {		
				if(tmptime < state.LeftTime) {
					tmptime = state.LeftTime;
					tmp = i;
				}
				continue;
			}

			if(state.LeftTime.GetDays() > 0) {
				str.Format("�� [%s] ����� (������� %d�� %d�ð� %d�� ���ҽ��ϴ�.)\n", 
					GameItem_GetItemName(g_MyGameItem.Item[i]), 
					state.LeftTime.GetDays(), state.LeftTime.GetHours(), state.LeftTime.GetMinutes());
			}
			else {
				str.Format("�� [%s] ����� (������� %d�ð� %d�� ���ҽ��ϴ�.)\n", 
					GameItem_GetItemName(g_MyGameItem.Item[i]), 
					state.LeftTime.GetHours(), state.LeftTime.GetMinutes());
			}

			g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(200,0,0));
		}
	}

	// [��ȣõ��] 2004.07.09
	if(tmp != -1) {
		if(tmptime.GetDays() > 0) {
			str.Format("�� [%s] ����� (���� ���� %d�� %d�ð� %d�� ���ҽ��ϴ�.)\n", 
				GameItem_GetItemName(g_MyGameItem.Item[tmp]), 
				tmptime.GetDays(), tmptime.GetHours(), tmptime.GetMinutes());
		}
		else {
			str.Format("�� [%s] ����� (���� ���� %d�ð� %d�� ���ҽ��ϴ�.)\n", 
				GameItem_GetItemName(g_MyGameItem.Item[tmp]), 
				tmptime.GetHours(), tmptime.GetMinutes());
		}
		
		g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(200,0,0));
	}
}

// [���� ������ �۾�]
void C62CutPokerDlg::ShowMyItemBag()
{
	// ���̾�α� ������ ���� �ʱ�ȭ
	m_GameItemDlg.m_ItemListDlg.m_StrMsg = "";
//	m_GameItemDlg.m_ItemListDlg.SetItemList(g_MyGameItem.nNum, g_MyGameItem.Item);
	// ���� ������ ��ȭ����
	m_GameItemDlg.DoModaless();
}

// [PC�� �۾�]
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
		g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"��ũ�� ���� \'�� ����\'������ �����Ͽ����ϴ�.");
	}
}


void C62CutPokerDlg::LoadSpriteFile()
{
	if(GameBackSpr.Load(".\\data\\GameBack.spr", 555)==FALSE) AfxMessageBox("GameBack.spr ������ ���� �� �����ϴ�");
//	if(GameBackSpr[1].Load(".\\data\\skin1\\GameBack1.spr", 555)==FALSE) AfxMessageBox("GameBack1.spr ������ ���� �� �����ϴ�");
//	if(GameBackSpr[2].Load(".\\data\\skin2\\GameBack2.spr", 555)==FALSE) AfxMessageBox("GameBack2.spr ������ ���� �� �����ϴ�");

	if(LevelSpr.Load(".\\data\\Level.spr", 555)==FALSE) AfxMessageBox("Level.spr ������ ���� �� �����ϴ�");
	if(CharBackSpr.Load(".\\data\\CharBack.spr", 555)==FALSE) AfxMessageBox("CharBack.spr ������ ���� �� �����ϴ�");
	if(InfoBoxSpr.Load(".\\data\\InfoBox.spr", 555)==FALSE) AfxMessageBox("InfoBox.spr ������ ���� �� �����ϴ�");
	if(ChatBoxSpr.Load(".\\data\\ChatBox.spr", 555)==FALSE) AfxMessageBox("ChatBox.spr ������ ���� �� �����ϴ�");
	if(ExitBtnSpr.Load(".\\data\\ExitBtn.spr", 555)==FALSE) AfxMessageBox("ExitBtn.spr ������ ���� �� �����ϴ�");
	//if(InviteBtnSpr.Load(".\\data\\InviteBtn.spr", 555)==FALSE) AfxMessageBox("InviteBtn.spr ������ ���� �� �����ϴ�");
	if(FullScreenBtnSpr.Load(".\\data\\FullScreenBtn.spr", 555)==FALSE) AfxMessageBox("FullScreenBtn.spr ������ ���� �� �����ϴ�");
	if(WindowBtnSpr.Load(".\\data\\WindowBtn.spr", 555)==FALSE) AfxMessageBox("WindowBtn.spr ������ ���� �� �����ϴ�");
	//if(OptionBtnSpr.Load(".\\data\\OptionBtn.spr", 555)==FALSE) AfxMessageBox("OptionBtn.spr ������ ���� �� �����ϴ�");
	//if(BadUserBtnSpr.Load(".\\data\\BadUserBtn.spr", 555)==FALSE) AfxMessageBox("BadUserBtn.spr ������ ���� �� �����ϴ�");
	//if(MasterBtnSpr.Load(".\\data\\MasterBtn.spr", 555)==FALSE) AfxMessageBox("MasterBtn.spr ������ ���� �� �����ϴ�");
	//if(MinimizeBtnSpr.Load(".\\data\\MinimizeBtn.spr", 555)==FALSE) AfxMessageBox("MinimizeBtn.spr ������ ���� �� �����ϴ�");
	////
	

	// ���� ���� ���� �̹��� 
	//if(GameStartBtnSpr.Load(".\\data\\MainStartBtn.spr", 555)==FALSE) AfxMessageBox("MainStartBtn.spr ������ ���� �� �����ϴ�");
	if(GameStartBtnSpr.Load(".\\data\\StartBtn.spr", 555)==FALSE) AfxMessageBox("MainStartBtn.spr ������ ���� �� �����ϴ�");
	if(GameQuitBtnSpr.Load(".\\data\\Mini\\endBtn.spr", 555)==FALSE) AfxMessageBox("endBtn.spr ������ ���� �� �����ϴ�");
	if(X2StartBtnSpr.Load(".\\data\\Mini\\bonusG.spr", 555)==FALSE) AfxMessageBox("bonusG.spr ������ ���� �� �����ϴ�");
	if(AvaSpr.Load(".\\data\\aba.spr", 555)==FALSE) AfxMessageBox("aba.spr ������ ���� �� �����ϴ�");
	if(AllinBtnSpr.Load(".\\data\\Allinbtn.spr", 555)==FALSE) AfxMessageBox("AllinBtn.spr ������ ���� �� �����ϴ�");
	if(DieBtnSpr.Load(".\\data\\FoldBtn.spr", 555)==FALSE) AfxMessageBox("DieBtn.spr ������ ���� �� �����ϴ�");
	if(CheckBtnSpr.Load(".\\data\\CheckBtn.spr", 555)==FALSE) AfxMessageBox("CheckBtn.spr ������ ���� �� �����ϴ�");
	if(CallBtnSpr.Load(".\\data\\CallBtn.spr", 555)==FALSE) AfxMessageBox("CallBtn.spr ������ ���� �� �����ϴ�");
	if(PingBtnSpr.Load(".\\data\\PingBtn.spr", 555)==FALSE) AfxMessageBox("PingBtn.spr ������ ���� �� �����ϴ�");
	if(DadangkBtnSpr.Load(".\\data\\DadangkBtn.spr", 555)==FALSE) AfxMessageBox("DadangkBtn.spr ������ ���� �� �����ϴ�");
	if(MaxBtnSpr.Load(".\\data\\MaxBtn.spr", 555)==FALSE) AfxMessageBox("MaxBtn.spr ������ ���� �� �����ϴ�");

	//[62]
	if(ChangeOKBtnSpr.Load(".\\data\\changeokBtn.spr", 555)==FALSE) AfxMessageBox("changeokBtn.spr ������ ���� �� �����ϴ�");
	if(ChangeCancelBtnSpr.Load(".\\data\\changecancelBtn.spr", 555)==FALSE) AfxMessageBox("changecancelBtn.spr ������ ���� �� �����ϴ�");
	if(ChangeCardInfoSpr.Load(".\\data\\changecardinfo.spr", 555)==FALSE) AfxMessageBox("changecardinfo.spr ������ ���� �� �����ϴ�");	

	if(RoomMasterSpr.Load(".\\data\\roommaster.spr", 555)==FALSE) AfxMessageBox("roommaster.spr ������ ���� �� �����ϴ�");
	if(BanishVoteSpr.Load(".\\data\\banishvote.spr", 555)==FALSE) AfxMessageBox("banishvote.spr ������ ���� �� �����ϴ�");
//	if(SortSpr.Load(".\\data\\sortbtn.spr", 555)==FALSE) AfxMessageBox("sortbtn.spr.spr ������ ���� �� �����ϴ�");

	///	
	//if(EtcBtnSpr.Load(".\\data\\EtcBtn.spr", 555)==FALSE) AfxMessageBox("EtcBtn.spr ������ ���� �� �����ϴ�");
	if(EtcSpr.Load(".\\data\\Etc.spr", 555)==FALSE) AfxMessageBox("Etc.spr ������ ���� �� �����ϴ�");
	///////////////////////////////////////////////////////////////////////////
	if(BigCardSpr.Load(".\\data\\BigCard.spr", 555)==FALSE) AfxMessageBox("BigCard.spr ������ ���� �� �����ϴ�");
	if(SmallCardSpr.Load(".\\data\\SmallCard.spr", 555)==FALSE) AfxMessageBox("SmallCard.spr ������ ���� �� �����ϴ�");

	if(FocusSpr.Load(".\\data\\Focus.spr", 555)==FALSE) AfxMessageBox("Focus.spr ������ ���� �� �����ϴ�");
	if(TimerSpr.Load(".\\data\\Timer.spr", 555)==FALSE) AfxMessageBox("Timer.spr ������ ���� �� �����ϴ�");
	
	// [ ###����### ] 
	if(JackPotBackSpr.Load(".\\data\\JackPotBack.spr", 555)==FALSE) AfxMessageBox("JackPotBack.spr ������ ���� �� �����ϴ�");
	if(JackPotSpr.Load(".\\data\\JackPot.spr", 555)==FALSE) AfxMessageBox("JackPot.spr ������ ���� �� �����ϴ�");
	if(ChipSpr.Load(".\\data\\Chip.spr", 555)==FALSE) AfxMessageBox("Chip.spr ������ ���� �� �����ϴ�");
	// ### [ �Ļ� ] ###
	if(OringCharSpr.Load(".\\data\\OringChar.spr", 555)==FALSE) AfxMessageBox("OringCharSpr.spr ������ ���� �� �����ϴ�");
	//if(ItemBagSpr.Load(".\\data\\ItemBagBtn.spr", 555)==FALSE) AfxMessageBox("ItemBagBtn.spr ������ ���� �� �����ϴ�");
	// ### [ �ߺ��� IP�� ] ###
	if(WarningSpr.Load(".\\data\\Warning.spr", 555)==FALSE) AfxMessageBox("Warning.spr ������ ���� �� �����ϴ�");
	// [ȭ��ĸ��]
	//if(CaptureSpr.Load(".\\data\\CaptureBtn.spr", 555)==FALSE) AfxMessageBox("CaptureBtn.spr ������ ���� �� �����ϴ�");
	// ### [ ������� ] ###
	if(ObserverSpr.Load(".\\data\\ObserverBtn.spr", 555)==FALSE) AfxMessageBox("ObserverBtn.spr ������ ���� �� �����ϴ�");

	//////////////////////////////////////////////////
	// ### [ ���� ����Ʈ ���� ] ###

	if(LobyListBackSpr.Load(".\\data\\LobyListBack.spr", 555)==FALSE) AfxMessageBox("LobyListBack.spr ������ ���� �� �����ϴ�");
	if(LobyListButtonSpr.Load(".\\data\\LobyListBackBtn.spr", 555)==FALSE) AfxMessageBox("LobyListBackBtn.spr ������ ���� �� �����ϴ�");
	if(LobyRoomImgSpr.Load(".\\data\\LobyRoomImg.spr", 555)==FALSE) AfxMessageBox("LobyRoomImg.spr ������ ���� �� �����ϴ�");

	//////////////////////////////////////////////////

	//�ִ����� (����)
	
	if(AllinAniSpr.Load(".\\data\\AllinAni.spr", 555)==FALSE) AfxMessageBox("AllinAni.spr ������ ���� �� �����ϴ�");
	
}

void C62CutPokerDlg::Reload_spritefile()
{
	CString str;		
	if(g_BgMode >0)str.Format(".\\data\\skin%d\\",g_BgMode);
	else str = ".\\data\\";
	
	CString temp,temp2;	
	temp2= "������ ������ �����ϴ�.";	
	

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

	g_pGameView->BatPage.PutSprAuto(0, 0, g_pGameView->m_pgamebackspr, 1);			// ���� Ĩ �������� �����
	Game.ChipFunc(NULL);//Ĩ �׸���

}

void C62CutPokerDlg::load_spritefile_7()
{
	// 7�ο� �̹���
	if(GameBackSpr_7.Load(".\\data\\new_7\\GameBack_7.spr", 555)==FALSE) AfxMessageBox("GameBack_7.spr ������ ���� �� �����ϴ�");
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
	case ACD_GO_CHARGE: // ���� ������ �̵�
		{
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			g_pMainDlg->bNowLogout = FALSE; // ������ �˸��� �״´�.
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
		
	case ACD_GO_LEADERS: // ������ ������ �̵�
		{
			
			CString str ="http://www.goodboy7poker.com/Premium/Club.asp?pgamode=netpoker";
			//		if(m_LJ.PremLType==0)
			//			g_pMainDlg->BrowserMove( str + m_LJ.LeadersJoinURL ); // ����
			//		else
			//			g_pMainDlg->BrowserMove( str + m_LJ.LeadersJoinURL2 );// ����
			
			BrowserMove(str);
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			g_Where = WH_LOBY;
			PostQuitMessage( 0 );
			SockMan.Destroy();
			
		} break;

	case ACD_INVINCIBLE_BUY: // õ�Ϲ��� ������ ����
		{
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			m_GameItemDlg.DoModaless(2,FALSE,ITEM_SCODE_INVINCIBLE);
		} break;

	case ACD_INVINCIBLE_DETAILS : // õ�Ϲ��� ������ �ڼ��� ���� 
		{
			g_Where = WH_LOBY;
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			CString url;
			url = _T("http://www.goodboy7poker.com/ItemShop/?targetUrl=game/gameitem_info.asp?item_no=385");
			BrowserMove(url);
			PostQuitMessage( 0 );
			SockMan.Destroy();			
		} break;

	case ACD_CLOSEWINDOW:	// ��ȣõ�� â�ݱ� 
		{
		//	::SendMessage((HWND)lParam,WM_CLOSE,0,0);
			g_Where = WH_LOBY;
			PostQuitMessage( 0 );
			SockMan.Destroy();
			::SendMessage((HWND)lParam,WM_CLOSE,0,0);
		} break;

	case ACD_ALLIN_INSURANCE_BUY:	//���� ���� ��� 
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

	case ACD_ALLIN_INSURANCE_USE:	//���� ���� ����ϱ� 
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

		
	case ACD_ALLIN_INSURANCE: // ���� ���� �ڼ��� ����
		{
			// [��ȣõ��] 2004.07.09

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



















