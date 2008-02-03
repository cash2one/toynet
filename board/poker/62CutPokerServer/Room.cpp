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


// ### [이벤트 티켓] ###
#define EVENT_GAMECOUNT 30 // 30판수
#define EVENT_GIVECOIN  3  // 2개 지급


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRoom::CRoom()
{
	pChan = NULL;
//	SndBuf = NULL;
	
	// 5, 7인용 작업
	m_Max_Player	= MAX_PLAYER - 1;
	m_Max_NewPlayer = m_Max_Player + MAX_OBSERVER;

	Clear();
}

CRoom::~CRoom()
{

}

// 5,7 인용 결정
void CRoom::set_player_usernum(int channel_usernum)
{
	if(channel_usernum==0) { // 방만들기 우선
		if(Ri.bPlayer==0 ) { // 5인용
			m_Max_Player	= MAX_PLAYER - 1;
			m_Max_NewPlayer = m_Max_Player + MAX_OBSERVER;
		} else {			// 6인용
			m_Max_Player	= MAX_PLAYER;
			m_Max_NewPlayer = m_Max_Player + MAX_OBSERVER;
		}
	} else if(channel_usernum==1)  // 채널 강제 셋팅 5인용
	{
		m_Max_Player	= MAX_PLAYER - 1;
		m_Max_NewPlayer = m_Max_Player + MAX_OBSERVER;
	} else {						// 채널 강제 셋팅 6인용

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
	ZeroMemory(Us, sizeof(Us));	// [게임 아이템 작업]
	bSuperRoomDestory = FALSE;	// [게임 아이템 작업]
	BanUserList.Clear();		// [게임 아이템 작업]
	VoteBanUserList.Clear();		
					

	ZeroMemory(m_DealerID, sizeof(m_DealerID));
	//ZeroMemory(m_PlayerChangeCradDeck, sizeof(m_PlayerChangeCradDeck));

	// ### [ 관전기능 ] ###
	bNowUserEmptyDestory = FALSE;
	ReservedRemoveAll(); //참여예약
	ObserverRemoveAll();
	m_AdminCount = 0;	// 현재 접속한 관리자 수


	////// 게임 진행 관련 변수들
	StartUserNum = 0;
	bRestartCnt=FALSE;
	RestartCnt=0;
	bWaitGameOver = FALSE;
	WaitGameOverCnt = 0;
	m_JackPotActive = 0;

	GameState = RSTATE_NONE;
	m_CurPlayer = -1;
	m_GameState = GAME_ROUND_READY;// 0: 1:카드 3장, 2: 카드 선택, 3: 4장, 4: 5장, 5: 6장, 6: 히든, 7: 승결정

	////// 추방 투표 관련 변수들
	BanishVoteState = 0;
	BanishVoteCnt = 0;
	BanishVoteAgreeNum = 0;
	BanishVoteDisgreeNum = 0;
	ZeroMemory(&BVI, sizeof(BANISHVOTEINFO));
	
	// 카드 배열
	ZeroMemory(m_CardDeck, sizeof(m_CardDeck));
	m_CurCardIndex = 0;
	m_nPrimeUser = 0;
	ResetGame();
	
	// ### [ 중복된 IP가 ] ###
	m_nIpCnt = 0;
	ZeroMemory(m_aIpIndex, sizeof(m_aIpIndex));
	for(int i=0; i<m_Max_Player; i++) {
		Ps[i].nCheckIp = -1; // $$$  기본값 -1
	}
}

int CRoom::GetPNum(char* id)
{
	if(id==NULL) return -1;
	for(int i=0; i<m_Max_NewPlayer; i++)// ### [ 관전기능 ] ### 
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
	m_SRi.NowObserverNum = Ri.NowObserverNum; // ### [ 관전기능 ] ### 

	// [ ###잭팟### ]
	m_SRi.JackPotMoney = g_JackPotMoney; // (로비 유저)방정보 변경시 같이 보낸다.

	return m_SRi;
}

const ENTERROOMINFO CRoom::GetEnterRoomInfo()
{
	ENTERROOMINFO ei;
	memcpy(&ei.Ri, &Ri, sizeof(Ri));
	memcpy(ei.Ui, Ui, sizeof(Ui));
	POKERGAME Pg = MakeInfo();
	// 겜중에 들어온다 (선을 보내준다)
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
	for(int i=0; i<m_Max_NewPlayer; i++) { // ### [ 관전기능 ] ###
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
	if(pnum < 0 || pnum >= m_Max_NewPlayer) return FALSE;  // ### [ 관전기능 ] ###
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
	for(int i=0; i<m_Max_NewPlayer; i++) {// ### [ 관전기능 ] ###
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
	if(pnum<0 || pnum>=m_Max_NewPlayer) return FALSE; // ### [ 관전기능 ] ###
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
	for(int i=0; i<m_Max_NewPlayer; i++) { // ### [ 관전기능 ] ###
		if(strlen(Ui[i].ID) > 0) {
			if(Ui[i].UNum<1 || Ui[i].UNum>=MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[Ui[i].UNum].UI.ID) != 0) continue;
			int sid = User[Ui[i].UNum].Sid;
			SockMan.SendData(sid, pMsgData, nMsgSize);
		}
	}
	return TRUE;
}

// ### [ 관전기능 ] ### // ### [ 중복된 IP가 ] ### // ### [사운드 추가 작업] ###
BOOL CRoom::OnUserEnterRoom(USERINFO *pUI, int pnum, int myindex, int sndFxKind)
{
	if(pUI == NULL) return FALSE;
	if(pUI->UNum < 1 || pUI->UNum >= MAX_USER) return FALSE;
	if(pnum<0 || pnum>=m_Max_NewPlayer) return FALSE;
	if(strlen(Ui[pnum].ID) > 0) return FALSE;	

	if(pnum < m_Max_Player) {
		if(Ri.NowUserNum+1 > Ri.MaxUserNum) return FALSE;
		if(pnum < 0 || pnum > Ri.MaxUserNum) return FALSE;
		// 방 정보 갱신
		Ri.NowUserNum++;
	}
	else {
		if( pnum != m_Max_NewPlayer-1 ) Ri.NowObserverNum++;
		else m_AdminCount++; // 현재 접속한 관리자 수
		User[pUI->UNum].bObserver = TRUE;
		User[pUI->UNum].UI.cObserver = 1;
		pUI->cObserver = 1;
	}
	
	memcpy(&Ui[pnum], pUI, sizeof(USERINFO));// 사용자 정보를 게임 정보에 저장한다
	User[pUI->UNum].PNum = pnum;			 // 플레이어 번호 초기화
	// ### [로그 기록용]###
	User[pUI->UNum].nBet = 0;				// 백업용 베팅액 초기화

	// 게임정보를 클리어
	if(pnum < m_Max_Player) {
		ZeroMemory(&Ps[pnum], sizeof(PLAYSTATE));
		ZeroMemory(&Bc[pnum], sizeof(BACKCARD));
		Ps[pnum].nSndFxKind = sndFxKind; // ### [사운드 추가 작업] ###
		Ps[pnum].nCheckIp = myindex;	 // ### [ 중복된 IP가 ] ###
	}
	ZeroMemory(&Ps2[pnum], sizeof(PLAYSTATE2)); // 반복횟수 추가로 인하여~
	IPARRINDEX iparray; 
	ZeroMemory(&iparray, sizeof(iparray));
	for(int i=0;i<m_Max_Player;i++) {	iparray.IpIndex[i]= (char)Ps[i].nCheckIp; }

	int rnum = Ri.RoomNum;
	// 같은 방의 다른 사용자에게 새로운 사용자의 입장을 통보
	CSV_USERENTERROOM uermsg(SndBuf);
	uermsg.Set(rnum, pUI, &iparray, sndFxKind); // ### [ 중복된 IP가 ] ### // ### [사운드 추가 작업] ###
	SendMsgExept(pnum, uermsg.pData, uermsg.GetTotalSize());

	// 게임 시작 카운터 가동 (시작버튼 삽입)
	
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
		// 시작버튼 
		int nPrimer    = GetPrimer(); // 선구하기
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,1);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}	
	return TRUE;
}


// ### [ 관전기능 ] ### 
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

	// 사용자의 플레이 번호를 구한다
	int pnum = GetPNum(id);
	if(pnum == -1) {
		CLogFile logfile;
		logfile.Writef("OnUserOutRoom() - Failed out room user (unum=%d, ID=%s)", unum, User[unum].UI.ID);
		return FALSE;
	}

	// 게임중이던 사용자이면, 게임을 포기시킨다 // ### [ 관전기능 ] ### 
	if(pnum < m_Max_Player && Ri.State!=0 && Ps[pnum].bOnGame && !Ps[pnum].bFold) {
		FoldUser(pnum, TRUE);
	}	
	
	if(User[unum].UI.PMoney < Ri.BetMoney)//나가기 요청시 오링이면 파산 메세지 날림
	{
		CSV_SERVERMSG smsg(SndBuf);
		CString strMess = "";
		smsg.Set(9, (char*)strMess.operator LPCTSTR()); // ## 파산
		User[unum].SendMsg(&smsg);	
	}

	// 접속 종료가 아니면 방에서 나감을 허가
	if(bDisconnect == FALSE) {
		CSV_ACCEPT_OUTROOM aormsg(SndBuf);
		aormsg.Set();
		SockMan.SendData(User[unum].Sid, aormsg.pData, aormsg.GetTotalSize());
	}		

	if(GameState == RSTATE_CHANGECARD)//[62]
	{	
		//62cut 플래그 초기화[62]
		Ps2[pnum].nChangeFlg = FALSE;
//		Ps2[pnum].nChangeWinEndFlg = FALSE;
		// 바꾸기 일때 레이스 체크
		ChangeCardRaiseCheck();	//[62]
	}


	// 추방 예약/진행중이면
	if(pnum < m_Max_Player && BanishVoteState) {// ### [ 관전기능 ] ### 
		if(faststrcmp(Ui[pnum].ID, BVI.MyID)==0 || faststrcmp(Ui[pnum].ID, BVI.TarID)==0) {
			// 사용자에게 서버메세지를 보냄
			CSV_SERVERMSG smsg(SndBuf);
			if(BanishVoteState==1) smsg.Set(0, "추방 투표 예약이 취소되었습니다");
			else smsg.Set(0, "추방 투표가 취소되었습니다");
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());

			// 추방 요청자나 대상자가 퇴실하는 경우 추방 취소
			BanishVoteState = 0;
			ZeroMemory(&BVI, sizeof(BANISHVOTEINFO));
		}
		else if(BanishVoteState==2 && Ps2[pnum].bJoinBanishVote) {
			int totjoin = 0;
			for(int i=0; i<m_Max_Player; i++) if(Ps2[i].bJoinBanishVote) totjoin++;
			if(totjoin-1 < BANISHVOTE_NUM) {
				// 사용자에게 서버메세지를 보냄
				CSV_SERVERMSG smsg(SndBuf);
				smsg.Set(0, "투표 인원이 3명 미만이므로 추방 투표가 취소되었습니다");
				SendMsgToAll(smsg.pData, smsg.GetTotalSize());

				// 투표가 진행중인 상황에서 현재 투표 참여 인원이 4명 이하면 투표 취소
				BanishVoteState = 0;
				ZeroMemory(&BVI, sizeof(BANISHVOTEINFO));
			}
		}
	}

	LogRecordFunc(unum, pnum);


	// ### [ 중복된 IP가 ] ###
	if(pnum < m_Max_Player && Ps[pnum].nCheckIp >= 0) // ### [ 관전기능 ] ### 
	{
		int ipindex= Ps[pnum].nCheckIp;
		int indexCnt=0;
		for(int i=0;i<m_Max_Player;i++) {
			if(pnum==i) continue;
			if(ipindex == Ps[i].nCheckIp) indexCnt++;
		}
		// 제거
		if(indexCnt == 1)	
		{
			for(i=0;i<m_Max_Player;i++) {
				if(pnum==i) continue;
				if(ipindex == Ps[i].nCheckIp) Ps[i].nCheckIp = -1; // 모두 해제
			}

			m_aIpIndex[ipindex] = 0;
			m_nIpCnt--;
		}
	}

	// 유저 정보를 삭제하고 게임참여상태를 클리어
	if(pnum < m_Max_Player) {
		ZeroMemory(&Ps[pnum], sizeof(PLAYSTATE));
		ZeroMemory(&Bc[pnum], sizeof(BACKCARD));
		Ps[pnum].nCheckIp = -1; // ### [ 중복된 IP가 ] ### $$$ 기본값 -1
	}
	// ### [ 관전기능 ] ### 
	Cancel_a_Observer(User[unum].UI.ID);//참여->관전 예약취소	
	Cancel_a_Reservation(User[unum].UI.ID);//관전->참여 예약취소	

	ZeroMemory(&Ps2[pnum], sizeof(PLAYSTATE2));
	ZeroMemory(&Us[pnum], sizeof(USERSTATE));	// [게임 아이템 작업]
	ZeroMemory(&Ui[pnum], sizeof(USERINFO));	// 사용자 삭제
	User[unum].bExitSubscript = FALSE;			// 나가기 예약상태 초기화	

	// 현재인원에서 한명 삭제
	if(User[unum].bObserver) {
		if( !User[unum].bAdminUser ) Ri.NowObserverNum--;
		else m_AdminCount--; // 현재 접속한 관리자 수
	}
	else
		Ri.NowUserNum--;


	///방인원을 채크 MIN_PLAYER 명 보다 적으면 시작 카운트 중이 (시작버튼)
	if(Ri.NowUserNum < MIN_PLAYER)
	{
		bRestartCnt = FALSE;
		RestartCnt  = 0;

		// 시작버튼 
		int nPrimer    = GetPrimer(); // 선구하기
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,0);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}

	// 5명이였다가 한명이 나가도 시작버튼 활성화 시킨다.cnt = 0으로 초기 
	if(GameState==RSTATE_NONE && Ri.NowUserNum >= MIN_PLAYER && Ri.NowUserNum < Ri.MaxUserNum) 
	{
		bRestartCnt = TRUE;
		RestartCnt  = 0;

		// 시작버튼 
		int nPrimer    = GetPrimer(); // 선구하기
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,1);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}

	// 방에 남은 사용자가 없으면 방을 삭제한다
	if(Ri.NowUserNum<=0 && Ri.NowObserverNum <=0 && m_AdminCount==0) // ### [ 관전기능 ] ### 
	{
		//Admin_Room_Remove(); // 현재 관리자가 접속해 있을수 다
		Ri.NowUserNum=0;
		// 모든 사용자에게 방정보가 삭제되었음을 알림
		CSV_OUTROOM ormsg(SndBuf);
	//	SMALLROOMINFO sri;
	//	sri = GetSmallRoomInfo();
		ormsg.Set(&GetSmallRoomInfo());
		pChan->SendMsgToLoby(ormsg.pData, ormsg.GetTotalSize());
		// 방 삭제
		Clear();
	}
	else
	{
		// ### [ 중복된 IP가 ] ###
		IPARRINDEX iparray;
		ZeroMemory(&iparray, sizeof(iparray));
		for( int i=0;i<m_Max_Player;i++)
			iparray.IpIndex[i]= (char)Ps[i].nCheckIp;

		// 같은 방 플레이어에게 퇴장했음을 알린다
		CSV_USEROUTROOM uomsg(SndBuf);
		uomsg.Set(rnum, unum, User[unum].UI.ID, &iparray);
		SendMsgToAll(uomsg.pData, uomsg.GetTotalSize());
		
		// 만일 방장이 나간 경우라면 방장이 변경됨을 통보한다.
		if(faststrcmp(Ri.ID, User[unum].UI.ID) == 0) {
			int mastnum = -1;
			for(int i=0; i<m_Max_NewPlayer; i++)
				if(strlen(Ui[i].ID) != 0) { mastnum = i; break; }

			if(mastnum != -1) {
				// ### [ 관전기능 ] ###
				//대기중이넘 겜 참여 시킨다
				strncpy(Ri.ID, Ui[mastnum].ID, 15);
				Ri.UNum = Ui[mastnum].UNum;
				// 방장이 바뀜을 알린다
				CSV_CHANGEROOMMASTER crmmsg(SndBuf);
				crmmsg.Set(rnum, Ui[mastnum].UNum, Ui[mastnum].ID);
				SendMsgToAll(crmmsg.pData, crmmsg.GetTotalSize());
				// [세븐포커] 카드스위칭
				Ri.nCard = Ui[mastnum].PI.nCard;
			}

			// [게임 아이템 작업]
			if(Ri.bSuperMaster) {
				// 슈퍼방이라면 방폭
				if(!bSuperRoomDestory) {
					bSuperRoomDestory = TRUE;

					if(Ri.State != 0) {
						// 게임중이라면 방폭 예약 메시지 알림
						CSV_SERVERMSG smsg(SndBuf);
						smsg.Set(0, "슈퍼방장이 퇴장하였으므로 이번 게임이 끝나면 모두 자동으로 퇴장하게 됩니다.");
						SendMsgToAll(smsg.pData, smsg.GetTotalSize());
					}
				}
			}
			// ### [ 관전기능 ] ### 
			else if(Ri.NowUserNum == 0)	{
				bNowUserEmptyDestory = TRUE;// 게임중이라면 방폭
			}
		}

		// 선이 나갔다면 
		if(faststrcmp(m_DealerID, User[unum].UI.ID) == 0) {
			strncpy(m_DealerID, Ri.ID, 15);
		}

		// 남은 인원이 1명보다 적거나 같다면 게임이 중단됨
		if(Ri.NowUserNum <= 1) {
			BOOL bOpen = FALSE;
			if(CheckGameOver(bOpen)) DoGameOver(bOpen);
		}

		// 모든 사용자에게 방정보가 변경되었음을 알림
		CSV_CHANGEROOM crmsg(SndBuf);
	//	SMALLROOMINFO sri;
	//	sri = GetSmallRoomInfo();
		crmsg.Set(&GetSmallRoomInfo());
		pChan->SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());
	}

	User[unum].PNum = -1;	// 플레이어 번호 초기화
	// ### [로그 기록용]###
	User[unum].nBet = 0;	// 백업용 배팅액 초기화

	// 사용자의 위치가 변경됐음
	User[unum].SUI.Position = WH_LOBY;				// 현재 위치는 대기실이다
	User[unum].SUI.RoomNum = 0;						// 현재 위치한 방번호(쓰레기값)
	User[unum].bLobyOpen = FALSE;
	// ### [ 관전기능 ] ### 
	User[unum].UI.cObserver = 0;
	User[unum].bObserver = FALSE;
	// 레벨상점 폐쇄
	int level = GetPokerLevel(User[unum].UI.PMoney);
	User[unum].UI.nIcon = level;
	User[unum].SUI.nIcon = level;

	// 접속 종료가 아니면 사용자 정보 변경을 알림
	if(bDisconnect == FALSE) {
		CSV_CHANGEUSER cumsg(SndBuf);
		cumsg.Set(&User[unum].SUI); // [ 잭팟 ] 갱신
		pChan->SendMsgToLobyOpen(cumsg.pData, cumsg.GetTotalSize());
	}
	
	// ### [ 관전기능 ] ### 
	Reserve_takeaSeat(); // 자리 앉기
	return TRUE;
}

