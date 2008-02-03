// NetData.h: interface for the CNetData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETDATA_H__866B5F54_DF14_11D3_91FF_0050BF0FBDAF__INCLUDED_)
#define AFX_NETDATA_H__866B5F54_DF14_11D3_91FF_0050BF0FBDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 데이타 구조체의 종류. 이클래스에서 사용할꺼야...
enum DATATYPE{ 
	NM_DATA=0,    // 일반적인 데이타이다.
	NM_SIGN,    // 신호 데이타이다.
	NM_USER,    // 사용자 정보이다.
	NM_STRING,  // 문자열 데이타이다.

	NM_QMUSER, // 퀴즈마블 사용자 정보이다.
	NM_SLDATA,
	NM_CASDATA,
	NM_QMSIGN, // 퀴즈마블용 신호데이다.
	NM_QMDATA, // 슬롯머신용 데이타이다.(**변경)

	QM_DATA    // 퀴즈마블 게임방 데이타.
};
 // 데이타 용량...
enum DATASIZE{ 
	NM_SMALL=0,   // 1024 byte 이하. 사용자 정보나...채팅 문자열...(클라이언트->서버).
	NM_MEDIUM,  // 1024 byte(1k byte)
	NM_LARGE,   // 2048
	NM_BIG      // 4096 (서버->클라이언트). 
};

// 게임 접속 및 메인 통신 루틴
enum SIGNAL{

	NM_NOSIGN=0,
	NM_NOSIGN0,        // 더미값을 주기위해...(**변경)
	NM_NOSIGN1,        // 더미값을 주기위해...(**변경)
	NM_NOSIGN2,        // 더미값을 주기위해...(**변경)
	NM_NOSIGN3,        // 더미값을 주기위해...(**변경)
	NM_NOSIGN4,        // 더미값을 주기위해...(**변경)

	JK_TURN, // 사용자 텬이다

	//// 카지노 게임방 루틴.
	CAS_SEVENPOKER,
	CAS_NETMARBLE,
	CAS_RAISE,
	CAS_CHECK,
	CAS_CALL,
	CAS_CONNECT,
	CAS_CONNECT2,
	CAS_CONNECTOK,
	CAS_CONNECTERROR,
	CAS_DISCONNECT,
	CAS_PLAYGAME,
	CAS_ENDGAME,
	CAS_TURN,
	CAS_CHATSNDTO,
	CAS_CHATRCVFROM,
	CAS_CHAT,
	CAS_INROOM,
	CAS_OUTROOM,
	CAS_OUTROOM2,
	CAS_USERLIST,
	CAS_SNDDATA,
	CAS_RCVDATA,
	CAS_OUTSRV,
	CAS_YOUSRV,
	CAS_NEWSRV,
	CAS_TOTALTURN,
	CAS_THISTURN,
	CAS_MOVECHAR,
	CAS_MYTURNEND,
	CAS_SEEGAME,
	CAS_GAMEMES, //(세븐포카-메인게임메시지)
	CAS_GAMEMESROOM, //(세븐포카-메인게임메시지)
	CAS_FOLD,
	CAS_DIE,
	CAS_HIT,
	CAS_INS,
	CAS_STAND,
	CAS_SPLIT,
	CAS_DDOWN,
	CAS_BAT,
	CAS_SPLITSEC,
	CAS_ENDGAME2,
	CAS_ENDSLOT,
	// 슬롯머신 추가사항
	NM_LOBBYON,
	CAS_NOTLOBBYUSER,
	CAS_MESSAGETO,
	CAS_SLOT_BAT,
	CAS_SLOT_ROLL,
	CAS_SLOT_OUT,
	CAS_SLOT_BONUS,
	CAS_SLOT_MES,
	QM_MESRCVFROM,
	QM_MESSNDTO,
	QM_MESSNDTOERROR,
	// 세븐포카 게임 메시지.
	CAS_SELECTCARD,
	CAS_WINDCARDEND,
	CAS_ACITIVEBTNSTART,
	CAS_ACITIVEBTN,
	CAS_OUTROOMNOMONEY,
	CAS_GAMEINFO,
	CAS_SPECIAL,
	NM_USERRESULT,
	CAS_HIDDENCARD,

