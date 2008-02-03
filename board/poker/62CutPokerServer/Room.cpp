// Room.cpp: implementation of the CRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "Room.h"
#include "62CutPokerServerView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// ### [�̺�Ʈ Ƽ��] ###
#define EVENT_GAMECOUNT 30 // 30�Ǽ�
#define EVENT_GIVECOIN  3  // 2�� ����


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRoom::CRoom()
{
	pChan = NULL;
//	SndBuf = NULL;
	
	// 5, 7�ο� �۾�
	m_Max_Player	= MAX_PLAYER - 1;
	m_Max_NewPlayer = m_Max_Player + MAX_OBSERVER;

	Clear();
}

CRoom::~CRoom()
{

}

// 5,7 �ο� ����
void CRoom::set_player_usernum(int channel_usernum)
{
	if(channel_usernum==0) { // �游��� �켱
		if(Ri.bPlayer==0 ) { // 5�ο�
			m_Max_Player	= MAX_PLAYER - 1;
			m_Max_NewPlayer = m_Max_Player + MAX_OBSERVER;
		} else {			// 6�ο�
			m_Max_Player	= MAX_PLAYER;
			m_Max_NewPlayer = m_Max_Player + MAX_OBSERVER;
		}
	} else if(channel_usernum==1)  // ä�� ���� ���� 5�ο�
	{
		m_Max_Player	= MAX_PLAYER - 1;
		m_Max_NewPlayer = m_Max_Player + MAX_OBSERVER;
	} else {						// ä�� ���� ���� 6�ο�

		m_Max_Player	= MAX_PLAYER;
		m_Max_NewPlayer = m_Max_Player + MAX_OBSERVER;
	}
}


void CRoom::Clear()
{
	bValid = FALSE;

	ZeroMemory(&Ri, sizeof(Ri));
	ZeroMemory(Ui, sizeof(Ui));
	ZeroMemory(Ps, sizeof(Ps));
	ZeroMemory(Ps2, sizeof(Ps2));
	ZeroMemory(Bc, sizeof(Bc));
	ZeroMemory(&m_SRi, sizeof(m_SRi));
	ZeroMemory(Us, sizeof(Us));	// [���� ������ �۾�]
	bSuperRoomDestory = FALSE;	// [���� ������ �۾�]
	BanUserList.Clear();		// [���� ������ �۾�]
	VoteBanUserList.Clear();		
					

	ZeroMemory(m_DealerID, sizeof(m_DealerID));
	//ZeroMemory(m_PlayerChangeCradDeck, sizeof(m_PlayerChangeCradDeck));

	// ### [ ������� ] ###
	bNowUserEmptyDestory = FALSE;
	ReservedRemoveAll(); //��������
	ObserverRemoveAll();
	m_AdminCount = 0;	// ���� ������ ������ ��


	////// ���� ���� ���� ������
	StartUserNum = 0;
	bRestartCnt=FALSE;
	RestartCnt=0;
	bWaitGameOver = FALSE;
	WaitGameOverCnt = 0;
	m_JackPotActive = 0;

	GameState = RSTATE_NONE;
	m_CurPlayer = -1;
	m_GameState = GAME_ROUND_READY;// 0: 1:ī�� 3��, 2: ī�� ����, 3: 4��, 4: 5��, 5: 6��, 6: ����, 7: �°���

	////// �߹� ��ǥ ���� ������
	BanishVoteState = 0;
	BanishVoteCnt = 0;
	BanishVoteAgreeNum = 0;
	BanishVoteDisgreeNum = 0;
	ZeroMemory(&BVI, sizeof(BANISHVOTEINFO));
	
	// ī�� �迭
	ZeroMemory(m_CardDeck, sizeof(m_CardDeck));
	m_CurCardIndex = 0;
	m_nPrimeUser = 0;
	ResetGame();
	
	// ### [ �ߺ��� IP�� ] ###
	m_nIpCnt = 0;
	ZeroMemory(m_aIpIndex, sizeof(m_aIpIndex));
	for(int i=0; i<m_Max_Player; i++) {
		Ps[i].nCheckIp = -1; // $$$  �⺻�� -1
	}
}

int CRoom::GetPNum(char* id)
{
	if(id==NULL) return -1;
	for(int i=0; i<m_Max_NewPlayer; i++)// ### [ ������� ] ### 
	{
		if(strlen(Ui[i].ID) > 0)
		{
			if(faststrcmp(Ui[i].ID, id)==0) return i;
		}
	}
	return -1;
}


SMALLROOMINFO & CRoom::GetSmallRoomInfo()
{
//	SMALLROOMINFO sri;

	ZeroMemory(&m_SRi, sizeof(m_SRi));

	m_SRi.RoomNum = Ri.RoomNum;
	m_SRi.UNum = Ri.UNum;
	strncpy(m_SRi.ID, Ri.ID, 15);
	m_SRi.NowUserNum = Ri.NowUserNum;
	m_SRi.MaxUserNum = Ri.MaxUserNum;
	m_SRi.State = Ri.State;
	m_SRi.GameKind = Ri.GameKind;
	m_SRi.NowObserverNum = Ri.NowObserverNum; // ### [ ������� ] ### 

	// [ ###����### ]
	m_SRi.JackPotMoney = g_JackPotMoney; // (�κ� ����)������ ����� ���� ������.

	return m_SRi;
}

const ENTERROOMINFO CRoom::GetEnterRoomInfo()
{
	ENTERROOMINFO ei;
	memcpy(&ei.Ri, &Ri, sizeof(Ri));
	memcpy(ei.Ui, Ui, sizeof(Ui));
	POKERGAME Pg = MakeInfo();
	// ���߿� ���´� (���� �����ش�)
	if(Ri.State == 1)
		Pg.CurPlayer = m_nPrimeUser;
	else
	{
		Pg.CurPlayer = GetPNum(m_DealerID);
	}
	memcpy(&ei.Pg, &Pg, sizeof(Pg));

	return ei;
}


BOOL CRoom::SendMsgTo(char* id, char *pMsgData, int nMsgSize)
{
	if(id==NULL) return FALSE;
	if(strlen(id)==0) return FALSE;
	for(int i=0; i<m_Max_NewPlayer; i++) { // ### [ ������� ] ###
		if(strlen(Ui[i].ID) != 0) {
			if(Ui[i].UNum<1 || Ui[i].UNum>=MAX_USER) continue;
			if(faststrcmp(id, Ui[i].ID) != 0) continue;
			if(faststrcmp(Ui[i].ID, User[Ui[i].UNum].UI.ID) == 0)
			{				
				int sid = User[Ui[i].UNum].Sid;
				SockMan.SendData(sid, pMsgData, nMsgSize);
				return TRUE;
			}
		}
	}
	return TRUE;
}


BOOL CRoom::SendMsgTo(int pnum, char *pMsgData, int nMsgSize)
{
	if(pnum < 0 || pnum >= m_Max_NewPlayer) return FALSE;  // ### [ ������� ] ###
	if(strlen(Ui[pnum].ID) != 0) {
		if(Ui[pnum].UNum<1 || Ui[pnum].UNum>=MAX_USER) return FALSE;
		if(faststrcmp(Ui[pnum].ID, User[Ui[pnum].UNum].UI.ID) == 0) {				
			int sid = User[Ui[pnum].UNum].Sid;
			SockMan.SendData(sid, pMsgData, nMsgSize);
			return TRUE;
		}
		else return FALSE;
	}
	return TRUE;
}

BOOL CRoom::SendMsgExept(char* id, char *pMsgData, int nMsgSize)
{
	if(id==NULL) return FALSE;
	if(strlen(id)==0) return FALSE;
	for(int i=0; i<m_Max_NewPlayer; i++) {// ### [ ������� ] ###
		if(strlen(Ui[i].ID) != 0) {
			if(Ui[i].UNum<1 || Ui[i].UNum>=MAX_USER) continue;
			if(faststrcmp(id, Ui[i].ID) == 0) continue;
			if(faststrcmp(Ui[i].ID, User[Ui[i].UNum].UI.ID) != 0) continue;
			int sid = User[Ui[i].UNum].Sid;
			SockMan.SendData(sid, pMsgData, nMsgSize);
		}
	}
	return TRUE;
}

BOOL CRoom::SendMsgExept(int pnum, char *pMsgData, int nMsgSize)
{
	if(pnum<0 || pnum>=m_Max_NewPlayer) return FALSE; // ### [ ������� ] ###
	for(int i=0; i<m_Max_NewPlayer; i++) {
		if(i == pnum) continue;
		if(strlen(Ui[i].ID) > 0) {
			if(Ui[i].UNum<1 || Ui[i].UNum>=MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[Ui[i].UNum].UI.ID) != 0) continue;
			int sid = User[Ui[i].UNum].Sid;
			SockMan.SendData(sid, pMsgData, nMsgSize);
		}
	}
	return TRUE;
}

BOOL CRoom::SendMsgToAll(char *pMsgData, int nMsgSize)
{
	for(int i=0; i<m_Max_NewPlayer; i++) { // ### [ ������� ] ###
		if(strlen(Ui[i].ID) > 0) {
			if(Ui[i].UNum<1 || Ui[i].UNum>=MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[Ui[i].UNum].UI.ID) != 0) continue;
			int sid = User[Ui[i].UNum].Sid;
			SockMan.SendData(sid, pMsgData, nMsgSize);
		}
	}
	return TRUE;
}

// ### [ ������� ] ### // ### [ �ߺ��� IP�� ] ### // ### [���� �߰� �۾�] ###
BOOL CRoom::OnUserEnterRoom(USERINFO *pUI, int pnum, int myindex, int sndFxKind)
{
	if(pUI == NULL) return FALSE;
	if(pUI->UNum < 1 || pUI->UNum >= MAX_USER) return FALSE;
	if(pnum<0 || pnum>=m_Max_NewPlayer) return FALSE;
	if(strlen(Ui[pnum].ID) > 0) return FALSE;	

	if(pnum < m_Max_Player) {
		if(Ri.NowUserNum+1 > Ri.MaxUserNum) return FALSE;
		if(pnum < 0 || pnum > Ri.MaxUserNum) return FALSE;
		// �� ���� ����
		Ri.NowUserNum++;
	}
	else {
		if( pnum != m_Max_NewPlayer-1 ) Ri.NowObserverNum++;
		else m_AdminCount++; // ���� ������ ������ ��
		User[pUI->UNum].bObserver = TRUE;
		User[pUI->UNum].UI.cObserver = 1;
		pUI->cObserver = 1;
	}
	
	memcpy(&Ui[pnum], pUI, sizeof(USERINFO));// ����� ������ ���� ������ �����Ѵ�
	User[pUI->UNum].PNum = pnum;			 // �÷��̾� ��ȣ �ʱ�ȭ
	// ### [�α� ��Ͽ�]###
	User[pUI->UNum].nBet = 0;				// ����� ���þ� �ʱ�ȭ

	// ���������� Ŭ����
	if(pnum < m_Max_Player) {
		ZeroMemory(&Ps[pnum], sizeof(PLAYSTATE));
		ZeroMemory(&Bc[pnum], sizeof(BACKCARD));
		Ps[pnum].nSndFxKind = sndFxKind; // ### [���� �߰� �۾�] ###
		Ps[pnum].nCheckIp = myindex;	 // ### [ �ߺ��� IP�� ] ###
	}
	ZeroMemory(&Ps2[pnum], sizeof(PLAYSTATE2)); // �ݺ�Ƚ�� �߰��� ���Ͽ�~
	IPARRINDEX iparray; 
	ZeroMemory(&iparray, sizeof(iparray));
	for(int i=0;i<m_Max_Player;i++) {	iparray.IpIndex[i]= (char)Ps[i].nCheckIp; }

	int rnum = Ri.RoomNum;
	// ���� ���� �ٸ� ����ڿ��� ���ο� ������� ������ �뺸
	CSV_USERENTERROOM uermsg(SndBuf);
	uermsg.Set(rnum, pUI, &iparray, sndFxKind); // ### [ �ߺ��� IP�� ] ### // ### [���� �߰� �۾�] ###
	SendMsgExept(pnum, uermsg.pData, uermsg.GetTotalSize());

	// ���� ���� ī���� ���� (���۹�ư ����)
	
	if(GameState==RSTATE_NONE && bRestartCnt==FALSE && Ri.NowUserNum == Ri.MaxUserNum)
	{
		bRestartCnt=TRUE;
		RestartCnt=3;	
		return TRUE;
	}
	else if(GameState==RSTATE_NONE && bRestartCnt==FALSE && Ri.NowUserNum >= MIN_PLAYER && Ri.NowUserNum < Ri.MaxUserNum)
	{
		bRestartCnt=TRUE;
		RestartCnt=0;
		// ���۹�ư 
		int nPrimer    = GetPrimer(); // �����ϱ�
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,1);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}	
	return TRUE;
}


// ### [ ������� ] ### 
BOOL CRoom::OnUserOutRoom(int unum, char *id, BOOL bDisconnect)
{
	int rnum = Ri.RoomNum;

	if(unum<1 || unum >= MAX_USER) {
		CLogFile logfile;
		logfile.Writef("OnUserOutRoom() - Wrong UNum(rnum=%d, unum=%d)", rnum, unum);
		return FALSE;
	}

	if(faststrcmp(User[unum].UI.ID, id) != 0) {
		CLogFile logfile;
		logfile.Writef("OnUserOutRoom() - Wrong UserID(rnum=%d, unum=%d, ID=%s)", rnum, unum, User[unum].UI.ID);
		return FALSE;
	}

	if(User[unum].UI.UNum != unum) {
		CLogFile logfile;
		logfile.Writef("OnUserOutRoom() - Not same UNum (ID=%s, unum=%d, UNum=%d)", User[unum].UI.ID, unum, User[unum].UI.UNum);
		return FALSE;
	}

	// ������� �÷��� ��ȣ�� ���Ѵ�
	int pnum = GetPNum(id);
	if(pnum == -1) {
		CLogFile logfile;
		logfile.Writef("OnUserOutRoom() - Failed out room user (unum=%d, ID=%s)", unum, User[unum].UI.ID);
		return FALSE;
	}

	// �������̴� ������̸�, ������ �����Ų�� // ### [ ������� ] ### 
	if(pnum < m_Max_Player && Ri.State!=0 && Ps[pnum].bOnGame && !Ps[pnum].bFold) {
		FoldUser(pnum, TRUE);
	}	
	
	if(User[unum].UI.PMoney < Ri.BetMoney)//������ ��û�� �����̸� �Ļ� �޼��� ����
	{
		CSV_SERVERMSG smsg(SndBuf);
		CString strMess = "";
		smsg.Set(9, (char*)strMess.operator LPCTSTR()); // ## �Ļ�
		User[unum].SendMsg(&smsg);	
	}

	// ���� ���ᰡ �ƴϸ� �濡�� ������ �㰡
	if(bDisconnect == FALSE) {
		CSV_ACCEPT_OUTROOM aormsg(SndBuf);
		aormsg.Set();
		SockMan.SendData(User[unum].Sid, aormsg.pData, aormsg.GetTotalSize());
	}		

	if(GameState == RSTATE_CHANGECARD)//[62]
	{	
		//62cut �÷��� �ʱ�ȭ[62]
		Ps2[pnum].nChangeFlg = FALSE;
//		Ps2[pnum].nChangeWinEndFlg = FALSE;
		// �ٲٱ� �϶� ���̽� üũ
		ChangeCardRaiseCheck();	//[62]
	}


	// �߹� ����/�������̸�
	if(pnum < m_Max_Player && BanishVoteState) {// ### [ ������� ] ### 
		if(faststrcmp(Ui[pnum].ID, BVI.MyID)==0 || faststrcmp(Ui[pnum].ID, BVI.TarID)==0) {
			// ����ڿ��� �����޼����� ����
			CSV_SERVERMSG smsg(SndBuf);
			if(BanishVoteState==1) smsg.Set(0, "�߹� ��ǥ ������ ��ҵǾ����ϴ�");
			else smsg.Set(0, "�߹� ��ǥ�� ��ҵǾ����ϴ�");
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			// �߹� ��û�ڳ� ����ڰ� ����ϴ� ��� �߹� ���
			BanishVoteState = 0;
			ZeroMemory(&BVI, sizeof(BANISHVOTEINFO));
		}
		else if(BanishVoteState==2 && Ps2[pnum].bJoinBanishVote) {
			int totjoin = 0;
			for(int i=0; i<m_Max_Player; i++) if(Ps2[i].bJoinBanishVote) totjoin++;
			if(totjoin-1 < BANISHVOTE_NUM) {
				// ����ڿ��� �����޼����� ����
				CSV_SERVERMSG smsg(SndBuf);
				smsg.Set(0, "��ǥ �ο��� 3�� �̸��̹Ƿ� �߹� ��ǥ�� ��ҵǾ����ϴ�");
				SendMsgToAll(smsg.pData, smsg.GetTotalSize());

				// ��ǥ�� �������� ��Ȳ���� ���� ��ǥ ���� �ο��� 4�� ���ϸ� ��ǥ ���
				BanishVoteState = 0;
				ZeroMemory(&BVI, sizeof(BANISHVOTEINFO));
			}
		}
	}

	LogRecordFunc(unum, pnum);


	// ### [ �ߺ��� IP�� ] ###
	if(pnum < m_Max_Player && Ps[pnum].nCheckIp >= 0) // ### [ ������� ] ### 
	{
		int ipindex= Ps[pnum].nCheckIp;
		int indexCnt=0;
		for(int i=0;i<m_Max_Player;i++) {
			if(pnum==i) continue;
			if(ipindex == Ps[i].nCheckIp) indexCnt++;
		}
		// ����
		if(indexCnt == 1)	
		{
			for(i=0;i<m_Max_Player;i++) {
				if(pnum==i) continue;
				if(ipindex == Ps[i].nCheckIp) Ps[i].nCheckIp = -1; // ��� ����
			}

			m_aIpIndex[ipindex] = 0;
			m_nIpCnt--;
		}
	}

	// ���� ������ �����ϰ� �����������¸� Ŭ����
	if(pnum < m_Max_Player) {
		ZeroMemory(&Ps[pnum], sizeof(PLAYSTATE));
		ZeroMemory(&Bc[pnum], sizeof(BACKCARD));
		Ps[pnum].nCheckIp = -1; // ### [ �ߺ��� IP�� ] ### $$$ �⺻�� -1
	}
	// ### [ ������� ] ### 
	Cancel_a_Observer(User[unum].UI.ID);//����->���� �������	
	Cancel_a_Reservation(User[unum].UI.ID);//����->���� �������	

	ZeroMemory(&Ps2[pnum], sizeof(PLAYSTATE2));
	ZeroMemory(&Us[pnum], sizeof(USERSTATE));	// [���� ������ �۾�]
	ZeroMemory(&Ui[pnum], sizeof(USERINFO));	// ����� ����
	User[unum].bExitSubscript = FALSE;			// ������ ������� �ʱ�ȭ	

	// �����ο����� �Ѹ� ����
	if(User[unum].bObserver) {
		if( !User[unum].bAdminUser ) Ri.NowObserverNum--;
		else m_AdminCount--; // ���� ������ ������ ��
	}
	else
		Ri.NowUserNum--;


	///���ο��� äũ MIN_PLAYER �� ���� ������ ���� ī��Ʈ ���� (���۹�ư)
	if(Ri.NowUserNum < MIN_PLAYER)
	{
		bRestartCnt = FALSE;
		RestartCnt  = 0;

		// ���۹�ư 
		int nPrimer    = GetPrimer(); // �����ϱ�
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,0);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}

	// 5���̿��ٰ� �Ѹ��� ������ ���۹�ư Ȱ��ȭ ��Ų��.cnt = 0���� �ʱ� 
	if(GameState==RSTATE_NONE && Ri.NowUserNum >= MIN_PLAYER && Ri.NowUserNum < Ri.MaxUserNum) 
	{
		bRestartCnt = TRUE;
		RestartCnt  = 0;

		// ���۹�ư 
		int nPrimer    = GetPrimer(); // �����ϱ�
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,1);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}

	// �濡 ���� ����ڰ� ������ ���� �����Ѵ�
	if(Ri.NowUserNum<=0 && Ri.NowObserverNum <=0 && m_AdminCount==0) // ### [ ������� ] ### 
	{
		//Admin_Room_Remove(); // ���� �����ڰ� ������ ������ ��
		Ri.NowUserNum=0;
		// ��� ����ڿ��� �������� �����Ǿ����� �˸�
		CSV_OUTROOM ormsg(SndBuf);
	//	SMALLROOMINFO sri;
	//	sri = GetSmallRoomInfo();
		ormsg.Set(&GetSmallRoomInfo());
		pChan->SendMsgToLoby(ormsg.pData, ormsg.GetTotalSize());
		// �� ����
		Clear();
	}
	else
	{
		// ### [ �ߺ��� IP�� ] ###
		IPARRINDEX iparray;
		ZeroMemory(&iparray, sizeof(iparray));
		for( int i=0;i<m_Max_Player;i++)
			iparray.IpIndex[i]= (char)Ps[i].nCheckIp;

		// ���� �� �÷��̾�� ���������� �˸���
		CSV_USEROUTROOM uomsg(SndBuf);
		uomsg.Set(rnum, unum, User[unum].UI.ID, &iparray);
		SendMsgToAll(uomsg.pData, uomsg.GetTotalSize());
		
		// ���� ������ ���� ����� ������ ������� �뺸�Ѵ�.
		if(faststrcmp(Ri.ID, User[unum].UI.ID) == 0) {
			int mastnum = -1;
			for(int i=0; i<m_Max_NewPlayer; i++)
				if(strlen(Ui[i].ID) != 0) { mastnum = i; break; }

			if(mastnum != -1) {
				// ### [ ������� ] ###
				//������̳� �� ���� ��Ų��
				strncpy(Ri.ID, Ui[mastnum].ID, 15);
				Ri.UNum = Ui[mastnum].UNum;
				// ������ �ٲ��� �˸���
				CSV_CHANGEROOMMASTER crmmsg(SndBuf);
				crmmsg.Set(rnum, Ui[mastnum].UNum, Ui[mastnum].ID);
				SendMsgToAll(crmmsg.pData, crmmsg.GetTotalSize());
				// [������Ŀ] ī�彺��Ī
				Ri.nCard = Ui[mastnum].PI.nCard;
			}

			// [���� ������ �۾�]
			if(Ri.bSuperMaster) {
				// ���۹��̶�� ����
				if(!bSuperRoomDestory) {
					bSuperRoomDestory = TRUE;

					if(Ri.State != 0) {
						// �������̶�� ���� ���� �޽��� �˸�
						CSV_SERVERMSG smsg(SndBuf);
						smsg.Set(0, "���۹����� �����Ͽ����Ƿ� �̹� ������ ������ ��� �ڵ����� �����ϰ� �˴ϴ�.");
						SendMsgToAll(smsg.pData, smsg.GetTotalSize());
					}
				}
			}
			// ### [ ������� ] ### 
			else if(Ri.NowUserNum == 0)	{
				bNowUserEmptyDestory = TRUE;// �������̶�� ����
			}
		}

		// ���� �����ٸ� 
		if(faststrcmp(m_DealerID, User[unum].UI.ID) == 0) {
			strncpy(m_DealerID, Ri.ID, 15);
		}

		// ���� �ο��� 1���� ���ų� ���ٸ� ������ �ߴܵ�
		if(Ri.NowUserNum <= 1) {
			BOOL bOpen = FALSE;
			if(CheckGameOver(bOpen)) DoGameOver(bOpen);
		}

		// ��� ����ڿ��� �������� ����Ǿ����� �˸�
		CSV_CHANGEROOM crmsg(SndBuf);
	//	SMALLROOMINFO sri;
	//	sri = GetSmallRoomInfo();
		crmsg.Set(&GetSmallRoomInfo());
		pChan->SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());
	}

	User[unum].PNum = -1;	// �÷��̾� ��ȣ �ʱ�ȭ
	// ### [�α� ��Ͽ�]###
	User[unum].nBet = 0;	// ����� ���þ� �ʱ�ȭ

	// ������� ��ġ�� �������
	User[unum].SUI.Position = WH_LOBY;				// ���� ��ġ�� �����̴�
	User[unum].SUI.RoomNum = 0;						// ���� ��ġ�� ���ȣ(�����Ⱚ)
	User[unum].bLobyOpen = FALSE;
	// ### [ ������� ] ### 
	User[unum].UI.cObserver = 0;
	User[unum].bObserver = FALSE;
	// �������� ���
	int level = GetPokerLevel(User[unum].UI.PMoney);
	User[unum].UI.nIcon = level;
	User[unum].SUI.nIcon = level;

	// ���� ���ᰡ �ƴϸ� ����� ���� ������ �˸�
	if(bDisconnect == FALSE) {
		CSV_CHANGEUSER cumsg(SndBuf);
		cumsg.Set(&User[unum].SUI); // [ ���� ] ����
		pChan->SendMsgToLobyOpen(cumsg.pData, cumsg.GetTotalSize());
	}
	
	// ### [ ������� ] ### 
	Reserve_takeaSeat(); // �ڸ� �ɱ�
	return TRUE;
}

