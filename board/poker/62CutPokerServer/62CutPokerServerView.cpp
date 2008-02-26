// 62CutPokerServerView.cpp : implementation of the C62CutPokerServerView class
//

#include "stdafx.h"
#include "62CutPokerServer.h"

#include "62CutPokerServerDoc.h"
#include "62CutPokerServerView.h"

// [서버 에이전트 작업] 
#include "MainFrm.h"

#include "ConfigDlg.h"
#include "Config2Dlg.h"
#include "Config3Dlg.h"
#include "ConfigJackPotDlg.h"
#include "ConfigEvent.h" // ### [이벤트 티켓] ###
#include "StartUpDlg.h"
#include "DBThread.h"
#include "AdminManager.h"	 // [관리자 모드 작업]
#include "CaptureTalk.h" // [대화 저장]	
#include "ConfigSADlg.h" // [ 통계기록 ]

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 전역 변수
C62CutPokerServerView *g_pMainView=NULL;
CDBMan DBMan;					// DB 메니저 전역 객체
CSockMan SockMan;				// 소켓 메니져 전역 객체
CChannel Chan[MAX_CHAN];		// 채널 클래스
CUser User[MAX_USER];			// 사용자 클래스
CNMStrMan g_StrMan;				// 언어(문자열)
POKERSET g_POKERSET;			// [세븐포커] 레벨 머니제한

// ### [로그 기록용] ###
CLogManDBMan LogDBMan;
INT64 g_LogRangeValue;

// [ ###잭팟### ]
INT64 g_JackPotMoney;			// 잭팟 머니
int g_ServerGrade;				// 등급

ADMIN_MANAGER             g_AdminInfo; // [관리자 모드 작업]
ADMIN_ACCESSDENY_USER	 g_AccessDeny[MAX_DENY_USER];

int  g_nAgentIndex = -1; // [서버 에이전트 작업]
CString      g_AppStartPath  = _T("");
extern C62CutPokerServerApp theApp;			// [PC방 작업]

CAPTURE_INFO       g_CaptureID; // [대화 저장]	
SERACH_INFO        g_SearchID; // [위치 저장]

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerView
IMPLEMENT_DYNCREATE(C62CutPokerServerView, CFormView)

BEGIN_MESSAGE_MAP(C62CutPokerServerView, CFormView)
	//{{AFX_MSG_MAP(C62CutPokerServerView)
	ON_MESSAGE(UM_SOCK_ACCEPT, OnServAccept)
	ON_MESSAGE(UM_SOCK_CLOSE, OnDataClose)
	ON_MESSAGE(UM_SOCK_CONNECT, OnDataConnect)
	ON_MESSAGE(UM_DBWORK_LOGIN, OnDbworkLogin)
	ON_MESSAGE(UM_DBWORK_LOGOUT, OnDbworkLogout)
	ON_MESSAGE(UM_DBWORK_SETDB, OnDbworkSetOK)
	ON_MESSAGE(UM_DBWORK_DCON, OnDbworkDoubleConnect)
	ON_COMMAND(ID_VIEW_CONFIG, OnViewConfig)
	ON_BN_CLICKED(IDC_BUTTON_SENDSERVERMSG, OnButtonSend)
	ON_BN_CLICKED(IDC_RADIO_SENDTEXT, OnRadioSendtext)
	ON_BN_CLICKED(IDC_RADIO_SENDMSGBOX, OnRadioSendmsgbox)
	ON_BN_CLICKED(IDC_RADIO_SENDMSGBOTH, OnRadioSendmsgboth)
	ON_BN_CLICKED(IDC_BUTTON_SETLOGINMSG, OnButtonSetloginmsg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FORCEDISCONNECT, OnButtonForcedisconnect)
	ON_BN_CLICKED(IDC_BUTTON_FORCEDELROOM, OnButtonForcedelroom)
	ON_COMMAND(ID_VIEW_CONFIG2, OnViewConfig2)
	ON_COMMAND(ID_VIEW_CONFIG3, OnViewConfig3)
	ON_COMMAND(ID_VIEW_CONFIG4, OnViewConfig4)
	ON_BN_CLICKED(IDC_BUTTON_JACKPOT, OnButtonJackpot)
	ON_BN_CLICKED(IDC_RADIO_EXIT1, OnRadioExit1)
	ON_BN_CLICKED(IDC_RADIO_EXIT10, OnRadioExit10)
	ON_BN_CLICKED(IDC_RADIO_EXIT3, OnRadioExit3)
	ON_BN_CLICKED(IDC_RADIO_EXIT30, OnRadioExit30)
	ON_BN_CLICKED(IDC_RADIO_EXIT30S, OnRadioExit30s)
	ON_BN_CLICKED(IDC_RADIO_EXIT5, OnRadioExit5)
	ON_BN_CLICKED(IDC_BUTTON_EXITOK, OnButtonExitok)
	ON_BN_CLICKED(IDC_BUTTON_EXITCANCEL, OnButtonExitcancel)
	ON_MESSAGE(UM_DBWORK_USEITEMRESULT, OnDbworkUseitemresult)	// [게임 아이템 작업]
	ON_MESSAGE(UM_DBWORK_GAMEITEMLIST, OnDbworkGameitemlist)	// [게임 아이템 작업]
	ON_COMMAND(IDM_SET_ADMIN, OnSetAdmin)
	ON_BN_CLICKED(IDC_BTN_DENYUSER, OnBtnDenyuser)
	ON_COMMAND(IDM_SET_CAPTUREID, OnSetCaptureid)
	ON_COMMAND(IDM_SET_SEARCH, OnSetSearch)
	ON_COMMAND(ID_VIEW_CONFIGEVENT, OnViewConfigevent)      // ### [이벤트 티켓] ###
	ON_BN_CLICKED(IDC_BUTTON_EVENTSTATE, OnButtonEventstate)// ### [이벤트 티켓] ###
	ON_MESSAGE(UM_DBWORK_EVENTSET, OnDbworkEventSet) // ### [이벤트 티켓] ###
	ON_COMMAND(ID_VIEW_CONFIGSA, OnViewConfigsa)	// [ 통계기록 ]
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerView construction/destruction

C62CutPokerServerView::C62CutPokerServerView()
	: CFormView(C62CutPokerServerView::IDD)
{
	g_pMainView=this;
	hMsgPoolingTimer = NULL;
	hGlobalGameTimer = NULL;
	hCloseTimer = NULL;
	bIsForceClose = FALSE;
	bConnectDeny = FALSE;
	TotUser = 0;
	RealUser = 0;
	g_ServerGrade = 0;
	// [ ###잭팟### ]
	g_JackPotMoney = 0;
	// ### [로그 기록용] ###
	g_LogRangeValue = 0;

	// ### [이벤트 티켓] ###
	TotEventTicketNum = 0;
	NowEventTicketNum = 0;
	LastEventTick = timeGetTime();

	////////////////////////////////////////////////////////////////
	///// 자동종료
	hExitReserveTimer = NULL;
	m_bExitReserve = FALSE;
	m_ReservTimeLimit = 0;
	///////////////////////////////////////////////////////////////	
	hMsgUerCntTimer = NULL;		// [PC방 작업]

	// [ 통계기록 ]
//	ZeroMemory(&m_StatisticsLog, sizeof(m_StatisticsLog));

//	m_JackPot = 0;
	//{{AFX_DATA_INIT(C62CutPokerServerView)
	m_ServerMsg = _T("");
	m_ServerLoginMsg = _T("");
	m_DiscTargetID = _T("");
	m_DelRoomNo = _T("");
	m_iExitRadio = -1;
	m_strMSCode = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

C62CutPokerServerView::~C62CutPokerServerView()
{
}

void C62CutPokerServerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C62CutPokerServerView)
	DDX_Control(pDX, IDC_BUTTON_EXITOK, m_ExitReserve);
	DDX_Control(pDX, IDC_BUTTON_EXITCANCEL, m_ExitCancel);
	DDX_Text(pDX, IDC_EDIT_SENDSERVERMSG, m_ServerMsg);
	DDX_Text(pDX, IDC_EDIT_LOGINMSG, m_ServerLoginMsg);
	DDX_Text(pDX, IDC_EDIT_DISCTARGETID, m_DiscTargetID);
	DDX_Text(pDX, IDC_EDIT_DELROOMNO, m_DelRoomNo);
	DDX_Radio(pDX, IDC_RADIO_EXIT30, m_iExitRadio);
	DDX_Text(pDX, IDC_EDIT_MSCODE, m_strMSCode);
	//}}AFX_DATA_MAP
}

BOOL C62CutPokerServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void C62CutPokerServerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();


//	SaveCurrentDir(); // 현재 디렉토리를 저장한다

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
	char strDir[500]={0,};
	GetCurrentDirectory(500, strDir);

	CString strReportDir;
	strReportDir.Format("%s\\DB_ERROR",strDir);//g_AppStartPath);
	if(_chdir(strReportDir.operator LPCTSTR())!=0){
		_mkdir(strReportDir.operator LPCTSTR());
	}
/*
	strReportDir.Format("%s\\BACKUP",strDir);//g_AppStartPath);
	if(_chdir(strReportDir.operator LPCTSTR())!=0){
		_mkdir(strReportDir.operator LPCTSTR());
	}
*/
	strReportDir.Format("%s\\Config",strDir);
	if(_chdir(strReportDir.operator LPCTSTR())!=0){
		_mkdir(strReportDir.operator LPCTSTR());
	}

	strReportDir.Format("%s\\Config",strDir);
	if(_chdir(strReportDir.operator LPCTSTR())!=0){
		_mkdir(strReportDir.operator LPCTSTR());
	}
	
	SetCurrentDirectory(strDir);
	//SetCurrentDirectory(strDi
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

	// 로그 기록 디렉토리 설정 및 찾기
	CLogFile logfile3(_T("DB_ERROR\\SERVERSTART.TXT"));
	if(!logfile3.Writef(_T("[INITIALIZE SERVER START]"))){
		MessageBox(_T("\"\\DB_ERROR\\SERVERSTART.TXT\" : Create init file error!\n"),_T("Error"),MB_OK);
		bIsForceClose = TRUE;
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return ;
	}
/*
	logfile3.SetLogFile(_T("BACKUP\\SERVERSTART.TXT"));
	if(!logfile3.Writef(_T("[INITIALIZE SERVER START]"))){
		MessageBox(_T("\"\\BACKUP\\SERVERSTART.TXT\" : Create init file error!\n"),_T("Error"),MB_OK);
		bIsForceClose = TRUE;
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return ;
	}
*/
	// [대화 저장] 아디
	LoadCaptureTalkID();
	// [위치 저장] 아이디
	LoadSearchID();


	// [관리자 모드 작업]
	LoadAdminInfo();
	for(int i=0; i<MAX_DENY_USER; i++) memset(&g_AccessDeny[i],0,sizeof(ADMIN_ACCESSDENY_USER));

	// 환경 설정파일을 읽는다.
	LoadConfig();
	LoadConfig2();
	LoadConfig3();
	LoadConfig4();
	LoadConfigEvent();// ### [이벤트 티켓] ###
	LoadConfigStatis(); // 통계기록

		/*
	// [PC방 작업] - 이부분을 주석처리
	CStartUpDlg StartUpDlg;

	if(StartUpDlg.DoModal() == IDCANCEL) {
		// 서버를 강제 종료한다
		bIsForceClose = TRUE;
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return;
	}
	else {
		// 다시 환경 설정파일을 읽는다.
		LoadConfig();
		LoadConfig2();
		LoadConfig3();
		LoadConfig4();
	}
	*/

	// [PC방 작업] - 이렇게 바꾼당~
	InitNetmarbleCommn();
	BOOL bAutoRun = FALSE;
	CStartUpDlg StartUpDlg;
	if(theApp.m_lpCmdLine[0] != '\0' && strcmp(theApp.m_lpCmdLine,"nonstop")==0) {
		StartUpDlg.m_bInitConnectStatus = FALSE;
	} 
	else {
		if(StartUpDlg.DoModal() == IDCANCEL) {
			// 서버를 강제 종료한다
			bIsForceClose = TRUE;
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
			return;
		}
		else {
			// 다시 환경 설정파일을 읽는다.
			LoadConfig();
			LoadConfig2();
			LoadConfig3();
			LoadConfig4();
			LoadConfigEvent();// ### [이벤트 티켓] ###
			LoadConfigStatis(); // 통계기록
		}
	}



	g_ServerGrade = 0;
	// 현서버 등급
	if(Cfg3.EnterLimitMode == 1 && Cfg3.bLoginLimit) {
		switch(Cfg3.LoginGrade)
		{
		case 0: // 자유
			{
				g_ServerGrade = 0;
				Cfg3.LoginLimitLevel1 = 0;
				Cfg3.LoginLimitLevel2 = 14;
			}
			break;
		case 1: // 초보
			{
				g_ServerGrade = 1;
				Cfg3.LoginLimitLevel1 = 0;
				Cfg3.LoginLimitLevel2 = 1;
			}
			break;
		case 2: // 중수
			{
				g_ServerGrade = 2;
				Cfg3.LoginLimitLevel1 = 2;
				Cfg3.LoginLimitLevel2 = 7;
			}
			break;
		case 3: // 고수
			{
				g_ServerGrade = 3;
				Cfg3.LoginLimitLevel1 = 8;
				Cfg3.LoginLimitLevel2 = 11;
			}
			break;
		case 4: // 도신
			{
				g_ServerGrade = 4;
				Cfg3.LoginLimitLevel1 = 12;
				Cfg3.LoginLimitLevel2 = 14;
			}
			break;
		
		default: //자유 0, 1
			{
				g_ServerGrade = 0;
				Cfg3.LoginLimitLevel1 = 0;
				Cfg3.LoginLimitLevel2 = 14;
			}
			break;
		}
	}


	// 채널 클래스 초기화
	for( i=0; i<MAX_CHAN; i++) 
		Chan[i].Init(i);

	TotUser = 0;
	RealUser = 0;

	// 랜덤 초기화
	srand((unsigned)timeGetTime());

	// 서버 메세지 보내기 에디트와 버튼등을 초기화
	SendMsgMode = 0;
	CheckRadioButton(IDC_RADIO_SENDTEXT, IDC_RADIO_SENDMSGBOTH, IDC_RADIO_SENDTEXT);
	ClientMode = 1;
	IPMode = 0;
	
	////////////////////////////////////////////////////////////////
	///// 자동종료
	// 서버 종료 메세지초기화, 버튼
	SendExitMode = 2;
	m_ReservTimeLimit = 300; // 기본 종료 3분
	m_iExitRadio = 2;
	UpdateData(FALSE);
	m_ExitCancel.EnableWindow(FALSE);
	/////////////////////////////////////////////////////////////////

	// 소켓 메니저 초기화
	SockMan.Init(m_hWnd);

	// 서버 메인 보여주기
	ShowServerState();

	// 메시지 다이얼로그 생성
	MsgDlg.Create(this, "DB 메니저를 초기화하는 중입니다");

	// DB 메니저를 초기화 한다
	if(DBMan.Init(Cfg2.MaxDBThread)==FALSE) {
		UINT rtn = AfxMessageBox("데이터 베이스를 열 수 없습니다. 지금 환경설정을 하시겠습니까?", MB_YESNO);
		if(rtn==IDYES) {
			CConfigDlg Dlg;
			Dlg.DoModal();

			// 서버를 강제 종료한다
			AfxMessageBox("변경된 설정을 적용시키기위하여 지금 서버를 종료합니다.");
			MsgDlg.DestroyWindow();
			bIsForceClose = TRUE;
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
			return;
		}
	}

#ifdef LOG_RECORD_
	if(g_pMainView->Cfg.bLogUseDB || g_pMainView->Cfg.bStatisticsDB || g_pMainView->Cfg.bAllinDB)
	{
		MsgDlg.SetMsg("로그 DB 메니저를 초기화하는 중입니다.");
		// ### [로그 기록용] ###
		if(LogDBMan.Init(Cfg2.MaxDBThread2)==FALSE) {
			UINT rtn = AfxMessageBox("데이터 베이스를 열 수 없습니다. 지금 환경설정을 하시겠습니까?", MB_YESNO);
			if(rtn==IDYES) {
				CConfigDlg Dlg;
				Dlg.DoModal();
				
				// 서버를 강제 종료한다
				AfxMessageBox("변경된 설정을 적용시키기위하여 지금 서버를 종료합니다.");
				MsgDlg.DestroyWindow();
				bIsForceClose = TRUE;
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return;
			}
		}
	}
	
#endif


	// 같은 코드를 사용하는 다른 서버정보가 존재하는지 검사한다
	if(Cfg.GameCode!=0 && Cfg.ServerCode!=0) {
		SockMan.CheckHostInfo();
		if(DBMan.CheckOtherServerExist(Cfg.GameCode, Cfg.ServerCode, SockMan.GetServerIP(), Cfg.ServerPort)) {
			CString strmsg;
			strmsg.Format("게임코드 %d번 서버코드(채널) %d번을 사용하는 IP혹은 포트번호가 다른 서버정보가 존재합니다.\n환경설정을 통하여 서버 정보를 변경하려면 [예]를 클릭하십시오.\n현재의 설정을 적용시키기 위해서는 [아니오]를 클릭하십시오.\n지금 환경설정을 하시겠습니까?", Cfg.GameCode, Cfg.ServerCode);
			UINT rtn = AfxMessageBox(strmsg, MB_YESNO);
			if(rtn==IDYES) {
				CConfigDlg Dlg;
				Dlg.DoModal();

				// 서버를 강제 종료한다
				AfxMessageBox("변경된 설정을 적용시키기위하여 지금 서버를 종료합니다.");
				MsgDlg.DestroyWindow();
				bIsForceClose = TRUE;
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return;
			}
		}
	}

	MsgDlg.SetMsg("DB쓰레드를 생성하고 테이블을 여는중입니다.");

	// 유지 모드의 DB쓰레드를 생성한다
	if(DBMan.CreateKeepAliveThread()==FALSE) {
		UINT rtn = AfxMessageBox("DB쓰레드 생성시 OPEN에 실패한 테이블이 존재합니다.\n지금 환경설정을 하시겠습니까?", MB_YESNO);
		if(rtn==IDYES) {
			CConfigDlg Dlg;
			Dlg.DoModal();

			// 서버를 강제 종료한다
			AfxMessageBox("변경된 설정을 적용시키기위하여 지금 서버를 종료합니다.");
			MsgDlg.DestroyWindow();
			bIsForceClose = TRUE;
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
			return;
		}
	}

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB || g_pMainView->Cfg.bStatisticsDB)
{
	MsgDlg.SetMsg("로그DB쓰레드를 생성하고 테이블을 여는중입니다.");
	// ### [로그 기록용] ###
	//유지 모드의 DB쓰레드를 생성한다
	if(LogDBMan.CreateKeepAliveThread()==FALSE) {
		UINT rtn = AfxMessageBox("DB쓰레드 생성시 OPEN에 실패한 테이블이 존재합니다.\n지금 환경설정을 하시겠습니까?", MB_YESNO);
		if(rtn==IDYES) {
			CConfigDlg Dlg;
			Dlg.DoModal();

			// 서버를 강제 종료한다
			AfxMessageBox("변경된 설정을 적용시키기위하여 지금 서버를 종료합니다.");
			MsgDlg.DestroyWindow();
			bIsForceClose = TRUE;
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
			return;
		}
	}
}
#endif

	MsgDlg.SetMsg("접속자 현황 DB를 초기화합니다.");
	if(Cfg.GameCode!=0 && Cfg.ServerCode!=0)
	{
		SockMan.CheckHostInfo();
		BOOL rtn = DBMan.InitStatusDB(Cfg.GameCode, Cfg.ServerCode, SockMan.GetServerIP(), Cfg.ServerPort);
		if(rtn==FALSE) AfxMessageBox("접속자 현황 DB초기화가 실패하였습니다");
	}

	if(StartUpDlg.m_bInitConnectStatus) {
	/*
		MsgDlg.SetMsg("게임코드와 채널번호를 검색합니다.");
	*/
	}

	MsgDlg.SetMsg("불량 아이디 신고 DB쓰레드를 가동합니다.");

	// 불량 아이디 신고 DB쓰레드 가동
	BeginBadUserDBThread();

	MsgDlg.SetMsg("임시로 오픈한 DB를 Close합니다.");

	// 임시로 오픈한 DB를 닫는다
	DBMan.CloseTempDB();


#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB || g_pMainView->Cfg.bStatisticsDB)
{
	// ### [로그 기록용] ###
	/////////////////////////////////////
	// 임시로 오픈한 DB를 닫는다
	LogDBMan.CloseTempDB();
	/////////////////////////////////////
}
#endif

	// 메시지 다이얼로그 닫음
	MsgDlg.DestroyWindow();

	// 서버용 소켓을 생성한다
	if(SockMan.CreateServerSocket(Cfg.ServerPort)==FALSE) {
		AfxMessageBox("서버용 리스닝 소켓을 생성할 수 없습니다.\n같은 포트번호를 사용하고 있는 어플리케이션이 존재하는지 확인하시기 바랍니다");
	}

	CLogFile logfile;
	logfile.Writef(" 서버 가동 - GameCode=%d, ServerCode=%d", Cfg.GameCode, Cfg.ServerCode);

	// 전역 게임 타이머 가동
	hGlobalGameTimer = SetTimer(GLOBALGAME_TIMER, 1000, NULL);
	// 소켓 메시지 풀링 타이머 가동
	hMsgPoolingTimer = SetTimer(MSGPOOLING_TIMER, 1000/12, NULL);
	
	// [ ###잭팟### ]
	CString strM = g_MakeCommaMoney(g_JackPotMoney);
	ShowState((char*)strM.operator LPCTSTR());

	// [PC방 작업]
	M_ImHereBridge(AfxGetMainWnd()->m_hWnd,g_pMainView->Cfg.GameCode,g_pMainView->Cfg.ServerCode,g_pMainView->Cfg.ServerPort,g_pMainView->Cfg.ServerName);
	hMsgUerCntTimer = SetTimer(USERCNT_TIMER, 10000, NULL);

	/////////////////////////////////////////////////
	// [서버 에이전트 작업] 처음 시버 실행시 접속거부 상태........
	CMainFrame *pMain = (CMainFrame*) GetParentFrame();
	pMain->Connectdeny();
	//SendMessage(WM_COMMAND,ID_FILE_CONNECTDENY);
	ShowState(_T("사용자의 접속을 거부합니다. 에이전트를 실행해 주십시요." ));
	/////////////////////////////////////////////////

	// [ 통계기록 ]
	SYSTEMTIME st;	GetLocalTime(&st);
