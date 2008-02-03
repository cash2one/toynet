#ifndef __COMMMSG_H__
#define __COMMMSG_H__

#include "UserStruct.h"

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define MSG_ID_LEN	2		// �޼��� �νĹ��� ����	
#define MSG_ID ("NP")		// �޼��� �νĹ���
#define PACKET_HEADERSIZE 16	// ��Ŷ�� ��� ������ = sizeof(PACKETHEADER)

// ��Ŷ ��� ����ü
typedef struct
{
	char MsgID[2];
	short MsgLen;
	short Signal;
	char Dummy[2];
	UINT PackCnt;	// ��Ŷ ī����
	UINT PackCRC;
} PACKETHEADER;


// ������ �޼��� #########################################################
enum
{
	////////////////// ���� ��ġ�� �������� ���ƾ� �� �κ� ////////////////

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_CHECKVERSION=10,		// ���� üũ
	SV_RECONNECT=11,		// �����ؼ� ����
	SV_ASK_LOGIN=12,		// ���� ��û �α��� ����	// [SSO �۾�] - SV_ASK_LOGIN�� �̸��� �ٲ�
	SV_SERVERINFO=13,		// ���� ����	[���� ������ �۾�]


	/////////////////// ������ ������ �޼��� ///////////////////
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_ACCEPT_LOGIN=125,	// �α����� �����
	SV_REFUSE_LOGIN,		// �α����� �ź���(����: ���̵� ����, ��й�ȣ Ʋ��, ������ Ʋ��)
	SV_ASK_LOGOUT,			// �α׾ƿ��� ��û��
	SV_ACCEPT_LOGOUT,		// �α׾ƿ��� �����
	SV_HEARTBEAT,			// ��Ʈ��Ʈ
	SV_ASK_OUTCHAN,			// ä�� ������ �䱸
	SV_ACCEPT_OUTCHAN,		// ä�� ������ �㰡
	SV_ASK_ENTERCHAN,		// ä�� ������ �䱸
	SV_ACCEPT_ENTERCHAN,	// ä�� ������ �㰡
	SV_ASK_FINDID,			// ���̵� ã�� ��û
	SV_RESULT_FINDID,		// ���̵� ã�� ���
	SV_ASK_DETAILUSERINFO,	// ������� �� ������ �䱸(�����ڿ�)
	SV_DETAILUSERINFO,		// ������� �� ����
	SV_USERINFO,			// ����� ������ ������(��� : Ư�� ����� ���� �䱸��)
	SV_CHANGEMYINFO,		// ����� ����� ������ �˷���
	SV_ALLUSERINFO,			// ��� ������ ����� �����ش�(��� : �䱸�ϴ� �����)
	SV_NEWUSER,				// ���ο� ����ڰ� ���ӵǾ���(��� : ��ü �����)
	SV_OUTUSER,				// ����ڰ� ���������Ͽ���(��� : ��ü �����)
	SV_CHANGEUSER,			// ������ �ٸ� ������� ������ ����Ǿ���(���: ��ü�����)
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_ACCEPT_CHANGECHAR=330,	// ����� ĳ���͸� �ٲٱ⸦ ���
	SV_REFUSE_CREATEROOM,	// �� ����⸦ �ź���(���: �� ������ �õ��� �����)
							// ������ ���� ���� �ڵ�(0:�� ���� ���� �� �ʰ�, 1:������ ���̵�)
	SV_ACCEPT_CREATEROOM,	// �� ����� �䱸�� ���� ���� ���������(���: �� ������ �õ��� �����)
							// ����ID, �� ����, �� ����(����, ����)
	SV_ALLROOMINFO,			// ������ ���� ��ü ������ �����ش�(��� : �䱸�ϴ� �����)
	SV_ROOMINFO,			// Ư�� ���� ������ �����ش�(��� : �䱸�ϴ� �����)
	SV_ROOMMASTER,			// Ư���� ���� ���� ������ �����ش�(��� : �䱸�ϴ� �����)
	SV_NEWROOM,				// ���ο� ���� �����Ǿ����� �˸�
	SV_OUTROOM,				// ���� ��������� �˸�
	SV_CHANGEROOM,			// �� ������ �����Ǿ����� �˸�
	SV_USEROUTROOM,			// ����ڰ� �濡�� ���Դ� - ���� �� �÷��̾�鿡�� �뺸
	SV_ACCEPT_OUTROOM,		// �濡�� �����⸦ �㰡
	SV_CHANGEROOMMASTER,	// ������ �ٲ��� �˸���
	SV_ABNORMALOUTROOM,		// ����ڿ��� ������������ �����ϱ⸦ ��û(������� �����ӿ� �������� ���)

	// ### [���� �߰� �۾�] ###
	SV_ASK_CHANGESNDFX,     // ��û
	SV_ACCEPT_CHANGESNDFX,  // ���

	/////////////////// ������ �޴� �޼��� ///////////////////

	SV_ASK_BADUSER,			// ä��â ��ȭ���� �Ű� ��û
	SV_ASK_CHANGEMYINFO,	// ���� ����� ���� ������ �䱸
	SV_ASK_ALLCHANINFO,		// ��ü ä�� ������ �䱸
	SV_ALLCHANINFO,			// ��ü ä�� ����
	SV_ASK_ALLROOMINFO,		// ��ü �� ����� �䱸
	SV_ASK_ROOMINFO,		// Ư�� �������� �䱸
	SV_ASK_ROOMMASTER,		// Ư�� ���� ���� ������ �䱸
	SV_ASK_ALLUSERINFO,		// ��� ������ ����� �䱸
	SV_ASK_USERINFO,		// Ư�� ����� ������ �䱸
	SV_ASK_CHANGECHAR,		// ����� ĳ���͸� �ٲٱ⸦ �䱸
	SV_ASK_CREATEROOM,		// �� ����⸦ �䱸
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_ASK_ENTERROOM=555,		// �濡 �����ϱ⸦ �䱸
	SV_ACCEPT_ENTERROOM,	// �濡 �����ϱ⸦ �㰡��(���: ������ �䱸�� �����)
	SV_REFUSE_ENTERROOM,	// �� �����û�� �ź���(���: ������ �䱸�� �����)
	SV_USERENTERROOM,		// ����ڰ� �濡 �����Ͽ���
	SV_LOBYOPEN,			// ���� ���� �ʴ�â ���� ���� �˸�
	SV_ASK_OUTROOM,			// �濡�� �����⸦ �䱸
	SV_ASK_INVITE,			// ��û �޼����� ����
	SV_REFUSE_INVITE,		// ��û �޼����� ����
	SV_ASK_BANISHVOTE,		// �߹� ��û
	SV_MYBANISHVOTE,		// ���� �߹� ��ǥ ���
	SV_NOWHISPER,			// �Ӹ� �ź�,�㰡 ���� ����
	SV_ASK_FORCEBANISH,		// ���� �߹� ��û	[���� ������ �۾�]

	/////////////////// ���� ������ �޼��� [���� ������ �۾�] ///////////////////

