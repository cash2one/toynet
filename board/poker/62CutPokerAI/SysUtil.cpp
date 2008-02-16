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
	// 실행 파일명을 알아낸다
	char origin[MAX_PATH];

	// GetModuleHandle(NULL) == AfxGetInstanceHandle()
	if(GetModuleFileName(GetModuleHandle(NULL), origin, MAX_PATH-1) == 0) {	
		return FALSE;
	}

	// 실행 파일명에서 폴더명을 분리한다
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
	// 실행 파일명을 알아낸다
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
	// 실행 파일명을 알아낸다
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
	// 실행 파일명을 알아낸다
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
	// 실행 파일명을 알아낸다
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
	// 실행 파일명을 알아낸다
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
	// 실행 파일명을 알아낸다
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
	HWND TargetHwnd;		// 프로세스 통신 때, 넘어온 핸들
	HWND AtlhWnd;			// 웹(activeX) 핸들
	int Signal;				// 메시지
	int bFlag;				// 플래그
	BOOL isMsgAlive;		// 메신저가 살아있는가?
	BOOL isAtlAlive;		// 띄워져 있는 웹이 있는가.?
	char ServIP[20];		// 접속할 IRC 서버 아이피
	UINT ServPort;			// 접속할 IRC 포트
	char UserID[13];		// 유저아이디(유니크 넘버임)
	char CheckPass[100];	// 프로세스 통신간 비교할 패스워드
	char TargetURL[100];	// 이동할 URL
	char Msg[512];			// IRC에서 날아오는 메시지
} IrcNotify;


// 내홈피를 열어주는 함수
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

	// 프로그램을 최소화 한다
	pMainWnd->ShowWindow(SW_MINIMIZE);

	return TRUE;
}

// 넷마블 웹페이지에서 특정 URL을 찾아서 열어주는 함수
BOOL OpenNetmarbleWebpage(LPCTSTR szMyUniqNo, LPCTSTR szTargetURL) 
{
	if(strlen(szMyUniqNo) == 0 || strlen(szTargetURL) == 0)
		return FALSE;

	CString str;
	IrcNotify notify = {0,};

	notify.Signal = NMGAME_REQUEST;
	strcpy(notify.UserID, szMyUniqNo);		// 내 유니크 넘버
	strcpy(notify.TargetURL, szTargetURL);	// 이동할 URL
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

	// 프로그램을 최소화 한다
	pMainWnd->ShowWindow(SW_MINIMIZE);

	return TRUE;
}

//===========================================================================================

BOOL ScreenCapture(HWND hwnd, LPCTSTR fname)
{
	if(!hwnd) 
		return FALSE;

	// 마지막에 캡춰한 시간을 기록하여 너무 잦은 캡춰를 막는다
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

	// 스크린 DC를 읽어온다
	HDC hdc = ::GetDC(NULL); 
	if(!hdc) 
		return FALSE;

	HDC hdcMem = ::CreateCompatibleDC(hdc);
	HGDIOBJ hOldBmp = ::SelectObject(hdcMem, page.hBmp);
	::BitBlt(hdcMem, 0, 0, rc.Width(), rc.Height(), hdc, rc.left, rc.top, SRCCOPY);

	CTime time = CTime::GetCurrentTime();
	CString fullpath;

	// 내문서 폴더 얻기
	char lpszPath[MAX_PATH] = {0,};
	SHGetSpecialFolderPath(hwnd, lpszPath, CSIDL_PERSONAL, FALSE);
	fullpath = lpszPath;
	fullpath += "\\";
	fullpath += fname;

	// 스크린 캡춰된 이미지를 JPG로 저장
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

	// 윈도우의 위치와 Z오더는 무시하고 사이즈만 변경시킨다
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


