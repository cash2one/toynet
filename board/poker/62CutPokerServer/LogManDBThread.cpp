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
// DB큐 클래스
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

	// DB큐의 최대 개수는 최대 소켓 개수의 1.25배 정도로 설정
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
	
	// Database 클래스
	pDatabase_LogRec = NULL;
	pDatabase_StatisticsRec = NULL;// [ 통계기록 ]
	pDatabase_PreUserLMLogRec = NULL;
	pDatabase_AllinChargeLog = NULL; //수호천사

	// DB 레코드셋 클래스 포인터
	m_pLogRecDB = NULL;
	m_pStatisticsRecDB = NULL;// [ 통계기록 ]
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
		// 쓰레드를 서스펜드 모드로 생성한다(쓰레드 프로시저에서 사용될 포인터의 초기화를 기다려야하기때문)
		pThread = ::AfxBeginThread(LogRecDBThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
		if(pThread == NULL)
		{
			DeleteAllDB(); 
			bWorking = FALSE;
			ZeroMemory(&Work, sizeof(Work));
			return FALSE;
		}
		// 쓰레드 가동
		bWorking = TRUE;
		Work.StartTick = timeGetTime();
		pThread->ResumeThread();
	}
	else {
		// 쓰레드 가동
		bWorking = TRUE;
		Work.StartTick = timeGetTime();
		pThread->ResumeThread();
	}

	return TRUE;
}


BOOL CLogManDBThread::Terminate()
{
	if(pThread==NULL) return FALSE;

	// 쓰레드 종료 플래그 세팅
	bNowTerminate = TRUE;
	// 활동중이지 않다면 쓰레드를 가동시킴
	if(bWorking==FALSE) pThread->ResumeThread();

	::Sleep(1);

	if(pThread && pThread->m_hThread) {
		// 쓰레드 종료를 1초간 기다림
		DWORD dwRetCode = ::WaitForSingleObject(pThread->m_hThread, 1000);

		if(dwRetCode == WAIT_OBJECT_0) {
			// 정상적인 종료를 수행하였음
		}
		else if(dwRetCode == WAIT_TIMEOUT) {
			// 쓰레드가 시간내에 저절로 종료되지 않으면 강제로 종료시킨다
			::TerminateThread(pThread->m_hThread, 0);


			// 로그 기록
			if(Work.WorkKind == 1)
			{
				CLogFile logfile("LogRecordDB.txt");
				logfile.Writef("LogRecDB 쓰레드 강제종료 : ID=%s, LoseMoney =%I64d, WinId=%s", Work.LogRecord.ID, Work.LogRecord.LoseMoney, Work.LogRecord.Win_ID);
				//logfile.Writef("DB ITEM 쓰레드 강제종료 : ID=%s, Exp=%d, Point=%d",
				//	Work.ID, -999, -999);//Work.UI.Exp, Work.UI.Point);
			}
		}
	}

	// 종결 처리
	bWorking = FALSE;
	pThread = NULL;

	return TRUE;
}



BOOL CLogManDBThread::CreateLogRecDB()
{
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
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
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_LogRec->SetLoginTimeout(60);
			pDatabase_LogRec->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CLogRecordDB::m_DSN, (LPCTSTR)CLogRecordDB::m_UID, (LPCTSTR)CLogRecordDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_LogRec->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile("LogRecordDB.txt");
			logfile.Writef("Database OpenEX 에러(CLogRecordDB) : %s", e->m_strError.operator LPCTSTR());
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

// [ 통계기록 ]
BOOL CLogManDBThread::CreateStatisticsLogDB()
{
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
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
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_StatisticsRec->SetLoginTimeout(60);
			pDatabase_StatisticsRec->SetQueryTimeout(60);
			
			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CStatisticsLogDB::m_DSN, (LPCTSTR)CStatisticsLogDB::m_UID, (LPCTSTR)CStatisticsLogDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_StatisticsRec->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile("StatisticsLogDB.txt");
			logfile.Writef("Database OpenEX 에러(CStatisticsLogDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_StatisticsRec;
			pDatabase_StatisticsRec = NULL;
			return FALSE;
		}
		END_CATCH
	}
	
	return TRUE;
}

//2004.05.07 리더스총액 제한
BOOL CLogManDBThread::CreatePreUserLMLogDB()
{
//	if( !g_pMainView->Cfg.bPreUserLGMLogDB ) return FALSE;
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
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
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_PreUserLMLogRec->SetLoginTimeout(60);
			pDatabase_PreUserLMLogRec->SetQueryTimeout(60);
			
			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CPre_UserLimitGMLogDB::m_DSN, (LPCTSTR)CPre_UserLimitGMLogDB::m_UID, (LPCTSTR)CPre_UserLimitGMLogDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_PreUserLMLogRec->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile("PreUserLMLogDB.txt");
			logfile.Writef("Database OpenEX 에러(PreUserLMLogDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_PreUserLMLogRec;
			pDatabase_PreUserLMLogRec = NULL;
			return FALSE;
		}
		END_CATCH
	}
	
	return TRUE;

}

