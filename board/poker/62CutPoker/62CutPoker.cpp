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

static HANDLE hImHere;  // ���������� ������ �Ǿ�߸� ��

BOOL C62CutPokerApp::InitInstance()
{
// ���������϶���....
// ###���׸���ƶ�
#ifndef _DEBUG
	// �ߺ� ���� ����
	hImHere=::CreateMutex(NULL, TRUE, _T("WoolalaMutex")); // ���� ���ý��� �����ϸ� �̹� �ٸ� ���α׷��� ��������
	if(ERROR_ALREADY_EXISTS==::GetLastError())
	{       
		HWND hWnd=::FindWindow(_T("#32770 (Dialog)"), m_pszAppName); // ClassName, WindowsName
	//	::SetFocus(hWnd);
	//	SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		::SetForegroundWindow(hWnd);
		::CloseHandle(hImHere);
		AfxMessageBox("�̹� �ٸ� �ݸ��� ���α׷��� �������Դϴ�");
		return FALSE;
	}
#endif

	// ����� Ŀ�ǵ� ���� �м�
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

// ###���׸���ƶ�
	// ���������϶���....
#ifndef _DEBUG
	// ����� ���ڰ� �ϳ��� ������ ���α׷� ����
	if(cmdInfo.m_nShellCommand==0)
	{
		int rtn=AfxMessageBox("�� ���α׷��� �ݸ��� Ȩ����������\n�����ؾ߸� �ùٸ��� �۵��մϴ�.\n���� Ȩ�������� �̵��Ͻðڽ��ϱ�?", MB_YESNO);
		if(rtn==IDYES) ::ShellExecute(NULL, "open", "http://www.netmarble.net/", NULL, NULL, SW_SHOWNORMAL);
		return FALSE;
	}
#endif

	CString szCmnd;
	szCmnd.Format("%s",::GetCommandLine());

	CString szDir, szArg1;
	int nStart = szCmnd.Find('\"', 0)+1;
	int nEnd = szCmnd.Find('\"', 1);
	if(szCmnd.GetAt(0) == '\"') // ������丮�� ���������� ������ �ð�� �ֵ���ǥ�� ������
	{
		nStart = szCmnd.Find('\"', 0)+1;
		nEnd = szCmnd.Find('\"', 1);
		// ����� ���� ����
		//if(szCmnd.GetLength() > nEnd+2) szArg1 = szCmnd.Mid(nEnd+2);
		// ����� ���� ����
		if(szCmnd.GetLength() > nEnd+2) szArg1 = szCmnd.Mid(nEnd+1);
		szArg1.TrimLeft();
	}
	else // ������ ���� ��� �ֵ���ǥ�� ����
	{
		nStart = 0;
		nEnd = szCmnd.Find(' ', 1);
		// ����� ���� ����
		if(szCmnd.GetLength() > nEnd+1) szArg1 = szCmnd.Mid(nEnd+1);
	}
	
	szDir.Format("%s", szCmnd.Mid(nStart, nEnd-nStart));

	// ���α׷��� ����� ���丮�� ����
	for(int i=szDir.GetLength(); i>0; i--){
		if(szDir.GetAt(i-1) == '\\')
		{
			szDir = szDir.Mid(0, i-1);
			break;
		}
	}

//###���׸���ƶ�
// ���������϶���....
#ifndef _DEBUG
	// ���ڸ� �м��ؼ� ���� ���� ȯ���� �ʱ�ȭ
	if(g_pMainDlg->AnalyzeArgument((char*)szArg1.operator LPCTSTR())==FALSE)
	{
		AfxMessageBox("���α׷��� �ʿ��� ȯ���� �ʱ�ȭ�� �� �����ϴ�"); 
		return FALSE;
	}
	
	if(g_AuthCookie.GetLength() == 0 || g_DataCookie.GetLength() == 0) {
		AfxMessageBox("������ ���� �ʾҽ��ϴ�. �α��� �Ͻñ� �ٶ��ϴ�."); 
		return FALSE;
	}

	// ���� ���丮�� ������ ����� ������ �����Ѵ�
	SetCurrentDirectory(szDir);
#endif
	
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxInitRichEdit();	// Rich Edit Contol�� ���̾�α� ���ø��� �����Ϸ��� �ʿ���


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