// [게임 아이템 작업]
void CRoom::OnForceBanish(CSV_ASK_FORCEBANISH *pMsg)
{
	// 슈퍼방이 아니면 취소
	if(!Ri.bSuperMaster) return;

	// 존재하지 않는 사용자면 취소
	int tpnum = GetPNum(pMsg->ID);
	if(tpnum == -1) return;


	if(Ri.State == 0) {
		// 대기중이라면 즉시 강제 퇴장
		DoForceBanish(tpnum);
	}
	else {
		// 게임중이면 강퇴 예약
		if(!Us[tpnum].bForceBanish) {
			
			Us[tpnum].bForceBanish = TRUE;

			CString str;
			str.Format("[%s]님 강제추방 예약", Ui[tpnum].ID);
			// 사용자에게 서버메세지를 보냄
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(0, (char*)(LPCTSTR)str);
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		}

		return;
	}
}

// [게임 아이템 작업]
void CRoom::DoForceBanish(int pnum)	
{
	//if(!Us[pnum].bForceBanish) return;
	if(strlen(Ui[pnum].ID) == 0) return;

	int tarunum = Ui[pnum].UNum;
	if(tarunum<1 || tarunum>=MAX_USER) return;
	if(strcmp(User[tarunum].UI.ID, Ui[pnum].ID) != 0) return;

	// 강퇴자 리스트에 추가
	BanUserList.AddID(User[tarunum].UI.ID);

	CString str;
	str.Format("[%s]님을 강제추방합니다.", User[tarunum].UI.ID);
	CSV_SERVERMSG smsg(SndBuf);
	smsg.Set(0, (char*)str.operator LPCTSTR());
	SendMsgToAll(smsg.pData, smsg.GetTotalSize());
	
	// 사용자에게 서버메세지를 보냄
	CSV_SERVERMSG smsg2(SndBuf);
	smsg2.Set(1, "슈퍼 방장에 의하여 강제 추방되었습니다");
	User[tarunum].SendMsg(&smsg2);
	
//	RestartCnt = 0;//강제  추방으로 시작 카운트가 리셋 된다.
	// 강제 추방
	OnUserOutRoom(tarunum, User[tarunum].UI.ID, FALSE);
}


////////////////////////////////////////////////////////////////////////////////////////