	/**************** 클라이언트 <-> 서버.*********************************/
	// 1. 기본 처리.
	NM_CONNECT,       // 접속되었다.
	NM_SEND_DATA,     // 데이타가 날라왔다.
	NM_SEND_TO_DATA,  // 특정 앤드포인트로 데이타를 보낸다.

	// 2. 미니 대화방에서...
	NM_CHANGECHATR,   // 대화방명을 바꾼다.         대화방명이 바뀌었다.
	NM_SILENTWORD,    // 귓속말을 보내고 싶어한다.  귓속말이 날라왔다.
	NM_DELCHATUSER,   // 방장이 강퇴를 요청하였다.  강퇴당하였다.

	
	/************** 클라이언트 -> 서버.(즉 서버가 해석한다.)****************/
	// 1. 연결 종료 처리.
	NM_LOGIN,         // 로그인 요청이다.
	NM_LOGOUT,        // 접속을 끊는단다.
	NM_CHANGE_USER,   // 사용자 정보를 변경한다고 통지.

	// 2. 미니 대화방 루틴. 서버에서 직접처리하자.
	NM_REFRESHCHAT,   // 대화방을 새로고침 요청.(퀴즈마블에서 사용)
	NM_CHATROOM,      // 대화방으로 가고싶다.
	NM_MAKECHATROOM,  // 대화방생성을 요청했다.
	NM_INCHATROOM,    // 대화방에 들어가고 싶어한다.
	NM_OUTCHATROOM,   // 대화방에서 나왔다.
	NM_CLOSECHATROOM, // 대화방을 종료시켰다.

	// 3. 게임방 루틴.
	NM_REFRESHGAME,   // 게임방 새로 고침.
	NM_GAMEROOM,      // 게임방으로 가고싶어한다.
	NM_MAKEGAMEROOM,  // 게임방만들기를 요청한다.
	NM_NOMAKEGAMEROOM,// 게임방을 만들수 없다.
	NM_INGAMEROOM,    // 게임방에 들어가고 싶어한다.
	NM_OUTGAMEROOM,   // 나 방에서 나왔어요...
	NM_CLOSEGAMEROOM, // 방을 파괴시키겠다.

	// 4. 게시판 및 메시지처리.
	NM_MESSAGETO,     // 누구에게 메시지를 보내겠단다.

		
	/*************** 서버 -> 클라이언트. (즉 클라이언트가 해석한다.)**************/
	// 1. 연결 종료 처리.
	NM_ERRMES,
	NM_ACCEPT,        // 클라이언트의 로그인을 허가받았다.
	NM_RELOGIN,       // 다시 로그인하라는 메시지를 받았다.
	NM_NEWUSER,       // 새로운 접속자다.
	NM_OUTUSER,       // 한명이나갔다.
	NM_DENY,          // 서버에서 거부 당했다.
	NM_CONDENY,       // 더이상 접속금지.
	NM_SRVFULL,       // 정원초과
	NM_TIMER,         // 서버의 타이머메시지.
	NM_NEWDENY,       // 이미 존재하는 아이디이다.
	NM_DOWN,          // 오잉? 서버가 다운 되었다네..그..그럴수가....
	NM_WHISDNY,       // 귓말금지.
	NM_WHISOK,
	NM_MESDNY,        // 메시지 금지
	NM_MESOK,
	NM_INVDNY,
	NM_INVERR,
	NM_INVITE,
	NM_INVDENYMES,
	NM_INVINROOM,
	NM_EXITFLAG,      // 일괄종료.
	NM_SOMEONECON,
	NM_DENYVER,
	NM_DENYUSER, // 강퇴루틴.
	NM_USERLIST, // 추가 접속자 루틴.
	NM_USERLIST1,
	NM_USERLIST2,
	NM_FINDUSER,
	NM_FINDERROR,
	NM_SENDROOMNUM,
	NM_ENDGAMEFLAGSRV,
	NM_USERINFO,
	NM_USERINFOERROR,
	NM_OUTROOMINFO,

	NM_SELCARD,
	NM_BUYCARD,
	NM_NEWUSERIDOK,    // 아이디 중복확인.
	NM_NEWUSERIDNO,    // 아이디 중복된다.
	NM_NEWUSERERROR, // 신규접속 에러다.
	NM_NEWUSEROK,    // 신규접속 성공이다.

