// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "62CutPokerServerView.h"
#include "User.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUser::CUser()
{
	Clear();
}

CUser::~CUser()
{

}

void CUser::Clear()
{
	bValid = FALSE;
	ZeroMemory(&UI, sizeof(USERINFO));
	ZeroMemory(&SUI, sizeof(SMALLUSERINFO));
	ZeroMemory(&OI, sizeof(ORGINFO));
	ZeroMemory(&GameItem, sizeof(GAMEITEM_LIST)); // [게임 아이템 작업]
	ZeroMemory(UserIP, sizeof(UserIP)); // 사용자의 접속 IP정보 ### [로그 기록용] ###
	memset(RegID, 0, sizeof(RegID));
	memset(UniqNo, 0, sizeof(UniqNo));
	
	ZeroMemory(&EventPrize, sizeof(EVENT_PRIZE)); // ### [이벤트 티켓] ###

	// ### [어뷰저 방지] ###
	// ### [ 중복된 IP가 ] ###
	memset(&PeerIP, 0, sizeof(in_addr));

	// [ 통계기록 ]
	m_UserLevel = 0;		// 로그인 레벨
	m_StartTime.SetDateTime(0,0,0,0,0,0);

	
	Sid = 0;
	ChNum = -1;
	Cid = -1;
	PNum = -1;

	m_LostMoney = 0; // [수호천사] 2004.07.08
	m_PrevMoney = 0; // [수호천사] 2004.07.08

	bSendAcceptLogin = FALSE;						// [게임 아이템 작업]

	bLogOutDBSaved = FALSE;
	bLobyOpen = FALSE;
	bExitSubscript = FALSE;
	bNoWhisper = FALSE;

	ConnectSec = 0;
	nBet          = 0;
	UniqueTick    = 0;
	bIsUpdateData = FALSE;

	//### [관리자 모드 작업] ###
	bAdminUser        = FALSE; 
	// ### [ 관전기능 ] ###
	bObserver = FALSE;

	// [ 통계기록 ]
	m_UserLevel = 0;		// 로그인 레벨
	m_StartTime.SetDateTime(0,0,0,0,0,0);

}

// [게임 아이템 작업]
// ### [이벤트 티켓] ###
void CUser::SetNewUser(USERINFO *pUI, GAMEITEM_LIST *pItemList, int sid, EVENT_PRIZE *pEvent)
{
	Clear();

	bValid = TRUE;
	memcpy(&UI, pUI, sizeof(USERINFO));						// 유저 정보 저장
	memcpy(&GameItem, pItemList, sizeof(GAMEITEM_LIST));	// 게임 아이템 정보 저장	[게임 아이템 작업]


	// 초기 유저 정보 저장
	OI.WinNum = UI.WinNum;
	OI.LooseNum = UI.LooseNum;
	OI.DrawNum = UI.DrawNum;
	OI.pBackup = UI.PMoney;

	Sid = sid;							// 소켓 번호 저장
					
	// 새로운 접속자 메세지 정보 준비
	SUI.UNum = UI.UNum;					// 유저번호
	strncpy(SUI.ID, UI.ID, 15);			// 아이디
	SUI.Sex = UI.Sex;					// 성별
	SUI.nIcon    = UI.nIcon;			// 아이콘
	SUI.Position = WH_CHAN;				// 현재 위치(채널)
	SUI.RoomNum = 0;					// 현재 위치한 방번호(방에 있을때만 유효)

	// ### [이벤트 티켓] ###
	memcpy(&EventPrize, pEvent, sizeof(EVENT_PRIZE));

	//### [관리자 모드 작업] ###
	if(UI.cAdminUser  == 1)	{
		bAdminUser = TRUE;
		SUI.cAdminUser = 1;
	}


		// [ 통계기록 ]
	m_UserLevel = UI.nIcon;				// 로그인 레벨
	m_StartTime  = COleDateTime::GetCurrentTime();  // 로그인시 시간
	if(g_nAgentIndex != -1)				// 에이전트는 제외 (포커류 사용) 포커와 다르다면 if문만 주석처리 할것
		g_pMainView->m_cSALog.Make_Log(LEVEL_CONCURRENT_USER, UI.UNum,  TRUE);// 동접,..


//	// [ 통계기록 ]
//	m_UserLevel = UI.nIcon;				// 로그인 레벨
//	m_StartTime  = COleDateTime::GetCurrentTime();  // 로그인시 시간
//	if(g_nAgentIndex != -1)				// 에이전트는 제외 (포커류 사용)
//		g_pMainView->Make_StatisticsLog(LEVEL_CONCURRENT_USER, UI.UNum,  TRUE);// 동접,..

	UniqueTick = ::timeGetTime();
}

// [게임 아이템 작업]
BOOL CUser::SendAcceptLoginMsg()
{
	if(!bValid) return FALSE;

	// 메시지를 보낸적이 있다면 취소
	if(bSendAcceptLogin) return FALSE;

	// 로그인 허가 메시지 날림
	LOGININFO Li;
	ZeroMemory(&Li, sizeof(Li));
	memcpy(&Li.UI, &UI, sizeof(USERINFO));

	// [게임 아이템 작업]
//	strncpy(Li.RegID, RegID, 15);	// 주민등록 번호
	strncpy(Li.UniqNo, UniqNo, 15);	// 유니크 넘버
	
	// 로그인 허용 메시지를 보냄
	CSV_ACCEPT_LOGIN msg(SndBuf);
	msg.Set(&Li);
	SendMsg(&msg);

	// 로그인 공지가 있다면 보내줌(메시지 박스)
	if(g_pMainView->m_ServerLoginMsg.GetLength() > 2) {
		CSV_SERVERMSG smsg(SndBuf);
		smsg.Set(1, (char*)((LPCTSTR)g_pMainView->m_ServerLoginMsg));
		SendMsg(&smsg);
	}

	if(strlen(g_pMainView->Cfg2.TitleNotice) > 2) {
		CSV_SERVERNOTIC nmsg(SndBuf); // 타이틀 공지
		nmsg.Set(0, (char*)((LPCTSTR)g_pMainView->Cfg2.TitleNotice) );
		SendMsg(&nmsg);
	}

	bSendAcceptLogin = TRUE;

	return TRUE;
}