//	m_cSALog.PrevTime_1 = st;
//	m_cSALog.PrevTime_2 = st;

	//////////
	// 상태

	GetDlgItem(IDC_EDIT_BSSO)->SetWindowText("▶[ DB Base ]");

	if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse)
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("▶ LogDB Use");
	else if(g_pMainView->Cfg.bLogUseDB)
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("▷ LogDB Use");
	else
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("◐ LogDB Use");


	if( g_pMainView->Cfg.bEventDB && g_pMainView->EventCfg.bRunEvent ) {	// ### [이벤트 티켓] ###
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("▶ EventDB Use");
	} else if( g_pMainView->Cfg.bEventDB ) {
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("▷ EventDB Use");
	} else {
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("◐ EventDB Use");
	}
	////////


	// AI, 클라이언트 실행 파일 구동 - jeong

	Sleep(50);
	::ShellExecute(NULL, "open", "NewPokerAI_Debug.exe", NULL, ".", SW_HIDE);
	//Sleep(1000);
	//::ShellExecute(NULL, "open", "NewPoker_Debug.exe", NULL, ".", SW_HIDE);
	
}


// 서버 상태를 보인다
void C62CutPokerServerView::ShowServerState()
{
	
	CString str;
	// 메인 타이틀바에 프로그램 이름과 함께 채널번호를 세팅
	AfxGetMainWnd()->GetWindowText(str);
	CString tmpstr;
	//tmpstr.Format(" %d (Ver%4.2f) 뉴포커(7 Poker) 서버입니다", Cfg.ServerCode, VERSION/100.0f);
	tmpstr.Format(" 뉴 7 포커 (Ver%4.2f) 서버입니다", VERSION/100.0f);

	str += tmpstr;
	AfxGetMainWnd()->SetWindowText(str);

	GetDlgItem(IDC_STATIC_SERVERNAME)->SetWindowText(Cfg.ServerName);
	str.Format("%d",Cfg.GameCode);
	GetDlgItem(IDC_STATIC_GAMECODE)->SetWindowText(str);
	str.Format("%d",Cfg.ServerCode);
	GetDlgItem(IDC_STATIC_SERVERCODE)->SetWindowText(str);
	GetDlgItem(IDC_STATIC_IP)->SetWindowText(SockMan.GetServerIP());
	GetDlgItem(IDC_STATIC_DNS)->SetWindowText(SockMan.GetServerDomain());
	str.Format("%d", Cfg.ServerPort);
	GetDlgItem(IDC_STATIC_PORT)->SetWindowText(str);
	str.Format("MaxSocket:%d MaxUser:%d(-1) MaxRoom:%d(-1) (SRoom:%d)", MAX_DATA_SOCKET, MAX_USER, MAX_ROOM, MAX_SUPERROOM);
	GetDlgItem(IDC_STATIC_MAXSTATE)->SetWindowText(str);
	
	switch(Cfg.nMSCode){
		case 0:	 m_strMSCode = _T("KOREAN");   break;
		case 1:	 m_strMSCode = _T("ENGLISH");  break;
		case 2:	 m_strMSCode = _T("JAPANESE"); break;
		case 3:  m_strMSCode = _T("CHINESE");  break;
		default: m_strMSCode = _T("KOREAN");   break;
	}

	GetDlgItem(IDC_EDIT_AGENTCONNECT)->SetWindowText(_T("◐No Agent ")); // [서버 에이전트 작업]

	UpdateData(FALSE);
}






/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerView diagnostics

#ifdef _DEBUG
void C62CutPokerServerView::AssertValid() const
{
	CFormView::AssertValid();
}

void C62CutPokerServerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

C62CutPokerServerDoc* C62CutPokerServerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C62CutPokerServerDoc)));
	return (C62CutPokerServerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerView message handlers

void C62CutPokerServerView::LoadConfig()
{
	// 환경설정 파일을 연다
	ZeroMemory(&Cfg, sizeof(Cfg));

	FILE *fp = fopen( "62CutPoker.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("환경설정 파일을 읽을 수 없습니다.\n다시 설정하세요.");
		return;
	}
	fread(&Cfg, sizeof(CONFIG), 1, fp);
	fclose(fp);
}

void C62CutPokerServerView::LoadConfig2()
{
	// 환경설정 파일을 연다
	ZeroMemory(&Cfg2, sizeof(Cfg2));

	FILE *fp = fopen( "game.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("게임옵션 설정 파일을 읽을 수 없습니다.\n다시 설정하세요.");
		return;
	}
	fread(&Cfg2, sizeof(CONFIG2), 1, fp);
	fclose(fp);
}

void C62CutPokerServerView::LoadConfig3()
{
	// 환경설정 파일을 연다
	ZeroMemory(&Cfg3, sizeof(Cfg3));
	FILE *fp = fopen( "channel.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("채널 설정 파일을 읽을 수 없습니다.\n다시 설정하세요.");
		return;
	}
	fread(&Cfg3, sizeof(CONFIG3), 1, fp);
	fclose(fp);

	
////////////////////////
	// 7 포커 // 제거 할것
	g_POKERSET.nHM = g_pMainView->Cfg3.nHM;
	g_POKERSET.nFM = g_pMainView->Cfg3.nFM;
	g_POKERSET.nGM = g_pMainView->Cfg3.nGM;

	g_POKERSET.nHL = g_pMainView->Cfg3.nHL;
	g_POKERSET.nFL = g_pMainView->Cfg3.nFL;
	g_POKERSET.nGL = g_pMainView->Cfg3.nGL;
	
///////////////////////

}

void C62CutPokerServerView::LoadConfig4()
{
	// 환경설정 파일을 연다
	ZeroMemory(&Cfg4, sizeof(Cfg4));
	FILE *fp = fopen( "JackPot.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("잭팟머니 파일을 읽을 수 없습니다.\n다시 설정하세요.");
		return;
	}
	fread(&Cfg4, sizeof(CONFIG4), 1, fp);
	fclose(fp);

	// [ ###잭팟### ]
	g_JackPotMoney = Cfg4.nJackPotMoney;
	// ### [로그 기록용] ###
	g_LogRangeValue = (INT64)Cfg4.Range * M_1Y;
}

// ### [이벤트 티켓] ###
void C62CutPokerServerView::LoadConfigEvent()
{
	// 환경설정 파일을 연다
	ZeroMemory(&EventCfg, sizeof(CONFIGEVENT));

	FILE *fp = fopen( "event.cfg", "rb");
	if( fp == NULL ) { 
		//초기값
		SYSTEMTIME st= {0,};
		st.wYear = 2003;
		st.wMonth = 1;
		st.wDay = 1;
		if(EventCfg.StartDate == 0) EventCfg.StartDate = st;
		if(EventCfg.EndDate == 0)	EventCfg.EndDate = st;
		AfxMessageBox("이벤트 설정 파일을 읽을 수 없습니다.\n다시 설정하세요.");
		return;
	}
	fread(&EventCfg, sizeof(CONFIGEVENT), 1, fp);
	fclose(fp);

	
}

BOOL C62CutPokerServerView::FindID(RESULTFINDID *pRF, char *id)
{
	if(!pRF || !id) return FALSE;
	if(strlen(id) >= 16) return FALSE;

	for(int i=1; i<MAX_USER; i++) {
		if(User[i].bValid) {
			if(faststrcmp(User[i].UI.ID, id) == 0) {
				strncpy(pRF->ID, User[i].UI.ID, 15);
				pRF->ChNum = User[i].ChNum;
				pRF->Position = User[i].SUI.Position;
				pRF->RoomNum = User[i].SUI.RoomNum;
				return TRUE;
			}
		}
	}
	return FALSE;
}

void C62CutPokerServerView::SendMsgToAll(char *pMsgData, int nMsgSize)
{
	for(int i=0; i<MAX_DATA_SOCKET; i++) {
		if(SockMan.pDataSock[i] != NULL && SockMan.pDataSock[i]->m_bCreated) {
			USERVAL *pTempUserVal = (USERVAL*)(SockMan.pDataSock[i]->m_pUserPtr);
			if(pTempUserVal && pTempUserVal->iUser1 == TRUE) {
				SockMan.SendData(i, pMsgData, nMsgSize);
			}
		}
	}
}

void C62CutPokerServerView::OnViewConfig() 
{
	// TODO: Add your command handler code here
	CConfigDlg Dlg;
	Dlg.DoModal();
}

void C62CutPokerServerView::OnViewConfig2() 
{
	// TODO: Add your command handler code here
	CConfig2Dlg Dlg;
	Dlg.DoModal();
}

void C62CutPokerServerView::OnViewConfig3() 
{
	// TODO: Add your command handler code here
	CConfig3Dlg Dlg;
	Dlg.DoModal();
}

void C62CutPokerServerView::OnViewConfig4() 
{
	// TODO: Add your command handler code here
	CConfigJackPotDlg Dlg;
	Dlg.DoModal();
	
}

LONG C62CutPokerServerView::OnServAccept(UINT wParam, LONG pSock)
{
	int ErrorCode = wParam;
	if(ErrorCode != 0) {
		TRACE("접속 요청이 들어왔으나 연결 실패했음\n");
		return 1;
	}

	int sid = SockMan.AcceptSocket();
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	if( sid < 0 || pSocket==NULL) {
		TRACE("접속 요청이 들어왔으나 연결 실패했음\n");
		return 1;
	}
	else
	{
		// 사용자 접속 등록 처리
		CString str;
		TotUser++;
		str.Format("%d명(%d)",TotUser,RealUser);
		((CWnd*)GetDlgItem(IDC_STATIC_TOTALUSER))->SetWindowText(str);

		// [서버 에이전트 작업] 에이젼트와 연결시
		// 접속 거부 상태라면 거부 메시지를 보낸후 접속을 종료
		if(bConnectDeny && g_nAgentIndex != -1) { // 에이젼트와 연결하기 위하여 임시로 접속을 허용한다.
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, "서버 정비로 인하여 지금은 접속할 수 없습니다.\n잠시후 접속하시거나 다른 서버를 이용해주세요.");
			pSocket->Send(smsg.pData, smsg.GetTotalSize());
			pSocket->ReservClose();
			return 1;
		}

		// 접속자 제한 인원 초과
		if(RealUser >= Cfg2.MaxUser) {
			CSV_SERVERMSG smsg;
			smsg.Set(1, "최대 수용인원을 초과하였습니다. 다른서버를 이용해주세요.");
			pSocket->Send(smsg.pData, smsg.GetTotalSize());
			pSocket->ReservClose();
			return 1;
		}

		// 버전 체크 정보 보냄
		CSV_CHECKVERSION cvmsg(SndBuf);
		cvmsg.Set(VERSION, VERSION+5);
		SockMan.SendData(sid, cvmsg.pData, cvmsg.GetTotalSize());
	}
	return 1;
}

int C62CutPokerServerView::CheckPacket(CMySocket* pSock, char *lpBuf, int *pSignal)
{
	int msglen;
	int bufsize = pSock->m_RecvQue.GetTotalSize();

	if(bufsize>=PACKET_HEADERSIZE) // 헤더에 필요한 사이즈를 받았으면
	{
		pSock->m_RecvQue.PeekQue(lpBuf, PACKET_HEADERSIZE);
			
		// 올바른 헤더인지 체크하고 헤더 정보를 읽음
		if(TmpMsg.CheckInfo(lpBuf, pSignal, &msglen)==TRUE) {
			int totsize = PACKET_HEADERSIZE + msglen;

			// 요구되는 사이즈가 버퍼보다 큰 경우 접속을 끊어버림
			if(totsize<0 || totsize > RECVQUESIZE) {
				TRACE("요구되는 패킷의 사이즈가 너무 크거나 0보다 작아서 접속을 끊음\n");
				return -1;
			}

			if(bufsize >= totsize) {
				// 소켓 큐에서 데이터 카피와 동시에 삭제
				if(pSock->m_RecvQue.DeQue(MsgBuf, totsize)==FALSE) {
					TRACE("패킷 DeQue 실패\n");
					return -1;
				}

				// 복호화가 실패하면 접속을 끊음
				if(!TmpMsg.DecodeAll(lpBuf)) 
					return -1;
				
				return totsize;
			}
		}
		else { 
			TRACE("잘못된 리시브 데이터가 발생하여 소켓을 닫음\n");
			return -1;
		}
	}

	return 0;
}

