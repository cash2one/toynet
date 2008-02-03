// MciMedia.cpp: implementation of the CMciMedia class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MciMedia.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define MCIMEDIA_SUCCESS   (0) // MCI 함수리턴값중 성공시 0을 리턴하는 경우가 많다

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMciMedia::CMciMedia()
{
	m_nDeviceID = 0;
	m_hMsgWnd = 0;
	m_bLoop = FALSE;
	m_dwErrCode = 0;
}

CMciMedia::~CMciMedia()
{
	Close();
	DestroyMsgWindow();
}

//////////////////////////////////////////////////////////////////////
// 미디어 함수
//////////////////////////////////////////////////////////////////////

// 주어진 파일이름으로 미디어를 오픈한다
BOOL CMciMedia::Open(LPCTSTR lpszFileName)
{
	MCI_OPEN_PARMS mciOpenParms;
	MCI_SET_PARMS  mciSetParms;

	memset(&mciOpenParms,0,sizeof(MCI_OPEN_PARMS));
	memset(&mciSetParms, 0,sizeof(MCI_SET_PARMS));

	// 이미 미디어가 오픈되어 있다면 닫는다
	if (GetStatus(MCI_STATUS_MODE) != MCI_MODE_NOT_READY) 
		Close();
	
	mciOpenParms.lpstrDeviceType  = NULL;
	mciOpenParms.lpstrElementName = lpszFileName;
	
	DWORD dwResult = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParms);
	
	if (dwResult == MCIMEDIA_SUCCESS) {
		
		m_nDeviceID = mciOpenParms.wDeviceID; // 정상적으로 오픈되었으면 장치ID 발급
		
		mciSetParms.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
		mciSendCommand( m_nDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID)&mciSetParms);
	}
	
	m_dwErrCode = dwResult;
	return (dwResult == MCIMEDIA_SUCCESS);
}

// 미디어를 닫는다
void CMciMedia::Close()
{
	if (m_nDeviceID != 0){
		mciSendCommand(m_nDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
		m_nDeviceID = 0;
	}
	DestroyMsgWindow();
}

// 에러 메시지 문자열을 구한다
BOOL CMciMedia::GetErrorString(DWORD dwErrCode, LPCTSTR lpszError, UINT nSize)
{
	return mciGetErrorString(dwErrCode, (LPTSTR)lpszError, nSize);
}

//////////////////////////////////////////////////////////////////////
// 미디어 연주 함수
//////////////////////////////////////////////////////////////////////

// 지정된 위치에서 미디어를 연주한다
BOOL CMciMedia::Play(DWORD dwStartPosition /*=0*/, BOOL bLoop /*=FALSE*/)
{
	if(!m_hMsgWnd)
		CreateMsgWindow();

	m_bLoop = bLoop;

	DWORD  dwResult = 0, dwStatus = 0;
	dwStatus = GetStatus(MCI_STATUS_MODE);
	
	if (dwStatus != MCI_MODE_NOT_READY) {
		MCI_PLAY_PARMS mciPlayParms;
		memset(&mciPlayParms, 0, sizeof(MCI_PLAY_PARMS));

		mciPlayParms.dwFrom     = dwStartPosition;
		mciPlayParms.dwCallback = (DWORD)m_hMsgWnd;

		dwResult = mciSendCommand(m_nDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD)(LPVOID)&mciPlayParms);
		
		if (dwResult != MCIMEDIA_SUCCESS) Close();
	}

	m_dwErrCode = dwResult;
	return (dwResult == MCIMEDIA_SUCCESS);
}

// 일시 정지
BOOL CMciMedia::Pause()
{
	MCI_GENERIC_PARMS mciGenericParms;
	DWORD dwResult = mciSendCommand(m_nDeviceID, MCI_PAUSE, MCI_WAIT,(DWORD)(LPVOID)&mciGenericParms);
	m_dwErrCode = dwResult;
	return (dwResult == MCIMEDIA_SUCCESS);
}

// 리줌
BOOL CMciMedia::Resume()
{
	MCI_GENERIC_PARMS mciGenericParms;
	DWORD dwResult = mciSendCommand(m_nDeviceID, MCI_RESUME, MCI_WAIT,(DWORD)(LPVOID)&mciGenericParms);
	m_dwErrCode = dwResult;
	return (dwResult == MCIMEDIA_SUCCESS);
}

// 멈춤
BOOL CMciMedia::Stop()
{
	MCI_GENERIC_PARMS mciGenericParms;
	DWORD dwResult = mciSendCommand(m_nDeviceID, MCI_STOP, 0,(DWORD)(LPVOID)&mciGenericParms);
	m_dwErrCode = dwResult;
	return (dwResult == MCIMEDIA_SUCCESS);
}

//////////////////////////////////////////////////////////////////////
// 미디어 상태 함수
//////////////////////////////////////////////////////////////////////

// 장치의 상태를 구한다
DWORD CMciMedia::GetStatus(DWORD dwFlag)
{
	MCI_STATUS_PARMS mciStatusParms;
	memset(&mciStatusParms, 0, sizeof(MCI_STATUS_PARMS));
	mciStatusParms.dwItem = dwFlag;
	mciSendCommand(m_nDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms);
	return mciStatusParms.dwReturn;
}