// [���� ������ �۾�]
void CRoom::OnForceBanish(CSV_ASK_FORCEBANISH *pMsg)
{
	// ���۹��� �ƴϸ� ���
	if(!Ri.bSuperMaster) return;

	// �������� �ʴ� ����ڸ� ���
	int tpnum = GetPNum(pMsg->ID);
	if(tpnum == -1) return;


	if(Ri.State == 0) {
		// ������̶�� ��� ���� ����
		DoForceBanish(tpnum);
	}
	else {
		// �������̸� ���� ����
		if(!Us[tpnum].bForceBanish) {
			
			Us[tpnum].bForceBanish = TRUE;

			CString str;
			str.Format("[%s]�� �����߹� ����", Ui[tpnum].ID);
			// ����ڿ��� �����޼����� ����
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(0, (char*)(LPCTSTR)str);
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		}

		return;
	}
}

// [���� ������ �۾�]
void CRoom::DoForceBanish(int pnum)	
{
	//if(!Us[pnum].bForceBanish) return;
	if(strlen(Ui[pnum].ID) == 0) return;

	int tarunum = Ui[pnum].UNum;
	if(tarunum<1 || tarunum>=MAX_USER) return;
	if(strcmp(User[tarunum].UI.ID, Ui[pnum].ID) != 0) return;

	// ������ ����Ʈ�� �߰�
	BanUserList.AddID(User[tarunum].UI.ID);

	CString str;
	str.Format("[%s]���� �����߹��մϴ�.", User[tarunum].UI.ID);
	CSV_SERVERMSG smsg(SndBuf);
	smsg.Set(0, (char*)str.operator LPCTSTR());
	SendMsgToAll(smsg.pData, smsg.GetTotalSize());
	
	// ����ڿ��� �����޼����� ����
	CSV_SERVERMSG smsg2(SndBuf);
	smsg2.Set(1, "���� ���忡 ���Ͽ� ���� �߹�Ǿ����ϴ�");
	User[tarunum].SendMsg(&smsg2);
	
//	RestartCnt = 0;//����  �߹����� ���� ī��Ʈ�� ���� �ȴ�.
	// ���� �߹�
	OnUserOutRoom(tarunum, User[tarunum].UI.ID, FALSE);
}


////////////////////////////////////////////////////////////////////////////////////////

// ### [ ������� ] ### 
void CRoom::OnGlobalGameTimer()
{
	// �������� �ʴ� �÷��̾��� ��� ���� ����
	for(int i=0; i<m_Max_NewPlayer; i++)// ### [ ������� ] ### 
	{
		if(*Ui[i].ID != 0)
		{
			int unum = Ui[i].UNum;
			BOOL bOut = FALSE;
			if(unum<1 || unum>=MAX_USER) bOut = TRUE;
			if(bOut==FALSE) {
				if(strlen(User[unum].UI.ID)==0) bOut=TRUE;
				else if(faststrcmp(Ui[i].ID, User[unum].UI.ID) !=0) bOut=TRUE;
			}
			
			if(bOut) {
				if(OnUserOutRoom(unum, Ui[i].ID, FALSE)==FALSE) {
					CLogFile logfile;
					logfile.Writef("����~~~~ CRoom::OnGlobalGameTimer()");
					
					if(i<m_Max_Player) {
						if(Ri.NowUserNum>0) Ri.NowUserNum--;
					}
					else
					{
						if(Ri.NowObserverNum>0) Ri.NowObserverNum--;
					}

					ZeroMemory(&Ui[i], sizeof(USERINFO));
					
					IPARRINDEX iparray;
					ZeroMemory(&iparray, sizeof(iparray));
					if(i<m_Max_Player)
					{
						ZeroMemory(&Ps[i], sizeof(PLAYSTATE));
						ZeroMemory(&Bc[i],  sizeof(BACKCARD));
						// ### [ �ߺ��� IP�� ] ###
						Ps[i].nCheckIp = -1;
						for( int kk=0;kk<m_Max_Player;kk++)
							iparray.IpIndex[kk]= (char)Ps[kk].nCheckIp;
					}
					ZeroMemory(&Ps2[i], sizeof(PLAYSTATE2));
					ZeroMemory(&Us[i], sizeof(USERSTATE));	// [���� ������ �۾�]
					
					// ���� �� �÷��̾�� ���������� �˸���
					CSV_USEROUTROOM uomsg(SndBuf);
					uomsg.Set(Ri.RoomNum, Ui[i].UNum, Ui[i].ID, &iparray);
					SendMsgToAll(uomsg.pData, uomsg.GetTotalSize());
					
					if(Ri.NowUserNum<=0 && Ri.NowObserverNum <=0 && m_AdminCount==0) {
						// ��� ����ڿ��� ���� �������� �˸�
						CSV_OUTROOM ormsg(SndBuf);
					//	SMALLROOMINFO sri = GetSmallRoomInfo();					
					//	ormsg.Set(&sri);
						ormsg.Set(&GetSmallRoomInfo());
						pChan->SendMsgToLoby(ormsg.pData, ormsg.GetTotalSize());
						
						Clear();
						return;
					}
				}
			}
			else if(Ri.State==1 && i<m_Max_Player)// ### [ ������� ] ###
			{
				{	
					// �����������̰�...
					if(Ps[i].JoinState == 1 && Ps[i].PlayState == 1 && Ps[i].bOnGame && !Ps[i].bFold ) 
					{
						// �����������̰�...
						if( Ps2[i].bWindOn ) // ī�� ������
						{
							Ps2[i].nWindOn++;
							if(Ps2[i].nWindOn>=MAX_WINDTIMEOUT) { // ��������� �������� 18��
								
								CString strMess = "�����ð� ���� ������ �����Ƿ� ���� �����մϴ�.";//g_StrMan.Get(_T("ROOM_TIMEOUT"));
								CSV_SERVERMSG smsg(SndBuf);
								smsg.Set(1, (char*)strMess.operator LPCTSTR());
								SendMsgTo(Ui[i].ID, smsg.pData, smsg.GetTotalSize());
								
								Ps2[i].bWindOn = FALSE;
								Ps2[i].nWindOn = 0;
								
								// ���� ���� ó��
								OnUserOutRoom(Ui[i].UNum, Ui[i].ID, FALSE);
							}
						}
						
						
						if( Ps2[i].bActive ) 
						{
							Ps2[i].nActive++;

							if(Ps2[i].nActive>=MAX_TURNTIMEOUT) { // 15��������� ��������
									FoldUser(i);
									// ### [ ������� ] ###
									Observer_takeaSeat(Ui[i].ID);									
							}

							/*
							if(GameState == RSTATE_CHANGECARD){
								if(Ps2[i].nActive>=MAX_TURNTIMEOUT) { // 10��������� ��������
									//ī�� �ٲٱ� �϶� ���� �ð��� �ʰ��ϰ� �Ǹ� ��� ����~~
									if(Ps2[i].nChangeFlg != TRUE)
									{
										FoldUser(i);
									// ### [ ������� ] ###
										Observer_takeaSeat(Ui[i].ID);									
									}
								}
							}
							else
							{
								if(Ps2[i].nActive>=MAX_TURNTIMEOUT) { // 15��������� ��������
									FoldUser(i);
									// ### [ ������� ] ###
									Observer_takeaSeat(Ui[i].ID);									
								}
							}
							*/
						}
					}
				}			
			}
		}
	}
		
	
	if(bWaitGameOver==TRUE)		// ���� ���� ī����
	{
		WaitGameOverCnt++;
	//	if(WaitGameOverCnt>m_JackPotActive+6){
		if(WaitGameOverCnt>m_JackPotActive+5){
			SendReset(); // ī�� ���¸�� ����
			bWaitGameOver = FALSE;
			WaitGameOverCnt=0;
			m_JackPotActive = 0;
			
			// ### [ ������� ] ###
			Want_Observer_Send();	// �����̵� ���ϴ� ���̵� �̵���Ų��
			
		}
	}
	else if(Ri.State == 0 && bRestartCnt==TRUE && BanishVoteState!=2)	// ����� �غ� ī����
	{
		RestartCnt++;
		
		if(RestartCnt >= 4 && RestartCnt <= 6){			
			// ����ڿ��� �����޼����� ����
			int num = 7;
			num = num - RestartCnt;
			CSV_SERVERMSG smsg(SndBuf);
			CString str;

			int winner = GetPrimer();
			for(i=0; i<m_Max_NewPlayer; i++){				
				if(strlen(Ui[i].ID)>0)
				{						
					if(Ri.MaxUserNum != Ri.NowUserNum)
					{
						if( i == winner)
						{							
							str.Format("%d�� �Ŀ� ������ �ڵ� �����մϴ�.   ���۹�ư�� �����ּ���",num);
							smsg.Set(30, (char*)str.operator LPCTSTR());
							SendMsgTo(Ui[i].ID,smsg.pData,smsg.GetTotalSize());
						}						
						else
						{
							str.Format("%d�� �Ŀ� ������ �ڵ� �����մϴ�.",num);
							smsg.Set(30, (char*)str.operator LPCTSTR());						
							SendMsgTo(Ui[i].ID,smsg.pData,smsg.GetTotalSize());
						}
					}
				}
			}							
		}

		if(RestartCnt>6){
			StartGame();
			bRestartCnt = FALSE;
			RestartCnt=0;
		}
	}
/*
	else if(bRestartCnt==TRUE)	// ����� �غ� ī����
	{
		RestartCnt++;
		if(RestartCnt>2) {
			StartGame();
			bRestartCnt = FALSE;
			RestartCnt=0;
		}
	}
*/	
	// �߹� ��ǥ�� �������̶�� ī��Ʈ ����
	if(BanishVoteState == 2) {
		BanishVoteCnt++;
		// �ð� �ʰ��Ǹ� �߹� ��ǥ ���
		if(BanishVoteCnt > 20) {
			BanishVoteState = 0;
			// ����ڿ��� �����޼����� ����
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(0, "�ð��ʰ��� �߹� ��ǥ�� ��ҵǾ����ϴ�");
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		}
	}
	
	
	///////////////////////////////////////////
	// ���� 
	
	// [���� ������ �۾�]
	if(Ri.bSuperMaster) {
		// ������ ���������� ���� ���۹��̰� ������ ����� ���¶�� ���� �����Ѵ�
		if(bSuperRoomDestory && Ri.State == 0 && bWaitGameOver != TRUE) {
			for(i=0; i<m_Max_NewPlayer; i++) {// ### [ ������� ] ### 
				if(strlen(Ui[i].ID) > 0) {
					int unum = Ui[i].UNum;
					if(unum < 1 || unum >= MAX_USER) continue;
					if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;
					
					// ����ڿ��� �����޼����� ����
					CSV_SERVERMSG smsg(SndBuf);
					smsg.Set(1, "���� ������ �����Ͽ����Ƿ� ��� �����ڵ��� �ڵ����� ����˴ϴ�.");
					User[unum].SendMsg(&smsg);

					OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
				}
			}
		}
	}
	
	// ### [ ������� ] ### 
	if(bNowUserEmptyDestory)
	{
		for(i=0; i<m_Max_NewPlayer; i++) {
			if(strlen(Ui[i].ID) > 0) {
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;
				
				// ����ڿ��� �����޼����� ����
				CSV_SERVERMSG smsg(SndBuf);
				smsg.Set(1, "���� ���������� �����Ƿ� ��� �����ڵ��� �ڵ����� ����˴ϴ�.");
				User[unum].SendMsg(&smsg);

				OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
			}
		}
	}
	
}

void CRoom::ResetGame()
{
	bRestartCnt     = FALSE;
	RestartCnt      = 0;
	bWaitGameOver   = FALSE;
	WaitGameOverCnt = 0;
	StartUserNum = 0;
	// ī�� �迭
	ZeroMemory(m_CardDeck, sizeof(m_CardDeck));
	//ZeroMemory(m_PlayerChangeCradDeck, sizeof(m_PlayerChangeCradDeck));//[62]
	m_CurCardIndex = 0;
	m_GameState = GAME_ROUND_READY;
	GameState = RSTATE_NONE;
//	m_JackPotActive = 0;
	m_CardWindState = 0;
	ZeroMemory(Bc, sizeof(Bc));
	m_Raise.Reset();
	

	for(int i=0; i<m_Max_NewPlayer; i++) {// ### [ �ߺ��� IP�� ] ###
		if(i<m_Max_Player)// ### [ ������� ] ###
		{
			Ps[i].bCheck = FALSE;
			Ps[i].bFold = FALSE;
			Ps[i].bOnGame = FALSE;
			Ps[i].JoinState = 0;
			
			Ps[i].nCard[0] = 0;
			Ps[i].nCard[1] = 0;
			Ps[i].nCard[2] = 0;
			Ps[i].nCard[3] = 0;
			
			Ps[i].nCardTotal = 0;
			Ps[i].nOpenTotal = 0;
			Ps[i].nState = 0;
			Ps[i].PlayState = 0;
		}
		
		// foldnum count ���� �ʱ�ȭ
		Ps2[i].bActive = FALSE;
		Ps2[i].bHaveBanishVote = FALSE;
		Ps2[i].bJoinBanishVote = FALSE;
		Ps2[i].bOnGame = FALSE;
		Ps2[i].bWindOn = FALSE;
		Ps2[i].nActive = 0;
		Ps2[i].nWindOn = 0;
		Ps2[i].In_OutCount = 0;// ### [ ������� ] ###
		Ps2[i].nChangeFlg = FALSE;
//		Ps2[i].nChangeWinEndFlg = FALSE;
	}
}

// ### [ ������� ] ###
void CRoom::StartGame()
{
	if(bValid == FALSE) return;
	if(Ri.State == 1) return;
	// ������Ʈ�� ������ ����� ������ ������ �� ����.
	if( g_pMainView->m_bExitReserve == TRUE || g_pMainView->bConnectDeny || g_nAgentIndex == -1) 
	{
		CSV_SERVERMSG smsg(SndBuf);
		if(g_nAgentIndex == -1)
			smsg.Set(0, "���� �ʱ�ȭ �غ�� ���Ͽ� ������ ������ �� �����ϴ�.");// �ݸ��� ��ڿ��� ������ �ֽʽÿ�");
		else
			smsg.Set(0, "�������� �������� ���Ͽ� ������ ������ �� �����ϴ�.");
		SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		return; // �ڵ������ �׽��� �ȵ�
	}

	Ri.ChangeCardStep = 0;

	ResetGame();
	GameState = RSTATE_DISTRIBUTECARD;
	m_JackPotActive = 0;
	// [������Ŀ] ��ƾ
	CardShuffle(); // ī�� ����

	int joinnum = 0;
	for(int i=0; i<m_Max_NewPlayer; i++) // ### [ ������� ] ###
	{
		if(strlen(Ui[i].ID) > 0) 
		{
			int unum = Ui[i].UNum;
			if(unum < 1 || unum >= MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) 
			{
				OnUserOutRoom(unum, Ui[i].ID, FALSE);
				continue;
			}

			if(i >= m_Max_Player) continue;// ### [ ������� ] ###

			// �ʱ� ���þ�
			if(User[unum].UI.PMoney < Ri.BetMoney) { // ###################### �� ����
			//	CSV_SERVERMSG smsg(SndBuf);
			//	CString strMess = "";
			//	smsg.Set(9, (char*)strMess.operator LPCTSTR()); // ## �Ļ�
			//	User[unum].SendMsg(&smsg);
				OnUserOutRoom(unum, Ui[i].ID, FALSE);
				continue;
			}

			Ps[i].JoinState = 1;// ��������
			Ps[i].PlayState = 1;// ������
			Ps[i].bOnGame   = 1;
		
			Ps[i].nCardTotal = 3;
			Ps[i].nOpenTotal = 0;
			Ps[i].nState     = 1; // ī��3��
			
			// ���ī�� ����
			Bc[i].nCard[0] = (char)GiveCard();
			Bc[i].nCard[1] = (char)GiveCard();
			Bc[i].nCard[2] = (char)GiveCard();
			// �߰�
			if(Ri.FormKind == 0) // ###[���̽�]### 
				Bc[i].nCard[3] = -1;
			else
				Bc[i].nCard[3] = (char)GiveCard();

			joinnum++;
		}
	}

	// �����ڰ� ������ �ο� �����̸� ���ӽ��� ���
	if(Ri.NowUserNum < MIN_PLAYER || joinnum < MIN_PLAYER ) {
		ResetGame();
		bRestartCnt = FALSE;
		RestartCnt  = 0;
		GameState   = RSTATE_NONE;
		return;
	}

	BOOL bRoomChange=FALSE;
	if(Ri.State==0) bRoomChange=TRUE;


	// ���� ���� �÷��� ����
	Ri.State  = 1;
	GameState = RSTATE_PREPARESTART;

	STARTINFO si;
	ZeroMemory(&si, sizeof(STARTINFO));

	// ���⼭ ī����� �ΰ� ��Ƽ���÷��� �Ҵ�
	for( i=0; i<m_Max_Player; i++)  
	{
		if(strlen(Ui[i].ID) > 0) 
		{
			int unum = Ui[i].UNum;
			if(unum < 1 || unum >= MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) 
			{
				OnUserOutRoom(unum, Ui[i].ID, FALSE);
				continue;
			}

			if(Ps[i].bOnGame && Ps[i].PlayState == 1) 
			{
				// ������ ���̵�
				strncpy(si.JoinID[i], Ui[i].ID, 15);
				
				// ### [�α� ��Ͽ�]###
				User[unum].nBet = User[unum].UI.PMoney;// ī��Ʈ Ƚ�� ��Ͽ� �ӽ� ����ü

				User[unum].Set_init_premoney(); // [��ȣõ��] 2004.07.08

				// �氪ó�� �ʱ⺣�þ�
				User[unum].UI.PMoney = User[unum].UI.PMoney - Ri.BetMoney;
				Ui[i].PMoney = User[unum].UI.PMoney;

				m_Raise.SetUser(i, Ri.BetMoney, Ui[i]); 
				// ���� ���� �κп��� ���ӿ� �����ϴ����� 
				// [ ����� ]
				g_pMainView->m_cSALog.Make_Log(LEVEL_USE_GAMECNT, unum, TRUE); // ���� ���Ӽ�

			}
		}
	}

	int nPrimer    = GetPrimer(); // �����ϱ�
	memcpy(si.Ps, Ps, sizeof(PLAYSTATE)*m_Max_Player);
	int first = GetPNum(m_DealerID);
	if(first < 0 || first >= m_Max_Player) {	first = GetPNum(Ri.ID); }
	si.nDistUser = first; // ī�� ��������

	for(i=0; i<m_Max_NewPlayer; i++) // ### [ ������� ] ###
	{
		if(i<m_Max_Player && strlen(Ui[i].ID) > 0 && Ps[i].bOnGame && Ps[i].PlayState == 1) 
		{				
			for(int n = 0; n < 4; n++) // choice �߰�
			{
				si.nCard[n] = Bc[i].nCard[n];
			}

			CSV_STARTCARD startc(SndBuf);
			startc.Set(Ri.RoomNum, Ui[i].UNum, Ui[i].ID, &si);
			SendMsgTo(Ui[i].ID,  startc.pData, startc.GetTotalSize());

			// ī�� ������ ��
			Ps2[i].bWindOn  = TRUE;
			Ps2[i].nWindOn  = 0;
		}
		// ### [ ������� ] ###
		else if(strlen(Ui[i].ID) > 0 )
		{
			for(int n = 0; n < 4; n++) // choice �߰�
			{
				si.nCard[n] = DUMY_CARD;
			}

			CSV_STARTCARD startc(SndBuf);
			startc.Set(Ri.RoomNum, Ui[i].UNum, Ui[i].ID, &si);
			SendMsgTo(i,  startc.pData, startc.GetTotalSize());
		}
	}

	// ���۽� �����ο� ����
	StartUserNum = joinnum;
	m_GameState = GAME_ROUND_3;// 0: 1:ī�� 3��, 2: ī�� ����, 3: 4��, 4: 5��, 5: 6��, 6: ����, 7: �°���
	
	// ��� ����ڿ��� �������� ����Ǿ����� �˸�
	if(bRoomChange) {
		CSV_CHANGEROOM crmsg(SndBuf);
	//	SMALLROOMINFO sri = GetSmallRoomInfo();			
		crmsg.Set(&GetSmallRoomInfo());
		pChan->SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());
	}
}

int CRoom::GetTotLivePlayer()
{
	int totplayer = 0;
	for(int i=0; i<m_Max_Player; i++) {
		if(strlen(Ui[i].ID) > 0 && Ps[i].JoinState!=0 && Ps[i].PlayState!=0) totplayer++;
	}
	return totplayer;
}

