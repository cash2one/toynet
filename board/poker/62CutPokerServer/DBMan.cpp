// DBMan.cpp: implementation of the CDBMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "62CutPokerServerView.h"
#include "DBMan.h"
#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBMan::CDBMan()
{
	pDBThread = NULL;
	DiscardNum = 0;
	TotThreadNum = 0;
}

CDBMan::~CDBMan()
{
	TermanateAllDBThread();
	if(pDBThread) { delete pDBThread; pDBThread = NULL; }
}

CDBThread** CDBMan::AllocDBThreadPtr(int threadnum)
{
	CDBThread** pp = new (CDBThread*[threadnum]);
	memset(pp, 0, sizeof(CDBThread*)*threadnum);
	return pp;
}

BOOL CDBMan::ChangeDBThreadNum(int threadnum)
{
	if(!pDBThread) return FALSE;

	if(threadnum < MINIMUM_DBTHREAD) return FALSE;
	if(threadnum > MAXIMUM_DBTHREAD) return FALSE;

	CDBThread** pNew = AllocDBThreadPtr(threadnum);

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

BOOL CDBMan::Init(int threadnum)
{
	if(pDBThread) return FALSE;

	if(threadnum < MINIMUM_DBTHREAD) threadnum = MINIMUM_DBTHREAD;
	if(threadnum > MAXIMUM_DBTHREAD) threadnum = MAXIMUM_DBTHREAD;

	pDBThread = AllocDBThreadPtr(threadnum);

	TotThreadNum = threadnum;

	// 메인 유저DB 정보 초기화
	CUserDB::m_DSN = g_pMainView->Cfg.UserDSN;
	CUserDB::m_Table = g_pMainView->Cfg.UserTable;
	CUserDB::m_UID = g_pMainView->Cfg.UserUID;
	CUserDB::m_Pass = g_pMainView->Cfg.UserPass;
	// 게임DB 정보 초기화
	CGameDB::m_DSN = g_pMainView->Cfg.GameDSN;
	CGameDB::m_Table = g_pMainView->Cfg.GameTable;
	CGameDB::m_UID = g_pMainView->Cfg.GameUID;
	CGameDB::m_Pass = g_pMainView->Cfg.GamePass;
	// 불량 아이디 신고DB 정보 초기화
	CBadUserDB::m_DSN = g_pMainView->Cfg.BadUserDSN;
	CBadUserDB::m_Table = g_pMainView->Cfg.BadUserTable;
	CBadUserDB::m_UID = g_pMainView->Cfg.BadUserUID;
	CBadUserDB::m_Pass = g_pMainView->Cfg.BadUserPass;
	// 접속자 인원 현황 DB 정보 초기화
	CStatusDB::m_DSN = g_pMainView->Cfg.StatusDSN;
	CStatusDB::m_Table = g_pMainView->Cfg.StatusTable;
	CStatusDB::m_UID = g_pMainView->Cfg.StatusUID;
	CStatusDB::m_Pass = g_pMainView->Cfg.StatusPass;
	// 아바타DB 정보 초기화
	CAvatarDB::m_DSN = g_pMainView->Cfg.AvatarDSN;
	CAvatarDB::m_Table = g_pMainView->Cfg.AvatarTable;
	CAvatarDB::m_UID = g_pMainView->Cfg.AvatarUID;
	CAvatarDB::m_Pass = g_pMainView->Cfg.AvatarPass;
	// 아이템DB 정보 초기화	[게임 아이템 작업]
	CItemDB::m_DSN = g_pMainView->Cfg.ItemDSN;
	CItemDB::m_Table = g_pMainView->Cfg.ItemTable;
	CItemDB::m_UID = g_pMainView->Cfg.ItemUID;
	CItemDB::m_Pass = g_pMainView->Cfg.ItemPass;
	// 프리미엄DB 정보 초기화	[프리미엄 작업]
	CPremiumDB::m_DSN = g_pMainView->Cfg.PremiumDSN;
	CPremiumDB::m_Table = g_pMainView->Cfg.PremiumTable;
	CPremiumDB::m_UID = g_pMainView->Cfg.PremiumUID;
	CPremiumDB::m_Pass = g_pMainView->Cfg.PremiumPass;
	// 프리미엄IP DB 정보 초기화	[PC방 작업]
	CPremIPDB::m_DSN = g_pMainView->Cfg.PremIPDSN;
	CPremIPDB::m_Table = g_pMainView->Cfg.PremIPTable;
	CPremIPDB::m_UID = g_pMainView->Cfg.PremIPUID;
	CPremIPDB::m_Pass = g_pMainView->Cfg.PremIPPass;

	// 이벤트  DB 정보 초기화  ### [이벤트 티켓] ###
	CEventPrizeDB::m_DSN = g_pMainView->Cfg.EventDSN;
	CEventPrizeDB::m_Table = g_pMainView->Cfg.EventTable;
	CEventPrizeDB::m_UID = g_pMainView->Cfg.EventUID;
	CEventPrizeDB::m_Pass = g_pMainView->Cfg.EventPass;

	TRY
	{		
		
			// 오픈 시간을 단축 시키기 위하여 의미없는 쿼리를 줌
		m_UserDB.m_strFilter = "[ID]='fdsfsdfdsfd'";
		m_AvatarDB.m_strFilter = "[id]='fdsfsafds'";
		
		m_GameDB.m_strFilter = "[NO]=0";
		m_BadUserDB.m_strFilter = "[NO]=0";
		m_StatusDB.m_strFilter = "[NO]=0";

		m_ItemDB.m_strFilter = "[id]='fdsfsafds'";		// [게임 아이템 작업]
		m_PremiumDB.m_strFilter = "[NO]='fdsfsafds'";	// [프리미엄 작업]
		m_PremIPDB.m_strFilter = "[ip]='fdsfsafds'";	// [PC방 작업]
		m_EventPrizeDB.m_strFilter = "[ID]='ffdfdfdf'"; // ### [이벤트 티켓] ###

		m_UserDB.Open();
		m_AvatarDB.Open();
		m_GameDB.Open();
		m_BadUserDB.Open();
		m_StatusDB.Open();	
		m_ItemDB.Open();		// [게임 아이템 작업]
		m_PremiumDB.Open();		// [프리미엄 작업]
		m_PremIPDB.Open();		// [PC방 작업]
		
		if( g_pMainView->Cfg.bEventDB ) {	// ### [이벤트 티켓] ###
			m_EventPrizeDB.Open();	
		}

		m_UserDB.m_pDatabase->SetQueryTimeout(60);
		m_GameDB.m_pDatabase->SetQueryTimeout(60);
		m_BadUserDB.m_pDatabase->SetQueryTimeout(60);
		m_StatusDB.m_pDatabase->SetQueryTimeout(60);
		m_AvatarDB.m_pDatabase->SetQueryTimeout(60);
		m_ItemDB.m_pDatabase->SetQueryTimeout(60);		// [게임 아이템 작업]
		m_PremiumDB.m_pDatabase->SetQueryTimeout(60);	// [프리미엄 작업]
		m_PremIPDB.m_pDatabase->SetQueryTimeout(60);	// [PC방 작업]

		if( g_pMainView->Cfg.bEventDB ) {	// ### [이벤트 티켓] ###
			m_EventPrizeDB.m_pDatabase->SetQueryTimeout(60);	
		}

	}
	CATCH(CDBException, e)
	{
		CString str;
		str.Format("DB 오픈 에러: %s", e->m_strError.operator LPCTSTR());
		AfxMessageBox(str);
		
		if(m_UserDB.IsOpen()) m_UserDB.Close();
		if(m_GameDB.IsOpen()) m_GameDB.Close();
		if(m_BadUserDB.IsOpen()) m_BadUserDB.Close();
		if(m_StatusDB.IsOpen()) m_StatusDB.Close();
		if(m_AvatarDB.IsOpen()) m_AvatarDB.Close();
		if(m_ItemDB.IsOpen()) m_ItemDB.Close();			// [게임 아이템 작업]
		if(m_PremiumDB.IsOpen()) m_PremiumDB.Close();	// [프리미엄 작업]
		if(m_PremIPDB.IsOpen()) m_PremIPDB.Close();		// [PC방 작업]
		
		if( g_pMainView->Cfg.bEventDB ) {	// ### [이벤트 티켓] ###
			if(m_EventPrizeDB.IsOpen()) m_EventPrizeDB.Close();
		}

		return FALSE;
	}
	END_CATCH

	if(m_UserDB.IsOpen()) m_UserDB.Close();
	if(m_GameDB.IsOpen()) m_GameDB.Close();
	if(m_BadUserDB.IsOpen()) m_BadUserDB.Close();
	if(m_StatusDB.IsOpen()) m_StatusDB.Close();
	if(m_AvatarDB.IsOpen()) m_AvatarDB.Close();
	if(m_ItemDB.IsOpen()) m_ItemDB.Close();			// [게임 아이템 작업]
	if(m_PremiumDB.IsOpen()) m_PremiumDB.Close();	// [프리미엄 작업]
	if(m_PremIPDB.IsOpen()) m_PremIPDB.Close();		// [PC방 작업]

	if( g_pMainView->Cfg.bEventDB ) {	// ### [이벤트 티켓] ###
		if(m_EventPrizeDB.IsOpen()) m_EventPrizeDB.Close();
	}


	return TRUE;
}

BOOL CDBMan::CloseTempDB()
{
	TRY
	{
		if(m_UserDB.m_pDatabase) {
			if(m_UserDB.IsOpen()) m_UserDB.Close();
			if(m_UserDB.m_pDatabase->IsOpen()) m_UserDB.m_pDatabase->Close();
		}
		if(m_GameDB.m_pDatabase) {
			if(m_GameDB.IsOpen()) m_GameDB.Close();
			if(m_GameDB.m_pDatabase->IsOpen()) m_GameDB.m_pDatabase->Close();
		}
		if(m_StatusDB.m_pDatabase) {
			if(m_StatusDB.IsOpen()) m_StatusDB.Close();
			if(m_StatusDB.m_pDatabase->IsOpen()) m_StatusDB.m_pDatabase->Close();
		}
		if(m_AvatarDB.m_pDatabase) {
			if(m_AvatarDB.IsOpen()) m_AvatarDB.Close();
			if(m_AvatarDB.m_pDatabase->IsOpen()) m_AvatarDB.m_pDatabase->Close();
		}
		if(m_ItemDB.m_pDatabase) {	// [게임 아이템 작업]
			if(m_ItemDB.IsOpen()) m_ItemDB.Close();
			if(m_ItemDB.m_pDatabase->IsOpen()) m_ItemDB.m_pDatabase->Close();
		}
		if(m_PremiumDB.m_pDatabase) {	// [프리미엄 작업]
			if(m_PremiumDB.IsOpen()) m_PremiumDB.Close();
			if(m_PremiumDB.m_pDatabase->IsOpen()) m_PremiumDB.m_pDatabase->Close();
		}
		if(m_PremIPDB.m_pDatabase) {	// [PC방 작업]
			if(m_PremIPDB.IsOpen()) m_PremIPDB.Close();
			if(m_PremIPDB.m_pDatabase->IsOpen()) m_PremIPDB.m_pDatabase->Close();
		}

		if(g_pMainView->Cfg.bEventDB ) { // ### [이벤트 티켓] ###
			if(m_EventPrizeDB.m_pDatabase) {	
				if(m_EventPrizeDB.IsOpen()) m_EventPrizeDB.Close();
				if(m_EventPrizeDB.m_pDatabase->IsOpen()) m_EventPrizeDB.m_pDatabase->Close();
			}
		}
	}
	CATCH(CDBException, e)
	{
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

BOOL CDBMan::CreateKeepAliveThread()
{
	BOOL bRtn = TRUE;
	for(int i=0; i<TotThreadNum; i++) {
		if(pDBThread[i] == NULL) {
			pDBThread[i] = new CDBThread();
			BOOL bFail = FALSE;			
			
			if(pDBThread[i]->CreateUserDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }
			if(pDBThread[i]->CreateGameDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }
			if(pDBThread[i]->CreateStatusDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }
			if(pDBThread[i]->CreateAvatarDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }
			if(pDBThread[i]->CreateItemDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }		// [게임 아이템 작업]
			if(pDBThread[i]->CreatePremiumDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }	// [프리미엄 작업]
			if(pDBThread[i]->CreatePremIPDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }	// [PC방 작업]

			if( g_pMainView->Cfg.bEventDB ) {	// ### [이벤트 티켓] ###
				if(pDBThread[i]->CreateEventPrizeDB()==FALSE) {bFail=TRUE; bRtn=FALSE;}	
			}

			if(bFail) { pDBThread[i]->DeleteAllDB(); continue; }

			// 환경 설정에서 DB클로즈 옵션이 설정되어 있다면 오픈된 DB를 닫음
			if(g_pMainView->Cfg2.bDBClose) pDBThread[i]->DeleteAllDB();

			// DB쓰레드를 생성(초기에는 아무일도 하지 않음)
			if(pDBThread[i]->Begin(&DBQue)==FALSE) {
				delete pDBThread[i];
				pDBThread[i] = NULL;
				bRtn=FALSE;
			}
		}
	}
	return bRtn;
}

BOOL CDBMan::SetDBWork(DBWORKS *pWork)
{
	if(!DBQue.EnQue(pWork)) return FALSE;

	for(int i=0; i<TotThreadNum; i++) {
		if(pDBThread[i] == NULL) {
			pDBThread[i] = new CDBThread();
			// 쓰레드 유지 모드로 구동
			return pDBThread[i]->Begin(&DBQue);
		}
		else if(!pDBThread[i]->bNowTerminate && pDBThread[i]->bWorking==FALSE) {
			// DB 작업 완료후 Suspend상태 있는 쓰레드라면 재가동
			return pDBThread[i]->Begin(&DBQue);
		}
	}

	// 모든 쓰레드가 풀 가동하는 경우 처리가 끝난 다른 쓰레드에서 자동으로 처리할것임
	return TRUE;
}

BOOL CDBMan::DiscardDBThread(int tnum)
{
	if(tnum<0 || tnum>TotThreadNum) return FALSE;
	if(pDBThread[tnum] == NULL) return FALSE;

	// DB 쓰레드 종료 플래그를 세팅한다
	pDBThread[tnum]->bNowTerminate = TRUE;

	// 로그 기록
	CString strFileName = GetLogFileName();
	if(strFileName!=_T("")){
		CLogFile logfile(strFileName.operator LPCTSTR());
		logfile.Writef(_T("Throw DB thread :  : (ID=[%s], UNum=%d,  Win=%d, Fail=%d,  PMoney증감치=%I64d, RSF=%d, SF=%d, FCARD=%d)"),
			pDBThread[tnum]->Work.UI.ID, pDBThread[tnum]->Work.UI.UNum, 
			//pDBThread[tnum]->Work.UI.WinNum, pDBThread[tnum]->Work.UI.LooseNum, pDBThread[tnum]->Work.UI.DrawNum, 
			pDBThread[tnum]->Work.UI.WinNum, pDBThread[tnum]->Work.UI.LooseNum, 
			(INT64)Max(pDBThread[tnum]->Work.UI.PMoney,0)-pDBThread[tnum]->Work.OI.pBackup,
			pDBThread[tnum]->Work.UI.PI.nRSF, pDBThread[tnum]->Work.UI.PI.nSF, pDBThread[tnum]->Work.UI.PI.nFCARD);
	}

/*
	// 로그 기록
	CLogFile logfile;
	logfile.Writef("DB 쓰레드 버림 : ID=%s, Marble=%d, Win=%d, Loose=%d, Point=%d",
		pDBThread[tnum]->Work.UI.ID, 
		pDBThread[tnum]->Work.UI.Marble, 
		pDBThread[tnum]->Work.UI.WinNum, 
		pDBThread[tnum]->Work.UI.LooseNum, 
		pDBThread[tnum]->Work.UI.Point);
*/
	// 쓰레드를 강제 종료
	//pDBThread[tnum]->Terminate();
	// 메모리 해제를 하지 않고 포인터를 초기화하여 그냥 버린다
	pDBThread[tnum] = NULL;

	DiscardNum++;
	CString str;
	str.Format("%d", DiscardNum);
	(g_pMainView->GetDlgItem(IDC_STATIC_DISCARDNUM))->SetWindowText(str);

	return TRUE;
}

int CDBMan::GetWorkingDBThreadNum()
{
	int totnum = 0;
	for(int i=0; i<TotThreadNum; i++) if(pDBThread[i] != NULL && pDBThread[i]->bWorking) totnum++;
	return totnum;
}

int CDBMan::GetLiveDBThreadNum()
{
	int totnum = 0;
	for(int i=0; i<TotThreadNum; i++) if(pDBThread[i] != NULL) totnum++;
	return totnum;
}

void CDBMan::CheckDBThread()
{
	for(int i=0; i<TotThreadNum; i++) 
	{
		if(pDBThread[i] == NULL) continue;
		// 올바르게 종료된 쓰레드는 메모리를 해제한다
		if(pDBThread[i]->pThread == NULL && pDBThread[i]->bWorking == FALSE) {
			delete pDBThread[i];
			pDBThread[i] = NULL;
			continue;
		}
		// 시간초과된 쓰레드는 버린다?
		if(pDBThread[i]->bWorking) {
			UINT dtick = timeGetTime() - pDBThread[i]->Work.StartTick;
			if(dtick > MAX_DBTHREAD_TIMEOUT) DiscardDBThread(i);
		}
	}
}

void CDBMan::TermanateAllDBThread()
{
	for(int i=0; i<TotThreadNum; i++) {
		if(pDBThread[i]==NULL) continue;
		if(pDBThread[i]->pThread != NULL) {
			// 쓰레드 종료
			pDBThread[i]->Terminate();
		}
		// DB 쓰레드 클래스 제거
		delete pDBThread[i];
		pDBThread[i] = NULL;
	}
}

BOOL CDBMan::CheckOtherServerExist(int GameCode, int ServerCode, const char *strIP, UINT nPort)
{
	if(GameCode==0 || ServerCode==0) return TRUE;

	BOOL bRtn = FALSE;
	TRY
	{	
		if(m_StatusDB.IsOpen()) m_StatusDB.Close();

		CString str;
		str.Format("[GameCode] = %d AND [ServerCode]=%d", GameCode, ServerCode);
		m_StatusDB.m_strFilter = str;
		m_StatusDB.Open();

		if(m_StatusDB.IsEOF()) 
			bRtn = FALSE;
		else {
			if(m_StatusDB.m_ServerIP.Compare(strIP) != 0 
				|| m_StatusDB.m_ServerPort != (int)nPort) bRtn = TRUE;
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("StatusDB에러 : %s", e->m_strError.operator LPCTSTR());
		if(m_StatusDB.IsOpen()) m_StatusDB.Close();
		return FALSE;
	}
	END_CATCH

	m_StatusDB.Close();

	return bRtn;
}

int CDBMan::GetSameConnectCodeNum(int GameCode, int ServerCode)
{
	CString str;
	str.Format("ConnectGameCode = %d AND ConnectServerCode=%d", GameCode, ServerCode);
	
	int totnum=0;

	TRY
	{
		if(m_UserDB.IsOpen()) m_UserDB.Close();
		// 넷마블 회원 DB에서 게임접속 코드 검색
		m_UserDB.m_strFilter = str;
		m_UserDB.Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);

		while(!m_UserDB.IsEOF()) {
			// 다음 레코드로 이동
			m_UserDB.MoveNext();
			totnum++;
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("DB에러 : %s", e->m_strError.operator LPCTSTR());
		if(m_UserDB.IsOpen()) m_UserDB.Close();
		return FALSE;
	}
	END_CATCH

	m_UserDB.Close();
	return totnum;
}

BOOL CDBMan::ClearConnectCode(int GameCode, int ServerCode)
{
	/*
	CString str;
	str.Format("ConnectGameCode = %d AND ConnectServerCode=%d", GameCode, ServerCode);

	TRY
	{
		if(m_UserDB.IsOpen()) m_UserDB.Close();
		// 넷마블 회원 DB에서 게임접속 코드 검색
		m_UserDB.m_strFilter = str;
		m_UserDB.Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);

		while(!m_UserDB.IsEOF()) {
			m_UserDB.Edit();
			// 접속 코드를 초기화
			m_UserDB.m_IsConnected = FALSE;
			m_UserDB.m_ConnectGameCode = 0;
			m_UserDB.m_ConnectServerCode = 0;
			// 회원 DB 업데이트
			m_UserDB.Update();
			// 다음 레코드로 이동
			m_UserDB.MoveNext();
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("DB에러(CDBMan::ClearConnectCode()-Oepn) : %s", e->m_strError.operator LPCTSTR());
		if(m_UserDB.IsOpen()) m_UserDB.Close();
		return FALSE;
	}
	END_CATCH

	m_UserDB.Close();
	*/

	return TRUE;
}

BOOL CDBMan::AddNewBadUserDB(CBadUserDBWorks *pWork) 
{
	if(pWork == NULL) return FALSE;
	if(pWork->MyID.GetLength()==0) return FALSE;
	if(pWork->Contents.GetLength()==0) return FALSE;

	TRY
	{
		// 열린 레코드셋을 닫음
		if(m_BadUserDB.IsOpen()) m_BadUserDB.Close();

		// 컨넥션이 연결되어 있지 않으면 연결한다(재연결시 ODBC설정 대화상자를 띄우지 않게하기 위함)
		if(m_BadUserDB.m_pDatabase && m_BadUserDB.m_pDatabase->IsOpen()==FALSE) {
			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CBadUserDB::m_DSN, (LPCTSTR)CBadUserDB::m_UID, (LPCTSTR)CBadUserDB::m_Pass);
			// DB를 오픈한다(ODBC 설정 대화상자를 띄우지 않음)
			m_BadUserDB.m_pDatabase->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}

		// 추가속도를 빠르게 하기위하여 최소한의 레코드만 오픈
		m_BadUserDB.m_strFilter = "[NO]=0";
		m_BadUserDB.Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);

		if(!m_BadUserDB.IsEOF()) m_BadUserDB.MoveLast();

		// DB를 추가하는 상태
		m_BadUserDB.AddNew();

		// 4000자 이하로 자른다
		int slen = pWork->Contents.GetLength();
		if(slen > 4000) pWork->Contents = pWork->Contents.Mid(slen-4000);

		// DB추가
		m_BadUserDB.m_MyID = pWork->MyID;
		m_BadUserDB.m_TargetID = pWork->TargetID;
		m_BadUserDB.m_WatchID = pWork->WatchID;
		m_BadUserDB.m_GameCode = pWork->GameCode;
		m_BadUserDB.m_ServerCode = pWork->ServerCode;
		m_BadUserDB.m_SubmitReason = pWork->SubmitReason;
		m_BadUserDB.m_SubmitDate = pWork->SubmitDate;
		m_BadUserDB.m_Contents = pWork->Contents;

		m_BadUserDB.Update();

	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("BadUserDB에러 : %s", e->m_strError.operator LPCTSTR());
		if(m_BadUserDB.IsOpen()) m_BadUserDB.Close();
		// 문제가 발생하는 경우 컨넥션을 완전히 끊는다
		if(m_BadUserDB.m_pDatabase && m_BadUserDB.m_pDatabase->IsOpen()) 
			m_BadUserDB.m_pDatabase->Close();
		return FALSE;
	}
	END_CATCH

	m_BadUserDB.Close();
	return TRUE;
}

BOOL CDBMan::InitStatusDB(int GameCode, int ServerCode, const char *strIP, UINT nPort)
{
	if(GameCode==0 || ServerCode==0) return TRUE;

	TRY
	{
		if(!m_StatusDB.IsOpen()) m_StatusDB.Open();
		
		CString str;
		str.Format("[GameCode] = %d AND [ServerCode]=%d", GameCode, ServerCode);
		m_StatusDB.m_strFilter = str;
		m_StatusDB.Requery();

		if(m_StatusDB.IsEOF()) {
			m_StatusDB.AddNew();
			m_StatusDB.m_GameCode = GameCode;
			m_StatusDB.m_ServerCode = ServerCode;
			m_StatusDB.m_UserNum = 0;
			m_StatusDB.m_ServerIP = strIP;
			m_StatusDB.m_ServerPort = nPort;
			m_StatusDB.Update();
		}

		m_StatusDB.m_strFilter = str;
		m_StatusDB.Requery();
		if(m_StatusDB.IsEOF()) {
			CLogFile logfile;
			logfile.Writef("StatusDB에러 : 접속자 인원 현황 DB에서 일치하는 GameCode, ServerCode를 찾지 못하였습니다");
			m_StatusDB.Close();
			return FALSE;
		}

		m_StatusDB.Edit();
		m_StatusDB.m_UserNum = 0;
		m_StatusDB.m_ServerIP = strIP;
		m_StatusDB.m_ServerPort = nPort;
		m_StatusDB.Update();
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("StatusDB에러 : %s", e->m_strError.operator LPCTSTR());
		if(m_StatusDB.IsOpen()) m_StatusDB.Close();
		return FALSE;
	}
	END_CATCH

	m_StatusDB.Close();

	return TRUE;
}

// 로그파일작성 타이틀을 생성한다
CString CDBMan::GetLogFileName()
{
	CString strFileName(_T(""));

	CTime  time = CTime::GetCurrentTime();

	strFileName.Format(g_StrMan.Get(_T("SPOKER_DBERR")),time.GetMonth(),time.GetDay());

	return strFileName;
}
