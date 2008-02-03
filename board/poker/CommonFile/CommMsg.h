#ifndef __COMMMSG_H__
#define __COMMMSG_H__

#include "UserStruct.h"

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define MSG_ID_LEN	2		// 메세지 인식문자 길이	
#define MSG_ID ("NP")		// 메세지 인식문자
#define PACKET_HEADERSIZE 16	// 패킷의 헤더 사이즈 = sizeof(PACKETHEADER)

// 패킷 헤더 구조체
typedef struct
{
	char MsgID[2];
	short MsgLen;
	short Signal;
	char Dummy[2];
	UINT PackCnt;	// 패킷 카운터
	UINT PackCRC;
} PACKETHEADER;


// 서버용 메세지 #########################################################
enum
{
	////////////////// 값과 위치를 변경하지 말아야 할 부분 ////////////////

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_CHECKVERSION=10,		// 버전 체크
	SV_RECONNECT=11,		// 리컨넥션 정보
	SV_ASK_LOGIN=12,		// 접속 요청 로그인 정보	// [SSO 작업] - SV_ASK_LOGIN로 이름을 바꿈
	SV_SERVERINFO=13,		// 서버 정보	[게임 아이템 작업]


	/////////////////// 서버가 보내는 메세지 ///////////////////
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_ACCEPT_LOGIN=125,	// 로그인을 허용함
	SV_REFUSE_LOGIN,		// 로그인을 거부함(원인: 아이디 없음, 비밀번호 틀림, 버전이 틀림)
	SV_ASK_LOGOUT,			// 로그아웃을 요청함
	SV_ACCEPT_LOGOUT,		// 로그아웃을 허용함
	SV_HEARTBEAT,			// 허트비트
	SV_ASK_OUTCHAN,			// 채널 퇴장을 요구
	SV_ACCEPT_OUTCHAN,		// 채널 퇴장을 허가
	SV_ASK_ENTERCHAN,		// 채널 입장을 요구
	SV_ACCEPT_ENTERCHAN,	// 채널 입장을 허가
	SV_ASK_FINDID,			// 아이디 찾기 요청
	SV_RESULT_FINDID,		// 아이디 찾기 결과
	SV_ASK_DETAILUSERINFO,	// 사용자의 상세 정보를 요구(개발자용)
	SV_DETAILUSERINFO,		// 사용자의 상세 정보
	SV_USERINFO,			// 사용자 정보를 보내줌(대상 : 특정 사용자 정보 요구시)
	SV_CHANGEMYINFO,		// 변경된 사용자 정보를 알려줌
	SV_ALLUSERINFO,			// 모든 접속자 목록을 보내준다(대상 : 요구하는 사용자)
	SV_NEWUSER,				// 새로운 사용자가 접속되었음(대상 : 전체 사용자)
	SV_OUTUSER,				// 사용자가 접속종료하였음(대상 : 전체 사용자)
	SV_CHANGEUSER,			// 접속한 다른 사용자의 정보가 변경되었음(대상: 전체사용자)
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_ACCEPT_CHANGECHAR=330,	// 사용자 캐릭터를 바꾸기를 허락
	SV_REFUSE_CREATEROOM,	// 방 만들기를 거부함(대상: 방 개설을 시도한 사용자)
							// 실패한 원인 종류 코드(0:방 개설 제한 수 초과, 1:시험판 아이디)
	SV_ACCEPT_CREATEROOM,	// 방 만들기 요구에 의해 방이 만들어졌음(대상: 방 개설을 시도한 사용자)
							// 방장ID, 방 제목, 방 종류(협동, 대전)
	SV_ALLROOMINFO,			// 개설된 방의 전체 정보를 보내준다(대상 : 요구하는 사용자)
	SV_ROOMINFO,			// 특정 방의 정보를 보내준다(대상 : 요구하는 사용자)
	SV_ROOMMASTER,			// 특정한 방의 방장 정보를 보내준다(대상 : 요구하는 사용자)
	SV_NEWROOM,				// 새로운 방이 개설되었음을 알림
	SV_OUTROOM,				// 방이 사라졌음을 알림
	SV_CHANGEROOM,			// 방 정보가 수정되었음을 알림
	SV_USEROUTROOM,			// 사용자가 방에서 나왔다 - 같은 방 플레이어들에게 통보
	SV_ACCEPT_OUTROOM,		// 방에서 나가기를 허가
	SV_CHANGEROOMMASTER,	// 방장이 바뀜을 알린다
	SV_ABNORMALOUTROOM,		// 사용자에게 비정상적으로 퇴장하기를 요청(방장과의 재접속에 실패했을 경우)

	// ### [사운드 추가 작업] ###
	SV_ASK_CHANGESNDFX,     // 요청
	SV_ACCEPT_CHANGESNDFX,  // 허락

	/////////////////// 서버가 받는 메세지 ///////////////////

	SV_ASK_BADUSER,			// 채팅창 대화내용 신고 요청
	SV_ASK_CHANGEMYINFO,	// 나의 사용자 정보 수정을 요구
	SV_ASK_ALLCHANINFO,		// 전체 채널 정보를 요구
	SV_ALLCHANINFO,			// 전체 채널 정보
	SV_ASK_ALLROOMINFO,		// 전체 방 목록을 요구
	SV_ASK_ROOMINFO,		// 특정 방정보를 요구
	SV_ASK_ROOMMASTER,		// 특정 방의 방장 정보를 요구
	SV_ASK_ALLUSERINFO,		// 모든 접속자 목록을 요구
	SV_ASK_USERINFO,		// 특정 사용자 정보를 요구
	SV_ASK_CHANGECHAR,		// 사용자 캐릭터를 바꾸기를 요구
	SV_ASK_CREATEROOM,		// 방 만들기를 요구
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_ASK_ENTERROOM=555,		// 방에 입장하기를 요구
	SV_ACCEPT_ENTERROOM,	// 방에 입장하기를 허가함(대상: 입장을 요구한 사용자)
	SV_REFUSE_ENTERROOM,	// 방 입장요청을 거부함(대상: 입장을 요구한 사용자)
	SV_USERENTERROOM,		// 사용자가 방에 입장하였음
	SV_LOBYOPEN,			// 대기실 보기 초대창 오픈 상태 알림
	SV_ASK_OUTROOM,			// 방에서 나가기를 요구
	SV_ASK_INVITE,			// 초청 메세지를 보냄
	SV_REFUSE_INVITE,		// 초청 메세지를 받음
	SV_ASK_BANISHVOTE,		// 추방 요청
	SV_MYBANISHVOTE,		// 나의 추방 투표 결과
	SV_NOWHISPER,			// 귓말 거부,허가 상태 변경
	SV_ASK_FORCEBANISH,		// 강제 추방 요청	[게임 아이템 작업]

	/////////////////// 게임 아이템 메세지 [게임 아이템 작업] ///////////////////

	SV_ASK_GAMEITEMLIST,	// 게임 아이템 목록 갱신을 요청
	SV_GAMEITEMLIST,		// 게임 아이템 목록을 보내줌
	SV_USEGAMEITEM,			// 게임 아이템 사용 요청
	SV_BUYGAMEITEM,			// 게임 아이템을 구입할것인지 물어봄
	
	/////////////////// 공통 메세지 ///////////////////

	SV_SERVERMSG,			// 서버 메세지
	SV_CHATDATA,			// 대기실용 채팅 데이타(주의: 보내고 받을때 모두 같은 메세지 사용)
	SV_WHISPERCHAT,			// 귓속말

	/////////////////// 게임 메세지 ///////////////////

	SV_STARTGAME,			// 게임이 시작되었음
	SV_GAMEOVERRESULT,		// 게임 종료되었을때 결과를 통보(대상 : 같은 방 플레이어 모두)
	SV_GAMEOVER,			// 서버에게 게임이 종료됨을 통보(방장이 보냄)
							// 방번호, ID, 푼 문제수, 남은 총알의 종류별 개수(참여한 사람 수 만큼)
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_STARTCARD=666,		// 처음 3개의 카드를 날림
	SV_SELECTCARD,			// 카드 타이머 제거	
	SV_CHANGECARD,			// 카드 바꾸기를 요청
	SV_BUYCARD,				// 카드 사기를 요청
	SV_SPECIALCARD,			// 모든 서버 접속자에게 알림
	SV_HIDDENCARD,			// 마지막 히든 카드 날림
	SV_RAISE,				// 서버 알림 레이즈
	SV_WINDCARDEND,			// 첫 3장을 보낸후 유저가 하나의 카드를 선택 => 모두 선택시 게임시작
	SV_ENDGAME,				// 판종료 알림
	SV_ENDRAISE,			// 모든 레이즈 끝 => 하이 로우 스윙 결정 요청
	
	SV_FOLDUSER,			// 유저가 폴드함
	SV_ASK_QUICKSTART,		// 게임 바로 시작
	SV_REFUSE_QUICKSTART,	// 바로시작 안됨 (방만들기)

	/////////////////// ### [이벤트 티켓] ### ///////////////////
	SV_EVENTPRIZE,			// 이벤트 당첨자 알림  
	SV_EVENTNOTICE,			// 이벤트 공지 알림(방 입장시)

	/////////////////// ## [62cut 추가 메세지] ////////////////////

