#ifndef __USERSTRUCT_H__
#define __USERSTRUCT_H__

// ### [ ������� ] ###
const int  MAX_PLAYER	=   6;
const int  MAX_OBSERVER =   5;//  - (1 ### [������ ��� �۾�] ###)  
const int  MAX_NEWPLAYER = (MAX_PLAYER+MAX_OBSERVER);


#define MAX_AVATARLAYER 26
#define BANISHVOTE_NUM 3


#define UPDATE_GAMENUM        20  //    20ȸ ########### �߰��� ��Ͻ� ������ �� ����Ƚ�� ###########
//#define BACKUP_GAMETIME     1800  //    30�� ################## ��ü��� �ð�(��) ###################



//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [���� ������ �۾�]                               //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_ITEM_NUM           (30) // �ִ������ ����

#define GAMEITEM_MES_ERROR     (-1) // ����
#define GAMEITEM_MES_SUCCESS   ( 0) // ����
#define GAMEITEM_MES_DBERROR   ( 1) // DB����
#define GAMEITEM_MES_NOITEM    ( 2) // �������� ���ų� ��밡���� ������ ����
#define GAMEITEM_MES_USEABLE   ( 3) // �������� �ְ� ������� �ʾҴ�
#define GAMEITEM_MES_EXPIRED   ( 4) // �̹� ������� �������̰�, �Ⱓ�� ����Ǿ���
#define GAMEITEM_MES_USEFAIL   ( 5) // �����ۻ�뿡 ����������, �ٽ� �õ� ��û

#define ITEM_SCODE_INVINCIBLE   21		// õ�Ϲ��� ������ ���� �ڵ� (���۹�, ����, �����) 

#define ITEM_SCODE_SAFEGUARDANGEL	61		// [��ȣõ��] 2004.07.08
#define ITEM_ECODE_SAFEGUARDANGEL	61

#define ITEM_SCODE_SECRET		100		// [�������] ������ ���� �ڵ�
#define ITEM_ECODE_SECRET		199		// [�������] ������ �� �ڵ�
/*
#define ITEM_SCODE_JUMP			200		// ���� ������ ���� �ڵ�
#define ITEM_ECODE_JUMP			299		// ���� ������ �� �ڵ�

#define ITEM_SCODE_SUPERMASTER	400		// ���� ���� ������ ���� �ڵ�
#define ITEM_ECODE_SUPERMASTER	499		// ���� ���� ������ �� �ڵ�

#define ITEM_SCODE_PLUSPOINT	500		// �÷��� ����Ʈ ������ ���� �ڵ�
#define ITEM_ECODE_PLUSPOINT	599		// �÷��� ����Ʈ ������ �� �ڵ�
*/

#define ITEM_SCODE_JUMP			201		// ���� ������ ���� �ڵ�
#define ITEM_ECODE_JUMP			201		// ���� ������ �� �ڵ�
#define ITEM_SCODE_SUPERMASTER	401		// ���� ���� ������ ���� �ڵ�
#define ITEM_ECODE_SUPERMASTER	401		// ���� ���� ������ �� �ڵ�
#define ITEM_SCODE_PLUSPOINT	501		// �÷��� ����Ʈ ������ ���� �ڵ�
#define ITEM_ECODE_PLUSPOINT	501		// �÷��� ����Ʈ ������ �� �ڵ�

// ### [ ���αغ� ������ ] ###
#define ITEM_SCODE_ALLINSUPERMASTER  800	//1�ð� ���۹���
#define ITEM_ECODE_ALLINSUPERMASTER  899	//1�ð� ���۹���


typedef struct
{
	int        Index;		// ��������Ȯ���ϴ� �ε���
	int        NO;			// ����ũ �ѹ�
	int        Code;		// ������ �ڵ�
	char       PresID[16];	// ������ ���
	short      UseDay;		// ��� ������ �Ⱓ(��)
	short      UseNum;		// ��� ������ ȸ��
	SYSTEMTIME StartDate;	// ��� ���� �ð�
	short      Use;			// ��� ����(�Ǵ� ����� ȸ��)
	char       Reserved[2];
} GAMEITEM;

