// 62CutPokerServerView.h : interface of the C62CutPokerServerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TETRISSERVERVIEW_H__8D85588A_B8D2_4EBF_ACC2_BAA9322B72F3__INCLUDED_)
#define AFX_TETRISSERVERVIEW_H__8D85588A_B8D2_4EBF_ACC2_BAA9322B72F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "62CutPokerServer.h"
#include <afxmt.h>			// 크리티컬 섹션 클래스용
#include "SockMan.h"		// 소켓 메니져
#include "CommMsg.h"		// 통신 메세지 처리 클래스
#include "Channel.h"		// 채널 클래스
#include "User.h"			// 사용자 클래스
#include "Room.h"			// 게임방 클래스
#include "MsgDlg.h"			// 메시지 다이얼로그
#include "LogFile.h"		// 로그파일 기록 클래스
#include "DBMan.h"			// DB 메니져 헤더
#include <direct.h>         // 디렉터리 생성

#include "LogManDBMan.h"    // ### [로그 기록용] ###
#include "NMSTRMAN.h"       // 문자열 클래스
#include "ServerCommy.h"	// 서버 관리 함수 헤더	[PC방 작업]
#include "Global.h"			// 전역 헤더
#include "62CutPokerServerDoc.h"
#include "StatisticsLog.h"  // [ 통계기록 ] <---- 추가

// 게임 버전
#define VERSION 102


#define MAX_CHAN 17//10		// 최대 채널 개수
#define MAX_USER 2501//3001//2001	// 최대 접속자수 (MAX_CHUSER*MAX_CHAN+1)
						// 수정시 FirstInclude.h의 MAX_DATA_SOCKET도 함께 수정할 것
						// Channel.h의 MAX_CHUSER를 함께 수정할 것
						// DBMan.h의 MAX_DBTHREAD를 함께 수정할 것
						// DBThread.h의 MAX_DBWORK를 함께 수정할 것
						// 아래의 MAX_LOGIN_DBWORKNUM 함께 수정할 것

#define MAX_TIMEOUT 1200				// 소켓 리시브 Time Out 20분(1초단위)
#define MAX_DBTHREAD_TIMEOUT 1800000	// DB 쓰레드 강제 종료 타임아웃(30분) = 60000 * 30
//#define MAX_LOGIN_DBWORKNUM 18			// 로그인 허용이 가능한 DB 큐 작업개수 

#define WIN_POINT			20		// 승리 포인트
#define WIN_MARBLE			100		// 승리 마블
#define LOOSE_POINT			10		// 패배 포인트
#define LOOSE_MARBLE		50		// 패배 마블

#define GLOBALGAME_TIMER	1000
#define MSGPOOLING_TIMER	1001
#define CLOSE_TIMER			1002
#define USERCNT_TIMER		1003   // [PC방 작업]

////////////////////////////////////////////////////////////////
#define EXITRESERVE_TIMER	1005		// 자동종료

class C62CutPokerServerView;
extern C62CutPokerServerView *g_pMainView;	// 메인뷰 전역포인터
extern CDBMan DBMan;					// DB 메니저 전역 객체
extern CSockMan SockMan;				// 소켓 메니져 전역 객체
extern CChannel Chan[MAX_CHAN];			// 채널 클래스
extern CUser User[MAX_USER];			// 사용자 정보 클래스
extern CNMStrMan g_StrMan;
extern POKERSET g_POKERSET;			//[세븐포커] 액수제한 설정

extern CLogManDBMan LogDBMan;		// ### [로그 기록용] ###
extern INT64 g_LogRangeValue;

extern INT64 g_JackPotMoney;		// [ ###잭팟### ]
extern int g_ServerGrade;			// 등급

extern CString      g_AppStartPath;	// 현재 디렉터리 저장

extern ADMIN_MANAGER             g_AdminInfo; // [관리자 모드 작업]
extern ADMIN_ACCESSDENY_USER	 g_AccessDeny[MAX_DENY_USER];

// [서버 에이전트 작업]
#define CONNECT_ID   "CTRA_CONNECT"
#define CONNECT_PASS "9669"
extern int  g_nAgentIndex;