	SV_ASK_CHANGECARD_62CUT,			// 유저가 가지고 있는 카드 바꾸기 ( 한장씩 뿌리
	SV_CHANGECARD_62CUT_RESULT,			// 유저가 가지고 있는 카드 바꾸기
	SV_ASK_CHANGECARD_62CUT_EX,			// 62cut 바꿀 카드 넘김 (동시에 뿌리기	

	////////////////////////////////////////////////////////////////
	SV_ACTIVE_STARTBUTTON,			// 시작 버튼 활성화

	SV_LEADERSJOIN,			//2004.05.07 리더스총액 제한

	/////////////////// ### [ 관전기능 ] ###  ///////////////////

	SV_ASK_PARTICIPATION=750,   // 겜참여 요청
	SV_ACCEPT_PARTICIPATION, // 참여 
	SV_ENTER_PARTICIPATION,  // 다른 사용자에게 알린다
	SV_ASK_EMERAGE,         // 겜참여에서 관전으로
	SV_ACCEPT_EMERAGE,      // 관전
	SV_ENTER_EMERAGE,		// 다른 사용자에게 알린다
	SV_RESERVATIONINFO,     // 예약자 정보
	SV_OBSERVER_MESSAGE,	// 관전 관련 메세지

	/////////////////// ### [관리자 모드 작업] ###  ///////////////////

	NM_ADMIN_WHISPERCHAT,	// 귓말하기(강제)
	NM_ADMIN_WARNUSER,		// 사용자 관리(운영자)
	NM_ADMIN_OUTUSER,		// 사용자 제거하기(운영자)
	NM_ADMIN_CHANGEROOM,	// 겜방 옵션 변경요청(운영자)
	NM_ADMIN_DELROOM,		// 방제거하기(운영자)
	SV_CHANGESTYLEROOMINFO, // 방이름 변경 알림(운영자)
	NM_SPECIALMESSAGE,      // 특별 관리 메세지(운영자)
	
	SV_SERVERNOTIC,			// 서버 공지 메세지

	SV_ASK_SAFEANGEL,		// [수호천사] 2004.07.08 사용할것이냐 
	CL_CHARGE_SAFEANGEL,	// [수호천사] 2004.07.08 머니 충전 메시지
};

// 클라이언트용 메세지 #########################################################

enum
{
	/////////////////// 공통 메세지 ///////////////////
	
	CL_EXITSUBSCRIPT=983,	// 플레이어의 나가기 예약 상태
	CL_CHATDATA,			// 클라이언트간의 채팅 메세지

	/////////////////// 게임 진행 관련 메세지 /////////

	CL_READYTOSTART,		// 게임 시작 준비 완료(재시작할때 사용함)
	CL_ASK_CHANGEGAMEKIND,	// 게임 방식 바꿈 요청(0:아이템 1:노템)
	CL_CHANGEGAMEKIND,		// 게임 방식 바꿈 결과(0:아이템 1:노템)
	CL_ASK_STARTGAME,		// 게임 시작 요청
	CL_STARTINFO,			// 게임 시작 정보(대상: 전체)
	
	CL_ASK_FOLD,			// 플레이어 폴드
	CL_ASK_SELECTCARD,		// 사용자의 카드선택
	CL_ASK_RAISE,			// 사용자의 레이즈
	CL_WINDCARDEND,			// 사용자의 카드날리기 종료
	CL_ROOM_TIMEOUT,		// 사용자 턴 타임 아웃 메세지 (인한 방에서 나가기)	
	CL_USERRESULT,			// 폴드한 사용자 유저정보
	CL_CARDCHANGE_62CUT_RESULT,	// 카드 바꾸기 결과값 보내기
	CL_CARDCHANGE_END_62CUT,	//카드 바꾸기 종료 레이스 요청
	CL_ASK_CHSTYLEROOMINFO,	// 방정보 변경 요청 

	CL_SELECTWINCASE,		// 어떤걸로 승부를 겨루겠는가 하이 로우 스윙

	// [ 게임 아이템 ]
	CL_ITEMRESULT,			// 아이템 결과값..( 에러값이 있을 경우에만.. )
	CL_ITEMLIST,			// 아이템 리스트..
	CL_ITEMLOGIN,			// 아이템을 사용한 로그인..
	CL_ASK_ITEMLOGIN,		// 아이템 사용할 것인가 물어봄.
	CL_ACCEPT_ITEMLOGIN,	// 아이템 서버 로그인 허가.

	// [점프 아이템 작업]
	CL_ASK_JUMPITEM,		// 채널에서 점프아이템 사용 물어봄.
	CL_JUMPITEM_LOGIN,		// 채널에서 점프아이템 사용 로긴
	CL_ACCEPT_JUMPITEM_CH,  // 유저 -> 서버 요청 DB OK => 유저결과(아이템 다이얼로그 박스만 보여준다)
	CL_ACCEPT_JUMPITEM,		// 채널에서 점프아이템 한번이라도 사용한 아템이 있을때(바로 입장)


	// [서버 에이전트 작업] 
	NM_AGENT_CONNECT = 5005, // 에이전트가 접속되었다
	NM_AGENT_NEWUSER,
	NM_AGENT_OUTUSER,
	NM_AGENT_NEWUSERERROR,	 // 중복 요청 
	NM_AGENT_USERLIST,		 // 유저 목록 요청
	NM_AGENT_DUBLECON,		 // 유저 목록 요청
	NM_AGENT_USERLIST1,
	NM_AGENT_USERLIST2,
	NM_AGENT_USERLIST3,
	NM_AGENT_USERLIST4,
	NM_AGENT_USERLIST5,
	NM_AGENT_USERLIST_R1,
	NM_AGENT_AVERAGE,       // 잭팟 금액의 평균을 구한다.
	//### [관리자 모드 작업] ###
	NM_AGENT_ADMININFO,		// 관리자 권한 아이디를 전송한다.

};

// 통신 메세지 기본 클래스 #########################################################
class CCommMsg
{
public:
	BOOL bReferedData;			// 참조된 데이터인가? (참조된 데이터는 메모리 포인터를 해제할 수 없다)
	int  TotalSize;				// 헤더를 포함한 데이터의 총 사이즈
	char *pData;				// 데이터 포인터(헤더 포함)
	PACKETHEADER *pHead;		// 패킷 헤더 포인터

	CCommMsg();					// 버퍼가 참조되지 않는 생성자(버퍼를 동적 할당하게됨)
	CCommMsg(char *pRefBuf);	// 버퍼를 참조하여 클래스를 생성하는 생성자
	~CCommMsg();

	void Clear();
	int GetTotalSize();

	static void BuildPattern(char *pdata, UINT packcnt, int size);
	static void EncodeAll(char *pdata);
	static BOOL DecodeAll(char *pdata);

	// 메모리 버퍼의 데이터로부터 헤더 메시지를 분석하여 필요한 정보를 알려줌
	// (lpdata는 최소한 헤더가 생성되는데 필요한 기본사이즈 이상 되어야한다)
	BOOL CheckInfo(char *lpdata, int *psignal, int *pmsglen);

	// 헤더를 만들어주고 필요한 사이즈만큼 메모리를 할당한다
	// (다음에 기록해야할 포인터를 반환한다)
	char* SetHeader(int signal, int msglen); 

	// 받은 데이터를 이용하여 헤더를 세팅한다(다음에 처리할 포인터를 반환한다)
	// 헤더를 포함한 데이터의 첫번째 포인터와 데이터의 전체 사이즈를 받는다
	char* GetHeader(char *lpdata, int size);

	// 메모리를 복사한만큼 타겟 포인터를 증가시켜준다(주의: 메모리가 할당된 포인터를 직접 넣지 말것)
	// 복사하기전(증가 되기전)의 포인터를 리턴한다
	char* SmartCpy(char **tar, const void *src, int size);

	// tar포인터를 증가시켜준다
	// 증가 시키기전의 포인터를 리턴한다
	char* Jump(char **tar, int size);
};

////////////////////////////////////////////////////////////////////////////
//                      서버용 메세지 클래스
////////////////////////////////////////////////////////////////////////////

