// LogManDBMan.cpp: implementation of the CLogManDBMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "62CutPokerServerView.h"
#include "LogManDBMan.h"
#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogManDBMan::CLogManDBMan()
{
	pDBThread = NULL;
	DiscardNum = 0;
	TotThreadNum = 0;
}

CLogManDBMan::~CLogManDBMan()
{
	TermanateAllDBThread();
	if(pDBThread) { delete pDBThread; pDBThread = NULL; }
}

CLogManDBThread** CLogManDBMan::AllocDBThreadPtr(int threadnum)
{
	CLogManDBThread** pp = new (CLogManDBThread*[threadnum]);
	memset(pp, 0, sizeof(CLogManDBThread*)*threadnum);
	return pp;
}

BOOL CLogManDBMan::ChangeDBThreadNum(int threadnum)
{
	if(!pDBThread) return FALSE;

	if(threadnum < MINIMUM_LOGDBTHREAD) return FALSE;
	if(threadnum > MAXIMUM_LOGDBTHREAD) return FALSE;

	CLogManDBThread** pNew = AllocDBThreadPtr(threadnum);

	CheckDBThread();

	int n = max(1,TotThreadNum);
	BOOL* kill = new BOOL[n];
	memset(kill, 0, sizeof(BOOL*)*n);

	int tot = 0;
	for(int i=0; i<TotThreadNum; i++) {
		if(pDBThread[i]) {
			if(pDBThread[i]->pThread && pDBThread[i]->bWorking) {
					if(tot < threadnum) pNew[tot] = pDBThread[i];
					tot++;
			}
			else {
				kill[i] = TRUE;
			}
		}
	}

	if(tot > threadnum) {
		delete pNew;
		delete kill;
		return FALSE;
	}

	for(i=0; i<TotThreadNum; i++) {
		if(pDBThread[i] && kill[i] == TRUE) {
			pDBThread[i]->Terminate();
			pDBThread[i]->DeleteAllDB();
			delete pDBThread[i];
			pDBThread[i] = NULL;
		}
	}

	delete kill;

	TotThreadNum = threadnum;

	delete pDBThread; 
	pDBThread = pNew; 

	return TRUE;
}

BOOL CLogManDBMan::Init(int threadnum)
{
	if(pDBThread) return FALSE;

	if(threadnum < MINIMUM_LOGDBTHREAD) threadnum = MINIMUM_LOGDBTHREAD;
	if(threadnum > MAXIMUM_LOGDBTHREAD) threadnum = MAXIMUM_LOGDBTHREAD;

	pDBThread = AllocDBThreadPtr(threadnum);
	TotThreadNum = threadnum;

	// ������ DB���� �ʱ�ȭ
	CLogRecordDB::m_DSN = g_pMainView->Cfg.LogRecDSN;
	CLogRecordDB::m_Table = g_pMainView->Cfg.LogRecTable;
	CLogRecordDB::m_UID = g_pMainView->Cfg.LogRecUID;
	CLogRecordDB::m_Pass = g_pMainView->Cfg.LogRecPass;


	// ������ DB���� �ʱ�ȭ
	CStatisticsLogDB::m_DSN = g_pMainView->Cfg.StatisticsDSN; // [�����]
	CStatisticsLogDB::m_Table = g_pMainView->Cfg.StatisticsTable;
	CStatisticsLogDB::m_UID = g_pMainView->Cfg.StatisticsUID;
	CStatisticsLogDB::m_Pass = g_pMainView->Cfg.StatisticsPass;

	//2004.05.07 �������Ѿ� ����
	CPre_UserLimitGMLogDB::m_DSN = g_pMainView->Cfg.PreUserLGMLogDSN; // [�����]
	CPre_UserLimitGMLogDB::m_Table = g_pMainView->Cfg.PreUserLGMLogTable;
	CPre_UserLimitGMLogDB::m_UID = g_pMainView->Cfg.PreUserLGMLogUID;
	CPre_UserLimitGMLogDB::m_Pass = g_pMainView->Cfg.PreUserLGMLogPass;


	//��ȣõ��
	CAllinCharge::m_DSN = g_pMainView->Cfg.AllinChargeDSN;
	CAllinCharge::m_Table = g_pMainView->Cfg.AllinChargeTable;
	CAllinCharge::m_UID = g_pMainView->Cfg.AllinChargeUID;
	CAllinCharge::m_Pass = g_pMainView->Cfg.AllinChargePass;




	TRY
	{
		// ���� �ð��� ���� ��Ű�� ���Ͽ� �ǹ̾��� ������ ��
		m_LogRecDB.m_strFilter = "[RegDate]=0";
		m_LogRecDB.Open();
		m_LogRecDB.m_pDatabase->SetQueryTimeout(60);
		
		if( g_pMainView->Cfg.bStatisticsDB ) {
			m_StatisticsDB.m_strFilter = "[c_Kind]=0";// [�����]
			m_StatisticsDB.Open();// [�����]
			m_StatisticsDB.m_pDatabase->SetQueryTimeout(60);// [�����]
		}

		if( g_pMainView->Cfg.bPreUserLGMLogDB ) {
			m_PreUserLMLog.m_strFilter = "[ID]='dfdfdfd'";// [�����]
			m_PreUserLMLog.Open();// [�����]
			m_PreUserLMLog.m_pDatabase->SetQueryTimeout(60);// [�����]
		}

		if( g_pMainView->Cfg.bAllinDB ) {
			// [��ȣõ��]
			m_AllInLogDB.m_strFilter = "[ID]='fdfdfdfdf'";
			m_AllInLogDB.Open();
			m_AllInLogDB.m_pDatabase->SetQueryTimeout(60);
		}



	}
	CATCH(CDBException, e)
	{
		CString str;
		str.Format("LogRecDB ���� ����: %s", e->m_strError.operator LPCTSTR());
		AfxMessageBox(str);
		
		if(m_LogRecDB.IsOpen()) m_LogRecDB.Close();
		if( g_pMainView->Cfg.bStatisticsDB ) {
			if(m_StatisticsDB.IsOpen()) m_StatisticsDB.Close();// [�����]
		}

		if( g_pMainView->Cfg.bPreUserLGMLogDB ) {
			if(m_PreUserLMLog.IsOpen()) m_PreUserLMLog.Close();// [�����]
		}
		// [��ȣõ��]
		if( g_pMainView->Cfg.bAllinDB) {
			if(m_AllInLogDB.IsOpen()) m_LogRecDB.Close();
		}

		return FALSE;
	}
	END_CATCH

	if(m_LogRecDB.IsOpen()) m_LogRecDB.Close();
	if( g_pMainView->Cfg.bStatisticsDB ) {
		if(m_StatisticsDB.IsOpen()) m_StatisticsDB.Close();// [�����]
	}

	if( g_pMainView->Cfg.bPreUserLGMLogDB ) {
		if(m_PreUserLMLog.IsOpen()) m_PreUserLMLog.Close();// [�����]
	}

	// [��ȣõ��]
	if( g_pMainView->Cfg.bAllinDB ) {
		if(m_AllInLogDB.IsOpen()) m_LogRecDB.Close();
	}


	return TRUE;
}