BOOL CMciMedia::IsReady()
{
	if(GetStatus(MCI_STATUS_MODE) != MCI_MODE_NOT_READY) return TRUE;
	return FALSE;
}

BOOL CMciMedia::IsPause()
{
	if(GetStatus(MCI_STATUS_MODE) == MCI_MODE_PAUSE) return TRUE;
	return FALSE;
}

BOOL CMciMedia::IsPlay()
{
	if(GetStatus(MCI_STATUS_MODE) == MCI_MODE_PLAY) return TRUE;
	return FALSE;
}

BOOL CMciMedia::IsStop()
{
	if(GetStatus(MCI_STATUS_MODE) == MCI_MODE_STOP) return TRUE;
	return FALSE;
}

BOOL CMciMedia::IsRecord()
{
	if(GetStatus(MCI_STATUS_MODE) == MCI_MODE_RECORD) return TRUE;
	return FALSE;
}

// 전체 길이를 밀리초단위로 구한다
DWORD CMciMedia::GetTotalLength()
{
	if(m_nDeviceID==0) return 0;
	return GetStatus(MCI_STATUS_LENGTH);
}

// 전체 길이를 밀리초단위로 구한다
DWORD CMciMedia::GetTotalPosition()
{
	return GetTotalLength();
}

// 재생 위치를 밀리초단위로 반환
DWORD CMciMedia::GetPosition()
{
	if(m_nDeviceID==0) return 0;
	return GetStatus(MCI_STATUS_POSITION);
}

//////////////////////////////////////////////////////////////////////
// 미디어 볼륨 함수
//////////////////////////////////////////////////////////////////////

DWORD CMciMedia::GetVolume()
{
	DWORD    dwVolume, dwReturn;
	MMRESULT mmsError;
	
	mmsError = waveOutGetVolume(0, (LPDWORD)&dwVolume);
	dwReturn = LOWORD(dwVolume);
	
	return dwReturn;
}

void CMciMedia::SetVolume(DWORD dwVolume)
{
	MMRESULT mmsError;
	long     lLeftVol, lRightVol;
	
	lLeftVol  = dwVolume;
	lRightVol = dwVolume;
	dwVolume  = (dwVolume << 16) + lRightVol;
	
	mmsError = waveOutSetVolume(0, dwVolume);
}

//////////////////////////////////////////////////////////////////////
// MCI 메시지 처리 윈도우 
//////////////////////////////////////////////////////////////////////

BOOL CMciMedia::CreateMsgWindow()
{
	if(m_hMsgWnd) return TRUE;

	//HWND hwndParent = hwndparent;
	//HINSTANCE hInst = (HINSTANCE)::GetWindowLong(hwndParent, GWL_HINSTANCE);
	HINSTANCE hInst = (HINSTANCE)::GetModuleHandle(NULL);
	
	WNDCLASSEX wndclass;

	wndclass.cbSize        = sizeof(wndclass);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = MsgWndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = sizeof(LONG);
	wndclass.hInstance     = hInst;
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = NULL;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = "MciMsgWindow";
	wndclass.hIconSm       = NULL;

	RegisterClassEx(&wndclass);
	
	m_hMsgWnd = CreateWindowEx(
						WS_EX_CLIENTEDGE,
						"MciMsgWindow",		// window class name
						"MciMsgWindow",		// window caption
						WS_OVERLAPPEDWINDOW,
						0,
						0,
						0,
						0,
						NULL, //hwndParent,	// parent window handle
						NULL,				// window menu handle
						hInst,				// program instance handle
						NULL);				// creation parameters

	if(!m_hMsgWnd)
		return FALSE;

	::SetWindowLong(m_hMsgWnd, GWL_USERDATA, (long)this);

	return TRUE;
}

BOOL CMciMedia::DestroyMsgWindow()
{
	if(m_hMsgWnd) {
		DestroyWindow(m_hMsgWnd);
		m_hMsgWnd = NULL;
		return TRUE;
	}
	else return FALSE;
}

// 이벤트 처리용 윈도우 프로시져
LRESULT CALLBACK CMciMedia::MsgWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	CMciMedia* pThis = (CMciMedia*)GetWindowLong(hwnd, GWL_USERDATA);

	if(pThis) {
		return pThis->DoMsgWndProc(hwnd, iMsg, wParam, lParam);
	}
	else {
		// WM_CREATE 메시지는 GetWindowLong()이 실패하므로 여기서 처리함
		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	}
}

LRESULT CALLBACK CMciMedia::DoMsgWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch(iMsg)
	{
	case WM_CREATE : 
		{
			// 이 메시지는 도착하지 않는다
		} break;

	case WM_DESTROY :
		{
		} break;

	case MM_MCINOTIFY :
		{
			if(wParam == MCI_NOTIFY_SUCCESSFUL){
				if(m_bLoop){
					DWORD dwResult = Play(0, m_bLoop); // 루프
					if(dwResult != MCIMEDIA_SUCCESS) 
						m_dwErrCode = dwResult;   // 에러값저장!
				}
			}
		} break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}