	SV_ASK_GAMEITEMLIST,	// ���� ������ ��� ������ ��û
	SV_GAMEITEMLIST,		// ���� ������ ����� ������
	SV_USEGAMEITEM,			// ���� ������ ��� ��û
	SV_BUYGAMEITEM,			// ���� �������� �����Ұ����� ���
	
	/////////////////// ���� �޼��� ///////////////////

	SV_SERVERMSG,			// ���� �޼���
	SV_CHATDATA,			// ���ǿ� ä�� ����Ÿ(����: ������ ������ ��� ���� �޼��� ���)
	SV_WHISPERCHAT,			// �ӼӸ�

	/////////////////// ���� �޼��� ///////////////////

	SV_STARTGAME,			// ������ ���۵Ǿ���
	SV_GAMEOVERRESULT,		// ���� ����Ǿ����� ����� �뺸(��� : ���� �� �÷��̾� ���)
	SV_GAMEOVER,			// �������� ������ ������� �뺸(������ ����)
							// ���ȣ, ID, Ǭ ������, ���� �Ѿ��� ������ ����(������ ��� �� ��ŭ)
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_STARTCARD=666,		// ó�� 3���� ī�带 ����
	SV_SELECTCARD,			// ī�� Ÿ�̸� ����	
	SV_CHANGECARD,			// ī�� �ٲٱ⸦ ��û
	SV_BUYCARD,				// ī�� ��⸦ ��û
	SV_SPECIALCARD,			// ��� ���� �����ڿ��� �˸�
	SV_HIDDENCARD,			// ������ ���� ī�� ����
	SV_RAISE,				// ���� �˸� ������
	SV_WINDCARDEND,			// ù 3���� ������ ������ �ϳ��� ī�带 ���� => ��� ���ý� ���ӽ���
	SV_ENDGAME,				// ������ �˸�
	SV_ENDRAISE,			// ��� ������ �� => ���� �ο� ���� ���� ��û
	
	SV_FOLDUSER,			// ������ ������
	SV_ASK_QUICKSTART,		// ���� �ٷ� ����
	SV_REFUSE_QUICKSTART,	// �ٷν��� �ȵ� (�游���)

	/////////////////// ### [�̺�Ʈ Ƽ��] ### ///////////////////
	SV_EVENTPRIZE,			// �̺�Ʈ ��÷�� �˸�  
	SV_EVENTNOTICE,			// �̺�Ʈ ���� �˸�(�� �����)

	/////////////////// ## [62cut �߰� �޼���] ////////////////////

	SV_ASK_CHANGECARD_62CUT,			// ������ ������ �ִ� ī�� �ٲٱ� ( ���徿 �Ѹ�
	SV_CHANGECARD_62CUT_RESULT,			// ������ ������ �ִ� ī�� �ٲٱ�
	SV_ASK_CHANGECARD_62CUT_EX,			// 62cut �ٲ� ī�� �ѱ� (���ÿ� �Ѹ���	

	////////////////////////////////////////////////////////////////
	SV_ACTIVE_STARTBUTTON,			// ���� ��ư Ȱ��ȭ

	SV_LEADERSJOIN,			//2004.05.07 �������Ѿ� ����

	/////////////////// ### [ ������� ] ###  ///////////////////

	SV_ASK_PARTICIPATION=750,   // ������ ��û
	SV_ACCEPT_PARTICIPATION, // ���� 
	SV_ENTER_PARTICIPATION,  // �ٸ� ����ڿ��� �˸���
	SV_ASK_EMERAGE,         // ���������� ��������
	SV_ACCEPT_EMERAGE,      // ����
	SV_ENTER_EMERAGE,		// �ٸ� ����ڿ��� �˸���
	SV_RESERVATIONINFO,     // ������ ����
	SV_OBSERVER_MESSAGE,	// ���� ���� �޼���

	/////////////////// ### [������ ��� �۾�] ###  ///////////////////

	NM_ADMIN_WHISPERCHAT,	// �Ӹ��ϱ�(����)
	NM_ADMIN_WARNUSER,		// ����� ����(���)
	NM_ADMIN_OUTUSER,		// ����� �����ϱ�(���)
	NM_ADMIN_CHANGEROOM,	// �׹� �ɼ� �����û(���)
	NM_ADMIN_DELROOM,		// �������ϱ�(���)
	SV_CHANGESTYLEROOMINFO, // ���̸� ���� �˸�(���)
	NM_SPECIALMESSAGE,      // Ư�� ���� �޼���(���)
	
	SV_SERVERNOTIC,			// ���� ���� �޼���

	SV_ASK_SAFEANGEL,		// [��ȣõ��] 2004.07.08 ����Ұ��̳� 
	CL_CHARGE_SAFEANGEL,	// [��ȣõ��] 2004.07.08 �Ӵ� ���� �޽���
};

// Ŭ���̾�Ʈ�� �޼��� #########################################################

enum
{
	/////////////////// ���� �޼��� ///////////////////
	
	CL_EXITSUBSCRIPT=983,	// �÷��̾��� ������ ���� ����
	CL_CHATDATA,			// Ŭ���̾�Ʈ���� ä�� �޼���

	/////////////////// ���� ���� ���� �޼��� /////////

	CL_READYTOSTART,		// ���� ���� �غ� �Ϸ�(������Ҷ� �����)
	CL_ASK_CHANGEGAMEKIND,	// ���� ��� �ٲ� ��û(0:������ 1:����)
	CL_CHANGEGAMEKIND,		// ���� ��� �ٲ� ���(0:������ 1:����)
	CL_ASK_STARTGAME,		// ���� ���� ��û
	CL_STARTINFO,			// ���� ���� ����(���: ��ü)
	
	CL_ASK_FOLD,			// �÷��̾� ����
	CL_ASK_SELECTCARD,		// ������� ī�弱��
	CL_ASK_RAISE,			// ������� ������
	CL_WINDCARDEND,			// ������� ī�峯���� ����
	CL_ROOM_TIMEOUT,		// ����� �� Ÿ�� �ƿ� �޼��� (���� �濡�� ������)	
	CL_USERRESULT,			// ������ ����� ��������
	CL_CARDCHANGE_62CUT_RESULT,	// ī�� �ٲٱ� ����� ������
	CL_CARDCHANGE_END_62CUT,	//ī�� �ٲٱ� ���� ���̽� ��û
	CL_ASK_CHSTYLEROOMINFO,	// ������ ���� ��û 

	CL_SELECTWINCASE,		// ��ɷ� �ºθ� �ܷ�ڴ°� ���� �ο� ����

	// [ ���� ������ ]
	CL_ITEMRESULT,			// ������ �����..( �������� ���� ��쿡��.. )
	CL_ITEMLIST,			// ������ ����Ʈ..
	CL_ITEMLOGIN,			// �������� ����� �α���..
	CL_ASK_ITEMLOGIN,		// ������ ����� ���ΰ� ���.
	CL_ACCEPT_ITEMLOGIN,	// ������ ���� �α��� �㰡.