BOOL CLogManDBMan::CloseTempDB()
{
	TRY
	{
		if(m_LogRecDB.m_pDatabase)
		{
			if(m_LogRecDB.IsOpen()) m_LogRecDB.Close();
			if(m_LogRecDB.m_pDatabase->IsOpen()) m_LogRecDB.m_pDatabase->Close();
		}

		if( g_pMainView->Cfg.bStatisticsDB && m_StatisticsDB.m_pDatabase)// [�����]
		{
			if(m_StatisticsDB.IsOpen()) m_StatisticsDB.Close();
			if(m_StatisticsDB.m_pDatabase->IsOpen()) m_StatisticsDB.m_pDatabase->Close();
		}

		// [��ȣõ��]
		if( g_pMainView->Cfg.bAllinDB && m_AllInLogDB.m_pDatabase) {
			if(m_AllInLogDB.IsOpen()) m_AllInLogDB.Close();
			if(m_AllInLogDB.m_pDatabase->IsOpen()) m_AllInLogDB.m_pDatabase->Close();
		}
	}
	CATCH(CDBException, e)
	{
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

BOOL CLogManDBMan::CreateKeepAliveThread()
{
	BOOL bRtn = TRUE;
	for(int i=0; i<TotThreadNum; i++)
	{
		if(pDBThread[i] == NULL) {
			pDBThread[i] = new CLogManDBThread();
			BOOL bFail = FALSE;
		
			if(pDBThread[i]->CreateLogRecDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }
			
			if( g_pMainView->Cfg.bStatisticsDB ) {
				if(pDBThread[i]->CreateStatisticsLogDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }// [�����]
			}

			if( g_pMainView->Cfg.bPreUserLGMLogDB)	{
				if(pDBThread[i]->CreatePreUserLMLogDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }// [�����]
			}
				
			// [��ȣõ��]
			if( g_pMainView->Cfg.bAllinDB)	{
				if(pDBThread[i]->CreateAllinLogDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }
			}


			if(bFail) { pDBThread[i]->DeleteAllDB(); continue; }

			// ȯ�� �������� DBŬ���� �ɼ��� �����Ǿ� �ִٸ� ���µ� DB�� ����
			if(g_pMainView->Cfg2.bDBClose2) pDBThread[i]->DeleteAllDB();

			// DB�����带 ����(�ʱ⿡�� �ƹ��ϵ� ���� ����)
			if(pDBThread[i]->Begin(&DBQue)==FALSE)
			{
				delete pDBThread[i];
				pDBThread[i] = NULL;
				bRtn=FALSE;
			}
		}
	}
	return bRtn;
}



BOOL CLogManDBMan::SetDBWork(LOGREC_DBWORKS *pWork)
{
	if(!DBQue.EnQue(pWork)) return FALSE;

	for(int i=0; i<TotThreadNum; i++)
	{
		if(pDBThread[i] == NULL) {
			pDBThread[i] = new CLogManDBThread();
			// ������ ���� ���� ����
			return pDBThread[i]->Begin(&DBQue);
		}
		else if(!pDBThread[i]->bNowTerminate && pDBThread[i]->bWorking==FALSE)
		{
			// DB �۾� �Ϸ��� Suspend���� �ִ� �������� �簡��
			return pDBThread[i]->Begin(&DBQue);
		}
	}

	// ��� �����尡 Ǯ �����ϴ� ��� ó���� ���� �ٸ� �����忡�� �ڵ����� ó���Ұ���
	return TRUE;
}

BOOL CLogManDBMan::DiscardDBThread(int tnum)
{
	if(tnum<0 || tnum>TotThreadNum) return FALSE;
	if(pDBThread[tnum] == NULL) return FALSE;

	// DB ������ ���� �÷��׸� �����Ѵ�
	pDBThread[tnum]->bNowTerminate = TRUE;

	// �α� ���
	if(pDBThread[tnum]->Work.WorkKind == 1) {// [�����]
		CLogFile logfile("LogRecordDB.txt");
		logfile.Writef(_T("LogRecDB ������ ����(Time=%d) : My_ID=%s, Status=%s, MyMoney=%I64d,\
			LoseMoney=%I64d, Win_ID=%s, WinM=%I64d, WinGetMoney =%I64d, myip=%s, winip=%s"),
			timeGetTime() - pDBThread[tnum]->Work.StartTick, pDBThread[tnum]->Work.LogRecord.ID,
			pDBThread[tnum]->Work.LogRecord.Status==31?"����":"����", pDBThread[tnum]->Work.LogRecord.CurMoney,
			pDBThread[tnum]->Work.LogRecord.LoseMoney, pDBThread[tnum]->Work.LogRecord.Win_ID,
			pDBThread[tnum]->Work.LogRecord.Win_CurMoney, pDBThread[tnum]->Work.LogRecord.Win_GetMoney,
			pDBThread[tnum]->Work.LogRecord.My_ip, pDBThread[tnum]->Work.LogRecord.Winner_ip);
	}

	
	// �����带 ���� ����
	//pDBThread[tnum]->Terminate();
	// �޸� ������ ���� �ʰ� �����͸� �ʱ�ȭ�Ͽ� �׳� ������
	pDBThread[tnum] = NULL;

	DiscardNum++;
	CString str;
	str.Format("%d", DiscardNum);
	(g_pMainView->GetDlgItem(IDC_STATIC_DISCARDNUM))->SetWindowText(str);

	return TRUE;
}

int CLogManDBMan::GetWorkingDBThreadNum()
{
	int totnum = 0;
	for(int i=0; i<TotThreadNum; i++) if(pDBThread[i] != NULL && pDBThread[i]->bWorking) totnum++;
	return totnum;
}

int CLogManDBMan::GetLiveDBThreadNum()
{
	int totnum = 0;
	for(int i=0; i<TotThreadNum; i++) if(pDBThread[i] != NULL) totnum++;
	return totnum;
}

void CLogManDBMan::CheckDBThread()
{
	for(int i=0; i<TotThreadNum; i++) 
	{
		if(pDBThread[i] == NULL) continue;
		// �ùٸ��� ����� ������� �޸𸮸� �����Ѵ�
		if(pDBThread[i]->pThread == NULL && pDBThread[i]->bWorking == FALSE)
		{
			delete pDBThread[i];
			pDBThread[i] = NULL;
			continue;
		}
		// �ð��ʰ��� ������� ������?
		if(pDBThread[i]->bWorking)
		{
			UINT dtick = timeGetTime() - pDBThread[i]->Work.StartTick;
			if(dtick > MAX_DBTHREAD_TIMEOUT) DiscardDBThread(i);
		}
	}
}

void CLogManDBMan::TermanateAllDBThread()
{
	for(int i=0; i<TotThreadNum; i++) 
	{
		if(pDBThread[i]==NULL) continue;

		if(pDBThread[i]->pThread != NULL) {
			// ������ ����
			pDBThread[i]->Terminate();
		}

		// DB ������ Ŭ���� ����
		delete pDBThread[i];
		pDBThread[i] = NULL;
	}
}