extern CAPTURE_INFO       g_CaptureID; // [대화 저장]
extern SERACH_INFO         g_SearchID; // [위치 저장]


typedef struct
{
	char ServerName[20];
	int  GameCode;
	int  ServerCode;
	UINT ServerPort;
	char UserDSN[40];
	char UserTable[40];
	char UserUID[20];
	char UserPass[20];
	char GameDSN[40];
	char GameTable[40];
	char GameUID[20];
	char GamePass[20];
	char BadUserDSN[40];
	char BadUserTable[40];
	char BadUserUID[20];
	char BadUserPass[20];
	char StatusDSN[40];
	char StatusTable[40];
	char StatusUID[20];
	char StatusPass[20];
	char AvatarDSN[40];
	char AvatarTable[40];
	char AvatarUID[20];
	char AvatarPass[20];
	// 언어 코드
	int  nMSCode;
	INT64 MAX_PMONEY;

	char ItemDSN[40];		// [게임 아이템 작업]
	char ItemTable[40];		// [게임 아이템 작업]
	char ItemUID[20];		// [게임 아이템 작업]
	char ItemPass[20];		// [게임 아이템 작업]
	char PremiumDSN[40];	// [프리미엄 작업]
	char PremiumTable[40];	// [프리미엄 작업]
	char PremiumUID[20];	// [프리미엄 작업]
	char PremiumPass[20];	// [프리미엄 작업]
	char PremIPDSN[40];		// [PC방 작업]
	char PremIPTable[40];	// [PC방 작업]
	char PremIPUID[20];		// [PC방 작업]
	char PremIPPass[20];	// [PC방 작업]

	//////////////////       
	char LogRecDSN[40];     //### [로그 기록용] ###
	char LogRecTable[40];
	char LogRecUID[20];
   	char LogRecPass[20];
	BOOL bLogUseDB;

	//////////////////
	char EventDSN[40];     // ### [이벤트 티켓] ###
	char EventTable[40];
	char EventUID[20];
   	char EventPass[20];
	BOOL bEventDB;

	char StatisticsDSN[40];     // [ 통계기록 ]
	char StatisticsTable[40];
	char StatisticsUID[20];
 	char StatisticsPass[20];
	BOOL bStatisticsDB;         // 통계기록용 테이블 사용유무(체크버튼)

		//////////////////////////////////////////
	char PreUserLGMLogDSN[40];     //2004.05.07 리더스총액 제한
	char PreUserLGMLogTable[40];
	char PreUserLGMLogUID[20];
   	char PreUserLGMLogPass[20];
	BOOL bPreUserLGMLogDB;

	char AllinChargeDSN[40];     // [수호천사]
	char AllinChargeTable[40];   // [수호천사]		
	char AllinChargeUID[20];	 // [수호천사]
 	char AllinChargePass[20];	 // [수호천사]
	BOOL bAllinDB;

	char Resreved[120*2];	// 테이블이 늘어날 것에 대비한 예약 바이트 [프리미엄 작업]	[PC방 작업]

} CONFIG;

typedef struct
{
	char NoticeInfoMsg[79];
	char NoticeURL[64];
	int  MaxUser;
	int  MaxDBThread;
	int  LoginDBQueNum;
	BOOL bDBClose;
	char BannerURL1[64];
	char BannerURL2[64];
	char ProfileURL[64];
	char LocationURL[64];	// <- 추가

	char ItemURL[64];		// [게임 아이템 작업]
	char ItemHelpURL[64];	// [게임 아이템 작업]

/////////////////////////   //  //### [로그 기록용] ###
	int  MaxDBThread2;
	int  LoginDBQueNum2;
	BOOL bDBClose2;
////////////////////////
	int	 SameIPLimit;// ### [ 중복된 IP가 ] ###
	BOOL UseMacLimit;	//맥어드레스 검사
	char TitleNotice[1024];
	char LeadersJoinURL[64];//2004.05.07 리더스총액 제한
	char CashURL[64];		// 캐쉬충전 URL 
	char Resreved[64*4];	// URL정보가 늘어날 것에 대비한 예약 바이트 [프리미엄 작업]

} CONFIG2;

