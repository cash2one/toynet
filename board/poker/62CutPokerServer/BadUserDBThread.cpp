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
// DBť Ŭ����
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
// DBť Ŭ����
//////////////////////////////////////////////////////////////////////

// DBť�� ũ��Ƽ�� ����
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
	// ������ Ȱ��ȭ
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
// ������ ó�� ���ú�
//////////////////////////////////////////////////////////////////////


// DB Work Que
CBadUserDBQue BadUserDBQue;

CWinThread *pBadUserDBThread=NULL;
static BOOL g_bBadUserDBThreadStop = FALSE;

void ProcessBadUserDBWork(CBadUserDBWorks &NowWork);
UINT BadUserDBThreadFunc(LPVOID pParam);

BOOL BeginBadUserDBThread()
{
	// �����带 ������� ���� �����Ѵ�
	pBadUserDBThread = ::AfxBeginThread(BadUserDBThreadFunc, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	if(pBadUserDBThread == NULL) return FALSE;
	// ������ ����
	pBadUserDBThread->ResumeThread();
	return TRUE;
}

BOOL TerminateBadUserDBThread()
{
	if(pBadUserDBThread == NULL) return FALSE;

	// ������ ���� �÷��� ����
	g_bBadUserDBThreadStop = TRUE;
	
	// ������ Ȱ��ȭ(���� ó���� �����ϱ� ����)
	pBadUserDBThread->ResumeThread();

	// ������ ���Ḧ �����ð����� ��ٸ�
	DWORD dwRetCode;
	dwRetCode = ::WaitForSingleObject(pBadUserDBThread->m_hThread, 30000);

	if(dwRetCode == WAIT_OBJECT_0)
	{
		// �������� ���Ḧ �����Ͽ���
		return TRUE;
	}
	else if(dwRetCode == WAIT_TIMEOUT)
	{
		// �����尡 �ð����� ������ ������� ������ ������ �����Ų��
		::TerminateThread(pBadUserDBThread->m_hThread, 0);
		CLogFile LogFile;
		LogFile.Writef("�Ű� DB������ Ÿ�Ӿƿ����� ���� �����Ŵ(������ �۾� ����=%d)", BadUserDBQue.TotNum);
	}

	return TRUE;
}

UINT BadUserDBThreadFunc(LPVOID pParam)
{
	CBadUserDBWorks Work;
	while(1)
	{
		BOOL bYesWork = BadUserDBQue.DeQue(Work);
	
		// �Ʒ��� �Լ����� ó���� DB�� ������ �������� Ȱ���� �����ȴ�
		// ��, ���� �÷��װ� ��Ʈ�� ���¿����� ��� �����Ͽ� ������ �� �ְ���
		if(bYesWork) ProcessBadUserDBWork(Work);
		else if(g_bBadUserDBThreadStop==FALSE) pBadUserDBThread->SuspendThread();

		// �� �ϵ� ���� �����带 ������� �÷��װ� ������ ���¶�� ������ ����
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