//[수호천사]
BOOL CLogManDBThread::CreateAllinLogDB() 
{
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
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
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_AllinChargeLog->SetLoginTimeout(60);
			pDatabase_AllinChargeLog->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CAllinCharge::m_DSN, (LPCTSTR)CAllinCharge::m_UID, (LPCTSTR)CAllinCharge::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_AllinChargeLog->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX 에러(AllinDB) : %s", e->m_strError.operator LPCTSTR());
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
	// 에러 카운트 수치 초기화
	ErrorCnt = 0;

	// DB 레코드셋 클래스 삭제
	if(m_pLogRecDB) {
		if(m_pLogRecDB->IsOpen()) m_pLogRecDB->Close();
		delete(m_pLogRecDB);
		m_pLogRecDB = NULL;
	}

	// DB 레코드셋 클래스 삭제
	if(m_pStatisticsRecDB) { // [ 통계기록 ]
		if(m_pStatisticsRecDB->IsOpen()) m_pStatisticsRecDB->Close();
		delete(m_pStatisticsRecDB);
		m_pStatisticsRecDB = NULL;
	}

	// DB 레코드셋 클래스 삭제
	if(m_pPreUserLMLogRecDB) { //2004.05.07 리더스총액 제한
		if(m_pPreUserLMLogRecDB->IsOpen()) m_pPreUserLMLogRecDB->Close();
		delete(m_pPreUserLMLogRecDB);
		m_pPreUserLMLogRecDB = NULL;
	}

	if(m_pAllinChargeDB){//[수호천사]
		if(m_pAllinChargeDB->IsOpen()) m_pAllinChargeDB->Close();
		delete(m_pAllinChargeDB);
		m_pAllinChargeDB = NULL;
	}

	


	

	// 데이터베이스 클래스 삭제
	if(pDatabase_LogRec) {
		if(pDatabase_LogRec->IsOpen()) pDatabase_LogRec->Close();
		delete(pDatabase_LogRec);
		pDatabase_LogRec = NULL;
	}

	// 데이터베이스 클래스 삭제
	if(pDatabase_StatisticsRec) { // [ 통계기록 ]
		if(pDatabase_StatisticsRec->IsOpen()) pDatabase_StatisticsRec->Close();
		delete(pDatabase_StatisticsRec);
		pDatabase_StatisticsRec = NULL;
	}

	// 데이터베이스 클래스 삭제
	if(pDatabase_PreUserLMLogRec) {  //2004.05.07 리더스총액 제한
		if(pDatabase_PreUserLMLogRec->IsOpen()) pDatabase_PreUserLMLogRec->Close();
		delete(pDatabase_PreUserLMLogRec);
		pDatabase_PreUserLMLogRec = NULL;
	}

	if(pDatabase_AllinChargeLog){//[수호천사]
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
	
		// 아래의 함수에서 처리할 DB가 없으면 쓰레드의 활동이 정지된다
		// 단, 종료 플래그가 세트된 상태에서는 계속 수행하여 종료할 수 있게함
		if(bYesWork) {
			pThis->bWorking = TRUE;
			pThis->ProcessDBWork();
		}
		else if(!pThis->bNowTerminate) {
			pThis->bWorking = FALSE;
			// 서스펜드 모드로 진입, 다음 일을 기다린다
			if(pThis->pThread && pThis->pThread->m_hThread) {
				pThis->pThread->SuspendThread();
			}
			else {
				// 쓰레드 핸들이 NULL이라면 쓰레드를 종료시킨다
				pThis->bNowTerminate = TRUE;
				break;
			}
		}

		// DB큐에 처리할 일도 없고 쓰레드를 종료해야할 상태라면 쓰레드 종료
		if(pThis->bNowTerminate && bYesWork==FALSE) break;
		
		// 쓰레드 폭주를 방지
		::Sleep(0);
	}

	// DB 레코드셋 닫기 및 클래스 삭제
	pThis->DeleteAllDB();
	ZeroMemory(&pThis->Work, sizeof(pThis->Work));
	pThis->bWorking = FALSE;
	pThis->pThread = NULL;

	return 0;
}