typedef struct
{
	BOOL	  bLogable;  // �α��� ���ɿ���
	int       nResult;   // -1:����, 0:����, 1:DB���¿���, 2:�����۾��ų� ��밡���� �����۾���, 3:�������� �ְ� ������� �ʾҴ�
	int       nNum;      // ������ ������ ����
	GAMEITEM  Item[MAX_ITEM_NUM];

} GAMEITEM_LIST;

// ���� ������ ����Ʈ ����(Ŭ���̾�Ʈ �۽ſ�)
typedef struct
{
	int			Kind;		// ����(0:������ ����Ʈ ���� 1:���� ������ ���� ����Ʈ ���� 100:������ ����Ʈ ���� ����)
	int			ErrorCode;	// ���� �ڵ�
	int			TotNum;		// �������� ����
	SYSTEMTIME	ServerTime;	// ������ ���� �ð�
	int			StrMsgLen1;	// ���ڿ� ���� 1 (������ ��� ������ ����� ���ڿ�)
	int			StrMsgLen2;	// ���ڿ� ���� 2 (������ ���� ���ڿ�)
} GAMEITEM_INFO;

// ���� ������ ���� ����
typedef struct
{
	int  Kind;			// ����
	char ItemName[256];	// ������ �̸�
	int  ItemCode;		// ������ �ڵ�
	int  StrMsgLen;		// ���� ���ڿ� ����
} BUYGAMEITEMINFO;

// ���Ӿ����� ��� ����(������ �ޱ� ��� ���)
typedef struct
{
	char		ID[16];			// ������ ����� ���̵�
	GAMEITEM	Item;			// �����(Ȥ�� �����) �������� ����
	BOOL		bUseError;		// ������ ����� ������ �߻��Ͽ��°�?
	BOOL		bMessage;		// �޽����� �����ϴ°�?
	char		StrMsg[256];	// �޽��� ��Ʈ��
} USEGAMEITEMINFO;

// ������ ���� ��ȯ ����ü
struct ItemState
{
	int		  iState;		// 0: �߸��� ������ 1:��밡���� ������ 2:������� ������ 3:����� �� ���� ������(�پ� ������)
	BOOL	  bNowUsing;	// ���� ������ΰ�?
	CTimeSpan LeftTime;		// ���� �ð�
	CTime	  EndDate;		// �����Ͻ�
};

// �������� ���¸� ������ ���� �Լ�
ItemState GameItem_GetItemState(GAMEITEM &item, SYSTEMTIME* pCurTime=NULL);

// �������� ������ �˾Ƴ��� ���� �Լ�(�������� �����ڵ带 ���� �ڵ�� ���)
int GameItem_GetItemKind(int itemcode);

// Ư�� ������ �������� ��������� �˾Ƴ�(�������� �����ϴ� ��� pExist�� �Ѿ��, ����/Ŭ���̾�Ʈ ����)
BOOL GameItem_IsUsingItem(GAMEITEM_LIST &itemlist, int itemcode, BOOL *pExist, SYSTEMTIME* pCurTime=NULL);

// �������� �̸��� ������ ���� �Լ�
CString GameItem_GetItemName(GAMEITEM& item);
CString GameItem_GetItemName(int code);

// �������� ��� ������ ������ ���� �Լ�
CString GameItem_GetItemHelp(int code);

// ������ �̹��� ������ ������ ���� �Լ�
CString GameItem_GetItemImageFile(int code);

// ������ �̹����� �ε����� ������ ���� �Լ�(Ŭ���̾�Ʈ�� �̹��� ����Ʈ���� ���)
int GameItem_GetItemImageIndex(int code);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetPokerLevel(INT64 money);
int GetLevelPoint(int point, int winnum, int loosenum);
int GetWinPro(int winnum, int loosenum);
int GetMaxBetMarble(int level);
int GetMyLeveltoGrade(int lv);

INT64 GetLevelToUserMoney(int lv); // �������� -> �Ӵ� 
int GetCreateRoomGrade(int gr);    // �游���� �ּҸӴ� �������

/////////////////////////////////////////////////////
////////////////     ��ī ����     //////////////////
/////////////////////////////////////////////////////
typedef struct
{
	char nCard; // ��ǥī��
	int  nRSF;
	int  nSF;
	int  nFCARD;
	char Card[12];
} POKERINFO;