	// [���� ������ �۾�]
	CL_ASK_JUMPITEM,		// ä�ο��� ���������� ��� ���.
	CL_JUMPITEM_LOGIN,		// ä�ο��� ���������� ��� �α�
	CL_ACCEPT_JUMPITEM_CH,  // ���� -> ���� ��û DB OK => �������(������ ���̾�α� �ڽ��� �����ش�)
	CL_ACCEPT_JUMPITEM,		// ä�ο��� ���������� �ѹ��̶� ����� ������ ������(�ٷ� ����)


	// [���� ������Ʈ �۾�] 
	NM_AGENT_CONNECT = 5005, // ������Ʈ�� ���ӵǾ���
	NM_AGENT_NEWUSER,
	NM_AGENT_OUTUSER,
	NM_AGENT_NEWUSERERROR,	 // �ߺ� ��û 
	NM_AGENT_USERLIST,		 // ���� ��� ��û
	NM_AGENT_DUBLECON,		 // ���� ��� ��û
	NM_AGENT_USERLIST1,
	NM_AGENT_USERLIST2,
	NM_AGENT_USERLIST3,
	NM_AGENT_USERLIST4,
	NM_AGENT_USERLIST5,
	NM_AGENT_USERLIST_R1,
	NM_AGENT_AVERAGE,       // ���� �ݾ��� ����� ���Ѵ�.
	//### [������ ��� �۾�] ###
	NM_AGENT_ADMININFO,		// ������ ���� ���̵� �����Ѵ�.

};

// ��� �޼��� �⺻ Ŭ���� #########################################################
class CCommMsg
{
public:
	BOOL bReferedData;			// ������ �������ΰ�? (������ �����ʹ� �޸� �����͸� ������ �� ����)
	int  TotalSize;				// ����� ������ �������� �� ������
	char *pData;				// ������ ������(��� ����)
	PACKETHEADER *pHead;		// ��Ŷ ��� ������

	CCommMsg();					// ���۰� �������� �ʴ� ������(���۸� ���� �Ҵ��ϰԵ�)
	CCommMsg(char *pRefBuf);	// ���۸� �����Ͽ� Ŭ������ �����ϴ� ������
	~CCommMsg();

	void Clear();
	int GetTotalSize();

	static void BuildPattern(char *pdata, UINT packcnt, int size);
	static void EncodeAll(char *pdata);
	static BOOL DecodeAll(char *pdata);

	// �޸� ������ �����ͷκ��� ��� �޽����� �м��Ͽ� �ʿ��� ������ �˷���
	// (lpdata�� �ּ��� ����� �����Ǵµ� �ʿ��� �⺻������ �̻� �Ǿ���Ѵ�)
	BOOL CheckInfo(char *lpdata, int *psignal, int *pmsglen);

	// ����� ������ְ� �ʿ��� �����ŭ �޸𸮸� �Ҵ��Ѵ�
	// (������ ����ؾ��� �����͸� ��ȯ�Ѵ�)
	char* SetHeader(int signal, int msglen); 

	// ���� �����͸� �̿��Ͽ� ����� �����Ѵ�(������ ó���� �����͸� ��ȯ�Ѵ�)
	// ����� ������ �������� ù��° �����Ϳ� �������� ��ü ����� �޴´�
	char* GetHeader(char *lpdata, int size);

	// �޸𸮸� �����Ѹ�ŭ Ÿ�� �����͸� ���������ش�(����: �޸𸮰� �Ҵ�� �����͸� ���� ���� ����)
	// �����ϱ���(���� �Ǳ���)�� �����͸� �����Ѵ�
	char* SmartCpy(char **tar, const void *src, int size);

	// tar�����͸� ���������ش�
	// ���� ��Ű������ �����͸� �����Ѵ�
	char* Jump(char **tar, int size);
};

////////////////////////////////////////////////////////////////////////////
//                      ������ �޼��� Ŭ����
////////////////////////////////////////////////////////////////////////////

/////////// ������ üũ��
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

/////////// ���� ����	[���� ������ �۾�]
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
/////////// �α��� ��û ����(ID, Password, Game Version)
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

//2004.05.07 �������Ѿ� ����
class CSV_LEADERSJOIN : public CCommMsg
{
public:
	CSV_LEADERSJOIN() : CCommMsg() {}
	CSV_LEADERSJOIN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	LEADERS_JOIN *LJ;
	BOOL Set(LEADERS_JOIN* pLJ);
	BOOL Get(char* lpdata, int size);
};




// [SSO �۾�] - �߰�
/////////// �α��� ��û ����(ID, Password, Game Version)
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

/////////// �α����� �ź���(�ź� ���� �ڵ带 ����)	[SSO �۾�] - ��ü
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


/////////// �α����� �����(����� ������ ����)
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
/////////// �α����� �ź���(�ź� ���� �ڵ带 ����)
class CSV_REFUSE_LOGIN : public CCommMsg
{
public:
	CSV_REFUSE_LOGIN() : CCommMsg() {}
	CSV_REFUSE_LOGIN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Code;
	BOOL Set(int code); // 0:���̵� Ʋ��, 1:��й�ȣ�� Ʋ��, 2:������ Ʋ��
	BOOL Get(char* lpdata, int size);
};
*/
/////////// �α׾ƿ��� ��û
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

/////////// �α׾ƿ��� ���
class CSV_ACCEPT_LOGOUT : public CCommMsg
{
public:
	CSV_ACCEPT_LOGOUT() : CCommMsg() {}
	CSV_ACCEPT_LOGOUT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// ��Ʈ��Ʈ
class CSV_HEARTBEAT : public CCommMsg
{
public:
	CSV_HEARTBEAT() : CCommMsg() {}
	CSV_HEARTBEAT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// ä�� ������ ��û
class CSV_ASK_ENTERCHAN : public CCommMsg
{
public:
	CSV_ASK_ENTERCHAN() : CCommMsg() {}
	CSV_ASK_ENTERCHAN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *ChNum;
	BOOL Set(int cnum);
	BOOL Get(char* lpdata, int size);
};

/////////// ä�� ������ ���
class CSV_ACCEPT_ENTERCHAN : public CCommMsg
{
public:
	CSV_ACCEPT_ENTERCHAN() : CCommMsg() {}
	CSV_ACCEPT_ENTERCHAN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ENTERCHANINFO *EC;
	BOOL Set(ENTERCHANINFO *pEC);
	BOOL Get(char* lpdata, int size);
};

/////////// ä�� ������ ��û
class CSV_ASK_OUTCHAN : public CCommMsg
{
public:
	CSV_ASK_OUTCHAN() : CCommMsg() {}
	CSV_ASK_OUTCHAN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// ä�� ������ ���
class CSV_ACCEPT_OUTCHAN : public CCommMsg
{
public:
	CSV_ACCEPT_OUTCHAN() : CCommMsg() {}
	CSV_ACCEPT_OUTCHAN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// ���̵� ã�� ��û
class CSV_ASK_FINDID : public CCommMsg
{
public:
	CSV_ASK_FINDID() : CCommMsg() {}
	CSV_ASK_FINDID(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;
	BOOL Set(char* id);
	BOOL Get(char* lpdata, int size);
};

/////////// ���̵� ã�� ���
class CSV_RESULT_FINDID : public CCommMsg
{
public:
	CSV_RESULT_FINDID() : CCommMsg() {}
	CSV_RESULT_FINDID(char *pRefBuf) : CCommMsg(pRefBuf) {}

