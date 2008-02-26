// 62CutPokerServerView.cpp : implementation of the C62CutPokerServerView class
//

#include "stdafx.h"
#include "62CutPokerServer.h"

#include "62CutPokerServerDoc.h"
#include "62CutPokerServerView.h"

// [���� ������Ʈ �۾�] 
#include "MainFrm.h"

#include "ConfigDlg.h"
#include "Config2Dlg.h"
#include "Config3Dlg.h"
#include "ConfigJackPotDlg.h"
#include "ConfigEvent.h" // ### [�̺�Ʈ Ƽ��] ###
#include "StartUpDlg.h"
#include "DBThread.h"
#include "AdminManager.h"	 // [������ ��� �۾�]
#include "CaptureTalk.h" // [��ȭ ����]	
#include "ConfigSADlg.h" // [ ����� ]

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ���� ����
C62CutPokerServerView *g_pMainView=NULL;
CDBMan DBMan;					// DB �޴��� ���� ��ü
CSockMan SockMan;				// ���� �޴��� ���� ��ü
CChannel Chan[MAX_CHAN];		// ä�� Ŭ����
CUser User[MAX_USER];			// ����� Ŭ����
CNMStrMan g_StrMan;				// ���(���ڿ�)
POKERSET g_POKERSET;			// [������Ŀ] ���� �Ӵ�����

// ### [�α� ��Ͽ�] ###
CLogManDBMan LogDBMan;
INT64 g_LogRangeValue;

// [ ###����### ]
INT64 g_JackPotMoney;			// ���� �Ӵ�
int g_ServerGrade;				// ���

ADMIN_MANAGER             g_AdminInfo; // [������ ��� �۾�]
ADMIN_ACCESSDENY_USER	 g_AccessDeny[MAX_DENY_USER];

int  g_nAgentIndex = -1; // [���� ������Ʈ �۾�]
CString      g_AppStartPath  = _T("");
extern C62CutPokerServerApp theApp;			// [PC�� �۾�]

CAPTURE_INFO       g_CaptureID; // [��ȭ ����]	
SERACH_INFO        g_SearchID; // [��ġ ����]

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
	ON_MESSAGE(UM_DBWORK_USEITEMRESULT, OnDbworkUseitemresult)	// [���� ������ �۾�]
	ON_MESSAGE(UM_DBWORK_GAMEITEMLIST, OnDbworkGameitemlist)	// [���� ������ �۾�]
	ON_COMMAND(IDM_SET_ADMIN, OnSetAdmin)
	ON_BN_CLICKED(IDC_BTN_DENYUSER, OnBtnDenyuser)
	ON_COMMAND(IDM_SET_CAPTUREID, OnSetCaptureid)
	ON_COMMAND(IDM_SET_SEARCH, OnSetSearch)
	ON_COMMAND(ID_VIEW_CONFIGEVENT, OnViewConfigevent)      // ### [�̺�Ʈ Ƽ��] ###
	ON_BN_CLICKED(IDC_BUTTON_EVENTSTATE, OnButtonEventstate)// ### [�̺�Ʈ Ƽ��] ###
	ON_MESSAGE(UM_DBWORK_EVENTSET, OnDbworkEventSet) // ### [�̺�Ʈ Ƽ��] ###
	ON_COMMAND(ID_VIEW_CONFIGSA, OnViewConfigsa)	// [ ����� ]
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
	// [ ###����### ]
	g_JackPotMoney = 0;
	// ### [�α� ��Ͽ�] ###
	g_LogRangeValue = 0;

	// ### [�̺�Ʈ Ƽ��] ###
	TotEventTicketNum = 0;
	NowEventTicketNum = 0;
	LastEventTick = timeGetTime();

	////////////////////////////////////////////////////////////////
	///// �ڵ�����
	hExitReserveTimer = NULL;
	m_bExitReserve = FALSE;
	m_ReservTimeLimit = 0;
	///////////////////////////////////////////////////////////////	
	hMsgUerCntTimer = NULL;		// [PC�� �۾�]

	// [ ����� ]
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


//	SaveCurrentDir(); // ���� ���丮�� �����Ѵ�

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

	// �α� ��� ���丮 ���� �� ã��
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
	// [��ȭ ����] �Ƶ�
	LoadCaptureTalkID();
	// [��ġ ����] ���̵�
	LoadSearchID();


	// [������ ��� �۾�]
	LoadAdminInfo();
	for(int i=0; i<MAX_DENY_USER; i++) memset(&g_AccessDeny[i],0,sizeof(ADMIN_ACCESSDENY_USER));

	// ȯ�� ���������� �д´�.
	LoadConfig();
	LoadConfig2();
	LoadConfig3();
	LoadConfig4();
	LoadConfigEvent();// ### [�̺�Ʈ Ƽ��] ###
	LoadConfigStatis(); // �����

		/*
	// [PC�� �۾�] - �̺κ��� �ּ�ó��
	CStartUpDlg StartUpDlg;

	if(StartUpDlg.DoModal() == IDCANCEL) {
		// ������ ���� �����Ѵ�
		bIsForceClose = TRUE;
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return;
	}
	else {
		// �ٽ� ȯ�� ���������� �д´�.
		LoadConfig();
		LoadConfig2();
		LoadConfig3();
		LoadConfig4();
	}
	*/

	// [PC�� �۾�] - �̷��� �ٲ۴�~
	InitNetmarbleCommn();
	BOOL bAutoRun = FALSE;
	CStartUpDlg StartUpDlg;
	if(theApp.m_lpCmdLine[0] != '\0' && strcmp(theApp.m_lpCmdLine,"nonstop")==0) {
		StartUpDlg.m_bInitConnectStatus = FALSE;
	} 
	else {
		if(StartUpDlg.DoModal() == IDCANCEL) {
			// ������ ���� �����Ѵ�
			bIsForceClose = TRUE;
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
			return;
		}
		else {
			// �ٽ� ȯ�� ���������� �д´�.
			LoadConfig();
			LoadConfig2();
			LoadConfig3();
			LoadConfig4();
			LoadConfigEvent();// ### [�̺�Ʈ Ƽ��] ###
			LoadConfigStatis(); // �����
		}
	}



	g_ServerGrade = 0;
	// ������ ���
	if(Cfg3.EnterLimitMode == 1 && Cfg3.bLoginLimit) {
		switch(Cfg3.LoginGrade)
		{
		case 0: // ����
			{
				g_ServerGrade = 0;
				Cfg3.LoginLimitLevel1 = 0;
				Cfg3.LoginLimitLevel2 = 14;
			}
			break;
		case 1: // �ʺ�
			{
				g_ServerGrade = 1;
				Cfg3.LoginLimitLevel1 = 0;
				Cfg3.LoginLimitLevel2 = 1;
			}
			break;
		case 2: // �߼�
			{
				g_ServerGrade = 2;
				Cfg3.LoginLimitLevel1 = 2;
				Cfg3.LoginLimitLevel2 = 7;
			}
			break;
		case 3: // ���
			{
				g_ServerGrade = 3;
				Cfg3.LoginLimitLevel1 = 8;
				Cfg3.LoginLimitLevel2 = 11;
			}
			break;
		case 4: // ����
			{
				g_ServerGrade = 4;
				Cfg3.LoginLimitLevel1 = 12;
				Cfg3.LoginLimitLevel2 = 14;
			}
			break;
		
		default: //���� 0, 1
			{
				g_ServerGrade = 0;
				Cfg3.LoginLimitLevel1 = 0;
				Cfg3.LoginLimitLevel2 = 14;
			}
			break;
		}
	}


	// ä�� Ŭ���� �ʱ�ȭ
	for( i=0; i<MAX_CHAN; i++) 
		Chan[i].Init(i);

	TotUser = 0;
	RealUser = 0;

	// ���� �ʱ�ȭ
	srand((unsigned)timeGetTime());

	// ���� �޼��� ������ ����Ʈ�� ��ư���� �ʱ�ȭ
	SendMsgMode = 0;
	CheckRadioButton(IDC_RADIO_SENDTEXT, IDC_RADIO_SENDMSGBOTH, IDC_RADIO_SENDTEXT);
	ClientMode = 1;
	IPMode = 0;
	
	////////////////////////////////////////////////////////////////
	///// �ڵ�����
	// ���� ���� �޼����ʱ�ȭ, ��ư
	SendExitMode = 2;
	m_ReservTimeLimit = 300; // �⺻ ���� 3��
	m_iExitRadio = 2;
	UpdateData(FALSE);
	m_ExitCancel.EnableWindow(FALSE);
	/////////////////////////////////////////////////////////////////

	// ���� �޴��� �ʱ�ȭ
	SockMan.Init(m_hWnd);

	// ���� ���� �����ֱ�
	ShowServerState();

	// �޽��� ���̾�α� ����
	MsgDlg.Create(this, "DB �޴����� �ʱ�ȭ�ϴ� ���Դϴ�");

	// DB �޴����� �ʱ�ȭ �Ѵ�
	if(DBMan.Init(Cfg2.MaxDBThread)==FALSE) {
		UINT rtn = AfxMessageBox("������ ���̽��� �� �� �����ϴ�. ���� ȯ�漳���� �Ͻðڽ��ϱ�?", MB_YESNO);
		if(rtn==IDYES) {
			CConfigDlg Dlg;
			Dlg.DoModal();

			// ������ ���� �����Ѵ�
			AfxMessageBox("����� ������ �����Ű�����Ͽ� ���� ������ �����մϴ�.");
			MsgDlg.DestroyWindow();
			bIsForceClose = TRUE;
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
			return;
		}
	}

#ifdef LOG_RECORD_
	if(g_pMainView->Cfg.bLogUseDB || g_pMainView->Cfg.bStatisticsDB || g_pMainView->Cfg.bAllinDB)
	{
		MsgDlg.SetMsg("�α� DB �޴����� �ʱ�ȭ�ϴ� ���Դϴ�.");
		// ### [�α� ��Ͽ�] ###
		if(LogDBMan.Init(Cfg2.MaxDBThread2)==FALSE) {
			UINT rtn = AfxMessageBox("������ ���̽��� �� �� �����ϴ�. ���� ȯ�漳���� �Ͻðڽ��ϱ�?", MB_YESNO);
			if(rtn==IDYES) {
				CConfigDlg Dlg;
				Dlg.DoModal();
				
				// ������ ���� �����Ѵ�
				AfxMessageBox("����� ������ �����Ű�����Ͽ� ���� ������ �����մϴ�.");
				MsgDlg.DestroyWindow();
				bIsForceClose = TRUE;
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return;
			}
		}
	}
	
#endif


	// ���� �ڵ带 ����ϴ� �ٸ� ���������� �����ϴ��� �˻��Ѵ�
	if(Cfg.GameCode!=0 && Cfg.ServerCode!=0) {
		SockMan.CheckHostInfo();
		if(DBMan.CheckOtherServerExist(Cfg.GameCode, Cfg.ServerCode, SockMan.GetServerIP(), Cfg.ServerPort)) {
			CString strmsg;
			strmsg.Format("�����ڵ� %d�� �����ڵ�(ä��) %d���� ����ϴ� IPȤ�� ��Ʈ��ȣ�� �ٸ� ���������� �����մϴ�.\nȯ�漳���� ���Ͽ� ���� ������ �����Ϸ��� [��]�� Ŭ���Ͻʽÿ�.\n������ ������ �����Ű�� ���ؼ��� [�ƴϿ�]�� Ŭ���Ͻʽÿ�.\n���� ȯ�漳���� �Ͻðڽ��ϱ�?", Cfg.GameCode, Cfg.ServerCode);
			UINT rtn = AfxMessageBox(strmsg, MB_YESNO);
			if(rtn==IDYES) {
				CConfigDlg Dlg;
				Dlg.DoModal();

				// ������ ���� �����Ѵ�
				AfxMessageBox("����� ������ �����Ű�����Ͽ� ���� ������ �����մϴ�.");
				MsgDlg.DestroyWindow();
				bIsForceClose = TRUE;
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return;
			}
		}
	}

	MsgDlg.SetMsg("DB�����带 �����ϰ� ���̺��� �������Դϴ�.");

	// ���� ����� DB�����带 �����Ѵ�
	if(DBMan.CreateKeepAliveThread()==FALSE) {
		UINT rtn = AfxMessageBox("DB������ ������ OPEN�� ������ ���̺��� �����մϴ�.\n���� ȯ�漳���� �Ͻðڽ��ϱ�?", MB_YESNO);
		if(rtn==IDYES) {
			CConfigDlg Dlg;
			Dlg.DoModal();

			// ������ ���� �����Ѵ�
			AfxMessageBox("����� ������ �����Ű�����Ͽ� ���� ������ �����մϴ�.");
			MsgDlg.DestroyWindow();
			bIsForceClose = TRUE;
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
			return;
		}
	}

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB || g_pMainView->Cfg.bStatisticsDB)
{
	MsgDlg.SetMsg("�α�DB�����带 �����ϰ� ���̺��� �������Դϴ�.");
	// ### [�α� ��Ͽ�] ###
	//���� ����� DB�����带 �����Ѵ�
	if(LogDBMan.CreateKeepAliveThread()==FALSE) {
		UINT rtn = AfxMessageBox("DB������ ������ OPEN�� ������ ���̺��� �����մϴ�.\n���� ȯ�漳���� �Ͻðڽ��ϱ�?", MB_YESNO);
		if(rtn==IDYES) {
			CConfigDlg Dlg;
			Dlg.DoModal();

			// ������ ���� �����Ѵ�
			AfxMessageBox("����� ������ �����Ű�����Ͽ� ���� ������ �����մϴ�.");
			MsgDlg.DestroyWindow();
			bIsForceClose = TRUE;
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
			return;
		}
	}
}
#endif

	MsgDlg.SetMsg("������ ��Ȳ DB�� �ʱ�ȭ�մϴ�.");
	if(Cfg.GameCode!=0 && Cfg.ServerCode!=0)
	{
		SockMan.CheckHostInfo();
		BOOL rtn = DBMan.InitStatusDB(Cfg.GameCode, Cfg.ServerCode, SockMan.GetServerIP(), Cfg.ServerPort);
		if(rtn==FALSE) AfxMessageBox("������ ��Ȳ DB�ʱ�ȭ�� �����Ͽ����ϴ�");
	}

	if(StartUpDlg.m_bInitConnectStatus) {
	/*
		MsgDlg.SetMsg("�����ڵ�� ä�ι�ȣ�� �˻��մϴ�.");
	*/
	}

	MsgDlg.SetMsg("�ҷ� ���̵� �Ű� DB�����带 �����մϴ�.");

	// �ҷ� ���̵� �Ű� DB������ ����
	BeginBadUserDBThread();

	MsgDlg.SetMsg("�ӽ÷� ������ DB�� Close�մϴ�.");

	// �ӽ÷� ������ DB�� �ݴ´�
	DBMan.CloseTempDB();


#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB || g_pMainView->Cfg.bStatisticsDB)
{
	// ### [�α� ��Ͽ�] ###
	/////////////////////////////////////
	// �ӽ÷� ������ DB�� �ݴ´�
	LogDBMan.CloseTempDB();
	/////////////////////////////////////
}
#endif

	// �޽��� ���̾�α� ����
	MsgDlg.DestroyWindow();

	// ������ ������ �����Ѵ�
	if(SockMan.CreateServerSocket(Cfg.ServerPort)==FALSE) {
		AfxMessageBox("������ ������ ������ ������ �� �����ϴ�.\n���� ��Ʈ��ȣ�� ����ϰ� �ִ� ���ø����̼��� �����ϴ��� Ȯ���Ͻñ� �ٶ��ϴ�");
	}

	CLogFile logfile;
	logfile.Writef(" ���� ���� - GameCode=%d, ServerCode=%d", Cfg.GameCode, Cfg.ServerCode);

	// ���� ���� Ÿ�̸� ����
	hGlobalGameTimer = SetTimer(GLOBALGAME_TIMER, 1000, NULL);
	// ���� �޽��� Ǯ�� Ÿ�̸� ����
	hMsgPoolingTimer = SetTimer(MSGPOOLING_TIMER, 1000/12, NULL);
	
	// [ ###����### ]
	CString strM = g_MakeCommaMoney(g_JackPotMoney);
	ShowState((char*)strM.operator LPCTSTR());

	// [PC�� �۾�]
	M_ImHereBridge(AfxGetMainWnd()->m_hWnd,g_pMainView->Cfg.GameCode,g_pMainView->Cfg.ServerCode,g_pMainView->Cfg.ServerPort,g_pMainView->Cfg.ServerName);
	hMsgUerCntTimer = SetTimer(USERCNT_TIMER, 10000, NULL);

	/////////////////////////////////////////////////
	// [���� ������Ʈ �۾�] ó�� �ù� ����� ���Ӱź� ����........
	CMainFrame *pMain = (CMainFrame*) GetParentFrame();
	pMain->Connectdeny();
	//SendMessage(WM_COMMAND,ID_FILE_CONNECTDENY);
	ShowState(_T("������� ������ �ź��մϴ�. ������Ʈ�� ������ �ֽʽÿ�." ));
	/////////////////////////////////////////////////

	// [ ����� ]
	SYSTEMTIME st;	GetLocalTime(&st);
//	m_cSALog.PrevTime_1 = st;
//	m_cSALog.PrevTime_2 = st;

	//////////
	// ����

	GetDlgItem(IDC_EDIT_BSSO)->SetWindowText("��[ DB Base ]");

	if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse)
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("�� LogDB Use");
	else if(g_pMainView->Cfg.bLogUseDB)
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("�� LogDB Use");
	else
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("�� LogDB Use");


	if( g_pMainView->Cfg.bEventDB && g_pMainView->EventCfg.bRunEvent ) {	// ### [�̺�Ʈ Ƽ��] ###
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("�� EventDB Use");
	} else if( g_pMainView->Cfg.bEventDB ) {
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("�� EventDB Use");
	} else {
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("�� EventDB Use");
	}
	////////


	// AI, Ŭ���̾�Ʈ ���� ���� ���� - jeong

	Sleep(50);
	::ShellExecute(NULL, "open", "NewPokerAI_Debug.exe", NULL, ".", SW_HIDE);
	//Sleep(1000);
	//::ShellExecute(NULL, "open", "NewPoker_Debug.exe", NULL, ".", SW_HIDE);
	
}


