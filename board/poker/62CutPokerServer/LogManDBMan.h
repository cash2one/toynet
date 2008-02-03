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


// ���� ������ DB ������ ���� ����
#define MINIMUM_LOGDBTHREAD 1
#define MAXIMUM_LOGDBTHREAD 200

class CLogManDBMan  
{
public:
	CLogManDBMan();
	virtual ~CLogManDBMan();

	CLogRecordDB			m_LogRecDB;
	CStatisticsLogDB		m_StatisticsDB; // [�����]
	CPre_UserLimitGMLogDB		m_PreUserLMLog; //2004.05.07 �������Ѿ� ����
	CAllinCharge				m_AllInLogDB;	// [��ȣõ��]

	CLogDBQue DBQue;

	int TotThreadNum;

	// ���� DB ó�� ������ Ŭ���� �迭 ������
	CLogManDBThread** pDBThread;

	// ������ ������ ����
	int DiscardNum;



	CLogManDBThread** AllocDBThreadPtr(int threadnum);
	BOOL ChangeDBThreadNum(int threadnum);

	// DB ���� �� �ʱ�ȭ �Լ�
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
