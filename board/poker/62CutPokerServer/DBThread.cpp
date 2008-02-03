// DBThread.cpp: implementation of the CDBThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "62CutPokerServerView.h"
#include "LogFile.h"


#pragma comment(lib, "winmm.lib")

#define CONNECTSTATUS_UPDATE_DELAY	120000
static UINT g_LastConnectStatusUpdateTime = 0;



// [SSO 작업] - CDBQue 클래스 전체를 바꾼다...

//////////////////////////////////////////////////////////////////////
// DB큐 클래스
//////////////////////////////////////////////////////////////////////

CDBQue::CDBQue()
{
	Work.clear();
}

CDBQue::~CDBQue()
{
	while(Work.size()) 
	{
		delete Work.front();
		Work.pop_front();
	}
	Work.clear();
}

BOOL CDBQue::EnQue(DBWORKS *pNewWork)
{
	DBQueLock.Lock();

	// DB큐의 최대 개수는 최대 소켓 개수의 1.25배 정도로 설정
	int max_dbworknum = MAX_DATA_SOCKET + (MAX_DATA_SOCKET>>2);

	if(Work.size() >= max_dbworknum) { 
		DBQueLock.Unlock(); 
		return FALSE; 
	}

	DBWORKS* pnew = new DBWORKS;
	memcpy(pnew, pNewWork, sizeof(DBWORKS));
	
	Work.push_back(pnew);	
	
	DBQueLock.Unlock();

	return TRUE;
}

BOOL CDBQue::DeQue(DBWORKS *pWork)
{
	DBQueLock.Lock();

	if(Work.size() == 0) {
		DBQueLock.Unlock();
		return FALSE;
	}

	memcpy(pWork, Work.front(), sizeof(DBWORKS));
	pWork->StartTick = timeGetTime();
	
	delete Work.front();
	Work.pop_front();

	DBQueLock.Unlock();

	return TRUE;
}

int CDBQue::GetTotNum()
{
	int tot = 0;
	DBQueLock.Lock();
	tot = Work.size();
	DBQueLock.Unlock();
	return tot;
}


/*
CDBQue::CDBQue()
{
	SPos = EPos = 0;
	TotNum = 0;
	memset(Work, 0, sizeof(Work));
}

CDBQue::~CDBQue()
{

}

BOOL CDBQue::EnQue(DBWORKS *pNewWork)
{
	DBQueLock.Lock();

	if(TotNum >= MAX_DBWORK) { DBQueLock.Unlock(); return FALSE; }
	
	memcpy(&Work[EPos], pNewWork, sizeof(DBWORKS));
	EPos++;
	if(EPos >= MAX_DBWORK) EPos = EPos - MAX_DBWORK;
	TotNum++;
	
	DBQueLock.Unlock();
	return TRUE;
}

BOOL CDBQue::DeQue(DBWORKS *pWork)
{
	DBQueLock.Lock();

	if(TotNum <= 0 || SPos == EPos) {
		DBQueLock.Unlock();
		return FALSE;
	}

	memcpy(pWork, &Work[SPos], sizeof(DBWORKS));
	pWork->StartTick = timeGetTime();
	ZeroMemory(&Work[SPos], sizeof(DBWORKS));
	SPos++;
	if(SPos >= MAX_DBWORK) SPos = SPos - MAX_DBWORK;
	TotNum--;

	DBQueLock.Unlock();

	return TRUE;
}

int CDBQue::GetTotNum()
{
	int tot = 0;
	DBQueLock.Lock();
	tot = TotNum;
	DBQueLock.Unlock();
	return tot;
}
*/


//////////////////////////////////////////////////////////////////////
// DB 처리 쓰레드 클래스
//////////////////////////////////////////////////////////////////////

CDBThread::CDBThread()
{
	pThread = NULL;
	pDBQue = NULL;

	bNowTerminate = FALSE;
	bWorking = FALSE;
	ErrorCnt = 0;

	ZeroMemory(&Work, sizeof(DBWORKS));

	pDatabase_User = NULL;
	pDatabase_Game = NULL;
	pDatabase_Status = NULL;
	pDatabase_Avatar = NULL;
	pDatabase_Item = NULL;		// [게임 아이템 작업]
	pDatabase_Premium = NULL;	// [프리미엄 작업]
	pDatabase_PremIP = NULL;	// [PC방 작업]
	pDatabase_EventPrize = NULL; // ### [이벤트 티켓] ###

	m_pUserDB = NULL;
	m_pGameDB = NULL;
	m_pStatusDB = NULL;
	m_pAvatarDB = NULL;
	m_pItemDB = NULL;		// [게임 아이템 작업]
	m_pPremiumDB = NULL;	// [프리미엄 작업]
	m_pPremIPDB = NULL;		// [PC방 작업]
	m_pEventPrizeDB= NULL;  // ### [이벤트 티켓] ###
}

CDBThread::~CDBThread()
{
	DeleteAllDB();
}

BOOL CDBThread::Begin(CDBQue *pQue)
{
	if(pQue == NULL || bWorking == TRUE)  return FALSE;

	pDBQue = pQue;

	if(pThread == NULL)	{
		// 쓰레드를 서스펜드 모드로 생성한다(쓰레드 프로시저에서 사용될 포인터의 초기화를 기다려야하기때문)
		pThread = ::AfxBeginThread(DBThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
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

BOOL CDBThread::Terminate()
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
			CString strFileName = GetLogFileName();
			if(strFileName!=_T("")){
				CLogFile logfile(strFileName.operator LPCTSTR());
				logfile.Writef(_T("Terminate active DB Thread! : ID=[%s],  Win=%d, Fail=%d, AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d"),
					Work.UI.ID,  Work.UI.WinNum, Work.UI.LooseNum,   (INT64)Max(Work.UI.PMoney,0)-Work.OI.pBackup, Work.UI.PI.nRSF, Work.UI.PI.nSF, Work.UI.PI.nFCARD);
			}
			// 로그 기록
			//CLogFile logfile;
			//logfile.Writef("DB 쓰레드 강제종료 : ID=%s, Marble=%d, Win=%d, Loose=%d, Point=%d",
			//	Work.UI.ID, Work.UI.Marble, Work.UI.WinNum, Work.UI.LooseNum, Work.UI.Point);
		}
	}

	// 종결 처리
	bWorking = FALSE;
	pThread = NULL;

	return TRUE;
}