typedef struct
{
	int EnterLimitMode;
	BOOL bLoginLimit;
	BOOL bChanLimit;
	int LoginLimitLevel1;
	int LoginLimitLevel2;

	int LoginGrade;
	BOOL bExamServer;

	INT64 nCM; // 초보 제한머니
	INT64 nHM; // 중수 제한머니
	INT64 nFM; // 고수 제한머니
	INT64 nGM; // 도신 제한머니

	int   nHL; // 중수 제한레벨
	int   nFL; // 고수 제한레벨
	int   nGL; // 도신 제한레벨

	CHANINFO_CFG Info[MAX_CHAN];
	char Resreved[64*5];	// URL정보가 늘어날 것에 대비한 예약 바이트 [프리미엄 작업]

} CONFIG3;

// ### [ 잭팟 ] ###
typedef struct
{
int JackPotMode;	  // 잭팟 유무
	INT64 nJackPotMoney;  // 잭팟 머니
	INT64 ChoBo;		  // 기본 잭팟 머니 초보
	INT64 JungSu;		  //	
	INT64 GoSu;			  //
	INT64 God;		      //
	INT64 Free;           //
	// 잭팟 적립 비율
	int   rChoBo;
	int   rJungSu;
	int   rGosu;
	int   rGod;
	int   rFree;
	// 잭팟 상금 비율 로티플
	int   jackPotChobo;
	int   jackPotJungSu;
	int   jackPotGoSu;
	int   jackPotDosin;
	int   jackPotFree;

	// 잭팟 상금 비율 스티플
	int   jackPotChoboS;
	int   jackPotJungSuS;
	int   jackPotGoSuS;
	int   jackPotDosinS;
	int   jackPotFreeS;

	// 잭팟 상금 비율 포카드
	int   jackPotChoboF;
	int   jackPotJungSuF;
	int   jackPotGoSuF;
	int   jackPotDosinF;
	int   jackPotFreeF;

	// ### [로그 기록용] ###
	BOOL bRealLogUse;
	int  Range;
	char Resreved[64*5];	// URL정보가 늘어날 것에 대비한 예약 바이트 [프리미엄 작업]

} CONFIG4;

// ### [이벤트 티켓] ###
typedef struct
{
	BOOL  bRunEvent;
	CTime StartDate;
	CTime EndDate;
	int   EventTicketNum;
	BOOL  bNoEventTime;
	int   NoEventStartHour;
	int   NoEventHour;
	int   MinUserNum;
	char  EventNotice[256];
	char  RoomEventNotice[256];
	BOOL  bAutoEventEnd;
	char Resreved[64*5];	// URL정보가 늘어날 것에 대비한 예약 바이트 [프리미엄 작업]
} CONFIGEVENT;

// 통계기록
typedef struct
{
BOOL	bSaveSetLog;	// [ 통계기록 ]
	
	//2004.05.07 리더스총액 제한
	BOOL    bTotalMoneyLimit;	// 머니 제한 사용유무
	INT64   NormalMoney;		// 일반 사용자
	INT64   LeadersMoney;		// 리더스 
	INT64   LeadersPlusMoney;	// 리더스 플러스

	BOOL    bSafeAngel;	// [수호천사] 2004.07.09
	INT64   MinMoneyLimit;
	INT64   MaxMoneyLimit;

	char    NormalNotice[256];		// 공지 문구
	char    LeadersNotice[256];
	char    LeadersPlusNotice[256];

	char Resreved[64*5];	// 정보가 늘어날 것에 대비한 예약 바이트
} CONFIGSTATIS;

/////////////////////////////////////////////////////