void CLogManDBThread::ProcessDBWork()
{
	// 로그 기록용
	if(Work.WorkKind == 1) {
		AddNewLogrecordDB(&Work.LogRecord);
	}

	// [ 통계기록 ]
	if(Work.WorkKind == 2) {

		AddNewStatisticsDB(&Work.StatisticsLog);
	}

	if(Work.WorkKind == 3) {
		AddNewPre_UsrLMLog(&Work.Pre_UserMLLog);
	}
	
	if(Work.WorkKind == 4) { // [수호천사]
		AddNewAllin_Log( &Work.AllInCharge_Log);
	}

	UINT logintime = ::timeGetTime() - Work.StartTick;
	static UINT Logmaxtime = 0;
	if(Logmaxtime < logintime) Logmaxtime = logintime;
	
	// 시간 측정 테스트
	CString teststr;
	teststr.Format("LogAddNewDBTime=%d (Max=%d)", logintime, Logmaxtime);
	g_pMainView->GetDlgItem(IDC_STATIC_TEST2)->SetWindowText(teststr);

	
	// 환경설정에서 작업 완료후 DB Close 옵션이 적용된 경우
	// DB 레코드셋 닫기 및 클래스 삭제
	//if(g_pMainView->Cfg2.bDBClose2 == TRUE) DeleteAllDB();
	if(ErrorCnt > 3) {
		if(Work.WorkKind == 1) {
			CLogFile logfile("LogRecordDB.txt");
			logfile.Write("--로그 기록용 DB 에러카운트 허용치 초과로 DB컨넥션을 Close함");
		}
		else if(Work.WorkKind == 4) {
			CLogFile logfile("AllInLogDB.txt");
			logfile.Write("--AllInLogDB DB 에러카운트 허용치 초과로 DB컨넥션을 Close함");

		}
		else {
			CLogFile logfile("StatisticsLogDB.txt");
			logfile.Write("--통계 기록용 DB 에러카운트 허용치 초과로 DB컨넥션을 Close함");
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
		logfile.Writef("AddNewLogrecordDB - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
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
			//logfile.Writef("LogRecordDB에러(%s)\n - Query: %s", e->m_strError, query);
			logfile.Writef("--LogRecordDB에러\n%s", query);
			//if(m_pLogRecDB->IsOpen()) m_pLogRecDB->Close();
			ErrorCnt++;	// 에러 카운트 증가
			return FALSE;
		}
		else {
			CLogFile logfile("LogRecordDB.txt");
			logfile.Writef("영향 받은 행이 없음(%s)\n - Query: %s", e->m_strError, query);
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
		logfile.Writef("AddNewStatisticsDB - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
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
			ErrorCnt++;	// 에러 카운트 증가
			return FALSE;
		}
		else {
			CLogFile logfile("StatisticsLogDB.txt");
			logfile.Writef("영향 받은 행이 없음(%s)\n - Query: %s", e->m_strError, query);
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
		logfile.Writef("AddNewPre_UsrLMLog - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
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
			ErrorCnt++;	// 에러 카운트 증가
			return FALSE;
		}
		else {
			CLogFile logfile("PreUserLMLogDB.txt");
			logfile.Writef("영향 받은 행이 없음(%s)\n - Query: %s", e->m_strError, query);
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
		logfile.Writef("AllInLogDB - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
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
			ErrorCnt++;	// 에러 카운트 증가
			return FALSE;
		}
		else {
			CLogFile logfile("AllInLogDB.txt");
			logfile.Writef("영향 받은 행이 없음(%s)\n - Query: %s", e->m_strError, query);
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
		logfile.Writef("AddNewLogrecordDB - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pLogRecDB==NULL) return FALSE;

//	CString str;
//	str.Format("[My_ID] = '%s'", pLog->ID);
// SELECT * FROM 테이블명 WHERE 필드명 IN (SELECT TOP 1 필드명 FROM 테이블명)

	TRY
	{
		// 레코드셋이 열려있으면 닫는다
		if(m_pLogRecDB->IsOpen()) m_pLogRecDB->Close();
		//m_pLogRecDB->m_strFilter = str;
		m_pLogRecDB->m_strFilter = "[No] = 0";
		if(!m_pLogRecDB->IsOpen()) m_pLogRecDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("LogRecordDB.txt");
		logfile.Writef("LogRecordDB 에러 - AddNewGameDB Open : %s", e->m_strError.operator LPCTSTR());
		ErrorCnt++;	// 에러 카운트 증가
		return FALSE;
	}
	END_CATCH


	// 같은 ID가 존재하면 추가 취소
//	if(!m_pGameDB->IsEOF() && m_pGameDB->m_ID.CompareNoCase(pUI->ID) == 0) {
//		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
//		return FALSE;
//	}

	// DB를 추가하는 상태
	TRY
	{
		if(!m_pLogRecDB->IsEOF()) m_pLogRecDB->MoveLast();
		m_pLogRecDB->AddNew();
		
		// DB추가
		//if(pLog->Status == 0)
		//	m_pLogRecDB->m_Status.Format("정상");//????
		//else
		//	m_pLogRecDB->m_Status.Format("폴드");//????
		m_pLogRecDB->m_Status = pLog->Status; // 1 정상 2 폴드

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
		//m_pLogRecDB->m_Date.GetAsSystemTime(pLog->time); // DB-> 읽기

		m_pLogRecDB->Update();
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("LogRecordDB.txt");
		logfile.Writef("LogRecordDB 에러 - AddNewGameDB Update (ID=%s) : %s", pLog->ID, e->m_strError.operator LPCTSTR());
		
		if(m_pLogRecDB->IsOpen()) m_pLogRecDB->Close();
		ErrorCnt++;	// 에러 카운트 증가
		return FALSE;
	}
	END_CATCH

	return TRUE;

}
*/


