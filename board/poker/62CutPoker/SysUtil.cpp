// SysUtil.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysUtil.h"
#include "Graphic.h"
#include <mmsystem.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib,"winmm.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


BOOL GetProfile_DefPath(char* strPath, int nBufLen)
{
	// ���� ���ϸ��� �˾Ƴ���
	char origin[MAX_PATH];

	// GetModuleHandle(NULL) == AfxGetInstanceHandle()
	if(GetModuleFileName(GetModuleHandle(NULL), origin, MAX_PATH-1) == 0) {	
		return FALSE;
	}

	// ���� ���ϸ��� �������� �и��Ѵ�
	strPath[0] = '\0';
	int slen = strlen(origin);
	for(int i=slen-1; i>0; i--) {
		if(origin[i] == '\\') {
			if(nBufLen < i+1) return 0;
			strncpy(strPath, origin, i);
			strPath[i] = '\0';
			return TRUE;
		}
	}

	return FALSE;
}

int GetProfile_int(LPCTSTR strSection, LPCTSTR strKeyname, int nDefault)
{
	// ���� ���ϸ��� �˾Ƴ���
	char path[MAX_PATH], fname[MAX_PATH];
	if(!GetProfile_DefPath(path, MAX_PATH-1))
		return nDefault;

	wsprintf(fname, "%s\\config\\config.ini", path);

	const int buflen = 1024;
	char strdef[buflen], strrtn[buflen];
	wsprintf(strdef, "%d", nDefault);

	if(GetPrivateProfileString(strSection, strKeyname, strdef, strrtn, buflen-1, fname) == 0) {
		return nDefault;
	}

	return atoi(strrtn);
}

float GetProfile_float(LPCTSTR strSection, LPCTSTR strKeyname, float fDefault)
{
	// ���� ���ϸ��� �˾Ƴ���
	char path[MAX_PATH], fname[MAX_PATH];
	if(!GetProfile_DefPath(path, MAX_PATH-1))
		return fDefault;

	wsprintf(fname, "%s\\config\\config.ini", path);

	const int buflen = 1024;
	char strdef[buflen], strrtn[buflen];
	wsprintf(strdef, "%f", fDefault);

	if(GetPrivateProfileString(strSection, strKeyname, strdef, strrtn, buflen-1, fname) == 0) {
		return fDefault;
	}

	return (float)atof(strrtn);
}

char* GetProfile_string(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault)
{
	// ���� ���ϸ��� �˾Ƴ���
	char path[MAX_PATH], fname[MAX_PATH];
	if(!GetProfile_DefPath(path, MAX_PATH-1))
		return (char*)strDefault;

	wsprintf(fname, "%s\\config\\config.ini", path);

	const int buflen = 1024;
	static char strrtn[buflen];
	strrtn[0] = 0;

	if(GetPrivateProfileString(strSection, strKeyname, strDefault, strrtn, buflen-1, fname) == 0) {
		return (char*)strDefault;
	}

	return strrtn;
}

BOOL WriteProfile_int(LPCTSTR strSection, LPCTSTR strKeyname, int nValue)
{
	// ���� ���ϸ��� �˾Ƴ���
	char path[MAX_PATH], fname[MAX_PATH];
	if(!GetProfile_DefPath(path, MAX_PATH-1))
		return FALSE;

	wsprintf(fname, "%s\\config\\config.ini", path);

	const int buflen = 1024;
	char strval[buflen];
	wsprintf(strval, "%d", nValue);

	return WritePrivateProfileString(strSection, strKeyname, strval, fname);
}

BOOL WriteProfile_float(LPCTSTR strSection, LPCTSTR strKeyname, float fValue)
{
	// ���� ���ϸ��� �˾Ƴ���
	char path[MAX_PATH], fname[MAX_PATH];
	if(!GetProfile_DefPath(path, MAX_PATH-1))
		return FALSE;

	wsprintf(fname, "%s\\config\\config.ini", path);

	const int buflen = 1024;
	char strval[buflen];
	wsprintf(strval, "%f", fValue);

	return WritePrivateProfileString(strSection, strKeyname, strval, fname);
}

BOOL WriteProfile_string(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strValue)
{
	// ���� ���ϸ��� �˾Ƴ���
	char path[MAX_PATH], fname[MAX_PATH];
	if(!GetProfile_DefPath(path, MAX_PATH-1))
		return FALSE;

	wsprintf(fname, "%s\\config\\config.ini", path);

	return WritePrivateProfileString(strSection, strKeyname, strValue, fname);
}

//========================================================================================

#define  MSG_NEHOMPY 13
#define NMGAME_REQUEST	30
#define HIDDEN_PROCESS_NAME ("asldkjsadlkajqwekhc120312093lqqndlqadlqwkjeqwlekjqeqeqwleu123u13-103")
#define CHECK_PASS ("12o3ijhlkjdfalojh139123ujlnaskdhaleu129-3j3-u52i-1jn.,andlkasuyd01e13")

