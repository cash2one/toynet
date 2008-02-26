// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__1494A741_1F9D_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_USER_H__1494A741_1F9D_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "DBThread.h"
#include "CommMsg.h"

#define WH_CHAN	0
#define WH_LOBY	1
#define WH_SHOP	2
#define WH_GAME	3

class CUser  
{

private:
	INT64			m_LostMoney; // [수호천사] 2004.07.08
	INT64			m_PrevMoney; // [수호천사] 2004.07.08
public:
	inline void  Set_init_premoney()  { m_PrevMoney = UI.PMoney; }  // [수호천사] 2004.07.08 ~~
	inline void  Set_init_lost_money(INT64 lmoney) { m_LostMoney = lmoney; }
	inline INT64 Get_lost_money() const { return m_LostMoney; }
	inline void  Set_refresh_lost_money() { m_LostMoney = m_PrevMoney - UI.PMoney; } //call:  DoGameOver, FoldUser

public:
	BOOL bValid;			// 유효 플래그
	USERINFO UI;			// 사용자 정보
	SMALLUSERINFO SUI;		// 작은 사용자 정보
	ORGINFO OI;			// 로그인 시의 오리지널 정보
	int Sid;				// 소켓 배열 ID
	int ChNum;				// 현재 채널 번호(-1인경우 아직 채널에 입장하지 않았음)
	int Cid;				// 채널 유저 번호(채널에 입장한 경우만 유효)
	int PNum;				// 플레이어 번호(방에 입장한 경우만 유효)	

	char			RegID[16];			// 주민등록 번호 [게임 아이템 작업]
	char			UniqNo[16];			// 유니크 넘버
	char			UserIP[24];			// 사용자의 접속 IP정보	### [로그 기록용] ###	

	// 어뷰져 방지(같은 아이피 클래스 같은방 입장 불가)
	// ### [어뷰저 방지] ###
	// ### [ 중복된 IP가 ] ###
	in_addr			PeerIP;

	BOOL bSendAcceptLogin;	// 로그인 허용 메시지를 보냈는가?		[게임 아이템 작업]
	BOOL bLogOutDBSaved;	// 로그아웃 DB가 처리요청되었는가?
	BOOL bLobyOpen;			// 대기실 초대창을 켜둔 상태인가?
	BOOL bExitSubscript;	// 나가기 예약 중인가?
	BOOL bNoWhisper;		// 귓말 거부중인가?
	UINT ConnectSec;		// 접속 시간(초)
	
	INT64 nBet;             // 포커머니 게임배팅액
	UINT  UniqueTick;        // 게임기록이 완료된 시점에서 비교하기위한 아이디
	BOOL  bIsUpdateData;     // 게임데이타베이스에 기록중이다  

	GAMEITEM_LIST GameItem; // [게임 아이템 작업]
	EVENT_PRIZE EventPrize; // ### [이벤트 티켓] ###

	BOOL bAdminUser;        //### [관리자 모드 작업] ###
	BOOL bObserver;			// ### [ 관전기능 ] ###

	// [ 통계기록 ]
	int		m_UserLevel;		// 로그인시 레벨
	COleDateTime	m_StartTime;		// 시작 시간

	INT64	nBMoney;			// 은행 돈

public:
	CUser();
	virtual ~CUser();
	void Clear();
	
	// ### [이벤트 티켓] ###
	void SetNewUser(USERINFO *pUI, GAMEITEM_LIST *pItemList, int sid, EVENT_PRIZE *pEvent);	// [게임 아이템 작업]
	BOOL SendAcceptLoginMsg();											// [게임 아이템 작업]

	BOOL MakeDBWork(int workkind, DBWORKS *pWork);
	BOOL IsSetDBWork();

	BOOL SendMsg(CCommMsg *pMsg);

};

#endif // !defined(AFX_USER_H__1494A741_1F9D_11D4_97A5_0050BF0FBE67__INCLUDED_)
