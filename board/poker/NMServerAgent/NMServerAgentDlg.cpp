// NMServerAgentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NMServerAgent.h"
#include "NMServerAgentDlg.h"

#include "ConncetDlg.h"
#include "LogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAINTAIN_CONNECT     100
//#define AUTO_CONNECT		1001 // 일정시간마다 접속시도
#define MAX_DATA_SAVE       5000   // 5000줄을 저장한다

CNMServerAgentDlg *g_pMainDlg = NULL;



///////////////////////////////////////////////////////////////////////
typedef struct{
	BOOL       bCon;    // 접속여부
	AGENT_USER au;      // 데이타
	int        Hour;	// 시
	int        Min;		// 분
	int        Sec;		// 초
	int        nSID;
} AGENT_USER_SAVE;

AGENT_USER_SAVE AUSD[MAX_DATA_SAVE];

int g_nSaveIndex = 0;

// 저장용으로 데이타를 저장한다
void CNMServerAgentDlg::InsertData(AGENT_USER &au,int nSID, BOOL bCon)
{
	CTime  time = CTime::GetCurrentTime();

	memcpy(&(AUSD[g_nSaveIndex].au),&au,sizeof(AGENT_USER));
	AUSD[g_nSaveIndex].bCon = bCon;
	AUSD[g_nSaveIndex].Hour = time.GetHour();
	AUSD[g_nSaveIndex].Min  = time.GetMinute();
	AUSD[g_nSaveIndex].Sec  = time.GetSecond();
	AUSD[g_nSaveIndex].nSID = nSID;

	g_nSaveIndex++;
	if(g_nSaveIndex>=MAX_DATA_SAVE){
		// 여기서 데이타를 저장한다
		SaveData();
		g_nSaveIndex = 0;
	}
}

// 히스토리 기능을 구현한다
void CNMServerAgentDlg::SaveData()
{
	const UINT lFileSize  = 128*MAX_DATA_SAVE; // 백업데이타 맥시멈 사이즈
	static char szLogData[lFileSize] = {0,};
	UINT  nTotalSize = 0;
	memset(szLogData,0,lFileSize);

	for(int i=0; i<min(MAX_DATA_SAVE-1,g_nSaveIndex); i++){
		CString strLogTemp(_T(""));
		if(AUSD[i].bCon) // 접속데이타이고 보유머니이다
			strLogTemp.Format("▲\"%s\" %I64d (%d:%d:%d) %s\r\n",
				AUSD[i].au.ID,AUSD[i].au.PMoney,AUSD[i].Hour,AUSD[i].Min,AUSD[i].Sec,ServerMan.GetName(AUSD[i].nSID));
		else             // 종료데이타이고 가감치이다
			strLogTemp.Format("▽\"%s\" %I64d (%d:%d:%d) %s\r\n",
				AUSD[i].au.ID,AUSD[i].au.PMoney,AUSD[i].Hour,AUSD[i].Min,AUSD[i].Sec,ServerMan.GetName(AUSD[i].nSID));

		int nTempSize = min(strLogTemp.GetLength(),128);
		memcpy(szLogData+nTotalSize,strLogTemp.operator LPCTSTR(),nTempSize);
		nTotalSize+=(UINT)nTempSize;
	}	

	CTime t = CTime::GetCurrentTime();

	CString strReportDir;

	strReportDir.Format("%s\\BACKUP",g_AppStartPath);
	if(_chdir(strReportDir.operator LPCTSTR())!=0){
		_mkdir(strReportDir.operator LPCTSTR());
	}
	SetCurrentDirectory(g_AppStartPath.operator LPCTSTR());

	strReportDir.Format("%s\\BACKUP\\%d월%d일",g_AppStartPath,t.GetMonth(),t.GetDay());
	if(_chdir(strReportDir.operator LPCTSTR())!=0){
		_mkdir(strReportDir.operator LPCTSTR());
	}
	CString strFileName;
	strFileName.Format("%s\\%d시%d분%d초.txt",strReportDir,t.GetHour(),t.GetMinute(),t.GetSecond());

	if(strFileName!=_T("")){
		CLogFile logfile(strFileName.operator LPCTSTR());
		logfile.WriteBackup(szLogData,min(nTotalSize,lFileSize));
	}

	SetCurrentDirectory(g_AppStartPath.operator LPCTSTR());
}
///////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
// CNMServerAgentDlg dialog

CNMServerAgentDlg::CNMServerAgentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNMServerAgentDlg::IDD, pParent)
{
	
	g_pMainDlg = this;

	m_pKRich   = NULL;

	//{{AFX_DATA_INIT(CNMServerAgentDlg)
	m_strUserID = _T("");
	m_bDCon = FALSE;
	m_AverJackPot = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNMServerAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNMServerAgentDlg)
	DDX_Control(pDX, IDC_COMBO_ULIST, m_comboUList);
	DDX_Control(pDX, IDC_LIST_USER, m_ctrUser);
	DDX_Control(pDX, IDC_LIST_SRV, m_ctrList);
	DDX_Text(pDX, IDC_EDIT_ID, m_strUserID);
	DDV_MaxChars(pDX, m_strUserID, 15);
	DDX_Check(pDX, IDC_CHECK_DCON, m_bDCon);
	DDX_Check(pDX, IDC_CHECK_AVERAGE, m_AverJackPot);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNMServerAgentDlg, CDialog)
	//{{AFX_MSG_MAP(CNMServerAgentDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_CBN_SELCHANGE(IDC_COMBO_ULIST, OnSelchangeComboUlist)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
	ON_WM_TIMER()
	ON_COMMAND(IDC_MENU_DISCONNECT, OnMenuDisconnect)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SRV, OnRclickListSrv)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SRV, OnItemchangedListSrv)
	ON_BN_CLICKED(IDC_CHECK_DCON, OnCheckDcon)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, OnButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_ALLDIS, OnButtonAlldis)
	ON_BN_CLICKED(IDC_CHECK_AVERAGE, OnCheckAverage)
	ON_BN_CLICKED(IDC_BUTTON_ADMIN, OnButtonAdmin)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_PACKET_NOTIFY, OnReceiveDataProc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNMServerAgentDlg message handlers

BOOL CNMServerAgentDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);		// Set small icon
	
	// TODO: Add extra initialization here



	// 통신 메세지 처리 클래스 초기화
	if(!m_SockCmnd.Init(this)) {
		AfxMessageBox("소켓 초기화에 실패하였습니다");
		return FALSE;
	}
	
	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_RICHEDITLIST); pWnd->GetWindowRect(&rc); ScreenToClient(&rc);
	m_pKRich = new CKHSRichEdit;
	if(m_pKRich==NULL) return FALSE;
	m_pKRich->Create(WS_VISIBLE | WS_CHILD |  WS_BORDER |
					WS_CLIPCHILDREN | WS_VSCROLL | ES_MULTILINE| ES_READONLY |
					ES_AUTOVSCROLL |ES_LEFT, rc, this, IDC_RICHEDITLIST);