/////////// 버젼을 체크함
class CSV_CHECKVERSION : public CCommMsg
{
public:
	CSV_CHECKVERSION() : CCommMsg() {}
	CSV_CHECKVERSION(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Ver;
	int *PreVersion;
	BOOL Set(int ver, int prversion);
	BOOL Get(char* lpdata, int size);
};

/////////// 서버 정보	[게임 아이템 작업]
class CSV_SERVERINFO : public CCommMsg
{
public:
	CSV_SERVERINFO() : CCommMsg() {}
	CSV_SERVERINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SERVERINFO *SI;
	BOOL Set(SERVERINFO *pLI);
	BOOL Get(char* lpdata, int size);
};

/*
/////////// 로그인 요청 정보(ID, Password, Game Version)
class CSV_LOGIN : public CCommMsg
{
public:
	CSV_LOGIN() : CCommMsg() {}
	CSV_LOGIN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;
	char *Pass;
	BOOL *Over;
	char *SockNameIP;
	char *HostNameIP;
	int *Port;
	BOOL Set(char *id, char *pass, BOOL bOver, char *socknameip, char *hostnameip, int port);
	BOOL Get(char* lpdata, int size);
};
*/

//2004.05.07 리더스총액 제한
class CSV_LEADERSJOIN : public CCommMsg
{
public:
	CSV_LEADERSJOIN() : CCommMsg() {}
	CSV_LEADERSJOIN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	LEADERS_JOIN *LJ;
	BOOL Set(LEADERS_JOIN* pLJ);
	BOOL Get(char* lpdata, int size);
};




// [SSO 작업] - 추가
/////////// 로그인 요청 정보(ID, Password, Game Version)
class CSV_ASK_LOGIN : public CCommMsg
{
public:
	CSV_ASK_LOGIN() : CCommMsg() {}
	CSV_ASK_LOGIN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ASK_LOGININFO *pAL;
	char *AuthCookie;
	char *DataCookie;
	BOOL Set(ASK_LOGININFO *pal, char* szAuthCookie, char* szDataCookie);
	BOOL Get(char* lpdata, int size);
};

/////////// 로그인을 거부함(거부 이유 코드를 보냄)	[SSO 작업] - 교체
class CSV_REFUSE_LOGIN : public CCommMsg
{
public:
	CSV_REFUSE_LOGIN() : CCommMsg() {}
	CSV_REFUSE_LOGIN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Code;
	int *l_StrMsg;
	char *StrMsg;
	BOOL Set(int code, char* strmsg);
	BOOL Get(char* lpdata, int size);
};


/////////// 로그인을 허용함(사용자 정보를 보냄)
class CSV_ACCEPT_LOGIN : public CCommMsg
{
public:
	CSV_ACCEPT_LOGIN() : CCommMsg() {}
	CSV_ACCEPT_LOGIN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	LOGININFO *LI;
	BOOL Set(LOGININFO *pLI);
	BOOL Get(char* lpdata, int size);
};


/*
/////////// 로그인을 거부함(거부 이유 코드를 보냄)
class CSV_REFUSE_LOGIN : public CCommMsg
{
public:
	CSV_REFUSE_LOGIN() : CCommMsg() {}
	CSV_REFUSE_LOGIN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Code;
	BOOL Set(int code); // 0:아이디가 틀림, 1:비밀번호가 틀림, 2:버전이 틀림
	BOOL Get(char* lpdata, int size);
};
*/
/////////// 로그아웃을 요청
class CSV_ASK_LOGOUT : public CCommMsg
{
public:
	CSV_ASK_LOGOUT() : CCommMsg() {}
	CSV_ASK_LOGOUT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *UNum;
	char *ID;
	BOOL Set(int unum, char* id);
	BOOL Get(char* lpdata, int size);
};

/////////// 로그아웃을 허용
class CSV_ACCEPT_LOGOUT : public CCommMsg
{
public:
	CSV_ACCEPT_LOGOUT() : CCommMsg() {}
	CSV_ACCEPT_LOGOUT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// 허트비트
class CSV_HEARTBEAT : public CCommMsg
{
public:
	CSV_HEARTBEAT() : CCommMsg() {}
	CSV_HEARTBEAT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// 채널 입장을 요청
class CSV_ASK_ENTERCHAN : public CCommMsg
{
public:
	CSV_ASK_ENTERCHAN() : CCommMsg() {}
	CSV_ASK_ENTERCHAN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *ChNum;
	BOOL Set(int cnum);
	BOOL Get(char* lpdata, int size);
};

/////////// 채널 입장을 허용
class CSV_ACCEPT_ENTERCHAN : public CCommMsg
{
public:
	CSV_ACCEPT_ENTERCHAN() : CCommMsg() {}
	CSV_ACCEPT_ENTERCHAN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ENTERCHANINFO *EC;
	BOOL Set(ENTERCHANINFO *pEC);
	BOOL Get(char* lpdata, int size);
};

/////////// 채널 퇴장을 요청
class CSV_ASK_OUTCHAN : public CCommMsg
{
public:
	CSV_ASK_OUTCHAN() : CCommMsg() {}
	CSV_ASK_OUTCHAN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// 채널 퇴장을 허용
class CSV_ACCEPT_OUTCHAN : public CCommMsg
{
public:
	CSV_ACCEPT_OUTCHAN() : CCommMsg() {}
	CSV_ACCEPT_OUTCHAN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// 아이디 찾기 요청
class CSV_ASK_FINDID : public CCommMsg
{
public:
	CSV_ASK_FINDID() : CCommMsg() {}
	CSV_ASK_FINDID(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;
	BOOL Set(char* id);
	BOOL Get(char* lpdata, int size);
};

/////////// 아이디 찾기 결과
class CSV_RESULT_FINDID : public CCommMsg
{
public:
	CSV_RESULT_FINDID() : CCommMsg() {}
	CSV_RESULT_FINDID(char *pRefBuf) : CCommMsg(pRefBuf) {}