BOOL C62CutPokerServerView::ProcessPacket(int sid, CMySocket *pSocket, char* lpdata, int totsize, int signal)
{
	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);
	if(pUserVal==NULL) {
		TRACE("ProcessPacket() - pUserVal == NULL\n"); 
		return FALSE;
	}

	//(추가)
	// 패킷 카운터를 검사하여 잘못된 경우 접속을 종료(패킷 스니핑 방지용)
	pUserVal->RecvPackCnt++;
	if(pUserVal->RecvPackCnt != ((PACKETHEADER*)lpdata)->PackCnt) {
	//	pSocket->PostClose();
		pSocket->ReservClose();
		return TRUE;
	}

	// 타임 아웃 카운트 초기화
	pUserVal->iTimeOutCnt = 0;

	switch(signal) // 메세지 신호
	{

	case SV_HEARTBEAT:
		{

		} break;
	
	case SV_ASK_LOGIN:  // 로그인 요청
		{
			CSV_ASK_LOGIN MsgData;
			MsgData.Get(lpdata, totsize);
			
			// 사용자 ID 인증 검사
			// MsgData.Ver   : 프로그램 버젼
			// MsgData.ID    : 사용자 아이디
			// MsgData.Pass  : 패스 워드
			// MsgData.RandomID  : 홈페이지 랜덤 코드
			// MsgData.IP    : 사용자의 IP주소
			// MsgData.Port  : 사용자의 포트번호

/*
			// [관리자 모드 작업] : 접속 중지중인 사용자이다
			UINT nDenySec = 0;
			if(IsAccessDeny(MsgData.ID,nDenySec)){
				CSV_SERVERMSG smsg(SndBuf);
				CString exitbuf;
				exitbuf.Format("%s 아이디는 해당서버에 접속중지 중입니다![잔여시간:%d초]",MsgData.ID,nDenySec);
				smsg.Set(1,(char*)exitbuf.operator LPCTSTR());
				pSocket->Send(smsg.pData, smsg.GetTotalSize());
				SockMan.ReservClose(sid); // 연결 종료 예약
				break;
			}
*/			
			// 사용자 ID 인증 검사
			// MsgData.pAL->ID				: 사용자 아이디
			// MsgData.pAL->Pass			: 패스 워드
			// MsgData.pAL->AuthCookieLen	: 인증 쿠키의 길이
			// MsgData.pAL->DataCookieLen	: 데이터 쿠키의 길이
			// MsgData.AuthCookie			: 인증 쿠키(주의! NULL로 끝나지 않는다)
			// MsgData.DataCookie			: 데이터 쿠키(주의! NULL로 끝나지 않는다)

			/* - jeong
			// [서버 에이전트 작업] 
			if(g_nAgentIndex == -1 && strcmp(MsgData.pAL->ID,CONNECT_ID)!=0)
			{
				CSV_SERVERMSG smsg(SndBuf);
				smsg.Set(1, "서버 정비로 인하여 지금은 접속할 수 없습니다.\n잠시후 접속하시거나 다른 서버를 이용해주세요..... ");
				pSocket->Send(smsg.pData, smsg.GetTotalSize());
				SockMan.ReservClose(sid); // 연결 종료 예약
				ShowState(_T("에이전트 종료로 인하여 사용자의 접속을 거부합니다." ));
				break;
			}
			*/
			
			// 접속자 제한 인원 초과
			if(RealUser >= Cfg2.MaxUser || RealUser >= MAX_USER-1) {
				CSV_REFUSE_LOGIN rlmsg(SndBuf); // 로그인 거부 메세지
				rlmsg.Set(3, "접속량이 많아 서버에 접속할 수 없습니다.\n잠시후 재접속 하시거나 다른 채널을 이용하세요.");	// [SSO 작업]
				pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
				SockMan.ReservClose(sid); // 연결 종료 예약
				break;
			}


			////////////////////////////////////////////////////////////////////////////////////////
			// [서버 에이전트 작업] : 접속을 허락한다!!   //에이전트 제거 - jeong
			//if(strcmp(MsgData.pAL->ID,CONNECT_ID)==0 && strcmp(MsgData.pAL->Pass,CONNECT_PASS)==0)
			//{

				// 비어있는 사용자 번호 검색
				int unum = -1;
				for(int i=1; i<MAX_USER; i++) if(User[i].bValid==FALSE) { unum=i; break;}

				// 사용자 수용인원 초과
				if(unum == -1) {
					CSV_REFUSE_LOGIN rlmsg(SndBuf); // 로그인 거부 메세지
					rlmsg.Set(3, "접속량이 많아 서버에 접속할 수 없습니다.\n잠시후 재접속 하시거나 다른 채널을 이용하세요.");	// [SSO 작업];
					pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
					SockMan.ReservClose(sid); // 연결 종료 예약
					break;
				}

				// 사용중인 아이디인지 검색
				BOOL bConnected=FALSE;
				for( i=0; i<MAX_USER; i++){
					if(strcmp(User[i].UI.ID, MsgData.pAL->ID) == 0){
						bConnected = TRUE;
						break;
					}
				}
				// 이미 사용중인 아이디
				if(bConnected){
					CSV_REFUSE_LOGIN rlmsg(SndBuf); // 로그인 거부 메세지
					rlmsg.Set(2, "이미 사용중인 아이디입니다.\n관리자에게 문의하세요");	// [SSO 작업]
					pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
			
					SockMan.ReservClose(sid); // 연결 종료 예약
					break;
				}
				
				// 접속 처리 하기
				pUserVal->iUser1 = TRUE; // 올바른 유저가 접속한 소켓임을 나타낸다.
				pUserVal->iUser2 = unum; // 사용자 접속 번호(소켓 클래스에 저장)
				pUserVal->iUser3 = (int)timeGetTime();// 소켓의 사용자 변수에 Time Tick을 기록(정확성 판별 및 타임 체크용)

				USERINFO ui;
				memset(&ui,0,sizeof(USERINFO));
				ui.UNum = unum;	               // 유저 번호
				strncpy(ui.ID,CONNECT_ID,15);  // 접속아이디
//				memset(ui.Pass, 0, sizeof(ui.Pass));// 비밀번호 필드 초기화
				ui.PMoney =  (INT64)1000000;

				DBWORKS Work1;
				ZeroMemory(&Work1, sizeof(DBWORKS));
				//User[unum].SetNewUser(&ui, sid);
				
				// ### [이벤트 티켓] ###
				User[unum].SetNewUser(&ui, &Work1.Item, sid, &Work1.EventPrize);		// [게임 아이템 작업]
				
				g_nAgentIndex = unum; // 특별한 접속자이다(Server Agent)!!!!

				CNM_AGENT_CONNECT nmsg(SndBuf);
				nmsg.Set(RealUser, g_JackPotMoney);
				pSocket->Send(nmsg.pData, nmsg.GetTotalSize());

				if( Cfg3.bExamServer )
					GetDlgItem(IDC_EDIT_AGENTCONNECT)->SetWindowText(_T("연습서버 ▶ Agent Connect!"));
				else
					GetDlgItem(IDC_EDIT_AGENTCONNECT)->SetWindowText(_T("  ▶ Agent Connect!"));

				/////////////////////////////////////////////////
				// [서버 에이전트 작업] 에이젼트와 연결시
				CMainFrame *pMain = (CMainFrame*) GetParentFrame();
				g_pMainView->bConnectDeny = TRUE;
				pMain->Connectdeny();
				ShowState(_T("사용자의 접속을 허가합니다." ));
				/////////////////////////////////////////////////

				// 일단 400명의 사용자를 보낸다
				const int nIncNum = 400;
				int totnum=0;
				AGENT_USER au[nIncNum];
				memset(au,0,sizeof(AGENT_USER)*nIncNum);

				for(i=0; i<nIncNum; i++){
					if(User[i].bValid) {
						au[totnum].nUNum  = User[i].UI.UNum;
						au[totnum].PMoney = User[i].OI.pBackup;
						strncpy(au[totnum].ID,User[i].UI.ID,15); // 아이디
						totnum++;
					}
				}
				if(totnum>0){
					// 사용자 목록을 보내줌
					CNM_AGENT_USERLIST_R1 r1msg(SndBuf);
					r1msg.Set(totnum, au, 0); // ***** 순서 *****
					pSocket->Send(r1msg.pData, r1msg.GetTotalSize());

				}

				// 인원수 증가 시켜야 되지 않을까?????????
				RealUser++;
				//break;				//에이전트 제거 - jeong
			//}

			// [SSO 작업] - 블럭 전체 수정(GetTotNum과 로그인 거부 메시지가 바뀌었음)
			// DB처리량 과다로 접속 불가
			if(DBMan.DBQue.GetTotNum() >= Cfg2.LoginDBQueNum) {
				CSV_REFUSE_LOGIN rlmsg(SndBuf); // 로그인 거부 메세지
				rlmsg.Set(3, "접속량이 많아 서버에 접속할 수 없습니다.\n잠시후 재접속 하시거나 다른 채널을 이용하세요.");	// [SSO 작업]
				pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
				SockMan.ReservClose(sid); // 연결 종료 예약
				break;
			}

			// 소켓의 유니크를 구분하기위한 Time Tick
			int StartTick = timeGetTime();
			
			// 소켓의 사용자 변수에 Time Tick을 기록(정확성 판별 및 타임 체크용)
			pUserVal->iUser3 = StartTick;
			
			// 연결된 소켓으로부터 사용자의 IP를 얻어냄			[PC방 작업]
			SOCKADDR_IN peername = {0,};
			int namelen = sizeof(SOCKADDR_IN);
			::getpeername(pSocket->m_hSocket, (SOCKADDR*)&peername, &namelen);
			
			DBWORKS Work;
			ZeroMemory(&Work, sizeof(Work));

			Work.bValid = TRUE;									// 유효 플래그
			Work.WorkKind = 1;									// 일의 종류
			Work.SID = sid;										// 소켓 SID
			Work.StartTick = StartTick;							// DB 처리 시작 시간
			Work.UniqueTick = StartTick;						// 소켓과 동일한 Time Tick
			strcpy(Work.UserIP, inet_ntoa(peername.sin_addr));	// 사용자의 IP
			//2004.05.07 리더스총액 제한
			Work.bForceLogin = MsgData.pAL->bForceLogin;// <-------------------추가
			
			
			if(strlen(MsgData.pAL->ID) > 0 && strlen(MsgData.pAL->Pass) > 0) {
				// 아이디와 패스워드를 받아서 편법으로 인증함
				strncpy(Work.UI.ID, MsgData.pAL->ID, 15);		// 유저 ID
				strncpy(Work.Passwd, MsgData.pAL->Pass, 15);	// 패스워드
			}
			else {
				CSV_REFUSE_LOGIN rlmsg(SndBuf);
				rlmsg.Set(3, "비정상적인 방법으로는 접속할 수 없습니다(3)");
				pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
				SockMan.ReservClose(sid); // 연결 종료 예약

				CLogFile LogFile;
				LogFile.Writef("비정상적인 방법으로 접속 시도(3) - %s", MsgData.pAL->ID);
				break;
			}


			/*
			DBWORKS Work;
			ZeroMemory(&Work, sizeof(Work));
			Work.bValid = TRUE;							// 유효 플래그
			Work.WorkKind = 1;							// 일의 종류
			Work.SID = sid;								// 소켓 SID
			Work.StartTick = StartTick;					// DB 처리 시작 시간
			Work.UniqueTick = StartTick;				// 소켓과 동일한 Time Tick
			strncpy(Work.UI.ID, MsgData.ID, 15);		// 유저 ID
			strncpy(Work.UI.Pass, MsgData.Pass, 10);	// 패스워드
			Work.bForceLogin = *MsgData.Over;			// 강제 로그인 플레그
		*/

			// DB 쓰레드 가동
			BOOL rtn = DBMan.SetDBWork(&Work);
			if(rtn == FALSE) {
				CSV_REFUSE_LOGIN rlmsg(SndBuf);
				rlmsg.Set(3, "최대 수용인원을 초과하였습니다.\n잠시후 다시 접속해주세요");	// [SSO 작업]
				pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
				SockMan.ReservClose(sid); // 연결 종료 예약
				break;
			}

			// 서버 정보 보내기	[게임 아이템 작업]
			{
				SERVERINFO si = {0,};
				si.GameCode = Cfg.GameCode;

				strncpy(si.ServerName, Cfg.ServerName, 19);
				strncpy(si.NoticeURL, Cfg2.NoticeURL, 63);
				strncpy(si.BannerURL1, Cfg2.BannerURL1, 63);
				strncpy(si.BannerURL2, Cfg2.BannerURL2, 63);
//				strncpy(si.ProfileURL, Cfg2.ProfileURL, 63);
				strncpy(si.LocationURL, Cfg2.LocationURL, 63);
				strncpy(si.ItemURL, Cfg2.ItemURL, 63);
				strncpy(si.ItemHelpURL, Cfg2.ItemHelpURL, 63);
				strncpy(si.CashURL, Cfg2.CashURL, 63);
				// 채널 로비 메세지
				strncpy(si.SrvInfoMsg, Cfg2.NoticeInfoMsg, 79);	//  서버메시지

				si.JackPot = g_JackPotMoney; // 로그인시 보낸다.

				CSV_SERVERINFO simsg(SndBuf);
				simsg.Set(&si);
				pSocket->Send(simsg.pData, simsg.GetTotalSize());
			}


		} break;

	case SV_ASK_LOGOUT: // 로그아웃 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_LOGOUT MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = *MsgData.UNum;

			// 에러검사
			if(unum < 0 || unum >= MAX_USER) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;
			if(strlen(MsgData.ID) == 0 || strlen(MsgData.ID) >= 16) break;

			// 채널 대기실이 아닌 곳에서 로그아웃 요청하면 의심스러운 사용자 이므로 접속 끊음
			if(User[unum].SUI.Position != WH_CHAN) {
				CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);
				if(pSocket==NULL || !pSocket->m_bCreated) break;
			//	pSocket->PostClose();
				pSocket->ReservClose();
				break;
			}

			if(!User[unum].bLogOutDBSaved) {
				// 게임 가감치에 변동사항이 있는 경우만 데이타베이스에 기록한다
				// 이부분에서 DB를 업데이트
				DBWORKS dbwork;
				User[unum].MakeDBWork(4, &dbwork);
				BOOL rtn = DBMan.SetDBWork(&dbwork);
				
				// DB 작업 큐가 꽉 찬 경우 로그 기록
				if(rtn == FALSE) {
					CLogFile LogFile;
					LogFile.Writef(_T("DB Work Que overflow - OnDestroy() : id=%s"), dbwork.UI.ID);
				}
				else User[unum].bLogOutDBSaved = TRUE;
			}

		
		} break;

	case SV_ASK_ALLCHANINFO: // 채널 정보 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			// 특별한 메세지가 없으므로 메세지 분석은 필요없다
			
			CHANNELINFO ci[MAX_CHAN];
			int totnum=0;
			for(int i=0; i<MAX_CHAN; i++) {
				ci[i] = Chan[i].GetChannelInfo();
				totnum++;
			}

			// 모든 채널 정보를 보내줌
			CSV_ALLCHANINFO acmsg(SndBuf);
			acmsg.Set(totnum, ci);
			SockMan.SendData(sid, acmsg.pData, acmsg.GetTotalSize());
		} break;

	case SV_ASK_ENTERCHAN: // 채널 입장 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_ENTERCHAN MsgData;
			MsgData.Get(lpdata, totsize);

			// 에러검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(User[unum].ChNum != -1) break;
			int cnum = *MsgData.ChNum;
			if(cnum < 0 || cnum >= MAX_CHAN) break;

			// 채널 입장 처리
			int nUseItem = 0;
			int code = Chan[cnum].OnNewUser(unum, nUseItem); // [점프 아이템# 작업] 채널용
			ENTERCHANINFO ec = {0,};
			/*
			// code :
			//         >= 0  정상 채널 입장;
			//		     -3  현재 등급상 점프 아이템 구매(다이얼로그 박스)	
			//           -4  점프아템 조차 해결않됨(2단계 이상 차이남)
			//			 -6  지금 사용않함(채널별 한도 금액)
			//			 -7  현재 등급에 입장 할 수 없는데 점프아이템은 있네(사용할것인가 물어?)
			//         <  0  ( -1  채널이 꽉찼음, ...)
			//
			// &nUseItem :
			//		      0  일반적 채널 입장
			//            1  이미 한번이라도 사용한 점프아이템을 가지고 입장
			*/

			if(code >= 0) {
				// 채널 입장을 허용	
				strncpy(ec.ChanName, Chan[cnum].strChanName, 19);
				ec.ChNum = cnum;
				ec.ChanStyle = Chan[cnum].ChannelStyle;
				ec.channel_usernum = Chan[cnum].m_channel_usernum; // 5, 7인용 작업
			} 
			else if(  code == -4) //  점프아템 사라고 알려준다 ,점프아템 조차 해결않됨(2단계 이상 차이남)
			{
				// #########################################################33
				// 테스트
				int level = User[unum].UI.nIcon;
				int style = Chan[cnum].ChannelStyle; // 채널 등급

						
				CSV_SERVERMSG smsg(SndBuf);
				CString str;

				char lv_buf0[50];
				ZeroMemory( lv_buf0, sizeof(char)*50 );
				char lv_buf1[50];
				ZeroMemory( lv_buf1, sizeof(char)*50 );
				char mylv[50];
				ZeroMemory( mylv, sizeof(char)*50 );

				char mygrade[50]; // 등급
				ZeroMemory( mygrade, sizeof(char)*50 );

				
				// 등급 사용시 추가
				int login1 = 0;
				int login2 = 0;
				if(style == 0 ) {		 // 자유
					login1 = 0;
					login2 = 14;
				} else if(style == 1 ) { // 초보
					login1 = 0;
					login2 = 3;
				} else if(style == 2 ) { // 중수
					login1 = 4;
					login2 = 7;
				} else if(style == 3 ) { // 고수
					login1 = 8;
					login2 = 11;
				} else if(style == 4 ) { // 도신	
					login1 = 12;
					login2 = 14;
				} 

				GetLevelName( login1, lv_buf1 ); // 레벨이름
				GetLevelName( level, mylv ); // 나의 레벨

				GetGradeName(level, mygrade); // 등급 구하기(초보등급,...)
				GetGradeName( login1, lv_buf0 ); // 등급이름

								
				str.Format( "[%s]회원은 [%s]레벨로, [%s] 입니다.\n선택하신 채널은 [%s(%s)]의 사용자가 입장할 수 있습니다. \n[%s] 채널을 이용해 주세요.", 
					User[unum].UI.ID, mylv, mygrade, lv_buf0, lv_buf1, mygrade );

				smsg.Set(4  , (char*)str.operator LPCTSTR() );
				

				////////////////////////////////////////////////////////////
				// 버튼 활성화 위해 (채널 선택창)
				CSV_ACCEPT_ENTERCHAN aemsg(SndBuf);
				ec.ChNum = -10;
				aemsg.Set(&ec);
				SockMan.SendData(sid, aemsg.pData, aemsg.GetTotalSize());
				////////////////////////////////////////////////////////////


				SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
				break;
				// #########################################################33
				
			}
			else {
				// 채널 입장을 거부
				ZeroMemory(&ec, sizeof(ec));
				ec.ChNum = code;
			}

			CSV_ACCEPT_ENTERCHAN aemsg(SndBuf);
			aemsg.Set(&ec);
			SockMan.SendData(sid, aemsg.pData, aemsg.GetTotalSize());

			// ### [이벤트 티켓] ###
			if(code >= 0) {

				SendChannelEventNotice(unum);
				//2004.05.07 리더스총액 제한
			//	SendPremLeadersMsg(unum, User[unum].UI.PremLeadersCode);
			
			}

		} break;

	case SV_ASK_OUTCHAN: // 채널 퇴장 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			// 특별한 메세지가 없으므로 메세지 분석은 필요없다

			// 에러검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			int cnum = User[unum].ChNum;
			if(cnum == -1) break;
			
			// 채널 퇴장 처리
			Chan[cnum].OnOutUser(unum);

			// 채널 퇴장을 허용
			CSV_ACCEPT_OUTCHAN aomsg(SndBuf);
			aomsg.Set();
			SockMan.SendData(sid, aomsg.pData, aomsg.GetTotalSize());
		} break;

	case SV_ASK_FINDID: // 아이디 찾기 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_FINDID MsgData;
			MsgData.Get(lpdata, totsize);

			RESULTFINDID rf;
			ZeroMemory(&rf, sizeof(rf));
			FindID(&rf, MsgData.ID);

			// 아이디 찾기 결과를 알려줌
			CSV_RESULT_FINDID rfmsg(SndBuf);
			rfmsg.Set(&rf);
			SockMan.SendData(sid, rfmsg.pData, rfmsg.GetTotalSize());
		} break;

	case SV_ASK_DETAILUSERINFO: // 특정 사용자의 상세 정보 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_DETAILUSERINFO MsgData;
			MsgData.Get(lpdata, totsize);

			USERINFO *pUI = NULL;
			USERINFO TempUI;
			ZeroMemory(&TempUI, sizeof(USERINFO));

			int unum = *MsgData.UNum;

			// 잘못된 아이디일경우 빈 사용자 정보를 보내줌
			if(unum < 0 || unum>=MAX_USER) pUI = &TempUI;
			else if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) pUI = &TempUI;
			else pUI = &User[unum].UI;

			CSV_DETAILUSERINFO duimsg(SndBuf);
			duimsg.Set(pUI);
			SockMan.SendData(sid, duimsg.pData, duimsg.GetTotalSize());
		} break;

	case SV_ASK_USERINFO: // 특정 사용자 정보 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_USERINFO MsgData;
			MsgData.Get(lpdata, totsize);

			USERINFO *pUI = NULL;
			USERINFO TempUI;
			ZeroMemory(&TempUI, sizeof(USERINFO));

			int unum = *MsgData.UNum;

			// 잘못된 아이디일경우 빈 사용자 정보를 보내줌
			if(unum<1 || unum>=MAX_USER) pUI = &TempUI;
			else if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) pUI = &TempUI;
			else pUI = &User[unum].UI;

			CSV_USERINFO uimsg(SndBuf);
			uimsg.Set(pUI);
			SockMan.SendData(sid, uimsg.pData, uimsg.GetTotalSize());
		} break;

	case SV_ASK_MONEYINFO: // 돈 정보 업데이트 요청 - jeong
		{
			
			CSV_ASK_MONEYINFO MsgData;
			MsgData.Get(lpdata, totsize);
			
			MONEYINFO *pUI = NULL;
			MONEYINFO TempUI;
			ZeroMemory(&TempUI, sizeof(MONEYINFO));
			
			int unum = *MsgData.UNum;
			int uMoney = *MsgData.UMoney;
			int uRoomIndex = *MsgData.UPlus;
			int uBank = *MsgData.UBank;

			User[unum].UI.PMoney += uMoney;
	
			int uChanIndex = User[unum].ChNum;
			int nPNum = User[unum].PNum;

			Chan[uChanIndex].Room[uRoomIndex].m_Raise.m_User[nPNum].nPMoney += uMoney;

			CSV_MONEYINFO uimsg(SndBuf);
			uimsg.Set(User[unum].UI.PMoney);
			SockMan.SendData(sid, uimsg.pData, uimsg.GetTotalSize());
		

		} break;

	case SV_ASK_BANKINFO: // 은행 정보 업데이트 요청 - jeong
		{
			
			CSV_ASK_BANKINFO MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = *MsgData.UNum;
			int uBank = *MsgData.UBank;
			
			User[unum].nBMoney = uBank;
			
			/*
			CSV_MONEYINFO uimsg(SndBuf);
			uimsg.Set(User[unum].UI.PMoney);
			SockMan.SendData(sid, uimsg.pData, uimsg.GetTotalSize());
			*/
			
		} break;

	case SV_ASK_CHANGECHAR: // 캐릭터 바꾸기를 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_CHANGECHAR MsgData;
			MsgData.Get(lpdata, totsize);
			int unum = *MsgData.UNum;
			int charnum = *MsgData.CharNum;
			if(unum<1 || unum>=MAX_USER) break;
			if(faststrcmp(MsgData.ID, User[unum].UI.ID) != 0) break;
			if(charnum < 0 || charnum > 72) break;
			//User[unum].UI.Character = charnum;

			CSV_ACCEPT_CHANGECHAR accmsg(SndBuf);
			accmsg.Set(charnum);
			SockMan.SendData(sid, accmsg.pData, accmsg.GetTotalSize());
		} break;

	case SV_NOWHISPER: // 귓말 허용 상태 변경
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_NOWHISPER MsgData;
			MsgData.Get(lpdata, totsize);

			// 에러검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;

			User[unum].bNoWhisper = *MsgData.bNoWhisper;
		} break;

	// [세븐포커] 추가루틴
	case SV_CHANGECARD: // 카드 바꾸기를 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_CHANGECARD MsgData;
			MsgData.Get(lpdata, totsize);
		
			// 에러검사
			int unum     = pUserVal->iUser2;
			int nCardNum = MsgData.pCC->nCard;
			if(unum < 1 || unum >= MAX_USER) break;
			if(unum != MsgData.pCC->nUNum) break;
			if(User[unum].bValid == FALSE) break;
			if(nCardNum < 0 || nCardNum >= 16) break;

			BOOL bMyCard = FALSE;
			if(nCardNum<4){
				bMyCard = TRUE;
			}
			for(int i=4;i<16;i++){
				if(nCardNum==i && User[unum].UI.PI.Card[i-4]==1){
					bMyCard = TRUE;
				}
			}
			if(bMyCard) User[unum].UI.PI.nCard = (char)nCardNum;

		} break;

	case SV_BUYCARD: // 카드 사기를 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_BUYCARD MsgData;
			MsgData.Get(lpdata, totsize);

			// 에러검사
			int unum     = pUserVal->iUser2;
			int nCardNum = MsgData.pBC->nCard;
			if(unum < 1 || unum >= MAX_USER) break;
			if(unum!=MsgData.pBC->nUNum) break;
			if(User[unum].bValid == FALSE) break;
			if(nCardNum < 0 || nCardNum >= 12) break;

			INT64 nPrice = 0;
			switch(nCardNum){
				case 0: nPrice = M_500;  break; // 5백만원
				case 1: nPrice = M_500;  break; // 5백만원
				case 2: nPrice = M_3000; break; // 3천만원
				case 3: nPrice = M_3000; break; // 3천만원

				case 4: nPrice = M_5000; break; // 5천만원
				case 5: nPrice = M_5000; break; // 5천만원
				case 6: nPrice = M_1Y;   break; // 1억원
				case 7: nPrice = M_1Y;   break; // 1억원

				case 8:   nPrice = M_5Y;  break; // 5억원
				case 9:   nPrice = M_5Y;  break; // 5억원
				case 10:  nPrice = M_10Y; break; // 10억원
				case 11:  nPrice = M_10Y; break; // 10억원
				default:  nPrice = M_10Y; break; // 10억원
			}

			if(User[unum].UI.PMoney<nPrice) break;
			if(User[unum].UI.nIcon<4 && nCardNum>3) break; // 바로 위 레벨만 사기 가능
			if(User[unum].UI.nIcon<8 && nCardNum>7) break;

			User[unum].UI.PMoney-=nPrice;
			User[unum].UI.PI.Card[nCardNum] = (char)1;


		} break;

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	// 서버 잭팟 평균
	case NM_AGENT_AVERAGE:
		{
			CNM_AGENT_AVERAGE MsgData;
			MsgData.Get(lpdata, totsize);

			if( *MsgData.JackPotMoney > 0)
			{
				g_JackPotMoney = *MsgData.JackPotMoney;
			}

		} break;

	// 관리자 정보 
	case NM_AGENT_ADMININFO:
		{
			CNM_AGENT_ADMININFO MsgData;
			MsgData.Get(lpdata, totsize);

			int total = *MsgData.TotNum;
			if(total < 0 || total >= MAX_ADMIN_NUM) break;
			ZeroMemory(&g_AdminInfo, sizeof(g_AdminInfo));

			g_AdminInfo.nTotal = total;
			memcpy(g_AdminInfo.admin, MsgData.AdminInfo, sizeof(ADMIN_INFO)*total);
			
			CString str;
			str.Format("Agent로 부터 관리자 아이디 %d개 전송받음", total);
			ShowState(str);
		
		} break;
    
	case NM_AGENT_USERLIST: // [서버 에이전트 작업] : 사용자 리스트를 요청한다
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CNM_AGENT_USERLIST MsgData;
			MsgData.Get(lpdata, totsize);
			
			int nUserVal = *MsgData.Reserve;
			
			if(g_nAgentIndex < 1 || g_nAgentIndex >= MAX_USER) break;
			if(User[g_nAgentIndex].Sid!=sid) break;

			const int nIncNum = 400; // 400개씩 묶어 보낸다!!

			int nStart = nIncNum*nUserVal; // 400 ~ (1 부터)
			int nEnd   = nStart+nIncNum;
			if(nStart<0 || nStart>=MAX_USER || nEnd<0) break;
			if(nEnd>MAX_USER) nEnd = MAX_USER;

			int totnum=0;
			AGENT_USER au[nIncNum];
			memset(au,0,sizeof(AGENT_USER)*nIncNum);

			for(int i=nStart; i<nEnd; i++){
				if(User[i].bValid) {
					au[totnum].nUNum  = User[i].UI.UNum;
					au[totnum].PMoney = User[i].OI.pBackup;
					strncpy(au[totnum].ID,User[i].UI.ID,15); // 아이디
					totnum++;
				}
			}

			if(totnum > 0) {

				CNM_AGENT_USERLIST_R1 r1msg(SndBuf);
				r1msg.Set(totnum, au, nUserVal); // 순서
				pSocket->Send(r1msg.pData, r1msg.GetTotalSize());
			}

		} break;

	// 중복 접속자 이다
	case NM_AGENT_NEWUSERERROR:
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CNM_AGENT_NEWUSERERROR MsgData;
			MsgData.Get(lpdata, totsize);

			AGENT_USER au; ZeroMemory(&au, sizeof(AGENT_USER));
			memcpy(&au, MsgData.ArraySUI, sizeof(AGENT_USER));

			int totNum = *MsgData.TotNum;

			// 에러검사
			int unum     = au.nUNum;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE)   break;

			if(strncmp(User[unum].UI.ID,au.ID,15)!=0) break;

			int usersid = User[unum].Sid;
			CMySocket *pSocket = SockMan.GetDataSocketPtr(usersid);

			if(pSocket==NULL) break;
			if(!pSocket->m_bCreated) break;
			USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);
			if(pUserVal==NULL) break;
			if(usersid != (int)pSocket->m_dwUserVal) break;
			
			//////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////
			if(totNum == 5)
			{// 대기실이라면 강제 종료(이미 접속해 있는 아이디 입니다)
				if(User[unum].SUI.Position == WH_CHAN || User[unum].SUI.Position == WH_LOBY || User[unum].SUI.Position == WH_GAME)
				{
					CSV_SERVERMSG smsg(SndBuf);
					CString strMess = "";
					strMess.Format("[%s] [%s]", Cfg.ServerName, User[unum].UI.ID);
					smsg.Set(0, (char*)strMess.operator LPCTSTR()); 
					SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
					
					smsg.Set(1, "현재 다른 게임서버에서 접속시도중입니다........");
					pSocket->Send(smsg.pData, smsg.GetTotalSize());
					SockMan.ReservClose(usersid); // 연결 종료 예약
				}
				else
				{
					CSV_SERVERMSG smsg(SndBuf);
					CString strMess = "";
					strMess.Format("[%s] [%s]", Cfg.ServerName, User[unum].UI.ID);
					smsg.Set(1, (char*)strMess.operator LPCTSTR()); 
					SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());

					strMess.Format("[%s]님이 현재 다른 게임서버에서 접속을 시도중입니다...", User[unum].UI.ID);
					smsg.Set(0, (char*)strMess.operator LPCTSTR()); 
					pSocket->Send(smsg.pData, smsg.GetTotalSize());
					// 연결을 끊지는 않는다
					SockMan.ReservClose(usersid); // 연결 종료 예약
				}
				break;
			}
			else if(totNum == 6)
			{					
				CSV_REFUSE_LOGIN rlmsg(SndBuf); // 로그인 거부 메세지
				rlmsg.Set(1, "현재 접속 종료 처리중입니다. 잠시후 다시 접속해 주십시요.");
				pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
				// 연결을 끊지는 않는다
				SockMan.ReservClose(usersid); // 연결 종료 예약
				break;
			}
			//////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////

			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, "접속되어있는 아이디가 존재합니다.확인후 다시 접속해 주십시요.");
			pSocket->Send(smsg.pData, smsg.GetTotalSize());
			SockMan.ReservClose(usersid); // 연결 종료 예약

		} break;

	/////////////////////////////////// [게임 아이템 작업] ////////////////////////////////////////

	case SV_ASK_GAMEITEMLIST: // 게임 아이템 목록을 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_GAMEITEMLIST MsgData;
			MsgData.Get(lpdata, totsize);

			// 아무런 메시지가 엄따

			// 에러검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;

			DBWORKS Work;
			User[unum].MakeDBWork(9, &Work);
			//Work.UniqueTick = StartTick;				// 소켓과 동일한 Time Tick

			// DB 쓰레드 가동
			BOOL rtn = DBMan.SetDBWork(&Work);

			if(rtn == FALSE) {
				// 쓰레드 가동에 실패하면 즉시 에러코드와 함께 아이템 리스트를 보냄
				
				// 게임 아이템 리스트 정보
				GAMEITEM_INFO in = {0,};
				in.Kind = 100;		// 아이템 리스트 보내기
				in.ErrorCode = 1;	// 아이템 쿼리 실패 에러코드
				in.TotNum = 0;
				in.StrMsgLen1 = 0;
				in.StrMsgLen2 = 0;
				CTime time = CTime::GetCurrentTime();
				time.GetAsSystemTime(in.ServerTime);

				// 아이템 리스트를 보내줌
				CSV_GAMEITEMLIST glmsg;
				glmsg.Set(&in, User[unum].GameItem.Item, NULL, NULL);
				User[unum].SendMsg(&glmsg);

				break;
			}

		} break;

	case SV_USEGAMEITEM: // 게임 아이템 사용 요청
		{
			// 유효한 유저가 아니면 처리하지 않음
			if(pUserVal->iUser1 == FALSE) break;

			CSV_USEGAMEITEM MsgData;
			MsgData.Get(lpdata, totsize);

			// 에러검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;

			int index = MsgData.pUG->Item.Index;
			
			if(index < 0 || index >= User[unum].GameItem.nNum) {
				//
				// 인덱스 초과로 잘못된 번호임
				//

				// 게임 아이템 사용 결과
				USEGAMEITEMINFO ug = {0,};
				strncpy(ug.ID, User[unum].UI.ID, 15);
				ug.bUseError = TRUE;
				ug.Item = User[unum].GameItem.Item[index];

				// 게임 아이템 사용 결과를 보냄
				CSV_USEGAMEITEM ugmsg;
				ugmsg.Set(&ug);
				User[unum].SendMsg(&ugmsg);

				break;
			}

			// 아이템의 상태를 얻어옴
			ItemState state = GameItem_GetItemState(User[unum].GameItem.Item[index]);
			if(state.iState != 1) {
				//
				// 사용 가능한 아이템이 아니면 사용 불가
				//

				// 게임 아이템 사용 결과
				USEGAMEITEMINFO ug = {0,};
				strncpy(ug.ID, User[unum].UI.ID, 15);
				ug.bUseError = TRUE;
				ug.Item = User[unum].GameItem.Item[index];
				ug.bMessage = TRUE;
				strncpy(ug.StrMsg, "사용할 수 없는 아이템입니다.", sizeof(ug.StrMsg)-1);

				// 게임 아이템 사용 결과를 보냄
				CSV_USEGAMEITEM ugmsg;
				ugmsg.Set(&ug);
				User[unum].SendMsg(&ugmsg);

				break;
			}

			BOOL bExist = FALSE;
			// ### [ 올인극복 아이템 ] ###
			// 슈퍼방, 슈퍼한방 아이템 둘 중에 하나만 사용가능(중복 불가)
			BOOL bUseing = FALSE;
			BOOL bInVincible = FALSE;
			int curCode = User[unum].GameItem.Item[index].Code;
			if(curCode >= ITEM_SCODE_SUPERMASTER && curCode <= ITEM_ECODE_SUPERMASTER ||
				curCode >= ITEM_SCODE_SECRET && curCode <= ITEM_ECODE_SECRET  )
			{
				if(GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_INVINCIBLE, &bExist)) {
					bUseing = TRUE;
					bInVincible = TRUE;
				}
			}

			// [수호천사] 2004.07.08
			if(curCode >= ITEM_SCODE_SAFEGUARDANGEL && curCode <= ITEM_ECODE_SAFEGUARDANGEL) 
			{
				// 게임진행중일때는 불가
				if(User[unum].SUI.Position == WH_GAME && User[unum].PNum != -1) {
					
					int cnum = User[unum].ChNum;
					int rnum = User[unum].SUI.RoomNum;
					int pnum = User[unum].PNum;
					if(cnum >= 0 && cnum < MAX_CHAN && rnum>0 && rnum <MAX_ROOM && pnum >= 0 && pnum < Chan[cnum].Room[rnum].get_maxplayer()) {
						if( Chan[cnum].Room[rnum].bValid ) {
							if(faststrcmp(User[unum].UI.ID, Chan[cnum].Room[rnum].Ui[pnum].ID)==0) {
								if(Chan[cnum].Room[rnum].Ps[pnum].PlayState == 1) {
									CSV_SERVERMSG msg;
									msg.Set( 0, "게임진행 중에는 수호천사 아이템을 사용할 수 없습니다." );
									User[unum].SendMsg(&msg);
									break;
								}
							}
						}
					}
				}

				if(!bUseing && User[unum].Get_lost_money() <= 0) 
				{
					CSV_SERVERMSG msg;
					msg.Set( 0, "마지막 잃으신 금액이 없거나, 이미 충전되어서 충전할 금액이 없습니다." );
					User[unum].SendMsg(&msg);
					break;
				}
			}
			else if(curCode >= ITEM_SCODE_SUPERMASTER && curCode <= ITEM_ECODE_SUPERMASTER)
			{// 슈퍼 한방 검사
				if(GameItem_IsUsingItem(User[unum].GameItem, curCode, &bExist)) bUseing = TRUE;
				if(GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_ALLINSUPERMASTER, &bExist)) bUseing = TRUE;
			}
			else if(curCode >= ITEM_SCODE_ALLINSUPERMASTER && 
				curCode <= ITEM_ECODE_ALLINSUPERMASTER)
			{// 슈퍼방 검사
				if(GameItem_IsUsingItem(User[unum].GameItem, curCode, &bExist)) bUseing = TRUE;
				if(GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_SUPERMASTER, &bExist)) bUseing = TRUE;

			} 
			else if(GameItem_IsUsingItem(User[unum].GameItem, curCode, &bExist)) {
				bUseing = TRUE;
			}

			if(bUseing )
			{
				//
				// 이미 사용중인 같은 종류의 아이템이 있다면 사용 불가
				//

				// 게임 아이템 사용 결과
				USEGAMEITEMINFO ug = {0,};
				strncpy(ug.ID, User[unum].UI.ID, 15);
				ug.bUseError = TRUE;
				ug.Item = User[unum].GameItem.Item[index];
				ug.bMessage = TRUE;

				if( bInVincible )
					strncpy(ug.StrMsg, "천하무적 아이템이 이미 사용중입니다.  \r슈퍼방, 찬스(고스톱/고스톱2),비공개방(세븐포커/세븐포커2), \r1일5회 2배 무료충전의 복합적인기능을 가진 아이템 입니다.", sizeof(ug.StrMsg)-1);
				else
					strncpy(ug.StrMsg, "이미 사용중인 아이템이 존재합니다.", sizeof(ug.StrMsg)-1);

				// 게임 아이템 사용 결과를 보냄
				CSV_USEGAMEITEM ugmsg;
				ugmsg.Set(&ug);
				User[unum].SendMsg(&ugmsg);

				break;
			}

			DBWORKS dbwork;
			User[unum].MakeDBWork(10, &dbwork);	
			// 사용할 아이템 세팅
			dbwork.UseItem = User[unum].GameItem.Item[index];

			// DB 작업 큐가 꽉 찬 경우 로그 기록
			if(!DBMan.SetDBWork(&dbwork)) {
				CLogFile LogFile;
				LogFile.Writef("DB작업 큐 오버플로우 - CServerView::ProccessPacket() SV_USEGAMEITEM : id=%s", dbwork.UI.ID);
			}

		} break;

	////////////////////////////////////////////////////////////////////////////////////////

	default :
		{
			// 각 채널 메시지는 채널 클래스에서 처리한다
			if(pUserVal->iUser2 < 1 || pUserVal->iUser2>= MAX_USER) break;
			if(User[pUserVal->iUser2].ChNum == -1 || !User[pUserVal->iUser2].bValid) break;
			Chan[User[pUserVal->iUser2].ChNum].ProcessPacket(sid, pUserVal, lpdata, totsize, signal);
		} break;

	}

	return TRUE;
}


	
LONG C62CutPokerServerView::OnDbworkLogin(UINT wParam, LONG lParam)
{
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	// lParam에서 받은 포인터는 new로 할당한 포인터이므로 반드시 delete해야한다
	//DBWORKS *pwork = (DBWORKS*)lParam;
	//memcpy(&Work, pwork, sizeof(DBWORKS));
	//delete pwork;

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB 처리가 끝나기전에 접속이 끊어진 사용자일 수 있음
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	if(sid != (int)pSocket->m_dwUserVal) {
		//CLogFile LogFile;
		//LogFile.Write("sid 값이 틀림- C62CutPokerServerView::OnDbworkLogin()");
		return 1;
	}

	// 원래의 소켓 ID가 아니면 종료(이미 DB처리가 끝나기전에 소켓이 끊어진 사용자일 가능성이 높음)
	if(Work.UniqueTick != (UINT)pUserVal->iUser3) return 1;



	CSV_REFUSE_LOGIN rlmsg(SndBuf); // 로그인 거부 메세지

	// 사용중인 아이디인지 검색
	BOOL bConnected=FALSE;
	for(int i=0; i<MAX_USER; i++) {
		if(User[i].bValid) {
			if(faststrcmp(User[i].UI.ID, Work.UI.ID) == 0) { 
				bConnected = TRUE; 
				CSV_HEARTBEAT hbmsg(SndBuf); // 허트비트 메세지
				hbmsg.Set();
				User[i].SendMsg(&hbmsg);
				break;
			}
		}
	}

	// 이미 사용중인 아이디
	if(bConnected) {
		rlmsg.Set(2, "이전 접속이 올바르게 종료되지 않았거나 이미 사용중인 아이디입니다.\n다시 접속을 시도해보시고 관리자에게 문의하세요");	// [SSO 작업]
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}

	// 접속자 제한 인원 초과
	if(RealUser >= Cfg2.MaxUser) {
		rlmsg.Set(3, "최대 수용인원을 초과하였습니다.\n잠시후 다시 접속해주세요");	// [SSO 작업]
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}

	// 비어있는 사용자 번호 검색
	int unum = -1;
	for(i=1; i<MAX_USER; i++) if(!User[i].bValid) { unum=i; break;}

	// 사용자 수용인원 초과
	if(unum == -1) {
		rlmsg.Set(3, "최대 수용인원을 초과하였습니다.\n잠시후 다시 접속해주세요");	// [SSO 작업]
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}


	// DB처리 결과로 얻은 사용자 정보 구조체
	USERINFO ui;
	memcpy(&ui, &Work.UI, sizeof(USERINFO));


	// DB와 상관없이 플레이어의 돈은 무조건 0 - jeong
	ui.PMoney = 0;
	//User[0].UI.PMoney = 1000;
/*
	// [게임 아이템 작업]
	if(Work.IDState != 0) {
		if(Work.IDState == 1) {
			// 사용 정지중인 아이디라면
			rlmsg.Set(6);
			pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
			SockMan.ReservClose(sid);
			return 1;
		}
		else if(Work.IDState == 2) {
			// 삭제될 아이디라면
			rlmsg.Set(7);
			pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
			SockMan.ReservClose(sid);
			return 1;
		}
		else {
			// 나머지 모든 코드는 사용 정지중인 아이디를 나타냄
			rlmsg.Set(6);
			pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
			SockMan.ReservClose(sid);
			return 1;
		}
	}
*/
	//--------------------- [SSO 작업]

// 모든 회원은 끝자리가 0인 경우만 게임에 정상적인 접속이 가능하다.
	// 단, 준회원은 끝자리가 0이어도 접속 불가!
	//
	// 0 ~  : 임시회원 (0=정상, 1=사용정지, 2=영구정지, 7=(기존준회원)정상, 8=(기존준회원)사용정지, 9=(기존준회원)영구정지) 
	//    --> 14세 미만 아동은 처음 가입시 임시회원이 되며 부모의 동의를 받으면 준회원이 된다(임시회원인 경우는 게임 가능)
	// 10 ~ : 준회원 (10=정상, 11=사용정지, 12=영구정지) 
	//    --> 준회원은 정상이라고 하더라도 사용자 정보를 수정하기 전(정회원이 되기 전)까지는 무조건 접속 금지 대상이 된다
	// 20 ~ : 정회원 (20=정상, 21=사용정지, 22=영구정지)
	//    --> 14세 이상이거나 부모 동의를 받아서 정회원으로 승격한 회원
	//
	
	// idstate가 0,7,20 인 경우만 정상 접속이 가능함
	int idstate = Work.IDState;
	if(idstate != 0 && idstate != 7 && idstate != 20) {
		// 사용 정지중인 아이디임을 알려준다
		rlmsg.Set(6, "본 아이디는 현재 사용 정지 상태이므로\n서버에 접속할 수 없습니다.\n관리자에게 문의하세요");
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}

	// 유니크 넘버가 틀렸다면
	if(Work.bIsWrongUniqNo == TRUE) {
		rlmsg.Set(1, "올바르지 않은 인증값으로 접속을 시도하셨습니다\n관리자에게 문의하세요");
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}

	/*
	// 비밀번호가 틀렸다면
	if(Work.bIsWrongPwd == TRUE) {
		rlmsg.Set(1, "비밀번호가 일치하지 않습니다.\n관리자에게 문의하세요");	// [SSO 작업]
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}
	*/

	if(Work.bIsNotUserID == TRUE) {
		//아이디가 존재하지 않는다
		rlmsg.Set(0, "존재하지 않거나 잘못된 사용자 ID입니다.\n관리자에게 문의하세요");	// [SSO 작업]
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}

	// [성인인증]
	if( !Work.bCertificate )
	{
		rlmsg.Set(1, "성인 인증여부를 확인해 주십시요.");
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}


	// 다른 게임에 접속 중이고 강제접속 플래그가 꺼진상태라면
	if(Work.bAnotherConnect==TRUE && Work.bForceLogin==FALSE) {
		// 다른게임에 접속중이므로 로그인을 거부함(접속을 끊지는 않는다)
		rlmsg.Set(5, "다른게임에 접속 중입니다.");
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		return 1;
	}



	/////////////////////////////////////////////////////////////////////////////////	
	//2004.05.07 리더스총액 제한
	// 처음로긴이구 머니제한에 걸렸다면 => 다시 로긴요청
	if( CheckTotalmoneylimit(&ui, NULL, 0) && Work.bForceLogin==FALSE && StatisCfg.bTotalMoneyLimit == TRUE ) { 
		
		LEADERS_JOIN join = {0,};
		strncpy(join.ID, ui.ID, 15);
		join.PremLType		  = ui.PremLeadersCode;
		join.CurMoney		  = ui.PMoney;
		join.NorMoney         = StatisCfg.NormalMoney;
		join.LeadersMoney	  = StatisCfg.LeadersMoney;
		join.LeadersPlusMoney = StatisCfg.LeadersPlusMoney;

		strncpy(join.UniqNo,Work.UniqNo,15);

		strncpy(join.LeadersJoinURL, 	Cfg2.LeadersJoinURL, 63); // 리더스 가입홈페이지

		CSV_LEADERSJOIN ljmsg(SndBuf);
		ljmsg.Set(&join);
		pSocket->Send(ljmsg.pData, ljmsg.GetTotalSize());
		return 1;
	}

	/////////////////////////////////////////////////////////////////////////////////

	//######## [연습 서버] #########
	if(g_pMainView->Cfg3.bExamServer)
	{
		switch(g_ServerGrade)
		{
		case 0:
		case 1: //초보
			ui.PMoney =50000000000000; // 100억
			break;
		case 2: //중수
			ui.PMoney =1000000000000; // 10억
			break;
		case 3: //고수
			ui.PMoney =1000000000000; // 1000억
			break;
		case 4: //도신
			ui.PMoney =1000000000000000; // 1000조
			break;
		}

		ui.nIcon = GetPokerLevel(ui.PMoney);
		Work.UI.PMoney = ui.PMoney;
		Work.UI.nIcon = ui.nIcon;
	}


	// [게임 아이템 작업]
	BOOL bAskUseJumpItem = FALSE;
	BOOL bAskBuyJumpItem = FALSE;

	//### [관리자 모드 작업] ###
	BOOL bAdminUser = FALSE;
	if(IsAdminUser(ui.ID, Work.UserIP)) bAdminUser = TRUE;
	if(!bAdminUser && Cfg3.EnterLimitMode == 1 && Cfg3.bLoginLimit) {// 서버 로그인 입장 제한 모드이면
		int level = Work.UI.nIcon;
	
		if(!CheckLevel(level, Work.UI.PMoney)) { // 입장불가
	
			int grade =  GetMyLeveltoGrade(level); // 레벨 -> 등급
			BOOL bAccessDeny = FALSE;
			
			// 점프아이템 사용중인지 판별
			BOOL bJumpUsing = FALSE;
			BOOL bJumpExist = FALSE;
			bJumpUsing = GameItem_IsUsingItem(Work.Item, ITEM_SCODE_JUMP, &bJumpExist);
					
			// ### [ 채널 입장 구분 ] ###
			if(grade+1 >= g_ServerGrade &&  grade < g_ServerGrade) // 한단계 위로 점프
			{
				// 입장 제한 레벨보다 자신의 레벨이 낮은 경우는 점프 입장권의 구입 의사나 사용 여부를 물어본다
				if( !bJumpUsing && 
					CheckPremiumState(&ui, NULL)==0 &&	// [프리미엄 작업]
					!ui.bPremIP)						// [PC방 작업]
				{	
					// 점프아이템을 사용하고 있지 않고 프리미엄 회원도 아니라면

					if(bJumpExist) {
						// 점프 아이템이 존재한다면 사용 여부를 물어보게 한다(일단 로그인 처리는 계속 진행)
						bAskUseJumpItem = TRUE;
					}
					else {
						// 점프 아이템을 사용하면 입장할 수 있는 등급인 경우는 구입 의사를 물어봄(일단 로그인 처리는 계속 진행)
						bAskBuyJumpItem = TRUE;
					}
				}
				else {
						// 점프 아이템을 사용중이거나 프리미엄 회원이라면 입장 허가
					bAccessDeny = FALSE;
				}

			}
			else
			{  // 불가 
				bAccessDeny = TRUE;

				// 불가 이유를 알려준다....
				// 점프 아템 조차 없다
				CSV_SERVERMSG smsg(SndBuf);
				CString str;

				char lv_buf0[50];
				ZeroMemory( lv_buf0, sizeof(char)*50 );
				char lv_buf1[50];
				ZeroMemory( lv_buf1, sizeof(char)*50 );
				char mylv[50];
				ZeroMemory( mylv, sizeof(char)*50 );
				
				char server_lv[50];
				ZeroMemory( server_lv, sizeof(char)*50);


				// 등급 사용시 추가
				int login1 = 0;
				int login2 = 0;
				if(g_ServerGrade == 0 ) {		 // 자유
					login1 = 0;
					login2 = 14;
				} else if(g_ServerGrade == 1 ) { // 초보
					login1 = 0;
					login2 = 3;
				} else if(g_ServerGrade == 2 ) { // 중수
					login1 = 4;
					login2 = 7;
				} else if(g_ServerGrade == 3 ) { // 고수
					login1 = 8;
					login2 = 11;
				} else if(g_ServerGrade == 4 ) { // 도신	
					login1 = 12;
					login2 = 14;
				} 

				GetLevelName( login1, lv_buf0 ); // 레벨이름
				GetLevelName( login2, lv_buf1 ); // 레벨이름
				GetLevelName( level, mylv ); // 나의 레벨
			//	GetGradeName(level, ) // 등급 구하기

				GetSuitableServerName( level, server_lv ); // 어느 서버 사용가능
		
				{// 2단계 이상 차이 
					str.Format( "[%s]는 [%s] ~ [%s]레벨만 입장가능합니다.\n[%s]회원은 레벨이 [%s]이므로 \"%s\"에 입장가능합니다.", 
					Cfg.ServerName, lv_buf0, lv_buf1, Work.UI.ID, mylv, server_lv );

					smsg.Set(1, (char*)str.operator LPCTSTR() );
					pSocket->Send(smsg.pData, smsg.GetTotalSize());
					// 소켓 종료 예약
					SockMan.ReservClose(sid);
					
				}
			
				return 1;
			}

			if(bAccessDeny) {
				// 점프 아이템도 없고 등급이 맞지 않는 경우는 경고 메시지 후 소켓 끊음
				CSV_SERVERMSG smsg(SndBuf);
				smsg.Set(1, "입장 가능한 등급이 아닙니다. 다른 서버를 이용해주세요.");
				pSocket->Send(smsg.pData, smsg.GetTotalSize());
				// 소켓 종료 예약
				SockMan.ReservClose(sid);
				return 1;
			}
		}
	}

	pUserVal->iUser1 = TRUE; // 올바른 유저가 접속한 소켓임을 나타낸다.
	pUserVal->iUser2 = unum; // 사용자 접속 번호(소켓 클래스에 저장)
			
	ui.UNum = unum;										// 유저 번호

	// 비밀번호 필드 초기화(지금부터는 비밀번호 정보는 필요없다. 해킹의 소지가 있으므로 초기화)
//	memset(ui.Pass, 0, sizeof(ui.Pass));

	//### [관리자 모드 작업] ###
	if(bAdminUser) {
		ui.cAdminUser = 1;
	}

	// 새로운 접속자 메세지 정보 준비
	// ### [이벤트 티켓] ###
	User[unum].SetNewUser(&ui, &Work.Item, sid, &Work.EventPrize);		// [게임 아이템 작업]

	////////////////////////////////////////////////
	//2004.05.07 리더스총액 제한
	if(StatisCfg.bTotalMoneyLimit == TRUE)
	CheckTotalmoneylimit(&User[unum].UI, Work.UniqNo, 2); // 머니 삭감
	////////////////////////////////////////////////

	strncpy(User[unum].RegID, Work.RegID, 15);			// [게임 아이템 작업]
	strncpy(User[unum].UniqNo, Work.UniqNo, 15);        // 유니크 넘버
	// ### [로그 기록용] ###
	strcpy(User[unum].UserIP, Work.UserIP); // 사용자의 접속 IP정보		[유저정보]

	// [수호천사] 2004.07.09
	User[unum].Set_init_lost_money( Work.LostMoney );


	// ### [어뷰저 방지] ###
	// ### [ 중복된 IP가 ] ###
	SOCKADDR_IN Sockaddr;
	int  Socklen = sizeof(Sockaddr);
	getpeername(pSocket->m_hSocket, (struct sockaddr FAR *)&Sockaddr, &Socklen);
	User[unum].PeerIP = Sockaddr.sin_addr;
/*
	BYTE IPC[4]={0,};

	IPC[0] = ip.S_un.S_un_b.s_b1;
	IPC[1] = ip.S_un.S_un_b.s_b2;
	IPC[2] = ip.S_un.S_un_b.s_b3;
	IPC[3] = ip.S_un.S_un_b.s_b4;
*/
	





			

	// [게임 아이템 작업]
	// 소유한 아이템이 존재한다면 아이템 리스트를 보냄
	if(User[unum].GameItem.nNum > 0) {
		
		GAMEITEM_INFO in = {0,};
		in.Kind = 0;
		in.TotNum = User[unum].GameItem.nNum;
		in.StrMsgLen1 = 0;
		in.StrMsgLen2 = 0;
		CTime time = CTime::GetCurrentTime();
		time.GetAsSystemTime(in.ServerTime);

		CSV_GAMEITEMLIST glmsg;
		glmsg.Set(&in, User[unum].GameItem.Item, NULL, NULL);
		User[unum].SendMsg(&glmsg);
	}

	// [게임 아이템 작업]
	if(bAskUseJumpItem) {
		// 점프 아이템이 존재한다면 사용 여부를 물어봄
		// (서버측에서 로그인은 된 상태이나 클라이언트에게 
		// 로그인 허가 메시지는 보내지 않은 상태임)

		// 아이템 사용 설명
		char* strmsg1 = "레벨이 낮아 서버에 접속하시려면 [점프 입장권 아이템]이 필요합니다.\n현재 소유하신 [점프 입장권 아이템]을 사용하시겠습니까?";
		char* strmsg2 = "[점프 입장권 아이템]을 사용하시면 서버의 등급이 높아도 입장하실 수 있습니다. 보유하신 아이템은 아래와 같으며, 아이템을 사용하시려면 목록에서 아이템을 선택후 더블클릭하거나 [사용하기] 버튼을 클릭하시면 됩니다.";

		GAMEITEM_INFO in = {0,};
		in.Kind = 1;
		in.ErrorCode = 0;
		in.StrMsgLen1 = strlen(strmsg1);
		in.StrMsgLen2 = strlen(strmsg2);
		CTime time = CTime::GetCurrentTime();
		time.GetAsSystemTime(in.ServerTime);

		/*
		// 사용 가능한 점프 아이템만 추려냄
		GAMEITEM gitem[MAX_ITEM_NUM] = {0,};
		int totnum = 0;
		for(int i=0; i<Work.Item.nNum; i++) {
			if(Work.Item.Item[i].Code >= ITEM_SCODE_JUMP && Work.Item.Item[i].Code <= ITEM_ECODE_JUMP) {
				ItemState state = GameItem_GetItemState(Work.Item.Item[i]);
				if(state.iState == 1) {
					gitem[totnum] = Work.Item.Item[i];
					totnum++;
				}
			}
		}
		in.TotNum = totnum;
		*/

		// 모든 아이템을 추려냄
		GAMEITEM gitem[MAX_ITEM_NUM] = {0,};
		int totnum = 0;
		for(int i=0; i<Work.Item.nNum; i++) {
			gitem[totnum] = Work.Item.Item[i];
			totnum++;
		}
		in.TotNum = totnum;
	
		CSV_GAMEITEMLIST glmsg;
		glmsg.Set(&in, gitem, strmsg1, strmsg2);
		pSocket->Send(glmsg.pData, glmsg.GetTotalSize());
	}
	else if(bAskBuyJumpItem) {
		// 점프 아이템을 사용하면 입장할 수 있는 등급인 경우는 구입 의사를 물어봄
		CString strmsg;
		strmsg.Format("레벨이 낮아 서버에 입장하시려면 [점프 입장권 아이템]이 필요합니다.\n[점프 입장권 아이템]을 구입하시겠습니까?");

		BUYGAMEITEMINFO bi = {0,};
		bi.Kind = 1;	// 로그인시 점프 입장권 아이템의 구입 의사를 물어봄
		strcpy(bi.ItemName, "점프 입장권 아이템");
		bi.ItemCode = ITEM_SCODE_JUMP;
		bi.StrMsgLen = strmsg.GetLength();

		CSV_BUYGAMEITEM bgmsg(SndBuf);
		bgmsg.Set(&bi, strmsg);
		pSocket->Send(bgmsg.pData, bgmsg.GetTotalSize());
	}
	else {
		// 로그인 허가 메세지 날림
		User[unum].SendAcceptLoginMsg();
	}

	// [서버 에이전트 작업] : 새로운 접속자의 접속이다!!
	if(g_nAgentIndex>0 && g_nAgentIndex<MAX_USER){ 
		int nAgentSID = User[g_nAgentIndex].Sid; // 서버 에이전트 넘 소켓 FD
		CMySocket *pASocket = SockMan.GetDataSocketPtr(nAgentSID);
		if(pASocket && pASocket->m_bCreated){
			USERVAL *pAUserVal = (USERVAL*)(pASocket->m_pUserPtr);
			if(pAUserVal && pAUserVal->iUser1 && nAgentSID == (int)pASocket->m_dwUserVal){
				AGENT_USER au; memset(&au,0,sizeof(AGENT_USER));
				au.nUNum  = unum;
				au.PMoney = ui.PMoney;
				strncpy(au.ID,ui.ID,15); // 아이디

				CNM_AGENT_NEWUSER newuser(SndBuf);
				newuser.Set(RealUser, &au, g_JackPotMoney);
				pASocket->Send(newuser.pData, newuser.GetTotalSize());
			}
		}
	}


	RealUser++;

	UINT logintime = ::timeGetTime() - Work.StartTick;
	static UINT maxtime = 0;
	if(maxtime < logintime) maxtime = logintime;

	// 시간 측정 테스트
	CString teststr;
	teststr.Format("LoginTime=%d (Max=%d)", logintime, maxtime);
	GetDlgItem(IDC_STATIC_TEST)->SetWindowText(teststr);

	return 1;
}	
		