//	m_pKRich->SetBackgroundColor(FALSE,RGB(255,255,222));
	m_pKRich->ShowWindow(SW_SHOW);

	CFile fileO;
	CFileException e;

	memset(&g_ServerInfoList,0,sizeof(SERVER_INFO_SAVE));

	if( !fileO.Open( "ServerList.cfg", CFile::modeRead, &e ) ) {
		AfxMessageBox("Can't find [ServerList.cfg] file!");
#ifdef _DEBUG
	  afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
	 
	}
	else{
		fileO.Read(&g_ServerInfoList,sizeof(SERVER_INFO_SAVE));
		fileO.Close();
	}
	

	m_ctrMenu.LoadMenu(IDR_POPMENU);

	// 리스트 초기화하기
	InitList();
	InitUList();

	m_nSelUser = 0;
	m_comboUList.ResetContent();
	m_comboUList.AddString("선택된 서버 없음");
	m_comboUList.SetCurSel(0);

	CString str; str.Format("서버관리자를 구동합니다");	ShowState(str,"알림",FALSE);

	// ### [ Queue ] ###
	m_head = (QDATA*)new QDATA;
	m_tail = (QDATA*)new QDATA;

	m_head->next = m_tail;
	m_tail->next = m_tail;
	
	m_TotalQueCnt = 0;;
	

	//### [관리자 모드 작업] ###
	memset(&g_AdminInfo,0,sizeof(g_AdminInfo));
	char *tmp = new char[sizeof(g_AdminInfo.AdminMan)];
	char *tmp1 = new char[sizeof(g_AdminInfo.AdminMan)];
	
	ZeroMemory(tmp, sizeof(g_AdminInfo.AdminMan));
	ZeroMemory(tmp1, sizeof(g_AdminInfo.AdminMan));


	CFile f;
	CFileException ef;

	if( !f.Open( "AdminInfo.cfg", CFile::modeRead, &ef ) )  {
		AfxMessageBox("Can't find [AdminInfo.cfg] file!\nSet Admin config!");
#ifdef _DEBUG
   afxDump << "File could not be opened " << e.m_cause << "\n";
#endif

	}
	else{
		f.Read(tmp,sizeof(g_AdminInfo.AdminMan));
		f.Close();
		for(int i=0; i<sizeof(g_AdminInfo.AdminMan);i++) {
			tmp1[i]^= (tmp[i]^'C'^'Y');
		}
		memcpy(&g_AdminInfo.AdminMan, tmp1, sizeof(g_AdminInfo.AdminMan));
	}

	delete tmp;
	delete tmp1;

	if(g_ServerInfoList.nTotalList >= MAX_CONNECT_SERVER) g_ServerInfoList.nTotalList = MAX_CONNECT_SERVER-1;

	m_bisContinue = FALSE; // 자동접속
	SetTimer(MAINTAIN_CONNECT,1000,NULL); // 접속유지 타이머를 켠다
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNMServerAgentDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	KillTimer(MAINTAIN_CONNECT);
//	KillTimer(AUTO_CONNECT);

	if(m_pKRich)  delete m_pKRich;  m_pKRich  = NULL;

	// ### [ Queue ] ###
	QueDeleteAll();

	// 소켓 메니저 종료
	SockMan.Destroy();	

}

void CNMServerAgentDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNMServerAgentDlg::OnPaint() 
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
HCURSOR CNMServerAgentDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNMServerAgentDlg::InitList()
{
	// 전체행 셀렉트 설정
	m_ctrList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//// Column 삽입부
	LV_COLUMN col;
	char szName[256];

	wsprintf(szName, "Name");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 100;
	col.pszText = szName;
	col.iSubItem = 0;
	m_ctrList.InsertColumn(0, &col);

	col.fmt  = LVCFMT_LEFT;
	col.cx   = 100;
	wsprintf(szName, "IP");
	col.iSubItem = 1;
	m_ctrList.InsertColumn(1, &col);

	col.fmt  = LVCFMT_CENTER;
	col.cx   = 50;
	wsprintf(szName, "Port");
	col.iSubItem = 2;
	m_ctrList.InsertColumn(2, &col);

	col.fmt  = LVCFMT_RIGHT;
	col.cx   = 60;
	wsprintf(szName, "User");
	col.iSubItem = 3;
	m_ctrList.InsertColumn(3, &col);

	/////////////////////////////////////
	// 잭팟
	col.fmt  = LVCFMT_RIGHT;
	col.cx   = 170;
	wsprintf(szName, "JackPot");
	col.iSubItem = 4;
	m_ctrList.InsertColumn(4, &col);
	/////////////////////////////////////


}

void CNMServerAgentDlg::InsItem(char *Name, char *IP, UINT nPort, int nNum, INT64 jackpot)
{
	// 레코드를 리스트 뷰에 삽입
	LV_ITEM item;
	char szName[256]={0,};

	// 같은 서버가 존재하는지 먼저 검사하고 이미 존재하면 취소
	int num=-1;
	CString szRN;
	for(;;)
	{
		num = m_ctrList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szRN = m_ctrList.GetItemText(num, 0);
		if(strcmp(Name, szRN.operator LPCTSTR())==0) return;
	}

	// 서버이름
	wsprintf(szName, "%s", Name);
	item.mask = LVIF_TEXT;
	item.pszText = szName;
	item.iItem = m_ctrList.GetItemCount();
	item.iSubItem = 0;
	m_ctrList.InsertItem(&item);

	// 서버아이피
	wsprintf(szName, "%s", IP);
	item.iSubItem = 1;
	m_ctrList.SetItem(&item);

	// 포트
	wsprintf(szName, "%d", nPort);
	item.iSubItem = 2;
	m_ctrList.SetItem(&item);

	// 인원
	wsprintf(szName, "%d", nNum);
	item.iSubItem = 3;
	m_ctrList.SetItem(&item);

	//////////////////////////////////
	// 잭팟
	CString strPM;
	strPM = g_MakeCommaMoney(jackpot);
	strPM += "원";
	wsprintf(szName, "%s", strPM.operator LPCTSTR());
	item.iSubItem = 4;
	m_ctrList.SetItem(&item);

	
}

void CNMServerAgentDlg::SetItem(char *Name, int nNum, INT64 jackpot)
{
	char szName[256]={0,};
	wsprintf(szName, "%s", Name);

	int num=-1;
	CString szSN(_T(""));

	for(;;)
	{
		num = m_ctrList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szSN = m_ctrList.GetItemText(num, 0);
		if(strcmp(szName, szSN.operator LPCTSTR())==0)
		{
			// 인원
			wsprintf(szName, _T("%d"), nNum);
			m_ctrList.SetItemText(num, 3, szName);

			// 잭팟
			CString strPM;
			strPM = g_MakeCommaMoney(jackpot);
			strPM += "원";

			wsprintf(szName, "%s", strPM.operator LPCTSTR());
			m_ctrList.SetItemText(num, 4, szName);

			break;
		}
	}
}

void CNMServerAgentDlg::SetItemJackPot(char *Name, INT64 jackpot)
{
	char szName[256]={0,};
	wsprintf(szName, "%s", Name);

	int num=-1;
	CString szSN(_T(""));

	for(;;)
	{
		num = m_ctrList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szSN = m_ctrList.GetItemText(num, 0);
		if(strcmp(szName, szSN.operator LPCTSTR())==0)
		{
			// 잭팟
			CString strPM;
			strPM = g_MakeCommaMoney(jackpot);
			strPM += "원";
					 
			wsprintf(szName, "%s", strPM.operator LPCTSTR());
			m_ctrList.SetItemText(num, 4, szName);

			break;
		}
	}
}

void CNMServerAgentDlg::DelItem(char *Name)
{
	char szName[256]={0,};
	wsprintf(szName, "%s", Name);

	int num=-1;
	CString szSN;

	for(;;)
	{
		num = m_ctrList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szSN = m_ctrList.GetItemText(num, 0);
		if(strcmp(szName, szSN.operator LPCTSTR())==0)
		{
			m_ctrList.DeleteItem(num); break;
		}
	}
}


