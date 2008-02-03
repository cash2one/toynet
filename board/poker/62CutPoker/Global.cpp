// Global....


#include "StdAfx.h"
#include "Global.h"

// 아바타 디스플레이 모듈 라이브러리 링크
#pragma comment (lib, "Avatar.lib")

int g_Max_Player    = MAX_PLAYER - 1;
int g_Max_NewPlayer = g_Max_Player + MAX_OBSERVER;

const int TOTAL_CARDNUM  = 52; //총 카드수
const int DUMY_CARD      = 52; //카드 뒷면
const int EMPTY_CARD     = -1;


C62CutPokerDlg *g_pMainDlg=NULL;		// 메인 다이얼로그 포인터(전역변수)
CTitleDlg *g_pTitleDlg=NULL;		// 타이틀 다이얼로그 포인터(전역변수)
CLobyDlg *g_pLobyDlg=NULL;			// 대기실 다이얼로그 포인터(전역변수)
CGameDlg *g_pGameDlg=NULL;			// 게임 다이얼로그 포인터(전역변수)
CGameView *g_pGameView=NULL;		// 게임 뷰 윈도 포인터(전역변수)
CWebProfileDlg g_WebProfileDlg;

CSockMan SockMan;			// 소켓 메니져 클래스
CSound Sound;
xHan g_Han;
CNoChat NoChat;
CBadChatMan BadChat;		// 욕설 필터링 클래스

CAdminMessageDlg *g_pAMes  = NULL; // [관리자 모드 작업]
/////////////////////////////////////////////////////////////////
xSprite GameBackSpr;		// 게임 배경화면 스프라이트
xSprite GameBackSpr_7;		// 게임 배경화면 스프라이트

xSprite LevelSpr;			// 레벨 스프라이트
xSprite CharBackSpr;		// 캐릭터 뒷배경
//////////////////////////////////////////////////////////////////

xSprite InfoBoxSpr;			// 정보창 스프라이트
xSprite ChatBoxSpr;			// 채팅 대화창 스프라이트

xSprite ExitBtnSpr;			// 나가기버튼 스프라이트
xSprite InviteBtnSpr;		// 초대버튼 스프라이트
xSprite FullScreenBtnSpr;	// 풀스크린 버튼 스프라이트
xSprite WindowBtnSpr;		// 윈도우모드 버튼 스프라이트
xSprite OptionBtnSpr;		// 옵션버튼 스프라이트
xSprite BadUserBtnSpr;		// 대화내용 신고 버튼 스프라이트
xSprite MasterBtnSpr;    // 방장
xSprite MinimizeBtnSpr;  // 최소화

// 게임진행 관련 버튼
xSprite GameStartBtnSpr;
xSprite AllinBtnSpr;
xSprite DieBtnSpr;
xSprite CheckBtnSpr;
xSprite CallBtnSpr;
xSprite PingBtnSpr;
xSprite DadangkBtnSpr;
xSprite MaxBtnSpr;
xSprite HighBtnSpr;
xSprite LowBtnSpr;
xSprite SwingBtnSpr;
xSprite EtcBtnSpr;
////////////////////////////////////////////////////////////////[62]
xSprite ChangeOKBtnSpr;
xSprite ChangeCancelBtnSpr;
// [62]체인지 카드 안내
xSprite ChangeCardInfoSpr; // 카드 바꾸기 안내 메세지
xSprite RoomMasterSpr;
xSprite BanishVoteSpr;
xSprite SortSpr;
xSprite AllinAniSpr;
/////////////////////////////////////////////////////////////////
xSprite EtcSpr;				// 기타
xSprite OringCharSpr;	// 파산
//////////////////////////////////////////////////////////////////////
xSprite BigCardSpr;		// 
xSprite SmallCardSpr;		// 카드

xSprite FocusSpr;		// 현재 턴
xSprite TimerSpr;		// 타이머 막대


// [ ###잭팟### ] 
xSprite JackPotBackSpr;	// 잭팟 배경
xSprite JackPotSpr;		// 잭팟 숫자
xSprite ChipSpr;		// 칩
/////////////////////////////////////////////////////////////////
xSprite ItemBagSpr;		// 
xSprite WarningSpr;		//  ### [ 중복된 IP가 ] ###
xSprite CaptureSpr;     // [화면캡쳐]
xSprite ObserverSpr;    // ### [ 관전기능 ] ###

//////////////////////////////////////////////////
// ### [ 대기실 리스트 관련 ] ###
xSprite			LobyListBackSpr;
xSprite			LobyListButtonSpr;
xSprite			LobyRoomImgSpr;		//	방정보 이미지
//////////////////////////////////////////////////