LONG C62CutPokerServerView::OnDbworkLogout(UINT wParam, LONG lParam)
{
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB 처리가 끝나기전에 접속이 끊어진 사용자일 수 있음
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	int unum = pUserVal->iUser2;
	if(unum<1 || unum>=MAX_USER) return 1;

	// DB 처리가 끝나기전에 접속이 끊어진 직후 다른 사용자가 접속한 경우일 수 있음
	if(sid != (int)pSocket->m_dwUserVal) return 1;

	// DB처리결과와 실제 아이디가 틀림(이미 접속이 끊어졌음)
	if(Work.UI.UNum != unum) return 1;

	// DB처리결과와 실제 아이디가 틀림(이미 접속이 끊어졌음)
	if(faststrcmp(Work.UI.ID, User[unum].UI.ID) != 0) return 1;

	// 로그아웃 DB처리 플래그가 세팅되지 않았으면 취소
	if(!User[unum].bLogOutDBSaved) return 1;

	// 로그아웃 허용
	CSV_ACCEPT_LOGOUT alomsg(SndBuf);
	alomsg.Set();
	SockMan.SendData(sid, alomsg.pData, alomsg.GetTotalSize());

	// 강제로 접속을 끊어버림(접속을 유지한 체로 게임을 진행하는 해킹을 방어하기 위함)
	//pSocket->PostClose();
	pSocket->ReservClose();

	return 1;
}