	RESULTFINDID *RF;
	BOOL Set(RESULTFINDID *pRF);
	BOOL Get(char* lpdata, int size);
};

/////////// ��ȭ���� �Ű��û
class CSV_ASK_BADUSER : public CCommMsg
{
public:
	CSV_ASK_BADUSER() : CCommMsg() {}
	CSV_ASK_BADUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL *bAnnounce;	// �Ű� �˸� ����
	int *RNum;			// ���ȣ(0�̸� ����)
	int *UNum;			// �Ű��� ������ȣ
	char *ID;			// �Ű��� ���̵�
	char *TargetID;		// �Ű��� ���̵�
	char *SubmitReason;	// �Ű����
	int *l_StrWatchID;	// ���� ���̵� ��Ʈ���� ����
	char *WatchID;		// ���� ���̵� ��Ʈ��
	int *l_StrChat;		// �Ű�� ä�� ��Ʈ���� ����
	char *StrChat;		// �Ű�� ä�� ��Ʈ��
	BOOL Set(BOOL announce, int rnum, int unum, char* id, char *tarid, char *reason, char *watchid, char *chat);
	BOOL Get(char* lpdata, int size);
};

/////////// ��ü ä�� ����� �䱸�Ѵ�
class CSV_ASK_ALLCHANINFO : public CCommMsg
{
public:
	CSV_ASK_ALLCHANINFO() : CCommMsg() {}
	CSV_ASK_ALLCHANINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// ��ü ä�� ����� �����ش�
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

/////////// ��ü ����� ����� �䱸�Ѵ�
class CSV_ASK_ALLUSERINFO : public CCommMsg
{
public:
	CSV_ASK_ALLUSERINFO() : CCommMsg() {}
	CSV_ASK_ALLUSERINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// ��ü ����� ����� �����ش�
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

/////////// Ư�� ������� �� ������ �䱸�Ѵ�
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

/////////// Ư�� ������� �� ������ ������
class CSV_DETAILUSERINFO : public CCommMsg
{
public:
	CSV_DETAILUSERINFO() : CCommMsg() {}
	CSV_DETAILUSERINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	USERINFO *UI;
	BOOL Set(USERINFO *pUI);
	BOOL Get(char* lpdata, int size);
};


/////////// Ư�� ����� ������ �䱸�Ѵ�
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

/////////// Ư�� ����� ������ ������
class CSV_USERINFO : public CCommMsg
{
public:
	CSV_USERINFO() : CCommMsg() {}
	CSV_USERINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	USERINFO *UI;
	BOOL Set(USERINFO *pUI);
	BOOL Get(char* lpdata, int size);
};

/////////// ���ο� ����ڰ� ���������� �˸���
class CSV_NEWUSER : public CCommMsg
{
public:
	CSV_NEWUSER() : CCommMsg() {}
	CSV_NEWUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SMALLUSERINFO *SUI;
	BOOL Set(SMALLUSERINFO* pSUI);
	BOOL Get(char* lpdata, int size);
};

/////////// ����ڰ� ������ ���������� �˸���
class CSV_OUTUSER : public CCommMsg
{
public:
	CSV_OUTUSER() : CCommMsg() {}
	CSV_OUTUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SMALLUSERINFO *SUI;
	BOOL Set(SMALLUSERINFO* pSUI);
	BOOL Get(char* lpdata, int size);
};

/////////// �ٸ� ������� ���� ������ ������
class CSV_CHANGEUSER : public CCommMsg
{
public:
	CSV_CHANGEUSER() : CCommMsg() {}
	CSV_CHANGEUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SMALLUSERINFO *SUI;
	BOOL Set(SMALLUSERINFO* pSUI);
	BOOL Get(char* lpdata, int size);
};

/////////// ��ü �� ����� �䱸�Ѵ�
class CSV_ASK_ALLROOMINFO : public CCommMsg
{
public:
	CSV_ASK_ALLROOMINFO() : CCommMsg() {}
	CSV_ASK_ALLROOMINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// ��ü �� ����� �����ش�
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

/////////// �� ������ �䱸
class CSV_ASK_ROOMINFO : public CCommMsg
{
public:
	CSV_ASK_ROOMINFO() : CCommMsg() {}
	CSV_ASK_ROOMINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	BOOL Set(int roomnum);
	BOOL Get(char* lpdata, int size);
};

/////////// �� ����
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

/////////// ĳ���� �ٲٱ⸦ ��û
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

/////////// ĳ���� �ٲٱ⸦ ��û ���
class CSV_ACCEPT_CHANGECHAR : public CCommMsg
{
public:
	CSV_ACCEPT_CHANGECHAR() : CCommMsg() {}
	CSV_ACCEPT_CHANGECHAR(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *CharNum;
	BOOL Set(int charnum);
	BOOL Get(char* lpdata, int size);
};

/////////// �� ���� �䱸
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

/////////// �� ���� �ź�
class CSV_REFUSE_CREATEROOM : public CCommMsg
{
public:
	CSV_REFUSE_CREATEROOM() : CCommMsg() {}
	CSV_REFUSE_CREATEROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Code;
	BOOL Set(int code);
	BOOL Get(char* lpdata, int size);
};

/////////// �� ���� ���
class CSV_ACCEPT_CREATEROOM : public CCommMsg
{
public:
	CSV_ACCEPT_CREATEROOM() : CCommMsg() {}
	CSV_ACCEPT_CREATEROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ROOMINFO *RI;
	BOOL Set(ROOMINFO* pRI);
	BOOL Get(char* lpdata, int size);
};

/////////// �� ������ ��û
class CSV_ASK_ENTERROOM : public CCommMsg
{
public:
	CSV_ASK_ENTERROOM() : CCommMsg() {}
	CSV_ASK_ENTERROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	char *ID;
	char *Pass;
	BOOL Set(int rnum, int unum, int sndKind, char *id, char *pass=NULL);// �߰�
	BOOL Get(char* lpdata, int size);
};

/////////// ������ �㰡��
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

/////////// ���� ��û�� �ź���
class CSV_REFUSE_ENTERROOM : public CCommMsg
{
public:
	CSV_REFUSE_ENTERROOM() : CCommMsg() {}
	CSV_REFUSE_ENTERROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Code;
	BOOL Set(int code); // 0:�ο��ʰ� 1:��й�ȣ�� Ʋ��
	BOOL Get(char* lpdata, int size);
};

/////////// ���ο� ���� �����Ǿ���
class CSV_NEWROOM : public CCommMsg
{
public:
	CSV_NEWROOM() : CCommMsg() {}
	CSV_NEWROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ROOMINFO *RI;
	BOOL Set(ROOMINFO* pRI);
	BOOL Get(char* lpdata, int size);
};

/////////// ���� �������
class CSV_OUTROOM : public CCommMsg
{
public:
	CSV_OUTROOM() : CCommMsg() {}
	CSV_OUTROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SMALLROOMINFO *SRI;
	BOOL Set(SMALLROOMINFO* pSRI);
	BOOL Get(char* lpdata, int size);
};

/////////// �� ������ ������
class CSV_CHANGEROOM : public CCommMsg
{
public:
	CSV_CHANGEROOM() : CCommMsg() {}
	CSV_CHANGEROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	SMALLROOMINFO *SRI;
	BOOL Set(SMALLROOMINFO* pSRI);
	BOOL Get(char* lpdata, int size);
};

/////////// ������ �ٲ�
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

// ### [ �ߺ��� IP�� ] ### // ### [���� �߰� �۾�] ###
/////////// �濡 ����ڰ� �����Ͽ���
class CSV_USERENTERROOM : public CCommMsg
{
public:
	CSV_USERENTERROOM() : CCommMsg() {}
	CSV_USERENTERROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	USERINFO *UI;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	BOOL Set(int rnum, USERINFO *pUI, IPARRINDEX *pIPA, int sndKind);
	BOOL Get(char* lpdata, int size);
};

// ### [ �ߺ��� IP�� ] ###
/////////// ����ڰ� �濡�� ������
class CSV_USEROUTROOM : public CCommMsg
{
public:
	CSV_USEROUTROOM() : CCommMsg() {}
	CSV_USEROUTROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	BOOL Set(int rnum, int unum, char *id, IPARRINDEX *pIPA);
	BOOL Get(char* lpdata, int size);
};

/////////// ����ڰ� �濡�� �����⸦ ��û
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

/////////// ����ڰ� �濡�� �����⸦ �㰡
class CSV_ACCEPT_OUTROOM : public CCommMsg
{
public:
	CSV_ACCEPT_OUTROOM() : CCommMsg() {}
	CSV_ACCEPT_OUTROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

/////////// ���� ����� ������ �����
class CSV_CHANGEMYINFO : public CCommMsg
{
public:
	CSV_CHANGEMYINFO() : CCommMsg() {}
	CSV_CHANGEMYINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGEMYINFO *pCMI;
	BOOL Set(CHANGEMYINFO *pcmi);
	BOOL Get(char* lpdata, int size);
};

/////////// ���������� ����(������� �����ӿ� �������� ���)
class CSV_ABNORMALOUTROOM : public CCommMsg
{
public:
	CSV_ABNORMALOUTROOM() : CCommMsg() {}
	CSV_ABNORMALOUTROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};

////////// ���� ���� �ʴ�â ���� ���� �˸�
class CSV_LOBYOPEN : public CCommMsg
{
public:
	CSV_LOBYOPEN() : CCommMsg() {}
	CSV_LOBYOPEN(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL *bLobyOpen;
	BOOL Set(BOOL blobyopen);
	BOOL Get(char* lpdata, int size);
};

/////////// ��û �޼����� ����
class CSV_ASK_INVITE : public CCommMsg
{
public:
	CSV_ASK_INVITE() : CCommMsg() {}
	CSV_ASK_INVITE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TarUNum;		// ��� ���� ��ȣ
	char *TarID;		// ��� ���̵�
	int *RoomNum;		// �� ��ȣ
	char *RoomPass;		// �� ��й�ȣ
	USERINFO *MyUI;		// ���� ����� ����
	int *l_StrMsg;		// ���Ҹ��� ����
	char *StrMsg;		// ���Ҹ�
	BOOL Set(int targetunum, char* targetid, int rnum, char *pass, USERINFO *pMyUI, char *strmsg);
	BOOL Get(char* lpdata, int size);
};

/////////// ��û �޼����� �ź���
class CSV_REFUSE_INVITE : public CCommMsg
{
public:
	CSV_REFUSE_INVITE() : CCommMsg() {}
	CSV_REFUSE_INVITE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TarUNum;		// ��� ���� ��ȣ
	char *TarID;		// ��� ���̵�
	char *MyID;			// ���� ���̵�
	int *l_StrMsg;		// ���Ҹ��� ����
	char *StrMsg;		// ���Ҹ�
	BOOL Set(int targetunum, char* targetid, char *myid, char *strmsg);
	BOOL Get(char* lpdata, int size);
};

/////////// �߹� ��ǥ ��û
class CSV_ASK_BANISHVOTE : public CCommMsg
{
public:
	CSV_ASK_BANISHVOTE() : CCommMsg() {}
	CSV_ASK_BANISHVOTE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BANISHVOTEINFO *pBV;
	BOOL Set(BANISHVOTEINFO *pbv);
	BOOL Get(char* lpdata, int size);
};

/////////// ���� �߹� ��ǥ ���
class CSV_MYBANISHVOTE : public CCommMsg
{
public:
	CSV_MYBANISHVOTE() : CCommMsg() {}
	CSV_MYBANISHVOTE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL *bAgree;
	BOOL Set(BOOL bagree);
	BOOL Get(char* lpdata, int size);
};


/////////// ���� �˸� �޼���
class CSV_SERVERMSG : public CCommMsg
{
public:
	CSV_SERVERMSG() : CCommMsg() {}
	CSV_SERVERMSG(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Kind;				// ����(0:�⺻ ��� 1:�޼��� �ڽ� ���)
	short *l_StrChat;		// ä�� ��Ʈ���� ����
	char *StrChat;			// ä�� ��Ʈ��
	BOOL *bColorAssigned;	// �ؽ�Ʈ �÷��� �����Ǿ��°�?
	COLORREF *Color;		// �ؽ�Ʈ �÷�

	BOOL Set(int kind, char *chat, BOOL bcolorassigned = FALSE, COLORREF color = RGB(255,0,0));
	BOOL Get(char* lpdata, int size);
};

/////////// �Ӹ� ��� ���� ����
class CSV_NOWHISPER : public CCommMsg
{
public:
	CSV_NOWHISPER() : CCommMsg() {}
	CSV_NOWHISPER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL *bNoWhisper;
	BOOL Set(BOOL bnowhisper);
	BOOL Get(char* lpdata, int size);
};

/////////// ���ǿ� ä�� ����Ÿ
class CSV_CHATDATA : public CCommMsg
{
public:
	CSV_CHATDATA() : CCommMsg() {}
	CSV_CHATDATA(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;			// ���̵� ��Ʈ��
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��

	BOOL Set(char* id, char *chat);
	BOOL Get(char* lpdata, int size);
};

/////////// �ӼӸ�
class CSV_WHISPERCHAT : public CCommMsg
{
public:
	CSV_WHISPERCHAT() : CCommMsg() {}
	CSV_WHISPERCHAT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;			// ������ ��� ���̵� ��Ʈ��
	int  *DestUNum;		// �޴� ��� ����� ��ȣ
	char *DestID;		// �޴� ��� ���̵�
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��
	BOOL Set(char *id, int destunum, char *destid, char *chat);
	BOOL Get(char* lpdata, int size);
};

////////// ���� �߹� ��û	[���� ������ �۾�]
class CSV_ASK_FORCEBANISH : public CCommMsg
{
public:
	CSV_ASK_FORCEBANISH() : CCommMsg() {}
	CSV_ASK_FORCEBANISH(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;
	BOOL Set(char* id);
	BOOL Get(char* lpdata, int size);
};

