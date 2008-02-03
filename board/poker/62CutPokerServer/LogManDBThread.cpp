// LogManDBThread.cpp: implementation of the CLogManDBThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogManDBThread.h"

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


CLogDBQue::CLogDBQue()
{
	Work.clear();
}

CLogDBQue::~CLogDBQue()
{
	while(Work.size()) 
	{
		delete Work.front();
		Work.pop_front();
	}
	Work.clear();
}

BOOL CLogDBQue::EnQue(LOGREC_DBWORKS *pNewWork)
{
	LogDBQueLock.Lock();

	// DBť�� �ִ� ������ �ִ� ���� ������ 1.25�� ������ ����
	int max_dbworknum = MAX_DATA_SOCKET + (MAX_DATA_SOCKET>>2);

	if(Work.size() >= max_dbworknum) { 
		LogDBQueLock.Unlock(); 
		return FALSE; 
	}

	LOGREC_DBWORKS* pnew = new LOGREC_DBWORKS;
	memcpy(pnew, pNewWork, sizeof(LOGREC_DBWORKS));
	
	Work.push_back(pnew);	
	
	LogDBQueLock.Unlock();

	return TRUE;
}

BOOL CLogDBQue::DeQue(LOGREC_DBWORKS *pWork)
{
	LogDBQueLock.Lock();

	if(Work.size() == 0) {
		LogDBQueLock.Unlock();
		return FALSE;
	}

	memcpy(pWork, Work.front(), sizeof(LOGREC_DBWORKS));
	pWork->StartTick = timeGetTime();
	
	delete Work.front();
	Work.pop_front();

	LogDBQueLock.Unlock();

	return TRUE;
}

int CLogDBQue::GetTotNum()
{
	int tot = 0;
	LogDBQueLock.Lock();
	tot = Work.size();
	LogDBQueLock.Unlock();
	return tot;
}