// ���� ���¸� ���δ�
void C62CutPokerServerView::ShowServerState()
{
	
	CString str;
	// ���� Ÿ��Ʋ�ٿ� ���α׷� �̸��� �Բ� ä�ι�ȣ�� ����
	AfxGetMainWnd()->GetWindowText(str);
	CString tmpstr;
	//tmpstr.Format(" %d (Ver%4.2f) ����Ŀ(7 Poker) �����Դϴ�", Cfg.ServerCode, VERSION/100.0f);
	tmpstr.Format(" �� 7 ��Ŀ (Ver%4.2f) �����Դϴ�", VERSION/100.0f);

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

	GetDlgItem(IDC_EDIT_AGENTCONNECT)->SetWindowText(_T("��No Agent ")); // [���� ������Ʈ �۾�]

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
	// ȯ�漳�� ������ ����
	ZeroMemory(&Cfg, sizeof(Cfg));

	FILE *fp = fopen( "62CutPoker.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("ȯ�漳�� ������ ���� �� �����ϴ�.\n�ٽ� �����ϼ���.");
		return;
	}
	fread(&Cfg, sizeof(CONFIG), 1, fp);
	fclose(fp);
}

void C62CutPokerServerView::LoadConfig2()
{
	// ȯ�漳�� ������ ����
	ZeroMemory(&Cfg2, sizeof(Cfg2));

	FILE *fp = fopen( "game.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("���ӿɼ� ���� ������ ���� �� �����ϴ�.\n�ٽ� �����ϼ���.");
		return;
	}
	fread(&Cfg2, sizeof(CONFIG2), 1, fp);
	fclose(fp);
}

void C62CutPokerServerView::LoadConfig3()
{
	// ȯ�漳�� ������ ����
	ZeroMemory(&Cfg3, sizeof(Cfg3));
	FILE *fp = fopen( "channel.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("ä�� ���� ������ ���� �� �����ϴ�.\n�ٽ� �����ϼ���.");
		return;
	}
	fread(&Cfg3, sizeof(CONFIG3), 1, fp);
	fclose(fp);

	
////////////////////////
	// 7 ��Ŀ // ���� �Ұ�
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
	// ȯ�漳�� ������ ����
	ZeroMemory(&Cfg4, sizeof(Cfg4));
	FILE *fp = fopen( "JackPot.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("���̸Ӵ� ������ ���� �� �����ϴ�.\n�ٽ� �����ϼ���.");
		return;
	}
	fread(&Cfg4, sizeof(CONFIG4), 1, fp);
	fclose(fp);

	// [ ###����### ]
	g_JackPotMoney = Cfg4.nJackPotMoney;
	// ### [�α� ��Ͽ�] ###
	g_LogRangeValue = (INT64)Cfg4.Range * M_1Y;
}