		/////////////// ���� ������ �޽��� [���� ������ �۾�] ////////////////

/////////// ���� ������ ����Ʈ�� ��û
class CSV_ASK_GAMEITEMLIST : public CCommMsg
{
public:
	CSV_ASK_GAMEITEMLIST() : CCommMsg() {}
	CSV_ASK_GAMEITEMLIST(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BOOL Set();
	BOOL Get(char* lpdata, int size);
};


/////////// ���� ������ ����Ʈ�� �����ش�
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

/////////// ���� �������� ���(��û, ��� ���)
class CSV_USEGAMEITEM : public CCommMsg
{
public:
	CSV_USEGAMEITEM() : CCommMsg() {}
	CSV_USEGAMEITEM(char *pRefBuf) : CCommMsg(pRefBuf) {}

	USEGAMEITEMINFO *pUG;

	BOOL Set(USEGAMEITEMINFO *pug);
	BOOL Get(char* lpdata, int size);
};

/////////// ���� ������ ������ ���
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




		/////////////// ���� ����� �޼��� ////////////////

/////////// ������ ���۵Ǿ���
class CSV_STARTGAME : public CCommMsg
{
public:
	CSV_STARTGAME() : CCommMsg() {}
	CSV_STARTGAME(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;		// �� ��ȣ
	BOOL Set(int roomnum);
	BOOL Get(char* lpdata, int size);
};

/////////// �������� ����� �뺸
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
//                      Ŭ���̾�Ʈ�� �޼��� Ŭ����
////////////////////////////////////////////////////////////////////////////

/////////// ������ ������� �˸�
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

/////////// ���ӹ�� ä�� ����Ÿ
class CCL_CHATDATA : public CCommMsg
{
public:
	CCL_CHATDATA() : CCommMsg() {}
	CCL_CHATDATA(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	char *ID;			// ���̵� ��Ʈ��
	char *Kind;			// ä�� �������� ����
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��
	BOOL Set(int rnum, char *id, char kind, char *chat);
	BOOL Get(char* lpdata, int size);
};


////////////////////////////////////////////////////////////////////////////
//                      ���� ����� �޼��� Ŭ����
////////////////////////////////////////////////////////////////////////////

/////////// ������ ������ �غ� ������(������ ������Ҷ�)
class CCL_READYTOSTART : public CCommMsg
{
public:
	CCL_READYTOSTART() : CCommMsg() {}
	CCL_READYTOSTART(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *Id;
	BOOL Set(char *id);
	BOOL Get(char* lpdata, int size);
};

/////////// ���� ��� �ٲٱ� ��û
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

/////////// ���� ��� �ٲٱ� ���
class CCL_CHANGEGAMEKIND : public CCommMsg
{
public:
	CCL_CHANGEGAMEKIND() : CCommMsg() {}
	CCL_CHANGEGAMEKIND(char *pRefBuf) : CCommMsg(pRefBuf) {}

