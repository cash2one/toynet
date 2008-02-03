// Global....


#include "StdAfx.h"
#include "Global.h"


CClientDlg *g_pMainDlg=NULL;		// 메인 다이얼로그 포인터(전역변수)
CTitleDlg *g_pTitleDlg=NULL;		// 타이틀 다이얼로그 포인터(전역변수)
CLobyDlg *g_pLobyDlg=NULL;			// 대기실 다이얼로그 포인터(전역변수)
CGameDlg *g_pGameDlg=NULL;			// 게임 다이얼로그 포인터(전역변수)
CGameView *g_pGameView=NULL;		// 게임 뷰 윈도 포인터(전역변수)

CSockMan SockMan;			// 소켓 메니져 클래스
CSound Sound;				// 사운드 클래스
CDisplay Display;			// 디스플레이 모드 변경 클래스
xHan g_Han;					// 16비트모드 한글 클래스
CNoChat NoChat;				// 채팅 금지 클래스

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

CString g_ServIP("211.115.65.132");		// 메인 서버의 IP
UINT g_ServPort=8100;					// 메인 서버의 포트번호
CString g_LoginID;						// 자동 로그인 ID
CString g_LoginPass;					// 자동 로그인 패스워드

CString g_ServerName;	// 접속한 서버 이름
CString g_ChannelName;	// 접속한 채널 이름
CString g_NoticeURL;	// 공지사항 URL
BOOL g_bIsItemChannel;	// 아이템 채널인가?

CRect g_OrgMainDlgRect;	// 메인 다이얼로그의 원래 크기
int g_MainSrvSID = 0;	// 메인 서버의 소켓 번호
int g_Where = WH_FIRST;	// 내가 위치한 곳(WH_FIRST,...)
int g_MyPN = 0;			// 나의 플레이어 번호
int g_MastPN = 0;		// 방장의 플레이어 번호

ROOMINFO g_RI={0,};		// 방 정보 구조체
CPlayer Play[10];		// 플레이어 정보


/////////////////////////////////////////

CField Field;
CGame Game;
CWhisperDlg g_WhisperDlg;