// ### [�̺�Ʈ Ƽ��] ###
void C62CutPokerServerView::LoadConfigEvent()
{
	// ȯ�漳�� ������ ����
	ZeroMemory(&EventCfg, sizeof(CONFIGEVENT));

	FILE *fp = fopen( "event.cfg", "rb");
	if( fp == NULL ) { 
		//�ʱⰪ
		SYSTEMTIME st= {0,};
		st.wYear = 2003;
		st.wMonth = 1;
		st.wDay = 1;
		if(EventCfg.StartDate == 0) EventCfg.StartDate = st;
		if(EventCfg.EndDate == 0)	EventCfg.EndDate = st;
		AfxMessageBox("�̺�Ʈ ���� ������ ���� �� �����ϴ�.\n�ٽ� �����ϼ���.");
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
		TRACE("���� ��û�� �������� ���� ��������\n");
		return 1;
	}

	int sid = SockMan.AcceptSocket();
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	if( sid < 0 || pSocket==NULL) {
		TRACE("���� ��û�� �������� ���� ��������\n");
		return 1;
	}
	else
	{
		// ����� ���� ��� ó��
		CString str;
		TotUser++;
		str.Format("%d��(%d)",TotUser,RealUser);
		((CWnd*)GetDlgItem(IDC_STATIC_TOTALUSER))->SetWindowText(str);

		// [���� ������Ʈ �۾�] ������Ʈ�� �����
		// ���� �ź� ���¶�� �ź� �޽����� ������ ������ ����
		if(bConnectDeny && g_nAgentIndex != -1) { // ������Ʈ�� �����ϱ� ���Ͽ� �ӽ÷� ������ ����Ѵ�.
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, "���� ����� ���Ͽ� ������ ������ �� �����ϴ�.\n����� �����Ͻðų� �ٸ� ������ �̿����ּ���.");
			pSocket->Send(smsg.pData, smsg.GetTotalSize());
			pSocket->ReservClose();
			return 1;
		}

		// ������ ���� �ο� �ʰ�
		if(RealUser >= Cfg2.MaxUser) {
			CSV_SERVERMSG smsg;
			smsg.Set(1, "�ִ� �����ο��� �ʰ��Ͽ����ϴ�. �ٸ������� �̿����ּ���.");
			pSocket->Send(smsg.pData, smsg.GetTotalSize());
			pSocket->ReservClose();
			return 1;
		}

		// ���� üũ ���� ����
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

	if(bufsize>=PACKET_HEADERSIZE) // ����� �ʿ��� ����� �޾�����
	{
		pSock->m_RecvQue.PeekQue(lpBuf, PACKET_HEADERSIZE);
			
		// �ùٸ� ������� üũ�ϰ� ��� ������ ����
		if(TmpMsg.CheckInfo(lpBuf, pSignal, &msglen)==TRUE) {
			int totsize = PACKET_HEADERSIZE + msglen;

			// �䱸�Ǵ� ����� ���ۺ��� ū ��� ������ �������
			if(totsize<0 || totsize > RECVQUESIZE) {
				TRACE("�䱸�Ǵ� ��Ŷ�� ����� �ʹ� ũ�ų� 0���� �۾Ƽ� ������ ����\n");
				return -1;
			}

			if(bufsize >= totsize) {
				// ���� ť���� ������ ī�ǿ� ���ÿ� ����
				if(pSock->m_RecvQue.DeQue(MsgBuf, totsize)==FALSE) {
					TRACE("��Ŷ DeQue ����\n");
					return -1;
				}

				// ��ȣȭ�� �����ϸ� ������ ����
				if(!TmpMsg.DecodeAll(lpBuf)) 
					return -1;
				
				return totsize;
			}
		}
		else { 
			TRACE("�߸��� ���ú� �����Ͱ� �߻��Ͽ� ������ ����\n");
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

	//(�߰�)
	// ��Ŷ ī���͸� �˻��Ͽ� �߸��� ��� ������ ����(��Ŷ ������ ������)
	pUserVal->RecvPackCnt++;
	if(pUserVal->RecvPackCnt != ((PACKETHEADER*)lpdata)->PackCnt) {
	//	pSocket->PostClose();
		pSocket->ReservClose();
		return TRUE;
	}

	// Ÿ�� �ƿ� ī��Ʈ �ʱ�ȭ
	pUserVal->iTimeOutCnt = 0;

	switch(signal) // �޼��� ��ȣ
	{

	case SV_HEARTBEAT:
		{

		} break;
	
	case SV_ASK_LOGIN:  // �α��� ��û
		{
			CSV_ASK_LOGIN MsgData;
			MsgData.Get(lpdata, totsize);
			
			// ����� ID ���� �˻�
			// MsgData.Ver   : ���α׷� ����
			// MsgData.ID    : ����� ���̵�
			// MsgData.Pass  : �н� ����
			// MsgData.RandomID  : Ȩ������ ���� �ڵ�
			// MsgData.IP    : ������� IP�ּ�
			// MsgData.Port  : ������� ��Ʈ��ȣ

/*
			// [������ ��� �۾�] : ���� �������� ������̴�
			UINT nDenySec = 0;
			if(IsAccessDeny(MsgData.ID,nDenySec)){
				CSV_SERVERMSG smsg(SndBuf);
				CString exitbuf;
				exitbuf.Format("%s ���̵�� �ش缭���� �������� ���Դϴ�![�ܿ��ð�:%d��]",MsgData.ID,nDenySec);
				smsg.Set(1,(char*)exitbuf.operator LPCTSTR());
				pSocket->Send(smsg.pData, smsg.GetTotalSize());
				SockMan.ReservClose(sid); // ���� ���� ����
				break;
			}
*/			
			// ����� ID ���� �˻�
			// MsgData.pAL->ID				: ����� ���̵�
			// MsgData.pAL->Pass			: �н� ����
			// MsgData.pAL->AuthCookieLen	: ���� ��Ű�� ����
			// MsgData.pAL->DataCookieLen	: ������ ��Ű�� ����
			// MsgData.AuthCookie			: ���� ��Ű(����! NULL�� ������ �ʴ´�)
			// MsgData.DataCookie			: ������ ��Ű(����! NULL�� ������ �ʴ´�)

			/* - jeong
			// [���� ������Ʈ �۾�] 
			if(g_nAgentIndex == -1 && strcmp(MsgData.pAL->ID,CONNECT_ID)!=0)
			{
				CSV_SERVERMSG smsg(SndBuf);
				smsg.Set(1, "���� ����� ���Ͽ� ������ ������ �� �����ϴ�.\n����� �����Ͻðų� �ٸ� ������ �̿����ּ���..... ");
				pSocket->Send(smsg.pData, smsg.GetTotalSize());
				SockMan.ReservClose(sid); // ���� ���� ����
				ShowState(_T("������Ʈ ����� ���Ͽ� ������� ������ �ź��մϴ�." ));
				break;
			}
			*/
			
			// ������ ���� �ο� �ʰ�
			if(RealUser >= Cfg2.MaxUser || RealUser >= MAX_USER-1) {
				CSV_REFUSE_LOGIN rlmsg(SndBuf); // �α��� �ź� �޼���
				rlmsg.Set(3, "���ӷ��� ���� ������ ������ �� �����ϴ�.\n����� ������ �Ͻðų� �ٸ� ä���� �̿��ϼ���.");	// [SSO �۾�]
				pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
				SockMan.ReservClose(sid); // ���� ���� ����
				break;
			}


			////////////////////////////////////////////////////////////////////////////////////////
			// [���� ������Ʈ �۾�] : ������ ����Ѵ�!!   //������Ʈ ���� - jeong
			//if(strcmp(MsgData.pAL->ID,CONNECT_ID)==0 && strcmp(MsgData.pAL->Pass,CONNECT_PASS)==0)
			//{

				// ����ִ� ����� ��ȣ �˻�
				int unum = -1;
				for(int i=1; i<MAX_USER; i++) if(User[i].bValid==FALSE) { unum=i; break;}

				// ����� �����ο� �ʰ�
				if(unum == -1) {
					CSV_REFUSE_LOGIN rlmsg(SndBuf); // �α��� �ź� �޼���
					rlmsg.Set(3, "���ӷ��� ���� ������ ������ �� �����ϴ�.\n����� ������ �Ͻðų� �ٸ� ä���� �̿��ϼ���.");	// [SSO �۾�];
					pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
					SockMan.ReservClose(sid); // ���� ���� ����
					break;
				}

				// ������� ���̵����� �˻�
				BOOL bConnected=FALSE;
				for( i=0; i<MAX_USER; i++){
					if(strcmp(User[i].UI.ID, MsgData.pAL->ID) == 0){
						bConnected = TRUE;
						break;
					}
				}
				// �̹� ������� ���̵�
				if(bConnected){
					CSV_REFUSE_LOGIN rlmsg(SndBuf); // �α��� �ź� �޼���
					rlmsg.Set(2, "�̹� ������� ���̵��Դϴ�.\n�����ڿ��� �����ϼ���");	// [SSO �۾�]
					pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
			
					SockMan.ReservClose(sid); // ���� ���� ����
					break;
				}
				
				// ���� ó�� �ϱ�
				pUserVal->iUser1 = TRUE; // �ùٸ� ������ ������ �������� ��Ÿ����.
				pUserVal->iUser2 = unum; // ����� ���� ��ȣ(���� Ŭ������ ����)
				pUserVal->iUser3 = (int)timeGetTime();// ������ ����� ������ Time Tick�� ���(��Ȯ�� �Ǻ� �� Ÿ�� üũ��)

				USERINFO ui;
				memset(&ui,0,sizeof(USERINFO));
				ui.UNum = unum;	               // ���� ��ȣ
				strncpy(ui.ID,CONNECT_ID,15);  // ���Ӿ��̵�
//				memset(ui.Pass, 0, sizeof(ui.Pass));// ��й�ȣ �ʵ� �ʱ�ȭ
				ui.PMoney =  (INT64)1000000;

				DBWORKS Work1;
				ZeroMemory(&Work1, sizeof(DBWORKS));
				//User[unum].SetNewUser(&ui, sid);
				
				// ### [�̺�Ʈ Ƽ��] ###
				User[unum].SetNewUser(&ui, &Work1.Item, sid, &Work1.EventPrize);		// [���� ������ �۾�]
				
				g_nAgentIndex = unum; // Ư���� �������̴�(Server Agent)!!!!

				CNM_AGENT_CONNECT nmsg(SndBuf);
				nmsg.Set(RealUser, g_JackPotMoney);
				pSocket->Send(nmsg.pData, nmsg.GetTotalSize());

				if( Cfg3.bExamServer )
					GetDlgItem(IDC_EDIT_AGENTCONNECT)->SetWindowText(_T("�������� �� Agent Connect!"));
				else
					GetDlgItem(IDC_EDIT_AGENTCONNECT)->SetWindowText(_T("  �� Agent Connect!"));

				/////////////////////////////////////////////////
				// [���� ������Ʈ �۾�] ������Ʈ�� �����
				CMainFrame *pMain = (CMainFrame*) GetParentFrame();
				g_pMainView->bConnectDeny = TRUE;
				pMain->Connectdeny();
				ShowState(_T("������� ������ �㰡�մϴ�." ));
				/////////////////////////////////////////////////

				// �ϴ� 400���� ����ڸ� ������
				const int nIncNum = 400;
				int totnum=0;
				AGENT_USER au[nIncNum];
				memset(au,0,sizeof(AGENT_USER)*nIncNum);

				for(i=0; i<nIncNum; i++){
					if(User[i].bValid) {
						au[totnum].nUNum  = User[i].UI.UNum;
						au[totnum].PMoney = User[i].OI.pBackup;
						strncpy(au[totnum].ID,User[i].UI.ID,15); // ���̵�
						totnum++;
					}
				}
				if(totnum>0){
					// ����� ����� ������
					CNM_AGENT_USERLIST_R1 r1msg(SndBuf);
					r1msg.Set(totnum, au, 0); // ***** ���� *****
					pSocket->Send(r1msg.pData, r1msg.GetTotalSize());

				}

				// �ο��� ���� ���Ѿ� ���� ������?????????
				RealUser++;
				//break;				//������Ʈ ���� - jeong
			//}

			// [SSO �۾�] - �� ��ü ����(GetTotNum�� �α��� �ź� �޽����� �ٲ����)
			// DBó���� ���ٷ� ���� �Ұ�
			if(DBMan.DBQue.GetTotNum() >= Cfg2.LoginDBQueNum) {
				CSV_REFUSE_LOGIN rlmsg(SndBuf); // �α��� �ź� �޼���
				rlmsg.Set(3, "���ӷ��� ���� ������ ������ �� �����ϴ�.\n����� ������ �Ͻðų� �ٸ� ä���� �̿��ϼ���.");	// [SSO �۾�]
				pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
				SockMan.ReservClose(sid); // ���� ���� ����
				break;
			}

			// ������ ����ũ�� �����ϱ����� Time Tick
			int StartTick = timeGetTime();
			
			// ������ ����� ������ Time Tick�� ���(��Ȯ�� �Ǻ� �� Ÿ�� üũ��)
			pUserVal->iUser3 = StartTick;
			
			// ����� �������κ��� ������� IP�� ��			[PC�� �۾�]
			SOCKADDR_IN peername = {0,};
			int namelen = sizeof(SOCKADDR_IN);
			::getpeername(pSocket->m_hSocket, (SOCKADDR*)&peername, &namelen);
			
			DBWORKS Work;
			ZeroMemory(&Work, sizeof(Work));

			Work.bValid = TRUE;									// ��ȿ �÷���
			Work.WorkKind = 1;									// ���� ����
			Work.SID = sid;										// ���� SID
			Work.StartTick = StartTick;							// DB ó�� ���� �ð�
			Work.UniqueTick = StartTick;						// ���ϰ� ������ Time Tick
			strcpy(Work.UserIP, inet_ntoa(peername.sin_addr));	// ������� IP
			//2004.05.07 �������Ѿ� ����
			Work.bForceLogin = MsgData.pAL->bForceLogin;// <-------------------�߰�
			
			
			if(strlen(MsgData.pAL->ID) > 0 && strlen(MsgData.pAL->Pass) > 0) {
				// ���̵�� �н����带 �޾Ƽ� ������� ������
				strncpy(Work.UI.ID, MsgData.pAL->ID, 15);		// ���� ID
				strncpy(Work.Passwd, MsgData.pAL->Pass, 15);	// �н�����
			}
			else {
				CSV_REFUSE_LOGIN rlmsg(SndBuf);
				rlmsg.Set(3, "���������� ������δ� ������ �� �����ϴ�(3)");
				pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
				SockMan.ReservClose(sid); // ���� ���� ����

				CLogFile LogFile;
				LogFile.Writef("���������� ������� ���� �õ�(3) - %s", MsgData.pAL->ID);
				break;
			}


			/*
			DBWORKS Work;
			ZeroMemory(&Work, sizeof(Work));
			Work.bValid = TRUE;							// ��ȿ �÷���
			Work.WorkKind = 1;							// ���� ����
			Work.SID = sid;								// ���� SID
			Work.StartTick = StartTick;					// DB ó�� ���� �ð�
			Work.UniqueTick = StartTick;				// ���ϰ� ������ Time Tick
			strncpy(Work.UI.ID, MsgData.ID, 15);		// ���� ID
			strncpy(Work.UI.Pass, MsgData.Pass, 10);	// �н�����
			Work.bForceLogin = *MsgData.Over;			// ���� �α��� �÷���
		*/

			// DB ������ ����
			BOOL rtn = DBMan.SetDBWork(&Work);
			if(rtn == FALSE) {
				CSV_REFUSE_LOGIN rlmsg(SndBuf);
				rlmsg.Set(3, "�ִ� �����ο��� �ʰ��Ͽ����ϴ�.\n����� �ٽ� �������ּ���");	// [SSO �۾�]
				pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
				SockMan.ReservClose(sid); // ���� ���� ����
				break;
			}

			// ���� ���� ������	[���� ������ �۾�]
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
				// ä�� �κ� �޼���
				strncpy(si.SrvInfoMsg, Cfg2.NoticeInfoMsg, 79);	//  �����޽���

				si.JackPot = g_JackPotMoney; // �α��ν� ������.

				CSV_SERVERINFO simsg(SndBuf);
				simsg.Set(&si);
				pSocket->Send(simsg.pData, simsg.GetTotalSize());
			}


		} break;

	case SV_ASK_LOGOUT: // �α׾ƿ� ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_LOGOUT MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = *MsgData.UNum;

			// �����˻�
			if(unum < 0 || unum >= MAX_USER) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;
			if(strlen(MsgData.ID) == 0 || strlen(MsgData.ID) >= 16) break;

			// ä�� ������ �ƴ� ������ �α׾ƿ� ��û�ϸ� �ǽɽ����� ����� �̹Ƿ� ���� ����
			if(User[unum].SUI.Position != WH_CHAN) {
				CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);
				if(pSocket==NULL || !pSocket->m_bCreated) break;
			//	pSocket->PostClose();
				pSocket->ReservClose();
				break;
			}

			if(!User[unum].bLogOutDBSaved) {
				// ���� ����ġ�� ���������� �ִ� ��츸 ����Ÿ���̽��� ����Ѵ�
				// �̺κп��� DB�� ������Ʈ
				DBWORKS dbwork;
				User[unum].MakeDBWork(4, &dbwork);
				BOOL rtn = DBMan.SetDBWork(&dbwork);
				
				// DB �۾� ť�� �� �� ��� �α� ���
				if(rtn == FALSE) {
					CLogFile LogFile;
					LogFile.Writef(_T("DB Work Que overflow - OnDestroy() : id=%s"), dbwork.UI.ID);
				}
				else User[unum].bLogOutDBSaved = TRUE;
			}

		
		} break;

	case SV_ASK_ALLCHANINFO: // ä�� ���� ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			// Ư���� �޼����� �����Ƿ� �޼��� �м��� �ʿ����
			
			CHANNELINFO ci[MAX_CHAN];
			int totnum=0;
			for(int i=0; i<MAX_CHAN; i++) {
				ci[i] = Chan[i].GetChannelInfo();
				totnum++;
			}

			// ��� ä�� ������ ������
			CSV_ALLCHANINFO acmsg(SndBuf);
			acmsg.Set(totnum, ci);
			SockMan.SendData(sid, acmsg.pData, acmsg.GetTotalSize());
		} break;

	case SV_ASK_ENTERCHAN: // ä�� ���� ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_ENTERCHAN MsgData;
			MsgData.Get(lpdata, totsize);

			// �����˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(User[unum].ChNum != -1) break;
			int cnum = *MsgData.ChNum;
			if(cnum < 0 || cnum >= MAX_CHAN) break;

			// ä�� ���� ó��
			int nUseItem = 0;
			int code = Chan[cnum].OnNewUser(unum, nUseItem); // [���� ������# �۾�] ä�ο�
			ENTERCHANINFO ec = {0,};
			/*
			// code :
			//         >= 0  ���� ä�� ����;
			//		     -3  ���� ��޻� ���� ������ ����(���̾�α� �ڽ�)	
			//           -4  �������� ���� �ذ�ʵ�(2�ܰ� �̻� ���̳�)
			//			 -6  ���� ������(ä�κ� �ѵ� �ݾ�)
			//			 -7  ���� ��޿� ���� �� �� ���µ� ������������ �ֳ�(����Ұ��ΰ� ����?)
			//         <  0  ( -1  ä���� ��á��, ...)
			//
			// &nUseItem :
			//		      0  �Ϲ��� ä�� ����
			//            1  �̹� �ѹ��̶� ����� ������������ ������ ����
			*/

			if(code >= 0) {
				// ä�� ������ ���	
				strncpy(ec.ChanName, Chan[cnum].strChanName, 19);
				ec.ChNum = cnum;
				ec.ChanStyle = Chan[cnum].ChannelStyle;
				ec.channel_usernum = Chan[cnum].m_channel_usernum; // 5, 7�ο� �۾�
			} 
			else if(  code == -4) //  �������� ���� �˷��ش� ,�������� ���� �ذ�ʵ�(2�ܰ� �̻� ���̳�)
			{
				// #########################################################33
				// �׽�Ʈ
				int level = User[unum].UI.nIcon;
				int style = Chan[cnum].ChannelStyle; // ä�� ���

						
				CSV_SERVERMSG smsg(SndBuf);
				CString str;

				char lv_buf0[50];
				ZeroMemory( lv_buf0, sizeof(char)*50 );
				char lv_buf1[50];
				ZeroMemory( lv_buf1, sizeof(char)*50 );
				char mylv[50];
				ZeroMemory( mylv, sizeof(char)*50 );

				char mygrade[50]; // ���
				ZeroMemory( mygrade, sizeof(char)*50 );

				
				// ��� ���� �߰�
				int login1 = 0;
				int login2 = 0;
				if(style == 0 ) {		 // ����
					login1 = 0;
					login2 = 14;
				} else if(style == 1 ) { // �ʺ�
					login1 = 0;
					login2 = 3;
				} else if(style == 2 ) { // �߼�
					login1 = 4;
					login2 = 7;
				} else if(style == 3 ) { // ���
					login1 = 8;
					login2 = 11;
				} else if(style == 4 ) { // ����	
					login1 = 12;
					login2 = 14;
				} 

				GetLevelName( login1, lv_buf1 ); // �����̸�
				GetLevelName( level, mylv ); // ���� ����

				GetGradeName(level, mygrade); // ��� ���ϱ�(�ʺ����,...)
				GetGradeName( login1, lv_buf0 ); // ����̸�

								
				str.Format( "[%s]ȸ���� [%s]������, [%s] �Դϴ�.\n�����Ͻ� ä���� [%s(%s)]�� ����ڰ� ������ �� �ֽ��ϴ�. \n[%s] ä���� �̿��� �ּ���.", 
					User[unum].UI.ID, mylv, mygrade, lv_buf0, lv_buf1, mygrade );

				smsg.Set(4  , (char*)str.operator LPCTSTR() );
				

				////////////////////////////////////////////////////////////
				// ��ư Ȱ��ȭ ���� (ä�� ����â)
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
				// ä�� ������ �ź�
				ZeroMemory(&ec, sizeof(ec));
				ec.ChNum = code;
			}

			CSV_ACCEPT_ENTERCHAN aemsg(SndBuf);
			aemsg.Set(&ec);
			SockMan.SendData(sid, aemsg.pData, aemsg.GetTotalSize());

			// ### [�̺�Ʈ Ƽ��] ###
			if(code >= 0) {

				SendChannelEventNotice(unum);
				//2004.05.07 �������Ѿ� ����
			//	SendPremLeadersMsg(unum, User[unum].UI.PremLeadersCode);
			
			}

		} break;

	case SV_ASK_OUTCHAN: // ä�� ���� ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			// Ư���� �޼����� �����Ƿ� �޼��� �м��� �ʿ����

			// �����˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			int cnum = User[unum].ChNum;
			if(cnum == -1) break;
			
			// ä�� ���� ó��
			Chan[cnum].OnOutUser(unum);

			// ä�� ������ ���
			CSV_ACCEPT_OUTCHAN aomsg(SndBuf);
			aomsg.Set();
			SockMan.SendData(sid, aomsg.pData, aomsg.GetTotalSize());
		} break;

	case SV_ASK_FINDID: // ���̵� ã�� ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_FINDID MsgData;
			MsgData.Get(lpdata, totsize);

			RESULTFINDID rf;
			ZeroMemory(&rf, sizeof(rf));
			FindID(&rf, MsgData.ID);

			// ���̵� ã�� ����� �˷���
			CSV_RESULT_FINDID rfmsg(SndBuf);
			rfmsg.Set(&rf);
			SockMan.SendData(sid, rfmsg.pData, rfmsg.GetTotalSize());
		} break;

	case SV_ASK_DETAILUSERINFO: // Ư�� ������� �� ���� ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_DETAILUSERINFO MsgData;
			MsgData.Get(lpdata, totsize);

			USERINFO *pUI = NULL;
			USERINFO TempUI;
			ZeroMemory(&TempUI, sizeof(USERINFO));

			int unum = *MsgData.UNum;

			// �߸��� ���̵��ϰ�� �� ����� ������ ������
			if(unum < 0 || unum>=MAX_USER) pUI = &TempUI;
			else if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) pUI = &TempUI;
			else pUI = &User[unum].UI;

			CSV_DETAILUSERINFO duimsg(SndBuf);
			duimsg.Set(pUI);
			SockMan.SendData(sid, duimsg.pData, duimsg.GetTotalSize());
		} break;

	case SV_ASK_USERINFO: // Ư�� ����� ���� ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_USERINFO MsgData;
			MsgData.Get(lpdata, totsize);

			USERINFO *pUI = NULL;
			USERINFO TempUI;
			ZeroMemory(&TempUI, sizeof(USERINFO));

			int unum = *MsgData.UNum;

			// �߸��� ���̵��ϰ�� �� ����� ������ ������
			if(unum<1 || unum>=MAX_USER) pUI = &TempUI;
			else if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) pUI = &TempUI;
			else pUI = &User[unum].UI;

			CSV_USERINFO uimsg(SndBuf);
			uimsg.Set(pUI);
			SockMan.SendData(sid, uimsg.pData, uimsg.GetTotalSize());
		} break;

	case SV_ASK_MONEYINFO: // �� ���� ������Ʈ ��û - jeong
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

	case SV_ASK_BANKINFO: // ���� ���� ������Ʈ ��û - jeong
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

	case SV_ASK_CHANGECHAR: // ĳ���� �ٲٱ⸦ ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
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

	case SV_NOWHISPER: // �Ӹ� ��� ���� ����
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CSV_NOWHISPER MsgData;
			MsgData.Get(lpdata, totsize);

			// �����˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;

			User[unum].bNoWhisper = *MsgData.bNoWhisper;
		} break;

	// [������Ŀ] �߰���ƾ
	case SV_CHANGECARD: // ī�� �ٲٱ⸦ ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CSV_CHANGECARD MsgData;
			MsgData.Get(lpdata, totsize);
		
			// �����˻�
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

	case SV_BUYCARD: // ī�� ��⸦ ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CSV_BUYCARD MsgData;
			MsgData.Get(lpdata, totsize);

			// �����˻�
			int unum     = pUserVal->iUser2;
			int nCardNum = MsgData.pBC->nCard;
			if(unum < 1 || unum >= MAX_USER) break;
			if(unum!=MsgData.pBC->nUNum) break;
			if(User[unum].bValid == FALSE) break;
			if(nCardNum < 0 || nCardNum >= 12) break;

			INT64 nPrice = 0;
			switch(nCardNum){
				case 0: nPrice = M_500;  break; // 5�鸸��
				case 1: nPrice = M_500;  break; // 5�鸸��
				case 2: nPrice = M_3000; break; // 3õ����
				case 3: nPrice = M_3000; break; // 3õ����

				case 4: nPrice = M_5000; break; // 5õ����
				case 5: nPrice = M_5000; break; // 5õ����
				case 6: nPrice = M_1Y;   break; // 1���
				case 7: nPrice = M_1Y;   break; // 1���

				case 8:   nPrice = M_5Y;  break; // 5���
				case 9:   nPrice = M_5Y;  break; // 5���
				case 10:  nPrice = M_10Y; break; // 10���
				case 11:  nPrice = M_10Y; break; // 10���
				default:  nPrice = M_10Y; break; // 10���
			}

			if(User[unum].UI.PMoney<nPrice) break;
			if(User[unum].UI.nIcon<4 && nCardNum>3) break; // �ٷ� �� ������ ��� ����
			if(User[unum].UI.nIcon<8 && nCardNum>7) break;

			User[unum].UI.PMoney-=nPrice;
			User[unum].UI.PI.Card[nCardNum] = (char)1;


		} break;

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	// ���� ���� ���
	case NM_AGENT_AVERAGE:
		{
			CNM_AGENT_AVERAGE MsgData;
			MsgData.Get(lpdata, totsize);

			if( *MsgData.JackPotMoney > 0)
			{
				g_JackPotMoney = *MsgData.JackPotMoney;
			}

		} break;

	// ������ ���� 
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
			str.Format("Agent�� ���� ������ ���̵� %d�� ���۹���", total);
			ShowState(str);
		
		} break;
    
	case NM_AGENT_USERLIST: // [���� ������Ʈ �۾�] : ����� ����Ʈ�� ��û�Ѵ�
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CNM_AGENT_USERLIST MsgData;
			MsgData.Get(lpdata, totsize);
			
			int nUserVal = *MsgData.Reserve;
			
			if(g_nAgentIndex < 1 || g_nAgentIndex >= MAX_USER) break;
			if(User[g_nAgentIndex].Sid!=sid) break;

			const int nIncNum = 400; // 400���� ���� ������!!

			int nStart = nIncNum*nUserVal; // 400 ~ (1 ����)
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
					strncpy(au[totnum].ID,User[i].UI.ID,15); // ���̵�
					totnum++;
				}
			}

			if(totnum > 0) {

				CNM_AGENT_USERLIST_R1 r1msg(SndBuf);
				r1msg.Set(totnum, au, nUserVal); // ����
				pSocket->Send(r1msg.pData, r1msg.GetTotalSize());
			}

		} break;

	// �ߺ� ������ �̴�
	case NM_AGENT_NEWUSERERROR:
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CNM_AGENT_NEWUSERERROR MsgData;
			MsgData.Get(lpdata, totsize);

			AGENT_USER au; ZeroMemory(&au, sizeof(AGENT_USER));
			memcpy(&au, MsgData.ArraySUI, sizeof(AGENT_USER));

			int totNum = *MsgData.TotNum;

			// �����˻�
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
			{// �����̶�� ���� ����(�̹� ������ �ִ� ���̵� �Դϴ�)
				if(User[unum].SUI.Position == WH_CHAN || User[unum].SUI.Position == WH_LOBY || User[unum].SUI.Position == WH_GAME)
				{
					CSV_SERVERMSG smsg(SndBuf);
					CString strMess = "";
					strMess.Format("[%s] [%s]", Cfg.ServerName, User[unum].UI.ID);
					smsg.Set(0, (char*)strMess.operator LPCTSTR()); 
					SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
					
					smsg.Set(1, "���� �ٸ� ���Ӽ������� ���ӽõ����Դϴ�........");
					pSocket->Send(smsg.pData, smsg.GetTotalSize());
					SockMan.ReservClose(usersid); // ���� ���� ����
				}
				else
				{
					CSV_SERVERMSG smsg(SndBuf);
					CString strMess = "";
					strMess.Format("[%s] [%s]", Cfg.ServerName, User[unum].UI.ID);
					smsg.Set(1, (char*)strMess.operator LPCTSTR()); 
					SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());

					strMess.Format("[%s]���� ���� �ٸ� ���Ӽ������� ������ �õ����Դϴ�...", User[unum].UI.ID);
					smsg.Set(0, (char*)strMess.operator LPCTSTR()); 
					pSocket->Send(smsg.pData, smsg.GetTotalSize());
					// ������ ������ �ʴ´�
					SockMan.ReservClose(usersid); // ���� ���� ����
				}
				break;
			}
			else if(totNum == 6)
			{					
				CSV_REFUSE_LOGIN rlmsg(SndBuf); // �α��� �ź� �޼���
				rlmsg.Set(1, "���� ���� ���� ó�����Դϴ�. ����� �ٽ� ������ �ֽʽÿ�.");
				pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
				// ������ ������ �ʴ´�
				SockMan.ReservClose(usersid); // ���� ���� ����
				break;
			}
			//////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////

			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, "���ӵǾ��ִ� ���̵� �����մϴ�.Ȯ���� �ٽ� ������ �ֽʽÿ�.");
			pSocket->Send(smsg.pData, smsg.GetTotalSize());
			SockMan.ReservClose(usersid); // ���� ���� ����

		} break;

	/////////////////////////////////// [���� ������ �۾�] ////////////////////////////////////////

	case SV_ASK_GAMEITEMLIST: // ���� ������ ����� ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CSV_ASK_GAMEITEMLIST MsgData;
			MsgData.Get(lpdata, totsize);

			// �ƹ��� �޽����� ����

			// �����˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;

			DBWORKS Work;
			User[unum].MakeDBWork(9, &Work);
			//Work.UniqueTick = StartTick;				// ���ϰ� ������ Time Tick

			// DB ������ ����
			BOOL rtn = DBMan.SetDBWork(&Work);

			if(rtn == FALSE) {
				// ������ ������ �����ϸ� ��� �����ڵ�� �Բ� ������ ����Ʈ�� ����
				
				// ���� ������ ����Ʈ ����
				GAMEITEM_INFO in = {0,};
				in.Kind = 100;		// ������ ����Ʈ ������
				in.ErrorCode = 1;	// ������ ���� ���� �����ڵ�
				in.TotNum = 0;
				in.StrMsgLen1 = 0;
				in.StrMsgLen2 = 0;
				CTime time = CTime::GetCurrentTime();
				time.GetAsSystemTime(in.ServerTime);

				// ������ ����Ʈ�� ������
				CSV_GAMEITEMLIST glmsg;
				glmsg.Set(&in, User[unum].GameItem.Item, NULL, NULL);
				User[unum].SendMsg(&glmsg);

				break;
			}

		} break;

	case SV_USEGAMEITEM: // ���� ������ ��� ��û
		{
			// ��ȿ�� ������ �ƴϸ� ó������ ����
			if(pUserVal->iUser1 == FALSE) break;

			CSV_USEGAMEITEM MsgData;
			MsgData.Get(lpdata, totsize);

			// �����˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;

			int index = MsgData.pUG->Item.Index;
			
			if(index < 0 || index >= User[unum].GameItem.nNum) {
				//
				// �ε��� �ʰ��� �߸��� ��ȣ��
				//

				// ���� ������ ��� ���
				USEGAMEITEMINFO ug = {0,};
				strncpy(ug.ID, User[unum].UI.ID, 15);
				ug.bUseError = TRUE;
				ug.Item = User[unum].GameItem.Item[index];

				// ���� ������ ��� ����� ����
				CSV_USEGAMEITEM ugmsg;
				ugmsg.Set(&ug);
				User[unum].SendMsg(&ugmsg);

				break;
			}

			// �������� ���¸� ����
			ItemState state = GameItem_GetItemState(User[unum].GameItem.Item[index]);
			if(state.iState != 1) {
				//
				// ��� ������ �������� �ƴϸ� ��� �Ұ�
				//

				// ���� ������ ��� ���
				USEGAMEITEMINFO ug = {0,};
				strncpy(ug.ID, User[unum].UI.ID, 15);
				ug.bUseError = TRUE;
				ug.Item = User[unum].GameItem.Item[index];
				ug.bMessage = TRUE;
				strncpy(ug.StrMsg, "����� �� ���� �������Դϴ�.", sizeof(ug.StrMsg)-1);

				// ���� ������ ��� ����� ����
				CSV_USEGAMEITEM ugmsg;
				ugmsg.Set(&ug);
				User[unum].SendMsg(&ugmsg);

				break;
			}

			BOOL bExist = FALSE;
			// ### [ ���αغ� ������ ] ###
			// ���۹�, �����ѹ� ������ �� �߿� �ϳ��� ��밡��(�ߺ� �Ұ�)
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

			// [��ȣõ��] 2004.07.08
			if(curCode >= ITEM_SCODE_SAFEGUARDANGEL && curCode <= ITEM_ECODE_SAFEGUARDANGEL) 
			{
				// �����������϶��� �Ұ�
				if(User[unum].SUI.Position == WH_GAME && User[unum].PNum != -1) {
					
					int cnum = User[unum].ChNum;
					int rnum = User[unum].SUI.RoomNum;
					int pnum = User[unum].PNum;
					if(cnum >= 0 && cnum < MAX_CHAN && rnum>0 && rnum <MAX_ROOM && pnum >= 0 && pnum < Chan[cnum].Room[rnum].get_maxplayer()) {
						if( Chan[cnum].Room[rnum].bValid ) {
							if(faststrcmp(User[unum].UI.ID, Chan[cnum].Room[rnum].Ui[pnum].ID)==0) {
								if(Chan[cnum].Room[rnum].Ps[pnum].PlayState == 1) {
									CSV_SERVERMSG msg;
									msg.Set( 0, "�������� �߿��� ��ȣõ�� �������� ����� �� �����ϴ�." );
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
					msg.Set( 0, "������ ������ �ݾ��� ���ų�, �̹� �����Ǿ ������ �ݾ��� �����ϴ�." );
					User[unum].SendMsg(&msg);
					break;
				}
			}
			else if(curCode >= ITEM_SCODE_SUPERMASTER && curCode <= ITEM_ECODE_SUPERMASTER)
			{// ���� �ѹ� �˻�
				if(GameItem_IsUsingItem(User[unum].GameItem, curCode, &bExist)) bUseing = TRUE;
				if(GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_ALLINSUPERMASTER, &bExist)) bUseing = TRUE;
			}
			else if(curCode >= ITEM_SCODE_ALLINSUPERMASTER && 
				curCode <= ITEM_ECODE_ALLINSUPERMASTER)
			{// ���۹� �˻�
				if(GameItem_IsUsingItem(User[unum].GameItem, curCode, &bExist)) bUseing = TRUE;
				if(GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_SUPERMASTER, &bExist)) bUseing = TRUE;

			} 
			else if(GameItem_IsUsingItem(User[unum].GameItem, curCode, &bExist)) {
				bUseing = TRUE;
			}

			if(bUseing )
			{
				//
				// �̹� ������� ���� ������ �������� �ִٸ� ��� �Ұ�
				//

				// ���� ������ ��� ���
				USEGAMEITEMINFO ug = {0,};
				strncpy(ug.ID, User[unum].UI.ID, 15);
				ug.bUseError = TRUE;
				ug.Item = User[unum].GameItem.Item[index];
				ug.bMessage = TRUE;

				if( bInVincible )
					strncpy(ug.StrMsg, "õ�Ϲ��� �������� �̹� ������Դϴ�.  \r���۹�, ����(����/����2),�������(������Ŀ/������Ŀ2), \r1��5ȸ 2�� ���������� �������α���� ���� ������ �Դϴ�.", sizeof(ug.StrMsg)-1);
				else
					strncpy(ug.StrMsg, "�̹� ������� �������� �����մϴ�.", sizeof(ug.StrMsg)-1);

				// ���� ������ ��� ����� ����
				CSV_USEGAMEITEM ugmsg;
				ugmsg.Set(&ug);
				User[unum].SendMsg(&ugmsg);

				break;
			}

			DBWORKS dbwork;
			User[unum].MakeDBWork(10, &dbwork);	
			// ����� ������ ����
			dbwork.UseItem = User[unum].GameItem.Item[index];

			// DB �۾� ť�� �� �� ��� �α� ���
			if(!DBMan.SetDBWork(&dbwork)) {
				CLogFile LogFile;
				LogFile.Writef("DB�۾� ť �����÷ο� - CServerView::ProccessPacket() SV_USEGAMEITEM : id=%s", dbwork.UI.ID);
			}

		} break;

	////////////////////////////////////////////////////////////////////////////////////////

	default :
		{
			// �� ä�� �޽����� ä�� Ŭ�������� ó���Ѵ�
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

	// lParam���� ���� �����ʹ� new�� �Ҵ��� �������̹Ƿ� �ݵ�� delete�ؾ��Ѵ�
	//DBWORKS *pwork = (DBWORKS*)lParam;
	//memcpy(&Work, pwork, sizeof(DBWORKS));
	//delete pwork;

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB ó���� ���������� ������ ������ ������� �� ����
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	if(sid != (int)pSocket->m_dwUserVal) {
		//CLogFile LogFile;
		//LogFile.Write("sid ���� Ʋ��- C62CutPokerServerView::OnDbworkLogin()");
		return 1;
	}

	// ������ ���� ID�� �ƴϸ� ����(�̹� DBó���� ���������� ������ ������ ������� ���ɼ��� ����)
	if(Work.UniqueTick != (UINT)pUserVal->iUser3) return 1;



	CSV_REFUSE_LOGIN rlmsg(SndBuf); // �α��� �ź� �޼���

	// ������� ���̵����� �˻�
	BOOL bConnected=FALSE;
	for(int i=0; i<MAX_USER; i++) {
		if(User[i].bValid) {
			if(faststrcmp(User[i].UI.ID, Work.UI.ID) == 0) { 
				bConnected = TRUE; 
				CSV_HEARTBEAT hbmsg(SndBuf); // ��Ʈ��Ʈ �޼���
				hbmsg.Set();
				User[i].SendMsg(&hbmsg);
				break;
			}
		}
	}

	// �̹� ������� ���̵�
	if(bConnected) {
		rlmsg.Set(2, "���� ������ �ùٸ��� ������� �ʾҰų� �̹� ������� ���̵��Դϴ�.\n�ٽ� ������ �õ��غ��ð� �����ڿ��� �����ϼ���");	// [SSO �۾�]
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}

	// ������ ���� �ο� �ʰ�
	if(RealUser >= Cfg2.MaxUser) {
		rlmsg.Set(3, "�ִ� �����ο��� �ʰ��Ͽ����ϴ�.\n����� �ٽ� �������ּ���");	// [SSO �۾�]
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}

	// ����ִ� ����� ��ȣ �˻�
	int unum = -1;
	for(i=1; i<MAX_USER; i++) if(!User[i].bValid) { unum=i; break;}

	// ����� �����ο� �ʰ�
	if(unum == -1) {
		rlmsg.Set(3, "�ִ� �����ο��� �ʰ��Ͽ����ϴ�.\n����� �ٽ� �������ּ���");	// [SSO �۾�]
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}


	// DBó�� ����� ���� ����� ���� ����ü
	USERINFO ui;
	memcpy(&ui, &Work.UI, sizeof(USERINFO));


	// DB�� ������� �÷��̾��� ���� ������ 0 - jeong
	ui.PMoney = 0;
	//User[0].UI.PMoney = 1000;
/*
	// [���� ������ �۾�]
	if(Work.IDState != 0) {
		if(Work.IDState == 1) {
			// ��� �������� ���̵���
			rlmsg.Set(6);
			pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
			SockMan.ReservClose(sid);
			return 1;
		}
		else if(Work.IDState == 2) {
			// ������ ���̵���
			rlmsg.Set(7);
			pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
			SockMan.ReservClose(sid);
			return 1;
		}
		else {
			// ������ ��� �ڵ�� ��� �������� ���̵� ��Ÿ��
			rlmsg.Set(6);
			pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
			SockMan.ReservClose(sid);
			return 1;
		}
	}
*/
	//--------------------- [SSO �۾�]

// ��� ȸ���� ���ڸ��� 0�� ��츸 ���ӿ� �������� ������ �����ϴ�.
	// ��, ��ȸ���� ���ڸ��� 0�̾ ���� �Ұ�!
	//
	// 0 ~  : �ӽ�ȸ�� (0=����, 1=�������, 2=��������, 7=(������ȸ��)����, 8=(������ȸ��)�������, 9=(������ȸ��)��������) 
	//    --> 14�� �̸� �Ƶ��� ó�� ���Խ� �ӽ�ȸ���� �Ǹ� �θ��� ���Ǹ� ������ ��ȸ���� �ȴ�(�ӽ�ȸ���� ���� ���� ����)
	// 10 ~ : ��ȸ�� (10=����, 11=�������, 12=��������) 
	//    --> ��ȸ���� �����̶�� �ϴ��� ����� ������ �����ϱ� ��(��ȸ���� �Ǳ� ��)������ ������ ���� ���� ����� �ȴ�
	// 20 ~ : ��ȸ�� (20=����, 21=�������, 22=��������)
	//    --> 14�� �̻��̰ų� �θ� ���Ǹ� �޾Ƽ� ��ȸ������ �°��� ȸ��
	//
	
	// idstate�� 0,7,20 �� ��츸 ���� ������ ������
	int idstate = Work.IDState;
	if(idstate != 0 && idstate != 7 && idstate != 20) {
		// ��� �������� ���̵����� �˷��ش�
		rlmsg.Set(6, "�� ���̵�� ���� ��� ���� �����̹Ƿ�\n������ ������ �� �����ϴ�.\n�����ڿ��� �����ϼ���");
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}

	// ����ũ �ѹ��� Ʋ�ȴٸ�
	if(Work.bIsWrongUniqNo == TRUE) {
		rlmsg.Set(1, "�ùٸ��� ���� ���������� ������ �õ��ϼ̽��ϴ�\n�����ڿ��� �����ϼ���");
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}

	/*
	// ��й�ȣ�� Ʋ�ȴٸ�
	if(Work.bIsWrongPwd == TRUE) {
		rlmsg.Set(1, "��й�ȣ�� ��ġ���� �ʽ��ϴ�.\n�����ڿ��� �����ϼ���");	// [SSO �۾�]
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}
	*/

	if(Work.bIsNotUserID == TRUE) {
		//���̵� �������� �ʴ´�
		rlmsg.Set(0, "�������� �ʰų� �߸��� ����� ID�Դϴ�.\n�����ڿ��� �����ϼ���");	// [SSO �۾�]
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}

	// [��������]
	if( !Work.bCertificate )
	{
		rlmsg.Set(1, "���� �������θ� Ȯ���� �ֽʽÿ�.");
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		SockMan.ReservClose(sid);
		return 1;
	}


	// �ٸ� ���ӿ� ���� ���̰� �������� �÷��װ� �������¶��
	if(Work.bAnotherConnect==TRUE && Work.bForceLogin==FALSE) {
		// �ٸ����ӿ� �������̹Ƿ� �α����� �ź���(������ ������ �ʴ´�)
		rlmsg.Set(5, "�ٸ����ӿ� ���� ���Դϴ�.");
		pSocket->Send(rlmsg.pData, rlmsg.GetTotalSize());
		return 1;
	}



	/////////////////////////////////////////////////////////////////////////////////	
	//2004.05.07 �������Ѿ� ����
	// ó���α��̱� �Ӵ����ѿ� �ɷȴٸ� => �ٽ� �α��û
	if( CheckTotalmoneylimit(&ui, NULL, 0) && Work.bForceLogin==FALSE && StatisCfg.bTotalMoneyLimit == TRUE ) { 
		
		LEADERS_JOIN join = {0,};
		strncpy(join.ID, ui.ID, 15);
		join.PremLType		  = ui.PremLeadersCode;
		join.CurMoney		  = ui.PMoney;
		join.NorMoney         = StatisCfg.NormalMoney;
		join.LeadersMoney	  = StatisCfg.LeadersMoney;
		join.LeadersPlusMoney = StatisCfg.LeadersPlusMoney;

		strncpy(join.UniqNo,Work.UniqNo,15);

		strncpy(join.LeadersJoinURL, 	Cfg2.LeadersJoinURL, 63); // ������ ����Ȩ������

		CSV_LEADERSJOIN ljmsg(SndBuf);
		ljmsg.Set(&join);
		pSocket->Send(ljmsg.pData, ljmsg.GetTotalSize());
		return 1;
	}

	/////////////////////////////////////////////////////////////////////////////////

	//######## [���� ����] #########
	if(g_pMainView->Cfg3.bExamServer)
	{
		switch(g_ServerGrade)
		{
		case 0:
		case 1: //�ʺ�
			ui.PMoney =50000000000000; // 100��
			break;
		case 2: //�߼�
			ui.PMoney =1000000000000; // 10��
			break;
		case 3: //���
			ui.PMoney =1000000000000; // 1000��
			break;
		case 4: //����
			ui.PMoney =1000000000000000; // 1000��
			break;
		}

		ui.nIcon = GetPokerLevel(ui.PMoney);
		Work.UI.PMoney = ui.PMoney;
		Work.UI.nIcon = ui.nIcon;
	}


	// [���� ������ �۾�]
	BOOL bAskUseJumpItem = FALSE;
	BOOL bAskBuyJumpItem = FALSE;

	//### [������ ��� �۾�] ###
	BOOL bAdminUser = FALSE;
	if(IsAdminUser(ui.ID, Work.UserIP)) bAdminUser = TRUE;
	if(!bAdminUser && Cfg3.EnterLimitMode == 1 && Cfg3.bLoginLimit) {// ���� �α��� ���� ���� ����̸�
		int level = Work.UI.nIcon;
	
		if(!CheckLevel(level, Work.UI.PMoney)) { // ����Ұ�
	
			int grade =  GetMyLeveltoGrade(level); // ���� -> ���
			BOOL bAccessDeny = FALSE;
			
			// ���������� ��������� �Ǻ�
			BOOL bJumpUsing = FALSE;
			BOOL bJumpExist = FALSE;
			bJumpUsing = GameItem_IsUsingItem(Work.Item, ITEM_SCODE_JUMP, &bJumpExist);
					
			// ### [ ä�� ���� ���� ] ###
			if(grade+1 >= g_ServerGrade &&  grade < g_ServerGrade) // �Ѵܰ� ���� ����
			{
				// ���� ���� �������� �ڽ��� ������ ���� ���� ���� ������� ���� �ǻ糪 ��� ���θ� �����
				if( !bJumpUsing && 
					CheckPremiumState(&ui, NULL)==0 &&	// [�����̾� �۾�]
					!ui.bPremIP)						// [PC�� �۾�]
				{	
					// ������������ ����ϰ� ���� �ʰ� �����̾� ȸ���� �ƴ϶��

					if(bJumpExist) {
						// ���� �������� �����Ѵٸ� ��� ���θ� ����� �Ѵ�(�ϴ� �α��� ó���� ��� ����)
						bAskUseJumpItem = TRUE;
					}
					else {
						// ���� �������� ����ϸ� ������ �� �ִ� ����� ���� ���� �ǻ縦 ���(�ϴ� �α��� ó���� ��� ����)
						bAskBuyJumpItem = TRUE;
					}
				}
				else {
						// ���� �������� ������̰ų� �����̾� ȸ���̶�� ���� �㰡
					bAccessDeny = FALSE;
				}

			}
			else
			{  // �Ұ� 
				bAccessDeny = TRUE;

				// �Ұ� ������ �˷��ش�....
				// ���� ���� ���� ����
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


				// ��� ���� �߰�
				int login1 = 0;
				int login2 = 0;
				if(g_ServerGrade == 0 ) {		 // ����
					login1 = 0;
					login2 = 14;
				} else if(g_ServerGrade == 1 ) { // �ʺ�
					login1 = 0;
					login2 = 3;
				} else if(g_ServerGrade == 2 ) { // �߼�
					login1 = 4;
					login2 = 7;
				} else if(g_ServerGrade == 3 ) { // ���
					login1 = 8;
					login2 = 11;
				} else if(g_ServerGrade == 4 ) { // ����	
					login1 = 12;
					login2 = 14;
				} 

				GetLevelName( login1, lv_buf0 ); // �����̸�
				GetLevelName( login2, lv_buf1 ); // �����̸�
				GetLevelName( level, mylv ); // ���� ����
			//	GetGradeName(level, ) // ��� ���ϱ�

				GetSuitableServerName( level, server_lv ); // ��� ���� ��밡��
		
				{// 2�ܰ� �̻� ���� 
					str.Format( "[%s]�� [%s] ~ [%s]������ ���尡���մϴ�.\n[%s]ȸ���� ������ [%s]�̹Ƿ� \"%s\"�� ���尡���մϴ�.", 
					Cfg.ServerName, lv_buf0, lv_buf1, Work.UI.ID, mylv, server_lv );

					smsg.Set(1, (char*)str.operator LPCTSTR() );
					pSocket->Send(smsg.pData, smsg.GetTotalSize());
					// ���� ���� ����
					SockMan.ReservClose(sid);
					
				}
			
				return 1;
			}

			if(bAccessDeny) {
				// ���� �����۵� ���� ����� ���� �ʴ� ���� ��� �޽��� �� ���� ����
				CSV_SERVERMSG smsg(SndBuf);
				smsg.Set(1, "���� ������ ����� �ƴմϴ�. �ٸ� ������ �̿����ּ���.");
				pSocket->Send(smsg.pData, smsg.GetTotalSize());
				// ���� ���� ����
				SockMan.ReservClose(sid);
				return 1;
			}
		}
	}

	pUserVal->iUser1 = TRUE; // �ùٸ� ������ ������ �������� ��Ÿ����.
	pUserVal->iUser2 = unum; // ����� ���� ��ȣ(���� Ŭ������ ����)
			
	ui.UNum = unum;										// ���� ��ȣ

	// ��й�ȣ �ʵ� �ʱ�ȭ(���ݺ��ʹ� ��й�ȣ ������ �ʿ����. ��ŷ�� ������ �����Ƿ� �ʱ�ȭ)
//	memset(ui.Pass, 0, sizeof(ui.Pass));

	//### [������ ��� �۾�] ###
	if(bAdminUser) {
		ui.cAdminUser = 1;
	}

	// ���ο� ������ �޼��� ���� �غ�
	// ### [�̺�Ʈ Ƽ��] ###
	User[unum].SetNewUser(&ui, &Work.Item, sid, &Work.EventPrize);		// [���� ������ �۾�]

	////////////////////////////////////////////////
	//2004.05.07 �������Ѿ� ����
	if(StatisCfg.bTotalMoneyLimit == TRUE)
	CheckTotalmoneylimit(&User[unum].UI, Work.UniqNo, 2); // �Ӵ� �谨
	////////////////////////////////////////////////

	strncpy(User[unum].RegID, Work.RegID, 15);			// [���� ������ �۾�]
	strncpy(User[unum].UniqNo, Work.UniqNo, 15);        // ����ũ �ѹ�
	// ### [�α� ��Ͽ�] ###
	strcpy(User[unum].UserIP, Work.UserIP); // ������� ���� IP����		[��������]

	// [��ȣõ��] 2004.07.09
	User[unum].Set_init_lost_money( Work.LostMoney );


	// ### [����� ����] ###
	// ### [ �ߺ��� IP�� ] ###
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
	





			

	// [���� ������ �۾�]
	// ������ �������� �����Ѵٸ� ������ ����Ʈ�� ����
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

	// [���� ������ �۾�]
	if(bAskUseJumpItem) {
		// ���� �������� �����Ѵٸ� ��� ���θ� ���
		// (���������� �α����� �� �����̳� Ŭ���̾�Ʈ���� 
		// �α��� �㰡 �޽����� ������ ���� ������)

		// ������ ��� ����
		char* strmsg1 = "������ ���� ������ �����Ͻ÷��� [���� ����� ������]�� �ʿ��մϴ�.\n���� �����Ͻ� [���� ����� ������]�� ����Ͻðڽ��ϱ�?";
		char* strmsg2 = "[���� ����� ������]�� ����Ͻø� ������ ����� ���Ƶ� �����Ͻ� �� �ֽ��ϴ�. �����Ͻ� �������� �Ʒ��� ������, �������� ����Ͻ÷��� ��Ͽ��� �������� ������ ����Ŭ���ϰų� [����ϱ�] ��ư�� Ŭ���Ͻø� �˴ϴ�.";

		GAMEITEM_INFO in = {0,};
		in.Kind = 1;
		in.ErrorCode = 0;
		in.StrMsgLen1 = strlen(strmsg1);
		in.StrMsgLen2 = strlen(strmsg2);
		CTime time = CTime::GetCurrentTime();
		time.GetAsSystemTime(in.ServerTime);

		/*
		// ��� ������ ���� �����۸� �߷���
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

		// ��� �������� �߷���
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
		// ���� �������� ����ϸ� ������ �� �ִ� ����� ���� ���� �ǻ縦 ���
		CString strmsg;
		strmsg.Format("������ ���� ������ �����Ͻ÷��� [���� ����� ������]�� �ʿ��մϴ�.\n[���� ����� ������]�� �����Ͻðڽ��ϱ�?");

		BUYGAMEITEMINFO bi = {0,};
		bi.Kind = 1;	// �α��ν� ���� ����� �������� ���� �ǻ縦 ���
		strcpy(bi.ItemName, "���� ����� ������");
		bi.ItemCode = ITEM_SCODE_JUMP;
		bi.StrMsgLen = strmsg.GetLength();

		CSV_BUYGAMEITEM bgmsg(SndBuf);
		bgmsg.Set(&bi, strmsg);
		pSocket->Send(bgmsg.pData, bgmsg.GetTotalSize());
	}
	else {
		// �α��� �㰡 �޼��� ����
		User[unum].SendAcceptLoginMsg();
	}

	// [���� ������Ʈ �۾�] : ���ο� �������� �����̴�!!
	if(g_nAgentIndex>0 && g_nAgentIndex<MAX_USER){ 
		int nAgentSID = User[g_nAgentIndex].Sid; // ���� ������Ʈ �� ���� FD
		CMySocket *pASocket = SockMan.GetDataSocketPtr(nAgentSID);
		if(pASocket && pASocket->m_bCreated){
			USERVAL *pAUserVal = (USERVAL*)(pASocket->m_pUserPtr);
			if(pAUserVal && pAUserVal->iUser1 && nAgentSID == (int)pASocket->m_dwUserVal){
				AGENT_USER au; memset(&au,0,sizeof(AGENT_USER));
				au.nUNum  = unum;
				au.PMoney = ui.PMoney;
				strncpy(au.ID,ui.ID,15); // ���̵�

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

	// �ð� ���� �׽�Ʈ
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

	// DB ó���� ���������� ������ ������ ������� �� ����
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	int unum = pUserVal->iUser2;
	if(unum<1 || unum>=MAX_USER) return 1;

	// DB ó���� ���������� ������ ������ ���� �ٸ� ����ڰ� ������ ����� �� ����
	if(sid != (int)pSocket->m_dwUserVal) return 1;

	// DBó������� ���� ���̵� Ʋ��(�̹� ������ ��������)
	if(Work.UI.UNum != unum) return 1;

	// DBó������� ���� ���̵� Ʋ��(�̹� ������ ��������)
	if(faststrcmp(Work.UI.ID, User[unum].UI.ID) != 0) return 1;

	// �α׾ƿ� DBó�� �÷��װ� ���õ��� �ʾ����� ���
	if(!User[unum].bLogOutDBSaved) return 1;

	// �α׾ƿ� ���
	CSV_ACCEPT_LOGOUT alomsg(SndBuf);
	alomsg.Set();
	SockMan.SendData(sid, alomsg.pData, alomsg.GetTotalSize());

	// ������ ������ �������(������ ������ ü�� ������ �����ϴ� ��ŷ�� ����ϱ� ����)
	//pSocket->PostClose();
	pSocket->ReservClose();

	return 1;
}



// [���� ������ �۾�]
LONG C62CutPokerServerView::OnDbworkGameitemlist(UINT wParam, LONG lParam)
{
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB ó���� ���������� ������ ������ ������� �� ����
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	int unum = pUserVal->iUser2;
	if(unum<1 || unum>=MAX_USER) return 1;
	if(!User[unum].bValid) return 1;

	// DB ó���� ���������� ������ ������ ���� �ٸ� ����ڰ� ������ ����� �� ����
	if(sid != (int)pSocket->m_dwUserVal) return 1;
	// DBó������� ���� ���̵� Ʋ��(�̹� ������ ��������)
	if(Work.UI.UNum != unum) return 1;
	// DBó������� ���� ���̵� Ʋ��(�̹� ������ ��������)
	if(faststrcmp(Work.UI.ID, User[unum].UI.ID) != 0) return 1;

	// ���� ������ ����Ʈ ����
	GAMEITEM_INFO in = {0,};
	in.Kind = 100;	// ������ ����Ʈ ������
	in.StrMsgLen1 = 0;
	in.StrMsgLen2 = 0;
	CTime time = CTime::GetCurrentTime();
	time.GetAsSystemTime(in.ServerTime);

	if(!Work.bErrorItemWork) {
		// ������ ������ �����ߴٸ� ������� ������ ����Ʈ�� ����
		User[unum].GameItem = Work.Item;
		
		in.ErrorCode = 0;
		in.TotNum = User[unum].GameItem.nNum;	
	}
	else {
		// ������ ���� ����
		in.ErrorCode = 1;
		in.TotNum = 0;
	}

	// ������ ����Ʈ�� ������
	CSV_GAMEITEMLIST glmsg;
	glmsg.Set(&in, User[unum].GameItem.Item, NULL, NULL);
	User[unum].SendMsg(&glmsg);

	return 1;
}

// [���� ������ �۾�]
LONG C62CutPokerServerView::OnDbworkUseitemresult(UINT wParam, LONG lParam)
{
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB ó���� ���������� ������ ������ ������� �� ����
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	int unum = pUserVal->iUser2;
	if(unum<1 || unum>=MAX_USER) return 1;
	if(!User[unum].bValid) return 1;

	// DB ó���� ���������� ������ ������ ���� �ٸ� ����ڰ� ������ ����� �� ����
	if(sid != (int)pSocket->m_dwUserVal) return 1;
	// DBó������� ���� ���̵� Ʋ��(�̹� ������ ��������)
	if(Work.UI.UNum != unum) return 1;
	// DBó������� ���� ���̵� Ʋ��(�̹� ������ ��������)
	if(faststrcmp(Work.UI.ID, User[unum].UI.ID) != 0) return 1;

	/////// ������ ��� ó��

	int index = Work.UseItem.Index;

	// �߸��� ������ �ε����� �Ǻ�
	if(index < 0 || index >= User[unum].GameItem.nNum)
		return 1;

	// ������ ���� ���� ����
	User[unum].GameItem.Item[index] = Work.UseItem;

	// ���� �������̶��
	if(Work.UseItem.Code >= ITEM_SCODE_JUMP && Work.UseItem.Code <= ITEM_ECODE_JUMP) {
		if(!User[unum].bSendAcceptLogin) {
			// ���������� �α��εǾ����� Ŭ���̾�Ʈ���� �α��� �㰡 �޽����� ������ ���� ������
			if(!Work.bErrorItemWork) {
				// �α��� �㰡 �޽����� ����
				User[unum].SendAcceptLoginMsg();
			}
		}
	}
	// ���� ���� �������̶��
	else if(Work.UseItem.Code >= ITEM_SCODE_SUPERMASTER && Work.UseItem.Code <= ITEM_ECODE_SUPERMASTER) {
	}
	// �÷��� ����Ʈ �������̶��
	else if(Work.UseItem.Code >= ITEM_SCODE_PLUSPOINT && Work.UseItem.Code <= ITEM_ECODE_PLUSPOINT) {
	}
	// [��ȣõ��] 2004.07.08
	else if(Work.UseItem.Code >= ITEM_SCODE_SAFEGUARDANGEL && Work.UseItem.Code <= ITEM_ECODE_SAFEGUARDANGEL) {
		INT64 lostMoney = User[unum].Get_lost_money();
		INT64 beforem = User[unum].UI.PMoney;
		if( lostMoney > 0) {	// �Ӵ� ����
			INT64 chargem = Get_angel_defmoney( lostMoney );
			
			User[unum].UI.PMoney += chargem;		

			int cnum = User[unum].ChNum;
			int rnum = User[unum].SUI.RoomNum;			

			//����� ui.pmoney �� �� �־���
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
			// ��ȣõ�� ������ �α� ����� 
			if( Cfg.bAllinDB ) {
				LOGREC_DBWORKS AWork;
				ZeroMemory(&AWork, sizeof(LOGREC_DBWORKS));
				AWork.WorkKind = 4; // [��ȣõ��]
				AWork.StartTick = timeGetTime();
				AWork.bValid = TRUE;
				strncpy(AWork.AllInCharge_Log.ID, User[unum].UI.ID, 15);
				
				AWork.AllInCharge_Log.BeforeMoney = beforem;
				AWork.AllInCharge_Log.AfterMoney = User[unum].UI.PMoney;
				AWork.AllInCharge_Log.AMountMoney = User[unum].UI.PMoney - beforem;
				
				// DB ������ ����
				BOOL rtn = LogDBMan.SetDBWork(&AWork);
				if(rtn == FALSE) {
					CLogFile logfile("AllInLogDB.txt");
					logfile.Writef("�Ѿ� ���� �αױ��  ����");
				}
			}
		}
		User[unum].Set_init_lost_money( 0 );
	}

	// ���� ������ ��� ���
	USEGAMEITEMINFO ug = {0,};
	strncpy(ug.ID, User[unum].UI.ID, 15);
	ug.bUseError = Work.bErrorItemWork;
	ug.Item = Work.UseItem;

	// ���� ������ ��� ����� ����
	CSV_USEGAMEITEM ugmsg;
	ugmsg.Set(&ug);
	User[unum].SendMsg(&ugmsg);

	return 1;
}





BOOL C62CutPokerServerView::UserDisconnectProccess(int unum)
{
	if(unum<1 || unum >= MAX_USER) {
		CLogFile logfile;
		logfile.Writef("UserDisconnectProccess() - �߸��� ������ȣ(unum=%d)", unum);
		return FALSE;
	}

	if(strlen(User[unum].UI.ID)==0) {
		CLogFile logfile;
		logfile.Writef("UserDisconnectProccess() - ���̵� ����(unum=%d)", unum);
		return FALSE;
	}

	// ä�ο� �������ִ� ������� �ش� ä�ο��� ���� ó��
	if(User[unum].ChNum != -1) {
		Chan[User[unum].ChNum].OnOutUser(unum);
	}

	RealUser--;
	if(unum!=g_nAgentIndex) // [���� ������Ʈ �۾�] : ������Ʈ�� �ƴҶ��� DB �۾�
	{ 
		if(!User[unum].bLogOutDBSaved) 
		{
			// ���� ����ġ�� ���������� �ִ� ��츸 ����Ÿ���̽��� ����Ѵ�
		//	if(User[unum].IsSetDBWork()==TRUE) // ��� ������ ���� �˻�
		//	{
				// �̺κп��� DB�� ������Ʈ
				DBWORKS dbwork;
				User[unum].MakeDBWork(4, &dbwork);
				BOOL rtn = DBMan.SetDBWork(&dbwork);

				// DB �۾� ť�� �� �� ��� �α� ���
				if(rtn == FALSE){
					CLogFile LogFile;
					LogFile.Writef(_T("DB Work Que overflow - CServerView::UserDisconnectProccess() : id=%s"), dbwork.UI.ID);
				}
				else User[unum].bLogOutDBSaved = TRUE;
	
		}
	
		//CNetData reData;
		// [���� ������Ʈ �۾�] : �����ڰ� �����Ѵ�!!
		if(g_nAgentIndex>0 && g_nAgentIndex<MAX_USER){
			int nAgentSID = User[g_nAgentIndex].Sid;
			CMySocket *pASocket = SockMan.GetDataSocketPtr(nAgentSID);
			if(pASocket && pASocket->m_bCreated){
				USERVAL *pAUserVal = (USERVAL*)(pASocket->m_pUserPtr);
				if(pAUserVal && pAUserVal->iUser1 && nAgentSID == (int)pASocket->m_dwUserVal){
					AGENT_USER au; memset(&au,0,sizeof(AGENT_USER));
					au.nUNum = unum;

					INT64 GamePMoney = User[unum].UI.PMoney;// �������� ���ӸӴ�
					if(GamePMoney<0) GamePMoney = 0;        // �������� ���ӸӴϴ� �� �¾ƾ��Ѵ�. �������� 0���� ó���� ���´�!!!

					au.PMoney = GamePMoney - User[unum].OI.pBackup;

					strncpy(au.ID,User[unum].UI.ID,15); // ���̵�
					
					CNM_AGENT_OUTUSER aoutuser(SndBuf);
					aoutuser.Set(RealUser, &au, g_JackPotMoney);
					pASocket->Send(aoutuser.pData, aoutuser.GetTotalSize());
				}
			}
		}
	}

	// [���� ������Ʈ �۾�] : ������Ʈ �ε����� �ʱ�ȭ�Ѵ�
	if(unum==g_nAgentIndex){ 
		g_nAgentIndex=-1;
		GetDlgItem(IDC_EDIT_AGENTCONNECT)->SetWindowText(_T("#### No Agent ####")); // [���� ������Ʈ �۾�]

		/////////////////////////////////////////////////
		// [���� ������Ʈ �۾�] ������Ʈ ����� ���Ӱź� ����........
		CMainFrame *pMain = (CMainFrame*) GetParentFrame();
		g_pMainView->bConnectDeny = FALSE;
		pMain->Connectdeny();
		ShowState(_T("������Ʈ ����� ���Ͽ� ������� ������ �ź��մϴ�." ));
		/////////////////////////////////////////////////

		// ������Ʈ �ο� ���� ????
		//RealUser--;
	}


/*
		// �̺κп��� DB�� ������Ʈ
		DBWORKS dbwork;
		User[unum].MakeDBWork(4, &dbwork);
		BOOL rtn = DBMan.SetDBWork(&dbwork);

		// DB �۾� ť�� �� �� ��� �α� ���
		if(rtn == FALSE) {
			CLogFile LogFile;
			LogFile.Writef("DB�۾� ť �����÷ο� - CServerView::UserDisconnectProccess() : id=%s", dbwork.UI.ID);
		}
		else User[unum].bLogOutDBSaved = TRUE;
*/


	// ����� ���� ����ü�� �����
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
		//logfile.Writef("Error : CServerView::OnDataClose() - �������� ���� ����(sid=%d, nSockCode=%d)", sid, nSockCode);
		return 1;
	}

	if(sid != (int)pDSock->m_dwUserVal) {
		CLogFile logfile;
		logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - SID���� Ʋ��(sid=%d, pDSock->m_dwUserVal=%d, nSockCode=%d)", sid, pDSock->m_dwUserVal, nSockCode);
	}

	USERVAL *pUserVal = (USERVAL*)(pDSock->m_pUserPtr);

	int i=0;
	int unum = pUserVal->iUser2; // ����� ���� ��ȣ

	if(pUserVal->iUser1==FALSE && unum != 0) 
	{
		CLogFile logfile;
		if(unum>0 && unum < MAX_USER)
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - pDSock->iUser1==FALSE (ID=%s, UNum=%d, User[unum].UI.UNum=%d, User[unum].Sid=%d, pDSock->hSID=%d)", User[unum].UI.ID, unum, User[unum].UI.UNum, User[unum].Sid, pDSock->m_dwUserVal);
		else
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - pDSock->iUser1==FALSE (������ȣ ���� �ʰ�:UNum=%d)", unum);
	}

	if(pUserVal->iUser1==TRUE)
	{
		// ��ȿ�� ����ڷ� �����
		pUserVal->iUser1 = FALSE;
		pUserVal->iUser2 = 0;

		if(unum<1 || unum >= MAX_USER)
		{
			CLogFile logfile;
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - �߸��� ���� ��ȣ(unum=%d)", unum);
		}
		else if(strlen(User[unum].UI.ID)==0)
		{
			CLogFile logfile;
			logfile.Write("Error : C62CutPokerServerView::OnDataClose() - ���̵��� ���̰� 0�̴�");
		}
		else if(User[unum].Sid != (int)pDSock->m_dwUserVal)
		{
			CLogFile logfile;
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - SID���� Ʋ��(%s ȸ��)", User[unum].UI.ID);
		}

		// ����� ���� ���� ó��
		BOOL rtn = UserDisconnectProccess(unum);

		if(rtn==FALSE)
		{
			CLogFile logfile;
			if(unum>0 && unum < MAX_USER)
				logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - ���� ���� ó�� ���� (ID=%s, UNum=%d, User[unum].UI.UNum=%d, User[unum].Sid=%d, pDSock->m_dwUserVal=%d)", User[unum].UI.ID, unum, User[unum].UI.UNum, User[unum].Sid, pDSock->m_dwUserVal);
			else
				logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - ���� ���� ó�� ���� (������ȣ ���� �ʰ�:UNum=%d)", unum);
		}
	}

	BOOL rtn = SockMan.CloseDataSocket(sid);
	if(rtn==FALSE)
	{
		TRACE("���� ���� ó�� ����\n");

		CLogFile logfile;
		if(unum>0 && unum < MAX_USER)
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - ���� �ݱ� ���� (ID=%s, UNum=%d, User[unum].UI.UNum=%d, User[unum].Sid=%d, pDSock->m_dwUserVal=%d)", User[unum].UI.ID, unum, User[unum].UI.UNum, User[unum].Sid, pDSock->m_dwUserVal);
		else
			logfile.Writef("Error : C62CutPokerServerView::OnDataClose() - ���� �ݱ� ���� (���� ���� - UNum=%d(������ȣ ���� �ʰ�), pDSock->m_dwUserVal=%d)", unum, pDSock->m_dwUserVal);
		return 1;
	}

	// ���� ���� ó��
	TotUser--;
	CString str;
	str.Format("%d��(%d)",TotUser,RealUser);
	((CWnd*)GetDlgItem(IDC_STATIC_TOTALUSER))->SetWindowText(str);

	return 1;
}

LONG C62CutPokerServerView::OnDataConnect(UINT wParam, LONG pSock)
{
	int sid = wParam;
	CMySocket *pDSock = SockMan.GetDataSocketPtr(sid);
	if(pDSock == NULL) return 1;

	// ���� ��� ó��

	//CString str;
	//str.Format("%d�� ����ڰ� Connect�Ǿ���", pDSock->hSID);
	//AfxMessageBox(str);

	return 1;
}

void C62CutPokerServerView::OnButtonSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_ServerMsg.GetLength()==0) return;

	int rtn = AfxMessageBox("��� ����ڿ��� �޼����� �����ðڽ��ϱ�?", MB_YESNO);
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
		//// �ڵ�����
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
	// [ ###����### ] �Ӵ� ����
	ShowState(_T("Close Server...   JackPot.cfg file write..."));
	JackPotSave();


	if(hGlobalGameTimer) KillTimer(GLOBALGAME_TIMER);
	hGlobalGameTimer = NULL;
	if(hMsgPoolingTimer) KillTimer(MSGPOOLING_TIMER);
	hMsgPoolingTimer = NULL;

	// [PC�� �۾�]
	if(hMsgUerCntTimer) KillTimer(hMsgUerCntTimer);
	hMsgUerCntTimer = NULL;

	// ���� �ڵ�����
	m_bExitReserve = FALSE; // �ڵ������ �׽�ŸƮ �ȵ�
	if(hExitReserveTimer) KillTimer(EXITRESERVE_TIMER);
	hExitReserveTimer = NULL;


	MsgDlg.Create(this, "���� �޴����� �����ϴ� ���Դϴ�");

	EnableWindow(FALSE);

	// ���� �޴��� ����
	SockMan.Destroy();

	MsgDlg.SetMsg("DB �����带 �����ϴ� ���Դϴ�");

	// �ҷ� ���̵� �Ű� DB������ ����
	TerminateBadUserDBThread();

	// ����� DB ������Ʈ
	if(TotUser > 0  && g_pMainView->Cfg3.bExamServer==FALSE) //######## [���� ä�� �۾�] #########
	{
		MsgDlg.SetMsg("����� DB�� ����ϴ� ���Դϴ�");

		for(int i=0; i<MAX_USER; i++)
		{

			if(i==g_nAgentIndex) continue; // [���� ������Ʈ �۾�] : �۾��� �ʿ� ����

			if(strlen(User[i].UI.ID) > 0)
			{

				// ���� ����ġ�� ���������� �ִ� ��츸 ����Ÿ���̽��� ����Ѵ�
				//if(User[i].IsSetDBWork()==TRUE){
					// �̺κп��� DB�� ������Ʈ
					DBWORKS dbwork;
					if(!User[i].MakeDBWork(4, &dbwork)) continue;
					BOOL rtn = DBMan.SetDBWork(&dbwork);

					// DB �۾� ť�� �� �� ��� �α� ���
					if(rtn == FALSE) {
						CLogFile LogFile;
						LogFile.Writef(_T("DB Work Que overflow - OnDestroy() : id=%s"), dbwork.UI.ID);
					}
			//	}
			}
		}
	}

	// [ ����� ]
	//if( !bIsForceClose ) CloseServer_SALog(); <- ����
	if( !bIsForceClose ) m_cSALog.CloseServer_savedata(); //<- �߰�

	// Ÿ�̸� �۵��ñ����� 1�ʰ��� �����̸� ���ֱ� ���� ���� ����
	OnCloseTimer();

	// ���� Ÿ�̸Ӹ� �����Ѵ�
	hCloseTimer = SetTimer(CLOSE_TIMER, 1000, NULL);

	// [PC�� �۾�]
	//M_ImDieNow(AfxGetMainWnd()->m_hWnd);
}

