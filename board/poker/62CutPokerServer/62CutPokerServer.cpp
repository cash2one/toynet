// 62CutPokerServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "62CutPokerServer.h"

#include "MainFrm.h"
#include "62CutPokerServerDoc.h"
#include "62CutPokerServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerApp

BEGIN_MESSAGE_MAP(C62CutPokerServerApp, CWinApp)
	//{{AFX_MSG_MAP(C62CutPokerServerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerApp construction

C62CutPokerServerApp::C62CutPokerServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only C62CutPokerServerApp object

C62CutPokerServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerApp initialization

BOOL C62CutPokerServerApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)



	// 문자열 파일 열기
	switch(0){
	case 0: // Korea
		if( g_StrMan.Open("SPSRV.STR")==FALSE ){
			AfxMessageBox("Can't find [SPSRV.STR] file!"); return FALSE;
		}
		break;
	case 1: // English
		if( g_StrMan.Open("SPSRV_ENG.STR")==FALSE ){
			AfxMessageBox("Can't find [SPSRV_ENG.STR] file!"); return FALSE;
		}
		break;
	case 2: // Japan
		if( g_StrMan.Open("SPSRV_JPN.STR")==FALSE ){
			AfxMessageBox("Can't find [SPSRV_JPN.STR] file!"); return FALSE;
		}
		break;
	case 3: // Chinese
		if( g_StrMan.Open("SPSRV_CHN.STR")==FALSE ){
			AfxMessageBox("Can't find [SPSRV_CHN.STR] file!"); return FALSE;
		}
		break;
	default:
		if( g_StrMan.Open("SPSRV.STR")==FALSE ){
			AfxMessageBox("Can't find [SPSRV.STR] file!"); return FALSE;
		}
		break;
	}
/*
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	SaveCurrentDir(); // 현재 디렉토리를 저장한다

	CString strReportDir;
	strReportDir.Format("%s\\LOGDATA",g_AppStartPath);
	if(_chdir(strReportDir.operator LPCTSTR())!=0){
		_mkdir(strReportDir.operator LPCTSTR());
	}
	SetCurrentDirectory(g_AppStartPath.operator LPCTSTR());
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
*/

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(C62CutPokerServerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(C62CutPokerServerView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	// 아래를 주석처리한다 [PC방 작업]
	//ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/*
// 현재 디렉토리를 저장한다
void C62CutPokerServerApp::SaveCurrentDir()
{
	// 명령행 커맨드 정보 분석
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	CString szCmnd;
	szCmnd.Format("%s",::GetCommandLine());

	CString szDir, szArgFile;
	int nStart, nEnd;
	if(szCmnd.GetAt(0) == '\"') // 실행디렉토리및 파일정보에 공백이 올경우 쌍따옴표로 감싸짐
	{
		nStart = szCmnd.Find('\"', 0)+1;
		nEnd = szCmnd.Find('\"', 1);
		if(nEnd == -1) nEnd = szCmnd.GetLength()-1;
		// 명령행 인자 추출
		if(szCmnd.GetLength() > nEnd+2) szArgFile = szCmnd.Mid(nEnd+2);
	}
	else // 공백이 없는 경우 쌍따옴표도 없음
	{
		nStart = 0;
		nEnd = szCmnd.Find(' ', 1);
		if(nEnd == -1) nEnd = szCmnd.GetLength()-1;
		// 명령행 인자 추출
		if(szCmnd.GetLength() > nEnd+1) szArgFile = szCmnd.Mid(nEnd+1);
	}
	szDir = szCmnd.Mid(nStart, nEnd-nStart);

	// 프로그램이 실행된 디렉토리명만 추출
	for(int i=szDir.GetLength(); i>0; i--){
		if(szDir.GetAt(i-1) == '\\')
		{
			szDir = szDir.Mid(0, i-1);
			break;
		}
	}
//#ifndef _KHSTEST
//	SetCurrentDirectory(szDir);
//	g_AppStartPath = szDir;
//#else
	char strDir[500]={0,};
	GetCurrentDirectory(500, strDir);
	g_AppStartPath = strDir;
//#endif //_KHSTEST

}
*/
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
		// No message handlers
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

// App command to run the dialog
void C62CutPokerServerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerApp message handlers