	RESULTFINDID *RF;
	BOOL Set(RESULTFINDID *pRF);
	BOOL Get(char* lpdata, int size);
};

/////////// 대화내용 신고요청
class CSV_ASK_BADUSER : public CCommMsg
{
public:
	CSV_ASK_BADUSER() : CCommMsg() {}
	CSV_ASK_BADUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL *bAnnounce;	// 신고 알림 여부
	int *RNum;			// 방번호(0이면 대기실)
	int *UNum;			// 신고인 유저번호
	char *ID;			// 신고인 아이디
	char *TargetID;		// 신고대상 아이디
	char *SubmitReason;	// 신고사유
	int *l_StrWatchID;	// 증인 아이디 스트링의 길이
	char *WatchID;		// 증인 아이디 스트링
	int *l_StrChat;		// 신고될 채팅 스트링의 길이
	char *StrChat;		// 신고될 채팅 스트링
	BOOL Set(BOOL announce, int rnum, int unum, char* id, char *tarid, char *reason, char *watchid, char *chat);
	BOOL Get(char* lpdata, int size);
};

/////////// 전체 채널 목록을 요구한다
class CSV_ASK_ALLCHANINFO : public CCommMsg
{
public:
	CSV_ASK_ALLCHANINFO() : CCommMsg() {}
	CSV_ASK_ALLCHANINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// 전체 채널 목록을 보내준다
class CSV_ALLCHANINFO : public CCommMsg
{
public:
	CSV_ALLCHANINFO() : CCommMsg() {}
	CSV_ALLCHANINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	CHANNELINFO *ArrayCI;
	BOOL Set(int totnum, CHANNELINFO* ppCI);
	BOOL Get(char* lpdata, int size);
};

/////////// 전체 사용자 목록을 요구한다
class CSV_ASK_ALLUSERINFO : public CCommMsg
{
public:
	CSV_ASK_ALLUSERINFO() : CCommMsg() {}
	CSV_ASK_ALLUSERINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// 전체 사용자 목록을 보내준다
class CSV_ALLUSERINFO : public CCommMsg
{
public:
	CSV_ALLUSERINFO() : CCommMsg() {}
	CSV_ALLUSERINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	SMALLUSERINFO *ArraySUI;
	BOOL Set(int totnum, SMALLUSERINFO* ppSUI);
	BOOL Get(char* lpdata, int size);
};

/////////// 특정 사용자의 상세 정보를 요구한다
class CSV_ASK_DETAILUSERINFO : public CCommMsg
{
public:
	CSV_ASK_DETAILUSERINFO() : CCommMsg() {}
	CSV_ASK_DETAILUSERINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *UNum;
	char *ID;
	BOOL Set(int unum, char* id);
	BOOL Get(char* lpdata, int size);
};

/////////// 특정 사용자의 상세 정보를 보내줌
class CSV_DETAILUSERINFO : public CCommMsg
{
public:
	CSV_DETAILUSERINFO() : CCommMsg() {}
	CSV_DETAILUSERINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	USERINFO *UI;
	BOOL Set(USERINFO *pUI);
	BOOL Get(char* lpdata, int size);
};


/////////// 특정 사용자 정보을 요구한다
class CSV_ASK_USERINFO : public CCommMsg
{
public:
	CSV_ASK_USERINFO() : CCommMsg() {}
	CSV_ASK_USERINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *UNum;
	char *ID;
	BOOL Set(int unum, char* id);
	BOOL Get(char* lpdata, int size);
};

/////////// 특정 사용자 정보를 보내줌
class CSV_USERINFO : public CCommMsg
{
public:
	CSV_USERINFO() : CCommMsg() {}
	CSV_USERINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	USERINFO *UI;
	BOOL Set(USERINFO *pUI);
	BOOL Get(char* lpdata, int size);
};

/////////// 새로운 사용자가 접속했음을 알린다
class CSV_NEWUSER : public CCommMsg
{
public:
	CSV_NEWUSER() : CCommMsg() {}
	CSV_NEWUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SMALLUSERINFO *SUI;
	BOOL Set(SMALLUSERINFO* pSUI);
	BOOL Get(char* lpdata, int size);
};

/////////// 사용자가 접속을 종료했음을 알린다
class CSV_OUTUSER : public CCommMsg
{
public:
	CSV_OUTUSER() : CCommMsg() {}
	CSV_OUTUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SMALLUSERINFO *SUI;
	BOOL Set(SMALLUSERINFO* pSUI);
	BOOL Get(char* lpdata, int size);
};

/////////// 다른 사용자의 접속 정보가 변했음
class CSV_CHANGEUSER : public CCommMsg
{
public:
	CSV_CHANGEUSER() : CCommMsg() {}
	CSV_CHANGEUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SMALLUSERINFO *SUI;
	BOOL Set(SMALLUSERINFO* pSUI);
	BOOL Get(char* lpdata, int size);
};

/////////// 전체 방 목록을 요구한다
class CSV_ASK_ALLROOMINFO : public CCommMsg
{
public:
	CSV_ASK_ALLROOMINFO() : CCommMsg() {}
	CSV_ASK_ALLROOMINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// 전체 방 목록을 보내준다
class CSV_ALLROOMINFO : public CCommMsg
{
public:
	CSV_ALLROOMINFO() : CCommMsg() {}
	CSV_ALLROOMINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	ROOMINFO *ArrayRI;
	BOOL Set(int totnum, ROOMINFO* ppRI);
	BOOL Get(char* lpdata, int size);
};

/////////// 방 정보를 요구
class CSV_ASK_ROOMINFO : public CCommMsg
{
public:
	CSV_ASK_ROOMINFO() : CCommMsg() {}
	CSV_ASK_ROOMINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	BOOL Set(int roomnum);
	BOOL Get(char* lpdata, int size);
};

/////////// 방 정보
class CSV_ROOMINFO : public CCommMsg
{
public:
	CSV_ROOMINFO() : CCommMsg() {}
	CSV_ROOMINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *ErrCode;
	GAMEINFO *GI;
	BOOL Set(int errcode, GAMEINFO *pGI);
	BOOL Get(char* lpdata, int size);
};

/////////// 캐릭터 바꾸기를 요청
class CSV_ASK_CHANGECHAR : public CCommMsg
{
public:
	CSV_ASK_CHANGECHAR() : CCommMsg() {}
	CSV_ASK_CHANGECHAR(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *UNum;
	char *ID;
	int *CharNum;
	BOOL Set(int unum, char *id, int charnum);
	BOOL Get(char* lpdata, int size);
};

/////////// 캐릭터 바꾸기를 요청 허락
class CSV_ACCEPT_CHANGECHAR : public CCommMsg
{
public:
	CSV_ACCEPT_CHANGECHAR() : CCommMsg() {}
	CSV_ACCEPT_CHANGECHAR(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *CharNum;
	BOOL Set(int charnum);
	BOOL Get(char* lpdata, int size);
};

/////////// 방 개설 요구
class CSV_ASK_CREATEROOM : public CCommMsg
{
public:
	CSV_ASK_CREATEROOM() : CCommMsg() {}
	CSV_ASK_CREATEROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ROOMINFO *RI;
	UINT *Port;
	BOOL Set(ROOMINFO* pRI, UINT port);
	BOOL Get(char* lpdata, int size);
};

/////////// 방 개설 거부
class CSV_REFUSE_CREATEROOM : public CCommMsg
{
public:
	CSV_REFUSE_CREATEROOM() : CCommMsg() {}
	CSV_REFUSE_CREATEROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Code;
	BOOL Set(int code);
	BOOL Get(char* lpdata, int size);
};

/////////// 방 개설 허용
class CSV_ACCEPT_CREATEROOM : public CCommMsg
{
public:
	CSV_ACCEPT_CREATEROOM() : CCommMsg() {}
	CSV_ACCEPT_CREATEROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ROOMINFO *RI;
	BOOL Set(ROOMINFO* pRI);
	BOOL Get(char* lpdata, int size);
};

/////////// 방 입장을 요청
class CSV_ASK_ENTERROOM : public CCommMsg
{
public:
	CSV_ASK_ENTERROOM() : CCommMsg() {}
	CSV_ASK_ENTERROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	int *SndFxKind; // ### [사운드 추가 작업] ###
	char *ID;
	char *Pass;
	BOOL Set(int rnum, int unum, int sndKind, char *id, char *pass=NULL);// 추가
	BOOL Get(char* lpdata, int size);
};

/////////// 입장을 허가함
class CSV_ACCEPT_ENTERROOM : public CCommMsg
{
public:
	CSV_ACCEPT_ENTERROOM() : CCommMsg() {}
	CSV_ACCEPT_ENTERROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ENTERROOMINFO *EI;
//	int *FieldNum;
//	BLOCKFIELDINFO *pBF;
	BOOL Set(ENTERROOMINFO *pEI);//, int fieldnum, BLOCKFIELDINFO *pbf);
	BOOL Get(char* lpdata, int size);
};

/////////// 입장 요청을 거부함
class CSV_REFUSE_ENTERROOM : public CCommMsg
{
public:
	CSV_REFUSE_ENTERROOM() : CCommMsg() {}
	CSV_REFUSE_ENTERROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Code;
	BOOL Set(int code); // 0:인원초과 1:비밀번호가 틀림
	BOOL Get(char* lpdata, int size);
};

/////////// 새로운 방이 개설되었음
class CSV_NEWROOM : public CCommMsg
{
public:
	CSV_NEWROOM() : CCommMsg() {}
	CSV_NEWROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ROOMINFO *RI;
	BOOL Set(ROOMINFO* pRI);
	BOOL Get(char* lpdata, int size);
};

/////////// 방이 사라졌음
class CSV_OUTROOM : public CCommMsg
{
public:
	CSV_OUTROOM() : CCommMsg() {}
	CSV_OUTROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SMALLROOMINFO *SRI;
	BOOL Set(SMALLROOMINFO* pSRI);
	BOOL Get(char* lpdata, int size);
};

/////////// 방 정보가 변했음
class CSV_CHANGEROOM : public CCommMsg
{
public:
	CSV_CHANGEROOM() : CCommMsg() {}
	CSV_CHANGEROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SMALLROOMINFO *SRI;
	BOOL Set(SMALLROOMINFO* pSRI);
	BOOL Get(char* lpdata, int size);
};

/////////// 방장이 바뀜
class CSV_CHANGEROOMMASTER : public CCommMsg
{
public:
	CSV_CHANGEROOMMASTER() : CCommMsg() {}
	CSV_CHANGEROOMMASTER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	char *ID;
	BOOL Set(int rnum, int unum, char *id);
	BOOL Get(char* lpdata, int size);
};

// ### [ 중복된 IP가 ] ### // ### [사운드 추가 작업] ###
/////////// 방에 사용자가 입장하였음
class CSV_USERENTERROOM : public CCommMsg
{
public:
	CSV_USERENTERROOM() : CCommMsg() {}
	CSV_USERENTERROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	USERINFO *UI;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	int *SndFxKind; // ### [사운드 추가 작업] ###
	BOOL Set(int rnum, USERINFO *pUI, IPARRINDEX *pIPA, int sndKind);
	BOOL Get(char* lpdata, int size);
};

// ### [ 중복된 IP가 ] ###
/////////// 사용자가 방에서 나갔음
class CSV_USEROUTROOM : public CCommMsg
{
public:
	CSV_USEROUTROOM() : CCommMsg() {}
	CSV_USEROUTROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	BOOL Set(int rnum, int unum, char *id, IPARRINDEX *pIPA);
	BOOL Get(char* lpdata, int size);
};

/////////// 사용자가 방에서 나가기를 요청
class CSV_ASK_OUTROOM : public CCommMsg
{
public:
	CSV_ASK_OUTROOM() : CCommMsg() {}
	CSV_ASK_OUTROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	char *ID;
	BOOL Set(int rnum, int unum, char *id);
	BOOL Get(char* lpdata, int size);
};

/////////// 사용자가 방에서 나가기를 허가
class CSV_ACCEPT_OUTROOM : public CCommMsg
{
public:
	CSV_ACCEPT_OUTROOM() : CCommMsg() {}
	CSV_ACCEPT_OUTROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// 나의 사용자 정보가 변경됨
class CSV_CHANGEMYINFO : public CCommMsg
{
public:
	CSV_CHANGEMYINFO() : CCommMsg() {}
	CSV_CHANGEMYINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGEMYINFO *pCMI;
	BOOL Set(CHANGEMYINFO *pcmi);
	BOOL Get(char* lpdata, int size);
};

/////////// 비정상적인 퇴장(방장과의 재접속에 실패했을 경우)
class CSV_ABNORMALOUTROOM : public CCommMsg
{
public:
	CSV_ABNORMALOUTROOM() : CCommMsg() {}
	CSV_ABNORMALOUTROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

////////// 대기실 보기 초대창 오픈 상태 알림
class CSV_LOBYOPEN : public CCommMsg
{
public:
	CSV_LOBYOPEN() : CCommMsg() {}
	CSV_LOBYOPEN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL *bLobyOpen;
	BOOL Set(BOOL blobyopen);
	BOOL Get(char* lpdata, int size);
};

/////////// 초청 메세지를 보냄
class CSV_ASK_INVITE : public CCommMsg
{
public:
	CSV_ASK_INVITE() : CCommMsg() {}
	CSV_ASK_INVITE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TarUNum;		// 대상 유저 번호
	char *TarID;		// 대상 아이디
	int *RoomNum;		// 방 번호
	char *RoomPass;		// 방 비밀번호
	USERINFO *MyUI;		// 나의 사용자 정보
	int *l_StrMsg;		// 전할말의 길이
	char *StrMsg;		// 전할말
	BOOL Set(int targetunum, char* targetid, int rnum, char *pass, USERINFO *pMyUI, char *strmsg);
	BOOL Get(char* lpdata, int size);
};

/////////// 초청 메세지를 거부함
class CSV_REFUSE_INVITE : public CCommMsg
{
public:
	CSV_REFUSE_INVITE() : CCommMsg() {}
	CSV_REFUSE_INVITE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TarUNum;		// 대상 유저 번호
	char *TarID;		// 대상 아이디
	char *MyID;			// 나의 아이디
	int *l_StrMsg;		// 전할말의 길이
	char *StrMsg;		// 전할말
	BOOL Set(int targetunum, char* targetid, char *myid, char *strmsg);
	BOOL Get(char* lpdata, int size);
};

/////////// 추방 투표 요청
class CSV_ASK_BANISHVOTE : public CCommMsg
{
public:
	CSV_ASK_BANISHVOTE() : CCommMsg() {}
	CSV_ASK_BANISHVOTE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BANISHVOTEINFO *pBV;
	BOOL Set(BANISHVOTEINFO *pbv);
	BOOL Get(char* lpdata, int size);
};

/////////// 나의 추방 투표 결과
class CSV_MYBANISHVOTE : public CCommMsg
{
public:
	CSV_MYBANISHVOTE() : CCommMsg() {}
	CSV_MYBANISHVOTE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL *bAgree;
	BOOL Set(BOOL bagree);
	BOOL Get(char* lpdata, int size);
};


/////////// 서버 알림 메세지
class CSV_SERVERMSG : public CCommMsg
{
public:
	CSV_SERVERMSG() : CCommMsg() {}
	CSV_SERVERMSG(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Kind;				// 종류(0:기본 출력 1:메세지 박스 출력)
	short *l_StrChat;		// 채팅 스트링의 길이
	char *StrChat;			// 채팅 스트링
	BOOL *bColorAssigned;	// 텍스트 컬러가 지정되었는가?
	COLORREF *Color;		// 텍스트 컬러

	BOOL Set(int kind, char *chat, BOOL bcolorassigned = FALSE, COLORREF color = RGB(255,0,0));
	BOOL Get(char* lpdata, int size);
};

/////////// 귓말 허용 상태 변경
class CSV_NOWHISPER : public CCommMsg
{
public:
	CSV_NOWHISPER() : CCommMsg() {}
	CSV_NOWHISPER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL *bNoWhisper;
	BOOL Set(BOOL bnowhisper);
	BOOL Get(char* lpdata, int size);
};

/////////// 대기실용 채팅 데이타
class CSV_CHATDATA : public CCommMsg
{
public:
	CSV_CHATDATA() : CCommMsg() {}
	CSV_CHATDATA(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;			// 아이디 스트링
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링

	BOOL Set(char* id, char *chat);
	BOOL Get(char* lpdata, int size);
};

/////////// 귓속말
class CSV_WHISPERCHAT : public CCommMsg
{
public:
	CSV_WHISPERCHAT() : CCommMsg() {}
	CSV_WHISPERCHAT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;			// 보내는 사람 아이디 스트링
	int  *DestUNum;		// 받는 사람 사용자 번호
	char *DestID;		// 받는 사람 아이디
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링
	BOOL Set(char *id, int destunum, char *destid, char *chat);
	BOOL Get(char* lpdata, int size);
};

////////// 강제 추방 요청	[게임 아이템 작업]
class CSV_ASK_FORCEBANISH : public CCommMsg
{
public:
	CSV_ASK_FORCEBANISH() : CCommMsg() {}
	CSV_ASK_FORCEBANISH(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;
	BOOL Set(char* id);
	BOOL Get(char* lpdata, int size);
};