// 서버에서 데이타를 받는 경우.
long CNMServerAgentDlg::OnReceiveDataProc(UINT wParam, LONG lParam)
{
	/*
	NETDATA_STRUCT *pNetData = (NETDATA_STRUCT*)wParam;
	if(!pNetData) return 0;

	BOOL  bProcess  = FALSE;
	int   nSID      = pNetData->nSID;
	int   Signal    = pNetData->Signal;
	int   TotSize   = pNetData->TotSize;
	char *lpData    = pNetData->lpData;

	// 올바른 데이타인지 한번 더 확인한다
	CNetData nData;
	if(nData.IsData((char *)lpData,TotSize) && nData.GetType()==NM_QMDATA) bProcess  = TRUE;
	if(!bProcess) return 0;
	int  nUserVal    = nData.GetUserVal(); // 사용자 값 확인
	SIGNAL signNum   = nData.GetSign();
	if((int)signNum != Signal) return 0;

	CSrvInfo *pInfo  = ServerMan.GetSrvInfo(nSID);
	switch(signNum)
*/
	GAMENOTIFY *pNotifyData = (GAMENOTIFY*)wParam;
	if(!pNotifyData) return 0;

	int Sid = pNotifyData->Sid;				// 소켓 번호
	int Signal = pNotifyData->Signal;		// 메시지 시그널
	int TotSize = pNotifyData->TotSize;		// 메시지 사이즈
	char *lpData = pNotifyData->lpData;		// 메시지 데이터 포인터
	
	int nSID = Sid;

	switch(Signal)
	{
	case SV_CHECKVERSION: // 버전 체크
		{
			CSV_CHECKVERSION msg;
			msg.Get(lpData, TotSize);
//			if(g_MainSrvSID != Sid) break;

			int ver = *msg.Ver;
			if(ver<0) break;

			// 버전이 틀리면 연결종료
//			if(ver != VERSION)
//			{
//				PostMessage(UM_DATA_CLOSE, NULL, (long)SockMan.pDataSock[g_MainSrvSID]); 
//				AfxMessageBox("프로그램의 버전이 일치하지 않습니다. 최신 버전을 다운받으세요");
//				m_ConnectMsgDlg.OnCancel();
//				break;
//			}
/*
			// 로그인 중이면
//			if(g_Where == WH_LOGIN)
			{
				CString strID,strPass;
				strID.Format("%s",CONNECT_ID);
				strPass.Format("%s",CONNECT_PASS);

				char *id = (char*)strID.operator LPCTSTR();
				char *pass = (char*)strPass.operator LPCTSTR();

				

				char *socknameip = (char*)SockMan.pDataSock[0]->SockAddr.operator LPCTSTR();	// 연결된 소켓의 어드레스
				char *hostnameip = (char*)SockMan.GetServerIP();	// 호스트 시스템 어드레스
				UINT port = SockMan.pDataSock[0]->SockPort;							// 연결된 소켓의 포트번호
				
				// 서버용 소켓을 생성하기 위해 20000정도 더해준 포트번호를 사용한다
				// 실제 사용하게될 포트번호는 메인서버에 의하여 결정된다
				port += 20000; 

				// 로그인 메세지를 보낸다
				CSV_LOGIN lgimsg;
				lgimsg.Set(id, pass, FALSE, socknameip, hostnameip, port);
				SockMan.SendData(Sid, lgimsg.pData, lgimsg.GetTotalSize());
			}
*/
				ASK_LOGININFO al;
				memset(&al, 0, sizeof(ASK_LOGININFO));

			
				strcpy(al.ID, CONNECT_ID);
				strcpy(al.Pass, CONNECT_PASS);
			

				strncpy(al.SockNameIP, SockMan.pDataSock[0]->SockAddr, 20);	// 연결된 소켓의 어드레스
				strncpy(al.HostNameIP, SockMan.GetServerIP(), 20);						// 호스트 시스템 어드레스
				al.Port = SockMan.pDataSock[0]->SockPort;				// 연결된 소켓의 포트번호
				// 서버용 소켓을 생성하기 위해 20000정도 더해준 포트번호를 사용한다
				// 실제 사용하게될 포트번호는 메인서버에 의하여 결정된다
				al.Port += 20000; 

				//al.AuthCookieLen = g_AuthCookie.GetLength();
				//al.DataCookieLen = g_DataCookie.GetLength();
				CString g_AuthCookie="";	// 인증 쿠키						[SSO 작업]
				CString g_DataCookie="";	// 데이터 쿠키	

				// 로그인 메세지를 보낸다
				CSV_ASK_LOGIN lgimsg;
				lgimsg.Set(&al, (char*)(LPCTSTR)g_AuthCookie, (char*)(LPCTSTR)g_DataCookie);
				SockMan.SendData(Sid, lgimsg.pData, lgimsg.GetTotalSize());


		} break;
	
	// [서버 에이전트 작업] : 에이전트가 접속되었다
	case NM_AGENT_CONNECT:
			{
				CNM_AGENT_CONNECT msg;
				msg.Get(lpData, TotSize);

				int nTotalList = *msg.TotalNum;
				
				INT64 jackpot = *msg.JackPot;
				if(jackpot < 0 ) jackpot = 0;


				ShowState("서버에 접속되었습니다",ServerMan.GetName(nSID), FALSE);

				InsItem((char *)ServerMan.GetName(nSID).operator LPCTSTR(),
					(char *)ServerMan.GetIP(nSID).operator LPCTSTR(),
					ServerMan.GetPort(nSID), nTotalList, jackpot); // 추가

				m_comboUList.AddString(ServerMan.GetName(nSID).operator LPCTSTR());

				ServerMan.ClearUserData(nSID);
				ServerMan.SetTotal(nSID,nTotalList, jackpot);
				ServerMan.Connect(nSID);

				GetJackPotAverage();// #### 잭팟 평균 ####

				////////////////////////////////////////////////////////////////////////
				//### [관리자 모드 작업] ###
			
				
				for(int i=0; i<MAX_CONNECT_SERVER; i++) {//관리자 리스트 보내준다
					if(ServerMan.m_Data[i].m_bValid && ServerMan.m_Data[i].m_bConnect){

						CNM_AGENT_ADMININFO agent;
						agent.Set(g_AdminInfo.AdminMan.nTotal, g_AdminInfo.AdminMan.admin);
						
						int nSID = ServerMan.m_Data[i].m_nSID;
						SockMan.SendData(nSID, agent.pData, agent.GetTotalSize());
					}
				}
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////


			} break;

	// 첫 접속후 400명 정보 받음 1차
	case NM_AGENT_USERLIST_R1:
		{
			CNM_AGENT_USERLIST_R1 msg;
			msg.Get(lpData, TotSize);

			int nTotalList = *msg.TotNum;
			
			if( nTotalList <= 0) break;

			int CurCnt = *msg.Count;

			if(CurCnt < 0 ) break;
			int NextCnt = 0;

			for(int i=0; i < nTotalList; i++)
			{
				AGENT_USER au;
				memcpy(&au, &msg.ArraySUI[i], sizeof(AGENT_USER));
				ServerMan.SetUser(nSID,au);
				InsUItem(au,(char *)ServerMan.GetName(nSID).operator LPCTSTR());
			}

			CString str;
			str.Format("사용자를 수신중입니다 : %d명",nTotalList);
			ShowState(str,ServerMan.GetName(nSID),FALSE);


			if(CurCnt >= 0)
			{	// 요청
				if(CurCnt == 0)  NextCnt = 1; //  400 -  799명
				if(CurCnt == 1)  NextCnt = 2; //  800 - 1199명
				if(CurCnt == 2)  NextCnt = 3; // 1200 - 1599명
				if(CurCnt == 3)  NextCnt = 4; // 1600 - 1999명
				if(CurCnt == 4)  NextCnt = 5; // 2000 - 2399명
				if(CurCnt == 5)  NextCnt = 6; // 2400 - 2799
				if(CurCnt == 6)  NextCnt = 7; // 2800 - 

				// 유저 목록 요청 2 ~ ?
				CNM_AGENT_USERLIST umsg;
				umsg.Set(NextCnt);
				SockMan.SendData(Sid, umsg.pData, umsg.GetTotalSize());
			}

		} break;

		case NM_AGENT_NEWUSER:
			{
				CNM_AGENT_NEWUSER msg;
				msg.Get(lpData, TotSize);

				int nTotalList = *msg.TotNum;//nUserVal;
				INT64 jackpot = *msg.JackPot;

				if(jackpot < 0 ) jackpot = 0;



				if(nTotalList < 0 || nTotalList > MAX_USER) break;

				AGENT_USER au; memset(&au,0,sizeof(AGENT_USER));
				memcpy(&au, msg.ArraySUI, sizeof(AGENT_USER));

				if(ServerMan.bFind(au.ID)==TRUE){
					CString strTest; strTest.Format("%s:동시 접속 시도!!!!!",au.ID);
					ShowState(strTest,ServerMan.GetName(nSID), FALSE);
					
					if(!m_bDCon){ // 중복 접속 허용인가?

						///////////////////////////////////////////////////////////////////////////
						// 추가 
						// 기존 접속해 있는 서버에 현재위치가 채널 선택창이라면 접속 종료
						CString strID = au.ID;
						AGENT_USER aus[MAX_CONNECT_SERVER];
						memset(aus,0,sizeof(AGENT_USER)*MAX_CONNECT_SERVER);
						
						int aSID[MAX_CONNECT_SERVER] = {0,};
						int nFindNum = ServerMan.GetUser(strID,(AGENT_USER *)aus,(int *)aSID);

						CString str,strName;
						
						if(nFindNum>0)
						{
							for(int i=0; i<nFindNum; i++)
							{// 각서버로 종료 요청
								CNM_AGENT_NEWUSERERROR aerror;
								aerror.Set(5, &aus[i]); // 요청
								SockMan.SendData(aSID[i], aerror.pData, aerror.GetTotalSize());
								
								//str.Format("\"%s\" 종료예약?",aus[i].ID);
								//ShowState(str,ServerMan.GetName(aSID[i]), TRUE);
							}
						}
						///////////////////////////////////////////////////////////////////////////
						//
						CNM_AGENT_NEWUSERERROR aerror1;
						aerror1.Set(0, &au);
						SockMan.SendData(Sid, aerror1.pData, aerror1.GetTotalSize());
						break;
					}
				}
				//리스트에 있나 없나 한번더 검사
				else
				{
					// ### [ Queue ] ###
					int nRcount = 0;
					if( FindQueId(au.ID, nRcount) )
					{
						CString strTest; strTest.Format("Queue....... .%s:동시 접속 시도  대기시간 %d !!!!!",au.ID, nRcount);
						ShowState(strTest,ServerMan.GetName(nSID), FALSE);
						
						if(!m_bDCon){ // 중복 접속 허용인가?
							
							///////////////////////////////////////////////////////////////////////////
							// 추가 
							// 기존 접속해 있는 서버에 현재위치가 채널 선택창이라면 접속 종료
							CString strID = au.ID;
							AGENT_USER aus[MAX_CONNECT_SERVER];
							memset(aus,0,sizeof(AGENT_USER)*MAX_CONNECT_SERVER);
							
							int aSID[MAX_CONNECT_SERVER] = {0,};
							int nFindNum = ServerMan.GetUser(strID,(AGENT_USER *)aus,(int *)aSID);
							
							CString str,strName;
							CNM_AGENT_NEWUSERERROR aerror;
							if(nFindNum>0)
							{
								for(int i=0; i<nFindNum; i++)
								{// 각서버로 종료 요청
									CNM_AGENT_NEWUSERERROR aerror;
									aerror.Set(5, &aus[i]); // 요청
									SockMan.SendData(aSID[i], aerror.pData, aerror.GetTotalSize());
									
									str.Format("\"%s\" 종료예약?",aus[i].ID);
									ShowState(str,ServerMan.GetName(aSID[i]), FALSE);
								}
							}
							///////////////////////////////////////////////////////////////////////////
							//
							CNM_AGENT_NEWUSERERROR aerror1;
							aerror1.Set(6, &au);
							SockMan.SendData(Sid, aerror1.pData, aerror1.GetTotalSize());
							break;
						}
					}
				}





				// 성공적인 접속이면 세팅한다
				ServerMan.SetUser(nSID,au);
				InsertData(au,nSID,TRUE); // 히스토리 저장용이다
				ServerMan.SetTotal(nSID, nTotalList, jackpot);
				SetItem((char *)ServerMan.GetName(nSID).operator LPCTSTR(),nTotalList, jackpot);
				InsUItem(au,(char *)ServerMan.GetName(nSID).operator LPCTSTR());

				//### [관리자 모드 작업] ###
				if(IsAdminUser(au.ID) )
				{
					g_AdminInfo.NowNum++;
				}
				// #### 잭팟 평균 ####
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////
				GetJackPotAverage();
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////



			} break;
			
		case SV_SERVERMSG: // 서버 알림 메세지가 도착했음
			{
				CSV_SERVERMSG msg;
				msg.Get(lpData, TotSize);
				
				CString str;
				CString chat;
				chat = (LPCTSTR)msg.StrChat;
				chat = chat.Mid(0, *msg.l_StrChat);
				str.Format("%s", chat.operator LPCTSTR());
				
				if(*msg.Kind == 0)
				{
					ShowState(str, "[서버 알림] (비 정상종료 아이디) 처리완료", FALSE);
				}
				else if(*msg.Kind == 1)
				{
					// 로그 파일 기록
					ShowState(str, "[서버 알림]  동시 접속시도", TRUE);
				}
			} break;
			
			
		case NM_AGENT_OUTUSER:
			{
				CNM_AGENT_OUTUSER msg;
				msg.Get(lpData, TotSize);

				int nTotalList = *msg.TotNum;
				INT64 jackpot = *msg.JackPot;
				
				if(jackpot < 0 ) jackpot = 0;

				if(nTotalList < 0 || nTotalList > MAX_USER) break;


				AGENT_USER au; memset(&au,0,sizeof(AGENT_USER));
				memcpy(&au, msg.ArraySUI, sizeof(AGENT_USER));

				// 성공적인 접속해제이면 세팅한다
				ServerMan.SetTotal(nSID,nTotalList, jackpot);
				ServerMan.DelUser(nSID,au);

				// ### [ Queue ] ###
				AddQueue(au.ID);

				InsertData(au,nSID,FALSE); // 히스토리 저장용이다

				SetItem((char *)ServerMan.GetName(nSID).operator LPCTSTR(),nTotalList, jackpot);

				DelUItem(au,(char *)ServerMan.GetName(nSID).operator LPCTSTR());

				//### [관리자 모드 작업] ###
				if(IsAdminUser(au.ID) )
				{
					if(g_AdminInfo.NowNum>0) g_AdminInfo.NowNum--;
				}

				
				// #### 잭팟 평균 ####
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////
				GetJackPotAverage();
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////

			} break;

		// [서버 에이전트 작업] : 중복접속 확인을 요청해왔다
		case NM_AGENT_DUBLECON:
			{
				CNM_AGENT_DUBLECON msg;
				msg.Get(lpData, TotSize);

				AGENT_USER au; memset(&au,0,sizeof(AGENT_USER));
				//memcpy(&au,nData.m_lpData,sizeof(au));
				memcpy(&au, msg.ArraySUI, sizeof(AGENT_USER));


				// 이미 접속해 있는 사용자인지 확인하자 (해당서버를 제외하고 확인한다)
				if(ServerMan.bFindExcept(au.ID,nSID)==TRUE){
					CString strTest; strTest.Format("%s:중복접속을 확인!(처리완료)",au.ID);
					ShowState(strTest,ServerMan.GetName(nSID), FALSE);
					if(!m_bDCon){

						CNM_AGENT_NEWUSERERROR aerror;
						aerror.Set(0, &au);
						SockMan.SendData(Sid, aerror.pData, aerror.GetTotalSize());
						break;
					}
				}

			} break;


	case SV_REFUSE_LOGIN: // 로그인이 거부 되었음
		{

			CSV_REFUSE_LOGIN msg;
			msg.Get(lpData, TotSize);
			CString str ="!!!";
			int code = *msg.Code;
			if(code==0) ShowState("사용자가 아닙니다. 유저ID를 확인하세요", str ,FALSE);
			if(code==1) ShowState("비밀번호가 틀렸습니다. 비밀번호를 다시 확인하세요", str,FALSE);
			if(code==2) ShowState("이미 사용중인 아이디입니다. 관리자에게 문의하세요", str,FALSE);
			if(code==3) ShowState("최대 수용인원을 초과하였습니다. 잠시후 다시 접속해주세요", str,FALSE);
			if(code==4) ShowState("인증되지 않은 사용자입니다", str ,FALSE);
			if(code==6) ShowState("본 아이디는 현재 사용 정지 상태이므로 서버에 접속할 수 없습니다.\n관리자에게 문의하세요", str ,FALSE);
			if(code==7) ShowState("본 아이디는 삭제된 아이디입니다. 관리자에게 문의하세요", str ,FALSE);
			if(code==5) ShowState("이전 게임에서 아직 접속 종료 처리가 이루어지지 않았습니다.", str ,FALSE);
		
		} break;

	// [ ###잭팟### ] 
	case SV_SPECIALCARD :
		{
			CSV_SPECIALCARD msg;
			msg.Get(lpData, TotSize);

			if(msg.pSC->NowJackPot > 0)
			{			
				CString spec("");
				CString str="";
				if(msg.pSC->nCard==1) spec = "포카드";
				else if(msg.pSC->nCard==2) spec = "스트레이트 플러쉬";
				else if(msg.pSC->nCard==3) spec = "로얄 스트레이트 플러쉬";

				CString strM = g_MakeCommaMoney(msg.pSC->JackPotPlus);
				strM +="원";
				str.Format("[알림] [%s] [%s,%d번방]의 [%s]회원이 %s 를 하셨습니다. ★ 잭팟 상금 %s",
				(char *)ServerMan.GetName(nSID).operator LPCTSTR(), msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, spec, strM);

				ShowState(str,"알림",FALSE);


				ServerMan.SetJackPot(nSID, msg.pSC->NowJackPot);
				// #### 잭팟 평균 ####
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////
				GetJackPotAverage();
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////

				// 각 서버에 알려준다.
				if( m_AverJackPot )
				{
					
					for(int i=0; i<MAX_CONNECT_SERVER; i++){
						if(ServerMan.m_Data[i].m_bValid && ServerMan.m_Data[i].m_bConnect){
							CNM_AGENT_AVERAGE gAver;
							gAver.Set(g_AverJackPot);
							
							int nSID = ServerMan.m_Data[i].m_nSID;
							SockMan.SendData(nSID, gAver.pData, gAver.GetTotalSize());
						}
					}
				}

			}

		} break;


		
	}


			///////////////////////////////////////////////
	return -1;
}