	ROOMINFO *pRI;
	BOOL Set(ROOMINFO *pri);
	BOOL Get(char* lpdata, int size);
};

/////////// ���� ������ ��û
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

/////////// ���� ���� ����
class CCL_STARTINFO : public CCommMsg
{
public:
	CCL_STARTINFO() : CCommMsg() {}
	CCL_STARTINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	STARTINFO *SI;
	BOOL Set(STARTINFO *pSI);
	BOOL Get(char* lpdata, int size);
};

// ������ ���� ��û
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

// ī�� ���� ��û
class CSV_CHANGECARD : public CCommMsg
{
public:
	CSV_CHANGECARD() : CCommMsg() {}
	CSV_CHANGECARD(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGECARD *pCC;
	BOOL Set(CHANGECARD *pcc);
	BOOL Get(char* lpdata, int size);
};

// [62]cut ī�� ���� ��û
class CSV_ASK_CHANGECARD_62CUT : public CCommMsg
{
public:
	CSV_ASK_CHANGECARD_62CUT() : CCommMsg() {}
	CSV_ASK_CHANGECARD_62CUT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGECARD_62CUT *pCC;
	BOOL Set(CHANGECARD_62CUT *pcc);
	BOOL Get(char* lpdata, int size);
};

// [62]cut ����Ǵ� ī�� ������.(���� Ȯ���̴� :���ÿ� �Ѹ���)
class CSV_ASK_CHANGECARD_62CUT_EX : public CCommMsg
{
public:
	CSV_ASK_CHANGECARD_62CUT_EX() : CCommMsg() {}
	CSV_ASK_CHANGECARD_62CUT_EX(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGECARD_62CUT_EX *pCC;
	BOOL Set(CHANGECARD_62CUT_EX *pcc);
	BOOL Get(char* lpdata, int size);
};

// 62cut ����Ǵ� ī�带 ����
class CSV_CHANGECARD_62CUT_RESULT : public CCommMsg
{
public:
	CSV_CHANGECARD_62CUT_RESULT() : CCommMsg() {}
	CSV_CHANGECARD_62CUT_RESULT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGECARD_62CUT *pCC;
	BOOL Set(CHANGECARD_62CUT *pcc);
	BOOL Get(char* lpdata, int size);
};

// 62cut client �� server �� �ٲ� ī�带 ���� 
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


//���۹�ư Ȱ��ȭ 
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

// ī�� ��� ��û
class CSV_BUYCARD : public CCommMsg
{
public:
	CSV_BUYCARD() : CCommMsg() {}
	CSV_BUYCARD(char *pRefBuf) : CCommMsg(pRefBuf) {}

	BUYCARD *pBC;
	BOOL Set(BUYCARD *pbc);
	BOOL Get(char* lpdata, int size);
};

// ����
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

// ī�� ����
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

// �� ���� ī�峯���� ����
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


// ����� ������ ���� �޼���
class CCL_ROOM_TIMEOUT : public CCommMsg
{
public:
	CCL_ROOM_TIMEOUT() : CCommMsg() {}
	CCL_ROOM_TIMEOUT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *UNum;
	char *ID;
	int  *l_StrMsg;		// �޼��� ��Ʈ���� ����
	char *StrMsg;		// �޼��� ��Ʈ��
	BOOL Set(int rnum, int unum, char* id, int l_strlen, char *msg);
	BOOL Get(char* lpdata, int size);
};


/////////// ���伣 ī��� �̰��� ��� ��� �����ڿ��� �˸�
class CSV_SPECIALCARD : public CCommMsg
{
public:
	CSV_SPECIALCARD() : CCommMsg() {}
	CSV_SPECIALCARD(char *pRefBuf) : CCommMsg(pRefBuf) {}