int CRoom::GetTotLiveTeam()
{
	int team[4]={0,};
	for(int i=0; i<m_Max_Player; i++) {
		if(strlen(Ui[i].ID) > 0 && Ps[i].JoinState!=0 && Ps[i].PlayState!=0) {
//			team[Ps[i].Team]++;
		}
	}
	int totteam = team[0];
	if(team[1]>0) totteam++;
	if(team[2]>0) totteam++;
	if(team[3]>0) totteam++;
	return totteam;
}

BOOL CRoom::CheckGameOver(BOOL &bOpen)
{
	if(bValid == FALSE) return FALSE;
	if(Ri.State == 0) return FALSE;

	int nGamer = 0;
	// �������� ���̸� �����ڰ� 1���̸� ���� ������(ī����±���)
	for(int i=0;i<m_Max_Player;i++){
		if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && !Ps[i].bFold) nGamer++;
	}
	if(nGamer<2){
		bOpen = FALSE;
		return TRUE;
	}

	// �������� ��緹��� �����ٸ�, ī�� ������ ���������
	if(m_Raise.EndRaise()){
		bOpen = TRUE;
		return TRUE;
	}
	if(Ri.NowUserNum == 1){
		if(GameState!=RSTATE_PREPARESTART) bOpen = FALSE;
		return TRUE;
	}

	return FALSE;
}

void CRoom::SetWaitGameOver()
{
	if(bWaitGameOver==FALSE) {
		bWaitGameOver = TRUE;
		WaitGameOverCnt = 0;
	}
	return;
}

void CRoom::DoGameOver(BOOL bOpen)
{
	if(bValid == FALSE) return;
	if(Ri.State == 0) return;
	if(GameState == RSTATE_RESULT) return;

	GameState=RSTATE_RESULT;
	bRestartCnt=FALSE;
	RestartCnt=0;

	int i=0;
	int nSpecialVal[MAX_PLAYER]={0,};

	// ### [�α� ��Ͽ�] ###
	char Winner_ip[24]={0,};
	// [ ###����### ]
	INT64 tmpJackPot = 0; // ��ī�� �̻������� ����(�ݾ�)

	GAMEOVERRESULT gr; 
	ZeroMemory(&gr, sizeof(GAMEOVERRESULT));

	// �� ��� �ֱ����� ���� 
	int PotWinner = -1;

	if(bOpen==FALSE){ // �ܷ��ο��� ���ڴ�(�������� �ʴ´�, ����ó�������ʴ´�)
		int nownum = 0;
	
		gr.bOpen = FALSE;
		gr.Pg    = MakeInfo();
		for(i = 0; i < m_Max_Player; i++)
		{
			if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && !Ps[i].bFold)
			{
				nownum = gr.nNowWinner = i;
				strncpy(gr.ID,Ui[i].ID,15);
				break;
			}
		}
		
		// ########### ���⼭ ���� ���������Ѵ� ###########
		//nHighNumCnt = 100; // �ӽ� �÷��� 
		int nWinner  = GetWinner(TRUE,nSpecialVal);
		m_nPrimeUser = nWinner; // ������ ����

		//################ ���θӴϰ��
		INT64 nRealTotal = m_Raise.GetWinnerMoney(nWinner);// ���Ӱ������ ���ʰ� ������ ���� �ִ� �ݾ��� �����Ѵ�( ���� )
		gr.nPercent      = GetPercentGory();
		INT64 nWinnerPMoney = nRealTotal-m_Raise.GetWinnerBet(nWinner); //  �¸��� �Ǻ���=> ȹ�� �ݾ�
		INT64 nGory         = CalcGory(nWinnerPMoney); // ���� ����(
		gr.nGory            = nGory;
		nRealTotal         -= nGory;
		if(nRealTotal<0)      nRealTotal = 0;
		nWinnerPMoney      -= nGory;
		//gr.Pg.nRealBat      = nRealTotal;    // �� �ݾ�
		gr.nRealBat      = nRealTotal;    // �� �ݾ�
		if(nWinnerPMoney<0)   nWinnerPMoney = 0;
		gr.nWinMoney        = nWinnerPMoney; // ȹ�� �ݾ�
		

		if(nWinner==nownum){
			int unum = Ui[nownum].UNum;
			if(unum >0 && unum < MAX_USER && faststrcmp(Ui[nownum].ID, User[unum].UI.ID) == 0) {

				// ### [�α� ��Ͽ�] ###
				strcpy(Winner_ip, User[unum].UserIP);
				// 1. �¼� ����
				User[unum].UI.WinNum++;
				Ui[nownum].WinNum   = User[unum].UI.WinNum;
				gr.Ui[nownum].WinNum = User[unum].UI.WinNum;
				gr.Ui[nownum].LooseNum = User[unum].UI.LooseNum;
				gr.Ui[nownum].DrawNum = User[unum].UI.DrawNum;
				strncpy(gr.Ui[nownum].ID, User[unum].UI.ID, 15);
				// 2. �Ӵ� ����
				INT64 nMoney = nRealTotal;

				// ���⼭ ��ȹ��� ����
				if(User[unum].UI.PMoney<0) User[unum].UI.PMoney = 0;
			
				// [ ##### ��Ŀ�Ӵ� �����÷ο� üũ ##### ] : ��������� �ƽ����� Ȯ���غ��ô�!!!
				INT64 tmpMoney = g_pMainView->Cfg.MAX_PMONEY - User[unum].UI.PMoney;
				if(nMoney > tmpMoney) User[unum].UI.PMoney = g_pMainView->Cfg.MAX_PMONEY;
				else
				{
					User[unum].UI.PMoney = User[unum].UI.PMoney + nMoney;
				}

				//���ڸ� �ʿ��ϴ�.
				//2004.05.07 �������Ѿ� ����
				//���ڸ� �ʿ��ϴ�.
				if( g_pMainView->StatisCfg.bTotalMoneyLimit ) {
					if( g_pMainView->CheckTotalmoneylimit(&User[unum].UI, NULL, 1 ) > 0) { //<--------------ã�Ƽ� ��� ����
						Ps2[i].Totalmoneylimit = 1;
					}
				}

				Ui[nownum].PMoney = User[unum].UI.PMoney;
				gr.Ui[nownum].PMoney = User[unum].UI.PMoney;
				

				PotWinner = nWinner;

				// 5. ���伣�����
				if(nSpecialVal[nownum]==1){ // ��ī��
					User[unum].UI.PI.nFCARD++;
					Ui[nownum].PI.nFCARD = User[unum].UI.PI.nFCARD;
				}
				if(nSpecialVal[nownum]==2){ // ��Ʈ����Ʈ �÷���
					User[unum].UI.PI.nSF++;
					Ui[nownum].PI.nSF = User[unum].UI.PI.nSF;

					int tJackp = 0;
					switch(pChan->ChannelStyle)
					{
					case 1:
						tJackp = g_pMainView->Cfg4.jackPotChoboS*2;
						break;
					case 2:
						tJackp = g_pMainView->Cfg4.jackPotJungSuS*2;
						break;
					case 3:
						tJackp = g_pMainView->Cfg4.jackPotGoSuS*2;
						break;
					case 4:
						tJackp = g_pMainView->Cfg4.jackPotDosinS*2;
						break;
					default: // ����
						tJackp = g_pMainView->Cfg4.jackPotFreeS*2;
						break;
					}
					
					if(tJackp > 0) {
						tmpJackPot = g_JackPotMoney/tJackp; // ���� ���X
						gr.bJackPot = 1;
					}
				}
				if(nSpecialVal[nownum]==3){ // �ξ� ��Ʈ����Ʈ �÷���
					User[unum].UI.PI.nRSF++;
					Ui[nownum].PI.nRSF = User[unum].UI.PI.nRSF;

					//ī�带 �������� �ʾƵ� ���� ���� 1/2 ����
					int tJackp = 0;
					switch(pChan->ChannelStyle)
					{
					case 1:
						tJackp = g_pMainView->Cfg4.jackPotChobo * 2;
						break;
					case 2:
						tJackp = g_pMainView->Cfg4.jackPotJungSu * 2;
						break;
					case 3:
						tJackp = g_pMainView->Cfg4.jackPotGoSu * 2;
						break;
					case 4:
						tJackp = g_pMainView->Cfg4.jackPotDosin * 2;
						break;
					default: // ����
						tJackp = g_pMainView->Cfg4.jackPotFree * 2;
						break;
					}
					
					if(tJackp > 0) {
						tmpJackPot = g_JackPotMoney/tJackp; // ���� ���X

						gr.bJackPot = 1;
					}
				}

				memcpy(&gr.Ui[nownum].PI, &Ui[nownum].PI, sizeof(POKERINFO));

			}
		}
		// ################# ����ó�� #################
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && i!=nWinner){
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

				INT64 nFailMoney = m_Raise.GetFailMoney(nWinner,i);
				
				if(User[unum].UI.PMoney < 0) User[unum].UI.PMoney = 0;

				gr.Ui[i].LoseMoney = m_Raise.m_User[i].nRealBat - nFailMoney; // ���� �Ӵ� 

				User[unum].UI.PMoney = User[unum].UI.PMoney + nFailMoney;
				gr.Ui[i].PMoney = Ui[i].PMoney = User[unum].UI.PMoney;
				strncpy(gr.Ui[i].ID, Ui[i].ID, 15);
				gr.Ui[i].WinNum = User[unum].UI.WinNum;
				gr.Ui[i].LooseNum = User[unum].UI.LooseNum;
				gr.Ui[i].DrawNum = User[unum].UI.DrawNum;
				memcpy(&gr.Ui[i].PI, &Ui[i].PI, sizeof(POKERINFO));

				User[unum].Set_refresh_lost_money(); // [��ȣõ��] 2004.07.08
			}
		}

		// ���� 
		if(g_pMainView->Cfg4.JackPotMode == 1) {
			int tJackp = 0;
			switch(pChan->ChannelStyle)
			{
			case 1:
				tJackp = g_pMainView->Cfg4.rChoBo;
				break;
			case 2:
				tJackp = g_pMainView->Cfg4.rJungSu;
				break;
			case 3:
				tJackp = g_pMainView->Cfg4.rGosu;
				break;
			case 4:
				tJackp = g_pMainView->Cfg4.rGod;
				break;
			default: // ����
				tJackp = g_pMainView->Cfg4.rFree;
				break;
			}
			
			if(tJackp > 0) {
				if(g_JackPotMoney < pChan->DefaultJackPot) g_JackPotMoney = pChan->DefaultJackPot; // �⺻ ���� �Ӵ�
				g_JackPotMoney += (gr.nGory/ tJackp); // ���� 
				if(g_JackPotMoney > M_1H) g_JackPotMoney = M_1H;// �� (�ְ� �ݾ�üũ)
			}
		}

		gr.JackPotMoney = g_JackPotMoney;
		// ���⼭ ���� ���� ����� ����
		CSV_GAMEOVERRESULT gormsg(SndBuf);
		gormsg.Set(&gr);
		SendMsgToAll(gormsg.pData, gormsg.GetTotalSize());

	
	}

	///////////////////////////////////////////
	//2004.05.07 �������Ѿ� ����
	for(i=0;i<m_Max_Player;i++)
	{
		if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && !Ps[i].bFold && Ps2[i].Totalmoneylimit > 0) 
		{
			int unum = Ui[i].UNum;
			if(unum < 1 || unum >= MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

			// �޼���
			g_pMainView->SendPremLeadersMsg(unum, Ui[i].PremLeadersCode );
		}
	}	
	///////////////////////////////////////////	

	if(bOpen==TRUE)
	{ // ����ī����� ����(����, ����ó��)
		
		gr.bOpen = TRUE;
		gr.Pg    = MakeInfo();

		int nWinner = GetWinner(TRUE,nSpecialVal);
		m_nPrimeUser = nWinner; // ������ ����

		//## ���ڰ� �Ѹ�

		//################ ���θӴϰ��
		INT64 nRealTotal = m_Raise.GetWinnerMoney(nWinner);//���ڰ� ȹ���� �Ӵ� (���� �Ӵ� + ��� ���� �����(�߰��� ������ ����))
		gr.nPercent      = GetPercentGory();
		INT64 nWinnerPMoney = nRealTotal-m_Raise.GetWinnerBet(nWinner); // ȹ�� �Ӵ� = �Ѻ��þ� - ���� �� ���þ�;
		INT64 nGory         = CalcGory(nWinnerPMoney); // ���� ����
		gr.nGory            = nGory;
		nRealTotal         -= nGory;
		if(nRealTotal<0)      nRealTotal = 0;
		nWinnerPMoney      -= nGory;
		if(nWinnerPMoney<0)   nWinnerPMoney = 0;
		gr.nRealBat			= nRealTotal;		// �� ���þ�
		gr.nWinMoney        = nWinnerPMoney;    // ȹ�� �ݾ�(+)
		
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && !Ps[i].bFold){
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

				// ########### ���⼭ ���� ���������Ѵ� ###########
				if(i==nWinner){

					// ### [�α� ��Ͽ�] ###
					strcpy(Winner_ip, User[unum].UserIP);
					User[unum].UI.WinNum++;
					Ui[i].WinNum   = User[unum].UI.WinNum;
					INT64 nMoney = nRealTotal;		// 2. �Ӵ� ����

					// ���⼭ ��ȹ��� ����
					if(User[unum].UI.PMoney < 0 ) User[unum].UI.PMoney = 0;
					
					// [ ##### ��Ŀ�Ӵ� �����÷ο� üũ ##### ] : ��������� �ƽ����� Ȯ���غ��ô�!!!
					INT64 tmpMoney = g_pMainView->Cfg.MAX_PMONEY - User[unum].UI.PMoney;
					if(nMoney > tmpMoney) User[unum].UI.PMoney = g_pMainView->Cfg.MAX_PMONEY;
					else
					{
						User[unum].UI.PMoney = User[unum].UI.PMoney + nMoney;
					}
					
					
					//2004.05.07 �������Ѿ� ����
					//���ڸ� �ʿ��ϴ�.
					if( g_pMainView->StatisCfg.bTotalMoneyLimit ) {
						if( g_pMainView->CheckTotalmoneylimit(&User[unum].UI, NULL, 1 ) > 0) {// <--------------ã�Ƽ� ��� ����
							Ps2[i].Totalmoneylimit = 1;
						}
					}

					Ui[i].PMoney = User[unum].UI.PMoney;
					gr.Ui[i].PMoney = User[unum].UI.PMoney;
					
					PotWinner = nWinner;
					///////////////////////////////////////////////////////////////////////
					// 5. ���伣����� =>  [ ###����### ]
					if(nSpecialVal[i]==1){
						User[unum].UI.PI.nFCARD++;
						Ui[i].PI.nFCARD = User[unum].UI.PI.nFCARD;
					}

					if(nSpecialVal[i]==2){
						User[unum].UI.PI.nSF++;
						Ui[i].PI.nSF = User[unum].UI.PI.nSF;

							// ���� 
						int tJackp = 0;
						switch(pChan->ChannelStyle)
						{
						case 1:
							tJackp = g_pMainView->Cfg4.jackPotChoboS;
							break;
						case 2:
							tJackp = g_pMainView->Cfg4.jackPotJungSuS;
							break;
						case 3:
							tJackp = g_pMainView->Cfg4.jackPotGoSuS;
							break;
						case 4:
							tJackp = g_pMainView->Cfg4.jackPotDosinS;
							break;
						default: // ����
							tJackp = g_pMainView->Cfg4.jackPotFreeS;
							break;
						}
						
						if(tJackp > 0)
							tmpJackPot = g_JackPotMoney/tJackp; // ���� ���X
					}

					if(nSpecialVal[i]==3){
						User[unum].UI.PI.nRSF++;
						Ui[i].PI.nRSF = User[unum].UI.PI.nRSF;
						// ���� 
						int tJackp = 0;
						switch(pChan->ChannelStyle)
						{
						case 1:
							tJackp = g_pMainView->Cfg4.jackPotChobo;
							break;
						case 2:
							tJackp = g_pMainView->Cfg4.jackPotJungSu;
							break;
						case 3:
							tJackp = g_pMainView->Cfg4.jackPotGoSu;
							break;
						case 4:
							tJackp = g_pMainView->Cfg4.jackPotDosin;
							break;
						default: // ����
							tJackp = g_pMainView->Cfg4.jackPotFree;
							break;
						}
						
						if(tJackp > 0)
							tmpJackPot = g_JackPotMoney/tJackp; // ���� ���X						
					}
					//////////////////////////////////////////////////////////////////////////////

					// ���� id 
					strncpy(gr.Ui[i].ID, Ui[i].ID, 15);
					gr.nNowWinner = nWinner;
					strncpy(gr.ID, Ui[i].ID,15);

					gr.Ui[i].WinNum = User[unum].UI.WinNum;
					gr.Ui[i].LooseNum = User[unum].UI.LooseNum;
					gr.Ui[i].DrawNum = User[unum].UI.DrawNum;
					memcpy(&gr.Ui[i].PI, &User[unum].UI.PI, sizeof(POKERINFO));
				}
				else
				{
					// ########### ���� ó�� �Ѵ� ###########
					// 1. �м� ����
					User[unum].UI.LooseNum++;
					Ui[i].LooseNum = User[unum].UI.LooseNum;
					// 2. ���伣�����
					if(nSpecialVal[i]==1){
						User[unum].UI.PI.nFCARD++;
						Ui[i].PI.nFCARD = User[unum].UI.PI.nFCARD;
					}
					if(nSpecialVal[i]==2){
						User[unum].UI.PI.nSF++;
						Ui[i].PI.nSF = User[unum].UI.PI.nSF;
					}
					if(nSpecialVal[i]==3){
						User[unum].UI.PI.nRSF++;
						Ui[i].PI.nRSF = User[unum].UI.PI.nRSF;
					}
				}
			}
		}

		// ################# ����ó�� #################
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && i!=nWinner){
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

				INT64 nFailMoney = m_Raise.GetFailMoney(nWinner,i);

				if(User[unum].UI.PMoney < 0) User[unum].UI.PMoney = 0;

				User[unum].UI.PMoney = User[unum].UI.PMoney + nFailMoney; // ���ڰ� �������� �̰�ٸ� ������ �� �����ش�
				Ui[i].PMoney = User[unum].UI.PMoney;
			
				gr.Ui[i].LoseMoney = m_Raise.m_User[i].nRealBat - nFailMoney;//���� �Ӵ�

				gr.Ui[i].PMoney = User[unum].UI.PMoney;
				strncpy(gr.Ui[i].ID, Ui[i].ID, 15);
				gr.Ui[i].WinNum = User[unum].UI.WinNum;
				gr.Ui[i].LooseNum = User[unum].UI.LooseNum;
				gr.Ui[i].DrawNum = User[unum].UI.DrawNum;
				memcpy(&gr.Ui[i].PI, &User[unum].UI.PI, sizeof(POKERINFO));

				User[unum].Set_refresh_lost_money(); // [��ȣõ��] 2004.07.08
			}
		}
		
		// [ ###����### ]
		if(g_pMainView->Cfg4.JackPotMode == 1 && pChan->JackPotSaveRatio > 0) {

			if(g_JackPotMoney < pChan->DefaultJackPot) g_JackPotMoney = pChan->DefaultJackPot; // �⺻ ���� �Ӵ�
			g_JackPotMoney += (gr.nGory/ pChan->JackPotSaveRatio); // ���� 
			if(g_JackPotMoney > M_1H) g_JackPotMoney = M_1H;// �� (�ְ� �ݾ�üũ)
		}

		gr.JackPotMoney = g_JackPotMoney;

		CSV_GAMEOVERRESULT gormsg(SndBuf);
		gormsg.Set(&gr);
		SendMsgToAll(gormsg.pData, gormsg.GetTotalSize());

		
	}
/*
	// ���� �޼��� ���� 
	for(i = 0 ; i < m_Max_Player ; i ++){
		if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame){
			if(Ui[i].PMoney <= 0){
				OnUserOutRoom(Ui[i].UNum,Ui[i].ID,FALSE);		
			}
		}
	}	
*/

	// [ ����� ]