typedef struct
{
	HWND TargetHwnd;		// ���μ��� ��� ��, �Ѿ�� �ڵ�
	HWND AtlhWnd;			// ��(activeX) �ڵ�
	int Signal;				// �޽���
	int bFlag;				// �÷���
	BOOL isMsgAlive;		// �޽����� ����ִ°�?
	BOOL isAtlAlive;		// ����� �ִ� ���� �ִ°�.?
	char ServIP[20];		// ������ IRC ���� ������
	UINT ServPort;			// ������ IRC ��Ʈ
	char UserID[13];		// �������̵�(����ũ �ѹ���)
	char CheckPass[100];	// ���μ��� ��Ű� ���� �н�����
	char TargetURL[100];	// �̵��� URL
	char Msg[512];			// IRC���� ���ƿ��� �޽���
} IrcNotify;


// ��Ȩ�Ǹ� �����ִ� �Լ�
BOOL OpenNehompy(LPCTSTR szMyUniqNo, LPCTSTR szTargetID) 
{
	if(strlen(szMyUniqNo) == 0 || strlen(szTargetID) == 0)
		return FALSE;

	CString str;
	IrcNotify notify = {0,};

	notify.Signal = MSG_NEHOMPY;
	strcpy(notify.UserID, szMyUniqNo);
	str.Format("CmdXFunc(\"1\", \"%s\")", szTargetID);
	strcpy(notify.TargetURL, str);
	strcpy(notify.CheckPass, CHECK_PASS);

	COPYDATASTRUCT cds = {0,};

	cds.dwData = 0;
	cds.cbData = sizeof(notify);
	cds.lpData = &notify;

	CWnd *pMainWnd = AfxGetMainWnd();
	if(!pMainWnd)
		return FALSE;

	CWnd *pWnd = CWnd::FindWindow(NULL, (LPSTR)(LPCSTR)HIDDEN_PROCESS_NAME);
	if(!pWnd)
		return FALSE;

	pWnd->SendMessage(WM_COPYDATA, (WPARAM)pMainWnd->GetSafeHwnd(), (LPARAM)&cds);

	// ���α׷��� �ּ�ȭ �Ѵ�
	pMainWnd->ShowWindow(SW_MINIMIZE);

	return TRUE;
}

// �ݸ��� ������������ Ư�� URL�� ã�Ƽ� �����ִ� �Լ�
BOOL OpenNetmarbleWebpage(LPCTSTR szMyUniqNo, LPCTSTR szTargetURL) 
{
	if(strlen(szMyUniqNo) == 0 || strlen(szTargetURL) == 0)
		return FALSE;

	CString str;
	IrcNotify notify = {0,};

	notify.Signal = NMGAME_REQUEST;
	strcpy(notify.UserID, szMyUniqNo);		// �� ����ũ �ѹ�
	strcpy(notify.TargetURL, szTargetURL);	// �̵��� URL
	strcpy(notify.CheckPass, CHECK_PASS);

	COPYDATASTRUCT cds = {0,};

	cds.dwData = 0;
	cds.cbData = sizeof(notify);
	cds.lpData = &notify;

	CWnd *pMainWnd = AfxGetMainWnd();
	if(!pMainWnd)
		return FALSE;

	CWnd *pWnd = CWnd::FindWindow(NULL, (LPSTR)(LPCSTR)HIDDEN_PROCESS_NAME);
	if(!pWnd)
		return FALSE;

	pWnd->SendMessage(WM_COPYDATA, (WPARAM)pMainWnd->GetSafeHwnd(), (LPARAM)&cds);

	// ���α׷��� �ּ�ȭ �Ѵ�
	pMainWnd->ShowWindow(SW_MINIMIZE);

	return TRUE;
}

//===========================================================================================

BOOL ScreenCapture(HWND hwnd, LPCTSTR fname)
{
	if(!hwnd) 
		return FALSE;

	// �������� ĸ���� �ð��� ����Ͽ� �ʹ� ���� ĸ�縦 ���´�
	static DWORD dwLastCapture = timeGetTime()-100;
	DWORD dwNowTick = timeGetTime();
	if(dwNowTick - dwLastCapture < 50) 
		return FALSE;

	CRect rc;
	GetWindowRect(hwnd, rc);

	if(rc.Width()==0 || rc.Height()==0)
		return FALSE;

	CPage page;
	page.Init(rc.Width(), rc.Height(), 16);

	// ��ũ�� DC�� �о�´�
	HDC hdc = ::GetDC(NULL); 
	if(!hdc) 
		return FALSE;

	HDC hdcMem = ::CreateCompatibleDC(hdc);
	HGDIOBJ hOldBmp = ::SelectObject(hdcMem, page.hBmp);
	::BitBlt(hdcMem, 0, 0, rc.Width(), rc.Height(), hdc, rc.left, rc.top, SRCCOPY);

	CTime time = CTime::GetCurrentTime();
	CString fullpath;

	// ������ ���� ���
	char lpszPath[MAX_PATH] = {0,};
	SHGetSpecialFolderPath(hwnd, lpszPath, CSIDL_PERSONAL, FALSE);
	fullpath = lpszPath;
	fullpath += "\\";
	fullpath += fname;

	// ��ũ�� ĸ��� �̹����� JPG�� ����
	if(!page.SaveToJpeg((char*)(LPCTSTR)fullpath, 88))
		return FALSE;

	::SelectObject(hdcMem, hOldBmp);
	::ReleaseDC(NULL, hdc);
	::DeleteDC(hdcMem);

	dwLastCapture = timeGetTime();

	return TRUE;
}