void CNMServerAgentDlg::OnOK() 
{
}

void CNMServerAgentDlg::OnCancel() 
{
	int rtn=AfxMessageBox("서버관리자을 종료하시겠습니까?", MB_YESNO);
	if(rtn!=IDYES) return;

	CString str; str.Format("서버관리자를 종료합니다");	ShowState(str,"알림",FALSE);
	
	CDialog::OnCancel();
}


// 서버에 접속한다
void CNMServerAgentDlg::OnButtonConnect() 
{
	CConncetDlg dlg;
	if(IDOK==dlg.DoModal()){
		// 이미 접속중인지 체크하기
		if(ServerMan.IsConnect(dlg.m_strServerName)){
			AfxMessageBox("이미 같은 이름의 서버 정보가 존재합니다!");
			return;
		}
		if(ServerMan.IsConnect(dlg.m_strIP, dlg.m_nPort)){
			AfxMessageBox("이미 같은 아이피와 포트번호의 서버 정보가 존재합니다!");
			return;
		}
	
		int nSID = SockMan.ConnectSocket((char *)(dlg.m_strIP.operator LPCTSTR()), dlg.m_nPort);

		ServerMan.SetData(dlg.m_strServerName,dlg.m_strIP,dlg.m_nPort,nSID);
		// 메인서버와 접속 시도
		//g_MainSrvSID = SockMan.ConnectSocket((char*)g_ServIP.operator LPCTSTR(), g_ServPort);

		ShowState("서버에 접속 중입니다...",dlg.m_strServerName, FALSE);
	}	
}