		/////////////// 게임 아이템 메시지 [게임 아이템 작업] ////////////////

/////////// 게임 아이템 리스트를 요청
class CSV_ASK_GAMEITEMLIST : public CCommMsg
{
public:
	CSV_ASK_GAMEITEMLIST() : CCommMsg() {}
	CSV_ASK_GAMEITEMLIST(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};


/////////// 게임 아이템 리스트를 보내준다
class CSV_GAMEITEMLIST : public CCommMsg
{
public:
	CSV_GAMEITEMLIST() : CCommMsg() {}
	CSV_GAMEITEMLIST(char *pRefBuf) : CCommMsg(pRefBuf) {}

	GAMEITEM_INFO* pIN;
	GAMEITEM *ArrayGI;
	char *StrMsg1;
	char *StrMsg2;

	BOOL Set(GAMEITEM_INFO* pin, GAMEITEM* ppGI, LPCTSTR strmsg1, LPCTSTR strmsg2);
	BOOL Get(char* lpdata, int size);
};

/////////// 게임 아이템을 사용(요청, 결과 모두)
class CSV_USEGAMEITEM : public CCommMsg
{
public:
	CSV_USEGAMEITEM() : CCommMsg() {}
	CSV_USEGAMEITEM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	USEGAMEITEMINFO *pUG;

	BOOL Set(USEGAMEITEMINFO *pug);
	BOOL Get(char* lpdata, int size);
};

/////////// 게임 아이템 구입을 물어봄
class CSV_BUYGAMEITEM : public CCommMsg
{
public:
	CSV_BUYGAMEITEM() : CCommMsg() {}
	CSV_BUYGAMEITEM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BUYGAMEITEMINFO *pBI;
	char *StrMsg;

	BOOL Set(BUYGAMEITEMINFO *pbi, LPCTSTR strmsg);
	BOOL Get(char* lpdata, int size);
};




		/////////////// 게임 진행용 메세지 ////////////////

/////////// 게임이 시작되었음
class CSV_STARTGAME : public CCommMsg
{
public:
	CSV_STARTGAME() : CCommMsg() {}
	CSV_STARTGAME(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;		// 방 번호
	BOOL Set(int roomnum);
	BOOL Get(char* lpdata, int size);
};

/////////// 게임종료 결과를 통보
class CSV_GAMEOVERRESULT : public CCommMsg
{
public:
	CSV_GAMEOVERRESULT() : CCommMsg() {}
	CSV_GAMEOVERRESULT(char *pRefBuf) : CCommMsg(pRefBuf) {}

//	int *TotNum;
	GAMEOVERRESULT *GOR;
	BOOL Set(GAMEOVERRESULT *pGOR);
	BOOL Get(char* lpdata, int size);
};


////////////////////////////////////////////////////////////////////////////
//                      클라이언트용 메세지 클래스
////////////////////////////////////////////////////////////////////////////

/////////// 나가기 예약상태 알림
class CCL_EXITSUBSCRIPT : public CCommMsg
{
public:
	CCL_EXITSUBSCRIPT() : CCommMsg() {}
	CCL_EXITSUBSCRIPT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	char *ID;
	int *bExit;
	BOOL Set(int rnum, int unum, char *id, int bexit);
	BOOL Get(char* lpdata, int size);
};

/////////// 게임방용 채팅 데이타
class CCL_CHATDATA : public CCommMsg
{
public:
	CCL_CHATDATA() : CCommMsg() {}
	CCL_CHATDATA(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	char *ID;			// 아이디 스트링
	char *Kind;			// 채팅 데이터의 종류
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링
	BOOL Set(int rnum, char *id, char kind, char *chat);
	BOOL Get(char* lpdata, int size);
};


////////////////////////////////////////////////////////////////////////////
//                      게임 진행용 메세지 클래스
////////////////////////////////////////////////////////////////////////////

/////////// 게임을 시작할 준비를 마쳤음(게임을 재시작할때)
class CCL_READYTOSTART : public CCommMsg
{
public:
	CCL_READYTOSTART() : CCommMsg() {}
	CCL_READYTOSTART(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *Id;
	BOOL Set(char *id);
	BOOL Get(char* lpdata, int size);
};

/////////// 게임 방식 바꾸기 요청
class CCL_ASK_CHANGEGAMEKIND : public CCommMsg
{
public:
	CCL_ASK_CHANGEGAMEKIND() : CCommMsg() {}
	CCL_ASK_CHANGEGAMEKIND(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *ServPNum;
	char *ID;
	int *GameKind;
	int *bWhis;
	int *nCard;
	BOOL Set(int rnum, int servpnum, char *id, int gamekind, int bwhis, int ncard);
	BOOL Get(char* lpdata, int size);
};

/////////// 게임 방식 바꾸기 결과
class CCL_CHANGEGAMEKIND : public CCommMsg
{
public:
	CCL_CHANGEGAMEKIND() : CCommMsg() {}
	CCL_CHANGEGAMEKIND(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ROOMINFO *pRI;
	BOOL Set(ROOMINFO *pri);
	BOOL Get(char* lpdata, int size);
};

/////////// 게임 시작을 요청
class CCL_ASK_STARTGAME : public CCommMsg
{
public:
	CCL_ASK_STARTGAME() : CCommMsg() {}
	CCL_ASK_STARTGAME(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *ServPNum;
	char *ID;
	BOOL Set(int rnum, int servpnum, char *id);
	BOOL Get(char* lpdata, int size);
};

/////////// 게임 시작 정보
class CCL_STARTINFO : public CCommMsg
{
public:
	CCL_STARTINFO() : CCommMsg() {}
	CCL_STARTINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	STARTINFO *SI;
	BOOL Set(STARTINFO *pSI);
	BOOL Get(char* lpdata, int size);
};

// 방정보 변경 요청
class CCL_ASK_CHSTYLEROOMINFO : public CCommMsg
{
public:
	CCL_ASK_CHSTYLEROOMINFO() : CCommMsg() {}
	CCL_ASK_CHSTYLEROOMINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	STYLEROOMINFO *CHRI;
	BOOL Set(STYLEROOMINFO *pCHRI);
	BOOL Get(char* lpdata, int size);
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


class CSV_STARTCARD : public CCommMsg
{
public:
	CSV_STARTCARD() : CCommMsg() {}
	CSV_STARTCARD(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	char *ID;
	STARTINFO *pSC;
	BOOL Set(int rnum, int unum, char *id, STARTINFO *psc);
	BOOL Get(char* lpdata, int size);
};

// 카드 변경 요청
class CSV_CHANGECARD : public CCommMsg
{
public:
	CSV_CHANGECARD() : CCommMsg() {}
	CSV_CHANGECARD(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGECARD *pCC;
	BOOL Set(CHANGECARD *pcc);
	BOOL Get(char* lpdata, int size);
};

// [62]cut 카드 변경 요청
class CSV_ASK_CHANGECARD_62CUT : public CCommMsg
{
public:
	CSV_ASK_CHANGECARD_62CUT() : CCommMsg() {}
	CSV_ASK_CHANGECARD_62CUT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGECARD_62CUT *pCC;
	BOOL Set(CHANGECARD_62CUT *pcc);
	BOOL Get(char* lpdata, int size);
};

// [62]cut 변경되는 카드 보내기.(위의 확장이다 :동시에 뿌리기)
class CSV_ASK_CHANGECARD_62CUT_EX : public CCommMsg
{
public:
	CSV_ASK_CHANGECARD_62CUT_EX() : CCommMsg() {}
	CSV_ASK_CHANGECARD_62CUT_EX(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGECARD_62CUT_EX *pCC;
	BOOL Set(CHANGECARD_62CUT_EX *pcc);
	BOOL Get(char* lpdata, int size);
};

// 62cut 변경되는 카드를 보냄
class CSV_CHANGECARD_62CUT_RESULT : public CCommMsg
{
public:
	CSV_CHANGECARD_62CUT_RESULT() : CCommMsg() {}
	CSV_CHANGECARD_62CUT_RESULT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGECARD_62CUT *pCC;
	BOOL Set(CHANGECARD_62CUT *pcc);
	BOOL Get(char* lpdata, int size);
};

// 62cut client 가 server 로 바꿀 카드를 보냄 
class CCL_CARDCHANGE_62CUT_RESULT: public CCommMsg
{
public:
	CCL_CARDCHANGE_62CUT_RESULT() : CCommMsg() {}
	CCL_CARDCHANGE_62CUT_RESULT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGECARD_62CUT *pCC;
	BOOL Set(CHANGECARD_62CUT *pcc);
	BOOL Get(char* lpdata, int size);
};

class CCL_CARDCHANGE_END_62CUT: public CCommMsg
{
public:
	CCL_CARDCHANGE_END_62CUT() : CCommMsg() {}
	CCL_CARDCHANGE_END_62CUT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *ServPNum;
	char *ID;		
	BOOL Set(int rnum, int servpnum, char *id);
	BOOL Get(char* lpdata, int size);
};


//시작버튼 활성화 
class CSV_ACTIVE_STARTBUTTON : public CCommMsg
{
public :
	CSV_ACTIVE_STARTBUTTON(): CCommMsg(){}
	CSV_ACTIVE_STARTBUTTON(char *pRefBuf):CCommMsg(pRefBuf){}

