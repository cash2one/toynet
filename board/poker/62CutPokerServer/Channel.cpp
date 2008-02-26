// Channel.cpp: implementation of the CChannel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Channel.h"

#include "UserStruct.h"
#include "CommMsg.h"
#include "User.h"
#include "62CutPokerServerView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 정적 맴버
//char CChannel::SndBuf[SENDQUESIZE];

CChannel::CChannel()
{
	ChNum = -1;
	TotUser = 0;
	ChannelStyle = 0; // 게임 채널 종류 - 0: 골드머신
	JackPotSaveRatio=1;
	m_betmoney_5user = 0;
	m_betmoney_6user = 0;
	memset(Cid, 0, sizeof(Cid));
	m_channel_usernum = 0;
}

CChannel::~CChannel()
{

}

void CChannel::Init(const int cnum)
{
	ChNum = cnum;
	TotUser = 0;
	for(int i=0; i<MAX_ROOM; i++) {
		Room[i].pChan = this;
//		Room[i].SndBuf = SndBuf;
	}
	memset(Cid, 0, sizeof(Cid));

	CString str = g_pMainView->Cfg3.Info[ChNum].ChanName;

	strChanName = g_pMainView->Cfg3.Info[ChNum].ChanName;
	// 채널 등급
	if(g_pMainView->Cfg3.EnterLimitMode == 1 && g_pMainView->Cfg3.bChanLimit) {
		ChannelStyle = g_pMainView->Cfg3.Info[ChNum].Grade; // 게임 채널 종류 
	}
	else 
		ChannelStyle = g_pMainView->Cfg3.Info[ChNum].Grade; // 게임 채널 종류 

	m_channel_usernum = g_pMainView->Cfg3.Info[ChNum].GameKind; // 게임 채널 종류  

	// 잭팟 적립 비율
	JackPotSaveRatio = 1;
	// 잭팟 상금 지급비율(로티플)
	switch(ChannelStyle)
	{
	case 0: // 자유
		m_betmoney_5user = 100;
		m_betmoney_6user = 100;

		DefaultJackPot = g_pMainView->Cfg4.Free;			// 기본 잭팟 머니
		JackPotSaveRatio = g_pMainView->Cfg4.rFree;			// 적립 비율
		break;
	case 1: // 초보
		m_betmoney_5user = 2;								// 기본 삥값 처리
		m_betmoney_6user = 100;

		DefaultJackPot = g_pMainView->Cfg4.ChoBo;
		JackPotSaveRatio = g_pMainView->Cfg4.rChoBo;
		break;
	case 2: // 중수
		m_betmoney_5user = 50;	
		m_betmoney_6user = 100;

		DefaultJackPot = g_pMainView->Cfg4.JungSu;
		JackPotSaveRatio = g_pMainView->Cfg4.rJungSu;
		break;
	case 3: // 고수	
		m_betmoney_5user = 1000;
		m_betmoney_6user = 100;

		DefaultJackPot = g_pMainView->Cfg4.GoSu;
		JackPotSaveRatio = g_pMainView->Cfg4.rGosu;
		break;
	case 4: // 도신
		m_betmoney_5user = 10000;	
		m_betmoney_6user = 100;

		DefaultJackPot = g_pMainView->Cfg4.God;
		JackPotSaveRatio = g_pMainView->Cfg4.rGod;
		break;
	
	default: // 0
		m_betmoney_5user = 10;
		m_betmoney_6user = 100;

		DefaultJackPot = g_pMainView->Cfg4.Free;
		JackPotSaveRatio = g_pMainView->Cfg4.rFree; 
		break;
	}
}

// ### [ 채널 입장 구분 ] ###
const CHANNELINFO CChannel::GetChannelInfo()
{
	CHANNELINFO ci={0,};
	ci.ChNum = ChNum;
	ci.NowUserNum = TotUser;
	ci.MaxUserNum = MAX_CHUSER;
	strncpy(ci.Title, strChanName, 19);
	// 2003 06021 채널 등급 표시(색깔)
	ci.ChannelStyle = ChannelStyle;
	ci.channel_usernum = m_channel_usernum;
	return ci;
}

void CChannel::SendMsgToAll(char *pMsgData, int nMsgSize)
{
	int sid;
	for(int i=0; i<MAX_CHUSER; i++) {
		if(Cid[i] == 0) continue;
		if(!User[Cid[i]].bValid) continue;
		sid = User[Cid[i]].Sid;
		SockMan.SendData(sid, pMsgData, nMsgSize);
	}
}

void CChannel::SendMsgToLoby(char *pMsgData, int nMsgSize)
{
	int sid;
	for(int i=0; i<MAX_CHUSER; i++) {
		if(Cid[i] <= 0) continue;
		if(!User[Cid[i]].bValid) continue;
		if(User[Cid[i]].SUI.Position != WH_LOBY) continue;
		sid = User[Cid[i]].Sid;
		SockMan.SendData(sid, pMsgData, nMsgSize);
	}
}

void CChannel::SendMsgToLobyOpen(char *pMsgData, int nMsgSize)
{
	int sid;
	for(int i=0; i<MAX_CHUSER; i++) {
		if(Cid[i] <= 0) continue;
		if(!User[Cid[i]].bValid) continue;
		if(User[Cid[i]].SUI.Position != WH_LOBY) 
			if(User[Cid[i]].SUI.Position != WH_GAME || !User[Cid[i]].bLobyOpen) continue;
		sid = User[Cid[i]].Sid;
		SockMan.SendData(sid, pMsgData, nMsgSize);
	}
}

int CChannel::OnNewUser(int unum, int &nUseItem)
{
	if(TotUser >= MAX_CHUSER) return -1;
	if(unum < 1 || unum >= MAX_USER) return -2;
	if(!User[unum].bValid) return -2;
	if(User[unum].ChNum != -1) return -3;
/*
	// OnNewUser() 리턴값
	// code :
	//         >= 0  정상 채널 입장;
	//		     -3  현재 등급상 점프 아이템 구매(다이얼로그 박스)	
	//           -4  점프아템 조차 해결않됨(2단계 이상 차이남)
	//			 -6  지금 사용않함(채널별 한도 금액)
	//			 -7  현재 등급에 입장 할 수 없는데 점프아이템은 있네(사용할것인가 물어?)
	//         <  0  ( 채널이 꽉찼음,...)
	//
	// nUseItem :
	//		      0  일반적 채널 입장
	//            1  이미 한번이라도 사용한 점프아이템을 가지고 입장
*/

	// [게임 아이템 작업]
	BOOL bAskUseJumpItem = FALSE;
	//### [관리자 모드 작업] ###
	// 채널 입장 제한 모드이면
	if(!User[unum].bAdminUser && g_pMainView->Cfg3.EnterLimitMode == 1 && g_pMainView->Cfg3.bChanLimit) {
		int nError = 0;// 채널 등급 제한에 따른 입장 가능여부를 판단
		if(!CheckGameLevel(User[unum].UI.PMoney, User[unum].UI.nIcon, 0, 0, nError))
		{	
			BOOL bAccessDeny = FALSE;
			BOOL bJumpUsing = FALSE;
			BOOL bJumpExist = FALSE;
			bJumpUsing = GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_JUMP, &bJumpExist);
			int level = User[unum].UI.nIcon;
			int grade =  GetMyLeveltoGrade(level); // 레벨 -> 등급

			// ### [ 채널 입장 구분 ] ###
			if(grade+1 >= ChannelStyle && grade < ChannelStyle) // 한단계 위로 점프
			{
				// 점프 가능
				// 입장 제한 레벨보다 자신의 레벨이 낮은 경우는 점프 입장권의 구입 의사나 사용 여부를 물어본다
				if( !bJumpUsing && 
					CheckPremiumState(&User[unum].UI, NULL)==0 &&	// [프리미엄 작업]
					!User[unum].UI.bPremIP )						// [PC방 작업]
				{
					// 점프아이템을 사용하고 있지 않고 프리미엄 회원도 아니라면

					if(bJumpExist) {
						// 점프 아이템이 존재한다면 사용 여부를 물어보게 한다(일단 로그인 처리는 계속 진행)
						bAskUseJumpItem = TRUE;
					}
					else {
						// 점프 아이템을 사용하면 입장할 수 있는 등급인 경우는 구입 의사를 물어봄(일단 로그인 처리는 계속 진행)
					//	bAskBuyJumpItem = TRUE;

						CString strmsg;
						strmsg.Format("레벨이 낮아 선택하신 채널에 입장하시려면 [점프 입장권 아이템]이 필요합니다. [점프 입장권 아이템]을 구입하시겠습니까?");

						BUYGAMEITEMINFO bi = {0,};
						bi.Kind = 2;	// 채널 입장시 점프 입장권 아이템의 구입 의사를 물어봄
						strcpy(bi.ItemName, "점프 입장권 아이템");
						bi.ItemCode = ITEM_SCODE_JUMP;
						//strcpy(bi.ItemURL, g_pMainView->Cfg2.ItemURL);
						bi.StrMsgLen = strmsg.GetLength();

						CSV_BUYGAMEITEM bgmsg(SndBuf);
						bgmsg.Set(&bi, strmsg);
						User[unum].SendMsg(&bgmsg);
						return -100;
					}
				}
				else {
					// 점프 아이템을 사용중이거나 프리미엄 회원이라면 입장 허가
					bAccessDeny = FALSE;
				}
			}
			else
			{  // 불가 
				bAccessDeny = TRUE;

				// [ 총액제한 ] 2004.04.20
				// 프리미엄 
				if(grade-1 == ChannelStyle) {
					if(CheckPremiumState(&User[unum].UI, NULL)!=0) bAccessDeny = FALSE;
				}
			}
			if(bAccessDeny) {
				// 점프 아이템도 없고 등급이 맞지 않는 경우는 채널에 입장할 수 없음
				return -4;
			}
		}
	}

	// [게임 아이템 작업]
	if(bAskUseJumpItem) {
		// 점프 입장권 아이템이 존재한다면 사용 여부를 물어봄

		// 아이템 사용 설명
		char* strmsg1 = "레벨이 낮아 선택하신 채널에 접속하시려면 [점프 입장권 아이템]이 필요합니다.\n현재 소유하신 [점프 입장권 아이템]을 사용하시겠습니까?";
		char* strmsg2 = "[점프 입장권 아이템]을 사용하시면 채널의 등급이 높아도 입장하실 수 있습니다. 보유하신 아이템은 아래와 같으며, 아이템을 사용하시려면 목록에서 아이템을 선택후 더블클릭하거나 [사용하기] 버튼을 클릭하시면 됩니다.";

		GAMEITEM_INFO in = {0,};
		in.Kind = 2;	// 채널 입장시 점프 입장권 아이템 사용을 물어봄
		in.StrMsgLen1 = strlen(strmsg1);
		in.StrMsgLen2 = strlen(strmsg2);
		CTime time = CTime::GetCurrentTime();
		time.GetAsSystemTime(in.ServerTime);

		/*
		// 사용 가능한 점프 입장권 아이템만 추려냄
		GAMEITEM gitem[MAX_ITEM_NUM] = {0,};
		int totnum = 0;
		for(int i=0; i<User[unum].GameItem.nNum; i++) {
			if(User[unum].GameItem.Item[i].Code >= ITEM_SCODE_JUMP && 
				User[unum].GameItem.Item[i].Code <= ITEM_ECODE_JUMP) 
			{
				ItemState state = GameItem_GetItemState(User[unum].GameItem.Item[i]);
				if(state.iState == 1) {
					gitem[totnum] = User[unum].GameItem.Item[i];
					totnum++;
				}
			}
		}
		in.TotNum = totnum;
		*/
		// 모든 아이템을 추려냄
		GAMEITEM gitem[MAX_ITEM_NUM] = {0,};
		int totnum = 0;
		for(int i=0; i<User[unum].GameItem.nNum; i++) {
			gitem[totnum] = User[unum].GameItem.Item[i];
			totnum++;
		}
		in.TotNum = totnum;
	
		CSV_GAMEITEMLIST glmsg;
		glmsg.Set(&in, gitem, strmsg1, strmsg2);
		User[unum].SendMsg(&glmsg);

		return -101;
	}

	int cid = -1;
	for(int i=0; i<MAX_CHUSER; i++) if(Cid[i] == 0) { cid=i; break;}
	if(cid == -1) return cid;

	// 사용자 채널 입장 처리
	Cid[cid] = unum;
	User[unum].ChNum = ChNum;
	User[unum].Cid = cid;

	User[unum].SUI.Position = WH_LOBY;
	User[unum].SUI.RoomNum = 0;

	// 모든 채널 사용자에게 새로운 유저가 채널에 입장하였음을 알림
	//### [관리자 모드 작업] ###
	if( User[unum].bAdminUser )
	{
		//등급별 머니 셋팅
	}

	// 모든 채널 사용자에게 새로운 유저가 채널에 입장하였음을 알림
	CSV_NEWUSER numsg(SndBuf);
	numsg.Set(&User[unum].SUI);
	SendMsgToLobyOpen(numsg.pData, numsg.GetTotalSize());
	
	TotUser++;
	return cid;
}

