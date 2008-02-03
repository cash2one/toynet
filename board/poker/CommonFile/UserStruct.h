#ifndef __USERSTRUCT_H__
#define __USERSTRUCT_H__

// ### [ 관전기능 ] ###
const int  MAX_PLAYER	=   6;
const int  MAX_OBSERVER =   5;//  - (1 ### [관리자 모드 작업] ###)  
const int  MAX_NEWPLAYER = (MAX_PLAYER+MAX_OBSERVER);


#define MAX_AVATARLAYER 26
#define BANISHVOTE_NUM 3


#define UPDATE_GAMENUM        20  //    20회 ########### 중간값 기록시 기준이 될 게임횟수 ###########
//#define BACKUP_GAMETIME     1800  //    30분 ################## 전체백업 시간(초) ###################



//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [게임 아이템 작업]                               //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_ITEM_NUM           (30) // 최대아이템 갯수

#define GAMEITEM_MES_ERROR     (-1) // 에러
#define GAMEITEM_MES_SUCCESS   ( 0) // 성공
#define GAMEITEM_MES_DBERROR   ( 1) // DB에러
#define GAMEITEM_MES_NOITEM    ( 2) // 아이템이 없거나 사용가능한 아이템 없다
#define GAMEITEM_MES_USEABLE   ( 3) // 아이템이 있고 사용하지 않았다
#define GAMEITEM_MES_EXPIRED   ( 4) // 이미 사용중인 아이템이고, 기간이 만료되었다
#define GAMEITEM_MES_USEFAIL   ( 5) // 아이템사용에 실패했으며, 다시 시도 요청

#define ITEM_SCODE_INVINCIBLE   21		// 천하무적 아이템 시작 코드 (슈퍼방, 찬스, 비공개) 

#define ITEM_SCODE_SAFEGUARDANGEL	61		// [수호천사] 2004.07.08
#define ITEM_ECODE_SAFEGUARDANGEL	61

#define ITEM_SCODE_SECRET		100		// [비공개방] 아이템 시작 코드
#define ITEM_ECODE_SECRET		199		// [비공개방] 아이템 끝 코드
/*
#define ITEM_SCODE_JUMP			200		// 점프 아이템 시작 코드
#define ITEM_ECODE_JUMP			299		// 점프 아이템 끝 코드

#define ITEM_SCODE_SUPERMASTER	400		// 슈퍼 방장 아이템 시작 코드
#define ITEM_ECODE_SUPERMASTER	499		// 슈퍼 방장 아이템 끝 코드

#define ITEM_SCODE_PLUSPOINT	500		// 플러스 포인트 아이템 시작 코드
#define ITEM_ECODE_PLUSPOINT	599		// 플러스 포인트 아이템 끝 코드
*/

#define ITEM_SCODE_JUMP			201		// 점프 아이템 시작 코드
#define ITEM_ECODE_JUMP			201		// 점프 아이템 끝 코드
#define ITEM_SCODE_SUPERMASTER	401		// 슈퍼 방장 아이템 시작 코드
#define ITEM_ECODE_SUPERMASTER	401		// 슈퍼 방장 아이템 끝 코드
#define ITEM_SCODE_PLUSPOINT	501		// 플러스 포인트 아이템 시작 코드
#define ITEM_ECODE_PLUSPOINT	501		// 플러스 포인트 아이템 끝 코드

// ### [ 올인극복 아이템 ] ###
#define ITEM_SCODE_ALLINSUPERMASTER  800	//1시간 슈퍼방장
#define ITEM_ECODE_ALLINSUPERMASTER  899	//1시간 슈퍼방장


typedef struct
{
	int        Index;		// 서버에서확인하는 인덱스
	int        NO;			// 유니크 넘버
	int        Code;		// 아이템 코드
	char       PresID[16];	// 선물준 사람
	short      UseDay;		// 사용 가능한 기간(일)
	short      UseNum;		// 사용 가능한 회수
	SYSTEMTIME StartDate;	// 사용 시작 시간
	short      Use;			// 사용 유무(또는 사용한 회수)
	char       Reserved[2];
} GAMEITEM;