//	g_pMainView->Make_StatisticsLog(GRADE_AVERAGE_MONEY, pChan->ChannelStyle,  TRUE, m_Raise.GetTotalBat(), gr.nGory); // ���� �ǵ�, ��(���Ӻ�)
	g_pMainView->m_cSALog.Make_Log(GRADE_AVERAGE_MONEY, pChan->ChannelStyle,  TRUE, m_Raise.GetTotalBat(), gr.nGory); // ���� �ǵ�, ��(���Ӻ�)



	if(m_nPrimeUser >= 0 && m_nPrimeUser < m_Max_Player)
	{
		strncpy(m_DealerID, Ui[m_nPrimeUser].ID, 15);
	}


	// ### [�̺�Ʈ Ƽ��] ###
	if(g_pMainView->EventCfg.bRunEvent && g_pMainView->Cfg.bEventDB )
	{
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame ){
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;
				
				User[unum].EventPrize.GameCnt++;
				if(User[unum].EventPrize.GameCnt== EVENT_GAMECOUNT) {
					User[unum].EventPrize.EventPrize +=EVENT_GIVECOIN; //��Ŀ, ��Ŀ2 30 -> 2����, ���̷ο� 20 -> 1
					
					// �ǽð� ��� ��ƾ!!!
					DBWORKS dbwork;
					memset(&dbwork,0,sizeof(DBWORKS));
					User[unum].MakeDBWork(5, &dbwork);
					BOOL rtn = DBMan.SetDBWork(&dbwork);
					if(rtn == FALSE) {
						CLogFile LogFile;
						LogFile.Writef(_T("DB Work Que overflow - �̺�Ʈ : id=%s"), dbwork.UI.ID);
					}

					SendRoomEventNotice(i);
					
				}
			}
		}
	}


	// [ ###����### ] 
	if(tmpJackPot > 0) 
	{
		for(i=0;i<m_Max_Player;i++) 
		{
			if(strlen(Ui[i].ID)>0 )
			{
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

				if(nSpecialVal[i]==3 || nSpecialVal[i]==2 )//|| nSpecialVal[i]==1)  // ��Ƽ�� , ��Ƽ��
				{ // �����ī��
					SPECIALCARD sc; 
					ZeroMemory(&sc, sizeof(sc));
					
					strncpy(sc.ID, Ui[i].ID, 15);
					strncpy(sc.ChanName, pChan->strChanName.operator LPCTSTR(), 24);// ä���̸�
					sc.nCard = nSpecialVal[i];
					sc.nRoom = Ri.RoomNum; // ���ȣ
					if(g_pMainView->Cfg4.JackPotMode == 1) // ���� ���� ���
					{
						if(PotWinner == i) // ���� �ݾ�
						{
							// [ ##### ��Ŀ�Ӵ� �����÷ο� üũ ##### ] : ��������� �ƽ����� Ȯ���غ��ô�!!!
							INT64 tmpMoney = g_pMainView->Cfg.MAX_PMONEY - User[unum].UI.PMoney;
							if(tmpJackPot > tmpMoney) User[unum].UI.PMoney = g_pMainView->Cfg.MAX_PMONEY; // 10��
							else
							{
								User[unum].UI.PMoney = User[unum].UI.PMoney + tmpJackPot;
							}

							Ui[i].PMoney = User[unum].UI.PMoney;
							
							// ä�κ� �ѵ��ݾ�
							//if(User[unum].UI.PMoney > ChannelMoney) User[unum].UI.PMoney = ChannelMoney;


							g_JackPotMoney -= tmpJackPot;
							if(g_JackPotMoney < pChan->DefaultJackPot) g_JackPotMoney = pChan->DefaultJackPot; // �⺻ ���� �Ӵ�
							sc.JackPotPlus = tmpJackPot;

							if(tmpJackPot > M_1Y) // 1�� ���� ũ�ٸ� ���� ����
								m_JackPotActive = 6; // Ŭ���̾�Ʈ ���̺����ֱ� �ð� ����
							else
								m_JackPotActive = 4; // Ŭ���̾�Ʈ ���̺����ֱ� �ð� ����
							
							// [ ����� ]
							//	g_pMainView->Make_StatisticsLog(GRADE_JACKPOT, pChan->ChannelStyle,  TRUE, tmpJackPot, nSpecialVal[i]); // ����, ���̱ݾ�
							g_pMainView->m_cSALog.Make_Log(GRADE_JACKPOT, pChan->ChannelStyle,  TRUE, tmpJackPot,0); // ����, ���̱ݾ�
						}
						else 
							sc.JackPotPlus = 0;
						
						
					}
					sc.NowJackPot = g_JackPotMoney;
					
					CSV_SPECIALCARD csc(SndBuf);
					csc.Set(&sc);
					if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse)
					{						
						//���̵� �α� ��Ͽ� �����.
						LOGREC_DBWORKS LWork;
						ZeroMemory(&LWork, sizeof(LOGREC_DBWORKS));
						LWork.WorkKind = 1; // [ ����� ]
						LWork.StartTick = timeGetTime();
						LWork.bValid = TRUE;
						// ���̷ο� �ڵ� 30~
						//	LWork.LogRecord.Status = 73; // �α� �����ڵ�� ����.
						LWork.LogRecord.Status = 3; // �α� �����ڵ�� ����.
						
						strncpy(LWork.LogRecord.ID, Ui[i].ID, 15);
						LWork.LogRecord.CurMoney = User[unum].UI.PMoney;	// ���� �Ӵ�
						LWork.LogRecord.LoseMoney = sc.JackPotPlus;			// ���� �Ӵ�
						strcpy(LWork.LogRecord.My_ip, User[unum].UserIP);
						
						if(nSpecialVal[i] == 3) {
							strcpy(LWork.LogRecord.Win_ID, "RSF");					
						}
						else if(nSpecialVal[i] == 2) {
							strcpy(LWork.LogRecord.Win_ID, "SF");					
						}
						else if(nSpecialVal[i] == 1) {
							strcpy(LWork.LogRecord.Win_ID, "FCARD");					
						}
						wsprintf(LWork.LogRecord.Winner_ip,"%d����", Ri.RoomNum);
						
						// DB ������ ����
						BOOL rtn = LogDBMan.SetDBWork(&LWork);
						if(rtn == FALSE) {
							CLogFile logfile("LogRecordDB.txt");
							logfile.Writef("���� ��� ����");
						}
					}
					
					if(nSpecialVal[i] == 3) {
						g_pMainView->SendMsgToAll(csc.pData, csc.GetTotalSize());
					}
					else if(nSpecialVal[i] == 2) {
						pChan->SendMsgToAll(csc.pData, csc.GetTotalSize());
					}
					else if(nSpecialVal[i] == 1) {
						pChan->SendMsgToAll(csc.pData, csc.GetTotalSize());
					}
				}			
			}
		}
	}



// ### [�α� ��Ͽ�] ###		
#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse)
{
	// ī��Ʈ Ƚ�� ��Ͽ� �ӽ� ����ü
	INT64 CurMoney = 0;
	for(i=0;i<m_Max_Player;i++) {
		if(strlen(Ui[i].ID)>0) {
			int unum = Ui[i].UNum;
			if(unum < 1 || unum >= MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

			
			// [��ġ ����] 
			if( g_SearchID.bUse && g_pMainView->IsSearchUserID(Ui[i].ID)) {

					CString str;
					str.Format("%s (%d) %s  %d����  ", g_pMainView->Cfg.ServerName, pChan->ChNum, pChan->strChanName, Ri.RoomNum);
					for(int k =0; k<m_Max_Player;k++)	{
						if(strlen(Ui[k].ID)>0) {
							str+=Ui[k].ID;
							str+=",";
						}
					}

					int l_StrChat = str.GetLength();
					char *pChat= new char[l_StrChat+2];
					memset(pChat, 0, l_StrChat+2);
					strncpy(pChat, str, l_StrChat);
					// ���� ��¥ �� �ð�
					CString Date;
					CTime t = CTime::GetCurrentTime();
					Date.Format("%4d/%2d/%2d(%2d:%2d:%2d)", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
					CBadUserDBWorks Work;
					Work.bValid = TRUE;
					Work.ServerCode = g_pMainView->Cfg.ServerCode;
					Work.GameCode = g_pMainView->Cfg.GameCode;
					Work.MyID=Ui[i].ID;
					Work.TargetID="netmarble";
					Work.SubmitDate = Date;
					// ���� ��ġ
					Work.SubmitReason = "��ġ�ľ�";
					Work.Contents = pChat;
					BadUserDBQue.EnQue(Work);
					if(pChat) delete pChat;
			}

			// ### [�α� ��Ͽ�]###
			// ���� �ǿ��� �󸶸� �Ҿ��°�?
			// ���� ���۽� �Ӵ� ���� -> ���� ����� 
			if(User[unum].UI.PMoney < 0) User[unum].UI.PMoney = 0;
			CurMoney = User[unum].nBet - User[unum].UI.PMoney;

			if(CurMoney < g_LogRangeValue) continue; // 10�� ���� 1��

			if(LogDBMan.DBQue.GetTotNum() >= g_pMainView->Cfg2.LoginDBQueNum2)
			{
				CLogFile logfile("LogRecordDB.txt");
				logfile.Writef("LogRecordDB ���� 03, id=%s, curmoney=%I64d, losemoney=%I64d, winid=%s",
					Ui[i].ID, User[unum].UI.PMoney, CurMoney, gr.ID);
			}
			else
			{
				// ������ ����ũ�� �����ϱ����� Time Tick
				LOGREC_DBWORKS LWork;
				ZeroMemory(&LWork, sizeof(LOGREC_DBWORKS));
				LWork.StartTick = timeGetTime();
				LWork.bValid = TRUE;
				LWork.WorkKind = 1; // [ ����� ] <-----------------------------�߰�
				
			//	LWork.LogRecord.Status = 71; // �������� ���ӿ��� // ���� �ڵ�� ��� �� ����ũ �ڵ�� ����.(1�� ��Ŀ2��)
				LWork.LogRecord.Status = 1; // �������� ���ӿ��� // ���� �ڵ�� ��� �� ����ũ �ڵ�� ����.(1�� ��Ŀ2��)
				
				strncpy(LWork.LogRecord.ID, Ui[i].ID, 15);
				LWork.LogRecord.CurMoney = User[unum].UI.PMoney;
				LWork.LogRecord.LoseMoney = CurMoney;
				strncpy(LWork.LogRecord.Win_ID, gr.ID, 15);
				
				LWork.LogRecord.Win_CurMoney = gr.Ui[gr.nNowWinner].PMoney; //?????
				LWork.LogRecord.Win_GetMoney = gr.nWinMoney;
				
				strcpy(LWork.LogRecord.My_ip, User[unum].UserIP);
				strcpy(LWork.LogRecord.Winner_ip, Winner_ip);
				
				// DB ������ ����
				BOOL rtn = LogDBMan.SetDBWork(&LWork);
				if(rtn == FALSE) {
					CLogFile logfile("LogRecordDB.txt");
					logfile.Writef("LogRecordDB ���� 04, id=%s, curmoney=%I64d, losemoney=%I64d, winid=%s",
						Ui[i].ID, User[unum].UI.PMoney, CurMoney, gr.ID);
				}
			}
			// ### [�α� ��Ͽ�]###
			User[unum].nBet = 0;// �Ӵ� �ʱ�ȭ
		}
	}
}
#endif

	Ri.State = 0;
	GameState=RSTATE_NONE;
	ResetGame();

	// ������ ���� �޼����� ��� ����ڿ��� ����
	CSV_CHANGEROOM crmsg(SndBuf);
//	SMALLROOMINFO sri = GetSmallRoomInfo();
	crmsg.Set(&GetSmallRoomInfo());
	pChan->SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());

	// �߹濹���� �Ǿ� �ִٸ� �߹� ��ǥ ����
	if(BanishVoteState==1) DoBanishVote();
	SetWaitGameOver(); // �������� ī��Ʈ �õ�
}

// ���� ���� ���� �޽���
void CRoom::OnAskChangeGameKind(CCL_ASK_CHANGEGAMEKIND *pMsg)
{
	if(bValid==FALSE) return;
	if(Ri.State != 0) return;

	int pnum = *pMsg->ServPNum;
	if(pnum<0 || pnum>=m_Max_Player) return;
	if(strlen(Ui[pnum].ID)==0 || faststrcmp(Ui[pnum].ID, pMsg->ID)!=0) return;
	int gamekind = *pMsg->GameKind;
//	if(gamekind!=0 && gamekind!=1) return;
	// ��û���� ������ �ƴϸ� ���
	if(faststrcmp(Ri.ID, pMsg->ID) != 0) return;
		
//	Ri.bWhis = sri.bWhis;
//	Ri.nCard = sri.nCard;
//	strncpy(Ri.Title, sri.Title, 30);
	
	Ri.GameKind = gamekind;
//	Ri.bWhis = *pMsg->bWhis;
	Ri.nCard = *pMsg->nCard;

	// ���� ��� ���� �޼��� ����
	CCL_CHANGEGAMEKIND cgmsg(SndBuf);
	cgmsg.Set(&Ri);
	SendMsgToAll(cgmsg.pData, cgmsg.GetTotalSize());
}


void CRoom::OnAskBanishVote(BANISHVOTEINFO *pbv)
{
	if(bValid==FALSE) return;

	int mypnum = GetPNum(pbv->MyID);
	int tarpnum = GetPNum(pbv->TarID);
	if(mypnum<0 || mypnum>=m_Max_Player) return;
	if(tarpnum<0 || tarpnum>=m_Max_Player) return;

	if(strlen(Ui[mypnum].ID)==0 || faststrcmp(Ui[mypnum].ID, pbv->MyID)!=0) return;
	if(strlen(Ui[tarpnum].ID)==0 || faststrcmp(Ui[tarpnum].ID, pbv->TarID)!=0) return;

	if(BanishVoteState)
	{
		// ����ڿ��� �����޼����� ����
		CSV_SERVERMSG smsg(SndBuf);
		if(BanishVoteState==1) smsg.Set(1, "�̹� �߹� ��ǥ�� ����Ǿ��ֽ��ϴ�.");
		if(BanishVoteState==2) smsg.Set(1, "�߹� ��ǥ�� �������Դϴ�.");
		SendMsgTo(Ui[mypnum].ID, smsg.pData, smsg.GetTotalSize());
		return;
	}

	if(Ri.NowUserNum<BANISHVOTE_NUM)
	{
		// ����ڿ��� �����޼����� ����
		CSV_SERVERMSG smsg(SndBuf);
		smsg.Set(1, "3�� �̻��϶��� �߹���ǥ�� �����մϴ�.");
		SendMsgTo(Ui[mypnum].ID, smsg.pData, smsg.GetTotalSize());
		return;
	}

	if(Ri.State==1)
	{
		BanishVoteState = 1;
		memcpy(&BVI, pbv, sizeof(BANISHVOTEINFO));

		// ����ڿ��� �����޼����� ����
		CSV_SERVERMSG smsg(SndBuf);
		smsg.Set(0, "�߹� ��ǥ�� ����Ǿ����ϴ�");
		SendMsgToAll(smsg.pData, smsg.GetTotalSize());
	}
	else
	{
		BanishVoteState = 2;
		memcpy(&BVI, pbv, sizeof(BANISHVOTEINFO));
		// �߹� ��ǥ ����
		DoBanishVote();
	}
}

void CRoom::DoBanishVote()
{
	if(bValid==FALSE) return;
	if(BanishVoteState==0) return;

	if(Ri.NowUserNum < BANISHVOTE_NUM)
	{
		BanishVoteState = 0;
		// ����ڿ��� �����޼����� ����
		CSV_SERVERMSG smsg;
		smsg.Set(0, "��ǥ�ο��� 3�� �̸��̹Ƿ� �߹� ��ǥ�� ��ҵǾ����ϴ�");
		SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		return;
	}

	int mypnum = GetPNum(BVI.MyID);
	int tarpnum = GetPNum(BVI.TarID);
	if(mypnum<0 || mypnum>=m_Max_Player) { BanishVoteState = 0; return; }
	if(tarpnum<0 || tarpnum>=m_Max_Player) { BanishVoteState = 0; return; }
	if(strlen(Ui[mypnum].ID)==0 || faststrcmp(Ui[mypnum].ID, BVI.MyID)!=0) { BanishVoteState = 0; return; }
	if(strlen(Ui[tarpnum].ID)==0 || faststrcmp(Ui[tarpnum].ID, BVI.TarID)!=0) { BanishVoteState = 0; return; }

	BanishVoteState = 2;
	BanishVoteCnt = 0;
	BanishVoteAgreeNum = 1;
	BanishVoteDisgreeNum = 0;
	for(int i=0; i<m_Max_Player; i++)
	{
		Ps2[i].bJoinBanishVote = FALSE;
		Ps2[i].bHaveBanishVote = FALSE;
		// �߹� ����ڸ� ������ ��� ����ڰ� ��ǥ�� �����Ѵ�
		if(strlen(Ui[i].ID)>0 && faststrcmp(Ui[i].ID, BVI.TarID)!=0) Ps2[i].bJoinBanishVote = TRUE;
		// �߹� ��û�ڴ� �ڵ����� ��ǥ�Ѱ����� ���ֵ�
		if(faststrcmp(Ui[i].ID, BVI.MyID)==0) Ps2[i].bHaveBanishVote = TRUE;
	}

	// �߹� ��ǥ ������ �˸�
	CSV_ASK_BANISHVOTE bvmsg(SndBuf);
	bvmsg.Set(&BVI);
	SendMsgToAll(bvmsg.pData, bvmsg.GetTotalSize());
}

void CRoom::OnMyBanishVote(char *id, BOOL bAgree)
{
	if(!bValid) return;
	if(BanishVoteState != 2) return;
	int pnum = GetPNum(id);
	if(pnum == -1) return;

	if(Ps2[pnum].bJoinBanishVote && Ps2[pnum].bHaveBanishVote == FALSE) {
		Ps2[pnum].bHaveBanishVote = TRUE;
		if(bAgree) BanishVoteAgreeNum++;
		else BanishVoteDisgreeNum++;
	}

	int totvote = 0;
	int joinnum = 0;
	for(int i=0; i<m_Max_Player; i++) {
		if(Ps2[i].bJoinBanishVote) joinnum++;
		if(Ps2[i].bHaveBanishVote) totvote++;
	}

	if(totvote >= joinnum) {
		BanishVoteState = 0;

		int tarpnum = GetPNum(BVI.TarID);
		if(tarpnum == -1) return;
		int tarunum = Ui[tarpnum].UNum;
		if(tarunum<1 || tarunum>=MAX_USER) return;
		if(faststrcmp(User[tarunum].UI.ID, Ui[tarpnum].ID) != 0) return;

		// ����ڿ��� �����޼����� ����
		CString str;
		str.Format("��ǥ��� :\n���� %dǥ, �ݴ� %dǥ\n", BanishVoteAgreeNum, BanishVoteDisgreeNum);
		if(BanishVoteAgreeNum>=(joinnum*2)/3) 
	//	if(BanishVoteDisgreeNum==0 && BanishVoteAgreeNum>=joinnum) 
		{
			str += "���ݼ��� �����Ͽ����Ƿ� ���� �߹��մϴ�.";
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(0, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
			
			// ����ڿ��� �����޼����� ����
			CSV_SERVERMSG smsg2(SndBuf);
			smsg2.Set(1, "��ǥ ����� ���Ͽ� ���� �߹�Ǿ����ϴ�");
			SockMan.SendData(User[tarunum].Sid, smsg2.pData, smsg2.GetTotalSize());

			VoteBanUserList.AddID(User[tarunum].UI.ID);
			
			// ���� �߹�
			OnUserOutRoom(tarunum, User[tarunum].UI.ID, FALSE);
		}
		else
		{
			str += "���ݼ��� �ݴ�ǥ�� �߹��� ����մϴ�.";
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(0, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		}
		
		BanishVoteState = 0;
		BanishVoteCnt = 0;
		BanishVoteAgreeNum = 0;
		BanishVoteDisgreeNum = 0;
	}
}


// ��Ŀ���� ������ �����Ѵ�
POKERGAME CRoom::MakeInfo()
{
	POKERGAME pg;
	ZeroMemory(&pg, sizeof(POKERGAME));
	pg.nRound   = m_Raise.GetRound();
	if(m_Raise.GetRound()>0) pg.bRaiseOn = TRUE;
	memcpy(pg.RU, m_Raise.m_User, sizeof(RAISEUSER)*m_Max_Player); 
	memcpy(pg.PS, Ps, sizeof(PLAYSTATE)*m_Max_Player);
	if(Ri.State == 1){ // �������̰�,���Ӱ���� ��������Ѵٸ� ������

		//if(m_GameState == GAME_ROUND_CHANGECARD2 && m_GameState != GAME_ROUND_RESULT){//softpark
		if(m_GameState == GAME_ROUND_HIDDEN && m_GameState != GAME_ROUND_RESULT){
			for(int i = 0 ; i < m_Max_Player ; i ++){
				if(Ps[i].bOnGame && Ps[i].PlayState == 1){
					if(Ps[i].nCard[3] < 0)
					{
						pg.BC[i].nCard[1] = Bc[i].nCard[1];
						pg.BC[i].nCard[2] = 52;
					}					
				}
			}			
		}
		if(GameState==RSTATE_RESULT){
			memcpy(pg.BC, Bc, sizeof(BACKCARD)*m_Max_Player);
		}
		
	}
	pg.nState = 0;
	switch(GameState){
		case RSTATE_PREPARESTART: pg.nState = 1; break;
		case RSTATE_STARTGAME:    pg.nState = 2; break;		
		case RSTATE_CHANGECARD:	  pg.nState = 3; break;			
		case RSTATE_RESULT:       pg.nState = 4; break;					
	}
	return pg;
}


//////////////////////////////////////////////////////////////////////
//////////////////////  [������Ŀ] ���ӷ�ƾ  /////////////////////////
//////////////////////////////////////////////////////////////////////

// ����ڰ� �����û�� �Ͽ���.
// ### [ ������� ] ###
BOOL CRoom::FoldUser(int nNum, BOOL bOut)
{
	if(bValid == FALSE) return FALSE;
	if(Ri.State!=1) return FALSE;
	// �ùٸ� ����Ÿ�� �ƴϴ�!!
	if(nNum<0 || nNum>=m_Max_Player) return FALSE;
	if(Ps[nNum].bOnGame != TRUE) return FALSE;
	if(Ps[nNum].bFold == TRUE) return FALSE;
	
	// ���� ����
	// ���⼭ ���� �޽��� ����
	CSV_FOLDUSER cfold(SndBuf);
	cfold.Set(Ri.RoomNum, Ui[nNum].UNum, nNum, Ui[nNum].ID, 0, g_JackPotMoney); // [ ###����### ] ����� ���� ������.
	SendMsgToAll(cfold.pData, cfold.GetTotalSize());


	Ps[nNum].bFold = TRUE;		// �������⼼��
	Ps[nNum].PlayState = 0;

	Ps2[nNum].bWindOn = FALSE;
	Ps2[nNum].nWindOn = 0;

	//62cut �÷��� �ʱ�ȭ[62]
	Ps2[nNum].nChangeFlg = FALSE; 
//	Ps2[nNum].nChangeWinEndFlg = FALSE;

	// ########## �������� ###########
	int unum = Ui[nNum].UNum;
	if(unum >0 && unum < MAX_USER && 
		faststrcmp(Ui[nNum].ID, User[unum].UI.ID)==0){
		User[unum].UI.DrawNum++;
		Ui[nNum].DrawNum  = User[unum].UI.DrawNum;
		// ### [�α� ��Ͽ�]###
		// ���� ����ڴ� ���þ��� ������ ����ڴ�! ����� ���þ��� �����
		//User[unum].nBet = 0;
		// ����� ����� ������ �˸�
		CHANGEMYINFO cmi={0,};
		cmi.WinNum = User[unum].UI.WinNum;
		cmi.LooseNum = User[unum].UI.LooseNum;
		cmi.DrawNum = User[unum].UI.DrawNum;
		cmi.PMoney  = User[unum].UI.PMoney;

		CSV_CHANGEMYINFO cmimsg(SndBuf);
		cmimsg.Set(&cmi);
		SendMsgTo(Ui[nNum].ID, cmimsg.pData, cmimsg.GetTotalSize());

		// [��ȣõ��] 2004.07.09
		User[unum].Set_refresh_lost_money(); // [��ȣõ��] 2004.07.08
	}

	if(GameState==RSTATE_PREPARESTART){ // ���� �غ��� �����ٸ�
		// �����Ƽ�� �������ٸ�..
		Ps2[nNum].bActive = FALSE;
		Ps2[nNum].nActive = 0;
		if(m_Raise.m_User[nNum].bInUse)	m_Raise.m_User[nNum].bFold = TRUE;
		Ps2[nNum].nFoldNum++;
		GetPrimer(); // ���ο� �� ����
		if(Ps2[nNum].nFoldNum>=3){
			CString strMess = g_StrMan.Get(_T("SPOKER_OUTROOM"));
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, (char*)strMess.operator LPCTSTR() );
			SendMsgTo(Ui[nNum].ID, smsg.pData, smsg.GetTotalSize());
			Ps2[nNum].nFoldNum = 0;
			// ���� ���� ó��
			if ( !bOut )
				OnUserOutRoom(Ui[nNum].UNum, Ui[nNum].ID, FALSE);
		}
		BOOL bOpen = FALSE;
		if(CheckGameOver(bOpen)){
			DoGameOver(bOpen);
			return TRUE;
		}
		// ī�� ������ ��, ī�� ���� �κ� ??????
		// m_GameState; 0: 1: ī�� 3�� 2: ī�� ���� 3: ī�� 4�� 4: ī�� 5�� 5: 6�� 6: ���� 7: �� ���� 8: ���� ����

		if(  m_GameState == GAME_ROUND_3 ) {
			AnalRaiseOn();
		}
		else //m_GameState == 2 ī�� ������ ������
		{
			StartRaise(); // ���� ���� ���Ȱų�, ���� �޴ٸ�
		}
		return TRUE;
	}
	else if(GameState == RSTATE_CHANGECARD)//[62]
	{
		// �����Ƽ�� �������ٸ�..
		Ps2[nNum].bActive = FALSE;
		Ps2[nNum].nActive = 0;		
		if(m_Raise.m_User[nNum].bInUse)	m_Raise.m_User[nNum].bFold = TRUE;
		Ps2[nNum].nFoldNum++;
		GetPrimer(); // ���ο� �� ����
		if(Ps2[nNum].nFoldNum>=3){
			CString strMess = g_StrMan.Get(_T("SPOKER_OUTROOM"));
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, (char*)strMess.operator LPCTSTR() );
			SendMsgTo(Ui[nNum].ID, smsg.pData, smsg.GetTotalSize());
			Ps2[nNum].nFoldNum = 0;
			// ���� ���� ó��
			if ( !bOut )
				OnUserOutRoom(Ui[nNum].UNum, Ui[nNum].ID, FALSE);
		}		
		
		BOOL bOpen = FALSE;
		if(CheckGameOver(bOpen)){
			DoGameOver(bOpen);			
			return TRUE;
		}

		// �ٲٱ� �϶� ���̽� üũ
		ChangeCardRaiseCheck();	//[62]	
	}
	else
	{ // ���� ī�� ������ �߿� �����ٸ�
		if(Ps2[nNum].bActive) {

			if(m_CardWindState == 1) // ���� �ִ�.
				AnalRaiseOn();

			POKERCLNT pc; 
			ZeroMemory(&pc, sizeof(pc));
			pc.nBtn   = 0;
			pc.nStyle = 0; // ����
			SendTurn(nNum, &pc);
			return TRUE;
		}
		else{
			Ps2[nNum].bActive = FALSE;
			Ps2[nNum].nActive = 0;
			if(m_Raise.m_User[nNum].bInUse)	m_Raise.m_User[nNum].bFold = TRUE;
			BOOL bOpen = FALSE;
			if(CheckGameOver(bOpen)){
				DoGameOver(bOpen);
				return TRUE;
			}
		
			if(m_CardWindState == 1)
				AnalRaiseOn();
		}
	}
	return TRUE;
}