void C62CutPokerServerView::OnCloseTimer()
{
	// 1�ʰ������� DBó�� ���� ���θ� �˻��ϰ� 180���� ���� �����Ѵ�
	static int cnt=180;
	if(cnt>0) {
		cnt--;
		DBMan.CheckDBThread();

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
	// ### [�α� ��Ͽ�] ###
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
	// ### [�α� ��Ͽ�] ###
		num1 = LogDBMan.DBQue.GetTotNum();
		work1= LogDBMan.GetWorkingDBThreadNum();
}
#endif
	//	num1 = 10;

		if(num > 0 || work > 0 || num1 > 0 || work1 > 0) {
			CString str;
			if(num > 0 || num1 > 0) str.Format("%d���� ����� DB, %d���� �α� DB�� ����ϴ� ���Դϴ�(���� �ð�: %d��)", num, num1, cnt);
			else str.Format("���� %d���� �����DB, %d�α�DB �����尡 Ȱ�� ���Դϴ�(���� �ð�: %d��)", work, work1, cnt);
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

	MsgDlg.SetMsg("������ ����ó�� ��...");

	DBMan.TermanateAllDBThread();


#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
	MsgDlg.SetMsg("�α� ��� ������ ����ó�� ��...");
	// ### [�α� ��Ͽ�] ###
	LogDBMan.TermanateAllDBThread();
}
#endif

	MsgDlg.DestroyWindow();

	AfxGetMainWnd()->PostMessage(WM_QUIT);
	// [PC�� �۾�]							// �߰�
	M_ImDieNow(AfxGetMainWnd()->m_hWnd);	// �߰�
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
					// �߸��� ��Ŷ Ȥ�� �������� ���Ͽ� ���� ����
					SockMan.ReservClose(sid);
					break;
				}
				else break;
			}
		}
	}
	else if(nIDEvent == GLOBALGAME_TIMER)
	{
		// �ʴ��� ���� ī����
		static UINT totcnt = 0;
		totcnt++;

		if(totcnt>=10000) totcnt = 40; // UINT���� �����÷ο� ����

		// �� ä�� ���� Ÿ�̸� ó��
		for(int i=0; i<MAX_CHAN; i++) 
		{
			if(Chan[i].TotUser > 0)	Chan[i].OnGlobalGameTimer();
		}

		// DB ������ Ÿ�Ӿƿ� ó��
		DBMan.CheckDBThread();

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
		// ### [�α� ��Ͽ�] ###
		LogDBMan.CheckDBThread();
}
#endif

		
		// ���� Ÿ�Ӿƿ� ó�� �� �ݱ� �ð� �ʰ� ���� �˻�
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
					// ��ȿ�� ����ڶ�� ���� Ÿ�� ī��Ʈ ����
					if(unum>0 && unum<MAX_USER) 
					{
						User[unum].ConnectSec++;
						if(unum !=g_nAgentIndex)
						{
							// [���� ������Ʈ �۾�] : ������ �ð��� �ߺ����� üũ
							if(User[unum].ConnectSec ==  15 || //  15�ʿ� �ѹ�...
							   User[unum].ConnectSec == 100 ){ // 100�ʿ� �ѹ�...

								FindDoubleConnect(unum); //: �ߺ������� Ȯ���Ѵ�
							}
						}
						// 60�� �������� heart beat�� ����  [SSO �۾�]
						if(totcnt%60 == 0) {
							CSV_HEARTBEAT hbmsg(SndBuf);
							hbmsg.Set();
							SockMan.SendData(i, hbmsg.pData, hbmsg.GetTotalSize());
						}
					}
				}
				else
				{
					// ��ȿ���� ���� ������̸� 10�ʰ������� heart beat�� ����
					if(totcnt%10 == 0) {
						CSV_HEARTBEAT hbmsg(SndBuf);
						hbmsg.Set();
						SockMan.SendData(i, hbmsg.pData, hbmsg.GetTotalSize());
					}
				}

				// Ÿ�Ӿƿ� ī��Ʈ ����
				pUserVal->iTimeOutCnt++;

				// Ÿ�Ӿƿ� ���� - jeong
				pUserVal->iTimeOutCnt = 0;

				// 3������ �̸� Ÿ�Ӿƿ��� �˸�
				if(pUserVal->iTimeOutCnt == MAX_TIMEOUT-3)
				{
					// ����ڿ��� �����޼����� ����
					CSV_SERVERMSG smsg;
					smsg.Set(1, "���� �ð� ���� �������� ������ �������Ƿ� ������ �������ϴ�.");
					SockMan.SendData(i, smsg.pData, smsg.GetTotalSize());
				}
				// �ִ� Ÿ�Ӿƿ� �ð��� �ѱ�� ���� �����Ŵ
				else if(pUserVal->iTimeOutCnt >= MAX_TIMEOUT)
				{
					pUserVal->iTimeOutCnt=0;
					SockMan.ReservClose(i); // ���� ���� ����
					continue;
				}
			}
		}

		// [������ ��� �۾�]
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
				logfile.Writef("������ Ÿ�Ӿƿ� ���� : C62CutPokerServerView::OnTimer() - [%s]ȸ�� : sid ���� �ʰ�(User[i].Sid=%d)", User[i].UI.ID, User[i].Sid);

				UserDisconnectProccess(i);
				continue;
			}

			if(SockMan.pDataSock[sid] == NULL)
			{
				CLogFile logfile;
				logfile.Writef("������ Ÿ�Ӿƿ� ���� : C62CutPokerServerView::OnTimer() - [%s]ȸ�� : SockMan.pDataSock[sid] == NULL", User[i].UI.ID);

				UserDisconnectProccess(i);
				continue;
			}
		}

		// ### [�̺�Ʈ Ƽ��] ###
		CheckGameEvent();

		// [ ����� ]
		//Check_StatisticsLog(); <- ����
		m_cSALog.Time_check(); //<- �߰�

	}
	else if(nIDEvent == CLOSE_TIMER)
	{
		OnCloseTimer();
	}
	// [PC�� �۾�]
	else if(nIDEvent == USERCNT_TIMER) {	
		SendUserSizeToBridge(AfxGetMainWnd()->m_hWnd, RealUser);
	}
	else if(nIDEvent == EXITRESERVE_TIMER)
	{////////////////////////////////////////////////////////////////
	 ///// �ڵ�����
		ExitReserveTimer(); // �������� ����
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
		AfxMessageBox("���� ������ �� ��ȣ�� �Է��ϼ���.");
		return;
	}

	int rnum = ::atoi(m_DelRoomNo);
	if(rnum<1 || rnum>=MAX_ROOM)
	{
		AfxMessageBox("�� ��ȣ�� ������ �ʰ��Ͽ����ϴ�. �ùٸ��� �Է��ϼ���");
		return;
	}

	CString msg, str;
	str.Format("���ȣ: %d (RoomNum=%d)\n", rnum, Room[rnum].Ri.RoomNum);
	msg += str;
	str.Format("������: %s\n", Room[rnum].Ri.Title);
	msg += str;
	str.Format("��  ��: %s (UNum=%d)\n", Room[rnum].Ri.ID, Room[rnum].Ri.UNum);
	msg += str;
	str.Format("��  ��: %d/%d\n", Room[rnum].Ri.NowUserNum, Room[rnum].Ri.MaxUserNum);
	msg += str;
	str.Format("��  ��: [%s]\n", Room[rnum].Ri.Pass);
	msg += str;
	msg += "\n���� ���ӹ��� �����Ͻðڽ��ϱ�?";

	int rtn = ::MessageBox(m_hWnd, msg, "�� ���� Ȯ��", MB_YESNO);
	if(rtn == IDNO) return;

	// ��� ����ڿ��� ���� �������� �˸�
	CSV_OUTROOM ormsg;
	SMALLROOMINFO sri = Room[rnum].GetSmallRoomInfo();
	ormsg.Set(&sri);
	SendMsgToAll(ormsg.pData, ormsg.GetTotalSize());
	
	// �� ����
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
		strtmp.Format("���� ������ ����� ID�� �Է��ϼ���\n\n"); str += strtmp;
		strtmp.Format("=========== ���� ���� ============\n"); str += strtmp;
		strtmp.Format("���� ���Ӽ��� ��=%d, ���� ����� ��=%d\n", realtotsock, realtotuser); str += strtmp;
		strtmp.Format("�ùٸ� UNUM ����=%d\n", sameusernum); str += strtmp;
		strtmp.Format("�ùٸ� ���� SID����=%d\n", sameusersid); str += strtmp;
		strtmp.Format("�ùٸ� ���� SID����=%d\n", samesocksid); str += strtmp;
		strtmp.Format("���� ����� ���ϰ���=%d\n", closepostsocknum); str += strtmp;
		strtmp.Format("�߸��� ���ϰ���=%d\n", invalidsocknum); str += strtmp;
		strtmp.Format("�����ʰ� ���� SID����=%d\n", oversocksid); str += strtmp;
		strtmp.Format("������ DB������ ����=%d\n", DBMan.GetLiveDBThreadNum()); str += strtmp;
		strtmp.Format("Ȱ������ DB������ ����=%d\n", DBMan.GetWorkingDBThreadNum()); str += strtmp;

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
		// ### [�α� ��Ͽ�] ###
		strtmp.Format("������ �αױ�Ͽ� DB������ ����=%d\n", LogDBMan.GetLiveDBThreadNum()); str += strtmp;
		strtmp.Format("Ȱ������ �αױ�Ͽ� DB������ ����=%d\n", LogDBMan.GetWorkingDBThreadNum()); str += strtmp;
}
#endif
		strtmp.Format("���� DBó����=%d\n", DBMan.DBQue.GetTotNum()); str += strtmp; // [SSO �۾�] - ����

#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB)
{
		// ### [�α� ��Ͽ�] ###
		strtmp.Format("���� ������ DBó����=%d\n", LogDBMan.DBQue.GetTotNum()); str += strtmp;
}
#endif
		strtmp.Format("���� �ҷ����̵� �Ű� DBó����=%d\n", BadUserDBQue.TotNum); str += strtmp;
		strtmp.Format("�κ�â Open ������=%d\n", lobyopennum); str += strtmp;
		AfxMessageBox(str);
		return;
	}

	CString str;
	for(int i=0; i<MAX_USER; i++)
	{
		if(m_DiscTargetID.Compare(User[i].UI.ID)==0) {
			int sid = User[i].Sid;
			str.Format("[%s]ȸ���� ���������Ͻðڽ��ϱ�?(sid=%d, Adress is %s)\n",User[i].UI.ID, sid, (SockMan.pDataSock[sid]==NULL)?"NULL!":"Valid.");
			int rtn = AfxMessageBox(str,MB_YESNO);
			if(rtn==IDYES) {
				USERVAL *pUserVal = NULL;
				if(SockMan.pDataSock[sid]!=NULL) pUserVal = (USERVAL*)(SockMan.pDataSock[sid]->m_pUserPtr);

				if(SockMan.pDataSock[sid]==NULL) {
					AfxMessageBox("���� �����Ͱ� NULL�Դϴ�. �ٸ�������� ���������ϰڽ��ϴ�.");
					int unum = User[i].UI.UNum;
					if(unum<0 || unum>=MAX_USER) {
						AfxMessageBox("UserInfo Struct�� UNum���� ������ �ʰ��Ͽ����ϴ�.");
						unum = i;
					}

					if(faststrcmp(User[unum].UI.ID, User[i].UI.ID) != 0) {
						rtn = AfxMessageBox("������ȣ�� �����ID�� ��ġ���� �ʽ��ϴ�. �׷��� ���������Ű�ڽ��ϱ�?", MB_YESNO);
						if(rtn==IDNO) return;
					}
					UserDisconnectProccess(i);
				}
				else if(pUserVal == NULL) {
					AfxMessageBox("pUserVal ���� NULL�Դϴ�. �ٸ�������� ���������ϰڽ��ϴ�.");
					UserDisconnectProccess(i);
					SockMan.ReservClose(sid); // ���� ���� ����
				}
				else if(pUserVal->iUser1 == FALSE) {
					AfxMessageBox("iUser1�� ���� FALSE�Դϴ�. �ٸ�������� ���������ϰڽ��ϴ�.");
					int unum = User[i].UI.UNum;
					if(unum<0 || unum>=MAX_USER) {
						AfxMessageBox("UserInfo Struct�� UNum���� ������ �ʰ��Ͽ����ϴ�.");
						unum = i;
					}
					else if(sid != User[i].Sid) AfxMessageBox("SID���� ���� �ٸ��ϴ�");

					if(faststrcmp(User[unum].UI.ID, User[i].UI.ID) != 0) {
						rtn = AfxMessageBox("������ȣ�� �����ID�� ��ġ���� �ʽ��ϴ�. �׷��� ���������Ű�ڽ��ϱ�?", MB_YESNO);
						if(rtn==IDNO) return;
					}
					UserDisconnectProccess(i);
				}
				else SockMan.ReservClose(sid); // ���� ���� ����
			}
			return;
		}
	}
	str.Format("[%s]ȸ���� �������� ���� ���̵�ų� �������� �ʽ��ϴ�", m_DiscTargetID.operator LPCTSTR());
	AfxMessageBox(str);
}