typedef struct
{
	INT64 nHM; // �߼� ���ѸӴ�
	INT64 nFM; // ��� ���ѸӴ�
	INT64 nGM; // ���� ���ѸӴ�

	char   nHL; // �߼� ���ѷ���
	char   nFL; // ��� ���ѷ���
	char   nGL; // ���� ���ѷ���
}POKERSET;

/////////////////////////////////////////////////////////////////////////////////////////
// �α��� ��û ����	[SSO �۾�] - �߰�
typedef struct
{
	char ID[16];			// ID
	char Pass[16];			// ��ȣȭ�� �н������ 4��� Ŀ��
	char SockNameIP[20];
	char HostNameIP[20];
	int	 Port;
	int  AuthCookieLen;		// ���� ��Ű ����
	int  DataCookieLen;		// ������ ��Ű ����
	int  bForceLogin;		//2004.05.07 �������Ѿ� ����    <--------- �߰�
} ASK_LOGININFO;


//2004.05.07 �������Ѿ� ����
typedef struct{
	char ID[16];
	char PremLType;
	INT64 CurMoney;
	INT64 NorMoney;
	INT64 LeadersMoney;
	INT64 LeadersPlusMoney;
	char UniqNo[16];		// ����ũ �ѹ�
	char LeadersJoinURL[64]; // ������ ����Ȩ������
	char LeadersJoinURL2[64]; // ������ ����Ȩ������
} LEADERS_JOIN;

//2004.05.07 �������Ѿ� ����
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
////////////////    ����� ����    //////////////////
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// ����� ����
typedef struct
{
	short  UNum;				// ���� ��ȣ				- DB�� ����Ǵ� ������ �ƴ�
	char ID[16];			// ����� ���̵�
	// [SSO �۾�] - �Ʒ� ���� ����
	//char		Pass[10];		// ��й�ȣ						[SSO �۾�] - ����
	char Sex;				// ����
	char		PremCode;		// �����̾� ���� �ڵ�							[�����̾� �۾�]
	SYSTEMTIME	PremDate;		// �����̾� ���� �Ⱓ(������)					[�����̾� �۾�]
	char		bPremIP;		// �����̾� PC�� IP�ΰ�?						[PC�� �۾�]
	UINT  WinNum;			// �¼�
	UINT  LooseNum;			// �м�
	UINT  DrawNum;			// ����
	char nIcon;             // ����������

	char cObserver;			// ### [ ������� ] ### 
	char cAdminUser;		//### [������ ��� �۾�] ###

	INT64  PMoney;		    // ��Ŀ �Ӵ�
	POKERINFO PI;           // ��Ŀ ����
	short AvatarInfo[MAX_AVATARLAYER];	// �ƹ�Ÿ ����
	char Desc[62];			// �ϰ���� ��
	char PremLeadersCode;
} USERINFO;

typedef struct  // ���� ����� ���� ���ſ�
{
	short UNum;				// ���� ��ȣ
	char  ID[16];			// ����� ���̵�
	char Sex;				// ����
	char nIcon;             // ����������
	char Position;			// ���� ��ġ(WH_CHAN:ä�μ��� WH_LOBY: ���� WH_SHOP:���� WH_GAME:���ӹ�)
	char RoomNum;			// ���ȣ(���� ��ġ�� ���ӹ��϶��� ��ȿ)
	short cAdminUser;		//### [������ ��� �۾�] ###
} SMALLUSERINFO;

// �α��ν� ���� ����
typedef struct
{
	UINT  WinNum;
	UINT  LooseNum;
	UINT  DrawNum;			// ����
	INT64 pBackup;
} ORGINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// ���� ����	[���� ������ �۾�]
typedef struct
{
	int  GameCode;			// ���� �ڵ�
	char ServerName[20];	// ���� �̸�
	char NoticeURL[64];		// �������� URL
	char BannerURL1[64];	// �κ� ���� URL
	char BannerURL2[64];	// ���� ���� URL
	char LocationURL[64];	// ��ġ�˸�(ģ��ã��)�� URL
	char ItemURL[64];		// ������ ���� URL
	char SrvInfoMsg[80];	// ä�ηκ� �޼���
	char ItemHelpURL[64];	// ������ ���� URL
	INT64 JackPot;			// [ ###����### ]�α�� ���� ���� ���� �Ӵ�
	char CashURL[64];		// ĳ������ URL
} SERVERINFO;