/*
CLogDBQue::CLogDBQue()
{
	SPos = EPos = 0;
	TotNum = 0;
	memset(Work, 0, sizeof(Work));
}

CLogDBQue::~CLogDBQue()
{

}

BOOL CLogDBQue::EnQue(LOGREC_DBWORKS *pNewWork)
{
	LogDBQueLock.Lock();

	if(TotNum >= MAX_LOGDBWORK) { LogDBQueLock.Unlock(); return FALSE; }
	
	memcpy(&Work[EPos], pNewWork, sizeof(LOGREC_DBWORKS));
	EPos++;
	if(EPos >= MAX_LOGDBWORK) EPos = EPos - MAX_LOGDBWORK;
	TotNum++;
	
	LogDBQueLock.Unlock();
	return TRUE;
}

BOOL CLogDBQue::DeQue(LOGREC_DBWORKS *pWork)
{
	LogDBQueLock.Lock();

	if(TotNum <= 0 || SPos == EPos) {
		LogDBQueLock.Unlock();
		return FALSE;
	}

	memcpy(pWork, &Work[SPos], sizeof(LOGREC_DBWORKS));
	pWork->StartTick = timeGetTime();
	ZeroMemory(&Work[SPos], sizeof(LOGREC_DBWORKS));
	SPos++;
	if(SPos >= MAX_LOGDBWORK) SPos = SPos - MAX_LOGDBWORK;
	TotNum--;

	LogDBQueLock.Unlock();

	return TRUE;
}

int CLogDBQue::GetTotNum()
{
	int tot = 0;
	LogDBQueLock.Lock();
	tot = TotNum;
	LogDBQueLock.Unlock();
	return tot;
}
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogManDBThread::CLogManDBThread()
{
	pThread = NULL;
	pDBQue = NULL;

	bNowTerminate = FALSE;
	bWorking = FALSE;
	ErrorCnt = 0;

	ZeroMemory(&Work, sizeof(LOGREC_DBWORKS));
	
	// Database Ŭ����
	pDatabase_LogRec = NULL;
	pDatabase_StatisticsRec = NULL;// [ ����� ]
	pDatabase_PreUserLMLogRec = NULL;
	pDatabase_AllinChargeLog = NULL; //��ȣõ��

	// DB ���ڵ�� Ŭ���� ������
	m_pLogRecDB = NULL;
	m_pStatisticsRecDB = NULL;// [ ����� ]
	m_pPreUserLMLogRecDB = NULL;
	m_pAllinChargeDB = NULL;
	

}

CLogManDBThread::~CLogManDBThread()
{
	DeleteAllDB();
}

BOOL CLogManDBThread::Begin(CLogDBQue *pQue)
{
	if(pQue == NULL || bWorking == TRUE)  return FALSE;

	pDBQue = pQue;

	if(pThread == NULL)	{
		// �����带 ������� ���� �����Ѵ�(������ ���ν������� ���� �������� �ʱ�ȭ�� ��ٷ����ϱ⶧��)
		pThread = ::AfxBeginThread(LogRecDBThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
		if(pThread == NULL)
		{
			DeleteAllDB(); 
			bWorking = FALSE;
			ZeroMemory(&Work, sizeof(Work));
			return FALSE;
		}
		// ������ ����
		bWorking = TRUE;
		Work.StartTick = timeGetTime();
		pThread->ResumeThread();
	}
	else {
		// ������ ����
		bWorking = TRUE;
		Work.StartTick = timeGetTime();
		pThread->ResumeThread();
	}

	return TRUE;
}


BOOL CLogManDBThread::Terminate()
{
	if(pThread==NULL) return FALSE;

	// ������ ���� �÷��� ����
	bNowTerminate = TRUE;
	// Ȱ�������� �ʴٸ� �����带 ������Ŵ
	if(bWorking==FALSE) pThread->ResumeThread();

	::Sleep(1);

	if(pThread && pThread->m_hThread) {
		// ������ ���Ḧ 1�ʰ� ��ٸ�
		DWORD dwRetCode = ::WaitForSingleObject(pThread->m_hThread, 1000);

		if(dwRetCode == WAIT_OBJECT_0) {
			// �������� ���Ḧ �����Ͽ���
		}
		else if(dwRetCode == WAIT_TIMEOUT) {
			// �����尡 �ð����� ������ ������� ������ ������ �����Ų��
			::TerminateThread(pThread->m_hThread, 0);


			// �α� ���
			if(Work.WorkKind == 1)
			{
				CLogFile logfile("LogRecordDB.txt");
				logfile.Writef("LogRecDB ������ �������� : ID=%s, LoseMoney =%I64d, WinId=%s", Work.LogRecord.ID, Work.LogRecord.LoseMoney, Work.LogRecord.Win_ID);
				//logfile.Writef("DB ITEM ������ �������� : ID=%s, Exp=%d, Point=%d",
				//	Work.ID, -999, -999);//Work.UI.Exp, Work.UI.Point);
			}
		}
	}

	// ���� ó��
	bWorking = FALSE;
	pThread = NULL;

	return TRUE;
}



BOOL CLogManDBThread::CreateLogRecDB()
{
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
	if(pDatabase_LogRec && !pDatabase_LogRec->IsOpen())
	{
		if(m_pLogRecDB) { delete m_pLogRecDB; m_pLogRecDB=NULL; }
		delete pDatabase_LogRec;
		pDatabase_LogRec=NULL;
	}

	if(pDatabase_LogRec == NULL)
	{
		pDatabase_LogRec = new CDatabase;
		if(pDatabase_LogRec == NULL) return FALSE;

		TRY
		{
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_LogRec->SetLoginTimeout(60);
			pDatabase_LogRec->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CLogRecordDB::m_DSN, (LPCTSTR)CLogRecordDB::m_UID, (LPCTSTR)CLogRecordDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_LogRec->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile("LogRecordDB.txt");
			logfile.Writef("Database OpenEX ����(CLogRecordDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_LogRec;
			pDatabase_LogRec = NULL;
			return FALSE;
		}
		END_CATCH
	}

	/*
	if(m_pLogRecDB == NULL)
	{
		m_pLogRecDB = new CLogRecordDB(pDatabase_LogRec);
		if(m_pLogRecDB==NULL) return FALSE;
	}
	*/

	return TRUE;
}

