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



// [SSO �۾�] - CDBQue Ŭ���� ��ü�� �ٲ۴�...

//////////////////////////////////////////////////////////////////////
// DBť Ŭ����
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

	// DBť�� �ִ� ������ �ִ� ���� ������ 1.25�� ������ ����
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
// DB ó�� ������ Ŭ����
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
	pDatabase_Item = NULL;		// [���� ������ �۾�]
	pDatabase_Premium = NULL;	// [�����̾� �۾�]
	pDatabase_PremIP = NULL;	// [PC�� �۾�]
	pDatabase_EventPrize = NULL; // ### [�̺�Ʈ Ƽ��] ###

	m_pUserDB = NULL;
	m_pGameDB = NULL;
	m_pStatusDB = NULL;
	m_pAvatarDB = NULL;
	m_pItemDB = NULL;		// [���� ������ �۾�]
	m_pPremiumDB = NULL;	// [�����̾� �۾�]
	m_pPremIPDB = NULL;		// [PC�� �۾�]
	m_pEventPrizeDB= NULL;  // ### [�̺�Ʈ Ƽ��] ###
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
		// �����带 ������� ���� �����Ѵ�(������ ���ν������� ���� �������� �ʱ�ȭ�� ��ٷ����ϱ⶧��)
		pThread = ::AfxBeginThread(DBThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
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

BOOL CDBThread::Terminate()
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
			CString strFileName = GetLogFileName();
			if(strFileName!=_T("")){
				CLogFile logfile(strFileName.operator LPCTSTR());
				logfile.Writef(_T("Terminate active DB Thread! : ID=[%s],  Win=%d, Fail=%d, AddPMoney=%I64d, RSF=%d, SF=%d, FCARD=%d"),
					Work.UI.ID,  Work.UI.WinNum, Work.UI.LooseNum,   (INT64)Max(Work.UI.PMoney,0)-Work.OI.pBackup, Work.UI.PI.nRSF, Work.UI.PI.nSF, Work.UI.PI.nFCARD);
			}
			// �α� ���
			//CLogFile logfile;
			//logfile.Writef("DB ������ �������� : ID=%s, Marble=%d, Win=%d, Loose=%d, Point=%d",
			//	Work.UI.ID, Work.UI.Marble, Work.UI.WinNum, Work.UI.LooseNum, Work.UI.Point);
		}
	}

	// ���� ó��
	bWorking = FALSE;
	pThread = NULL;

	return TRUE;
}