// [게임 아이템 작업]
LONG C62CutPokerServerView::OnDbworkGameitemlist(UINT wParam, LONG lParam)
{
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB 처리가 끝나기전에 접속이 끊어진 사용자일 수 있음
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	int unum = pUserVal->iUser2;
	if(unum<1 || unum>=MAX_USER) return 1;
	if(!User[unum].bValid) return 1;

	// DB 처리가 끝나기전에 접속이 끊어진 직후 다른 사용자가 접속한 경우일 수 있음
	if(sid != (int)pSocket->m_dwUserVal) return 1;
	// DB처리결과와 실제 아이디가 틀림(이미 접속이 끊어졌음)
	if(Work.UI.UNum != unum) return 1;
	// DB처리결과와 실제 아이디가 틀림(이미 접속이 끊어졌음)
	if(faststrcmp(Work.UI.ID, User[unum].UI.ID) != 0) return 1;

	// 게임 아이템 리스트 정보
	GAMEITEM_INFO in = {0,};
	in.Kind = 100;	// 아이템 리스트 보내기
	in.StrMsgLen1 = 0;
	in.StrMsgLen2 = 0;
	CTime time = CTime::GetCurrentTime();
	time.GetAsSystemTime(in.ServerTime);

	if(!Work.bErrorItemWork) {
		// 아이템 쿼리에 성공했다면 사용자의 아이템 리스트를 갱신
		User[unum].GameItem = Work.Item;
		
		in.ErrorCode = 0;
		in.TotNum = User[unum].GameItem.nNum;	
	}
	else {
		// 아이템 쿼리 실패
		in.ErrorCode = 1;
		in.TotNum = 0;
	}

	// 아이템 리스트를 보내줌
	CSV_GAMEITEMLIST glmsg;
	glmsg.Set(&in, User[unum].GameItem.Item, NULL, NULL);
	User[unum].SendMsg(&glmsg);

	return 1;
}

// [게임 아이템 작업]
LONG C62CutPokerServerView::OnDbworkUseitemresult(UINT wParam, LONG lParam)
{
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB 처리가 끝나기전에 접속이 끊어진 사용자일 수 있음
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	int unum = pUserVal->iUser2;
	if(unum<1 || unum>=MAX_USER) return 1;
	if(!User[unum].bValid) return 1;

	// DB 처리가 끝나기전에 접속이 끊어진 직후 다른 사용자가 접속한 경우일 수 있음
	if(sid != (int)pSocket->m_dwUserVal) return 1;
	// DB처리결과와 실제 아이디가 틀림(이미 접속이 끊어졌음)
	if(Work.UI.UNum != unum) return 1;
	// DB처리결과와 실제 아이디가 틀림(이미 접속이 끊어졌음)
	if(faststrcmp(Work.UI.ID, User[unum].UI.ID) != 0) return 1;

	/////// 아이템 사용 처리

	int index = Work.UseItem.Index;

	// 잘못된 아이템 인덱스를 판별
	if(index < 0 || index >= User[unum].GameItem.nNum)
		return 1;

	// 아이템 변경 상태 저장
	User[unum].GameItem.Item[index] = Work.UseItem;

	// 점프 아이템이라면
	if(Work.UseItem.Code >= ITEM_SCODE_JUMP && Work.UseItem.Code <= ITEM_ECODE_JUMP) {
		if(!User[unum].bSendAcceptLogin) {
			// 서버에서는 로그인되었으나 클라이언트에게 로그인 허가 메시지를 날리지 않은 상태임
			if(!Work.bErrorItemWork) {
				// 로그인 허가 메시지를 날림
				User[unum].SendAcceptLoginMsg();
			}
		}
	}
	// 슈퍼 방장 아이템이라면
	else if(Work.UseItem.Code >= ITEM_SCODE_SUPERMASTER && Work.UseItem.Code <= ITEM_ECODE_SUPERMASTER) {
	}
	// 플러스 포인트 아이템이라면
	else if(Work.UseItem.Code >= ITEM_SCODE_PLUSPOINT && Work.UseItem.Code <= ITEM_ECODE_PLUSPOINT) {
	}
	// [수호천사] 2004.07.08
	else if(Work.UseItem.Code >= ITEM_SCODE_SAFEGUARDANGEL && Work.UseItem.Code <= ITEM_ECODE_SAFEGUARDANGEL) {
		INT64 lostMoney = User[unum].Get_lost_money();
		INT64 beforem = User[unum].UI.PMoney;
		if( lostMoney > 0) {	// 머니 복구
			INT64 chargem = Get_angel_defmoney( lostMoney );
			
			User[unum].UI.PMoney += chargem;		

			int cnum = User[unum].ChNum;
			int rnum = User[unum].SUI.RoomNum;			

			//방안의 ui.pmoney 에 값 넣어줌
			for(int i = 0 ; i < Chan[cnum].Room[rnum].m_Max_Player ; i ++)
			{
				if(strcmp(Chan[cnum].Room[rnum].Ui[i].ID,User[unum].UI.ID) == 0)
				{
					Chan[cnum].Room[rnum].Ui[i].PMoney = User[unum].UI.PMoney;
				}
			}			
					
			CCL_CHARGE_SAFEANGEL cmsg;
			cmsg.Set(User[unum].UI.ID, CCL_CHARGE_SAFEANGEL::CHARGE_ANGEL, chargem);
			User[unum].SendMsg(&cmsg);

			User[unum].Set_init_lost_money(0);

			if(User[unum].SUI.Position == WH_GAME && User[unum].PNum != -1) {

				int cnum = User[unum].ChNum;
				int rnum = User[unum].SUI.RoomNum;
				int pnum = User[unum].PNum;
				if(cnum >= 0 && cnum < MAX_CHAN && rnum>0 && rnum <MAX_ROOM && pnum >= 0 && pnum < Chan[cnum].Room[rnum].get_maxplayer()) {
					if( Chan[cnum].Room[rnum].bValid ) {
						Chan[cnum].Room[rnum].SendMsgExept(User[unum].PNum, cmsg.pData, cmsg.GetTotalSize() );
					}
				}
			}
			// 수호천사 아이템 로그 남기기 
			if( Cfg.bAllinDB ) {
				LOGREC_DBWORKS AWork;
				ZeroMemory(&AWork, sizeof(LOGREC_DBWORKS));
				AWork.WorkKind = 4; // [수호천사]
				AWork.StartTick = timeGetTime();
				AWork.bValid = TRUE;
				strncpy(AWork.AllInCharge_Log.ID, User[unum].UI.ID, 15);
				
				AWork.AllInCharge_Log.BeforeMoney = beforem;
				AWork.AllInCharge_Log.AfterMoney = User[unum].UI.PMoney;
				AWork.AllInCharge_Log.AMountMoney = User[unum].UI.PMoney - beforem;
				
				// DB 쓰레드 가동
				BOOL rtn = LogDBMan.SetDBWork(&AWork);
				if(rtn == FALSE) {
					CLogFile logfile("AllInLogDB.txt");
					logfile.Writef("총액 제한 로그기록  에러");
				}
			}
		}
		User[unum].Set_init_lost_money( 0 );
	}

	// 게임 아이템 사용 결과
	USEGAMEITEMINFO ug = {0,};
	strncpy(ug.ID, User[unum].UI.ID, 15);
	ug.bUseError = Work.bErrorItemWork;
	ug.Item = Work.UseItem;

	// 게임 아이템 사용 결과를 보냄
	CSV_USEGAMEITEM ugmsg;
	ugmsg.Set(&ug);
	User[unum].SendMsg(&ugmsg);

	return 1;
}





BOOL C62CutPokerServerView::UserDisconnectProccess(int unum)
{
	if(unum<1 || unum >= MAX_USER) {
		CLogFile logfile;
		logfile.Writef("UserDisconnectProccess() - 잘못된 유저번호(unum=%d)", unum);
		return FALSE;
	}

	if(strlen(User[unum].UI.ID)==0) {
		CLogFile logfile;
		logfile.Writef("UserDisconnectProccess() - 아이디가 없음(unum=%d)", unum);
		return FALSE;
	}

	// 채널에 입장해있던 유저라면 해당 채널에서 퇴장 처리
	if(User[unum].ChNum != -1) {
		Chan[User[unum].ChNum].OnOutUser(unum);
	}

	RealUser--;
	if(unum!=g_nAgentIndex) // [서버 에이전트 작업] : 에이전트가 아닐때만 DB 작업
	{ 
		if(!User[unum].bLogOutDBSaved) 
		{
			// 게임 가감치에 변동사항이 있는 경우만 데이타베이스에 기록한다
		//	if(User[unum].IsSetDBWork()==TRUE) // 디비 스레드 에서 검사
		//	{
				// 이부분에서 DB를 업데이트
				DBWORKS dbwork;
				User[unum].MakeDBWork(4, &dbwork);
				BOOL rtn = DBMan.SetDBWork(&dbwork);

				// DB 작업 큐가 꽉 찬 경우 로그 기록
				if(rtn == FALSE){
					CLogFile LogFile;
					LogFile.Writef(_T("DB Work Que overflow - CServerView::UserDisconnectProccess() : id=%s"), dbwork.UI.ID);
				}
				else User[unum].bLogOutDBSaved = TRUE;
	
		}
	
		//CNetData reData;
		// [서버 에이전트 작업] : 접속자가 종료한다!!
		if(g_nAgentIndex>0 && g_nAgentIndex<MAX_USER){
			int nAgentSID = User[g_nAgentIndex].Sid;
			CMySocket *pASocket = SockMan.GetDataSocketPtr(nAgentSID);
			if(pASocket && pASocket->m_bCreated){
				USERVAL *pAUserVal = (USERVAL*)(pASocket->m_pUserPtr);
				if(pAUserVal && pAUserVal->iUser1 && nAgentSID == (int)pASocket->m_dwUserVal){
					AGENT_USER au; memset(&au,0,sizeof(AGENT_USER));
					au.nUNum = unum;

					INT64 GamePMoney = User[unum].UI.PMoney;// 접속후의 게임머니
					if(GamePMoney<0) GamePMoney = 0;        // 접속후의 게임머니는 딱 맞아야한다. 음수값을 0으로 처리해 놓는다!!!

					au.PMoney = GamePMoney - User[unum].OI.pBackup;

					strncpy(au.ID,User[unum].UI.ID,15); // 아이디
					
					CNM_AGENT_OUTUSER aoutuser(SndBuf);
					aoutuser.Set(RealUser, &au, g_JackPotMoney);
					pASocket->Send(aoutuser.pData, aoutuser.GetTotalSize());
				}
			}
		}
	}

	// [서버 에이전트 작업] : 에이전트 인덱스를 초기화한다
	if(unum==g_nAgentIndex){ 
		g_nAgentIndex=-1;
		GetDlgItem(IDC_EDIT_AGENTCONNECT)->SetWindowText(_T("#### No Agent ####")); // [서버 에이전트 작업]

		/////////////////////////////////////////////////
		// [서버 에이전트 작업] 에이전트 종료시 접속거부 상태........
		CMainFrame *pMain = (CMainFrame*) GetParentFrame();
		g_pMainView->bConnectDeny = FALSE;
		pMain->Connectdeny();
		ShowState(_T("에이전트 종료로 인하여 사용자의 접속을 거부합니다." ));
		/////////////////////////////////////////////////

		// 에이전트 인원 감소 ????
		//RealUser--;
	}


/*
		// 이부분에서 DB를 업데이트
		DBWORKS dbwork;
		User[unum].MakeDBWork(4, &dbwork);
		BOOL rtn = DBMan.SetDBWork(&dbwork);

		// DB 작업 큐가 꽉 찬 경우 로그 기록
		if(rtn == FALSE) {
			CLogFile LogFile;
			LogFile.Writef("DB작업 큐 오버플로우 - CServerView::UserDisconnectProccess() : id=%s", dbwork.UI.ID);
		}
		else User[unum].bLogOutDBSaved = TRUE;
*/


	// 사용자 정보 구조체를 비워둠
	User[unum].Clear();

	return TRUE;
}

LONG C62CutPokerServerView::OnDataClose(UINT wParam, LONG nSockCode)
{
	int sid = wParam;
	CMySocket *pDSock = SockMan.GetDataSocketPtr(sid);

	if(pDSock == NULL) {
		//CLogFile logfile;
		//logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - pDSock == NULL(sid=%d, nSockCode=%d)", sid, nSockCode);
		return 1;
	}

	if(!pDSock->m_bCreated) {
		//CLogFile logfile;
		//logfile.Writef("Error : CServerView::OnDataClose() - 생성되지 않은 소켓(sid=%d, nSockCode=%d)", sid, nSockCode);
		return 1;
	}

	if(sid != (int)pDSock->m_dwUserVal) {
		CLogFile logfile;
		logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - SID값이 틀림(sid=%d, pDSock->m_dwUserVal=%d, nSockCode=%d)", sid, pDSock->m_dwUserVal, nSockCode);
	}

	USERVAL *pUserVal = (USERVAL*)(pDSock->m_pUserPtr);

	int i=0;
	int unum = pUserVal->iUser2; // 사용자 접속 번호

	if(pUserVal->iUser1==FALSE && unum != 0) 
	{
		CLogFile logfile;
		if(unum>0 && unum < MAX_USER)
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - pDSock->iUser1==FALSE (ID=%s, UNum=%d, User[unum].UI.UNum=%d, User[unum].Sid=%d, pDSock->hSID=%d)", User[unum].UI.ID, unum, User[unum].UI.UNum, User[unum].Sid, pDSock->m_dwUserVal);
		else
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - pDSock->iUser1==FALSE (유저번호 범위 초과:UNum=%d)", unum);
	}

	if(pUserVal->iUser1==TRUE)
	{
		// 무효한 사용자로 만든다
		pUserVal->iUser1 = FALSE;
		pUserVal->iUser2 = 0;

		if(unum<1 || unum >= MAX_USER)
		{
			CLogFile logfile;
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - 잘못된 유저 번호(unum=%d)", unum);
		}
		else if(strlen(User[unum].UI.ID)==0)
		{
			CLogFile logfile;
			logfile.Write("Error : C62CutPokerServerView::OnDataClose() - 아이디의 길이가 0이다");
		}
		else if(User[unum].Sid != (int)pDSock->m_dwUserVal)
		{
			CLogFile logfile;
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - SID값이 틀림(%s 회원)", User[unum].UI.ID);
		}

		// 사용자 접속 종료 처리
		BOOL rtn = UserDisconnectProccess(unum);

		if(rtn==FALSE)
		{
			CLogFile logfile;
			if(unum>0 && unum < MAX_USER)
				logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - 접속 종료 처리 실패 (ID=%s, UNum=%d, User[unum].UI.UNum=%d, User[unum].Sid=%d, pDSock->m_dwUserVal=%d)", User[unum].UI.ID, unum, User[unum].UI.UNum, User[unum].Sid, pDSock->m_dwUserVal);
			else
				logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - 접속 종료 처리 실패 (유저번호 범위 초과:UNum=%d)", unum);
		}
	}

	BOOL rtn = SockMan.CloseDataSocket(sid);
	if(rtn==FALSE)
	{
		TRACE("접속 종료 처리 실패\n");

		CLogFile logfile;
		if(unum>0 && unum < MAX_USER)
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - 소켓 닫기 실패 (ID=%s, UNum=%d, User[unum].UI.UNum=%d, User[unum].Sid=%d, pDSock->m_dwUserVal=%d)", User[unum].UI.ID, unum, User[unum].UI.UNum, User[unum].Sid, pDSock->m_dwUserVal);
		else
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - 소켓 닫기 실패 (유저 정보 - UNum=%d(유저번호 범위 초과), pDSock->m_dwUserVal=%d)", unum, pDSock->m_dwUserVal);
		return 1;
	}

	// 접속 종료 처리
	TotUser--;
	CString str;
	str.Format("%d명(%d)",TotUser,RealUser);
	((CWnd*)GetDlgItem(IDC_STATIC_TOTALUSER))->SetWindowText(str);

	return 1;
}