BOOL CChannel::OnOutUser(int unum)
{
	if(TotUser <= 0) return FALSE;
	if(unum < 1 || unum >= MAX_USER) return FALSE;
	if(!User[unum].bValid) return FALSE;
	if(User[unum].ChNum != ChNum) return FALSE;

	int cid = User[unum].Cid;
	if(cid < 0 || cid >= MAX_CHUSER) return FALSE;
	if(Cid[cid] != unum) return FALSE;

	// 게임 중에 종료되는 경우라면
	if(User[unum].SUI.Position == WH_GAME) {
		int rnum = User[unum].SUI.RoomNum;
		if(rnum > 0 && rnum < MAX_ROOM) {
			// 사용자 방 퇴장 처리
			Room[rnum].OnUserOutRoom(unum, User[unum].UI.ID, TRUE);
		}
		else {
			CLogFile logfile;
			logfile.Writef("CChannel::OnOutUser() - 잘못된 방번호로 인한 방퇴장 실패(rnum=%d)", rnum);
		}
	}

	// 모든 채널 사용자에게 유저가 채널에서 퇴장 했음을 알림
	CSV_OUTUSER oumsg(SndBuf);
	oumsg.Set(&User[unum].SUI);
	SendMsgToLobyOpen(oumsg.pData, oumsg.GetTotalSize());

	// 사용자 채널 퇴장 처리
	Cid[cid] = 0;
	User[unum].ChNum = -1;
	User[unum].Cid = -1;

	User[unum].SUI.Position = WH_CHAN;
	User[unum].SUI.RoomNum = 0;

	TotUser--;
	return TRUE;
}

void CChannel::OnGlobalGameTimer()
{
	for(int i=1; i<MAX_ROOM; i++) {
		if( Room[i].bValid ) Room[i].OnGlobalGameTimer();
	}
}