	// 추가헤더
	NM_THISCONDENY,   // 현재 당 서버에 누군가 접속해 있다
	NM_CHECKVER,      // 버전을 확인한다
	NM_CHANNELLIST,   // 채널 리스트 요구
	NM_CHANNELIN,     // 채널 입장 요청
	NM_CHANNELOUT,    // 채널 퇴장 요청
	NM_CHANNELFIND,   // 채널에서 사용자 찾기
	NM_ROOMLIST,      // 방리스트 요청
	NM_HEARTBEAT,     // 접속유지 플래그
	NM_DISCONMES,     // 강제 종료 메시지
	NM_CHATSNDTO,     // 귓말 보내기
	NM_CHATSNDTO2,    // 로비를 켜지 않은 사람의 귓말
	NM_CHATSNDTOERROR,
	NM_SMALLUSERINFO, // 게임결과 관련
	NM_CHANGEROOM,



	// 2. 1)미니 대화방 밖에서....
	NM_LISTCHATROOM,  // 대화방 목록이 날라왔다.
	NM_LISTCHATNEW,   // 새로운대화방이 생성되었단다.
	NM_LISTCHATOUT,   // 대화방이 하나 꺼졌다.
	NM_CONCHATOK,     // 대화방 출입을 승인받았다.
	//    2)미니 대화방 안에서...
	NM_CONCHATUSERS,  // 대화방에 접속되어있는 접속자를 보내왔다.
	NM_NEWCHATUSER,   // 새로운 사용자가 들어왔다.
	NM_OUTCHATUSER,   // 한명이 나갔다.
	NM_INHERCROOM,    // 대화방을 승계받았다.

	// 3. 게임방 루틴 밖에서.
	NM_LISTGAMEROOM,  // 게임방 목록이 날라왔다.
	NM_LISTGAMENEW,   // 새로운 게임방이 생성되었단다.
	NM_LISTGAMEOUT,   // 게임방이 하나 꺼졌다.
	NM_CONGAMEOK,     // 게임방 출입을 승인받았다.

	// 4. 게시판 및 메시지처리.
	NM_MESSAGE,       // 서버로 부터 메시지가 날라왔다.
	NM_MESSAGEFROM,   // 누군가로 부터 메시지가 날라왔다.

	
	NM_ADV,           // 광고를 보내왔다...이런 ...

	///// 퀴즈마블용 메시지.
	NM_QMCHATDATA,   // 채팅데이타이다.
	NM_CANCELMAKEROOM, // 게임방 만들기를 취소하자.
	NM_MAKEROOMOK,    // 방을 만들어 놓았다.
	NM_QMOUTSERVER,   // 게임서버가 방을 나갔다..
	NM_QMOUTONSERVER,
	NM_QMDELROOM,     // 방리스트가 하나 제거되었다.
	NM_QMILREGALROOM, // 올바른 게임방이아니다.
	NM_QMCONNECTROOMERROR, // 게임방과 연결이 안된다.
	NM_QMFULLROOM,   // 게임방인원이 꽉찾다.
	NM_QMROOMPASSNO, // 겜방 비밀번호가 틀리다.
	NM_QMROOMSRVIP,  // 겜방 아이피가 날라왔다.
	NM_QMROOMINOK,   // 겜방에 한명 접속되었다.
	NM_QMROOMFULLNOW,// 겜방인원이 방금 풀이다.
	NM_QMROOMINOKCLNT, // 겜방에 접속되었다고 접속자가 알려왔다.
	NM_QMPLAYGAME,   // 게임이 시작되었다.
	NM_OUTGAMEONROOM, // 진행중인 겜방에서 나간다.
	NM_QMGAMEOVER,   // 게임이 끝났다.
	NM_QMCHANGESRV,  // 게임서버가 바뀐다.
	NM_QMYOUSRV,     // 게임서버가 되었다.
	NM_SETPOINT,     // 사용자 게임점수 세팅이다.
	NM_SETPOINT2,
	NM_QMCHANGEINFO, // 사용자 정보변경.
	NM_QMGAMEDATA,
	NM_QMONINROOMERROR,
	NM_ROOMCHATDATA,
	NM_ROOMINFO,
	NM_MYINFO,
	NM_CHANGECARD,





