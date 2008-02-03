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
//#define AUTO_CONNECT		1001 // �����ð����� ���ӽõ�
#define MAX_DATA_SAVE       5000   // 5000���� �����Ѵ�

CNMServerAgentDlg *g_pMainDlg = NULL;



///////////////////////////////////////////////////////////////////////
typedef struct{
	BOOL       bCon;    // ���ӿ���
	AGENT_USER au;      // ����Ÿ
	int        Hour;	// ��
	int        Min;		// ��
	int        Sec;		// ��
	int        nSID;
} AGENT_USER_SAVE;

AGENT_USER_SAVE AUSD[MAX_DATA_SAVE];

int g_nSaveIndex = 0;

// ��������� ����Ÿ�� �����Ѵ�
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
		// ���⼭ ����Ÿ�� �����Ѵ�
		SaveData();
		g_nSaveIndex = 0;
	}
}

// �����丮 ����� �����Ѵ�
void CNMServerAgentDlg::SaveData()
{
	const UINT lFileSize  = 128*MAX_DATA_SAVE; // �������Ÿ �ƽø� ������
	static char szLogData[lFileSize] = {0,};
	UINT  nTotalSize = 0;
	memset(szLogData,0,lFileSize);

	for(int i=0; i<min(MAX_DATA_SAVE-1,g_nSaveIndex); i++){
		CString strLogTemp(_T(""));
		if(AUSD[i].bCon) // ���ӵ���Ÿ�̰� �����Ӵ��̴�
			strLogTemp.Format("��\"%s\" %I64d (%d:%d:%d) %s\r\n",
				AUSD[i].au.ID,AUSD[i].au.PMoney,AUSD[i].Hour,AUSD[i].Min,AUSD[i].Sec,ServerMan.GetName(AUSD[i].nSID));
		else             // ���ᵥ��Ÿ�̰� ����ġ�̴�
			strLogTemp.Format("��\"%s\" %I64d (%d:%d:%d) %s\r\n",
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

	strReportDir.Format("%s\\BACKUP\\%d��%d��",g_AppStartPath,t.GetMonth(),t.GetDay());
	if(_chdir(strReportDir.operator LPCTSTR())!=0){
		_mkdir(strReportDir.operator LPCTSTR());
	}
	CString strFileName;
	strFileName.Format("%s\\%d��%d��%d��.txt",strReportDir,t.GetHour(),t.GetMinute(),t.GetSecond());

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



	// ��� �޼��� ó�� Ŭ���� �ʱ�ȭ
	if(!m_SockCmnd.Init(this)) {
		AfxMessageBox("���� �ʱ�ȭ�� �����Ͽ����ϴ�");
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

	// ����Ʈ �ʱ�ȭ�ϱ�
	InitList();
	InitUList();

	m_nSelUser = 0;
	m_comboUList.ResetContent();
	m_comboUList.AddString("���õ� ���� ����");
	m_comboUList.SetCurSel(0);

	CString str; str.Format("���������ڸ� �����մϴ�");	ShowState(str,"�˸�",FALSE);

	// ### [ Queue ] ###
	m_head = (QDATA*)new QDATA;
	m_tail = (QDATA*)new QDATA;

	m_head->next = m_tail;
	m_tail->next = m_tail;
	
	m_TotalQueCnt = 0;;
	

	//### [������ ��� �۾�] ###
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

	m_bisContinue = FALSE; // �ڵ�����
	SetTimer(MAINTAIN_CONNECT,1000,NULL); // �������� Ÿ�̸Ӹ� �Ҵ�
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

	// ���� �޴��� ����
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
	// ��ü�� ����Ʈ ����
	m_ctrList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//// Column ���Ժ�
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
	// ����
	col.fmt  = LVCFMT_RIGHT;
	col.cx   = 170;
	wsprintf(szName, "JackPot");
	col.iSubItem = 4;
	m_ctrList.InsertColumn(4, &col);
	/////////////////////////////////////


}

void CNMServerAgentDlg::InsItem(char *Name, char *IP, UINT nPort, int nNum, INT64 jackpot)
{
	// ���ڵ带 ����Ʈ �信 ����
	LV_ITEM item;
	char szName[256]={0,};

	// ���� ������ �����ϴ��� ���� �˻��ϰ� �̹� �����ϸ� ���
	int num=-1;
	CString szRN;
	for(;;)
	{
		num = m_ctrList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szRN = m_ctrList.GetItemText(num, 0);
		if(strcmp(Name, szRN.operator LPCTSTR())==0) return;
	}

	// �����̸�
	wsprintf(szName, "%s", Name);
	item.mask = LVIF_TEXT;
	item.pszText = szName;
	item.iItem = m_ctrList.GetItemCount();
	item.iSubItem = 0;
	m_ctrList.InsertItem(&item);

	// ����������
	wsprintf(szName, "%s", IP);
	item.iSubItem = 1;
	m_ctrList.SetItem(&item);

	// ��Ʈ
	wsprintf(szName, "%d", nPort);
	item.iSubItem = 2;
	m_ctrList.SetItem(&item);

	// �ο�
	wsprintf(szName, "%d", nNum);
	item.iSubItem = 3;
	m_ctrList.SetItem(&item);

	//////////////////////////////////
	// ����
	CString strPM;
	strPM = g_MakeCommaMoney(jackpot);
	strPM += "��";
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
			// �ο�
			wsprintf(szName, _T("%d"), nNum);
			m_ctrList.SetItemText(num, 3, szName);

			// ����
			CString strPM;
			strPM = g_MakeCommaMoney(jackpot);
			strPM += "��";

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
			// ����
			CString strPM;
			strPM = g_MakeCommaMoney(jackpot);
			strPM += "��";
					 
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


// �������� ����Ÿ�� �޴� ���.
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

	// �ùٸ� ����Ÿ���� �ѹ� �� Ȯ���Ѵ�
	CNetData nData;
	if(nData.IsData((char *)lpData,TotSize) && nData.GetType()==NM_QMDATA) bProcess  = TRUE;
	if(!bProcess) return 0;
	int  nUserVal    = nData.GetUserVal(); // ����� �� Ȯ��
	SIGNAL signNum   = nData.GetSign();
	if((int)signNum != Signal) return 0;

	CSrvInfo *pInfo  = ServerMan.GetSrvInfo(nSID);
	switch(signNum)
*/
	GAMENOTIFY *pNotifyData = (GAMENOTIFY*)wParam;
	if(!pNotifyData) return 0;

	int Sid = pNotifyData->Sid;				// ���� ��ȣ
	int Signal = pNotifyData->Signal;		// �޽��� �ñ׳�
	int TotSize = pNotifyData->TotSize;		// �޽��� ������
	char *lpData = pNotifyData->lpData;		// �޽��� ������ ������
	
	int nSID = Sid;

	switch(Signal)
	{
	case SV_CHECKVERSION: // ���� üũ
		{
			CSV_CHECKVERSION msg;
			msg.Get(lpData, TotSize);
//			if(g_MainSrvSID != Sid) break;

			int ver = *msg.Ver;
			if(ver<0) break;

			// ������ Ʋ���� ��������
//			if(ver != VERSION)
//			{
//				PostMessage(UM_DATA_CLOSE, NULL, (long)SockMan.pDataSock[g_MainSrvSID]); 
//				AfxMessageBox("���α׷��� ������ ��ġ���� �ʽ��ϴ�. �ֽ� ������ �ٿ��������");
//				m_ConnectMsgDlg.OnCancel();
//				break;
//			}
/*
			// �α��� ���̸�
//			if(g_Where == WH_LOGIN)
			{
				CString strID,strPass;
				strID.Format("%s",CONNECT_ID);
				strPass.Format("%s",CONNECT_PASS);

				char *id = (char*)strID.operator LPCTSTR();
				char *pass = (char*)strPass.operator LPCTSTR();

				

				char *socknameip = (char*)SockMan.pDataSock[0]->SockAddr.operator LPCTSTR();	// ����� ������ ��巹��
				char *hostnameip = (char*)SockMan.GetServerIP();	// ȣ��Ʈ �ý��� ��巹��
				UINT port = SockMan.pDataSock[0]->SockPort;							// ����� ������ ��Ʈ��ȣ
				
				// ������ ������ �����ϱ� ���� 20000���� ������ ��Ʈ��ȣ�� ����Ѵ�
				// ���� ����ϰԵ� ��Ʈ��ȣ�� ���μ����� ���Ͽ� �����ȴ�
				port += 20000; 

				// �α��� �޼����� ������
				CSV_LOGIN lgimsg;
				lgimsg.Set(id, pass, FALSE, socknameip, hostnameip, port);
				SockMan.SendData(Sid, lgimsg.pData, lgimsg.GetTotalSize());
			}
*/
				ASK_LOGININFO al;
				memset(&al, 0, sizeof(ASK_LOGININFO));

			
				strcpy(al.ID, CONNECT_ID);
				strcpy(al.Pass, CONNECT_PASS);
			

				strncpy(al.SockNameIP, SockMan.pDataSock[0]->SockAddr, 20);	// ����� ������ ��巹��
				strncpy(al.HostNameIP, SockMan.GetServerIP(), 20);						// ȣ��Ʈ �ý��� ��巹��
				al.Port = SockMan.pDataSock[0]->SockPort;				// ����� ������ ��Ʈ��ȣ
				// ������ ������ �����ϱ� ���� 20000���� ������ ��Ʈ��ȣ�� ����Ѵ�
				// ���� ����ϰԵ� ��Ʈ��ȣ�� ���μ����� ���Ͽ� �����ȴ�
				al.Port += 20000; 

				//al.AuthCookieLen = g_AuthCookie.GetLength();
				//al.DataCookieLen = g_DataCookie.GetLength();
				CString g_AuthCookie="";	// ���� ��Ű						[SSO �۾�]
				CString g_DataCookie="";	// ������ ��Ű	

				// �α��� �޼����� ������
				CSV_ASK_LOGIN lgimsg;
				lgimsg.Set(&al, (char*)(LPCTSTR)g_AuthCookie, (char*)(LPCTSTR)g_DataCookie);
				SockMan.SendData(Sid, lgimsg.pData, lgimsg.GetTotalSize());


		} break;
	
	// [���� ������Ʈ �۾�] : ������Ʈ�� ���ӵǾ���
	case NM_AGENT_CONNECT:
			{
				CNM_AGENT_CONNECT msg;
				msg.Get(lpData, TotSize);

				int nTotalList = *msg.TotalNum;
				
				INT64 jackpot = *msg.JackPot;
				if(jackpot < 0 ) jackpot = 0;


				ShowState("������ ���ӵǾ����ϴ�",ServerMan.GetName(nSID), FALSE);

				InsItem((char *)ServerMan.GetName(nSID).operator LPCTSTR(),
					(char *)ServerMan.GetIP(nSID).operator LPCTSTR(),
					ServerMan.GetPort(nSID), nTotalList, jackpot); // �߰�

				m_comboUList.AddString(ServerMan.GetName(nSID).operator LPCTSTR());

				ServerMan.ClearUserData(nSID);
				ServerMan.SetTotal(nSID,nTotalList, jackpot);
				ServerMan.Connect(nSID);

				GetJackPotAverage();// #### ���� ��� ####

				////////////////////////////////////////////////////////////////////////
				//### [������ ��� �۾�] ###
			
				
				for(int i=0; i<MAX_CONNECT_SERVER; i++) {//������ ����Ʈ �����ش�
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

	// ù ������ 400�� ���� ���� 1��
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
			str.Format("����ڸ� �������Դϴ� : %d��",nTotalList);
			ShowState(str,ServerMan.GetName(nSID),FALSE);


			if(CurCnt >= 0)
			{	// ��û
				if(CurCnt == 0)  NextCnt = 1; //  400 -  799��
				if(CurCnt == 1)  NextCnt = 2; //  800 - 1199��
				if(CurCnt == 2)  NextCnt = 3; // 1200 - 1599��
				if(CurCnt == 3)  NextCnt = 4; // 1600 - 1999��
				if(CurCnt == 4)  NextCnt = 5; // 2000 - 2399��
				if(CurCnt == 5)  NextCnt = 6; // 2400 - 2799
				if(CurCnt == 6)  NextCnt = 7; // 2800 - 

				// ���� ��� ��û 2 ~ ?
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
					CString strTest; strTest.Format("%s:���� ���� �õ�!!!!!",au.ID);
					ShowState(strTest,ServerMan.GetName(nSID), FALSE);
					
					if(!m_bDCon){ // �ߺ� ���� ����ΰ�?

						///////////////////////////////////////////////////////////////////////////
						// �߰� 
						// ���� ������ �ִ� ������ ������ġ�� ä�� ����â�̶�� ���� ����
						CString strID = au.ID;
						AGENT_USER aus[MAX_CONNECT_SERVER];
						memset(aus,0,sizeof(AGENT_USER)*MAX_CONNECT_SERVER);
						
						int aSID[MAX_CONNECT_SERVER] = {0,};
						int nFindNum = ServerMan.GetUser(strID,(AGENT_USER *)aus,(int *)aSID);

						CString str,strName;
						
						if(nFindNum>0)
						{
							for(int i=0; i<nFindNum; i++)
							{// �������� ���� ��û
								CNM_AGENT_NEWUSERERROR aerror;
								aerror.Set(5, &aus[i]); // ��û
								SockMan.SendData(aSID[i], aerror.pData, aerror.GetTotalSize());
								
								//str.Format("\"%s\" ���Ό��?",aus[i].ID);
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
				//����Ʈ�� �ֳ� ���� �ѹ��� �˻�
				else
				{
					// ### [ Queue ] ###
					int nRcount = 0;
					if( FindQueId(au.ID, nRcount) )
					{
						CString strTest; strTest.Format("Queue....... .%s:���� ���� �õ�  ���ð� %d !!!!!",au.ID, nRcount);
						ShowState(strTest,ServerMan.GetName(nSID), FALSE);
						
						if(!m_bDCon){ // �ߺ� ���� ����ΰ�?
							
							///////////////////////////////////////////////////////////////////////////
							// �߰� 
							// ���� ������ �ִ� ������ ������ġ�� ä�� ����â�̶�� ���� ����
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
								{// �������� ���� ��û
									CNM_AGENT_NEWUSERERROR aerror;
									aerror.Set(5, &aus[i]); // ��û
									SockMan.SendData(aSID[i], aerror.pData, aerror.GetTotalSize());
									
									str.Format("\"%s\" ���Ό��?",aus[i].ID);
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





				// �������� �����̸� �����Ѵ�
				ServerMan.SetUser(nSID,au);
				InsertData(au,nSID,TRUE); // �����丮 ������̴�
				ServerMan.SetTotal(nSID, nTotalList, jackpot);
				SetItem((char *)ServerMan.GetName(nSID).operator LPCTSTR(),nTotalList, jackpot);
				InsUItem(au,(char *)ServerMan.GetName(nSID).operator LPCTSTR());

				//### [������ ��� �۾�] ###
				if(IsAdminUser(au.ID) )
				{
					g_AdminInfo.NowNum++;
				}
				// #### ���� ��� ####
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////
				GetJackPotAverage();
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////



			} break;
			
		case SV_SERVERMSG: // ���� �˸� �޼����� ��������
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
					ShowState(str, "[���� �˸�] (�� �������� ���̵�) ó���Ϸ�", FALSE);
				}
				else if(*msg.Kind == 1)
				{
					// �α� ���� ���
					ShowState(str, "[���� �˸�]  ���� ���ӽõ�", TRUE);
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

				// �������� ���������̸� �����Ѵ�
				ServerMan.SetTotal(nSID,nTotalList, jackpot);
				ServerMan.DelUser(nSID,au);

				// ### [ Queue ] ###
				AddQueue(au.ID);

				InsertData(au,nSID,FALSE); // �����丮 ������̴�

				SetItem((char *)ServerMan.GetName(nSID).operator LPCTSTR(),nTotalList, jackpot);

				DelUItem(au,(char *)ServerMan.GetName(nSID).operator LPCTSTR());

				//### [������ ��� �۾�] ###
				if(IsAdminUser(au.ID) )
				{
					if(g_AdminInfo.NowNum>0) g_AdminInfo.NowNum--;
				}

				
				// #### ���� ��� ####
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////
				GetJackPotAverage();
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////

			} break;

		// [���� ������Ʈ �۾�] : �ߺ����� Ȯ���� ��û�ؿԴ�
		case NM_AGENT_DUBLECON:
			{
				CNM_AGENT_DUBLECON msg;
				msg.Get(lpData, TotSize);

				AGENT_USER au; memset(&au,0,sizeof(AGENT_USER));
				//memcpy(&au,nData.m_lpData,sizeof(au));
				memcpy(&au, msg.ArraySUI, sizeof(AGENT_USER));


				// �̹� ������ �ִ� ��������� Ȯ������ (�ش缭���� �����ϰ� Ȯ���Ѵ�)
				if(ServerMan.bFindExcept(au.ID,nSID)==TRUE){
					CString strTest; strTest.Format("%s:�ߺ������� Ȯ��!(ó���Ϸ�)",au.ID);
					ShowState(strTest,ServerMan.GetName(nSID), FALSE);
					if(!m_bDCon){

						CNM_AGENT_NEWUSERERROR aerror;
						aerror.Set(0, &au);
						SockMan.SendData(Sid, aerror.pData, aerror.GetTotalSize());
						break;
					}
				}

			} break;


	case SV_REFUSE_LOGIN: // �α����� �ź� �Ǿ���
		{

			CSV_REFUSE_LOGIN msg;
			msg.Get(lpData, TotSize);
			CString str ="!!!";
			int code = *msg.Code;
			if(code==0) ShowState("����ڰ� �ƴմϴ�. ����ID�� Ȯ���ϼ���", str ,FALSE);
			if(code==1) ShowState("��й�ȣ�� Ʋ�Ƚ��ϴ�. ��й�ȣ�� �ٽ� Ȯ���ϼ���", str,FALSE);
			if(code==2) ShowState("�̹� ������� ���̵��Դϴ�. �����ڿ��� �����ϼ���", str,FALSE);
			if(code==3) ShowState("�ִ� �����ο��� �ʰ��Ͽ����ϴ�. ����� �ٽ� �������ּ���", str,FALSE);
			if(code==4) ShowState("�������� ���� ������Դϴ�", str ,FALSE);
			if(code==6) ShowState("�� ���̵�� ���� ��� ���� �����̹Ƿ� ������ ������ �� �����ϴ�.\n�����ڿ��� �����ϼ���", str ,FALSE);
			if(code==7) ShowState("�� ���̵�� ������ ���̵��Դϴ�. �����ڿ��� �����ϼ���", str ,FALSE);
			if(code==5) ShowState("���� ���ӿ��� ���� ���� ���� ó���� �̷������ �ʾҽ��ϴ�.", str ,FALSE);
		
		} break;

	// [ ###����### ] 
	case SV_SPECIALCARD :
		{
			CSV_SPECIALCARD msg;
			msg.Get(lpData, TotSize);

			if(msg.pSC->NowJackPot > 0)
			{			
				CString spec("");
				CString str="";
				if(msg.pSC->nCard==1) spec = "��ī��";
				else if(msg.pSC->nCard==2) spec = "��Ʈ����Ʈ �÷���";
				else if(msg.pSC->nCard==3) spec = "�ξ� ��Ʈ����Ʈ �÷���";

				CString strM = g_MakeCommaMoney(msg.pSC->JackPotPlus);
				strM +="��";
				str.Format("[�˸�] [%s] [%s,%d����]�� [%s]ȸ���� %s �� �ϼ̽��ϴ�. �� ���� ��� %s",
				(char *)ServerMan.GetName(nSID).operator LPCTSTR(), msg.pSC->ChanName, msg.pSC->nRoom, msg.pSC->ID, spec, strM);

				ShowState(str,"�˸�",FALSE);


				ServerMan.SetJackPot(nSID, msg.pSC->NowJackPot);
				// #### ���� ��� ####
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////
				GetJackPotAverage();
				/////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////

				// �� ������ �˷��ش�.
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
	int rtn=AfxMessageBox("������������ �����Ͻðڽ��ϱ�?", MB_YESNO);
	if(rtn!=IDYES) return;

	CString str; str.Format("���������ڸ� �����մϴ�");	ShowState(str,"�˸�",FALSE);
	
	CDialog::OnCancel();
}


// ������ �����Ѵ�
void CNMServerAgentDlg::OnButtonConnect() 
{
	CConncetDlg dlg;
	if(IDOK==dlg.DoModal()){
		// �̹� ���������� üũ�ϱ�
		if(ServerMan.IsConnect(dlg.m_strServerName)){
			AfxMessageBox("�̹� ���� �̸��� ���� ������ �����մϴ�!");
			return;
		}
		if(ServerMan.IsConnect(dlg.m_strIP, dlg.m_nPort)){
			AfxMessageBox("�̹� ���� �����ǿ� ��Ʈ��ȣ�� ���� ������ �����մϴ�!");
			return;
		}
	
		int nSID = SockMan.ConnectSocket((char *)(dlg.m_strIP.operator LPCTSTR()), dlg.m_nPort);

		ServerMan.SetData(dlg.m_strServerName,dlg.m_strIP,dlg.m_nPort,nSID);
		// ���μ����� ���� �õ�
		//g_MainSrvSID = SockMan.ConnectSocket((char*)g_ServIP.operator LPCTSTR(), g_ServPort);

		ShowState("������ ���� ���Դϴ�...",dlg.m_strServerName, FALSE);
	}	
}

// ���¸� ���δ�
void CNMServerAgentDlg::ShowState(CString str, CString strName, BOOL bLog)
{
	if(str=="") return;

	CTime  time = CTime::GetCurrentTime();

	CString strState; COLORREF crCol = RGB(0,0,0);
	strState.Format(_T(" [%d/%d/%d:%d] %s : %s\r\n"),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),strName,str);

	m_pKRich->AddText(strState,crCol);

	if(bLog){
		// ��¥���� �α׸� ����Ѵ�
		CString strReportDir;
		CString strFileName;
		strReportDir.Format("%s\\LOGDATA",g_AppStartPath);
		strFileName.Format("%s\\Log_%d��%d��.txt",strReportDir,time.GetMonth(),time.GetDay());

		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Write(strState);
		}
	}

//	UpdateData(FALSE);
}

// ���ῡ �����Ѵ�
void CNMServerAgentDlg::ConnectFail(int nSID)
{
	CSrvInfo *pInfo = ServerMan.GetSrvInfo(nSID);
	if(pInfo){
		ShowState("������ ������ �����Ͽ����ϴ�!", pInfo->m_strName, FALSE);
	}

	ServerMan.Clear(nSID);
	SockMan.CloseDataSocket(nSID);
}

// ������ ��������
void CNMServerAgentDlg::Disconnect(int nSID)
{
	CSrvInfo *pInfo = ServerMan.GetSrvInfo(nSID);
	if(pInfo){
		ShowState("������ ������ ���������ϴ�!", pInfo->m_strName, FALSE);
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
						// �ش� �۾����� ����Ʈ�� ó���Ѵ�!!!
						m_ctrUser.DeleteAllItems();
					}
					bLoop = TRUE;
				}
			}
		}
	}

	ServerMan.Clear(nSID);
	SockMan.CloseDataSocket(nSID);


	// #### ���� ��� ####
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	GetJackPotAverage();
	
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
}

void CNMServerAgentDlg::InitUList()
{
	// ��ü�� ����Ʈ ����
	m_ctrUser.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//// Column ���Ժ�
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
	// ���ڵ带 ����Ʈ �信 ����
	LV_ITEM item;
	char szName[256]={0,};
	wsprintf(szName, "%s", Name);

	CString strSelList =_T("");
	m_comboUList.GetLBText(m_comboUList.GetCurSel(),strSelList);

	if(strcmp(szName,strSelList.operator LPCTSTR())!=0) return;

	// ���̵�
	wsprintf(szName, "%s", au.ID);
	item.mask = LVIF_TEXT;
	item.pszText = szName;
	item.iItem = m_ctrUser.GetItemCount();
	item.iSubItem = 0;
	m_ctrUser.InsertItem(&item);

	// ������
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

// ����� ����Ʈ ����
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

// ����� ã��
void CNMServerAgentDlg::OnButtonFind() 
{
	CString strID("");
	GetDlgItemText(IDC_EDIT_ID,strID);
	
	if(strID==""){
		AfxMessageBox("ã�� ���̵� �Է��Ͽ� �ֽñ� �ٶ��ϴ�");
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
		str.Format("\"%s\"ȸ���� �������� ó�����Դϴ�.",strID);
		ShowState(str,"����ó����",FALSE);
	}
	//////////////////////////////////////////////


	
	if(nFindNum<=0){
		str.Format("\"%s\"ȸ���� ������ ���� �ʽ��ϴ�",strID);
		ShowState(str,"ã�����",FALSE);
		return;
	}

	for(int i=0; i<nFindNum; i++){
		str.Format("\"%s\"ȸ���� ã�ҽ��ϴ�",au[i].ID);
		ShowState(str,ServerMan.GetName(aSID[i]),FALSE);
	}
}

// ����� ���� �����Ű��
void CNMServerAgentDlg::OnButtonDisconnect() 
{
	int rtn=AfxMessageBox("�ش� ���̵� ���������� �����ŵ�ϴ�. ��� �����ұ��?", MB_YESNO);
	if(rtn!=IDYES) return;

	CString strID("");
	GetDlgItemText(IDC_EDIT_ID,strID);
	
	if(strID==""){
		AfxMessageBox("���������ų ���̵� �Է��Ͽ� �ֽñ� �ٶ��ϴ�");
		return;
	}

	if(strID.Compare(CONNECT_ID)==0){
		AfxMessageBox("������ ���̵�� �����ų�� �����ϴ�");
		return;
	}

	AGENT_USER au[MAX_CONNECT_SERVER];
	memset(au,0,sizeof(AGENT_USER)*MAX_CONNECT_SERVER);
	
	int aSID[MAX_CONNECT_SERVER] = {0,};

	int nFindNum = ServerMan.GetUser(strID,(AGENT_USER *)au,(int *)aSID);

	CString str,strName;
	if(nFindNum<=0){
		str.Format("\"%s\"ȸ���� ������ ���� �ʽ��ϴ�",strID);
		ShowState(str,"�������",FALSE);
		return;
	}

	
	
	for(int i=0; i<nFindNum; i++){

		CNM_AGENT_NEWUSERERROR aerror;
		aerror.Set(0, &au[i]);
		SockMan.SendData(aSID[i], aerror.pData, aerror.GetTotalSize());
		
		str.Format("\"%s\"ȸ���� �����ŵ�ϴ�",au[i].ID);
		ShowState(str,ServerMan.GetName(aSID[i]), TRUE);
	}

 
//	CNetData SendData;
//	for(int i=0; i<nFindNum; i++){
//		SendData.SetData((LPVOID)(&(au[i])),sizeof(AGENT_USER),NM_QMDATA,NM_AGENT_NEWUSERERROR,1);
//		SockMan.SendData(aSID[i], SendData.GetData(),SendData.GetSize());
//		str.Format("\"%s\"ȸ���� �����ŵ�ϴ�",au[i].ID);
//		ShowState(str,ServerMan.GetName(aSID[i]),TRUE);
//	}
}
void CNMServerAgentDlg::OnTimer(UINT nIDEvent) 
{
	static int nConnectDelay = 0;
	// �ڵ� ����
	static int timeCnt = 0;

	if(nIDEvent==MAINTAIN_CONNECT)
	{
		// ### [ Queue ] ###
		QueueProcess();
		CString strPM;
		strPM.Format("%d", m_TotalQueCnt);
		GetDlgItem(IDC_STATIC_QUEUE)->SetWindowText(strPM.operator LPCTSTR());

		nConnectDelay++;
		if(nConnectDelay>2){ // 1 ���� �������� ��� �����ϱ�
			// #### ���� ��� ####
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
				
				// ������ �ִٸ� ����
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
							ShowState("������ ���� ���Դϴ�...", strName, FALSE);
							
							str.Format("�ڵ����� �õ��� [%s]",g_ServerInfoList.INFO[timeCnt].Name );
							GetDlgItem(IDC_STATIC_AUTO_MSG)->SetWindowText(str.operator LPCTSTR());
						}
						else
						{
							ShowState("�̹� ���� �����ǿ� ��Ʈ��ȣ�� ���� ������ �����մϴ�.", strName, FALSE);
						}

					}
				}
				else {
					//ShowState("�̹� ���� �̸��� ���� ������ �����մϴ�!", strName, FALSE);
					str.Format("���� �Ϸ� %s",strName);
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
			
			// ���� �ֱ�
			BOOL bCon = FALSE;
			
			//if(delay > 0 && delay < 20)
			if(delay >= 0 && delay < g_ServerInfoList.nTotalList && timeCnt < g_ServerInfoList.nTotalList)
			{	
				bCon = TRUE;
				str.Format("�ڵ����� �õ��� [%s]",g_ServerInfoList.INFO[timeCnt].Name );
				GetDlgItem(IDC_STATIC_AUTO_MSG)->SetWindowText(str.operator LPCTSTR());
			}
			else 
			{
				timeCnt = 0;
				bCon = FALSE;
				
				if(delay != 0)
				{
					str.Format("�ڵ����� ����� %d",g_ServerInfoList.nTotalList+10-delay );
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
					if(!ServerMan.IsConnect(strName)) // �̸�
					{
						strIP = g_ServerInfoList.INFO[timeCnt].IP;
						nPort = g_ServerInfoList.INFO[timeCnt].nPort;
						if(nPort > 1000) 
						{
							if(!ServerMan.IsConnect(strIP, nPort)) // ������,��Ʈ
							{
								int nSID = SockMan.ConnectSocket((char *)(strIP.operator LPCTSTR()), nPort);
								ServerMan.SetData(strName, strIP, nPort, nSID);
								ShowState("������ ���� ���Դϴ�...", strName, FALSE);
							}
							else
							{
								ShowState("�̹� ���� �����ǿ� ��Ʈ��ȣ�� ���� ������ �����մϴ�.", strName, FALSE);
							}
						}
					}
					else
					{
						//	ShowState("�̹� ���� �̸��� ���� ������ �����մϴ�!", strName, FALSE);
						str.Format("���� �Ϸ� %s",strName);
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
	else if(nIDEvent == AUTO_CONNECT) // 10�� ���� ��������
	{//�����ð� �������� �ڵ� ����

		if(!m_bisContinue)	
		{
			CString strIP = "";
			UINT nPort = 0;
			
			CString strName = g_ServerInfoList.INFO[timeCnt].Name;
			if(strName.GetLength() != 0) 
			{
				if(!ServerMan.IsConnect(strName)) // �̸�
				{
					strIP = g_ServerInfoList.INFO[timeCnt].IP;
					nPort = g_ServerInfoList.INFO[timeCnt].nPort;
					if(nPort > 1000) 
					{
						if(!ServerMan.IsConnect(strIP, nPort)) // ������,��Ʈ
						{
							int nSID = SockMan.ConnectSocket((char *)(strIP.operator LPCTSTR()), nPort);
							ServerMan.SetData(strName, strIP, nPort, nSID);
							ShowState("������ ���� ���Դϴ�...", strName, FALSE);
						}
						else
						{
							ShowState("�̹� ���� �����ǿ� ��Ʈ��ȣ�� ���� ������ �����մϴ�.", strName, FALSE);
						}
					}
				}
				else
				{
				//	ShowState("�̹� ���� �̸��� ���� ������ �����մϴ�!", strName, FALSE);
					str.Format("���� �Ϸ� %s",strName);
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

// ������ ���´�
void CNMServerAgentDlg::OnMenuDisconnect() 
{
	if(m_nSelNum < 0) return;

	int rtn=AfxMessageBox("������ ������ �����ϴ�. ��� �����ұ��?", MB_YESNO);
	if(rtn!=IDYES) return;
	
	// ���õ� �������� ù��° �������� ��´�
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

	// �˾� �޴�
	CPoint pnt;
	GetCursorPos(&pnt);
	m_ctrMenu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
}

// �ߺ����� ��뿩��
void CNMServerAgentDlg::OnCheckDcon() 
{
	UpdateData();
	if(m_bDCon)
		AfxMessageBox("[���] �ߺ������� ����մϴ�!");
	else
		AfxMessageBox("[�˸�] �ߺ������� �����մϴ�!");
}

// �����丮 ����Ÿ �������� ����
void CNMServerAgentDlg::OnButtonSave() 
{
/*	CString str;
	str.Format("���ڿ� �׽�Ʈ�Դϴ�");
	ShowState(str,"�׽�Ʈ",FALSE);
*/

	if(g_nSaveIndex<=0){
		AfxMessageBox("������ ����Ÿ�� �����ϴ�!"); return;
	}

	CString str;
	str.Format("�޸𸮻� %d���� �����丮 ����Ÿ�� �ֽ��ϴ�\n����Ÿ�� ���Ϸ� �����ұ��?",g_nSaveIndex);
	int rtn=AfxMessageBox(str, MB_YESNO);
	if(rtn!=IDYES) return;

	// ���⼭ ����Ÿ�� �����Ѵ�
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
				ShowState("�̹� ���� �̸��� ���� ������ �����մϴ�!", strName, FALSE);
				continue;
			}

			strIP = g_ServerInfoList.INFO[i].IP;
			nPort = g_ServerInfoList.INFO[i].nPort;
			if(nPort < 1000) continue;
			
			if(ServerMan.IsConnect(strIP, nPort)) 
			{
				ShowState("�̹� ���� �����ǿ� ��Ʈ��ȣ�� ���� ������ �����մϴ�.", strName, FALSE);
				continue;
			}

			int nSID = SockMan.ConnectSocket((char *)(strIP.operator LPCTSTR()), nPort);
			ServerMan.SetData(strName, strIP, nPort, nSID);
			ShowState("������ ���� ���Դϴ�...", strName, FALSE);
			

		}
	}
}

void CNMServerAgentDlg::OnButtonAlldis() 
{
	// TODO: Add your control notification handler code here
	//if(m_nSelNum < 0) return;

	//int rtn=AfxMessageBox("������ ������ �����ϴ�. ��� �����ұ��?", MB_YESNO);
	//if(rtn!=IDYES) return;
	
	// ���õ� �������� ù��° �������� ��´�
	POSITION pos = m_ctrList.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox("������ �׸��� �����ϼ���");
		return;
	}

	
	//m_nSelNum = m_ctrList.GetNextSelectedItem(pos);
	int num = m_ctrList.GetNextSelectedItem(pos);
	if(num < 0) return;
	if(num >= m_ctrList.GetItemCount()) return;

	int totsel = m_ctrList.GetSelectedCount();
	if(totsel >= 1) {
		CString str;
		str.Format("���õ� %d���� �׸��� �����մϴ�. ����Ͻðڽ��ϱ�?", totsel);
		if(MessageBox(str, "���", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) return;
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
		AfxMessageBox("[���] ��� ���� ���̱ݾ��� �����մϴ�.!");
	else
		AfxMessageBox("[�˸�] ���� ���̱ݾ� ����!");
}

// ���� �ݾ� ��� ���ϱ�
void CNMServerAgentDlg::GetJackPotAverage()
{
	// #### ���� ��� ####
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
		// ����
		CString strPM;
		strPM = g_MakeCommaMoney(JackPot);
		strPM += "��";
		GetDlgItem(IDC_STATIC_JACKPOT)->SetWindowText(strPM.operator LPCTSTR());
		
		g_AverJackPot = JackPot;
	}
	else
	{
		JackPot = 0;
		// ����
		CString strPM;
		strPM = g_MakeCommaMoney(JackPot);
		strPM += "��";
		GetDlgItem(IDC_STATIC_JACKPOT)->SetWindowText(strPM.operator LPCTSTR());
		
		g_AverJackPot = 0;
	}


	// �� �ٽ� ����
	if( m_AverJackPot )
	{
		for(i=0; i<MAX_CONNECT_SERVER; i++){
			if(ServerMan.m_Data[i].m_bValid && ServerMan.m_Data[i].m_bConnect){
				ServerMan.m_Data[i].m_JackPot = g_AverJackPot;

				// ���� �ݾ� ����Ʈ ��Ʈ�� ����
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

	//���� �߰�
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
	//ó������ ó��
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
		{//�տ��� ���� ����
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
	//ó������ ó��
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

//### [������ ��� �۾�] ###
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