	int *RoomNum;
	int *WinnerServPNum;
	char *WinnerID;
	int *bActive;

	BOOL Set(int rnum, int servpnum, char *id,int bactive);
	BOOL Get(char* lpdata, int size);
};

// 카드 사기 요청
class CSV_BUYCARD : public CCommMsg
{
public:
	CSV_BUYCARD() : CCommMsg() {}
	CSV_BUYCARD(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BUYCARD *pBC;
	BOOL Set(BUYCARD *pbc);
	BOOL Get(char* lpdata, int size);
};

// 폴드
class CCL_ASK_FOLD : public CCommMsg
{
public:
	CCL_ASK_FOLD() : CCommMsg() {}
	CCL_ASK_FOLD(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	int *PNum;
	char *ID;
	POKERCLNT *pFD;
	BOOL Set(int rnum, int unum, int pnum, char* id, POKERCLNT *pfd);
	BOOL Get(char* lpdata, int size);
};

// 카드 선택
class CCL_ASK_SELECTCARD : public CCommMsg
{
public:
	CCL_ASK_SELECTCARD() : CCommMsg() {}
	CCL_ASK_SELECTCARD(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	int *PNum;
	char *ID;
	POKERCLNT *pSD;
	int *DisCard;
	BOOL Set(int rnum, int unum, int pnum, char* id, POKERCLNT *psd, int discard);
	BOOL Get(char* lpdata, int size);
};


// Raise
class CCL_ASK_RAISE : public CCommMsg
{
public:
	CCL_ASK_RAISE() : CCommMsg() {}
	CCL_ASK_RAISE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	int *PNum;
	char *ID;
	POKERCLNT *pRA;
	BOOL Set(int rnum, int unum, int pnum, char* id, POKERCLNT *pra);
	BOOL Get(char* lpdata, int size);
};

// 각 유저 카드날리기 종료
class CCL_WINDCARDEND : public CCommMsg
{
public:
	CCL_WINDCARDEND() : CCommMsg() {}
	CCL_WINDCARDEND(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
//	POKERCLNT *pWD;
	BOOL Set(int rnum, int pnum, int unum, char* id);//, POKERCLNT *pwd);
	BOOL Get(char* lpdata, int size);
};


// 사용자 턴으로 인한 메세지
class CCL_ROOM_TIMEOUT : public CCommMsg
{
public:
	CCL_ROOM_TIMEOUT() : CCommMsg() {}
	CCL_ROOM_TIMEOUT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	char *ID;
	int  *l_StrMsg;		// 메세지 스트링의 길이
	char *StrMsg;		// 메세지 스트링
	BOOL Set(int rnum, int unum, char* id, int l_strlen, char *msg);
	BOOL Get(char* lpdata, int size);
};


/////////// 스페샬 카드로 이겼을 경우 모든 접속자에게 알림
class CSV_SPECIALCARD : public CCommMsg
{
public:
	CSV_SPECIALCARD() : CCommMsg() {}
	CSV_SPECIALCARD(char *pRefBuf) : CCommMsg(pRefBuf) {}


	SPECIALCARD *pSC;
	BOOL Set(SPECIALCARD *psc);
	BOOL Get(char* lpdata, int size);
};


/////////// 폴드한 사용자 유저 정보
class CCL_USERRESULT : public CCommMsg
{
public:
	CCL_USERRESULT() : CCommMsg() {}
	CCL_USERRESULT(char *pRefBuf) : CCommMsg(pRefBuf) {}


	USERRESULT *pRS;
	BOOL Set(USERRESULT *prs);
	BOOL Get(char* lpdata, int size);
};

/////////// 서버 알림 레이즈
class CSV_RAISE : public CCommMsg
{
public:
	CSV_RAISE() : CCommMsg() {}
	CSV_RAISE(char *pRefBuf) : CCommMsg(pRefBuf) {}


	POKERGAME *pPG;
	BOOL Set(POKERGAME *ppg);
	BOOL Get(char* lpdata, int size);
};

// 마지막 히든 카드 한장 (각각 날림)
class CSV_HIDDENCARD : public CCommMsg
{
public:
	CSV_HIDDENCARD() : CCommMsg() {}
	CSV_HIDDENCARD(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	char *ID;
	int  *nCard;
	BOOL Set(int rnum, int unum, char* id, int card);
	BOOL Get(char* lpdata, int size);
};

//첫 3장을 보낸후 유저가 하나의 카드를 선택 => 모두 선택시 게임시작
class CSV_WINDCARDEND : public CCommMsg
{
public:
	CSV_WINDCARDEND() : CCommMsg() {}
	CSV_WINDCARDEND(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Reserve;
	BOOL Set(int reserve);
	BOOL Get(char* lpdata, int size);
};

// 판 종료 알림
class CSV_ENDGAME : public CCommMsg
{
public:
	CSV_ENDGAME() : CCommMsg() {}
	CSV_ENDGAME(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Reserve;
	BOOL Set(int reserve);
	BOOL Get(char* lpdata, int size);
};



/////////// 서버 알림 레이즈 끝
class CSV_ENDRAISE : public CCommMsg
{
public:
	CSV_ENDRAISE() : CCommMsg() {}
	CSV_ENDRAISE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Flag;
	POKERGAME *pPG;
	BOOL Set(int flag, POKERGAME *ppg);
	BOOL Get(char* lpdata, int size);
};


// 어떤걸로 승부를 겨루겠는가
class CCL_SELECTWINCASE : public CCommMsg
{
public:
	CCL_SELECTWINCASE() : CCommMsg() {}
	CCL_SELECTWINCASE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *ServPNum;	
	int *UNum;
	char *ID;
	int  *nCase; // 1: 하이 2: 로우 3: 스윙
	BOOL Set(int rnum, int servpnum, int unum, char* id, int ncase);
	BOOL Get(char* lpdata, int size);
};

/////////////////////////////////////////////////////////////////////////////////////////
// [ 게임 아이템 ]

class CCL_ITEMRESULT : public CCommMsg
{
public:
	CCL_ITEMRESULT() : CCommMsg() {}
	CCL_ITEMRESULT( char* pRefBuf ) : CCommMsg(pRefBuf) {}

	int *RetCode;
	BOOL Set( int recv_code );
	BOOL Get( char* lpdata, int size );
};

class CCL_ITEMLIST : public CCommMsg
{
public :
	CCL_ITEMLIST() : CCommMsg() {}
	CCL_ITEMLIST( char* pRefBuf ) : CCommMsg(pRefBuf) {}

