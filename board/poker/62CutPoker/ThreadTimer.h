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

	CWinThread *m_pThread;		// ������ ������
	CEvent m_WaitEvent;			// Ÿ�� �̺�Ʈ
	BOOL m_bNowTerminate;		// ������ ���� �÷���
	
	HWND m_hWnd;				// Ÿ�̸� �޽����� ���� ������ �ڵ�
	UINT m_nIDEvent;			// Ÿ�̸� ���̵�
	UINT m_uElapse;				// Ÿ�� �ֱ�
	TIMERPROC m_lpTimerFunc;	// Ÿ�̸� ���ν���

	int TmpCnt;

	CThreadTimer();
	virtual ~CThreadTimer();
	void ClearMember();

	BOOL SetTimer(HWND hWnd, UINT nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
	BOOL KillTimer();
	BOOL SetEvent();

	BOOL BeginThread();
	BOOL TerminateThread();

	// ������ ���
	static UINT ThreadFunc(LPVOID pParam);
};


// for static function
static UINT CThreadTimer::ThreadFunc(LPVOID pParam);

#endif // !defined(AFX_THREADTIMER_H__6795DBD7_F4DC_437C_850D_EDEA61B73217__INCLUDED_)
