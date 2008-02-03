// LogManDBMan.h: interface for the CLogManDBMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMANDBMAN_H__C0291A64_CCF7_469C_85D9_BAE049C98635__INCLUDED_)
#define AFX_LOGMANDBMAN_H__C0291A64_CCF7_469C_85D9_BAE049C98635__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "LogRecordDB.h"
#include "StatisticsLogDB.h"
#include "Pre_UserLimitGMLogDB.h"

#include "UserStruct.h"
#include "LogManDBThread.h"


// 설정 가능한 DB 쓰레드 개수 범위
#define MINIMUM_LOGDBTHREAD 1
#define MAXIMUM_LOGDBTHREAD 200

class CLogManDBMan  
{
public:
	CLogManDBMan();
	virtual ~CLogManDBMan();

	CLogRecordDB			m_LogRecDB;
	CStatisticsLogDB		m_StatisticsDB; // [통계기록]
	CPre_UserLimitGMLogDB		m_PreUserLMLog; //2004.05.07 리더스총액 제한
	CAllinCharge				m_AllInLogDB;	// [수호천사]

	CLogDBQue DBQue;

	int TotThreadNum;

	// 유저 DB 처리 쓰레드 클래스 배열 포인터
	CLogManDBThread** pDBThread;

	// 버려진 쓰레드 개수
	int DiscardNum;



	CLogManDBThread** AllocDBThreadPtr(int threadnum);
	BOOL ChangeDBThreadNum(int threadnum);

	// DB 오픈 및 초기화 함수
	BOOL Init(int threadnum);
	BOOL CloseTempDB();
	BOOL CreateKeepAliveThread();

	BOOL SetDBWork(LOGREC_DBWORKS *pWork);
	BOOL DiscardDBThread(int tnum);
	int GetWorkingDBThreadNum();
	int GetLiveDBThreadNum();
	void CheckDBThread();
	void TermanateAllDBThread();

};

#endif // !defined(AFX_LOGMANDBMAN_H__C0291A64_CCF7_469C_85D9_BAE049C98635__INCLUDED_)