class C62CutPokerServerView : public CFormView
{
public:
	//{{AFX_DATA(C62CutPokerServerView)
	enum { IDD = IDD_62CUTPOKERSERVER_FORM };
	CButton	m_ExitReserve;
	CButton	m_ExitCancel;
	CString	m_ServerMsg;
	CString	m_ServerLoginMsg;
	CString	m_DiscTargetID;
	CString	m_DelRoomNo;
	int		m_iExitRadio;
	CString	m_strMSCode;
	//}}AFX_DATA

// Attributes
public:
	CMsgDlg MsgDlg;			// 메시지 출력용 대화상자
	CONFIG Cfg;				// 환경설정 정보 1 (변경시 서버 종료후 재가동해야 적용됨)
	CONFIG2 Cfg2;			// 환경설정 정보 2 (변경시 실시간으로 적용됨)
	CONFIG3 Cfg3;			// 환경설정 정보 3 (변경시 서버 종료후 재가동해야 적용됨)
	CONFIG4 Cfg4;			// 환경설절 정보 잭팟 머니
	CONFIGEVENT EventCfg;   // ### [이벤트 티켓] ###
	CONFIGSTATIS StatisCfg; // 통계기록


	/////////// 게임 이벤트용 변수 //////////
	// ### [이벤트 티켓] ###
	int			TotEventTicketNum;	// 지금까지 지급된 이벤트 티켓의 개수
	int			NowEventTicketNum;	// 현재 이벤트 티켓을 줄 수 있는 개수
	DWORD		LastEventTick;		// 마지막으로 티켓이 지급된 Time Tick

	BOOL bNowStartUpDlg;	// 서버 스타트업 대화상자가 실행중인가?
	BOOL bIsForceClose;		// 강제 서버 종료 상태인가?
	BOOL bConnectDeny;		// 접속 거부 상태
	int SendMsgMode;		// 서버 메세지 보내기 모드
	int ClientMode;			// 클라이언트 서버 소켓 생성 모드
	int IPMode;				// 클라이언트 IP 감지 및 지정 모드

	int TotUser;			// 총 접속중인 사용자 수(연결된 소켓 수)
	int RealUser;			// 총 접속중인 실제 사용자 수(인증된 사용자 수)

	
	// [ 통계기록 ]
//	STATISTICS_SINFO m_StatisticsLog; 
	CStatisticsLog   m_cSALog;		//<-- 추가

	////////////////////////////////////////////////////////////////
	///// [ ### 자동종료 ### ]
	int SendExitMode;				//	라디오 버튼 시간 선택(1분, 5분...)
	BOOL m_bExitReserve;			//  현재 상태(예약, 취소)
	UINT hExitReserveTimer;			//  1000 타이머
	int m_ReservTimeLimit;			//	종료 시간 예약(초단위)
	void ShowState(LPCTSTR szChar); //  서버 상태
	void ExitReserveTimer();		//  종료 메세지
	///////////////////////////////////////////////////////////////


	UINT hMsgPoolingTimer;
	UINT hGlobalGameTimer;
	UINT hCloseTimer;
	UINT		hMsgUerCntTimer;	// [PC방 작업] 사용자 카운트 타이머

	CCommMsg TmpMsg;			// 패킷 분석용 임시 메시지 클래스
	char MsgBuf[RECVQUESIZE];	// 패킷 분석용 임시 버퍼
	int MsgSignal;				// 패킷 분석용 임시 시그널 변수
//	char SndBuf[SENDQUESIZE];	// 패킷 송신용 임시 버퍼

	int CheckPacket(CMySocket* pSock, char *lpBuf, int *pSignal);
	BOOL ProcessPacket(int sid, CMySocket *pSocket, char* lpdata, int totsize, int signal);

	void LoadConfig();
	void LoadConfig2();
	void LoadConfig3();
	void LoadConfig4(); // jackpot3
	void LoadConfigEvent();// ### [이벤트 티켓] ###
	void LoadConfigStatis();// [ 통계기록 ]

//	void Make_StatisticsLog(int Kind, int index, BOOL bIncrease = TRUE, INT64 pmoney= 0, INT64 usurymoney=0);// [ 통계기록 ]
//	void Check_StatisticsLog();			// [ 통계기록 ]
//	void Make_SetSADBWork(int Kind);	// [ 통계기록 ]
//	void CloseServer_SALog();			// [ 통계기록 ]


	void SendMsgToAll(char *pMsgData, int nMsgSize);
	BOOL FindID(RESULTFINDID *pRF, char *id);
	BOOL UserDisconnectProccess(int unum);
	void PrepareClose();
	void OnCloseTimer();

	CString GetLogFileName_view();