BOOL CChannel::ProcessPacket(int sid, USERVAL *pUserVal, char* lpdata, int totsize, int signal)
{
	// 유효한 유저가 아니면 처리하지 않음
	if(pUserVal->iUser1 == FALSE) return FALSE;

	switch(signal) // 메시지 신호
	{
	case SV_ASK_BADUSER: // 대화내용 신고 요청
		{
			CSV_ASK_BADUSER MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RNum;
			int unum = *MsgData.UNum;
			int l_WatchID = *MsgData.l_StrWatchID;
			int l_StrChat = *MsgData.l_StrChat;

			// 에러검사
			if(unum < 1 || unum >= MAX_USER) break;
			if(strlen(MsgData.ID) == 0 || strlen(MsgData.ID) >= 16) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;
			if(l_WatchID<0 || l_WatchID>256) break;
			if(l_StrChat<=0 || l_StrChat>20000) break;

			// 스트링 메모리 할당
			char *pWatchID = new char[l_WatchID+2];
			memset(pWatchID, 0, l_WatchID+2);
			strncpy(pWatchID, MsgData.WatchID, l_WatchID);
			char *pChat = new char[l_StrChat+2];
			memset(pChat, 0, l_StrChat+2);
			strncpy(pChat, MsgData.StrChat, l_StrChat);

			// 현재 날짜 및 시간
			CString Date;
			CTime t = CTime::GetCurrentTime();
			Date.Format("%4d/%2d/%2d(%2d:%2d:%2d)", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

			CBadUserDBWorks Work;
			Work.bValid = TRUE;
			Work.ServerCode = g_pMainView->Cfg.ServerCode;
			Work.GameCode = g_pMainView->Cfg.GameCode;
			Work.MyID = MsgData.ID;
			Work.TargetID = MsgData.TargetID;
			Work.SubmitDate = Date;
			Work.SubmitReason = MsgData.SubmitReason;
			Work.WatchID = pWatchID;
			Work.Contents = pChat;

			// BadUserDB Work 큐에 처리할 일 삽입
			BadUserDBQue.EnQue(Work);
	
			if(pWatchID) delete pWatchID;
			if(pChat) delete pChat;

			CString str;

			if(*MsgData.bAnnounce == FALSE)
			{
				str.Format("신고내용이 접수되었습니다.(이메세지는 회원님에게만 통보됩니다)");
				CSV_SERVERMSG smsg(SndBuf);
				smsg.Set(0, (char*)str.operator LPCTSTR());
				SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
			}
			else
			{
				str.Format("<%s>님이 대화내용을 신고하였습니다", MsgData.ID);
				if(rnum == 0)
				{
					CSV_SERVERMSG smsg(SndBuf);
					smsg.Set(0, (char*)str.operator LPCTSTR());
					// 대기실에 있는 모든 사용자에게 보냄
					SendMsgToLoby(smsg.pData, smsg.GetTotalSize());
				}
				else
				{
					if(rnum<1 || rnum>=MAX_ROOM) break;
					// 같은 방안의 사용자에게 보냄
					CSV_SERVERMSG smsg;
					smsg.Set(0, (char*)str.operator LPCTSTR());
					Room[rnum].SendMsgToAll(smsg.pData, smsg.GetTotalSize());
				}
			}
		} break;

	case SV_ASK_ALLUSERINFO: // 모든 사용자 목록 요청
		{
			// 특별한 메세지가 없으므로 메세지 분석은 필요없다
			
			SMALLUSERINFO sui[MAX_CHUSER];
			int totnum=0;
			for(int i=0; i<MAX_CHUSER; i++) {
				if(Cid[i] > 0) {
					if(User[Cid[i]].bValid) {
						memcpy(&sui[totnum], &User[Cid[i]].SUI, sizeof(SMALLUSERINFO));
						totnum++;
					}
				}
			}

			// 모든 사용자 목록을 보내줌
			CSV_ALLUSERINFO msg(SndBuf);
			msg.Set(totnum, sui);
			SockMan.SendData(sid, msg.pData, msg.GetTotalSize());
		} break;

	case SV_ASK_ALLROOMINFO: // 모든 방정보 요청
		{
			// 특별한 메세지가 없으므로 메세지 분석은 필요없다

			ROOMINFO ri[MAX_ROOM];
			int totnum=0;
			for(int i=1; i<MAX_ROOM; i++) {
				if(Room[i].bValid) {
					memcpy(&ri[totnum], &Room[i].Ri, sizeof(ROOMINFO));
					totnum++;
				}
			}
			
			// 모든 방정보를 보내줌
			CSV_ALLROOMINFO msg(SndBuf);
			msg.Set(totnum, ri);
			SockMan.SendData(sid, msg.pData, msg.GetTotalSize());
		} break;

	case SV_ASK_ROOMINFO: // 방 정보를 요구(참여자 정보)
		{
			CSV_ASK_ROOMINFO MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			//if(rnum<1 || rnum>=MAX_ROOM) break;

			int ErrCode=0;
			if(rnum<1 || rnum>=MAX_ROOM) ErrCode=1; // 잘못된 방번호
			else if(strlen(Room[rnum].Ri.ID)==0) ErrCode=2; // 요청한 방이 존재하지 않음

			// 문제가 있는 방인 경우 삭제
			if(ErrCode==2) {
				Room[rnum].Clear();
			}
			
			GAMEINFO gi;
			ZeroMemory(&gi, sizeof(GAMEINFO));
			if(ErrCode==0) {
				memcpy(&gi.Ri, &Room[rnum].Ri, sizeof(ROOMINFO));
				for(int i=0; i<Room[rnum].get_maxnewplayer(); i++) memcpy(&gi.Ui[i], &Room[rnum].Ui[i], sizeof(USERINFO));
			}
			CSV_ROOMINFO rimsg(SndBuf);
			rimsg.Set(ErrCode, &gi);
			SockMan.SendData(sid, rimsg.pData, rimsg.GetTotalSize());
		} break;

	case SV_ASK_CREATEROOM: // 방 개설 요청
		{
			CSV_ASK_CREATEROOM MsgData;
			MsgData.Get(lpdata, totsize);

			// [게임 아이템 작업]
			int rn1, rn2;
			if(MsgData.RI->bSuperMaster) {
				// 슈퍼방 개설 조건이라면 방 리스트의 앞쪽에서 빈방을 검색
				rn1 = 1;
				rn2 = MAX_SUPERROOM + 1;
			}
			else {
				// 일반방 개설 조건이라면 방 리스트의 뒤쪽에서 빈방을 검색
				rn1 = MAX_SUPERROOM + 1;
				rn2 = MAX_ROOM;
			}
			
			int i = 0, rnum = -1;
			for(i = rn1; i < rn2; i++) {
				// 비어있는 방을 찾는다.
				if(!Room[i].bValid) { rnum=i; break;}
			}


			// [게임 아이템 작업]
			if(rnum < 0) {
				if(MsgData.RI->bSuperMaster) {
					// 방을 생성할 수 없으면 방 개설 실패 메세지를 보냄
					CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
					sndmsg.Set(999); // 아무런 메시지를 출력하지 않게함
					SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());

					// 사용자에게 서버메시지를 보냄
					CSV_SERVERMSG smsg(SndBuf);
					smsg.Set(1, "더 이상 슈퍼방을 만들 수 없습니다.");
					SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());

					break;
				}
				else {
					// 방을 생성할 수 없으면 방 개설 실패 메세지를 보냄
					CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
					sndmsg.Set(0); // 방 개설 수 초과
					SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());
					break;
				}
			}

			int unum = MsgData.RI->UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum < 1 || unum >= MAX_USER) break;

			// 방 개설을 신청한 아이디가 현재 사용자와 틀리거나 현재 위치가 대기실이 아니라면
			if(unum != User[unum].UI.UNum 
				|| faststrcmp(User[unum].UI.ID, MsgData.RI->ID) != 0
				|| User[unum].SUI.Position != WH_LOBY) 
			{
				CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
				sndmsg.Set(1); // 방을 생성할 수 없음
				SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());
				break;
			}

			if( User[unum].bAdminUser ) // ### [ 관전기능 ] ###
			{
				CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
				sndmsg.Set(1); // 방 개설 수 초과
				SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());
				break;
			}

			int roombetmoney = 0;
			if(MsgData.RI->bPlayer==0)
				roombetmoney = m_betmoney_5user;
			else 
				roombetmoney = m_betmoney_6user;

			/*			
			if(User[unum].UI.PMoney < roombetmoney) // 가진 마블이 적다면 방을 만들 수 없다 deleted by jeong
			{
				CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
				sndmsg.Set(2); // 방을 생성할 수 없음
				SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());
				break;
			}
			*/
			
			int gamekind = MsgData.RI->GameKind;
			int level = MsgData.RI->Level;

			// [세븐포커] 방만들수 있나 확인한다
			//int nError = 0;

			// [비공개방]일반서버라면 비공개방 아이템 문의
			if(MsgData.RI->Secret !=0  ) {
				// 비공개방을 개설하려고 하였다면 비공개방 아이템을 사용중인지 검사
				BOOL bExist = FALSE;
				if(!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_SECRET, &bExist) &&
					!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_INVINCIBLE, &bExist) && // [천하무적 아이템]
					CheckPremiumState(&User[unum].UI, NULL) == 0 &&	    // [프리미엄 작업]
					!User[unum].UI.bPremIP )							// [PC방 작업]
				{
					// 비공개방 아이템을 사용하고있지 않다면 구매 혹은 사용을 유도

					// 방을 생성할 수 없으면 방 개설 실패 메세지를 보냄
					CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
					sndmsg.Set(999); // 아무런 메시지를 출력하지 않게함
					SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());

					// 슈퍼방장 아이템의 구매 혹은 사용을 물어보는 메시지를 보냄
					CString strmsg;
					strmsg.Format("비공개방을 개설하기 위하여 [비공개방 아이템]이 필요합니다.\n[비공개방 아이템]을 구입 혹은 사용하시겠습니까?");

					BUYGAMEITEMINFO bi = {0,};
					bi.Kind = 5;	// 비공개방 아이템의 구입 혹은 사용 의사를 물어봄
					strcpy(bi.ItemName, "비공개방 아이템");
					bi.ItemCode = ITEM_SCODE_SECRET;
					bi.StrMsgLen = strmsg.GetLength();

					CSV_BUYGAMEITEM bgmsg(SndBuf);
					bgmsg.Set(&bi, strmsg);
					SockMan.SendData(sid, bgmsg.pData, bgmsg.GetTotalSize());

					break;
				}
			}
					
			// [게임 아이템 작업]
			if(MsgData.RI->bSuperMaster) {
				// 슈퍼방을 개설하려고 하였다면 슈퍼방장 아이템을 사용중이거나 프리미엄 회원인지 검사
				// 슈퍼방을 개설하려고 하였다면 천하무적 아이템을 사용중인지 검사 
				BOOL bExist = FALSE;
				if(!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_SUPERMASTER, &bExist) &&
					//### [ 올인극복 아이템 ] ###
					!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_ALLINSUPERMASTER, &bExist) &&
					!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_SAFEGUARDANGEL, &bExist) &&// [수호천사]
					!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_INVINCIBLE, &bExist) && // [천하무적 아이템]
					CheckPremiumState(&User[unum].UI, NULL) == 0 &&		// [프리미엄 작업]
					!User[unum].UI.bPremIP )							// [PC방 작업]
				{
					// 슈퍼방장 아이템을 사용하고있지 않았고 프리미엄 회원이 아니라면 구매 혹은 사용을 유도

					// 방을 생성할 수 없으면 방 개설 실패 메세지를 보냄
					CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
					sndmsg.Set(999); // 아무런 메시지를 출력하지 않게함
					SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());

					// 슈퍼방장 아이템의 구매 혹은 사용을 물어보는 메시지를 보냄
					CString strmsg;
					strmsg.Format("슈퍼방을 개설하기 위하여 [슈퍼방장 아이템]이 필요합니다.\n[슈퍼방장 아이템]을 구입 혹은 사용하시겠습니까?");

					BUYGAMEITEMINFO bi = {0,};
					bi.Kind = 3;	// 슈퍼방장 아이템의 구입 혹은 사용 의사를 물어봄
					strcpy(bi.ItemName, "슈퍼방장 아이템");
					bi.ItemCode = ITEM_SCODE_SUPERMASTER;
					bi.StrMsgLen = strmsg.GetLength();

					CSV_BUYGAMEITEM bgmsg(SndBuf);
					bgmsg.Set(&bi, strmsg);
					SockMan.SendData(sid, bgmsg.pData, bgmsg.GetTotalSize());

					break;
				}
			}



			// 게임 정보 구성
			Room[rnum].Clear();
			Room[rnum].bValid = TRUE;
			memcpy(&Room[rnum].Ri, MsgData.RI, sizeof(ROOMINFO));

			Room[rnum].set_player_usernum(m_channel_usernum); //방만들기 우선, 5, 7 인용 결정

			//if(Room[rnum].Ri.GameKind != 0) Room[rnum].Ri.GameKind = 1;	// 게임 방식

			// ################### 게임 스타일 #################
			if(Room[rnum].Ri.GameKind<0 || Room[rnum].Ri.GameKind>2) Room[rnum].Ri.GameKind=0; // 쿼터,하프,풀
			if(Room[rnum].Ri.Level<0 || Room[rnum].Ri.Level>2) Room[rnum].Ri.Level=0; // 1회,2회,3회레이즈
			if(Room[rnum].Ri.FormKind < 0 || Room[rnum].Ri.FormKind > 1) Room[rnum].Ri.FormKind = 0; // normal, choice
			// [세븐포커]
			Room[rnum].Ri.Level    = GetRaiseNum(Room[rnum].Ri.bPlayer); // 채널 구분
			Room[rnum].Ri.GameKind = GetBetStyle(Room[rnum].Ri.bPlayer); // 쿼터, 하프
			
			// [ 최소머니 설정 ]
			if(Room[rnum].Ri.UsernIcon < 0 || Room[rnum].Ri.UsernIcon > 15) Room[rnum].Ri.UsernIcon = 0;
			if(Room[rnum].Ri.CreateRGrade < 0 || Room[rnum].Ri.CreateRGrade > 6) Room[rnum].Ri.CreateRGrade = 0;

			strncpy(Room[rnum].m_DealerID, Room[rnum].Ri.ID, 15);    // 방장이 우승자로 셋팅한다.
			Room[rnum].m_nPrimeUser = 0; // 선
			Room[rnum].Ri.RoomNum = rnum; // 방 번호 부여
			Room[rnum].Ri.NowUserNum = 1;
			Room[rnum].Ri.NowObserverNum = 0; // ### [ 관전기능 ] ###
			
			Room[rnum].m_Raise.BetMoney = roombetmoney;// 삥값 셋팅
			Room[rnum].Ri.BetMoney =	  roombetmoney;// 삥값 셋팅

			//Room[rnum].Ri.MaxUserNum = 2;
			Room[rnum].Ri.MaxUserNum = min(Room[rnum].get_maxplayer(), Room[rnum].Ri.MaxUserNum);
			Room[rnum].Ri.nCard = User[unum].UI.PI.nCard; // 겜방카드

			// ### [ 관전기능 ] ###
			User[unum].bObserver = FALSE; 
			User[unum].UI.cObserver= 0;

			memcpy(&Room[rnum].Ui[0], &User[unum].UI, sizeof(USERINFO));

			// 방 개설을 허락함
			CSV_ACCEPT_CREATEROOM msg(SndBuf);
			msg.Set(&Room[rnum].Ri);
			SockMan.SendData(sid, msg.pData, msg.GetTotalSize());

			// 이벤트가 있는 경우 알림
			Room[rnum].SendRoomEventNotice(0);	// ### [이벤트 티켓] ###

			User[unum].PNum = 0;				// 플레이어 번호 설정
			User[unum].bExitSubscript = FALSE;	// 나가기 예약상태 초기화
			
			// ### [로그 기록용]###
			User[unum].nBet = 0;// 백업용 베팅데이타 초기화

			// 사용자의 위치가 변경됐음
			User[unum].SUI.Position = WH_GAME;	// 현재 위치는 게임방이다
			User[unum].SUI.RoomNum = rnum;		// 현재 위치한 방번호

			Room[rnum].Ps[0].nSndFxKind = *MsgData.Port;// ### [사운드 추가 작업] ###

			// 모든 사용자에게 방이 개설되었음을 알리고 사용자 정보가 갱신됨을 알림
			CSV_NEWROOM nrmsg(SndBuf);
			nrmsg.Set(&Room[rnum].Ri);
			SendMsgToLoby(nrmsg.pData, nrmsg.GetTotalSize());

			CSV_CHANGEUSER cumsg(SndBuf);
			cumsg.Set(&User[unum].SUI);
			SendMsgToLobyOpen(cumsg.pData, cumsg.GetTotalSize());
		} break;

	case SV_ASK_ENTERROOM: // 방에 입장하기를 요구
		{
			CSV_ASK_ENTERROOM MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum >= MAX_USER) break;
			if(!User[unum].bValid) break;

			int rnum = *MsgData.RoomNum;

			int ErrCode=0;
			if(rnum<1 || rnum>=MAX_ROOM) // 잘못된 방정보
				ErrCode=1;
			else if(Room[rnum].bValid==FALSE) // 요청한 방이 존재하지 않음
				ErrCode=2;
			// ### [ 관전기능 ] ###
			//else if(Room[rnum].Ri.NowUserNum >= Room[rnum].Ri.MaxUserNum) // 최대인원 초과
			//	ErrCode=4;
			else if(Room[rnum].GetPNum(MsgData.ID)!=-1) // 이미 존재하고 있는 아이디
				ErrCode=5;
			// ### [ 관전기능 ] ###
			else if(!User[unum].bAdminUser && Room[rnum].Ri.Secret == TRUE && strncmp(Room[rnum].Ri.Pass, MsgData.Pass, 4) != 0) // 비번 틀림
				ErrCode=6;
			else if(unum != User[unum].UI.UNum || faststrcmp(MsgData.ID, User[unum].UI.ID) != 0) // 요청인 아이디가 잘못됨
				ErrCode=7;
			// [ 최소머니 설정 ]
			else if(Room[rnum].Ri.CreateRGrade > 0 && !User[unum].bAdminUser) { // 관리자 제외
				INT64 Money = GetLevelToUserMoney( Room[rnum].Ri.UsernIcon );
				Money = Money * GetCreateRoomGrade(Room[rnum].Ri.CreateRGrade) / 10; // 방만들기시 최소머니 설정등급
				if(Money > User[unum].UI.PMoney) {
					ErrCode =16;
					
					CSV_REFUSE_ENTERROOM rermsg(SndBuf);
					rermsg.Set(ErrCode);
					SockMan.SendData(sid, rermsg.pData, rermsg.GetTotalSize());

					CSV_SERVERMSG smsg(SndBuf);
					CString str, strM;
					strM = g_MakeCommaMoney(Money);
					str.Format("최소머니제한이 설정된 방입니다. %s원 이상만 입장할 수 있습니다.", strM);
					smsg.Set(1, (char*)(LPCTSTR)str);
					SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
					break;
				}
			}
			
			if(ErrCode == 0) {
				// 현재 위치가 대기실이 아니라면
				if(User[unum].SUI.Position != WH_LOBY) ErrCode = 7;
			}

			int i=0;
			int pnum = -1;
			// ### [ 관전기능 ] ### 
			//무조건 관전으로 들어간다
			//겜참여? 융통성 부여
			//### [관리자 모드 작업] ###
			if(ErrCode == 0) {
				if( User[unum].bAdminUser )
					pnum = Room[rnum].get_maxnewplayer()-1; //관리자 자리는 비워둔다.
				else
				{
					if(Room[rnum].Ri.NowUserNum < Room[rnum].Ri.MaxUserNum )
					{	//참여
						for(i=0; i < Room[rnum].Ri.MaxUserNum; i++) { 
							if(strlen(Room[rnum].Ui[i].ID) == 0){ pnum=i; break;}
						}
						
					} else {
						//관전
						for(i=Room[rnum].get_maxplayer(); i < Room[rnum].get_maxnewplayer()-1; i++) {
							if(strlen(Room[rnum].Ui[i].ID) == 0){ pnum=i; break;}
						}
					}
					if(pnum == -1) ErrCode=4;
				}
			}

			if(ErrCode == 0) {
				// 가진 돈이 모자라면 입장할 수 없다
				if(User[unum].UI.PMoney < Room[rnum].Ri.BetMoney) ErrCode=9;
			}

			if(Room[rnum].bSuperRoomDestory == TRUE){
				ErrCode = 2;
			}

			// [게임 아이템 작업]
			if(ErrCode == 0) {
				if(Room[rnum].Ri.bSuperMaster) {
					// 슈퍼방인 경우 강퇴자 리스트에 아이디가 존재한다면 입장 실패
					if(Room[rnum].BanUserList.FindID(User[unum].UI.ID)) ErrCode = 10;
				}
			}

			if(ErrCode == 0) {				
				// 일반 강퇴자 리스트에 아이디가 존재한다면 입장 실패
				if(Room[rnum].VoteBanUserList.FindID(User[unum].UI.ID)) ErrCode = 10;				
			}	

			///////////////////////////////////////////////////////////////////////////////////////////////
			// ### [ 중복된 IP가 ] ###
			int myIpIndex = -1;
			if(ErrCode == 0) {
				if(g_pMainView->Cfg2.SameIPLimit > 0 &&  pnum<Room[rnum].get_maxplayer() ) {// ### [ 관전기능 ] ###
					int samecnt = 0;
					int tmpIp[MAX_PLAYER]={0,};
					
					// 같은 IP 대역이 몇개 존재하는지 검사
					for(int i=0; i<Room[rnum].get_maxplayer(); i++) {
						int tarunum = Room[rnum].Ui[i].UNum;
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
						ErrCode = 15;
					}
					else if(samecnt !=0) // 추가 클라이언트에서 이미지로 보여준다.(같은 아피)
					{
						if(samecnt == 1) // 처음
						{
							for(int kk=0; kk < Room[rnum].get_maxplayer();kk++)
							{
								if(Room[rnum].m_aIpIndex[kk] == 0) break;
							}
							Room[rnum].m_aIpIndex[kk] = 1;
							
							for(i=0;i<Room[rnum].get_maxplayer();i++)
							{
								if(tmpIp[i] != 0) Room[rnum].Ps[i].nCheckIp = kk;
							}
							myIpIndex = kk;
							Room[rnum].m_nIpCnt++;
						}
						else
						{// 기존
							myIpIndex = Room[rnum].Ps[myIpIndex].nCheckIp;
							//Room[rnum].m_aIpIndex[myIpIndex]++;// 인원수 증가
						}
					}
				}
			}
			///////////////////////////////////////////////////////////////////////////////////////////////				

			if(ErrCode == 0) {
				// 방에 입장처리 // ### [ 중복된 IP가 ] ### // ### [사운드 추가 작업] ###
				if(Room[rnum].OnUserEnterRoom(&User[unum].UI, pnum, myIpIndex,*MsgData.SndFxKind) == FALSE) ErrCode = 1;
			}

			// 입장하는데 문제가 있다면 입장을 거부함
			if(ErrCode != 0) {
				CSV_REFUSE_ENTERROOM rermsg(SndBuf);
				rermsg.Set(ErrCode);
				SockMan.SendData(sid, rermsg.pData, rermsg.GetTotalSize());
				break;
			}

			////// 입장 처리 

			User[unum].bExitSubscript = FALSE;// 나가기 예약상태 초기화

			// 사용자의 위치가 변경됐음
			User[unum].SUI.Position = WH_GAME;	// 현재 위치는 게임방이다
			User[unum].SUI.RoomNum = rnum;		// 현재 위치한 방번호
			// 요청한 사용자에게 방에 입장 허가를 통보
			CSV_ACCEPT_ENTERROOM aermsg(SndBuf);
			ENTERROOMINFO ei = Room[rnum].GetEnterRoomInfo();

			/*
			//[62] 바꾸기 시에는 게임 정보를 안보냄 그래서 새로 들어오는넘을 강제로 셋팅
			if(Room[rnum].GameState == RSTATE_CHANGECARD){
				ei.Pg.nState = 3;
			}
			*/
			aermsg.Set(&ei);
			SockMan.SendData(sid, aermsg.pData, aermsg.GetTotalSize());

			////////////////// ### [ 관전기능 ] ### ////////////////////
			// 예약 관련
			Room[rnum].GetReservaitonInfo();
			
			Room[rnum].SendRoomEventNotice(pnum); // ### [이벤트 티켓] ###

			// 모든 사용자에게 방정보가 변경되었음을 알리고 사용자 정보가 갱신됨을 알림
			CSV_CHANGEROOM crmsg(SndBuf);
		//	SMALLROOMINFO sri = Room[rnum].GetSmallRoomInfo();
		//	crmsg.Set(&sri);
			crmsg.Set(&Room[rnum].GetSmallRoomInfo());
			SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());

			CSV_CHANGEUSER cumsg(SndBuf);
			cumsg.Set(&User[unum].SUI);
			SendMsgToLobyOpen(cumsg.pData, cumsg.GetTotalSize());
		} break;

	// [ ###바로시작### ]
	case SV_ASK_QUICKSTART: // 바로 게임 시작
		{
			CSV_ASK_QUICKSTART MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum >= MAX_USER) break;
			if(!User[unum].bValid) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break; // 잘못된 유저번호이다

			//if(User[unum].SUI.Position != WH_LOBY) break;

			int gamekind = *MsgData.GameKind; // 0: 모두 1: 5인 2: 6인

			int nRoomNum1 = -1;
			int nRoomNum2 = -1;
			for(int i=1; i<MAX_ROOM; i++) 
			{
				if(Room[i].bValid) 
				{
					// 5인, 7인
				//	if(gamekind != 0) {
				//		if(gamekind-1 != Room[i].Ri.bPlayer) continue;
				//	}

					if(Room[i].bSuperRoomDestory)continue;						

					if(gamekind != 0) {
						if(gamekind-1 != Room[i].Ri.FormKind) continue;
					}

					// [게임 아이템 작업]
					if(Room[i].Ri.bSuperMaster) {
					// 슈퍼방인 경우 강퇴자 리스트에 아이디가 존재한다면 입장 실패
						if(Room[i].BanUserList.FindID(User[unum].UI.ID)) continue;
					}

					
					// 일반 강퇴자 리스트에 아이디가 존재한다면 입장 실패
					if(Room[i].VoteBanUserList.FindID(User[unum].UI.ID)) continue;
								

					// 최소 머니 설정
					if(Room[i].Ri.CreateRGrade > 0) {
						INT64 Money = GetLevelToUserMoney( Room[i].Ri.UsernIcon );
						Money = Money * GetCreateRoomGrade(Room[i].Ri.CreateRGrade) / 10; // 방만들기시 최소머니 설정등급
						if(Money > User[unum].UI.PMoney) continue;
					}

					///////////////////////////////////////////////////////////////////////////////////////////////
					// ### [ 중복된 IP가 ] ###IP 주소
					if(g_pMainView->Cfg2.SameIPLimit > 0) {
						int samecnt = 0;
						// 같은 IP 대역이 몇개 존재하는지 검사
						for(int k=0; k<Room[i].get_maxplayer(); k++) {
							int tarunum = Room[i].Ui[k].UNum;
							if(tarunum<1 || tarunum >= MAX_USER) continue;
							if(User[unum].PeerIP.S_un.S_un_b.s_b1 == User[tarunum].PeerIP.S_un.S_un_b.s_b1 && 
								User[unum].PeerIP.S_un.S_un_b.s_b2 == User[tarunum].PeerIP.S_un.S_un_b.s_b2 && 
								User[unum].PeerIP.S_un.S_un_b.s_b3 == User[tarunum].PeerIP.S_un.S_un_b.s_b3 )//&&
								//User[unum].PeerIP.S_un.S_un_b.s_b4 == User[tarunum].PeerIP.S_un.S_un_b.s_b4) //테스트 
							{
								samecnt++;
							}
						}
						
						if(samecnt >= g_pMainView->Cfg2.SameIPLimit) {
							// 중복된 IP가 존재하여 입장 불가
							//ErrCode = 15;
							continue;
						}
					}
					///////////////////////////////////////////////////////////////////////////////////////////////
					
					if(Room[i].Ri.State == 0 && Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum 
						&& Room[i].Ri.Secret == 0 && (gamekind-1) == Room[i].Ri.FormKind && gamekind > 0)	// 상태(0:대기중 1:게임중)
					{// 대기중 빈방
						nRoomNum1 = i;
						break;
					}
					else if( Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum 
						&& Room[i].Ri.Secret == 0 && (gamekind-1) == Room[i].Ri.FormKind && gamekind > 0)	// 상태(0:대기중 1:게임중)
					{// 게임중 빈방
						nRoomNum2 = i;
					}
					else if(Room[i].Ri.State == 0 && Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum && Room[i].Ri.Secret == 0)	// 상태(0:대기중 1:게임중)
					{// 대기중 빈방
						nRoomNum1 = i;
						break;
					}
					else if( Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum && Room[i].Ri.Secret == 0)	// 상태(0:대기중 1:게임중)
					{// 게임중 빈방
						nRoomNum2 = i;
					}

				/*
					if(Room[i].Ri.State == 0 && Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum && Room[i].Ri.Secret == 0)	// 상태(0:대기중 1:게임중)
					{// 대기중 빈방
						nRoomNum1 = i;
						break;
					}
					else if( Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum && Room[i].Ri.Secret == 0)	// 상태(0:대기중 1:게임중)
					{// 게임중 빈방
						nRoomNum2 = i;
					}
				*/
				}
			}


			if(nRoomNum1 != -1 || nRoomNum2 != -1)
			{// 방 입장
				int rnum = 0;
				if(nRoomNum1 != -1)
					rnum = nRoomNum1;
				else
					rnum = nRoomNum2;

				int ErrCode=0;
				if(rnum<1 || rnum>=MAX_ROOM) // 잘못된 방정보
					ErrCode=1;
				else if(Room[rnum].bValid==FALSE) // 요청한 방이 존재하지 않음
					ErrCode=2;
				else if(Room[rnum].Ri.NowUserNum >= Room[rnum].Ri.MaxUserNum) // 최대인원 초과
					ErrCode=4;
				else if(Room[rnum].GetPNum(MsgData.ID)!=-1) // 이미 존재하고 있는 아이디
					ErrCode=5;
			//	else if(Room[rnum].Ri.Secret == TRUE && strncmp(Room[rnum].Ri.Pass, MsgData.Pass, 4) != 0) // 비번 틀림
			//		ErrCode=6;
				else if(unum != User[unum].UI.UNum || faststrcmp(MsgData.ID, User[unum].UI.ID) != 0) // 요청인 아이디가 잘못됨
					ErrCode=7;				
				
				
				if(ErrCode == 0) {
					// 현재 위치가 대기실이 아니라면
					if(User[unum].SUI.Position != WH_LOBY) ErrCode = 7;
				}
				/*
				if(ErrCode == 0) {
					// 머니 가진 돈이 모자라면 입장할 수 없다				deleted by jeong
					if(User[unum].UI.PMoney < Room[rnum].Ri.BetMoney) ErrCode=9;
				}
				*/
				
				int i=0;
				int pnum = -1;
				// ### [ 관전기능 ] ###
				if(ErrCode == 0) {
					if( User[unum].bAdminUser )
						pnum = Room[rnum].get_maxnewplayer()-1;
					else {
						for(i=0; i<Room[rnum].Ri.MaxUserNum; i++)
							if(strlen(Room[rnum].Ui[i].ID) == 0){ pnum=i; break;}
							if(pnum == -1) ErrCode=4;
					}
				}
				
				
				///////////////////////////////////////////////////////////////////////////////////////////////
				// ### [ 중복된 IP가 ] ###
				int myIpIndex = -1;
				if(ErrCode == 0) {
					if(g_pMainView->Cfg2.SameIPLimit > 0 && pnum<Room[rnum].get_maxplayer() ) {// ### [ 관전기능 ] ###
						int samecnt = 0;
						int tmpIp[MAX_PLAYER]={0,};
						
						// 같은 IP 대역이 몇개 존재하는지 검사
						for(int i=0; i<Room[rnum].get_maxplayer(); i++) {
							int tarunum = Room[rnum].Ui[i].UNum;
							if(tarunum<1 || tarunum >= MAX_USER) continue;
							if(User[unum].PeerIP.S_un.S_un_b.s_b1 == User[tarunum].PeerIP.S_un.S_un_b.s_b1 && 
								User[unum].PeerIP.S_un.S_un_b.s_b2 == User[tarunum].PeerIP.S_un.S_un_b.s_b2 && 
								User[unum].PeerIP.S_un.S_un_b.s_b3 == User[tarunum].PeerIP.S_un.S_un_b.s_b3 )//&&
									//User[unum].PeerIP.S_un.S_un_b.s_b4 == User[tarunum].PeerIP.S_un.S_un_b.s_b4) //테스트
							{
								samecnt++;
								tmpIp[i]=1;
								myIpIndex = i;
							}
						}
						
						if(samecnt >= g_pMainView->Cfg2.SameIPLimit) {
							// 중복된 IP가 존재하여 입장 불가
							ErrCode = 15;
						}
						else if(samecnt !=0) // 추가 클라이언트에서 이미지로 보여준다.(같은 아피)
						{
							if(samecnt == 1) // 처음
							{
								for(int kk=0; kk < Room[rnum].get_maxplayer();kk++)
								{
									if(Room[rnum].m_aIpIndex[kk] == 0) break;
								}
								Room[rnum].m_aIpIndex[kk] = 1;

								Room[rnum].m_nIpCnt++; // 현재 사용치 않는다.
								
								for(i=0;i<Room[rnum].get_maxplayer();i++)
								{
									if(tmpIp[i] != 0) Room[rnum].Ps[i].nCheckIp = kk;
								}
								myIpIndex = kk;
							
							}
							else
							{// 기존
								myIpIndex = Room[rnum].Ps[myIpIndex].nCheckIp;
								//	Room[rnum].m_aIpIndex[myIpIndex]++;// 인원수 증가
							}
						}
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				
				
				if(ErrCode == 0) {
					// 방에 입장처리 // ### [ 중복된 IP가 ] ### // ### [사운드 추가 작업] ###
					if(Room[rnum].OnUserEnterRoom(&User[unum].UI, pnum, myIpIndex, *MsgData.SndFxKind) == FALSE) ErrCode = 1; // ### [ 중복된 IP가 ] ###
				}

				
				// 입장하는데 문제가 있다면 입장을 거부함
				if(ErrCode != 0) {
					CSV_REFUSE_ENTERROOM rermsg(SndBuf);
					rermsg.Set(ErrCode);
					SockMan.SendData(sid, rermsg.pData, rermsg.GetTotalSize());
					break;
				}
				

			

				////// 입장 처리 

				// 나가기 예약상태 초기화
				User[unum].bExitSubscript = FALSE;
				// 사용자의 위치가 변경됐음
				User[unum].SUI.Position = WH_GAME;	// 현재 위치는 게임방이다
				User[unum].SUI.RoomNum = rnum;	// 현재 위치한 방번호
				// 플레이어 번호 설정
				//User[unum].PNum = pnum;

				
				// 요청한 사용자에게 방에 입장 허가를 통보
				CSV_ACCEPT_ENTERROOM aermsg(SndBuf);
				ENTERROOMINFO ei = Room[rnum].GetEnterRoomInfo();
				aermsg.Set(&ei);
				SockMan.SendData(sid, aermsg.pData, aermsg.GetTotalSize());

				// 이벤트가 있는 경우 알림
				Room[rnum].SendRoomEventNotice(pnum); // ### [이벤트 티켓] ###

				////////////////// ### [ 관전기능 ] ### ////////////////////
				// 예약 관련
				Room[rnum].GetReservaitonInfo();

				// 모든 사용자에게 방정보가 변경되었음을 알리고 사용자 정보가 갱신됨을 알림
				CSV_CHANGEROOM crmsg(SndBuf);
			//	SMALLROOMINFO sri = Room[rnum].GetSmallRoomInfo();
			//	crmsg.Set(&sri);
				crmsg.Set(&Room[rnum].GetSmallRoomInfo());
				SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());

				CSV_CHANGEUSER cumsg(SndBuf);
				cumsg.Set(&User[unum].SUI);
				SendMsgToLobyOpen(cumsg.pData, cumsg.GetTotalSize());
			}
			else
			{// 없다 ( 방 생성)
				CSV_REFUSE_QUICKSTART rqstart(SndBuf);
				rqstart.Set(0);
				SockMan.SendData(sid, rqstart.pData, rqstart.GetTotalSize());
			}


			// Bank 정보 전송 - jeong
			CSV_BANKINFO uimsg(SndBuf);
			uimsg.Set(User[unum].nBMoney);
			SockMan.SendData(sid, uimsg.pData, uimsg.GetTotalSize());

		} break;

	case SV_ASK_OUTROOM: // 사용자가 방에서 나오기를 요청
		{
			CSV_ASK_OUTROOM MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(rnum < 1 || rnum >= MAX_ROOM) break;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid==FALSE) break; // 잘못된 유저번호이다
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break; // 잘못된 유저번호이다
			if(Room[rnum].bValid == FALSE) break; // 이미 삭제된 방이다
			if(User[unum].SUI.Position != WH_GAME) break;	//현재 위치가 게임방이 아니다

			// 나가기 예약상태 초기화
			User[unum].bExitSubscript = FALSE;

			Room[rnum].OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
			
			// ### [이벤트 티켓] ###
			g_pMainView->SendChannelEventNotice(unum);
		} break;

	case SV_LOBYOPEN: // 로비창 오픈 상태 변경
		{
			CSV_LOBYOPEN MsgData;
			MsgData.Get(lpdata, totsize);

			// 에러검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;

			User[unum].bLobyOpen = *MsgData.bLobyOpen;

			if(User[unum].bLobyOpen == TRUE) {
				SMALLUSERINFO sui[MAX_CHUSER];
				int totnum=0;
				for(int i=0; i<MAX_CHUSER; i++) {
					if(Cid[i] > 0) {
						if(User[Cid[i]].bValid) {
							memcpy(&sui[totnum], &User[Cid[i]].SUI, sizeof(SMALLUSERINFO));
							totnum++;
						}
					}
				}

				// 모든 사용자 목록을 보내줌
				CSV_ALLUSERINFO msg(SndBuf);
				msg.Set(totnum, sui);
				SockMan.SendData(sid, msg.pData, msg.GetTotalSize());
			}
		} break;

	case SV_ASK_INVITE: // 사용자가 초청메세지를 보냄
		{
			CSV_ASK_INVITE MsgData;
			MsgData.Get(lpdata, totsize);
			
			// 에러검사
			int rnum = *MsgData.RoomNum;
			int tarunum = *MsgData.TarUNum;
			if(rnum < 0 || rnum >= MAX_ROOM) break;
			if(tarunum < 0 || tarunum >= MAX_USER) break;

			if(faststrcmp(User[tarunum].UI.ID, MsgData.TarID) != 0) break; // 잘못된 아이디이다
			if(strlen(MsgData.TarID) == 0) break;				// 잘못된 아이디이다
			if(strlen(Room[rnum].Ri.ID) == 0) break;			// 이미 삭제된 방이다
			if(User[tarunum].SUI.Position != WH_LOBY) break;	// 대기실에 있지 않다
			SockMan.SendData(User[tarunum].Sid, MsgData.pData, MsgData.GetTotalSize());
		} break;

	case SV_REFUSE_INVITE: // 사용자가 초청메세지를 거부함
		{
			CSV_REFUSE_INVITE MsgData;
			MsgData.Get(lpdata, totsize);

			// 에러검사
			int tarunum = *MsgData.TarUNum;
			if(tarunum < 0 || tarunum >= MAX_USER) break;

			if(faststrcmp(User[tarunum].UI.ID, MsgData.TarID) != 0) break; // 잘못된 아이디이다
			if(strlen(MsgData.TarID) == 0) break; // 잘못된 아이디이다
			SockMan.SendData(User[tarunum].Sid, MsgData.pData, MsgData.GetTotalSize());
		} break;

	case SV_ASK_BANISHVOTE: // 추방 투표 요청
		{
			CSV_ASK_BANISHVOTE MsgData;
			MsgData.Get(lpdata, totsize);
			
			// 에러검사
			int rnum = MsgData.pBV->RoomNum;
			if(rnum < 1 || rnum >= MAX_ROOM) break;
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
	
			if(!Room[rnum].bValid || strlen(Room[rnum].Ri.ID) == 0) break;	// 이미 삭제된 방이다
			if(faststrcmp(User[unum].UI.ID, MsgData.pBV->MyID) != 0) break;	// 잘못된 아이디이다
			if(User[unum].SUI.Position != WH_GAME) break;	// 게임룸에 있지 않다

			Room[rnum].OnAskBanishVote(MsgData.pBV);
			
		} break;

	case SV_MYBANISHVOTE: // 나의 추방 투표 결과
		{
			CSV_MYBANISHVOTE MsgData;
			MsgData.Get(lpdata, totsize);
			
			// 에러검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(User[unum].SUI.Position != WH_GAME) break; // 게임룸에 있지 않다
			int rnum = User[unum].SUI.RoomNum;
			if(rnum < 1 || rnum >= MAX_ROOM) break;
			
			if(!Room[rnum].bValid || strlen(Room[rnum].Ri.ID) == 0) break;	// 이미 삭제된 방이다
			Room[rnum].OnMyBanishVote(User[unum].UI.ID, *MsgData.bAgree);
		} break;

	case SV_CHATDATA: // 대기실 채팅 데이타
		{
			CSV_CHATDATA MsgData;
			MsgData.Get(lpdata, totsize);

			//(추가)
			// 아이디가 올바른지 검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) {// ### [관리자 모드 작업] ###
				if(!User[unum].bAdminUser || faststrcmp("관리자", MsgData.ID)!=0) {
					// 아이디를 변조한 경우는 접속을 종료시킴
					SockMan.GetDataSocketPtr(sid)->PostClose();
					break;
				}
			}
			
			// [대화 저장] 특정 아이디를 찾아서 대화 저장
			if(g_CaptureID.bUse &&  g_pMainView->IsCaptureUserID(MsgData.ID) )
			{
				if(strlen(MsgData.ID) == 0 || strlen(MsgData.ID) >= 16) break;
				int l_StrChat = *MsgData.l_StrChat;
				if(l_StrChat<=0 || l_StrChat>20000) break;

				char *pChat= new char[l_StrChat+2];
				memset(pChat, 0, l_StrChat+2);
				strncpy(pChat, MsgData.StrChat, l_StrChat);
				
				// 현재 날짜 및 시간
				CString Date;
				CTime t = CTime::GetCurrentTime();
				Date.Format("%4d/%2d/%2d(%2d:%2d:%2d)", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

				CBadUserDBWorks Work;
				
				Work.bValid = TRUE;
				Work.ServerCode = g_pMainView->Cfg.ServerCode;
				Work.GameCode = g_pMainView->Cfg.GameCode;
				Work.MyID = MsgData.ID;
				Work.TargetID="netmarble";
				Work.SubmitDate = Date;
					// 현재 위치
				Work.SubmitReason = "대기실";
				//Work.WatchID = pWatchID;
				Work.Contents = pChat;

				// BadUserDB Work 큐에 처리할 일 삽입
				BadUserDBQue.EnQue(Work);
				if(pChat) delete pChat;
			}
			// 대기실에 있는 사용자에게만 메시지를 보냄
			SendMsgToLoby(MsgData.pData, MsgData.GetTotalSize());
		} break;

	case SV_NOWHISPER: // 귓말 허용 상태 변경
		{
			CSV_NOWHISPER MsgData;
			MsgData.Get(lpdata, totsize);

			// 에러검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;

			User[unum].bNoWhisper = *MsgData.bNoWhisper;
		} break;

	case SV_WHISPERCHAT: // 귓속말
		{
			CSV_WHISPERCHAT MsgData;
			MsgData.Get(lpdata, totsize);

			//(추가)
			// 아이디가 올바른지 검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) {
				// 아이디를 변조한 경우는 접속을 종료시킴
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}


			int destunum = *MsgData.DestUNum;
			char *pdestid = MsgData.DestID;
			if(destunum < 0 || destunum >= MAX_USER) break;
			if(strlen(pdestid)==0 || strlen(pdestid)>=16) break;
			if(User[destunum].bValid==FALSE) break;
			if(faststrcmp(pdestid, User[destunum].UI.ID) != 0) break;

			if(User[destunum].bNoWhisper) {
				CSV_SERVERMSG smsg(SndBuf);
				CString str;
				str.Format("[%s]님은 귓말 거부중입니다.", pdestid);
				smsg.Set(0, (char*)(LPCTSTR)str);
				SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
				break;
			}

			// [대화 저장] 특정 아이디를 찾아서 대화 저장
			if( g_CaptureID.bUse &&  g_pMainView->IsCaptureUserID(MsgData.ID))
			{
				if(strlen(MsgData.ID) == 0 || strlen(MsgData.ID) >= 16) break;
				int l_StrChat = *MsgData.l_StrChat;
				if(l_StrChat<=0 || l_StrChat>20000) break;

				char *pChat= new char[l_StrChat+2];
				memset(pChat, 0, l_StrChat+2);
				strncpy(pChat, MsgData.StrChat, l_StrChat);
				// 현재 날짜 및 시간
				CString Date;
				CTime t = CTime::GetCurrentTime();
				Date.Format("%4d/%2d/%2d(%2d:%2d:%2d)", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

				CBadUserDBWorks Work;
				
				Work.bValid = TRUE;
				Work.ServerCode = g_pMainView->Cfg.ServerCode;
				Work.GameCode = g_pMainView->Cfg.GameCode;
				Work.MyID = MsgData.ID;
				Work.TargetID="netmarble";
				Work.SubmitDate = Date;
				// 현재 위치
				Work.SubmitReason = "귓속말";
				Work.WatchID = pdestid;
				Work.Contents = pChat;
				// BadUserDB Work 큐에 처리할 일 삽입
				BadUserDBQue.EnQue(Work);
				if(pChat) delete pChat;
			}
			SockMan.SendData(User[destunum].Sid, MsgData.pData, MsgData.GetTotalSize());
		} break;


	case SV_ASK_FORCEBANISH: // 강제 추방 요청	[게임 아이템 작업]
		{
		CSV_ASK_FORCEBANISH MsgData;
			MsgData.Get(lpdata, totsize);

			// 에러검사
			int unum = pUserVal->iUser2;
			if(unum<1 || unum>=MAX_USER) break;
			if(!User[unum].bValid) break;

			if(User[unum].SUI.Position != WH_GAME) break;
			int rnum = User[unum].SUI.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int pnum = User[unum].PNum;
			if(pnum == -1) break;

			// 방장이 아니면 취소     <--- 추가!!!
			if(strncmp(Room[rnum].Ri.ID, Room[rnum].Ui[pnum].ID, 15) != 0)
				break;

			Room[rnum].OnForceBanish(&MsgData);
		} break;

	case CL_CHATDATA: // 게임방 채팅 데이타
		{
			CCL_CHATDATA MsgData;
			MsgData.Get(lpdata, totsize);

			//(추가)
			// 아이디가 올바른지 검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) {// ### [관리자 모드 작업] ###
				if(!User[unum].bAdminUser || faststrcmp("관리자", MsgData.ID)!=0) {
					// 아이디를 변조한 경우는 접속을 종료시킴
					SockMan.GetDataSocketPtr(sid)->PostClose();
					break;
				}
			}

			if(User[unum].SUI.Position != WH_GAME) break;

			// 에러검사
			int rnum = *MsgData.RoomNum;
			if(rnum<0 || rnum>=MAX_ROOM) break;

			// [대화 저장] 특정 아이디를 찾아서 대화 저장
			if( g_CaptureID.bUse &&  g_pMainView->IsCaptureUserID(MsgData.ID))
			{
				if(strlen(MsgData.ID) == 0 || strlen(MsgData.ID) >= 16) break;
				int l_StrChat = *MsgData.l_StrChat;
				if(l_StrChat<=0 || l_StrChat>20000) break;

				char *pChat= new char[l_StrChat+2];
				memset(pChat, 0, l_StrChat+2);
				strncpy(pChat, MsgData.StrChat, l_StrChat);
				// 현재 날짜 및 시간
				CString Date;
				CTime t = CTime::GetCurrentTime();
				Date.Format("%4d/%2d/%2d(%2d:%2d:%2d)", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

				CBadUserDBWorks Work;
				
				Work.bValid = TRUE;
				Work.ServerCode = g_pMainView->Cfg.ServerCode;
				Work.GameCode = g_pMainView->Cfg.GameCode;
				Work.MyID = MsgData.ID;
				Work.TargetID="netmarble";
				Work.SubmitDate = Date;
				// 현재 위치
				Work.SubmitReason = "방";
				//Work.WatchID = pWatchID;
				Work.Contents = pChat;

				// BadUserDB Work 큐에 처리할 일 삽입
				BadUserDBQue.EnQue(Work);
				if(pChat) delete pChat;
			}
			Room[rnum].SendMsgToAll(MsgData.pData, MsgData.GetTotalSize());

		} break;

	case CL_EXITSUBSCRIPT: // 나가기 예약, 취소
		{
			CCL_EXITSUBSCRIPT MsgData;
			MsgData.Get(lpdata, totsize);

			// 에러검사
			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break; // 이미 삭제된 방이다

			int unum = *MsgData.UNum;
			if(unum<1 || unum>=MAX_USER) break;
			if(unum != pUserVal->iUser2) break;
			if(User[unum].bValid==FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;
			if(User[unum].SUI.Position != WH_GAME) break;	//현재 위치가 게임방이 아니다
			int pnum = Room[rnum].GetPNum(MsgData.ID);
			if(pnum == -1) break;
			Room[rnum].ExitSubScript(&MsgData, pnum, unum);
		} break;
		
	////////////////////// 게임 진행 관련 메세지 /////////////////////

	case CL_ASK_STARTGAME://게임 시작 요청
		{			
			CCL_ASK_STARTGAME MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;
			int pnum = Room[rnum].GetPNum(MsgData.ID);			
			if(pnum == -1) break;
			if(strcmp(Room[rnum].Ui[pnum].ID,MsgData.ID)!=0)break;			
			if(Room[rnum].GameState != RSTATE_NONE)break;

			Room[rnum].bRestartCnt=FALSE;
			Room[rnum].RestartCnt=0;
			Room[rnum].StartGame();			
		}break;

	case CL_ASK_CHANGEGAMEKIND: // 게임 방식 변경 요청
		{
			CCL_ASK_CHANGEGAMEKIND MsgData;
			MsgData.Get(lpdata, totsize);

			// 에러검사
			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			Room[rnum].OnAskChangeGameKind(&MsgData);
		} break;
 
	///////////////////////////////////////////////////////////////////////
	//*    7Card 
	case CL_ASK_FOLD: // 게임 폴드
		{
			CCL_ASK_FOLD MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid==FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;
			
			if(User[unum].SUI.Position != WH_GAME) break;	//현재 위치가 게임방이 아니다
			
			int pnum = *MsgData.PNum;
			if(pnum<0 || pnum>=Room[rnum].get_maxplayer()) break;
			if(pnum != User[unum].PNum) break;

			// ### [ 관전기능 ] ###
			if( Room[rnum].FoldUser(pnum) )
			{
				// ### [ 관전기능 ] ###
				Room[rnum].Observer_takeaSeat(User[unum].UI.ID);
			}

		} break;

	case CL_ASK_SELECTCARD: // 사용자의 카드선택
		{
			CCL_ASK_SELECTCARD MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid==FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;

			if(User[unum].SUI.Position != WH_GAME) break;	//현재 위치가 게임방이 아니다
			
			int pnum = *MsgData.PNum;
			if(pnum<0 || pnum>=Room[rnum].get_maxplayer()) break;
			if(pnum != User[unum].PNum) break;

			int discard = *MsgData.DisCard;
			if(discard < 1 || discard > 4) break;
			Room[rnum].OnReadyToStart(pnum, MsgData.pSD, discard);
		} break;

		// 방정보 변경 
	case CL_ASK_CHSTYLEROOMINFO:
		{
			CCL_ASK_CHSTYLEROOMINFO MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = MsgData.CHRI->RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int unum = MsgData.CHRI->UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid==FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.CHRI->ID) != 0) break;

			Room[rnum].ChangeRoomOption(&MsgData);
		}break;

	/*
	case CL_CARDCHANGE_62CUT_RESULT://[62]
		{
			CCL_CARDCHANGE_62CUT_RESULT MsgData;
			MsgData.Get(lpdata, totsize);			

			CHANGECARD_62CUT data;
			ZeroMemory(&data,sizeof(CHANGECARD_62CUT));		

			memcpy(&data,MsgData.pCC,sizeof(CHANGECARD_62CUT));

			int rnum = data.nRoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int unum = data.nUNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid==FALSE) break;
			
			if(faststrcmp(User[unum].UI.ID, MsgData.pCC->ID) != 0) break;

			Room[rnum].ChangeCard_62cut(data);

		}break;

	case CL_CARDCHANGE_END_62CUT:
		{
			CCL_CARDCHANGE_END_62CUT MsgData;
			MsgData.Get(lpdata, totsize);			

			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int pnum = *MsgData.ServPNum;
			if(faststrcmp(Room[rnum].Ui[pnum].ID,MsgData.ID) != 0) break;

			Room[rnum].ChangeCardEnd_62Cut(*MsgData.ServPNum,MsgData.ID);

		}break;

	*/

	case CL_ASK_RAISE: // 사용자의 레이즈
		{

			CCL_ASK_RAISE MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid==FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;

			if(User[unum].SUI.Position != WH_GAME) break;	//현재 위치가 게임방이 아니다
			
			int pnum = *MsgData.PNum;
			if(pnum<0 || pnum>=Room[rnum].get_maxplayer()) break;
			if(pnum != User[unum].PNum) break;

			Room[rnum].SendTurn(pnum,  MsgData.pRA);
		} break;

	case CL_WINDCARDEND: // 사용자의 카드날리기 종료
		{
			CCL_WINDCARDEND MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid==FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;

			if(User[unum].SUI.Position != WH_GAME) break;	//현재 위치가 게임방이 아니다
			
			int pnum = *MsgData.PNum;
			if(pnum<0 || pnum>=Room[rnum].get_maxplayer()) break;
			if(pnum != User[unum].PNum) break;

			Room[rnum].EndWind(pnum);

		} break;
	
	case CL_SELECTWINCASE:
		{
			CCL_SELECTWINCASE MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid==FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;

			if(User[unum].SUI.Position != WH_GAME) break;	//현재 위치가 게임방이 아니다
			
			int pnum = *MsgData.ServPNum;
			if(pnum<0 || pnum>=Room[rnum].get_maxplayer()) break;
			if(pnum != User[unum].PNum) break;

			int winCase = *MsgData.nCase;
			if(winCase < 0 || winCase > 4) break; 

			Room[rnum].SelectWinCase(pnum,  winCase);
		} break;

	case SV_ASK_CHANGESNDFX: // ### [사운드 추가 작업] ###
		{			
			CSV_ASK_CHANGESNDFX MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int unum = *MsgData.UNum;
			int sndfx = *MsgData.SndFX;
			if(unum<1 || unum>=MAX_USER) break;
			if(faststrcmp(MsgData.ID, User[unum].UI.ID) != 0) break;

			if(User[unum].SUI.Position != WH_GAME) break;

			int pnum = *MsgData.ServPNum;
			if(pnum<0 || pnum>=Room[rnum].get_maxnewplayer()) break;// ### [ 관전기능 ] ###
			if(pnum != User[unum].PNum) break;
			if(sndfx < 0 || sndfx > 10) break;
			// ### [ 관전기능 ] ###
			if(pnum<Room[rnum].get_maxplayer())	{ 
				Room[rnum].Ps[pnum].nSndFxKind = sndfx;
			}
			// 모두에게 알림다.
			Room[rnum].SendMsgToAll(MsgData.pData, MsgData.GetTotalSize());

		} break;

	////////////////////////////////////////////
	/////////// ### [ 관전기능 ] ###   /////////
	////////////////////////////////////////////
	// 대기 -> 겜참여
	case SV_ASK_PARTICIPATION:
		{
			CSV_ASK_PARTICIPATION MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int unum = *MsgData.UNum;
			int sndfx = *MsgData.SndFX;
			if(unum<1 || unum>=MAX_USER) break;
			if(faststrcmp(MsgData.ID, User[unum].UI.ID) != 0) break;
			if(User[unum].SUI.Position != WH_GAME) break;
			if(sndfx < 0 || sndfx > 10) break;

			Room[rnum].Participation(unum, *MsgData.ServPNum, sndfx);
		} break;

		// ### [ 관전기능 ] ###
		// 겜참여 -> 대기
	case SV_ASK_EMERAGE:
		{
			CSV_ASK_EMERAGE MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int unum = *MsgData.UNum;
			if(unum<1 || unum>=MAX_USER) break;
			if(faststrcmp(MsgData.ID, User[unum].UI.ID) != 0) break;
			if(User[unum].SUI.Position != WH_GAME) break;

			Room[rnum].Emerge_Out(unum, *MsgData.ServPNum);
		} break;

	////////////////////////////////////////////
	//////    ### [관리자 모드 작업] ###  //////
	////////////////////////////////////////////
	case NM_ADMIN_OUTUSER: // 사용자 제거하기(운영자)
		{
			CNM_ADMIN_OUTUSER MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = pUserVal->iUser2;
			if(unum != MsgData.pAMESG->AdminUNum) 
			{	// 아이디를 변조한 경우는 접속을 종료시킴
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid!=TRUE) break;
			if(User[unum].bAdminUser!=TRUE) break; 
			if(faststrcmp(MsgData.pAMESG->AdminID, User[unum].UI.ID) != 0) 
			{	// 아이디를 변조한 경우는 접속을 종료시킴
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
			
			int Tarunum = MsgData.pAMESG->TarUNum;
			if(Tarunum < 1 || Tarunum >= MAX_USER) break;
			if(User[Tarunum].bValid!=TRUE) break;
			if(faststrcmp(MsgData.pAMESG->TarID, User[Tarunum].UI.ID) != 0) break;
						
			int usersid = User[Tarunum].Sid;
			CMySocket *pSocket = SockMan.GetDataSocketPtr(usersid);
						
			if(pSocket==NULL) break;
			if(!pSocket->m_bCreated) break;
			USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);
			if(pUserVal==NULL) break;
			if(usersid != (int)pSocket->m_dwUserVal) break;

			CSV_SERVERMSG smsg(SndBuf);
			CString strMess;
			strMess.Format("관리자에 의해 서버와의 접속이 종료됩니다.");
			smsg.Set(0, (char*)strMess.operator LPCTSTR()); 
			pSocket->Send(smsg.pData, smsg.GetTotalSize());
			SockMan.ReservClose(usersid); // 연결 종료 예약
		} break;

	case NM_ADMIN_DELROOM:
		{
			CNM_ADMIN_DELROOM MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = pUserVal->iUser2;
			if(unum != MsgData.pAMESG->AdminUNum) 
			{	// 아이디를 변조한 경우는 접속을 종료시킴
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid!=TRUE) break;
			if(User[unum].bAdminUser!=TRUE) break; 
			if(faststrcmp(MsgData.pAMESG->AdminID, User[unum].UI.ID) != 0) 
			{	// 아이디를 변조한 경우는 접속을 종료시킴
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}

			int rnum = MsgData.pAMESG->TarUNum; //방번호
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(faststrcmp(Room[rnum].Ri.ID, MsgData.pAMESG->TarID) != 0) break;

			Room[rnum].Admin_Room_Remove();
		} break;
			//귓말 하기
	case NM_ADMIN_WHISPERCHAT:
		{
			CNM_ADMIN_WHISPERCHAT MsgData;
			MsgData.Get(lpdata, totsize);
			//(추가)
			// 아이디가 올바른지 검사
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(User[unum].bAdminUser!=TRUE) break; 
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) {
				// 아이디를 변조한 경우는 접속을 종료시킴
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
		
			int destunum = *MsgData.DestUNum;
			char *pdestid = MsgData.DestID;
			if(destunum < 0 || destunum >= MAX_USER) break;
			if(strlen(pdestid)==0 || strlen(pdestid)>=16) break;
			if(User[destunum].bValid==FALSE) break;
			if(faststrcmp(pdestid, User[destunum].UI.ID) != 0) break;

			if(User[destunum].bNoWhisper) {
				CSV_SERVERMSG smsg(SndBuf);
				CString str;
				str.Format("[%s]님은 귓말 거부중입니다. [강제]", pdestid);
				smsg.Set(0, (char*)(LPCTSTR)str);
				SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
				//무시
			}

			SockMan.SendData(User[destunum].Sid, MsgData.pData, MsgData.GetTotalSize());
		} break;

	case NM_ADMIN_CHANGEROOM: // [관리자 모드 작업] 겜방 옵션 변경요청(운영자)
		{
			CNM_ADMIN_CHANGEROOM MsgData;
			MsgData.Get(lpdata, totsize);
			int unum = pUserVal->iUser2;
			if(unum != MsgData.pAMESG->AdminUNum) 
			{	// 아이디를 변조한 경우는 접속을 종료시킴
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid!=TRUE) break;
			if(User[unum].bAdminUser!=TRUE) break; 
			if(faststrcmp(MsgData.pAMESG->AdminID, User[unum].UI.ID) != 0) 
			{	// 아이디를 변조한 경우는 접속을 종료시킴
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
		
			int rnum = MsgData.STYLE->RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;
			if(faststrcmp(MsgData.STYLE->ID, Room[rnum].Ri.ID)!=0) break;
			Room[rnum].ChangeRoomOption_ADMIN( &MsgData);

		} break;

	case NM_SPECIALMESSAGE:
		{
			CNM_SPECIALMESSAGE MsgData;
			MsgData.Get(lpdata, totsize);
	
			int unum = pUserVal->iUser2;
			if(unum != MsgData.pAMESG->AdminUNum) 
			{	// 아이디를 변조한 경우는 접속을 종료시킴
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid!=TRUE) break;
			if(User[unum].bAdminUser!=TRUE) break; 
			if(faststrcmp(MsgData.pAMESG->AdminID, User[unum].UI.ID) != 0) 
			{	// 아이디를 변조한 경우는 접속을 종료시킴
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
			
			int Tarunum = MsgData.pAMESG->TarUNum;
			if(Tarunum < 1 || Tarunum >= MAX_USER) break;
			if(User[Tarunum].bValid!=TRUE) break;
			if(faststrcmp(MsgData.pAMESG->TarID, User[Tarunum].UI.ID) != 0) break;
						
			int usersid = User[Tarunum].Sid;
			CMySocket *pSocket = SockMan.GetDataSocketPtr(usersid);
						
			if(pSocket==NULL) break;
			if(!pSocket->m_bCreated) break;
			USERVAL *pUserVal = (USERVAL*)(pSocket->m_pUserPtr);
			if(pUserVal==NULL) break;
			if(usersid != (int)pSocket->m_dwUserVal) break;

			pSocket->Send(MsgData.pData, MsgData.GetTotalSize() );


			//CSV_SERVERMSG smsg(SndBuf);
			//CString strMess;
			//strMess.Format("관리자에 의해 서버와의 접속이 종료됩니다.");
			//smsg.Set(0, (char*)strMess.operator LPCTSTR()); 
			//pSocket->Send(smsg.pData, smsg.GetTotalSize());
			//SockMan.ReservClose(usersid); // 연결 종료 예약
		
		} break;
	}
	return TRUE;
}


