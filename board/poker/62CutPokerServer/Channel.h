// Channel.h: interface for the CChannel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNEL_H__52EB9D79_BBF5_4ECE_910A_2438BE9AAB71__INCLUDED_)
#define AFX_CHANNEL_H__52EB9D79_BBF5_4ECE_910A_2438BE9AAB71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SockMan.h"
#include "Room.h"

#define MAX_SUPERROOM	30		// 최대 슈퍼방 개수	[게임 아이템 작업]
#define MAX_ROOM		70		// 최대 게임룸의 개수(default: 60개)
#define MAX_CHUSER 200	// 채널 최대 접속가능 유저수(수정시 62CutPokerServerView.h의 MAX_USER를 수정해야함)

// [세븐포커] 루틴
#define M_100     (1000000) //  백만원
#define M_500     (5000000) // 5백만원
#define M_1000    (10000000) //  천만원
#define M_3000    (30000000) // 3천만원
#define M_5000    (50000000) // 5천만원   // 초기치
#define M_1Y      (100000000) //   1억원
#define M_3Y      (300000000) //   3억원
#define M_5Y      (500000000) //   5억원
#define M_10Y     (1000000000) //  10억원
#define M_100Y    (10000000000) //  100억원				금액제한 초보
#define M_1000Y   (100000000000) //  1000억원			금액제한 초보
#define M_5000Y   (500000000000) //  5000억원			금액제한 중수
#define M_1B	  (1000000000000) //  1조 (billion) 금액제한 고수
#define M_10B	  (10000000000000) //  10조 (billion) 금액제한 고수
#define M_100B	  (100000000000000) //  100조 (billion) 금액제한 고수
#define M_1000B	  (1000000000000000) //  1000조 (billion) 금액제한 고수
#define M_1H      (10000000000000000)   //   1경	(hard money)  금액제한 도신
#define M_10H 	  (100000000000000000) // 10경	금액제한 오버플로우 방지 제한금액



class CChannel  
{
public:
	BOOL CheckGameMoney(INT64 Money);
	int ChNum;				// 채널 번호
	int TotUser;			// 채널 이용자 수
	
	int ChannelStyle; // 채널 스타일
	int m_channel_usernum;

	// 데이터 센드용 임시 버퍼
	// 멀티 쓰레드 채널로 구현할 경우 static을 사용하지 말것
//	static char SndBuf[SENDQUESIZE];

	CRoom Room[MAX_ROOM];	// 룸 클래스
	int Cid[MAX_CHUSER];	// 채널 유저 ID 인덱스
	
	CString strChanName;		// 채널 이름
	INT64	DefaultJackPot;		// 레벨별 기본 잭팟 머니
	int     JackPotSaveRatio;	// 적립 비율
//	int     JackPotPrizeRatio;		// 채널별 잭팟 상금 비율(로티플)
		
//	INT64   ChannelMoney; // 한도금액
	int m_betmoney_5user;
	int m_betmoney_6user;


	CChannel();
	virtual ~CChannel();

	void Init(const int cnum);

	const CHANNELINFO GetChannelInfo();
	void SendMsgToAll(char *pMsgData, int nMsgSize);
	void SendMsgToLoby(char *pMsgData, int nMsgSize);
	void SendMsgToLobyOpen(char *pMsgData, int nMsgSize);

	int OnNewUser(int unum, int &nUseItem); // [점프 아이템# 작업]
	BOOL OnOutUser(int unum);
	void OnGlobalGameTimer();

	BOOL ProcessPacket(int sid, USERVAL *pUserVal, char* lpdata, int totsize, int signal);
	
	// [세븐포커]루틴 
	BOOL CheckGameLevel(INT64 PMoney,int nIcon, int nStyle,int nLevel,int &nError);

	char GetBetStyle(int bPlayer);
	char GetRaiseNum(int bPlayer);
};


#endif // !defined(AFX_CHANNEL_H__52EB9D79_BBF5_4ECE_910A_2438BE9AAB71__INCLUDED_)
