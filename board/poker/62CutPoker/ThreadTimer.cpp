// ThreadTimer.cpp: implementation of the CThreadTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "client.h"
#include "ThreadTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib, "winmm.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadTimer::CThreadTimer()
{
	ClearMember();
}

CThreadTimer::~CThreadTimer()
{
	TerminateThread();
}

void CThreadTimer::ClearMember()
{
	m_pThread = NULL;
	m_bNowTerminate = FALSE;
	
	m_hWnd = NULL;
	m_nIDEvent = 0;
	m_uElapse = 0;
	m_lpTimerFunc = NULL;

	TmpCnt = 0;
}

BOOL CThreadTimer::SetTimer(HWND hWnd, UINT nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
{
	// uElapse 값이 0인경우 타이머는 무한정 대기상태에 빠진다
	// 이때는 SetEvent() 함수로 수동 타이머 호출이 가능하다
	if(m_pThread)  return FALSE;
	m_hWnd = hWnd;
	m_nIDEvent = nIDEvent;
	m_uElapse = uElapse;
	m_lpTimerFunc = lpTimerFunc;
	BeginThread();
	return TRUE;
}

BOOL CThreadTimer::KillTimer()
{
	if(m_pThread==NULL) return FALSE;
	TerminateThread();
	ClearMember();
	return TRUE;
}

BOOL CThreadTimer::SetEvent()
{
	// 타이머 주기값이 0으로 설정된 경우 수동 이벤트 발생시 사용
	if(m_pThread==NULL) return FALSE;
	m_WaitEvent.SetEvent();
	return TRUE;
}

BOOL CThreadTimer::BeginThread()
{
	if(m_pThread)  return FALSE;

	// 쓰레드를 서스펜드 모드로 생성한다(쓰레드 프로시저에서 사용될 포인터의 초기화를 기다려야하기때문)
	m_pThread = ::AfxBeginThread(ThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	if(m_pThread == NULL) return FALSE;

	// 이벤트 객체및 종료 플래그를 초기화
	m_bNowTerminate = FALSE;
	m_WaitEvent.ResetEvent();

	// 쓰레드 가동
	m_pThread->ResumeThread();
	return TRUE;
}

BOOL CThreadTimer::TerminateThread()
{
	if(m_pThread==NULL) return FALSE;

	// 쓰레드 종료 플래그 세팅
	m_bNowTerminate = TRUE;
	m_WaitEvent.SetEvent();

	// 쓰레드 종료를 1초간 기다림
	DWORD dwRetCode = ::WaitForSingleObject(m_pThread->m_hThread, 1000);

	if(dwRetCode == WAIT_OBJECT_0) {
		// 정상적인 종료를 수행하였음
	}
	else if(dwRetCode == WAIT_TIMEOUT) {
		// 쓰레드가 시간내에 저절로 종료되지 않으면 강제로 종료시킨다
		::TerminateThread(m_pThread->m_hThread, 0);
	}

	// 종결 처리
	m_pThread = NULL;

	return TRUE;
}

UINT CThreadTimer::ThreadFunc(LPVOID pParam)
{
	if(pParam==NULL) return 0;
	CThreadTimer *pThis = (CThreadTimer*)pParam;

	while(TRUE)
	{
		// 설정된 시간만큼 이벤트를 기다린다
		DWORD dwRetCode = ::WaitForSingleObject(pThis->m_WaitEvent.m_hObject, pThis->m_uElapse);

		if(dwRetCode == WAIT_OBJECT_0) {
			// 이벤트가 세팅된 상태(종료 함수로 인한경우임)
		}
		else if(dwRetCode == WAIT_TIMEOUT) {
			// 타임아웃된 경우(정상적인 타임 주기임)
		}
		
		if(pThis->m_bNowTerminate) break;

		if(pThis->m_lpTimerFunc) {
			pThis->m_lpTimerFunc(pThis->m_hWnd, WM_TIMER, pThis->m_nIDEvent, GetTickCount());
		}
		else if(pThis->m_hWnd) {
			SendMessage(pThis->m_hWnd, WM_TIMER, pThis->m_nIDEvent, (LPARAM)pThis->m_lpTimerFunc);
			// 일정한 주기로 한번씩 Sleep하게 한다.(윈도 Message처리의 여유를 두기 위함)
			//if(pThis->m_uElapse == 0) Sleep(3);
		}
	}

	return 0;
}