CString g_ServIP("127.0.0.1");		// 메인 서버의 IP
UINT g_ServPort=8003;					// 메인 서버의 포트번호
//CString g_ServIP("218.153.10.161");		// 메인 서버의 IP
//UINT g_ServPort=24201;					// 메인 서버의 포트번호
// 서버 2
//CString g_ServIP("218.153.10.238");		// 메인 서버의 IP
//UINT g_ServPort=24202;					// 메인 서버의 포트번호
//CString g_ServIP("127.0.0.1");		// 메인 서버의 IP
//CString g_ServIP("211.59.93.175");		// 메인 서버의 IP
//CString g_ServIP("218.153.10.226");		// 메인 서버의 IP
//UINT g_ServPort=25899;					// 메인 서버의 포트번호

//CString g_ServIP("218.153.10.164");		// 메인 서버의 IP
//UINT g_ServPort=24299;					// 메인 서버의 포트번호

CString g_AuthCookie;					// 인증 쿠키						[SSO 작업]
CString g_DataCookie;					// 데이터 쿠키						[SSO 작업]
CString g_CpCookie;						// CP쿠키							[SSO 작업]
CString g_SpareParam;					// 여분의 인자(CP게임등에서 사용)	[SSO 작업]
CString g_LoginID;						// 자동 로그인 ID
CString g_LoginPass("1111");			// 자동 로그인 패스워드
//CString g_Extra;						// 엑스트라 인자					[SSO 작업] - 삭제
CString g_MyUniqNo;						// 엑스트라 인자

CString g_ServerName;					// 접속한 서버 이름
CString g_ChannelName;					// 접속한 채널 이름
CString g_NoticeURL;					// 공지사항 URL
CString g_BannerURL1;					// 로비 광고 URL
CString g_BannerURL2;					// 게임 광고 URL
CString g_ProfileURL;					// 프로필 URL
CString g_ItemURL;						// 아이템 상점 URL	[게임 아이템 작업]
CString g_ItemHelpURL;					// 아이템 도움말 URL	[게임 아이템 작업]
CString g_CashURL;						// 캐쉬충전 URL

CString g_strSrvInfo;					// 채널 공지

int        g_nChannelStyle = 0;  // 게임 스타일
int        g_nchannel_usernum=0; // 게임 5, 7인용 결정

CNMStrMan    g_StrMan;
int        g_MSCode = 0; // 0: Korean 1: English 2:Japanese 3:Chinese

CString g_LocationURL;					// 위치알림(친구찾기)용 URL

int g_GameCode = 0;
int g_BgMode = 0;			// 스킨 플래그

CRect g_OrgMainDlgRect;					// 메인 다이얼로그의 원래 크기
int g_MainSrvSID = 0;					// 메인 서버의 소켓 번호
int g_Where = WH_FIRST;					// 내가 위치한 곳(0:로그인하기 전, 1:로그인중, 2:대기실, 3:게임중)
int g_MyPN = 0;							// 나의 플레이어 번호(0-2)
int g_MastPN = 0;						// 방장의 플레이어 번호(0-2)
BOOL g_Wait = FALSE;					// 응답을 기다리는 중인가?


BOOL g_bChannel = FALSE;


//////////// [게임 아이템 작업] ////////////
GAMEITEM_LIST g_MyGameItem;				// 내 게임 아이템
CTime   g_ServerTime = CTime::GetCurrentTime(); // 서버의 시간
CTime   g_ClientTime = CTime::GetCurrentTime();	// 클라이언트의 시간 (서버와 클라이언트와의 시간차를 구할때 사용)
CTimeSpan g_CSTimeGap;					// 서버와 클라이언트의 시간차
////////////////////////////////////////////




ROOMINFO g_RI={0,};			// 방 정보 구조체
// ### [ 관전기능 ] ###
CPlayer Play[MAX_NEWPLAYER];// 플레이어 정보
CPlayer g_MyInfo;
int  g_MyIndex;
BOOL g_MyObserver;			//관전중인가
BOOL g_bAdminUser = FALSE;  // [관리자 모드 작업]

CPlayScr PlayScr[MAX_PLAYER];			// 플레이어 스크린 클래스
CDisplay Display;						// 디스플레이 모드 변경 클래스
BOOL g_bChangeCardStage;		// 카드 바꾸기
////////////////


POKERGAME g_Poker;
CHAT_MACRO g_ChatMacro; // 메크로(채팅 관련)



// [비공개방 아이템] 
void g_GetNowSrvTime(COleDateTime &NowSrvTime)
{
//	COleDateTime ClntTime     = g_ClntTime;
//	COleDateTime SrvTime      = g_SrvTime;
//	COleDateTime NowClntTime  = COleDateTime::GetCurrentTime();

//	COleDateTimeSpan TimeSpan = NowClntTime - ClntTime; // 접속후의 시간을 구한다
//	NowSrvTime = SrvTime+TimeSpan; // 서버의 현재시간을 산출한다
}


