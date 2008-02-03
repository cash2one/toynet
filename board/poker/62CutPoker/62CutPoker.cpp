// 62CutPoker.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "62CutPokerDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerApp

BEGIN_MESSAGE_MAP(C62CutPokerApp, CWinApp)
	//{{AFX_MSG_MAP(C62CutPokerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerApp construction

C62CutPokerApp::C62CutPokerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only C62CutPokerApp object

C62CutPokerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerApp initialization

static HANDLE hImHere;  // 전역변수로 선언이 되어야만 함

BOOL C62CutPokerApp::InitInstance()
{
// 릴리즈모드일때만....
// ###버그를잡아라
#ifndef _DEBUG
	// 중복 실행 방지
	hImHere=::CreateMutex(NULL, TRUE, _T("WoolalaMutex")); // 같은 뮤택스가 존재하면 이미 다른 프로그램이 실행중임
	if(ERROR_ALREADY_EXISTS==::GetLastError())
	{       
		HWND hWnd=::FindWindow(_T("#32770 (Dialog)"), m_pszAppName); // ClassName, WindowsName
	//	::SetFocus(hWnd);
	//	SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		::SetForegroundWindow(hWnd);
		::CloseHandle(hImHere);
		AfxMessageBox("이미 다른 넷마블 프로그램이 실행중입니다");
		return FALSE;
	}
#endif

	// 명령행 커맨드 정보 분석
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

// ###버그를잡아라
	// 릴리즈모드일때만....
#ifndef _DEBUG
	// 명령행 인자가 하나도 없으면 프로그램 종료
	if(cmdInfo.m_nShellCommand==0)
	{
		int rtn=AfxMessageBox("이 프로그램은 넷마블 홈페이지에서\n실행해야만 올바르게 작동합니다.\n지금 홈페이지로 이동하시겠습니까?", MB_YESNO);
		if(rtn==IDYES) ::ShellExecute(NULL, "open", "http://www.netmarble.net/", NULL, NULL, SW_SHOWNORMAL);
		return FALSE;
	}
#endif

	CString szCmnd;
	szCmnd.Format("%s",::GetCommandLine());

	CString szDir, szArg1;
	int nStart = szCmnd.Find('\"', 0)+1;
	int nEnd = szCmnd.Find('\"', 1);
	if(szCmnd.GetAt(0) == '\"') // 실행디렉토리및 파일정보에 공백이 올경우 쌍따옴표로 감싸짐
	{
		nStart = szCmnd.Find('\"', 0)+1;
		nEnd = szCmnd.Find('\"', 1);
		// 명령행 인자 추출
		//if(szCmnd.GetLength() > nEnd+2) szArg1 = szCmnd.Mid(nEnd+2);
		// 명령행 인자 추출
		if(szCmnd.GetLength() > nEnd+2) szArg1 = szCmnd.Mid(nEnd+1);
		szArg1.TrimLeft();
	}
	else // 공백이 없는 경우 쌍따옴표도 없음
	{
		nStart = 0;
		nEnd = szCmnd.Find(' ', 1);
		// 명령행 인자 추출
		if(szCmnd.GetLength() > nEnd+1) szArg1 = szCmnd.Mid(nEnd+1);
	}
	
	szDir.Format("%s", szCmnd.Mid(nStart, nEnd-nStart));

	// 프로그램이 실행된 디렉토리명만 추출
	for(int i=szDir.GetLength(); i>0; i--){
		if(szDir.GetAt(i-1) == '\\')
		{
			szDir = szDir.Mid(0, i-1);
			break;
		}
	}

//###버그를잡아라
// 릴리즈모드일때만....
#ifndef _DEBUG
	// 인자를 분석해서 게임 접속 환경을 초기화
	if(g_pMainDlg->AnalyzeArgument((char*)szArg1.operator LPCTSTR())==FALSE)
	{
		AfxMessageBox("프로그램에 필요한 환경을 초기화할 수 없습니다"); 
		return FALSE;
	}
	
	if(g_AuthCookie.GetLength() == 0 || g_DataCookie.GetLength() == 0) {
		AfxMessageBox("인증이 되지 않았습니다. 로그인 하시기 바랍니다."); 
		return FALSE;
	}

	// 현재 디렉토리를 게임이 실행된 곳으로 설정한다
	SetCurrentDirectory(szDir);
#endif
	
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxInitRichEdit();	// Rich Edit Contol을 다이얼로그 템플릿에 삽입하려면 필요함


/////////////////////////////////////////////////////////////////////////////////////////	
	char strDir[500]={0,};
	GetCurrentDirectory(500, strDir);

	switch(g_MSCode){
	case 0:  // Korean
		{
			strcat(strDir, "\\Data\\NewPoker.str");
			if( !g_StrMan.Open(strDir) ){
				AfxMessageBox(_T("Can't find [NewPoker.str] file!")); return FALSE;
			}
		} break;
	case 1:  // English
		{
			//strDir = _T("\\Data_ENG\\"); 
			strcat(strDir, "\\Data\\SPoker_ENG.str");
			if( !g_StrMan.Open(strDir) ){
				AfxMessageBox(_T("Can't find [NewPoker_ENG.str] file!")); return FALSE;
			}
		} break;
	case 2:  // Japanese
		{
			//strDir = _T("\\Data_JPN\\"); 
			strcat(strDir, "\\Data\\SPoker_JPN.str");
			if( !g_StrMan.Open(strDir) ){
				AfxMessageBox(_T("Can't find [NewPoker_JPN.str] file!")); return FALSE;
			}
		} break;
	case 3:  // Chinese
		{
			//strDir = _T("\\Data_CHN\\"); 
			strcat(strDir, "\\Data\\SPoker_CHN.str");
			if( !g_StrMan.Open(strDir) ){
				AfxMessageBox(_T("Can't find [NewPoker_CHN.str] file!")); return FALSE;
			}
		} break;
	default: // Korean
		{
			//strDir = _T("\\Data\\"); 
			strcat(strDir, "\\Data\\NewPoker.str");
			if( !g_StrMan.Open(strDir) ){
				AfxMessageBox(_T("Can't find [NewPoker.str] file!")); return FALSE;
			}
		} break;

	}
////////////////////////////////////////////////////////////////////////////////////////////



	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	C62CutPokerDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