typedef struct
{
	BOOL	  bLogable;  // 로그인 가능여부
	int       nResult;   // -1:에러, 0:성공, 1:DB오픈에러, 2:아이템없거나 사용가능한 아이템없다, 3:아이템이 있고 사용하지 않았다
	int       nNum;      // 추출한 아이템 갯수
	GAMEITEM  Item[MAX_ITEM_NUM];

} GAMEITEM_LIST;

// 게임 아이템 리스트 정보(클라이언트 송신용)
typedef struct
{
	int			Kind;		// 종류(0:아이템 리스트 정보 1:점프 아이템 사용시 리스트 정보 100:아이템 리스트 갱신 정보)
	int			ErrorCode;	// 에러 코드
	int			TotNum;		// 아이템의 개수
	SYSTEMTIME	ServerTime;	// 서버의 현재 시간
	int			StrMsgLen1;	// 문자열 길이 1 (아이템 사용 유무를 물어보는 문자열)
	int			StrMsgLen2;	// 문자열 길이 2 (아이템 설명 문자열)
} GAMEITEM_INFO;

// 게임 아이템 구입 정보
typedef struct
{
	int  Kind;			// 종류
	char ItemName[256];	// 아이템 이름
	int  ItemCode;		// 아이템 코드
	int  StrMsgLen;		// 설명 문자열 길이
} BUYGAMEITEMINFO;

// 게임아이템 사용 정보(보내고 받기 모두 사용)
typedef struct
{
	char		ID[16];			// 아이템 사용자 아이디
	GAMEITEM	Item;			// 사용할(혹은 사용한) 아이템의 정보
	BOOL		bUseError;		// 아이템 사용중 에러가 발생하였는가?
	BOOL		bMessage;		// 메시지를 포함하는가?
	char		StrMsg[256];	// 메시지 스트링
} USEGAMEITEMINFO;

// 아이템 상태 반환 구조체
struct ItemState
{
	int		  iState;		// 0: 잘못된 아이템 1:사용가능한 아이템 2:사용중인 아이템 3:사용할 수 없는 아이템(다쓴 아이템)
	BOOL	  bNowUsing;	// 현재 사용중인가?
	CTimeSpan LeftTime;		// 남은 시간
	CTime	  EndDate;		// 만료일시
};

// 아이템의 상태를 얻어오는 편의 함수
ItemState GameItem_GetItemState(GAMEITEM &item, SYSTEMTIME* pCurTime=NULL);

// 아이템의 종류를 알아내는 편의 함수(아이템의 시작코드를 종류 코드로 사용)
int GameItem_GetItemKind(int itemcode);

// 특정 종류의 아이템이 사용중인지 알아냄(아이템이 존재하는 경우 pExist에 넘어옴, 서버/클라이언트 공용)
BOOL GameItem_IsUsingItem(GAMEITEM_LIST &itemlist, int itemcode, BOOL *pExist, SYSTEMTIME* pCurTime=NULL);

// 아이템의 이름을 얻어오는 편의 함수
CString GameItem_GetItemName(GAMEITEM& item);
CString GameItem_GetItemName(int code);

// 아이템의 기능 도움말을 얻어오는 편의 함수
CString GameItem_GetItemHelp(int code);

// 아이템 이미지 파일을 얻어오는 편의 함수
CString GameItem_GetItemImageFile(int code);

// 아이템 이미지의 인덱스를 얻어오는 편의 함수(클라이언트의 이미지 리스트에서 사용)
int GameItem_GetItemImageIndex(int code);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetPokerLevel(INT64 money);
int GetLevelPoint(int point, int winnum, int loosenum);
int GetWinPro(int winnum, int loosenum);
int GetMaxBetMarble(int level);
int GetMyLeveltoGrade(int lv);

INT64 GetLevelToUserMoney(int lv); // 유저레벨 -> 머니 
int GetCreateRoomGrade(int gr);    // 방만들기시 최소머니 설정등급

/////////////////////////////////////////////////////
////////////////     포카 정보     //////////////////
/////////////////////////////////////////////////////
typedef struct
{
	char nCard; // 대표카드
	int  nRSF;
	int  nSF;
	int  nFCARD;
	char Card[12];
} POKERINFO;

typedef struct
{
	INT64 nHM; // 중수 제한머니
	INT64 nFM; // 고수 제한머니
	INT64 nGM; // 도신 제한머니

	char   nHL; // 중수 제한레벨
	char   nFL; // 고수 제한레벨
	char   nGL; // 도신 제한레벨
}POKERSET;