BOOL CDBThread::CreateUserDB()
{
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
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
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_User->SetLoginTimeout(60);
			pDatabase_User->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CUserDB::m_DSN, (LPCTSTR)CUserDB::m_UID, (LPCTSTR)CUserDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_User->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX ����(UserDB) : %s", e->m_strError.operator LPCTSTR());
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
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
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
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_Game->SetLoginTimeout(60);
			pDatabase_Game->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CGameDB::m_DSN, (LPCTSTR)CGameDB::m_UID, (LPCTSTR)CGameDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_Game->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX ����(GameDB) : %s", e->m_strError.operator LPCTSTR());
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
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
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
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_Status->SetLoginTimeout(60);
			pDatabase_Status->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CStatusDB::m_DSN, (LPCTSTR)CStatusDB::m_UID, (LPCTSTR)CStatusDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_Status->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX ����(StatusDB) : %s", e->m_strError.operator LPCTSTR());
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
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
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
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_Avatar->SetLoginTimeout(60);
			pDatabase_Avatar->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CAvatarDB::m_DSN, (LPCTSTR)CAvatarDB::m_UID, (LPCTSTR)CAvatarDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_Avatar->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX ����(AvatarDB) : %s", e->m_strError.operator LPCTSTR());
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

// [���� ������ �۾�]
BOOL CDBThread::CreateItemDB()
{
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
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
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_Item->SetLoginTimeout(60);
			pDatabase_Item->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CItemDB::m_DSN, (LPCTSTR)CItemDB::m_UID, (LPCTSTR)CItemDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_Item->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX ����(ItemDB) : %s", e->m_strError.operator LPCTSTR());
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

// [�����̾� �۾�]
BOOL CDBThread::CreatePremiumDB()
{
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
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
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_Premium->SetLoginTimeout(60);
			pDatabase_Premium->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CPremiumDB::m_DSN, (LPCTSTR)CPremiumDB::m_UID, (LPCTSTR)CPremiumDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_Premium->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX ����(PremiumDB) : %s", e->m_strError.operator LPCTSTR());
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

// [PC�� �۾�]
BOOL CDBThread::CreatePremIPDB()
{
	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
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
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_PremIP->SetLoginTimeout(60);
			pDatabase_PremIP->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CPremIPDB::m_DSN, (LPCTSTR)CPremIPDB::m_UID, (LPCTSTR)CPremIPDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_PremIP->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX ����(PremIPDB) : %s", e->m_strError.operator LPCTSTR());
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

// ### [�̺�Ʈ Ƽ��] ###
BOOL CDBThread::CreateEventPrizeDB()
{
	if( !g_pMainView->Cfg.bEventDB ) return FALSE;

	// Ŭ������ �Ҵ��� �Ǿ��ִ��� ���µǾ����� �ʴٸ� Ŭ���� ����
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
			// �α��� �� ���� Ÿ�Ӿƿ� ����
			pDatabase_EventPrize->SetLoginTimeout(60);
			pDatabase_EventPrize->SetQueryTimeout(60);

			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CEventPrizeDB::m_DSN, (LPCTSTR)CEventPrizeDB::m_UID, (LPCTSTR)CEventPrizeDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			pDatabase_EventPrize->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}
		CATCH(CDBException, e)
		{
			CLogFile logfile;
			logfile.Writef("Database OpenEX ����(EventPrizeDB) : %s", e->m_strError.operator LPCTSTR());
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
	// ���� ī��Ʈ ��ġ �ʱ�ȭ
	ErrorCnt = 0;

	// DB ���ڵ�� Ŭ���� ����
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
	if(m_pItemDB) {	// [���� ������ �۾�]
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		delete(m_pItemDB);
		m_pItemDB = NULL;
	}
	if(m_pPremiumDB) {	// [�����̾� �۾�]
		if(m_pPremiumDB->IsOpen()) m_pPremiumDB->Close();
		delete(m_pPremiumDB);
		m_pPremiumDB = NULL;
	}
	if(m_pPremIPDB) {	// [PC�� �۾�]
		if(m_pPremIPDB->IsOpen()) m_pPremIPDB->Close();
		delete(m_pPremIPDB);
		m_pPremIPDB = NULL;
	}

	if(g_pMainView->Cfg.bEventDB && m_pEventPrizeDB) {	// ### [�̺�Ʈ Ƽ��] ###
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		delete(m_pEventPrizeDB);
		m_pEventPrizeDB = NULL;
	}


	if(m_pUserDB) {	// ���� ������ �ִ� ���ڵ���� ���� ���߿� ����
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		delete(m_pUserDB);
		m_pUserDB = NULL;
	}	

	// �����ͺ��̽� Ŭ���� ����
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
	if(pDatabase_Item) {	// [���� ������ �۾�]
		if(pDatabase_Item->IsOpen()) pDatabase_Item->Close();
		delete(pDatabase_Item);
		pDatabase_Item = NULL;
	}
	if(pDatabase_Premium) {	// [�����̾� �۾�]
		if(pDatabase_Premium->IsOpen()) pDatabase_Premium->Close();
		delete(pDatabase_Premium);
		pDatabase_Premium = NULL;
	}
	if(pDatabase_PremIP) {	// [PC�� �۾�]
		if(pDatabase_PremIP->IsOpen()) pDatabase_PremIP->Close();
		delete(pDatabase_PremIP);
		pDatabase_PremIP = NULL;
	}
	if(g_pMainView->Cfg.bEventDB && pDatabase_EventPrize) {	// ### [�̺�Ʈ Ƽ��] ###
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

void CDBThread::ProcessDBWork()
{
	// ���� �α��� ��û��
	if(Work.WorkKind == 1)
	{
		Work.bIsWrongPwd=FALSE;		// �н����� Ʋ�� ����
		Work.bAnotherConnect=FALSE;	// �ٸ� ���ӿ� �̹� �α��� ���ΰ�?[SSO �۾�] - ����
		BOOL bRtn = GetUserInfo(&Work);
		
		if(bRtn==FALSE) Work.bIsNotUserID = TRUE;
		::SendMessage(g_pMainView->m_hWnd, UM_DBWORK_LOGIN, NULL, (LONG)&Work);
	}
	// �α� �ƿ�
	if(Work.WorkKind == 4)
	{
		// ### [�̺�Ʈ Ƽ��] ###
		SetUserInfo(&Work);
		// ��� ��� ���Ḧ �˸�
		::SendMessage(g_pMainView->m_hWnd, UM_DBWORK_LOGOUT, NULL, (LONG)&Work);
	}

	// ### [�̺�Ʈ Ƽ��] ###
	if(Work.WorkKind == 5)
	{
		SetEventPrizeDB(Work.UI.ID, &Work.EventPrize);	
		//���ϰ� �޾ƾ� �Ҷ�...
		::SendMessage(g_pMainView->m_hWnd, UM_DBWORK_EVENTSET, NULL, (LONG)&Work);
	}


	// ������ ����Ʈ ����
	if(Work.WorkKind == 9) {
		if(!GetItemList(Work.UI.ID, &Work.Item)) 
			Work.bErrorItemWork = TRUE;
		else
			Work.bErrorItemWork = FALSE;
		// ��� ��� ���Ḧ �˸�
		::SendMessage(g_pMainView->m_hWnd, UM_DBWORK_GAMEITEMLIST, NULL, (LONG)&Work);
	}
	
	// ������ ���
	if(Work.WorkKind == 10) {
		if(!UseGameItem(Work.UI.ID, &Work.UseItem)) 		
			Work.bErrorItemWork = TRUE;
		else 
			Work.bErrorItemWork = FALSE;
		// ��� ��� ���Ḧ �˸�
		::SendMessage(g_pMainView->m_hWnd, UM_DBWORK_USEITEMRESULT, NULL, (LONG)&Work);
	}
	
	// ȯ�漳������ �۾� �Ϸ��� DB Close �ɼ��� ����� ���
	// DB ���ڵ�� �ݱ� �� Ŭ���� ����
	if(g_pMainView->Cfg2.bDBClose == TRUE) DeleteAllDB();
	if(ErrorCnt > 3) {
		CLogFile logfile;
		logfile.Write("DB ����ī��Ʈ ���ġ �ʰ��� DB���ؼ��� Close��");
		// ���� ī��Ʈ ��ġ �ʱ�ȭ
		ErrorCnt = 0;
		// DB ���ڵ�� �ݱ� �� Ŭ���� ����
		DeleteAllDB();
	}
}




///////////////////////////////// DB ó�� �κ� /////////////////////////////////////
// ### [�̺�Ʈ Ƽ��] ###
BOOL CDBThread::SetUserInfo(DBWORKS *pWork)
{
	USERINFO *pUI = &pWork->UI;
	ORGINFO *pOI = &pWork->OI;
	EVENT_PRIZE *pEvent = &pWork->EventPrize;
	int connectsec = pWork->ConnectTime;
	
	//######## [���� ä�� �۾�] #########
	if(!g_pMainView->Cfg3.bExamServer ) { 
		
		
		// ���� ����� ������ ������ DB�� ������� ����
		if( pUI->WinNum == pOI->WinNum && 
			pUI->LooseNum == pOI->LooseNum && 
			pUI->DrawNum  == pOI->DrawNum &&
			pUI->PMoney == pOI->pBackup ) return TRUE;
		
		
		if( !CreateGameDB() ) {
			CLogFile logfile;
			logfile.Writef("SetUserInfo - DB ���ؼ� ���� ����");
			// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
			ErrorCnt = 100;
			return FALSE;
		}
		if( m_pGameDB==NULL ) return FALSE;
		// ���ڵ���� ���������� �ݴ´�
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		
		CString str;
		str.Format("[ID] = '%s'", pUI->ID);
		
			
		/////////// ���� DB���� ���̵� �˻�
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
			// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
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
		
		// ��ȭ�� ����ġ ���
		int winnum = pUI->WinNum - pOI->WinNum;
		int loosenum = pUI->LooseNum - pOI->LooseNum;
		int draw = pUI->DrawNum - pOI->DrawNum;
		
		/////////// GAME DB����
		TRY
		{
			m_pGameDB->Edit();
			
			if(m_pGameDB->IsFieldNull(&m_pGameDB->m_Fold))
				m_pGameDB->m_Fold = 0;
			
			m_pGameDB->m_Vic += winnum;
			m_pGameDB->m_Fail += loosenum;
			m_pGameDB->m_Fold += draw;
			
			INT64 NowPMoney  = (INT64)::_atoi64(m_pGameDB->m_PMoney.operator LPCTSTR()); // ����Ǿ����� �𸣴� ����Ÿ���̽� ��Ŀ�Ӵ�
			INT64 GamePMoney = pUI->PMoney;                // �������� ���ӸӴ�
			if(GamePMoney<0) GamePMoney = 0;               // �������� ���ӸӴϴ� �� �¾ƾ��Ѵ�. �������� 0���� ó���� ���´�!!!
			INT64 ResPMoney  = GamePMoney - pOI->pBackup;  // ��Ŀ�Ӵ� ����ġ ���
			//INT64 ResPMoney  = pUI->PMoney - pUI->pBackup; // ��������� ��Ŀ�Ӵ� ����ġ ���
			NowPMoney+=ResPMoney;                          // ��Ŀ�Ӵ� ������
			
			// ���� 
			if(NowPMoney<0) 
			{
				CLogFile logfile(_T("Abuse.txt"));
				logfile.Writef("ID= %s PMoney = %I64d, OMoney = %I64d, ResPMoney = %I64d, NowPMoney = %I64d", pUI->ID, pUI->PMoney, pOI->pBackup, ResPMoney, NowPMoney);
				
				NowPMoney = 0;                 // ����� ������ ���� ����(<0)ó�� ���� �ʴ´�!!!
			}			
			
			// ���� ����
			m_pGameDB->m_Level   = pUI->nIcon; // ��Ŀ���� ������� �ʴ´� �������� ����ϴ� �б����뵥��Ÿ�̴�
			if(NowPMoney>g_pMainView->Cfg.MAX_PMONEY) NowPMoney = g_pMainView->Cfg.MAX_PMONEY; // [ $$$$ ��Ŀ�Ӵ� �����÷ο� üũ $$$$ ]
			
			CString strPM; strPM.Format(_T("%I64d"),NowPMoney);
			m_pGameDB->m_PMoney = strPM;

			//[��ȣõ��]
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
			m_pGameDB->m_Level2 = pUI->PI.nCard; // ��ǥī��
			
			m_pGameDB->m_RSF    = pUI->PI.nRSF;
			m_pGameDB->m_SF     = pUI->PI.nSF;
			m_pGameDB->m_FCARD  = pUI->PI.nFCARD;
			
			m_pGameDB->m_WinPer = GetWinPro(pUI->WinNum, pUI->LooseNum);//, pUI->DrawNum);
			
			// ���� ����� ���� �ڵ带 �ʱ�ȭ
			int ServerCode = g_pMainView->Cfg.ServerCode;
			if(m_pGameDB->m_IsConnected==ServerCode) m_pGameDB->m_IsConnected = 0;
			
			// �����丮 �۾��� �����Ѵ� ( �����ø� ����Ѵ�)
			if(GamePMoney<=0){
				int nHistoryIndex = (int)m_pGameDB->m_HistoryIndex;
				if(nHistoryIndex<0 || nHistoryIndex>2) nHistoryIndex = 0;
				CString strHistory;
				CTime  time = CTime::GetCurrentTime();
				
				strHistory.Format("%I64d(%d:%d:%d:%d)", // ����ġ�� �����Ѵ� 32������� ���´�!!
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
			// ���� DB ������Ʈ
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
					ErrorCnt++;	// ���� ī��Ʈ ����
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
		
		// ������ DB�� �ݴ´�
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		
	} //######## [���� ä�� �۾�] #########
	

	// ### [�̺�Ʈ Ƽ��] ###]
	SetEventPrizeDB(pUI->ID, pEvent);
	// ������ �� ������Ʈ(���� �˻�� ���� �ʴ´�)
	UpdateConnectStatus();

	return TRUE;
}


// [SSO �۾�] - �߰�
BOOL CDBThread::UpdateConnectStatus() 
{
	int GameCode = g_pMainView->Cfg.GameCode;
	int ServerCode = g_pMainView->Cfg.ServerCode;
	if(GameCode<=0 || ServerCode<=0) 
		return FALSE;

	// ���� �����ڼ��� 0�� �ƴϰ� ���������� ������Ʈ�� �ð��� ������ �ð��� �ʰ����� ���� ��� ����
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
			logfile.Writef("Status DB����: ������ ��Ȳ DB�� ��ġ�ϴ� �����ڵ尡 �����ϴ�");
			if(m_pStatusDB->IsOpen()) m_pStatusDB->Close();
			return FALSE;
		}

		m_pStatusDB->Edit();
		m_pStatusDB->m_UserNum = g_pMainView->RealUser;
		// ������Ʈ �� �� �ִ� ���¶�� ������Ʈ�ϰ� �׷��� ������ ������Ʈ ���
		if(m_pStatusDB->CanUpdate()) m_pStatusDB->Update();
		else m_pStatusDB->CancelUpdate();
	}
	CATCH(CDBException, e)
	{
		// ���������尡 ���ÿ� ���⶧���� ������ ���� �߻��Ͽ� �αױ���� ���� ����
		if(m_pStatusDB->IsOpen()) m_pStatusDB->Close();
		return FALSE;
	}
	END_CATCH

	if(m_pStatusDB->IsOpen()) m_pStatusDB->Close();

	return TRUE;
}


//�ݸ��� ID�� �մµ� DB�� ������ ID�� �ش�Ǵ� ��� �����.
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
		logfile.Writef("GameDB���� - AddNewGameDB Open : %s", e->m_strError.operator LPCTSTR());
		ErrorCnt++;	// ���� ī��Ʈ ����
		return FALSE;
	}
	END_CATCH


	// ���� ID�� �����ϸ� �߰� ���
	if(!m_pGameDB->IsEOF() && m_pGameDB->m_ID.CompareNoCase(pUI->ID) == 0) {
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// DB�� �߰��ϴ� ����
	TRY
	{
		if(!m_pGameDB->IsEOF()) m_pGameDB->MoveLast();
		m_pGameDB->AddNew();

		// DB�߰�
		m_pGameDB->m_ID.Format("%s", pUI->ID);
		//m_pGameDB->m_Name.Format("%s", pUI->Name);
		m_pGameDB->m_Vic     = 0;
		m_pGameDB->m_Fail    = 0;
		m_pGameDB->m_Fold    = 0;
		m_pGameDB->m_Level2  = (BYTE)0; // ��ǥī��
		
		//m_pGameDB->m_Level1  = 0; // ��������
		m_pGameDB->m_Level   = (BYTE)0; // ����

		
		//pUI->PMoney  = M_5000;//M_1000; // [������Ŀ] 1000 ���� ����
		
		// �̺�Ʈ ��
		pUI->PMoney  = M_3Y;//M_1000; // [����Ŀ] 3�� ���� //�ű����� �Ӵ� ����
		
#ifdef _DEBUG
		//!!! �����Ҳ�
//		pUI->PMoney  = M_10Y;
#endif
		CString strPM; strPM.Format(_T("%I64d"),pUI->PMoney);
		m_pGameDB->m_PMoney = strPM;

		m_pGameDB->m_LMoney.Format(""); // [��ȣõ��] 2004.07.08

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

		// �����丮 �۾�
		m_pGameDB->m_History1.Format("");
		m_pGameDB->m_History2.Format("");
		m_pGameDB->m_History3.Format("");
		m_pGameDB->m_HistoryIndex = (BYTE)0;


		m_pGameDB->Update();
		
	}
	CATCH(CDBException, e)
	{
	//	CLogFile logfile;
	//	logfile.Writef("GameDB���� - AddNewGameDB Update (ID=%s) : %s", pUI->ID, e->m_strError.operator LPCTSTR());
		CString strFileName = GetLogFileName();
		if(strFileName!=_T("")){
			CLogFile logfile(strFileName.operator LPCTSTR());
			logfile.Writef(_T("GameDB error - AddNewGameDB Update (ID=%s) : %s"), pUI->ID, e->m_strError.operator LPCTSTR());
		}
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt++;	// ���� ī��Ʈ ����
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

// �α������ۼ� Ÿ��Ʋ�� �����Ѵ�
CString CDBThread::GetLogFileName()
{
	CString strFileName(_T(""));

	CTime  time = CTime::GetCurrentTime();

	strFileName.Format(g_StrMan.Get(_T("SPOKER_DBERR")),time.GetMonth(),time.GetDay());

	return strFileName;
}


// [SSO �۾�] - �Լ��� ���κ��� �ٲ���� (�����Ͽ� ������ ������Ʈ�� �°� �����Ͽ� ����� ��)
BOOL CDBThread::GetUserInfo(DBWORKS *pWork) 
{
	// ���� ���� ������ �о�´�

		// DB���� �о�´�
	if(!GetMainUserInfo_FromDB(pWork)) 
		return FALSE;


	// ���� ������ �о�´�
	if(!GetGameInfo(pWork)) 
		return FALSE;

	// [�����̾� �۾�]
	// �����̾� ������ �о�´�
	if(!GetPremiumInfo(pWork->UniqNo, &pWork->UI))
		return FALSE;
	
	// [���� ������ �۾�]
	// ���� �������� �о�´�
	if(!GetItemList(pWork->UI.ID, &pWork->Item)) 
		return FALSE;

	// [PC�� �۾�]
	// �����̾� PC�� IP������ �о�´�
	// �����ϴ��� ���� ó���Ѵ�
	GetPremiumIP(pWork);

	// ### [�̺�Ʈ Ƽ��] ###
	// �����ϴ���� ���� ó���Ѵ�.
	GetEventPrize(pWork);
	

	return TRUE;
}

// [SSO �۾�] - �߰�!! (�����Ͽ� ������ ������Ʈ�� �°� �����Ͽ� ����� ��)
BOOL CDBThread::GetMainUserInfo_FromDB(DBWORKS *pWork)
{	
	USERINFO *pUI = &pWork->UI;

	if(!CreateUserDB()  || !CreateAvatarDB()) {
		CLogFile logfile;
		logfile.Writef("GetUserInfo - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pUserDB==NULL || m_pAvatarDB==NULL) return FALSE;
	if(pUI == NULL) return FALSE;

	CString str;
	str.Format("[ID] = '%s'", pUI->ID);

	TRY
	{
		// ���ڵ���� ���������� �ݴ´�
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();

		m_pUserDB->m_strFilter = str;
		// ��������DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
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
		logfile.Writef("UserDB���� - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// NULL �ʵ� �ʱ�ȭ		[�����̾� �۾�]
	if(m_pUserDB->IsFieldNull(&m_pUserDB->m_UniqNO))
		m_pUserDB->m_UniqNO = "";

	// ��й�ȣ �˻�(DB�� �����ϴ� ���� ������ �˻�)
	if(m_pUserDB->m_Passwd.CompareNoCase(pWork->Passwd)==0) {
		pWork->bIsWrongPwd = FALSE;
	}
	else {
		pWork->bIsWrongPwd = TRUE;
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		return FALSE;
	}


	// ���� ���� Ȥ�� ������ ���̵����� �˻�
	pWork->IDState = m_pUserDB->m_IDState;								// ���̵� ����
	strncpy(pUI->ID, m_pUserDB->m_ID.operator LPCTSTR(), 15);			// ����� ID
	strncpy(pWork->Passwd, m_pUserDB->m_Passwd.operator LPCTSTR(), 15);	// ����� �н�����
	pUI->Sex = (char)m_pUserDB->m_Sex;
	strncpy(pUI->Desc, m_pUserDB->m_Desc.operator LPCTSTR(), 60);

	// [���� ������ �۾�]
	strncpy(pWork->RegID, (LPCTSTR)m_pUserDB->m_RegID, 15);	// �ֹε�� ��ȣ
	strncpy(pWork->UniqNo, (LPCTSTR)m_pUserDB->m_UniqNO, 15);	// ����ũ �ѹ�


	// [��������]
	if(m_pUserDB->m_Certificate == NULL)
		pWork->bCertificate = FALSE;
	else
		pWork->bCertificate = m_pUserDB->m_Certificate;
	

	
	// �ƹ�Ÿ DB �б�
	TRY
	{
		m_pAvatarDB->m_strFilter = str;
		// ��������DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
		m_pAvatarDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pAvatarDB->IsEOF() || m_pAvatarDB->m_ID.CompareNoCase(pUI->ID)!=0) {
			// �ƹ�Ÿ DB�� �������� ������ �⺻�� ����(���� ��ȣ�� �Է�)
			if(pUI->Sex == 1) pUI->AvatarInfo[5] = 1;
			else pUI->AvatarInfo[5] = 2;
		}
		else {
			// �ƹ�Ÿ ���� ����
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
		logfile.Writef("AvatarDB���� - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
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

// [SSO �۾�] - �߰�!! (�����Ͽ� ������ ������Ʈ�� �°� �����Ͽ� ����� ��)
BOOL CDBThread::GetGameInfo(DBWORKS *pWork) 
{
	USERINFO *pUI = &pWork->UI;

	if(!CreateGameDB()) {
		CLogFile logfile;
		logfile.Writef("GetUserInfo - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pGameDB==NULL) return FALSE;
	
	// ���ڵ���� ���������� �ݴ´�
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	CString str;
	str.Format("[ID] = '%s'", pUI->ID);

		// ���� DB �б�
	TRY
	{
		m_pGameDB->m_strFilter = str;
		m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("GameDB���� - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH
	

	// �ݸ��� ȸ��ID�� ������ ���� DB�� ��ϵǾ� ���� ����
	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		if(AddNewGameDB(pUI) == FALSE) {
			CLogFile logfile;
			logfile.Writef("GameDB���� - GetUserInfo (ID=%s) : Game DB ���̵� �߰� ����", pUI->ID);
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
			logfile.Writef("GameDB���� - GetUserInfo Requery (ID=%s) : %s", pUI->ID, e->m_strError.operator LPCTSTR());
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			ErrorCnt++;	// ���� ī��Ʈ ����
			return FALSE;
		}
		END_CATCH
	}

	// ���� DB�߰� ������ ���
	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		CLogFile logfile;
		logfile.Writef("GameDB���� (ID=%s) : Game DB ���̵� �߰� ����", pUI->ID);
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// ���� DB�� �о�� (KHS_FAST_CHAN)
	// ##############  [������Ŀ] ����ó��
	INT64 nPMoney = (INT64)::_atoi64(m_pGameDB->m_PMoney.operator LPCTSTR());
/*
	if(nPMoney<0){ // ###### ��Ŀ�Ӵ� ������
		CLogFile logfile(_T("Abuse.txt"));
		logfile.Writef(g_StrMan._Get(_T("SPOKER_INVAL_PM")), id, nPMoney);
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();

		if(pNegPMoney!=NULL) *pNegPMoney = TRUE;

		return FALSE;
	}
*/
	// �������� ������ ��Ŀ�Ӵϸ� �ҷ��ͼ�...���߿� ����Ÿ���̽��� �� ��Ŀ�Ӵ��� ����ġ�� ����ϸ� �ȴ�!!
	if(nPMoney>g_pMainView->Cfg.MAX_PMONEY) nPMoney = g_pMainView->Cfg.MAX_PMONEY; // [ $$$$ ��Ŀ�Ӵ� �����÷ο� üũ $$$$ ]

#ifdef _DEBUG
	if(nPMoney <=0)
		pUI->PMoney  = 150000000;
	else 
		pUI->PMoney  = nPMoney;
#else
	pUI->PMoney  = nPMoney;
#endif	

	// �������� ���
	pUI->nIcon = GetPokerLevel(nPMoney);

	nPMoney = (INT64)::_atoi64(m_pGameDB->m_LMoney.operator LPCTSTR());  // [��ȣõ��] 2004.07.08 // �ν�Ʈ �Ӵ�
	pWork->LostMoney = nPMoney;

	pUI->WinNum    = m_pGameDB->m_Vic;
	pUI->LooseNum  = m_pGameDB->m_Fail;
	pUI->DrawNum   = m_pGameDB->m_Fold;

	// ī�� �ε��� ???
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
	pUI->PI.nCard    = (char)m_pGameDB->m_Level2; // ��ǥī��
	pUI->PI.nRSF     = m_pGameDB->m_RSF;
	pUI->PI.nSF      = m_pGameDB->m_SF;
	pUI->PI.nFCARD   = m_pGameDB->m_FCARD;

	// ����� DB ���̺��� ����
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;

}



// Ư�� ���̵��� ���� �������� �о�´�
BOOL CDBThread::GetItemList(char *id, GAMEITEM_LIST *pItemList)
{ 
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// ������ DB ���� //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	if(!CreateItemDB()) {
		CLogFile logfile("ItemDB_Log.txt");
		logfile.Writef("GetItemList - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pItemDB == NULL) return FALSE;
	if(pItemList == NULL) return FALSE;

	// ���ڵ���� ���������� �ݴ´�
	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	CString strItem; // �����ڵ�� ���̵� �д´�
	/*
	// ���ӿ� �ʿ��� �����۸� �����Ѵ�
	strItem.Format(_T("[id]='%s' AND \
		( ([c_gametype]=%d AND [c_code]>=%d AND [c_code]<=%d) OR \
		([c_code]>=%d AND [c_code]<=%d) OR ([c_code]>=%d AND [c_code]<=%d) OR \
		([c_gametype]=%d AND [c_code]>=%d AND [c_code]<=%d) OR \
		([c_code]=%d) )"), // [õ�Ϲ��� ������]
		id, 
		g_pMainView->Cfg.GameCode, ITEM_SCODE_JUMP, ITEM_ECODE_JUMP,	// ���� ������ 
		ITEM_SCODE_SUPERMASTER, ITEM_ECODE_SUPERMASTER,					// ���� ���� ������
		ITEM_SCODE_ALLINSUPERMASTER, ITEM_ECODE_ALLINSUPERMASTER,	    // �����ѹ� [���� ����] (����)
		7, ITEM_SCODE_SECRET, ITEM_ECODE_SECRET,						// ����� ������ - ������Ŀ1, 2 ���� �����ڵ�� 7������ �д´�.
		ITEM_SCODE_INVINCIBLE );										// [õ�Ϲ��� ������]	
	
	*/

	// ���ӿ� �ʿ��� �����۸� �����Ѵ�
	strItem.Format(_T("[id]='%s' AND \
		( ([c_code]>=%d AND [c_code]<=%d) OR \
		([c_code]>=%d AND [c_code]<=%d) OR \
		([c_code]>=%d AND [c_code]<=%d) OR \
		([c_gametype]=%d AND [c_code]>=%d AND [c_code]<=%d) OR \
		([c_code]=%d) )"), // [õ�Ϲ��� ������]
		id, 
		ITEM_SCODE_JUMP, ITEM_ECODE_JUMP,								// ���� ������ 
		ITEM_SCODE_SUPERMASTER, ITEM_ECODE_SUPERMASTER,					// ���� ���� ������
		ITEM_SCODE_SAFEGUARDANGEL, ITEM_ECODE_SAFEGUARDANGEL,			// [��ȣõ��] 2004.07.08
		7, ITEM_SCODE_SECRET, ITEM_ECODE_SECRET,						// ����� ������ - ������Ŀ1, 2 ���� �����ڵ�� 7������ �д´�.
		ITEM_SCODE_INVINCIBLE );										// [õ�Ϲ��� ������]											// [õ�Ϲ��� ������]		


	// ������ DB �б�
	TRY
	{
		m_pItemDB->m_strFilter = strItem;
		// ������DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
		m_pItemDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pItemDB->IsEOF()) {
			pItemList->nResult = GAMEITEM_MES_NOITEM; // �������� �����ϰ� �����ʴ�
			if(m_pItemDB->IsOpen()) m_pItemDB->Close();
			return TRUE;
		}

		if(!m_pItemDB->IsBOF()) m_pItemDB->MoveFirst();
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("ItemDB_Log.txt");
		logfile.Writef(_T("ItemDB error - GetItemList Open (ID=%s): %s"), id, (LPCTSTR)(e->m_strError));

		pItemList->nResult = GAMEITEM_MES_DBERROR; // ������ DB�� �дµ� �����Ͽ���
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// ���������� �о����
	int nItemNum = 0;
	TRY
	{
		for(;;)
		{
			if(m_pItemDB->IsEOF() || nItemNum >= MAX_ITEM_NUM) break; // �о�� ����Ʈ ��� �аų�, ���� �ʰ��Ǿ��ٸ� �����Ѵ�

			// ������ ����� Ȯ��
			if(m_pItemDB->m_ID.CompareNoCase(id)==0) {

				GAMEITEM item;
				memset(&item, 0, sizeof(GAMEITEM));

				item.Index  = nItemNum;										// �ε��� �ɱ�
				item.NO     = (int)m_pItemDB->m_NO;							// ����ũ�ѹ�
				item.Code   = (int)m_pItemDB->m_Code;						// ������ �ڵ�
				strncpy(item.PresID, (LPCTSTR)(m_pItemDB->m_PresID), 15);	// ������ ���̵�
				item.UseDay = (short)m_pItemDB->m_UseDay;					// ��ȿ �Ⱓ
				m_pItemDB->m_StartDate.GetAsSystemTime(item.StartDate);		// ��ŸƮ�ð�
				item.Use = (short)m_pItemDB->m_Use;							// ��� ����(Ȥ�� ��� ȸ��)

				// [��ȣõ��] 2004.07.08
				if( m_pItemDB->IsFieldNull(&m_pItemDB->m_UseNum) ) 	item.UseNum = 0;
				else item.UseNum = (short)m_pItemDB->m_UseNum;
			
				if(item.Use > 0) { 
					// ���Ǿ��� �������̶�� ������ ��¥�� �ִ�
					COleDateTime st = m_pItemDB->m_StartDate; // ��ŸƮ�ð�
					if(st.GetStatus() != COleDateTime::valid){
						// �ð� �ʵ尡 ��ȿ���� ����
						m_pItemDB->MoveNext();
						continue;
					}
				}

				// �ùٸ� ����Ÿ�� �ƴϴ�
				if(item.UseDay < 0 || item.UseNum < 0) {
					m_pItemDB->MoveNext();
					continue;					
				}

				// �������� ���� ������ ��´�
				ItemState state = GameItem_GetItemState(item);

				if(state.iState == 0 || state.iState == 3) {
					// �߸��� �������̰ų� ����� ����� �������̴�
					m_pItemDB->MoveNext();
					continue;
				}

				//if(state.bNowUsing) {
				//	// ���� �������� �ְ�, ��ȿ�Ѱ� 1���� �ִٸ� ���尡���ϴ�!!!
				//	pItemList->bLogable = TRUE;
				//}

				// ������ ������ ����
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

		pItemList->nResult = GAMEITEM_MES_DBERROR; // �о�� ������ DB�� �̵��ϴµ� �����Ͽ���
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		ErrorCnt++;
		return FALSE;
	}
	END_CATCH	

	// ������ ���� ����
	pItemList->nNum = nItemNum;

	// ������ �������� �Ѱ��� ���ٸ�...
	if(nItemNum <= 0){
		pItemList->nResult = GAMEITEM_MES_NOITEM; // �������� �����ϰ� �����ʴ�
	}

	// ��ȿ�ϰ� ������� �������� �ִ�
	if(pItemList->bLogable){
		pItemList->nResult = GAMEITEM_MES_SUCCESS; // �α��� �����ϴ�
	}
	// �ѹ��� �Ⱦ� �������� �Ѱ��̻� �����Ѵ�
	else{
		pItemList->nResult = GAMEITEM_MES_USEABLE; // �����ۻ�� ����
	}
	

	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	return TRUE;
}


// Ư�� ���̵��� ���� �������� ����Ѵ�
BOOL CDBThread::UseGameItem(char *id, GAMEITEM *pItem)
{ 
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// ������ DB ���� //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	if(!CreateItemDB()) {
		CLogFile logfile("ItemDB_Log.txt");
		logfile.Writef("GetItemList - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pItemDB == NULL) return FALSE;
	if(pItem == NULL) return FALSE;

	// ���ڵ���� ���������� �ݴ´�
	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	CString strItem;
	
	// ������ ����
	strItem.Format(_T("[c_no]=%d"), pItem->NO);

	// ������ DB �б�
	TRY
	{
		m_pItemDB->m_strFilter = strItem;
		m_pItemDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
		// �������� �������� �ʴ� ��� ����
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
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// ���̵� ��ġ���� �ʴ� ��� ����
	if(m_pItemDB->m_ID.CompareNoCase(id) != 0) {
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		return FALSE;
	}
	// ������ �ڵ尡 ��ġ���� �ʴ� ��� ����
	if(pItem->Code != (short)m_pItemDB->m_Code) {
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		return FALSE;
	}

	TRY
	{
		m_pItemDB->Edit();

		// ��¥ �ʵ尡 NULL�̸� ���� �Ͻ� ���
		if(m_pItemDB->IsFieldNull(&m_pItemDB->m_StartDate)) {
			m_pItemDB->m_StartDate = COleDateTime::GetCurrentTime();			
		}

		if(m_pItemDB->IsFieldNull(&m_pItemDB->m_Use)) 
			m_pItemDB->m_Use = 0;

		m_pItemDB->m_Use += 1;
		// ����� ���� �ڵ带 ���
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

	// �������� ���¸� ���
	m_pItemDB->m_StartDate.GetAsSystemTime(pItem->StartDate);		// ��ŸƮ�ð�
	pItem->Use = (short)m_pItemDB->m_Use;							// ��� ����(Ȥ�� ��� ȸ��)

	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	return TRUE;
}


// [�����̾� �۾�]
// �����̾� ������ �о�´�
BOOL CDBThread::GetPremiumInfo(LPCTSTR uniqno, USERINFO* pUI)
{ 
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// ������ DB ���� //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	// �⺻�� ����
	pUI->PremCode = 0;
	memset(&pUI->PremDate, 0, sizeof(pUI->PremDate));

	// ����ũ �ѹ��� �������� �ʴ� ���� ���� ��Ȳ�̹Ƿ� �Ϲ� ȸ������ ����
	if(strlen(uniqno) == 0) 
		return TRUE;

	// ���̺� ����
	if(!CreatePremiumDB()) {
		CLogFile logfile("PremiumDB_Log.txt");
		logfile.Writef("GetPremiumList - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pPremiumDB == NULL) return FALSE;

	// ���ڵ���� ���������� �ݴ´�
	if(m_pPremiumDB->IsOpen()) m_pPremiumDB->Close();

	CString strPremium; // �����ڵ�� ���̵� �д´�
	
	// ����ũ �ѹ��� �����̾� ������ �����Ѵ�
	strPremium.Format(_T("[NO]='%s'"), uniqno);

	// �����̾� DB �б�
	TRY
	{
		m_pPremiumDB->m_strFilter = strPremium;
		// DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
		m_pPremiumDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("PremiumDB_Log.txt");
		logfile.Writef(_T("PremiumDB error - GetPremiumList Open (UniqNO=%s): %s"), uniqno, (LPCTSTR)(e->m_strError));
		if(m_pPremiumDB->IsOpen()) m_pPremiumDB->Close();
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// NULL �ʵ� �ʱ�ȭ
	if(m_pPremiumDB->IsFieldNull(&m_pPremiumDB->m_UniqNO)) {
		m_pPremiumDB->m_UniqNO = "";
	}

	if(m_pPremiumDB->IsEOF() || 
		stricmp(m_pPremiumDB->m_UniqNO, uniqno)!=0 ||
		m_pPremiumDB->m_UniqNO.GetLength()==0 ) 
	{
		// �����̾� ���ڵ尡 �������� �ʴ´ٸ� �Ϲ� ȸ���̴�
		if(m_pPremiumDB->IsOpen()) m_pPremiumDB->Close();		// [PC�� �۾�] - �����̾� �۾����� ������ ��
		return TRUE;
	}

	// ���� ����
	pUI->PremCode = (char)m_pPremiumDB->m_PremCode;				// �����̾� �ڵ�
	m_pPremiumDB->m_PremDate.GetAsSystemTime(pUI->PremDate);	// �����̾� ���� �Ͻ�

	//2004.05.07 �������Ѿ� ����
	pUI->PremLeadersCode = (char)m_pPremiumDB->m_LeadersCode;   // 1 ������ 4 ������ �÷���

	// ���̺� �ݱ�
	if(m_pPremiumDB->IsOpen()) m_pPremiumDB->Close();

	return TRUE;
}

// �����̾� IP ���� �б�			[PC�� �۾�]
BOOL CDBThread::GetPremiumIP(DBWORKS *pWork)
{
	USERINFO *pUI = &pWork->UI;
	// �⺻�� ����
	pUI->bPremIP = FALSE;

	// ���̺� ����
	if(!CreatePremIPDB()) {
		CLogFile logfile("PremIPDB_Log.txt");
		logfile.Writef("GetPremiumIP - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pPremIPDB == NULL) return FALSE;

	// ���ڵ���� ���������� �ݴ´�
	if(m_pPremIPDB->IsOpen()) m_pPremIPDB->Close();

	CString strQuery;
	
	// ������� IP��ȣ�� �����̾� ������ �����Ѵ�
	//strQuery.Format(_T("[ip]='%s'"), pWork->UserIP);

	// ������� IP��ȣ�� �����̾� ������ �����Ѵ�(�÷��װ� 0�� ���ڵ常 ����)
	strQuery.Format(_T("[ip]='%s' AND [flag]=%d"), pWork->UserIP, 0);

	// �����̾� DB �б�
	TRY
	{
		m_pPremIPDB->m_strFilter = strQuery;
		// DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
		m_pPremIPDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("PremIPDB_Log.txt");
		logfile.Writef(_T("PremIPDB error - GetPremiumIP Open (ID=%s:IP=%s): %s"), pUI->ID, pWork->UserIP, (LPCTSTR)(e->m_strError));
		if(m_pPremIPDB->IsOpen()) m_pPremIPDB->Close();
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// NULL �ʵ� �ʱ�ȭ
	//if(m_pPremIPDB->IsFieldNull(&m_pPremIPDB->m_UniqNO)) {
	//	m_pPremIPDB->m_UniqNO = "";
	//}

	if(m_pPremIPDB->IsEOF() || 
		stricmp(m_pPremIPDB->m_IP, pWork->UserIP)!=0 ||
		m_pPremIPDB->m_IP.GetLength()==0 ) 
	{
		// IP������ �������� �ʴ´ٸ� �Ϲ� ȸ���̴�
		if(m_pPremIPDB->IsOpen()) m_pPremIPDB->Close();
		return TRUE;
	}

	// �÷��װ� 0�� ��츸 �ùٸ� PC�� IP�̴�
	if(m_pPremIPDB->m_Flag == 0) {
		pUI->bPremIP = TRUE;
	}

	// ���̺� �ݱ�
	if(m_pPremIPDB->IsOpen()) m_pPremIPDB->Close();

	return TRUE;
}


// ���ڵ� �߰� ### [�̺�Ʈ Ƽ��] ### 
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
		logfile.Writef("EventPrizeDB���� - AddNewEventPrizeDB Open : %s", e->m_strError.operator LPCTSTR());
		ErrorCnt++;	// ���� ī��Ʈ ����
		return FALSE;
	}
	END_CATCH


	// ���� ID�� �����ϸ� �߰� ���
	if(!m_pEventPrizeDB->IsEOF() && m_pEventPrizeDB->m_ID.CompareNoCase(pUI->ID) == 0) {
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		return FALSE;
	}

	// DB�� �߰��ϴ� ����
	TRY
	{
		if(!m_pEventPrizeDB->IsEOF()) m_pEventPrizeDB->MoveLast();
		m_pEventPrizeDB->AddNew();

		// DB�߰�
		m_pEventPrizeDB->m_ID.Format("%s", pUI->ID);
		m_pEventPrizeDB->m_EventCoin = 0;
		m_pEventPrizeDB->m_GameCount = 0;
		m_pEventPrizeDB->m_GameCode = g_pMainView->Cfg.GameCode; //�����ڵ�
		m_pEventPrizeDB->m_LastPlayDate = COleDateTime::GetCurrentTime();

		m_pEventPrizeDB->Update();
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("EventPrizeDB���� - AddNewEventPrizeDB Update (ID=%s) : %s", pUI->ID, e->m_strError.operator LPCTSTR());
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		ErrorCnt++;	// ���� ī��Ʈ ����
		return FALSE;
	}
	END_CATCH

	return TRUE;

}


// ### [�̺�Ʈ Ƽ��] ###
BOOL CDBThread::GetEventPrize(DBWORKS *pWork)
{
	if( !g_pMainView->Cfg.bEventDB  ) return FALSE;
	// ���̺� ����
	if(!CreateEventPrizeDB()) {
		CLogFile logfile("EventPrize_Log.txt");
		logfile.Writef("GetEventPrize - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pEventPrizeDB == NULL) return FALSE;

	// ���ڵ���� ���������� �ݴ´�
	if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();

	USERINFO *pUI = &pWork->UI;

	CString strQuery;
	// ������� ID, �����ڵ�� �����Ѵ�
	strQuery.Format(_T("[ID]='%s' AND [GameCode]= %d"), pWork->UI.ID, g_pMainView->Cfg.GameCode);

	// �̺�Ʈ  DB �б�
	TRY
	{
		m_pEventPrizeDB->m_strFilter = strQuery;
		// DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
		m_pEventPrizeDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile("EventPrize_Log.txt");
		logfile.Writef(_T("GetEventPrize error - GetEventPrize Open (ID=%s): %s"), pWork->UI.ID, (LPCTSTR)(e->m_strError));
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// �ݸ��� ȸ��ID�� ������ �̺�Ʈ  DB�� ��ϵǾ� ���� ����
	if(m_pEventPrizeDB->IsEOF() || m_pEventPrizeDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		if(AddNewEventPrizeDB(pUI) == FALSE) {
			CLogFile logfile;
			logfile.Writef("EventPrizeDB���� - GetEventPrize (ID=%s) : Event DB ���̵� �߰� ����", pUI->ID);
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
			logfile.Writef("EventPrizeDB���� - GetEventPrize Requery (ID=%s) : %s", pUI->ID, e->m_strError.operator LPCTSTR());
			if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
			ErrorCnt++;	// ���� ī��Ʈ ����
			return FALSE;
		}
		END_CATCH
	}

	// ���� DB�߰� ������ ���
	if(m_pEventPrizeDB->IsEOF() || m_pEventPrizeDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		CLogFile logfile;
		logfile.Writef("EventPrizeDB ���� - GetEventPrize (ID=%s) : Event DB ���̵� �߰� ����", pUI->ID);
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		return FALSE;
	}

	// ȹ���̺�Ʈ, 	�����Ǽ�, ���������Ͻ�
	pWork->EventPrize.EventPrize = m_pEventPrizeDB->m_EventCoin;
	pWork->EventPrize.GameCnt  	 = m_pEventPrizeDB->m_GameCount;
	pWork->EventPrize.OrigEventPrize = pWork->EventPrize.EventPrize;//�����
	
	COleDateTime dbst = m_pEventPrizeDB->m_LastPlayDate; // ���������Ͻ�
	if(dbst.GetStatus() != COleDateTime::valid){// �ð� �ʵ尡 ��ȿ���� ����
		dbst = COleDateTime::GetCurrentTime();
		m_pEventPrizeDB->m_LastPlayDate = dbst; // ���� �ð� ��
	}
	
	CTime nt = CTime::GetCurrentTime();  //�ð���� Ȯ��
	if(dbst.GetDay() != nt.GetDay()) {
		pWork->EventPrize.GameCnt = 0;
	}

	if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
	return TRUE;
}	

// ### [�̺�Ʈ Ƽ��] ###
BOOL CDBThread::SetEventPrizeDB(char *id, EVENT_PRIZE *pEvent, BOOL bInit)
{
	if( !g_pMainView->Cfg.bEventDB || !g_pMainView->EventCfg.bRunEvent ) return FALSE;

	if(id==NULL) return FALSE;
	if(pEvent==NULL) return FALSE;

	if(!CreateEventPrizeDB()) {
		CLogFile logfile("EventPrize_Log.txt");
		logfile.Writef("SetEventDB - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pEventPrizeDB == NULL) return FALSE;
	if(pEvent == NULL) return FALSE;

	// ���ڵ���� ���������� �ݴ´�
	if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();

	CString strQuery;
	// ������� ID, �����ڵ�� �����Ѵ�
	strQuery.Format(_T("[ID]='%s' AND [GameCode]= %d"), id, g_pMainView->Cfg.GameCode);

	// ������ DB �б�
	TRY
	{
		m_pEventPrizeDB->m_strFilter = strQuery;
		m_pEventPrizeDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
		// �������� �������� �ʴ� ��� ����
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
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// ���̵� ��ġ���� �ʴ� ��� ����
	if(m_pEventPrizeDB->m_ID.CompareNoCase(id) != 0) {
		if(m_pEventPrizeDB->IsOpen()) m_pEventPrizeDB->Close();
		return FALSE;
	}


	TRY
	{
		m_pEventPrizeDB->Edit();

		// ����ð�, �����Ǽ�, ȹ���̺�Ʈ ī��Ʈ�� ��
		m_pEventPrizeDB->m_LastPlayDate = COleDateTime::GetCurrentTime(); // ���� ���ӽð�
		m_pEventPrizeDB->m_GameCount = pEvent->GameCnt;
		int Event =  pEvent->EventPrize - pEvent->OrigEventPrize;
		m_pEventPrizeDB->m_EventCoin += Event; // ��������

		// �ǽð� ���
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
// [���� ������ �۾�] [���� ������ �۾�] ���� �������� �д´�(������ ���� �̶��) 
BOOL CDBThread::GetUserInfo2(char *id, char *pass, 
							USERINFO *pUI, BOOL *pIsAnotherConnect, BOOL *pIsWrongPwd, 
							int *pIDState, int *pSrvCode, BOOL *pNegPMoney, GAMEITEM_LIST *pItem) 
{
	if(!CreateUserDB() || !CreateGameDB() || !CreateAvatarDB() || !CreateItemDB()) {
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pUserDB==NULL || m_pGameDB==NULL || m_pAvatarDB==NULL || m_pItemDB==NULL) return FALSE;
	if(pUI == NULL || pItem == NULL) return FALSE;

	pItem->bLogable = FALSE;                 // �α��� �����Ѱ�?
	pItem->nResult  = GAMEITEM_MES_ERROR;    // �α��� �Ұ��ڵ�


	CString str;
	str.Format(_T("[ID] = '%s'"), id);

	////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// ���� DB ���� ///////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	TRY
	{
		// ���ڵ���� ���������� �ݴ´�
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();

		m_pUserDB->m_strFilter = str;
		// ��������DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
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
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// ��й�ȣ �˻�
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

	// ���� ���� Ȥ�� ������ ���̵����� �˻�
	if(pIDState != NULL) *pIDState = m_pUserDB->m_IDState;

	strncpy(pUI->ID, m_pUserDB->m_ID.operator LPCTSTR(), 15);
	strncpy(pUI->Pass, m_pUserDB->m_Passwd.operator LPCTSTR(), 8);
	pUI->Sex = (char)m_pUserDB->m_Sex;
	strncpy(pUI->Desc, m_pUserDB->m_Desc.operator LPCTSTR(), 60);

	// ���⼭ ���̻����Ѵ�
	//CString strAge = m_pUserDB->m_RegID;
	//if(strAge!=_T("")){
	//	pUI->nAge = GetAge(strAge);
	//}



	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// ������ DB ���� //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	CString strItem; // �����ڵ�� ���̵� �д´�
		
	if(g_pMainView->Cfg.bItemServer)
	{
		strItem.Format(_T("[c_gametype] = %d AND [c_code]>0 AND [c_code]<100 AND [id] = '%s'"), 
		g_pMainView->Cfg.GameCode, pUI->ID); // ����� �������� c_code�� 1 ~ 100 ���̷� �Ѵ�
	}
	else 
	{
		strItem.Format(_T("[c_gametype] = %d AND [c_code]>=200 AND [c_code]<300 AND [id] = '%s'"), 
		g_pMainView->Cfg.GameCode, pUI->ID); // [���� ������ �۾�] �� c_code�� 200 ~ 299 ���̷� �Ѵ�

	}

	// ������ DB �б�
	TRY
	{
		m_pItemDB->m_strFilter = strItem;
		// ������DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
		m_pItemDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pItemDB->IsEOF() || m_pItemDB->m_ID.CompareNoCase(id)!=0) {
			
			pItem->nResult = GAMEITEM_MES_NOITEM; // �������� �����ϰ� �����ʴ�
	
			if(g_pMainView->Cfg.bItemServer)
			{
				if(m_pUserDB->IsOpen()) m_pUserDB->Close(); //[ ������� �ʿ� ] 
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
		pItem->nResult = GAMEITEM_MES_DBERROR; // ������ DB�� �дµ� �����Ͽ���
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	COleDateTime nt = COleDateTime::GetCurrentTime();

	// ���������� �о����
	int nItemNum = 0;
	TRY
	{
		//if(!m_pItemDB->IsBOF()) m_pItemDB->MoveFirst();
		for(;;)
		{
			if(m_pItemDB->IsEOF() || nItemNum>= MAX_ITEM_NUM) break; // 30�� �о�� ����Ʈ ��� �аų�, ���� �ʰ��Ǿ��ٸ� �����Ѵ�

			// ������ ����� Ȯ��
			if(m_pItemDB->m_ID.CompareNoCase(id)==0) {
				short Day  = (short)m_pItemDB->m_Day; // ��ȿ�Ⱓ (����¥�� 1, 30,...)
				// �ùٸ� ����Ÿ�� �ƴϴ�
				if(Day<=0){
					m_pItemDB->MoveNext();
					continue;					
				}
				// ��ȿ�� �Ⱓ�� �����Ѵ�-�д����� �����Ѵ�
				double period = (double)(Day*60*24);
				int   Use    = m_pItemDB->m_Use; // �������
				SYSTEMTIME sysTime; memset(&sysTime,0,sizeof(SYSTEMTIME));
				if(Use>0){ // ���Ǿ��� �������̶�� ������ ��¥�� �ִ�
					COleDateTime st = m_pItemDB->m_Date; // ��ŸƮ�ð�
					if(st.GetStatus()!=COleDateTime::valid){
						m_pItemDB->MoveNext();
						continue;
					}
					st.GetAsSystemTime(sysTime);
					COleDateTimeSpan TimeSpan = nt - st;     // ����ð����� ���̸� ���Ѵ�
					if(TimeSpan.GetTotalMinutes()>period){   // �پ� �������̴�
						m_pItemDB->MoveNext();
						continue;					
					}
					// ���� �������� �ְ�, ��ȿ�Ѱ� 1���� �ִٸ� ���尡���ϴ�!!!
					pItem->bLogable = TRUE;
				}
				pItem->Item[nItemNum].Index  = nItemNum; // �ε��� �ɱ�
				pItem->Item[nItemNum].Day    = Day;      // ��ȿ�Ⱓ
				pItem->Item[nItemNum].Use    = (short)Use;      // �������
				memcpy(&(pItem->Item[nItemNum].StartDate),&sysTime,sizeof(SYSTEMTIME)); // ���� �ð�

				pItem->Item[nItemNum].NO     = (int)m_pItemDB->m_NO;    // ����ũ�ѹ�
				pItem->Item[nItemNum].Code   = (int)m_pItemDB->m_Code;  // ������ �ڵ�
				strncpy(pItem->Item[nItemNum].PresID,m_pItemDB->m_PresID.operator LPCTSTR(),15); // ������ ���̵�

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

		pItem->nResult = GAMEITEM_MES_DBERROR; // �о�� ������ DB�� �̵��ϴµ� �����Ͽ���
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		ErrorCnt++;
		return FALSE;
	}
	END_CATCH	


	// ����� �����̱� ������ ���ٸ� (���� ������ ����� ������ ���� ����Ҽ� ����!!)
	if( g_pMainView->Cfg.bItemServer && nItemNum<=0)
	{
		pItem->nResult = GAMEITEM_MES_NOITEM; // �������� �����ϰ� �����ʴ�
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		return FALSE;
	}

	if(g_pMainView->Cfg.bJumpItem)
	{
		// [���� ������ �۾�]
		if(nItemNum<=0)
		{
			pItem->nResult = GAMEITEM_MES_NOITEM; // �������� �����ϰ� �����ʴ�
		}
		else
		{
			pItem->nResult = GAMEITEM_MES_SUCCESS; // �������� �����ϰ� �ִ�
		}
	}



	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// �ƹ�Ÿ DB ���� //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	// �ƹ�Ÿ DB �б�
	TRY
	{
		m_pAvatarDB->m_strFilter = str;
		// ��������DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
		m_pAvatarDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pAvatarDB->IsEOF() || m_pAvatarDB->m_ID.CompareNoCase(id)!=0) {
			// �ƹ�Ÿ DB�� �������� ������ �⺻�� ����(���� ��ȣ�� �Է�)
			if(pUI->Sex == 1) pUI->AvatarInfo[5] = 1;
			else pUI->AvatarInfo[5] = 2;
		}
		else {
			// �ƹ�Ÿ ���� ����
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
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		return FALSE;
	}
	END_CATCH

	////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// ���� DB ���� ///////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	// ���� DB �б�
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
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH
	

	// �ݸ��� ȸ��ID�� ������ ���� DB�� ��ϵǾ� ���� ����
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
			ErrorCnt++;	// ���� ī��Ʈ ����
			return FALSE;
		}
		END_CATCH
	}

	// ���� DB�߰� ������ ���
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

	// ���� DB�� �о�� (KHS_FAST_CHAN)

	// ##############  [������Ŀ] ����ó��

	INT64 nPMoney = (INT64)::_atoi64(m_pGameDB->m_PMoney.operator LPCTSTR());
	
	// �������� ������ ��Ŀ�Ӵϸ� �ҷ��ͼ�...���߿� ����Ÿ���̽��� �� ��Ŀ�Ӵ��� ����ġ�� ����ϸ� �ȴ�!!
	if(nPMoney>g_pMainView->Cfg.MAX_PMONEY) nPMoney = g_pMainView->Cfg.MAX_PMONEY; // [ $$$$ ��Ŀ�Ӵ� �����÷ο� üũ $$$$ ]

	pUI->PMoney  = nPMoney;
	pUI->pBackup = nPMoney; // ��Ŀ�Ӵ� ���

	// �������� ���
	pUI->nIcon = GetPokerLevel(nPMoney);

	pUI->WinNum    = m_pGameDB->m_Vic;
	pUI->LooseNum  = m_pGameDB->m_Fail;
//	pUI->DrawNum   = m_pGameDB->m_Fold;

	// ī�� �ε���
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
	pUI->PI.nCard    = (char)m_pGameDB->m_Level2; // ��ǥī��
	pUI->PI.nRSF     = m_pGameDB->m_RSF;
	pUI->PI.nSF      = m_pGameDB->m_SF;
	pUI->PI.nFCARD   = m_pGameDB->m_FCARD;

	// �������� �����ڵ� �˻�
	if(pSrvCode != NULL) *pSrvCode = m_pGameDB->m_IsConnected;

	// [���� ������ �۾�] ########### ������ ó���۾� ���� ���� ###########

	// �ƹ�Ÿ/���ӵ�� ���������� �а�/ó���ߴٸ�,
	// �������̾��� ������ ���� ������ �����Ѵ� !!!!


	pItem->nNum = nItemNum; // �о�� ������ ������ �����Ѵ�(�������� 0���� Ŀ���Ѵ�!!!!)

//	 [���� ������ �۾�] ����
	// ��ȿ�ϰ� ������� �������� �ִ�
//	if(pItem->bLogable){
//		pItem->nResult = GAMEITEM_MES_SUCCESS; // �α��� �����ϴ�
//	}
//	// �ѹ��� �Ⱦ� �������� �Ѱ��̻� �����Ѵ�
//	else{
//		pItem->nResult = GAMEITEM_MES_USEABLE; // �����ۻ�� ����
//	}

	if(m_pUserDB->IsOpen()) m_pUserDB->Close();
	if(m_pItemDB->IsOpen()) m_pItemDB->Close();
	if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;
}
*/

/*
// [���� ������ �۾�] [���� ������ �۾�] ���� ������ ����� ����Ѵ�. 
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
	////////////////////////////// ������ DB ���� //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	pItem->nResult = GAMEITEM_MES_NOITEM; // ��밡���� �������� ����

	int nNO = pItem->Item[0].NO;
	CString strItem; // ����ũ�ڵ带 �־� �������� �д´� ID �� �ʿ� ���°�???
	strItem.Format(_T("[c_no] = %d"), nNO);

	TRY
	{
		m_pItemDB->m_strFilter = strItem;
		m_pItemDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);
	}
	CATCH(CDBException, e)
	{
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		ErrorCnt = 100; // DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		return FALSE;
	}
	END_CATCH

	// �����ϴ� �������� ���ų�, ����ũ���̵� Ʋ���ų�, �����ڰ� Ʋ���ٸ� ���� �Ұ���
	if(m_pItemDB->IsEOF() || m_pItemDB->m_NO != nNO || m_pItemDB->m_ID.CompareNoCase(pUI->ID) != 0)
	{
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		return FALSE;
	}

	BOOL bUse = m_pItemDB->m_Use;
	// �̹� ������� �������̶��..�Ƹ��� �ٸ���(��,���Ӽ���)���� ����÷��׸� �������� �𸥴�
	// ������� ���� �������ϰ�츸 ���׷��̵� �Ѵ�
	if(!bUse)
	{
		// DB�� �����ϴ� ����
		TRY
		{
			m_pItemDB->Edit();
			
			// ������ ������� �ۼ�
			m_pItemDB->m_Use  = 1;
			m_pItemDB->m_Date = pItem->Item[0].StartDate; // ���� �ð�( ���� �ð�)

			// ���� DB ������Ʈ
			m_pItemDB->Update();
		}
		CATCH(CDBException, e)
		{
			if(m_pItemDB->IsOpen()) m_pItemDB->Close();
			pItem->nResult = GAMEITEM_MES_USEFAIL; // ������ ��뿡 �����Ͽ���
			ErrorCnt++;	// ���� ī��Ʈ ����
			return FALSE;
		}
		END_CATCH
	}

	// ������ DB�� �ݴ´�
	if(m_pItemDB->IsOpen()) m_pItemDB->Close();

	return TRUE;
}
*/





/*

// [������� ������] 
//BOOL CDBThread::GetUserInfo(char *id, char *pass, USERINFO *pUI, BOOL *pIsAnotherConnect, BOOL *pIsWrongPwd, int *pIDState) 
BOOL CDBThread::GetUserInfo1(char *id, char *pass, USERINFO *pUI, BOOL *pIsAnotherConnect, BOOL *pIsWrongPwd, int *pIDState, int *pSrvCode, BOOL *pNegPMoney, GAMEITEM_LIST *pItem) 
{
	if(!CreateUserDB() || !CreateGameDB() || !CreateAvatarDB() || !CreateItemDB()) {
		CLogFile logfile;
		logfile.Writef("GetUserInfo - DB ���ؼ� ���� ����");
		// ���ؼ� ���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}

	if(m_pUserDB==NULL || m_pGameDB==NULL || m_pAvatarDB==NULL || m_pItemDB==NULL) return FALSE;
	if(pUI == NULL || pItem == NULL) return FALSE;


	pItem->bLogable = FALSE;                 // �α��� �����Ѱ�?
	pItem->nResult  = GAMEITEM_MES_ERROR;    // �α��� �Ұ��ڵ�



	CString str;
	str.Format("[ID] = '%s'", id);

	TRY
	{
		// ���ڵ���� ���������� �ݴ´�
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();

		m_pUserDB->m_strFilter = str;
		// ��������DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
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
		logfile.Writef("UserDB���� - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	// ��й�ȣ �˻�
	if(pIsWrongPwd != NULL) {
		if(m_pUserDB->m_Passwd.CompareNoCase(pass)==0)
			*pIsWrongPwd = FALSE;
		else {
			*pIsWrongPwd = TRUE;
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			return FALSE;
		}
	}

	// �ٸ� ���ӿ� �̹� ���������� �˻�
	//if(pIsAnotherConnect != NULL) *pIsAnotherConnect = m_pUserDB->m_IsConnected;

	// ���� ���� Ȥ�� ������ ���̵����� �˻�
	if(pIDState != NULL) *pIDState = m_pUserDB->m_IDState;

	strncpy(pUI->ID, m_pUserDB->m_ID.operator LPCTSTR(), 15);
	strncpy(pUI->Pass, m_pUserDB->m_Passwd.operator LPCTSTR(), 8);
	//strncpy(pUI->Name, m_pUserDB->m_Name.operator LPCTSTR(), 10);
	pUI->Sex = (char)m_pUserDB->m_Sex;
//	pUI->Marble = (int)m_pUserDB->m_Total_Marble;
	strncpy(pUI->Desc, m_pUserDB->m_Desc.operator LPCTSTR(), 60);

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// ������ DB ���� //////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	CString strItem; // �����ڵ�� ���̵� �д´�
	//strItem.Format(_T("[c_gametype] = %d AND [id] = '%s'"), g_pMainView->Cfg.GameCode, pUI->ID);
	strItem.Format(_T("[c_gametype] = %d AND [c_code]>0 AND [c_code]<100 AND [id] = '%s'"), 
		g_pMainView->Cfg.GameCode, pUI->ID); // ����� �������� c_code�� 1 ~ 100 ���̷� �Ѵ�

	// ������ DB �б�
	TRY
	{
		m_pItemDB->m_strFilter = strItem;
		// ������DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
		m_pItemDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pItemDB->IsEOF() || m_pItemDB->m_ID.CompareNoCase(id)!=0) {
			pItem->nResult = GAMEITEM_MES_NOITEM; // �������� �����ϰ� �����ʴ�
		// ############### ��й� �������� ���ٰ� �����ų�� ���� ##############
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
		pItem->nResult = GAMEITEM_MES_DBERROR; // ������ DB�� �дµ� �����Ͽ���
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH

	COleDateTime nt = COleDateTime::GetCurrentTime();

	// ���������� �о����
	int nItemNum = 0;
	TRY
	{
		//if(!m_pItemDB->IsBOF()) m_pItemDB->MoveFirst();
		for(;;)
		{
			if(m_pItemDB->IsEOF() || nItemNum>= MAX_ITEM_NUM) break; // �о�� ����Ʈ ��� �аų�, ���� �ʰ��Ǿ��ٸ� �����Ѵ�

			// ������ ����� Ȯ��
			if(m_pItemDB->m_ID.CompareNoCase(id)==0) {
				short Day  = (short)m_pItemDB->m_Day; // ��ȿ�Ⱓ
				// �ùٸ� ����Ÿ�� �ƴϴ�
				if(Day<=0){
					m_pItemDB->MoveNext();
					continue;					
				}
				// ��ȿ�� �Ⱓ�� �����Ѵ�-�д����� �����Ѵ�
				double period = (double)(Day*60*24);
				int   Use    = m_pItemDB->m_Use; // �������
				SYSTEMTIME sysTime; memset(&sysTime,0,sizeof(SYSTEMTIME));
				if(Use>0){ // ���Ǿ��� �������̶�� ������ ��¥�� �ִ�
					COleDateTime st = m_pItemDB->m_Date; // ��ŸƮ�ð�
					if(st.GetStatus()!=COleDateTime::valid){
						m_pItemDB->MoveNext();
						continue;
					}
					st.GetAsSystemTime(sysTime);
					COleDateTimeSpan TimeSpan = nt - st;     // ����ð����� ���̸� ���Ѵ�
					if(TimeSpan.GetTotalMinutes()>period){   // �پ� �������̴�
						m_pItemDB->MoveNext();
						continue;					
					}
					// ���� �������� �ְ�, ��ȿ�Ѱ� 1���� �ִٸ� ���尡���ϴ�!!!
					pItem->bLogable = TRUE;
				}
				pItem->Item[nItemNum].Index  = nItemNum; // �ε��� �ɱ�
				pItem->Item[nItemNum].Day    = Day;      // ��ȿ�Ⱓ
				pItem->Item[nItemNum].Use    = (short)Use;      // �������
				memcpy(&(pItem->Item[nItemNum].StartDate),&sysTime,sizeof(SYSTEMTIME)); // ���� �ð�

				pItem->Item[nItemNum].NO     = (int)m_pItemDB->m_NO;    // ����ũ�ѹ�
				pItem->Item[nItemNum].Code   = (int)m_pItemDB->m_Code;  // ������ �ڵ�
				strncpy(pItem->Item[nItemNum].PresID,m_pItemDB->m_PresID.operator LPCTSTR(),15); // ������ ���̵�

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

		pItem->nResult = GAMEITEM_MES_DBERROR; // �о�� ������ DB�� �̵��ϴµ� �����Ͽ���
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		ErrorCnt++;
		return FALSE;
	}
	END_CATCH	

	// ������ �������� �Ѱ��� ���ٸ�...
	if(nItemNum<=0){
		pItem->nResult = GAMEITEM_MES_NOITEM; // �������� �����ϰ� �����ʴ�
		// ############### ��й� �������� ���ٰ� �����ų�� ���� ##############
		//if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		//if(m_pItemDB->IsOpen()) m_pItemDB->Close();
		//return FALSE;
	}




	// �ƹ�Ÿ DB �б�
	TRY
	{
		m_pAvatarDB->m_strFilter = str;
		// ��������DB�� readOnly���� ���� �ӵ� ����� ���Ͽ� excuteDirect �ɼ��� ���(Requery�� �� ����)
		m_pAvatarDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
		if(m_pAvatarDB->IsEOF() || m_pAvatarDB->m_ID.CompareNoCase(id)!=0) {
			// �ƹ�Ÿ DB�� �������� ������ �⺻�� ����(���� ��ȣ�� �Է�)
			if(pUI->Sex == 1) pUI->AvatarInfo[5] = 1;
			else pUI->AvatarInfo[5] = 2;
		}
		else {
			// �ƹ�Ÿ ���� ����
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
		logfile.Writef("AvatarDB���� - GetUserInfo Open (ID=%s): %s", pUI->ID, e->m_strError.operator LPCTSTR());
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		return FALSE;
	}
	END_CATCH

	// ���� DB �б�
	TRY
	{
		m_pGameDB->m_strFilter = str;
		m_pGameDB->Open(CRecordset::snapshot, NULL, CRecordset::executeDirect | CRecordset::readOnly);
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("GameDB���� - GetUserInfo Open (ID=%s): %s", id, e->m_strError.operator LPCTSTR());
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		// DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		ErrorCnt = 100;
		return FALSE;
	}
	END_CATCH
	

	// �ݸ��� ȸ��ID�� ������ ���� DB�� ��ϵǾ� ���� ����
	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		if(AddNewGameDB(pUI) == FALSE) {
			CLogFile logfile;
			logfile.Writef("GameDB���� - GetUserInfo (ID=%s) : Game DB ���̵� �߰� ����", id);
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
			logfile.Writef("GameDB���� - GetUserInfo Requery (ID=%s) : %s", id, e->m_strError.operator LPCTSTR());
			if(m_pUserDB->IsOpen()) m_pUserDB->Close();
			if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
			if(m_pGameDB->IsOpen()) m_pGameDB->Close();
			ErrorCnt++;	// ���� ī��Ʈ ����
			return FALSE;
		}
		END_CATCH
	}

	// ���� DB�߰� ������ ���
	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID)!=0)
	{
		CLogFile logfile;
		logfile.Writef("GameDB���� (ID=%s) : Game DB ���̵� �߰� ����", id);
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// ���� DB�� �о�� (KHS_FAST_CHAN)
	// ##############  [������Ŀ] ����ó��
	INT64 nPMoney = (INT64)::_atoi64(m_pGameDB->m_PMoney.operator LPCTSTR());
	if(nPMoney<0){ // ###### ��Ŀ�Ӵ� ������
		CLogFile logfile(_T("Abuse.txt"));
		logfile.Writef(g_StrMan._Get(_T("SPOKER_INVAL_PM")), id, nPMoney);
		if(m_pUserDB->IsOpen()) m_pUserDB->Close();
		if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();

		if(pNegPMoney!=NULL) *pNegPMoney = TRUE;

		return FALSE;
	}
	// �������� ������ ��Ŀ�Ӵϸ� �ҷ��ͼ�...���߿� ����Ÿ���̽��� �� ��Ŀ�Ӵ��� ����ġ�� ����ϸ� �ȴ�!!
	if(nPMoney>g_pMainView->Cfg.MAX_PMONEY) nPMoney = g_pMainView->Cfg.MAX_PMONEY; // [ $$$$ ��Ŀ�Ӵ� �����÷ο� üũ $$$$ ]

	pUI->PMoney  = nPMoney;
	pUI->pBackup = nPMoney; // ��Ŀ�Ӵ� ���

	// �������� ���
	pUI->nIcon = GetPokerLevel(nPMoney);

//	pUI->Character = m_pGameDB->m_Character;
	pUI->WinNum    = m_pGameDB->m_Vic;
	pUI->LooseNum  = m_pGameDB->m_Fail;
//	pUI->DrawNum   = m_pGameDB->m_Fold;
//	pUI->Point     = m_pGameDB->m_Level1;

	// ī�� �ε��� ???
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
	pUI->PI.nCard    = (char)m_pGameDB->m_Level2; // ��ǥī��
	pUI->PI.nRSF     = m_pGameDB->m_RSF;
	pUI->PI.nSF      = m_pGameDB->m_SF;
	pUI->PI.nFCARD   = m_pGameDB->m_FCARD;


	// �������� �����ڵ� �˻�
	if(pSrvCode != NULL) *pSrvCode = m_pGameDB->m_IsConnected;



	// [���� ������ �۾�] ########### ������ ó���۾� ���� ���� ###########

	// �ƹ�Ÿ/���ӵ�� ���������� �а�/ó���ߴٸ�,
	// �������̾��� ������ ���� ������ �����Ѵ� !!!!

	pItem->nNum = nItemNum; // �о�� ������ ������ �����Ѵ�(�������� 0���� Ŀ���Ѵ�!!!!)

	// ��ȿ�ϰ� ������� �������� �ִ�
	if(pItem->bLogable){
		pItem->nResult = GAMEITEM_MES_SUCCESS; // �α��� �����ϴ�
	}
	// �ѹ��� �Ⱦ� �������� �Ѱ��̻� �����Ѵ�
	else{
		pItem->nResult = GAMEITEM_MES_USEABLE; // �����ۻ�� ����
	}



	if(m_pUserDB->IsOpen()) m_pUserDB->Close();
	if(m_pAvatarDB->IsOpen()) m_pAvatarDB->Close();
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;
}
*/


/*
// ������Ŀ ������ �ߺ� ������ ����Ѵ�(���ӷα��ν� ����ϴ� ��ƾ�̴�)
BOOL CDBThread::SetLogInInfo(USERINFO *pUI, int *pSrvCode, BOOL &bDCon)
{

	return FALSE;

	if(pUI == NULL) return FALSE;

	/////////////////////////////// ���ӵ�� ������Ʈ ///////////////////////////////////

	if(!CreateGameDB()) {
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pGameDB==NULL) return FALSE;
	if(m_pGameDB->IsOpen()) m_pGameDB->Close(); // ���ڵ���� ���������� �ݴ´�

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
		ErrorCnt = 100; // DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		return FALSE;
	}
	END_CATCH

	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID) != 0)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// �ٽ��ѹ� �α��� �ߺ� ���� ���ϱ�
	if(m_pGameDB->m_IsConnected!=0){
		bDCon = TRUE;
		if(pSrvCode  != NULL) *pSrvCode  = m_pGameDB->m_IsConnected;
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}
		
	// DB�� �����ϴ� ����
	TRY
	{
		m_pGameDB->Edit();
		// �����ڵ� �ۼ�
		m_pGameDB->m_IsConnected = g_pMainView->Cfg.ServerCode;//g_MainView.ServerCode;
		// ���� DB ������Ʈ
		m_pGameDB->Update();
	}
	CATCH(CDBException, e)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt++;	// ���� ī��Ʈ ����
		return FALSE;
	}
	END_CATCH

	// ������ DB�� �ݴ´�
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;
}
*/

/*
// ������Ŀ ������ �ߺ� ������ ����Ѵ�(���ӷα׾ƿ��� ����ϴ� ��ƾ�̴�)
BOOL CDBThread::SetLogOutInfo(USERINFO *pUI)
{
	if(pUI == NULL) return FALSE;

	/////////////////////////////// ���ӵ�� ������Ʈ ///////////////////////////////////

	if(!CreateGameDB()) {
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pGameDB==NULL) return FALSE;
	if(m_pGameDB->IsOpen()) m_pGameDB->Close(); // ���ڵ���� ���������� �ݴ´�

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
		ErrorCnt = 100; // DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		return FALSE;
	}
	END_CATCH

	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID) != 0)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// �̹� 0 ���� ���õǾ� �ִٸ� ���ʿ��ϴ�, �Ǵ� �̹� �ٸ������� �������̴�!
	if(m_pGameDB->m_IsConnected==0 || m_pGameDB->m_IsConnected!=g_pMainView->Cfg.ServerCode)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return TRUE; // ���� ����
	}
		
	// DB�� �����ϴ� ����
	TRY
	{
		m_pGameDB->Edit();
		// ���� ����� ���� �ڵ带 �ʱ�ȭ
		m_pGameDB->m_IsConnected = 0; 
		// ���� DB ������Ʈ
		m_pGameDB->Update();
	}
	CATCH(CDBException, e)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		ErrorCnt++;	// ���� ī��Ʈ ����
		return FALSE;
	}
	END_CATCH

	// ������ DB�� �ݴ´�
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;
}
*/

/*
// ������Ŀ ������ ����Ѵ�(�����߰��� ����ϴ� ��ƾ�̴�)
BOOL CDBThread::SetGameInfo(USERINFO *pUI)
{
	
	return FALSE;





	if(pUI == NULL) return FALSE;

	/////////////////////////////// ���ӵ�� ������Ʈ ///////////////////////////////////

	if(!CreateGameDB()) {
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pGameDB==NULL) return FALSE;
	if(m_pGameDB->IsOpen()) m_pGameDB->Close(); // ���ڵ���� ���������� �ݴ´�

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
		ErrorCnt = 100; // DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
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
	
	// DB�� �����ϴ� ����
	TRY
	{
		m_pGameDB->Edit();

		m_pGameDB->m_Vic     = pUI->WinNum ;
		m_pGameDB->m_Fail    = pUI->LooseNum;
//		m_pGameDB->m_Fold    = pUI->DrawNum;
//		m_pGameDB->m_Level1  = pUI->Point;


		INT64 NowPMoney  = (INT64)::_atoi64(m_pGameDB->m_PMoney.operator LPCTSTR()); // ����Ǿ����� �𸣴� ����Ÿ���̽� ��Ŀ�Ӵ�
		INT64 GamePMoney = pUI->PMoney;                // �������� ���ӸӴ�
		if(GamePMoney<0) GamePMoney = 0;               // �������� ���ӸӴϴ� �� �¾ƾ��Ѵ�. �������� 0���� ó���� ���´�!!!
		INT64 ResPMoney  = GamePMoney - pUI->pBackup;  // ��Ŀ�Ӵ� ����ġ ���
//		INT64 ResPMoney  = pUI->PMoney - pUI->pBackup; // ��������� ��Ŀ�Ӵ� ����ġ ���
		NowPMoney+=ResPMoney;                          // ��Ŀ�Ӵ� ������
		
		// ����
		if(NowPMoney<0) 
		{
			CLogFile logfile(_T("Abuse.txt"));
			logfile.Writef(g_StrMan._Get(_T("SPOKER_INVAL_PM")), pUI->ID, NowPMoney);
			NowPMoney = 0;                 // ����� ������ ���� ����(<0)ó�� ���� �ʴ´�!!!
		}

		// �������� ������ ��Ŀ�Ӵϸ� �ҷ��ͼ�...���߿� ����Ÿ���̽��� �� ��Ŀ�Ӵ��� ����ġ�� ����ϸ� �ȴ�!!
		if(NowPMoney>g_pMainView->Cfg.MAX_PMONEY) NowPMoney = g_pMainView->Cfg.MAX_PMONEY; // [ $$$$ ��Ŀ�Ӵ� �����÷ο� üũ $$$$ ]

		// ���� ����
		m_pGameDB->m_Level   = GetPokerLevel(NowPMoney);  // ��Ŀ���� ������� �ʴ´� �������� ����ϴ� �б����뵥��Ÿ�̴�


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
		m_pGameDB->m_Level2 = pUI->PI.nCard; // ��ǥī��
		m_pGameDB->m_RSF    = pUI->PI.nRSF;
		m_pGameDB->m_SF     = pUI->PI.nSF;
		m_pGameDB->m_FCARD  = pUI->PI.nFCARD;

		m_pGameDB->m_WinPer = GetWinPro(pUI->WinNum, pUI->LooseNum);//, pUI->DrawNum);

		// ���� ���� DB ������Ʈ
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
		ErrorCnt++;	// ���� ī��Ʈ ����
		return FALSE;
	}
	END_CATCH

	// ������ DB�� �ݴ´�
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return TRUE;
}
*/

/*
// �ߺ������� Ȯ���Ѵ�
BOOL CDBThread::AnalDoubleConnect(USERINFO *pUI, int *pSrvCode)
{
	if(pUI == NULL) return FALSE;

	/////////////////////////////// ���ӵ�� ������Ʈ ///////////////////////////////////

	if(!CreateGameDB()) {
		ErrorCnt = 100;
		return FALSE;
	}
	if(m_pGameDB==NULL) return FALSE;
	if(m_pGameDB->IsOpen()) m_pGameDB->Close(); // ���ڵ���� ���������� �ݴ´�

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
		ErrorCnt = 100; // DB���� ������ ��� ���� ī��Ʈ�� 100���� ����
		return FALSE;
	}
	END_CATCH

	if(m_pGameDB->IsEOF() || m_pGameDB->m_ID.CompareNoCase(pUI->ID) != 0)
	{
		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return FALSE;
	}

	// �����ڵ尡 �����ʴٸ�...�ߺ� �����̴�!
	if(m_pGameDB->m_IsConnected!=g_pMainView->Cfg.ServerCode)
	{
		if(pSrvCode  != NULL) *pSrvCode  = m_pGameDB->m_IsConnected;

		if(m_pGameDB->IsOpen()) m_pGameDB->Close();
		return TRUE; // �ߺ����� �ǹ��� ���� ����
	}
		
	// ������ DB�� �ݴ´�
	if(m_pGameDB->IsOpen()) m_pGameDB->Close();

	return FALSE;
}
*/