	int*	  Itemcount;
	GAMEITEM* Item;
	int*	  Use;
	BOOL Set( GAMEITEM* recv_item, int itemcount, int recv_use );
	BOOL Get( char* lpdata, int size );
};

class CCL_ASK_ITEMLOGIN: public CCommMsg
{
public:
	CCL_ASK_ITEMLOGIN() : CCommMsg() {}
	CCL_ASK_ITEMLOGIN( char* pRefBuf ) : CCommMsg(pRefBuf) {}

	int*	  Itemcount;
	GAMEITEM* Item;
	BOOL Set( GAMEITEM* recv_item, int itemcount );
	BOOL Get( char* lpdata, int size );
};

class CCL_ACCEPT_ITEMLOGIN: public CCommMsg
{
public:
	CCL_ACCEPT_ITEMLOGIN() : CCommMsg() {}
	CCL_ACCEPT_ITEMLOGIN( char* pRefBuf ) : CCommMsg(pRefBuf) {}

	int*	  Itemcount;
	GAMEITEM* Item;
	BOOL Set( GAMEITEM* recv_item, int itemcount );
	BOOL Get( char* lpdata, int size );
};



class CCL_ITEMLOGIN : public CCommMsg
{
public :
	CCL_ITEMLOGIN() : CCommMsg() {}
	CCL_ITEMLOGIN( char* pRefBuf ) : CCommMsg(pRefBuf) {}

	int* Index;

	BOOL Set( int index );
	BOOL Get( char* lpdata, int size );
};


// 유저 폴드
class CSV_FOLDUSER : public CCommMsg
{
public:
	CSV_FOLDUSER() : CCommMsg() {}
	CSV_FOLDUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	int *PNum;
	char *ID;
	int  *Kind;
	INT64 *JackPotMoney;
	BOOL Set(int rnum, int unum, int pnum, char* id, int kind, INT64 jackpot);
	BOOL Get(char* lpdata, int size);
};

/////////// 바로 시작 방 입장을 요청
class CSV_ASK_QUICKSTART : public CCommMsg
{
public:
	CSV_ASK_QUICKSTART() : CCommMsg() {}
	CSV_ASK_QUICKSTART(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *UNum;
	char *ID;
	int *SndFxKind; // ### [사운드 추가 작업] ###
	int *GameKind;  // 5인용, 7인용
	BOOL Set(int unum, char *id, int sndKind, int gamekind);
	BOOL Get(char* lpdata, int size);
};


/////////// 바로 입장 불가(방생성)
class CSV_REFUSE_QUICKSTART : public CCommMsg
{
public:
	CSV_REFUSE_QUICKSTART() : CCommMsg() {}
	CSV_REFUSE_QUICKSTART(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Code;
	BOOL Set(int code);
	BOOL Get(char* lpdata, int size);
};



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// 서버에이전트 작업	
// 접속
class CNM_AGENT_CONNECT : public CCommMsg
{
public:
	CNM_AGENT_CONNECT() : CCommMsg() {}
	CNM_AGENT_CONNECT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotalNum;
	INT64 *JackPot; // 잭팟금액
	BOOL Set(int tot, INT64 jackpot);
	BOOL Get(char* lpdata, int size);
};

// 유저 목록 요청
class CNM_AGENT_USERLIST : public CCommMsg
{
public:
	CNM_AGENT_USERLIST() : CCommMsg() {}
	CNM_AGENT_USERLIST(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Reserve;
	BOOL Set(int reserve);
	BOOL Get(char* lpdata, int size);
};


/////////// 1차 목록 400 명
class CNM_AGENT_USERLIST_R1 : public CCommMsg
{
public:
	CNM_AGENT_USERLIST_R1() : CCommMsg() {}
	CNM_AGENT_USERLIST_R1(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	int *Count;
	AGENT_USER *ArraySUI;

	BOOL Set(int totnum, AGENT_USER* ppSUI, int count);
	BOOL Get(char* lpdata, int size);
};


/////////// 새로운 유저 입장
class CNM_AGENT_NEWUSER : public CCommMsg
{
public:
	CNM_AGENT_NEWUSER() : CCommMsg() {}
	CNM_AGENT_NEWUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	AGENT_USER *ArraySUI;
	INT64 *JackPot; // 잭팟금액

	BOOL Set(int totnum, AGENT_USER* ppSUI, INT64 jackpot);
	BOOL Get(char* lpdata, int size);
};

///////////  유저 아웃
class CNM_AGENT_OUTUSER : public CCommMsg
{
public:
	CNM_AGENT_OUTUSER() : CCommMsg() {}
	CNM_AGENT_OUTUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	AGENT_USER *ArraySUI;
	INT64 *JackPot; // 잭팟금액

	BOOL Set(int totnum, AGENT_USER* ppSUI, INT64 jackpot);
	BOOL Get(char* lpdata, int size);
};


/////////// 중복접속 확인
class CNM_AGENT_DUBLECON : public CCommMsg
{
public:
	CNM_AGENT_DUBLECON() : CCommMsg() {}
	CNM_AGENT_DUBLECON(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	AGENT_USER *ArraySUI;

	BOOL Set(int totnum, AGENT_USER* ppSUI);
	BOOL Get(char* lpdata, int size);
};

/////////// 중복접속자
class CNM_AGENT_NEWUSERERROR : public CCommMsg
{
public:
	CNM_AGENT_NEWUSERERROR() : CCommMsg() {}
	CNM_AGENT_NEWUSERERROR(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	AGENT_USER *ArraySUI;

	BOOL Set(int totnum, AGENT_USER* ppSUI);
	BOOL Get(char* lpdata, int size);
};


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// 점프 아이템 작업


// 점프 아이템 사용 채널 입장 요청 
class CCL_ASK_JUMPITEM: public CCommMsg
{
public:
	CCL_ASK_JUMPITEM() : CCommMsg() {}
	CCL_ASK_JUMPITEM( char* pRefBuf ) : CCommMsg(pRefBuf) {}

	int*	  Itemcount;
	GAMEITEM* Item;
	
	
	BOOL Set( GAMEITEM* recv_item, int itemcount);
	BOOL Get( char* lpdata, int size );
};

// 점프 아이템 채널 입장 허용
class CCL_ACCEPT_JUMPITEM: public CCommMsg
{
public:
	CCL_ACCEPT_JUMPITEM() : CCommMsg() {}
	CCL_ACCEPT_JUMPITEM( char* pRefBuf ) : CCommMsg(pRefBuf) {}

	int*	  Itemcount;
	GAMEITEM* Item;
	ENTERCHANINFO *EC;	
	
	BOOL Set( GAMEITEM* recv_item, int itemcount, ENTERCHANINFO *pEC );
	BOOL Get( char* lpdata, int size );
};

// 점프 아이템을 가지고 로긴 한다
class CCL_JUMPITEM_LOGIN : public CCommMsg
{
public :
	CCL_JUMPITEM_LOGIN() : CCommMsg() {}
	CCL_JUMPITEM_LOGIN( char* pRefBuf ) : CCommMsg(pRefBuf) {}

	int* Index;

	BOOL Set( int index );
	BOOL Get( char* lpdata, int size );
};

//  DB 에 사용 유무 업데이트 후 결과를 알려준다.
class CCL_ACCEPT_JUMPITEM_CH: public CCommMsg
{
public:
	CCL_ACCEPT_JUMPITEM_CH() : CCommMsg() {}
	CCL_ACCEPT_JUMPITEM_CH( char* pRefBuf ) : CCommMsg(pRefBuf) {}

	int*	  Itemcount;
	GAMEITEM* Item;
	BOOL Set( GAMEITEM* recv_item, int itemcount );
	BOOL Get( char* lpdata, int size );
};

// 잭팟 평균 
class CNM_AGENT_AVERAGE : public CCommMsg
{
public:
	CNM_AGENT_AVERAGE() : CCommMsg() {}
	CNM_AGENT_AVERAGE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	INT64 *JackPotMoney;
	BOOL Set(INT64 jackpot);
	BOOL Get(char* lpdata, int size);
};

// ### [사운드 추가 작업] ### 요청
class CSV_ASK_CHANGESNDFX : public CCommMsg
{
public:
	CSV_ASK_CHANGESNDFX() : CCommMsg() {}
	CSV_ASK_CHANGESNDFX(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *UNum;
	int *ServPNum;
	int *RoomNum;
	char *ID;
	int *SndFX;
	BOOL Set(int unum, int spnum, int rnum, char *id, int sndfx);
	BOOL Get(char* lpdata, int size);
};


// ### [사운드 추가 작업] ### 요청 허락
class CSV_ACCEPT_CHANGESNDFX : public CCommMsg
{
public:
	CSV_ACCEPT_CHANGESNDFX() : CCommMsg() {}
	CSV_ACCEPT_CHANGESNDFX(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *SndFx;
	BOOL Set(int sndfx);
	BOOL Get(char* lpdata, int size);
};

// ### [이벤트 티켓] ###
/////////// 이벤트 당첨자 알림
class CSV_EVENTPRIZE : public CCommMsg
{
public:
	CSV_EVENTPRIZE() : CCommMsg() {}
	CSV_EVENTPRIZE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	EVENTPRIZEINFO *pEI;
	BOOL Set(EVENTPRIZEINFO *pei);
	BOOL Get(char* lpdata, int size);
};

// ### [이벤트 티켓] ###
/////////// 이벤트 공지
class CSV_EVENTNOTICE : public CCommMsg
{
public:
	CSV_EVENTNOTICE() : CCommMsg() {}
	CSV_EVENTNOTICE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	short *Kind;		// 데이터의 종류
	short *l_StrChat;	// 스트링의 길이
	char  *StrChat;		// 스트링
	COLORREF *Color;	// 텍스트 컬러
	BOOL Set(short kind, char *chat, COLORREF color);
	BOOL Get(char* lpdata, int size);
};


///////////////////////////////////////////////////////////////////
/////////////////// ### [관리자 모드 작업] ###  ///////////////////

/////////// 관리자 아디를 전송한다
class CNM_AGENT_ADMININFO : public CCommMsg
{
public:
	CNM_AGENT_ADMININFO() : CCommMsg() {}
	CNM_AGENT_ADMININFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	ADMIN_INFO *AdminInfo;
	BOOL Set(int totnum, ADMIN_INFO* ppCI);
	BOOL Get(char* lpdata, int size);
};

// 유저 종료하기
class CNM_ADMIN_OUTUSER : public CCommMsg 
{
public:
	CNM_ADMIN_OUTUSER() : CCommMsg() {}
	CNM_ADMIN_OUTUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}