	SPECIALCARD *pSC;
	BOOL Set(SPECIALCARD *psc);
	BOOL Get(char* lpdata, int size);
};


/////////// ������ ����� ���� ����
class CCL_USERRESULT : public CCommMsg
{
public:
	CCL_USERRESULT() : CCommMsg() {}
	CCL_USERRESULT(char *pRefBuf) : CCommMsg(pRefBuf) {}


	USERRESULT *pRS;
	BOOL Set(USERRESULT *prs);
	BOOL Get(char* lpdata, int size);
};

/////////// ���� �˸� ������
class CSV_RAISE : public CCommMsg
{
public:
	CSV_RAISE() : CCommMsg() {}
	CSV_RAISE(char *pRefBuf) : CCommMsg(pRefBuf) {}


	POKERGAME *pPG;
	BOOL Set(POKERGAME *ppg);
	BOOL Get(char* lpdata, int size);
};

// ������ ���� ī�� ���� (���� ����)
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

//ù 3���� ������ ������ �ϳ��� ī�带 ���� => ��� ���ý� ���ӽ���
class CSV_WINDCARDEND : public CCommMsg
{
public:
	CSV_WINDCARDEND() : CCommMsg() {}
	CSV_WINDCARDEND(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Reserve;
	BOOL Set(int reserve);
	BOOL Get(char* lpdata, int size);
};

// �� ���� �˸�
class CSV_ENDGAME : public CCommMsg
{
public:
	CSV_ENDGAME() : CCommMsg() {}
	CSV_ENDGAME(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Reserve;
	BOOL Set(int reserve);
	BOOL Get(char* lpdata, int size);
};



/////////// ���� �˸� ������ ��
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


// ��ɷ� �ºθ� �ܷ�ڴ°�
class CCL_SELECTWINCASE : public CCommMsg
{
public:
	CCL_SELECTWINCASE() : CCommMsg() {}
	CCL_SELECTWINCASE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *ServPNum;	
	int *UNum;
	char *ID;
	int  *nCase; // 1: ���� 2: �ο� 3: ����
	BOOL Set(int rnum, int servpnum, int unum, char* id, int ncase);
	BOOL Get(char* lpdata, int size);
};

/////////////////////////////////////////////////////////////////////////////////////////
// [ ���� ������ ]

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


// ���� ����
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

/////////// �ٷ� ���� �� ������ ��û
class CSV_ASK_QUICKSTART : public CCommMsg
{
public:
	CSV_ASK_QUICKSTART() : CCommMsg() {}
	CSV_ASK_QUICKSTART(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *UNum;
	char *ID;
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	int *GameKind;  // 5�ο�, 7�ο�
	BOOL Set(int unum, char *id, int sndKind, int gamekind);
	BOOL Get(char* lpdata, int size);
};


/////////// �ٷ� ���� �Ұ�(�����)
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
// ����������Ʈ �۾�	
// ����
class CNM_AGENT_CONNECT : public CCommMsg
{
public:
	CNM_AGENT_CONNECT() : CCommMsg() {}
	CNM_AGENT_CONNECT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotalNum;
	INT64 *JackPot; // ���̱ݾ�
	BOOL Set(int tot, INT64 jackpot);
	BOOL Get(char* lpdata, int size);
};

// ���� ��� ��û
class CNM_AGENT_USERLIST : public CCommMsg
{
public:
	CNM_AGENT_USERLIST() : CCommMsg() {}
	CNM_AGENT_USERLIST(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Reserve;
	BOOL Set(int reserve);
	BOOL Get(char* lpdata, int size);
};


/////////// 1�� ��� 400 ��
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


/////////// ���ο� ���� ����
class CNM_AGENT_NEWUSER : public CCommMsg
{
public:
	CNM_AGENT_NEWUSER() : CCommMsg() {}
	CNM_AGENT_NEWUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	AGENT_USER *ArraySUI;
	INT64 *JackPot; // ���̱ݾ�

	BOOL Set(int totnum, AGENT_USER* ppSUI, INT64 jackpot);
	BOOL Get(char* lpdata, int size);
};

///////////  ���� �ƿ�
class CNM_AGENT_OUTUSER : public CCommMsg
{
public:
	CNM_AGENT_OUTUSER() : CCommMsg() {}
	CNM_AGENT_OUTUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *TotNum;
	AGENT_USER *ArraySUI;
	INT64 *JackPot; // ���̱ݾ�

	BOOL Set(int totnum, AGENT_USER* ppSUI, INT64 jackpot);
	BOOL Get(char* lpdata, int size);
};


/////////// �ߺ����� Ȯ��
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

/////////// �ߺ�������
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
// ���� ������ �۾�


// ���� ������ ��� ä�� ���� ��û 
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

// ���� ������ ä�� ���� ���
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

// ���� �������� ������ �α� �Ѵ�
class CCL_JUMPITEM_LOGIN : public CCommMsg
{
public :
	CCL_JUMPITEM_LOGIN() : CCommMsg() {}
	CCL_JUMPITEM_LOGIN( char* pRefBuf ) : CCommMsg(pRefBuf) {}

	int* Index;

	BOOL Set( int index );
	BOOL Get( char* lpdata, int size );
};

//  DB �� ��� ���� ������Ʈ �� ����� �˷��ش�.
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

// ���� ��� 
class CNM_AGENT_AVERAGE : public CCommMsg
{
public:
	CNM_AGENT_AVERAGE() : CCommMsg() {}
	CNM_AGENT_AVERAGE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	INT64 *JackPotMoney;
	BOOL Set(INT64 jackpot);
	BOOL Get(char* lpdata, int size);
};

// ### [���� �߰� �۾�] ### ��û
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


// ### [���� �߰� �۾�] ### ��û ���
class CSV_ACCEPT_CHANGESNDFX : public CCommMsg
{
public:
	CSV_ACCEPT_CHANGESNDFX() : CCommMsg() {}
	CSV_ACCEPT_CHANGESNDFX(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *SndFx;
	BOOL Set(int sndfx);
	BOOL Get(char* lpdata, int size);
};

// ### [�̺�Ʈ Ƽ��] ###
/////////// �̺�Ʈ ��÷�� �˸�
class CSV_EVENTPRIZE : public CCommMsg
{
public:
	CSV_EVENTPRIZE() : CCommMsg() {}
	CSV_EVENTPRIZE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	EVENTPRIZEINFO *pEI;
	BOOL Set(EVENTPRIZEINFO *pei);
	BOOL Get(char* lpdata, int size);
};

// ### [�̺�Ʈ Ƽ��] ###
/////////// �̺�Ʈ ����
class CSV_EVENTNOTICE : public CCommMsg
{
public:
	CSV_EVENTNOTICE() : CCommMsg() {}
	CSV_EVENTNOTICE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	short *Kind;		// �������� ����
	short *l_StrChat;	// ��Ʈ���� ����
	char  *StrChat;		// ��Ʈ��
	COLORREF *Color;	// �ؽ�Ʈ �÷�
	BOOL Set(short kind, char *chat, COLORREF color);
	BOOL Get(char* lpdata, int size);
};


///////////////////////////////////////////////////////////////////
/////////////////// ### [������ ��� �۾�] ###  ///////////////////

/////////// ������ �Ƶ� �����Ѵ�
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

// ���� �����ϱ�
class CNM_ADMIN_OUTUSER : public CCommMsg 
{
public:
	CNM_ADMIN_OUTUSER() : CCommMsg() {}
	CNM_ADMIN_OUTUSER(char *pRefBuf) : CCommMsg(pRefBuf) {}