//===========================================================================================

BOOL WindowResize2Bitmap(HWND hwnd, HGDIOBJ hbmp)
{
	if(!hbmp || !hwnd)
		return FALSE;

	BITMAP bm = {0,};
	GetObject(hbmp, sizeof(bm), &bm);

	if(bm.bmWidth==0 || bm.bmHeight==0)
		return FALSE;

	// �������� ��ġ�� Z������ �����ϰ� ����� �����Ų��
	::SetWindowPos(hwnd, NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE|SWP_NOZORDER);

	return TRUE;
}


BOOL WindowResize2Bitmap(HWND hwnd, LPCTSTR bmpfilename)
{
	if(!bmpfilename || !hwnd)
		return FALSE;

	CBitmap bmp;

	HBITMAP hBitmap = NULL; 
	hBitmap = (HBITMAP)LoadImage(NULL, bmpfilename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
	if(!bmp.Attach(hBitmap))
		return FALSE;

	return WindowResize2Bitmap(hwnd, bmp.GetSafeHandle());
}


//===========================================================================================

CMsgServiceControl::CMsgServiceControl()
{
	m_bIsStart = FALSE;
	m_SCMHandle = NULL;
}

CMsgServiceControl::~CMsgServiceControl()
{
	if(m_Inited) StopService(1-m_bIsStart);
	CloseServiceHandle(m_SCMHandle);
}

BOOL CMsgServiceControl::InitService(LPCTSTR lpServiceName)
{
	m_SCMHandle = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);	// COM/DCOM Service Manager Handle Init

	m_Inited = FALSE;
	if(m_SCMHandle) {
		SC_HANDLE m_myservice = OpenService(m_SCMHandle,lpServiceName,SERVICE_ALL_ACCESS);
		if(m_myservice)
		{
			QueryServiceStatus(m_myservice,&m_serviceStatus);
			m_Servicename = lpServiceName;

			if(m_serviceStatus.dwCurrentState != SERVICE_STOPPED && m_serviceStatus.dwCurrentState != SERVICE_STOP_PENDING
				&& m_serviceStatus.dwCurrentState != SERVICE_PAUSED && m_serviceStatus.dwCurrentState != SERVICE_PAUSE_PENDING)
				m_bIsStart = TRUE;

			CloseServiceHandle(m_myservice);
			m_Inited = TRUE;
			StopService(1);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMsgServiceControl::StopService(BOOL bStop)
{
	if(!m_Inited) return TRUE;
	SC_HANDLE  ServiceHandle;
	ServiceHandle = OpenService(m_SCMHandle,m_Servicename,SERVICE_ALL_ACCESS);
	if(ServiceHandle)
	{
		QueryServiceStatus(ServiceHandle,&m_serviceStatus);
		switch(bStop)
		{
			case 0:
				{
					if(m_serviceStatus.dwCurrentState == SERVICE_STOPPED)
					{
						BOOL tf = StartService(ServiceHandle,0,NULL);
						QueryServiceStatus(ServiceHandle,&m_serviceStatus);
						CloseServiceHandle(ServiceHandle);
						return tf;
					} else
					if(m_serviceStatus.dwCurrentState == SERVICE_PAUSED)
					{
						BOOL tf =ControlService(ServiceHandle,SERVICE_CONTROL_CONTINUE,&m_serviceStatus);
						return tf;
					}
				}
				break;
			case 1:
				{
					if(m_serviceStatus.dwCurrentState != SERVICE_STOPPED && m_serviceStatus.dwCurrentState != SERVICE_STOP_PENDING)
					{
						ControlService(ServiceHandle,SERVICE_CONTROL_STOP,&m_serviceStatus);
					}
					QueryServiceStatus(ServiceHandle,&m_serviceStatus);
					CloseServiceHandle(ServiceHandle);
					if(m_serviceStatus.dwCurrentState == SERVICE_STOPPED) return TRUE;

				}
				break;
		}
	}
	return FALSE;
}

//===================================================================


