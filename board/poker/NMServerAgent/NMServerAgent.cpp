// NMServerAgent.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "NMServerAgent.h"
#include "NMServerAgentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNMServerAgentApp

BEGIN_MESSAGE_MAP(CNMServerAgentApp, CWinApp)
	//{{AFX_MSG_MAP(CNMServerAgentApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNMServerAgentApp construction

CNMServerAgentApp::CNMServerAgentApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNMServerAgentApp object

CNMServerAgentApp theApp;

CSockMan     SockMan; // ���� �޴��� ���� ��ü
CSrvInfoMan  ServerMan;

CString      g_AppStartPath = _T("");

SERVER_INFO_SAVE g_ServerInfoList; // ����� ���� ����

INT64 g_AverJackPot = 0;

//������Ʈ ����
AGENT_ADMIN g_AdminInfo;

CString g_MakeCommaMoney(INT64 nMoney)
{
	CString strVal("0");

	if(nMoney<0) return strVal;
	strVal.Format("%I64d",nMoney);

	for(int i=18; i>0; i-=3){
		if(strVal.GetLength()>i){
			strVal.Insert(strVal.GetLength()-i,",");
		}
	}
	if(strVal=="") strVal= "0";
	return strVal;
}


/////////////////////////////////////////////////////////////////////////////
// CNMServerAgentApp initialization

BOOL CNMServerAgentApp::InitInstance()
{
	AfxEnableControlContainer();
	AfxInitRichEdit();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SaveCurrentDir(); // ���� ���丮�� �����Ѵ�

	CString strReportDir;
	strReportDir.Format("%s\\LOGDATA",g_AppStartPath);
	if(_chdir(strReportDir.operator LPCTSTR())!=0){
		_mkdir(strReportDir.operator LPCTSTR());
	}
	SetCurrentDirectory(g_AppStartPath.operator LPCTSTR());

	CNMServerAgentDlg dlg;
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

// ���� ���丮�� �����Ѵ�
void CNMServerAgentApp::SaveCurrentDir()
{
	// ����� Ŀ�ǵ� ���� �м�
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	CString szCmnd;
	szCmnd.Format("%s",::GetCommandLine());

	CString szDir, szArgFile;
	int nStart, nEnd;
	if(szCmnd.GetAt(0) == '\"') // ������丮�� ���������� ������ �ð�� �ֵ���ǥ�� ������
	{
		nStart = szCmnd.Find('\"', 0)+1;
		nEnd = szCmnd.Find('\"', 1);
		if(nEnd == -1) nEnd = szCmnd.GetLength()-1;
		// ����� ���� ����
		if(szCmnd.GetLength() > nEnd+2) szArgFile = szCmnd.Mid(nEnd+2);
	}
	else // ������ ���� ��� �ֵ���ǥ�� ����
	{
		nStart = 0;
		nEnd = szCmnd.Find(' ', 1);
		if(nEnd == -1) nEnd = szCmnd.GetLength()-1;
		// ����� ���� ����
		if(szCmnd.GetLength() > nEnd+1) szArgFile = szCmnd.Mid(nEnd+1);
	}
	szDir = szCmnd.Mid(nStart, nEnd-nStart);

	// ���α׷��� ����� ���丮�� ����
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
