// DBThread.h: interface for the CDBThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBTHREAD_H__E5A49F86_7CD0_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_DBTHREAD_H__E5A49F86_7CD0_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include <mmsystem.h>
#include "UserDB.h"
#include "GameDB.h"
#include "BadUserDB.h"
#include "StatusDB.h"
#include "AvatarDB.h"

#include "ItemDB.h"		// [게임 아이템 작업]
#include "PremiumDB.h"	// [프리미엄 작업]
#include "PremIPDB.h"	// [PC방 작업]
#include "EventPrizeDB.h" // ### [이벤트 티켓] ###


#include "UserStruct.h"

// [SSO 작업] - 아래 한줄 삭제
#define MAX_DBWORK 2700//3100//2100					// 최대 DB 큐 작업 개수(최대 유저수의 1.5배정도)
#define UM_DBWORK_LOGIN (WM_USER+1201)					// 유저 로그인 사용자 메시지
#define UM_DBWORK_LOGOUT (WM_USER+1202)					// 유저 로그아웃 사용자 메시지
#define UM_DBWORK_DCON  (WM_USER+1203)					// 중복접속 중이다
#define UM_DBWORK_SETDB (WM_USER+1204)					// 유저 중간값 기록 완료 메시지
#define UM_DBWORK_GAMEITEMLIST	(WM_USER+1209)			// 아이템 목록 갱신 결과 메시지	[게임 아이템 작업]
#define UM_DBWORK_USEITEMRESULT (WM_USER+1210)			// 아이템 사용 결과 메시지		[게임 아이템 작업]
#define UM_DBWORK_EVENTSET (WM_USER+1220)				// ### [이벤트 티켓] ### 이벤트 기록(중간기록)




typedef struct
{
	BOOL			bValid;					// 유효한 상태인가?
	int				WorkKind;				// 처리할 일의 종류(1:로그인 가능여부 처리 2:로그인 기록 처리 3:게임기록 처리 4:로그아웃 처리)
	int				SID;					// 소켓 번호
	UINT			StartTick;				// DB 처리 시작 시간
	UINT			UniqueTick;				// 처리상의 유니크를 부여하기위한 타임 Tick
	USERINFO		UI;						// 처리할 사용자 정보
	ORGINFO			OI;						// 백업용 사용자 정보
	INT64			LostMoney;				// [수호천사] 2004.07.08 마지막 읽은 금액
	char			RegID[16];				// 주민등록 번호 [게임 아이템 작업]
	char			UniqNo[16];				// 유니크 넘버
	char			UserIP[24];				// 사용자의 접속 IP정보		[PC방 작업]
	char			Passwd[16];				// 패스워드(암호화되어 커짐)	[SSO 작업] - 추가

	int				ConnectTime;			// 접속 시간(분 단위)
	// 아래의 변수들은 DB처리 결과로 세팅됨
	BOOL			bIsWrongPwd;			// 패스워드 틀림 여부
	BOOL			bIsWrongUniqNo;			// 유니크 넘버의 틀림 여부	[SSO 작업] - 추가
	BOOL			bIsNotUserID;			// 회원 아이디가 존재하지 않는가?
	int				IDState;				// 아이디 정상 사용가능 여부 플래그(0:정상 1:정지중 2:삭제예정)
	BOOL			bAnotherConnect;	// 다른 게임에 이미 로그인 중인가?
	// [게임 아이템 작업]
	GAMEITEM_LIST	Item;					// 아이템 리스트
	int				bErrorItemWork;			// 아이템 작업 결과 코드(아이템 사용 혹은 아이템 리스트 갱신)
	GAMEITEM		UseItem;				// 사용할 아이템
	BOOL			bCertificate;			// [성인인증]
	EVENT_PRIZE     EventPrize;				// ### [이벤트 티켓] ###
	int				bForceLogin;		//2004.05.07 리더스총액 제한    <--------- 추가

} DBWORKS;

//////////////////////////////////////////////////////////////////////
// DB큐 클래스
//////////////////////////////////////////////////////////////////////
// [SSO 작업] - 아래 클래스를 이 코드로 대체
class CDBQue  
{
public:
	CCriticalSection DBQueLock;

	list<DBWORKS*> Work;

	CDBQue();
	virtual ~CDBQue();

