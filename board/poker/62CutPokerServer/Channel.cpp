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

// ���� �ɹ�
//char CChannel::SndBuf[SENDQUESIZE];

CChannel::CChannel()
{
	ChNum = -1;
	TotUser = 0;
	ChannelStyle = 0; // ���� ä�� ���� - 0: ���ӽ�
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
	// ä�� ���
	if(g_pMainView->Cfg3.EnterLimitMode == 1 && g_pMainView->Cfg3.bChanLimit) {
		ChannelStyle = g_pMainView->Cfg3.Info[ChNum].Grade; // ���� ä�� ���� 
	}
	else 
		ChannelStyle = g_pMainView->Cfg3.Info[ChNum].Grade; // ���� ä�� ���� 

	m_channel_usernum = g_pMainView->Cfg3.Info[ChNum].GameKind; // ���� ä�� ����  

	// ���� ���� ����
	JackPotSaveRatio = 1;
	// ���� ��� ���޺���(��Ƽ��)
	switch(ChannelStyle)
	{
	case 0: // ����
		m_betmoney_5user = 100;
		m_betmoney_6user = 100;

		DefaultJackPot = g_pMainView->Cfg4.Free;			// �⺻ ���� �Ӵ�
		JackPotSaveRatio = g_pMainView->Cfg4.rFree;			// ���� ����
		break;
	case 1: // �ʺ�
		m_betmoney_5user = 2;								// �⺻ �氪 ó��
		m_betmoney_6user = 100;

		DefaultJackPot = g_pMainView->Cfg4.ChoBo;
		JackPotSaveRatio = g_pMainView->Cfg4.rChoBo;
		break;
	case 2: // �߼�
		m_betmoney_5user = 50;	
		m_betmoney_6user = 100;

		DefaultJackPot = g_pMainView->Cfg4.JungSu;
		JackPotSaveRatio = g_pMainView->Cfg4.rJungSu;
		break;
	case 3: // ���	
		m_betmoney_5user = 1000;
		m_betmoney_6user = 100;

		DefaultJackPot = g_pMainView->Cfg4.GoSu;
		JackPotSaveRatio = g_pMainView->Cfg4.rGosu;
		break;
	case 4: // ����
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

// ### [ ä�� ���� ���� ] ###
const CHANNELINFO CChannel::GetChannelInfo()
{
	CHANNELINFO ci={0,};
	ci.ChNum = ChNum;
	ci.NowUserNum = TotUser;
	ci.MaxUserNum = MAX_CHUSER;
	strncpy(ci.Title, strChanName, 19);
	// 2003 06021 ä�� ��� ǥ��(����)
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
	// OnNewUser() ���ϰ�
	// code :
	//         >= 0  ���� ä�� ����;
	//		     -3  ���� ��޻� ���� ������ ����(���̾�α� �ڽ�)	
	//           -4  �������� ���� �ذ�ʵ�(2�ܰ� �̻� ���̳�)
	//			 -6  ���� ������(ä�κ� �ѵ� �ݾ�)
	//			 -7  ���� ��޿� ���� �� �� ���µ� ������������ �ֳ�(����Ұ��ΰ� ����?)
	//         <  0  ( ä���� ��á��,...)
	//
	// nUseItem :
	//		      0  �Ϲ��� ä�� ����
	//            1  �̹� �ѹ��̶� ����� ������������ ������ ����
*/

	// [���� ������ �۾�]
	BOOL bAskUseJumpItem = FALSE;
	//### [������ ��� �۾�] ###
	// ä�� ���� ���� ����̸�
	if(!User[unum].bAdminUser && g_pMainView->Cfg3.EnterLimitMode == 1 && g_pMainView->Cfg3.bChanLimit) {
		int nError = 0;// ä�� ��� ���ѿ� ���� ���� ���ɿ��θ� �Ǵ�
		if(!CheckGameLevel(User[unum].UI.PMoney, User[unum].UI.nIcon, 0, 0, nError))
		{	
			BOOL bAccessDeny = FALSE;
			BOOL bJumpUsing = FALSE;
			BOOL bJumpExist = FALSE;
			bJumpUsing = GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_JUMP, &bJumpExist);
			int level = User[unum].UI.nIcon;
			int grade =  GetMyLeveltoGrade(level); // ���� -> ���

			// ### [ ä�� ���� ���� ] ###
			if(grade+1 >= ChannelStyle && grade < ChannelStyle) // �Ѵܰ� ���� ����
			{
				// ���� ����
				// ���� ���� �������� �ڽ��� ������ ���� ���� ���� ������� ���� �ǻ糪 ��� ���θ� �����
				if( !bJumpUsing && 
					CheckPremiumState(&User[unum].UI, NULL)==0 &&	// [�����̾� �۾�]
					!User[unum].UI.bPremIP )						// [PC�� �۾�]
				{
					// ������������ ����ϰ� ���� �ʰ� �����̾� ȸ���� �ƴ϶��

					if(bJumpExist) {
						// ���� �������� �����Ѵٸ� ��� ���θ� ����� �Ѵ�(�ϴ� �α��� ó���� ��� ����)
						bAskUseJumpItem = TRUE;
					}
					else {
						// ���� �������� ����ϸ� ������ �� �ִ� ����� ���� ���� �ǻ縦 ���(�ϴ� �α��� ó���� ��� ����)
					//	bAskBuyJumpItem = TRUE;

						CString strmsg;
						strmsg.Format("������ ���� �����Ͻ� ä�ο� �����Ͻ÷��� [���� ����� ������]�� �ʿ��մϴ�. [���� ����� ������]�� �����Ͻðڽ��ϱ�?");

						BUYGAMEITEMINFO bi = {0,};
						bi.Kind = 2;	// ä�� ����� ���� ����� �������� ���� �ǻ縦 ���
						strcpy(bi.ItemName, "���� ����� ������");
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
					// ���� �������� ������̰ų� �����̾� ȸ���̶�� ���� �㰡
					bAccessDeny = FALSE;
				}
			}
			else
			{  // �Ұ� 
				bAccessDeny = TRUE;

				// [ �Ѿ����� ] 2004.04.20
				// �����̾� 
				if(grade-1 == ChannelStyle) {
					if(CheckPremiumState(&User[unum].UI, NULL)!=0) bAccessDeny = FALSE;
				}
			}
			if(bAccessDeny) {
				// ���� �����۵� ���� ����� ���� �ʴ� ���� ä�ο� ������ �� ����
				return -4;
			}
		}
	}

	// [���� ������ �۾�]
	if(bAskUseJumpItem) {
		// ���� ����� �������� �����Ѵٸ� ��� ���θ� ���

		// ������ ��� ����
		char* strmsg1 = "������ ���� �����Ͻ� ä�ο� �����Ͻ÷��� [���� ����� ������]�� �ʿ��մϴ�.\n���� �����Ͻ� [���� ����� ������]�� ����Ͻðڽ��ϱ�?";
		char* strmsg2 = "[���� ����� ������]�� ����Ͻø� ä���� ����� ���Ƶ� �����Ͻ� �� �ֽ��ϴ�. �����Ͻ� �������� �Ʒ��� ������, �������� ����Ͻ÷��� ��Ͽ��� �������� ������ ����Ŭ���ϰų� [����ϱ�] ��ư�� Ŭ���Ͻø� �˴ϴ�.";

		GAMEITEM_INFO in = {0,};
		in.Kind = 2;	// ä�� ����� ���� ����� ������ ����� ���
		in.StrMsgLen1 = strlen(strmsg1);
		in.StrMsgLen2 = strlen(strmsg2);
		CTime time = CTime::GetCurrentTime();
		time.GetAsSystemTime(in.ServerTime);

		/*
		// ��� ������ ���� ����� �����۸� �߷���
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
		// ��� �������� �߷���
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

	// ����� ä�� ���� ó��
	Cid[cid] = unum;
	User[unum].ChNum = ChNum;
	User[unum].Cid = cid;

	User[unum].SUI.Position = WH_LOBY;
	User[unum].SUI.RoomNum = 0;

	// ��� ä�� ����ڿ��� ���ο� ������ ä�ο� �����Ͽ����� �˸�
	//### [������ ��� �۾�] ###
	if( User[unum].bAdminUser )
	{
		//��޺� �Ӵ� ����
	}

	// ��� ä�� ����ڿ��� ���ο� ������ ä�ο� �����Ͽ����� �˸�
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

	// ���� �߿� ����Ǵ� �����
	if(User[unum].SUI.Position == WH_GAME) {
		int rnum = User[unum].SUI.RoomNum;
		if(rnum > 0 && rnum < MAX_ROOM) {
			// ����� �� ���� ó��
			Room[rnum].OnUserOutRoom(unum, User[unum].UI.ID, TRUE);
		}
		else {
			CLogFile logfile;
			logfile.Writef("CChannel::OnOutUser() - �߸��� ���ȣ�� ���� ������ ����(rnum=%d)", rnum);
		}
	}

	// ��� ä�� ����ڿ��� ������ ä�ο��� ���� ������ �˸�
	CSV_OUTUSER oumsg(SndBuf);
	oumsg.Set(&User[unum].SUI);
	SendMsgToLobyOpen(oumsg.pData, oumsg.GetTotalSize());

	// ����� ä�� ���� ó��
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
	// ��ȿ�� ������ �ƴϸ� ó������ ����
	if(pUserVal->iUser1 == FALSE) return FALSE;

	switch(signal) // �޽��� ��ȣ
	{
	case SV_ASK_BADUSER: // ��ȭ���� �Ű� ��û
		{
			CSV_ASK_BADUSER MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RNum;
			int unum = *MsgData.UNum;
			int l_WatchID = *MsgData.l_StrWatchID;
			int l_StrChat = *MsgData.l_StrChat;

			// �����˻�
			if(unum < 1 || unum >= MAX_USER) break;
			if(strlen(MsgData.ID) == 0 || strlen(MsgData.ID) >= 16) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;
			if(l_WatchID<0 || l_WatchID>256) break;
			if(l_StrChat<=0 || l_StrChat>20000) break;

			// ��Ʈ�� �޸� �Ҵ�
			char *pWatchID = new char[l_WatchID+2];
			memset(pWatchID, 0, l_WatchID+2);
			strncpy(pWatchID, MsgData.WatchID, l_WatchID);
			char *pChat = new char[l_StrChat+2];
			memset(pChat, 0, l_StrChat+2);
			strncpy(pChat, MsgData.StrChat, l_StrChat);

			// ���� ��¥ �� �ð�
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

			// BadUserDB Work ť�� ó���� �� ����
			BadUserDBQue.EnQue(Work);
	
			if(pWatchID) delete pWatchID;
			if(pChat) delete pChat;

			CString str;

			if(*MsgData.bAnnounce == FALSE)
			{
				str.Format("�Ű����� �����Ǿ����ϴ�.(�̸޼����� ȸ���Կ��Ը� �뺸�˴ϴ�)");
				CSV_SERVERMSG smsg(SndBuf);
				smsg.Set(0, (char*)str.operator LPCTSTR());
				SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
			}
			else
			{
				str.Format("<%s>���� ��ȭ������ �Ű��Ͽ����ϴ�", MsgData.ID);
				if(rnum == 0)
				{
					CSV_SERVERMSG smsg(SndBuf);
					smsg.Set(0, (char*)str.operator LPCTSTR());
					// ���ǿ� �ִ� ��� ����ڿ��� ����
					SendMsgToLoby(smsg.pData, smsg.GetTotalSize());
				}
				else
				{
					if(rnum<1 || rnum>=MAX_ROOM) break;
					// ���� ����� ����ڿ��� ����
					CSV_SERVERMSG smsg;
					smsg.Set(0, (char*)str.operator LPCTSTR());
					Room[rnum].SendMsgToAll(smsg.pData, smsg.GetTotalSize());
				}
			}
		} break;

	case SV_ASK_ALLUSERINFO: // ��� ����� ��� ��û
		{
			// Ư���� �޼����� �����Ƿ� �޼��� �м��� �ʿ����
			
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

			// ��� ����� ����� ������
			CSV_ALLUSERINFO msg(SndBuf);
			msg.Set(totnum, sui);
			SockMan.SendData(sid, msg.pData, msg.GetTotalSize());
		} break;

	case SV_ASK_ALLROOMINFO: // ��� ������ ��û
		{
			// Ư���� �޼����� �����Ƿ� �޼��� �м��� �ʿ����

			ROOMINFO ri[MAX_ROOM];
			int totnum=0;
			for(int i=1; i<MAX_ROOM; i++) {
				if(Room[i].bValid) {
					memcpy(&ri[totnum], &Room[i].Ri, sizeof(ROOMINFO));
					totnum++;
				}
			}
			
			// ��� �������� ������
			CSV_ALLROOMINFO msg(SndBuf);
			msg.Set(totnum, ri);
			SockMan.SendData(sid, msg.pData, msg.GetTotalSize());
		} break;

	case SV_ASK_ROOMINFO: // �� ������ �䱸(������ ����)
		{
			CSV_ASK_ROOMINFO MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			//if(rnum<1 || rnum>=MAX_ROOM) break;

			int ErrCode=0;
			if(rnum<1 || rnum>=MAX_ROOM) ErrCode=1; // �߸��� ���ȣ
			else if(strlen(Room[rnum].Ri.ID)==0) ErrCode=2; // ��û�� ���� �������� ����

			// ������ �ִ� ���� ��� ����
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

	case SV_ASK_CREATEROOM: // �� ���� ��û
		{
			CSV_ASK_CREATEROOM MsgData;
			MsgData.Get(lpdata, totsize);

			// [���� ������ �۾�]
			int rn1, rn2;
			if(MsgData.RI->bSuperMaster) {
				// ���۹� ���� �����̶�� �� ����Ʈ�� ���ʿ��� ����� �˻�
				rn1 = 1;
				rn2 = MAX_SUPERROOM + 1;
			}
			else {
				// �Ϲݹ� ���� �����̶�� �� ����Ʈ�� ���ʿ��� ����� �˻�
				rn1 = MAX_SUPERROOM + 1;
				rn2 = MAX_ROOM;
			}
			
			int i = 0, rnum = -1;
			for(i = rn1; i < rn2; i++) {
				// ����ִ� ���� ã�´�.
				if(!Room[i].bValid) { rnum=i; break;}
			}


			// [���� ������ �۾�]
			if(rnum < 0) {
				if(MsgData.RI->bSuperMaster) {
					// ���� ������ �� ������ �� ���� ���� �޼����� ����
					CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
					sndmsg.Set(999); // �ƹ��� �޽����� ������� �ʰ���
					SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());

					// ����ڿ��� �����޽����� ����
					CSV_SERVERMSG smsg(SndBuf);
					smsg.Set(1, "�� �̻� ���۹��� ���� �� �����ϴ�.");
					SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());

					break;
				}
				else {
					// ���� ������ �� ������ �� ���� ���� �޼����� ����
					CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
					sndmsg.Set(0); // �� ���� �� �ʰ�
					SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());
					break;
				}
			}

			int unum = MsgData.RI->UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum < 1 || unum >= MAX_USER) break;

			// �� ������ ��û�� ���̵� ���� ����ڿ� Ʋ���ų� ���� ��ġ�� ������ �ƴ϶��
			if(unum != User[unum].UI.UNum 
				|| faststrcmp(User[unum].UI.ID, MsgData.RI->ID) != 0
				|| User[unum].SUI.Position != WH_LOBY) 
			{
				CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
				sndmsg.Set(1); // ���� ������ �� ����
				SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());
				break;
			}

			if( User[unum].bAdminUser ) // ### [ ������� ] ###
			{
				CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
				sndmsg.Set(1); // �� ���� �� �ʰ�
				SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());
				break;
			}

			int roombetmoney = 0;
			if(MsgData.RI->bPlayer==0)
				roombetmoney = m_betmoney_5user;
			else 
				roombetmoney = m_betmoney_6user;

			/*			
			if(User[unum].UI.PMoney < roombetmoney) // ���� ������ ���ٸ� ���� ���� �� ���� deleted by jeong
			{
				CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
				sndmsg.Set(2); // ���� ������ �� ����
				SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());
				break;
			}
			*/
			
			int gamekind = MsgData.RI->GameKind;
			int level = MsgData.RI->Level;

			// [������Ŀ] �游��� �ֳ� Ȯ���Ѵ�
			//int nError = 0;

			// [�������]�Ϲݼ������ ������� ������ ����
			if(MsgData.RI->Secret !=0  ) {
				// ��������� �����Ϸ��� �Ͽ��ٸ� ������� �������� ��������� �˻�
				BOOL bExist = FALSE;
				if(!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_SECRET, &bExist) &&
					!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_INVINCIBLE, &bExist) && // [õ�Ϲ��� ������]
					CheckPremiumState(&User[unum].UI, NULL) == 0 &&	    // [�����̾� �۾�]
					!User[unum].UI.bPremIP )							// [PC�� �۾�]
				{
					// ������� �������� ����ϰ����� �ʴٸ� ���� Ȥ�� ����� ����

					// ���� ������ �� ������ �� ���� ���� �޼����� ����
					CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
					sndmsg.Set(999); // �ƹ��� �޽����� ������� �ʰ���
					SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());

					// ���۹��� �������� ���� Ȥ�� ����� ����� �޽����� ����
					CString strmsg;
					strmsg.Format("��������� �����ϱ� ���Ͽ� [������� ������]�� �ʿ��մϴ�.\n[������� ������]�� ���� Ȥ�� ����Ͻðڽ��ϱ�?");

					BUYGAMEITEMINFO bi = {0,};
					bi.Kind = 5;	// ������� �������� ���� Ȥ�� ��� �ǻ縦 ���
					strcpy(bi.ItemName, "������� ������");
					bi.ItemCode = ITEM_SCODE_SECRET;
					bi.StrMsgLen = strmsg.GetLength();

					CSV_BUYGAMEITEM bgmsg(SndBuf);
					bgmsg.Set(&bi, strmsg);
					SockMan.SendData(sid, bgmsg.pData, bgmsg.GetTotalSize());

					break;
				}
			}
					
			// [���� ������ �۾�]
			if(MsgData.RI->bSuperMaster) {
				// ���۹��� �����Ϸ��� �Ͽ��ٸ� ���۹��� �������� ������̰ų� �����̾� ȸ������ �˻�
				// ���۹��� �����Ϸ��� �Ͽ��ٸ� õ�Ϲ��� �������� ��������� �˻� 
				BOOL bExist = FALSE;
				if(!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_SUPERMASTER, &bExist) &&
					//### [ ���αغ� ������ ] ###
					!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_ALLINSUPERMASTER, &bExist) &&
					!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_SAFEGUARDANGEL, &bExist) &&// [��ȣõ��]
					!GameItem_IsUsingItem(User[unum].GameItem, ITEM_SCODE_INVINCIBLE, &bExist) && // [õ�Ϲ��� ������]
					CheckPremiumState(&User[unum].UI, NULL) == 0 &&		// [�����̾� �۾�]
					!User[unum].UI.bPremIP )							// [PC�� �۾�]
				{
					// ���۹��� �������� ����ϰ����� �ʾҰ� �����̾� ȸ���� �ƴ϶�� ���� Ȥ�� ����� ����

					// ���� ������ �� ������ �� ���� ���� �޼����� ����
					CSV_REFUSE_CREATEROOM sndmsg(SndBuf);
					sndmsg.Set(999); // �ƹ��� �޽����� ������� �ʰ���
					SockMan.SendData(sid, sndmsg.pData, sndmsg.GetTotalSize());

					// ���۹��� �������� ���� Ȥ�� ����� ����� �޽����� ����
					CString strmsg;
					strmsg.Format("���۹��� �����ϱ� ���Ͽ� [���۹��� ������]�� �ʿ��մϴ�.\n[���۹��� ������]�� ���� Ȥ�� ����Ͻðڽ��ϱ�?");

					BUYGAMEITEMINFO bi = {0,};
					bi.Kind = 3;	// ���۹��� �������� ���� Ȥ�� ��� �ǻ縦 ���
					strcpy(bi.ItemName, "���۹��� ������");
					bi.ItemCode = ITEM_SCODE_SUPERMASTER;
					bi.StrMsgLen = strmsg.GetLength();

					CSV_BUYGAMEITEM bgmsg(SndBuf);
					bgmsg.Set(&bi, strmsg);
					SockMan.SendData(sid, bgmsg.pData, bgmsg.GetTotalSize());

					break;
				}
			}



			// ���� ���� ����
			Room[rnum].Clear();
			Room[rnum].bValid = TRUE;
			memcpy(&Room[rnum].Ri, MsgData.RI, sizeof(ROOMINFO));

			Room[rnum].set_player_usernum(m_channel_usernum); //�游��� �켱, 5, 7 �ο� ����

			//if(Room[rnum].Ri.GameKind != 0) Room[rnum].Ri.GameKind = 1;	// ���� ���

			// ################### ���� ��Ÿ�� #################
			if(Room[rnum].Ri.GameKind<0 || Room[rnum].Ri.GameKind>2) Room[rnum].Ri.GameKind=0; // ����,����,Ǯ
			if(Room[rnum].Ri.Level<0 || Room[rnum].Ri.Level>2) Room[rnum].Ri.Level=0; // 1ȸ,2ȸ,3ȸ������
			if(Room[rnum].Ri.FormKind < 0 || Room[rnum].Ri.FormKind > 1) Room[rnum].Ri.FormKind = 0; // normal, choice
			// [������Ŀ]
			Room[rnum].Ri.Level    = GetRaiseNum(Room[rnum].Ri.bPlayer); // ä�� ����
			Room[rnum].Ri.GameKind = GetBetStyle(Room[rnum].Ri.bPlayer); // ����, ����
			
			// [ �ּҸӴ� ���� ]
			if(Room[rnum].Ri.UsernIcon < 0 || Room[rnum].Ri.UsernIcon > 15) Room[rnum].Ri.UsernIcon = 0;
			if(Room[rnum].Ri.CreateRGrade < 0 || Room[rnum].Ri.CreateRGrade > 6) Room[rnum].Ri.CreateRGrade = 0;

			strncpy(Room[rnum].m_DealerID, Room[rnum].Ri.ID, 15);    // ������ ����ڷ� �����Ѵ�.
			Room[rnum].m_nPrimeUser = 0; // ��
			Room[rnum].Ri.RoomNum = rnum; // �� ��ȣ �ο�
			Room[rnum].Ri.NowUserNum = 1;
			Room[rnum].Ri.NowObserverNum = 0; // ### [ ������� ] ###
			
			Room[rnum].m_Raise.BetMoney = roombetmoney;// �氪 ����
			Room[rnum].Ri.BetMoney =	  roombetmoney;// �氪 ����

			//Room[rnum].Ri.MaxUserNum = 2;
			Room[rnum].Ri.MaxUserNum = min(Room[rnum].get_maxplayer(), Room[rnum].Ri.MaxUserNum);
			Room[rnum].Ri.nCard = User[unum].UI.PI.nCard; // �׹�ī��

			// ### [ ������� ] ###
			User[unum].bObserver = FALSE; 
			User[unum].UI.cObserver= 0;

			memcpy(&Room[rnum].Ui[0], &User[unum].UI, sizeof(USERINFO));

			// �� ������ �����
			CSV_ACCEPT_CREATEROOM msg(SndBuf);
			msg.Set(&Room[rnum].Ri);
			SockMan.SendData(sid, msg.pData, msg.GetTotalSize());

			// �̺�Ʈ�� �ִ� ��� �˸�
			Room[rnum].SendRoomEventNotice(0);	// ### [�̺�Ʈ Ƽ��] ###

			User[unum].PNum = 0;				// �÷��̾� ��ȣ ����
			User[unum].bExitSubscript = FALSE;	// ������ ������� �ʱ�ȭ
			
			// ### [�α� ��Ͽ�]###
			User[unum].nBet = 0;// ����� ���õ���Ÿ �ʱ�ȭ

			// ������� ��ġ�� �������
			User[unum].SUI.Position = WH_GAME;	// ���� ��ġ�� ���ӹ��̴�
			User[unum].SUI.RoomNum = rnum;		// ���� ��ġ�� ���ȣ

			Room[rnum].Ps[0].nSndFxKind = *MsgData.Port;// ### [���� �߰� �۾�] ###

			// ��� ����ڿ��� ���� �����Ǿ����� �˸��� ����� ������ ���ŵ��� �˸�
			CSV_NEWROOM nrmsg(SndBuf);
			nrmsg.Set(&Room[rnum].Ri);
			SendMsgToLoby(nrmsg.pData, nrmsg.GetTotalSize());

			CSV_CHANGEUSER cumsg(SndBuf);
			cumsg.Set(&User[unum].SUI);
			SendMsgToLobyOpen(cumsg.pData, cumsg.GetTotalSize());
		} break;

	case SV_ASK_ENTERROOM: // �濡 �����ϱ⸦ �䱸
		{
			CSV_ASK_ENTERROOM MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum >= MAX_USER) break;
			if(!User[unum].bValid) break;

			int rnum = *MsgData.RoomNum;

			int ErrCode=0;
			if(rnum<1 || rnum>=MAX_ROOM) // �߸��� ������
				ErrCode=1;
			else if(Room[rnum].bValid==FALSE) // ��û�� ���� �������� ����
				ErrCode=2;
			// ### [ ������� ] ###
			//else if(Room[rnum].Ri.NowUserNum >= Room[rnum].Ri.MaxUserNum) // �ִ��ο� �ʰ�
			//	ErrCode=4;
			else if(Room[rnum].GetPNum(MsgData.ID)!=-1) // �̹� �����ϰ� �ִ� ���̵�
				ErrCode=5;
			// ### [ ������� ] ###
			else if(!User[unum].bAdminUser && Room[rnum].Ri.Secret == TRUE && strncmp(Room[rnum].Ri.Pass, MsgData.Pass, 4) != 0) // ��� Ʋ��
				ErrCode=6;
			else if(unum != User[unum].UI.UNum || faststrcmp(MsgData.ID, User[unum].UI.ID) != 0) // ��û�� ���̵� �߸���
				ErrCode=7;
			// [ �ּҸӴ� ���� ]
			else if(Room[rnum].Ri.CreateRGrade > 0 && !User[unum].bAdminUser) { // ������ ����
				INT64 Money = GetLevelToUserMoney( Room[rnum].Ri.UsernIcon );
				Money = Money * GetCreateRoomGrade(Room[rnum].Ri.CreateRGrade) / 10; // �游���� �ּҸӴ� �������
				if(Money > User[unum].UI.PMoney) {
					ErrCode =16;
					
					CSV_REFUSE_ENTERROOM rermsg(SndBuf);
					rermsg.Set(ErrCode);
					SockMan.SendData(sid, rermsg.pData, rermsg.GetTotalSize());

					CSV_SERVERMSG smsg(SndBuf);
					CString str, strM;
					strM = g_MakeCommaMoney(Money);
					str.Format("�ּҸӴ������� ������ ���Դϴ�. %s�� �̻� ������ �� �ֽ��ϴ�.", strM);
					smsg.Set(1, (char*)(LPCTSTR)str);
					SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
					break;
				}
			}
			
			if(ErrCode == 0) {
				// ���� ��ġ�� ������ �ƴ϶��
				if(User[unum].SUI.Position != WH_LOBY) ErrCode = 7;
			}

			int i=0;
			int pnum = -1;
			// ### [ ������� ] ### 
			//������ �������� ����
			//������? ���뼺 �ο�
			//### [������ ��� �۾�] ###
			if(ErrCode == 0) {
				if( User[unum].bAdminUser )
					pnum = Room[rnum].get_maxnewplayer()-1; //������ �ڸ��� ����д�.
				else
				{
					if(Room[rnum].Ri.NowUserNum < Room[rnum].Ri.MaxUserNum )
					{	//����
						for(i=0; i < Room[rnum].Ri.MaxUserNum; i++) { 
							if(strlen(Room[rnum].Ui[i].ID) == 0){ pnum=i; break;}
						}
						
					} else {
						//����
						for(i=Room[rnum].get_maxplayer(); i < Room[rnum].get_maxnewplayer()-1; i++) {
							if(strlen(Room[rnum].Ui[i].ID) == 0){ pnum=i; break;}
						}
					}
					if(pnum == -1) ErrCode=4;
				}
			}

			if(ErrCode == 0) {
				// ���� ���� ���ڶ�� ������ �� ����
				if(User[unum].UI.PMoney < Room[rnum].Ri.BetMoney) ErrCode=9;
			}

			if(Room[rnum].bSuperRoomDestory == TRUE){
				ErrCode = 2;
			}

			// [���� ������ �۾�]
			if(ErrCode == 0) {
				if(Room[rnum].Ri.bSuperMaster) {
					// ���۹��� ��� ������ ����Ʈ�� ���̵� �����Ѵٸ� ���� ����
					if(Room[rnum].BanUserList.FindID(User[unum].UI.ID)) ErrCode = 10;
				}
			}

			if(ErrCode == 0) {				
				// �Ϲ� ������ ����Ʈ�� ���̵� �����Ѵٸ� ���� ����
				if(Room[rnum].VoteBanUserList.FindID(User[unum].UI.ID)) ErrCode = 10;				
			}	

			///////////////////////////////////////////////////////////////////////////////////////////////
			// ### [ �ߺ��� IP�� ] ###
			int myIpIndex = -1;
			if(ErrCode == 0) {
				if(g_pMainView->Cfg2.SameIPLimit > 0 &&  pnum<Room[rnum].get_maxplayer() ) {// ### [ ������� ] ###
					int samecnt = 0;
					int tmpIp[MAX_PLAYER]={0,};
					
					// ���� IP �뿪�� � �����ϴ��� �˻�
					for(int i=0; i<Room[rnum].get_maxplayer(); i++) {
						int tarunum = Room[rnum].Ui[i].UNum;
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
						ErrCode = 15;
					}
					else if(samecnt !=0) // �߰� Ŭ���̾�Ʈ���� �̹����� �����ش�.(���� ����)
					{
						if(samecnt == 1) // ó��
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
						{// ����
							myIpIndex = Room[rnum].Ps[myIpIndex].nCheckIp;
							//Room[rnum].m_aIpIndex[myIpIndex]++;// �ο��� ����
						}
					}
				}
			}
			///////////////////////////////////////////////////////////////////////////////////////////////				

			if(ErrCode == 0) {
				// �濡 ����ó�� // ### [ �ߺ��� IP�� ] ### // ### [���� �߰� �۾�] ###
				if(Room[rnum].OnUserEnterRoom(&User[unum].UI, pnum, myIpIndex,*MsgData.SndFxKind) == FALSE) ErrCode = 1;
			}

			// �����ϴµ� ������ �ִٸ� ������ �ź���
			if(ErrCode != 0) {
				CSV_REFUSE_ENTERROOM rermsg(SndBuf);
				rermsg.Set(ErrCode);
				SockMan.SendData(sid, rermsg.pData, rermsg.GetTotalSize());
				break;
			}

			////// ���� ó�� 

			User[unum].bExitSubscript = FALSE;// ������ ������� �ʱ�ȭ

			// ������� ��ġ�� �������
			User[unum].SUI.Position = WH_GAME;	// ���� ��ġ�� ���ӹ��̴�
			User[unum].SUI.RoomNum = rnum;		// ���� ��ġ�� ���ȣ
			// ��û�� ����ڿ��� �濡 ���� �㰡�� �뺸
			CSV_ACCEPT_ENTERROOM aermsg(SndBuf);
			ENTERROOMINFO ei = Room[rnum].GetEnterRoomInfo();

			/*
			//[62] �ٲٱ� �ÿ��� ���� ������ �Ⱥ��� �׷��� ���� �����³��� ������ ����
			if(Room[rnum].GameState == RSTATE_CHANGECARD){
				ei.Pg.nState = 3;
			}
			*/
			aermsg.Set(&ei);
			SockMan.SendData(sid, aermsg.pData, aermsg.GetTotalSize());

			////////////////// ### [ ������� ] ### ////////////////////
			// ���� ����
			Room[rnum].GetReservaitonInfo();
			
			Room[rnum].SendRoomEventNotice(pnum); // ### [�̺�Ʈ Ƽ��] ###

			// ��� ����ڿ��� �������� ����Ǿ����� �˸��� ����� ������ ���ŵ��� �˸�
			CSV_CHANGEROOM crmsg(SndBuf);
		//	SMALLROOMINFO sri = Room[rnum].GetSmallRoomInfo();
		//	crmsg.Set(&sri);
			crmsg.Set(&Room[rnum].GetSmallRoomInfo());
			SendMsgToLoby(crmsg.pData, crmsg.GetTotalSize());

			CSV_CHANGEUSER cumsg(SndBuf);
			cumsg.Set(&User[unum].SUI);
			SendMsgToLobyOpen(cumsg.pData, cumsg.GetTotalSize());
		} break;

	// [ ###�ٷν���### ]
	case SV_ASK_QUICKSTART: // �ٷ� ���� ����
		{
			CSV_ASK_QUICKSTART MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(unum<1 || unum >= MAX_USER) break;
			if(!User[unum].bValid) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break; // �߸��� ������ȣ�̴�

			//if(User[unum].SUI.Position != WH_LOBY) break;

			int gamekind = *MsgData.GameKind; // 0: ��� 1: 5�� 2: 6��

			int nRoomNum1 = -1;
			int nRoomNum2 = -1;
			for(int i=1; i<MAX_ROOM; i++) 
			{
				if(Room[i].bValid) 
				{
					// 5��, 7��
				//	if(gamekind != 0) {
				//		if(gamekind-1 != Room[i].Ri.bPlayer) continue;
				//	}

					if(Room[i].bSuperRoomDestory)continue;						

					if(gamekind != 0) {
						if(gamekind-1 != Room[i].Ri.FormKind) continue;
					}

					// [���� ������ �۾�]
					if(Room[i].Ri.bSuperMaster) {
					// ���۹��� ��� ������ ����Ʈ�� ���̵� �����Ѵٸ� ���� ����
						if(Room[i].BanUserList.FindID(User[unum].UI.ID)) continue;
					}

					
					// �Ϲ� ������ ����Ʈ�� ���̵� �����Ѵٸ� ���� ����
					if(Room[i].VoteBanUserList.FindID(User[unum].UI.ID)) continue;
								

					// �ּ� �Ӵ� ����
					if(Room[i].Ri.CreateRGrade > 0) {
						INT64 Money = GetLevelToUserMoney( Room[i].Ri.UsernIcon );
						Money = Money * GetCreateRoomGrade(Room[i].Ri.CreateRGrade) / 10; // �游���� �ּҸӴ� �������
						if(Money > User[unum].UI.PMoney) continue;
					}

					///////////////////////////////////////////////////////////////////////////////////////////////
					// ### [ �ߺ��� IP�� ] ###IP �ּ�
					if(g_pMainView->Cfg2.SameIPLimit > 0) {
						int samecnt = 0;
						// ���� IP �뿪�� � �����ϴ��� �˻�
						for(int k=0; k<Room[i].get_maxplayer(); k++) {
							int tarunum = Room[i].Ui[k].UNum;
							if(tarunum<1 || tarunum >= MAX_USER) continue;
							if(User[unum].PeerIP.S_un.S_un_b.s_b1 == User[tarunum].PeerIP.S_un.S_un_b.s_b1 && 
								User[unum].PeerIP.S_un.S_un_b.s_b2 == User[tarunum].PeerIP.S_un.S_un_b.s_b2 && 
								User[unum].PeerIP.S_un.S_un_b.s_b3 == User[tarunum].PeerIP.S_un.S_un_b.s_b3 )//&&
								//User[unum].PeerIP.S_un.S_un_b.s_b4 == User[tarunum].PeerIP.S_un.S_un_b.s_b4) //�׽�Ʈ 
							{
								samecnt++;
							}
						}
						
						if(samecnt >= g_pMainView->Cfg2.SameIPLimit) {
							// �ߺ��� IP�� �����Ͽ� ���� �Ұ�
							//ErrCode = 15;
							continue;
						}
					}
					///////////////////////////////////////////////////////////////////////////////////////////////
					
					if(Room[i].Ri.State == 0 && Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum 
						&& Room[i].Ri.Secret == 0 && (gamekind-1) == Room[i].Ri.FormKind && gamekind > 0)	// ����(0:����� 1:������)
					{// ����� ���
						nRoomNum1 = i;
						break;
					}
					else if( Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum 
						&& Room[i].Ri.Secret == 0 && (gamekind-1) == Room[i].Ri.FormKind && gamekind > 0)	// ����(0:����� 1:������)
					{// ������ ���
						nRoomNum2 = i;
					}
					else if(Room[i].Ri.State == 0 && Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum && Room[i].Ri.Secret == 0)	// ����(0:����� 1:������)
					{// ����� ���
						nRoomNum1 = i;
						break;
					}
					else if( Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum && Room[i].Ri.Secret == 0)	// ����(0:����� 1:������)
					{// ������ ���
						nRoomNum2 = i;
					}

				/*
					if(Room[i].Ri.State == 0 && Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum && Room[i].Ri.Secret == 0)	// ����(0:����� 1:������)
					{// ����� ���
						nRoomNum1 = i;
						break;
					}
					else if( Room[i].Ri.MaxUserNum > Room[i].Ri.NowUserNum && Room[i].Ri.Secret == 0)	// ����(0:����� 1:������)
					{// ������ ���
						nRoomNum2 = i;
					}
				*/
				}
			}


			if(nRoomNum1 != -1 || nRoomNum2 != -1)
			{// �� ����
				int rnum = 0;
				if(nRoomNum1 != -1)
					rnum = nRoomNum1;
				else
					rnum = nRoomNum2;

				int ErrCode=0;
				if(rnum<1 || rnum>=MAX_ROOM) // �߸��� ������
					ErrCode=1;
				else if(Room[rnum].bValid==FALSE) // ��û�� ���� �������� ����
					ErrCode=2;
				else if(Room[rnum].Ri.NowUserNum >= Room[rnum].Ri.MaxUserNum) // �ִ��ο� �ʰ�
					ErrCode=4;
				else if(Room[rnum].GetPNum(MsgData.ID)!=-1) // �̹� �����ϰ� �ִ� ���̵�
					ErrCode=5;
			//	else if(Room[rnum].Ri.Secret == TRUE && strncmp(Room[rnum].Ri.Pass, MsgData.Pass, 4) != 0) // ��� Ʋ��
			//		ErrCode=6;
				else if(unum != User[unum].UI.UNum || faststrcmp(MsgData.ID, User[unum].UI.ID) != 0) // ��û�� ���̵� �߸���
					ErrCode=7;				
				
				
				if(ErrCode == 0) {
					// ���� ��ġ�� ������ �ƴ϶��
					if(User[unum].SUI.Position != WH_LOBY) ErrCode = 7;
				}
				/*
				if(ErrCode == 0) {
					// �Ӵ� ���� ���� ���ڶ�� ������ �� ����				deleted by jeong
					if(User[unum].UI.PMoney < Room[rnum].Ri.BetMoney) ErrCode=9;
				}
				*/
				
				int i=0;
				int pnum = -1;
				// ### [ ������� ] ###
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
				// ### [ �ߺ��� IP�� ] ###
				int myIpIndex = -1;
				if(ErrCode == 0) {
					if(g_pMainView->Cfg2.SameIPLimit > 0 && pnum<Room[rnum].get_maxplayer() ) {// ### [ ������� ] ###
						int samecnt = 0;
						int tmpIp[MAX_PLAYER]={0,};
						
						// ���� IP �뿪�� � �����ϴ��� �˻�
						for(int i=0; i<Room[rnum].get_maxplayer(); i++) {
							int tarunum = Room[rnum].Ui[i].UNum;
							if(tarunum<1 || tarunum >= MAX_USER) continue;
							if(User[unum].PeerIP.S_un.S_un_b.s_b1 == User[tarunum].PeerIP.S_un.S_un_b.s_b1 && 
								User[unum].PeerIP.S_un.S_un_b.s_b2 == User[tarunum].PeerIP.S_un.S_un_b.s_b2 && 
								User[unum].PeerIP.S_un.S_un_b.s_b3 == User[tarunum].PeerIP.S_un.S_un_b.s_b3 )//&&
									//User[unum].PeerIP.S_un.S_un_b.s_b4 == User[tarunum].PeerIP.S_un.S_un_b.s_b4) //�׽�Ʈ
							{
								samecnt++;
								tmpIp[i]=1;
								myIpIndex = i;
							}
						}
						
						if(samecnt >= g_pMainView->Cfg2.SameIPLimit) {
							// �ߺ��� IP�� �����Ͽ� ���� �Ұ�
							ErrCode = 15;
						}
						else if(samecnt !=0) // �߰� Ŭ���̾�Ʈ���� �̹����� �����ش�.(���� ����)
						{
							if(samecnt == 1) // ó��
							{
								for(int kk=0; kk < Room[rnum].get_maxplayer();kk++)
								{
									if(Room[rnum].m_aIpIndex[kk] == 0) break;
								}
								Room[rnum].m_aIpIndex[kk] = 1;

								Room[rnum].m_nIpCnt++; // ���� ���ġ �ʴ´�.
								
								for(i=0;i<Room[rnum].get_maxplayer();i++)
								{
									if(tmpIp[i] != 0) Room[rnum].Ps[i].nCheckIp = kk;
								}
								myIpIndex = kk;
							
							}
							else
							{// ����
								myIpIndex = Room[rnum].Ps[myIpIndex].nCheckIp;
								//	Room[rnum].m_aIpIndex[myIpIndex]++;// �ο��� ����
							}
						}
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				
				
				if(ErrCode == 0) {
					// �濡 ����ó�� // ### [ �ߺ��� IP�� ] ### // ### [���� �߰� �۾�] ###
					if(Room[rnum].OnUserEnterRoom(&User[unum].UI, pnum, myIpIndex, *MsgData.SndFxKind) == FALSE) ErrCode = 1; // ### [ �ߺ��� IP�� ] ###
				}

				
				// �����ϴµ� ������ �ִٸ� ������ �ź���
				if(ErrCode != 0) {
					CSV_REFUSE_ENTERROOM rermsg(SndBuf);
					rermsg.Set(ErrCode);
					SockMan.SendData(sid, rermsg.pData, rermsg.GetTotalSize());
					break;
				}
				

			

				////// ���� ó�� 

				// ������ ������� �ʱ�ȭ
				User[unum].bExitSubscript = FALSE;
				// ������� ��ġ�� �������
				User[unum].SUI.Position = WH_GAME;	// ���� ��ġ�� ���ӹ��̴�
				User[unum].SUI.RoomNum = rnum;	// ���� ��ġ�� ���ȣ
				// �÷��̾� ��ȣ ����
				//User[unum].PNum = pnum;

				
				// ��û�� ����ڿ��� �濡 ���� �㰡�� �뺸
				CSV_ACCEPT_ENTERROOM aermsg(SndBuf);
				ENTERROOMINFO ei = Room[rnum].GetEnterRoomInfo();
				aermsg.Set(&ei);
				SockMan.SendData(sid, aermsg.pData, aermsg.GetTotalSize());

				// �̺�Ʈ�� �ִ� ��� �˸�
				Room[rnum].SendRoomEventNotice(pnum); // ### [�̺�Ʈ Ƽ��] ###

				////////////////// ### [ ������� ] ### ////////////////////
				// ���� ����
				Room[rnum].GetReservaitonInfo();

				// ��� ����ڿ��� �������� ����Ǿ����� �˸��� ����� ������ ���ŵ��� �˸�
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
			{// ���� ( �� ����)
				CSV_REFUSE_QUICKSTART rqstart(SndBuf);
				rqstart.Set(0);
				SockMan.SendData(sid, rqstart.pData, rqstart.GetTotalSize());
			}


			// Bank ���� ���� - jeong
			CSV_BANKINFO uimsg(SndBuf);
			uimsg.Set(User[unum].nBMoney);
			SockMan.SendData(sid, uimsg.pData, uimsg.GetTotalSize());

		} break;

	case SV_ASK_OUTROOM: // ����ڰ� �濡�� �����⸦ ��û
		{
			CSV_ASK_OUTROOM MsgData;
			MsgData.Get(lpdata, totsize);

			int rnum = *MsgData.RoomNum;
			int unum = *MsgData.UNum;
			if(unum != pUserVal->iUser2) break;
			if(rnum < 1 || rnum >= MAX_ROOM) break;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid==FALSE) break; // �߸��� ������ȣ�̴�
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break; // �߸��� ������ȣ�̴�
			if(Room[rnum].bValid == FALSE) break; // �̹� ������ ���̴�
			if(User[unum].SUI.Position != WH_GAME) break;	//���� ��ġ�� ���ӹ��� �ƴϴ�

			// ������ ������� �ʱ�ȭ
			User[unum].bExitSubscript = FALSE;

			Room[rnum].OnUserOutRoom(unum, User[unum].UI.ID, FALSE);
			
			// ### [�̺�Ʈ Ƽ��] ###
			g_pMainView->SendChannelEventNotice(unum);
		} break;

	case SV_LOBYOPEN: // �κ�â ���� ���� ����
		{
			CSV_LOBYOPEN MsgData;
			MsgData.Get(lpdata, totsize);

			// �����˻�
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

				// ��� ����� ����� ������
				CSV_ALLUSERINFO msg(SndBuf);
				msg.Set(totnum, sui);
				SockMan.SendData(sid, msg.pData, msg.GetTotalSize());
			}
		} break;

	case SV_ASK_INVITE: // ����ڰ� ��û�޼����� ����
		{
			CSV_ASK_INVITE MsgData;
			MsgData.Get(lpdata, totsize);
			
			// �����˻�
			int rnum = *MsgData.RoomNum;
			int tarunum = *MsgData.TarUNum;
			if(rnum < 0 || rnum >= MAX_ROOM) break;
			if(tarunum < 0 || tarunum >= MAX_USER) break;

			if(faststrcmp(User[tarunum].UI.ID, MsgData.TarID) != 0) break; // �߸��� ���̵��̴�
			if(strlen(MsgData.TarID) == 0) break;				// �߸��� ���̵��̴�
			if(strlen(Room[rnum].Ri.ID) == 0) break;			// �̹� ������ ���̴�
			if(User[tarunum].SUI.Position != WH_LOBY) break;	// ���ǿ� ���� �ʴ�
			SockMan.SendData(User[tarunum].Sid, MsgData.pData, MsgData.GetTotalSize());
		} break;

	case SV_REFUSE_INVITE: // ����ڰ� ��û�޼����� �ź���
		{
			CSV_REFUSE_INVITE MsgData;
			MsgData.Get(lpdata, totsize);

			// �����˻�
			int tarunum = *MsgData.TarUNum;
			if(tarunum < 0 || tarunum >= MAX_USER) break;

			if(faststrcmp(User[tarunum].UI.ID, MsgData.TarID) != 0) break; // �߸��� ���̵��̴�
			if(strlen(MsgData.TarID) == 0) break; // �߸��� ���̵��̴�
			SockMan.SendData(User[tarunum].Sid, MsgData.pData, MsgData.GetTotalSize());
		} break;

	case SV_ASK_BANISHVOTE: // �߹� ��ǥ ��û
		{
			CSV_ASK_BANISHVOTE MsgData;
			MsgData.Get(lpdata, totsize);
			
			// �����˻�
			int rnum = MsgData.pBV->RoomNum;
			if(rnum < 1 || rnum >= MAX_ROOM) break;
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
	
			if(!Room[rnum].bValid || strlen(Room[rnum].Ri.ID) == 0) break;	// �̹� ������ ���̴�
			if(faststrcmp(User[unum].UI.ID, MsgData.pBV->MyID) != 0) break;	// �߸��� ���̵��̴�
			if(User[unum].SUI.Position != WH_GAME) break;	// ���ӷ뿡 ���� �ʴ�

			Room[rnum].OnAskBanishVote(MsgData.pBV);
			
		} break;

	case SV_MYBANISHVOTE: // ���� �߹� ��ǥ ���
		{
			CSV_MYBANISHVOTE MsgData;
			MsgData.Get(lpdata, totsize);
			
			// �����˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(User[unum].SUI.Position != WH_GAME) break; // ���ӷ뿡 ���� �ʴ�
			int rnum = User[unum].SUI.RoomNum;
			if(rnum < 1 || rnum >= MAX_ROOM) break;
			
			if(!Room[rnum].bValid || strlen(Room[rnum].Ri.ID) == 0) break;	// �̹� ������ ���̴�
			Room[rnum].OnMyBanishVote(User[unum].UI.ID, *MsgData.bAgree);
		} break;

	case SV_CHATDATA: // ���� ä�� ����Ÿ
		{
			CSV_CHATDATA MsgData;
			MsgData.Get(lpdata, totsize);

			//(�߰�)
			// ���̵� �ùٸ��� �˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) {// ### [������ ��� �۾�] ###
				if(!User[unum].bAdminUser || faststrcmp("������", MsgData.ID)!=0) {
					// ���̵� ������ ���� ������ �����Ŵ
					SockMan.GetDataSocketPtr(sid)->PostClose();
					break;
				}
			}
			
			// [��ȭ ����] Ư�� ���̵� ã�Ƽ� ��ȭ ����
			if(g_CaptureID.bUse &&  g_pMainView->IsCaptureUserID(MsgData.ID) )
			{
				if(strlen(MsgData.ID) == 0 || strlen(MsgData.ID) >= 16) break;
				int l_StrChat = *MsgData.l_StrChat;
				if(l_StrChat<=0 || l_StrChat>20000) break;

				char *pChat= new char[l_StrChat+2];
				memset(pChat, 0, l_StrChat+2);
				strncpy(pChat, MsgData.StrChat, l_StrChat);
				
				// ���� ��¥ �� �ð�
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
					// ���� ��ġ
				Work.SubmitReason = "����";
				//Work.WatchID = pWatchID;
				Work.Contents = pChat;

				// BadUserDB Work ť�� ó���� �� ����
				BadUserDBQue.EnQue(Work);
				if(pChat) delete pChat;
			}
			// ���ǿ� �ִ� ����ڿ��Ը� �޽����� ����
			SendMsgToLoby(MsgData.pData, MsgData.GetTotalSize());
		} break;

	case SV_NOWHISPER: // �Ӹ� ��� ���� ����
		{
			CSV_NOWHISPER MsgData;
			MsgData.Get(lpdata, totsize);

			// �����˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;

			User[unum].bNoWhisper = *MsgData.bNoWhisper;
		} break;

	case SV_WHISPERCHAT: // �ӼӸ�
		{
			CSV_WHISPERCHAT MsgData;
			MsgData.Get(lpdata, totsize);

			//(�߰�)
			// ���̵� �ùٸ��� �˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) {
				// ���̵� ������ ���� ������ �����Ŵ
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
				str.Format("[%s]���� �Ӹ� �ź����Դϴ�.", pdestid);
				smsg.Set(0, (char*)(LPCTSTR)str);
				SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
				break;
			}

			// [��ȭ ����] Ư�� ���̵� ã�Ƽ� ��ȭ ����
			if( g_CaptureID.bUse &&  g_pMainView->IsCaptureUserID(MsgData.ID))
			{
				if(strlen(MsgData.ID) == 0 || strlen(MsgData.ID) >= 16) break;
				int l_StrChat = *MsgData.l_StrChat;
				if(l_StrChat<=0 || l_StrChat>20000) break;

				char *pChat= new char[l_StrChat+2];
				memset(pChat, 0, l_StrChat+2);
				strncpy(pChat, MsgData.StrChat, l_StrChat);
				// ���� ��¥ �� �ð�
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
				// ���� ��ġ
				Work.SubmitReason = "�ӼӸ�";
				Work.WatchID = pdestid;
				Work.Contents = pChat;
				// BadUserDB Work ť�� ó���� �� ����
				BadUserDBQue.EnQue(Work);
				if(pChat) delete pChat;
			}
			SockMan.SendData(User[destunum].Sid, MsgData.pData, MsgData.GetTotalSize());
		} break;


	case SV_ASK_FORCEBANISH: // ���� �߹� ��û	[���� ������ �۾�]
		{
		CSV_ASK_FORCEBANISH MsgData;
			MsgData.Get(lpdata, totsize);

			// �����˻�
			int unum = pUserVal->iUser2;
			if(unum<1 || unum>=MAX_USER) break;
			if(!User[unum].bValid) break;

			if(User[unum].SUI.Position != WH_GAME) break;
			int rnum = User[unum].SUI.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			int pnum = User[unum].PNum;
			if(pnum == -1) break;

			// ������ �ƴϸ� ���     <--- �߰�!!!
			if(strncmp(Room[rnum].Ri.ID, Room[rnum].Ui[pnum].ID, 15) != 0)
				break;

			Room[rnum].OnForceBanish(&MsgData);
		} break;

	case CL_CHATDATA: // ���ӹ� ä�� ����Ÿ
		{
			CCL_CHATDATA MsgData;
			MsgData.Get(lpdata, totsize);

			//(�߰�)
			// ���̵� �ùٸ��� �˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) {// ### [������ ��� �۾�] ###
				if(!User[unum].bAdminUser || faststrcmp("������", MsgData.ID)!=0) {
					// ���̵� ������ ���� ������ �����Ŵ
					SockMan.GetDataSocketPtr(sid)->PostClose();
					break;
				}
			}

			if(User[unum].SUI.Position != WH_GAME) break;

			// �����˻�
			int rnum = *MsgData.RoomNum;
			if(rnum<0 || rnum>=MAX_ROOM) break;

			// [��ȭ ����] Ư�� ���̵� ã�Ƽ� ��ȭ ����
			if( g_CaptureID.bUse &&  g_pMainView->IsCaptureUserID(MsgData.ID))
			{
				if(strlen(MsgData.ID) == 0 || strlen(MsgData.ID) >= 16) break;
				int l_StrChat = *MsgData.l_StrChat;
				if(l_StrChat<=0 || l_StrChat>20000) break;

				char *pChat= new char[l_StrChat+2];
				memset(pChat, 0, l_StrChat+2);
				strncpy(pChat, MsgData.StrChat, l_StrChat);
				// ���� ��¥ �� �ð�
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
				// ���� ��ġ
				Work.SubmitReason = "��";
				//Work.WatchID = pWatchID;
				Work.Contents = pChat;

				// BadUserDB Work ť�� ó���� �� ����
				BadUserDBQue.EnQue(Work);
				if(pChat) delete pChat;
			}
			Room[rnum].SendMsgToAll(MsgData.pData, MsgData.GetTotalSize());

		} break;

	case CL_EXITSUBSCRIPT: // ������ ����, ���
		{
			CCL_EXITSUBSCRIPT MsgData;
			MsgData.Get(lpdata, totsize);

			// �����˻�
			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break; // �̹� ������ ���̴�

			int unum = *MsgData.UNum;
			if(unum<1 || unum>=MAX_USER) break;
			if(unum != pUserVal->iUser2) break;
			if(User[unum].bValid==FALSE) break;
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) break;
			if(User[unum].SUI.Position != WH_GAME) break;	//���� ��ġ�� ���ӹ��� �ƴϴ�
			int pnum = Room[rnum].GetPNum(MsgData.ID);
			if(pnum == -1) break;
			Room[rnum].ExitSubScript(&MsgData, pnum, unum);
		} break;
		
	////////////////////// ���� ���� ���� �޼��� /////////////////////

	case CL_ASK_STARTGAME://���� ���� ��û
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

	case CL_ASK_CHANGEGAMEKIND: // ���� ��� ���� ��û
		{
			CCL_ASK_CHANGEGAMEKIND MsgData;
			MsgData.Get(lpdata, totsize);

			// �����˻�
			int rnum = *MsgData.RoomNum;
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(Room[rnum].bValid == FALSE) break;

			Room[rnum].OnAskChangeGameKind(&MsgData);
		} break;
 
	///////////////////////////////////////////////////////////////////////
	//*    7Card 
	case CL_ASK_FOLD: // ���� ����
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
			
			if(User[unum].SUI.Position != WH_GAME) break;	//���� ��ġ�� ���ӹ��� �ƴϴ�
			
			int pnum = *MsgData.PNum;
			if(pnum<0 || pnum>=Room[rnum].get_maxplayer()) break;
			if(pnum != User[unum].PNum) break;

			// ### [ ������� ] ###
			if( Room[rnum].FoldUser(pnum) )
			{
				// ### [ ������� ] ###
				Room[rnum].Observer_takeaSeat(User[unum].UI.ID);
			}

		} break;

	case CL_ASK_SELECTCARD: // ������� ī�弱��
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

			if(User[unum].SUI.Position != WH_GAME) break;	//���� ��ġ�� ���ӹ��� �ƴϴ�
			
			int pnum = *MsgData.PNum;
			if(pnum<0 || pnum>=Room[rnum].get_maxplayer()) break;
			if(pnum != User[unum].PNum) break;

			int discard = *MsgData.DisCard;
			if(discard < 1 || discard > 4) break;
			Room[rnum].OnReadyToStart(pnum, MsgData.pSD, discard);
		} break;

		// ������ ���� 
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

	case CL_ASK_RAISE: // ������� ������
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

			if(User[unum].SUI.Position != WH_GAME) break;	//���� ��ġ�� ���ӹ��� �ƴϴ�
			
			int pnum = *MsgData.PNum;
			if(pnum<0 || pnum>=Room[rnum].get_maxplayer()) break;
			if(pnum != User[unum].PNum) break;

			Room[rnum].SendTurn(pnum,  MsgData.pRA);
		} break;

	case CL_WINDCARDEND: // ������� ī�峯���� ����
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

			if(User[unum].SUI.Position != WH_GAME) break;	//���� ��ġ�� ���ӹ��� �ƴϴ�
			
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

			if(User[unum].SUI.Position != WH_GAME) break;	//���� ��ġ�� ���ӹ��� �ƴϴ�
			
			int pnum = *MsgData.ServPNum;
			if(pnum<0 || pnum>=Room[rnum].get_maxplayer()) break;
			if(pnum != User[unum].PNum) break;

			int winCase = *MsgData.nCase;
			if(winCase < 0 || winCase > 4) break; 

			Room[rnum].SelectWinCase(pnum,  winCase);
		} break;

	case SV_ASK_CHANGESNDFX: // ### [���� �߰� �۾�] ###
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
			if(pnum<0 || pnum>=Room[rnum].get_maxnewplayer()) break;// ### [ ������� ] ###
			if(pnum != User[unum].PNum) break;
			if(sndfx < 0 || sndfx > 10) break;
			// ### [ ������� ] ###
			if(pnum<Room[rnum].get_maxplayer())	{ 
				Room[rnum].Ps[pnum].nSndFxKind = sndfx;
			}
			// ��ο��� �˸���.
			Room[rnum].SendMsgToAll(MsgData.pData, MsgData.GetTotalSize());

		} break;

	////////////////////////////////////////////
	/////////// ### [ ������� ] ###   /////////
	////////////////////////////////////////////
	// ��� -> ������
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

		// ### [ ������� ] ###
		// ������ -> ���
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
	//////    ### [������ ��� �۾�] ###  //////
	////////////////////////////////////////////
	case NM_ADMIN_OUTUSER: // ����� �����ϱ�(���)
		{
			CNM_ADMIN_OUTUSER MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = pUserVal->iUser2;
			if(unum != MsgData.pAMESG->AdminUNum) 
			{	// ���̵� ������ ���� ������ �����Ŵ
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid!=TRUE) break;
			if(User[unum].bAdminUser!=TRUE) break; 
			if(faststrcmp(MsgData.pAMESG->AdminID, User[unum].UI.ID) != 0) 
			{	// ���̵� ������ ���� ������ �����Ŵ
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
			strMess.Format("�����ڿ� ���� �������� ������ ����˴ϴ�.");
			smsg.Set(0, (char*)strMess.operator LPCTSTR()); 
			pSocket->Send(smsg.pData, smsg.GetTotalSize());
			SockMan.ReservClose(usersid); // ���� ���� ����
		} break;

	case NM_ADMIN_DELROOM:
		{
			CNM_ADMIN_DELROOM MsgData;
			MsgData.Get(lpdata, totsize);

			int unum = pUserVal->iUser2;
			if(unum != MsgData.pAMESG->AdminUNum) 
			{	// ���̵� ������ ���� ������ �����Ŵ
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid!=TRUE) break;
			if(User[unum].bAdminUser!=TRUE) break; 
			if(faststrcmp(MsgData.pAMESG->AdminID, User[unum].UI.ID) != 0) 
			{	// ���̵� ������ ���� ������ �����Ŵ
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}

			int rnum = MsgData.pAMESG->TarUNum; //���ȣ
			if(rnum<1 || rnum>=MAX_ROOM) break;
			if(faststrcmp(Room[rnum].Ri.ID, MsgData.pAMESG->TarID) != 0) break;

			Room[rnum].Admin_Room_Remove();
		} break;
			//�Ӹ� �ϱ�
	case NM_ADMIN_WHISPERCHAT:
		{
			CNM_ADMIN_WHISPERCHAT MsgData;
			MsgData.Get(lpdata, totsize);
			//(�߰�)
			// ���̵� �ùٸ��� �˻�
			int unum = pUserVal->iUser2;
			if(unum < 1 || unum >= MAX_USER) break;
			if(User[unum].bValid == FALSE) break;
			if(User[unum].bAdminUser!=TRUE) break; 
			if(faststrcmp(User[unum].UI.ID, MsgData.ID) != 0) {
				// ���̵� ������ ���� ������ �����Ŵ
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
				str.Format("[%s]���� �Ӹ� �ź����Դϴ�. [����]", pdestid);
				smsg.Set(0, (char*)(LPCTSTR)str);
				SockMan.SendData(sid, smsg.pData, smsg.GetTotalSize());
				//����
			}

			SockMan.SendData(User[destunum].Sid, MsgData.pData, MsgData.GetTotalSize());
		} break;

	case NM_ADMIN_CHANGEROOM: // [������ ��� �۾�] �׹� �ɼ� �����û(���)
		{
			CNM_ADMIN_CHANGEROOM MsgData;
			MsgData.Get(lpdata, totsize);
			int unum = pUserVal->iUser2;
			if(unum != MsgData.pAMESG->AdminUNum) 
			{	// ���̵� ������ ���� ������ �����Ŵ
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid!=TRUE) break;
			if(User[unum].bAdminUser!=TRUE) break; 
			if(faststrcmp(MsgData.pAMESG->AdminID, User[unum].UI.ID) != 0) 
			{	// ���̵� ������ ���� ������ �����Ŵ
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
			{	// ���̵� ������ ���� ������ �����Ŵ
				SockMan.GetDataSocketPtr(sid)->PostClose();
				break;
			}
			if(unum<1 || unum>=MAX_USER) break;
			if(User[unum].bValid!=TRUE) break;
			if(User[unum].bAdminUser!=TRUE) break; 
			if(faststrcmp(MsgData.pAMESG->AdminID, User[unum].UI.ID) != 0) 
			{	// ���̵� ������ ���� ������ �����Ŵ
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
			//strMess.Format("�����ڿ� ���� �������� ������ ����˴ϴ�.");
			//smsg.Set(0, (char*)strMess.operator LPCTSTR()); 
			//pSocket->Send(smsg.pData, smsg.GetTotalSize());
			//SockMan.ReservClose(usersid); // ���� ���� ����
		
		} break;
	}
	return TRUE;
}


BOOL CChannel::CheckGameLevel(INT64 PMoney, int nIcon, int nStyle,int nLevel,int &nError)
{
	// ### [ ä�� ���� ���� ] ###
	
	nError = 0;

	switch(ChannelStyle){
	case 0: // ����ä�� 
		{
			return TRUE;
		} break;
		
	case 1:// �ʺ�����
		{
			if(nIcon >= 0 && nIcon <= 3) return TRUE;
			// ���� 
			if(nIcon == 4 && PMoney <= 5000000000) return TRUE; // 50��
			
		} break;			
	case 2: // �߼����� 
		{
			if(nIcon >= 4) return TRUE;
		} break;
	case 3: // �������
		{
			if(nIcon >= 8) return TRUE;			
		} break;
	case 4: // ���ż��� ���� ó��(���尡��)
		{
			if(nIcon >= 12) return TRUE;			
		} break;
	}
/*
	switch(ChannelStyle){
	case 0: // ����ä�� 
		{
			return TRUE;
		} break;
		
	case 1:// �ʺ�����
		{
			if(nIcon >= 0 && nIcon <= 3) return TRUE;
			// ���� 
			if(nIcon == 4 && PMoney <= 5000000000) return TRUE; // 50��
			
		} break;			
	case 2: // �߼����� 
		{
			if(nIcon >= 4 && nIcon <= 7) return TRUE;
			// ���� 
			if(nIcon == 8 && PMoney <= 150000000000) return TRUE; // 1500��
			
		} break;
	case 3: // �������
		{
			if(nIcon >= 8 && nIcon <= 11) return TRUE;
			// ���� 
			if(nIcon == 12 && PMoney <= 3000000000000) return TRUE; //3��
			// ���� �߰�(���尡��) ������ ��� �������� ���尡�� 2003 0625
			if(nIcon >= 12) return TRUE;
			
		} break;
	case 4: // ���ż��� ���� ó��(���尡��)
		{
			if(nIcon >= 12 && nIcon <= 14) return TRUE;
			
		} break;
		
	default : // ��������
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
		case 0: // ����ä��
			nRaise     =  0; // 1ȸ������
			break; 
		case 1: // �ʺ�ä��
			nRaise     =  1; // 1ȸ������
			break; 
		case 2: // �߼� ä��
			nRaise     =  2; // 1ȸ������
			break; 
		case 3: // ���ä��
			nRaise     =  3; // 2ȸ������
			break; 
		case 4: // ����ä��
			nRaise     =  4; // 2ȸ������, ����3ȸ
			break; 
		}
	}
	else {
		switch(ChannelStyle){
		case 0: // ����ä��
			nRaise     =  0; // 1ȸ������
			break; 
		case 1: // �ʺ�ä��
			nRaise     =  1; // 1ȸ������
			break; 
		case 2: // �߼� ä��
			nRaise     =  2; // 1ȸ������
			break; 
		case 3: // ���ä��
			nRaise     =  3; // 2ȸ������
			break; 
		case 4: // ����ä��
			nRaise     =  4; // 2ȸ������, ����3ȸ
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
		case 0: // ����ä��
			BetStyle     =  1; // ����
			break; 
		case 1: // �ʺ�ä��
			BetStyle     =  1; // ����		
			break; 
		case 2: // �߼� ä��
			BetStyle     =  1; // ����		
			break; 
		case 3: // ���ä��
			BetStyle     =  1; // ����
			break; 
		case 4: // ����ä��
			BetStyle     =  1; // ����
			break; 
		}
	} else {
		switch(ChannelStyle){
		case 0: // ����ä��
			BetStyle     =  0; // ����
			break; 
		case 1: // �ʺ�ä��
			BetStyle     =  0; // ����
			break; 
		case 2: // �߼� ä��
			BetStyle     =  0; // ����
			break; 
		case 3: // ���ä��
			BetStyle     =  1; // ����
			break; 
		case 4: // ����ä��
			BetStyle     =  1; // ����
			break; 
		}
	}
	return BetStyle;
}

BOOL CChannel::CheckGameMoney(INT64 Money)
{
	switch(ChannelStyle){
	case 1: //�ʺ�ä�� ##�Ӵ�����: 0 ~ g_POKERSET.nHM*2)
		{
			if(Money >= g_pMainView->Cfg3.nCM) {// 100 ��
				return FALSE;
			}
		} break;
	case 2: // �߼�ä�� ##�Ӵ�����: g_POKERSET.nHM ~ g_POKERSET.nFM*20)
		{
			if(Money >= g_pMainView->Cfg3.nHM){ // 5000 ��
				return FALSE;
			}
		} break;
	case 3: // ���ä�� ##�Ӵ�����: g_POKERSET.nFM ~ g_POKERSET.nGM*50)
		{
			if(Money >= g_pMainView->Cfg3.nFM){ // 100��
				return FALSE;
			}

		} break;
	case 4: // ����ä�� ##�Ӵ�����: g_POKERSET.nGM �̻�)
		{
			if(Money >= g_pMainView->Cfg3.nGM){ // 1��
					return FALSE;
			}

		} break;
	default: // ����ä��  ##�Ӵ�����: X
		{
			return TRUE;
		
		} break;
	}
	return TRUE;
}