	ADMINMESSAGE *pAMESG;
	BOOL Set(ADMINMESSAGE *pmsg);
	BOOL Get(char* lpdata, int size);
};

// 방제거하기
class CNM_ADMIN_DELROOM : public CCommMsg 
{
public:
	CNM_ADMIN_DELROOM() : CCommMsg() {}
	CNM_ADMIN_DELROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}


	ADMINMESSAGE *pAMESG;
	BOOL Set(ADMINMESSAGE *pmsg);
	BOOL Get(char* lpdata, int size);
};

/////////// 귓속말
class CNM_ADMIN_WHISPERCHAT : public CCommMsg
{
public:
	CNM_ADMIN_WHISPERCHAT() : CCommMsg() {}
	CNM_ADMIN_WHISPERCHAT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;			// 보내는 사람 아이디 스트링
	int  *DestUNum;		// 받는 사람 사용자 번호
	char *DestID;		// 받는 사람 아이디
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링
	BOOL Set(char *id, int destunum, char *destid, char *chat);
	BOOL Get(char* lpdata, int size);
};

// 방제 변경하기
class CNM_ADMIN_CHANGEROOM : public CCommMsg 
{
public:
	CNM_ADMIN_CHANGEROOM() : CCommMsg() {}
	CNM_ADMIN_CHANGEROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ADMINMESSAGE *pAMESG;
	STYLEROOMINFO *STYLE;
	BOOL Set(ADMINMESSAGE *padmsg, STYLEROOMINFO *pmsg);
	BOOL Get(char* lpdata, int size);
};

/////////// 방 정보 변경 요청 알림
class CSV_CHANGESTYLEROOMINFO : public CCommMsg
{
public:
	CSV_CHANGESTYLEROOMINFO() : CCommMsg() {}
	CSV_CHANGESTYLEROOMINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGESTYLEROOMINFO *SRI;
	BOOL Set(CHANGESTYLEROOMINFO* pSRI);
	BOOL Get(char* lpdata, int size);
};

// 어뷰져 놀려주기 
class CNM_SPECIALMESSAGE : public CCommMsg 
{
public:
	CNM_SPECIALMESSAGE() : CCommMsg() {}
	CNM_SPECIALMESSAGE(char *pRefBuf) : CCommMsg(pRefBuf) {}


	ADMINMESSAGE *pAMESG;
	BOOL Set(ADMINMESSAGE *pmsg);
	BOOL Get(char* lpdata, int size);
};
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////
/////////////////// ### [ 관전기능 ] ###  ///////////////////

/////////// 겜참여 요청
class CSV_ASK_PARTICIPATION : public CCommMsg
{
public:
	CSV_ASK_PARTICIPATION() : CCommMsg() {}
	CSV_ASK_PARTICIPATION(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	int *ServPNum;
	char *ID;
	int *SndFX;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	BOOL Set(int rnum, int unum, int servpnum, char *id, int sndfx);
	BOOL Get(char* lpdata, int size);
};


//////////// 겜참여 가능
class CSV_ACCEPT_PARTICIPATION : public CCommMsg
{
public:
	CSV_ACCEPT_PARTICIPATION() : CCommMsg() {}
	CSV_ACCEPT_PARTICIPATION(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	int *SndFxKind; // ### [사운드 추가 작업] ###
	char *ID;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	BOOL Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id,  IPARRINDEX *pIPA);// 추가
	BOOL Get(char* lpdata, int size);
};

//////////// 겜참여 가능
class CSV_ENTER_PARTICIPATION : public CCommMsg
{
public:
	CSV_ENTER_PARTICIPATION() : CCommMsg() {}
	CSV_ENTER_PARTICIPATION(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	int *SndFxKind; // ### [사운드 추가 작업] ###
	char *ID;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	BOOL Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id, IPARRINDEX *pIPA);// 추가
	BOOL Get(char* lpdata, int size);
};

/////////// 겜참여 요청
class CSV_ASK_EMERAGE : public CCommMsg
{
public:
	CSV_ASK_EMERAGE() : CCommMsg() {}
	CSV_ASK_EMERAGE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	int *ServPNum;
	char *ID;
	BOOL Set(int rnum, int unum, int servpnum, char *id);
	BOOL Get(char* lpdata, int size);
};


//////////// 관전 가능
class CSV_ACCEPT_EMERAGE : public CCommMsg
{
public:
	CSV_ACCEPT_EMERAGE() : CCommMsg() {}
	CSV_ACCEPT_EMERAGE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	BOOL Set(int rnum, int nowpnum, int newpnum, char *id, IPARRINDEX *pIPA);// 추가
	BOOL Get(char* lpdata, int size);
};

//////////// 관전 같은방의 다른유저에게 알린다
class CSV_ENTER_EMERAGE : public CCommMsg
{
public:
	CSV_ENTER_EMERAGE() : CCommMsg() {}
	CSV_ENTER_EMERAGE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ 중복된 IP가 ] ###
	BOOL Set(int rnum, int nowpnum, int newpnum, char *id, IPARRINDEX *pIPA);// 추가
	BOOL Get(char* lpdata, int size);
};


/////////// 예약자 정보 전송
class CSV_RESERVATIONINFO : public CCommMsg
{
public:
	CSV_RESERVATIONINFO() : CCommMsg() {}
	CSV_RESERVATIONINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	RESERVATIONINFO *ReservedInfo;
	BOOL Set(int totnum, RESERVATIONINFO* reservation);
	BOOL Get(char* lpdata, int size);
};

/////////// 관전관련 알림 메세지
class CSV_OBSERVER_MESSAGE : public CCommMsg
{
public:
	CSV_OBSERVER_MESSAGE() : CCommMsg() {}
	CSV_OBSERVER_MESSAGE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Kind;			// 종류(0:기본 출력 1:메세지 박스 출력)
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링

	BOOL Set(int kind, char *chat);
	BOOL Get(char* lpdata, int size);
};
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

/////////// 서버 공지 메세지
class CSV_SERVERNOTIC : public CCommMsg
{
public:
	CSV_SERVERNOTIC() : CCommMsg() {}
	CSV_SERVERNOTIC(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Kind;				// 종류(0:기본 출력 1:메세지 박스 출력)
	short *l_StrChat;		// 채팅 스트링의 길이
	char *StrChat;			// 채팅 스트링
	BOOL *bColorAssigned;	// 텍스트 컬러가 지정되었는가?
	COLORREF *Color;		// 텍스트 컬러

	BOOL Set(int kind, char *chat, BOOL bcolorassigned = FALSE, COLORREF color = RGB(255,0,0));
	BOOL Get(char* lpdata, int size);
};

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


// 카드 선택 타이머 알림
class CSV_SELECTCARD : public CCommMsg
{
public:
	CSV_SELECTCARD() : CCommMsg() {}
	CSV_SELECTCARD(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	int *ServPNum;
	char *ID;
	BOOL Set(int rnum, int unum, int servpnum, char *id);
	BOOL Get(char* lpdata, int size);
};

// [수호천사] 2004.07.08 유저->사용유무 확인
class CSV_ASK_SAFEANGEL : public CCommMsg
{
public:
	CSV_ASK_SAFEANGEL() : CCommMsg() {}
	CSV_ASK_SAFEANGEL(char *pRefBuf) : CCommMsg(pRefBuf) {}
	BOOL Set();
	BOOL Get(char* lpdata, int size);
};


// [수호천사] 2004.07.08 머니 충전 메시지
class CCL_CHARGE_SAFEANGEL : public CCommMsg
{
public:
	enum CHARGEFLAG{
		CHARGE_ANGEL=1,			// 수호 천사로 채워지는 금액
	};

public:
	CCL_CHARGE_SAFEANGEL() : CCommMsg() {}
	CCL_CHARGE_SAFEANGEL(char *pRefBuf) : CCommMsg(pRefBuf) {};

	int*   kind;
	INT64* money;
	char	*ID;

	BOOL Set(char *id, int Kind, INT64 Money );
	BOOL Get(char* lpdata, int size);
};


#endif