/////////////////////////////////////////////////////////////////////////////////////////
// 로그인 요청 정보	[SSO 작업] - 추가
typedef struct
{
	char ID[16];			// ID
	char Pass[16];			// 암호화된 패스워드는 4배로 커짐
	char SockNameIP[20];
	char HostNameIP[20];
	int	 Port;
	int  AuthCookieLen;		// 인증 쿠키 길이
	int  DataCookieLen;		// 데이터 쿠키 길이
	int  bForceLogin;		//2004.05.07 리더스총액 제한    <--------- 추가
} ASK_LOGININFO;


//2004.05.07 리더스총액 제한
typedef struct{
	char ID[16];
	char PremLType;
	INT64 CurMoney;
	INT64 NorMoney;
	INT64 LeadersMoney;
	INT64 LeadersPlusMoney;
	char UniqNo[16];		// 유니크 넘버
	char LeadersJoinURL[64]; // 리더스 가입홈페이지
	char LeadersJoinURL2[64]; // 리더스 가입홈페이지
} LEADERS_JOIN;

//2004.05.07 리더스총액 제한
typedef struct {
	char ID[16];
	char NO[16];
	int  MoneyType;
	INT64 Amount;
	INT64 BeforeM;
	INT64 AfterM;
} PRE_USERLIMITGMLOG;
///////////////////////////////////////////////////////
/////////////////////////////////////////////////////
////////////////    사용자 정보    //////////////////
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// 사용자 정보
typedef struct
{
	short  UNum;				// 유저 번호				- DB에 저장되는 정보가 아님
	char ID[16];			// 사용자 아이디
	// [SSO 작업] - 아래 한줄 삭제
	//char		Pass[10];		// 비밀번호						[SSO 작업] - 삭제
	char Sex;				// 성별
	char		PremCode;		// 프리미엄 서비스 코드							[프리미엄 작업]
	SYSTEMTIME	PremDate;		// 프리미엄 서비스 기간(만료일)					[프리미엄 작업]
	char		bPremIP;		// 프리미엄 PC방 IP인가?						[PC방 작업]
	UINT  WinNum;			// 승수
	UINT  LooseNum;			// 패수
	UINT  DrawNum;			// 비긴수
	char nIcon;             // 레벨아이콘

	char cObserver;			// ### [ 관전기능 ] ### 
	char cAdminUser;		//### [관리자 모드 작업] ###

	INT64  PMoney;		    // 포커 머니
	POKERINFO PI;           // 포커 정보
	short AvatarInfo[MAX_AVATARLAYER];	// 아바타 정보
	char Desc[62];			// 하고싶은 말
	char PremLeadersCode;
} USERINFO;

typedef struct  // 대기실 사용자 정보 갱신용
{
	short UNum;				// 유저 번호
	char  ID[16];			// 사용자 아이디
	char Sex;				// 성별
	char nIcon;             // 레벨아이콘
	char Position;			// 현재 위치(WH_CHAN:채널선택 WH_LOBY: 대기실 WH_SHOP:상점 WH_GAME:게임방)
	char RoomNum;			// 방번호(현재 위치가 게임방일때만 유효)
	short cAdminUser;		//### [관리자 모드 작업] ###
} SMALLUSERINFO;

// 로그인시 유저 정보
typedef struct
{
	UINT  WinNum;
	UINT  LooseNum;
	UINT  DrawNum;			// 비긴수
	INT64 pBackup;
} ORGINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// 서버 정보	[게임 아이템 작업]
typedef struct
{
	int  GameCode;			// 게임 코드
	char ServerName[20];	// 서버 이름
	char NoticeURL[64];		// 공지사항 URL
	char BannerURL1[64];	// 로비 광고 URL
	char BannerURL2[64];	// 게임 광고 URL
	char LocationURL[64];	// 위치알림(친구찾기)용 URL
	char ItemURL[64];		// 아이템 상점 URL
	char SrvInfoMsg[80];	// 채널로비 메세지
	char ItemHelpURL[64];	// 아이템 도움말 URL
	INT64 JackPot;			// [ ###잭팟### ]로긴시 현재 서버 잭팟 머니
	char CashURL[64];		// 캐쉬충전 URL
} SERVERINFO;