BOOL CDBThread::CreateUserDB()
{
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
	if(pDatabase_User && !pDatabase_User->IsOpen())
	{
		if(m_pUserDB) { delete m_pUserDB; m_pUserDB=NULL; }
		delete pDatabase_User;
		pDatabase_User=NULL;
	}

	if(pDatabase_User == NULL)
	{
		pDatabase_User = new CDatabase;
		if(pDatabase_User == NULL) return FALSE;

		TRY
		{
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_User->SetLoginTimeout(60);
			pDatabase_User->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CUserDB::m_DSN, (LPCTSTR)CUserDB::m_UID, (LPCTSTR)CUserDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_User->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX 에러(UserDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_User;
			pDatabase_User = NULL;
			return FALSE;
		}
		END_CATCH
	}

	if(m_pUserDB == NULL)
	{
		m_pUserDB = new CUserDB(pDatabase_User);
		if(m_pUserDB==NULL) return FALSE;
	}
	return TRUE;
}

BOOL CDBThread::CreateGameDB()
{
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
	if(pDatabase_Game && !pDatabase_Game->IsOpen())
	{
		if(m_pGameDB) { delete m_pGameDB; m_pGameDB=NULL; }
		delete pDatabase_Game;
		pDatabase_Game=NULL;
	}

	if(pDatabase_Game==NULL)
	{
		pDatabase_Game = new CDatabase;
		if(pDatabase_Game == NULL) return FALSE;

		TRY
		{
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_Game->SetLoginTimeout(60);
			pDatabase_Game->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CGameDB::m_DSN, (LPCTSTR)CGameDB::m_UID, (LPCTSTR)CGameDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_Game->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX 에러(GameDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_Game;
			pDatabase_Game = NULL;
			return FALSE;
		}
		END_CATCH
	}

	if(m_pGameDB == NULL)
	{
		m_pGameDB = new CGameDB(pDatabase_Game);
		if(m_pGameDB==NULL) return FALSE;
	}
	return TRUE;
}

BOOL CDBThread::CreateStatusDB()
{
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
	if(pDatabase_Status && !pDatabase_Status->IsOpen())
	{
		if(m_pStatusDB) { delete m_pStatusDB; m_pStatusDB=NULL; }
		delete pDatabase_Status;
		pDatabase_Status=NULL;
	}

	if(pDatabase_Status == NULL)
	{
		pDatabase_Status = new CDatabase;
		if(pDatabase_Status == NULL) return FALSE;

		TRY
		{
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_Status->SetLoginTimeout(60);
			pDatabase_Status->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CStatusDB::m_DSN, (LPCTSTR)CStatusDB::m_UID, (LPCTSTR)CStatusDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_Status->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX 에러(StatusDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_Status;
			pDatabase_Status = NULL;
			return FALSE;
		}
		END_CATCH
	}

	if(m_pStatusDB == NULL)
	{
		m_pStatusDB = new CStatusDB(pDatabase_Status);
		if(m_pStatusDB==NULL) return FALSE;
	}
	return TRUE;
}

BOOL CDBThread::CreateAvatarDB()
{
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
	if(pDatabase_Avatar && !pDatabase_Avatar->IsOpen())
	{
		if(m_pAvatarDB) { delete m_pAvatarDB; m_pAvatarDB=NULL; }
		delete pDatabase_Avatar;
		pDatabase_Avatar=NULL;
	}

	if(pDatabase_Avatar == NULL)
	{
		pDatabase_Avatar = new CDatabase;
		if(pDatabase_Avatar == NULL) return FALSE;

		TRY
		{
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_Avatar->SetLoginTimeout(60);
			pDatabase_Avatar->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CAvatarDB::m_DSN, (LPCTSTR)CAvatarDB::m_UID, (LPCTSTR)CAvatarDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_Avatar->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX 에러(AvatarDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_Avatar;
			pDatabase_Avatar = NULL;
			return FALSE;
		}
		END_CATCH
	}

	if(m_pAvatarDB == NULL)
	{
		m_pAvatarDB = new CAvatarDB(pDatabase_Avatar);
		if(m_pAvatarDB==NULL) return FALSE;
	}
	return TRUE;
}

// [게임 아이템 작업]
BOOL CDBThread::CreateItemDB()
{
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
	if(pDatabase_Item && !pDatabase_Item->IsOpen()) {
		if(m_pItemDB) { delete m_pItemDB; m_pItemDB=NULL; }
		delete pDatabase_Item;
		pDatabase_Item=NULL;
	}

	if(pDatabase_Item == NULL) {
		pDatabase_Item = new CDatabase;
		if(pDatabase_Item == NULL) return FALSE;

		TRY
		{
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_Item->SetLoginTimeout(60);
			pDatabase_Item->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CItemDB::m_DSN, (LPCTSTR)CItemDB::m_UID, (LPCTSTR)CItemDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_Item->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX 에러(ItemDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_Item;
			pDatabase_Item = NULL;
			return FALSE;
		}
		END_CATCH
	}

	if(m_pItemDB == NULL) {
		m_pItemDB = new CItemDB(pDatabase_Item);
		if(m_pItemDB==NULL) return FALSE;
	}
	return TRUE;
}

// [프리미엄 작업]
BOOL CDBThread::CreatePremiumDB()
{
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
	if(pDatabase_Premium && !pDatabase_Premium->IsOpen()) {
		if(m_pPremiumDB) { delete m_pPremiumDB; m_pPremiumDB=NULL; }
		delete pDatabase_Premium;
		pDatabase_Premium=NULL;
	}

	if(pDatabase_Premium == NULL) {
		pDatabase_Premium = new CDatabase;
		if(pDatabase_Premium == NULL) return FALSE;

		TRY
		{
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_Premium->SetLoginTimeout(60);
			pDatabase_Premium->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CPremiumDB::m_DSN, (LPCTSTR)CPremiumDB::m_UID, (LPCTSTR)CPremiumDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_Premium->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX 에러(PremiumDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_Premium;
			pDatabase_Premium = NULL;
			return FALSE;
		}
		END_CATCH
	}

	if(m_pPremiumDB == NULL) {
		m_pPremiumDB = new CPremiumDB(pDatabase_Premium);
		if(m_pPremiumDB==NULL) return FALSE;
	}
	return TRUE;
}

// [PC방 작업]
BOOL CDBThread::CreatePremIPDB()
{
	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
	if(pDatabase_PremIP && !pDatabase_PremIP->IsOpen()) {
		if(m_pPremIPDB) { delete m_pPremIPDB; m_pPremIPDB=NULL; }
		delete pDatabase_PremIP;
		pDatabase_PremIP=NULL;
	}

	if(pDatabase_PremIP == NULL) {
		pDatabase_PremIP = new CDatabase;
		if(pDatabase_PremIP == NULL) return FALSE;

		TRY
		{
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_PremIP->SetLoginTimeout(60);
			pDatabase_PremIP->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CPremIPDB::m_DSN, (LPCTSTR)CPremIPDB::m_UID, (LPCTSTR)CPremIPDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_PremIP->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX 에러(PremIPDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_PremIP;
			pDatabase_PremIP = NULL;
			return FALSE;
		}
		END_CATCH
	}

	if(m_pPremIPDB == NULL) {
		m_pPremIPDB = new CPremIPDB(pDatabase_PremIP);
		if(m_pPremIPDB==NULL) return FALSE;
	}
	return TRUE;
}

// ### [이벤트 티켓] ###
BOOL CDBThread::CreateEventPrizeDB()
{
	if( !g_pMainView->Cfg.bEventDB ) return FALSE;

	// 클래스가 할당이 되어있더라도 오픈되어있지 않다면 클래스 삭제
	if(pDatabase_EventPrize && !pDatabase_EventPrize->IsOpen()) {
		if(m_pEventPrizeDB) { delete m_pEventPrizeDB; m_pEventPrizeDB=NULL; }
		delete pDatabase_EventPrize;
		pDatabase_EventPrize=NULL;
	}

	if(pDatabase_EventPrize == NULL) {
		pDatabase_EventPrize = new CDatabase;
		if(pDatabase_EventPrize == NULL) return FALSE;

		TRY
		{
			// 로그인 및 쿼리 타임아웃 설정
			pDatabase_EventPrize->SetLoginTimeout(60);
			pDatabase_EventPrize->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CEventPrizeDB::m_DSN, (LPCTSTR)CEventPrizeDB::m_UID, (LPCTSTR)CEventPrizeDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			pDatabase_EventPrize->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX 에러(EventPrizeDB) : %s", e->m_strError.operator LPCTSTR());
			delete pDatabase_EventPrize;
			pDatabase_EventPrize = NULL;
			return FALSE;
		}
		END_CATCH
	}

	if(m_pEventPrizeDB == NULL) {
		m_pEventPrizeDB = new CEventPrizeDB(pDatabase_EventPrize);
		if(m_pEventPrizeDB==NULL) return FALSE;
	}
	return TRUE;

}

void CDBThread::DeleteAllDB()
{
	// 에러 카운트 수치 초기화
	ErrorCnt = 0;

	// DB 레코드셋 클래스 삭제
	if(m_pStatusDB) {
		if(m_pStatusDB->IsOpen()) m_pStatusDB->Close();
		delete(m_pStatusDB);
		m_pStatusDB = NULL;
	}
	if(m_pGameDB) {
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		delete(m_pGameDB);
		m_pGameDB = NULL;
	}
	if(m_pAvatarDB) {
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		delete(m_pAvatarDB);
		m_pAvatarDB = NULL;
	}
	if(m_pItemDB) {	// [게임 아이템 작업]
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		delete(m_pItemDB);
		m_pItemDB = NULL;
	}
	if(m_pPremiumDB) {	// [프리미엄 작업]
		if(m_pPremiumDB->IsOpen()) m_pPremiumDB->Close();
		delete(m_pPremiumDB);
		m_pPremiumDB = NULL;
	}
	if(m_pPremIPDB) {	// [PC방 작업]
		if(m_pPremIPDB->IsOpen()) m_pPremIPDB->Close();
		delete(m_pPremIPDB);
		m_pPremIPDB = NULL;
	}

	if(g_pMainView->Cfg.bEventDB && m_pEventPrizeDB) {	// ### [이벤트 티켓] ###
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		delete(m_pEventPrizeDB);
		m_pEventPrizeDB = NULL;
	}


	if(m_pUserDB) {	// 블럭의 위험이 있는 레코드셋은 제일 나중에 삭제
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		delete(m_pUserDB);
		m_pUserDB = NULL;
	}	

	// 데이터베이스 클래스 삭제
	if(pDatabase_Status) {
		if(pDatabase_Status->IsOpen()) pDatabase_Status->Close();
		delete(pDatabase_Status);
		pDatabase_Status = NULL;
	}
	if(pDatabase_Game) {
		if(pDatabase_Game->IsOpen()) pDatabase_Game->Close();
		delete(pDatabase_Game);
		pDatabase_Game = NULL;
	}
	if(pDatabase_Avatar) {
		if(pDatabase_Avatar->IsOpen()) pDatabase_Avatar->Close();
		delete(pDatabase_Avatar);
		pDatabase_Avatar = NULL;
	}
	if(pDatabase_Item) {	// [게임 아이템 작업]
		if(pDatabase_Item->IsOpen()) pDatabase_Item->Close();
		delete(pDatabase_Item);
		pDatabase_Item = NULL;
	}
	if(pDatabase_Premium) {	// [프리미엄 작업]
		if(pDatabase_Premium->IsOpen()) pDatabase_Premium->Close();
		delete(pDatabase_Premium);
		pDatabase_Premium = NULL;
	}
	if(pDatabase_PremIP) {	// [PC방 작업]
		if(pDatabase_PremIP->IsOpen()) pDatabase_PremIP->Close();
		delete(pDatabase_PremIP);
		pDatabase_PremIP = NULL;
	}
	if(g_pMainView->Cfg.bEventDB && pDatabase_EventPrize) {	// ### [이벤트 티켓] ###
		if(pDatabase_EventPrize->IsOpen()) pDatabase_EventPrize->Close();
		delete(pDatabase_EventPrize);
		pDatabase_EventPrize = NULL;
	}

	if(pDatabase_User) {
		if(pDatabase_User->IsOpen()) pDatabase_User->Close();
		delete(pDatabase_User);
		pDatabase_User = NULL;
	}

	
}

UINT CDBThread::DBThreadFunc(LPVOID pParam)
{
	if(pParam==NULL) return 0;
	CDBThread *pThis = (CDBThread*)pParam;

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

void CDBThread::ProcessDBWork()
{
	// 최초 로그인 요청시
	if(Work.WorkKind == 1)
	{
		Work.bIsWrongPwd=FALSE;		// 패스워드 틀림 여부
		Work.bAnotherConnect=FALSE;	// 다른 게임에 이미 로그인 중인가?[SSO 작업] - 삭제
		BOOL bRtn = GetUserInfo(&Work);
		
		if(bRtn==FALSE) Work.bIsNotUserID = TRUE;
		::SendMessage(g_pMainView->m_hWnd, UM_DBWORK_LOGIN, NULL, (LONG)&Work);
	}
	// 로그 아웃
	if(Work.WorkKind == 4)
	{
		// ### [이벤트 티켓] ###
		SetUserInfo(&Work);
		// 디비 기록 종료를 알림
		::SendMessage(g_pMainView->m_hWnd, UM_DBWORK_LOGOUT, NULL, (LONG)&Work);
	}

	// ### [이벤트 티켓] ###
	if(Work.WorkKind == 5)
	{
		SetEventPrizeDB(Work.UI.ID, &Work.EventPrize);	
		//리턴값 받아야 할뜻...
		::SendMessage(g_pMainView->m_hWnd, UM_DBWORK_EVENTSET, NULL, (LONG)&Work);
	}


	// 아이템 리스트 갱신
	if(Work.WorkKind == 9) {
		if(!GetItemList(Work.UI.ID, &Work.Item)) 
			Work.bErrorItemWork = TRUE;
		else
			Work.bErrorItemWork = FALSE;
		// 디비 기록 종료를 알림
		::SendMessage(g_pMainView->m_hWnd, UM_DBWORK_GAMEITEMLIST, NULL, (LONG)&Work);
	}
	
	// 아이템 사용
	if(Work.WorkKind == 10) {
		if(!UseGameItem(Work.UI.ID, &Work.UseItem)) 		
			Work.bErrorItemWork = TRUE;
		else 
			Work.bErrorItemWork = FALSE;
		// 디비 기록 종료를 알림
		::SendMessage(g_pMainView->m_hWnd, UM_DBWORK_USEITEMRESULT, NULL, (LONG)&Work);
	}
	
	// 환경설정에서 작업 완료후 DB Close 옵션이 적용된 경우
	// DB 레코드셋 닫기 및 클래스 삭제
	if(g_pMainView->Cfg2.bDBClose == TRUE) DeleteAllDB();
	if(ErrorCnt > 3) {
		CLogFile logfile;
		logfile.Write("DB 에러카운트 허용치 초과로 DB컨넥션을 Close함");
		// 에러 카운트 수치 초기화
		ErrorCnt = 0;
		// DB 레코드셋 닫기 및 클래스 삭제
		DeleteAllDB();
	}
}




///////////////////////////////// DB 처리 부분 /////////////////////////////////////
// ### [이벤트 티켓] ###
BOOL CDBThread::SetUserInfo(DBWORKS *pWork)
{
	USERINFO *pUI = &pWork->UI;
	ORGINFO *pOI = &pWork->OI;
	EVENT_PRIZE *pEvent = &pWork->EventPrize;
	int connectsec = pWork->ConnectTime;
	
	//######## [연습 채널 작업] #########
	if(!g_pMainView->Cfg3.bExamServer ) { 
		
		
		// 게임 기록의 변동이 없으면 DB를 기록하지 않음
		if( pUI->WinNum == pOI->WinNum && 
			pUI->LooseNum == pOI->LooseNum && 
			pUI->DrawNum  == pOI->DrawNum &&
			pUI->PMoney == pOI->pBackup ) return TRUE;
		
		
		if( !CreateGameDB() ) {
			CLogFile logfile;
			logfile.Writef("SetUserInfo - DB 컨넥션 연결 실패");
			// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
			ErrorCnt = 100;
			return FALSE;
		}
		if( m_pGameDB==NULL ) return FALSE;
		// 레코드셋이 열려있으면 닫는다
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		
		CString str;
		str.Format("[ID] = '%s'", pUI->ID);
		
			
		/////////// 게임 DB에서 아이디 검색
		TRY
		{
			m_pGameDB->m_strFilter = str;
			m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
		}
		CATCH(CDBException, e)
		{
			CString strFileName = GetLogFileName();
			if(strFileName!=_T("")){
				CLogFile logfile(strFileName.operator LPCTSTR());
				logfile.Writef(_T("Failed open GameDB - SetUserInfo Open (ID=[%s], UNum=%d,  Win=%d, Fail=%d,  AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d) : %s"),
				pUI->ID, pUI->UNum,  pUI->WinNum, pUI->LooseNum,  (INT64)Max(pUI->PMoney,0)-pOI->pBackup, pUI->PI.nRSF, pUI->PI.nSF, pUI->PI.nFCARD, e->m_strError.operator LPCTSTR());
			}
			
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
			ErrorCnt = 100;
			return FALSE;
		}
		END_CATCH
			
		if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID) != 0) {
			CString strFileName = GetLogFileName();
			if(strFileName!=_T("")){
				CLogFile logfile(strFileName.operator LPCTSTR());
				logfile.Writef(_T("CDBThread::SetUserInfo() - Can't find UserID from GameDB (ID=[%s], UNum=%d,  Win=%d, Fail=%d,  AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d)"),
				pUI->ID, pUI->UNum, pUI->WinNum, pUI->LooseNum, (INT64)Max(pUI->PMoney,0)-pOI->pBackup, pUI->PI.nRSF, pUI->PI.nSF, pUI->PI.nFCARD);
			}
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			return FALSE;
		}
		
		// 변화된 증감치 계산
		int winnum = pUI->WinNum - pOI->WinNum;
		int loosenum = pUI->LooseNum - pOI->LooseNum;
		int draw = pUI->DrawNum - pOI->DrawNum;
		
		/////////// GAME DB수정
		TRY
		{
			m_pGameDB->Edit();
			
			if(m_pGameDB->IsFieldNull(&m_pGameDB->m_Fold))
				m_pGameDB->m_Fold = 0;
			
			m_pGameDB->m_Vic += winnum;
			m_pGameDB->m_Fail += loosenum;
			m_pGameDB->m_Fold += draw;
			
			INT64 NowPMoney  = (INT64)::_atoi64(m_pGameDB->m_PMoney.operator LPCTSTR()); // 변경되었을지 모르는 데이타베이스 포커머니
			INT64 GamePMoney = pUI->PMoney;                // 접속후의 게임머니
			if(GamePMoney<0) GamePMoney = 0;               // 접속후의 게임머니는 딱 맞아야한다. 음수값을 0으로 처리해 놓는다!!!
			INT64 ResPMoney  = GamePMoney - pOI->pBackup;  // 포커머니 가감치 계산
			//INT64 ResPMoney  = pUI->PMoney - pUI->pBackup; // 기존방식의 포커머니 가감치 계산
			NowPMoney+=ResPMoney;                          // 포커머니 가감산
			
			// 변경 
			if(NowPMoney<0) 
			{
				CLogFile logfile(_T("Abuse.txt"));
				logfile.Writef("ID= %s PMoney = %I64d, OMoney = %I64d, ResPMoney = %I64d, NowPMoney = %I64d", pUI->ID, pUI->PMoney, pOI->pBackup, ResPMoney, NowPMoney);
				
				NowPMoney = 0;                 // 어뷰져 방지를 위해 음수(<0)처리 하지 않는다!!!
			}			
			
			// 레벨 쓰기
			m_pGameDB->m_Level   = pUI->nIcon; // 포커레벨 기록하지 않는다 웹에서만 사용하는 읽기전용데이타이다
			if(NowPMoney>g_pMainView->Cfg.MAX_PMONEY) NowPMoney = g_pMainView->Cfg.MAX_PMONEY; // [ $$$$ 포커머니 오버플로우 체크 $$$$ ]
			
			CString strPM; strPM.Format(_T("%I64d"),NowPMoney);
			m_pGameDB->m_PMoney = strPM;

			//[수호천사]
			strPM.Format( "%I64d", pWork->LostMoney );
			m_pGameDB->m_LMoney = strPM;
			
			m_pGameDB->m_CARD04 = (BOOL)pUI->PI.Card[ 0];
			m_pGameDB->m_CARD05 = (BOOL)pUI->PI.Card[ 1];
			m_pGameDB->m_CARD06 = (BOOL)pUI->PI.Card[ 2];
			m_pGameDB->m_CARD07 = (BOOL)pUI->PI.Card[ 3];
			m_pGameDB->m_CARD08 = (BOOL)pUI->PI.Card[ 4];
			m_pGameDB->m_CARD09 = (BOOL)pUI->PI.Card[ 5];
			m_pGameDB->m_CARD10 = (BOOL)pUI->PI.Card[ 6];
			m_pGameDB->m_CARD11 = (BOOL)pUI->PI.Card[ 7];
			m_pGameDB->m_CARD12 = (BOOL)pUI->PI.Card[ 8];
			m_pGameDB->m_CARD13 = (BOOL)pUI->PI.Card[ 9];
			m_pGameDB->m_CARD14 = (BOOL)pUI->PI.Card[10];
			m_pGameDB->m_CARD15 = (BOOL)pUI->PI.Card[11];
			m_pGameDB->m_Level2 = pUI->PI.nCard; // 대표카드
			
			m_pGameDB->m_RSF    = pUI->PI.nRSF;
			m_pGameDB->m_SF     = pUI->PI.nSF;
			m_pGameDB->m_FCARD  = pUI->PI.nFCARD;
			
			m_pGameDB->m_WinPer = GetWinPro(pUI->WinNum, pUI->LooseNum);//, pUI->DrawNum);
			
			// 접속 종료시 접속 코드를 초기화
			int ServerCode = g_pMainView->Cfg.ServerCode;
			if(m_pGameDB->m_IsConnected==ServerCode) m_pGameDB->m_IsConnected = 0;
			
			// 히스토리 작업을 시행한다 ( 오링시만 기록한다)
			if(GamePMoney<=0){
				int nHistoryIndex = (int)m_pGameDB->m_HistoryIndex;
				if(nHistoryIndex<0 || nHistoryIndex>2) nHistoryIndex = 0;
				CString strHistory;
				CTime  time = CTime::GetCurrentTime();
				
				strHistory.Format("%I64d(%d:%d:%d:%d)", // 가감치를 저장한다 32사이즈로 묶는다!!
					ResPMoney, time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute());
				if(nHistoryIndex==0){
					m_pGameDB->m_History1 = strHistory;
				}
				else if(nHistoryIndex==1){
					m_pGameDB->m_History2 = strHistory;
				}
				else {
					m_pGameDB->m_History3 = strHistory;
				}
				nHistoryIndex++; if(nHistoryIndex>2) nHistoryIndex = 0;
				m_pGameDB->m_HistoryIndex = (BYTE)nHistoryIndex;
			}
			// 게임 DB 업데이트
			m_pGameDB->Update();
		}
		CATCH(CDBException, e)
		{
			CString strFileName = GetLogFileName();
			if(strFileName!=_T("")){
				if(e->m_nRetCode != AFX_SQL_ERROR_NO_ROWS_AFFECTED){
					CLogFile logfile(strFileName.operator LPCTSTR());
					logfile.Writef(_T("GameDB error - SetUserInfo Update (ID=[%s], UNum=%d,  Win=%d, Fail=%d,  AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d) : %s"),
					pUI->ID, pUI->UNum,  pUI->WinNum, pUI->LooseNum,  (INT64)Max(pUI->PMoney,0)-pOI->pBackup, pUI->PI.nRSF, pUI->PI.nSF, pUI->PI.nFCARD, e->m_strError.operator LPCTSTR());
					
					if(m_pGameDB->IsOpen()) m_pGameDB->Close();
					ErrorCnt++;	// 에러 카운트 증가
					return FALSE;
				}
				else {
					CLogFile logfile(strFileName.operator LPCTSTR());
					logfile.Writef(_T("GameDB error - Update result is none (ID=[%s], UNum=%d,  Win=%d, Fail=%d,  AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d) : %s"),
						pUI->ID, pUI->UNum,  pUI->WinNum, pUI->LooseNum,  (INT64)Max(pUI->PMoney,0)-pOI->pBackup, pUI->PI.nRSF, pUI->PI.nSF, pUI->PI.nFCARD, e->m_strError.operator LPCTSTR());
				}
				
			}
		}
		END_CATCH
		
		// 오픈한 DB를 닫는다
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		
	} //######## [연습 채널 작업] #########
	

	// ### [이벤트 티켓] ###]
	SetEventPrizeDB(pUI->ID, pEvent);
	// 접속자 수 업데이트(에러 검사는 하지 않는다)
	UpdateConnectStatus();

	return TRUE;
}


// [SSO 작업] - 추가
BOOL CDBThread::UpdateConnectStatus() 
{
	int GameCode = g_pMainView->Cfg.GameCode;
	int ServerCode = g_pMainView->Cfg.ServerCode;
	if(GameCode<=0 || ServerCode<=0) 
		return FALSE;

	// 현재 접속자수가 0이 아니고 마지막으로 업데이트한 시간이 지정된 시간을 초과하지 않은 경우 리턴
	UINT NowTick = timeGetTime();
	if(g_pMainView->TotUser != 0 && NowTick-g_LastConnectStatusUpdateTime < CONNECTSTATUS_UPDATE_DELAY) 
		return TRUE;

	if(!CreateStatusDB()) 
		return FALSE;

	if(!m_pStatusDB) 
		return FALSE;

	g_LastConnectStatusUpdateTime = NowTick;
	
	TRY
	{
		if(m_pStatusDB->IsOpen()) m_pStatusDB->Close();
		m_pStatusDB->m_strFilter.Format("GameCode = %d AND ServerCode=%d", GameCode, ServerCode);

		m_pStatusDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
		if(m_pStatusDB->IsEOF()) {
			CLogFile logfile;
			logfile.Writef("Status DB에러: 접속자 현황 DB에 일치하는 게임코드가 없습니다");
			if(m_pStatusDB->IsOpen()) m_pStatusDB->Close();
			return FALSE;
		}

		m_pStatusDB->Edit();
		m_pStatusDB->m_UserNum = g_pMainView->RealUser;
		// 업데이트 할 수 있는 상태라면 업데이트하고 그렇지 않으면 업데이트 취소
		if(m_pStatusDB->CanUpdate()) m_pStatusDB->Update();
		else m_pStatusDB->CancelUpdate();
	}
	CATCH(CDBException, e)
	{
		// 여러쓰레드가 동시에 쓰기때문에 에러가 자주 발생하여 로그기록을 하지 않음
		if(m_pStatusDB->IsOpen()) m_pStatusDB->Close();
		return FALSE;
	}
	END_CATCH

	if(m_pStatusDB->IsOpen()) m_pStatusDB->Close();

	return TRUE;
}


//넷마블 ID는 잇는데 DB에 없을때 ID에 해당되는 디비를 만든다.
BOOL CDBThread::AddNewGameDB(USERINFO *pUI) 
{
	if(pUI == NULL) return FALSE;

	CString str;
	str.Format("[ID] = '%s'", pUI->ID);

	TRY
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		m_pGameDB->m_strFilter = str;
		if(!m_pGameDB->IsOpen()) m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("GameDB에러 - AddNewGameDB Open : %s", e->m_strError.operator LPCTSTR());
		ErrorCnt++;	// 에러 카운트 증가
		return FALSE;
	}
	END_CATCH


	// 같은 ID가 존재하면 추가 취소
	if(!m_pGameDB->IsEOF() && m_pGameDB->m_ID.CompareNoCase(pUI->ID) == 0) {
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// DB를 추가하는 상태
	TRY
	{
		if(!m_pGameDB->IsEOF()) m_pGameDB->MoveLast();
		m_pGameDB->AddNew();

		// DB추가
		m_pGameDB->m_ID.Format("%s", pUI->ID);
		//m_pGameDB->m_Name.Format("%s", pUI->Name);
		m_pGameDB->m_Vic     = 0;
		m_pGameDB->m_Fail    = 0;
		m_pGameDB->m_Fold    = 0;
		m_pGameDB->m_Level2  = (BYTE)0; // 대표카드
		
		//m_pGameDB->m_Level1  = 0; // 누적점수
		m_pGameDB->m_Level   = (BYTE)0; // 레벨

		
		//pUI->PMoney  = M_5000;//M_1000; // [세븐포커] 1000 만원 제공
		
		// 이벤트 용
		pUI->PMoney  = M_3Y;//M_1000; // [뉴포커] 3억 제공 //신규유저 머니 지급
		
#ifdef _DEBUG
		//!!! 제거할껏
//		pUI->PMoney  = M_10Y;
#endif
		CString strPM; strPM.Format(_T("%I64d"),pUI->PMoney);
		m_pGameDB->m_PMoney = strPM;

		m_pGameDB->m_LMoney.Format(""); // [수호천사] 2004.07.08

		m_pGameDB->m_CARD04 = FALSE;
		m_pGameDB->m_CARD05 = FALSE;
		m_pGameDB->m_CARD06 = FALSE;
		m_pGameDB->m_CARD07 = FALSE;
		m_pGameDB->m_CARD08 = FALSE;
		m_pGameDB->m_CARD09 = FALSE;
		m_pGameDB->m_CARD10 = FALSE;
		m_pGameDB->m_CARD11 = FALSE;
		m_pGameDB->m_CARD12 = FALSE;
		m_pGameDB->m_CARD13 = FALSE;
		m_pGameDB->m_CARD14 = FALSE;
		m_pGameDB->m_CARD15 = FALSE;
		
		m_pGameDB->m_RSF    = 0;
		m_pGameDB->m_SF     = 0;
		m_pGameDB->m_FCARD  = 0;

		m_pGameDB->m_WinPer = (BYTE)0;
		m_pGameDB->m_IsConnected = (BYTE)0;

		// 히스토리 작업
		m_pGameDB->m_History1.Format("");
		m_pGameDB->m_History2.Format("");
		m_pGameDB->m_History3.Format("");
		m_pGameDB->m_HistoryIndex = (BYTE)0;


		m_pGameDB->Update();
		
	}
	CATCH(CDBException, e)
	{
	//	CLogFile logfile;
	//	logfile.Writef("GameDB에러 - AddNewGameDB Update (ID=%s) : %s", pUI->ID, e->m_strError.operator LPCTSTR());
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Writef(_T("GameDB error - AddNewGameDB Update (ID=%s) : %s"), pUI->ID, e->m_strError.operator LPCTSTR());
		}
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt++;	// 에러 카운트 증가
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

// 로그파일작성 타이틀을 생성한다
CString CDBThread::GetLogFileName()
{
	CString strFileName(_T(""));

	CTime  time = CTime::GetCurrentTime();

	strFileName.Format(g_StrMan.Get(_T("SPOKER_DBERR")),time.GetMonth(),time.GetDay());

	return strFileName;
}


// [SSO 작업] - 함수의 상당부분이 바뀌었음 (참고하여 본인의 프로젝트에 맞게 수정하여 사용할 것)
BOOL CDBThread::GetUserInfo(DBWORKS *pWork) 
{
	// 메인 유저 정보를 읽어온다

		// DB에서 읽어온다
	if(!GetMainUserInfo_FromDB(pWork)) 
		return FALSE;


	// 게임 정보를 읽어온다
	if(!GetGameInfo(pWork)) 
		return FALSE;

	// [프리미엄 작업]
	// 프리미엄 정보를 읽어온다
	if(!GetPremiumInfo(pWork->UniqNo, &pWork->UI))
		return FALSE;
	
	// [게임 아이템 작업]
	// 게임 아이템을 읽어온다
	if(!GetItemList(pWork->UI.ID, &pWork->Item)) 
		return FALSE;

	// [PC방 작업]
	// 프리미엄 PC방 IP정보를 읽어온다
	// 실패하더라도 정상 처리한다
	GetPremiumIP(pWork);

	// ### [이벤트 티켓] ###
	// 실패하더라고 정상 처리한다.
	GetEventPrize(pWork);
	

	return TRUE;
}

// [SSO 작업] - 추가!! (참고하여 본인의 프로젝트에 맞게 수정하여 사용할 것)
BOOL CDBThread::GetMainUserInfo_FromDB(DBWORKS *pWork)
{	
	USERINFO *pUI = &pWork->UI;

	if(!CreateUserDB()  || !CreateAvatarDB()) {
		CLogFile logfile;
		logfile.Writef("GetUserInfo - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pUserDB==NULL || m_pAvatarDB==NULL) return FALSE;
	if(pUI == NULL) return FALSE;

	CString str;
	str.Format("[ID] = '%s'", pUI->ID);

	TRY
	{
		// 레코드셋이 열려있으면 닫는다
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();

		m_pUserDB->m_strFilter = str;
		// 메인유저DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pUserDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pUserDB->IsEOF() || m_pUserDB->m_ID.CompareNoCase(pUI->ID)!=0)
		{
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			return FALSE;
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("UserDB에러 - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// NULL 필드 초기화		[프리미엄 작업]
	if(m_pUserDB->IsFieldNull(&m_pUserDB->m_UniqNO))
		m_pUserDB->m_UniqNO = "";

	// 비밀번호 검사(DB로 쿼리하는 경우는 무조건 검사)
	if(m_pUserDB->m_Passwd.CompareNoCase(pWork->Passwd)==0) {
		pWork->bIsWrongPwd = FALSE;
	}
	else {
		pWork->bIsWrongPwd = TRUE;
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		return FALSE;
	}


	// 접속 중지 혹은 삭제된 아이디인지 검사
	pWork->IDState = m_pUserDB->m_IDState;								// 아이디 상태
	strncpy(pUI->ID, m_pUserDB->m_ID.operator LPCTSTR(), 15);			// 사용자 ID
	strncpy(pWork->Passwd, m_pUserDB->m_Passwd.operator LPCTSTR(), 15);	// 사용자 패스워드
	pUI->Sex = (char)m_pUserDB->m_Sex;
	strncpy(pUI->Desc, m_pUserDB->m_Desc.operator LPCTSTR(), 60);

	// [게임 아이템 작업]
	strncpy(pWork->RegID, (LPCTSTR)m_pUserDB->m_RegID, 15);	// 주민등록 번호
	strncpy(pWork->UniqNo, (LPCTSTR)m_pUserDB->m_UniqNO, 15);	// 유니크 넘버


	// [성인인증]
	if(m_pUserDB->m_Certificate == NULL)
		pWork->bCertificate = FALSE;
	else
		pWork->bCertificate = m_pUserDB->m_Certificate;
	

	
	// 아바타 DB 읽기
	TRY
	{
		m_pAvatarDB->m_strFilter = str;
		// 메인유저DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pAvatarDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pAvatarDB->IsEOF() || m_pAvatarDB->m_ID.CompareNoCase(pUI->ID)!=0) {
			// 아바타 DB가 존재하지 않으면 기본값 세팅(몸통 번호만 입력)
			if(pUI->Sex == 1) pUI->AvatarInfo[5] = 1;
			else pUI->AvatarInfo[5] = 2;
		}
		else {
			// 아바타 정보 저장
			pUI->AvatarInfo[0] = (short)m_pAvatarDB->m_layer0;
			pUI->AvatarInfo[1] = (short)m_pAvatarDB->m_layer1;
			pUI->AvatarInfo[2] = (short)m_pAvatarDB->m_layer2;
			pUI->AvatarInfo[3] = (short)m_pAvatarDB->m_layer3;
			pUI->AvatarInfo[4] = (short)m_pAvatarDB->m_layer4;
			pUI->AvatarInfo[5] = (short)m_pAvatarDB->m_layer5;
			pUI->AvatarInfo[6] = (short)m_pAvatarDB->m_layer6;
			pUI->AvatarInfo[7] = (short)m_pAvatarDB->m_layer7;
			pUI->AvatarInfo[8] = (short)m_pAvatarDB->m_layer8;
			pUI->AvatarInfo[9] = (short)m_pAvatarDB->m_layer9;
			pUI->AvatarInfo[10] = (short)m_pAvatarDB->m_layer10;
			pUI->AvatarInfo[11] = (short)m_pAvatarDB->m_layer11;
			pUI->AvatarInfo[12] = (short)m_pAvatarDB->m_layer12;
			pUI->AvatarInfo[13] = (short)m_pAvatarDB->m_layer13;
			pUI->AvatarInfo[14] = (short)m_pAvatarDB->m_layer14;
			pUI->AvatarInfo[15] = (short)m_pAvatarDB->m_layer15;
			pUI->AvatarInfo[16] = (short)m_pAvatarDB->m_layer16;
			pUI->AvatarInfo[17] = (short)m_pAvatarDB->m_layer17;
			pUI->AvatarInfo[18] = (short)m_pAvatarDB->m_layer18;
			pUI->AvatarInfo[19] = (short)m_pAvatarDB->m_layer19;
			pUI->AvatarInfo[20] = (short)m_pAvatarDB->m_layer20;
			pUI->AvatarInfo[21] = (short)m_pAvatarDB->m_layer21;
			pUI->AvatarInfo[22] = (short)m_pAvatarDB->m_layer22;
			pUI->AvatarInfo[23] = (short)m_pAvatarDB->m_layer23;
			pUI->AvatarInfo[24] = (short)m_pAvatarDB->m_layer24;
			pUI->AvatarInfo[25] = (short)m_pAvatarDB->m_layer25;
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("AvatarDB에러 - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		return FALSE;
	}
	END_CATCH

	if(m_pUserDB->IsOpen()) m_pUserDB->Close();
	if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();

	return TRUE;
}

// [SSO 작업] - 추가!! (참고하여 본인의 프로젝트에 맞게 수정하여 사용할 것)
BOOL CDBThread::GetGameInfo(DBWORKS *pWork) 
{
	USERINFO *pUI = &pWork->UI;

	if(!CreateGameDB()) {
		CLogFile logfile;
		logfile.Writef("GetUserInfo - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pGameDB==NULL) return FALSE;
	
	// 레코드셋이 열려있으면 닫는다
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	CString str;
	str.Format("[ID] = '%s'", pUI->ID);

		// 게임 DB 읽기
	TRY
	{
		m_pGameDB->m_strFilter = str;
		m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("GameDB에러 - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH
	

	// 넷마블 회원ID는 있지만 게임 DB에 등록되어 있지 않음
	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		if(AddNewGameDB(pUI) == FALSE) {
			CLogFile logfile;
			logfile.Writef("GameDB에러 - GetUserInfo (ID=%s) : Game DB 아이디 추가 실패", pUI->ID);
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			return FALSE;
		}

		TRY
		{
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			m_pGameDB->m_strFilter = str;
			m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("GameDB에러 - GetUserInfo Requery (ID=%s) : %s", pUI->ID, e->m_strError.operator LPCTSTR());
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			ErrorCnt++;	// 에러 카운트 증가
			return FALSE;
		}
		END_CATCH
	}

	// 게임 DB추가 실패한 경우
	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		CLogFile logfile;
		logfile.Writef("GameDB에러 (ID=%s) : Game DB 아이디 추가 실패", pUI->ID);
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// 게임 DB를 읽어옴 (KHS_FAST_CHAN)
	// ##############  [세븐포커] 에러처리
	INT64 nPMoney = (INT64)::_atoi64(m_pGameDB->m_PMoney.operator LPCTSTR());
/*
	if(nPMoney<0){ // ###### 포커머니 에러다
		CLogFile logfile(_T("Abuse.txt"));
		logfile.Writef(g_StrMan._Get(_T("SPOKER_INVAL_PM")), id, nPMoney);
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();

		if(pNegPMoney!=NULL) *pNegPMoney = TRUE;

		return FALSE;
	}
*/
	// 일정량의 정해진 포커머니만 불러와서...나중에 데이타베이스에 그 포커머니의 증감치만 기록하면 된다!!
	if(nPMoney>g_pMainView->Cfg.MAX_PMONEY) nPMoney = g_pMainView->Cfg.MAX_PMONEY; // [ $$$$ 포커머니 오버플로우 체크 $$$$ ]

#ifdef _DEBUG
	if(nPMoney <=0)
		pUI->PMoney  = 150000000;
	else 
		pUI->PMoney  = nPMoney;
#else
	pUI->PMoney  = nPMoney;
#endif	

	// 레벨상점 폐쇄
	pUI->nIcon = GetPokerLevel(nPMoney);

	nPMoney = (INT64)::_atoi64(m_pGameDB->m_LMoney.operator LPCTSTR());  // [수호천사] 2004.07.08 // 로스트 머니
	pWork->LostMoney = nPMoney;

	pUI->WinNum    = m_pGameDB->m_Vic;
	pUI->LooseNum  = m_pGameDB->m_Fail;
	pUI->DrawNum   = m_pGameDB->m_Fold;

	// 카드 인덱스 ???
	pUI->PI.Card[ 0] = (char)m_pGameDB->m_CARD04;
	pUI->PI.Card[ 1] = (char)m_pGameDB->m_CARD05;
	pUI->PI.Card[ 2] = (char)m_pGameDB->m_CARD06;
	pUI->PI.Card[ 3] = (char)m_pGameDB->m_CARD07;
	pUI->PI.Card[ 4] = (char)m_pGameDB->m_CARD08;
	pUI->PI.Card[ 5] = (char)m_pGameDB->m_CARD09;
	pUI->PI.Card[ 6] = (char)m_pGameDB->m_CARD10;
	pUI->PI.Card[ 7] = (char)m_pGameDB->m_CARD11;
	pUI->PI.Card[ 8] = (char)m_pGameDB->m_CARD12;
	pUI->PI.Card[ 9] = (char)m_pGameDB->m_CARD13;
	pUI->PI.Card[10] = (char)m_pGameDB->m_CARD14;
	pUI->PI.Card[11] = (char)m_pGameDB->m_CARD15;
	pUI->PI.nCard    = (char)m_pGameDB->m_Level2; // 대표카드
	pUI->PI.nRSF     = m_pGameDB->m_RSF;
	pUI->PI.nSF      = m_pGameDB->m_SF;
	pUI->PI.nFCARD   = m_pGameDB->m_FCARD;

	// 사용후 DB 테이블을 닫음
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;

}



// 특정 아이디의 게임 아이템을 읽어온다
BOOL CDBThread::GetItemList(char *id, GAMEITEM_LIST *pItemList)
{ 
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// 아이템 DB 열기 //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	if(!CreateItemDB()) {
		CLogFile logfile("ItemDB_Log.txt");
		logfile.Writef("GetItemList - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pItemDB == NULL) return FALSE;
	if(pItemList == NULL) return FALSE;

	// 레코드셋이 열려있으면 닫는다
	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	CString strItem; // 게임코드와 아이디를 읽는다
	/*
	// 게임에 필요한 아이템만 쿼리한다
	strItem.Format(_T("[id]='%s' AND \
		( ([c_gametype]=%d AND [c_code]>=%d AND [c_code]<=%d) OR \
		([c_code]>=%d AND [c_code]<=%d) OR ([c_code]>=%d AND [c_code]<=%d) OR \
		([c_gametype]=%d AND [c_code]>=%d AND [c_code]<=%d) OR \
		([c_code]=%d) )"), // [천하무적 아이템]
		id, 
		g_pMainView->Cfg.GameCode, ITEM_SCODE_JUMP, ITEM_ECODE_JUMP,	// 점프 아이템 
		ITEM_SCODE_SUPERMASTER, ITEM_ECODE_SUPERMASTER,					// 슈퍼 방장 아이템
		ITEM_SCODE_ALLINSUPERMASTER, ITEM_ECODE_ALLINSUPERMASTER,	    // 슈퍼한방 [슈퍼 방장] (공통)
		7, ITEM_SCODE_SECRET, ITEM_ECODE_SECRET,						// 비공개 아이템 - 세븐포커1, 2 공용 게임코드는 7번으로 읽는다.
		ITEM_SCODE_INVINCIBLE );										// [천하무적 아이템]	
	
	*/

	// 게임에 필요한 아이템만 쿼리한다
	strItem.Format(_T("[id]='%s' AND \
		( ([c_code]>=%d AND [c_code]<=%d) OR \
		([c_code]>=%d AND [c_code]<=%d) OR \
		([c_code]>=%d AND [c_code]<=%d) OR \
		([c_gametype]=%d AND [c_code]>=%d AND [c_code]<=%d) OR \
		([c_code]=%d) )"), // [천하무적 아이템]
		id, 
		ITEM_SCODE_JUMP, ITEM_ECODE_JUMP,								// 점프 아이템 
		ITEM_SCODE_SUPERMASTER, ITEM_ECODE_SUPERMASTER,					// 슈퍼 방장 아이템
		ITEM_SCODE_SAFEGUARDANGEL, ITEM_ECODE_SAFEGUARDANGEL,			// [수호천사] 2004.07.08
		7, ITEM_SCODE_SECRET, ITEM_ECODE_SECRET,						// 비공개 아이템 - 세븐포커1, 2 공용 게임코드는 7번으로 읽는다.
		ITEM_SCODE_INVINCIBLE );										// [천하무적 아이템]											// [천하무적 아이템]		


	// 아이템 DB 읽기
	TRY
	{
		m_pItemDB->m_strFilter = strItem;
		// 아이템DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pItemDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pItemDB->IsEOF()) {
			pItemList->nResult = GAMEITEM_MES_NOITEM; // 아이템을 보유하고 있지않다
			if(m_pItemDB->IsOpen()) m_pItemDB->Close();
			return TRUE;
		}

		if(!m_pItemDB->IsBOF()) m_pItemDB->MoveFirst();
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("ItemDB_Log.txt");
		logfile.Writef(_T("ItemDB error - GetItemList Open (ID=%s): %s"), id, (LPCTSTR)(e->m_strError));

		pItemList->nResult = GAMEITEM_MES_DBERROR; // 아이템 DB를 읽는데 실패하였다
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// 아이템정보 읽어오기
	int nItemNum = 0;
	TRY
	{
		for(;;)
		{
			if(m_pItemDB->IsEOF() || nItemNum >= MAX_ITEM_NUM) break; // 읽어온 리스트 모두 읽거나, 갯수 초과되었다면 중지한다

			// 아이템 사용자 확인
			if(m_pItemDB->m_ID.CompareNoCase(id)==0) {

				GAMEITEM item;
				memset(&item, 0, sizeof(GAMEITEM));

				item.Index  = nItemNum;										// 인덱스 걸기
				item.NO     = (int)m_pItemDB->m_NO;							// 유니크넘버
				item.Code   = (int)m_pItemDB->m_Code;						// 아이템 코드
				strncpy(item.PresID, (LPCTSTR)(m_pItemDB->m_PresID), 15);	// 선물준 아이디
				item.UseDay = (short)m_pItemDB->m_UseDay;					// 유효 기간
				m_pItemDB->m_StartDate.GetAsSystemTime(item.StartDate);		// 스타트시간
				item.Use = (short)m_pItemDB->m_Use;							// 사용 여부(혹은 사용 회수)

				// [수호천사] 2004.07.08
				if( m_pItemDB->IsFieldNull(&m_pItemDB->m_UseNum) ) 	item.UseNum = 0;
				else item.UseNum = (short)m_pItemDB->m_UseNum;
			
				if(item.Use > 0) { 
					// 사용되어진 아이템이라면 시작한 날짜가 있다
					COleDateTime st = m_pItemDB->m_StartDate; // 스타트시간
					if(st.GetStatus() != COleDateTime::valid){
						// 시간 필드가 유효하지 않음
						m_pItemDB->MoveNext();
						continue;
					}
				}

				// 올바른 데이타가 아니다
				if(item.UseDay < 0 || item.UseNum < 0) {
					m_pItemDB->MoveNext();
					continue;					
				}

				// 아이템의 상태 정보를 얻는다
				ItemState state = GameItem_GetItemState(item);

				if(state.iState == 0 || state.iState == 3) {
					// 잘못된 아이템이거나 사용이 만료된 아이템이다
					m_pItemDB->MoveNext();
					continue;
				}

				//if(state.bNowUsing) {
				//	// 사용된 아이템이 있고, 유효한개 1개라도 있다면 입장가능하다!!!
				//	pItemList->bLogable = TRUE;
				//}

				// 아이템 정보를 저장
				pItemList->Item[nItemNum] = item;

				nItemNum++;
			}

			m_pItemDB->MoveNext();
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("ItemDB_Log.txt");
		logfile.Writef(_T("ItemDB error - GetItemList() on MoveNext (ID=%s): %s"), id, (LPCTSTR)(e->m_strError));

		pItemList->nResult = GAMEITEM_MES_DBERROR; // 읽어온 아이템 DB를 이동하는데 실패하였다
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		ErrorCnt++;
		return FALSE;
	}
	END_CATCH	

	// 아이템 개수 저장
	pItemList->nNum = nItemNum;

	// 쓸만한 아이템이 한개도 없다면...
	if(nItemNum <= 0){
		pItemList->nResult = GAMEITEM_MES_NOITEM; // 아이템을 보유하고 있지않다
	}

	// 유효하게 사용중인 아이템이 있다
	if(pItemList->bLogable){
		pItemList->nResult = GAMEITEM_MES_SUCCESS; // 로그인 가능하다
	}
	// 한번도 안쓴 아이템이 한개이상 존재한다
	else{
		pItemList->nResult = GAMEITEM_MES_USEABLE; // 아이템사용 문의
	}
	

	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	return TRUE;
}


// 특정 아이디의 게임 아이템을 사용한다
BOOL CDBThread::UseGameItem(char *id, GAMEITEM *pItem)
{ 
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// 아이템 DB 열기 //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	if(!CreateItemDB()) {
		CLogFile logfile("ItemDB_Log.txt");
		logfile.Writef("GetItemList - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pItemDB == NULL) return FALSE;
	if(pItem == NULL) return FALSE;

	// 레코드셋이 열려있으면 닫는다
	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	CString strItem;
	
	// 아이템 쿼리
	strItem.Format(_T("[c_no]=%d"), pItem->NO);

	// 아이템 DB 읽기
	TRY
	{
		m_pItemDB->m_strFilter = strItem;
		m_pItemDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
		// 아이템이 존재하지 않는 경우 실패
		if(m_pItemDB->IsEOF()) {
			if(m_pItemDB->IsOpen()) m_pItemDB->Close();
			return FALSE;
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("ItemDB_Log.txt");
		logfile.Writef(_T("ItemDB error - UseGameItem Open (ID=%s): %s"), id, (LPCTSTR)(e->m_strError));
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// 아이디가 일치하지 않는 경우 실패
	if(m_pItemDB->m_ID.CompareNoCase(id) != 0) {
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		return FALSE;
	}
	// 아이템 코드가 일치하지 않는 경우 실패
	if(pItem->Code != (short)m_pItemDB->m_Code) {
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		return FALSE;
	}

	TRY
	{
		m_pItemDB->Edit();

		// 날짜 필드가 NULL이면 현재 일시 기록
		if(m_pItemDB->IsFieldNull(&m_pItemDB->m_StartDate)) {
			m_pItemDB->m_StartDate = COleDateTime::GetCurrentTime();			
		}

		if(m_pItemDB->IsFieldNull(&m_pItemDB->m_Use)) 
			m_pItemDB->m_Use = 0;

		m_pItemDB->m_Use += 1;
		// 사용한 게임 코드를 기록
		m_pItemDB->m_UsedGameCode = g_pMainView->Cfg.GameCode;

		m_pItemDB->Update();

	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("ItemDB_Log.txt");
		logfile.Writef(_T("ItemDB error - UseGameItem() on Update (ID=%s): %s"), id, (LPCTSTR)(e->m_strError));
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		ErrorCnt++;
		return FALSE;
	}
	END_CATCH	

	// 아이템의 상태를 기록
	m_pItemDB->m_StartDate.GetAsSystemTime(pItem->StartDate);		// 스타트시간
	pItem->Use = (short)m_pItemDB->m_Use;							// 사용 여부(혹은 사용 회수)

	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	return TRUE;
}


// [프리미엄 작업]
// 프리미엄 정보를 읽어온다
BOOL CDBThread::GetPremiumInfo(LPCTSTR uniqno, USERINFO* pUI)
{ 
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// 아이템 DB 열기 //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	// 기본값 설정
	pUI->PremCode = 0;
	memset(&pUI->PremDate, 0, sizeof(pUI->PremDate));

	// 유니크 넘버가 존재하지 않는 경우는 예외 상황이므로 일반 회원으로 간주
	if(strlen(uniqno) == 0) 
		return TRUE;

	// 테이블 생성
	if(!CreatePremiumDB()) {
		CLogFile logfile("PremiumDB_Log.txt");
		logfile.Writef("GetPremiumList - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pPremiumDB == NULL) return FALSE;

	// 레코드셋이 열려있으면 닫는다
	if(m_pPremiumDB->IsOpen()) m_pPremiumDB->Close();

	CString strPremium; // 게임코드와 아이디를 읽는다
	
	// 유니크 넘버로 프리미엄 정보를 쿼리한다
	strPremium.Format(_T("[NO]='%s'"), uniqno);

	// 프리미엄 DB 읽기
	TRY
	{
		m_pPremiumDB->m_strFilter = strPremium;
		// DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pPremiumDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("PremiumDB_Log.txt");
		logfile.Writef(_T("PremiumDB error - GetPremiumList Open (UniqNO=%s): %s"), uniqno, (LPCTSTR)(e->m_strError));
		if(m_pPremiumDB->IsOpen()) m_pPremiumDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// NULL 필드 초기화
	if(m_pPremiumDB->IsFieldNull(&m_pPremiumDB->m_UniqNO)) {
		m_pPremiumDB->m_UniqNO = "";
	}

	if(m_pPremiumDB->IsEOF() || 
		stricmp(m_pPremiumDB->m_UniqNO, uniqno)!=0 ||
		m_pPremiumDB->m_UniqNO.GetLength()==0 ) 
	{
		// 프리미엄 레코드가 존재하지 않는다면 일반 회원이다
		if(m_pPremiumDB->IsOpen()) m_pPremiumDB->Close();		// [PC방 작업] - 프리미엄 작업에서 빼먹은 것
		return TRUE;
	}

	// 정보 저장
	pUI->PremCode = (char)m_pPremiumDB->m_PremCode;				// 프리미엄 코드
	m_pPremiumDB->m_PremDate.GetAsSystemTime(pUI->PremDate);	// 프리미엄 만료 일시

	//2004.05.07 리더스총액 제한
	pUI->PremLeadersCode = (char)m_pPremiumDB->m_LeadersCode;   // 1 리더스 4 리더스 플러스

	// 테이블 닫기
	if(m_pPremiumDB->IsOpen()) m_pPremiumDB->Close();

	return TRUE;
}

// 프리미엄 IP 정보 읽기			[PC방 작업]
BOOL CDBThread::GetPremiumIP(DBWORKS *pWork)
{
	USERINFO *pUI = &pWork->UI;
	// 기본값 설정
	pUI->bPremIP = FALSE;

	// 테이블 생성
	if(!CreatePremIPDB()) {
		CLogFile logfile("PremIPDB_Log.txt");
		logfile.Writef("GetPremiumIP - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pPremIPDB == NULL) return FALSE;

	// 레코드셋이 열려있으면 닫는다
	if(m_pPremIPDB->IsOpen()) m_pPremIPDB->Close();

	CString strQuery;
	
	// 사용자의 IP번호로 프리미엄 정보를 쿼리한다
	//strQuery.Format(_T("[ip]='%s'"), pWork->UserIP);

	// 사용자의 IP번호로 프리미엄 정보를 쿼리한다(플래그가 0인 레코드만 쿼리)
	strQuery.Format(_T("[ip]='%s' AND [flag]=%d"), pWork->UserIP, 0);

	// 프리미엄 DB 읽기
	TRY
	{
		m_pPremIPDB->m_strFilter = strQuery;
		// DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pPremIPDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("PremIPDB_Log.txt");
		logfile.Writef(_T("PremIPDB error - GetPremiumIP Open (ID=%s:IP=%s): %s"), pUI->ID, pWork->UserIP, (LPCTSTR)(e->m_strError));
		if(m_pPremIPDB->IsOpen()) m_pPremIPDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// NULL 필드 초기화
	//if(m_pPremIPDB->IsFieldNull(&m_pPremIPDB->m_UniqNO)) {
	//	m_pPremIPDB->m_UniqNO = "";
	//}

	if(m_pPremIPDB->IsEOF() || 
		stricmp(m_pPremIPDB->m_IP, pWork->UserIP)!=0 ||
		m_pPremIPDB->m_IP.GetLength()==0 ) 
	{
		// IP정보가 존재하지 않는다면 일반 회원이다
		if(m_pPremIPDB->IsOpen()) m_pPremIPDB->Close();
		return TRUE;
	}

	// 플래그가 0인 경우만 올바른 PC방 IP이다
	if(m_pPremIPDB->m_Flag == 0) {
		pUI->bPremIP = TRUE;
	}

	// 테이블 닫기
	if(m_pPremIPDB->IsOpen()) m_pPremIPDB->Close();

	return TRUE;
}


// 레코드 추가 ### [이벤트 티켓] ### 
BOOL CDBThread::AddNewEventPrizeDB(USERINFO *pUI)
{
	if(pUI == NULL) return FALSE;

	CString str;
	str.Format("[ID] = '%s' AND [GameCode]= %d", pUI->ID, g_pMainView->Cfg.GameCode);

	TRY
	{
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		m_pEventPrizeDB->m_strFilter = str;
		if(!m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("EventPrizeDB에러 - AddNewEventPrizeDB Open : %s", e->m_strError.operator LPCTSTR());
		ErrorCnt++;	// 에러 카운트 증가
		return FALSE;
	}
	END_CATCH


	// 같은 ID가 존재하면 추가 취소
	if(!m_pEventPrizeDB->IsEOF() && m_pEventPrizeDB->m_ID.CompareNoCase(pUI->ID) == 0) {
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		return FALSE;
	}

	// DB를 추가하는 상태
	TRY
	{
		if(!m_pEventPrizeDB->IsEOF()) m_pEventPrizeDB->MoveLast();
		m_pEventPrizeDB->AddNew();

		// DB추가
		m_pEventPrizeDB->m_ID.Format("%s", pUI->ID);
		m_pEventPrizeDB->m_EventCoin = 0;
		m_pEventPrizeDB->m_GameCount = 0;
		m_pEventPrizeDB->m_GameCode = g_pMainView->Cfg.GameCode; //게임코드
		m_pEventPrizeDB->m_LastPlayDate = COleDateTime::GetCurrentTime();

		m_pEventPrizeDB->Update();
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("EventPrizeDB에러 - AddNewEventPrizeDB Update (ID=%s) : %s", pUI->ID, e->m_strError.operator LPCTSTR());
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		ErrorCnt++;	// 에러 카운트 증가
		return FALSE;
	}
	END_CATCH

	return TRUE;

}


// ### [이벤트 티켓] ###
BOOL CDBThread::GetEventPrize(DBWORKS *pWork)
{
	if( !g_pMainView->Cfg.bEventDB  ) return FALSE;
	// 테이블 생성
	if(!CreateEventPrizeDB()) {
		CLogFile logfile("EventPrize_Log.txt");
		logfile.Writef("GetEventPrize - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pEventPrizeDB == NULL) return FALSE;

	// 레코드셋이 열려있으면 닫는다
	if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();

	USERINFO *pUI = &pWork->UI;

	CString strQuery;
	// 사용자의 ID, 게임코드로 쿼리한다
	strQuery.Format(_T("[ID]='%s' AND [GameCode]= %d"), pWork->UI.ID, g_pMainView->Cfg.GameCode);

	// 이벤트  DB 읽기
	TRY
	{
		m_pEventPrizeDB->m_strFilter = strQuery;
		// DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pEventPrizeDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("EventPrize_Log.txt");
		logfile.Writef(_T("GetEventPrize error - GetEventPrize Open (ID=%s): %s"), pWork->UI.ID, (LPCTSTR)(e->m_strError));
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// 넷마블 회원ID는 있지만 이벤트  DB에 등록되어 있지 않음
	if(m_pEventPrizeDB->IsEOF() || m_pEventPrizeDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		if(AddNewEventPrizeDB(pUI) == FALSE) {
			CLogFile logfile;
			logfile.Writef("EventPrizeDB에러 - GetEventPrize (ID=%s) : Event DB 아이디 추가 실패", pUI->ID);
			if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
			return FALSE;
		}

		TRY
		{
			if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
			m_pEventPrizeDB->m_strFilter = strQuery;
			m_pEventPrizeDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("EventPrizeDB에러 - GetEventPrize Requery (ID=%s) : %s", pUI->ID, e->m_strError.operator LPCTSTR());
			if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
			ErrorCnt++;	// 에러 카운트 증가
			return FALSE;
		}
		END_CATCH
	}

	// 게임 DB추가 실패한 경우
	if(m_pEventPrizeDB->IsEOF() || m_pEventPrizeDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		CLogFile logfile;
		logfile.Writef("EventPrizeDB 에러 - GetEventPrize (ID=%s) : Event DB 아이디 추가 실패", pUI->ID);
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		return FALSE;
	}

	// 획득이벤트, 	게임판수, 초종게임일시
	pWork->EventPrize.EventPrize = m_pEventPrizeDB->m_EventCoin;
	pWork->EventPrize.GameCnt  	 = m_pEventPrizeDB->m_GameCount;
	pWork->EventPrize.OrigEventPrize = pWork->EventPrize.EventPrize;//백업용
	
	COleDateTime dbst = m_pEventPrizeDB->m_LastPlayDate; // 최종게임일시
	if(dbst.GetStatus() != COleDateTime::valid){// 시간 필드가 유효하지 않음
		dbst = COleDateTime::GetCurrentTime();
		m_pEventPrizeDB->m_LastPlayDate = dbst; // 현재 시간 셋
	}
	
	CTime nt = CTime::GetCurrentTime();  //시간경과 확인
	if(dbst.GetDay() != nt.GetDay()) {
		pWork->EventPrize.GameCnt = 0;
	}

	if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
	return TRUE;
}	

// ### [이벤트 티켓] ###
BOOL CDBThread::SetEventPrizeDB(char *id, EVENT_PRIZE *pEvent, BOOL bInit)
{
	if( !g_pMainView->Cfg.bEventDB || !g_pMainView->EventCfg.bRunEvent ) return FALSE;

	if(id==NULL) return FALSE;
	if(pEvent==NULL) return FALSE;

	if(!CreateEventPrizeDB()) {
		CLogFile logfile("EventPrize_Log.txt");
		logfile.Writef("SetEventDB - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pEventPrizeDB == NULL) return FALSE;
	if(pEvent == NULL) return FALSE;

	// 레코드셋이 열려있으면 닫는다
	if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();

	CString strQuery;
	// 사용자의 ID, 게임코드로 쿼리한다
	strQuery.Format(_T("[ID]='%s' AND [GameCode]= %d"), id, g_pMainView->Cfg.GameCode);

	// 아이템 DB 읽기
	TRY
	{
		m_pEventPrizeDB->m_strFilter = strQuery;
		m_pEventPrizeDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
		// 아이템이 존재하지 않는 경우 실패
		if(m_pEventPrizeDB->IsEOF()) {
			if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
			return FALSE;
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("EventPrize_Log.txt");
		logfile.Writef(_T("EventPrizeDB error - SetEventDB Open (ID=%s): %s"), id, (LPCTSTR)(e->m_strError));
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// 아이디가 일치하지 않는 경우 실패
	if(m_pEventPrizeDB->m_ID.CompareNoCase(id) != 0) {
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		return FALSE;
	}


	TRY
	{
		m_pEventPrizeDB->Edit();

		// 현재시간, 게임판수, 획득이벤트 카운트만 셋
		m_pEventPrizeDB->m_LastPlayDate = COleDateTime::GetCurrentTime(); // 최종 게임시간
		m_pEventPrizeDB->m_GameCount = pEvent->GameCnt;
		int Event =  pEvent->EventPrize - pEvent->OrigEventPrize;
		m_pEventPrizeDB->m_EventCoin += Event; // 증가량만

		// 실시간 기록
		pEvent->EventPrize = pEvent->OrigEventPrize =  m_pEventPrizeDB->m_EventCoin;

		m_pEventPrizeDB->Update();

	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("EventPrize_Log.txt");
		logfile.Writef(_T("EventPrizeDB error - SetEventDB() on Update (ID=%s): %s"), id, (LPCTSTR)(e->m_strError));
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		ErrorCnt++;
		return FALSE;
	}
	END_CATCH	



	if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();

	return TRUE;
}












/*
// [게임 아이템 작업] [점프 아이템 작업] 게임 아이템을 읽는다(아이템 서버 이라면) 
BOOL CDBThread::GetUserInfo2(char *id, char *pass, 
							USERINFO *pUI, BOOL *pIsAnotherConnect, BOOL *pIsWrongPwd, 
							int *pIDState, int *pSrvCode, BOOL *pNegPMoney, GAMEITEM_LIST *pItem) 
{
	if(!CreateUserDB() || !CreateGameDB() || !CreateAvatarDB() || !CreateItemDB()) {
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pUserDB==NULL || m_pGameDB==NULL || m_pAvatarDB==NULL || m_pItemDB==NULL) return FALSE;
	if(pUI == NULL || pItem == NULL) return FALSE;

	pItem->bLogable = FALSE;                 // 로그인 가능한가?
	pItem->nResult  = GAMEITEM_MES_ERROR;    // 로그인 불가코드


	CString str;
	str.Format(_T("[ID] = '%s'"), id);

	////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// 메인 DB 열기 ///////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	TRY
	{
		// 레코드셋이 열려있으면 닫는다
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();

		m_pUserDB->m_strFilter = str;
		// 메인유저DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pUserDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pUserDB->IsEOF() || m_pUserDB->m_ID.CompareNoCase(id)!=0)
		{
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			return FALSE;
		}
	}
	CATCH(CDBException, e)
	{
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Writef(_T("UserDB error - GetUserInfo Open (ID=%s): %s"), pUI->ID, e->m_strError.operator LPCTSTR());
		}
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// 비밀번호 검사
	if(pIsWrongPwd != NULL)
	{
		if(m_pUserDB->m_Passwd.CompareNoCase(pass)==0)
			*pIsWrongPwd = FALSE;
		else
		{
			*pIsWrongPwd = TRUE;
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			return FALSE;
		}
	}

	// 접속 중지 혹은 삭제된 아이디인지 검사
	if(pIDState != NULL) *pIDState = m_pUserDB->m_IDState;

	strncpy(pUI->ID, m_pUserDB->m_ID.operator LPCTSTR(), 15);
	strncpy(pUI->Pass, m_pUserDB->m_Passwd.operator LPCTSTR(), 8);
	pUI->Sex = (char)m_pUserDB->m_Sex;
	strncpy(pUI->Desc, m_pUserDB->m_Desc.operator LPCTSTR(), 60);

	// 여기서 나이산출한다
	//CString strAge = m_pUserDB->m_RegID;
	//if(strAge!=_T("")){
	//	pUI->nAge = GetAge(strAge);
	//}



	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// 아이템 DB 열기 //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	CString strItem; // 게임코드와 아이디를 읽는다
		
	if(g_pMainView->Cfg.bItemServer)
	{
		strItem.Format(_T("[c_gametype] = %d AND [c_code]>0 AND [c_code]<100 AND [id] = '%s'"), 
		g_pMainView->Cfg.GameCode, pUI->ID); // 스페셜 아이템은 c_code를 1 ~ 100 사이로 한다
	}
	else 
	{
		strItem.Format(_T("[c_gametype] = %d AND [c_code]>=200 AND [c_code]<300 AND [id] = '%s'"), 
		g_pMainView->Cfg.GameCode, pUI->ID); // [점프 아이템 작업] 은 c_code를 200 ~ 299 사이로 한다

	}

	// 아이템 DB 읽기
	TRY
	{
		m_pItemDB->m_strFilter = strItem;
		// 아이템DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pItemDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pItemDB->IsEOF() || m_pItemDB->m_ID.CompareNoCase(id)!=0) {
			
			pItem->nResult = GAMEITEM_MES_NOITEM; // 아이템을 보유하고 있지않다
	
			if(g_pMainView->Cfg.bItemServer)
			{
				if(m_pUserDB->IsOpen()) m_pUserDB->Close(); //[ 스페셜은 필요 ] 
			    if(m_pItemDB->IsOpen()) m_pItemDB->Close();
			    return FALSE;
			}
		}
	}
	CATCH(CDBException, e)
	{
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Writef(_T("ItemDB error - GetUserInfo Open (ID=%s): %s"), pUI->ID, e->m_strError.operator LPCTSTR());
		}
		pItem->nResult = GAMEITEM_MES_DBERROR; // 아이템 DB를 읽는데 실패하였다
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	COleDateTime nt = COleDateTime::GetCurrentTime();

	// 아이템정보 읽어오기
	int nItemNum = 0;
	TRY
	{
		//if(!m_pItemDB->IsBOF()) m_pItemDB->MoveFirst();
		for(;;)
		{
			if(m_pItemDB->IsEOF() || nItemNum>= MAX_ITEM_NUM) break; // 30개 읽어온 리스트 모두 읽거나, 갯수 초과되었다면 중지한다

			// 아이템 사용자 확인
			if(m_pItemDB->m_ID.CompareNoCase(id)==0) {
				short Day  = (short)m_pItemDB->m_Day; // 유효기간 (몇일짜리 1, 30,...)
				// 올바른 데이타가 아니다
				if(Day<=0){
					m_pItemDB->MoveNext();
					continue;					
				}
				// 유효한 기간을 세팅한다-분단위로 조정한다
				double period = (double)(Day*60*24);
				int   Use    = m_pItemDB->m_Use; // 사용유무
				SYSTEMTIME sysTime; memset(&sysTime,0,sizeof(SYSTEMTIME));
				if(Use>0){ // 사용되어진 아이템이라면 시작한 날짜가 있다
					COleDateTime st = m_pItemDB->m_Date; // 스타트시간
					if(st.GetStatus()!=COleDateTime::valid){
						m_pItemDB->MoveNext();
						continue;
					}
					st.GetAsSystemTime(sysTime);
					COleDateTimeSpan TimeSpan = nt - st;     // 현재시간과의 차이를 구한다
					if(TimeSpan.GetTotalMinutes()>period){   // 다쓴 아이템이다
						m_pItemDB->MoveNext();
						continue;					
					}
					// 사용된 아이템이 있고, 유효한개 1개라도 있다면 입장가능하다!!!
					pItem->bLogable = TRUE;
				}
				pItem->Item[nItemNum].Index  = nItemNum; // 인덱스 걸기
				pItem->Item[nItemNum].Day    = Day;      // 유효기간
				pItem->Item[nItemNum].Use    = (short)Use;      // 사용유무
				memcpy(&(pItem->Item[nItemNum].StartDate),&sysTime,sizeof(SYSTEMTIME)); // 시작 시간

				pItem->Item[nItemNum].NO     = (int)m_pItemDB->m_NO;    // 유니크넘버
				pItem->Item[nItemNum].Code   = (int)m_pItemDB->m_Code;  // 아이템 코드
				strncpy(pItem->Item[nItemNum].PresID,m_pItemDB->m_PresID.operator LPCTSTR(),15); // 선물준 아이디

				nItemNum++;
			}

			m_pItemDB->MoveNext();
		}
	}
	CATCH(CDBException, e)
	{
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Writef(_T("ItemDB error - GetUserInfo Move (ID=%s): %s"), pUI->ID, e->m_strError.operator LPCTSTR());
		}

		pItem->nResult = GAMEITEM_MES_DBERROR; // 읽어온 아이템 DB를 이동하는데 실패하였다
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		ErrorCnt++;
		return FALSE;
	}
	END_CATCH	


	// 스페셜 서버이구 아템이 없다면 (현재 점프와 스페셜 아템은 같이 사용할수 없다!!)
	if( g_pMainView->Cfg.bItemServer && nItemNum<=0)
	{
		pItem->nResult = GAMEITEM_MES_NOITEM; // 아이템을 보유하고 있지않다
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		return FALSE;
	}

	if(g_pMainView->Cfg.bJumpItem)
	{
		// [점프 아이템 작업]
		if(nItemNum<=0)
		{
			pItem->nResult = GAMEITEM_MES_NOITEM; // 아이템을 보유하고 있지않다
		}
		else
		{
			pItem->nResult = GAMEITEM_MES_SUCCESS; // 아이템을 보유하고 있다
		}
	}



	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// 아바타 DB 열기 //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	// 아바타 DB 읽기
	TRY
	{
		m_pAvatarDB->m_strFilter = str;
		// 메인유저DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pAvatarDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pAvatarDB->IsEOF() || m_pAvatarDB->m_ID.CompareNoCase(id)!=0) {
			// 아바타 DB가 존재하지 않으면 기본값 세팅(몸통 번호만 입력)
			if(pUI->Sex == 1) pUI->AvatarInfo[5] = 1;
			else pUI->AvatarInfo[5] = 2;
		}
		else {
			// 아바타 정보 저장
			pUI->AvatarInfo[0] = (short)m_pAvatarDB->m_layer0;
			pUI->AvatarInfo[1] = (short)m_pAvatarDB->m_layer1;
			pUI->AvatarInfo[2] = (short)m_pAvatarDB->m_layer2;
			pUI->AvatarInfo[3] = (short)m_pAvatarDB->m_layer3;
			pUI->AvatarInfo[4] = (short)m_pAvatarDB->m_layer4;
			pUI->AvatarInfo[5] = (short)m_pAvatarDB->m_layer5;
			pUI->AvatarInfo[6] = (short)m_pAvatarDB->m_layer6;
			pUI->AvatarInfo[7] = (short)m_pAvatarDB->m_layer7;
			pUI->AvatarInfo[8] = (short)m_pAvatarDB->m_layer8;
			pUI->AvatarInfo[9] = (short)m_pAvatarDB->m_layer9;
			pUI->AvatarInfo[10] = (short)m_pAvatarDB->m_layer10;
			pUI->AvatarInfo[11] = (short)m_pAvatarDB->m_layer11;
			pUI->AvatarInfo[12] = (short)m_pAvatarDB->m_layer12;
			pUI->AvatarInfo[13] = (short)m_pAvatarDB->m_layer13;
			pUI->AvatarInfo[14] = (short)m_pAvatarDB->m_layer14;
			pUI->AvatarInfo[15] = (short)m_pAvatarDB->m_layer15;
			pUI->AvatarInfo[16] = (short)m_pAvatarDB->m_layer16;
			pUI->AvatarInfo[17] = (short)m_pAvatarDB->m_layer17;
			pUI->AvatarInfo[18] = (short)m_pAvatarDB->m_layer18;
			pUI->AvatarInfo[19] = (short)m_pAvatarDB->m_layer19;
			pUI->AvatarInfo[20] = (short)m_pAvatarDB->m_layer20;
			pUI->AvatarInfo[21] = (short)m_pAvatarDB->m_layer21;
			pUI->AvatarInfo[22] = (short)m_pAvatarDB->m_layer22;
			pUI->AvatarInfo[23] = (short)m_pAvatarDB->m_layer23;
			pUI->AvatarInfo[24] = (short)m_pAvatarDB->m_layer24;
			pUI->AvatarInfo[25] = (short)m_pAvatarDB->m_layer25;
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("AvatarDB Error - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		return FALSE;
	}
	END_CATCH

	////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// 게임 DB 열기 ///////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	// 게임 DB 읽기
	TRY
	{
		m_pGameDB->m_strFilter = str;
		m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Writef(_T("GameDB error - GetUserInfo Open (ID=[%s]): %s"), id, e->m_strError.operator LPCTSTR());
		}
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH
	

	// 넷마블 회원ID는 있지만 게임 DB에 등록되어 있지 않음
	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		if(AddNewGameDB(pUI) == FALSE)
		{
			CString strFileName = GetLogFileName();
			if(strFileName!=_T("")){
				CLogFile logfile(strFileName.operator LPCTSTR());
				logfile.Writef(_T("GameDB error - GetUserInfo (ID=[%s]) : Failed add new ID"), id);
			}
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			if(m_pItemDB->IsOpen()) m_pItemDB->Close();
			if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			return FALSE;
		}

		TRY
		{
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			m_pGameDB->m_strFilter = str;
			m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		}
		CATCH(CDBException, e)
		{
			CString strFileName = GetLogFileName();
			if(strFileName!=_T("")){
				CLogFile logfile(strFileName.operator LPCTSTR());
				logfile.Writef(_T("GameDB error - GetUserInfo Requery (ID=[%s]) : %s"), id, e->m_strError.operator LPCTSTR());
			}
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			if(m_pItemDB->IsOpen()) m_pItemDB->Close();
			if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			ErrorCnt++;	// 에러 카운트 증가
			return FALSE;
		}
		END_CATCH
	}

	// 게임 DB추가 실패한 경우
	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Writef(_T("GameDB error (ID=[%s]) : Failed add new ID"), id);
		}
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// 게임 DB를 읽어옴 (KHS_FAST_CHAN)

	// ##############  [세븐포커] 에러처리

	INT64 nPMoney = (INT64)::_atoi64(m_pGameDB->m_PMoney.operator LPCTSTR());
	
	// 일정량의 정해진 포커머니만 불러와서...나중에 데이타베이스에 그 포커머니의 증감치만 기록하면 된다!!
	if(nPMoney>g_pMainView->Cfg.MAX_PMONEY) nPMoney = g_pMainView->Cfg.MAX_PMONEY; // [ $$$$ 포커머니 오버플로우 체크 $$$$ ]

	pUI->PMoney  = nPMoney;
	pUI->pBackup = nPMoney; // 포커머니 백업

	// 레벨상점 폐쇄
	pUI->nIcon = GetPokerLevel(nPMoney);

	pUI->WinNum    = m_pGameDB->m_Vic;
	pUI->LooseNum  = m_pGameDB->m_Fail;
//	pUI->DrawNum   = m_pGameDB->m_Fold;

	// 카드 인덱스
	pUI->PI.Card[ 0] = (char)m_pGameDB->m_CARD04;
	pUI->PI.Card[ 1] = (char)m_pGameDB->m_CARD05;
	pUI->PI.Card[ 2] = (char)m_pGameDB->m_CARD06;
	pUI->PI.Card[ 3] = (char)m_pGameDB->m_CARD07;
	pUI->PI.Card[ 4] = (char)m_pGameDB->m_CARD08;
	pUI->PI.Card[ 5] = (char)m_pGameDB->m_CARD09;
	pUI->PI.Card[ 6] = (char)m_pGameDB->m_CARD10;
	pUI->PI.Card[ 7] = (char)m_pGameDB->m_CARD11;
	pUI->PI.Card[ 8] = (char)m_pGameDB->m_CARD12;
	pUI->PI.Card[ 9] = (char)m_pGameDB->m_CARD13;
	pUI->PI.Card[10] = (char)m_pGameDB->m_CARD14;
	pUI->PI.Card[11] = (char)m_pGameDB->m_CARD15;
	pUI->PI.nCard    = (char)m_pGameDB->m_Level2; // 대표카드
	pUI->PI.nRSF     = m_pGameDB->m_RSF;
	pUI->PI.nSF      = m_pGameDB->m_SF;
	pUI->PI.nFCARD   = m_pGameDB->m_FCARD;

	// 접속중인 서버코드 검사
	if(pSrvCode != NULL) *pSrvCode = m_pGameDB->m_IsConnected;

	// [게임 아이템 작업] ########### 아이템 처리작업 순서 주의 ###########

	// 아바타/게임디비를 성공적으로 읽고/처리했다면,
	// 성공적이었던 아이템 관련 정보를 세팅한다 !!!!


	pItem->nNum = nItemNum; // 읽어온 아이템 갯수를 저장한다(아이템이 0보다 커야한다!!!!)

//	 [점프 아이템 작어] 제거
	// 유효하게 사용중인 아이템이 있다
//	if(pItem->bLogable){
//		pItem->nResult = GAMEITEM_MES_SUCCESS; // 로그인 가능하다
//	}
//	// 한번도 안쓴 아이템이 한개이상 존재한다
//	else{
//		pItem->nResult = GAMEITEM_MES_USEABLE; // 아이템사용 문의
//	}

	if(m_pUserDB->IsOpen()) m_pUserDB->Close();
	if(m_pItemDB->IsOpen()) m_pItemDB->Close();
	if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;
}
*/

/*
// [게임 아이템 작업] [점프 아이템 작업] 게임 아이템 사용을 기록한다. 
BOOL CDBThread::SetLogInInfo(USERINFO *pUI, GAMEITEM_LIST *pItem)
{
	if(pUI == NULL || pItem == NULL) return FALSE;

	if(!CreateItemDB()) {
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pItemDB==NULL) return FALSE;
	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// 아이템 DB 열기 //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	pItem->nResult = GAMEITEM_MES_NOITEM; // 사용가능한 아이템이 없다

	int nNO = pItem->Item[0].NO;
	CString strItem; // 유니크코드를 주어 아이템을 읽는다 ID 는 필요 없는가???
	strItem.Format(_T("[c_no] = %d"), nNO);

	TRY
	{
		m_pItemDB->m_strFilter = strItem;
		m_pItemDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
	}
	CATCH(CDBException, e)
	{
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		ErrorCnt = 100; // DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		return FALSE;
	}
	END_CATCH

	// 존재하는 아이템이 없거나, 유니크아이디가 틀리거나, 보유자가 틀리다면 입장 불가다
	if(m_pItemDB->IsEOF() || m_pItemDB->m_NO != nNO || m_pItemDB->m_ID.CompareNoCase(pUI->ID) != 0)
	{
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		return FALSE;
	}

	BOOL bUse = m_pItemDB->m_Use;
	// 이미 사용중인 아이템이라면..아마도 다른곳(웹,게임서버)에서 사용플래그를 켰을지도 모른다
	// 사용하지 않은 아이템일경우만 업그레이드 한다
	if(!bUse)
	{
		// DB를 수정하는 상태
		TRY
		{
			m_pItemDB->Edit();
			
			// 아이템 사용정보 작성
			m_pItemDB->m_Use  = 1;
			m_pItemDB->m_Date = pItem->Item[0].StartDate; // 시작 시간( 현재 시간)

			// 게임 DB 업데이트
			m_pItemDB->Update();
		}
		CATCH(CDBException, e)
		{
			if(m_pItemDB->IsOpen()) m_pItemDB->Close();
			pItem->nResult = GAMEITEM_MES_USEFAIL; // 아이템 사용에 실패하였다
			ErrorCnt++;	// 에러 카운트 증가
			return FALSE;
		}
		END_CATCH
	}

	// 오픈한 DB를 닫는다
	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	return TRUE;
}
*/





/*

// [비공개방 아이템] 
//BOOL CDBThread::GetUserInfo(char *id, char *pass, USERINFO *pUI, BOOL *pIsAnotherConnect, BOOL *pIsWrongPwd, int *pIDState) 
BOOL CDBThread::GetUserInfo1(char *id, char *pass, USERINFO *pUI, BOOL *pIsAnotherConnect, BOOL *pIsWrongPwd, int *pIDState, int *pSrvCode, BOOL *pNegPMoney, GAMEITEM_LIST *pItem) 
{
	if(!CreateUserDB() || !CreateGameDB() || !CreateAvatarDB() || !CreateItemDB()) {
		CLogFile logfile;
		logfile.Writef("GetUserInfo - DB 컨넥션 연결 실패");
		// 컨넥션 오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pUserDB==NULL || m_pGameDB==NULL || m_pAvatarDB==NULL || m_pItemDB==NULL) return FALSE;
	if(pUI == NULL || pItem == NULL) return FALSE;


	pItem->bLogable = FALSE;                 // 로그인 가능한가?
	pItem->nResult  = GAMEITEM_MES_ERROR;    // 로그인 불가코드



	CString str;
	str.Format("[ID] = '%s'", id);

	TRY
	{
		// 레코드셋이 열려있으면 닫는다
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();

		m_pUserDB->m_strFilter = str;
		// 메인유저DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pUserDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pUserDB->IsEOF() || m_pUserDB->m_ID.CompareNoCase(id)!=0)
		{
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			return FALSE;
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("UserDB에러 - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// 비밀번호 검사
	if(pIsWrongPwd != NULL) {
		if(m_pUserDB->m_Passwd.CompareNoCase(pass)==0)
			*pIsWrongPwd = FALSE;
		else {
			*pIsWrongPwd = TRUE;
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			return FALSE;
		}
	}

	// 다른 게임에 이미 접속중인지 검사
	//if(pIsAnotherConnect != NULL) *pIsAnotherConnect = m_pUserDB->m_IsConnected;

	// 접속 중지 혹은 삭제된 아이디인지 검사
	if(pIDState != NULL) *pIDState = m_pUserDB->m_IDState;

	strncpy(pUI->ID, m_pUserDB->m_ID.operator LPCTSTR(), 15);
	strncpy(pUI->Pass, m_pUserDB->m_Passwd.operator LPCTSTR(), 8);
	//strncpy(pUI->Name, m_pUserDB->m_Name.operator LPCTSTR(), 10);
	pUI->Sex = (char)m_pUserDB->m_Sex;
//	pUI->Marble = (int)m_pUserDB->m_Total_Marble;
	strncpy(pUI->Desc, m_pUserDB->m_Desc.operator LPCTSTR(), 60);

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// 아이템 DB 열기 //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	CString strItem; // 게임코드와 아이디를 읽는다
	//strItem.Format(_T("[c_gametype] = %d AND [id] = '%s'"), g_pMainView->Cfg.GameCode, pUI->ID);
	strItem.Format(_T("[c_gametype] = %d AND [c_code]>0 AND [c_code]<100 AND [id] = '%s'"), 
		g_pMainView->Cfg.GameCode, pUI->ID); // 스페셜 아이템은 c_code를 1 ~ 100 사이로 한다

	// 아이템 DB 읽기
	TRY
	{
		m_pItemDB->m_strFilter = strItem;
		// 아이템DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pItemDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pItemDB->IsEOF() || m_pItemDB->m_ID.CompareNoCase(id)!=0) {
			pItem->nResult = GAMEITEM_MES_NOITEM; // 아이템을 보유하고 있지않다
		// ############### 비밀방 아이템이 없다고 종료시킬순 없다 ##############
		//	if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		//	if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		//	return FALSE;
		}
	}
	CATCH(CDBException, e)
	{
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Writef(_T("ItemDB error - GetUserInfo Open (ID=%s): %s"), pUI->ID, e->m_strError.operator LPCTSTR());
		}
		pItem->nResult = GAMEITEM_MES_DBERROR; // 아이템 DB를 읽는데 실패하였다
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	COleDateTime nt = COleDateTime::GetCurrentTime();

	// 아이템정보 읽어오기
	int nItemNum = 0;
	TRY
	{
		//if(!m_pItemDB->IsBOF()) m_pItemDB->MoveFirst();
		for(;;)
		{
			if(m_pItemDB->IsEOF() || nItemNum>= MAX_ITEM_NUM) break; // 읽어온 리스트 모두 읽거나, 갯수 초과되었다면 중지한다

			// 아이템 사용자 확인
			if(m_pItemDB->m_ID.CompareNoCase(id)==0) {
				short Day  = (short)m_pItemDB->m_Day; // 유효기간
				// 올바른 데이타가 아니다
				if(Day<=0){
					m_pItemDB->MoveNext();
					continue;					
				}
				// 유효한 기간을 세팅한다-분단위로 조정한다
				double period = (double)(Day*60*24);
				int   Use    = m_pItemDB->m_Use; // 사용유무
				SYSTEMTIME sysTime; memset(&sysTime,0,sizeof(SYSTEMTIME));
				if(Use>0){ // 사용되어진 아이템이라면 시작한 날짜가 있다
					COleDateTime st = m_pItemDB->m_Date; // 스타트시간
					if(st.GetStatus()!=COleDateTime::valid){
						m_pItemDB->MoveNext();
						continue;
					}
					st.GetAsSystemTime(sysTime);
					COleDateTimeSpan TimeSpan = nt - st;     // 현재시간과의 차이를 구한다
					if(TimeSpan.GetTotalMinutes()>period){   // 다쓴 아이템이다
						m_pItemDB->MoveNext();
						continue;					
					}
					// 사용된 아이템이 있고, 유효한개 1개라도 있다면 입장가능하다!!!
					pItem->bLogable = TRUE;
				}
				pItem->Item[nItemNum].Index  = nItemNum; // 인덱스 걸기
				pItem->Item[nItemNum].Day    = Day;      // 유효기간
				pItem->Item[nItemNum].Use    = (short)Use;      // 사용유무
				memcpy(&(pItem->Item[nItemNum].StartDate),&sysTime,sizeof(SYSTEMTIME)); // 시작 시간

				pItem->Item[nItemNum].NO     = (int)m_pItemDB->m_NO;    // 유니크넘버
				pItem->Item[nItemNum].Code   = (int)m_pItemDB->m_Code;  // 아이템 코드
				strncpy(pItem->Item[nItemNum].PresID,m_pItemDB->m_PresID.operator LPCTSTR(),15); // 선물준 아이디

				nItemNum++;
			}

			m_pItemDB->MoveNext();
		}
	}
	CATCH(CDBException, e)
	{
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Writef(_T("ItemDB error - GetUserInfo Move (ID=%s): %s"), pUI->ID, e->m_strError.operator LPCTSTR());
		}

		pItem->nResult = GAMEITEM_MES_DBERROR; // 읽어온 아이템 DB를 이동하는데 실패하였다
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		ErrorCnt++;
		return FALSE;
	}
	END_CATCH	

	// 쓸만한 아이템이 한개도 없다면...
	if(nItemNum<=0){
		pItem->nResult = GAMEITEM_MES_NOITEM; // 아이템을 보유하고 있지않다
		// ############### 비밀방 아이템이 없다고 종료시킬순 없다 ##############
		//if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		//if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		//return FALSE;
	}




	// 아바타 DB 읽기
	TRY
	{
		m_pAvatarDB->m_strFilter = str;
		// 메인유저DB를 readOnly모드로 열고 속도 향상을 위하여 excuteDirect 옵션을 사용(Requery할 수 없음)
		m_pAvatarDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pAvatarDB->IsEOF() || m_pAvatarDB->m_ID.CompareNoCase(id)!=0) {
			// 아바타 DB가 존재하지 않으면 기본값 세팅(몸통 번호만 입력)
			if(pUI->Sex == 1) pUI->AvatarInfo[5] = 1;
			else pUI->AvatarInfo[5] = 2;
		}
		else {
			// 아바타 정보 저장
			pUI->AvatarInfo[0] = (short)m_pAvatarDB->m_layer0;
			pUI->AvatarInfo[1] = (short)m_pAvatarDB->m_layer1;
			pUI->AvatarInfo[2] = (short)m_pAvatarDB->m_layer2;
			pUI->AvatarInfo[3] = (short)m_pAvatarDB->m_layer3;
			pUI->AvatarInfo[4] = (short)m_pAvatarDB->m_layer4;
			pUI->AvatarInfo[5] = (short)m_pAvatarDB->m_layer5;
			pUI->AvatarInfo[6] = (short)m_pAvatarDB->m_layer6;
			pUI->AvatarInfo[7] = (short)m_pAvatarDB->m_layer7;
			pUI->AvatarInfo[8] = (short)m_pAvatarDB->m_layer8;
			pUI->AvatarInfo[9] = (short)m_pAvatarDB->m_layer9;
			pUI->AvatarInfo[10] = (short)m_pAvatarDB->m_layer10;
			pUI->AvatarInfo[11] = (short)m_pAvatarDB->m_layer11;
			pUI->AvatarInfo[12] = (short)m_pAvatarDB->m_layer12;
			pUI->AvatarInfo[13] = (short)m_pAvatarDB->m_layer13;
			pUI->AvatarInfo[14] = (short)m_pAvatarDB->m_layer14;
			pUI->AvatarInfo[15] = (short)m_pAvatarDB->m_layer15;
			pUI->AvatarInfo[16] = (short)m_pAvatarDB->m_layer16;
			pUI->AvatarInfo[17] = (short)m_pAvatarDB->m_layer17;
			pUI->AvatarInfo[18] = (short)m_pAvatarDB->m_layer18;
			pUI->AvatarInfo[19] = (short)m_pAvatarDB->m_layer19;
			pUI->AvatarInfo[20] = (short)m_pAvatarDB->m_layer20;
			pUI->AvatarInfo[21] = (short)m_pAvatarDB->m_layer21;
			pUI->AvatarInfo[22] = (short)m_pAvatarDB->m_layer22;
			pUI->AvatarInfo[23] = (short)m_pAvatarDB->m_layer23;
			pUI->AvatarInfo[24] = (short)m_pAvatarDB->m_layer24;
			pUI->AvatarInfo[25] = (short)m_pAvatarDB->m_layer25;
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("AvatarDB에러 - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		return FALSE;
	}
	END_CATCH

	// 게임 DB 읽기
	TRY
	{
		m_pGameDB->m_strFilter = str;
		m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("GameDB에러 - GetUserInfo Open (ID=%s): %s", id, e->m_strError.operator LPCTSTR());
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		// DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH
	

	// 넷마블 회원ID는 있지만 게임 DB에 등록되어 있지 않음
	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		if(AddNewGameDB(pUI) == FALSE) {
			CLogFile logfile;
			logfile.Writef("GameDB에러 - GetUserInfo (ID=%s) : Game DB 아이디 추가 실패", id);
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			return FALSE;
		}

		TRY
		{
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			m_pGameDB->m_strFilter = str;
			m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("GameDB에러 - GetUserInfo Requery (ID=%s) : %s", id, e->m_strError.operator LPCTSTR());
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			ErrorCnt++;	// 에러 카운트 증가
			return FALSE;
		}
		END_CATCH
	}

	// 게임 DB추가 실패한 경우
	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		CLogFile logfile;
		logfile.Writef("GameDB에러 (ID=%s) : Game DB 아이디 추가 실패", id);
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// 게임 DB를 읽어옴 (KHS_FAST_CHAN)
	// ##############  [세븐포커] 에러처리
	INT64 nPMoney = (INT64)::_atoi64(m_pGameDB->m_PMoney.operator LPCTSTR());
	if(nPMoney<0){ // ###### 포커머니 에러다
		CLogFile logfile(_T("Abuse.txt"));
		logfile.Writef(g_StrMan._Get(_T("SPOKER_INVAL_PM")), id, nPMoney);
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();

		if(pNegPMoney!=NULL) *pNegPMoney = TRUE;

		return FALSE;
	}
	// 일정량의 정해진 포커머니만 불러와서...나중에 데이타베이스에 그 포커머니의 증감치만 기록하면 된다!!
	if(nPMoney>g_pMainView->Cfg.MAX_PMONEY) nPMoney = g_pMainView->Cfg.MAX_PMONEY; // [ $$$$ 포커머니 오버플로우 체크 $$$$ ]

	pUI->PMoney  = nPMoney;
	pUI->pBackup = nPMoney; // 포커머니 백업

	// 레벨상점 폐쇄
	pUI->nIcon = GetPokerLevel(nPMoney);

//	pUI->Character = m_pGameDB->m_Character;
	pUI->WinNum    = m_pGameDB->m_Vic;
	pUI->LooseNum  = m_pGameDB->m_Fail;
//	pUI->DrawNum   = m_pGameDB->m_Fold;
//	pUI->Point     = m_pGameDB->m_Level1;

	// 카드 인덱스 ???
	pUI->PI.Card[ 0] = (char)m_pGameDB->m_CARD04;
	pUI->PI.Card[ 1] = (char)m_pGameDB->m_CARD05;
	pUI->PI.Card[ 2] = (char)m_pGameDB->m_CARD06;
	pUI->PI.Card[ 3] = (char)m_pGameDB->m_CARD07;
	pUI->PI.Card[ 4] = (char)m_pGameDB->m_CARD08;
	pUI->PI.Card[ 5] = (char)m_pGameDB->m_CARD09;
	pUI->PI.Card[ 6] = (char)m_pGameDB->m_CARD10;
	pUI->PI.Card[ 7] = (char)m_pGameDB->m_CARD11;
	pUI->PI.Card[ 8] = (char)m_pGameDB->m_CARD12;
	pUI->PI.Card[ 9] = (char)m_pGameDB->m_CARD13;
	pUI->PI.Card[10] = (char)m_pGameDB->m_CARD14;
	pUI->PI.Card[11] = (char)m_pGameDB->m_CARD15;
	pUI->PI.nCard    = (char)m_pGameDB->m_Level2; // 대표카드
	pUI->PI.nRSF     = m_pGameDB->m_RSF;
	pUI->PI.nSF      = m_pGameDB->m_SF;
	pUI->PI.nFCARD   = m_pGameDB->m_FCARD;


	// 접속중인 서버코드 검사
	if(pSrvCode != NULL) *pSrvCode = m_pGameDB->m_IsConnected;



	// [게임 아이템 작업] ########### 아이템 처리작업 순서 주의 ###########

	// 아바타/게임디비를 성공적으로 읽고/처리했다면,
	// 성공적이었던 아이템 관련 정보를 세팅한다 !!!!

	pItem->nNum = nItemNum; // 읽어온 아이템 갯수를 저장한다(아이템이 0보다 커야한다!!!!)

	// 유효하게 사용중인 아이템이 있다
	if(pItem->bLogable){
		pItem->nResult = GAMEITEM_MES_SUCCESS; // 로그인 가능하다
	}
	// 한번도 안쓴 아이템이 한개이상 존재한다
	else{
		pItem->nResult = GAMEITEM_MES_USEABLE; // 아이템사용 문의
	}



	if(m_pUserDB->IsOpen()) m_pUserDB->Close();
	if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;
}
*/


/*
// 세븐포커 게임을 중복 접속을 기록한다(게임로그인시 기록하는 루틴이다)
BOOL CDBThread::SetLogInInfo(USERINFO *pUI, int *pSrvCode, BOOL &bDCon)
{

	return FALSE;

	if(pUI == NULL) return FALSE;

	/////////////////////////////// 게임디비 업데이트 ///////////////////////////////////

	if(!CreateGameDB()) {
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pGameDB==NULL) return FALSE;
	if(m_pGameDB->IsOpen()) m_pGameDB->Close(); // 레코드셋이 열려있으면 닫는다

	CString str;
	str.Format(_T("[ID] = '%s'"), pUI->ID);
	
	TRY
	{
		m_pGameDB->m_strFilter = str;
		m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
	}
	CATCH(CDBException, e)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt = 100; // DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		return FALSE;
	}
	END_CATCH

	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID) != 0)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// 다시한번 로그인 중복 접속 비교하기
	if(m_pGameDB->m_IsConnected!=0){
		bDCon = TRUE;
		if(pSrvCode  != NULL) *pSrvCode  = m_pGameDB->m_IsConnected;
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}
		
	// DB를 수정하는 상태
	TRY
	{
		m_pGameDB->Edit();
		// 접속코드 작성
		m_pGameDB->m_IsConnected = g_pMainView->Cfg.ServerCode;//g_MainView.ServerCode;
		// 게임 DB 업데이트
		m_pGameDB->Update();
	}
	CATCH(CDBException, e)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt++;	// 에러 카운트 증가
		return FALSE;
	}
	END_CATCH

	// 오픈한 DB를 닫는다
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;
}
*/

/*
// 세븐포커 게임을 중복 접속을 기록한다(게임로그아웃시 기록하는 루틴이다)
BOOL CDBThread::SetLogOutInfo(USERINFO *pUI)
{
	if(pUI == NULL) return FALSE;

	/////////////////////////////// 게임디비 업데이트 ///////////////////////////////////

	if(!CreateGameDB()) {
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pGameDB==NULL) return FALSE;
	if(m_pGameDB->IsOpen()) m_pGameDB->Close(); // 레코드셋이 열려있으면 닫는다

	CString str;
	str.Format(_T("[ID] = '%s'"), pUI->ID);

	TRY
	{
		m_pGameDB->m_strFilter = str;
		m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
	}
	CATCH(CDBException, e)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt = 100; // DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		return FALSE;
	}
	END_CATCH

	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID) != 0)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// 이미 0 으로 세팅되어 있다면 불필요하다, 또는 이미 다른서버에 접속중이다!
	if(m_pGameDB->m_IsConnected==0 || m_pGameDB->m_IsConnected!=g_pMainView->Cfg.ServerCode)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return TRUE; // 참값 리턴
	}
		
	// DB를 수정하는 상태
	TRY
	{
		m_pGameDB->Edit();
		// 접속 종료시 접속 코드를 초기화
		m_pGameDB->m_IsConnected = 0; 
		// 게임 DB 업데이트
		m_pGameDB->Update();
	}
	CATCH(CDBException, e)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt++;	// 에러 카운트 증가
		return FALSE;
	}
	END_CATCH

	// 오픈한 DB를 닫는다
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;
}
*/

/*
// 세븐포커 게임을 기록한다(게임중간에 기록하는 루틴이다)
BOOL CDBThread::SetGameInfo(USERINFO *pUI)
{
	
	return FALSE;





	if(pUI == NULL) return FALSE;

	/////////////////////////////// 게임디비 업데이트 ///////////////////////////////////

	if(!CreateGameDB()) {
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pGameDB==NULL) return FALSE;
	if(m_pGameDB->IsOpen()) m_pGameDB->Close(); // 레코드셋이 열려있으면 닫는다

	CString str;
	str.Format(_T("[ID] = '%s'"), pUI->ID);

	TRY
	{
		m_pGameDB->m_strFilter = str;
		m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
	}
	CATCH(CDBException, e)
	{
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			//logfile.Writef(_T("Save Interval data error - SetGameInfo Open (ID=[%s], UNum=%d,  Win=%d, Fail=%d, Fold=%d, AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d) : %s"),
			//	pUI->ID, pUI->UNum,  pUI->WinNum, pUI->LooseNum, pUI->DrawNum, (INT64)Max(pUI->PMoney,0)-pUI->pBackup, pUI->PI.nRSF, pUI->PI.nSF, pUI->PI.nFCARD, e->m_strError.operator LPCTSTR());
			logfile.Writef(_T("Save Interval data error - SetGameInfo Open (ID=[%s], UNum=%d,  Win=%d, Fail=%d,  AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d) : %s"),
				pUI->ID, pUI->UNum,  pUI->WinNum, pUI->LooseNum,  (INT64)Max(pUI->PMoney,0)-pUI->pBackup, pUI->PI.nRSF, pUI->PI.nSF, pUI->PI.nFCARD, e->m_strError.operator LPCTSTR());
		}
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt = 100; // DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		return FALSE;
	}
	END_CATCH

	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID) != 0)
	{
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			//logfile.Writef(_T("Save Interval data error - Can't find UserID from GameDB (ID=[%s], UNum=%d,  Win=%d, Fail=%d, Fold=%d, AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d)"),
			//	pUI->ID, pUI->UNum,  pUI->WinNum, pUI->LooseNum, pUI->DrawNum, (INT64)Max(pUI->PMoney,0)-pUI->pBackup, pUI->PI.nRSF, pUI->PI.nSF, pUI->PI.nFCARD);
			logfile.Writef(_T("Save Interval data error - Can't find UserID from GameDB (ID=[%s], UNum=%d,  Win=%d, Fail=%d,  AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d)"),
				pUI->ID, pUI->UNum,  pUI->WinNum, pUI->LooseNum,  (INT64)Max(pUI->PMoney,0)-pUI->pBackup, pUI->PI.nRSF, pUI->PI.nSF, pUI->PI.nFCARD);
		}

		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}
	
	// DB를 수정하는 상태
	TRY
	{
		m_pGameDB->Edit();

		m_pGameDB->m_Vic     = pUI->WinNum ;
		m_pGameDB->m_Fail    = pUI->LooseNum;
//		m_pGameDB->m_Fold    = pUI->DrawNum;
//		m_pGameDB->m_Level1  = pUI->Point;


		INT64 NowPMoney  = (INT64)::_atoi64(m_pGameDB->m_PMoney.operator LPCTSTR()); // 변경되었을지 모르는 데이타베이스 포커머니
		INT64 GamePMoney = pUI->PMoney;                // 접속후의 게임머니
		if(GamePMoney<0) GamePMoney = 0;               // 접속후의 게임머니는 딱 맞아야한다. 음수값을 0으로 처리해 놓는다!!!
		INT64 ResPMoney  = GamePMoney - pUI->pBackup;  // 포커머니 가감치 계산
//		INT64 ResPMoney  = pUI->PMoney - pUI->pBackup; // 기존방식의 포커머니 가감치 계산
		NowPMoney+=ResPMoney;                          // 포커머니 가감산
		
		// 변경
		if(NowPMoney<0) 
		{
			CLogFile logfile(_T("Abuse.txt"));
			logfile.Writef(g_StrMan._Get(_T("SPOKER_INVAL_PM")), pUI->ID, NowPMoney);
			NowPMoney = 0;                 // 어뷰져 방지를 위해 음수(<0)처리 하지 않는다!!!
		}

		// 일정량의 정해진 포커머니만 불러와서...나중에 데이타베이스에 그 포커머니의 증감치만 기록하면 된다!!
		if(NowPMoney>g_pMainView->Cfg.MAX_PMONEY) NowPMoney = g_pMainView->Cfg.MAX_PMONEY; // [ $$$$ 포커머니 오버플로우 체크 $$$$ ]

		// 레벨 쓰기
		m_pGameDB->m_Level   = GetPokerLevel(NowPMoney);  // 포커레벨 기록하지 않는다 웹에서만 사용하는 읽기전용데이타이다


		CString strPM; strPM.Format(_T("%I64d"),NowPMoney);
		m_pGameDB->m_PMoney = strPM;

		m_pGameDB->m_CARD04 = (BOOL)pUI->PI.Card[ 0];
		m_pGameDB->m_CARD05 = (BOOL)pUI->PI.Card[ 1];
		m_pGameDB->m_CARD06 = (BOOL)pUI->PI.Card[ 2];
		m_pGameDB->m_CARD07 = (BOOL)pUI->PI.Card[ 3];
		m_pGameDB->m_CARD08 = (BOOL)pUI->PI.Card[ 4];
		m_pGameDB->m_CARD09 = (BOOL)pUI->PI.Card[ 5];
		m_pGameDB->m_CARD10 = (BOOL)pUI->PI.Card[ 6];
		m_pGameDB->m_CARD11 = (BOOL)pUI->PI.Card[ 7];
		m_pGameDB->m_CARD12 = (BOOL)pUI->PI.Card[ 8];
		m_pGameDB->m_CARD13 = (BOOL)pUI->PI.Card[ 9];
		m_pGameDB->m_CARD14 = (BOOL)pUI->PI.Card[10];
		m_pGameDB->m_CARD15 = (BOOL)pUI->PI.Card[11];
		m_pGameDB->m_Level2 = pUI->PI.nCard; // 대표카드
		m_pGameDB->m_RSF    = pUI->PI.nRSF;
		m_pGameDB->m_SF     = pUI->PI.nSF;
		m_pGameDB->m_FCARD  = pUI->PI.nFCARD;

		m_pGameDB->m_WinPer = GetWinPro(pUI->WinNum, pUI->LooseNum);//, pUI->DrawNum);

		// 먼저 게임 DB 업데이트
		m_pGameDB->Update();

	}
	CATCH(CDBException, e)
	{
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			//logfile.Writef(_T("Save Interval data error - SetUserInfo Update (ID=[%s], UNum=%d,  Win=%d, Fail=%d, Fold=%d, AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d) : %s"),
			//	pUI->ID, pUI->UNum,  pUI->WinNum, pUI->LooseNum, pUI->DrawNum, (INT64)Max(pUI->PMoney,0)-pUI->pBackup, pUI->PI.nRSF, pUI->PI.nSF, pUI->PI.nFCARD, e->m_strError.operator LPCTSTR());
			logfile.Writef(_T("Save Interval data error - SetUserInfo Update (ID=[%s], UNum=%d,  Win=%d, Fail=%d,  AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d) : %s"),
				pUI->ID, pUI->UNum,  pUI->WinNum, pUI->LooseNum,  (INT64)Max(pUI->PMoney,0)-pUI->pBackup, pUI->PI.nRSF, pUI->PI.nSF, pUI->PI.nFCARD, e->m_strError.operator LPCTSTR());
		}

		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt++;	// 에러 카운트 증가
		return FALSE;
	}
	END_CATCH

	// 오픈한 DB를 닫는다
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;
}
*/

/*
// 중복접속을 확인한다
BOOL CDBThread::AnalDoubleConnect(USERINFO *pUI, int *pSrvCode)
{
	if(pUI == NULL) return FALSE;

	/////////////////////////////// 게임디비 업데이트 ///////////////////////////////////

	if(!CreateGameDB()) {
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pGameDB==NULL) return FALSE;
	if(m_pGameDB->IsOpen()) m_pGameDB->Close(); // 레코드셋이 열려있으면 닫는다

	CString str;
	str.Format(_T("[ID] = '%s'"), pUI->ID);

	TRY
	{
		m_pGameDB->m_strFilter = str;
		m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
	}
	CATCH(CDBException, e)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt = 100; // DB오픈 실패인 경우 에러 카운트를 100으로 세팅
		return FALSE;
	}
	END_CATCH

	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID) != 0)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// 서버코드가 같지않다면...중복 접속이다!
	if(m_pGameDB->m_IsConnected!=g_pMainView->Cfg.ServerCode)
	{
		if(pSrvCode  != NULL) *pSrvCode  = m_pGameDB->m_IsConnected;

		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return TRUE; // 중복접속 의미인 참값 리턴
	}
		
	// 오픈한 DB를 닫는다
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return FALSE;
}
*/