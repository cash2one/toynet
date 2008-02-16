// ThreadTimer.h: interface for the CThreadTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADTIMER_H__6795DBD7_F4DC_437C_850D_EDEA61B73217__INCLUDED_)
#define AFX_THREADTIMER_H__6795DBD7_F4DC_437C_850D_EDEA61B73217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include <mmsystem.h>

class CThreadTimer  
{
public:

	CWinThread *m_pThread;		// 쓰레드 포인터
	CEvent m_WaitEvent;			// 타임 이벤트
	BOOL m_bNowTerminate;		// 쓰레드 종료 플래그
	
	HWND m_hWnd;				// 타이머 메시지를 받을 윈도우 핸들
	UINT m_nIDEvent;			// 타이머 아이디
	UINT m_uElapse;				// 타임 주기
	TIMERPROC m_lpTimerFunc;	// 타이머 프로시저

	int TmpCnt;

	CThreadTimer();
	virtual ~CThreadTimer();
	void ClearMember();

	BOOL SetTimer(HWND hWnd, UINT nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillTimer();
	BOOL SetEvent();

	BOOL BeginThread();
	BOOL TerminateThread();

	// 쓰레드 펑션
	static UINT ThreadFunc(LPVOID pParam);
};


// for static function
static UINT CThreadTimer::ThreadFunc(LPVOID pParam);

#endif // !defined(AFX_THREADTIMER_H__6795DBD7_F4DC_437C_850D_EDEA61B73217__INCLUDED_)