LONG C62CutPokerServerView::OnDataConnect(UINT wParam, LONG pSock)
{
	int sid = wParam;
	CMySocket *pDSock = SockMan.GetDataSocketPtr(sid);
	if(pDSock == NULL) return 1;

	// 접속 등록 처리

	//CString str;
	//str.Format("%d번 사용자가 Connect되었다", pDSock->hSID);
	//AfxMessageBox(str);

	return 1;
}

void C62CutPokerServerView::OnButtonSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_ServerMsg.GetLength()==0) return;

	int rtn = AfxMessageBox("모든 사용자에게 메세지를 보내시겠습니까?", MB_YESNO);
	if(rtn==IDYES) {
		if(SendMsgMode==0) {
			CSV_SERVERMSG smsg;
			smsg.Set(0, (char*)m_ServerMsg.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		}

		if(SendMsgMode==1) {
			CSV_SERVERMSG smsg;
			smsg.Set(1, (char*)m_ServerMsg.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		}

		if(SendMsgMode==2) {
			CSV_SERVERMSG smsg1, smsg2;
			smsg1.Set(0, (char*)m_ServerMsg.operator LPCTSTR());
			SendMsgToAll(smsg1.pData, smsg1.GetTotalSize());
			smsg2.Set(1, (char*)m_ServerMsg.operator LPCTSTR());
			SendMsgToAll(smsg2.pData, smsg2.GetTotalSize());
		}

		////////////////////////////////////////
		//// 자동종료
		CString str;
		str = m_ServerMsg;//.operator LPCTSTR(); 
		str += " SendMessage for All User";
		ShowState(_T(str.operator LPCTSTR() ));
	}
}

void C62CutPokerServerView::OnRadioSendtext() 
{
	SendMsgMode = 0;
}

void C62CutPokerServerView::OnRadioSendmsgbox() 
{
	SendMsgMode = 1;
}

void C62CutPokerServerView::OnRadioSendmsgboth() 
{
	SendMsgMode = 2;
}

void C62CutPokerServerView::OnButtonSetloginmsg() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void C62CutPokerServerView::PrepareClose()
{
	// [ ###잭팟### ] 머니 저장
	ShowState(_T("Close Server...   JackPot.cfg file write..."));
	JackPotSave();


	if(hGlobalGameTimer) KillTimer(GLOBALGAME_TIMER);
	hGlobalGameTimer = NULL;
	if(hMsgPoolingTimer) KillTimer(MSGPOOLING_TIMER);
	hMsgPoolingTimer = NULL;

	// [PC방 작업]
	if(hMsgUerCntTimer) KillTimer(hMsgUerCntTimer);
	hMsgUerCntTimer = NULL;

	// 서버 자동종료
	m_bExitReserve = FALSE; // 자동종료시 겜스타트 안됨
	if(hExitReserveTimer) KillTimer(EXITRESERVE_TIMER);
	hExitReserveTimer = NULL;


	MsgDlg.Create(this, "소켓 메니저를 종료하는 중입니다");

	EnableWindow(FALSE);

	// 소켓 메니저 종료
	SockMan.Destroy();

	MsgDlg.SetMsg("DB 쓰레드를 종료하는 중입니다");

	// 불량 아이디 신고 DB쓰레드 종료
	TerminateBadUserDBThread();

	// 사용자 DB 업데이트
	if(TotUser > 0  && g_pMainView->Cfg3.bExamServer==FALSE) //######## [연습 채널 작업] #########
	{
		MsgDlg.SetMsg("사용자 DB를 기록하는 중입니다");

		for(int i=0; i<MAX_USER; i++)
		{

			if(i==g_nAgentIndex) continue; // [서버 에이전트 작업] : 작업할 필요 없다

			if(strlen(User[i].UI.ID) > 0)
			{

				// 게임 가감치에 변동사항이 있는 경우만 데이타베이스에 기록한다
				//if(User[i].IsSetDBWork()==TRUE){
					// 이부분에서 DB를 업데이트
					DBWORKS dbwork;
					if(!User[i].MakeDBWork(4, &dbwork)) continue;
					BOOL rtn = DBMan.SetDBWork(&dbwork);

					// DB 작업 큐가 꽉 찬 경우 로그 기록
					if(rtn == FALSE) {
						CLogFile LogFile;
						LogFile.Writef(_T("DB Work Que overflow - OnDestroy() : id=%s"), dbwork.UI.ID);
					}
			//	}
			}
		}
	}

	// [ 통계기록 ]
	//if( !bIsForceClose ) CloseServer_SALog(); <- 삭제
	if( !bIsForceClose ) m_cSALog.CloseServer_savedata(); //<- 추가

	// 타이머 작동시까지의 1초간의 딜레이를 없애기 위해 먼저 실행
	OnCloseTimer();

	// 종료 타이머를 생성한다
	hCloseTimer = SetTimer(CLOSE_TIMER, 1000, NULL);

	// [PC방 작업]
	//M_ImDieNow(AfxGetMainWnd()->m_hWnd);
}

void C62CutPokerServerView::OnCloseTimer()
{
	// 1초간격으로 DB처리 종결 여부를 검사하고 180초후 강제 종료한다
	static int cnt=180;
	if(cnt>0) {
		cnt--;
		DBMan.CheckDBThread();

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
	// ### [로그 기록용] ###
		LogDBMan.CheckDBThread();
}
#endif

		int num = DBMan.DBQue.GetTotNum();
		int work = DBMan.GetWorkingDBThreadNum();

		int num1 = 0;
		int work1= 0;

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
	// ### [로그 기록용] ###
		num1 = LogDBMan.DBQue.GetTotNum();
		work1= LogDBMan.GetWorkingDBThreadNum();
}
#endif
	//	num1 = 10;

		if(num > 0 || work > 0 || num1 > 0 || work1 > 0) {
			CString str;
			if(num > 0 || num1 > 0) str.Format("%d개의 사용자 DB, %d개의 로그 DB를 기록하는 중입니다(남은 시간: %d초)", num, num1, cnt);
			else str.Format("아직 %d개의 사용자DB, %d로그DB 쓰레드가 활동 중입니다(남은 시간: %d초)", work, work1, cnt);
			MsgDlg.SetMsg((LPCTSTR)str);
			return;
		}
	}

	KillTimer(hCloseTimer);

	MsgDlg.SetMsg(_T("Clear ServerCode of GameDB..."));

	// ????????
//	if(Cfg.ServerCode!=0 && bIsForceClose==FALSE){
//		BOOL bRes = DBMan.ClearSameServerCode(Cfg.ServerCode);
//		if(bRes==FALSE) AfxMessageBox(_T("Failed clear servercode of GameDB!"));
//	}

	MsgDlg.SetMsg("쓰레드 종료처리 중...");

	DBMan.TermanateAllDBThread();


#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
	MsgDlg.SetMsg("로그 기록 쓰레드 종료처리 중...");
	// ### [로그 기록용] ###
	LogDBMan.TermanateAllDBThread();
}
#endif

	MsgDlg.DestroyWindow();

	AfxGetMainWnd()->PostMessage(WM_QUIT);
	// [PC방 작업]							// 추가
	M_ImDieNow(AfxGetMainWnd()->m_hWnd);	// 추가
}

void C62CutPokerServerView::OnDestroy() 
{	
	// TODO: Add your message handler code here
	
	CFormView::OnDestroy();
}

void C62CutPokerServerView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == MSGPOOLING_TIMER)
	{
		int totsize=0;
		CMySocket *pSocket = NULL;
		for(int sid=0; sid<MAX_DATA_SOCKET; sid++)
		{
			pSocket = SockMan.GetDataSocketPtr(sid);
			if(pSocket==NULL) continue;
			if(!pSocket->m_bCreated || !pSocket->m_bIsOnline || pSocket->m_bClosePosted) continue;

			while(TRUE)
			{
				pSocket->Lock();
				totsize = CheckPacket(pSocket, MsgBuf, &MsgSignal);
				pSocket->Unlock();

				if(totsize > 0) {
					ProcessPacket(sid, pSocket, MsgBuf, totsize, MsgSignal);
				}
				else if(totsize < 0) {
					// 잘못된 패킷 혹은 소켓으로 인하여 접속 끊음
					SockMan.ReservClose(sid);
					break;
				}
				else break;
			}
		}
	}
	else if(nIDEvent == GLOBALGAME_TIMER)
	{
		// 초단위 정적 카운터
		static UINT totcnt = 0;
		totcnt++;

		if(totcnt>=10000) totcnt = 40; // UINT형의 오버플로우 방지

		// 각 채널 게임 타이머 처리
		for(int i=0; i<MAX_CHAN; i++) 
		{
			if(Chan[i].TotUser > 0)	Chan[i].OnGlobalGameTimer();
		}

		// DB 쓰레드 타임아웃 처리
		DBMan.CheckDBThread();

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
		// ### [로그 기록용] ###
		LogDBMan.CheckDBThread();
}
#endif

		
		// 유저 타임아웃 처리 및 닫기 시간 초과 소켓 검사
		for(i=0; i<MAX_DATA_SOCKET; i++)
		{
			if(SockMan.pDataSock[i] != NULL)
			{
				if(!SockMan.pDataSock[i]->m_bCreated || !SockMan.pDataSock[i]->m_bIsOnline) continue;

				USERVAL *pUserVal = (USERVAL*)(SockMan.pDataSock[i]->m_pUserPtr);
				if(!pUserVal) continue;

				int unum = pUserVal->iUser2;

				if(pUserVal->iUser1 == TRUE) 
				{
					// 유효한 사용자라면 접속 타임 카운트 증가
					if(unum>0 && unum<MAX_USER) 
					{
						User[unum].ConnectSec++;
						if(unum !=g_nAgentIndex)
						{
							// [서버 에이전트 작업] : 정해진 시간에 중복접속 체크
							if(User[unum].ConnectSec ==  15 || //  15초에 한번...
							   User[unum].ConnectSec == 100 ){ // 100초에 한번...

								FindDoubleConnect(unum); //: 중복접속을 확인한다
							}
						}
						// 60초 간격으로 heart beat를 보냄  [SSO 작업]
						if(totcnt%60 == 0) {
							CSV_HEARTBEAT hbmsg(SndBuf);
							hbmsg.Set();
							SockMan.SendData(i, hbmsg.pData, hbmsg.GetTotalSize());
						}
					}
				}
				else
				{
					// 유효하지 않은 사용자이면 10초간격으로 heart beat를 보냄
					if(totcnt%10 == 0) {
						CSV_HEARTBEAT hbmsg(SndBuf);
						hbmsg.Set();
						SockMan.SendData(i, hbmsg.pData, hbmsg.GetTotalSize());
					}
				}

				// 타임아웃 카운트 증가
				pUserVal->iTimeOutCnt++;

				// 타임아웃 금지 - jeong
				pUserVal->iTimeOutCnt = 0;

				// 3초전에 미리 타임아웃을 알림
				if(pUserVal->iTimeOutCnt == MAX_TIMEOUT-3)
				{
					// 사용자에게 서버메세지를 보냄
					CSV_SERVERMSG smsg;
					smsg.Set(1, "일정 시간 동안 서버와의 교신이 없었으므로 접속이 끊어집니다.");
					SockMan.SendData(i, smsg.pData, smsg.GetTotalSize());
				}
				// 최대 타임아웃 시간을 넘기면 접속 종료시킴
				else if(pUserVal->iTimeOutCnt >= MAX_TIMEOUT)
				{
					pUserVal->iTimeOutCnt=0;
					SockMan.ReservClose(i); // 연결 종료 예약
					continue;
				}
			}
		}

		// [관리자 모드 작업]
		for(i=0; i<MAX_DENY_USER; i++){
			if(g_AccessDeny[i].nSecond==0) continue;
			g_AccessDeny[i].nSecond--;
			if(g_AccessDeny[i].nSecond<0) g_AccessDeny[i].nSecond =0;
		}

		for(i=1; i<MAX_USER; i++)
		{
			if(strlen(User[i].UI.ID) == 0) continue;

			int sid = User[i].Sid;
			if(sid < 0 || sid >= MAX_DATA_SOCKET)
			{
				CLogFile logfile;
				logfile.Writef("비정상 타임아웃 종료 : C62CutPokerServerView::OnTimer() - [%s]회원 : sid 범위 초과(User[i].Sid=%d)", User[i].UI.ID, User[i].Sid);

				UserDisconnectProccess(i);
				continue;
			}

			if(SockMan.pDataSock[sid] == NULL)
			{
				CLogFile logfile;
				logfile.Writef("비정상 타임아웃 종료 : C62CutPokerServerView::OnTimer() - [%s]회원 : SockMan.pDataSock[sid] == NULL", User[i].UI.ID);

				UserDisconnectProccess(i);
				continue;
			}
		}

		// ### [이벤트 티켓] ###
		CheckGameEvent();

		// [ 통계기록 ]
		//Check_StatisticsLog(); <- 삭제
		m_cSALog.Time_check(); //<- 추가

	}
	else if(nIDEvent == CLOSE_TIMER)
	{
		OnCloseTimer();
	}
	// [PC방 작업]
	else if(nIDEvent == USERCNT_TIMER) {	
		SendUserSizeToBridge(AfxGetMainWnd()->m_hWnd, RealUser);
	}
	else if(nIDEvent == EXITRESERVE_TIMER)
	{////////////////////////////////////////////////////////////////
	 ///// 자동종료
		ExitReserveTimer(); // 서버종료 예약
	}

	CFormView::OnTimer(nIDEvent);
}

void C62CutPokerServerView::OnButtonForcedelroom() 
{
	// TODO: Add your control notification handler code here
	/*
	UpdateData(TRUE);
	if(m_DelRoomNo.GetLength()==0)
	{
		AfxMessageBox("강제 삭제할 방 번호를 입력하세요.");
		return;
	}

	int rnum = ::atoi(m_DelRoomNo);
	if(rnum<1 || rnum>=MAX_ROOM)
	{
		AfxMessageBox("방 번호가 범위를 초과하였습니다. 올바르게 입력하세요");
		return;
	}

	CString msg, str;
	str.Format("방번호: %d (RoomNum=%d)\n", rnum, Room[rnum].Ri.RoomNum);
	msg += str;
	str.Format("방제목: %s\n", Room[rnum].Ri.Title);
	msg += str;
	str.Format("방  장: %s (UNum=%d)\n", Room[rnum].Ri.ID, Room[rnum].Ri.UNum);
	msg += str;
	str.Format("인  원: %d/%d\n", Room[rnum].Ri.NowUserNum, Room[rnum].Ri.MaxUserNum);
	msg += str;
	str.Format("비  번: [%s]\n", Room[rnum].Ri.Pass);
	msg += str;
	msg += "\n위의 게임방을 삭제하시겠습니까?";

	int rtn = ::MessageBox(m_hWnd, msg, "방 삭제 확인", MB_YESNO);
	if(rtn == IDNO) return;

	// 모든 사용자에게 방이 삭제됨을 알림
	CSV_OUTROOM ormsg;
	SMALLROOMINFO sri = Room[rnum].GetSmallRoomInfo();
	ormsg.Set(&sri);
	SendMsgToAll(ormsg.pData, ormsg.GetTotalSize());
	
	// 방 삭제
	Room[rnum].Clear();
	*/
}

void C62CutPokerServerView::OnButtonForcedisconnect() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_DiscTargetID.GetLength()==0)
	{
		int realtotsock = 0;
		int realtotuser = 0;
		int sameusernum = 0;
		int sameusersid = 0;
		int samesocksid = 0;
		int oversocksid = 0;
		int closepostsocknum = 0;
		int invalidsocknum = 0;
		int lobyopennum = 0;

		int i;
		for(i=0; i<MAX_DATA_SOCKET; i++) if(SockMan.pDataSock[i] && SockMan.pDataSock[i]->m_bCreated) realtotsock++;
		for(i=0; i<MAX_USER; i++) {
			if(User[i].bValid) {
				realtotuser++;
				if(i == User[i].UI.UNum) sameusernum++;
				if(User[i].Sid >= 0 && User[i].Sid < MAX_DATA_SOCKET)
					if(User[i].Sid == (int)(SockMan.pDataSock[User[i].Sid]->m_dwUserVal)) sameusersid++;
				if(User[i].bLobyOpen) lobyopennum++;
			}

		}
		for(i=0; i<MAX_DATA_SOCKET; i++) {
			if(SockMan.pDataSock[i] != NULL) {
				if(!SockMan.pDataSock[i]->m_bCreated) continue;
				if(SockMan.pDataSock[i]->m_hSocket == INVALID_SOCKET) invalidsocknum++;
				if(SockMan.pDataSock[i]->m_bClosePosted) closepostsocknum++;

				USERVAL *pUserVal = (USERVAL*)(SockMan.pDataSock[i]->m_pUserPtr);
				if(pUserVal) {
					if(i == pUserVal->hSID) samesocksid++;
					if(pUserVal->hSID < 0 || pUserVal->hSID >= MAX_DATA_SOCKET) oversocksid++;
				}
			}
		}

		CString str, strtmp;
		strtmp.Format("강제 종료할 대상의 ID를 입력하세요\n\n"); str += strtmp;
		strtmp.Format("=========== 참고 정보 ============\n"); str += strtmp;
		strtmp.Format("실제 접속소켓 수=%d, 실제 사용자 수=%d\n", realtotsock, realtotuser); str += strtmp;
		strtmp.Format("올바른 UNUM 개수=%d\n", sameusernum); str += strtmp;
		strtmp.Format("올바른 유저 SID개수=%d\n", sameusersid); str += strtmp;
		strtmp.Format("올바른 소켓 SID개수=%d\n", samesocksid); str += strtmp;
		strtmp.Format("종료 예약된 소켓개수=%d\n", closepostsocknum); str += strtmp;
		strtmp.Format("잘못된 소켓개수=%d\n", invalidsocknum); str += strtmp;
		strtmp.Format("범위초과 소켓 SID개수=%d\n", oversocksid); str += strtmp;
		strtmp.Format("생성된 DB쓰레드 개수=%d\n", DBMan.GetLiveDBThreadNum()); str += strtmp;
		strtmp.Format("활동중인 DB쓰레드 개수=%d\n", DBMan.GetWorkingDBThreadNum()); str += strtmp;

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
		// ### [로그 기록용] ###
		strtmp.Format("생성된 로그기록용 DB쓰레드 개수=%d\n", LogDBMan.GetLiveDBThreadNum()); str += strtmp;
		strtmp.Format("활동중인 로그기록용 DB쓰레드 개수=%d\n", LogDBMan.GetWorkingDBThreadNum()); str += strtmp;
}
#endif
		strtmp.Format("남은 DB처리량=%d\n", DBMan.DBQue.GetTotNum()); str += strtmp; // [SSO 작업] - 수정

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
		// ### [로그 기록용] ###
		strtmp.Format("남은 아이템 DB처리량=%d\n", LogDBMan.DBQue.GetTotNum()); str += strtmp;
}
#endif
		strtmp.Format("남은 불량아이디 신고 DB처리량=%d\n", BadUserDBQue.TotNum); str += strtmp;
		strtmp.Format("로비창 Open 유저수=%d\n", lobyopennum); str += strtmp;
		AfxMessageBox(str);
		return;
	}

	CString str;
	for(int i=0; i<MAX_USER; i++)
	{
		if(m_DiscTargetID.Compare(User[i].UI.ID)==0) {
			int sid = User[i].Sid;
			str.Format("[%s]회원을 강제종료하시겠습니까?(sid=%d, Adress is %s)\n",User[i].UI.ID, sid, (SockMan.pDataSock[sid]==NULL)?"NULL!":"Valid.");
			int rtn = AfxMessageBox(str,MB_YESNO);
			if(rtn==IDYES) {
				USERVAL *pUserVal = NULL;
				if(SockMan.pDataSock[sid]!=NULL) pUserVal = (USERVAL*)(SockMan.pDataSock[sid]->m_pUserPtr);

				if(SockMan.pDataSock[sid]==NULL) {
					AfxMessageBox("소켓 포인터가 NULL입니다. 다른방법으로 강제종료하겠습니다.");
					int unum = User[i].UI.UNum;
					if(unum<0 || unum>=MAX_USER) {
						AfxMessageBox("UserInfo Struct의 UNum값이 범위를 초과하였습니다.");
						unum = i;
					}

					if(faststrcmp(User[unum].UI.ID, User[i].UI.ID) != 0) {
						rtn = AfxMessageBox("유저번호와 사용자ID가 일치하지 않습니다. 그래도 접속종료시키겠습니까?", MB_YESNO);
						if(rtn==IDNO) return;
					}
					UserDisconnectProccess(i);
				}
				else if(pUserVal == NULL) {
					AfxMessageBox("pUserVal 값이 NULL입니다. 다른방법으로 강제종료하겠습니다.");
					UserDisconnectProccess(i);
					SockMan.ReservClose(sid); // 연결 종료 예약
				}
				else if(pUserVal->iUser1 == FALSE) {
					AfxMessageBox("iUser1의 값이 FALSE입니다. 다른방법으로 강제종료하겠습니다.");
					int unum = User[i].UI.UNum;
					if(unum<0 || unum>=MAX_USER) {
						AfxMessageBox("UserInfo Struct의 UNum값이 범위를 초과하였습니다.");
						unum = i;
					}
					else if(sid != User[i].Sid) AfxMessageBox("SID값이 서로 다릅니다");

					if(faststrcmp(User[unum].UI.ID, User[i].UI.ID) != 0) {
						rtn = AfxMessageBox("유저번호와 사용자ID가 일치하지 않습니다. 그래도 접속종료시키겠습니까?", MB_YESNO);
						if(rtn==IDNO) return;
					}
					UserDisconnectProccess(i);
				}
				else SockMan.ReservClose(sid); // 연결 종료 예약
			}
			return;
		}
	}
	str.Format("[%s]회원은 접속하지 않은 아이디거나 존재하지 않습니다", m_DiscTargetID.operator LPCTSTR());
	AfxMessageBox(str);
}





// 전체 백업용 로그파일작성 타이틀을 생성한다
CString C62CutPokerServerView::GetLogFileName_view()
{//사용 하지 않는다
	CString strFileName(_T(""));

	CTime  time = CTime::GetCurrentTime();

	strFileName.Format(g_StrMan.Get(_T("SPOKER_BACKUP")),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute());

	return strFileName;
}