///////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// 로그인 정보
typedef struct
{
	USERINFO UI;			// 사용자 정보
	char RegID[16];			// 주민등록 번호 [게임 아이템 작업]
	char UniqNo[16];		// 유니크 넘버
} LOGININFO;

/////////////////////////////////////////////////////////////////////////////////////////
// 채널 입장 정보
typedef struct
{
	int ChNum;				// 채널 번호(0보다 작으면 입장 불가 코드)
	char ChanName[20];		// 채널 이름
	char ChanStyle;			// 채널 스타일
	char channel_usernum;	// 5,7 인용 결정
} ENTERCHANINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// 채널 정보
typedef struct
{
	char  ChNum;			// 채널 번호
	char  ChannelStyle;     // 채널 등급 추가 2003 0621 	// ### [ 채널 입장 구분 ] ###
	short NowUserNum;		// 현재 참여인원
	short MaxUserNum;		// 최대 참여인원
	char Title[20];			// 채널명
	char channel_usernum;	// 5,7 인용 결정
} CHANNELINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// 사용자 찾기 결과
typedef struct
{
	char ID[16];			// 아이디
	int  ChNum;				// 채널 번호
	short Position;			// 위치
	short RoomNum;			// 방번호
} RESULTFINDID;

////////////////////////////////////////////////////////////////////////////////////////
// 방 정보

typedef struct
{
	short RoomNum;			// 방번호
	char  Title[32];			// 방제목
	short UNum;				// 방장의 유저 번호

	char  ID[16];			// 방장 ID
	char  NowUserNum;		// 현재 참여인원
	char  MaxUserNum;		// 최대 참여인원
	char  NowObserverNum;    // ### [ 관전기능 ] ### 관전자 인원
	char  Level;				// 방의 레벨(0:하수 1:중수 2:고수)

	char  Secret;			// 공개 여부(0=공개 1=비공개)
	char  State;				// 상태(0:대기중 1:게임중)
	char  GameKind;			// 게임 베팅
	char  Pass[5];			// 패스워드(비공개방 일경우)
	//  최소 머니 설정   (유저등급머니 * 최소머니등급)
	char UsernIcon;         // 유저 등급
	char CreateRGrade;      // 최소머니 등급
	char  nCard;			// 카드 무늬
	char  FormKind;			// Normal, Choice 4장

	int		BetMoney;			// 베팅머니
	
	char  bSuperMaster;		// 슈퍼 방인가?	[게임 아이템 작업]
	char  bPlayer;			// 5, 7 
	char  ChangeCardStep;	// 카드바꾸기 횟수 1,2
	char Reserved;
} ROOMINFO;

typedef struct	// 대기실 방 정보 갱신용
{
	short  RoomNum;			// 방번호
	short  UNum;				// 방장의 유저 번호
	char   ID[16];			// 방장 ID
	char   NowUserNum;		// 현재 참여인원
	char   MaxUserNum;		// 최대 참여인원
	char   NowObserverNum;    // ### [ 관전기능 ] ###관전자 인원
	char   State;				// 상태(0:대기중 1:게임중)
	char   GameKind;			// 게임형식(0:아이템, 1:노템)
	INT64 JackPotMoney;		// 잭팟 
} SMALLROOMINFO;

////////////////////////////////////////////////////////////////////////////////////////
// 게임 정보
typedef struct
{
	ROOMINFO Ri;
	USERINFO Ui[MAX_NEWPLAYER];
} GAMEINFO;

////////////////////////////////////////////////////////////////////////////////////////
// 플레이어 상태 정보
typedef struct
{
	char JoinState;		// 게임 참여 상태(0:불참, 1:참여)
	char PlayState;		// 게임 상태(0:게임종료 1:게임중)
	char nState;     // 0: 아무상태아님, 1: 카드3장, 2: 카드2장,오픈1, 3: 카드4~5장, 4: 히든, 5: 결과
	char nCardTotal; // 총카드 수
	char nOpenTotal; // 오픈된 카드수
	char nCard[4];   // 오픈 카드 별도관리 보유카드 ###[8포커]### 5
	char bCheck;

	char bOnGame; // 게임 참여 여부
	char bFold;   // 게임 포기 여부
	char nCheckIp;	// ### [ 중복된 IP가 ] ###
	char nSndFxKind; // ### [사운드 추가 작업] ###
	char Reserved[2];
} PLAYSTATE;