BOOL CChannel::CheckGameLevel(INT64 PMoney, int nIcon, int nStyle,int nLevel,int &nError)
{
	// ### [ 채널 입장 구분 ] ###
	
	nError = 0;

	switch(ChannelStyle){
	case 0: // 자유채널 
		{
			return TRUE;
		} break;
		
	case 1:// 초보서버
		{
			if(nIcon >= 0 && nIcon <= 3) return TRUE;
			// 예외 
			if(nIcon == 4 && PMoney <= 5000000000) return TRUE; // 50억
			
		} break;			
	case 2: // 중수서버 
		{
			if(nIcon >= 4) return TRUE;
		} break;
	case 3: // 고수서버
		{
			if(nIcon >= 8) return TRUE;			
		} break;
	case 4: // 도신서버 예외 처리(입장가능)
		{
			if(nIcon >= 12) return TRUE;			
		} break;
	}
/*
	switch(ChannelStyle){
	case 0: // 자유채널 
		{
			return TRUE;
		} break;
		
	case 1:// 초보서버
		{
			if(nIcon >= 0 && nIcon <= 3) return TRUE;
			// 예외 
			if(nIcon == 4 && PMoney <= 5000000000) return TRUE; // 50억
			
		} break;			
	case 2: // 중수서버 
		{
			if(nIcon >= 4 && nIcon <= 7) return TRUE;
			// 예외 
			if(nIcon == 8 && PMoney <= 150000000000) return TRUE; // 1500억
			
		} break;
	case 3: // 고수서버
		{
			if(nIcon >= 8 && nIcon <= 11) return TRUE;
			// 예외 
			if(nIcon == 12 && PMoney <= 3000000000000) return TRUE; //3조
			// 예외 추가(입장가능) 도신은 고수 서버까지 입장가능 2003 0625
			if(nIcon >= 12) return TRUE;
			
		} break;
	case 4: // 도신서버 예외 처리(입장가능)
		{
			if(nIcon >= 12 && nIcon <= 14) return TRUE;
			
		} break;
		
	default : // 자유서버
		{
			return TRUE;
		}
		break;
	}
*/	
	return FALSE;

}