// [ ����� ]
BOOL CLogManDBThread::CreateStatisticsLogDB()
{
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
	if(pDatabase_StatisticsRec && !pDatabase_StatisticsRec->IsOpen())
	{
		if(m_pStatisticsRecDB) { delete m_pStatisticsRecDB; m_pStatisticsRecDB=NULL; }
		delete pDatabase_StatisticsRec;
		pDatabase_StatisticsRec=NULL;
	}
	
	if(pDatabase_StatisticsRec == NULL)
	{
		pDatabase_StatisticsRec = new CDatabase;
		if(pDatabase_StatisticsRec == NULL) return FALSE;
		
		TRY
		{
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_StatisticsRec->SetLoginTimeout(60);
			pDatabase_StatisticsRec->SetQueryTimeout(60);
			
			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CStatisticsLogDB::m_DSN, (LPCTSTR)CStatisticsLogDB::m_UID, (LPCTSTR)CStatisticsLogDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_StatisticsRec->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile("StatisticsLogDB.txt");
			logfile.Writef("Database OpenEX ����(CStatisticsLogDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_StatisticsRec;
			pDatabase_StatisticsRec = NULL;
			return FALSE;
		}
		END_CATCH
	}
	
	return TRUE;
}

//2004.05.07 �������Ѿ� ����
BOOL CLogManDBThread::CreatePreUserLMLogDB()
{
//	if( !g_pMainView->Cfg.bPreUserLGMLogDB ) return FALSE;
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
	if(pDatabase_PreUserLMLogRec && !pDatabase_PreUserLMLogRec->IsOpen())
	{
		if(pDatabase_PreUserLMLogRec) { delete pDatabase_PreUserLMLogRec; pDatabase_PreUserLMLogRec=NULL; }
		delete pDatabase_PreUserLMLogRec;
		pDatabase_PreUserLMLogRec=NULL;
	}
	
	if(pDatabase_PreUserLMLogRec == NULL)
	{
		pDatabase_PreUserLMLogRec = new CDatabase;
		if(pDatabase_PreUserLMLogRec == NULL) return FALSE;
		
		TRY
		{
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_PreUserLMLogRec->SetLoginTimeout(60);
			pDatabase_PreUserLMLogRec->SetQueryTimeout(60);
			
			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CPre_UserLimitGMLogDB::m_DSN, (LPCTSTR)CPre_UserLimitGMLogDB::m_UID, (LPCTSTR)CPre_UserLimitGMLogDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_PreUserLMLogRec->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile("PreUserLMLogDB.txt");
			logfile.Writef("Database OpenEX ����(PreUserLMLogDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_PreUserLMLogRec;
			pDatabase_PreUserLMLogRec = NULL;
			return FALSE;
		}
		END_CATCH
	}
	
	return TRUE;

}

//[��ȣõ��]
BOOL CLogManDBThread::CreateAllinLogDB() 
{
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
	if(pDatabase_AllinChargeLog && !pDatabase_AllinChargeLog->IsOpen()) {
		if(m_pAllinChargeDB) { delete m_pAllinChargeDB; m_pAllinChargeDB=NULL; }
		delete pDatabase_AllinChargeLog;
		pDatabase_AllinChargeLog=NULL;
	}

	if(pDatabase_AllinChargeLog == NULL) {
		pDatabase_AllinChargeLog = new CDatabase;
		if(pDatabase_AllinChargeLog == NULL) return FALSE;

		TRY
		{
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_AllinChargeLog->SetLoginTimeout(60);
			pDatabase_AllinChargeLog->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CAllinCharge::m_DSN, (LPCTSTR)CAllinCharge::m_UID, (LPCTSTR)CAllinCharge::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_AllinChargeLog->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX ����(AllinDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_AllinChargeLog;
			pDatabase_AllinChargeLog = NULL;
			return FALSE;
		}
		END_CATCH
	}
	return TRUE;
}

void CLogManDBThread::DeleteAllDB()
{
	// ���� ī��Ʈ ��ġ �ʱ�ȭ
	ErrorCnt = 0;

	// DB ���ڵ�� Ŭ���� ����
	if(m_pLogRecDB) {
		if(m_pLogRecDB->IsOpen()) m_pLogRecDB->Close();
		delete(m_pLogRecDB);
		m_pLogRecDB = NULL;
	}

	// DB ���ڵ�� Ŭ���� ����
	if(m_pStatisticsRecDB) { // [ ����� ]
		if(m_pStatisticsRecDB->IsOpen()) m_pStatisticsRecDB->Close();
		delete(m_pStatisticsRecDB);
		m_pStatisticsRecDB = NULL;
	}

	// DB ���ڵ�� Ŭ���� ����
	if(m_pPreUserLMLogRecDB) { //2004.05.07 �������Ѿ� ����
		if(m_pPreUserLMLogRecDB->IsOpen()) m_pPreUserLMLogRecDB->Close();
		delete(m_pPreUserLMLogRecDB);
		m_pPreUserLMLogRecDB = NULL;
	}

	if(m_pAllinChargeDB){//[��ȣõ��]
		if(m_pAllinChargeDB->IsOpen()) m_pAllinChargeDB->Close();
		delete(m_pAllinChargeDB);
		m_pAllinChargeDB = NULL;
	}

	


	

	// �����ͺ��̽� Ŭ���� ����
	if(pDatabase_LogRec) {
		if(pDatabase_LogRec->IsOpen()) pDatabase_LogRec->Close();
		delete(pDatabase_LogRec);
		pDatabase_LogRec = NULL;
	}

	// �����ͺ��̽� Ŭ���� ����
	if(pDatabase_StatisticsRec) { // [ ����� ]
		if(pDatabase_StatisticsRec->IsOpen()) pDatabase_StatisticsRec->Close();
		delete(pDatabase_StatisticsRec);
		pDatabase_StatisticsRec = NULL;
	}

	// �����ͺ��̽� Ŭ���� ����
	if(pDatabase_PreUserLMLogRec) {  //2004.05.07 �������Ѿ� ����
		if(pDatabase_PreUserLMLogRec->IsOpen()) pDatabase_PreUserLMLogRec->Close();
		delete(pDatabase_PreUserLMLogRec);
		pDatabase_PreUserLMLogRec = NULL;
	}

	if(pDatabase_AllinChargeLog){//[��ȣõ��]
		if(pDatabase_AllinChargeLog->IsOpen()) pDatabase_AllinChargeLog->Close();
		delete(pDatabase_AllinChargeLog);
		pDatabase_AllinChargeLog = NULL;
	}	
}

UINT CLogManDBThread::LogRecDBThreadFunc(LPVOID pParam)
{
	if(pParam==NULL) return 0;
	CLogManDBThread *pThis = (CLogManDBThread*)pParam;

	while(TRUE)
	{
		BOOL bYesWork = pThis->pDBQue->DeQue(&pThis->Work);
	
		// �Ʒ��� �Լ����� ó���� DB�� ������ �������� Ȱ���� �����ȴ�
		// ��, ���� �÷��װ� ��Ʈ�� ���¿����� ��� �����Ͽ� ������ �� �ְ���
		if(bYesWork) {
			pThis->bWorking = TRUE;
			pThis->ProcessDBWork();
		}
		else if(!pThis->bNowTerminate) {
			pThis->bWorking = FALSE;
			// ������� ���� ����, ���� ���� ��ٸ���
			if(pThis->pThread && pThis->pThread->m_hThread) {
				pThis->pThread->SuspendThread();
			}
			else {
				// ������ �ڵ��� NULL�̶�� �����带 �����Ų��
				pThis->bNowTerminate = TRUE;
				break;
			}
		}

		// DBť�� ó���� �ϵ� ���� �����带 �����ؾ��� ���¶�� ������ ����
		if(pThis->bNowTerminate && bYesWork==FALSE) break;
		
		// ������ ���ָ� ����
		::Sleep(0);
	}

	// DB ���ڵ�� �ݱ� �� Ŭ���� ����
	pThis->DeleteAllDB();
	ZeroMemory(&pThis->Work, sizeof(pThis->Work));
	pThis->bWorking = FALSE;
	pThis->pThread = NULL;

	return 0;
}


void CLogManDBThread::ProcessDBWork()
{
	// �α� ��Ͽ�
	if(Work.WorkKind == 1) {
		AddNewLogrecordDB(&Work.LogRecord);
	}

	// [ ����� ]
	if(Work.WorkKind == 2) {

		AddNewStatisticsDB(&Work.StatisticsLog);
	}

	if(Work.WorkKind == 3) {
		AddNewPre_UsrLMLog(&Work.Pre_UserMLLog);
	}
	
	if(Work.WorkKind == 4) { // [��ȣõ��]
		AddNewAllin_Log( &Work.AllInCharge_Log);
	}

	UINT logintime = ::timeGetTime() - Work.StartTick;
	static UINT Logmaxtime = 0;
	if(Logmaxtime < logintime) Logmaxtime = logintime;
	
	// �ð� ���� �׽�Ʈ
	CString teststr;
	teststr.Format("LogAddNewDBTime=%d (Max=%d)", logintime, Logmaxtime);
	g_pMainView->GetDlgItem(IDC_STATIC_TEST2)->SetWindowText(teststr);

	
	// ȯ�漳������ �۾� �Ϸ��� DB Close �ɼ��� ����� ���
	// DB ���ڵ�� �ݱ� �� Ŭ���� ����
	//if(g_pMainView->Cfg2.bDBClose2 == TRUE) DeleteAllDB();
	if(ErrorCnt > 3) {
		if(Work.WorkKind == 1) {
			CLogFile logfile("LogRecordDB.txt");
			logfile.Write("--�α� ��Ͽ� DB ����ī��Ʈ ���ġ �ʰ��� DB���ؼ��� Close��");
		}
		else if(Work.WorkKind == 4) {
			CLogFile logfile("AllInLogDB.txt");
			logfile.Write("--AllInLogDB DB ����ī��Ʈ ���ġ �ʰ��� DB���ؼ��� Close��");

		}
		else {
			CLogFile logfile("StatisticsLogDB.txt");
			logfile.Write("--��� ��Ͽ� DB ����ī��Ʈ ���ġ �ʰ��� DB���ؼ��� Close��");
		}
		

		ErrorCnt = 0;
		DeleteAllDB();
	}
}


BOOL CLogManDBThread::AddNewLogrecordDB(LOGRECORD *pLog)
{
	if(pLog == NULL) return FALSE;

	if(!CreateLogRecDB()) 
	{
		CLogFile logfile("LogRecordDB.txt");
		logfile.Writef("AddNewLogrecordDB - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	CString query, tablename;

	tablename = g_pMainView->Cfg.LogRecTable;//"LogRecordDB";
	query.Format("INSERT INTO %s (GameCode, Status, My_ID, My_CurMoney, My_LoseMoney, Win_ID, Win_CurMoney, Win_GetMoney, My_IP, Winner_IP)\
		values (%d, %d, '%s', %I64d, %I64d, '%s', %I64d, %I64d, '%s', '%s')",\
		tablename, g_pMainView->Cfg.GameCode, pLog->Status, pLog->ID, pLog->CurMoney, pLog->LoseMoney, pLog->Win_ID, pLog->Win_CurMoney, pLog->Win_GetMoney, \
		pLog->My_ip, pLog->Winner_ip);

	TRY
	{
		pDatabase_LogRec->ExecuteSQL(query);
	}
	CATCH(CDBException, e)
	{
		if(e->m_nRetCode != AFX_SQL_ERROR_NO_ROWS_AFFECTED) {
			CLogFile logfile("LogRecordDB.txt");
			//logfile.Writef("LogRecordDB����(%s)\n - Query: %s", e->m_strError, query);
			logfile.Writef("--LogRecordDB����\n%s", query);
			//if(m_pLogRecDB->IsOpen()) m_pLogRecDB->Close();
			ErrorCnt++;	// ���� ī��Ʈ ����
			return FALSE;
		}
		else {
			CLogFile logfile("LogRecordDB.txt");
			logfile.Writef("���� ���� ���� ����(%s)\n - Query: %s", e->m_strError, query);
		}
	}
	END_CATCH

	return TRUE;

}

BOOL CLogManDBThread::AddNewStatisticsDB(STATISTICS_DBRECORD *p)
{
	if(p == NULL) return FALSE;
	if( !CreateStatisticsLogDB() )	
	{
		CLogFile logfile("StatisticsLogDB.txt");
		logfile.Writef("AddNewStatisticsDB - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	CString query=""; 
	CString tablename;
	tablename = g_pMainView->Cfg.StatisticsTable;

	query.Format("INSERT INTO %s (c_GameCode, c_ServerCode, c_Kind, c_BigData_0, c_BigData_1, c_BigData_2,\
	c_BigData_3, c_BigData_4, c_SmallData_0, c_SmallData_1, c_SmallData_2, c_SmallData_3, c_SmallData_4, c_SmallData_5,\
	c_SmallData_6, c_SmallData_7, c_SmallData_8, c_SmallData_9, c_SmallData_10, c_SmallData_11, c_SmallData_12,c_SmallData_13,\
	c_SmallData_14,	c_SmallData_15,	c_SmallData_16,	c_SmallData_17,	c_SmallData_18, c_SmallData_19, RegDate,c_extra ) values(\
	%d, %d, %d, %I64d, %I64d, %I64d, %I64d, %I64d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,%d, %f,  %d)",\
	tablename,g_pMainView->Cfg.GameCode, g_pMainView->Cfg.ServerCode, p->Kind, p->BigData[0], p->BigData[1],  p->BigData[2],\
	p->BigData[3], p->BigData[4], p->SmallData[0],p->SmallData[1],p->SmallData[2],p->SmallData[3],\
	p->SmallData[4],p->SmallData[5],p->SmallData[6],p->SmallData[7],p->SmallData[8],p->SmallData[9],\
	p->SmallData[10],p->SmallData[11],p->SmallData[12],p->SmallData[13],p->SmallData[14],p->SmallData[15],\
	p->SmallData[16],p->SmallData[17],p->SmallData[18], p->SmallData[19], p->LDate,  p->Extra);

	
	TRY
	{
		pDatabase_StatisticsRec->ExecuteSQL(query);
	}
	CATCH(CDBException, e)
	{
		if(e->m_nRetCode != AFX_SQL_ERROR_NO_ROWS_AFFECTED) {
			CLogFile logfile("StatisticsLogDB.txt");
			//logfile.Writef("--StatisticsLogDB %s\n%s",e->m_strError, query);
			logfile.Writef("--StatisticsLogDB \n%s", query);
			ErrorCnt++;	// ���� ī��Ʈ ����
			return FALSE;
		}
		else {
			CLogFile logfile("StatisticsLogDB.txt");
			logfile.Writef("���� ���� ���� ����(%s)\n - Query: %s", e->m_strError, query);
		}
	}
	END_CATCH
	return TRUE;
}

BOOL CLogManDBThread::AddNewPre_UsrLMLog(PRE_USERLIMITGMLOG *p)
{
	
	if(p == NULL) return FALSE;
	if( !CreatePreUserLMLogDB() )	
	{
		CLogFile logfile("PreUserLMLogDB.txt");
		logfile.Writef("AddNewPre_UsrLMLog - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	CString query=""; 
	CString tablename;
	tablename = g_pMainView->Cfg.PreUserLGMLogTable;

	query.Format("INSERT INTO %s (NO, ID, MoneyType, Amount, BeforeM, AfterM) values ('%s', '%s', %d, %I64d, %I64d, %I64d)",\
		tablename, p->NO, p->ID, p->MoneyType, p->Amount, p->BeforeM, p->AfterM);


	TRY
	{
		pDatabase_PreUserLMLogRec->ExecuteSQL(query);
	}
	CATCH(CDBException, e)
	{
		if(e->m_nRetCode != AFX_SQL_ERROR_NO_ROWS_AFFECTED) {
			CLogFile logfile("PreUserLMLogDB.txt");
			//logfile.Writef("--PreUserLMLogDB %s\n%s",e->m_strError, query);
			logfile.Writef("--PreUserLMLogDB \n%s",query);
			ErrorCnt++;	// ���� ī��Ʈ ����
			return FALSE;
		}
		else {
			CLogFile logfile("PreUserLMLogDB.txt");
			logfile.Writef("���� ���� ���� ����(%s)\n - Query: %s", e->m_strError, query);
		}
	}
	END_CATCH
	return TRUE;

}

BOOL CLogManDBThread::AddNewAllin_Log(ALLIN_LOGRECORD *p)
{
	if(p == NULL) return FALSE;
	if(!CreateAllinLogDB())	
	{
		CLogFile logfile("AllInLogDB.txt");
		logfile.Writef("AllInLogDB - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	CString query=""; 
	CString tablename;
	tablename = g_pMainView->Cfg.AllinChargeTable;

	query.Format("INSERT INTO %s (GAMECODE, ID, BeforeM, AMountM, AfterM) values (%d, '%s',%I64d, %I64d, %I64d)",\
		tablename, g_pMainView->Cfg.GameCode, p->ID, p->BeforeMoney,p->AMountMoney,p->AfterMoney);


	TRY
	{
		pDatabase_AllinChargeLog->ExecuteSQL(query);
	}
	CATCH(CDBException, e)
	{
		if(e->m_nRetCode != AFX_SQL_ERROR_NO_ROWS_AFFECTED) {
			CLogFile logfile("AllInLogDB.txt");
			logfile.Writef("--AllInLogDB \n%s",query);
			ErrorCnt++;	// ���� ī��Ʈ ����
			return FALSE;
		}
		else {
			CLogFile logfile("AllInLogDB.txt");
			logfile.Writef("���� ���� ���� ����(%s)\n - Query: %s", e->m_strError, query);
		}
	}
	END_CATCH
	return TRUE;
}



/*
BOOL CLogManDBThread::AddNewLogrecordDB(LOGRECORD *pLog)
{
	if(pLog == NULL) return FALSE;

	if(!CreateLogRecDB()) 
	{
		CLogFile logfile("LogRecordDB.txt");
		logfile.Writef("AddNewLogrecordDB - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pLogRecDB==NULL) return FALSE;

//	CString str;
//	str.Format("[My_ID] = '%s'", pLog->ID);
// SELECT * FROM ���̺�� WHERE �ʵ�� IN (SELECT TOP 1 �ʵ�� FROM ���̺��)

	TRY
	{
		// ���ڵ���� ���������� �ݴ´�
		if(m_pLogRecDB->IsOpen()) m_pLogRecDB->Close();
		//m_pLogRecDB->m_strFilter = str;
		m_pLogRecDB->m_strFilter = "[No] = 0";
		if(!m_pLogRecDB->IsOpen()) m_pLogRecDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("LogRecordDB.txt");
		logfile.Writef("LogRecordDB ���� - AddNewGameDB Open : %s", e->m_strError.operator LPCTSTR());
		ErrorCnt++;	// ���� ī��Ʈ ����
		return FALSE;
	}
	END_CATCH


	// ���� ID�� �����ϸ� �߰� ���
//	if(!m_pGameDB->IsEOF() && m_pGameDB->m_ID.CompareNoCase(pUI->ID) == 0) {
//		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
//		return FALSE;
//	}

	// DB�� �߰��ϴ� ����
	TRY
	{
		if(!m_pLogRecDB->IsEOF()) m_pLogRecDB->MoveLast();
		m_pLogRecDB->AddNew();
		
		// DB�߰�
		//if(pLog->Status == 0)
		//	m_pLogRecDB->m_Status.Format("����");//????
		//else
		//	m_pLogRecDB->m_Status.Format("����");//????
		m_pLogRecDB->m_Status = pLog->Status; // 1 ���� 2 ����

		m_pLogRecDB->m_MyID.Format("%s", pLog->ID);

		CString strPM; strPM.Format(_T("%I64d"),pLog->CurMoney);
		m_pLogRecDB->m_MyCurMoney = strPM;

		strPM.Format(_T("%I64d"), pLog->LoseMoney);
		m_pLogRecDB->m_MyLoseMoney = strPM;

		m_pLogRecDB->m_WinnerID.Format("%s", pLog->Win_ID);

		strPM.Format(_T("%I64d"), pLog->Win_CurMoney);
		m_pLogRecDB->m_WinCurMoney = strPM;

		strPM.Format(_T("%I64d"), pLog->Win_GetMoney);
		m_pLogRecDB->m_WinGetMoney = strPM;

		m_pLogRecDB->m_My_IP.Format("%s", pLog->My_ip);
		m_pLogRecDB->m_Winner_IP.Format("%s", pLog->Winner_ip);

		//strPM.Format("%d:%d:%d:%d", pLog->time.GetMonth(),pLog->time.GetDay, pLog->time.GetHour,
		//	pLog->time.GetMinute);
		//m_pLogRecDB->m_Date.Format("%d:%d:%d:%d", pLog->time.GetMonth(),pLog->time.GetDay(), pLog->time.GetHour(),pLog->time.GetMinute());
		
		//m_pLogRecDB->m_Date = COleDateTime::GetCurrentTime();
		//m_pLogRecDB->m_Date.GetAsSystemTime(pLog->time); // DB-> �б�

		m_pLogRecDB->Update();
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("LogRecordDB.txt");
		logfile.Writef("LogRecordDB ���� - AddNewGameDB Update (ID=%s) : %s", pLog->ID, e->m_strError.operator LPCTSTR());
		
		if(m_pLogRecDB->IsOpen()) m_pLogRecDB->Close();
		ErrorCnt++;	// ���� ī��Ʈ ����
		return FALSE;
	}
	END_CATCH

	return TRUE;

}
*/


