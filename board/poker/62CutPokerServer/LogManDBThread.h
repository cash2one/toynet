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
#include "StatisticsLogDB.h" // [ ����� ]
#include "Pre_UserLimitGMLogDB.h"
#include "AllinCharge.h" //[��ȣõ��]

#define MAX_LOGDBWORK 2200					// �ִ� DB ť �۾� ����
#define UM_LOGDBWORK_LOGIN (WM_USER+1205)	// ���� �α��� ����� �޽���
#define UM_LOGDBWORK_LOGOUT (WM_USER+1206)	// ���� �α׾ƿ� ����� �޽���

typedef struct
{
	BOOL bValid;			// ��ȿ�� �����ΰ�?
	// [ ����� ]
	int WorkKind;			//  1:���ӷα� 2: ���α� 3: ������ �Ѿ׸Ӵ����� 4: ��ȣõ�� �α�

	UINT StartTick;			// DB ó�� ���� �ð�
	
	LOGRECORD	LogRecord;  // �α� ��Ͽ�
	STATISTICS_DBRECORD  StatisticsLog; // [ ����� ]
	PRE_USERLIMITGMLOG   Pre_UserMLLog;//2004.05.07 �������Ѿ� ���� 
	ALLIN_LOGRECORD		 AllInCharge_Log;	//[��ȣõ��]
 
} LOGREC_DBWORKS;



//////////////////////////////////////////////////////////////////////
// DBť Ŭ����
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

	CWinThread *pThread;		// ������ ������
	CLogDBQue *pDBQue;			// DBť ������

	BOOL bNowTerminate;		// ������ ���� �÷���
	BOOL bWorking;			// DBó�� ���ΰ�?
	int ErrorCnt;			// DB���� ī��Ʈ
	LOGREC_DBWORKS Work;			// ó���� DB

	// Database Ŭ����
	CDatabase *pDatabase_LogRec; 
	CDatabase *pDatabase_StatisticsRec;	// [ ����� ]
	CDatabase *pDatabase_PreUserLMLogRec;	//2004.05.07 �������Ѿ� ����
	CDatabase *pDatabase_AllinChargeLog;		// [��ȣõ��]

	// DB ���ڵ�� Ŭ���� ������
	CLogRecordDB		*m_pLogRecDB;
	CStatisticsLogDB	*m_pStatisticsRecDB; // [ ����� ]
	CPre_UserLimitGMLogDB *m_pPreUserLMLogRecDB; //2004.05.07 �������Ѿ� ����

	CAllinCharge *m_pAllinChargeDB;		// [��ȣõ��]

	// ������
	BOOL Begin(CLogDBQue *pQue);
	BOOL Terminate();

	// ���ڵ�� ���� �Լ�
	BOOL CreateLogRecDB();
	BOOL CreateStatisticsLogDB(); // [ ����� ]
	BOOL CreatePreUserLMLogDB();
	BOOL CreateAllinLogDB(); //[��ȣõ��]
	

	// ��� ���ڵ���� ������
	void DeleteAllDB();

	// DB�۾� ó���Լ�
	void ProcessDBWork();

	BOOL AddNewLogrecordDB(LOGRECORD *pLog);
	BOOL AddNewStatisticsDB(STATISTICS_DBRECORD *p);
	BOOL AddNewPre_UsrLMLog(PRE_USERLIMITGMLOG *p);
	BOOL AddNewAllin_Log(ALLIN_LOGRECORD *pallinlog);

	// DB ������ ���
	static UINT LogRecDBThreadFunc(LPVOID pParam);

};

// for static function
static UINT CLogManDBThread::LogRecDBThreadFunc(LPVOID pParam);

#endif // !defined(AFX_LOGMANDBTHREAD_H__80DD008D_FC48_464E_86D9_5304996ABA86__INCLUDED_)