char CChannel::GetRaiseNum(int bPlayer)
{
	char nRaise = 0;
	if(bPlayer == 0) {
		switch(ChannelStyle){
		case 0: // 자유채널
			nRaise     =  0; // 1회레이즈
			break; 
		case 1: // 초보채널
			nRaise     =  1; // 1회레이즈
			break; 
		case 2: // 중수 채널
			nRaise     =  2; // 1회레이즈
			break; 
		case 3: // 고수채널
			nRaise     =  3; // 2회레이즈
			break; 
		case 4: // 도신채널
			nRaise     =  4; // 2회레이즈, 히든3회
			break; 
		}
	}
	else {
		switch(ChannelStyle){
		case 0: // 자유채널
			nRaise     =  0; // 1회레이즈
			break; 
		case 1: // 초보채널
			nRaise     =  1; // 1회레이즈
			break; 
		case 2: // 중수 채널
			nRaise     =  2; // 1회레이즈
			break; 
		case 3: // 고수채널
			nRaise     =  3; // 2회레이즈
			break; 
		case 4: // 도신채널
			nRaise     =  4; // 2회레이즈, 히든3회
			break; 
		}
	}
	return nRaise;
}

	
char CChannel::GetBetStyle(int bPlayer)
{
	char BetStyle = 0;
	if(bPlayer==0) {
		switch(ChannelStyle){
		case 0: // 자유채널
			BetStyle     =  1; // 하프
			break; 
		case 1: // 초보채널
			BetStyle     =  1; // 하프		
			break; 
		case 2: // 중수 채널
			BetStyle     =  1; // 하프		
			break; 
		case 3: // 고수채널
			BetStyle     =  1; // 하프
			break; 
		case 4: // 도신채널
			BetStyle     =  1; // 하프
			break; 
		}
	} else {
		switch(ChannelStyle){
		case 0: // 자유채널
			BetStyle     =  0; // 쿼터
			break; 
		case 1: // 초보채널
			BetStyle     =  0; // 쿼터
			break; 
		case 2: // 중수 채널
			BetStyle     =  0; // 쿼터
			break; 
		case 3: // 고수채널
			BetStyle     =  1; // 하프
			break; 
		case 4: // 도신채널
			BetStyle     =  1; // 하프
			break; 
		}
	}
	return BetStyle;
}

BOOL CChannel::CheckGameMoney(INT64 Money)
{
	switch(ChannelStyle){
	case 1: //초보채널 ##머니제한: 0 ~ g_POKERSET.nHM*2)
		{
			if(Money >= g_pMainView->Cfg3.nCM) {// 100 억
				return FALSE;
			}
		} break;
	case 2: // 중수채널 ##머니제한: g_POKERSET.nHM ~ g_POKERSET.nFM*20)
		{
			if(Money >= g_pMainView->Cfg3.nHM){ // 5000 억
				return FALSE;
			}
		} break;
	case 3: // 고수채널 ##머니제한: g_POKERSET.nFM ~ g_POKERSET.nGM*50)
		{
			if(Money >= g_pMainView->Cfg3.nFM){ // 100조
				return FALSE;
			}

		} break;
	case 4: // 도신채널 ##머니제한: g_POKERSET.nGM 이상)
		{
			if(Money >= g_pMainView->Cfg3.nGM){ // 1경
					return FALSE;
			}

		} break;
	default: // 자유채널  ##머니제한: X
		{
			return TRUE;
		
		} break;
	}
	return TRUE;
}