// �� ÷ ������
void CRoom::StartRaise()
{
	if(!bValid) return;
	if(Ri.State != 1) return; // ������
	if(GameState != RSTATE_PREPARESTART) return;

	// ���ӽ��� Ȯ���� ���ӽ���
	int i=0;
	BOOL bStart  = TRUE;
	for(i=0;i<m_Max_Player;i++){
		if( Ps2[i].bActive ) bStart=FALSE; // ��� ī�� ���� �ߴٸ�
	}
	BOOL bOpen = FALSE;
	if(CheckGameOver(bOpen)){
		DoGameOver(bOpen);
		return;
	}

	// ������ ����
	if(bStart) {

		m_GameState = GAME_ROUND_4;
		// ��Ƽ�� �÷��� ����
		for( i=0;i<m_Max_Player;i++){
			Ps2[i].bActive = FALSE;
			Ps2[i].nActive = 0;
			Ps2[i].bWindOn = FALSE; // ī������� Ÿ������
			Ps2[i].nWindOn = 0;
		}

		int nSV[MAX_PLAYER]={0,};
		int nDistUser = GetWinner(FALSE, nSV); // (�� 3��) 1�尡�� ������ ���� ī�� ������
		
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && !Ps[i].bFold){ // �������� ����
				Ps[i].nState     = 3; // ī��4�忡 ���������
				Ps[i].nCardTotal = 4;
				Ps[i].nOpenTotal = 2;
				Ps[i].nCard[1]   = (char)GiveCard(); // ���� ���µ� ī��
		
				Ps2[i].bWindOn  = TRUE;
				Ps2[i].nWindOn  = 0;
			}
		}

		m_CardWindState = 1;

		GameState = RSTATE_STARTGAME;
		// [ ###���÷�### ]
		//          ä��		���þ�
		m_Raise.Set(Ri.Level, Ri.GameKind);

		int nActive = GetWinner(FALSE,nSV); // (�� 4��) 2�� �� ���� ū�� ����
		Ps2[nActive].bActive = TRUE;
		Ps2[nActive].nActive = 0;
//		Ps2[nActive].bCheck  = TRUE;
		POKERGAME pg = MakeInfo();
		pg.bDistCard = TRUE;		// ī�� ������ ����
		pg.nDistUser = nDistUser;   // ī�� ������ ����
		pg.CurPlayer = nActive;	// ���� ��

		CSV_RAISE craise(SndBuf);
		craise.Set(&pg);
		SendMsgToAll(craise.pData, craise.GetTotalSize() );


		// ##################### ����????? #################
		//Ps2[nActive].bActive = FALSE;
		//Ps2[nActive].nActive = 0;

	}
}

/*

//[62] �ٲ� ī�带 �޾� ó���ϴ°�
void CRoom::ChangeCard_62cut(CHANGECARD_62CUT data)
{
	if(bValid == FALSE) return;
	if(Ri.State!=1) return;	
	if(GameState != RSTATE_CHANGECARD)return;
	if(strlen(data.ID) == 0)return;	

	int pnum = data.nServerPNum;

	if(pnum < 0 || pnum > m_Max_Player)return;
	if(Ps[pnum].bFold || Ps[pnum].PlayState == 0) return;	
	if(faststrcmp(Ui[pnum].ID, data.ID) != 0) return;		

	// ī�带 ���ȴٸ� ���� ī�带 ��� �д�.
	if(data.nFlag > -1 && data.nFlag < 52)
	{			
		int pcindex = -1;
		int bcindex = -1;

		//�������� �ٲٱⰡ �ƴѵ� �Ǵ� �ٲٱ⸦ �߰ų� �׷��� ������ ���� 
		if(Ps2[pnum].nChangeFlg != TRUE){
			ChangeCardEnd_62Cut(pnum,data.ID);
			return;
		}

		Ps2[pnum].nChangeFlg  = FALSE;

		BOOL bcheck = FALSE;
		for(int i = 0 ; i < 4 ; i++){
			if(Bc[pnum].nCard[i] == data.nChangeCardNum && i < 2)
			{
				Bc[pnum].nCard[i] = -10;
				Ps[pnum].nCardTotal-- ;
				bcindex = i;
				bcheck = TRUE;
				
				if(GAME_ROUND_CHANGECARD2 == m_GameState)
				{
					for(int x = i ; x < 2 ; x ++){						
						Bc[pnum].nCard[x] = Bc[pnum].nCard[x+1];				
					}					

					Bc[pnum].nCard[1] = Ps[pnum].nCard[0];
					x = 0;
					for(x = 0 ; x < 4 ; x ++){						
						Ps[pnum].nCard[x] = Ps[pnum].nCard[x+1];				
					}					
					Ps[pnum].nCard[3] = -1;
				}				
				
				Bc[pnum].nCard[2] = -1;
				Bc[pnum].nCard[3] = -1;

				break;
			}			
		}	
		
		if(bcindex == -1)
		{
			for(i = 0 ; i < 4 ; i++){
				if(Ps[pnum].nCard[i] == data.nChangeCardNum)
				{				
					Ps[pnum].nOpenTotal--;
					Ps[pnum].nCardTotal--;

					for(int x = i ; x < 4 ; x ++){						
						Ps[pnum].nCard[x] = Ps[pnum].nCard[x+1];				
					}				
					
					Ps[pnum].nCard[3] = -10;
					pcindex = 3;
					bcheck = TRUE;
					break;							
				}
			}
		}

		// ���⼭ �ٲ� ������ �˷��ش�..�ֳ�..ī�� ������ �ؾ� �ϴϱ�.�ڱ� �ڽ��� �����Ŀ� ���� ���� �Ҳ���.
		if(bcheck == TRUE && (pcindex > -1 || bcindex  > -1) )
		{		
			CSV_ASK_CHANGECARD_62CUT caskchange(SndBuf);

			CHANGECARD_62CUT cd;
			ZeroMemory(&cd,sizeof(CHANGECARD_62CUT));						
			strcpy(cd.ID,data.ID);
			cd.nFlag  = 100;
			cd.nCardIndex = data.nCardIndex;			
			cd.nChangeCardNum = data.nChangeCardNum;
			cd.nServerPNum = pnum;	

			cd.nCardNum = (char)GiveCard();			
			
			Ps2[pnum].bActive = FALSE;
			Ps2[pnum].nActive = 0;
			Ps2[pnum].bWindOn = FALSE; // ī������� Ÿ������
			Ps2[pnum].nWindOn = 0;		
			Ps[pnum].nState     = 3;  			
			
			int temp = -1;			

			//���� ī���϶� ���� ī�� ���� �� ���� 
			if((bcindex == 0 || bcindex == 1) && pcindex == -1) // ����ī�� �϶�
			{
				//2cut ������ bcī�� �־� �ش�.�������� �ֱ⶧�� 
				if(GAME_ROUND_CHANGECARD2 == m_GameState)// 2cut ���� �������� �ش�.
				{
					 temp = Bc[pnum].nCard[2] = cd.nCardNum;
				}
				if(GAME_ROUND_CHANGECARD1 == m_GameState)
				{
					temp = Bc[pnum].nCard[bcindex] = cd.nCardNum;
				}
			}

			//���� ī�带 �ٲپ��ٸ� ���� ī�� ���� �� ����
			if(pcindex == 3)
			{
				// 2cut ���� �������� �ش�.
				if(GAME_ROUND_CHANGECARD2 == m_GameState)
				{
					temp = Bc[pnum].nCard[2] = cd.nCardNum;
				}
				if(GAME_ROUND_CHANGECARD1 == m_GameState)
				{
					temp = Ps[pnum].nCard[3] = cd.nCardNum;
					Ps[pnum].nOpenTotal++;
				}			
			}
			
			Ps[pnum].nCardTotal++;			


			// ī�带 �����ش�. ���� ī��
			if(data.nCardIndex == 0 ||  data.nCardIndex == 1 )//����ī�� 
			{			
				for(i=0; i<m_Max_NewPlayer; i++){				
					if(strlen(Ui[i].ID)>0)
					{						
						if( i == pnum && strcmp(Ui[i].ID,data.ID)==0)
						{
							//�ڱ� �ڽŸ� ���� �־���
							cd.nCardNum = temp;
							caskchange.Set(&cd);
							SendMsgTo(Ui[i].ID,caskchange.pData, caskchange.GetTotalSize());
						}						
						else
						{
							//�ٸ� ����鿡�� ���� ī�带 ������
							cd.nCardNum = 52;
							caskchange.Set(&cd);
							SendMsgTo(Ui[i].ID,caskchange.pData, caskchange.GetTotalSize());
						}
					}
				}				
			}

			// ī�带 �����ش�. ���� ī��
			if(data.nCardIndex > 1 && data.nCardIndex < 6)//����ī��
			{
				// 2cut ���� �������� �ش�.
				if(GAME_ROUND_CHANGECARD2 == m_GameState)
				{
					for(i=0; i<m_Max_NewPlayer; i++){						
						if(strlen(Ui[i].ID)>0)
						{						
							//2�ƿ����� �������� �ڱ� �ڽŸ� ���� �־���
							if( i == pnum && strcmp(Ui[i].ID,data.ID)==0)
							{
								cd.nCardNum = temp;
								caskchange.Set(&cd);
								SendMsgTo(Ui[i].ID,caskchange.pData, caskchange.GetTotalSize());
							}						
							else
							{							
								cd.nCardNum = 52;
								caskchange.Set(&cd);
								SendMsgTo(Ui[i].ID,caskchange.pData, caskchange.GetTotalSize());
							}
						}
					}				
				}							
				// 1���϶��� ������� �� �����ش�. ����ī���̱� ����
				if(GAME_ROUND_CHANGECARD1 == m_GameState)
				{
					caskchange.Set(&cd);
					SendMsgToAll(caskchange.pData, caskchange.GetTotalSize() );
				}	
			}				
		}			
	}	
	// ī�� �ٲٱ⸦ ���� ������.(��Ҹ� ������) 
	if(data.nFlag == -1)
	{
		Ps2[pnum].nChangeFlg  = FALSE;
		
		CSV_ASK_CHANGECARD_62CUT caskchange(SndBuf);
		
		CHANGECARD_62CUT cd;
		ZeroMemory(&cd,sizeof(CHANGECARD_62CUT));						
		strcpy(cd.ID,data.ID);
		cd.nFlag  = 100;		
		cd.nServerPNum = data.nServerPNum;		
		
		cd.nCardIndex = -1;	
		cd.nChangeCardNum = -1;		
		cd.nCardNum = -1;
		
		Ps2[pnum].bWindOn = FALSE; // ī������� Ÿ������
		Ps2[pnum].nWindOn = 0;		
		Ps[pnum].nState     = 3;

		caskchange.Set(&cd);
		SendMsgToAll(caskchange.pData, caskchange.GetTotalSize() );

//		ChangeCardEnd_62Cut(pnum,data.ID);
	}

	BOOL bOpen = FALSE;
	if(CheckGameOver(bOpen)){
		DoGameOver(bOpen);
	}	

	
	/*
	if((data.nFlag <= 52 && data.nFlag > -1)|| data.nFlag == -1)
	{
		int index = -1;
		int bindex = -1;

		if(Ps2[pnum].nChangeFlg != TRUE)return;
		
		Ps2[pnum].nChangeFlg  = FALSE;
		
		if(data.nFlag > -1){//�ٲٱ⸦ �ѳѵ�
			for(int i = 0 ; i < 4 ; i++){
				if(Bc[pnum].nCard[i] == data.nChangeCardNum && i < 2)
				{
					Bc[pnum].nCard[i] = -10;
					Ps[pnum].nCardTotal-- ;				
					
					m_PlayerChangeCradDeck[pnum].nCardIndex = i;
					m_PlayerChangeCradDeck[pnum].nChangeCardNum = data.nChangeCardNum; 
					m_PlayerChangeCradDeck[pnum].nPNum = pnum;				
					m_PlayerChangeCradDeck[pnum].nFlag  = 100;
					bindex = i;

					if(GAME_ROUND_CHANGECARD2 == m_GameState){
						for(int x = i ; x < 4 ; x ++){						
							Bc[pnum].nCard[x] = Bc[pnum].nCard[x+1];				
						}					
						Bc[pnum].nCard[1] = Ps[pnum].nCard[0];
						for(x = 0 ; x < 4 ; x ++){						
							Ps[pnum].nCard[x] = Ps[pnum].nCard[x+1];				
						}					
						Ps[pnum].nCard[3] = -1;
					}

					Bc[pnum].nCard[2] = -1;
					Bc[pnum].nCard[3] = -1;
					break;
				}			
			}	
			
			if(bindex == -1){
				for(i = 0 ; i < 4 ; i++){
					if(Ps[pnum].nCard[i] == data.nChangeCardNum)
					{				
						Ps[pnum].nOpenTotal--;
						Ps[pnum].nCardTotal--;
						for(int x = i ; x < 4 ; x ++){						
							Ps[pnum].nCard[x] = Ps[pnum].nCard[x+1];				
						}									
						Ps[pnum].nCard[3] = -10;				

						if(data.nCardIndex != i+2)return;

						m_PlayerChangeCradDeck[pnum].nCardIndex = data.nCardIndex;
						m_PlayerChangeCradDeck[pnum].nChangeCardNum = data.nChangeCardNum; 
						m_PlayerChangeCradDeck[pnum].nPNum = pnum;
						m_PlayerChangeCradDeck[pnum].nFlag  = 100;					
						break;							
					}
				}
			}		
		}
		else{//�ٲٱ⸦ ���� �����ѵ�

			memset(&m_PlayerChangeCradDeck[pnum],-1,sizeof(m_PlayerChangeCradDeck[pnum]));
			m_PlayerChangeCradDeck[pnum].nPNum = pnum;
			m_PlayerChangeCradDeck[pnum].nFlag  = 101;

		}

		int changeusercnt = 0 ;
		int nowusercnt = 0 ;
		BOOL bChangeCardEnd = TRUE;// �� ���� �ߴ���
		// �� �Դ��� �˻� 
		for(int i = 0 ; i < m_Max_Player ; i ++){
			if(Ps2[i].nChangeFlg == TRUE && Ps[i].bOnGame && Ps[i].PlayState == 1){
				bChangeCardEnd = FALSE;
				break;
			}	
		}			
		
		// �ٲٱ� ��û�� ��������	
		if(bChangeCardEnd)
		{
			CSV_ASK_CHANGECARD_62CUT_EX caskchange(SndBuf);			
			CHANGECARD_62CUT_EX cd;
			memset(&cd,-1,sizeof(CHANGECARD_62CUT_EX));						
			
			int nSV[MAX_PLAYER]={0,};

			int nDistUser = GetWinner(FALSE,nSV);
			
			
			cd.nDistUser = nDistUser; // ������ ������.
			cd.nRoomNum = Ri.RoomNum;
			
			for(int x = 0 ; x < m_Max_Player ; x++)
			{
				if(Ps2[x].nChangeFlg == FALSE && Ps[x].bOnGame && Ps[x].PlayState == 1){

					int index = 0;
					strcpy(cd.data[x].ID,Ui[x].ID);
					cd.data[x].nFlag  = 100;

					//GAME_ROUND_CHANGECARD2 // �ι�° ���� ���� ī���

					//�ٲ۳ѵ�
					if(m_PlayerChangeCradDeck[x].nFlag == 100)
					{						
						cd.data[x].nServerPNum = x;	
						cd.data[x].nChangeCardNum = m_PlayerChangeCradDeck[x].nChangeCardNum;
						cd.data[x].nCardIndex = m_PlayerChangeCradDeck[x].nCardIndex;
						//���ο� ī�带 �� 
						m_PlayerChangeCradDeck[x].nCardNum = cd.data[x].nCardNum = (char)GiveCard();

						int cardindex = m_PlayerChangeCradDeck[x].nCardIndex; 
						int newcardnum = m_PlayerChangeCradDeck[x].nCardNum;						
						
						if(GAME_ROUND_CHANGECARD1 == m_GameState){							
							if(cardindex > 1 && cardindex < 6){//���� ī��
								Ps[x].nCard[3] = newcardnum;
								Ps[x].nOpenTotal++;
							}
							else{// ���� ī��
								Bc[x].nCard[cardindex] = newcardnum;
							}						
						}
						else if(GAME_ROUND_CHANGECARD2 == m_GameState)// 2cut ���� �������� �ش�.
						{								
							Bc[x].nCard[2] = newcardnum;
							
						//	if(cardindex <= 1){
						///		Bc[x].nCard[1] = Ps[x].nCard[0];
						///		for(int i = 0 ; i < 4 ; i ++){						
						//			Ps[x].nCard[i] = Ps[x].nCard[i+1];				
						//		}
						//	}					
						//	Ps[x].nCard[3] = newcardnum;							
						//	
						}

						// ���⼭ ī�� ������ ����
						Ps2[x].bWindOn  = TRUE;
						Ps2[x].nWindOn  = 0;	
						
						Ps[x].nCardTotal++;			
					}
					else{//�ٲ��� �����ѵ�
						// ���⼭ ī�� ������ ����
						Ps2[x].bWindOn  = TRUE;
						Ps2[x].nWindOn  = 0;	
					}
					//	Ps[x].nState     = 3;
				}
			}	
			
			// ���� �ѳѾ� �����ش�..������ �ٲ۳ѵ���� ���� ī��� �ٲ㼭 ������.
			int j = 0;
			for(i=0; i<m_Max_NewPlayer; i++){
				if(strlen(Ui[i].ID)>0){// && Ps[i].bOnGame && !Ps[i].bFold && Ps[i].PlayState == 1) {
					
					//������ �ٲ۳ѵ���� ���� ī��� �ٲ㼭 ������.
					for(j = 0 ; j < m_Max_Player ; j ++){					
						if((cd.data[j].nCardIndex == 0 || cd.data[j].nCardIndex == 1 || GAME_ROUND_CHANGECARD2 == m_GameState ) && cd.data[j].nServerPNum != i){
							cd.data[j].nCardNum = DUMY_CARD;
						}
					}	
					
					caskchange.Set(&cd);												
					SendMsgTo(Ui[i].ID,caskchange.pData, caskchange.GetTotalSize());	

					//���̽� ������ äũ�ϴ� �÷��װ� �ȴ�.
					if(Ps[i].bOnGame && !Ps[i].bFold && Ps[i].PlayState == 1){
						Ps2[i].nChangeWinEndFlg = TRUE;						
					}
					
					for(j = 0 ; j < m_Max_Player ; j ++){
						if(strlen(Ui[i].ID)>0){
							cd.data[j].nCardNum = m_PlayerChangeCradDeck[j].nCardNum;
						}
					}									
				}
			}
			memset(&m_PlayerChangeCradDeck,-1,sizeof(m_PlayerChangeCradDeck));
		}					
		else 
		{
			//�ٲ۳����� �� �ٲ��(��,�н�),�ٸ��ѵ� �ٲ�� ��� �˷���.
			
			CSV_ASK_CHANGECARD_62CUT caskchange(SndBuf);			
			CHANGECARD_62CUT cd;
			memset(&cd,-1,sizeof(CHANGECARD_62CUT));			
			
			strcpy(cd.ID,Ui[pnum].ID);
			cd.nFlag  = 100;		
			
			cd.nServerPNum = pnum;	
			cd.nCardIndex = data.nCardIndex;
			cd.nServerPNum = pnum;	
			cd.nChangeCardNum = data.nChangeCardNum;

			caskchange.Set(&cd);
			SendMsgToAll(caskchange.pData, caskchange.GetTotalSize() );			
		}
	}

	BOOL bOpen = FALSE;
	if(CheckGameOver(bOpen)){
		DoGameOver(bOpen);
	}
	*/
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���徿 �Ѹ��� 
	/*
	// ī�带 ���ȴٸ� ���� ī�带 ��� �д�.
	if(data.nFlag > -1)
	{			
	//�������� �ٲٱⰡ �ƴѵ� �Ǵ� �ٲٱ⸦ �߰ų� �׷��� ������ ���� 
	if(Ps2[pnum].nChangeFlg != TRUE)return;
	
	  Ps2[pnum].nChangeFlg  = FALSE;
	  
		BOOL bcheck = FALSE;
		for(int i = 0 ; i < 4 ; i++){
			if(Bc[pnum].nCard[i] == data.nChangeCardNum && i < 2)
			{
				Bc[pnum].nCard[i] = -10;
				Ps[pnum].nCardTotal-- ;
				bindex = i;
				bcheck = TRUE;
				break;
			}			
		}	
		
		if(bindex == -1){
			for(i = 0 ; i < 4 ; i++){
				if(Ps[pnum].nCard[i] == data.nChangeCardNum)
				{				
					Ps[pnum].nOpenTotal--;
					Ps[pnum].nCardTotal--;
					for(int x = i ; x < 4 ; x ++){						
						Ps[pnum].nCard[x] = Ps[pnum].nCard[x+1];				
					}				
					
					Ps[pnum].nCard[3] = -10;
					index = 3;
					bcheck = TRUE;
					break;							
				}
			}
		}
		// ���⼭ �ٲ� ������ �˷��ش�..�ֳ�..ī�� ������ �ؾ� �ϴϱ�.�ڱ� �ڽ��� �����Ŀ� ���� ���� �Ҳ���.		
		if(bcheck == TRUE && (index > -1 || bindex  > -1) )
		{		
			CSV_ASK_CHANGECARD_62CUT caskchange(SndBuf);

			CHANGECARD_62CUT cd;
			ZeroMemory(&cd,sizeof(CHANGECARD_62CUT));						
			strcpy(cd.ID,Ui[pnum].ID);
			cd.nFlag  = 100;
			cd.nCardIndex = data.nCardIndex;
			cd.nServerPNum = pnum;	
			cd.nChangeCardNum = data.nChangeCardNum;

			cd.nCardNum = (char)GiveCard();			
			
			Ps2[pnum].bActive = FALSE;
			Ps2[pnum].nActive = 0;
			Ps2[pnum].bWindOn = FALSE; // ī������� Ÿ������
			Ps2[pnum].nWindOn = 0;		
			Ps[pnum].nState     = 3;  						
			
			int temp = -1;
			if(index == 3){
				temp = Ps[pnum].nCard[index] = cd.nCardNum;
				Ps[pnum].nOpenTotal++;
			}

			if(bindex > -1 && bindex < 2)temp = Bc[pnum].nCard[bindex] = cd.nCardNum;
			
			Ps[pnum].nCardTotal++;			

			if(data.nCardIndex <= 1){			
				for(i=0; i<m_Max_Player; i++){
					if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && !Ps[i].bFold && Ps[i].PlayState == 1) {

						if( i == pnum){
							cd.nCardNum = temp;
							caskchange.Set(&cd);
						}
						else {
							cd.nCardNum = DUMY_CARD;
							caskchange.Set(&cd);							
						}
						SendMsgTo(Ui[i].ID,caskchange.pData, caskchange.GetTotalSize());							
					}
				}				
			}
			else 
			{
				caskchange.Set(&cd);
				SendMsgToAll(caskchange.pData, caskchange.GetTotalSize() );
			}				
		}			
	}	
	else 
	{
		Ps2[pnum].nChangeFlg  = FALSE;
		// ī�� �ٲٱ⸦ ���� ������.(��Ҹ� ������)
		CSV_ASK_CHANGECARD_62CUT caskchange(SndBuf);
		
		CHANGECARD_62CUT cd;
		ZeroMemory(&cd,sizeof(CHANGECARD_62CUT));						
		strcpy(cd.ID,Ui[pnum].ID);
		cd.nFlag  = 100;		
		cd.nServerPNum = pnum;		
		
		cd.nCardIndex = -1;	
		cd.nChangeCardNum = -1;		
		cd.nCardNum = -1;
		
		Ps2[pnum].bWindOn = FALSE; // ī������� Ÿ������
		Ps2[pnum].nWindOn = 0;		
		Ps[pnum].nState     = 3;

		caskchange.Set(&cd);
		SendMsgToAll(caskchange.pData, caskchange.GetTotalSize() );

		ChangeCardEnd_62Cut(pnum,Ui[pnum].ID);
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BOOL bOpen = FALSE;
	if(CheckGameOver(bOpen)){
		DoGameOver(bOpen);
	}	
	
}
*/