// ��ü ����� �α������ۼ� Ÿ��Ʋ�� �����Ѵ�
CString C62CutPokerServerView::GetLogFileName_view()
{//��� ���� �ʴ´�
	CString strFileName(_T(""));

	CTime  time = CTime::GetCurrentTime();

	strFileName.Format(g_StrMan.Get(_T("SPOKER_BACKUP")),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute());

	return strFileName;
}



// [ ###����### ] ���� ����� �����Ѵ�.
void C62CutPokerServerView::JackPotSave()
{

	g_pMainView->Cfg4.nJackPotMoney = g_JackPotMoney;

	FILE *fp = fopen( "JackPot.cfg", "wb");
	if( fp == NULL ) { 
		AfxMessageBox("���� ������ ����� �� �����ϴ�.");
		return;
	}
	fwrite(&g_pMainView->Cfg4, sizeof(CONFIG4), 1, fp);
	fclose(fp);

}

// [ ###����### ]  ���� �Ӵ� ����� �ӽ�����
void C62CutPokerServerView::OnButtonJackpot() 
{
	// TODO: Add your control notification handler code here
	// ���� ���̱�
	JackPotSave();
	CString strM = g_MakeCommaMoney(g_JackPotMoney);
	ShowState((char*)strM.operator LPCTSTR());
}