////////////////////////////////////////////////////////////////////////////////////////
// 플레이어 상태 정보 2
typedef struct
{
	BOOL bJoinBanishVote;	// 추방 투표에 참여중인가?
	BOOL bHaveBanishVote;	// 추방 투표를 마쳤는가?
	// [ 세븐 포커 ]
	BOOL bActive;			// 액티브 유저(현재 차례)
	int  nActive;			// 제한 시간
	BOOL bWindOn;			// 날리기 제한시간 플래그
	int  nWindOn;			// 날리기 제한시간
	int  nFoldNum;			// 자동포기횟수(3회이상이면 방퇴장) 레이즈 참여시 플래그 초기화
	BOOL bOnGame;			// 게임 참여 여부
	int  In_OutCount;		// ### [ 관전기능 ] ### 관전, 참여 반복제한
	int  nChangeFlg;		// 카드 바꾸기 플래그
//	short  nChangeWinEndFlg;// 카드 바꾸고 날리기 끝을 알리는 플래그
	int  Totalmoneylimit;	//2004.05.07 리더스총액 제한
} PLAYSTATE2;


typedef struct
{
	char nCard[4];   // 보유카드+히든카드+ 초이스카드
} BACKCARD; // ############## 카드 날리기 엔딩플래그로도 사용한다

/////////////////////////////////////////////////////
///////////      게방용 레이즈 정보     /////////////
/////////////////////////////////////////////////////
typedef struct{
	int      nUNum;  // 사용자 아이디
	char    bInUse;  // 사용유무(게임참여플래그이며 끝까지 간다)
	char       nUse; // 사용자의 레이즈횟수
	char     nState; // 0: 아무상태아님, 1:카드3장, 2:카드2장오픈1, 3: 카드4장~6장, 4: 히든모드, 5: 오픈모드
	char     nStyle; // 0: 체크, 1: 폴드, 2: 배트, 3: 다이
	char       nBtn; // 0: 삥(최소), 1: 따당(적정), 2: 맥스(최대)
	char     bFold;  // 포기 여부
	char     bCall;  // 콜사인 여부
	char   nLastCmd; // 마지막 명령
	INT64  nNowBat;  // 레이즈진행중인 머니
	INT64  nTotBat;  // 총배팅 머니
	INT64 nRealBat;  // 실배팅 머니
	INT64  nPMoney;  // 보유중인 포커머니
	INT64 nReCall;   // 받고 
	INT64 nCalcBat;  // 더
}RAISEUSER;

/////////////////////////////////////////////////////
///////////      게방용 플레이 정보     /////////////
/////////////////////////////////////////////////////
typedef struct{
	char  bDistCard;
	char  nDistUser;
	char  nRound;	 // 베팅 라운드
	char  nState;
	char  bRaiseOn;  // 레이즈 지속여부
	char  bBtnBet;
	char  bBtnCheck;
	char  nPreCmd;
	char CurPlayer;
	char LastPlayer;
	char LastStyle;

	PLAYSTATE PS[MAX_PLAYER];
	RAISEUSER RU[MAX_PLAYER]; 	
	BACKCARD  BC[MAX_PLAYER]; // 히든 카드
}POKERGAME;

typedef struct{
	int  nStyle;    // 0: 폴드, 1.체크 , 2: 콜, 3: 배트, 4: 다이
	int  nBtn;      // 0: 삥(최소), 1: 따당(적정), 2: 하프, 3: 맥스(최대)
}POKERCLNT; //  ##############3 카드날리기 엔딩으로도 사용한다

typedef struct{
	char ID[16];
	char  nCard;             // 로티플, 스티플, 포카드
	short  nRoom;
	char ChanName[25];		// 채널 이름
	INT64 JackPotPlus;	    // 잭팟 보너스 점수
	INT64 NowJackPot;	    // 현재 잔액
}SPECIALCARD;

typedef struct{
	char     ID[16];
	int      nUNum;
	USERINFO UI;
}USERRESULT;

typedef struct{
	char     ID[16];
	int      nUNum;
	int      nCard;
	INT64    nMoney;
}BUYCARD;