///////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// �α��� ����
typedef struct
{
	USERINFO UI;			// ����� ����
	char RegID[16];			// �ֹε�� ��ȣ [���� ������ �۾�]
	char UniqNo[16];		// ����ũ �ѹ�
} LOGININFO;

/////////////////////////////////////////////////////////////////////////////////////////
// ä�� ���� ����
typedef struct
{
	int ChNum;				// ä�� ��ȣ(0���� ������ ���� �Ұ� �ڵ�)
	char ChanName[20];		// ä�� �̸�
	char ChanStyle;			// ä�� ��Ÿ��
	char channel_usernum;	// 5,7 �ο� ����
} ENTERCHANINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// ä�� ����
typedef struct
{
	char  ChNum;			// ä�� ��ȣ
	char  ChannelStyle;     // ä�� ��� �߰� 2003 0621 	// ### [ ä�� ���� ���� ] ###
	short NowUserNum;		// ���� �����ο�
	short MaxUserNum;		// �ִ� �����ο�
	char Title[20];			// ä�θ�
	char channel_usernum;	// 5,7 �ο� ����
} CHANNELINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// ����� ã�� ���
typedef struct
{
	char ID[16];			// ���̵�
	int  ChNum;				// ä�� ��ȣ
	short Position;			// ��ġ
	short RoomNum;			// ���ȣ
} RESULTFINDID;

////////////////////////////////////////////////////////////////////////////////////////
// �� ����

typedef struct
{
	short RoomNum;			// ���ȣ
	char  Title[32];			// ������
	short UNum;				// ������ ���� ��ȣ

	char  ID[16];			// ���� ID
	char  NowUserNum;		// ���� �����ο�
	char  MaxUserNum;		// �ִ� �����ο�
	char  NowObserverNum;    // ### [ ������� ] ### ������ �ο�
	char  Level;				// ���� ����(0:�ϼ� 1:�߼� 2:���)

	char  Secret;			// ���� ����(0=���� 1=�����)
	char  State;				// ����(0:����� 1:������)
	char  GameKind;			// ���� ����
	char  Pass[5];			// �н�����(������� �ϰ��)
	//  �ּ� �Ӵ� ����   (������޸Ӵ� * �ּҸӴϵ��)
	char UsernIcon;         // ���� ���
	char CreateRGrade;      // �ּҸӴ� ���
	char  nCard;			// ī�� ����
	char  FormKind;			// Normal, Choice 4��

	int		BetMoney;			// ���øӴ�
	
	char  bSuperMaster;		// ���� ���ΰ�?	[���� ������ �۾�]
	char  bPlayer;			// 5, 7 
	char  ChangeCardStep;	// ī��ٲٱ� Ƚ�� 1,2
	char Reserved;
} ROOMINFO;

typedef struct	// ���� �� ���� ���ſ�
{
	short  RoomNum;			// ���ȣ
	short  UNum;				// ������ ���� ��ȣ
	char   ID[16];			// ���� ID
	char   NowUserNum;		// ���� �����ο�
	char   MaxUserNum;		// �ִ� �����ο�
	char   NowObserverNum;    // ### [ ������� ] ###������ �ο�
	char   State;				// ����(0:����� 1:������)
	char   GameKind;			// ��������(0:������, 1:����)
	INT64 JackPotMoney;		// ���� 
} SMALLROOMINFO;

////////////////////////////////////////////////////////////////////////////////////////
// ���� ����
typedef struct
{
	ROOMINFO Ri;
	USERINFO Ui[MAX_NEWPLAYER];
} GAMEINFO;