////////////////////////////////////////////////////////////////
///// �ڵ�����
// ���� ������ �۾��� ���δ�
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
///// �ڵ�����
void C62CutPokerServerView::OnRadioExit30s() 
{//30��
	// TODO: Add your control notification handler code here
	SendExitMode = 0;
	m_ReservTimeLimit = 30;
	m_iExitRadio =5;
	UpdateData(FALSE);
	
}

void C62CutPokerServerView::OnRadioExit1() 
{//1��
	// TODO: Add your control notification handler code here
	SendExitMode = 1;
	m_ReservTimeLimit = 60;
	m_iExitRadio = 4;
	UpdateData(FALSE);
	
}

void C62CutPokerServerView::OnRadioExit3() 
{//3��
	// TODO: Add your control notification handler code here
	SendExitMode = 2;
	m_ReservTimeLimit = 180;
	m_iExitRadio = 3;
	UpdateData(FALSE);
}

void C62CutPokerServerView::OnRadioExit5() 
{//5��
	// TODO: Add your control notification handler code here
	SendExitMode = 3;
	m_ReservTimeLimit = 300;
	m_iExitRadio = 2;
	UpdateData(FALSE);
	
}

void C62CutPokerServerView::OnRadioExit10() 
{//10��
	// TODO: Add your control notification handler code here
	SendExitMode = 4;
	m_ReservTimeLimit = 600;
	m_iExitRadio = 1;
	UpdateData(FALSE);
	
}

void C62CutPokerServerView::OnRadioExit30() 
{//30��
	// TODO: Add your control notification handler code here
	SendExitMode = 5;
	m_ReservTimeLimit = 1800;
	m_iExitRadio = 0;
	UpdateData(FALSE);
	
}