// ���� �ƿ� ��������  äũ 
void CRoom::ChangeCardRaiseCheck()
{
	if(bValid == FALSE) return;
	if(Ri.State!=1) return;	
	
	if(GameState != RSTATE_CHANGECARD)return;

	BOOL bCheck = TRUE;
	for(int i = 0 ; i < m_Max_Player ; i ++)
	{
		if(Ps[i].PlayState == 1 && strlen(Ui[i].ID) > 0){
			if(Ps2[i].nChangeFlg  == TRUE )
			{
				bCheck = FALSE;
				break;
			}
		}
	}

	if(bCheck)
	{
		for(int i = 0 ; i < m_Max_Player ; i ++){			
			Ps2[i].nChangeFlg = FALSE;			
		}

		GameState = RSTATE_STARTGAME;

		int nSV[MAX_PLAYER]={0,};
		int nDistUser = GetWinner(FALSE,nSV);
		int nActive = nDistUser;
		m_nPrimeUser = nActive; // ������ ����
		Ps2[nActive].bActive = TRUE;
		Ps2[nActive].nActive = 0;
		Ps[nActive].bCheck  = TRUE; // üũ��ư ����
		
		m_Raise.Start(Ps); // ���ο� ������ �غ��϶�!!
		
		POKERGAME pg = MakeInfo();
		pg.bDistCard = FALSE;
		pg.nDistUser = 0;
		//		pg.bSex      = Ui[nPNum].Sex;
		pg.bRaiseOn  = TRUE;
		pg.nPreCmd   = 3;
		pg.bBtnBet   = 1;
		pg.bBtnCheck = TRUE;
		pg.CurPlayer = nActive;	
		pg.LastPlayer = nActive;
		pg.LastStyle = 2; 
		
		CSV_RAISE craise(SndBuf);
		craise.Set(&pg);
		SendMsgToAll(craise.pData, craise.GetTotalSize() );	
	}
}

/*
//[62] // ī�� ��������� �Ϸ� �Ȼ��¸� �˷��ش�.���̽� ���ۼ���
void CRoom::ChangeCardEnd_62Cut(int pnum,char *id)
{
	if(bValid == FALSE) return;
	if(Ri.State!=1) return;
	if(pnum < 0 || pnum >= m_Max_Player)return;
	if(!Ps[pnum].bOnGame ) return;
	if(Ps[pnum].bFold) return;
	if(GameState != RSTATE_CHANGECARD)return;
	if(Ps[pnum].PlayState == 0)return;		
		
	Ps2[pnum].bActive = FALSE;
	Ps2[pnum].nActive = 0;		

	BOOL bCheck = TRUE;
	for(int i = 0 ; i < m_Max_Player ; i ++)
	{	
		if(Ps2[i].nChangeFlg == TRUE && Ps[i].PlayState == 1 && strlen(Ui[i].ID) > 0)
		{
			bCheck = FALSE;
			break;
		}	
	}

	BOOL bOpen = FALSE;
	if(CheckGameOver(bOpen)){
		DoGameOver(bOpen);
		return;
	}	
	
	if(bCheck)
	{
		for(int i = 0 ; i < m_Max_Player ; i ++){				
			Ps2[i].nChangeFlg = FALSE;	
		}

		GameState = RSTATE_STARTGAME;

		int nSV[MAX_PLAYER]={0,};
		int nDistUser = GetWinner(FALSE,nSV);
		int nActive = nDistUser;
		m_nPrimeUser = nActive; // ������ ����
		Ps2[nActive].bActive = TRUE;
		Ps2[nActive].nActive = 0;
		Ps[nActive].bCheck  = TRUE; // üũ��ư ����
		
		m_Raise.Start(Ps); // ���ο� ������ �غ��϶�!!
		
		POKERGAME pg = MakeInfo();
		pg.bDistCard = FALSE;
		pg.nDistUser = 0;
		//		pg.bSex      = Ui[nPNum].Sex;
		pg.bRaiseOn  = TRUE;
		pg.nPreCmd   = 3;
		pg.bBtnBet   = 1;
		pg.bBtnCheck = TRUE;
		pg.CurPlayer = nActive;	
		pg.LastPlayer = nActive;
		pg.LastStyle = 2; 
		
		CSV_RAISE craise(SndBuf);
		craise.Set(&pg);
		SendMsgToAll(craise.pData, craise.GetTotalSize() );	
	}
}
*/

// ��Ŀ���� �������̴�
void CRoom::SendTurn(int nPNum, POKERCLNT *pc)
{	
	if(bValid == FALSE) return;
	if(Ri.State!=1) return;
	// �߰� ���� ��
	if(!Ps[nPNum].bOnGame ) return;
	if(Ps2[nPNum].bActive != TRUE) return;
	
	int i=0;
	int unum = Ui[nPNum].UNum;
	// ��Ƽ�� �÷��� ����
	for( i=0;i<m_Max_Player;i++){
		Ps2[i].bActive = FALSE;
		Ps2[i].nActive = 0;
		Ps2[i].bWindOn = FALSE; // ī������� Ÿ������
		Ps2[i].nWindOn = 0;
	}
	// Ŀ�ǵ� ����
	int nStyle = pc->nStyle; // 0: ����, 1.üũ , 2: ��, 3: ��Ʈ, 4: ����
	int nCmd   = pc->nBtn;   // 0: ��(�ּ�), 1: ����(����), 2: ����, 3: �ƽ�(�ִ�),4:����(������ �� �о�)
	int nPreCmd= 0;

	enum ERAISESIGN sign = ERAISE_FOLD;
	BOOL bBtnCheck  = FALSE;
	int  nNextUser  = 0;
	BOOL bNextBtnBat = FALSE, bNextBtnCheck = FALSE, bNextBtnCheck2 = FALSE;
	BOOL bHiddenCard = FALSE;

	switch(nStyle){
	case 1: // üũ
		sign = ERAISE_CHECK;
		nPreCmd= 2;
		for(i=0;i<m_Max_Player;i++) Ps[i].bCheck = FALSE; 
		break;
	case 2: // ��
		sign = ERAISE_CALL;
		nPreCmd= 3;
		break;
	case 3: // ����
		sign = ERAISE_BAT;
		if(nCmd==0)	nPreCmd= 4;
		if(nCmd==1)	nPreCmd= 5;
		if(nCmd==2 || nCmd==3) nPreCmd= 6;
		if(nCmd==4)nPreCmd = 7; //����
		break;
	default:  // ����/����
		sign = ERAISE_FOLD;
		nPreCmd= 1;
		Ps[nPNum].bFold = TRUE; // �������⼼��
		if(Ps[nPNum].bCheck){
			Ps[nPNum].bCheck = FALSE;
			bBtnCheck = TRUE;
		}
		break;
	} // End of switch!!

	BOOL bContinue = m_Raise.RaiseCommand(nPNum, unum, sign,
			bBtnCheck, nCmd, nNextUser, bNextBtnBat, bNextBtnCheck);
	// ���� �� �� �ߴٸ� �Ӵ� �������Ѵ�
	if(nStyle==3 || nStyle==2){
		if(Ps[nPNum].bOnGame && !Ps[nPNum].bFold){
			if(unum >0 && unum < MAX_USER && faststrcmp(Ui[nPNum].ID, User[unum].UI.ID)== 0){
				User[unum].UI.PMoney = m_Raise.m_User[nPNum].nPMoney;
				Ui[nPNum].PMoney = User[unum].UI.PMoney;

				// ### [�α� ��Ͽ�]###
				// ������ ���õ���Ÿ�� �����Ѵ�
				//User[unum].nBet = m_Raise.m_User[nPNum].nRealBat;
			}
		}
	}

	if(bContinue){ // ����� �����϶�!!
		// �����Ƽ������ ����
		if(nNextUser<0 || nNextUser>=m_Max_Player) nNextUser = GetNextUser(nNextUser);
		if(strlen(Ui[nNextUser].ID)==0 || Ps[nNextUser].bFold) nNextUser = GetNextUser(nNextUser);

		Ps2[nNextUser].bActive = TRUE;
		Ps2[nNextUser].nActive = 0;
		Ps[nNextUser].bCheck  = bBtnCheck;


//		Ri.ChangeCardStep = 0;

		m_nPrimeUser = nNextUser;

		POKERGAME pg = MakeInfo();
		pg.bDistCard = FALSE; // ī��й����!
//		pg.bSex      = Ui[nPNum].Sex;
		pg.nPreCmd   = nPreCmd;
		pg.bBtnBet   = bNextBtnBat;
		pg.bBtnCheck = bBtnCheck;

		pg.CurPlayer = nNextUser;
		pg.LastPlayer = nPNum; // Ĩ������ ���� �� ������
		pg.LastStyle = nStyle; // ���̸� 2
		
		CSV_RAISE craise(SndBuf);
		craise.Set(&pg);
		SendMsgToAll(craise.pData, craise.GetTotalSize() );

		BOOL bOpen = FALSE;
		if(CheckGameOver(bOpen)){
			DoGameOver(bOpen);
		}
		return;
	}

	else{ // ������ ���̴�(��ī��? ���ӳ�?)
		for(i=0;i<m_Max_Player;i++) Ps[i].bCheck = FALSE; 
	
		BOOL bOpen = FALSE;
		if(CheckGameOver(bOpen)){
			POKERGAME pg = MakeInfo();
//			pg.bSex      = Ui[nPNum].Sex;
			pg.nPreCmd   = nPreCmd;
			pg.bRaiseOn  = FALSE;

			pg.CurPlayer = nPNum;
			pg.LastPlayer = nPNum; // Ĩ������ ���� �� ������
			
			if(nStyle == 2) // ����� ȣ�� ����(���̸� )
				pg.LastStyle = -1;

//			Ri.ChangeCardStep = 0;

			CSV_RAISE craise(SndBuf);
			craise.Set(&pg);
			SendMsgToAll(craise.pData, craise.GetTotalSize() );
			m_GameState = GAME_ROUND_RESULT; // ���� ���� ����
			DoGameOver(bOpen);

			return;
		}
		else
		{
			if(nStyle == 2) // �� �϶���
			{
				// [ ������ �� �߰� ]
				POKERGAME pg = MakeInfo();
				pg.nPreCmd   = nPreCmd;
				pg.bRaiseOn  = FALSE;
				pg.CurPlayer = nPNum;
				pg.LastStyle = -1;
				pg.LastPlayer = nPNum; // Ĩ������ ���� �� ������
				

				CSV_RAISE craise(SndBuf);
				craise.Set(&pg);
				SendMsgToAll(craise.pData, craise.GetTotalSize() );
			}
		}


// ####### ����......

		int nSV[MAX_PLAYER]={0,};

		int nDistUser = GetWinner(FALSE,nSV);
		//char eightCard = 0;
		
		//������ Ǭ��. softpark
		if(m_Raise.GetRound()==3)      // ����ī�� 
		{
			bHiddenCard = TRUE;
			m_GameState = 6;
		}
	//	if( m_GameState == GAME_ROUND_6)//[62]
		/*if(m_Raise.GetRound() == RAISE_5)
		{
			m_GameState = GAME_ROUND_CHANGECARD1;
			GameState = RSTATE_CHANGECARD;			

			CSV_ASK_CHANGECARD_62CUT caskchange(SndBuf);

			CHANGECARD_62CUT data;
			ZeroMemory(&data,sizeof(CHANGECARD_62CUT));						
			data.nFlag  = 6;
			data.nChangeStep = 	Ri.ChangeCardStep = 1;
			caskchange.Set(&data);
			SendMsgToAll(caskchange.pData, caskchange.GetTotalSize() );

			for( i=0;i<m_Max_Player;i++){
				if(Ps[i].PlayState == 1 && !Ps[i].bFold)
				{
					Ps2[i].bActive = TRUE;
					Ps2[i].nActive = 0;	
					Ps2[i].nChangeFlg = TRUE; 
				}
			}					
			return;
		}			

		if(m_Raise.GetRound() == RAISE_6)//[62]
		{
			m_GameState = GAME_ROUND_CHANGECARD2;
			GameState = RSTATE_CHANGECARD;		

			CSV_ASK_CHANGECARD_62CUT caskchange(SndBuf);

			CHANGECARD_62CUT data;
			ZeroMemory(&data,sizeof(CHANGECARD_62CUT));						
			data.nFlag  = 6;
			data.nChangeStep = 	Ri.ChangeCardStep = 2;
			
			caskchange.Set(&data);
			SendMsgToAll(caskchange.pData, caskchange.GetTotalSize() );

			for( i=0;i<m_Max_Player;i++){
				if(Ps[i].PlayState == 1 && !Ps[i].bFold)
				{
					Ps2[i].bActive = TRUE;
					Ps2[i].nActive = 0;		
					Ps2[i].nChangeFlg = TRUE; 
				}
			}		
			return;
		}
		*/

		//����ī��		
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && !Ps[i].bFold) {
				if(bHiddenCard){ // ����ī�� �й��!!!
					//softpark ������ Ǭ��.
					Bc[i].nCard[2] = (char)GiveCard();
					m_GameState = 6;
				}
				else
				{ // ���� ī�� ���� ����
					
					Ps[i].nCard[Ps[i].nOpenTotal] = (char)GiveCard();

					if(m_GameState == GAME_ROUND_5 && Ps[i].nOpenTotal == 3)
						m_GameState = GAME_ROUND_6;
					else if(m_GameState == GAME_ROUND_4 && Ps[i].nOpenTotal == 2)
					m_GameState=GAME_ROUND_5;

					Ps[i].nOpenTotal++;					
				}
				Ps[i].nCardTotal++;
			
				// ���⼭ ī�� ������ ����
				Ps2[i].bWindOn  = TRUE;
				Ps2[i].nWindOn  = 0;
			}
		}
		

		m_CardWindState = 1;


		int nActive = 0;
		if(bHiddenCard) nActive = nDistUser;
		else            nActive = GetWinner(FALSE,nSV);
		m_nPrimeUser = nActive; // ������ ����
		GetPrimer();
		Ps2[nActive].bActive = TRUE;
		Ps2[nActive].nActive = 0;
		Ps[nActive].bCheck  = TRUE; // üũ��ư ����

		m_Raise.Start(Ps); // ���ο� ������ �غ��϶�!!

		POKERGAME pg = MakeInfo();
		pg.bDistCard = TRUE;
		pg.nDistUser = nDistUser;
//		pg.bSex      = Ui[nPNum].Sex;
		pg.nPreCmd   = nPreCmd;
		pg.bBtnBet   = bNextBtnBat;
		pg.bBtnCheck = TRUE;
		
		pg.CurPlayer = nActive;
		pg.LastPlayer = nPNum; // Ĩ������ ���� �� ������
		pg.LastStyle = nStyle;

		CSV_RAISE craise(SndBuf);
		craise.Set(&pg);
		SendMsgToAll(craise.pData, craise.GetTotalSize() );
		
		//softprk ������ Ǭ��.
		if(bHiddenCard) { // ����ī�� �й��ƾ�̴�
			for(i=0; i<m_Max_Player; i++){
				if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && !Ps[i].bFold) {
					int cardNum = Bc[i].nCard[2];
					
					CSV_HIDDENCARD hcard(SndBuf);
					hcard.Set(Ri.RoomNum, Ui[i].UNum, Ui[i].ID, cardNum);
					SendMsgTo(Ui[i].ID, hcard.pData, hcard.GetTotalSize() );
				}
			}
		}
		
	}
}

int CRoom::Random_Num(int min, int max)
{
	/*
	int num = 0;
	do
	{
		num = rand();
		//if (num) srand(num);
		num = ( num % (max+1)) + min;
	} while (num < min || num > max);
	return num;
	*/

	int range = (max - min)+1;

	return (min + rand()%range);

}


// ī�� ����
void CRoom::CardShuffle()
{
	int i = 0;
	int k = 0, count = 0;
	char CardFlag[52] = { 0,};

	//srand(time(NULL));
	for (i = 0 ; i < 52 ; i++)
	{
		count = 0;
		do
		{
			k = Random_Num(0, 51);
			count++;
			if(count > 10000)
			{
				for(k = 0; k < 52; k++)
					if(CardFlag[k] == 0) break;
			}
		} while ( CardFlag[k] );
		CardFlag[k] = 1;
		m_CardDeck[i] = k; // 52 ���� ī�带 ����
	}

	m_CurCardIndex = 0;
/*
	// 0 ����
	m_CardDeck[0] = 0;
	m_CardDeck[1] = 10;
	m_CardDeck[2] = 11;
	
	// 1 ����
	m_CardDeck[3] = 10;
	m_CardDeck[4] = 23;
	m_CardDeck[5] = 36;
	
	// 2 ����
	m_CardDeck[6] = 10;
	m_CardDeck[7] = 45;
	m_CardDeck[8] = 10;

	// 0 ����
	m_CardDeck[9] = 13;
	// 1 ����
	m_CardDeck[10] = 5;
	// 2 ����
	m_CardDeck[11] = 14;

	// 0 ����
	m_CardDeck[12] = 9;
	// 1 ����
	m_CardDeck[13] = 17;
	// 2 ����
	m_CardDeck[14] = 18;

	// 0 ����
	m_CardDeck[15] = 12;
	// 1 ����
	m_CardDeck[16] = 38;
	// 2 ����
	m_CardDeck[17] = 20;
/*

	// 0 ����
	m_CardDeck[0] = 0;
	m_CardDeck[1] = 1;
	m_CardDeck[2] = 2;
	
	// 1 ����
	m_CardDeck[3] = 10;
	m_CardDeck[4] = 23;
	m_CardDeck[5] = 36;
	
	// 2 ����
	m_CardDeck[6] = 10;
	m_CardDeck[7] = 45;
	m_CardDeck[8] = 10;

	// 0 ����
	m_CardDeck[9] = 13;
	// 1 ����
	m_CardDeck[10] = 5;
	// 2 ����
	m_CardDeck[11] = 14;

	// 0 ����
	m_CardDeck[12] = 4;
	// 1 ����
	m_CardDeck[13] = 17;
	// 2 ����
	m_CardDeck[14] = 18;

	// 0 ����
	m_CardDeck[15] = 3;
	// 1 ����
	m_CardDeck[16] = 38;
	// 2 ����
	m_CardDeck[17] = 20;
	
/*

	// 0 ����
	m_CardDeck[0] = 0;
	m_CardDeck[1] = 13;
	m_CardDeck[2] = 26;
	
	// 1 ����
	m_CardDeck[3] = 10;
	m_CardDeck[4] = 23;
	m_CardDeck[5] = 36;
	
	// 2 ����
	m_CardDeck[6] = 10;
	m_CardDeck[7] = 45;
	m_CardDeck[8] = 10;

	// 0 ����
	m_CardDeck[9] = 2;
	// 1 ����
	m_CardDeck[10] = 4;
	// 2 ����
	m_CardDeck[11] = 14;

	// 0 ����
	m_CardDeck[12] = 15;
	// 1 ����
	m_CardDeck[13] = 17;
	// 2 ����
	m_CardDeck[14] = 18;

	// 0 ����
	m_CardDeck[15] = 1;
	// 1 ����
	m_CardDeck[16] = 38;
	// 2 ����
	m_CardDeck[17] = 20;

	// 0 ����
	m_CardDeck[18] = 4;
	// 1 ����
	m_CardDeck[19] = 50;
	// 2 ����
	m_CardDeck[20] = 45;
*/
/*

	// 0 ����
	m_CardDeck[0] = 0;
	m_CardDeck[1] = 2;
	m_CardDeck[2] = 3;
	
	// 1 ����
	m_CardDeck[3] = 13;
	m_CardDeck[4] = 15;
	m_CardDeck[5] = 16;
	
	// 2 ����
	m_CardDeck[6] = 10;
	m_CardDeck[7] = 45;
	m_CardDeck[8] = 10;

	// 0 ����
	m_CardDeck[9] = 6;
	// 1 ����
	m_CardDeck[10] = 19;
	// 2 ����
	m_CardDeck[11] = 14;

	// 0 ����
	m_CardDeck[12] = 7;
	// 1 ����
	m_CardDeck[13] = 20;
	// 2 ����
	m_CardDeck[14] = 18;

	// 0 ����
	m_CardDeck[15] = 1;
	// 1 ����
	m_CardDeck[16] = 38;
	// 2 ����
	m_CardDeck[17] = 20;

	// 0 ����
	m_CardDeck[18] = 4;
	// 1 ����
	m_CardDeck[19] = 50;
	// 2 ����
	m_CardDeck[20] = 45;
*/

}