// 상태를 보인다
void CNMServerAgentDlg::ShowState(CString str, CString strName, BOOL bLog)
{
	if(str=="") return;

	CTime  time = CTime::GetCurrentTime();

	CString strState; COLORREF crCol = RGB(0,0,0);
	strState.Format(_T(" [%d/%d/%d:%d] %s : %s\r\n"),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),strName,str);

	m_pKRich->AddText(strState,crCol);

	if(bLog){
		// 날짜별로 로그를 기록한다
		CString strReportDir;
		CString strFileName;
		strReportDir.Format("%s\\LOGDATA",g_AppStartPath);
		strFileName.Format("%s\\Log_%d월%d일.txt",strReportDir,time.GetMonth(),time.GetDay());

		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Write(strState);
		}
	}

//	UpdateData(FALSE);
}

// 연결에 실패한다
void CNMServerAgentDlg::ConnectFail(int nSID)
{
	CSrvInfo *pInfo = ServerMan.GetSrvInfo(nSID);
	if(pInfo){
		ShowState("서버에 접속을 실패하였습니다!", pInfo->m_strName, FALSE);
	}

	ServerMan.Clear(nSID);
	SockMan.CloseDataSocket(nSID);
}

// 접속이 끊어진다
void CNMServerAgentDlg::Disconnect(int nSID)
{
	CSrvInfo *pInfo = ServerMan.GetSrvInfo(nSID);
	if(pInfo){
		ShowState("서버와 접속이 끊어졌습니다!", pInfo->m_strName, FALSE);
	}

	DelItem((char *)pInfo->m_strName.operator LPCTSTR());

	if(m_comboUList.GetCount()!=CB_ERR){
		CString strOrg=_T("");
		m_comboUList.GetLBText(m_comboUList.GetCurSel(),strOrg);
		BOOL bLoop = TRUE;
		while(bLoop){
			bLoop = FALSE;
			if(m_comboUList.GetCount()==CB_ERR) break;
			for(int i=0;i<m_comboUList.GetCount();i++){
				CString strTemp=_T("");
				m_comboUList.GetLBText(i,strTemp);
				if(pInfo->m_strName == strTemp){
					m_comboUList.DeleteString(i);
					if(pInfo->m_strName==strOrg){
						m_comboUList.SetCurSel(0);
						m_nSelUser = 0;
						// 해당 작업중의 리스트를 처리한다!!!
						m_ctrUser.DeleteAllItems();
					}
					bLoop = TRUE;
				}
			}
		}
	}

	ServerMan.Clear(nSID);
	SockMan.CloseDataSocket(nSID);


	// #### 잭팟 평균 ####
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	GetJackPotAverage();
	
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
}

void CNMServerAgentDlg::InitUList()
{
	// 전체행 셀렉트 설정
	m_ctrUser.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//// Column 삽입부
	LV_COLUMN col;
	char szName[256];

	wsprintf(szName, "ID");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 110;
	col.pszText = szName;
	col.iSubItem = 0;
	m_ctrUser.InsertColumn(0, &col);

	col.fmt  = LVCFMT_CENTER;
	col.cx   = 50;
	wsprintf(szName, "UNum");
	col.iSubItem = 1;
	m_ctrUser.InsertColumn(1, &col);

}

void CNMServerAgentDlg::InsUItem(AGENT_USER &au, char *Name)
{
	// 레코드를 리스트 뷰에 삽입
	LV_ITEM item;
	char szName[256]={0,};
	wsprintf(szName, "%s", Name);

	CString strSelList =_T("");
	m_comboUList.GetLBText(m_comboUList.GetCurSel(),strSelList);

	if(strcmp(szName,strSelList.operator LPCTSTR())!=0) return;

	// 아이디
	wsprintf(szName, "%s", au.ID);
	item.mask = LVIF_TEXT;
	item.pszText = szName;
	item.iItem = m_ctrUser.GetItemCount();
	item.iSubItem = 0;
	m_ctrUser.InsertItem(&item);

	// 유저넘
	wsprintf(szName, "%d", au.nUNum);
	item.iSubItem = 1;
	m_ctrUser.SetItem(&item);

}