////////////////////////////////////////////////////////////////
///// �ڵ�����
void C62CutPokerServerView::OnButtonExitok() 
{
	// TODO: Add your control notification handler code here
	int rtn = AfxMessageBox("���� �ڵ� ���Ḧ ���� �Ͻðڽ��ϱ�?", MB_YESNO);
	if(rtn==IDYES) {

		
		if(m_ServerMsg.GetLength()==0) {
			
			m_ServerMsg.Format(" �ȳ��ϼ���!! ��� �Դϴ�. ���� ���� ����� ���Ͽ� ������ ���� �ϰڽ��ϴ�.");
		}
		

		int Mode = max(0, SendMsgMode);
		CString str;
		str = m_ServerMsg;
		if(SendExitMode==0) {

			str += " 30�� �Ŀ� �ڵ� ����˴ϴ�.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		
			ShowState(_T(str.operator LPCTSTR() ));
			
		}
		else if(SendExitMode==1) 
		{
			str += " 1�� �Ŀ� �ڵ� ����˴ϴ�.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			ShowState(_T(str.operator LPCTSTR() ));
			
		} 
		else if(SendExitMode==2) 
		{
			str += " 3�� �Ŀ� �ڵ� ����˴ϴ�.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			ShowState(_T(str.operator LPCTSTR() ));
		}
		else if(SendExitMode==3) 
		{
			str += " 5�� �Ŀ� �ڵ� ����˴ϴ�.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			ShowState(_T(str.operator LPCTSTR() ));
		}
		else if(SendExitMode==4) 
		{
			str += " 10���Ŀ� �ڵ� ����˴ϴ�.";
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			ShowState(_T(str.operator LPCTSTR() ));
		}
		else
		{
			str += " 30�� �Ŀ� �ڵ� ����˴ϴ�.";
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
///// �ڵ�����
void C62CutPokerServerView::OnButtonExitcancel() 
{
	// TODO: Add your control notification handler code here
	int rtn = AfxMessageBox("�����ڵ� ���� �޼����� ��� �Ͻðڽ��ϱ�?", MB_YESNO);
	if(rtn==IDYES) {

		m_bExitReserve = FALSE;
		

		if(hExitReserveTimer) KillTimer(EXITRESERVE_TIMER);
		hExitReserveTimer = NULL;

		
		m_ExitReserve.EnableWindow(TRUE);
		m_ExitCancel.EnableWindow(FALSE);

		ShowState(_T("���� �ڵ����Ḧ ����ϼ̽��ϴ�." ));
		m_ServerMsg = "";

		UpdateData(FALSE);
	}
	
}


////////////////////////////////////////////////////////////////
///// �ڵ�����
void C62CutPokerServerView::ExitReserveTimer()
{
	int ReservCnt = m_ReservTimeLimit--;

	// �ؽ�Ʈ, �޼��� �ڽ� ����
	int Mode = max(0, SendMsgMode);

	if(SendExitMode==0) 
	{// 30��
		CString str;
		str.Format(" %d �� �Ŀ� �ڵ� ����˴ϴ�.", ReservCnt);
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
	{// 1��
		CString str;
		str.Format(" %d �� �Ŀ� �ڵ� ����˴ϴ�.", ReservCnt);
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
	{// 3�� �̻�~~
		CString str;
		BOOL bFlag = FALSE;
				
		if(m_ReservTimeLimit >= 60 && (ReservCnt%60 == 0) )
		{
			str = "";
			str.Format(" %d �� �Ŀ� �ڵ� ����˴ϴ�.", ReservCnt/60);
			
			CSV_SERVERMSG smsg;
			smsg.Set(Mode, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
			
			bFlag = TRUE;
			
		}
		else if(m_ReservTimeLimit <= 30 && (ReservCnt%10 == 0) )
		{
			
			str = "";
			str.Format(" %d �� �Ŀ� �ڵ� ����˴ϴ�.", ReservCnt);

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
				str.Format(" %d �� %d�� �Ŀ� �ڵ� ����˴ϴ�.  SendMessage for All User!", minute, second);
			else
				str.Format(" %d �� %d�� �Ŀ� �ڵ� ����˴ϴ�.", minute, second);

			ShowState(_T(str.operator LPCTSTR() ));
		}
		else 
		{
			str = "";

			if(bFlag)
				str.Format(" %d �� �Ŀ� �ڵ� ����˴ϴ�.  SendMessage for All User!", ReservCnt);
			else
				str.Format(" %d �� �Ŀ� �ڵ� ����˴ϴ�.", ReservCnt);
			
			ShowState(_T(str.operator LPCTSTR() ));
		}

	} 

	if(m_ReservTimeLimit < 0)
	{
		CString str1;
		str1.Format("������ �����մϴ�.");

		CSV_SERVERMSG smsg;
		smsg.Set(0, (char*)str1.operator LPCTSTR());
		SendMsgToAll(smsg.pData, smsg.GetTotalSize());

		ShowState(_T(str1.operator LPCTSTR() ));
		
		// ���� �۾�
		PrepareClose();
	}
}



///////////////////////////////////////////////////////////////////
/////////////////// ### [������ ��� �۾�] ###  ///////////////////

// [������ ��� �۾�]
void C62CutPokerServerView::OnSetAdmin() 
{
	// TODO: Add your command handler code here
	CAdminManager dlg;
	dlg.DoModal();
	
}


// [������ ��� �۾�]  �о�´�
void C62CutPokerServerView::LoadAdminInfo()
{
	// ȯ�漳�� ������ ����
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

// [������ ��� �۾�] ������ ���̵��ΰ�? �ƴѰ�?
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

// [������ ��� �۾�] ���������� �����ΰ�? �ƴѰ�?
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


// [������ ��� �۾�]
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
	else AfxMessageBox("���Ӱźε� ����ڰ� �����ϴ�");
	
}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////




// [���� ������Ʈ �۾�] : �ߺ������� Ȯ���Ѵ�
void C62CutPokerServerView::FindDoubleConnect(const int unum) 
{
	// �ϴ��� ��/��/��/�� ���� ���� �׽�Ʈ�غ���
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
			strncpy(au.ID,User[unum].UI.ID,15); // ���̵�

			CNM_AGENT_DUBLECON duser(SndBuf);
			duser.Set(RealUser, &au);
			pASocket->Send(duser.pData, duser.GetTotalSize());
		}
	}
}



// ���� ��������
BOOL C62CutPokerServerView::CheckLevel(int nIcon, INT64 money)
{

	// ### [ ä�� ���� ���� ] ###
	// ���� ���
	switch(g_ServerGrade)
	{
	case 0: // ��������
		{
			return TRUE;
		} break;
		
	case 1:// �ʺ�����
		{
			if(nIcon >= 0 && nIcon <= 3) return TRUE;
			// ���� 
			if(nIcon == 4 && money <= 1500000000) return TRUE; // 15��
						
		} break;
	case 2: // �߼����� 
		{
			if(nIcon >= 4 && nIcon <= 7) return TRUE;
			// ���� 
			if(nIcon == 8 && money <= 55000000000) return TRUE; // 1500��
			
		} break;
	case 3: // �������
		{
			if(nIcon >= 8 && nIcon <= 11) return TRUE;
			// ���� 
			if(nIcon == 12 && money <= 150000000000) return TRUE; //1050��

			//���� �߰�
			if(nIcon >= 12) return TRUE; // ��� �������� ���ŵ� ���尡��
			
		} break;
	case 4: // ���ż��� 
		{
			if(nIcon >= 12 && nIcon <= 14) return TRUE;
			
		} break;
	default : // ��������
		{
			return TRUE;
		}
		break;
	}

	return FALSE;

}



// �߰����� ����Ѵ�
LONG C62CutPokerServerView::OnDbworkSetOK(UINT wParam, LONG lParam)
{
	return 1; // [����� ��� ����] ���� ó������ �ʴ´�

	/*
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memset(&Work,0,sizeof(DBWORKS));
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB ó���� ���������� ������ ������ ������� �� ����
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	if(sid != (int)pSocket->m_dwUserVal){
		return 1;
	}

	// ������� ���̵����� �˻�
	BOOL bConnected=FALSE;
	int  nFindUNum = 0;
	for(int i=0; i<MAX_USER; i++) {
		if(User[i].bValid ) {
//			if(faststrcmp(User[i].UI.ID, Work.UI.ID) == 0 && sid == User[i].Sid && Work.StartTick==User[i].UniqueTick) { // ��Ȯ�� ���ӿ��� Ȯ��
			if(faststrcmp(User[i].UI.ID, Work.UI.ID) == 0 && sid == User[i].Sid) { // ��Ȯ�� ���ӿ��� Ȯ��
				bConnected = TRUE; 
				nFindUNum  = i;
				break;
			}
		}
	}

	// �������� ���̵��� ó���Ѵ�
	if(bConnected && nFindUNum<MAX_USER){
		if(User[nFindUNum].bIsUpdateData){
			User[nFindUNum].bIsUpdateData = FALSE;
			if(Work.bDBSetOK){ // ���������� ����Ÿ���̽��� ��ϵǾ���
				User[nFindUNum].UI.pBackup = Work.UI.PMoney; // ����ġ������ �ֽ� �� ����
				User[nFindUNum].nGameNum   = 0;              // ����Ƚ�� ���ʱ�ȭ 20 ȸ
			}
		}
	}

	return 1;
*/
}


// �ߺ��������̴�!!
LONG C62CutPokerServerView::OnDbworkDoubleConnect(UINT wParam, LONG lParam)
{
//	CNetData reData; LPTSTR buf = _T("sign");

	return 1; // [����� ��� ����] ���� ó������ �ʴ´�

/*
	if(lParam == NULL) return 1;
	DBWORKS Work;
	memset(&Work,0,sizeof(DBWORKS));
	memcpy(&Work, (void*)lParam, sizeof(DBWORKS));

	int sid = Work.SID;
	CMySocket *pSocket = SockMan.GetDataSocketPtr(sid);

	// DB ó���� ���������� ������ ������ ������� �� ����
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	if(sid != (int)pSocket->m_dwUserVal){
		return 1;
	}

	// ������� ���̵����� �˻�
	BOOL bConnected=FALSE;
	int  nFindUNum = 0;
	for(int i=0; i<MAX_USER; i++) {
		if(User[i].bValid ) {
			if(faststrcmp(User[i].UI.ID, Work.UI.ID) == 0 && sid == User[i].Sid) { // ��Ȯ�� ���ӿ��� Ȯ��
				bConnected = TRUE; 
				nFindUNum  = i;
				break;
			}
		}
	}

	// �������� ���̵��� ó���Ѵ� (�ߺ��������̹Ƿ� ����ó���Ѵ�!)
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
		SockMan.ReservClose(sid); // ���� ���� ����
	}

	return 1;
*/
}

//	[��ȭ ����]	
void C62CutPokerServerView::LoadCaptureTalkID()
{
	// ȯ�漳�� ������ ����
	ZeroMemory(&g_CaptureID, sizeof(g_CaptureID));
	FILE *fp = fopen( "CaptureID.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("Can't find [CaptureID.cfg] file!\nSetup CaptureID config!");
		return;
	}
	fread(&g_CaptureID, sizeof(g_CaptureID), 1, fp);
	fclose(fp);

}


//	[��ȭ ����]
void C62CutPokerServerView::OnSetCaptureid() 
{
	// TODO: Add your command handler code here
	CCaptureTalk dlg;
	dlg.DoModal();
}

//	[��ȭ ����]	 ���̵��ΰ�? �ƴѰ�?
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


// [��ġ ����] 
void C62CutPokerServerView::LoadSearchID()
{
		// ȯ�漳�� ������ ����
	ZeroMemory(&g_SearchID, sizeof(g_SearchID));
	FILE *fp = fopen( "SearchID.cfg", "rb");
	if( fp == NULL ) { 
		AfxMessageBox("Can't find [SearchID.cfg] file!\nSetup SearchID config!");
		return;
	}
	fread(&g_SearchID, sizeof(g_SearchID), 1, fp);
	fclose(fp);

}


// [��ġ ����] 
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
// [��ġ ����] 
#include "SearchDlg.h"
void C62CutPokerServerView::OnSetSearch() 
{
	// TODO: Add your command handler code here
	CSearchDlg dlg;
	dlg.DoModal();
	
}


// ### [�̺�Ʈ Ƽ��] ###
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

	// DB ó���� ���������� ������ ������ ������� �� ����
	if(pSocket==NULL) return 1;
	if(!pSocket->m_bCreated) return 1;

	USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);

	int unum = pUserVal->iUser2;
	if(unum<1 || unum>=MAX_USER) return 1;
	if(!User[unum].bValid) return 1;

	// DB ó���� ���������� ������ ������ ���� �ٸ� ����ڰ� ������ ����� �� ����
	if(sid != (int)pSocket->m_dwUserVal) return 1;
	// DBó������� ���� ���̵� Ʋ��(�̹� ������ ��������)
	if(Work.UI.UNum != unum) return 1;
	// DBó������� ���� ���̵� Ʋ��(�̹� ������ ��������)
	if(faststrcmp(Work.UI.ID, User[unum].UI.ID) != 0) return 1;

	/////// ����
	User[unum].EventPrize.EventPrize = Work.EventPrize.EventPrize;
	User[unum].EventPrize.OrigEventPrize = Work.EventPrize.OrigEventPrize; //�����
	User[unum].EventPrize.GameCnt = Work.EventPrize.GameCnt;
	
	return 1;
}


void C62CutPokerServerView::CheckGameEvent()
{
	if(Cfg.bEventDB != TRUE) return;
	if(EventCfg.bRunEvent != TRUE) return;

	// �̺�Ʈ �Ⱓ�� �߸� �����Ǿ� �ִ��� �˻�
	if(EventCfg.StartDate > EventCfg.EndDate) return;

	// �̺�Ʈ �����ϰ� �������� �˻�
	CTime nowtime = CTime::GetCurrentTime();
	if(nowtime < EventCfg.StartDate || nowtime > EventCfg.EndDate) {
		// �ڵ� �̺�Ʈ ���ᰡ �����Ǿ� �ִٸ� �̺�Ʈ ����
		if(EventCfg.bAutoEventEnd) EventCfg.bRunEvent = FALSE;
		
		if( g_pMainView->Cfg.bEventDB && g_pMainView->EventCfg.bRunEvent ) {	// ### [�̺�Ʈ Ƽ��] ###
			g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("�� EventDB Use");
		} else if( g_pMainView->Cfg.bEventDB ) {
			g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("�� EventDB Use");
		} else {
			g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("�� EventDB Use");
		}
		return;
	}

	// Ư���ð��� �Ǹ� �̺�Ʈ���� �ʱ�ȭ
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
		AfxMessageBox("�̺�Ʈ�� �����Ǿ� ���� �ʽ��ϴ�.");
		return;
	}

	if(EventCfg.StartDate > EventCfg.EndDate) {
		AfxMessageBox("�̺�Ʈ �Ⱓ�� �߸� �����Ǿ� �ֽ��ϴ�");
		return;
	}

	CTime nowtime = CTime::GetCurrentTime();
	if( nowtime < EventCfg.StartDate || nowtime > EventCfg.EndDate) {
		str.Format("�� �̺�Ʈ �Ⱓ�� ����Ǿ����ϴ� ��\n\n");
		strevent += str;
	}
	else {
		BOOL bNoEventTime = FALSE;
		if(EventCfg.bNoEventTime) {
			// �̺�Ʈ �߻� ���� ���� �ð�
			CTime st(nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(), EventCfg.NoEventStartHour, 0, 0);
			// �̺�Ʈ �߻� ���� ���� �ð�
			CTime et = st;
			CTimeSpan tt(0, EventCfg.NoEventHour, 0, 0);
			et += tt;
			// ���� �ð��� �̺�Ʈ �߻��� ������ �ð����� �˻�
			if(nowtime >= st && nowtime <= et) 	bNoEventTime = TRUE;
		}

		if(bNoEventTime == FALSE) {
			str.Format("�� ���� �̺�Ʈ�� �������Դϴ� ��\n\n");
			strevent += str;
		}
		else {
			str.Format("�� ���� �̺�Ʈ�� �������̳� ������ �̺�Ʈ �߻��� ������ �ð��Դϴ١�\n\n");
			strevent += str;
		}
	}

	str.Format("�� ���޵� Ƽ�� ��: %d/%d \n", TotEventTicketNum, EventCfg.EventTicketNum);
	strevent += str;
	str.Format("���޵� ���� Ƽ�� ��: %d \n", NowEventTicketNum);
	strevent += str;

	AfxMessageBox(strevent);
}


// ### [�̺�Ʈ Ƽ��] ###
void C62CutPokerServerView::SendChannelEventNotice(int unum)
{
	// �̺�Ʈ�� ���������� �ʴٸ� ����
	if(!g_pMainView->EventCfg.bRunEvent) return;

	if(unum < 1 || unum >= MAX_USER) return;
	if(User[unum].bValid==FALSE) return; // �߸��� ������ȣ�̴�
	if(strlen(User[unum].UI.ID)==0) return;

	if(EventCfg.bRunEvent) {
		// �̺�Ʈ ������ �ִٸ�
		if(strlen(EventCfg.EventNotice) > 0) {
			// ����ڿ��� �����޼����� ����
			
			CString str1 = EventCfg.EventNotice;
			CString str2;
			str2.Format("���� [%s]ȸ������ ȹ���� �̺�Ʈ ������ %d���Դϴ�. ��Ŀ2������  30���̻� �Ͻø� �̺�Ʈ ������ 3�� �帳�ϴ�.(1�� 1ȸ) �̺�Ʈ ������ ��ڰ�� �������� �̺�Ʈ�� Ȯ���� �ּ���~~!",
						User[unum].UI.ID, User[unum].EventPrize.EventPrize);
			str1+="\n";
			str1+=str2;
			
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(2, (char*)str1.operator LPCTSTR(), TRUE, RGB(200,0,0));
			User[unum].SendMsg(&smsg);
		}
	}
}

// �����
void C62CutPokerServerView::LoadConfigStatis()
{
	// ȯ�漳�� ������ ����
	ZeroMemory(&StatisCfg, sizeof(CONFIGSTATIS));

	FILE *fp = fopen( "statistics.cfg", "rb");
	if( fp == NULL ) { 
		//�ʱⰪ
		AfxMessageBox("����Ͽ� ���� ������ ���� �� �����ϴ�.\n�ٽ� �����ϼ���.");
		return;
	}
	fread(&StatisCfg, sizeof(CONFIGSTATIS), 1, fp);
	fclose(fp);
}


// �����
void C62CutPokerServerView::OnViewConfigsa() 
{
	// TODO: Add your command handler code here
	CConfigSADlg Dlg;
	Dlg.DoModal();
	
}
/*
// [ ����� ]
void C62CutPokerServerView::Make_StatisticsLog(int Kind, int index, BOOL bIncrease, INT64 pmoney, INT64 usurymoney)
{
	if( !Cfg.bStatisticsDB || !StatisCfg.bSaveSetLog ) return;
	if(Kind == LEVEL_CONCURRENT_USER) { 
		// 11 ���� ���ϱ�, ���μ�, ���� ���ӽð�
		int Level = User[index].m_UserLevel;
		if( bIncrease ) {
			m_StatisticsLog.Level_CurUser[ Level ] += 1;		// ������ ����
		} else {
			m_StatisticsLog.Level_CurUser[ Level ] -= 1;		// ������ ����
			if( User[index].UI.PMoney <=  0) m_StatisticsLog.Level_AllIn_User[ Level ] += 1; // ���μ� ����
			COleDateTime nt = COleDateTime::GetCurrentTime();
			COleDateTimeSpan span = nt - User[index].m_StartTime;
			if(User[index].OI.WinNum != User[index].UI.WinNum ||
				User[index].OI.LooseNum != User[index].UI.LooseNum ||
				User[index].OI.DrawNum != User[index].UI.DrawNum )
			{
				int time = span.GetSeconds();	
				m_StatisticsLog.Level_UseTime[ Level ] += time;	// ���� ���ӽð�
				m_StatisticsLog.Level_UseTimeUserCnt[ Level ] += 1;			
			}
		}
	} else if(Kind == LEVEL_USE_GAMECNT) {						// 13 �������Ӽ�
		int Level = User[index].m_UserLevel;
		m_StatisticsLog.Level_UseGameCnt[ Level ] += 1;
	} else if(Kind == GRADE_AVERAGE_MONEY) {					// ��޺� ���ӸӴ�, ���Ӻ�
		int grade = index; // ä�� ����
		if(pmoney > 10000000) {
			m_StatisticsLog.Grade_GameCnt[ grade ]	+= 1;			// ��޺� �����Ǽ�
			m_StatisticsLog.Grade_AverGameMoney[ grade ]+= pmoney	/ 10000000  ;	// �� ���øӴ�
		}
		if(usurymoney > 1000) {
			m_StatisticsLog.Grade_UsuryCnt[ grade ]	+= 1;			// ��޺� �����Ǽ�
			m_StatisticsLog.Grade_UsuryMoney[ grade ]+= usurymoney	/ 1000;	// ���Ӻ�	
		}
	} else if(Kind == GRADE_MISSION) {							// �̼� ����, ���� �Ӵ�
		int grade = index; // ä�� ����
		m_StatisticsLog.Grade_MissonCnt[ grade ] += 1;
		m_StatisticsLog.Grade_MissonMoney[ grade ] += pmoney    / 100000000;
	} else if(Kind == GRADE_JACKPOT) {							// ���� ����, ���޸Ӵ�
		int grade = index; // ä�� ����
		m_StatisticsLog.Grade_JackPotCnt[ grade ] += 1;
		m_StatisticsLog.Grade_JackPotMoney[ grade ] += pmoney   / 100000000;
	}
}

// [ ����� ]
void C62CutPokerServerView::Make_SetSADBWork(int Kind)
{
	LOGREC_DBWORKS SWork;
	ZeroMemory(&SWork, sizeof(LOGREC_DBWORKS));
	SWork.WorkKind = 2; 
	SWork.StartTick = timeGetTime();
	SWork.bValid = TRUE;
	SWork.StatisticsLog.Kind = Kind; 

	if(Kind == LEVEL_CONCURRENT_USER) {		// ����
		for(int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Level_CurUser[i];
		}
	} else if(Kind == LEVEL_ALLIN_USER) {	// ��������
		for( int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Level_AllIn_User[i];
			m_StatisticsLog.Level_AllIn_User[i]= 0;	
		}
	} else if(Kind == GRADE_AVERAGE_MONEY) {// ��޺� �����ǵ�
		for(int i=0;i<5;i++) {
			if(m_StatisticsLog.Grade_GameCnt[i] > 0) {
				SWork.StatisticsLog.BigData[i]	= m_StatisticsLog.Grade_AverGameMoney[i]/m_StatisticsLog.Grade_GameCnt[i];
				SWork.StatisticsLog.SmallData[i]= m_StatisticsLog.Grade_GameCnt[i];
				m_StatisticsLog.Grade_AverGameMoney[i] = 0; 
				m_StatisticsLog.Grade_GameCnt[i] = 0;		
			}
		}
	} else if(Kind ==  LEVEL_USE_GAMECNT) { // ������ �������Ӽ�
		for( int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Level_UseGameCnt[i];
			m_StatisticsLog.Level_UseGameCnt[i] = 0;
		}
	} else if(Kind == LEVEL_USE_TIME) {		// ����̿�ð�
		for( int i=0; i<20; i++) {
			if(m_StatisticsLog.Level_UseTimeUserCnt[i]>0) {
				SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Level_UseTime[i]/m_StatisticsLog.Level_UseTimeUserCnt[i];
				m_StatisticsLog.Level_UseTime[i]= 0;			
				m_StatisticsLog.Level_UseTimeUserCnt[i] = 0;	
			}
		}
	} else if(Kind == GRADE_USURY_MONEY) {  // ���Ӻ� ������
		for( int i=0; i<5; i++) {
			SWork.StatisticsLog.BigData[i]   = m_StatisticsLog.Grade_UsuryMoney[i];
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Grade_UsuryCnt[i];
			m_StatisticsLog.Grade_UsuryMoney[i] = 0;
			m_StatisticsLog.Grade_UsuryCnt[i] = 0;
		}

	} else if(Kind == GRADE_JACKPOT) {		// ���� ���
		for( int i=0; i<5; i++) {
			SWork.StatisticsLog.SmallData[i] = m_StatisticsLog.Grade_JackPotCnt[i];
			SWork.StatisticsLog.BigData[i]	 = m_StatisticsLog.Grade_JackPotMoney[i];
			m_StatisticsLog.Grade_JackPotCnt[i]   = 0;
			m_StatisticsLog.Grade_JackPotMoney[i] = 0;
		}
	} else if(Kind == GRADE_MISSION) {		// �̼� ���
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

// [ ����� ]
void C62CutPokerServerView::Check_StatisticsLog()
{
	if( !Cfg.bStatisticsDB || !StatisCfg.bSaveSetLog ) return;
	
	CTime nt = CTime::GetCurrentTime();				// ������ ���� �ð�
	CTime st = m_StatisticsLog.PrevTime_1;			// ���� ��� �ð�
	CTimeSpan TimeSpan = nt - st;					// ����ð����� ���̸� ���Ѵ�
	int passedS = TimeSpan.GetTotalSeconds();

	
	if(passedS >= 60*10) {					// 10�� ���� ���
		Make_SetSADBWork(LEVEL_CONCURRENT_USER);	// ����
		Make_SetSADBWork(LEVEL_ALLIN_USER);			// ��������
		Make_SetSADBWork(LEVEL_USE_GAMECNT);		// ������ �������Ӽ�
		Make_SetSADBWork(GRADE_AVERAGE_MONEY);		// ��޺� �����ǵ�
		Make_SetSADBWork(GRADE_USURY_MONEY);		// ���Ӻ� ������
		
		// !!! ���ӿ� ���� ���ٸ� �ּ� ó�� �� ��
		Make_SetSADBWork(GRADE_JACKPOT);			// ���� ���
		// !!! ���ӿ� �̼� ���ٸ� �ּ� ó�� �� ��
		//Make_SetSADBWork(GRADE_MISSION);			// �̼� ���

		nt.GetAsSystemTime( m_StatisticsLog.PrevTime_1 );	// �ð� �ʱ�ȭ
	}
	
	// 24�� 10�п� �����Ѵ�.
	if(nt.GetHour() == 0 && nt.GetMinute() == 10 && nt.GetSecond() == 0 ) { 
		Make_SetSADBWork(LEVEL_USE_TIME);					// ����̿�ð�
		nt.GetAsSystemTime( m_StatisticsLog.PrevTime_2 );	// �ð� �ʱ�ȭ
	}
}

// [ ����� ]
void C62CutPokerServerView::CloseServer_SALog()		// ���� ����� ������ ����
{
	if( !Cfg.bStatisticsDB || !StatisCfg.bSaveSetLog ) return;

	Make_SetSADBWork(LEVEL_CONCURRENT_USER);	// ����
	Make_SetSADBWork(LEVEL_ALLIN_USER);			// ��������
	Make_SetSADBWork(GRADE_AVERAGE_MONEY);		// ��޺� �����ǵ�
	Make_SetSADBWork(LEVEL_USE_GAMECNT);		// ������ �������Ӽ�
	Make_SetSADBWork(LEVEL_USE_TIME);			// ����̿�ð�
	Make_SetSADBWork(GRADE_USURY_MONEY);		// ���Ӻ� ������
	
	// !!! ���ӿ� ���� ���ٸ� �ּ� ó�� �� ��
	Make_SetSADBWork(GRADE_JACKPOT);			// ���� ���
	
	// !!! ���ӿ� �̼� ���ٸ� �ּ� ó�� �� ��
	//Make_SetSADBWork(GRADE_MISSION);			// �̼� ���
	
	GetLocalTime( &m_StatisticsLog.PrevTime_2 );// �ð� �ʱ�ȭ
	GetLocalTime( &m_StatisticsLog.PrevTime_1 ); // �ð� �ʱ�ȭ
}
*/
//�Լ� ����
int C62CutPokerServerView::CheckTotalmoneylimit(USERINFO *pUI, char *uniqno, int bCheckWrite) // 0 üũ, 1 �Ӵ����� 2 �Ӵ����� ���
{
	if(pUI==NULL) return 0;
	int resMsg = 0;	
	int premLCode = pUI->PremLeadersCode;

	INT64 amount = 0;
	INT64 beforem = 0;
	INT64 afterm = 0;
	switch( premLCode )
	{
	case 0: // �Ϲ�ȸ��
		{
			if(pUI->PMoney > g_pMainView->StatisCfg.NormalMoney) {
				if( bCheckWrite > 0  ) { 
					
					amount = g_pMainView->StatisCfg.NormalMoney - pUI->PMoney; // ����
					beforem = pUI->PMoney; // ����ݾ�
					afterm = g_pMainView->StatisCfg.NormalMoney;
					
					pUI->PMoney = g_pMainView->StatisCfg.NormalMoney;
				}
				resMsg = 1;
			}
			break;
		}
	case 1: // ������ȸ��
		{
			if(pUI->PMoney > g_pMainView->StatisCfg.LeadersMoney) {
				if( bCheckWrite > 0  ) { 
					
					amount = g_pMainView->StatisCfg.LeadersMoney - pUI->PMoney; // ����
					beforem = pUI->PMoney; // ����ݾ�
					afterm = g_pMainView->StatisCfg.LeadersMoney;

					pUI->PMoney = g_pMainView->StatisCfg.LeadersMoney;
				}
				resMsg = 1;
			}
			break;
		}
	case 4: // �������÷��� ȸ��
		{
			if(pUI->PMoney > g_pMainView->StatisCfg.LeadersPlusMoney) {
				if( bCheckWrite > 0  ) { 
					
					amount = g_pMainView->StatisCfg.LeadersPlusMoney - pUI->PMoney; // ����
					beforem = pUI->PMoney; // ����ݾ�
					afterm = g_pMainView->StatisCfg.LeadersPlusMoney;

					pUI->PMoney = g_pMainView->StatisCfg.LeadersPlusMoney;
				}
				resMsg = 1;
			}
			break;
		}
	}
	
	if(bCheckWrite == 2  && uniqno!=NULL&&  resMsg == 1) { //ù �α�ø� �����
		// �α� ��Ͽ� �����.
		LOGREC_DBWORKS PWork;
		ZeroMemory(&PWork, sizeof(LOGREC_DBWORKS));
		PWork.WorkKind = 3; //2004.05.07 �������Ѿ� ����
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

		// DB ������ ����
		BOOL rtn = LogDBMan.SetDBWork(&PWork);
		if(rtn == FALSE) {
			CLogFile logfile("PreUserLMLogDB.txt");
			logfile.Writef("�Ѿ� ���� �αױ��  ����");
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
		str.Format("�����Ӵ� %s�� �̻��� ȹ���� �� �����ϴ�.", strM );
		str+="������ ���񽺿� �����Ͻø�, �� ���� ���ӸӴϸ� ������ �� �ְ� �˴ϴ�.";
	} else if(PremLeadersCode==1) {
		strM = MoneyToString(g_pMainView->StatisCfg.LeadersMoney);
		str.Format("�����Ӵ� %s�� �̻��� ȹ���� �� �����ϴ�.", strM );
		str+="�������÷����� ��ǰ ��ȯ�Ͻø�, �� ���� ���ӸӴϸ� ������ �� �ְ� �˴ϴ�.";
	} else if(PremLeadersCode==4) {
		strM = MoneyToString(g_pMainView->StatisCfg.LeadersPlusMoney);
		str.Format("�����Ӵ� %s�� �̻��� ȹ���� �� �����ϴ�.", strM);
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

	INT64 v = t / (INT64)10000000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	v = t / (INT64)1000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	v = t / (INT64)100000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)100000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}
	
	v = t / (INT64)10000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000;
		strtemp.Format("%I64d��", v);
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
// [��ȣõ��] 2004.07.08
INT64 C62CutPokerServerView::Get_angel_defmoney(const INT64 &lmoney) 
{
	if(lmoney<=StatisCfg.MinMoneyLimit)	  return StatisCfg.MinMoneyLimit;
	else if(lmoney>=StatisCfg.MaxMoneyLimit) return StatisCfg.MaxMoneyLimit;
	else return lmoney;
	return (INT64)0;
}