// ���� ���� ī�� �й�.
int CRoom::GiveCard()
{
	if(m_CurCardIndex < 0 || m_CurCardIndex > 51) return -1;
	int cardNum = m_CardDeck[m_CurCardIndex++];
	return cardNum;

}

// ������ ���ϱ�
int CRoom::GetRand(int nMod)
{
	int nRand = 0;
	nRand = (int)(rand()%nMod);
	if(nRand<0) nRand = -nRand;
	return nRand;
}

// ���� ���� ���Ѵ�.
int CRoom::GetPrimer()
{
	if(m_nPrimeUser<0) m_nPrimeUser = 0;
	if(m_nPrimeUser>Ri.MaxUserNum) m_nPrimeUser = 0;

	int nPrime=0; 
	BOOL bFind = FALSE;
	// �ϴ� ���� ���� �ùٸ� ���ΰ� �Ǵ��Ѵ�.
	// ���ٸ� ���� ����ڸ� �����Ѵ�.
	if(strlen(Ui[m_nPrimeUser].ID) == 0) {
		for(int j = m_nPrimeUser + 1; j < Ri.MaxUserNum + m_nPrimeUser; j++) {
			int index = j;
			if(index >= Ri.MaxUserNum) index = index-Ri.MaxUserNum;
			if(Ri.State == 1)
			{
				if(strlen(Ui[index].ID) >0 && !Ps[index].bFold)
				{
					nPrime = index;
					bFind = TRUE;
					break;
				}
			}
			else
			{
				if(strlen(Ui[index].ID) >0)
				{
					nPrime = index;
					bFind = TRUE;
					break;
				}
			}
		}
	}
	if(bFind) m_nPrimeUser = nPrime;
	return m_nPrimeUser;
}


int CRoom::GetWinner(BOOL bEnd,int nSpecialVal[])
//int CRoom::GetWinner(BOOL bEnd,int &nSpecialVal)
{
	enum SEVEN_CARD eName[MAX_PLAYER];
	int nTotalUser=0,i=0;
	int nVal[MAX_PLAYER]     = {0,};
	int aCardVal[MAX_PLAYER] = {0,};
	int auser[MAX_PLAYER]    = {0,};
	INT64 aSecVal[MAX_PLAYER]= {0,};

	for(i=0;i<m_Max_Player;i++){
		nSpecialVal[i] = 0; // 3: ��Ƽ��, 2:��Ƽ��, 1:��ī��
		if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && !Ps[i].bFold){ // ���ǰ� ����(����)�� �ƴ϶�� ī�� �ؼ��Ѵ�.
			int aCalCard[8] = {0,};

			int nCardNum = Ps[i].nOpenTotal; // �ϴ� ����ī��� ���			
			

			if(m_GameState == 100)//GAME_ROUND_CHANGECARD2)
			{
				if(nCardNum == 3)//���� �� ī�带 �ٲ۳�
				{					
					aCalCard[0] = Ps[i].nCard[0];
					aCalCard[1] = Ps[i].nCard[1];
					aCalCard[2] = Ps[i].nCard[2];					
				}
				if(nCardNum == 4)//���綧 ī�带 �ٲ��� ������
				{
					if(Ps[i].nCard[3] == -1)
					{
						aCalCard[0] = Bc[i].nCard[1];
						aCalCard[1] = Ps[i].nCard[0];
						aCalCard[2] = Ps[i].nCard[1];
						aCalCard[3] = Ps[i].nCard[2];
					}
					else
					{
						for(int n=0 ; n < 4; n++){  // ###[8��Ŀ]###
							aCalCard[n] = Ps[i].nCard[n];
						}
					}					
				}
			}
			else 
			{
				for(int n=0;n<Min(nCardNum,4);n++){  // ###[8��Ŀ]###
					aCalCard[n] = Ps[i].nCard[n];
				}
			}

			// ��������� ���ī�� �м�
			if(GameState==RSTATE_RESULT)
			{ 	
				//������ �����Ⱚ�� ������ ������ �𸣴� �ʱ�ȭ ���ش�.
				memset(aCalCard,-1,sizeof(aCalCard));

				nCardNum = Min(Ps[i].nCardTotal,7);  // ###[8��Ŀ]###
			
				aCalCard[0] = Bc[i].nCard[0];
				aCalCard[1] = Bc[i].nCard[1];

				for(int m=0;m<Min(Ps[i].nOpenTotal,4);m++){
					aCalCard[m+2] = Ps[i].nCard[m];
				}

				aCalCard[6] = Bc[i].nCard[2];

				//���� �� ī�带 �ٲ۳�
				//softpark
				/*if(Bc[i].nCard[2] != -1)
					aCalCard[5] = Bc[i].nCard[2];
				//���� �� ī�带 �ٲ��� ���� ��
				if(Bc[i].nCard[2] == -1)
					aCalCard[5] = Ps[i].nCard[3]; */
				
			}
			eName[nTotalUser]    = (SEVEN_CARD)m_Seven.GetValue(aCalCard,nCardNum,&nVal[nTotalUser]);
			m_Seven.IsSpecial(eName[nTotalUser],nSpecialVal[i]);
			aCardVal[nTotalUser] = (int)eName[nTotalUser]*1000000+nVal[nTotalUser];
			auser[nTotalUser]    = i;
			aSecVal[nTotalUser]  = m_Seven.GetSecondVal(aCalCard, nCardNum, eName[nTotalUser], nVal[nTotalUser]);
			nTotalUser++;
		}
	}
	// ��Ÿ�� �� �¸��� ������ƾ�̴�.
	int nStarter = m_Seven.GetWinner(aCardVal, aSecVal, auser, nTotalUser);
	if(nStarter<0 || nStarter>=m_Max_Player) nStarter = 0;
	return nStarter;

}

// [ ���� �ο� ]
int CRoom::GetHiLowWinner(BOOL bEnd,int nSpecialVal[]) //bEnd == true ������ ����
{
	return 0;
}


void CRoom::GetLowHandWinner()
{
}


// �������ڸ� �����Ѵ�.
int CRoom::GetNextUser(int nPrev)
{
	int i=0;
	int nNext=0;
	for( i=nPrev+1; i<Ri.MaxUserNum+nPrev; i++){
		int index = i;
		if(index>=Ri.MaxUserNum) index = index-Ri.MaxUserNum;
		if(strlen(Ui[index].ID)>0 && Ps[index].bOnGame && !Ps[index].bFold){
			nNext = index;
			break;
		}
	}
	return nNext;
}

// ù 3���� ������ ������ �ϳ��� ī�带 ���� => ��� ���ý� ���ӽ���
void CRoom::AnalRaiseOn()
{
	if(bValid == FALSE) return;
	if(Ri.State!=1) return;

	// ���ӽ��ۿ��� Ȯ��
	int nRemind = 0;
	for( int i = 0; i < m_Max_Player; i++) {
		if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && Ps[i].JoinState == 1 && Ps[i].PlayState == 1 ) {
			if( Ps2[i].bWindOn ) nRemind++;
		}
	}

	if(nRemind==0) 	{
		for( int i = 0; i < m_Max_Player; i++) {
			Ps2[i].bWindOn = FALSE; // ī������� Ÿ������
			Ps2[i].nWindOn = 0;
		}

		m_CardWindState = 0;
		// ���⼭ ���� ������ ����(������ ����)
		CSV_WINDCARDEND wend(SndBuf);
		wend.Set(0);
		SendMsgToAll(wend.pData, wend.GetTotalSize() );

		for( i = 0; i < m_Max_Player; i++)
		{
			if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && Ps[i].JoinState == 1 && Ps[i].PlayState == 1 )
			{
				if(Ps2[i].bActive == TRUE) Ps2[i].nActive = 0; // �ٽ� �ʱ�ȭ ���ش� (�̶� ���� �� �ð� �ٽ� üũ)
			}
		}

		// ī�� ���� �κ��̶�� ��ο��� �ð������� �д�.
		if(GameState == RSTATE_PREPARESTART)
		{
			// ù���� ��
			m_GameState = GAME_ROUND_CARDCHOICE; // ī�� ���� 3�� 
			for(i = 0; i < m_Max_Player; i++)
			{
				if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && Ps[i].JoinState == 1 && Ps[i].PlayState == 1 )
				{
					Ps2[i].bActive = TRUE;
					Ps2[i].nActive = 0;
				}
			}
		}
	}
}

// ī�� �����°��� ���´ٸ�
void CRoom::EndWind(int pnum)//, POKERCLNT *pc)
{
	if(bValid == FALSE) return;
	if(Ri.State!=1) return;

	if(!Ps[pnum].bOnGame || Ps[pnum].bFold) return;
	if(Ps2[pnum].bWindOn!=TRUE) return;
	

	Ps2[pnum].bWindOn  = FALSE;
	Ps2[pnum].nWindOn  = 0;

	AnalRaiseOn();
}


// ����ڰ� ī�带 �����Ͽ���.
void CRoom::OnReadyToStart(int pnum, POKERCLNT *pc, int discard)
{
	if(!bValid) return;
	if(Ri.State != 1) return;

	if(!Ps[pnum].bOnGame || Ps[pnum].bFold) return;
	if(Ps2[pnum].bActive != TRUE) return;
	
	if(m_GameState != GAME_ROUND_CARDCHOICE) return;

	if(pc->nStyle!=3) return;
	if(pc->nBtn < 1 || pc->nBtn > 3) return;

	Ps2[pnum].bActive = FALSE;
	Ps2[pnum].nActive = 0;

	Ps2[pnum].bWindOn = FALSE;
	Ps2[pnum].nWindOn = 0;
	Ps2[pnum].nFoldNum = 0; //  ���� ������ �ʱ�ȭ ���� �ʰ� ���⼭ �ʱ�ȭ �Ѵ�

	int i=0;
	// �����ī�����ġ
	char aBuf[4] ={0,};
	memcpy(aBuf, Bc[pnum].nCard, sizeof(char)*4); 
	memset(Bc[pnum].nCard, -1, sizeof(char)*4);
	int nIndex = 0, nSelCard = 0;

	if(Ri.FormKind == 0)
	{
		for(i=0;i<3;i++){
			if(pc->nBtn==i+1){ // 1 , 2 , 3
				nSelCard = aBuf[i];
			}
			else{
				Bc[pnum].nCard[nIndex] = aBuf[i]; // ���� ī��( ���� 2��)
				nIndex++;
			}
		}
	}
	else
	{// ###[���̽�]###
		if(discard != 4) // 1 ~ 3
		{
			for( i = discard-1; i < 3; i++)
			{
				aBuf[i] = aBuf[i+1];
				aBuf[i+1] = 0;
			}
		}
		
		for(i=0;i<3;i++){
			if(pc->nBtn==i+1){ // 1 , 2 , 3
				nSelCard = aBuf[i];
			}
			else{
				Bc[pnum].nCard[nIndex] = aBuf[i]; // ���� ī��( ���� 2��)
				nIndex++;
			}
		}
	}

	Ps[pnum].nState     = 2;  
	Ps[pnum].nCardTotal = 3;	// ��3��
	Ps[pnum].nOpenTotal = 1;	// ��2�� + ����1
	Ps[pnum].nCard[0]   = nSelCard; // ���õ�ī�带 ����ī��� ����

	Bc[pnum].nCard[2] = -1;
	Bc[pnum].nCard[3] = -1;

	// ī�� ���� Ÿ�̸� ���� �׸���
	CSV_SELECTCARD selcard(SndBuf);
	selcard.Set(Ri.RoomNum, Ui[pnum].UNum, pnum, Ui[pnum].ID);
	SendMsgToAll( selcard.pData, selcard.GetTotalSize() );


	StartRaise();

}

// �������� �����ð��� ���Ӽ���
void CRoom::SendReset()
{
	if(bValid == FALSE) return;
	if(Ri.State==1) return;

	// �� ����
	CSV_ENDGAME gend(SndBuf);
	gend.Set(0);
	SendMsgToAll(gend.pData, gend.GetTotalSize() );

	// ���� �����ѻ��...�ƿ���Ų��
	for(int i=0;i<m_Max_Player;i++) {
		if(strlen(Ui[i].ID) > 0)  {
			int unum = Ui[i].UNum;
			if(unum < 1 || unum >= MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) {
				OnUserOutRoom(unum, Ui[i].ID, FALSE);
				continue;
			}			
			
			if(User[unum].UI.PMoney < Ri.BetMoney) { // ######################
			//	CSV_SERVERMSG smsg(SndBuf);
			//	CString strMess = "";
			//	smsg.Set(9, (char*)strMess.operator LPCTSTR()); // ## �Ļ�â
				//SendMsgTo(Ui[i].ID, smsg.pData, smsg.GetTotalSize());
			//	User[unum].SendMsg(&smsg);
			//	OnUserOutRoom(unum, Ui[i].ID, FALSE);		

				
				// [��ȣõ��] 2004.07.09
				BOOL bExist = FALSE;
				GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_SAFEGUARDANGEL, &bExist);
				if( !bExist ) {
				//	CSV_SERVERMSG smsg(SndBuf);
				//	CString strMess = "";
				//	smsg.Set(9, (char*)strMess.operator LPCTSTR()); // ## �Ļ�â
				//	//SendMsgTo(Ui[i].ID, smsg.pData, smsg.GetTotalSize());
				//	User[unum].SendMsg(&smsg);
					OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
				}

			}			
		}
	}	

	// [���� ������ �۾�]
	// �����߹� �����ڰ� ������ �����Ŵ
	for(i=0; i<m_Max_NewPlayer; i++) {// ### [ ������� ] ###
		if(strlen(Ui[i].ID) > 0 && Us[i].bForceBanish) {
			DoForceBanish(i);
		}
	}

	if(GameState==RSTATE_NONE && bRestartCnt==FALSE && Ri.NowUserNum == Ri.MaxUserNum)
	{
		bRestartCnt=TRUE;
		RestartCnt=3;	
		return;
	}

	// ���Ӹ��� ����� �������� �����ī��Ʈ �õ� //softpark
	if(GameState==RSTATE_NONE && bRestartCnt==FALSE && Ri.NowUserNum >= MIN_PLAYER && Ri.NowUserNum < Ri.MaxUserNum){//���۹�ư ����	
		bRestartCnt=TRUE;
		RestartCnt=0;
		// ���۹�ư 
		int nPrimer    = GetPrimer(); // �����ϱ�
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,1);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}
}


void CRoom::SelectWinCase(int pnum, int winCase)
{
	if(bValid == FALSE) return;
	if(Ri.State!=1) return;
}
		

void CRoom::CheckSelectWinCase()
{
}

// ���ݾ��� �����Ѵ�
INT64 CRoom::CalcGory(INT64 nRealTotal)
{
	INT64 nGory    = (INT64)((GetPercentGory()*nRealTotal)/100); // ���� ���
	INT64 nRestVal = (INT64)(nGory%10);
	if(nRestVal<0) nRestVal = 0;
	nGory  -= nRestVal;
	if(nGory<10)   nGory    = 0;
	return nGory;
}

// ä���̳� ���ӽ�Ÿ�Ϻ��� ���ݾ����� ����ȭ �����Ѵ�
int CRoom::GetPercentGory()
{
	int nVal = 1;

	switch(pChan->ChannelStyle){
		case 1: nVal =  5; break; // �ʺ� ( 2%)
		case 2: nVal =  5; break; // �߼� ( 3%)
		case 3: nVal =  5; break; // ��� ( 3%)
		case 4: nVal =  5; break; // ���� ( 3%)
	}
	return nVal;
}

// ### [ ������� ] ### 
void CRoom::ExitSubScript(CCL_EXITSUBSCRIPT *pMsg, int pnum, int unum)
{
	if(pnum < 0 || pnum >= m_Max_NewPlayer) return;
	if(unum<1 || unum>=MAX_USER) return;

	if(Ui[pnum].UNum != unum) return;

	if( pnum >= m_Max_Player ) //�����ڴ� �ٷ�����
	{
		User[unum].bExitSubscript = FALSE;
		OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
		
		// ### [�̺�Ʈ Ƽ��] ###
		g_pMainView->SendChannelEventNotice(unum);
		return;
	}

	if( Ri.State == 0 || !Ps[pnum].bOnGame || Ps[pnum].bFold)
	{// ������
		User[unum].bExitSubscript = FALSE;
		OnUserOutRoom(unum, User[unum].UI.ID, FALSE);

		// ### [�̺�Ʈ Ƽ��] ###
		g_pMainView->SendChannelEventNotice(unum);
	}
	else
	{// ������ ����/���  
		
		if(*pMsg->bExit == 1 ) 
			User[unum].bExitSubscript = TRUE;  // 1 ����
		else 
			User[unum].bExitSubscript = FALSE; // 2 ���
		
		CCL_EXITSUBSCRIPT esmsg(SndBuf);
		esmsg.Set(Ri.RoomNum, unum, pMsg->ID, *pMsg->bExit);
		SendMsgToAll(esmsg.pData, esmsg.GetTotalSize());
	}
}

// ### [�̺�Ʈ Ƽ��] ###
void CRoom::SendRoomEventNotice(int pnum)
{
	// �̺�Ʈ�� ���������� �ʴٸ� ����
	if(!g_pMainView->EventCfg.bRunEvent) return;
	if(pnum<0 || pnum > m_Max_Player ) return;

	int unum = Ui[pnum].UNum;
	if(unum < 1 || unum >= MAX_USER) return;
	if(faststrcmp(Ui[pnum].ID, User[unum].UI.ID) != 0) return;

	// ���ӷ� �̺�Ʈ ������ �ִٸ�
	if(strlen(g_pMainView->EventCfg.RoomEventNotice) > 0) {

		CString str1 = g_pMainView->EventCfg.RoomEventNotice;
		CString str2;
		str2.Format("���� [%s]ȸ������ ȹ���� �̺�Ʈ ������ %d���Դϴ�. ������ ����� ��� ����̺�Ʈ �������� �����Ͻñ� �ٶ��ϴ�.", User[unum].UI.ID, User[unum].EventPrize.EventPrize);
		str1+="\n";
		str1+=str2;

		// ����ڿ��� �����޼����� ����
		CSV_EVENTNOTICE enmsg(SndBuf);
		enmsg.Set(1, (char*)str1.operator LPCTSTR(), RGB(255,100,0));
		SendMsgTo(pnum, enmsg.pData, enmsg.GetTotalSize());
	}

}

////////////////////////////////////////////////////////////
/////////////////// ### [ ������� ] ###  //////////////////
////////////////////////////////////////////////////////////

//���� -> ����
void CRoom::Participation(int unum, int pnum, int sndFxKind)
{
	if(pnum < m_Max_Player || pnum >= m_Max_NewPlayer) return;
	if(unum<1 || unum>=MAX_USER) return;
	if(strcmp(User[unum].UI.ID, Ui[pnum].ID) != 0) return;
	
	if( User[unum].bAdminUser ) return;
	if( User[unum].bObserver != TRUE) return;
	if( ReservationFindID(Ui[pnum].ID) ) {
		CSV_OBSERVER_MESSAGE smsg2(SndBuf);
		smsg2.Set(0, "�̹� ���� ���� ������ �Ͽ����ϴ�");
		User[unum].SendMsg(&smsg2);
		return; // ����Ǿ� �ִٸ�
	}

	//������ 
	int epnum = -1;
	for(int i=0; i<Ri.MaxUserNum; i++)	{
		if(strlen(Ui[i].ID) == 0) { epnum = i; break; }
	}

	if(epnum == -1) {
		Make_a_Reservaiton(pnum, unum, sndFxKind); // ��������
		return;
	}

	int myIpIndex = Room_MyIp_Check(unum);// ������ �ߺ��˻�
	if( myIpIndex == -2) {
		CSV_REFUSE_ENTERROOM rermsg(SndBuf);
		rermsg.Set(51);
		SendMsgTo(Ui[pnum].ID, rermsg.pData, rermsg.GetTotalSize());
		return;
	}
	
	User[unum].PNum = epnum;
	User[unum].bObserver = FALSE;
	User[unum].UI.cObserver = 0;
	Ui[pnum].cObserver = 0;

	int inOutCnt = Ps2[pnum].In_OutCount;//�ݺ�����
	// ���������� Ŭ����
	ZeroMemory(&Ps[epnum], sizeof(PLAYSTATE));
	ZeroMemory(&Bc[epnum], sizeof(BACKCARD));
	ZeroMemory(&Ps2[epnum], sizeof(PLAYSTATE2));
	Ps2[epnum].In_OutCount = inOutCnt;// �ݺ� ����
	
	// ### [���� �߰� �۾�] ###
	Ps[epnum].nSndFxKind = sndFxKind;
	Ps[epnum].nCheckIp = myIpIndex;

	memcpy(&Ui[epnum], &Ui[pnum], sizeof(USERINFO));
	ZeroMemory(&Ui[pnum], sizeof(USERINFO));

	Ri.NowUserNum++;
	if(Ri.NowObserverNum>0) Ri.NowObserverNum--;
	
	// ### [ �ߺ��� IP�� ] ###
	IPARRINDEX iparray;
	ZeroMemory(&iparray, sizeof(iparray));
	for( i=0;i<m_Max_Player;i++) { iparray.IpIndex[i]= (char)Ps[i].nCheckIp; }
		
	// ���� ���� �ٸ� ����ڿ��� ���ο� ������� ������ �뺸
	CSV_ENTER_PARTICIPATION uermsg(SndBuf);
	uermsg.Set(Ri.RoomNum, pnum, epnum, sndFxKind, Ui[epnum].ID, &iparray);
	SendMsgExept(epnum, uermsg.pData, uermsg.GetTotalSize());

	CSV_ACCEPT_PARTICIPATION aparti(SndBuf);
	aparti.Set(Ri.RoomNum, pnum, epnum, sndFxKind, Ui[epnum].ID, &iparray);
	SendMsgTo(Ui[epnum].ID, aparti.pData, aparti.GetTotalSize());

	CSV_CHANGEROOM crmsg(SndBuf);
//	SMALLROOMINFO sri = GetSmallRoomInfo();
	crmsg.Set(&GetSmallRoomInfo());
	pChan->SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());

	// ���� ���� ī���� ����
	if(GameState==RSTATE_NONE && Ri.NowUserNum >= MIN_PLAYER && Ri.NowUserNum < Ri.MaxUserNum)
	{
		bRestartCnt=TRUE;
		RestartCnt=0;

		// ���۹�ư 
		int nPrimer    = GetPrimer(); // �����ϱ�
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,1);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}
	// ��� ����ڿ��� �������� ����Ǿ����� �˸��� ����� ������ ���ŵ��� �˸�
	
}