void CNMServerAgentDlg::DelUItem(AGENT_USER &au, char *Name)
{
	char szName[256]={0,};
	wsprintf(szName, "%s", Name);

	CString strSelList =_T("");
	m_comboUList.GetLBText(m_comboUList.GetCurSel(),strSelList);
	if(strcmp(szName,strSelList.operator LPCTSTR())!=0) return;

	int num=-1;
	CString szID;

	for(;;)
	{
		num = m_ctrUser.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_ctrUser.GetItemText(num, 0);
		CString szUNum  = m_ctrUser.GetItemText(num,1);
		CString strName = m_ctrUser.GetItemText(num,2);

		int nUNum = ::atoi(szUNum.operator LPCTSTR());
		if(strcmp(au.ID, szID.operator LPCTSTR())==0 && au.nUNum==nUNum)
		{
			m_ctrUser.DeleteItem(num); break;
		}
	}
}

// 사용자 리스트 보기
void CNMServerAgentDlg::OnSelchangeComboUlist() 
{
	int nSelNum = m_comboUList.GetCurSel();
	if(nSelNum<0) return;
	if(nSelNum==m_nSelUser) return;
	m_nSelUser = nSelNum;

	CString str;
	m_comboUList.GetLBText(m_nSelUser,str);

	m_ctrUser.DeleteAllItems();

	CSrvInfo *pInfo = ServerMan.GetSrvInfo(str);
	if(pInfo==NULL) return;
	
	for(int i=0; i<MAX_USER; i++){
		if(strlen(pInfo->m_AU[i].ID)<2) continue;
		InsUItem(pInfo->m_AU[i],(char *)(str.operator LPCTSTR()) );
	}
}

// 사용자 찾기
void CNMServerAgentDlg::OnButtonFind() 
{
	CString strID("");
	GetDlgItemText(IDC_EDIT_ID,strID);
	
	if(strID==""){
		AfxMessageBox("찾을 아이디를 입력하여 주시길 바랍니다");
		return;
	}

	AGENT_USER au[MAX_CONNECT_SERVER];
	memset(au,0,sizeof(AGENT_USER)*MAX_CONNECT_SERVER);
	
	int aSID[MAX_CONNECT_SERVER] = {0,};

	int nFindNum = ServerMan.GetUser(strID,(AGENT_USER *)au,(int *)aSID);

	CString str,strName;


	//////////////////////////////////////////////
	// ### [ Queue ] ###
	int nRcount = 0;
	if(FindQueId( (char*)strID.operator LPCTSTR(), nRcount) ) 
	{
		str.Format("\"%s\"회원은 접속종료 처리중입니다.",strID);
		ShowState(str,"종료처리중",FALSE);
	}
	//////////////////////////////////////////////


	
	if(nFindNum<=0){
		str.Format("\"%s\"회원은 접속해 있지 않습니다",strID);
		ShowState(str,"찾기실패",FALSE);
		return;
	}

	for(int i=0; i<nFindNum; i++){
		str.Format("\"%s\"회원을 찾았습니다",au[i].ID);
		ShowState(str,ServerMan.GetName(aSID[i]),FALSE);
	}
}

// 사용자 접속 종료시키기
void CNMServerAgentDlg::OnButtonDisconnect() 
{
	int rtn=AfxMessageBox("해당 아이디를 전서버에서 종료시킵니다. 계속 진행할까요?", MB_YESNO);
	if(rtn!=IDYES) return;

	CString strID("");
	GetDlgItemText(IDC_EDIT_ID,strID);
	
	if(strID==""){
		AfxMessageBox("접속종료시킬 아이디를 입력하여 주시길 바랍니다");
		return;
	}

	if(strID.Compare(CONNECT_ID)==0){
		AfxMessageBox("관리자 아이디는 종료시킬수 없습니다");
		return;
	}

	AGENT_USER au[MAX_CONNECT_SERVER];
	memset(au,0,sizeof(AGENT_USER)*MAX_CONNECT_SERVER);
	
	int aSID[MAX_CONNECT_SERVER] = {0,};

	int nFindNum = ServerMan.GetUser(strID,(AGENT_USER *)au,(int *)aSID);

	CString str,strName;
	if(nFindNum<=0){
		str.Format("\"%s\"회원은 접속해 있지 않습니다",strID);
		ShowState(str,"종료실패",FALSE);
		return;
	}

	
	
	for(int i=0; i<nFindNum; i++){

		CNM_AGENT_NEWUSERERROR aerror;
		aerror.Set(0, &au[i]);
		SockMan.SendData(aSID[i], aerror.pData, aerror.GetTotalSize());
		
		str.Format("\"%s\"회원을 종료시킵니다",au[i].ID);
		ShowState(str,ServerMan.GetName(aSID[i]), TRUE);
	}

 
//	CNetData SendData;
//	for(int i=0; i<nFindNum; i++){
//		SendData.SetData((LPVOID)(&(au[i])),sizeof(AGENT_USER),NM_QMDATA,NM_AGENT_NEWUSERERROR,1);
//		SockMan.SendData(aSID[i], SendData.GetData(),SendData.GetSize());
//		str.Format("\"%s\"회원을 종료시킵니다",au[i].ID);
//		ShowState(str,ServerMan.GetName(aSID[i]),TRUE);
//	}
}
void CNMServerAgentDlg::OnTimer(UINT nIDEvent) 
{
	static int nConnectDelay = 0;
	// 자동 접속
	static int timeCnt = 0;

	if(nIDEvent==MAINTAIN_CONNECT)
	{
		// ### [ Queue ] ###
		QueueProcess();
		CString strPM;
		strPM.Format("%d", m_TotalQueCnt);
		GetDlgItem(IDC_STATIC_QUEUE)->SetWindowText(strPM.operator LPCTSTR());

		nConnectDelay++;
		if(nConnectDelay>2){ // 1 마다 접속유지 모드 전송하기
			// #### 잭팟 평균 ####
			/////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////
			GetJackPotAverage();

			int i;
			if( m_AverJackPot )
			{
				
				for(i=0; i<MAX_CONNECT_SERVER; i++){
					if(ServerMan.m_Data[i].m_bValid && ServerMan.m_Data[i].m_bConnect){
						
						CNM_AGENT_AVERAGE gAver;
						gAver.Set(g_AverJackPot);
						int nSID = ServerMan.m_Data[i].m_nSID;
						SockMan.SendData(nSID, gAver.pData, gAver.GetTotalSize());
					}
				}
			}
			/////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////
			else
			{
				
				// 잭팟이 있다면 제거
				for(i=0; i<MAX_CONNECT_SERVER; i++){
					if(ServerMan.m_Data[i].m_bValid && ServerMan.m_Data[i].m_bConnect){

						CSV_HEARTBEAT hbmsg;
						hbmsg.Set();
						int nSID = ServerMan.m_Data[i].m_nSID;
						SockMan.SendData(nSID, hbmsg.pData, hbmsg.GetTotalSize());
					}
				}
			}
			
			nConnectDelay = 0;
		}

		CString str;
		static int delay = 0;
		if( m_bisContinue )
		{
			CString strIP = "";
			UINT nPort = 0;

			CString strName = g_ServerInfoList.INFO[timeCnt].Name;
			if(strName.GetLength() != 0) 
			{
				if(!ServerMan.IsConnect(strName)) 
				{
					strIP = g_ServerInfoList.INFO[timeCnt].IP;
					nPort = g_ServerInfoList.INFO[timeCnt].nPort;
					if(nPort > 1000) 
					{
						if(!ServerMan.IsConnect(strIP, nPort)) 
						{
							int nSID = SockMan.ConnectSocket((char *)(strIP.operator LPCTSTR()), nPort);
							ServerMan.SetData(strName, strIP, nPort, nSID);
							ShowState("서버에 접속 중입니다...", strName, FALSE);
							
							str.Format("자동연결 시도중 [%s]",g_ServerInfoList.INFO[timeCnt].Name );
							GetDlgItem(IDC_STATIC_AUTO_MSG)->SetWindowText(str.operator LPCTSTR());
						}
						else
						{
							ShowState("이미 같은 아이피와 포트번호의 서버 정보가 존재합니다.", strName, FALSE);
						}

					}
				}
				else {
					//ShowState("이미 같은 이름의 서버 정보가 존재합니다!", strName, FALSE);
					str.Format("연결 완료 %s",strName);
					GetDlgItem(IDC_STATIC_AUTO_MSG)->SetWindowText(str.operator LPCTSTR());
				}
			}

			timeCnt++;
			if(timeCnt >= g_ServerInfoList.nTotalList) 
			{
				timeCnt = 0;
				delay = 0;
				m_bisContinue = FALSE;
			}
		}
		else
		{
			
			// 일정 주기
			BOOL bCon = FALSE;
			
			//if(delay > 0 && delay < 20)
			if(delay >= 0 && delay < g_ServerInfoList.nTotalList && timeCnt < g_ServerInfoList.nTotalList)
			{	
				bCon = TRUE;
				str.Format("자동연결 시도중 [%s]",g_ServerInfoList.INFO[timeCnt].Name );
				GetDlgItem(IDC_STATIC_AUTO_MSG)->SetWindowText(str.operator LPCTSTR());
			}
			else 
			{
				timeCnt = 0;
				bCon = FALSE;
				
				if(delay != 0)
				{
					str.Format("자동연결 대기중 %d",g_ServerInfoList.nTotalList+10-delay );
					GetDlgItem(IDC_STATIC_AUTO_MSG)->SetWindowText(str.operator LPCTSTR());
				}
			}

			if(bCon)
			{
				CString strIP = "";
				UINT nPort = 0;
				
				CString strName = g_ServerInfoList.INFO[timeCnt].Name;
				if(strName.GetLength() != 0) 
				{
					if(!ServerMan.IsConnect(strName)) // 이름
					{
						strIP = g_ServerInfoList.INFO[timeCnt].IP;
						nPort = g_ServerInfoList.INFO[timeCnt].nPort;
						if(nPort > 1000) 
						{
							if(!ServerMan.IsConnect(strIP, nPort)) // 아이피,포트
							{
								int nSID = SockMan.ConnectSocket((char *)(strIP.operator LPCTSTR()), nPort);
								ServerMan.SetData(strName, strIP, nPort, nSID);
								ShowState("서버에 접속 중입니다...", strName, FALSE);
							}
							else
							{
								ShowState("이미 같은 아이피와 포트번호의 서버 정보가 존재합니다.", strName, FALSE);
							}
						}
					}
					else
					{
						//	ShowState("이미 같은 이름의 서버 정보가 존재합니다!", strName, FALSE);
						str.Format("연결 완료 %s",strName);
						GetDlgItem(IDC_STATIC_AUTO_MSG)->SetWindowText(str.operator LPCTSTR());
					}
				}
				
				timeCnt++;
				if(timeCnt >= g_ServerInfoList.nTotalList) 
				{
					timeCnt = 0;
				}
			}

			delay++;
			if(delay > g_ServerInfoList.nTotalList+10) 
			{
				delay = 0;
				timeCnt = 0;
			}
		}
	}


/*
	else if(nIDEvent == AUTO_CONNECT) // 10초 마다 접속유지
	{//일정시간 간격으로 자동 접속

		if(!m_bisContinue)	
		{
			CString strIP = "";
			UINT nPort = 0;
			
			CString strName = g_ServerInfoList.INFO[timeCnt].Name;
			if(strName.GetLength() != 0) 
			{
				if(!ServerMan.IsConnect(strName)) // 이름
				{
					strIP = g_ServerInfoList.INFO[timeCnt].IP;
					nPort = g_ServerInfoList.INFO[timeCnt].nPort;
					if(nPort > 1000) 
					{
						if(!ServerMan.IsConnect(strIP, nPort)) // 아이피,포트
						{
							int nSID = SockMan.ConnectSocket((char *)(strIP.operator LPCTSTR()), nPort);
							ServerMan.SetData(strName, strIP, nPort, nSID);
							ShowState("서버에 접속 중입니다...", strName, FALSE);
						}
						else
						{
							ShowState("이미 같은 아이피와 포트번호의 서버 정보가 존재합니다.", strName, FALSE);
						}
					}
				}
				else
				{
				//	ShowState("이미 같은 이름의 서버 정보가 존재합니다!", strName, FALSE);
					str.Format("연결 완료 %s",strName);
					GetDlgItem(IDC_STATIC_AUTO_MSG)->SetWindowText(str.operator LPCTSTR());
				}
			}
		
			timeCnt++;
			if(timeCnt >= MAX_CONNECT_SERVER-1) 
			{
				timeCnt = 0;
			}
		}
	}
*/
	
	CDialog::OnTimer(nIDEvent);
}