// [ 세븐 포커 ] 잭팟
// [ ###잭팟### ] 
INT64 g_JackPotMoney = 0;     // 클라이언트
INT64 g_TmpJackPotMoney = 0;  // 서버			==> 증가치 보여줄때 사용


/////////////////////////////////////////

// 지역 스트링
char *StrArea[17]={
	"지역선택", "서울",     "부산",     "대전",     "대구",
	"광주",     "인천",     "울산",     "경기도",   "강원도",
	"충청북도", "충청남도", "경상북도", "경상남도", "전라북도",
	"전라남도", "제주도"};	

// 직업 스트링
char* StrJob[21]={
	"직업선택", "초등학생", "중고생", "대학(원)생", "회사원",
	"공무원",   "교직자",   "주부",   "군인",       "의료인",
	"법조인",   "종교인",   "언론인", "예술인",     "자영업",
	"농림업",   "축산업",   "수산업", "광고업",     "무직",
	"기타"};

// 지역 스트링을 얻음
char* GetStrArea(int num)
{
	if(num<0 || num>=17) return NULL;
	return StrArea[num];
}

// 직업 스트링을 얻음
char* GetStrJob(int num)
{
	if(num<0 || num>=21) return NULL;
	return StrJob[num];
}

CGame Game;
CWhisperDlg g_WhisperDlg;


// [세븐포커] 레벨 머니제한
//POKERSET g_POKERSET;

CString g_MakeCommaMoney(INT64 nMoney)
{
	CString strVal("0");

	bool minus = false;

	if(nMoney<0)
	{		
		minus = true;
		nMoney = -nMoney;
	}

	strVal.Format("%I64d",nMoney);


	for(int i=18; i>0; i-=3){
		if(strVal.GetLength()>i){
			strVal.Insert(strVal.GetLength()-i,",");
		}
	}
	if(strVal=="") strVal= "0";

	if( minus ) strVal.Insert(0,"-");
	
	return strVal;
/*
	CString strVal("0");

	if(nMoney<0) return strVal;
	strVal.Format("%I64d",nMoney);

	for(int i=18; i>0; i-=3){
		if(strVal.GetLength()>i){
			strVal.Insert(strVal.GetLength()-i,",");
		}
	}
	if(strVal=="") strVal= "0";
	return strVal;
	*/
}


CString NumberToOrientalString( INT64 number )
{
	CString strrtn;
	CString strtemp;
	INT64 t = number; 

	BOOL bMinus = FALSE;
	if( t < 0 ) {
		bMinus = TRUE;
		t *= -1;
	}

	INT64 v = t / (INT64)10000000000000000;	// 경 단위
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		strtemp.Format("%I64d경", v);
		strrtn += strtemp;
	}

	v = t / (INT64)1000000000000;	// 조 단위
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		strtemp.Format("%I64d조", v);
		strrtn += strtemp;
	}

	v = t / (INT64)100000000;	// 억 단위
	if(v > 0) {
		t = t - v*(INT64)100000000;
		strtemp.Format("%I64d억", v);
		strrtn += strtemp;
	}
	
	v = t / (INT64)10000;	// 만 단위
	if(v > 0) {
		t = t - v*(INT64)10000;
		strtemp.Format("%I64d만", v);
		strrtn += strtemp;
	}

	if( t > 0 ) {
		strtemp.Format("%I64d", t);
		strrtn += strtemp;
	}

	if( number == 0 )
	{
		strrtn = "0";
	}

	if( bMinus )
	{
		strrtn.Insert(0,"-");
		//CString temp;
		//temp.Format( "-%s", strrtn );
		//strrtn = temp;
	}
	

   //NUMBERFMT nFmt = { 0, 0, 4, ".", ",", 0 };
   //TCHAR    szVal[32];
   //TCHAR     szOut[32];
   //sprintf ( szVal,"%I64d", number );
   //GetNumberFormat ( NULL, NULL, szVal, &nFmt, szOut, 32 );

   	return strrtn;
}