// ### [ ������� ] ###
//���� -> ����
void CRoom::Emerge_Out(int unum, int pnum)
{
	if(pnum < 0 || pnum >= m_Max_Player ) return;
	if(unum<1 || unum>=MAX_USER) return;
	if(strcmp(User[unum].UI.ID, Ui[pnum].ID) != 0) return;
	if( User[unum].bObserver == TRUE) return;
	if(strcmp(User[unum].UI.ID, Ri.ID) == 0) { //����
		CSV_OBSERVER_MESSAGE smsg2(SndBuf);
		smsg2.Set(0, "������ �������� �̵��� �� �����ϴ�");
		User[unum].SendMsg(&smsg2);
		return;
	}
		
	if(Observer_FindID(Ui[pnum].ID) )
	{
		CSV_OBSERVER_MESSAGE smsg2(SndBuf);
		smsg2.Set(0, "�̹� ���� ������ �Ͽ����ϴ�.");
		User[unum].SendMsg(&smsg2);
		return;
	}

	if(Ps[pnum].bOnGame && !Ps[pnum].bFold) //����
	{
		Make_a_Observer(pnum, unum);
		return; //�����ϰ��
	}

	if(Ps2[pnum].In_OutCount++ > 0) //�ݺ� Ƚ�� ����
	{
		if(Ps2[pnum].In_OutCount >  5) {
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, "���輺 ��ư Ŭ������ ���Ͽ�, �ڵ� ���� �Ǿ����ϴ�");
			User[unum].SendMsg(&smsg);

			OnUserOutRoom(unum, User[unum].UI.ID, FALSE); //���
			return;
		}
		CSV_OBSERVER_MESSAGE smsg2(SndBuf);
		smsg2.Set(0, "���� �ǿ� �̵� �� �� �ֽ��ϴ�.");
		User[unum].SendMsg(&smsg2);
		return;
	}

	int epnum = -1, i = 0;
	if(User[unum].bAdminUser)
		epnum = m_Max_NewPlayer-1;
	else {
		for(i=m_Max_Player; i<m_Max_NewPlayer-1; i++) {
			if(strlen(Ui[i].ID) == 0) { epnum = i; break; }
		}
	}

	if(epnum == -1) 
	{
		CSV_OBSERVER_MESSAGE smsg2(SndBuf);
		smsg2.Set(0, "������ �ڸ��� �����ϴ�.");
		User[unum].SendMsg(&smsg2);
		return;
	}


	LogRecordFunc(unum, pnum);  // ### [�α� ��Ͽ�]###

	// ### [ �ߺ��� IP�� ] ###
	if(Ps[pnum].nCheckIp >= 0)
	{
		int ipindex= Ps[pnum].nCheckIp;
		int indexCnt=0;
		for(int i=0;i<m_Max_Player;i++)
		{
			if(pnum==i) continue;
			if(ipindex == Ps[i].nCheckIp) indexCnt++;
		}
		// ����
		if(indexCnt == 1)
		{
			for(i=0;i<m_Max_Player;i++) {
				if(pnum==i) continue;
				if(ipindex == Ps[i].nCheckIp) Ps[i].nCheckIp = -1; // ��� ����
			}
			
			m_aIpIndex[ipindex] = 0;
			m_nIpCnt--;
		}
	}
	
	int inOutCnt= Ps2[pnum].In_OutCount;//�ݺ�����
	// ���� ������ �����ϰ� �����������¸� Ŭ����
	ZeroMemory(&Ps[pnum], sizeof(PLAYSTATE));
	ZeroMemory(&Ps2[pnum], sizeof(PLAYSTATE2));
	ZeroMemory(&Bc[pnum], sizeof(BACKCARD));
	// ### [ �ߺ��� IP�� ] ###
	Ps[pnum].nCheckIp = -1; // $$$ �⺻�� -1
	ZeroMemory(&Us[pnum], sizeof(USERSTATE));	// [���� ������ �۾�]
	// ����� ����
	ZeroMemory(&Ui[pnum], sizeof(USERINFO));
	// ������ ������� �ʱ�ȭ
	User[unum].bExitSubscript = FALSE;

	// �����ο����� �Ѹ� ����
	Ri.NowUserNum--;
	Ri.NowObserverNum++;
	
	User[unum].bObserver = TRUE;
	User[unum].UI.cObserver = 1;
	ZeroMemory(&Ps2[epnum], sizeof(PLAYSTATE2));
	memcpy(&Ui[epnum], &User[unum].UI, sizeof(USERINFO));
	Ps2[epnum].In_OutCount = inOutCnt;// �ݺ� ����
		
	// �÷��̾� ��ȣ �ʱ�ȭ
	User[unum].PNum = epnum;
	// ### [�α� ��Ͽ�]###
	// ����� ���þ� �ʱ�ȭ
	User[unum].nBet = 0;

	// ### [ �ߺ��� IP�� ] ###
	IPARRINDEX iparray;
	ZeroMemory(&iparray, sizeof(iparray));
	for(  i=0;i<m_Max_Player;i++)
		iparray.IpIndex[i]= (char)Ps[i].nCheckIp;


	// ���� ������ ���� ����� ������ ������� �뺸�Ѵ�.
	if(faststrcmp(Ri.ID, User[unum].UI.ID) == 0) {
		int mastnum = -1;
		for(int i=0; i<m_Max_NewPlayer; i++)
			if(strlen(Ui[i].ID) != 0) { mastnum = i; break; }
			
			if(mastnum != -1) {
				// ### [ ������� ] ###
				//������̳� �� ���� ��Ų��
				
				strncpy(Ri.ID, Ui[mastnum].ID, 15);
				Ri.UNum = Ui[mastnum].UNum;
				
				// ������ �ٲ��� �˸���
				CSV_CHANGEROOMMASTER crmmsg(SndBuf);
				crmmsg.Set(Ri.RoomNum, Ui[mastnum].UNum, Ui[mastnum].ID);
				SendMsgToAll(crmmsg.pData, crmmsg.GetTotalSize());
				
				// [������Ŀ] ī�彺��Ī
				Ri.nCard = Ui[mastnum].PI.nCard;
			}
		/*	
			// [���� ������ �۾�]
			if(Ri.bSuperMaster) {
				// ���۹��̶�� ����
				if(!bSuperRoomDestory) {
					bSuperRoomDestory = TRUE;
					
					if(Ri.State != 0) {
						// �������̶�� ���� ���� �޽��� �˸�
						CSV_SERVERMSG smsg(SndBuf);
						smsg.Set(0, "���۹����� �����Ͽ����Ƿ� �̹� ������ ������ ��� �ڵ����� �����ϰ� �˴ϴ�.");
						SendMsgToAll(smsg.pData, smsg.GetTotalSize());
					}
				}
			}
		*/
	}
	
	// ���� �����ٸ� 
	if(faststrcmp(m_DealerID, User[unum].UI.ID) == 0) {
		strncpy(m_DealerID, Ri.ID, 15);
	}

	// ���� ���� �ٸ� ����ڿ��� ���ο� ������� ������ �뺸
	CSV_ENTER_EMERAGE entermsg(SndBuf);
	entermsg.Set(Ri.RoomNum, pnum, epnum, Ui[epnum].ID, &iparray);
	SendMsgExept(epnum, entermsg.pData, entermsg.GetTotalSize() );


	CSV_ACCEPT_EMERAGE emerage(SndBuf);
	emerage.Set(Ri.RoomNum, pnum, epnum, Ui[epnum].ID, &iparray);
	SendMsgTo(Ui[epnum].ID, emerage.pData, emerage.GetTotalSize());


	// ��� ����ڿ��� �������� ����Ǿ����� �˸�
	CSV_CHANGEROOM crmsg(SndBuf);
//	SMALLROOMINFO sri;
//	sri = GetSmallRoomInfo();
	crmsg.Set(&GetSmallRoomInfo());
	pChan->SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());

	// ���� ���� ī���� ����
	if(GameState == RSTATE_NONE && Ri.NowUserNum < MIN_PLAYER)
	{
		bRestartCnt=FALSE;
		RestartCnt=0;

		// ���۹�ư 
		int nPrimer    = GetPrimer(); // �����ϱ�
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,0);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}
	
	Reserve_takeaSeat();
}

// ### [ ������� ] ###
// ������ 
int CRoom::Room_MyIp_Check(int unum)
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// ### [ �ߺ��� IP�� ] ###
	int myIpIndex = -1;
	if(g_pMainView->Cfg2.SameIPLimit > 0) {
		int samecnt = 0;
		int tmpIp[MAX_PLAYER]={0,};
		
		// ���� IP �뿪�� � �����ϴ��� �˻�
		for(int i=0; i<m_Max_Player; i++) {
			int tarunum = Ui[i].UNum;
			if(tarunum<1 || tarunum >= MAX_USER) continue;
			if(User[unum].PeerIP.S_un.S_un_b.s_b1 == User[tarunum].PeerIP.S_un.S_un_b.s_b1 && 
				User[unum].PeerIP.S_un.S_un_b.s_b2 == User[tarunum].PeerIP.S_un.S_un_b.s_b2 && 
				User[unum].PeerIP.S_un.S_un_b.s_b3 == User[tarunum].PeerIP.S_un.S_un_b.s_b3)// &&
				//User[unum].PeerIP.S_un.S_un_b.s_b4 == User[tarunum].PeerIP.S_un.S_un_b.s_b4) // �׽�Ʈ
			{
				samecnt++;
				tmpIp[i]=1;
				myIpIndex = i;
			}
		}
		
		if(samecnt >= g_pMainView->Cfg2.SameIPLimit) {
			// �ߺ��� IP�� �����Ͽ� ���� �Ұ�
			//ErrCode = 15;
			myIpIndex = -2;
		}
		else if(samecnt !=0) // �߰� Ŭ���̾�Ʈ���� �̹����� �����ش�.(���� ����)
		{
			if(samecnt == 1) // ó��
			{
				for(int kk=0; kk < m_Max_Player;kk++)
				{
					if(m_aIpIndex[kk] == 0) break;
				}
				m_aIpIndex[kk] = 1;
				
				for(i=0;i<m_Max_Player;i++)
				{
					if(tmpIp[i] != 0) Ps[i].nCheckIp = kk;
				}
				myIpIndex = kk;
				m_nIpCnt++;
			}
			else
			{// ����
				myIpIndex = Ps[myIpIndex].nCheckIp;
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	return myIpIndex;
	
}



////////////////// ### [ ������� ] ### ////////////////////
// ���� ���� (���� => ����)
void CRoom::Make_a_Reservaiton(int pnum, int unum, int nSndFxKind)  // ��������
{
	RESERVEDINFO *pList = (RESERVEDINFO*) new RESERVEDINFO;
	pList->PNum = pnum;
	pList->UNum = unum;
	pList->nSndFxKind = nSndFxKind;
	pList->pUI = &Ui[pnum];
	m_ReservedList.AddTail(pList); 
	GetReservaitonInfo();

	CString str;
	str.Format("�����ڷ� ����Ǿ����ϴ�. %d����", GetReservationSequential() );

	CSV_OBSERVER_MESSAGE smsg2(SndBuf);
	smsg2.Set(0, (char*) str.operator LPCTSTR() );
	User[unum].SendMsg(&smsg2);
}

//�������
int CRoom::GetReservationSequential()
{
	return m_ReservedList.GetCount();
}


void CRoom::Cancel_a_Reservation(char *id)  //�������� ���
{	
	POSITION pos1, pos2;
	RESERVEDINFO* pa;
	for( pos1 = m_ReservedList.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		if( strcmp( ((RESERVEDINFO*)m_ReservedList.GetNext( pos1 ))->pUI->ID, id)==0 )
		{
			pa =(RESERVEDINFO*)m_ReservedList.GetAt( pos2 ); // Save the old pointer for
			//deletion.
			m_ReservedList.RemoveAt( pos2 );
			delete pa; // Deletion avoids memory leak.
		}
	}
#ifdef _DEBUG
	afxDump.SetDepth( 1 );
	afxDump << "RemoveAt example: " << &m_ReservedList << "\n";
#endif

	GetReservaitonInfo();
}

void CRoom::Reserve_takeaSeat() // �ڸ� �ɱ�
{
	if(Ri.NowUserNum < Ri.MaxUserNum && m_ReservedList.GetCount() > 0){
		RESERVEDINFO *pList =  (RESERVEDINFO*)m_ReservedList.GetHead();
		m_ReservedList.RemoveHead();
		Participation(pList->UNum, pList->PNum, pList->nSndFxKind);
		delete pList;
		GetReservaitonInfo();
	}
}

BOOL CRoom::ReservationFindID(char *id) // ���� �Ǿ� �ִ°�
{
	RESERVEDINFO *pList = NULL;
	POSITION pos = m_ReservedList.GetHeadPosition();
	while(pos)	{
		pList = (RESERVEDINFO*)m_ReservedList.GetNext(pos);
		if(pList != NULL) {
			if(strcmp(pList->pUI->ID, id)==0) return TRUE;
		}
	}
	return FALSE;
}

void CRoom::ReservedRemoveAll() // ����
{
	RESERVEDINFO *pDelList = NULL;
	POSITION pos = m_ReservedList.GetHeadPosition();
	while(pos)	{
		pDelList = (RESERVEDINFO*)m_ReservedList.GetNext(pos);
		if(pDelList != NULL) {
			delete pDelList;
		}
	}
	m_ReservedList.RemoveAll();
	ASSERT( m_ReservedList.IsEmpty() );
}

void CRoom::GetReservaitonInfo() // ��������
{
	if(m_ReservedList.GetCount()==0) return;
	int cnt = 0;
	RESERVATIONINFO sInfo[MAX_OBSERVER] = {0,};
	
	RESERVEDINFO *pList = NULL;
	POSITION pos = m_ReservedList.GetHeadPosition();
	while(pos)	{
		pList = (RESERVEDINFO*)m_ReservedList.GetNext(pos);
		if(pList != NULL) {
			sInfo[cnt].Seq = cnt; //�������
			sInfo[cnt].UNum = pList->UNum;
			strncpy(sInfo[cnt].ID, pList->pUI->ID, 15);
			cnt++;
#ifdef _DEBUG
			if(cnt >= MAX_OBSERVER -1) {
				AfxMessageBox("�����ʰ�");
			}
#endif
		}
	}
	if(cnt == 0) return;
	CSV_RESERVATIONINFO rmsg(SndBuf);
	rmsg.Set(cnt, sInfo);
	SendMsgToAll(rmsg.pData, rmsg.GetTotalSize() );
}


////////////////////////////////////
//���� -> ���� ����
void CRoom::Make_a_Observer(int pnum, int unum)
{
	RESERVEDINFO *pList = (RESERVEDINFO*) new RESERVEDINFO;
	pList->PNum = pnum;
	pList->UNum = unum;
	pList->pUI = &Ui[pnum];
	m_ObservedList.AddTail(pList); 

	CSV_OBSERVER_MESSAGE smsg2(SndBuf);
	smsg2.Set(0, "���� ������ �Ͽ����ϴ�. ���� ���� �� ���� ���·� �̵� �մϴ�.");
	User[unum].SendMsg(&smsg2);
}

void CRoom::Cancel_a_Observer(char *id)
{
	POSITION pos1, pos2;
	RESERVEDINFO* pa;
	for( pos1 = m_ObservedList.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		if( strcmp( ((RESERVEDINFO*)m_ObservedList.GetNext( pos1 ))->pUI->ID, id)==0 )
		{
			pa =(RESERVEDINFO*)m_ObservedList.GetAt( pos2 ); // Save the old pointer for
			//deletion.
			m_ObservedList.RemoveAt( pos2 );
			delete pa; // Deletion avoids memory leak.
		}
	}
}


BOOL CRoom::Observer_FindID(char *id)
{
	RESERVEDINFO *pList = NULL;
	POSITION pos = m_ObservedList.GetHeadPosition();
	while(pos)	{
		pList = (RESERVEDINFO*)m_ObservedList.GetNext(pos);
		if(pList != NULL) {
			if(strcmp(pList->pUI->ID, id)==0) return TRUE;
		}
	}
	return FALSE;
}

void CRoom::ObserverRemoveAll()
{
	RESERVEDINFO *pDelList = NULL;
	POSITION pos = m_ObservedList.GetHeadPosition();
	while(pos)	{
		pDelList = (RESERVEDINFO*)m_ObservedList.GetNext(pos);
		if(pDelList != NULL) {
			delete pDelList;
		}
	}
	m_ObservedList.RemoveAll();
	ASSERT( m_ReservedList.IsEmpty() );
}

void CRoom::Observer_takeaSeat(char *id)
{
	if( Observer_FindID(id) ) {
		if(Ri.NowObserverNum < MAX_OBSERVER-1) {
			RESERVEDINFO *pList = NULL;
			POSITION pos = m_ObservedList.GetHeadPosition();
			while(pos)	{
				pList = (RESERVEDINFO*)m_ObservedList.GetNext(pos);
				if(pList != NULL) {
					if(strcmp(pList->pUI->ID, id)==0) 
					{
						int unum = pList->UNum;
						int pnum = pList->PNum;
						Cancel_a_Observer(id); //����
						Emerge_Out(unum, pnum);
						return;
					}
				}
			}
		}
	}
}

//������������ �̵�
void CRoom::Want_Observer_Send()
{
	for(int i=0;i<m_Max_Player;i++) {
		if(strlen(Ui[i].ID) > 0) {
			if(Ps[i].bOnGame && !Ps[i].bFold) continue;
			Observer_takeaSeat(Ui[i].ID);
		}
	}
}






////////////////////////////////////////////////////////////
//////////////// ### [������ ��� �۾�] ### ////////////////
////////////////////////////////////////////////////////////

void CRoom::Admin_Room_Remove()
{
	for(int i=0; i<m_Max_NewPlayer; i++) {
		if(strlen(Ui[i].ID) > 0) {
			int unum = Ui[i].UNum;
			if(unum < 1 || unum >= MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;
			
			// ����ڿ��� �����޼����� ����
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, "�����ڿ� ���� ���� �����Ǿ����ϴ�.");
			User[unum].SendMsg(&smsg);

			OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
		}
	}
}

//[������] ��ɼ��� ���� �Ѵ�
void CRoom::ChangeRoomOption(CCL_ASK_CHSTYLEROOMINFO *pMsg)
{
	if(bValid == FALSE) return;
	strncpy(Ri.Title, pMsg->CHRI->Title,30);

	CHANGESTYLEROOMINFO croom={0,};
	strncpy(croom.ID, Ri.ID, 16);
	croom.UNum = Ri.UNum;
	croom.nCard = 0;
	croom.RoomNum = Ri.RoomNum;
	memcpy(croom.Title, Ri.Title, 30);

	// ���� ���ο����� �˸�.
	CSV_CHANGESTYLEROOMINFO scrm(SndBuf);
	scrm.Set(&croom);
	SendMsgToAll(scrm.pData, scrm.GetTotalSize());

	// ���ǿ� �˸�.
	pChan->SendMsgToLoby(scrm.pData, scrm.GetTotalSize());

}

// [������ ��� �۾�] ��ɼ��� �����Ѵ�
void CRoom::ChangeRoomOption_ADMIN(CNM_ADMIN_CHANGEROOM *pMsg)
{
	if(bValid == FALSE) return;
	strncpy(Ri.Title, pMsg->STYLE->Title,30);

	CString str;
	str = "��ڿ� ���� ������ ����Ǿ����ϴ�";

	CHANGESTYLEROOMINFO croom={0,};
	strncpy(croom.ID, Ri.ID, 16);
	croom.UNum = Ri.UNum;
	croom.nCard = 0;
	croom.RoomNum = Ri.RoomNum;
	memcpy(croom.Title, Ri.Title, 30);

	// ���� ���ο����� �˸�.
	CSV_CHANGESTYLEROOMINFO scrm(SndBuf);
	scrm.Set(&croom);
	SendMsgToAll(scrm.pData, scrm.GetTotalSize());

	// ���ǿ� �˸�.
	pChan->SendMsgToLoby(scrm.pData, scrm.GetTotalSize());
}


// ### [�α� ��Ͽ�]###
void CRoom::LogRecordFunc(int unum, int pnum)
{
	if(unum<1 || unum >= MAX_USER) return;
	if(pnum<0 || pnum >= m_Max_Player) return;

	if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse) {
		
		if(User[unum].UI.PMoney < 0) User[unum].UI.PMoney = 0;
		INT64 CurMoney = User[unum].nBet - User[unum].UI.PMoney;
		if(pnum < m_Max_Player && Ps[pnum].bFold && CurMoney > g_LogRangeValue) {// ### [ ������� ] ###   // 10�� ���� 1��
			
			if(LogDBMan.DBQue.GetTotNum() >= g_pMainView->Cfg2.LoginDBQueNum2) {
				CLogFile logfile("LogRecordDB.txt");
				logfile.Writef("LogRecordDB ���� 01, id=%s, curmoney=%I64d, losemoney=%I64d, winid=netmarble",Ui[pnum].ID, User[unum].UI.PMoney, CurMoney);
			}
			else {
				// ������ ����ũ�� �����ϱ����� Time Tick
				//int StartTick = timeGetTime();
				
				LOGREC_DBWORKS LWork;
				ZeroMemory(&LWork, sizeof(LOGREC_DBWORKS));
				LWork.StartTick = timeGetTime();
				LWork.bValid = TRUE;
				LWork.WorkKind = 1; // [ ����� ] <-----------------------------�߰�
				
			//	LWork.LogRecord.Status = 72; // ������ ���� ����
				LWork.LogRecord.Status = 2; // ������ ���� ����
				
				strncpy(LWork.LogRecord.ID, User[unum].UI.ID, 15);
				LWork.LogRecord.CurMoney = User[unum].UI.PMoney;
				LWork.LogRecord.LoseMoney = CurMoney;
				
				strcpy(LWork.LogRecord.Win_ID, "netmarble");
				LWork.LogRecord.Win_CurMoney = -9999; //?????
				LWork.LogRecord.Win_GetMoney = -9999;
				
				strcpy(LWork.LogRecord.My_ip, User[unum].UserIP);
				//strcpy(LWork.LogRecord.Winner_ip, "netmarble");
				wsprintf(LWork.LogRecord.Winner_ip,"%d����", Ri.RoomNum);
				
				
				// DB ������ ����
				BOOL rtn = LogDBMan.SetDBWork(&LWork);
				if(rtn == FALSE) {
					CLogFile logfile("LogRecordDB.txt");
					logfile.Writef("LogRecordDB ���� 02, id=%s, curmoney=%I64d, losemoney=%I64d, winid=netmarble",Ui[pnum].ID, User[unum].UI.PMoney, CurMoney);
				}
			}
		}
	}
}