// 접속을 끊는다
void CNMServerAgentDlg::OnMenuDisconnect() 
{
	if(m_nSelNum < 0) return;

	int rtn=AfxMessageBox("서버와 접속을 끊습니다. 계속 진행할까요?", MB_YESNO);
	if(rtn!=IDYES) return;
	
	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_ctrList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	m_nSelNum = m_ctrList.GetNextSelectedItem(pos);

	if(m_nSelNum < 0) return;
	if(m_nSelNum >= m_ctrList.GetItemCount()) return;

	CString str;
	str = m_ctrList.GetItemText(m_nSelNum, 0);

	CSrvInfo *pInfo = ServerMan.GetSrvInfo(str);
	if(pInfo) Disconnect(pInfo->m_nSID);
}

void CNMServerAgentDlg::OnItemchangedListSrv(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nSelNum = pNMListView->iItem;
	*pResult = 0;
}

void CNMServerAgentDlg::OnRclickListSrv(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nSelNum = pNMListView->iItem;
	*pResult = 0;

	if(m_nSelNum < 0)
	{
		m_ctrMenu.GetSubMenu(1)->EnableMenuItem(IDC_MENU_DISCONNECT, MF_GRAYED);
	}
	else
	{
		m_ctrMenu.GetSubMenu(1)->EnableMenuItem(IDC_MENU_DISCONNECT, MF_ENABLED);
	}

	// 팝업 메뉴
	CPoint pnt;
	GetCursorPos(&pnt);
	m_ctrMenu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
}

// 중복접속 허용여부
void CNMServerAgentDlg::OnCheckDcon() 
{
	UpdateData();
	if(m_bDCon)
		AfxMessageBox("[경고] 중복접속을 허용합니다!");
	else
		AfxMessageBox("[알림] 중복접속을 불허합니다!");
}

// 히드토리 데이타 수동으로 저장
void CNMServerAgentDlg::OnButtonSave() 
{
/*	CString str;
	str.Format("문자열 테스트입니다");
	ShowState(str,"테스트",FALSE);
*/

	if(g_nSaveIndex<=0){
		AfxMessageBox("저장할 데이타가 없습니다!"); return;
	}

	CString str;
	str.Format("메모리상에 %d개의 히스토리 데이타가 있습니다\n데이타를 파일로 저장할까요?",g_nSaveIndex);
	int rtn=AfxMessageBox(str, MB_YESNO);
	if(rtn!=IDYES) return;

	// 여기서 데이타를 저장한다
	SaveData();
	g_nSaveIndex = 0;

}

void CNMServerAgentDlg::OnButtonAuto() 
{
	// TODO: Add your control notification handler code here

	if( m_bisContinue ) return;
	m_bisContinue = TRUE;
	return ;



	CString strIP = "";
	UINT nPort = 0;

	for(int i=0; i<MAX_CONNECT_SERVER; i++)
	{
		CString strName = g_ServerInfoList.INFO[i].Name;
		if(strName.GetLength() != 0) 
		{
			if(ServerMan.IsConnect(strName)) 
			{
				ShowState("이미 같은 이름의 서버 정보가 존재합니다!", strName, FALSE);
				continue;
			}

			strIP = g_ServerInfoList.INFO[i].IP;
			nPort = g_ServerInfoList.INFO[i].nPort;
			if(nPort < 1000) continue;
			
			if(ServerMan.IsConnect(strIP, nPort)) 
			{
				ShowState("이미 같은 아이피와 포트번호의 서버 정보가 존재합니다.", strName, FALSE);
				continue;
			}

			int nSID = SockMan.ConnectSocket((char *)(strIP.operator LPCTSTR()), nPort);
			ServerMan.SetData(strName, strIP, nPort, nSID);
			ShowState("서버에 접속 중입니다...", strName, FALSE);
			

		}
	}
}

