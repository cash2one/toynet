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
	// uElapse ���� 0�ΰ�� Ÿ�̸Ӵ� ������ �����¿� ������
	// �̶��� SetEvent() �Լ��� ���� Ÿ�̸� ȣ���� �����ϴ�
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
	// Ÿ�̸� �ֱⰪ�� 0���� ������ ��� ���� �̺�Ʈ �߻��� ���
	if(m_pThread==NULL) return FALSE;
	m_WaitEvent.SetEvent();
	return TRUE;
}

BOOL CThreadTimer::BeginThread()
{
	if(m_pThread)  return FALSE;

	// �����带 ������� ���� �����Ѵ�(������ ���ν������� ���� �������� �ʱ�ȭ�� ��ٷ����ϱ⶧��)
	m_pThread = ::AfxBeginThread(ThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	if(m_pThread == NULL) return FALSE;

	// �̺�Ʈ ��ü�� ���� �÷��׸� �ʱ�ȭ
	m_bNowTerminate = FALSE;
	m_WaitEvent.ResetEvent();

	// ������ ����
	m_pThread->ResumeThread();
	return TRUE;
}

BOOL CThreadTimer::TerminateThread()
{
	if(m_pThread==NULL) return FALSE;

	// ������ ���� �÷��� ����
	m_bNowTerminate = TRUE;
	m_WaitEvent.SetEvent();

	// ������ ���Ḧ 1�ʰ� ��ٸ�
	DWORD dwRetCode = ::WaitForSingleObject(m_pThread->m_hThread, 1000);

	if(dwRetCode == WAIT_OBJECT_0) {
		// �������� ���Ḧ �����Ͽ���
	}
	else if(dwRetCode == WAIT_TIMEOUT) {
		// �����尡 �ð����� ������ ������� ������ ������ �����Ų��
		::TerminateThread(m_pThread->m_hThread, 0);
	}

	// ���� ó��
	m_pThread = NULL;

	return TRUE;
}

UINT CThreadTimer::ThreadFunc(LPVOID pParam)
{
	if(pParam==NULL) return 0;
	CThreadTimer *pThis = (CThreadTimer*)pParam;

	while(TRUE)
	{
		// ������ �ð���ŭ �̺�Ʈ�� ��ٸ���
		DWORD dwRetCode = ::WaitForSingleObject(pThis->m_WaitEvent.m_hObject, pThis->m_uElapse);

		if(dwRetCode == WAIT_OBJECT_0) {
			// �̺�Ʈ�� ���õ� ����(���� �Լ��� ���Ѱ����)
		}
		else if(dwRetCode == WAIT_TIMEOUT) {
			// Ÿ�Ӿƿ��� ���(�������� Ÿ�� �ֱ���)
		}
		
		if(pThis->m_bNowTerminate) break;

		if(pThis->m_lpTimerFunc) {
			pThis->m_lpTimerFunc(pThis->m_hWnd, WM_TIMER, pThis->m_nIDEvent, GetTickCount());
		}
		else if(pThis->m_hWnd) {
			SendMessage(pThis->m_hWnd, WM_TIMER, pThis->m_nIDEvent, (LPARAM)pThis->m_lpTimerFunc);
			// ������ �ֱ�� �ѹ��� Sleep�ϰ� �Ѵ�.(���� Messageó���� ������ �α� ����)
			//if(pThis->m_uElapse == 0) Sleep(3);
		}
	}

	return 0;
}