// ### [ 관전기능 ] ### 
void CRoom::OnGlobalGameTimer()
{
	// 존재하지 않는 플레이어의 경우 강제 삭제
	for(int i=0; i<m_Max_NewPlayer; i++)// ### [ 관전기능 ] ### 
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
					logfile.Writef("에러~~~~ CRoom::OnGlobalGameTimer()");
					
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
						// ### [ 중복된 IP가 ] ###
						Ps[i].nCheckIp = -1;
						for( int kk=0;kk<m_Max_Player;kk++)
							iparray.IpIndex[kk]= (char)Ps[kk].nCheckIp;
					}
					ZeroMemory(&Ps2[i], sizeof(PLAYSTATE2));
					ZeroMemory(&Us[i], sizeof(USERSTATE));	// [게임 아이템 작업]
					
					// 같은 방 플레이어에게 퇴장했음을 알린다
					CSV_USEROUTROOM uomsg(SndBuf);
					uomsg.Set(Ri.RoomNum, Ui[i].UNum, Ui[i].ID, &iparray);
					SendMsgToAll(uomsg.pData, uomsg.GetTotalSize());
					
					if(Ri.NowUserNum<=0 && Ri.NowObserverNum <=0 && m_AdminCount==0) {
						// 모든 사용자에게 방이 삭제됨을 알림
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
			else if(Ri.State==1 && i<m_Max_Player)// ### [ 관전기능 ] ###
			{
				{	
					// 게임참여중이고...
					if(Ps[i].JoinState == 1 && Ps[i].PlayState == 1 && Ps[i].bOnGame && !Ps[i].bFold ) 
					{
						// 게임참여중이고...
						if( Ps2[i].bWindOn ) // 카드 날리기
						{
							Ps2[i].nWindOn++;
							if(Ps2[i].nWindOn>=MAX_WINDTIMEOUT) { // 사용자텬을 지나간다 18초
								
								CString strMess = "일정시간 동안 응답이 없으므로 강제 종료합니다.";//g_StrMan.Get(_T("ROOM_TIMEOUT"));
								CSV_SERVERMSG smsg(SndBuf);
								smsg.Set(1, (char*)strMess.operator LPCTSTR());
								SendMsgTo(Ui[i].ID, smsg.pData, smsg.GetTotalSize());
								
								Ps2[i].bWindOn = FALSE;
								Ps2[i].nWindOn = 0;
								
								// 강제 퇴장 처리
								OnUserOutRoom(Ui[i].UNum, Ui[i].ID, FALSE);
							}
						}
						
						
						if( Ps2[i].bActive ) 
						{
							Ps2[i].nActive++;

							if(Ps2[i].nActive>=MAX_TURNTIMEOUT) { // 15사용자텬을 지나간다
									FoldUser(i);
									// ### [ 관전기능 ] ###
									Observer_takeaSeat(Ui[i].ID);									
							}

							/*
							if(GameState == RSTATE_CHANGECARD){
								if(Ps2[i].nActive>=MAX_TURNTIMEOUT) { // 10사용자텬을 지나간다
									//카드 바꾸기 일때 일정 시간을 초과하게 되면 경기 종료~~
									if(Ps2[i].nChangeFlg != TRUE)
									{
										FoldUser(i);
									// ### [ 관전기능 ] ###
										Observer_takeaSeat(Ui[i].ID);									
									}
								}
							}
							else
							{
								if(Ps2[i].nActive>=MAX_TURNTIMEOUT) { // 15사용자텬을 지나간다
									FoldUser(i);
									// ### [ 관전기능 ] ###
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
		
	
	if(bWaitGameOver==TRUE)		// 게임 종료 카운팅
	{
		WaitGameOverCnt++;
	//	if(WaitGameOverCnt>m_JackPotActive+6){
		if(WaitGameOverCnt>m_JackPotActive+5){
			SendReset(); // 카드 리셋명령 전송
			bWaitGameOver = FALSE;
			WaitGameOverCnt=0;
			m_JackPotActive = 0;
			
			// ### [ 관전기능 ] ###
			Want_Observer_Send();	// 관전이동 원하는 쉐이들 이동시킨다
			
		}
	}
	else if(Ri.State == 0 && bRestartCnt==TRUE && BanishVoteState!=2)	// 재시작 준비 카운터
	{
		RestartCnt++;
		
		if(RestartCnt >= 4 && RestartCnt <= 6){			
			// 사용자에게 서버메세지를 보냄
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
							str.Format("%d초 후에 게임이 자동 시작합니다.   시작버튼을 눌러주세요",num);
							smsg.Set(30, (char*)str.operator LPCTSTR());
							SendMsgTo(Ui[i].ID,smsg.pData,smsg.GetTotalSize());
						}						
						else
						{
							str.Format("%d초 후에 게임이 자동 시작합니다.",num);
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
	else if(bRestartCnt==TRUE)	// 재시작 준비 카운터
	{
		RestartCnt++;
		if(RestartCnt>2) {
			StartGame();
			bRestartCnt = FALSE;
			RestartCnt=0;
		}
	}
*/	
	// 추방 투표가 진행중이라면 카운트 증가
	if(BanishVoteState == 2) {
		BanishVoteCnt++;
		// 시간 초과되면 추방 투표 취소
		if(BanishVoteCnt > 20) {
			BanishVoteState = 0;
			// 사용자에게 서버메세지를 보냄
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(0, "시간초과로 추방 투표가 취소되었습니다");
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		}
	}
	
	
	///////////////////////////////////////////
	// 수정 
	
	// [게임 아이템 작업]
	if(Ri.bSuperMaster) {
		// 게임이 진행중이지 않은 슈퍼방이고 방폭이 예약된 상태라면 방을 폭파한다
		if(bSuperRoomDestory && Ri.State == 0 && bWaitGameOver != TRUE) {
			for(i=0; i<m_Max_NewPlayer; i++) {// ### [ 관전기능 ] ### 
				if(strlen(Ui[i].ID) > 0) {
					int unum = Ui[i].UNum;
					if(unum < 1 || unum >= MAX_USER) continue;
					if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;
					
					// 사용자에게 서버메세지를 보냄
					CSV_SERVERMSG smsg(SndBuf);
					smsg.Set(1, "슈퍼 방장이 퇴장하였으므로 모든 참여자들은 자동으로 퇴장됩니다.");
					User[unum].SendMsg(&smsg);

					OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
				}
			}
		}
	}
	
	// ### [ 관전기능 ] ### 
	if(bNowUserEmptyDestory)
	{
		for(i=0; i<m_Max_NewPlayer; i++) {
			if(strlen(Ui[i].ID) > 0) {
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;
				
				// 사용자에게 서버메세지를 보냄
				CSV_SERVERMSG smsg(SndBuf);
				smsg.Set(1, "게임 참여유저가 없으므로 모든 참여자들은 자동으로 퇴장됩니다.");
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
	// 카드 배열
	ZeroMemory(m_CardDeck, sizeof(m_CardDeck));
	//ZeroMemory(m_PlayerChangeCradDeck, sizeof(m_PlayerChangeCradDeck));//[62]
	m_CurCardIndex = 0;
	m_GameState = GAME_ROUND_READY;
	GameState = RSTATE_NONE;
//	m_JackPotActive = 0;
	m_CardWindState = 0;
	ZeroMemory(Bc, sizeof(Bc));
	m_Raise.Reset();
	

	for(int i=0; i<m_Max_NewPlayer; i++) {// ### [ 중복된 IP가 ] ###
		if(i<m_Max_Player)// ### [ 관전기능 ] ###
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
		
		// foldnum count 개별 초기화
		Ps2[i].bActive = FALSE;
		Ps2[i].bHaveBanishVote = FALSE;
		Ps2[i].bJoinBanishVote = FALSE;
		Ps2[i].bOnGame = FALSE;
		Ps2[i].bWindOn = FALSE;
		Ps2[i].nActive = 0;
		Ps2[i].nWindOn = 0;
		Ps2[i].In_OutCount = 0;// ### [ 관전기능 ] ###
		Ps2[i].nChangeFlg = FALSE;
//		Ps2[i].nChangeWinEndFlg = FALSE;
	}
}

// ### [ 관전기능 ] ###
void CRoom::StartGame()
{
	if(bValid == FALSE) return;
	if(Ri.State == 1) return;
	// 에이젼트와 접속이 끊기면 게임을 시작할 수 없다.
	if( g_pMainView->m_bExitReserve == TRUE || g_pMainView->bConnectDeny || g_nAgentIndex == -1) 
	{
		CSV_SERVERMSG smsg(SndBuf);
		if(g_nAgentIndex == -1)
			smsg.Set(0, "게임 초기화 준비로 인하여 게임을 시작할 수 없습니다.");// 넷마블 운영자에게 문의해 주십시요");
		else
			smsg.Set(0, "서버종료 예약으로 인하여 게임을 시작할 수 없습니다.");
		SendMsgToAll(smsg.pData, smsg.GetTotalSize());
		return; // 자동종료시 겜시작 안됨
	}

	Ri.ChangeCardStep = 0;

	ResetGame();
	GameState = RSTATE_DISTRIBUTECARD;
	m_JackPotActive = 0;
	// [세븐포커] 루틴
	CardShuffle(); // 카드 섞기

	int joinnum = 0;
	for(int i=0; i<m_Max_NewPlayer; i++) // ### [ 관전기능 ] ###
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

			if(i >= m_Max_Player) continue;// ### [ 관전기능 ] ###

			// 초기 베팅액
			if(User[unum].UI.PMoney < Ri.BetMoney) { // ###################### 던 부족
			//	CSV_SERVERMSG smsg(SndBuf);
			//	CString strMess = "";
			//	smsg.Set(9, (char*)strMess.operator LPCTSTR()); // ## 파산
			//	User[unum].SendMsg(&smsg);
				OnUserOutRoom(unum, Ui[i].ID, FALSE);
				continue;
			}

			Ps[i].JoinState = 1;// 게임참여
			Ps[i].PlayState = 1;// 게임중
			Ps[i].bOnGame   = 1;
		
			Ps[i].nCardTotal = 3;
			Ps[i].nOpenTotal = 0;
			Ps[i].nState     = 1; // 카드3장
			
			// 백업카드 생성
			Bc[i].nCard[0] = (char)GiveCard();
			Bc[i].nCard[1] = (char)GiveCard();
			Bc[i].nCard[2] = (char)GiveCard();
			// 추가
			if(Ri.FormKind == 0) // ###[초이스]### 
				Bc[i].nCard[3] = -1;
			else
				Bc[i].nCard[3] = (char)GiveCard();

			joinnum++;
		}
	}

	// 참여자가 정해진 인원 이하이면 게임시작 취소
	if(Ri.NowUserNum < MIN_PLAYER || joinnum < MIN_PLAYER ) {
		ResetGame();
		bRestartCnt = FALSE;
		RestartCnt  = 0;
		GameState   = RSTATE_NONE;
		return;
	}

	BOOL bRoomChange=FALSE;
	if(Ri.State==0) bRoomChange=TRUE;


	// 게임 시작 플래그 세팅
	Ri.State  = 1;
	GameState = RSTATE_PREPARESTART;

	STARTINFO si;
	ZeroMemory(&si, sizeof(STARTINFO));

	// 여기서 카드받은 인간 액티브플래그 켠다
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
				// 겜참여 아이디
				strncpy(si.JoinID[i], Ui[i].ID, 15);
				
				// ### [로그 기록용]###
				User[unum].nBet = User[unum].UI.PMoney;// 카운트 횟수 기록용 임시 구조체

				User[unum].Set_init_premoney(); // [수호천사] 2004.07.08

				// 삥값처리 초기베팅액
				User[unum].UI.PMoney = User[unum].UI.PMoney - Ri.BetMoney;
				Ui[i].PMoney = User[unum].UI.PMoney;

				m_Raise.SetUser(i, Ri.BetMoney, Ui[i]); 
				// 게임 시작 부분에서 게임에 참여하는유저 
				// [ 통계기록 ]
				g_pMainView->m_cSALog.Make_Log(LEVEL_USE_GAMECNT, unum, TRUE); // 누적 게임수

			}
		}
	}

	int nPrimer    = GetPrimer(); // 선구하기
	memcpy(si.Ps, Ps, sizeof(PLAYSTATE)*m_Max_Player);
	int first = GetPNum(m_DealerID);
	if(first < 0 || first >= m_Max_Player) {	first = GetPNum(Ri.ID); }
	si.nDistUser = first; // 카드 날릴순서

	for(i=0; i<m_Max_NewPlayer; i++) // ### [ 관전기능 ] ###
	{
		if(i<m_Max_Player && strlen(Ui[i].ID) > 0 && Ps[i].bOnGame && Ps[i].PlayState == 1) 
		{				
			for(int n = 0; n < 4; n++) // choice 추가
			{
				si.nCard[n] = Bc[i].nCard[n];
			}

			CSV_STARTCARD startc(SndBuf);
			startc.Set(Ri.RoomNum, Ui[i].UNum, Ui[i].ID, &si);
			SendMsgTo(Ui[i].ID,  startc.pData, startc.GetTotalSize());

			// 카드 날리기 셋
			Ps2[i].bWindOn  = TRUE;
			Ps2[i].nWindOn  = 0;
		}
		// ### [ 관전기능 ] ###
		else if(strlen(Ui[i].ID) > 0 )
		{
			for(int n = 0; n < 4; n++) // choice 추가
			{
				si.nCard[n] = DUMY_CARD;
			}

			CSV_STARTCARD startc(SndBuf);
			startc.Set(Ri.RoomNum, Ui[i].UNum, Ui[i].ID, &si);
			SendMsgTo(i,  startc.pData, startc.GetTotalSize());
		}
	}

	// 시작시 참여인원 설정
	StartUserNum = joinnum;
	m_GameState = GAME_ROUND_3;// 0: 1:카드 3장, 2: 카드 선택, 3: 4장, 4: 5장, 5: 6장, 6: 히든, 7: 승결정
	
	// 모든 사용자에게 방정보가 변경되었음을 알림
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
	// 레이즈후 게이머 생존자가 1명이면 게임 끝낸다(카드오픈금지)
	for(int i=0;i<m_Max_Player;i++){
		if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && !Ps[i].bFold) nGamer++;
	}
	if(nGamer<2){
		bOpen = FALSE;
		return TRUE;
	}

	// 레이즈후 모든레이즈가 끝났다면, 카드 오픈후 게임종료다
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

	// ### [로그 기록용] ###
	char Winner_ip[24]={0,};
	// [ ###잭팟### ]
	INT64 tmpJackPot = 0; // 포카드 이상족보시 잭팟(금액)

	GAMEOVERRESULT gr; 
	ZeroMemory(&gr, sizeof(GAMEOVERRESULT));

	// 팟 상금 주기위한 승자 
	int PotWinner = -1;

	if(bOpen==FALSE){ // 잔류인원이 승자다(오픈하지 않는다, 패자처리하지않는다)
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
		
		// ########### 여기서 승자 마블가감산한다 ###########
		//nHighNumCnt = 100; // 임시 플레그 
		int nWinner  = GetWinner(TRUE,nSpecialVal);
		m_nPrimeUser = nWinner; // 다음선 결정

		//################ 올인머니계산
		INT64 nRealTotal = m_Raise.GetWinnerMoney(nWinner);// 게임결과시의 위너가 가지고 갈수 있는 금액을 산출한다( 실제 )
		gr.nPercent      = GetPercentGory();
		INT64 nWinnerPMoney = nRealTotal-m_Raise.GetWinnerBet(nWinner); //  승리자 실베팅=> 획득 금액
		INT64 nGory         = CalcGory(nWinnerPMoney); // 고리를 뗀다(
		gr.nGory            = nGory;
		nRealTotal         -= nGory;
		if(nRealTotal<0)      nRealTotal = 0;
		nWinnerPMoney      -= nGory;
		//gr.Pg.nRealBat      = nRealTotal;    // 총 금액
		gr.nRealBat      = nRealTotal;    // 총 금액
		if(nWinnerPMoney<0)   nWinnerPMoney = 0;
		gr.nWinMoney        = nWinnerPMoney; // 획득 금액
		

		if(nWinner==nownum){
			int unum = Ui[nownum].UNum;
			if(unum >0 && unum < MAX_USER && faststrcmp(Ui[nownum].ID, User[unum].UI.ID) == 0) {

				// ### [로그 기록용] ###
				strcpy(Winner_ip, User[unum].UserIP);
				// 1. 승수 증가
				User[unum].UI.WinNum++;
				Ui[nownum].WinNum   = User[unum].UI.WinNum;
				gr.Ui[nownum].WinNum = User[unum].UI.WinNum;
				gr.Ui[nownum].LooseNum = User[unum].UI.LooseNum;
				gr.Ui[nownum].DrawNum = User[unum].UI.DrawNum;
				strncpy(gr.Ui[nownum].ID, User[unum].UI.ID, 15);
				// 2. 머니 증가
				INT64 nMoney = nRealTotal;

				// 여기서 실획득액 산정
				if(User[unum].UI.PMoney<0) User[unum].UI.PMoney = 0;
			
				// [ ##### 포커머니 오버플로우 체크 ##### ] : 게임종료시 맥스값을 확인해봅시다!!!
				INT64 tmpMoney = g_pMainView->Cfg.MAX_PMONEY - User[unum].UI.PMoney;
				if(nMoney > tmpMoney) User[unum].UI.PMoney = g_pMainView->Cfg.MAX_PMONEY;
				else
				{
					User[unum].UI.PMoney = User[unum].UI.PMoney + nMoney;
				}

				//승자만 필요하다.
				//2004.05.07 리더스총액 제한
				//승자만 필요하다.
				if( g_pMainView->StatisCfg.bTotalMoneyLimit ) {
					if( g_pMainView->CheckTotalmoneylimit(&User[unum].UI, NULL, 1 ) > 0) { //<--------------찾아서 모두 수정
						Ps2[i].Totalmoneylimit = 1;
					}
				}

				Ui[nownum].PMoney = User[unum].UI.PMoney;
				gr.Ui[nownum].PMoney = User[unum].UI.PMoney;
				

				PotWinner = nWinner;

				// 5. 스페샬값기록
				if(nSpecialVal[nownum]==1){ // 포카드
					User[unum].UI.PI.nFCARD++;
					Ui[nownum].PI.nFCARD = User[unum].UI.PI.nFCARD;
				}
				if(nSpecialVal[nownum]==2){ // 스트레이트 플러쉬
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
					default: // 자유
						tJackp = g_pMainView->Cfg4.jackPotFreeS*2;
						break;
					}
					
					if(tJackp > 0) {
						tmpJackPot = g_JackPotMoney/tJackp; // 비율 사용X
						gr.bJackPot = 1;
					}
				}
				if(nSpecialVal[nownum]==3){ // 로얄 스트레이트 플러쉬
					User[unum].UI.PI.nRSF++;
					Ui[nownum].PI.nRSF = User[unum].UI.PI.nRSF;

					//카드를 오픈하지 않아도 잭팟 지급 1/2 지급
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
					default: // 자유
						tJackp = g_pMainView->Cfg4.jackPotFree * 2;
						break;
					}
					
					if(tJackp > 0) {
						tmpJackPot = g_JackPotMoney/tJackp; // 비율 사용X

						gr.bJackPot = 1;
					}
				}

				memcpy(&gr.Ui[nownum].PI, &Ui[nownum].PI, sizeof(POKERINFO));

			}
		}
		// ################# 패자처리 #################
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && i!=nWinner){
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

				INT64 nFailMoney = m_Raise.GetFailMoney(nWinner,i);
				
				if(User[unum].UI.PMoney < 0) User[unum].UI.PMoney = 0;

				gr.Ui[i].LoseMoney = m_Raise.m_User[i].nRealBat - nFailMoney; // 잃은 머니 

				User[unum].UI.PMoney = User[unum].UI.PMoney + nFailMoney;
				gr.Ui[i].PMoney = Ui[i].PMoney = User[unum].UI.PMoney;
				strncpy(gr.Ui[i].ID, Ui[i].ID, 15);
				gr.Ui[i].WinNum = User[unum].UI.WinNum;
				gr.Ui[i].LooseNum = User[unum].UI.LooseNum;
				gr.Ui[i].DrawNum = User[unum].UI.DrawNum;
				memcpy(&gr.Ui[i].PI, &Ui[i].PI, sizeof(POKERINFO));

				User[unum].Set_refresh_lost_money(); // [수호천사] 2004.07.08
			}
		}

		// 잭팟 
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
			default: // 자유
				tJackp = g_pMainView->Cfg4.rFree;
				break;
			}
			
			if(tJackp > 0) {
				if(g_JackPotMoney < pChan->DefaultJackPot) g_JackPotMoney = pChan->DefaultJackPot; // 기본 잭팟 머니
				g_JackPotMoney += (gr.nGory/ tJackp); // 적립 
				if(g_JackPotMoney > M_1H) g_JackPotMoney = M_1H;// 경 (최고 금액체크)
			}
		}

		gr.JackPotMoney = g_JackPotMoney;
		// 여기서 게임 종료 결과를 보냄
		CSV_GAMEOVERRESULT gormsg(SndBuf);
		gormsg.Set(&gr);
		SendMsgToAll(gormsg.pData, gormsg.GetTotalSize());

	
	}

	///////////////////////////////////////////
	//2004.05.07 리더스총액 제한
	for(i=0;i<m_Max_Player;i++)
	{
		if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && !Ps[i].bFold && Ps2[i].Totalmoneylimit > 0) 
		{
			int unum = Ui[i].UNum;
			if(unum < 1 || unum >= MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

			// 메세지
			g_pMainView->SendPremLeadersMsg(unum, Ui[i].PremLeadersCode );
		}
	}	
	///////////////////////////////////////////	

	if(bOpen==TRUE)
	{ // 히든카드까지 갔다(오픈, 패자처리)
		
		gr.bOpen = TRUE;
		gr.Pg    = MakeInfo();

		int nWinner = GetWinner(TRUE,nSpecialVal);
		m_nPrimeUser = nWinner; // 다음선 결정

		//## 승자가 한명

		//################ 올인머니계산
		INT64 nRealTotal = m_Raise.GetWinnerMoney(nWinner);//승자가 획득한 머니 (나의 머니 + 모든 베팅 사용자(중간에 폴드한 유저))
		gr.nPercent      = GetPercentGory();
		INT64 nWinnerPMoney = nRealTotal-m_Raise.GetWinnerBet(nWinner); // 획득 머니 = 총베팅액 - 나의 총 베팅액;
		INT64 nGory         = CalcGory(nWinnerPMoney); // 고리를 뗀다
		gr.nGory            = nGory;
		nRealTotal         -= nGory;
		if(nRealTotal<0)      nRealTotal = 0;
		nWinnerPMoney      -= nGory;
		if(nWinnerPMoney<0)   nWinnerPMoney = 0;
		gr.nRealBat			= nRealTotal;		// 총 베팅액
		gr.nWinMoney        = nWinnerPMoney;    // 획득 금액(+)
		
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && !Ps[i].bFold){
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

				// ########### 여기서 승자 마블가감산한다 ###########
				if(i==nWinner){

					// ### [로그 기록용] ###
					strcpy(Winner_ip, User[unum].UserIP);
					User[unum].UI.WinNum++;
					Ui[i].WinNum   = User[unum].UI.WinNum;
					INT64 nMoney = nRealTotal;		// 2. 머니 증가

					// 여기서 실획득액 산정
					if(User[unum].UI.PMoney < 0 ) User[unum].UI.PMoney = 0;
					
					// [ ##### 포커머니 오버플로우 체크 ##### ] : 게임종료시 맥스값을 확인해봅시다!!!
					INT64 tmpMoney = g_pMainView->Cfg.MAX_PMONEY - User[unum].UI.PMoney;
					if(nMoney > tmpMoney) User[unum].UI.PMoney = g_pMainView->Cfg.MAX_PMONEY;
					else
					{
						User[unum].UI.PMoney = User[unum].UI.PMoney + nMoney;
					}
					
					
					//2004.05.07 리더스총액 제한
					//승자만 필요하다.
					if( g_pMainView->StatisCfg.bTotalMoneyLimit ) {
						if( g_pMainView->CheckTotalmoneylimit(&User[unum].UI, NULL, 1 ) > 0) {// <--------------찾아서 모두 수정
							Ps2[i].Totalmoneylimit = 1;
						}
					}

					Ui[i].PMoney = User[unum].UI.PMoney;
					gr.Ui[i].PMoney = User[unum].UI.PMoney;
					
					PotWinner = nWinner;
					///////////////////////////////////////////////////////////////////////
					// 5. 스페샬값기록 =>  [ ###잭팟### ]
					if(nSpecialVal[i]==1){
						User[unum].UI.PI.nFCARD++;
						Ui[i].PI.nFCARD = User[unum].UI.PI.nFCARD;
					}

					if(nSpecialVal[i]==2){
						User[unum].UI.PI.nSF++;
						Ui[i].PI.nSF = User[unum].UI.PI.nSF;

							// 잭팟 
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
						default: // 자유
							tJackp = g_pMainView->Cfg4.jackPotFreeS;
							break;
						}
						
						if(tJackp > 0)
							tmpJackPot = g_JackPotMoney/tJackp; // 비율 사용X
					}

					if(nSpecialVal[i]==3){
						User[unum].UI.PI.nRSF++;
						Ui[i].PI.nRSF = User[unum].UI.PI.nRSF;
						// 잭팟 
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
						default: // 자유
							tJackp = g_pMainView->Cfg4.jackPotFree;
							break;
						}
						
						if(tJackp > 0)
							tmpJackPot = g_JackPotMoney/tJackp; // 비율 사용X						
					}
					//////////////////////////////////////////////////////////////////////////////

					// 승자 id 
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
					// ########### 패자 처리 한다 ###########
					// 1. 패수 증가
					User[unum].UI.LooseNum++;
					Ui[i].LooseNum = User[unum].UI.LooseNum;
					// 2. 스페샬값기록
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

		// ################# 패자처리 #################
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame && i!=nWinner){
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

				INT64 nFailMoney = m_Raise.GetFailMoney(nWinner,i);

				if(User[unum].UI.PMoney < 0) User[unum].UI.PMoney = 0;

				User[unum].UI.PMoney = User[unum].UI.PMoney + nFailMoney; // 승자가 올인으로 이겼다면 나머지 던 돌려준다
				Ui[i].PMoney = User[unum].UI.PMoney;
			
				gr.Ui[i].LoseMoney = m_Raise.m_User[i].nRealBat - nFailMoney;//잃은 머니

				gr.Ui[i].PMoney = User[unum].UI.PMoney;
				strncpy(gr.Ui[i].ID, Ui[i].ID, 15);
				gr.Ui[i].WinNum = User[unum].UI.WinNum;
				gr.Ui[i].LooseNum = User[unum].UI.LooseNum;
				gr.Ui[i].DrawNum = User[unum].UI.DrawNum;
				memcpy(&gr.Ui[i].PI, &User[unum].UI.PI, sizeof(POKERINFO));

				User[unum].Set_refresh_lost_money(); // [수호천사] 2004.07.08
			}
		}
		
		// [ ###잭팟### ]
		if(g_pMainView->Cfg4.JackPotMode == 1 && pChan->JackPotSaveRatio > 0) {

			if(g_JackPotMoney < pChan->DefaultJackPot) g_JackPotMoney = pChan->DefaultJackPot; // 기본 잭팟 머니
			g_JackPotMoney += (gr.nGory/ pChan->JackPotSaveRatio); // 적립 
			if(g_JackPotMoney > M_1H) g_JackPotMoney = M_1H;// 경 (최고 금액체크)
		}

		gr.JackPotMoney = g_JackPotMoney;

		CSV_GAMEOVERRESULT gormsg(SndBuf);
		gormsg.Set(&gr);
		SendMsgToAll(gormsg.pData, gormsg.GetTotalSize());

		
	}