	//// 퀴즈마블 게임방 루틴.
	QM_CONNECT,
	QM_CONNECT2,
	QM_CONNECTOK,
	QM_CONNECTERROR,
	QM_DISCONNECT,
	QM_PLAYGAME,
	QM_ENDGAME,
	QM_TURN,
	QM_CHATSNDTO,
	QM_CHATSNDTOERROR,
	QM_CHATRCVFROM,
	QM_CHAT,
	QM_INROOM,
	QM_OUTROOM,
	QM_OUTROOM2,
	QM_USERLIST,
	QM_SNDDATA,
	QM_RCVDATA,
	QM_OUTSRV,
	QM_YOUSRV,
	QM_NEWSRV,
	QM_TOTALTURN,
	QM_THISTURN,
	QM_MOVECHAR,
	QM_MYTURNEND,
	QM_SEEGAME,
	QM_GAMEMES,
	QM_STARTOK,
	QM_STARTNO,
	QM_QMGAMEDATA,
	QM_READYGAME,
	QM_NEWUSER,


	NM_CLEARUSER,
	NM_LOCKROOM,
	NM_CLEARROOM,


	// 신고관련 메시지.
	NM_CAPFILE,      // 켑쳐 파일전송.
	NM_CAPFILEALL,   // 켑쳐 파일전송.
	NM_STOPUSER,     // 사용정지된 사용자.
	NM_DELUSER,      // 각제된 사용자.
	NM_CAPRESULTMES, // 신고접수 메시지 전송.


	// 장기관련 메시지
	NM_JKPLAY, // 대국요청
	NM_JKPLAYOK,//대국 승락.
	NM_JKDENY, // 대국거절
	NM_JKREADY,    // 상차림 메시지
	NM_JKPLAYNOTUSER, // 겜방에 없는 사용자이다.
	NM_JKALEADYON, // 이미 게임이 시작되었다.
	NM_JKGAMEDATA, // 장기 게임데이타이다.
	NM_JKPLAYGAME, // 실지로 게임을 시작한다.
	NM_JKPASS, // 한수 통과하였다.
	NM_JKOHNO, // 무르기요청
	NM_JKOHNONO,// 무르기 불허
	NM_JKOHNOOK,//무르기 허용.
	NM_JKSTARTERROR, // 장기시작에 문제있다.
	NM_JKRESET, // 장기게임을 리셋시킨다.
	NM_JKFOLD,  // 장기를 포기한다.
	NM_JKSAME,  // 비김신청을 한다.
	NM_JKSAMEOK,
	NM_JKSAMENO,
	NM_JKENDGAME, // 게임이 끝났다.
	NM_JKDATA, // 장기 데이타이다.
	NM_JKNEWLEVEL,

	NM_JKWITHGAME,   // 대국을 신청한다
	NM_JKWITHGAMEERR,// 대국신청에러(이미 방에 있다,이미 접속이 끊겼다)
	NM_JKWITHGAMENO, // 대국신청 거절
	NM_JKWITHGAMEOK, // 대국신청 승인
	NM_JKROOMERROR,  // 게임방에러
	NM_JKBIGJANG,    // 빅장처리
	NM_ACTROOMSRV,
	NM_ACTROOMCON,
	NM_LISTGAMENEWACT,

	NM_AGENT_CONNECT, // [서버 에이전트 작업] : 에이전트가 접속되었다
	NM_AGENT_NEWUSER,
	NM_AGENT_OUTUSER,
	NM_AGENT_NEWUSERERROR,
	NM_AGENT_USERLIST,

	NM_AGENT_USERLIST1,
	NM_AGENT_USERLIST2,
	NM_AGENT_USERLIST3,
	NM_AGENT_USERLIST4,
	NM_AGENT_USERLIST5,