////////////////////////////////////////////////////////////////////////////////////////
// �÷��̾� ���� ����
typedef struct
{
	char JoinState;		// ���� ���� ����(0:����, 1:����)
	char PlayState;		// ���� ����(0:�������� 1:������)
	char nState;     // 0: �ƹ����¾ƴ�, 1: ī��3��, 2: ī��2��,����1, 3: ī��4~5��, 4: ����, 5: ���
	char nCardTotal; // ��ī�� ��
	char nOpenTotal; // ���µ� ī���
	char nCard[4];   // ���� ī�� �������� ����ī�� ###[8��Ŀ]### 5
	char bCheck;

	char bOnGame; // ���� ���� ����
	char bFold;   // ���� ���� ����
	char nCheckIp;	// ### [ �ߺ��� IP�� ] ###
	char nSndFxKind; // ### [���� �߰� �۾�] ###
	char Reserved[2];
} PLAYSTATE;

////////////////////////////////////////////////////////////////////////////////////////
// �÷��̾� ���� ���� 2
typedef struct
{
	BOOL bJoinBanishVote;	// �߹� ��ǥ�� �������ΰ�?
	BOOL bHaveBanishVote;	// �߹� ��ǥ�� ���ƴ°�?
	// [ ���� ��Ŀ ]
	BOOL bActive;			// ��Ƽ�� ����(���� ����)
	int  nActive;			// ���� �ð�
	BOOL bWindOn;			// ������ ���ѽð� �÷���
	int  nWindOn;			// ������ ���ѽð�
	int  nFoldNum;			// �ڵ�����Ƚ��(3ȸ�̻��̸� ������) ������ ������ �÷��� �ʱ�ȭ
	BOOL bOnGame;			// ���� ���� ����
	int  In_OutCount;		// ### [ ������� ] ### ����, ���� �ݺ�����
	int  nChangeFlg;		// ī�� �ٲٱ� �÷���
//	short  nChangeWinEndFlg;// ī�� �ٲٰ� ������ ���� �˸��� �÷���
	int  Totalmoneylimit;	//2004.05.07 �������Ѿ� ����
} PLAYSTATE2;


typedef struct
{
	char nCard[4];   // ����ī��+����ī��+ ���̽�ī��
} BACKCARD; // ############## ī�� ������ �����÷��׷ε� ����Ѵ�

/////////////////////////////////////////////////////
///////////      �Թ�� ������ ����     /////////////
/////////////////////////////////////////////////////
typedef struct{
	int      nUNum;  // ����� ���̵�
	char    bInUse;  // �������(���������÷����̸� ������ ����)
	char       nUse; // ������� ������Ƚ��
	char     nState; // 0: �ƹ����¾ƴ�, 1:ī��3��, 2:ī��2�����1, 3: ī��4��~6��, 4: ������, 5: ���¸��
	char     nStyle; // 0: üũ, 1: ����, 2: ��Ʈ, 3: ����
	char       nBtn; // 0: ��(�ּ�), 1: ����(����), 2: �ƽ�(�ִ�)
	char     bFold;  // ���� ����
	char     bCall;  // �ݻ��� ����
	char   nLastCmd; // ������ ���
	INT64  nNowBat;  // �������������� �Ӵ�
	INT64  nTotBat;  // �ѹ��� �Ӵ�
	INT64 nRealBat;  // �ǹ��� �Ӵ�
	INT64  nPMoney;  // �������� ��Ŀ�Ӵ�
	INT64 nReCall;   // �ް� 
	INT64 nCalcBat;  // ��
}RAISEUSER;

/////////////////////////////////////////////////////
///////////      �Թ�� �÷��� ����     /////////////
/////////////////////////////////////////////////////
typedef struct{
	char  bDistCard;
	char  nDistUser;
	char  nRound;	 // ���� ����
	char  nState;
	char  bRaiseOn;  // ������ ���ӿ���
	char  bBtnBet;
	char  bBtnCheck;
	char  nPreCmd;
	char CurPlayer;
	char LastPlayer;
	char LastStyle;

	PLAYSTATE PS[MAX_PLAYER];
	RAISEUSER RU[MAX_PLAYER]; 	
	BACKCARD  BC[MAX_PLAYER]; // ���� ī��
}POKERGAME;

