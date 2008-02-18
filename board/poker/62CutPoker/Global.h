// �۷ι� ����, Ŭ����, �Լ�..

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

#define MAX_ROOM	70	// �ִ� ���ӷ��� ����

extern const int TOTAL_CARDNUM;  //�� ī���
extern const int DUMY_CARD;      //ī��޸�
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

extern CSockMan SockMan;		// ���� �޴��� Ŭ����
extern CSound Sound;
extern xHan g_Han;
extern CNoChat NoChat;
extern CBadChatMan BadChat;		// �弳 ���͸� Ŭ����

extern CAdminMessageDlg *g_pAMes; // [������ ��� �۾�]
/////////////////////////////////////////////////////////////////

extern xSprite GameBackSpr;		// ���� ���ȭ�� ��������Ʈ
extern xSprite GameBackSpr_7;		// ���� ���ȭ�� ��������Ʈ

extern xSprite LevelSpr;		// ���� ��������Ʈ

//extern xSprite SCharSpr;		// ���� ĳ���� ��������Ʈ
//extern xSprite LCharSpr;		// ū ĳ���� ��������Ʈ
extern xSprite CharBackSpr;		// ĳ���� �޹��


/////////////////////////////////////////////////////////////////

extern xSprite InfoBoxSpr;		// ����â ��������Ʈ
extern xSprite ChatBoxSpr;		// ä�� ��ȭâ ��������Ʈ

extern xSprite ExitBtnSpr;		// �������ư ��������Ʈ
extern xSprite InviteBtnSpr;	// �ʴ��ư ��������Ʈ
extern xSprite FullScreenBtnSpr;// Ǯ��ũ�� ��ư ��������Ʈ
extern xSprite WindowBtnSpr;	// �������� ��ư ��������Ʈ
extern xSprite OptionBtnSpr;	// �ɼǹ�ư ��������Ʈ
extern xSprite BadUserBtnSpr;	// ��ȭ���� �Ű� ��ư ��������Ʈ

extern xSprite MasterBtnSpr;    // ����
extern xSprite MinimizeBtnSpr;  // �ּ�ȭ

//extern xSprite Dog19Spr;		// 19�� �̸� ���Ҹ� ��������Ʈ

// �������� ���� ��ư
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
// [62]ü���� ī�� �ȳ�
extern xSprite ChangeCardInfoSpr;
extern xSprite RoomMasterSpr;
extern xSprite BanishVoteSpr;
extern xSprite SortSpr;
extern xSprite AllinAniSpr;
extern xSprite InsertCoinSpr;

////////////////////////////////////////////////////////////////
//extern xSprite BigNumSpr;		// ū ���� ��������Ʈ
extern xSprite EtcSpr;			// ��Ÿ
extern xSprite OringCharSpr;	// �Ļ�
/////////////////////////////////////////////////////////////////////////
extern xSprite BigCardSpr;		// ī��
extern xSprite SmallCardSpr;		// ī��

extern xSprite FocusSpr;		// ���� ��
extern xSprite TimerSpr;

// [ ###����### ] 
extern xSprite JackPotBackSpr;	// ���� ���
extern xSprite JackPotSpr;		// ���� ����
extern xSprite ChipSpr;			// Ĩ 
/////////////////////////////////////////////////////////////////
extern xSprite ItemBagSpr;		// 
// ### [ �ߺ��� IP�� ] ###
extern xSprite WarningSpr;		// 
extern xSprite CaptureSpr;      // [ȭ��ĸ��]
extern xSprite ObserverSpr;		// ### [ ������� ] ###

//////////////////////////////////////////////////
// ### [ ���� ����Ʈ ���� ] ###
extern	xSprite			LobyListBackSpr;
extern	xSprite			LobyListButtonSpr;
extern  xSprite			LobyRoomImgSpr;		//	������ �̹���
//////////////////////////////////////////////////



extern CString g_ServIP;		// ���� ������ IP
extern UINT g_ServPort;			// ���� ������ ��Ʈ��ȣ
extern CString g_AuthCookie;	// ���� ��Ű						[SSO �۾�]
extern CString g_DataCookie;	// ������ ��Ű						[SSO �۾�]
extern CString g_CpCookie;		// CP��Ű							[SSO �۾�]
extern CString g_SpareParam;	// ������ ����(CP���ӵ�� ���)	[SSO �۾�]
extern CString g_LoginID;		// �ڵ� �α��� ID
extern CString g_LoginPass;		// �ڵ� �α��� �н�����
//extern CString g_Extra;			// ����Ʈ�� ����				[SSO �۾�] - ����
extern CString g_MyUniqNo;		// �� ����ũ �ѹ�


extern CString g_ServerName;	// ������ ���� �̸�
extern CString g_ChannelName;	// ������ ä�� �̸�
extern CString g_NoticeURL;		// �������� URL
extern CString g_BannerURL1;	// �κ� ���� URL
extern CString g_BannerURL2;	// ���� ���� URL
extern CString g_ProfileURL;	// ������ URL
extern CString g_ItemURL;		// ������ ���� URL	[���� ������ �۾�]
extern CString g_ItemHelpURL;	// ������ ���� URL	[���� ������ �۾�]
extern CString g_CashURL;		// ĳ������ URL

extern CString g_strSrvInfo;	// ä�� ����

extern int       g_nChannelStyle; // ���� ��Ÿ��
extern int       g_nchannel_usernum; // ���� 5, 7�ο� ����

extern int        g_MSCode;
extern CNMStrMan    g_StrMan;

extern CString g_LocationURL;					// ��ġ�˸�(ģ��ã��)�� URL

extern int g_GameCode;

extern int g_MainSrvSID;		// ���� ������ ���� ��ȣ

#define WH_FIRST	0			// �α����ϱ���
#define WH_LOGIN	1			// �α�����
#define WH_TITLE	2			// Ÿ��Ʋ ȭ��
#define WH_LOBY		3			// ����
#define WH_GAME		4			// ������

//#define BBOX_WIDTH 10
//#define BBOX_HEIGHT 20

extern CRect g_OrgMainDlgRect;	// ���� ���̾�α��� ���� ũ��
extern int g_Where;				// ���� ��ġ�� ��(0:�α����ϱ� ��, 1:�α����� 2:����, 3:������)
extern BOOL g_Wait;				// ������ ��ٸ��� ���ΰ�?


extern 	BOOL g_bChannel;
extern int g_BgMode;			// ��Ų �÷���

extern int g_MyPN;				// ���� �÷��̾� ��ȣ(0-2)
extern int g_MastPN;			// ������ �÷��̾� ��ȣ(0-2)
extern ROOMINFO g_RI;			// �� ���� ����ü
extern CPlayScr PlayScr[MAX_PLAYER];		// �÷��̾� ��ũ�� Ŭ����
extern CCoverCard g_CvCard;
extern CDisplay Display;		// ���÷��� ��� ���� Ŭ����
extern BOOL g_bChangeCardStage;		// ī�� �ٲٱ�
//////////////////////////

// ### [ ������� ] ###
extern CPlayer Play[MAX_NEWPLAYER];		// �÷��̾� ����
extern CPlayer g_MyInfo;
extern int  g_MyIndex;
extern BOOL g_MyObserver;
extern BOOL g_bAdminUser; // [������ ��� �۾�]

//////////// [���� ������ �۾�] ////////////
extern GAMEITEM_LIST g_MyGameItem;	// �� ���� ������
extern CTime   g_ServerTime;	// ������ �ð�
extern CTime   g_ClientTime;	// Ŭ���̾�Ʈ�� �ð� (������ Ŭ���̾�Ʈ���� �ð����� ���Ҷ� ���)
extern CTimeSpan g_CSTimeGap;	// ������ Ŭ���̾�Ʈ�� �ð���
////////////////////////////////////////////

extern POKERGAME g_Poker;
void g_GetNowSrvTime(COleDateTime &NowSrvTime);// [������� ������] 

// [ ���� ��Ŀ ] ����
// [ ###����### ] 
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

// [������Ŀ] ���� �Ӵ�����
//extern POKERSET g_POKERSET;

CString g_MakeCommaMoney(INT64 nMoney);
CString NumberToOrientalString( INT64 number );
int NumberToParse(INT64 money, int &Num1, int &Num2, int &Num3, int &Num4);

CString g_MoneyMark(INT64 nMoney);

//void BrowserMove( CString targeturl ); // [ �Ļ� ]

CString g_GetLevelName(int nLevel);


typedef	struct {	
	char	Macrostr[10][40];	
}CHAT_MACRO;

extern CHAT_MACRO g_ChatMacro; // ��ũ��(ä�� ����)

#endif