// [ ###잭팟### ] 서버 종료시 저장한다.
void C62CutPokerServerView::JackPotSave()
{

	g_pMainView->Cfg4.nJackPotMoney = g_JackPotMoney;

	FILE *fp = fopen( "JackPot.cfg", "wb");
	if( fp == NULL ) { 
		AfxMessageBox("잭팟 파일을 기록할 수 없습니다.");
		return;
	}
	fwrite(&g_pMainView->Cfg4, sizeof(CONFIG4), 1, fp);
	fclose(fp);

}

// [ ###잭팟### ]  잭팟 머니 보기시 임시저장
void C62CutPokerServerView::OnButtonJackpot() 
{
	// TODO: Add your control notification handler code here
	// 잭팟 보이기
	JackPotSave();
	CString strM = g_MakeCommaMoney(g_JackPotMoney);
	ShowState((char*)strM.operator LPCTSTR());
}


////////////////////////////////////////////////////////////////
///// 자동종료
// 서버 마지막 작업을 보인다
void C62CutPokerServerView::ShowState(LPCTSTR szChar)
{
	if(szChar==NULL) return;
	if(strlen(szChar)<=0) return;

	CTime  time = CTime::GetCurrentTime();

	CString strMessage;
	if(m_bExitReserve)
	{
		int Mode = max(0, SendMsgMode);
		if(Mode == 0)
			strMessage.Format(_T("[%d/%d/%d/%d:%d] Message Mode: Text\r\n\r\n%s"),time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),szChar);
		else if(Mode == 1)
			strMessage.Format(_T("[%d/%d/%d/%d:%d] Message Mode: MessageBox\r\n\r\n%s"),time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),szChar);
		else if(Mode == 2)
			strMessage.Format(_T("[%d/%d/%d/%d:%d] Message Mode: Text and MessageBox\r\n\r\n%s"),time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),szChar);

	}
	else
		strMessage.Format(_T("[%d/%d/%d/%d:%d]\r\n%s"),time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),szChar);

	GetDlgItem(IDC_EDIT_STATE)->SetWindowText(strMessage.operator LPCTSTR());
}


////////////////////////////////////////////////////////////////
///// 자동종료
void C62CutPokerServerView::OnRadioExit30s() 
{//30초
	// TODO: Add your control notification handler code here
	SendExitMode = 0;
	m_ReservTimeLimit = 30;
	m_iExitRadio =5;
	UpdateData(FALSE);
	
}

void C62CutPokerServerView::OnRadioExit1() 
{//1분
	// TODO: Add your control notification handler code here
	SendExitMode = 1;
	m_ReservTimeLimit = 60;
	m_iExitRadio = 4;
	UpdateData(FALSE);
	
}

void C62CutPokerServerView::OnRadioExit3() 
{//3분
	// TODO: Add your control notification handler code here
	SendExitMode = 2;
	m_ReservTimeLimit = 180;
	m_iExitRadio = 3;
	UpdateData(FALSE);
}

void C62CutPokerServerView::OnRadioExit5() 
{//5분
	// TODO: Add your control notification handler code here
	SendExitMode = 3;
	m_ReservTimeLimit = 300;
	m_iExitRadio = 2;
	UpdateData(FALSE);
	
}

void C62CutPokerServerView::OnRadioExit10() 
{//10분
	// TODO: Add your control notification handler code here
	SendExitMode = 4;
	m_ReservTimeLimit = 600;
	m_iExitRadio = 1;
	UpdateData(FALSE);
	
}

void C62CutPokerServerView::OnRadioExit30() 
{//30분
	// TODO: Add your control notification handler code here
	SendExitMode = 5;
	m_ReservTimeLimit = 1800;
	m_iExitRadio = 0;
	UpdateData(FALSE);
	
}

////////////////////////////////////////////////////////////////
///// 자동종료
void C62CutPokerServerView::OnButtonExitok() 
{
	// TODO: Add your control notification handler code here
	int rtn = AfxMessageBox("서버 자동 종료를 예약 하시겠습니까?", MB_YESNO);
	if(rtn==IDYES) {

		
		if(m_ServerMsg.GetLength()==0) {
			
			m_ServerMsg.Format(" 안녕하세요!! 운영자 입니다. 서버 정비 관계로 인하여 서버를 종료 하겠습니다.");
		}
		

		int Mode = max(0, SendMsgMode);
		CString str;
		str = m_ServerMsg;
		if(SendExitMode==0) {

			str += " 30초 후에 자동 종료됩니다.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		
			ShowState(_T(str.operator LPCTSTR() ));
			
		}
		else if(SendExitMode==1) 
		{
			str += " 1분 후에 자동 종료됩니다.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			ShowState(_T(str.operator LPCTSTR() ));
			
		} 
		else if(SendExitMode==2) 
		{
			str += " 3분 후에 자동 종료됩니다.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			ShowState(_T(str.operator LPCTSTR() ));
		}
		else if(SendExitMode==3) 
		{
			str += " 5분 후에 자동 종료됩니다.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			ShowState(_T(str.operator LPCTSTR() ));
		}
		else if(SendExitMode==4) 
		{
			str += " 10분후에 자동 종료됩니다.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			ShowState(_T(str.operator LPCTSTR() ));
		}
		else
		{
			str += " 30분 후에 자동 종료됩니다.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			ShowState(_T(str.operator LPCTSTR() ));
		}

		if(hExitReserveTimer) KillTimer(EXITRESERVE_TIMER);
		hExitReserveTimer = NULL;

		hExitReserveTimer = SetTimer(EXITRESERVE_TIMER, 1000, NULL);
	
		m_bExitReserve = TRUE;
						
		m_ExitReserve.EnableWindow(FALSE);
		m_ExitCancel.EnableWindow(TRUE);
		UpdateData(FALSE);
	}
}

////////////////////////////////////////////////////////////////
///// 자동종료
void C62CutPokerServerView::OnButtonExitcancel() 
{
	// TODO: Add your control notification handler code here
	int rtn = AfxMessageBox("서버자동 종료 메세지를 취소 하시겠습니까?", MB_YESNO);
	if(rtn==IDYES) {

		m_bExitReserve = FALSE;
		

		if(hExitReserveTimer) KillTimer(EXITRESERVE_TIMER);
		hExitReserveTimer = NULL;

		
		m_ExitReserve.EnableWindow(TRUE);
		m_ExitCancel.EnableWindow(FALSE);

		ShowState(_T("서버 자동종료를 취소하셨습니다." ));
		m_ServerMsg = "";

		UpdateData(FALSE);
	}
	
}


