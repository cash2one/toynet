// LogManDBThread.h: interface for the CLogManDBThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMANDBTHREAD_H__80DD008D_FC48_464E_86D9_5304996ABA86__INCLUDED_)
#define AFX_LOGMANDBTHREAD_H__80DD008D_FC48_464E_86D9_5304996ABA86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include <mmsystem.h>
#include "UserStruct.h"
#include "LogRecordDB.h"
#include "StatisticsLogDB.h" // [ 통계기록 ]
#include "Pre_UserLimitGMLogDB.h"
#include "AllinCharge.h" //[수호천사]

#define MAX_LOGDBWORK 2200					// 최대 DB 큐 작업 개수
#define UM_LOGDBWORK_LOGIN (WM_USER+1205)	// 유저 로그인 사용자 메시지
#define UM_LOGDBWORK_LOGOUT (WM_USER+1206)	// 유저 로그아웃 사용자 메시지

typedef struct
{
	BOOL bValid;			// 유효한 상태인가?
	// [ 통계기록 ]
	int WorkKind;			//  1:게임로그 2: 통계로그 3: 리더스 총액머니제한 4: 수호천사 로그

	UINT StartTick;			// DB 처리 시작 시간
	
	LOGRECORD	LogRecord;  // 로그 기록용
	STATISTICS_DBRECORD  StatisticsLog; // [ 통계기록 ]
	PRE_USERLIMITGMLOG   Pre_UserMLLog;//2004.05.07 리더스총액 제한 
	ALLIN_LOGRECORD		 AllInCharge_Log;	//[수호천사]
 
} LOGREC_DBWORKS;



//////////////////////////////////////////////////////////////////////
// DB큐 클래스
//////////////////////////////////////////////////////////////////////

class CLogDBQue  
{
public:
	CCriticalSection LogDBQueLock;

	list<LOGREC_DBWORKS*> Work;

	CLogDBQue();
	virtual ~CLogDBQue();

	BOOL EnQue(LOGREC_DBWORKS *pNewWork);
	BOOL DeQue(LOGREC_DBWORKS *pWork);
	int GetTotNum();
};
/*
class CLogDBQue  
{
public:
	CCriticalSection LogDBQueLock;
	int SPos, EPos;
	int TotNum;
	LOGREC_DBWORKS Work[MAX_LOGDBWORK];

	CLogDBQue();
	virtual ~CLogDBQue();

	BOOL EnQue(LOGREC_DBWORKS *pNewWork);
	BOOL DeQue(LOGREC_DBWORKS *pWork);
	int GetTotNum();
};
*/
class CLogManDBThread  
{
public:
	
		
	CLogManDBThread();
	virtual ~CLogManDBThread();

	CWinThread *pThread;		// 쓰레드 포인터
	CLogDBQue *pDBQue;			// DB큐 포인터

	BOOL bNowTerminate;		// 쓰레드 종료 플래그
	BOOL bWorking;			// DB처리 중인가?
	int ErrorCnt;			// DB에러 카운트
	LOGREC_DBWORKS Work;			// 처리할 DB

	// Database 클래스
	CDatabase *pDatabase_LogRec; 
	CDatabase *pDatabase_StatisticsRec;	// [ 통계기록 ]
	CDatabase *pDatabase_PreUserLMLogRec;	//2004.05.07 리더스총액 제한
	CDatabase *pDatabase_AllinChargeLog;		// [수호천사]

	// DB 레코드셋 클래스 포인터
	CLogRecordDB		*m_pLogRecDB;
	CStatisticsLogDB	*m_pStatisticsRecDB; // [ 통계기록 ]
	CPre_UserLimitGMLogDB *m_pPreUserLMLogRecDB; //2004.05.07 리더스총액 제한

	CAllinCharge *m_pAllinChargeDB;		// [수호천사]

	// 쓰레드
	BOOL Begin(CLogDBQue *pQue);
	BOOL Terminate();

	// 레코드셋 생성 함수
	BOOL CreateLogRecDB();
	BOOL CreateStatisticsLogDB(); // [ 통계기록 ]
	BOOL CreatePreUserLMLogDB();
	BOOL CreateAllinLogDB(); //[수호천사]
	

	// 모든 레코드셋을 삭제함
	void DeleteAllDB();

	// DB작업 처리함수
	void ProcessDBWork();

	BOOL AddNewLogrecordDB(LOGRECORD *pLog);
	BOOL AddNewStatisticsDB(STATISTICS_DBRECORD *p);
	BOOL AddNewPre_UsrLMLog(PRE_USERLIMITGMLOG *p);
	BOOL AddNewAllin_Log(ALLIN_LOGRECORD *pallinlog);

	// DB 쓰레드 펑션
	static UINT LogRecDBThreadFunc(LPVOID pParam);

};

// for static function
static UINT CLogManDBThread::LogRecDBThreadFunc(LPVOID pParam);

#endif // !defined(AFX_LOGMANDBTHREAD_H__80DD008D_FC48_464E_86D9_5304996ABA86__INCLUDED_)