/*
	// 오링 메세지 보냄 
	for(i = 0 ; i < m_Max_Player ; i ++){
		if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame){
			if(Ui[i].PMoney <= 0){
				OnUserOutRoom(Ui[i].UNum,Ui[i].ID,FALSE);		
			}
		}
	}	
*/

	// [ 통계기록 ]
//	g_pMainView->Make_StatisticsLog(GRADE_AVERAGE_MONEY, pChan->ChannelStyle,  TRUE, m_Raise.GetTotalBat(), gr.nGory); // 게임 판돈, 고리(게임비)
	g_pMainView->m_cSALog.Make_Log(GRADE_AVERAGE_MONEY, pChan->ChannelStyle,  TRUE, m_Raise.GetTotalBat(), gr.nGory); // 게임 판돈, 고리(게임비)



	if(m_nPrimeUser >= 0 && m_nPrimeUser < m_Max_Player)
	{
		strncpy(m_DealerID, Ui[m_nPrimeUser].ID, 15);
	}


	// ### [이벤트 티켓] ###
	if(g_pMainView->EventCfg.bRunEvent && g_pMainView->Cfg.bEventDB )
	{
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID)>0 && Ps[i].bOnGame ){
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;
				
				User[unum].EventPrize.GameCnt++;
				if(User[unum].EventPrize.GameCnt== EVENT_GAMECOUNT) {
					User[unum].EventPrize.EventPrize +=EVENT_GIVECOIN; //포커, 포커2 30 -> 2개씩, 하이로우 20 -> 1
					
					// 실시간 기록 루틴!!!
					DBWORKS dbwork;
					memset(&dbwork,0,sizeof(DBWORKS));
					User[unum].MakeDBWork(5, &dbwork);
					BOOL rtn = DBMan.SetDBWork(&dbwork);
					if(rtn == FALSE) {
						CLogFile LogFile;
						LogFile.Writef(_T("DB Work Que overflow - 이벤트 : id=%s"), dbwork.UI.ID);
					}

					SendRoomEventNotice(i);
					
				}
			}
		}
	}


	// [ ###잭팟### ] 
	if(tmpJackPot > 0) 
	{
		for(i=0;i<m_Max_Player;i++) 
		{
			if(strlen(Ui[i].ID)>0 )
			{
				int unum = Ui[i].UNum;
				if(unum < 1 || unum >= MAX_USER) continue;
				if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

				if(nSpecialVal[i]==3 || nSpecialVal[i]==2 )//|| nSpecialVal[i]==1)  // 로티플 , 스티플
				{ // 스페셜카드
					SPECIALCARD sc; 
					ZeroMemory(&sc, sizeof(sc));
					
					strncpy(sc.ID, Ui[i].ID, 15);
					strncpy(sc.ChanName, pChan->strChanName.operator LPCTSTR(), 24);// 채널이름
					sc.nCard = nSpecialVal[i];
					sc.nRoom = Ri.RoomNum; // 방번호
					if(g_pMainView->Cfg4.JackPotMode == 1) // 현재 잭팟 모드
					{
						if(PotWinner == i) // 잭팟 금액
						{
							// [ ##### 포커머니 오버플로우 체크 ##### ] : 게임종료시 맥스값을 확인해봅시다!!!
							INT64 tmpMoney = g_pMainView->Cfg.MAX_PMONEY - User[unum].UI.PMoney;
							if(tmpJackPot > tmpMoney) User[unum].UI.PMoney = g_pMainView->Cfg.MAX_PMONEY; // 10경
							else
							{
								User[unum].UI.PMoney = User[unum].UI.PMoney + tmpJackPot;
							}

							Ui[i].PMoney = User[unum].UI.PMoney;
							
							// 채널별 한도금액
							//if(User[unum].UI.PMoney > ChannelMoney) User[unum].UI.PMoney = ChannelMoney;


							g_JackPotMoney -= tmpJackPot;
							if(g_JackPotMoney < pChan->DefaultJackPot) g_JackPotMoney = pChan->DefaultJackPot; // 기본 잭팟 머니
							sc.JackPotPlus = tmpJackPot;

							if(tmpJackPot > M_1Y) // 1억 보다 크다면 지연 증가
								m_JackPotActive = 6; // 클라이언트 잭팟보여주기 시간 연장
							else
								m_JackPotActive = 4; // 클라이언트 잭팟보여주기 시간 연장
							
							// [ 통계기록 ]
							//	g_pMainView->Make_StatisticsLog(GRADE_JACKPOT, pChan->ChannelStyle,  TRUE, tmpJackPot, nSpecialVal[i]); // 잭팟, 잭팟금액
							g_pMainView->m_cSALog.Make_Log(GRADE_JACKPOT, pChan->ChannelStyle,  TRUE, tmpJackPot,0); // 잭팟, 잭팟금액
						}
						else 
							sc.JackPotPlus = 0;
						
						
					}
					sc.NowJackPot = g_JackPotMoney;
					
					CSV_SPECIALCARD csc(SndBuf);
					csc.Set(&sc);
					if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse)
					{						
						//잭팟도 로그 기록에 남긴다.
						LOGREC_DBWORKS LWork;
						ZeroMemory(&LWork, sizeof(LOGREC_DBWORKS));
						LWork.WorkKind = 1; // [ 통계기록 ]
						LWork.StartTick = timeGetTime();
						LWork.bValid = TRUE;
						// 하이로우 코드 30~
						//	LWork.LogRecord.Status = 73; // 로그 게임코드로 쓴다.
						LWork.LogRecord.Status = 3; // 로그 게임코드로 쓴다.
						
						strncpy(LWork.LogRecord.ID, Ui[i].ID, 15);
						LWork.LogRecord.CurMoney = User[unum].UI.PMoney;	// 현재 머니
						LWork.LogRecord.LoseMoney = sc.JackPotPlus;			// 잭팟 머니
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
						wsprintf(LWork.LogRecord.Winner_ip,"%d번방", Ri.RoomNum);
						
						// DB 쓰레드 가동
						BOOL rtn = LogDBMan.SetDBWork(&LWork);
						if(rtn == FALSE) {
							CLogFile logfile("LogRecordDB.txt");
							logfile.Writef("잭팟 기록 에러");
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



// ### [로그 기록용] ###		
#ifdef LOG_RECORD_
if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse)
{
	// 카운트 횟수 기록용 임시 구조체
	INT64 CurMoney = 0;
	for(i=0;i<m_Max_Player;i++) {
		if(strlen(Ui[i].ID)>0) {
			int unum = Ui[i].UNum;
			if(unum < 1 || unum >= MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;

			
			// [위치 저장] 
			if( g_SearchID.bUse && g_pMainView->IsSearchUserID(Ui[i].ID)) {

					CString str;
					str.Format("%s (%d) %s  %d번방  ", g_pMainView->Cfg.ServerName, pChan->ChNum, pChan->strChanName, Ri.RoomNum);
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
					// 현재 날짜 및 시간
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
					// 현재 위치
					Work.SubmitReason = "위치파악";
					Work.Contents = pChat;
					BadUserDBQue.EnQue(Work);
					if(pChat) delete pChat;
			}

			// ### [로그 기록용]###
			// 현재 판에서 얼마를 잃었는가?
			// 게임 시작시 머니 셋팅 -> 게임 결과시 
			if(User[unum].UI.PMoney < 0) User[unum].UI.PMoney = 0;
			CurMoney = User[unum].nBet - User[unum].UI.PMoney;

			if(CurMoney < g_LogRangeValue) continue; // 10억 부터 1조

			if(LogDBMan.DBQue.GetTotNum() >= g_pMainView->Cfg2.LoginDBQueNum2)
			{
				CLogFile logfile("LogRecordDB.txt");
				logfile.Writef("LogRecordDB 에러 03, id=%s, curmoney=%I64d, losemoney=%I64d, winid=%s",
					Ui[i].ID, User[unum].UI.PMoney, CurMoney, gr.ID);
			}
			else
			{
				// 소켓의 유니크를 구분하기위한 Time Tick
				LOGREC_DBWORKS LWork;
				ZeroMemory(&LWork, sizeof(LOGREC_DBWORKS));
				LWork.StartTick = timeGetTime();
				LWork.bValid = TRUE;
				LWork.WorkKind = 1; // [ 통계기록 ] <-----------------------------추가
				
			//	LWork.LogRecord.Status = 71; // 정상적인 게임오버 // 게임 코드랑 비슷 한 유니크 코드로 변경.(1은 포커2다)
				LWork.LogRecord.Status = 1; // 정상적인 게임오버 // 게임 코드랑 비슷 한 유니크 코드로 변경.(1은 포커2다)
				
				strncpy(LWork.LogRecord.ID, Ui[i].ID, 15);
				LWork.LogRecord.CurMoney = User[unum].UI.PMoney;
				LWork.LogRecord.LoseMoney = CurMoney;
				strncpy(LWork.LogRecord.Win_ID, gr.ID, 15);
				
				LWork.LogRecord.Win_CurMoney = gr.Ui[gr.nNowWinner].PMoney; //?????
				LWork.LogRecord.Win_GetMoney = gr.nWinMoney;
				
				strcpy(LWork.LogRecord.My_ip, User[unum].UserIP);
				strcpy(LWork.LogRecord.Winner_ip, Winner_ip);
				
				// DB 쓰레드 가동
				BOOL rtn = LogDBMan.SetDBWork(&LWork);
				if(rtn == FALSE) {
					CLogFile logfile("LogRecordDB.txt");
					logfile.Writef("LogRecordDB 에러 04, id=%s, curmoney=%I64d, losemoney=%I64d, winid=%s",
						Ui[i].ID, User[unum].UI.PMoney, CurMoney, gr.ID);
				}
			}
			// ### [로그 기록용]###
			User[unum].nBet = 0;// 머니 초기화
		}
	}
}
#endif

	Ri.State = 0;
	GameState=RSTATE_NONE;
	ResetGame();

	// 방정보 변경 메세지를 모든 사용자에게 보냄
	CSV_CHANGEROOM crmsg(SndBuf);
//	SMALLROOMINFO sri = GetSmallRoomInfo();
	crmsg.Set(&GetSmallRoomInfo());
	pChan->SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());

	// 추방예약이 되어 있다면 추방 투표 진행
	if(BanishVoteState==1) DoBanishVote();
	SetWaitGameOver(); // 게임종료 카운트 시동
}

// 게임 종류 변경 메시지
void CRoom::OnAskChangeGameKind(CCL_ASK_CHANGEGAMEKIND *pMsg)
{
	if(bValid==FALSE) return;
	if(Ri.State != 0) return;

	int pnum = *pMsg->ServPNum;
	if(pnum<0 || pnum>=m_Max_Player) return;
	if(strlen(Ui[pnum].ID)==0 || faststrcmp(Ui[pnum].ID, pMsg->ID)!=0) return;
	int gamekind = *pMsg->GameKind;
//	if(gamekind!=0 && gamekind!=1) return;
	// 요청인이 방장이 아니면 취소
	if(faststrcmp(Ri.ID, pMsg->ID) != 0) return;
		
//	Ri.bWhis = sri.bWhis;
//	Ri.nCard = sri.nCard;
//	strncpy(Ri.Title, sri.Title, 30);
	
	Ri.GameKind = gamekind;
//	Ri.bWhis = *pMsg->bWhis;
	Ri.nCard = *pMsg->nCard;

	// 게임 방식 변경 메세지 보냄
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
		// 사용자에게 서버메세지를 보냄
		CSV_SERVERMSG smsg(SndBuf);
		if(BanishVoteState==1) smsg.Set(1, "이미 추방 투표가 예약되어있습니다.");
		if(BanishVoteState==2) smsg.Set(1, "추방 투표가 진행중입니다.");
		SendMsgTo(Ui[mypnum].ID, smsg.pData, smsg.GetTotalSize());
		return;
	}

	if(Ri.NowUserNum<BANISHVOTE_NUM)
	{
		// 사용자에게 서버메세지를 보냄
		CSV_SERVERMSG smsg(SndBuf);
		smsg.Set(1, "3명 이상일때만 추방투표가 가능합니다.");
		SendMsgTo(Ui[mypnum].ID, smsg.pData, smsg.GetTotalSize());
		return;
	}

	if(Ri.State==1)
	{
		BanishVoteState = 1;
		memcpy(&BVI, pbv, sizeof(BANISHVOTEINFO));

		// 사용자에게 서버메세지를 보냄
		CSV_SERVERMSG smsg(SndBuf);
		smsg.Set(0, "추방 투표가 예약되었습니다");
		SendMsgToAll(smsg.pData, smsg.GetTotalSize());
	}
	else
	{
		BanishVoteState = 2;
		memcpy(&BVI, pbv, sizeof(BANISHVOTEINFO));
		// 추방 투표 진행
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
		// 사용자에게 서버메세지를 보냄
		CSV_SERVERMSG smsg;
		smsg.Set(0, "투표인원이 3명 미만이므로 추방 투표가 취소되었습니다");
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
		// 추방 대상자를 제외한 모든 사용자가 투표에 참여한다
		if(strlen(Ui[i].ID)>0 && faststrcmp(Ui[i].ID, BVI.TarID)!=0) Ps2[i].bJoinBanishVote = TRUE;
		// 추방 요청자는 자동으로 투표한것으로 간주됨
		if(faststrcmp(Ui[i].ID, BVI.MyID)==0) Ps2[i].bHaveBanishVote = TRUE;
	}

	// 추방 투표 시작을 알림
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

		// 사용자에게 서버메세지를 보냄
		CString str;
		str.Format("투표결과 :\n찬성 %d표, 반대 %d표\n", BanishVoteAgreeNum, BanishVoteDisgreeNum);
		if(BanishVoteAgreeNum>=(joinnum*2)/3) 
	//	if(BanishVoteDisgreeNum==0 && BanishVoteAgreeNum>=joinnum) 
		{
			str += "과반수가 찬성하였으므로 강제 추방합니다.";
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(0, (char*)str.operator LPCTSTR());
			SendMsgToAll(smsg.pData, smsg.GetTotalSize());
			
			// 사용자에게 서버메세지를 보냄
			CSV_SERVERMSG smsg2(SndBuf);
			smsg2.Set(1, "투표 결과에 의하여 강제 추방되었습니다");
			SockMan.SendData(User[tarunum].Sid, smsg2.pData, smsg2.GetTotalSize());

			VoteBanUserList.AddID(User[tarunum].UI.ID);
			
			// 강제 추방
			OnUserOutRoom(tarunum, User[tarunum].UI.ID, FALSE);
		}
		else
		{
			str += "과반수가 반대표로 추방을 취소합니다.";
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


// 포커게임 정보를 생성한다
POKERGAME CRoom::MakeInfo()
{
	POKERGAME pg;
	ZeroMemory(&pg, sizeof(POKERGAME));
	pg.nRound   = m_Raise.GetRound();
	if(m_Raise.GetRound()>0) pg.bRaiseOn = TRUE;
	memcpy(pg.RU, m_Raise.m_User, sizeof(RAISEUSER)*m_Max_Player); 
	memcpy(pg.PS, Ps, sizeof(PLAYSTATE)*m_Max_Player);
	if(Ri.State == 1){ // 게임중이고,게임결과를 보여줘야한다면 보낸다

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
//////////////////////  [세븐포커] 게임루틴  /////////////////////////
//////////////////////////////////////////////////////////////////////

// 사용자가 포기요청을 하였다.
// ### [ 관전기능 ] ###
BOOL CRoom::FoldUser(int nNum, BOOL bOut)
{
	if(bValid == FALSE) return FALSE;
	if(Ri.State!=1) return FALSE;
	// 올바른 데이타가 아니다!!
	if(nNum<0 || nNum>=m_Max_Player) return FALSE;
	if(Ps[nNum].bOnGame != TRUE) return FALSE;
	if(Ps[nNum].bFold == TRUE) return FALSE;
	
	// 유저 폴드
	// 여기서 포기 메시지 전송
	CSV_FOLDUSER cfold(SndBuf);
	cfold.Set(Ri.RoomNum, Ui[nNum].UNum, nNum, Ui[nNum].ID, 0, g_JackPotMoney); // [ ###잭팟### ] 폴드시 같이 보낸다.
	SendMsgToAll(cfold.pData, cfold.GetTotalSize());


	Ps[nNum].bFold = TRUE;		// 게임포기세팅
	Ps[nNum].PlayState = 0;

	Ps2[nNum].bWindOn = FALSE;
	Ps2[nNum].nWindOn = 0;

	//62cut 플래그 초기화[62]
	Ps2[nNum].nChangeFlg = FALSE; 
//	Ps2[nNum].nChangeWinEndFlg = FALSE;

	// ########## 포기증가 ###########
	int unum = Ui[nNum].UNum;
	if(unum >0 && unum < MAX_USER && 
		faststrcmp(Ui[nNum].ID, User[unum].UI.ID)==0){
		User[unum].UI.DrawNum++;
		Ui[nNum].DrawNum  = User[unum].UI.DrawNum;
		// ### [로그 기록용]###
		// 포기 사용자는 베팅액을 포기한 사용자다! 백업용 베팅액을 지운다
		//User[unum].nBet = 0;
		// 변경된 사용자 정보를 알림
		CHANGEMYINFO cmi={0,};
		cmi.WinNum = User[unum].UI.WinNum;
		cmi.LooseNum = User[unum].UI.LooseNum;
		cmi.DrawNum = User[unum].UI.DrawNum;
		cmi.PMoney  = User[unum].UI.PMoney;

		CSV_CHANGEMYINFO cmimsg(SndBuf);
		cmimsg.Set(&cmi);
		SendMsgTo(Ui[nNum].ID, cmimsg.pData, cmimsg.GetTotalSize());

		// [수호천사] 2004.07.09
		User[unum].Set_refresh_lost_money(); // [수호천사] 2004.07.08
	}

	if(GameState==RSTATE_PREPARESTART){ // 게임 준비중 나갔다면
		// 만약액티브 유저였다면..
		Ps2[nNum].bActive = FALSE;
		Ps2[nNum].nActive = 0;
		if(m_Raise.m_User[nNum].bInUse)	m_Raise.m_User[nNum].bFold = TRUE;
		Ps2[nNum].nFoldNum++;
		GetPrimer(); // 새로운 선 세팅
		if(Ps2[nNum].nFoldNum>=3){
			CString strMess = g_StrMan.Get(_T("SPOKER_OUTROOM"));
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, (char*)strMess.operator LPCTSTR() );
			SendMsgTo(Ui[nNum].ID, smsg.pData, smsg.GetTotalSize());
			Ps2[nNum].nFoldNum = 0;
			// 강제 퇴장 처리
			if ( !bOut )
				OnUserOutRoom(Ui[nNum].UNum, Ui[nNum].ID, FALSE);
		}
		BOOL bOpen = FALSE;
		if(CheckGameOver(bOpen)){
			DoGameOver(bOpen);
			return TRUE;
		}
		// 카드 날리는 중, 카드 선택 부분 ??????
		// m_GameState; 0: 1: 카드 3장 2: 카드 선택 3: 카드 4장 4: 카드 5장 5: 6장 6: 히든 7: 승 결정 8: 게임 오버

		if(  m_GameState == GAME_ROUND_3 ) {
			AnalRaiseOn();
		}
		else //m_GameState == 2 카드 선택중 나갔다
		{
			StartRaise(); // 현재 나가 버렸거나, 폴드 햇다면
		}
		return TRUE;
	}
	else if(GameState == RSTATE_CHANGECARD)//[62]
	{
		// 만약액티브 유저였다면..
		Ps2[nNum].bActive = FALSE;
		Ps2[nNum].nActive = 0;		
		if(m_Raise.m_User[nNum].bInUse)	m_Raise.m_User[nNum].bFold = TRUE;
		Ps2[nNum].nFoldNum++;
		GetPrimer(); // 새로운 선 세팅
		if(Ps2[nNum].nFoldNum>=3){
			CString strMess = g_StrMan.Get(_T("SPOKER_OUTROOM"));
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, (char*)strMess.operator LPCTSTR() );
			SendMsgTo(Ui[nNum].ID, smsg.pData, smsg.GetTotalSize());
			Ps2[nNum].nFoldNum = 0;
			// 강제 퇴장 처리
			if ( !bOut )
				OnUserOutRoom(Ui[nNum].UNum, Ui[nNum].ID, FALSE);
		}		
		
		BOOL bOpen = FALSE;
		if(CheckGameOver(bOpen)){
			DoGameOver(bOpen);			
			return TRUE;
		}

		// 바꾸기 일때 레이스 체크
		ChangeCardRaiseCheck();	//[62]	
	}
	else
	{ // 현재 카드 날리는 중에 나갔다면
		if(Ps2[nNum].bActive) {

			if(m_CardWindState == 1) // 문제 있다.
				AnalRaiseOn();

			POKERCLNT pc; 
			ZeroMemory(&pc, sizeof(pc));
			pc.nBtn   = 0;
			pc.nStyle = 0; // 폴드
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

// 젤 첨 레이즈
void CRoom::StartRaise()
{
	if(!bValid) return;
	if(Ri.State != 1) return; // 게임중
	if(GameState != RSTATE_PREPARESTART) return;

	// 게임시작 확인후 게임시작
	int i=0;
	BOOL bStart  = TRUE;
	for(i=0;i<m_Max_Player;i++){
		if( Ps2[i].bActive ) bStart=FALSE; // 모두 카드 선택 했다면
	}
	BOOL bOpen = FALSE;
	if(CheckGameOver(bOpen)){
		DoGameOver(bOpen);
		return;
	}

	// 레이즈 시작
	if(bStart) {

		m_GameState = GAME_ROUND_4;
		// 액티브 플래그 제거
		for( i=0;i<m_Max_Player;i++){
			Ps2[i].bActive = FALSE;
			Ps2[i].nActive = 0;
			Ps2[i].bWindOn = FALSE; // 카드딜레이 타임제거
			Ps2[i].nWindOn = 0;
		}

		int nSV[MAX_PLAYER]={0,};
		int nDistUser = GetWinner(FALSE, nSV); // (총 3장) 1장가장 높은값 부터 카드 날린다
		
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && !Ps[i].bFold){ // 게임중인 유저
				Ps[i].nState     = 3; // 카드4장에 레이즈시작
				Ps[i].nCardTotal = 4;
				Ps[i].nOpenTotal = 2;
				Ps[i].nCard[1]   = (char)GiveCard(); // 각각 오픈된 카드
		
				Ps2[i].bWindOn  = TRUE;
				Ps2[i].nWindOn  = 0;
			}
		}

		m_CardWindState = 1;

		GameState = RSTATE_STARTGAME;
		// [ ###베팅룰### ]
		//          채널		베팅액
		m_Raise.Set(Ri.Level, Ri.GameKind);

		int nActive = GetWinner(FALSE,nSV); // (총 4장) 2장 중 가장 큰값 베팅
		Ps2[nActive].bActive = TRUE;
		Ps2[nActive].nActive = 0;
//		Ps2[nActive].bCheck  = TRUE;
		POKERGAME pg = MakeInfo();
		pg.bDistCard = TRUE;		// 카드 날리기 차례
		pg.nDistUser = nDistUser;   // 카드 날리기 순서
		pg.CurPlayer = nActive;	// 현재 턴

		CSV_RAISE craise(SndBuf);
		craise.Set(&pg);
		SendMsgToAll(craise.pData, craise.GetTotalSize() );


		// ##################### 과연????? #################
		//Ps2[nActive].bActive = FALSE;
		//Ps2[nActive].nActive = 0;

	}
}

/*

//[62] 바꿀 카드를 받아 처리하는곳
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

	// 카드를 버렸다면 빽업 카드를 비워 둔다.
	if(data.nFlag > -1 && data.nFlag < 52)
	{			
		int pcindex = -1;
		int bcindex = -1;

		//보낸넘이 바꾸기가 아닌데 또는 바꾸기를 했거나 그럴때 들어오면 리턴 
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

		// 여기서 바뀐 정보를 알려준다..왜냐..카드 정렬을 해야 하니깡.자기 자신은 보낸후에 먼저 정리 할꺼다.
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
			Ps2[pnum].bWindOn = FALSE; // 카드딜레이 타임제거
			Ps2[pnum].nWindOn = 0;		
			Ps[pnum].nState     = 3;  			
			
			int temp = -1;			

			//히든 카드일때 서버 카드 덱에 값 셋팅 
			if((bcindex == 0 || bcindex == 1) && pcindex == -1) // 히든카드 일때
			{
				//2cut 에서는 bc카드 넣어 준다.히든으로 주기때문 
				if(GAME_ROUND_CHANGECARD2 == m_GameState)// 2cut 에는 히든으로 준다.
				{
					 temp = Bc[pnum].nCard[2] = cd.nCardNum;
				}
				if(GAME_ROUND_CHANGECARD1 == m_GameState)
				{
					temp = Bc[pnum].nCard[bcindex] = cd.nCardNum;
				}
			}

			//오픈 카드를 바꾸었다면 서버 카드 덱에 값 셋팅
			if(pcindex == 3)
			{
				// 2cut 에는 히든으로 준다.
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


			// 카드를 나눠준다. 히든 카드
			if(data.nCardIndex == 0 ||  data.nCardIndex == 1 )//히든카드 
			{			
				for(i=0; i<m_Max_NewPlayer; i++){				
					if(strlen(Ui[i].ID)>0)
					{						
						if( i == pnum && strcmp(Ui[i].ID,data.ID)==0)
						{
							//자기 자신만 값을 넣어줌
							cd.nCardNum = temp;
							caskchange.Set(&cd);
							SendMsgTo(Ui[i].ID,caskchange.pData, caskchange.GetTotalSize());
						}						
						else
						{
							//다른 사람들에겐 더미 카드를 보내줌
							cd.nCardNum = 52;
							caskchange.Set(&cd);
							SendMsgTo(Ui[i].ID,caskchange.pData, caskchange.GetTotalSize());
						}
					}
				}				
			}

			// 카드를 나눠준다. 오픈 카드
			if(data.nCardIndex > 1 && data.nCardIndex < 6)//오픈카드
			{
				// 2cut 에는 히든으로 준다.
				if(GAME_ROUND_CHANGECARD2 == m_GameState)
				{
					for(i=0; i<m_Max_NewPlayer; i++){						
						if(strlen(Ui[i].ID)>0)
						{						
							//2컷에서는 히든으로 자기 자신만 값을 넣어줌
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
				// 1컷일때는 상관없다 다 보내준다. 오픈카드이기 때문
				if(GAME_ROUND_CHANGECARD1 == m_GameState)
				{
					caskchange.Set(&cd);
					SendMsgToAll(caskchange.pData, caskchange.GetTotalSize() );
				}	
			}				
		}			
	}	
	// 카드 바꾸기를 하지 않은넘.(취소를 누른넘) 
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
		
		Ps2[pnum].bWindOn = FALSE; // 카드딜레이 타임제거
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
		
		if(data.nFlag > -1){//바꾸기를 한넘들
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
		else{//바꾸기를 하지 않은넘들

			memset(&m_PlayerChangeCradDeck[pnum],-1,sizeof(m_PlayerChangeCradDeck[pnum]));
			m_PlayerChangeCradDeck[pnum].nPNum = pnum;
			m_PlayerChangeCradDeck[pnum].nFlag  = 101;

		}

		int changeusercnt = 0 ;
		int nowusercnt = 0 ;
		BOOL bChangeCardEnd = TRUE;// 다 도착 했는지
		// 다 왔는지 검사 
		for(int i = 0 ; i < m_Max_Player ; i ++){
			if(Ps2[i].nChangeFlg == TRUE && Ps[i].bOnGame && Ps[i].PlayState == 1){
				bChangeCardEnd = FALSE;
				break;
			}	
		}			
		
		// 바꾸기 요청이 끝났으면	
		if(bChangeCardEnd)
		{
			CSV_ASK_CHANGECARD_62CUT_EX caskchange(SndBuf);			
			CHANGECARD_62CUT_EX cd;
			memset(&cd,-1,sizeof(CHANGECARD_62CUT_EX));						
			
			int nSV[MAX_PLAYER]={0,};

			int nDistUser = GetWinner(FALSE,nSV);
			
			
			cd.nDistUser = nDistUser; // 선부터 돌린다.
			cd.nRoomNum = Ri.RoomNum;
			
			for(int x = 0 ; x < m_Max_Player ; x++)
			{
				if(Ps2[x].nChangeFlg == FALSE && Ps[x].bOnGame && Ps[x].PlayState == 1){

					int index = 0;
					strcpy(cd.data[x].ID,Ui[x].ID);
					cd.data[x].nFlag  = 100;

					//GAME_ROUND_CHANGECARD2 // 두번째 컷은 히든 카드로

					//바꾼넘들
					if(m_PlayerChangeCradDeck[x].nFlag == 100)
					{						
						cd.data[x].nServerPNum = x;	
						cd.data[x].nChangeCardNum = m_PlayerChangeCradDeck[x].nChangeCardNum;
						cd.data[x].nCardIndex = m_PlayerChangeCradDeck[x].nCardIndex;
						//새로운 카드를 줌 
						m_PlayerChangeCradDeck[x].nCardNum = cd.data[x].nCardNum = (char)GiveCard();

						int cardindex = m_PlayerChangeCradDeck[x].nCardIndex; 
						int newcardnum = m_PlayerChangeCradDeck[x].nCardNum;						
						
						if(GAME_ROUND_CHANGECARD1 == m_GameState){							
							if(cardindex > 1 && cardindex < 6){//오픈 카드
								Ps[x].nCard[3] = newcardnum;
								Ps[x].nOpenTotal++;
							}
							else{// 히든 카드
								Bc[x].nCard[cardindex] = newcardnum;
							}						
						}
						else if(GAME_ROUND_CHANGECARD2 == m_GameState)// 2cut 에는 히든으로 준다.
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

						// 여기서 카드 날리기 세팅
						Ps2[x].bWindOn  = TRUE;
						Ps2[x].nWindOn  = 0;	
						
						Ps[x].nCardTotal++;			
					}
					else{//바꾸지 않은넘들
						// 여기서 카드 날리기 세팅
						Ps2[x].bWindOn  = TRUE;
						Ps2[x].nWindOn  = 0;	
					}
					//	Ps[x].nState     = 3;
				}
			}	
			
			// 각각 한넘씩 보내준다..히든을 바꾼넘들것은 더미 카드로 바꿔서 보내줌.
			int j = 0;
			for(i=0; i<m_Max_NewPlayer; i++){
				if(strlen(Ui[i].ID)>0){// && Ps[i].bOnGame && !Ps[i].bFold && Ps[i].PlayState == 1) {
					
					//히든을 바꾼넘들것은 더미 카드로 바꿔서 보내줌.
					for(j = 0 ; j < m_Max_Player ; j ++){					
						if((cd.data[j].nCardIndex == 0 || cd.data[j].nCardIndex == 1 || GAME_ROUND_CHANGECARD2 == m_GameState ) && cd.data[j].nServerPNum != i){
							cd.data[j].nCardNum = DUMY_CARD;
						}
					}	
					
					caskchange.Set(&cd);												
					SendMsgTo(Ui[i].ID,caskchange.pData, caskchange.GetTotalSize());	

					//레이스 시작을 채크하는 플래그가 된다.
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
			//바꾼넘한테 너 바꿨다(컷,패스),다른넘도 바꿨다 라고 알려줌.
			
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
	//한장씩 뿌리기 
	/*
	// 카드를 버렸다면 빽업 카드를 비워 둔다.
	if(data.nFlag > -1)
	{			
	//보낸넘이 바꾸기가 아닌데 또는 바꾸기를 했거나 그럴때 들어오면 리턴 
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
		// 여기서 바뀐 정보를 알려준다..왜냐..카드 정렬을 해야 하니깡.자기 자신은 보낸후에 먼저 정리 할꺼다.		
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
			Ps2[pnum].bWindOn = FALSE; // 카드딜레이 타임제거
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
		// 카드 바꾸기를 하지 않은넘.(취소를 누른넘)
		CSV_ASK_CHANGECARD_62CUT caskchange(SndBuf);
		
		CHANGECARD_62CUT cd;
		ZeroMemory(&cd,sizeof(CHANGECARD_62CUT));						
		strcpy(cd.ID,Ui[pnum].ID);
		cd.nFlag  = 100;		
		cd.nServerPNum = pnum;		
		
		cd.nCardIndex = -1;	
		cd.nChangeCardNum = -1;		
		cd.nCardNum = -1;
		
		Ps2[pnum].bWindOn = FALSE; // 카드딜레이 타임제거
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

// 유저 아웃 시점에서  채크 
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
		m_nPrimeUser = nActive; // 다음선 결정
		Ps2[nActive].bActive = TRUE;
		Ps2[nActive].nActive = 0;
		Ps[nActive].bCheck  = TRUE; // 체크버튼 세팅
		
		m_Raise.Start(Ps); // 새로운 레이즈 준비하라!!
		
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
//[62] // 카드 날리기까지 완료 된상태를 알려준다.레이스 시작셋팅
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
		m_nPrimeUser = nActive; // 다음선 결정
		Ps2[nActive].bActive = TRUE;
		Ps2[nActive].nActive = 0;
		Ps[nActive].bCheck  = TRUE; // 체크버튼 세팅
		
		m_Raise.Start(Ps); // 새로운 레이즈 준비하라!!
		
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

// 포커메인 게임텬이다
void CRoom::SendTurn(int nPNum, POKERCLNT *pc)
{	
	if(bValid == FALSE) return;
	if(Ri.State!=1) return;
	// 추가 현재 턴
	if(!Ps[nPNum].bOnGame ) return;
	if(Ps2[nPNum].bActive != TRUE) return;
	
	int i=0;
	int unum = Ui[nPNum].UNum;
	// 액티브 플래그 제거
	for( i=0;i<m_Max_Player;i++){
		Ps2[i].bActive = FALSE;
		Ps2[i].nActive = 0;
		Ps2[i].bWindOn = FALSE; // 카드딜레이 타임제거
		Ps2[i].nWindOn = 0;
	}
	// 커맨드 추출
	int nStyle = pc->nStyle; // 0: 폴드, 1.체크 , 2: 콜, 3: 배트, 4: 다이
	int nCmd   = pc->nBtn;   // 0: 삥(최소), 1: 따당(적정), 2: 하프, 3: 맥스(최대),4:올인(가진돈 다 털어)
	int nPreCmd= 0;

	enum ERAISESIGN sign = ERAISE_FOLD;
	BOOL bBtnCheck  = FALSE;
	int  nNextUser  = 0;
	BOOL bNextBtnBat = FALSE, bNextBtnCheck = FALSE, bNextBtnCheck2 = FALSE;
	BOOL bHiddenCard = FALSE;

	switch(nStyle){
	case 1: // 체크
		sign = ERAISE_CHECK;
		nPreCmd= 2;
		for(i=0;i<m_Max_Player;i++) Ps[i].bCheck = FALSE; 
		break;
	case 2: // 콜
		sign = ERAISE_CALL;
		nPreCmd= 3;
		break;
	case 3: // 배팅
		sign = ERAISE_BAT;
		if(nCmd==0)	nPreCmd= 4;
		if(nCmd==1)	nPreCmd= 5;
		if(nCmd==2 || nCmd==3) nPreCmd= 6;
		if(nCmd==4)nPreCmd = 7; //올인
		break;
	default:  // 폴드/다이
		sign = ERAISE_FOLD;
		nPreCmd= 1;
		Ps[nPNum].bFold = TRUE; // 게임포기세팅
		if(Ps[nPNum].bCheck){
			Ps[nPNum].bCheck = FALSE;
			bBtnCheck = TRUE;
		}
		break;
	} // End of switch!!

	BOOL bContinue = m_Raise.RaiseCommand(nPNum, unum, sign,
			bBtnCheck, nCmd, nNextUser, bNextBtnBat, bNextBtnCheck);
	// 배팅 및 콜 했다면 머니 가감산한다
	if(nStyle==3 || nStyle==2){
		if(Ps[nPNum].bOnGame && !Ps[nPNum].bFold){
			if(unum >0 && unum < MAX_USER && faststrcmp(Ui[nPNum].ID, User[unum].UI.ID)== 0){
				User[unum].UI.PMoney = m_Raise.m_User[nPNum].nPMoney;
				Ui[nPNum].PMoney = User[unum].UI.PMoney;

				// ### [로그 기록용]###
				// 벡업용 베팅데이타를 세팅한다
				//User[unum].nBet = m_Raise.m_User[nPNum].nRealBat;
			}
		}
	}

	if(bContinue){ // 레이즈를 지속하라!!
		// 차기액티브사용자 세팅
		if(nNextUser<0 || nNextUser>=m_Max_Player) nNextUser = GetNextUser(nNextUser);
		if(strlen(Ui[nNextUser].ID)==0 || Ps[nNextUser].bFold) nNextUser = GetNextUser(nNextUser);

		Ps2[nNextUser].bActive = TRUE;
		Ps2[nNextUser].nActive = 0;
		Ps[nNextUser].bCheck  = bBtnCheck;


//		Ri.ChangeCardStep = 0;

		m_nPrimeUser = nNextUser;

		POKERGAME pg = MakeInfo();
		pg.bDistCard = FALSE; // 카드분배없다!
//		pg.bSex      = Ui[nPNum].Sex;
		pg.nPreCmd   = nPreCmd;
		pg.bBtnBet   = bNextBtnBat;
		pg.bBtnCheck = bBtnCheck;

		pg.CurPlayer = nNextUser;
		pg.LastPlayer = nPNum; // 칩날리기 위한 전 베팅자
		pg.LastStyle = nStyle; // 콜이면 2
		
		CSV_RAISE craise(SndBuf);
		craise.Set(&pg);
		SendMsgToAll(craise.pData, craise.GetTotalSize() );

		BOOL bOpen = FALSE;
		if(CheckGameOver(bOpen)){
			DoGameOver(bOpen);
		}
		return;
	}

	else{ // 레이즈 끝이다(새카드? 게임끝?)
		for(i=0;i<m_Max_Player;i++) Ps[i].bCheck = FALSE; 
	
		BOOL bOpen = FALSE;
		if(CheckGameOver(bOpen)){
			POKERGAME pg = MakeInfo();
//			pg.bSex      = Ui[nPNum].Sex;
			pg.nPreCmd   = nPreCmd;
			pg.bRaiseOn  = FALSE;

			pg.CurPlayer = nPNum;
			pg.LastPlayer = nPNum; // 칩날리기 위한 전 베팅자
			
			if(nStyle == 2) // 폴드시 호출 금지(콜이면 )
				pg.LastStyle = -1;

//			Ri.ChangeCardStep = 0;

			CSV_RAISE craise(SndBuf);
			craise.Set(&pg);
			SendMsgToAll(craise.pData, craise.GetTotalSize() );
			m_GameState = GAME_ROUND_RESULT; // 게임 오버 상태
			DoGameOver(bOpen);

			return;
		}
		else
		{
			if(nStyle == 2) // 콜 일때만
			{
				// [ 마지막 콜 추가 ]
				POKERGAME pg = MakeInfo();
				pg.nPreCmd   = nPreCmd;
				pg.bRaiseOn  = FALSE;
				pg.CurPlayer = nPNum;
				pg.LastStyle = -1;
				pg.LastPlayer = nPNum; // 칩날리기 위한 전 베팅자
				

				CSV_RAISE craise(SndBuf);
				craise.Set(&pg);
				SendMsgToAll(craise.pData, craise.GetTotalSize() );
			}
		}


// ####### 지연......

		int nSV[MAX_PLAYER]={0,};

		int nDistUser = GetWinner(FALSE,nSV);
		//char eightCard = 0;
		
		//막힌거 푼다. softpark
		if(m_Raise.GetRound()==3)      // 히든카드 
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

		//히든카드		
		for(i=0;i<m_Max_Player;i++){
			if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && !Ps[i].bFold) {
				if(bHiddenCard){ // 히든카드 분배다!!!
					//softpark 막힌거 푼다.
					Bc[i].nCard[2] = (char)GiveCard();
					m_GameState = 6;
				}
				else
				{ // 오픈 카드 별도 관리
					
					Ps[i].nCard[Ps[i].nOpenTotal] = (char)GiveCard();

					if(m_GameState == GAME_ROUND_5 && Ps[i].nOpenTotal == 3)
						m_GameState = GAME_ROUND_6;
					else if(m_GameState == GAME_ROUND_4 && Ps[i].nOpenTotal == 2)
					m_GameState=GAME_ROUND_5;

					Ps[i].nOpenTotal++;					
				}
				Ps[i].nCardTotal++;
			
				// 여기서 카드 날리기 세팅
				Ps2[i].bWindOn  = TRUE;
				Ps2[i].nWindOn  = 0;
			}
		}
		

		m_CardWindState = 1;


		int nActive = 0;
		if(bHiddenCard) nActive = nDistUser;
		else            nActive = GetWinner(FALSE,nSV);
		m_nPrimeUser = nActive; // 다음선 결정
		GetPrimer();
		Ps2[nActive].bActive = TRUE;
		Ps2[nActive].nActive = 0;
		Ps[nActive].bCheck  = TRUE; // 체크버튼 세팅

		m_Raise.Start(Ps); // 새로운 레이즈 준비하라!!

		POKERGAME pg = MakeInfo();
		pg.bDistCard = TRUE;
		pg.nDistUser = nDistUser;
//		pg.bSex      = Ui[nPNum].Sex;
		pg.nPreCmd   = nPreCmd;
		pg.bBtnBet   = bNextBtnBat;
		pg.bBtnCheck = TRUE;
		
		pg.CurPlayer = nActive;
		pg.LastPlayer = nPNum; // 칩날리기 위한 전 베팅자
		pg.LastStyle = nStyle;

		CSV_RAISE craise(SndBuf);
		craise.Set(&pg);
		SendMsgToAll(craise.pData, craise.GetTotalSize() );
		
		//softprk 막힌거 푼다.
		if(bHiddenCard) { // 히든카드 분배루틴이다
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


// 카드 섞기
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
		m_CardDeck[i] = k; // 52 장의 카드를 저장
	}

	m_CurCardIndex = 0;
/*
	// 0 유저
	m_CardDeck[0] = 0;
	m_CardDeck[1] = 10;
	m_CardDeck[2] = 11;
	
	// 1 유저
	m_CardDeck[3] = 10;
	m_CardDeck[4] = 23;
	m_CardDeck[5] = 36;
	
	// 2 유저
	m_CardDeck[6] = 10;
	m_CardDeck[7] = 45;
	m_CardDeck[8] = 10;

	// 0 유저
	m_CardDeck[9] = 13;
	// 1 유저
	m_CardDeck[10] = 5;
	// 2 유저
	m_CardDeck[11] = 14;

	// 0 유저
	m_CardDeck[12] = 9;
	// 1 유저
	m_CardDeck[13] = 17;
	// 2 유저
	m_CardDeck[14] = 18;

	// 0 유저
	m_CardDeck[15] = 12;
	// 1 유저
	m_CardDeck[16] = 38;
	// 2 유저
	m_CardDeck[17] = 20;
/*

	// 0 유저
	m_CardDeck[0] = 0;
	m_CardDeck[1] = 1;
	m_CardDeck[2] = 2;
	
	// 1 유저
	m_CardDeck[3] = 10;
	m_CardDeck[4] = 23;
	m_CardDeck[5] = 36;
	
	// 2 유저
	m_CardDeck[6] = 10;
	m_CardDeck[7] = 45;
	m_CardDeck[8] = 10;

	// 0 유저
	m_CardDeck[9] = 13;
	// 1 유저
	m_CardDeck[10] = 5;
	// 2 유저
	m_CardDeck[11] = 14;

	// 0 유저
	m_CardDeck[12] = 4;
	// 1 유저
	m_CardDeck[13] = 17;
	// 2 유저
	m_CardDeck[14] = 18;

	// 0 유저
	m_CardDeck[15] = 3;
	// 1 유저
	m_CardDeck[16] = 38;
	// 2 유저
	m_CardDeck[17] = 20;
	
/*

	// 0 유저
	m_CardDeck[0] = 0;
	m_CardDeck[1] = 13;
	m_CardDeck[2] = 26;
	
	// 1 유저
	m_CardDeck[3] = 10;
	m_CardDeck[4] = 23;
	m_CardDeck[5] = 36;
	
	// 2 유저
	m_CardDeck[6] = 10;
	m_CardDeck[7] = 45;
	m_CardDeck[8] = 10;

	// 0 유저
	m_CardDeck[9] = 2;
	// 1 유저
	m_CardDeck[10] = 4;
	// 2 유저
	m_CardDeck[11] = 14;

	// 0 유저
	m_CardDeck[12] = 15;
	// 1 유저
	m_CardDeck[13] = 17;
	// 2 유저
	m_CardDeck[14] = 18;

	// 0 유저
	m_CardDeck[15] = 1;
	// 1 유저
	m_CardDeck[16] = 38;
	// 2 유저
	m_CardDeck[17] = 20;

	// 0 유저
	m_CardDeck[18] = 4;
	// 1 유저
	m_CardDeck[19] = 50;
	// 2 유저
	m_CardDeck[20] = 45;
*/
/*

	// 0 유저
	m_CardDeck[0] = 0;
	m_CardDeck[1] = 2;
	m_CardDeck[2] = 3;
	
	// 1 유저
	m_CardDeck[3] = 13;
	m_CardDeck[4] = 15;
	m_CardDeck[5] = 16;
	
	// 2 유저
	m_CardDeck[6] = 10;
	m_CardDeck[7] = 45;
	m_CardDeck[8] = 10;

	// 0 유저
	m_CardDeck[9] = 6;
	// 1 유저
	m_CardDeck[10] = 19;
	// 2 유저
	m_CardDeck[11] = 14;

	// 0 유저
	m_CardDeck[12] = 7;
	// 1 유저
	m_CardDeck[13] = 20;
	// 2 유저
	m_CardDeck[14] = 18;

	// 0 유저
	m_CardDeck[15] = 1;
	// 1 유저
	m_CardDeck[16] = 38;
	// 2 유저
	m_CardDeck[17] = 20;

	// 0 유저
	m_CardDeck[18] = 4;
	// 1 유저
	m_CardDeck[19] = 50;
	// 2 유저
	m_CardDeck[20] = 45;
*/

}




// 랜덤 유일 카드 분배.
int CRoom::GiveCard()
{
	if(m_CurCardIndex < 0 || m_CurCardIndex > 51) return -1;
	int cardNum = m_CardDeck[m_CurCardIndex++];
	return cardNum;

}

// 랜덤값 구하기
int CRoom::GetRand(int nMod)
{
	int nRand = 0;
	nRand = (int)(rand()%nMod);
	if(nRand<0) nRand = -nRand;
	return nRand;
}

// 게임 선을 구한다.
int CRoom::GetPrimer()
{
	if(m_nPrimeUser<0) m_nPrimeUser = 0;
	if(m_nPrimeUser>Ri.MaxUserNum) m_nPrimeUser = 0;

	int nPrime=0; 
	BOOL bFind = FALSE;
	// 일단 기존 선이 올바른 선인가 판단한다.
	// 없다면 다음 사용자를 세팅한다.
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
		nSpecialVal[i] = 0; // 3: 로티플, 2:스티플, 1:포카드
		if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && !Ps[i].bFold){ // 사용되고 폴드(다이)가 아니라면 카드 해석한다.
			int aCalCard[8] = {0,};

			int nCardNum = Ps[i].nOpenTotal; // 일단 오픈카드로 계산			
			

			if(m_GameState == 100)//GAME_ROUND_CHANGECARD2)
			{
				if(nCardNum == 3)//히든 때 카드를 바꾼넘
				{					
					aCalCard[0] = Ps[i].nCard[0];
					aCalCard[1] = Ps[i].nCard[1];
					aCalCard[2] = Ps[i].nCard[2];					
				}
				if(nCardNum == 4)//히든때 카드를 바꾸지 않은넘
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
						for(int n=0 ; n < 4; n++){  // ###[8포커]###
							aCalCard[n] = Ps[i].nCard[n];
						}
					}					
				}
			}
			else 
			{
				for(int n=0;n<Min(nCardNum,4);n++){  // ###[8포커]###
					aCalCard[n] = Ps[i].nCard[n];
				}
			}

			// 게임종료시 모든카드 분석
			if(GameState==RSTATE_RESULT)
			{ 	
				//위에서 쓰레기값을 가지고 있을지 모르니 초기화 해준다.
				memset(aCalCard,-1,sizeof(aCalCard));

				nCardNum = Min(Ps[i].nCardTotal,7);  // ###[8포커]###
			
				aCalCard[0] = Bc[i].nCard[0];
				aCalCard[1] = Bc[i].nCard[1];

				for(int m=0;m<Min(Ps[i].nOpenTotal,4);m++){
					aCalCard[m+2] = Ps[i].nCard[m];
				}

				aCalCard[6] = Bc[i].nCard[2];

				//히든 때 카드를 바꾼넘
				//softpark
				/*if(Bc[i].nCard[2] != -1)
					aCalCard[5] = Bc[i].nCard[2];
				//히든 때 카드를 바꾸지 않은 넘
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
	// 스타터 및 승리자 결정루틴이다.
	int nStarter = m_Seven.GetWinner(aCardVal, aSecVal, auser, nTotalUser);
	if(nStarter<0 || nStarter>=m_Max_Player) nStarter = 0;
	return nStarter;

}

// [ 하이 로우 ]
int CRoom::GetHiLowWinner(BOOL bEnd,int nSpecialVal[]) //bEnd == true 마지막 승자
{
	return 0;
}


void CRoom::GetLowHandWinner()
{
}


// 차기사용자를 추출한다.
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

// 첫 3장을 보낸후 유저가 하나의 카드를 선택 => 모두 선택시 게임시작
void CRoom::AnalRaiseOn()
{
	if(bValid == FALSE) return;
	if(Ri.State!=1) return;

	// 게임시작여부 확인
	int nRemind = 0;
	for( int i = 0; i < m_Max_Player; i++) {
		if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && Ps[i].JoinState == 1 && Ps[i].PlayState == 1 ) {
			if( Ps2[i].bWindOn ) nRemind++;
		}
	}

	if(nRemind==0) 	{
		for( int i = 0; i < m_Max_Player; i++) {
			Ps2[i].bWindOn = FALSE; // 카드딜레이 타임제거
			Ps2[i].nWindOn = 0;
		}

		m_CardWindState = 0;
		// 여기서 게임 시작을 보냄(레이즈 시작)
		CSV_WINDCARDEND wend(SndBuf);
		wend.Set(0);
		SendMsgToAll(wend.pData, wend.GetTotalSize() );

		for( i = 0; i < m_Max_Player; i++)
		{
			if(strlen(Ui[i].ID) >0 && Ps[i].bOnGame && Ps[i].JoinState == 1 && Ps[i].PlayState == 1 )
			{
				if(Ps2[i].bActive == TRUE) Ps2[i].nActive = 0; // 다시 초기화 해준다 (이때 부터 턴 시간 다시 체크)
			}
		}

		// 카드 선택 부분이라면 모두에게 시간제한을 둔다.
		if(GameState == RSTATE_PREPARESTART)
		{
			// 첫시작 턴
			m_GameState = GAME_ROUND_CARDCHOICE; // 카드 선택 3장 
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

// 카드 날리는것을 끝냈다면
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


// 사용자가 카드를 선택하였다.
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
	Ps2[pnum].nFoldNum = 0; //  게임 오버시 초기화 하지 않고 여기서 초기화 한다

	int i=0;
	// 사용자카드재배치
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
				Bc[pnum].nCard[nIndex] = aBuf[i]; // 히든 카드( 현재 2장)
				nIndex++;
			}
		}
	}
	else
	{// ###[초이스]###
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
				Bc[pnum].nCard[nIndex] = aBuf[i]; // 히든 카드( 현재 2장)
				nIndex++;
			}
		}
	}

	Ps[pnum].nState     = 2;  
	Ps[pnum].nCardTotal = 3;	// 총3장
	Ps[pnum].nOpenTotal = 1;	// 백2장 + 오픈1
	Ps[pnum].nCard[0]   = nSelCard; // 선택된카드를 오픈카드로 지정

	Bc[pnum].nCard[2] = -1;
	Bc[pnum].nCard[3] = -1;

	// 카드 선택 타이머 막대 그리기
	CSV_SELECTCARD selcard(SndBuf);
	selcard.Set(Ri.RoomNum, Ui[pnum].UNum, pnum, Ui[pnum].ID);
	SendMsgToAll( selcard.pData, selcard.GetTotalSize() );


	StartRaise();

}

// 판종료후 일정시간후 게임셋팅
void CRoom::SendReset()
{
	if(bValid == FALSE) return;
	if(Ri.State==1) return;

	// 판 종료
	CSV_ENDGAME gend(SndBuf);
	gend.Set(0);
	SendMsgToAll(gend.pData, gend.GetTotalSize() );

	// 돈이 부족한사람...아웃시킨다
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
			//	smsg.Set(9, (char*)strMess.operator LPCTSTR()); // ## 파산창
				//SendMsgTo(Ui[i].ID, smsg.pData, smsg.GetTotalSize());
			//	User[unum].SendMsg(&smsg);
			//	OnUserOutRoom(unum, Ui[i].ID, FALSE);		

				
				// [수호천사] 2004.07.09
				BOOL bExist = FALSE;
				GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_SAFEGUARDANGEL, &bExist);
				if( !bExist ) {
				//	CSV_SERVERMSG smsg(SndBuf);
				//	CString strMess = "";
				//	smsg.Set(9, (char*)strMess.operator LPCTSTR()); // ## 파산창
				//	//SendMsgTo(Ui[i].ID, smsg.pData, smsg.GetTotalSize());
				//	User[unum].SendMsg(&smsg);
					OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
				}

			}			
		}
	}	

	// [게임 아이템 작업]
	// 강제추방 예약자가 있으면 퇴장시킴
	for(i=0; i<m_Max_NewPlayer; i++) {// ### [ 관전기능 ] ###
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

	// 게임리셋 명령을 전송한후 재시작카운트 시동 //softpark
	if(GameState==RSTATE_NONE && bRestartCnt==FALSE && Ri.NowUserNum >= MIN_PLAYER && Ri.NowUserNum < Ri.MaxUserNum){//시작버튼 관련	
		bRestartCnt=TRUE;
		RestartCnt=0;
		// 시작버튼 
		int nPrimer    = GetPrimer(); // 선구하기
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

// 고리금액을 산출한다
INT64 CRoom::CalcGory(INT64 nRealTotal)
{
	INT64 nGory    = (INT64)((GetPercentGory()*nRealTotal)/100); // 고리값 계산
	INT64 nRestVal = (INT64)(nGory%10);
	if(nRestVal<0) nRestVal = 0;
	nGory  -= nRestVal;
	if(nGory<10)   nGory    = 0;
	return nGory;
}

// 채널이나 게임스타일별로 고리금액율을 차별화 적용한다
int CRoom::GetPercentGory()
{
	int nVal = 1;

	switch(pChan->ChannelStyle){
		case 1: nVal =  5; break; // 초보 ( 2%)
		case 2: nVal =  5; break; // 중수 ( 3%)
		case 3: nVal =  5; break; // 고수 ( 3%)
		case 4: nVal =  5; break; // 도신 ( 3%)
	}
	return nVal;
}

// ### [ 관전기능 ] ### 
void CRoom::ExitSubScript(CCL_EXITSUBSCRIPT *pMsg, int pnum, int unum)
{
	if(pnum < 0 || pnum >= m_Max_NewPlayer) return;
	if(unum<1 || unum>=MAX_USER) return;

	if(Ui[pnum].UNum != unum) return;

	if( pnum >= m_Max_Player ) //관전자는 바로퇴장
	{
		User[unum].bExitSubscript = FALSE;
		OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
		
		// ### [이벤트 티켓] ###
		g_pMainView->SendChannelEventNotice(unum);
		return;
	}

	if( Ri.State == 0 || !Ps[pnum].bOnGame || Ps[pnum].bFold)
	{// 나가기
		User[unum].bExitSubscript = FALSE;
		OnUserOutRoom(unum, User[unum].UI.ID, FALSE);

		// ### [이벤트 티켓] ###
		g_pMainView->SendChannelEventNotice(unum);
	}
	else
	{// 나가기 예약/취소  
		
		if(*pMsg->bExit == 1 ) 
			User[unum].bExitSubscript = TRUE;  // 1 예약
		else 
			User[unum].bExitSubscript = FALSE; // 2 취소
		
		CCL_EXITSUBSCRIPT esmsg(SndBuf);
		esmsg.Set(Ri.RoomNum, unum, pMsg->ID, *pMsg->bExit);
		SendMsgToAll(esmsg.pData, esmsg.GetTotalSize());
	}
}

// ### [이벤트 티켓] ###
void CRoom::SendRoomEventNotice(int pnum)
{
	// 이벤트가 진행중이지 않다면 리턴
	if(!g_pMainView->EventCfg.bRunEvent) return;
	if(pnum<0 || pnum > m_Max_Player ) return;

	int unum = Ui[pnum].UNum;
	if(unum < 1 || unum >= MAX_USER) return;
	if(faststrcmp(Ui[pnum].ID, User[unum].UI.ID) != 0) return;

	// 게임룸 이벤트 공지가 있다면
	if(strlen(g_pMainView->EventCfg.RoomEventNotice) > 0) {

		CString str1 = g_pMainView->EventCfg.RoomEventNotice;
		CString str2;
		str2.Format("오늘 [%s]회원님이 획득한 이벤트 쿠폰은 %d장입니다. 쿠폰의 사용은 대박 경매이벤트 페이지를 참조하시기 바랍니다.", User[unum].UI.ID, User[unum].EventPrize.EventPrize);
		str1+="\n";
		str1+=str2;

		// 사용자에게 서버메세지를 보냄
		CSV_EVENTNOTICE enmsg(SndBuf);
		enmsg.Set(1, (char*)str1.operator LPCTSTR(), RGB(255,100,0));
		SendMsgTo(pnum, enmsg.pData, enmsg.GetTotalSize());
	}

}

////////////////////////////////////////////////////////////
/////////////////// ### [ 관전기능 ] ###  //////////////////
////////////////////////////////////////////////////////////

//관전 -> 참여
void CRoom::Participation(int unum, int pnum, int sndFxKind)
{
	if(pnum < m_Max_Player || pnum >= m_Max_NewPlayer) return;
	if(unum<1 || unum>=MAX_USER) return;
	if(strcmp(User[unum].UI.ID, Ui[pnum].ID) != 0) return;
	
	if( User[unum].bAdminUser ) return;
	if( User[unum].bObserver != TRUE) return;
	if( ReservationFindID(Ui[pnum].ID) ) {
		CSV_OBSERVER_MESSAGE smsg2(SndBuf);
		smsg2.Set(0, "이미 게임 참여 예약을 하였습니다");
		User[unum].SendMsg(&smsg2);
		return; // 예약되어 있다면
	}

	//겜참여 
	int epnum = -1;
	for(int i=0; i<Ri.MaxUserNum; i++)	{
		if(strlen(Ui[i].ID) == 0) { epnum = i; break; }
	}

	if(epnum == -1) {
		Make_a_Reservaiton(pnum, unum, sndFxKind); // 참여예약
		return;
	}

	int myIpIndex = Room_MyIp_Check(unum);// 아이피 중복검사
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

	int inOutCnt = Ps2[pnum].In_OutCount;//반복금지
	// 게임정보를 클리어
	ZeroMemory(&Ps[epnum], sizeof(PLAYSTATE));
	ZeroMemory(&Bc[epnum], sizeof(BACKCARD));
	ZeroMemory(&Ps2[epnum], sizeof(PLAYSTATE2));
	Ps2[epnum].In_OutCount = inOutCnt;// 반복 금지
	
	// ### [사운드 추가 작업] ###
	Ps[epnum].nSndFxKind = sndFxKind;
	Ps[epnum].nCheckIp = myIpIndex;

	memcpy(&Ui[epnum], &Ui[pnum], sizeof(USERINFO));
	ZeroMemory(&Ui[pnum], sizeof(USERINFO));

	Ri.NowUserNum++;
	if(Ri.NowObserverNum>0) Ri.NowObserverNum--;
	
	// ### [ 중복된 IP가 ] ###
	IPARRINDEX iparray;
	ZeroMemory(&iparray, sizeof(iparray));
	for( i=0;i<m_Max_Player;i++) { iparray.IpIndex[i]= (char)Ps[i].nCheckIp; }
		
	// 같은 방의 다른 사용자에게 새로운 사용자의 입장을 통보
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

	// 게임 시작 카운터 가동
	if(GameState==RSTATE_NONE && Ri.NowUserNum >= MIN_PLAYER && Ri.NowUserNum < Ri.MaxUserNum)
	{
		bRestartCnt=TRUE;
		RestartCnt=0;

		// 시작버튼 
		int nPrimer    = GetPrimer(); // 선구하기
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,1);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}
	// 모든 사용자에게 방정보가 변경되었음을 알리고 사용자 정보가 갱신됨을 알림
	
}

// ### [ 관전기능 ] ###
//참여 -> 관전
void CRoom::Emerge_Out(int unum, int pnum)
{
	if(pnum < 0 || pnum >= m_Max_Player ) return;
	if(unum<1 || unum>=MAX_USER) return;
	if(strcmp(User[unum].UI.ID, Ui[pnum].ID) != 0) return;
	if( User[unum].bObserver == TRUE) return;
	if(strcmp(User[unum].UI.ID, Ri.ID) == 0) { //방장
		CSV_OBSERVER_MESSAGE smsg2(SndBuf);
		smsg2.Set(0, "방장은 관전으로 이동할 수 없습니다");
		User[unum].SendMsg(&smsg2);
		return;
	}
		
	if(Observer_FindID(Ui[pnum].ID) )
	{
		CSV_OBSERVER_MESSAGE smsg2(SndBuf);
		smsg2.Set(0, "이미 관전 예약을 하였습니다.");
		User[unum].SendMsg(&smsg2);
		return;
	}

	if(Ps[pnum].bOnGame && !Ps[pnum].bFold) //예약
	{
		Make_a_Observer(pnum, unum);
		return; //겜중일경우
	}

	if(Ps2[pnum].In_OutCount++ > 0) //반복 횟수 제한
	{
		if(Ps2[pnum].In_OutCount >  5) {
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, "도배성 버튼 클릭으로 인하여, 자동 퇴장 되었습니다");
			User[unum].SendMsg(&smsg);

			OnUserOutRoom(unum, User[unum].UI.ID, FALSE); //경고
			return;
		}
		CSV_OBSERVER_MESSAGE smsg2(SndBuf);
		smsg2.Set(0, "다음 판에 이동 할 수 있습니다.");
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
		smsg2.Set(0, "관전자 자리가 없습니다.");
		User[unum].SendMsg(&smsg2);
		return;
	}


	LogRecordFunc(unum, pnum);  // ### [로그 기록용]###

	// ### [ 중복된 IP가 ] ###
	if(Ps[pnum].nCheckIp >= 0)
	{
		int ipindex= Ps[pnum].nCheckIp;
		int indexCnt=0;
		for(int i=0;i<m_Max_Player;i++)
		{
			if(pnum==i) continue;
			if(ipindex == Ps[i].nCheckIp) indexCnt++;
		}
		// 제거
		if(indexCnt == 1)
		{
			for(i=0;i<m_Max_Player;i++) {
				if(pnum==i) continue;
				if(ipindex == Ps[i].nCheckIp) Ps[i].nCheckIp = -1; // 모두 해제
			}
			
			m_aIpIndex[ipindex] = 0;
			m_nIpCnt--;
		}
	}
	
	int inOutCnt= Ps2[pnum].In_OutCount;//반복금지
	// 유저 정보를 삭제하고 게임참여상태를 클리어
	ZeroMemory(&Ps[pnum], sizeof(PLAYSTATE));
	ZeroMemory(&Ps2[pnum], sizeof(PLAYSTATE2));
	ZeroMemory(&Bc[pnum], sizeof(BACKCARD));
	// ### [ 중복된 IP가 ] ###
	Ps[pnum].nCheckIp = -1; // $$$ 기본값 -1
	ZeroMemory(&Us[pnum], sizeof(USERSTATE));	// [게임 아이템 작업]
	// 사용자 삭제
	ZeroMemory(&Ui[pnum], sizeof(USERINFO));
	// 나가기 예약상태 초기화
	User[unum].bExitSubscript = FALSE;

	// 현재인원에서 한명 삭제
	Ri.NowUserNum--;
	Ri.NowObserverNum++;
	
	User[unum].bObserver = TRUE;
	User[unum].UI.cObserver = 1;
	ZeroMemory(&Ps2[epnum], sizeof(PLAYSTATE2));
	memcpy(&Ui[epnum], &User[unum].UI, sizeof(USERINFO));
	Ps2[epnum].In_OutCount = inOutCnt;// 반복 금지
		
	// 플레이어 번호 초기화
	User[unum].PNum = epnum;
	// ### [로그 기록용]###
	// 백업용 배팅액 초기화
	User[unum].nBet = 0;

	// ### [ 중복된 IP가 ] ###
	IPARRINDEX iparray;
	ZeroMemory(&iparray, sizeof(iparray));
	for(  i=0;i<m_Max_Player;i++)
		iparray.IpIndex[i]= (char)Ps[i].nCheckIp;


	// 만일 방장이 나간 경우라면 방장이 변경됨을 통보한다.
	if(faststrcmp(Ri.ID, User[unum].UI.ID) == 0) {
		int mastnum = -1;
		for(int i=0; i<m_Max_NewPlayer; i++)
			if(strlen(Ui[i].ID) != 0) { mastnum = i; break; }
			
			if(mastnum != -1) {
				// ### [ 관전기능 ] ###
				//대기중이넘 겜 참여 시킨다
				
				strncpy(Ri.ID, Ui[mastnum].ID, 15);
				Ri.UNum = Ui[mastnum].UNum;
				
				// 방장이 바뀜을 알린다
				CSV_CHANGEROOMMASTER crmmsg(SndBuf);
				crmmsg.Set(Ri.RoomNum, Ui[mastnum].UNum, Ui[mastnum].ID);
				SendMsgToAll(crmmsg.pData, crmmsg.GetTotalSize());
				
				// [세븐포커] 카드스위칭
				Ri.nCard = Ui[mastnum].PI.nCard;
			}
		/*	
			// [게임 아이템 작업]
			if(Ri.bSuperMaster) {
				// 슈퍼방이라면 방폭
				if(!bSuperRoomDestory) {
					bSuperRoomDestory = TRUE;
					
					if(Ri.State != 0) {
						// 게임중이라면 방폭 예약 메시지 알림
						CSV_SERVERMSG smsg(SndBuf);
						smsg.Set(0, "슈퍼방장이 퇴장하였으므로 이번 게임이 끝나면 모두 자동으로 퇴장하게 됩니다.");
						SendMsgToAll(smsg.pData, smsg.GetTotalSize());
					}
				}
			}
		*/
	}
	
	// 선이 나갔다면 
	if(faststrcmp(m_DealerID, User[unum].UI.ID) == 0) {
		strncpy(m_DealerID, Ri.ID, 15);
	}

	// 같은 방의 다른 사용자에게 새로운 사용자의 입장을 통보
	CSV_ENTER_EMERAGE entermsg(SndBuf);
	entermsg.Set(Ri.RoomNum, pnum, epnum, Ui[epnum].ID, &iparray);
	SendMsgExept(epnum, entermsg.pData, entermsg.GetTotalSize() );


	CSV_ACCEPT_EMERAGE emerage(SndBuf);
	emerage.Set(Ri.RoomNum, pnum, epnum, Ui[epnum].ID, &iparray);
	SendMsgTo(Ui[epnum].ID, emerage.pData, emerage.GetTotalSize());


	// 모든 사용자에게 방정보가 변경되었음을 알림
	CSV_CHANGEROOM crmsg(SndBuf);
//	SMALLROOMINFO sri;
//	sri = GetSmallRoomInfo();
	crmsg.Set(&GetSmallRoomInfo());
	pChan->SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());

	// 게임 시작 카운터 가동
	if(GameState == RSTATE_NONE && Ri.NowUserNum < MIN_PLAYER)
	{
		bRestartCnt=FALSE;
		RestartCnt=0;

		// 시작버튼 
		int nPrimer    = GetPrimer(); // 선구하기
		CSV_ACTIVE_STARTBUTTON startmsg;
		startmsg.Set(this->Ri.RoomNum,nPrimer,Ui[nPrimer].ID,0);
		SendMsgTo(Ui[nPrimer].ID,startmsg.pData,startmsg.GetTotalSize());
	}
	
	Reserve_takeaSeat();
}

// ### [ 관전기능 ] ###
// 아이피 
int CRoom::Room_MyIp_Check(int unum)
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// ### [ 중복된 IP가 ] ###
	int myIpIndex = -1;
	if(g_pMainView->Cfg2.SameIPLimit > 0) {
		int samecnt = 0;
		int tmpIp[MAX_PLAYER]={0,};
		
		// 같은 IP 대역이 몇개 존재하는지 검사
		for(int i=0; i<m_Max_Player; i++) {
			int tarunum = Ui[i].UNum;
			if(tarunum<1 || tarunum >= MAX_USER) continue;
			if(User[unum].PeerIP.S_un.S_un_b.s_b1 == User[tarunum].PeerIP.S_un.S_un_b.s_b1 && 
				User[unum].PeerIP.S_un.S_un_b.s_b2 == User[tarunum].PeerIP.S_un.S_un_b.s_b2 && 
				User[unum].PeerIP.S_un.S_un_b.s_b3 == User[tarunum].PeerIP.S_un.S_un_b.s_b3)// &&
				//User[unum].PeerIP.S_un.S_un_b.s_b4 == User[tarunum].PeerIP.S_un.S_un_b.s_b4) // 테스트
			{
				samecnt++;
				tmpIp[i]=1;
				myIpIndex = i;
			}
		}
		
		if(samecnt >= g_pMainView->Cfg2.SameIPLimit) {
			// 중복된 IP가 존재하여 입장 불가
			//ErrCode = 15;
			myIpIndex = -2;
		}
		else if(samecnt !=0) // 추가 클라이언트에서 이미지로 보여준다.(같은 아피)
		{
			if(samecnt == 1) // 처음
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
			{// 기존
				myIpIndex = Ps[myIpIndex].nCheckIp;
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	return myIpIndex;
	
}



////////////////// ### [ 관전기능 ] ### ////////////////////
// 예약 관련 (관전 => 참여)
void CRoom::Make_a_Reservaiton(int pnum, int unum, int nSndFxKind)  // 참여예약
{
	RESERVEDINFO *pList = (RESERVEDINFO*) new RESERVEDINFO;
	pList->PNum = pnum;
	pList->UNum = unum;
	pList->nSndFxKind = nSndFxKind;
	pList->pUI = &Ui[pnum];
	m_ReservedList.AddTail(pList); 
	GetReservaitonInfo();

	CString str;
	str.Format("참여자로 예약되었습니다. %d순위", GetReservationSequential() );

	CSV_OBSERVER_MESSAGE smsg2(SndBuf);
	smsg2.Set(0, (char*) str.operator LPCTSTR() );
	User[unum].SendMsg(&smsg2);
}

//예약순서
int CRoom::GetReservationSequential()
{
	return m_ReservedList.GetCount();
}


void CRoom::Cancel_a_Reservation(char *id)  //참여예약 취소
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

void CRoom::Reserve_takeaSeat() // 자리 앉기
{
	if(Ri.NowUserNum < Ri.MaxUserNum && m_ReservedList.GetCount() > 0){
		RESERVEDINFO *pList =  (RESERVEDINFO*)m_ReservedList.GetHead();
		m_ReservedList.RemoveHead();
		Participation(pList->UNum, pList->PNum, pList->nSndFxKind);
		delete pList;
		GetReservaitonInfo();
	}
}

BOOL CRoom::ReservationFindID(char *id) // 예약 되어 있는가
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

void CRoom::ReservedRemoveAll() // 제거
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

void CRoom::GetReservaitonInfo() // 예약정보
{
	if(m_ReservedList.GetCount()==0) return;
	int cnt = 0;
	RESERVATIONINFO sInfo[MAX_OBSERVER] = {0,};
	
	RESERVEDINFO *pList = NULL;
	POSITION pos = m_ReservedList.GetHeadPosition();
	while(pos)	{
		pList = (RESERVEDINFO*)m_ReservedList.GetNext(pos);
		if(pList != NULL) {
			sInfo[cnt].Seq = cnt; //예약순위
			sInfo[cnt].UNum = pList->UNum;
			strncpy(sInfo[cnt].ID, pList->pUI->ID, 15);
			cnt++;
#ifdef _DEBUG
			if(cnt >= MAX_OBSERVER -1) {
				AfxMessageBox("범위초과");
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
//참여 -> 관전 예약
void CRoom::Make_a_Observer(int pnum, int unum)
{
	RESERVEDINFO *pList = (RESERVEDINFO*) new RESERVEDINFO;
	pList->PNum = pnum;
	pList->UNum = unum;
	pList->pUI = &Ui[pnum];
	m_ObservedList.AddTail(pList); 

	CSV_OBSERVER_MESSAGE smsg2(SndBuf);
	smsg2.Set(0, "관전 예약을 하였습니다. 게임 종료 후 관전 상태로 이동 합니다.");
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
						Cancel_a_Observer(id); //제거
						Emerge_Out(unum, pnum);
						return;
					}
				}
			}
		}
	}
}

//관람예약유저 이동
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
//////////////// ### [관리자 모드 작업] ### ////////////////
////////////////////////////////////////////////////////////

void CRoom::Admin_Room_Remove()
{
	for(int i=0; i<m_Max_NewPlayer; i++) {
		if(strlen(Ui[i].ID) > 0) {
			int unum = Ui[i].UNum;
			if(unum < 1 || unum >= MAX_USER) continue;
			if(faststrcmp(Ui[i].ID, User[unum].UI.ID) != 0) continue;
			
			// 사용자에게 서버메세지를 보냄
			CSV_SERVERMSG smsg(SndBuf);
			smsg.Set(1, "관리자에 의해 방이 삭제되었습니다.");
			User[unum].SendMsg(&smsg);

			OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
		}
	}
}

//[방장기능] 방옵션을 변경 한다
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

	// 같은 방인원에게 알림.
	CSV_CHANGESTYLEROOMINFO scrm(SndBuf);
	scrm.Set(&croom);
	SendMsgToAll(scrm.pData, scrm.GetTotalSize());

	// 대기실에 알림.
	pChan->SendMsgToLoby(scrm.pData, scrm.GetTotalSize());

}

// [관리자 모드 작업] 방옵션을 변경한다
void CRoom::ChangeRoomOption_ADMIN(CNM_ADMIN_CHANGEROOM *pMsg)
{
	if(bValid == FALSE) return;
	strncpy(Ri.Title, pMsg->STYLE->Title,30);

	CString str;
	str = "운영자에 의해 방제가 변경되었습니다";

	CHANGESTYLEROOMINFO croom={0,};
	strncpy(croom.ID, Ri.ID, 16);
	croom.UNum = Ri.UNum;
	croom.nCard = 0;
	croom.RoomNum = Ri.RoomNum;
	memcpy(croom.Title, Ri.Title, 30);

	// 같은 방인원에게 알림.
	CSV_CHANGESTYLEROOMINFO scrm(SndBuf);
	scrm.Set(&croom);
	SendMsgToAll(scrm.pData, scrm.GetTotalSize());

	// 대기실에 알림.
	pChan->SendMsgToLoby(scrm.pData, scrm.GetTotalSize());
}


// ### [로그 기록용]###
void CRoom::LogRecordFunc(int unum, int pnum)
{
	if(unum<1 || unum >= MAX_USER) return;
	if(pnum<0 || pnum >= m_Max_Player) return;

	if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse) {
		
		if(User[unum].UI.PMoney < 0) User[unum].UI.PMoney = 0;
		INT64 CurMoney = User[unum].nBet - User[unum].UI.PMoney;
		if(pnum < m_Max_Player && Ps[pnum].bFold && CurMoney > g_LogRangeValue) {// ### [ 관전기능 ] ###   // 10억 부터 1조
			
			if(LogDBMan.DBQue.GetTotNum() >= g_pMainView->Cfg2.LoginDBQueNum2) {
				CLogFile logfile("LogRecordDB.txt");
				logfile.Writef("LogRecordDB 에러 01, id=%s, curmoney=%I64d, losemoney=%I64d, winid=netmarble",Ui[pnum].ID, User[unum].UI.PMoney, CurMoney);
			}
			else {
				// 소켓의 유니크를 구분하기위한 Time Tick
				//int StartTick = timeGetTime();
				
				LOGREC_DBWORKS LWork;
				ZeroMemory(&LWork, sizeof(LOGREC_DBWORKS));
				LWork.StartTick = timeGetTime();
				LWork.bValid = TRUE;
				LWork.WorkKind = 1; // [ 통계기록 ] <-----------------------------추가
				
			//	LWork.LogRecord.Status = 72; // 폴드후 게임 종료
				LWork.LogRecord.Status = 2; // 폴드후 게임 종료
				
				strncpy(LWork.LogRecord.ID, User[unum].UI.ID, 15);
				LWork.LogRecord.CurMoney = User[unum].UI.PMoney;
				LWork.LogRecord.LoseMoney = CurMoney;
				
				strcpy(LWork.LogRecord.Win_ID, "netmarble");
				LWork.LogRecord.Win_CurMoney = -9999; //?????
				LWork.LogRecord.Win_GetMoney = -9999;
				
				strcpy(LWork.LogRecord.My_ip, User[unum].UserIP);
				//strcpy(LWork.LogRecord.Winner_ip, "netmarble");
				wsprintf(LWork.LogRecord.Winner_ip,"%d번방", Ri.RoomNum);
				
				
				// DB 쓰레드 가동
				BOOL rtn = LogDBMan.SetDBWork(&LWork);
				if(rtn == FALSE) {
					CLogFile logfile("LogRecordDB.txt");
					logfile.Writef("LogRecordDB 에러 02, id=%s, curmoney=%I64d, losemoney=%I64d, winid=netmarble",Ui[pnum].ID, User[unum].UI.PMoney, CurMoney);
				}
			}
		}
	}
}
