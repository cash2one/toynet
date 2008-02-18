// 글로벌 변수, 클래스, 함수..

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define VERSION 102 //103


#include "62CutPokerDlg.h"
#include "TitleDlg.h"
#include "LobyDlg.h"
#include "GameDlg.h"
#include "GameView.h"
#include "SockMan.h"
#include "Graphic.h"
#include "xHan.h"
#include "xAniMan.h"
#include "xSound.h"
#include "Sound.h"
#include "UserStruct.h"
#include "Avatar.h"
#include "Player.h"
#include "PlayScr.h"
#include "Game.h"
#include "Display.h"
#include "WhisperDlg.h"
#include "NoChat.h"
#include "BadChatMan.h"
#include "DialogCtrlReposition.h"
#include "WebProfileDlg.h"
#include "MiniDlg.h"

#include "NMSTRMAN.h"
#include "AdminMessageDlg.h"

#define MAX_ROOM	70	// 최대 게임룸의 개수

extern const int TOTAL_CARDNUM;  //총 카드수
extern const int DUMY_CARD;      //카드뒷면
extern const int EMPTY_CARD;    

#define INOUT_COLOR			RGB(100,255,100)
#define OBINOUT_COLOR		RGB(100,255,100)

#define RESERVER_COLOR		RGB(150,150,150)
#define SERVERMSG_COLOR		RGB(255,148,230)
#define GAMEOVERMSG_COLOR	RGB(0,246,255)
#define CHATTING_COLOR		RGB(255,255,255)
#define WARNING_COLOR		RGB(200, 0, 0)	
#define WHISPER_COLOR		RGB(255,255,255)
#define NOTIC_COLOR			RGB(255,255,255)
#define SKIN_NUM 3


extern int g_Max_Player;
extern int g_Max_NewPlayer;


extern C62CutPokerDlg *g_pMainDlg;
extern CTitleDlg *g_pTitleDlg;
extern CLobyDlg *g_pLobyDlg;
extern CGameDlg *g_pGameDlg;
extern CGameView *g_pGameView;
extern CWebProfileDlg g_WebProfileDlg;

extern CSockMan SockMan;		// 소켓 메니져 클래스
extern CSound Sound;
extern xHan g_Han;
extern CNoChat NoChat;
extern CBadChatMan BadChat;		// 욕설 필터링 클래스

extern CAdminMessageDlg *g_pAMes; // [관리자 모드 작업]
/////////////////////////////////////////////////////////////////

extern xSprite GameBackSpr;		// 게임 배경화면 스프라이트
extern xSprite GameBackSpr_7;		// 게임 배경화면 스프라이트

extern xSprite LevelSpr;		// 레벨 스프라이트

//extern xSprite SCharSpr;		// 작은 캐릭터 스프라이트
//extern xSprite LCharSpr;		// 큰 캐릭터 스프라이트
extern xSprite CharBackSpr;		// 캐릭터 뒷배경


/////////////////////////////////////////////////////////////////

extern xSprite InfoBoxSpr;		// 정보창 스프라이트
extern xSprite ChatBoxSpr;		// 채팅 대화창 스프라이트

extern xSprite ExitBtnSpr;		// 나가기버튼 스프라이트
extern xSprite InviteBtnSpr;	// 초대버튼 스프라이트
extern xSprite FullScreenBtnSpr;// 풀스크린 버튼 스프라이트
extern xSprite WindowBtnSpr;	// 윈도우모드 버튼 스프라이트
extern xSprite OptionBtnSpr;	// 옵션버튼 스프라이트
extern xSprite BadUserBtnSpr;	// 대화내용 신고 버튼 스프라이트

extern xSprite MasterBtnSpr;    // 방장
extern xSprite MinimizeBtnSpr;  // 최소화

//extern xSprite Dog19Spr;		// 19세 미만 개소리 스프라이트

// 게임진행 관련 버튼
extern xSprite GameStartBtnSpr;
extern xSprite GameQuitBtnSpr;
extern xSprite X2StartBtnSpr;
extern xSprite AvaSpr;
extern xSprite AllinBtnSpr;
extern xSprite DieBtnSpr;
extern xSprite CheckBtnSpr;
extern xSprite CallBtnSpr;
extern xSprite PingBtnSpr;
extern xSprite DadangkBtnSpr;
extern xSprite MaxBtnSpr;
extern xSprite HighBtnSpr;
extern xSprite LowBtnSpr;
extern xSprite SwingBtnSpr;
extern xSprite EtcBtnSpr;
////////////////////////////////////////////////////////////////[62]
extern xSprite ChangeOKBtnSpr;
extern xSprite ChangeCancelBtnSpr;
// [62]체인지 카드 안내
extern xSprite ChangeCardInfoSpr;
extern xSprite RoomMasterSpr;
extern xSprite BanishVoteSpr;
extern xSprite SortSpr;
extern xSprite AllinAniSpr;
extern xSprite InsertCoinSpr;

////////////////////////////////////////////////////////////////
//extern xSprite BigNumSpr;		// 큰 숫자 스프라이트
extern xSprite EtcSpr;			// 기타
extern xSprite OringCharSpr;	// 파산
/////////////////////////////////////////////////////////////////////////
extern xSprite BigCardSpr;		// 카드
extern xSprite SmallCardSpr;		// 카드

extern xSprite FocusSpr;		// 현재 턴
extern xSprite TimerSpr;

// [ ###잭팟### ] 
extern xSprite JackPotBackSpr;	// 잭팟 배경
extern xSprite JackPotSpr;		// 잭팟 숫자
extern xSprite ChipSpr;			// 칩 
/////////////////////////////////////////////////////////////////
extern xSprite ItemBagSpr;		// 
// ### [ 중복된 IP가 ] ###
extern xSprite WarningSpr;		// 
extern xSprite CaptureSpr;      // [화면캡쳐]
extern xSprite ObserverSpr;		// ### [ 관전기능 ] ###