typedef struct{
	int  nStyle;    // 0: ����, 1.üũ , 2: ��, 3: ��Ʈ, 4: ����
	int  nBtn;      // 0: ��(�ּ�), 1: ����(����), 2: ����, 3: �ƽ�(�ִ�)
}POKERCLNT; //  ##############3 ī�峯���� �������ε� ����Ѵ�

typedef struct{
	char ID[16];
	char  nCard;             // ��Ƽ��, ��Ƽ��, ��ī��
	short  nRoom;
	char ChanName[25];		// ä�� �̸�
	INT64 JackPotPlus;	    // ���� ���ʽ� ����
	INT64 NowJackPot;	    // ���� �ܾ�
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
// �� ���� ����
typedef struct
{
	ROOMINFO Ri;
	USERINFO Ui[MAX_NEWPLAYER];// ### [ ������� ] ###
	POKERGAME Pg;
} ENTERROOMINFO;

////////////////////////////////////////////////////////////////////////////////////////
// �� ����� ���� ���� ����
typedef struct
{
	UINT WinNum;
	UINT LooseNum;
	UINT DrawNum;
	INT64 PMoney;
} CHANGEMYINFO;

////////////////////////////////////////////////////////////////////////////////////////
// �߹� ��ǥ ��û ����
typedef struct
{
	short RoomNum;		// �� ��ȣ
	short MyServPNum;	// ��û�� ���� �÷��� ��ȣ
	char MyID[16];		// ��û�� ���̵�
	short TarServPNum;	// �߹� ��� ���� �÷��� ��ȣ
	char TarID[16];		// �߹� ��� ���̵�
	char Reson[42];		// �߹� ����
} BANISHVOTEINFO;

////////////////////////////////////////////////////////////////////////////////////////
// ���� ���� ����

typedef struct
{
	char JoinID[MAX_PLAYER][16];		// ������ ���̵�
	PLAYSTATE Ps[MAX_PLAYER];			// �÷��̾� ���� ����ü
	char  nCard[4];
	int   nDistUser;					// ī�� ������
} STARTINFO;
////////////////////////////////////////////////////////////////////////////////////////

typedef struct{
	char     ID[16];

	short    nUNum;
	char     nFlag;			// -1: 0 : �ٲ��� �ʾҴ� 1 : �ٲ۴�	
	char     nChangeCardNum; // �ٲ� ���� 

	short    nCardNum;		// ���� ���� ���� 
	char     nCardIndex;
	char     nChangeStep;	// �ٲ� Ƚ�� 

	short	 nRoomNum;
	char	 nPNum;
	char	 nServerPNum;	
}CHANGECARD_62CUT;

typedef struct{
	CHANGECARD_62CUT data[MAX_PLAYER];
	short   nDistUser;					// ī�� ������
	short	nRoomNum;	
}CHANGECARD_62CUT_EX;


////////////////////////////////////////////////////////////////////////////////////////
// ���� ���� ���
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
	char      nNowWinner;   // ���� ���� �÷��̾� �ѹ�
	char      ID[16];		// ���� �Ƶ�	
	char      bOpen;		// ��� �̰�°�?(��� �׾ Ȧ�� �̰�°� FALSE, ������ �����ߴ°� TRUE)
	char      bJackPot;     // ������ �ִ°�   		
	
	INT64	  nRealBat;  // ���� ������ �������� �Ӵ� X
	INT64     nWinMoney;	// ���� ������ ����(�� ���þ��� ������ ����)

	POKERGAME Pg;
	GAMEOVERINFO  Ui[MAX_PLAYER];
	
	INT64     nGory;    // ���ݾ�
	int       nPercent; // �ۼ�Ƽ�� : �ܼ��� �����ִ� ��ɸ� �ִ� 1%,5%,10% ���	
	INT64     JackPotMoney;
} GAMEOVERRESULT;


/////////////////////////////////////////////////////
////////////    [���� ������Ʈ �۾�]   //////////////
/////////////////////////////////////////////////////

typedef struct
{
	short  nUNum;   // ����� ��ȣ
	char ID[16];  // ���̵�
	INT64 PMoney; // ��Ŀ�Ӵ� ����ġ
} AGENT_USER;


/////////////////////////////////////////////////////
///////////     [������ ��� �۾�]     //////////////
/////////////////////////////////////////////////////