BOOL CUser::MakeDBWork(int workkind, DBWORKS *pWork)
{
	if(!pWork) return FALSE;
	ZeroMemory(pWork, sizeof(DBWORKS));

	switch(workkind)
	{
	case 3: // 중간값 기록을 위한 데이타베이스 워크값 세팅
		{
			pWork->bValid    = TRUE;					// 유효 플래그
			pWork->WorkKind = 3;						// 일의 종류
			pWork->SID      = Sid;						// 소켓 SID
			pWork->StartTick   = ::timeGetTime();       // Time Tick
			pWork->ConnectTime = max(0, ConnectSec);	// 게임 접속 시간
			memcpy(&pWork->UI, &UI, sizeof(USERINFO));
			bIsUpdateData = TRUE;
		} break;
		
	case 4: // 로그아웃시 기록할 데이타 베이용 데이타
		{
			pWork->bValid = TRUE;						// 유효 플래그
			pWork->WorkKind = 4;						// 일의 종류
			pWork->SID = Sid;							// 소켓 SID
			pWork->StartTick = ::timeGetTime();			// Time Tick
			pWork->ConnectTime = max(0, ConnectSec);	// 게임 접속 시간
			memcpy(&pWork->UI, &UI, sizeof(USERINFO));
			memcpy(&pWork->OI, &OI, sizeof(ORGINFO));
			// ### [이벤트 티켓] ###
			memcpy(&pWork->EventPrize, &EventPrize, sizeof(EVENT_PRIZE));
		// [ 통계기록 ]
		//	g_pMainView->Make_StatisticsLog(LEVEL_CONCURRENT_USER, UI.UNum,  FALSE); // 동접, 올인사용자, 이용시간
			// [ 통계기록 ]
			g_pMainView->m_cSALog.Make_Log(LEVEL_CONCURRENT_USER, UI.UNum,  FALSE); // 동접, 올인사용자, 이용시간

			pWork->LostMoney  = m_LostMoney;			// [수호천사] 2004.07.08

		} break;

	case 5:
		{	// ### [이벤트 티켓] ###
			pWork->bValid = TRUE;						// 유효 플래그
			pWork->WorkKind = 5;						// 일의 종류
			pWork->SID = Sid;							// 소켓 SID
			pWork->StartTick = ::timeGetTime();			// Time Tick
			pWork->ConnectTime = max(0, ConnectSec);	// 게임 접속 시간
			memcpy(&pWork->UI, &UI, sizeof(USERINFO));
			memcpy(&pWork->EventPrize, &EventPrize, sizeof(EVENT_PRIZE));
		} break;

	case 9: // 게임 아이템 리스트 갱신 DB 작업	[게임 아이템 작업]
	case 10: // 게임 아이템 사용 DB 작업	[게임 아이템 작업]
		{
			pWork->bValid    = TRUE;					// 유효 플래그
			pWork->WorkKind  = workkind;				// 일의 종류
			pWork->SID       = Sid;						// 소켓 SID
			pWork->StartTick   = ::timeGetTime();		// Time Tick
			memcpy(&pWork->UI, &UI, sizeof(USERINFO));
		}
		break;
		/*	
		
//	case 5: // 로그아웃시 기록할 데이타 베이용 데이타(메인유저기록은 하지않는다. 접속 코드만 초기화)
//	case 6: // 수시로 중복접속여부 확인(현재는 1분에 한번)
	case 10: // [게임 아이템 작업] 추가된 게임아이템 작업
	case 11: // [비공개방 아이템] 추가된 게임아이템 작업
	case 12: // [점프 아이템]
		pWork->bValid = TRUE;						// 유효 플래그
		pWork->WorkKind = workkind;						// 일의 종류
		pWork->SID = Sid;							// 소켓 SID
		pWork->StartTick = ::timeGetTime();			// Time Tick
		pWork->ConnectTime = max(0, ConnectSec);	// 게임 접속 시간
//		pWork->PlusMarble = max(0, PlusMarble);		// 획득 마블
//		pWork->MinusMarble = max(0, MinusMarble);	// 잃은 마블
		memcpy(&pWork->UI, &UI, sizeof(USERINFO));
		memcpy(&pWork->OI, &OI, sizeof(USERINFO));
		break;
*/
	default : 
		return FALSE;
	}
	return TRUE;
}

// 게임 가감치가 동일하다면 굳이 데이타베이스에 게임결과값을 기록할 필요 없다
BOOL CUser::IsSetDBWork()
{
	return FALSE;
	if(bValid==FALSE) return FALSE;
	if(ConnectSec<30) return FALSE; // 30초미만자는 처리하지 않는다
	if(memcmp(&UI,&OI,sizeof(USERINFO))==0) return FALSE;
	return TRUE;
}

// [게임 아이템 작업]
BOOL CUser::SendMsg(CCommMsg *pMsg)
{
	if(Sid < 0 || Sid >= MAX_DATA_SOCKET) return FALSE;
	return SockMan.SendData(Sid, pMsg->pData, pMsg->GetTotalSize());
}