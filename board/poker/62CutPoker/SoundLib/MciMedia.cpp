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

#define MCIMEDIA_SUCCESS   (0) // MCI �Լ����ϰ��� ������ 0�� �����ϴ� ��찡 ����

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
// �̵�� �Լ�
//////////////////////////////////////////////////////////////////////

// �־��� �����̸����� �̵� �����Ѵ�
BOOL CMciMedia::Open(LPCTSTR lpszFileName)
{
	MCI_OPEN_PARMS mciOpenParms;
	MCI_SET_PARMS  mciSetParms;

	memset(&mciOpenParms,0,sizeof(MCI_OPEN_PARMS));
	memset(&mciSetParms, 0,sizeof(MCI_SET_PARMS));

	// �̹� �̵� ���µǾ� �ִٸ� �ݴ´�
	if (GetStatus(MCI_STATUS_MODE) != MCI_MODE_NOT_READY) 
		Close();
	
	mciOpenParms.lpstrDeviceType  = NULL;
	mciOpenParms.lpstrElementName = lpszFileName;
	
	DWORD dwResult = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParms);
	
	if (dwResult == MCIMEDIA_SUCCESS) {
		
		m_nDeviceID = mciOpenParms.wDeviceID; // ���������� ���µǾ����� ��ġID �߱�
		
		mciSetParms.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
		mciSendCommand( m_nDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID)&mciSetParms);
	}
	
	m_dwErrCode = dwResult;
	return (dwResult == MCIMEDIA_SUCCESS);
}

// �̵� �ݴ´�
void CMciMedia::Close()
{
	if (m_nDeviceID != 0){
		mciSendCommand(m_nDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
		m_nDeviceID = 0;
	}
	DestroyMsgWindow();
}

// ���� �޽��� ���ڿ��� ���Ѵ�
BOOL CMciMedia::GetErrorString(DWORD dwErrCode, LPCTSTR lpszError, UINT nSize)
{
	return mciGetErrorString(dwErrCode, (LPTSTR)lpszError, nSize);
}

//////////////////////////////////////////////////////////////////////
// �̵�� ���� �Լ�
//////////////////////////////////////////////////////////////////////

// ������ ��ġ���� �̵� �����Ѵ�
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

// �Ͻ� ����
BOOL CMciMedia::Pause()
{
	MCI_GENERIC_PARMS mciGenericParms;
	DWORD dwResult = mciSendCommand(m_nDeviceID, MCI_PAUSE, MCI_WAIT,(DWORD)(LPVOID)&mciGenericParms);
	m_dwErrCode = dwResult;
	return (dwResult == MCIMEDIA_SUCCESS);
}

// ����
BOOL CMciMedia::Resume()
{
	MCI_GENERIC_PARMS mciGenericParms;
	DWORD dwResult = mciSendCommand(m_nDeviceID, MCI_RESUME, MCI_WAIT,(DWORD)(LPVOID)&mciGenericParms);
	m_dwErrCode = dwResult;
	return (dwResult == MCIMEDIA_SUCCESS);
}

// ����
BOOL CMciMedia::Stop()
{
	MCI_GENERIC_PARMS mciGenericParms;
	DWORD dwResult = mciSendCommand(m_nDeviceID, MCI_STOP, 0,(DWORD)(LPVOID)&mciGenericParms);
	m_dwErrCode = dwResult;
	return (dwResult == MCIMEDIA_SUCCESS);
}

//////////////////////////////////////////////////////////////////////
// �̵�� ���� �Լ�
//////////////////////////////////////////////////////////////////////

// ��ġ�� ���¸� ���Ѵ�
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

// ��ü ���̸� �и��ʴ����� ���Ѵ�
DWORD CMciMedia::GetTotalLength()
{
	if(m_nDeviceID==0) return 0;
	return GetStatus(MCI_STATUS_LENGTH);
}

// ��ü ���̸� �и��ʴ����� ���Ѵ�
DWORD CMciMedia::GetTotalPosition()
{
	return GetTotalLength();
}

// ��� ��ġ�� �и��ʴ����� ��ȯ
DWORD CMciMedia::GetPosition()
{
	if(m_nDeviceID==0) return 0;
	return GetStatus(MCI_STATUS_POSITION);
}

//////////////////////////////////////////////////////////////////////
// �̵�� ���� �Լ�
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
// MCI �޽��� ó�� ������ 
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

// �̺�Ʈ ó���� ������ ���ν���
LRESULT CALLBACK CMciMedia::MsgWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	CMciMedia* pThis = (CMciMedia*)GetWindowLong(hwnd, GWL_USERDATA);

	if(pThis) {
		return pThis->DoMsgWndProc(hwnd, iMsg, wParam, lParam);
	}
	else {
		// WM_CREATE �޽����� GetWindowLong()�� �����ϹǷ� ���⼭ ó����
		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	}
}

LRESULT CALLBACK CMciMedia::DoMsgWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch(iMsg)
	{
	case WM_CREATE : 
		{
			// �� �޽����� �������� �ʴ´�
		} break;

	case WM_DESTROY :
		{
		} break;

	case MM_MCINOTIFY :
		{
			if(wParam == MCI_NOTIFY_SUCCESSFUL){
				if(m_bLoop){
					DWORD dwResult = Play(0, m_bLoop); // ����
					if(dwResult != MCIMEDIA_SUCCESS) 
						m_dwErrCode = dwResult;   // ����������!
				}
			}
		} break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}