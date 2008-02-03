// DBThread.h: interface for the CDBThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BADUSERDBTHREAD_H__E5A49F86_7CD0_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_BADUSERDBTHREAD_H__E5A49F86_7CD0_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_BADUSERDBWORK 100		// 최대 DB 쓰레드 작업 개수
#define UM_BADUSERDBWORK_PROCESSED (WM_USER+1301)	// 신고 처리 사용자 메시지

//////////////////////////////////////////////////////////////////////
// DB큐 클래스
//////////////////////////////////////////////////////////////////////

class CCriticalSection;
extern CCriticalSection g_BadUserDBQueLock;

class CBadUserDBWorks
{
public:
	BOOL bValid;			// 유효한 상태인가?
	int GameCode;
	int ServerCode;
	CString MyID;
	CString SubmitDate;
	CString SubmitReason;
	CString TargetID;
	CString WatchID;
	CString Contents;
	
	CBadUserDBWorks();
	~CBadUserDBWorks();
	void Clear();
	void operator =(CBadUserDBWorks &val);
};

class CBadUserDBQue  
{
public:
	int SPos, EPos;
	int TotNum;
	CBadUserDBWorks Work[MAX_BADUSERDBWORK];

	CBadUserDBQue();
	virtual ~CBadUserDBQue();

	BOOL EnQue(CBadUserDBWorks &NewWork);
	BOOL DeQue(CBadUserDBWorks &GetWork);
};

//////////////////////////////////////////////////////////////////////
// 쓰레드 처리 관련부
//////////////////////////////////////////////////////////////////////

extern CBadUserDBQue BadUserDBQue;
extern CWinThread *pBadUserDBThread;

BOOL BeginBadUserDBThread();
BOOL TerminateBadUserDBThread();
UINT BadUserDBThreadFunc(LPVOID *pParam);

#endif // !defined(AFX_BADUSERDBTHREAD_H__E5A49F86_7CD0_11D4_97A4_0050BF0FBE67__INCLUDED_)