typedef struct{
	char     ID[16];
	int      nUNum;
	int      nCard;
}CHANGECARD;

////////////////////////////////////////////////////////////////////////////////////////
// 방 입장 정보
typedef struct
{
	ROOMINFO Ri;
	USERINFO Ui[MAX_NEWPLAYER];// ### [ 관전기능 ] ###
	POKERGAME Pg;
} ENTERROOMINFO;

////////////////////////////////////////////////////////////////////////////////////////
// 내 사용자 정보 변경 정보
typedef struct
{
	UINT WinNum;
	UINT LooseNum;
	UINT DrawNum;
	INT64 PMoney;
} CHANGEMYINFO;

////////////////////////////////////////////////////////////////////////////////////////
// 추방 투표 요청 정보
typedef struct
{
	short RoomNum;		// 방 번호
	short MyServPNum;	// 요청자 서버 플레이 번호
	char MyID[16];		// 요청자 아이디
	short TarServPNum;	// 추방 대상 서버 플레이 번호
	char TarID[16];		// 추방 대상 아이디
	char Reson[42];		// 추방 사유
} BANISHVOTEINFO;

////////////////////////////////////////////////////////////////////////////////////////
// 게임 시작 정보

typedef struct
{
	char JoinID[MAX_PLAYER][16];		// 참여자 아이디
	PLAYSTATE Ps[MAX_PLAYER];			// 플레이어 상태 구조체
	char  nCard[4];
	int   nDistUser;					// 카드 날리기
} STARTINFO;
////////////////////////////////////////////////////////////////////////////////////////

typedef struct{
	char     ID[16];

	short    nUNum;
	char     nFlag;			// -1: 0 : 바꾸지 않았다 1 : 바꾼다	
	char     nChangeCardNum; // 바꾼 숫자 

	short    nCardNum;		// 세로 받은 숫자 
	char     nCardIndex;
	char     nChangeStep;	// 바꾼 횟수 

	short	 nRoomNum;
	char	 nPNum;
	char	 nServerPNum;	
}CHANGECARD_62CUT;

typedef struct{
	CHANGECARD_62CUT data[MAX_PLAYER];
	short   nDistUser;					// 카드 날리기
	short	nRoomNum;	
}CHANGECARD_62CUT_EX;


////////////////////////////////////////////////////////////////////////////////////////
// 게임 종료 결과
typedef struct 
{
	char  ID[16];
	UINT   WinNum;
	UINT   LooseNum;
	UINT   DrawNum;
	INT64 PMoney;
	INT64 LoseMoney;
	POKERINFO PI;
} GAMEOVERINFO;


typedef struct
{
	char      nNowWinner;   // 하이 승자 플레이어 넘버
	char      ID[16];		// 승자 아디	
	char      bOpen;		// 어떻게 이겼는가?(모두 죽어서 홀로 이겼는가 FALSE, 끝까지 베팅했는가 TRUE)
	char      bJackPot;     // 잭팟이 있는가   		
	
	INT64	  nRealBat;  // 게임 오버시 실제배팅 머니 X
	INT64     nWinMoney;	// 실제 증가분 하이(내 베팅액을 제외한 실제)

	POKERGAME Pg;
	GAMEOVERINFO  Ui[MAX_PLAYER];
	
	INT64     nGory;    // 고리금액
	int       nPercent; // 퍼센티지 : 단순히 보여주는 기능만 있다 1%,5%,10% 등등	
	INT64     JackPotMoney;
} GAMEOVERRESULT;


/////////////////////////////////////////////////////
////////////    [서버 에이전트 작업]   //////////////
/////////////////////////////////////////////////////

typedef struct
{
	short  nUNum;   // 사용자 번호
	char ID[16];  // 아이디
	INT64 PMoney; // 포커머니 증감치
} AGENT_USER;


/////////////////////////////////////////////////////
///////////     [관리자 모드 작업]     //////////////
/////////////////////////////////////////////////////

#define MAX_ADMIN_NUM           (20) // 최대 관리자 수
#define MAX_DENY_USER          (200) // 최대 접속 중지 가능 유저

//관리자 정보
typedef struct
{
	char ID[16]; // 관리자 아이디
	char Grade;  // 등급
	char UserIP[24];			// 관리자의 접속 IP정보	
} ADMIN_INFO;

