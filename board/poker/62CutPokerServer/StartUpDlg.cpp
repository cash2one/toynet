// StartUpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "StartUpDlg.h"

#include "62CutPokerServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartUpDlg dialog


CStartUpDlg::CStartUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStartUpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStartUpDlg)
	m_Info = _T("");
	m_bInitConnectStatus = FALSE;
	//}}AFX_DATA_INIT
}


void CStartUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStartUpDlg)
	DDX_Text(pDX, IDC_EDIT_INFO, m_Info);
	DDX_Check(pDX, IDC_CHECK_INITCONNECTSTATUS, m_bInitConnectStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartUpDlg, CDialog)
	//{{AFX_MSG_MAP(CStartUpDlg)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, OnButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG2, OnButtonConfig2)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG3, OnButtonConfig3)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG4, OnButtonConfig4)
	ON_BN_CLICKED(IDC_BUTTON_EVENT, OnButtonEvent) // ### [이벤트 티켓] ###
	ON_BN_CLICKED(IDC_BUTTON_SA, OnButtonSa)		// [ 통계기록 ]
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_TRAYICON_MSG, TrayIconMsg)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartUpDlg message handlers

BOOL CStartUpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Info = "";

	CString str;
	str.Format("※ 서버명 : %s\r\n", g_pMainView->Cfg.ServerName);
	m_Info += str;
	str.Format("※ GameCode : %d\r\n", g_pMainView->Cfg.GameCode);
	m_Info += str;
	str.Format("※ ServerCode : %d\r\n", g_pMainView->Cfg.ServerCode);
	m_Info += str;
	str.Format("※ Port : %d\r\n", g_pMainView->Cfg.ServerPort);
	m_Info += str;
	str.Format("※ 공지URL : %s\r\n", g_pMainView->Cfg2.NoticeURL);
	m_Info += str;
	str.Format("※ 접속 허용 인원 : %d\r\n", g_pMainView->Cfg2.MaxUser);
	m_Info += str;

	CString strM = g_MakeCommaMoney(g_pMainView->Cfg4.nJackPotMoney);
	str.Format("※ JackPot Money : %s\r\n", (char*)strM.operator LPCTSTR());
	//str.Format("※ JackPot Money : %I64d\r\n", );
	m_Info += str;

	CString strCode;
	switch(g_pMainView->Cfg.nMSCode){
		case 0:  strCode = _T("KOREAN");   break;
		case 1:  strCode = _T("ENGLISH");  break;
		case 2:  strCode = _T("JAPANESE"); break;
		case 3:  strCode = _T("CHINESE");  break;
		default: strCode = _T("KOREAN");   break;
	}

	str.Format("※ 언어  : %s\r\n", (char*)strCode.operator LPCTSTR());
	m_Info += str;



	m_bInitConnectStatus = FALSE;

	UpdateData(FALSE);
	g_pMainView->bNowStartUpDlg = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStartUpDlg::OnOK() 
{
	// TODO: Add extra validation here
	g_pMainView->bNowStartUpDlg = FALSE;
	CDialog::OnOK();
}

void CStartUpDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	g_pMainView->bNowStartUpDlg = FALSE;
	CDialog::OnCancel();
}

void CStartUpDlg::OnButtonConfig() 
{
	// TODO: Add your control notification handler code here
	g_pMainView->LoadConfig();
	g_pMainView->PostMessage(WM_COMMAND, ID_VIEW_CONFIG);
}

void CStartUpDlg::OnButtonConfig2() 
{
	// TODO: Add your control notification handler code here
	g_pMainView->LoadConfig2();
	g_pMainView->PostMessage(WM_COMMAND, ID_VIEW_CONFIG2);
}

void CStartUpDlg::OnButtonConfig3() 
{
	// TODO: Add your control notification handler code here
	g_pMainView->LoadConfig3();
	g_pMainView->PostMessage(WM_COMMAND, ID_VIEW_CONFIG3);
}

void CStartUpDlg::OnButtonConfig4() 
{
	// TODO: Add your control notification handler code here
	g_pMainView->LoadConfig4();
	g_pMainView->PostMessage(WM_COMMAND, ID_VIEW_CONFIG4);
	
}

// ### [이벤트 티켓] ###
void CStartUpDlg::OnButtonEvent() 
{
	// TODO: Add your control notification handler code here
	g_pMainView->LoadConfigEvent();
	g_pMainView->PostMessage(WM_COMMAND, ID_VIEW_CONFIGEVENT);
	
}

// [ 통계기록 ]
void CStartUpDlg::OnButtonSa() 
{
	// TODO: Add your control notification handler code here
	g_pMainView->LoadConfigStatis();
	g_pMainView->PostMessage(WM_COMMAND, ID_VIEW_CONFIGSA);
	
}

void CStartUpDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here

	// 자동 StartUp - jeong
	//m_bIsTrayIcon = FALSE;
	//RegistTrayIcon();
	//lpwndpos->flags &= ~SWP_SHOWWINDOW;
	//g_pMainView->bNowStartUpDlg = FALSE;
	CDialog::OnOK();
}

void CStartUpDlg::RegistTrayIcon()
{
	NOTIFYICONDATA  nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd; // 메인 윈도우 핸들
	nid.uID = IDR_MAINFRAME;  // 아이콘 리소스 ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; // 플래그 설정
	nid.uCallbackMessage = WM_TRAYICON_MSG; // 콜백메시지 설정
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // 아이콘 로드 

	char strTitle[256]="Operation1";
	//GetWindowText(strTitle, sizeof(strTitle)); // 캡션바에 출력된 문자열 얻음
	lstrcpy(nid.szTip, strTitle); 
	Shell_NotifyIcon(NIM_ADD, &nid);
	SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)nid.hIcon);
	m_bIsTrayIcon = TRUE;

}

void CStartUpDlg::TrayIconMsg(WPARAM wParam, LPARAM lParam)
{
	if(lParam == WM_LBUTTONDBLCLK)
	{  
		ShowWindow(SW_SHOW);
	}
}

void CStartUpDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_bIsTrayIcon) // 현재 트레이 아이콘으로 설정되었는지 확인 
	 {
	  NOTIFYICONDATA  nid;
	  nid.cbSize = sizeof(nid);
	  nid.hWnd = m_hWnd; // 메인 윈도우 핸들
	  nid.uID = IDR_MAINFRAME;

	  // 작업 표시줄(TaskBar)의 상태 영역에 아이콘을 삭제한다.
	  Shell_NotifyIcon(NIM_DELETE, &nid);
	 }	
}

void CStartUpDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnSysCommand(nID, lParam);
}