//////////////////////////////////////////////////
// ### [ 대기실 리스트 관련 ] ###
extern	xSprite			LobyListBackSpr;
extern	xSprite			LobyListButtonSpr;
extern  xSprite			LobyRoomImgSpr;		//	방정보 이미지
//////////////////////////////////////////////////



extern CString g_ServIP;		// 메인 서버의 IP
extern UINT g_ServPort;			// 메인 서버의 포트번호
extern CString g_AuthCookie;	// 인증 쿠키						[SSO 작업]
extern CString g_DataCookie;	// 데이터 쿠키						[SSO 작업]
extern CString g_CpCookie;		// CP쿠키							[SSO 작업]
extern CString g_SpareParam;	// 여분의 인자(CP게임등에서 사용)	[SSO 작업]
extern CString g_LoginID;		// 자동 로그인 ID
extern CString g_LoginPass;		// 자동 로그인 패스워드
//extern CString g_Extra;			// 엑스트라 인자				[SSO 작업] - 삭제
extern CString g_MyUniqNo;		// 내 유니크 넘버


extern CString g_ServerName;	// 접속한 서버 이름
extern CString g_ChannelName;	// 접속한 채널 이름
extern CString g_NoticeURL;		// 공지사항 URL
extern CString g_BannerURL1;	// 로비 광고 URL
extern CString g_BannerURL2;	// 게임 광고 URL
extern CString g_ProfileURL;	// 프로필 URL
extern CString g_ItemURL;		// 아이템 상점 URL	[게임 아이템 작업]
extern CString g_ItemHelpURL;	// 아이템 도움말 URL	[게임 아이템 작업]
extern CString g_CashURL;		// 캐쉬충전 URL

extern CString g_strSrvInfo;	// 채널 공지

extern int       g_nChannelStyle; // 게임 스타일
extern int       g_nchannel_usernum; // 게임 5, 7인용 결정

extern int        g_MSCode;
extern CNMStrMan    g_StrMan;

extern CString g_LocationURL;					// 위치알림(친구찾기)용 URL

extern int g_GameCode;

extern int g_MainSrvSID;		// 메인 서버의 소켓 번호

#define WH_FIRST	0			// 로그인하기전
#define WH_LOGIN	1			// 로그인중
#define WH_TITLE	2			// 타이틀 화면
#define WH_LOBY		3			// 대기실
#define WH_GAME		4			// 게임중

//#define BBOX_WIDTH 10
//#define BBOX_HEIGHT 20

extern CRect g_OrgMainDlgRect;	// 메인 다이얼로그의 원래 크기
extern int g_Where;				// 내가 위치한 곳(0:로그인하기 전, 1:로그인중 2:대기실, 3:게임중)
extern BOOL g_Wait;				// 응답을 기다리는 중인가?


extern 	BOOL g_bChannel;
extern int g_BgMode;			// 스킨 플래그

extern int g_MyPN;				// 나의 플레이어 번호(0-2)
extern int g_MastPN;			// 방장의 플레이어 번호(0-2)
extern ROOMINFO g_RI;			// 방 정보 구조체
extern CPlayScr PlayScr[MAX_PLAYER];		// 플레이어 스크린 클래스
extern CCoverCard g_CvCard;
extern CDisplay Display;		// 디스플레이 모드 변경 클래스
extern BOOL g_bChangeCardStage;		// 카드 바꾸기
//////////////////////////

// ### [ 관전기능 ] ###
extern CPlayer Play[MAX_NEWPLAYER];		// 플레이어 정보
extern CPlayer g_MyInfo;
extern int  g_MyIndex;
extern BOOL g_MyObserver;
extern BOOL g_bAdminUser; // [관리자 모드 작업]

//////////// [게임 아이템 작업] ////////////
extern GAMEITEM_LIST g_MyGameItem;	// 내 게임 아이템
extern CTime   g_ServerTime;	// 서버의 시간
extern CTime   g_ClientTime;	// 클라이언트의 시간 (서버와 클라이언트와의 시간차를 구할때 사용)
extern CTimeSpan g_CSTimeGap;	// 서버와 클라이언트의 시간차
////////////////////////////////////////////

extern POKERGAME g_Poker;
void g_GetNowSrvTime(COleDateTime &NowSrvTime);// [비공개방 아이템] 

// [ 세븐 포커 ] 잭팟
// [ ###잭팟### ] 
extern INT64 g_JackPotMoney;
extern INT64 g_TmpJackPotMoney;

//////////////////////////

char* GetStrArea(int num);
char* GetStrJob(int num);

//////////////////////////

//int GetLevel(int point, int winnum, int loosenum, int foldnum);


//////////////////////////

class CGame;
extern CGame Game;
extern CWhisperDlg g_WhisperDlg;

extern CMiniDlg g_Mini;

// [세븐포커] 레벨 머니제한
//extern POKERSET g_POKERSET;

CString g_MakeCommaMoney(INT64 nMoney);
CString NumberToOrientalString( INT64 number );
int NumberToParse(INT64 money, int &Num1, int &Num2, int &Num3, int &Num4);

CString g_MoneyMark(INT64 nMoney);

//void BrowserMove( CString targeturl ); // [ 파산 ]

CString g_GetLevelName(int nLevel);


typedef	struct {	
	char	Macrostr[10][40];	
}CHAT_MACRO;

extern CHAT_MACRO g_ChatMacro; // 메크로(채팅 관련)

#endif