	BOOL EnQue(DBWORKS *pNewWork);
	BOOL DeQue(DBWORKS *pWork);
	int GetTotNum();
};
/*
class CDBQue  
{
public:
	CCriticalSection DBQueLock;
	int SPos, EPos;
	int TotNum;
	DBWORKS Work[MAX_DBWORK];

	CDBQue();
	virtual ~CDBQue();

	BOOL EnQue(DBWORKS *pNewWork);
	BOOL DeQue(DBWORKS *pWork);
	int GetTotNum();
};
*/
//////////////////////////////////////////////////////////////////////
// DB 처리 쓰레드 클래스
//////////////////////////////////////////////////////////////////////

class CDBThread
{
public:
	
	CWinThread *pThread;	// 쓰레드 포인터
	CDBQue *pDBQue;			// DB큐 포인터

	BOOL bNowTerminate;		// 쓰레드 종료 플래그
	BOOL bWorking;			// DB처리 중인가?
	int ErrorCnt;			// DB에러 카운트
	DBWORKS Work;			// 처리할 DB

	// Database 클래스
	CDatabase*		pDatabase_User;
	CDatabase*		pDatabase_Game;
	CDatabase*		pDatabase_Status;
	CDatabase*		pDatabase_Avatar;
	CDatabase*		pDatabase_Item;		// [게임 아이템 작업]
	CDatabase*		pDatabase_Premium;	// [프리미엄 작업]
	CDatabase*		pDatabase_PremIP;	// [PC방 작업]
	CDatabase*      pDatabase_EventPrize;    // ### [이벤트 티켓] ###
	

	// DB 레코드셋 클래스 포인터
	CUserDB*		m_pUserDB;
	CGameDB*		m_pGameDB;
	CStatusDB*		m_pStatusDB;
	CAvatarDB*		m_pAvatarDB;
	CItemDB*		m_pItemDB;		// [게임 아이템 작업]
	CPremiumDB*		m_pPremiumDB;	// [프리미엄 작업]
	CPremIPDB*		m_pPremIPDB;	// [PC방 작업]
	CEventPrizeDB*  m_pEventPrizeDB;		// ### [이벤트 티켓] ###
	
	CDBThread();
	virtual ~CDBThread();

	// 쓰레드
	BOOL Begin(CDBQue *pQue);
	BOOL Terminate();

	// 레코드셋 생성 함수
	BOOL CreateUserDB();
	BOOL CreateGameDB();
	BOOL CreateStatusDB();
	BOOL CreateAvatarDB();
	BOOL CreateItemDB();	// [게임 아이템 작업]
	BOOL CreatePremiumDB();	// [프리미엄 작업]
	BOOL CreatePremIPDB();	// [PC방 작업]
	BOOL CreateEventPrizeDB();// ### [이벤트 티켓] ###


	// 모든 레코드셋을 삭제함
	void DeleteAllDB();

	// DB작업 처리함수
	void ProcessDBWork();

	// 사용자 정보 읽기 및 저장 함수
	// ### [이벤트 티켓] ###
	BOOL SetUserInfo(DBWORKS *pWork);// [SSO 작업] - 수정
	// 게임 DB 유저 추가 함수
	BOOL AddNewGameDB(USERINFO *pUI);
	BOOL GetUserInfo(DBWORKS *pWork);			// [게임 아이템 작업]


	BOOL GetMainUserInfo_FromDB(DBWORKS *pWork);	// [SSO 작업] - 추가
	BOOL GetGameInfo(DBWORKS *pWork);				// [SSO 작업] - 추가
	BOOL UpdateConnectStatus();						// [SSO 작업] - 추가
	
	// 게임 아이템 읽기 및 쓰기 함수	[게임 아이템 작업]
	BOOL GetItemList(char *id, GAMEITEM_LIST *pItemList);
	BOOL UseGameItem(char *id, GAMEITEM *pItem);
	// 프리미엄 정보 읽기				[프리미엄 작업]
	BOOL GetPremiumInfo(LPCTSTR uniqno, USERINFO* pUI);
	// 프리미엄 IP 정보 읽기			[PC방 작업] [SSO 작업]
	BOOL GetPremiumIP(DBWORKS *pWork);
	
	// 이벤트 정보 읽기					 ### [이벤트 티켓] ###
	BOOL GetEventPrize(DBWORKS *pWork);
	BOOL AddNewEventPrizeDB(USERINFO *pUI);
	BOOL SetEventPrizeDB(char *id, EVENT_PRIZE *pEvent, BOOL bInit = FALSE);
	CString GetLogFileName();

	// DB 쓰레드 펑션
	static UINT DBThreadFunc(LPVOID pParam);
};

// for static function
static UINT CDBThread::DBThreadFunc(LPVOID pParam);


#endif // !defined(AFX_DBTHREAD_H__E5A49F86_7CD0_11D4_97A4_0050BF0FBE67__INCLUDED_)