void CNMServerAgentDlg::OnButtonAlldis() 
{
	// TODO: Add your control notification handler code here
	//if(m_nSelNum < 0) return;

	//int rtn=AfxMessageBox("서버와 접속을 끊습니다. 계속 진행할까요?", MB_YESNO);
	//if(rtn!=IDYES) return;
	
	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_ctrList.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox("종료할 항목을 선택하세요");
		return;
	}

	
	//m_nSelNum = m_ctrList.GetNextSelectedItem(pos);
	int num = m_ctrList.GetNextSelectedItem(pos);
	if(num < 0) return;
	if(num >= m_ctrList.GetItemCount()) return;

	int totsel = m_ctrList.GetSelectedCount();
	if(totsel >= 1) {
		CString str;
		str.Format("선택된 %d개의 항목을 종료합니다. 계속하시겠습니까?", totsel);
		if(MessageBox(str, "경고", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) return;
	}

	pos = m_ctrList.GetFirstSelectedItemPosition();


	while(pos) {
		pos = m_ctrList.GetFirstSelectedItemPosition();
		num = m_ctrList.GetNextSelectedItem(pos);
		if(num < 0) break;

		CString str;
		str = m_ctrList.GetItemText(num, 0);

		CSrvInfo *pInfo = ServerMan.GetSrvInfo(str);
		if(pInfo) Disconnect(pInfo->m_nSID);
	}

	m_ctrList.SetFocus();
}

void CNMServerAgentDlg::OnCheckAverage() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_AverJackPot)
		AfxMessageBox("[경고] 모든 서버 잭팟금액을 통일합니다.!");
	else
		AfxMessageBox("[알림] 각각 잭팟금액 유지!");
}

// 잭팟 금액 평균 구하기
void CNMServerAgentDlg::GetJackPotAverage()
{
	// #### 잭팟 평균 ####
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	int cntServer = 0;
	INT64 JackPot = 0;
	for(int i=0; i<MAX_CONNECT_SERVER; i++){
		if(ServerMan.m_Data[i].m_bValid && ServerMan.m_Data[i].m_bConnect){
			cntServer++;
			JackPot += ServerMan.m_Data[i].m_JackPot;
		}
	}
	
	if(cntServer > 0)
	{
		JackPot /= cntServer;
		// 잭팟
		CString strPM;
		strPM = g_MakeCommaMoney(JackPot);
		strPM += "원";
		GetDlgItem(IDC_STATIC_JACKPOT)->SetWindowText(strPM.operator LPCTSTR());
		
		g_AverJackPot = JackPot;
	}
	else
	{
		JackPot = 0;
		// 잭팟
		CString strPM;
		strPM = g_MakeCommaMoney(JackPot);
		strPM += "원";
		GetDlgItem(IDC_STATIC_JACKPOT)->SetWindowText(strPM.operator LPCTSTR());
		
		g_AverJackPot = 0;
	}


	// 값 다시 셋팅
	if( m_AverJackPot )
	{
		for(i=0; i<MAX_CONNECT_SERVER; i++){
			if(ServerMan.m_Data[i].m_bValid && ServerMan.m_Data[i].m_bConnect){
				ServerMan.m_Data[i].m_JackPot = g_AverJackPot;

				// 잭팟 금액 리스트 콘트롤 갱신
				SetItemJackPot((char *)ServerMan.GetName(i).operator LPCTSTR(), g_AverJackPot);
			}
		}
	}


/*	
	if( m_AverJackPot )
	{
		CNM_AGENT_AVERAGE gAver;
		gAver.Set(g_AverJackPot);
		for(i=0; i<MAX_CONNECT_SERVER; i++){
			if(ServerMan.m_Data[i].m_bValid && ServerMan.m_Data[i].m_bConnect){
				
				ServerMan.m_Data[i].m_JackPot = g_AverJackPot;
				
				int nSID = ServerMan.m_Data[i].m_nSID;
				SockMan.SendData(nSID, gAver.pData, gAver.GetTotalSize());
			}
		}
	}
*/	
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////


}


// ### [ Queue ] ###
BOOL CNMServerAgentDlg::AddQueue(char *id)
{
	if( id == NULL) return FALSE;
	LPQDATA  lpData, lpTmp;
	lpData = new QDATA;

	ZeroMemory(lpData, sizeof(QDATA));
	strncpy(lpData->ID, id, 15);
	lpData->Count = 0;

	//끝에 추가
	lpTmp = m_head->next;
	if(lpTmp != m_tail)	{
		while(lpTmp != m_tail)
		{
			if(lpTmp->next == m_tail)
			{
				lpTmp->next = lpData;
				break;
			}
			lpTmp = lpTmp->next;
		}
	}
	else {
		m_head->next = lpData;
	}
	lpData->next = m_tail;
	m_TotalQueCnt++;
	return TRUE;

}

BOOL CNMServerAgentDlg::QueueProcess()
{
	if(m_TotalQueCnt<=0) return FALSE;
	LPQDATA  lpData;
	//처음부터 처리
	lpData = m_head->next;
	if(lpData == m_tail) 
	{
	
#ifdef _DEBUG
		AfxMessageBox("lpData == m_tail");
#endif
		return FALSE;
	}
	
	while(lpData != m_tail)
	{
		lpData->Count++;
		if(lpData->Count > 2)
		{//앞에서 부터 삭제
			m_head->next = lpData->next;
			delete lpData;
			m_TotalQueCnt--;
			if(m_TotalQueCnt <= 0) {
				m_TotalQueCnt = 0;
			}
			lpData = m_head->next;
		}
		else
		{
			lpData = lpData->next;
		}
	} 

	return TRUE;
}


BOOL CNMServerAgentDlg::FindQueId(char *id, int &nRcount)
{
	if(m_TotalQueCnt<=0) return FALSE;
	LPQDATA  lpData;
	//처음부터 처리
	lpData = m_head->next;
	if(lpData == m_tail) 
	{
	
#ifdef _DEBUG
		AfxMessageBox("lpData == m_tail");
#endif
		return FALSE;
	}

	while(lpData != m_tail)
	{
		if(strcmp(lpData->ID, id)==0) 
		{
			nRcount = 2 - lpData->Count;
			return TRUE;
		}
		lpData = lpData->next;
	}
	return FALSE;
}

void CNMServerAgentDlg::QueDeleteAll()
{
	LPQDATA lpData, lptmp;
	lptmp = m_head->next;
	while(lptmp != m_tail)
	{
		lpData = lptmp;
		lptmp = lptmp->next;
		delete lpData;
	}

	m_head->next = m_tail;
	m_TotalQueCnt = 0;
}


#include "AdminManager.h"
void CNMServerAgentDlg::OnButtonAdmin() 
{
	// TODO: Add your control notification handler code here
	CAdminManager Dlg;
	Dlg.DoModal();
	
}

//### [관리자 모드 작업] ###
BOOL CNMServerAgentDlg::IsAdminUser(char *id)
{
	if(id==NULL) return FALSE;
	
	BOOL bAdmin = FALSE;

	for(int i=0; i<MAX_ADMIN_NUM; i++){
		if(strlen(g_AdminInfo.AdminMan.admin[i].ID) > 0) {
			if(strcmp(g_AdminInfo.AdminMan.admin[i].ID, id)==0){
				bAdmin = TRUE;
				break;
			}
		}
	}
	return bAdmin;
	
}