#define MAX_ADMIN_NUM           (20) // �ִ� ������ ��
#define MAX_DENY_USER          (200) // �ִ� ���� ���� ���� ����

//������ ����
typedef struct
{
	char ID[16]; // ������ ���̵�
	char Grade;  // ���
	char UserIP[24];			// �������� ���� IP����	
} ADMIN_INFO;

//�Ѱ�����
typedef struct
{
	int nTotal;
	ADMIN_INFO admin[MAX_ADMIN_NUM];
} ADMIN_MANAGER; //���� ����

//������Ʈ ������
typedef struct
{
	int NowNum;
	ADMIN_MANAGER AdminMan;
} AGENT_ADMIN;

//������ �޼���
typedef struct
{
	int  UNum;
	char ID[16];
	char Message[130]; // �����δ� 128������ ���ȴ�
} ADMIN_MESSAGE;

//Ư������ ���ӱ���
typedef struct
{
	int  UNum;
	char ID[16];  // ������ ���̵�
	UINT nSecond; // ���ӺҰ� �ð�
} ADMIN_ACCESSDENY_USER;

//### [������ ��� �۾�] ###
typedef struct
{
	short  AdminUNum;
	char AdminID[16];
	short  TarUNum;
	char TarID[16];
} ADMINMESSAGE;

//  �� �ɼ� ���� ���� 
typedef struct
{
	short   RoomNum;			// ���ȣ
	short   UNum;				// ������ ���� ��ȣ
	char  ID[16];			// ���� ID
	char  Title[32];        // ������
	char  nCard;
	BOOL  bWhis;
	char  Reserved[2];
} STYLEROOMINFO;

//���� ���� �˸� �޼���
typedef struct //���
{
	short   RoomNum;		// ���ȣ
	short   UNum;			// ������ ���� ��ȣ
	char  ID[16];			// ���� ID
	char  Title[32];        // ������
	int  nCard;

} CHANGESTYLEROOMINFO;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [�����̾� �۾�]	                                //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int CheckPremiumState(USERINFO* pUI, SYSTEMTIME* pCurTime=NULL);


///////////////////////////////////////////////////////
////////////  ### [�α� ��Ͽ�] ###   /////////////////
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
////////////		[��ȭ ����]		   //////////////
/////////////////////////////////////////////////////
#define MAX_CAPTURE_NUM           (300) // �ִ� ��������

typedef struct
{
	BOOL bUse; // �������
	int  nTotal;                // ������ ��
	char ID[MAX_CAPTURE_NUM][16]; // ������ ���̵�
} CAPTURE_INFO;

// ### [ �ߺ��� IP�� ] ###
typedef struct 
{
	char IpIndex[MAX_PLAYER];
} IPARRINDEX;

//Ư�����̵�˻�
typedef struct
{
	BOOL bUse; // �������
	int  nTotal;                // ������ ��
	char ID[MAX_CAPTURE_NUM][16]; // ������ ���̵�
} SERACH_INFO;

////////////////////////////////////////////////////////////////////////////////////////
//  ### [�̺�Ʈ Ƽ��] ###
//	�̺�Ʈ ��÷�� �˸� ����
typedef struct
{
	char ID[16];		// ���̵�
	int  NowEventPrize;	// ���� �̺�Ʈ ��÷ ȸ��
} EVENTPRIZEINFO;

typedef struct {
	//BOOL bHaveEvent;	// �α��ν� �̺�Ʈ Ƽ���� ������ �ִ°�? ���� ���� ����
	int GameCnt;		// ����ȹ��
	int EventPrize;		// ȹ��
	int OrigEventPrize; // �����
	SYSTEMTIME LastDate;
} EVENT_PRIZE;
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
////////////    ### [ ������� ] ###   //////////////
/////////////////////////////////////////////////////
//����˸� �޼���
typedef struct
{
	short Seq; //�������
	short UNum;
	char ID[16];
} RESERVATIONINFO;


// dbwork - insert
typedef struct {
	int		Kind;
	INT64   	BigData[5];		// ��޺�
	int		SmallData[20];  // ������
	int     	Extra;
	COleDateTime 	LDate;
} STATISTICS_DBRECORD;


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#endif