////////////////////////////////////////////////////////////////
///// 자동종료
void C62CutPokerServerView::ExitReserveTimer()
{
	int ReservCnt = m_ReservTimeLimit--;

	// 텍스트, 메세지 박스 구분
	int Mode = max(0, SendMsgMode);

	if(SendExitMode==0) 
	{// 30초
		CString str;
		str.Format(" %d 초 후에 자동 종료됩니다.", ReservCnt);
		if(m_ReservTimeLimit <= 30 && (ReservCnt%10 == 0) )
		{
			
			CSV_SERVERMSG smsg;
			smsg.Set(0, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
			
			str += "  SendMessage for All User";
		}
		
		
		ShowState(_T(str.operator LPCTSTR() ));
		
	}
	else if(SendExitMode==1) 
	{// 1분
		CString str;
		str.Format(" %d 초 후에 자동 종료됩니다.", ReservCnt);
		if(m_ReservTimeLimit <= 30 && (ReservCnt%10 == 0) )
		{
			
			CSV_SERVERMSG smsg;
			smsg.Set(0, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
			
			str += "  SendMessage for All User";
		}
		
		
		ShowState(_T(str.operator LPCTSTR() ));
		
	}
	else 
	{// 3분 이상~~
		CString str;
		BOOL bFlag = FALSE;
				
		if(m_ReservTimeLimit >= 60 && (ReservCnt%60 == 0) )
		{
			str = "";
			str.Format(" %d 분 후에 자동 종료됩니다.", ReservCnt/60);
			
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
			
			bFlag = TRUE;
			
		}
		else if(m_ReservTimeLimit <= 30 && (ReservCnt%10 == 0) )
		{
			
			str = "";
			str.Format(" %d 초 후에 자동 종료됩니다.", ReservCnt);

			CSV_SERVERMSG smsg;
			smsg.Set(0, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			bFlag = TRUE;
			
		}


		if(m_ReservTimeLimit >= 60 )
		{
			int minute = ReservCnt/60;
			int second = ReservCnt%60;
			str = "";
			
			if(bFlag)
				str.Format(" %d 분 %d초 후에 자동 종료됩니다.  SendMessage for All User!", minute, second);
			else
				str.Format(" %d 분 %d초 후에 자동 종료됩니다.", minute, second);

			ShowState(_T(str.operator LPCTSTR() ));
		}
		else 
		{
			str = "";

			if(bFlag)
				str.Format(" %d 초 후에 자동 종료됩니다.  SendMessage for All User!", ReservCnt);
			else
				str.Format(" %d 초 후에 자동 종료됩니다.", ReservCnt);
			
			ShowState(_T(str.operator LPCTSTR() ));
		}

	} 

	if(m_ReservTimeLimit < 0)
	{
		CString str1;
		str1.Format("서버를 종료합니다.");

		CSV_SERVERMSG smsg;
		smsg.Set(0, (char*)str1.operator LPCTSTR());
		SendMsgToAll(smsg.pData, smsg.GetTotalSize());

		ShowState(_T(str1.operator LPCTSTR() ));
		
		// 종료 작업
		PrepareClose();
	}
}



///////////////////////////////////////////////////////////////////
/////////////////// ### [관리자 모드 작업] ###  ///////////////////

// [관리자 모드 작업]
void C62CutPokerServerView::OnSetAdmin() 
{
	// TODO: Add your command handler code here
	CAdminManager dlg;
	dlg.DoModal();
	
}


// [관리자 모드 작업]  읽어온다
void C62CutPokerServerView::LoadAdminInfo()
{
	// 환경설정 파일을 연다
	ZeroMemory(&g_AdminInfo, sizeof(g_AdminInfo));
	FILE *fp = fopen( "AdminInfo.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("Can't find [AdminInfo.cfg] file!\nSetup Admin config!");
		return;
	}
	char *tmp = new char[sizeof(g_AdminInfo)];
	char *tmp1 = new char[sizeof(g_AdminInfo)];
	ZeroMemory(tmp, sizeof(g_AdminInfo));
	ZeroMemory(tmp1, sizeof(g_AdminInfo));
	fread(tmp, sizeof(g_AdminInfo), 1, fp);
	for(int i=0; i<sizeof(g_AdminInfo);i++) {
		tmp1[i]^= (tmp[i]^'C'^'Y');
	}
	memcpy(&g_AdminInfo, tmp1, sizeof(g_AdminInfo));
	delete tmp;
	delete tmp1;
	fclose(fp);
}

// [관리자 모드 작업] 관리자 아이디인가? 아닌가?
BOOL C62CutPokerServerView::IsAdminUser(char* id, char *ip)
{
	if(id==NULL) return FALSE;
	
	BOOL bAdmin = FALSE;

	for(int i=0; i<MAX_ADMIN_NUM; i++){
		if(strlen(g_AdminInfo.admin[i].ID) > 0) {
			if(faststrcmp(g_AdminInfo.admin[i].ID, id)==0){
				if(faststrcmp(g_AdminInfo.admin[i].UserIP, ip) == 0) {
					bAdmin = TRUE;
					break;
				}
			}
		}
	}
	return bAdmin;
}

// [관리자 모드 작업] 접속중지된 유저인가? 아닌가?
BOOL C62CutPokerServerView::IsAccessDeny(char* id, UINT &nSec)
{
	if(id==NULL) return FALSE;
	
	BOOL bDeny = FALSE;

	for(int i=0; i<MAX_DENY_USER; i++){
		if(g_AccessDeny[i].nSecond<=0) continue;
		if(strlen(g_AccessDeny[i].ID) > 0) {
			if(_stricmp(g_AccessDeny[i].ID, id)==0){
				nSec = g_AccessDeny[i].nSecond;
				bDeny = TRUE;
				break;
			}
		}
	}
	return bDeny;
}


// [관리자 모드 작업]
void C62CutPokerServerView::OnBtnDenyuser() 
{
	// TODO: Add your control notification handler code here
	CString strUser = _T("");
	CString str;

	for(int i=0; i<MAX_DENY_USER; i++){
		if(g_AccessDeny[i].nSecond<=0) continue;
		if(strlen(g_AccessDeny[i].ID) > 0) {
			str.Format("%s(%d)\n",g_AccessDeny[i].ID,g_AccessDeny[i].nSecond);
			strUser+=str;
		}
	}
	if(strUser!=_T("")) AfxMessageBox(strUser);
	else AfxMessageBox("접속거부된 사용자가 없습니다");
	
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////




// [서버 에이전트 작업] : 중복접속을 확인한다
void C62CutPokerServerView::FindDoubleConnect(const int unum) 
{
	// 일단은 초/중/고/신 서버 전부 테스트해본다
	if(g_nAgentIndex<1 || g_nAgentIndex>=MAX_USER) return;
	if(unum < 1 || unum >= MAX_USER)  return;
	if(strlen(User[unum].UI.ID)==0)   return;

	int nAgentSID = User[g_nAgentIndex].Sid;
	CMySocket *pASocket = SockMan.GetDataSocketPtr(nAgentSID);
	if(pASocket && pASocket->m_bCreated){
		USERVAL *pAUserVal = (USERVAL*)(pASocket->m_pUserPtr);
		if(pAUserVal && pAUserVal->iUser1 && nAgentSID == (int)pASocket->m_dwUserVal){
			AGENT_USER au; memset(&au,0,sizeof(AGENT_USER));
			au.nUNum  = unum;
			au.PMoney = (INT64)0;
			strncpy(au.ID,User[unum].UI.ID,15); // 아이디

			CNM_AGENT_DUBLECON duser(SndBuf);
			duser.Set(RealUser, &au);
			pASocket->Send(duser.pData, duser.GetTotalSize());
		}
	}
}



// 서버 입장제한
BOOL C62CutPokerServerView::CheckLevel(int nIcon, INT64 money)
{

	// ### [ 채널 입장 구분 ] ###
	// 서버 등급
	switch(g_ServerGrade)
	{
	case 0: // 자유서버
		{
			return TRUE;
		} break;
		
	case 1:// 초보서버
		{
			if(nIcon >= 0 && nIcon <= 3) return TRUE;
			// 예외 
			if(nIcon == 4 && money <= 1500000000) return TRUE; // 15억
						
		} break;
	case 2: // 중수서버 
		{
			if(nIcon >= 4 && nIcon <= 7) return TRUE;
			// 예외 
			if(nIcon == 8 && money <= 55000000000) return TRUE; // 1500억
			
		} break;
	case 3: // 고수서버
		{
			if(nIcon >= 8 && nIcon <= 11) return TRUE;
			// 예외 
			if(nIcon == 12 && money <= 150000000000) return TRUE; //1050조

			//예외 추가
			if(nIcon >= 12) return TRUE; // 고수 서버에는 도신도 입장가능
			
		} break;
	case 4: // 도신서버 
		{
			if(nIcon >= 12 && nIcon <= 14) return TRUE;
			
		} break;
	default : // 자유서버
		{
			return TRUE;
		}
		break;
	}

	return FALSE;

}



// 중간값을 기록한다
LONG C62CutPokerServerView::OnDbworkSetOK(UINT wParam, LONG lParam)
{
	return 1; // [어뷰저 백업 제거] 이하 처리하지 않는다

	/*
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memset(&Work,0,sizeof(DBWORKS));
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB 처리가 끝나기전에 접속이 끊어진 사용자일 수 있음
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	if(sid != (int)pSocket->m_dwUserVal){
		return 1;
	}

	// 사용중인 아이디인지 검색
	BOOL bConnected=FALSE;
	int  nFindUNum = 0;
	for(int i=0; i<MAX_USER; i++) {
		if(User[i].bValid ) {
//			if(faststrcmp(User[i].UI.ID, Work.UI.ID) == 0 && sid == User[i].Sid && Work.StartTick==User[i].UniqueTick) { // 정확한 접속여부 확인
			if(faststrcmp(User[i].UI.ID, Work.UI.ID) == 0 && sid == User[i].Sid) { // 정확한 접속여부 확인
				bConnected = TRUE; 
				nFindUNum  = i;
				break;
			}
		}
	}

	// 접속중인 아이디라면 처리한다
	if(bConnected && nFindUNum<MAX_USER){
		if(User[nFindUNum].bIsUpdateData){
			User[nFindUNum].bIsUpdateData = FALSE;
			if(Work.bDBSetOK){ // 성공적으로 데이타베이스에 기록되었다
				User[nFindUNum].UI.pBackup = Work.UI.PMoney; // 가감치용으로 최신 값 갱신
				User[nFindUNum].nGameNum   = 0;              // 게임횟수 재초기화 20 회
			}
		}
	}

	return 1;
*/
}


// 중복접속자이다!!
LONG C62CutPokerServerView::OnDbworkDoubleConnect(UINT wParam, LONG lParam)
{
//	CNetData reData; LPTSTR buf = _T("sign");

	return 1; // [어뷰저 백업 제거] 이하 처리하지 않는다

/*
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memset(&Work,0,sizeof(DBWORKS));
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB 처리가 끝나기전에 접속이 끊어진 사용자일 수 있음
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	if(sid != (int)pSocket->m_dwUserVal){
		return 1;
	}

	// 사용중인 아이디인지 검색
	BOOL bConnected=FALSE;
	int  nFindUNum = 0;
	for(int i=0; i<MAX_USER; i++) {
		if(User[i].bValid ) {
			if(faststrcmp(User[i].UI.ID, Work.UI.ID) == 0 && sid == User[i].Sid) { // 정확한 접속여부 확인
				bConnected = TRUE; 
				nFindUNum  = i;
				break;
			}
		}
	}

	// 접속중인 아이디라면 처리한다 (중복접속자이므로 종료처리한다!)
	if(bConnected && nFindUNum<MAX_USER){
		
		CSV_SERVERMSG smsg(SndBuf);
		CString exitbuf;
		exitbuf.Format(g_StrMan.Get(_T("SPOKER_DOUBLE_CON")),Work.ServerCode,Work.ServerCode); 
		smsg.Set(1,(char*)exitbuf.operator LPCTSTR());
		pSocket->Send(smsg.pData, smsg.GetTotalSize());
		//CString exitbuf;
		//exitbuf.Format(g_StrMan.Get(_T("SPOKER_DOUBLE_CON")),Work.ServerCode,Work.ServerCode); 
		//CString strSend;
		//strSend.Format(_T("$%3d%s"),strlen(exitbuf),exitbuf);
					
		//reData.SetData((LPVOID)strSend.operator LPCTSTR(),strSend.GetLength(),NM_QMDATA,NM_DISCONMES);
		//pSocket->Send(reData.GetData(),reData.GetSize());
		SockMan.ReservClose(sid); // 연결 종료 예약
	}

	return 1;
*/
}

//	[대화 저장]	
void C62CutPokerServerView::LoadCaptureTalkID()
{
	// 환경설정 파일을 연다
	ZeroMemory(&g_CaptureID, sizeof(g_CaptureID));
	FILE *fp = fopen( "CaptureID.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("Can't find [CaptureID.cfg] file!\nSetup CaptureID config!");
		return;
	}
	fread(&g_CaptureID, sizeof(g_CaptureID), 1, fp);
	fclose(fp);

}


//	[대화 저장]
void C62CutPokerServerView::OnSetCaptureid() 
{
	// TODO: Add your command handler code here
	CCaptureTalk dlg;
	dlg.DoModal();
}

//	[대화 저장]	 아이디인가? 아닌가?
BOOL C62CutPokerServerView::IsCaptureUserID(char *id)
{
	if(id==NULL) return FALSE;
	
	BOOL bCapture = FALSE;

	for(int i=0; i<g_CaptureID.nTotal; i++){//MAX_CAPTURE_NUM
		if(strlen(g_CaptureID.ID[i]) > 0) {
			if(faststrcmp(g_CaptureID.ID[i], id)==0){
				bCapture = TRUE;
				break;
			}
		}
	}
	return bCapture;
}


// [위치 저장] 
void C62CutPokerServerView::LoadSearchID()
{
		// 환경설정 파일을 연다
	ZeroMemory(&g_SearchID, sizeof(g_SearchID));
	FILE *fp = fopen( "SearchID.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("Can't find [SearchID.cfg] file!\nSetup SearchID config!");
		return;
	}
	fread(&g_SearchID, sizeof(g_SearchID), 1, fp);
	fclose(fp);

}


// [위치 저장] 
BOOL C62CutPokerServerView::IsSearchUserID(char *id)
{
	if(id==NULL) return FALSE;
	
	BOOL bCapture = FALSE;

	for(int i=0; i<g_SearchID.nTotal; i++){//MAX_CAPTURE_NUM
		if(strlen(g_SearchID.ID[i]) > 0) {
			if(faststrcmp(g_SearchID.ID[i], id)==0){
				bCapture = TRUE;
				break;
			}
		}
	}
	return bCapture;

}
// [위치 저장] 
#include "SearchDlg.h"
void C62CutPokerServerView::OnSetSearch() 
{
	// TODO: Add your command handler code here
	CSearchDlg dlg;
	dlg.DoModal();
	
}


// ### [이벤트 티켓] ###
void C62CutPokerServerView::OnViewConfigevent() 
{
	// TODO: Add your command handler code here
	CConfigEvent Dlg;
	Dlg.DoModal();
	
}

void C62CutPokerServerView::ReadUsedEventTicketNum()
{
	TotEventTicketNum = 0;
	FILE *fp = fopen( "eventnum.txt", "rb");
	if( fp == NULL ) return;
	fscanf(fp, "%d", &TotEventTicketNum);
	if(TotEventTicketNum < 0) TotEventTicketNum = 0;
	fclose(fp);
}

void C62CutPokerServerView::WriteUsedEventTicketNum()
{
	FILE *fp = fopen( "eventnum.txt", "wb");
	if( fp == NULL ) return;
	fprintf(fp, "%d", TotEventTicketNum);
	fclose(fp);
}

LONG C62CutPokerServerView::OnDbworkEventSet(UINT wParam, LONG lParam)
{
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB 처리가 끝나기전에 접속이 끊어진 사용자일 수 있음
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	int unum = pUserVal->iUser2;
	if(unum<1 || unum>=MAX_USER) return 1;
	if(!User[unum].bValid) return 1;

	// DB 처리가 끝나기전에 접속이 끊어진 직후 다른 사용자가 접속한 경우일 수 있음
	if(sid != (int)pSocket->m_dwUserVal) return 1;
	// DB처리결과와 실제 아이디가 틀림(이미 접속이 끊어졌음)
	if(Work.UI.UNum != unum) return 1;
	// DB처리결과와 실제 아이디가 틀림(이미 접속이 끊어졌음)
	if(faststrcmp(Work.UI.ID, User[unum].UI.ID) != 0) return 1;

	/////// 갱신
	User[unum].EventPrize.EventPrize = Work.EventPrize.EventPrize;
	User[unum].EventPrize.OrigEventPrize = Work.EventPrize.OrigEventPrize; //백업용
	User[unum].EventPrize.GameCnt = Work.EventPrize.GameCnt;
	
	return 1;
}


void C62CutPokerServerView::CheckGameEvent()
{
	if(Cfg.bEventDB != TRUE) return;
	if(EventCfg.bRunEvent != TRUE) return;

	// 이벤트 기간이 잘못 설정되어 있는지 검사
	if(EventCfg.StartDate > EventCfg.EndDate) return;

	// 이벤트 시작일과 종료일을 검사
	CTime nowtime = CTime::GetCurrentTime();
	if(nowtime < EventCfg.StartDate || nowtime > EventCfg.EndDate) {
		// 자동 이벤트 종료가 설정되어 있다면 이벤트 종료
		if(EventCfg.bAutoEventEnd) EventCfg.bRunEvent = FALSE;
		
		if( g_pMainView->Cfg.bEventDB && g_pMainView->EventCfg.bRunEvent ) {	// ### [이벤트 티켓] ###
			g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("▶ EventDB Use");
		} else if( g_pMainView->Cfg.bEventDB ) {
			g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("▷ EventDB Use");
		} else {
			g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("◐ EventDB Use");
		}
		return;
	}

	// 특정시간이 되면 이벤트정보 초기화
	SYSTEMTIME st;
	GetLocalTime(&st);
	if(st.wHour== 23 && st.wMinute == 59 && st.wSecond == 59 ) {
		for(int i=0; i<MAX_DATA_SOCKET; i++) {
			if(SockMan.pDataSock[i] != NULL) {
				if(!SockMan.pDataSock[i]->m_bCreated || !SockMan.pDataSock[i]->m_bIsOnline) continue;
				USERVAL *pUserVal = (USERVAL*)(SockMan.pDataSock[i]->m_pUserPtr);
				if(!pUserVal) continue;
				int unum = pUserVal->iUser2;
				if(pUserVal->iUser1 == TRUE) 
				{
					if(unum !=g_nAgentIndex)
					{
						User[unum].EventPrize.GameCnt = 0;
						User[unum].EventPrize.LastDate = st;
					}
					
				}
			}
		}
	}
}


void C62CutPokerServerView::OnButtonEventstate() 
{
	// TODO: Add your control notification handler code here
		CString strevent, str;

	if(EventCfg.bRunEvent != TRUE) {
		AfxMessageBox("이벤트가 설정되어 있지 않습니다.");
		return;
	}

	if(EventCfg.StartDate > EventCfg.EndDate) {
		AfxMessageBox("이벤트 기간이 잘못 설정되어 있습니다");
		return;
	}

	CTime nowtime = CTime::GetCurrentTime();
	if( nowtime < EventCfg.StartDate || nowtime > EventCfg.EndDate) {
		str.Format("◎ 이벤트 기간이 종료되었습니다 ◎\n\n");
		strevent += str;
	}
	else {
		BOOL bNoEventTime = FALSE;
		if(EventCfg.bNoEventTime) {
			// 이벤트 발생 금지 시작 시간
			CTime st(nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(), EventCfg.NoEventStartHour, 0, 0);
			// 이벤트 발생 금지 종료 시간
			CTime et = st;
			CTimeSpan tt(0, EventCfg.NoEventHour, 0, 0);
			et += tt;
			// 현재 시간이 이벤트 발생이 금지된 시간인지 검사
			if(nowtime >= st && nowtime <= et) 	bNoEventTime = TRUE;
		}

		if(bNoEventTime == FALSE) {
			str.Format("★ 현재 이벤트가 진행중입니다 ★\n\n");
			strevent += str;
		}
		else {
			str.Format("★ 현재 이벤트가 진행중이나 지금은 이벤트 발생이 금지된 시간입니다★\n\n");
			strevent += str;
		}
	}

	str.Format("총 지급된 티켓 수: %d/%d \n", TotEventTicketNum, EventCfg.EventTicketNum);
	strevent += str;
	str.Format("지급될 누적 티켓 수: %d \n", NowEventTicketNum);
	strevent += str;

	AfxMessageBox(strevent);
}


// ### [이벤트 티켓] ###
void C62CutPokerServerView::SendChannelEventNotice(int unum)
{
	// 이벤트가 진행중이지 않다면 리턴
	if(!g_pMainView->EventCfg.bRunEvent) return;

	if(unum < 1 || unum >= MAX_USER) return;
	if(User[unum].bValid==FALSE) return; // 잘못된 유저번호이다
	if(strlen(User[unum].UI.ID)==0) return;

	if(EventCfg.bRunEvent) {
		// 이벤트 공지가 있다면
		if(strlen(EventCfg.EventNotice) > 0) {
			// 사용자에게 서버메세지를 보냄
			
			CString str1 = EventCfg.EventNotice;
			CString str2;
			str2.Format("오늘 [%s]회원님이 획득한 이벤트 쿠폰은 %d장입니다. 포커2게임을  30판이상 하시면 이벤트 쿠폰을 3장 드립니다.(1일 1회) 이벤트 참여는 대박경매 페이지의 이벤트를 확인해 주세요~~!",
						User[unum].UI.ID, User[unum].EventPrize.EventPrize);
			str1+="\n";
			str1+=str2;
			
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(2, (char*)str1.operator LPCTSTR(), TRUE, RGB(200,0,0));
			User[unum].SendMsg(&smsg);
		}
	}
}

// 통계기록
void C62CutPokerServerView::LoadConfigStatis()
{
	// 환경설정 파일을 연다
	ZeroMemory(&StatisCfg, sizeof(CONFIGSTATIS));

	FILE *fp = fopen( "statistics.cfg", "rb");
	if( fp == NULL ) { 
		//초기값
		AfxMessageBox("통계기록용 설정 파일을 읽을 수 없습니다.\n다시 설정하세요.");
		return;
	}
	fread(&StatisCfg, sizeof(CONFIGSTATIS), 1, fp);
	fclose(fp);
}


// 통계기록
void C62CutPokerServerView::OnViewConfigsa() 
{
	// TODO: Add your command handler code here
	CConfigSADlg Dlg;
	Dlg.DoModal();
	
}
/*
// [ 통계기록 ]
void C62CutPokerServerView::Make_StatisticsLog(int Kind, int index, BOOL bIncrease, INT64 pmoney, INT64 usurymoney)
{
	if( !Cfg.bStatisticsDB || !StatisCfg.bSaveSetLog ) return;
	if(Kind == LEVEL_CONCURRENT_USER) { 
		// 11 동접 구하기, 올인수, 유저 접속시간
		int Level = User[index].m_UserLevel;
		if( bIncrease ) {
			m_StatisticsLog.Level_CurUser[ Level ] += 1;		// 동접수 증가
		} else {
			m_StatisticsLog.Level_CurUser[ Level ] -= 1;		// 동접수 감소
			if( User[index].UI.PMoney <=  0) m_StatisticsLog.Level_AllIn_User[ Level ] += 1; // 올인수 증가
			COleDateTime nt = COleDateTime::GetCurrentTime();
			COleDateTimeSpan span = nt - User[index].m_StartTime;
			if(User[index].OI.WinNum != User[index].UI.WinNum ||
				User[index].OI.LooseNum != User[index].UI.LooseNum ||
				User[index].OI.DrawNum != User[index].UI.DrawNum )
			{
				int time = span.GetSeconds();	
				m_StatisticsLog.Level_UseTime[ Level ] += time;	// 유저 접속시간
				m_StatisticsLog.Level_UseTimeUserCnt[ Level ] += 1;			
			}
		}
	} else if(Kind == LEVEL_USE_GAMECNT) {						// 13 누적게임수
		int Level = User[index].m_UserLevel;
		m_StatisticsLog.Level_UseGameCnt[ Level ] += 1;
	} else if(Kind == GRADE_AVERAGE_MONEY) {					// 등급별 게임머니, 게임비
		int grade = index; // 채널 구분
		if(pmoney > 10000000) {
			m_StatisticsLog.Grade_GameCnt[ grade ]	+= 1;			// 등급별 게임판수
			m_StatisticsLog.Grade_AverGameMoney[ grade ]+= pmoney	/ 10000000  ;	// 총 베팅머니
		}
		if(usurymoney > 1000) {
			m_StatisticsLog.Grade_UsuryCnt[ grade ]	+= 1;			// 등급별 게임판수
			m_StatisticsLog.Grade_UsuryMoney[ grade ]+= usurymoney	/ 1000;	// 게임비	
		}
	} else if(Kind == GRADE_MISSION) {							// 미션 성공, 지급 머니
		int grade = index; // 채널 구분
		m_StatisticsLog.Grade_MissonCnt[ grade ] += 1;
		m_StatisticsLog.Grade_MissonMoney[ grade ] += pmoney    / 100000000;
	} else if(Kind == GRADE_JACKPOT) {							// 잭팟 성공, 지급머니
		int grade = index; // 채널 구분
		m_StatisticsLog.Grade_JackPotCnt[ grade ] += 1;
		m_StatisticsLog.Grade_JackPotMoney[ grade ] += pmoney   / 100000000;
	}
}

// [ 통계기록 ]
void C62CutPokerServerView::Make_SetSADBWork(int Kind)
{
	LOGREC_DBWORKS SWork;
	ZeroMemory(&SWork, sizeof(LOGREC_DBWORKS));
	SWork.WorkKind = 2; 
	SWork.StartTick = timeGetTime();
	SWork.bValid = TRUE;
	SWork.StatisticsLog.Kind = Kind; 

	if(Kind == LEVEL_CONCURRENT_USER) {		// 동접
		for(int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Level_CurUser[i];
		}
	} else if(Kind == LEVEL_ALLIN_USER) {	// 올인유저
		for( int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Level_AllIn_User[i];
			m_StatisticsLog.Level_AllIn_User[i]= 0;	
		}
	} else if(Kind == GRADE_AVERAGE_MONEY) {// 등급별 게임판돈
		for(int i=0;i<5;i++) {
			if(m_StatisticsLog.Grade_GameCnt[i] > 0) {
				SWork.StatisticsLog.BigData[i]	= m_StatisticsLog.Grade_AverGameMoney[i]/m_StatisticsLog.Grade_GameCnt[i];
				SWork.StatisticsLog.SmallData[i]= m_StatisticsLog.Grade_GameCnt[i];
				m_StatisticsLog.Grade_AverGameMoney[i] = 0; 
				m_StatisticsLog.Grade_GameCnt[i] = 0;		
			}
		}
	} else if(Kind ==  LEVEL_USE_GAMECNT) { // 레벨별 누적게임수
		for( int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Level_UseGameCnt[i];
			m_StatisticsLog.Level_UseGameCnt[i] = 0;
		}
	} else if(Kind == LEVEL_USE_TIME) {		// 평균이용시간
		for( int i=0; i<20; i++) {
			if(m_StatisticsLog.Level_UseTimeUserCnt[i]>0) {
				SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Level_UseTime[i]/m_StatisticsLog.Level_UseTimeUserCnt[i];
				m_StatisticsLog.Level_UseTime[i]= 0;			
				m_StatisticsLog.Level_UseTimeUserCnt[i] = 0;	
			}
		}
	} else if(Kind == GRADE_USURY_MONEY) {  // 게임비 누적액
		for( int i=0; i<5; i++) {
			SWork.StatisticsLog.BigData[i]   = m_StatisticsLog.Grade_UsuryMoney[i];
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Grade_UsuryCnt[i];
			m_StatisticsLog.Grade_UsuryMoney[i] = 0;
			m_StatisticsLog.Grade_UsuryCnt[i] = 0;
		}

	} else if(Kind == GRADE_JACKPOT) {		// 잭팟 기록
		for( int i=0; i<5; i++) {
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Grade_JackPotCnt[i];
			SWork.StatisticsLog.BigData[i]	 = m_StatisticsLog.Grade_JackPotMoney[i];
			m_StatisticsLog.Grade_JackPotCnt[i]   = 0;
			m_StatisticsLog.Grade_JackPotMoney[i] = 0;
		}
	} else if(Kind == GRADE_MISSION) {		// 미션 기록
		for( int i=0; i<5; i++) {
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Grade_MissonCnt[i];
			SWork.StatisticsLog.BigData[i]	 = m_StatisticsLog.Grade_MissonMoney[i];
			m_StatisticsLog.Grade_MissonCnt[i]   = 0;
			m_StatisticsLog.Grade_MissonMoney[i] = 0;
		}
	}

	BOOL rtn = LogDBMan.SetDBWork(&SWork);
	if(rtn == FALSE) {
		CLogFile logfile("StatisticsLogDB.txt");
		logfile.Writef("Check_StatisticsLog() Error");
	}
}

// [ 통계기록 ]
void C62CutPokerServerView::Check_StatisticsLog()
{
	if( !Cfg.bStatisticsDB || !StatisCfg.bSaveSetLog ) return;
	
	CTime nt = CTime::GetCurrentTime();				// 현재의 실제 시간
	CTime st = m_StatisticsLog.PrevTime_1;			// 이전 기록 시간
	CTimeSpan TimeSpan = nt - st;					// 현재시간과의 차이를 구한다
	int passedS = TimeSpan.GetTotalSeconds();

	
	if(passedS >= 60*10) {					// 10분 마다 기록
		Make_SetSADBWork(LEVEL_CONCURRENT_USER);	// 동접
		Make_SetSADBWork(LEVEL_ALLIN_USER);			// 올인유저
		Make_SetSADBWork(LEVEL_USE_GAMECNT);		// 레벨별 누적게임수
		Make_SetSADBWork(GRADE_AVERAGE_MONEY);		// 등급별 게임판돈
		Make_SetSADBWork(GRADE_USURY_MONEY);		// 게임비 누적액
		
		// !!! 게임에 잭팟 없다면 주석 처리 할 것
		Make_SetSADBWork(GRADE_JACKPOT);			// 잭팟 기록
		// !!! 게임에 미션 없다면 주석 처리 할 것
		//Make_SetSADBWork(GRADE_MISSION);			// 미션 기록

		nt.GetAsSystemTime( m_StatisticsLog.PrevTime_1 );	// 시간 초기화
	}
	
	// 24시 10분에 저장한다.
	if(nt.GetHour() == 0 && nt.GetMinute() == 10 && nt.GetSecond() == 0 ) { 
		Make_SetSADBWork(LEVEL_USE_TIME);					// 평균이용시간
		nt.GetAsSystemTime( m_StatisticsLog.PrevTime_2 );	// 시간 초기화
	}
}

// [ 통계기록 ]
void C62CutPokerServerView::CloseServer_SALog()		// 서버 종료시 강제로 저장
{
	if( !Cfg.bStatisticsDB || !StatisCfg.bSaveSetLog ) return;

	Make_SetSADBWork(LEVEL_CONCURRENT_USER);	// 동접
	Make_SetSADBWork(LEVEL_ALLIN_USER);			// 올인유저
	Make_SetSADBWork(GRADE_AVERAGE_MONEY);		// 등급별 게임판돈
	Make_SetSADBWork(LEVEL_USE_GAMECNT);		// 레벨별 누적게임수
	Make_SetSADBWork(LEVEL_USE_TIME);			// 평균이용시간
	Make_SetSADBWork(GRADE_USURY_MONEY);		// 게임비 누적액
	
	// !!! 게임에 잭팟 없다면 주석 처리 할 것
	Make_SetSADBWork(GRADE_JACKPOT);			// 잭팟 기록
	
	// !!! 게임에 미션 없다면 주석 처리 할 것
	//Make_SetSADBWork(GRADE_MISSION);			// 미션 기록
	
	GetLocalTime( &m_StatisticsLog.PrevTime_2 );// 시간 초기화
	GetLocalTime( &m_StatisticsLog.PrevTime_1 ); // 시간 초기화
}
*/
//함수 수정
int C62CutPokerServerView::CheckTotalmoneylimit(USERINFO *pUI, char *uniqno, int bCheckWrite) // 0 체크, 1 머니제한 2 머니제한 기록
{
	if(pUI==NULL) return 0;
	int resMsg = 0;	
	int premLCode = pUI->PremLeadersCode;

	INT64 amount = 0;
	INT64 beforem = 0;
	INT64 afterm = 0;
	switch( premLCode )
	{
	case 0: // 일반회원
		{
			if(pUI->PMoney > g_pMainView->StatisCfg.NormalMoney) {
				if( bCheckWrite > 0  ) { 
					
					amount = g_pMainView->StatisCfg.NormalMoney - pUI->PMoney; // 차액
					beforem = pUI->PMoney; // 현재금액
					afterm = g_pMainView->StatisCfg.NormalMoney;
					
					pUI->PMoney = g_pMainView->StatisCfg.NormalMoney;
				}
				resMsg = 1;
			}
			break;
		}
	case 1: // 리더스회원
		{
			if(pUI->PMoney > g_pMainView->StatisCfg.LeadersMoney) {
				if( bCheckWrite > 0  ) { 
					
					amount = g_pMainView->StatisCfg.LeadersMoney - pUI->PMoney; // 차액
					beforem = pUI->PMoney; // 현재금액
					afterm = g_pMainView->StatisCfg.LeadersMoney;

					pUI->PMoney = g_pMainView->StatisCfg.LeadersMoney;
				}
				resMsg = 1;
			}
			break;
		}
	case 4: // 리더스플러스 회원
		{
			if(pUI->PMoney > g_pMainView->StatisCfg.LeadersPlusMoney) {
				if( bCheckWrite > 0  ) { 
					
					amount = g_pMainView->StatisCfg.LeadersPlusMoney - pUI->PMoney; // 차액
					beforem = pUI->PMoney; // 현재금액
					afterm = g_pMainView->StatisCfg.LeadersPlusMoney;

					pUI->PMoney = g_pMainView->StatisCfg.LeadersPlusMoney;
				}
				resMsg = 1;
			}
			break;
		}
	}
	
	if(bCheckWrite == 2  && uniqno!=NULL&&  resMsg == 1) { //첫 로긴시만 디비기록
		// 로그 기록에 남긴다.
		LOGREC_DBWORKS PWork;
		ZeroMemory(&PWork, sizeof(LOGREC_DBWORKS));
		PWork.WorkKind = 3; //2004.05.07 리더스총액 제한
		PWork.StartTick = timeGetTime();
		PWork.bValid = TRUE;

		strncpy(PWork.Pre_UserMLLog.ID, pUI->ID, 15);
		if(strlen(uniqno)!=0)
			strncpy(PWork.Pre_UserMLLog.NO, uniqno, 12);
		else
			strncpy(PWork.Pre_UserMLLog.NO, "NNNNNNNNNNNN", 12);
		PWork.Pre_UserMLLog.MoneyType = Cfg.GameCode;
		PWork.Pre_UserMLLog.Amount = amount;
		PWork.Pre_UserMLLog.BeforeM = beforem;
		PWork.Pre_UserMLLog.AfterM = afterm;

		// DB 쓰레드 가동
		BOOL rtn = LogDBMan.SetDBWork(&PWork);
		if(rtn == FALSE) {
			CLogFile logfile("PreUserLMLogDB.txt");
			logfile.Writef("총액 제한 로그기록  에러");
		}
	}


return resMsg;
}
void C62CutPokerServerView::SendPremLeadersMsg(int unum, int PremLeadersCode)
{
	if(unum < 1 || unum > MAX_USER) return;

	CSV_SERVERMSG smsg(SndBuf);
	CString str="";
	CString strM="";
	if(PremLeadersCode==0) {
		strM = MoneyToString(g_pMainView->StatisCfg.NormalMoney);
		str.Format("보유머니 %s원 이상은 획득할 수 없습니다.", strM );
		str+="리더스 서비스에 가입하시면, 더 많은 게임머니를 보유할 수 있게 됩니다.";
	} else if(PremLeadersCode==1) {
		strM = MoneyToString(g_pMainView->StatisCfg.LeadersMoney);
		str.Format("보유머니 %s원 이상은 획득할 수 없습니다.", strM );
		str+="리더스플러스로 상품 전환하시면, 더 많은 게임머니를 보유할 수 있게 됩니다.";
	} else if(PremLeadersCode==4) {
		strM = MoneyToString(g_pMainView->StatisCfg.LeadersPlusMoney);
		str.Format("보유머니 %s원 이상은 획득할 수 없습니다.", strM);
	}
	smsg.Set(11, (char*)str.operator LPCTSTR());
	User[unum].SendMsg(&smsg);

}

CString C62CutPokerServerView::MoneyToString(INT64 number)
{
	CString strrtn;
	CString strtemp;
	INT64 t = number; 

	BOOL bMinus = FALSE;
	if( t < 0 ) {
		bMinus = TRUE;
		t *= -1;
	}

	INT64 v = t / (INT64)10000000000000000;	// 경 단위
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		strtemp.Format("%I64d경", v);
		strrtn += strtemp;
	}

	v = t / (INT64)1000000000000;	// 조 단위
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		strtemp.Format("%I64d조", v);
		strrtn += strtemp;
	}

	v = t / (INT64)100000000;	// 억 단위
	if(v > 0) {
		t = t - v*(INT64)100000000;
		strtemp.Format("%I64d억", v);
		strrtn += strtemp;
	}
	
	v = t / (INT64)10000;	// 만 단위
	if(v > 0) {
		t = t - v*(INT64)10000;
		strtemp.Format("%I64d만", v);
		strrtn += strtemp;
	}

	if( t > 0 ) {
		strtemp.Format("%I64d", t);
		strrtn += strtemp;
	}

	if( number == 0 )
	{
		strrtn = "0";
	}

	if( bMinus )
	{
		strrtn.Insert(0,"-");
		//CString temp;
		//temp.Format( "-%s", strrtn );
		//strrtn = temp;
	}
	
   	return strrtn;
}
// [수호천사] 2004.07.08
INT64 C62CutPokerServerView::Get_angel_defmoney(const INT64 &lmoney) 
{
	if(lmoney<=StatisCfg.MinMoneyLimit)	  return StatisCfg.MinMoneyLimit;
	else if(lmoney>=StatisCfg.MaxMoneyLimit) return StatisCfg.MaxMoneyLimit;
	else return lmoney;
	return (INT64)0;
}