//총관리자
typedef struct
{
	int nTotal;
	ADMIN_INFO admin[MAX_ADMIN_NUM];
} ADMIN_MANAGER; //서버 공통

//에이젼트 관리용
typedef struct
{
	int NowNum;
	ADMIN_MANAGER AdminMan;
} AGENT_ADMIN;

//관리자 메세지
typedef struct
{
	int  UNum;
	char ID[16];
	char Message[130]; // 실제로는 128까지만 사용된다
} ADMIN_MESSAGE;

//특정유저 접속금지
typedef struct
{
	int  UNum;
	char ID[16];  // 관리자 아이디
	UINT nSecond; // 접속불가 시간
} ADMIN_ACCESSDENY_USER;

//### [관리자 모드 작업] ###
typedef struct
{
	short  AdminUNum;
	char AdminID[16];
	short  TarUNum;
	char TarID[16];
} ADMINMESSAGE;

//  방 옵션 변경 정보 
typedef struct
{
	short   RoomNum;			// 방번호
	short   UNum;				// 방장의 유저 번호
	char  ID[16];			// 방장 ID
	char  Title[32];        // 방제목
	char  nCard;
	BOOL  bWhis;
	char  Reserved[2];
} STYLEROOMINFO;

//정보 변경 알림 메세지
typedef struct //결과
{
	short   RoomNum;		// 방번호
	short   UNum;			// 방장의 유저 번호
	char  ID[16];			// 방장 ID
	char  Title[32];        // 방제목
	int  nCard;

} CHANGESTYLEROOMINFO;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [프리미엄 작업]	                                //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int CheckPremiumState(USERINFO* pUI, SYSTEMTIME* pCurTime=NULL);


///////////////////////////////////////////////////////
////////////  ### [로그 기록용] ###   /////////////////
///////////////////////////////////////////////////////
typedef struct
{
	int Status;
	char ID[16];
	INT64 CurMoney;
	INT64 LoseMoney;
	char Win_ID[16];
	INT64 Win_CurMoney;
	INT64 Win_GetMoney;
	char My_ip[24];
	char Winner_ip[24];
} LOGRECORD;


typedef struct
{
	char ID[16];
	INT64 BeforeMoney;
	INT64 AMountMoney;
	INT64 AfterMoney;	
} ALLIN_LOGRECORD;


/////////////////////////////////////////////////////
////////////		[대화 저장]		   //////////////
/////////////////////////////////////////////////////
#define MAX_CAPTURE_NUM           (300) // 최대 관리유조

typedef struct
{
	BOOL bUse; // 사용유무
	int  nTotal;                // 관리자 수
	char ID[MAX_CAPTURE_NUM][16]; // 관리자 아이디
} CAPTURE_INFO;

// ### [ 중복된 IP가 ] ###
typedef struct 
{
	char IpIndex[MAX_PLAYER];
} IPARRINDEX;

//특정아이디검색
typedef struct
{
	BOOL bUse; // 사용유무
	int  nTotal;                // 관리자 수
	char ID[MAX_CAPTURE_NUM][16]; // 관리자 아이디
} SERACH_INFO;

////////////////////////////////////////////////////////////////////////////////////////
//  ### [이벤트 티켓] ###
//	이벤트 당첨자 알림 정보
typedef struct
{
	char ID[16];		// 아이디
	int  NowEventPrize;	// 현재 이벤트 당첨 회수
} EVENTPRIZEINFO;

typedef struct {
	//BOOL bHaveEvent;	// 로그인시 이벤트 티겟을 가지고 있는가? 저장 유무 결정
	int GameCnt;		// 게임획수
	int EventPrize;		// 획득
	int OrigEventPrize; // 백업용
	SYSTEMTIME LastDate;
} EVENT_PRIZE;
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
////////////    ### [ 관전기능 ] ###   //////////////
/////////////////////////////////////////////////////
//예약알림 메세지
typedef struct
{
	short Seq; //예약순서
	short UNum;
	char ID[16];
} RESERVATIONINFO;


// dbwork - insert
typedef struct {
	int		Kind;
	INT64   	BigData[5];		// 등급별
	int		SmallData[20];  // 레벨별
	int     	Extra;
	COleDateTime 	LDate;
} STATISTICS_DBRECORD;


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#endif