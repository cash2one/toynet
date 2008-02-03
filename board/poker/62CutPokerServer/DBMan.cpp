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

	// ���� ����DB ���� �ʱ�ȭ
	CUserDB::m_DSN = g_pMainView->Cfg.UserDSN;
	CUserDB::m_Table = g_pMainView->Cfg.UserTable;
	CUserDB::m_UID = g_pMainView->Cfg.UserUID;
	CUserDB::m_Pass = g_pMainView->Cfg.UserPass;
	// ����DB ���� �ʱ�ȭ
	CGameDB::m_DSN = g_pMainView->Cfg.GameDSN;
	CGameDB::m_Table = g_pMainView->Cfg.GameTable;
	CGameDB::m_UID = g_pMainView->Cfg.GameUID;
	CGameDB::m_Pass = g_pMainView->Cfg.GamePass;
	// �ҷ� ���̵� �Ű�DB ���� �ʱ�ȭ
	CBadUserDB::m_DSN = g_pMainView->Cfg.BadUserDSN;
	CBadUserDB::m_Table = g_pMainView->Cfg.BadUserTable;
	CBadUserDB::m_UID = g_pMainView->Cfg.BadUserUID;
	CBadUserDB::m_Pass = g_pMainView->Cfg.BadUserPass;
	// ������ �ο� ��Ȳ DB ���� �ʱ�ȭ
	CStatusDB::m_DSN = g_pMainView->Cfg.StatusDSN;
	CStatusDB::m_Table = g_pMainView->Cfg.StatusTable;
	CStatusDB::m_UID = g_pMainView->Cfg.StatusUID;
	CStatusDB::m_Pass = g_pMainView->Cfg.StatusPass;
	// �ƹ�ŸDB ���� �ʱ�ȭ
	CAvatarDB::m_DSN = g_pMainView->Cfg.AvatarDSN;
	CAvatarDB::m_Table = g_pMainView->Cfg.AvatarTable;
	CAvatarDB::m_UID = g_pMainView->Cfg.AvatarUID;
	CAvatarDB::m_Pass = g_pMainView->Cfg.AvatarPass;
	// ������DB ���� �ʱ�ȭ	[���� ������ �۾�]
	CItemDB::m_DSN = g_pMainView->Cfg.ItemDSN;
	CItemDB::m_Table = g_pMainView->Cfg.ItemTable;
	CItemDB::m_UID = g_pMainView->Cfg.ItemUID;
	CItemDB::m_Pass = g_pMainView->Cfg.ItemPass;
	// �����̾�DB ���� �ʱ�ȭ	[�����̾� �۾�]
	CPremiumDB::m_DSN = g_pMainView->Cfg.PremiumDSN;
	CPremiumDB::m_Table = g_pMainView->Cfg.PremiumTable;
	CPremiumDB::m_UID = g_pMainView->Cfg.PremiumUID;
	CPremiumDB::m_Pass = g_pMainView->Cfg.PremiumPass;
	// �����̾�IP DB ���� �ʱ�ȭ	[PC�� �۾�]
	CPremIPDB::m_DSN = g_pMainView->Cfg.PremIPDSN;
	CPremIPDB::m_Table = g_pMainView->Cfg.PremIPTable;
	CPremIPDB::m_UID = g_pMainView->Cfg.PremIPUID;
	CPremIPDB::m_Pass = g_pMainView->Cfg.PremIPPass;

	// �̺�Ʈ  DB ���� �ʱ�ȭ  ### [�̺�Ʈ Ƽ��] ###
	CEventPrizeDB::m_DSN = g_pMainView->Cfg.EventDSN;
	CEventPrizeDB::m_Table = g_pMainView->Cfg.EventTable;
	CEventPrizeDB::m_UID = g_pMainView->Cfg.EventUID;
	CEventPrizeDB::m_Pass = g_pMainView->Cfg.EventPass;

	TRY
	{		
		
			// ���� �ð��� ���� ��Ű�� ���Ͽ� �ǹ̾��� ������ ��
		m_UserDB.m_strFilter = "[ID]='fdsfsdfdsfd'";
		m_AvatarDB.m_strFilter = "[id]='fdsfsafds'";
		
		m_GameDB.m_strFilter = "[NO]=0";
		m_BadUserDB.m_strFilter = "[NO]=0";
		m_StatusDB.m_strFilter = "[NO]=0";

		m_ItemDB.m_strFilter = "[id]='fdsfsafds'";		// [���� ������ �۾�]
		m_PremiumDB.m_strFilter = "[NO]='fdsfsafds'";	// [�����̾� �۾�]
		m_PremIPDB.m_strFilter = "[ip]='fdsfsafds'";	// [PC�� �۾�]
		m_EventPrizeDB.m_strFilter = "[ID]='ffdfdfdf'"; // ### [�̺�Ʈ Ƽ��] ###

		m_UserDB.Open();
		m_AvatarDB.Open();
		m_GameDB.Open();
		m_BadUserDB.Open();
		m_StatusDB.Open();	
		m_ItemDB.Open();		// [���� ������ �۾�]
		m_PremiumDB.Open();		// [�����̾� �۾�]
		m_PremIPDB.Open();		// [PC�� �۾�]
		
		if( g_pMainView->Cfg.bEventDB ) {	// ### [�̺�Ʈ Ƽ��] ###
			m_EventPrizeDB.Open();	
		}

		m_UserDB.m_pDatabase->SetQueryTimeout(60);
		m_GameDB.m_pDatabase->SetQueryTimeout(60);
		m_BadUserDB.m_pDatabase->SetQueryTimeout(60);
		m_StatusDB.m_pDatabase->SetQueryTimeout(60);
		m_AvatarDB.m_pDatabase->SetQueryTimeout(60);
		m_ItemDB.m_pDatabase->SetQueryTimeout(60);		// [���� ������ �۾�]
		m_PremiumDB.m_pDatabase->SetQueryTimeout(60);	// [�����̾� �۾�]
		m_PremIPDB.m_pDatabase->SetQueryTimeout(60);	// [PC�� �۾�]

		if( g_pMainView->Cfg.bEventDB ) {	// ### [�̺�Ʈ Ƽ��] ###
			m_EventPrizeDB.m_pDatabase->SetQueryTimeout(60);	
		}

	}
	CATCH(CDBException, e)
	{
		CString str;
		str.Format("DB ���� ����: %s", e->m_strError.operator LPCTSTR());
		AfxMessageBox(str);
		
		if(m_UserDB.IsOpen()) m_UserDB.Close();
		if(m_GameDB.IsOpen()) m_GameDB.Close();
		if(m_BadUserDB.IsOpen()) m_BadUserDB.Close();
		if(m_StatusDB.IsOpen()) m_StatusDB.Close();
		if(m_AvatarDB.IsOpen()) m_AvatarDB.Close();
		if(m_ItemDB.IsOpen()) m_ItemDB.Close();			// [���� ������ �۾�]
		if(m_PremiumDB.IsOpen()) m_PremiumDB.Close();	// [�����̾� �۾�]
		if(m_PremIPDB.IsOpen()) m_PremIPDB.Close();		// [PC�� �۾�]
		
		if( g_pMainView->Cfg.bEventDB ) {	// ### [�̺�Ʈ Ƽ��] ###
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
	if(m_ItemDB.IsOpen()) m_ItemDB.Close();			// [���� ������ �۾�]
	if(m_PremiumDB.IsOpen()) m_PremiumDB.Close();	// [�����̾� �۾�]
	if(m_PremIPDB.IsOpen()) m_PremIPDB.Close();		// [PC�� �۾�]

	if( g_pMainView->Cfg.bEventDB ) {	// ### [�̺�Ʈ Ƽ��] ###
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
		if(m_ItemDB.m_pDatabase) {	// [���� ������ �۾�]
			if(m_ItemDB.IsOpen()) m_ItemDB.Close();
			if(m_ItemDB.m_pDatabase->IsOpen()) m_ItemDB.m_pDatabase->Close();
		}
		if(m_PremiumDB.m_pDatabase) {	// [�����̾� �۾�]
			if(m_PremiumDB.IsOpen()) m_PremiumDB.Close();
			if(m_PremiumDB.m_pDatabase->IsOpen()) m_PremiumDB.m_pDatabase->Close();
		}
		if(m_PremIPDB.m_pDatabase) {	// [PC�� �۾�]
			if(m_PremIPDB.IsOpen()) m_PremIPDB.Close();
			if(m_PremIPDB.m_pDatabase->IsOpen()) m_PremIPDB.m_pDatabase->Close();
		}

		if(g_pMainView->Cfg.bEventDB ) { // ### [�̺�Ʈ Ƽ��] ###
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
			if(pDBThread[i]->CreateItemDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }		// [���� ������ �۾�]
			if(pDBThread[i]->CreatePremiumDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }	// [�����̾� �۾�]
			if(pDBThread[i]->CreatePremIPDB()==FALSE) { bFail=TRUE; bRtn=FALSE; }	// [PC�� �۾�]

			if( g_pMainView->Cfg.bEventDB ) {	// ### [�̺�Ʈ Ƽ��] ###
				if(pDBThread[i]->CreateEventPrizeDB()==FALSE) {bFail=TRUE; bRtn=FALSE;}	
			}

			if(bFail) { pDBThread[i]->DeleteAllDB(); continue; }

			// ȯ�� �������� DBŬ���� �ɼ��� �����Ǿ� �ִٸ� ���µ� DB�� ����
			if(g_pMainView->Cfg2.bDBClose) pDBThread[i]->DeleteAllDB();

			// DB�����带 ����(�ʱ⿡�� �ƹ��ϵ� ���� ����)
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
			// ������ ���� ���� ����
			return pDBThread[i]->Begin(&DBQue);
		}
		else if(!pDBThread[i]->bNowTerminate && pDBThread[i]->bWorking==FALSE) {
			// DB �۾� �Ϸ��� Suspend���� �ִ� �������� �簡��
			return pDBThread[i]->Begin(&DBQue);
		}
	}

	// ��� �����尡 Ǯ �����ϴ� ��� ó���� ���� �ٸ� �����忡�� �ڵ����� ó���Ұ���
	return TRUE;
}

BOOL CDBMan::DiscardDBThread(int tnum)
{
	if(tnum<0 || tnum>TotThreadNum) return FALSE;
	if(pDBThread[tnum] == NULL) return FALSE;

	// DB ������ ���� �÷��׸� �����Ѵ�
	pDBThread[tnum]->bNowTerminate = TRUE;

	// �α� ���
	CString strFileName = GetLogFileName();
	if(strFileName!=_T("")){
		CLogFile logfile(strFileName.operator LPCTSTR());
		logfile.Writef(_T("Throw DB thread :  : (ID=[%s], UNum=%d,  Win=%d, Fail=%d,  PMoney����ġ=%I64d, RSF=%d, SF=%d, FCARD=%d)"),
			pDBThread[tnum]->Work.UI.ID, pDBThread[tnum]->Work.UI.UNum, 
			//pDBThread[tnum]->Work.UI.WinNum, pDBThread[tnum]->Work.UI.LooseNum, pDBThread[tnum]->Work.UI.DrawNum, 
			pDBThread[tnum]->Work.UI.WinNum, pDBThread[tnum]->Work.UI.LooseNum, 
			(INT64)Max(pDBThread[tnum]->Work.UI.PMoney,0)-pDBThread[tnum]->Work.OI.pBackup,
			pDBThread[tnum]->Work.UI.PI.nRSF, pDBThread[tnum]->Work.UI.PI.nSF, pDBThread[tnum]->Work.UI.PI.nFCARD);
	}

/*
	// �α� ���
	CLogFile logfile;
	logfile.Writef("DB ������ ���� : ID=%s, Marble=%d, Win=%d, Loose=%d, Point=%d",
		pDBThread[tnum]->Work.UI.ID, 
		pDBThread[tnum]->Work.UI.Marble, 
		pDBThread[tnum]->Work.UI.WinNum, 
		pDBThread[tnum]->Work.UI.LooseNum, 
		pDBThread[tnum]->Work.UI.Point);
*/
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
		// �ùٸ��� ����� ������� �޸𸮸� �����Ѵ�
		if(pDBThread[i]->pThread == NULL && pDBThread[i]->bWorking == FALSE) {
			delete pDBThread[i];
			pDBThread[i] = NULL;
			continue;
		}
		// �ð��ʰ��� ������� ������?
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
			// ������ ����
			pDBThread[i]->Terminate();
		}
		// DB ������ Ŭ���� ����
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
		logfile.Writef("StatusDB���� : %s", e->m_strError.operator LPCTSTR());
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
		// �ݸ��� ȸ�� DB���� �������� �ڵ� �˻�
		m_UserDB.m_strFilter = str;
		m_UserDB.Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);

		while(!m_UserDB.IsEOF()) {
			// ���� ���ڵ�� �̵�
			m_UserDB.MoveNext();
			totnum++;
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("DB���� : %s", e->m_strError.operator LPCTSTR());
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
		// �ݸ��� ȸ�� DB���� �������� �ڵ� �˻�
		m_UserDB.m_strFilter = str;
		m_UserDB.Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);

		while(!m_UserDB.IsEOF()) {
			m_UserDB.Edit();
			// ���� �ڵ带 �ʱ�ȭ
			m_UserDB.m_IsConnected = FALSE;
			m_UserDB.m_ConnectGameCode = 0;
			m_UserDB.m_ConnectServerCode = 0;
			// ȸ�� DB ������Ʈ
			m_UserDB.Update();
			// ���� ���ڵ�� �̵�
			m_UserDB.MoveNext();
		}
	}
	CATCH(CDBException, e)
	{
		CLogFile logfile;
		logfile.Writef("DB����(CDBMan::ClearConnectCode()-Oepn) : %s", e->m_strError.operator LPCTSTR());
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
		// ���� ���ڵ���� ����
		if(m_BadUserDB.IsOpen()) m_BadUserDB.Close();

		// ���ؼ��� ����Ǿ� ���� ������ �����Ѵ�(�翬��� ODBC���� ��ȭ���ڸ� ����� �ʰ��ϱ� ����)
		if(m_BadUserDB.m_pDatabase && m_BadUserDB.m_pDatabase->IsOpen()==FALSE) {
			CString strConnect;
			strConnect.Format("DSN=%s;UID=%s;PWD=%s;", (LPCTSTR)CBadUserDB::m_DSN, (LPCTSTR)CBadUserDB::m_UID, (LPCTSTR)CBadUserDB::m_Pass);
			// DB�� �����Ѵ�(ODBC ���� ��ȭ���ڸ� ����� ����)
			m_BadUserDB.m_pDatabase->OpenEx(strConnect, CDatabase::noOdbcDialog | CDatabase::useCursorLib);
		}

		// �߰��ӵ��� ������ �ϱ����Ͽ� �ּ����� ���ڵ常 ����
		m_BadUserDB.m_strFilter = "[NO]=0";
		m_BadUserDB.Open(CRecordset::snapshot, NULL, CRecordset::executeDirect);

		if(!m_BadUserDB.IsEOF()) m_BadUserDB.MoveLast();

		// DB�� �߰��ϴ� ����
		m_BadUserDB.AddNew();

		// 4000�� ���Ϸ� �ڸ���
		int slen = pWork->Contents.GetLength();
		if(slen > 4000) pWork->Contents = pWork->Contents.Mid(slen-4000);

		// DB�߰�
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
		logfile.Writef("BadUserDB���� : %s", e->m_strError.operator LPCTSTR());
		if(m_BadUserDB.IsOpen()) m_BadUserDB.Close();
		// ������ �߻��ϴ� ��� ���ؼ��� ������ ���´�
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
			logfile.Writef("StatusDB���� : ������ �ο� ��Ȳ DB���� ��ġ�ϴ� GameCode, ServerCode�� ã�� ���Ͽ����ϴ�");
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
		logfile.Writef("StatusDB���� : %s", e->m_strError.operator LPCTSTR());
		if(m_StatusDB.IsOpen()) m_StatusDB.Close();
		return FALSE;
	}
	END_CATCH

	m_StatusDB.Close();

	return TRUE;
}

// �α������ۼ� Ÿ��Ʋ�� �����Ѵ�
CString CDBMan::GetLogFileName()
{
	CString strFileName(_T(""));

	CTime  time = CTime::GetCurrentTime();

	strFileName.Format(g_StrMan.Get(_T("SPOKER_DBERR")),time.GetMonth(),time.GetDay());

	return strFileName;
}
