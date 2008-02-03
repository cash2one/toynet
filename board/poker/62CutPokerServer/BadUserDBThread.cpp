// DBThread.cpp: implementation of the CDBThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BadUserDBThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "62CutPokerServerView.h"
#include "LogFile.h"

//////////////////////////////////////////////////////////////////////
// DB큐 클래스
//////////////////////////////////////////////////////////////////////


CBadUserDBWorks::CBadUserDBWorks()
{
	Clear();
}

CBadUserDBWorks::~CBadUserDBWorks()
{

}

void CBadUserDBWorks::Clear()
{
	bValid = FALSE;
	GameCode = 0;
	ServerCode = 0;
	MyID = "";
	SubmitDate = "";
	SubmitReason = "";
	TargetID = "";
	WatchID = "";
	Contents = "";
}

void CBadUserDBWorks::operator =(CBadUserDBWorks &val)
{
	bValid = val.bValid;
	GameCode = val.GameCode;
	ServerCode = val.ServerCode;
	MyID = val.MyID;
	SubmitDate = val.SubmitDate;
	SubmitReason = val.SubmitReason;
	TargetID = val.TargetID;
	WatchID = val.WatchID;
	Contents = val.Contents;
}

//////////////////////////////////////////////////////////////////////
// DB큐 클래스
//////////////////////////////////////////////////////////////////////

// DB큐용 크리티컬 섹션
CCriticalSection g_BadUserDBQueLock;

CBadUserDBQue::CBadUserDBQue()
{
	SPos = EPos = 0;
	TotNum = 0;
	for(int i=0; i<MAX_BADUSERDBWORK; i++) Work[i].Clear();
}

CBadUserDBQue::~CBadUserDBQue()
{

}


BOOL CBadUserDBQue::EnQue(CBadUserDBWorks &NewWork)
{
	// 쓰레드 활성화
	if(pBadUserDBThread) pBadUserDBThread->ResumeThread();

	g_BadUserDBQueLock.Lock();

	if(TotNum >= MAX_BADUSERDBWORK) { g_BadUserDBQueLock.Unlock(); return FALSE; }
	
	//memcpy(&Work[EPos], pWork, sizeof(DBWORKS));
	Work[EPos] = NewWork;
	EPos++;
	if(EPos >= MAX_BADUSERDBWORK) EPos = EPos - MAX_BADUSERDBWORK;
	TotNum++;
	
	g_BadUserDBQueLock.Unlock();
	return TRUE;
}

BOOL CBadUserDBQue::DeQue(CBadUserDBWorks &GetWork)
{
	g_BadUserDBQueLock.Lock();

	if(TotNum <= 0 || SPos == EPos)
	{
		g_BadUserDBQueLock.Unlock();
		return FALSE;
	}

	//memcpy(pWork, &Work[SPos], sizeof(DBWORKS));
	//ZeroMemory(&Work[SPos], sizeof(DBWORKS));
	GetWork = Work[SPos];
	Work[SPos].Clear();
	SPos++;
	if(SPos >= MAX_BADUSERDBWORK) SPos = SPos - MAX_BADUSERDBWORK;
	TotNum--;

	g_BadUserDBQueLock.Unlock();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// 쓰레드 처리 관련부
//////////////////////////////////////////////////////////////////////


// DB Work Que
CBadUserDBQue BadUserDBQue;

CWinThread *pBadUserDBThread=NULL;
static BOOL g_bBadUserDBThreadStop = FALSE;

void ProcessBadUserDBWork(CBadUserDBWorks &NowWork);
UINT BadUserDBThreadFunc(LPVOID pParam);

BOOL BeginBadUserDBThread()
{
	// 쓰레드를 서스펜드 모드로 생성한다
	pBadUserDBThread = ::AfxBeginThread(BadUserDBThreadFunc, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	if(pBadUserDBThread == NULL) return FALSE;
	// 쓰레드 가동
	pBadUserDBThread->ResumeThread();
	return TRUE;
}

BOOL TerminateBadUserDBThread()
{
	if(pBadUserDBThread == NULL) return FALSE;

	// 쓰레드 종료 플래그 세팅
	g_bBadUserDBThreadStop = TRUE;
	
	// 쓰레드 활성화(종료 처리를 수행하기 위해)
	pBadUserDBThread->ResumeThread();

	// 쓰레드 종료를 일정시간동안 기다림
	DWORD dwRetCode;
	dwRetCode = ::WaitForSingleObject(pBadUserDBThread->m_hThread, 30000);

	if(dwRetCode == WAIT_OBJECT_0)
	{
		// 정상적인 종료를 수행하였음
		return TRUE;
	}
	else if(dwRetCode == WAIT_TIMEOUT)
	{
		// 쓰레드가 시간내에 저절로 종료되지 않으면 강제로 종료시킨다
		::TerminateThread(pBadUserDBThread->m_hThread, 0);
		CLogFile LogFile;
		LogFile.Writef("신고 DB쓰레드 타임아웃으로 강제 종료시킴(미종결 작업 개수=%d)", BadUserDBQue.TotNum);
	}

	return TRUE;
}

UINT BadUserDBThreadFunc(LPVOID pParam)
{
	CBadUserDBWorks Work;
	while(1)
	{
		BOOL bYesWork = BadUserDBQue.DeQue(Work);
	
		// 아래의 함수에서 처리할 DB가 없으면 쓰레드의 활동이 정지된다
		// 단, 종료 플래그가 세트된 상태에서는 계속 수행하여 종료할 수 있게함
		if(bYesWork) ProcessBadUserDBWork(Work);
		else if(g_bBadUserDBThreadStop==FALSE) pBadUserDBThread->SuspendThread();

		// 할 일도 없고 쓰레드를 멈춰야할 플레그가 설정된 상태라면 쓰레드 종료
		if(g_bBadUserDBThreadStop==TRUE && bYesWork==FALSE) break;
		::Sleep(0);
	}

	return 0;
}

void ProcessBadUserDBWork(CBadUserDBWorks &NowWork)
{
	CBadUserDBWorks Work;
	Work = NowWork;
	DBMan.AddNewBadUserDB(&Work);
}