	ADMINMESSAGE *pAMESG;
	BOOL Set(ADMINMESSAGE *pmsg);
	BOOL Get(char* lpdata, int size);
};

// �������ϱ�
class CNM_ADMIN_DELROOM : public CCommMsg 
{
public:
	CNM_ADMIN_DELROOM() : CCommMsg() {}
	CNM_ADMIN_DELROOM(char *pRefBuf) : CCommMsg(pRefBuf) {}


	ADMINMESSAGE *pAMESG;
	BOOL Set(ADMINMESSAGE *pmsg);
	BOOL Get(char* lpdata, int size);
};

/////////// �ӼӸ�
class CNM_ADMIN_WHISPERCHAT : public CCommMsg
{
public:
	CNM_ADMIN_WHISPERCHAT() : CCommMsg() {}
	CNM_ADMIN_WHISPERCHAT(char *pRefBuf) : CCommMsg(pRefBuf) {}

	char *ID;			// ������ ��� ���̵� ��Ʈ��
	int  *DestUNum;		// �޴� ��� ����� ��ȣ
	char *DestID;		// �޴� ��� ���̵�
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��
	BOOL Set(char *id, int destunum, char *destid, char *chat);
	BOOL Get(char* lpdata, int size);
};

// ���� �����ϱ�
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

/////////// �� ���� ���� ��û �˸�
class CSV_CHANGESTYLEROOMINFO : public CCommMsg
{
public:
	CSV_CHANGESTYLEROOMINFO() : CCommMsg() {}
	CSV_CHANGESTYLEROOMINFO(char *pRefBuf) : CCommMsg(pRefBuf) {}

	CHANGESTYLEROOMINFO *SRI;
	BOOL Set(CHANGESTYLEROOMINFO* pSRI);
	BOOL Get(char* lpdata, int size);
};

// ����� ����ֱ� 
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
/////////////////// ### [ ������� ] ###  ///////////////////

/////////// ������ ��û
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
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	BOOL Set(int rnum, int unum, int servpnum, char *id, int sndfx);
	BOOL Get(char* lpdata, int size);
};


//////////// ������ ����
class CSV_ACCEPT_PARTICIPATION : public CCommMsg
{
public:
	CSV_ACCEPT_PARTICIPATION() : CCommMsg() {}
	CSV_ACCEPT_PARTICIPATION(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	char *ID;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	BOOL Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id,  IPARRINDEX *pIPA);// �߰�
	BOOL Get(char* lpdata, int size);
};

//////////// ������ ����
class CSV_ENTER_PARTICIPATION : public CCommMsg
{
public:
	CSV_ENTER_PARTICIPATION() : CCommMsg() {}
	CSV_ENTER_PARTICIPATION(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	int *SndFxKind; // ### [���� �߰� �۾�] ###
	char *ID;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	BOOL Set(int rnum, int nowpnum, int newpnum, int sndKind, char *id, IPARRINDEX *pIPA);// �߰�
	BOOL Get(char* lpdata, int size);
};

/////////// ������ ��û
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


//////////// ���� ����
class CSV_ACCEPT_EMERAGE : public CCommMsg
{
public:
	CSV_ACCEPT_EMERAGE() : CCommMsg() {}
	CSV_ACCEPT_EMERAGE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	BOOL Set(int rnum, int nowpnum, int newpnum, char *id, IPARRINDEX *pIPA);// �߰�
	BOOL Get(char* lpdata, int size);
};

//////////// ���� �������� �ٸ��������� �˸���
class CSV_ENTER_EMERAGE : public CCommMsg
{
public:
	CSV_ENTER_EMERAGE() : CCommMsg() {}
	CSV_ENTER_EMERAGE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *RoomNum;
	int *NowPNum;
	int *NewPNum;
	char *ID;
	IPARRINDEX *IPA;// ### [ �ߺ��� IP�� ] ###
	BOOL Set(int rnum, int nowpnum, int newpnum, char *id, IPARRINDEX *pIPA);// �߰�
	BOOL Get(char* lpdata, int size);
};


/////////// ������ ���� ����
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

/////////// �������� �˸� �޼���
class CSV_OBSERVER_MESSAGE : public CCommMsg
{
public:
	CSV_OBSERVER_MESSAGE() : CCommMsg() {}
	CSV_OBSERVER_MESSAGE(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Kind;			// ����(0:�⺻ ��� 1:�޼��� �ڽ� ���)
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��

	BOOL Set(int kind, char *chat);
	BOOL Get(char* lpdata, int size);
};
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

/////////// ���� ���� �޼���
class CSV_SERVERNOTIC : public CCommMsg
{
public:
	CSV_SERVERNOTIC() : CCommMsg() {}
	CSV_SERVERNOTIC(char *pRefBuf) : CCommMsg(pRefBuf) {}

	int *Kind;				// ����(0:�⺻ ��� 1:�޼��� �ڽ� ���)
	short *l_StrChat;		// ä�� ��Ʈ���� ����
	char *StrChat;			// ä�� ��Ʈ��
	BOOL *bColorAssigned;	// �ؽ�Ʈ �÷��� �����Ǿ��°�?
	COLORREF *Color;		// �ؽ�Ʈ �÷�

	BOOL Set(int kind, char *chat, BOOL bcolorassigned = FALSE, COLORREF color = RGB(255,0,0));
	BOOL Get(char* lpdata, int size);
};

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


// ī�� ���� Ÿ�̸� �˸�
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

// [��ȣõ��] 2004.07.08 ����->������� Ȯ��
class CSV_ASK_SAFEANGEL : public CCommMsg
{
public:
	CSV_ASK_SAFEANGEL() : CCommMsg() {}
	CSV_ASK_SAFEANGEL(char *pRefBuf) : CCommMsg(pRefBuf) {}
	BOOL Set();
	BOOL Get(char* lpdata, int size);
};


// [��ȣõ��] 2004.07.08 �Ӵ� ���� �޽���
class CCL_CHARGE_SAFEANGEL : public CCommMsg
{
public:
	enum CHARGEFLAG{
		CHARGE_ANGEL=1,			// ��ȣ õ��� ä������ �ݾ�
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