CString g_MoneyMark(INT64 nMoney)
{
	if(g_pMainDlg->CFG.MoneyMark==0) { // 숫자 표시
		CString strVal("0");
		
		bool minus = false;
		
		if(nMoney<0)
		{		
			minus = true;
			nMoney = -nMoney;
		}
		
		strVal.Format("%I64d",nMoney);
		
		
		for(int i=18; i>0; i-=3){
			if(strVal.GetLength()>i){
				strVal.Insert(strVal.GetLength()-i,",");
			}
		}
		if(strVal=="") strVal= "0";
		
		if( minus ) strVal.Insert(0,"-");
		
		return strVal;
	}

	CString strrtn; // 한글표시
	CString strtemp;
	INT64 t = nMoney; 

	BOOL bMinus = FALSE;
	if( t < 0 ) {
		bMinus = TRUE;
		t *= -1;
	}

	INT64 v = t / (INT64)10000000000000000;	// 경 단위
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		strtemp.Format("%I64d경", v);
		strrtn += strtemp;
	}

	v = t / (INT64)1000000000000;	// 조 단위
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		strtemp.Format("%I64d조", v);
		strrtn += strtemp;
	}

	v = t / (INT64)100000000;	// 억 단위
	if(v > 0) {
		t = t - v*(INT64)100000000;
		strtemp.Format("%I64d억", v);
		strrtn += strtemp;
	}
	
	v = t / (INT64)10000;	// 만 단위
	if(v > 0) {
		t = t - v*(INT64)10000;
		strtemp.Format("%I64d만", v);
		strrtn += strtemp;
	}

	if( t > 0 ) {
		strtemp.Format("%I64d", t);
		strrtn += strtemp;
	}

	if( nMoney == 0 )
	{
		strrtn = "0";
	}

	if( bMinus )
	{
		strrtn.Insert(0,"-");
		//CString temp;
		//temp.Format( "-%s", strrtn );
		//strrtn = temp;`
	}
	

   //NUMBERFMT nFmt = { 0, 0, 4, ".", ",", 0 };
   //TCHAR    szVal[32];
   //TCHAR     szOut[32];
   //sprintf ( szVal,"%I64d", number );
   //GetNumberFormat ( NULL, NULL, szVal, &nFmt, szOut, 32 );

   	return strrtn;


}

//조, 억, 만 
int NumberToParse(INT64 money, int &Num1, int &Num2, int &Num3, int &Num4) {

	INT64 t = money; 
/*	
	INT64 v = t / (INT64)10000000000000000;	// 경 단위
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		Num1 = v;
	}
*/
	INT64 v = t / (INT64)1000000000000;	// 조 단위
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		Num1 = v;
	}

	v = t / (INT64)100000000;	// 억 단위
	if(v > 0) {
		t = t - v*(INT64)100000000;
		Num2 = v;
	}
	
	v = t / (INT64)10000;	// 만 단위
	if(v > 0) {
		t = t - v*(INT64)10000;
		Num3 = v;
	}

	if( t > 0 ) {
		Num4 = t;
	}
	return 0;
}

/*

CString g_MakeCommaMoney(INT64 nMoney)
{
	CString strVal("0");

	bool minus = false;

	if(nMoney<0)
	{		
		minus = true;
		nMoney = -nMoney;
	}

	strVal.Format("%I64d",nMoney);


	for(int i=18; i>0; i-=3){
		if(strVal.GetLength()>i){
			strVal.Insert(strVal.GetLength()-i,",");
		}
	}
	if(strVal=="") strVal= "0";

	if( minus ) strVal.Insert(0,"-");
	
	return strVal;
}
*/
/*
void BrowserMove( CString targeturl ) 
{ 
        //아바타 모듈에 들어있는 함수입니다. 
        char strbuf[1024]; 
        GetBaseAvatarDir(strbuf, 1023); 

        CString exeName = strbuf; 
        exeName += "\\NMIEMoveMFC.exe"; 

        ::ShellExecute(NULL, "open" ,exeName, targeturl, NULL, SW_HIDE); 
} 
*/
CString g_GetLevelName(int nLevel)
{
	if(nLevel== 0) return g_StrMan.Get(_T("LEVEL_NAME1"));	  
	if(nLevel== 1) return g_StrMan.Get(_T("LEVEL_NAME2"));
	if(nLevel== 2) return g_StrMan.Get(_T("LEVEL_NAME3"));
	if(nLevel== 3) return g_StrMan.Get(_T("LEVEL_NAME4"));
	if(nLevel== 4) return g_StrMan.Get(_T("LEVEL_NAME5"));
	if(nLevel== 5) return g_StrMan.Get(_T("LEVEL_NAME6"));
	if(nLevel== 6) return g_StrMan.Get(_T("LEVEL_NAME7"));
	if(nLevel== 7) return g_StrMan.Get(_T("LEVEL_NAME8"));
	if(nLevel== 8) return g_StrMan.Get(_T("LEVEL_NAME9"));
	if(nLevel== 9) return g_StrMan.Get(_T("LEVEL_NAME10"));
	if(nLevel==10) return g_StrMan.Get(_T("LEVEL_NAME11"));
	if(nLevel==11) return g_StrMan.Get(_T("LEVEL_NAME12"));
	if(nLevel==12) return g_StrMan.Get(_T("LEVEL_NAME13"));
	if(nLevel==13) return g_StrMan.Get(_T("LEVEL_NAME14"));
	if(nLevel==14) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==15) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==16) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==17) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==18) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==19) return g_StrMan.Get(_T("LEVEL_NAME15"));

	return  g_StrMan.Get(_T("LEVEL_NAME15"));
}