	NM_ITEM_LOGIN,   // 아이템 전용 로그인 허가
	NM_ITEM_LOGOUT,  // 아이템 전용 로그아웃 허가
	NM_ITEM_LIST,    // 아이템 리스트
	NM_ITEM_NOITEM,  // 아이템 보유하지 않음
	NM_ITEM_READERR, // 아이템 읽기 에러
	NM_ITEM_NOUSE,   // 아이템 사용하지 않음
	NM_ITEM_EXPIRED, // 아이템 유효기간 만료
	NM_ITEM_RETRY,   // 아이템 사용 에러, 다시 시도하세요
	NM_ITEM_ERROR1,  // 아이템 에러
	NM_ITEM_ERROR2,  // 아이템 에러
	NM_ITEM_ERROR3,  // 아이템 에러
	NM_ITEM_ERROR4,  // 아이템 에러

	NM_ITME_RLIST,
	NM_ITME_RENTER,
	NM_ITME_RUSE,

	// [관리자 모드 작업]

	NM_ADMIN_CHANGEROOM, // 운영자의 옵션변경
	NM_ADMIN_DELROOM,    // 방제거
	NM_ADMIN_WARNUSER,   // 유저에게 경고하기
	NM_ADMIN_OUTUSER,    // 사용자 종료시키기



	DUMY



};

// 네트워크 데이타 헤더.(32 byte)
typedef struct tagNetHeader{ 
	char      Header[8];   // ( 8 byte)구조체 헤더. 인식 문자.
	UINT      Size;        // ( 4 byte)실질적인 데이타 사이즈.
	SIGNAL    Signal;      // ( 4 byte)명령어다...
	DATATYPE  Type;        // ( 4 byte)이 구조체의 형태는 ...
	BOOL      Compress;    // ( 4 byte)압축여부.
	UINT      Code;        // ( 4 byte)암호코드.
	int       UserVal;     // ( 4 byte)사용자 값
}NETHEADER,*LPNETHEADER;

// 게임방 게임루틴.
enum EGAMESIGN{
	EGAME_NOSIGN=0,
	EGAME_INIT,
	EGAME_INUSER,
	EGAME_OUTUSER,
	EGAME_START,
	EGAME_SELECT,
	EGAME_RESULT,
	EGAME_END,
	EGAME_DISTCARD,
	EGAME_HIDECARD,
	EGAME_CHAT,
	EGAME_FOLD,
	EGAME_DIE,
	EGAME_BAT,
	EGAME_CALL,
	EGAME_CHECK,
	EGAME_ACTBTNSTART,
	EGAME_ACTBTN,
	EGAME_RESET,
	EGAME_ALLFOLD,
	EGAME_SETFLAG,
	EGAME_TIMERON,
	EGAME_ONLYWIN,
	EGAME_INFO,


	EGAME_DUMMY,
};

#define  NETMARBLEHEADER   "NMJANGGI" // (**변경)
#define  NETCODENUM        (12)

// 넷데이타.
class CNetData : public CObject 
{
protected:
	DECLARE_SERIAL(CNetData)
public:
	int  GetUserVal(); // 사용자 정의 값을 구한다
	BOOL CheckHeader(char *lpdata, int *psignal, int *pmsglen);

	int  InitClass(); // 변수 및 메모리 초기화.
	CNetData();
	virtual ~CNetData();
	CNetData& operator=( CNetData &assign );

	DATATYPE  GetType(); // 데이타 형태를 구한다.
	SIGNAL    GetSign(); // 데이타 신호를 구한다.
	char*     GetData(); // 실질 통신 데이타를 구한다.
	UINT      GetSize(); // 실질 통신 데이타 사이즈를 구한다.

	BOOL      SetData(LPVOID lpdata, UINT nsize, DATATYPE type, SIGNAL signal, int nuserval=0); // 데이타 세팅.
	BOOL      IsData(LPVOID lpData, int size); // 데이타인지 확인하고 맞으면 세팅한다.

	NETHEADER m_NetHeader; // 데이타 헤더.
	LPVOID    m_lpData;    // 핵심적인 내부 데이타.
	UINT      m_Size;      // 내부 데이타 사이즈.

	LPVOID    m_NetData;   // 실질적으로 통신에서 쓰일 데이타.(헤더+데이타)
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetData)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CNetData)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
private:
	TCHAR m_chCode[NETCODENUM];
	TCHAR m_ch1;
	TCHAR m_ch2;
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

#endif // !defined(AFX_NETDATA_H__866B5F54_DF14_11D3_91FF_0050BF0FBDAF__INCLUDED_)