	// [관리자 모드 작업]
	BOOL IsAccessDeny(char* id, UINT &nSec); // [관리자 모드 작업]
	BOOL IsAdminUser(char* id, char* ip);    // [관리자 모드 작업]
	void LoadAdminInfo();                    // [관리자 모드 작업]
	void FindDoubleConnect(const int unum) ;

	// ### [ 채널 입장 구분 ] ###
	int CheckLevel(int nIcon ,INT64 money);

	// ### [이벤트 티켓] ###
	void ReadUsedEventTicketNum();
	void WriteUsedEventTicketNum();
	void CheckGameEvent();

	// 함수 인자 수정 리더스 총액제한 
	
	INT64 Get_angel_defmoney(const INT64 &lmoney); // [수호천사] 2004.07.08

	////////////////////////////////////////////////////////////  <----- 추가
	//2004.05.07 리더스총액 제한
	void SendPremLeadersMsg(int unum, int PremLeadersCode);
	int CheckTotalmoneylimit(USERINFO* pUI, char *uniqno, int bCheckWrite=0); // 0 체크, 1 머니제한 2 머니제한 기록
	CString MoneyToString(INT64 number);
	////////////////////////////////////////////////////////////

	C62CutPokerServerDoc* GetDocument();

	C62CutPokerServerView();
	DECLARE_DYNCREATE(C62CutPokerServerView)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C62CutPokerServerView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	void SendChannelEventNotice(int unum);
	BOOL IsSearchUserID(char *id);
	void LoadSearchID();
	
// [대화 저장]
	BOOL IsCaptureUserID(char* id);
	void LoadCaptureTalkID();


	void ShowServerState();
	void JackPotSave();
	
	virtual ~C62CutPokerServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// 자동종료
// Generated message map functions
protected:
	//{{AFX_MSG(C62CutPokerServerView)
	afx_msg LONG OnServAccept(UINT, LONG);
	afx_msg LONG OnDataClose(UINT, LONG);
	afx_msg LONG OnDataConnect(UINT, LONG);
	afx_msg LONG OnDbworkLogin(UINT, LONG);
	afx_msg LONG OnDbworkLogout(UINT, LONG);
	afx_msg LONG OnDbworkSetOK(UINT, LONG);
	afx_msg LONG OnDbworkDoubleConnect(UINT, LONG);
	afx_msg void OnViewConfig();
	afx_msg void OnButtonSend();
	afx_msg void OnRadioSendtext();
	afx_msg void OnRadioSendmsgbox();
	afx_msg void OnRadioSendmsgboth();
	afx_msg void OnButtonSetloginmsg();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonForcedisconnect();
	afx_msg void OnButtonForcedelroom();
	afx_msg void OnViewConfig2();
	afx_msg void OnViewConfig3();
	afx_msg void OnViewConfig4();
	afx_msg void OnButtonJackpot();
	afx_msg void OnRadioExit1();
	afx_msg void OnRadioExit10();
	afx_msg void OnRadioExit3();
	afx_msg void OnRadioExit30();
	afx_msg void OnRadioExit30s();
	afx_msg void OnRadioExit5();
	afx_msg void OnButtonExitok();
	afx_msg void OnButtonExitcancel();
	afx_msg LONG OnDbworkUseitemresult(UINT, LONG);	// [게임 아이템 작업]
	afx_msg LONG OnDbworkGameitemlist(UINT, LONG);	// [게임 아이템 작업]
	afx_msg void OnSetAdmin();
	afx_msg void OnBtnDenyuser();
	afx_msg void OnSetCaptureid();
	afx_msg void OnSetSearch();
	afx_msg void OnViewConfigevent();  // ### [이벤트 티켓] ###
	afx_msg void OnButtonEventstate();
	afx_msg LONG OnDbworkEventSet(UINT, LONG);	// ### [이벤트 티켓] ###
	afx_msg void OnViewConfigsa();	   // [통계기록]
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SutdaServerView.cpp
inline C62CutPokerServerDoc* C62CutPokerServerView::GetDocument()
   { return (C62CutPokerServerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TETRISSERVERVIEW_H__8D85588A_B8D2_4EBF_ACC2_BAA9322B72F3__INCLUDED_)
