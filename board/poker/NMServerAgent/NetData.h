// NetData.h: interface for the CNetData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETDATA_H__866B5F54_DF14_11D3_91FF_0050BF0FBDAF__INCLUDED_)
#define AFX_NETDATA_H__866B5F54_DF14_11D3_91FF_0050BF0FBDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ����Ÿ ����ü�� ����. ��Ŭ�������� ����Ҳ���...
enum DATATYPE{ 
	NM_DATA=0,    // �Ϲ����� ����Ÿ�̴�.
	NM_SIGN,    // ��ȣ ����Ÿ�̴�.
	NM_USER,    // ����� �����̴�.
	NM_STRING,  // ���ڿ� ����Ÿ�̴�.

	NM_QMUSER, // ����� ����� �����̴�.
	NM_SLDATA,
	NM_CASDATA,
	NM_QMSIGN, // ������ ��ȣ���̴�.
	NM_QMDATA, // ���Ըӽſ� ����Ÿ�̴�.(**����)

	QM_DATA    // ����� ���ӹ� ����Ÿ.
};
 // ����Ÿ �뷮...
enum DATASIZE{ 
	NM_SMALL=0,   // 1024 byte ����. ����� ������...ä�� ���ڿ�...(Ŭ���̾�Ʈ->����).
	NM_MEDIUM,  // 1024 byte(1k byte)
	NM_LARGE,   // 2048
	NM_BIG      // 4096 (����->Ŭ���̾�Ʈ). 
};

// ���� ���� �� ���� ��� ��ƾ
enum SIGNAL{

	NM_NOSIGN=0,
	NM_NOSIGN0,        // ���̰��� �ֱ�����...(**����)
	NM_NOSIGN1,        // ���̰��� �ֱ�����...(**����)
	NM_NOSIGN2,        // ���̰��� �ֱ�����...(**����)
	NM_NOSIGN3,        // ���̰��� �ֱ�����...(**����)
	NM_NOSIGN4,        // ���̰��� �ֱ�����...(**����)

	JK_TURN, // ����� ���̴�

	//// ī���� ���ӹ� ��ƾ.
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
	CAS_GAMEMES, //(������ī-���ΰ��Ӹ޽���)
	CAS_GAMEMESROOM, //(������ī-���ΰ��Ӹ޽���)
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
	// ���Ըӽ� �߰�����
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
	// ������ī ���� �޽���.
	CAS_SELECTCARD,
	CAS_WINDCARDEND,
	CAS_ACITIVEBTNSTART,
	CAS_ACITIVEBTN,
	CAS_OUTROOMNOMONEY,
	CAS_GAMEINFO,
	CAS_SPECIAL,
	NM_USERRESULT,
	CAS_HIDDENCARD,

	/**************** Ŭ���̾�Ʈ <-> ����.*********************************/
	// 1. �⺻ ó��.
	NM_CONNECT,       // ���ӵǾ���.
	NM_SEND_DATA,     // ����Ÿ�� ����Դ�.
	NM_SEND_TO_DATA,  // Ư�� �ص�����Ʈ�� ����Ÿ�� ������.

	// 2. �̴� ��ȭ�濡��...
	NM_CHANGECHATR,   // ��ȭ����� �ٲ۴�.         ��ȭ����� �ٲ����.
	NM_SILENTWORD,    // �ӼӸ��� ������ �;��Ѵ�.  �ӼӸ��� ����Դ�.
	NM_DELCHATUSER,   // ������ ���� ��û�Ͽ���.  ������Ͽ���.

	
	/************** Ŭ���̾�Ʈ -> ����.(�� ������ �ؼ��Ѵ�.)****************/
	// 1. ���� ���� ó��.
	NM_LOGIN,         // �α��� ��û�̴�.
	NM_LOGOUT,        // ������ ���´ܴ�.
	NM_CHANGE_USER,   // ����� ������ �����Ѵٰ� ����.

	// 2. �̴� ��ȭ�� ��ƾ. �������� ����ó������.
	NM_REFRESHCHAT,   // ��ȭ���� ���ΰ�ħ ��û.(������� ���)
	NM_CHATROOM,      // ��ȭ������ ����ʹ�.
	NM_MAKECHATROOM,  // ��ȭ������� ��û�ߴ�.
	NM_INCHATROOM,    // ��ȭ�濡 ���� �;��Ѵ�.
	NM_OUTCHATROOM,   // ��ȭ�濡�� ���Դ�.
	NM_CLOSECHATROOM, // ��ȭ���� ������״�.

	// 3. ���ӹ� ��ƾ.
	NM_REFRESHGAME,   // ���ӹ� ���� ��ħ.
	NM_GAMEROOM,      // ���ӹ����� ����;��Ѵ�.
	NM_MAKEGAMEROOM,  // ���ӹ游��⸦ ��û�Ѵ�.
	NM_NOMAKEGAMEROOM,// ���ӹ��� ����� ����.
	NM_INGAMEROOM,    // ���ӹ濡 ���� �;��Ѵ�.
	NM_OUTGAMEROOM,   // �� �濡�� ���Ծ��...
	NM_CLOSEGAMEROOM, // ���� �ı���Ű�ڴ�.

	// 4. �Խ��� �� �޽���ó��.
	NM_MESSAGETO,     // �������� �޽����� �����ڴܴ�.

		
	/*************** ���� -> Ŭ���̾�Ʈ. (�� Ŭ���̾�Ʈ�� �ؼ��Ѵ�.)**************/
	// 1. ���� ���� ó��.
	NM_ERRMES,
	NM_ACCEPT,        // Ŭ���̾�Ʈ�� �α����� �㰡�޾Ҵ�.
	NM_RELOGIN,       // �ٽ� �α����϶�� �޽����� �޾Ҵ�.
	NM_NEWUSER,       // ���ο� �����ڴ�.
	NM_OUTUSER,       // �Ѹ��̳�����.
	NM_DENY,          // �������� �ź� ���ߴ�.
	NM_CONDENY,       // ���̻� ���ӱ���.
	NM_SRVFULL,       // �����ʰ�
	NM_TIMER,         // ������ Ÿ�̸Ӹ޽���.
	NM_NEWDENY,       // �̹� �����ϴ� ���̵��̴�.
	NM_DOWN,          // ����? ������ �ٿ� �Ǿ��ٳ�..��..�׷�����....
	NM_WHISDNY,       // �Ӹ�����.
	NM_WHISOK,
	NM_MESDNY,        // �޽��� ����
	NM_MESOK,
	NM_INVDNY,
	NM_INVERR,
	NM_INVITE,
	NM_INVDENYMES,
	NM_INVINROOM,
	NM_EXITFLAG,      // �ϰ�����.
	NM_SOMEONECON,
	NM_DENYVER,
	NM_DENYUSER, // �����ƾ.
	NM_USERLIST, // �߰� ������ ��ƾ.
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
	NM_NEWUSERIDOK,    // ���̵� �ߺ�Ȯ��.
	NM_NEWUSERIDNO,    // ���̵� �ߺ��ȴ�.
	NM_NEWUSERERROR, // �ű����� ������.
	NM_NEWUSEROK,    // �ű����� �����̴�.

	// �߰����
	NM_THISCONDENY,   // ���� �� ������ ������ ������ �ִ�
	NM_CHECKVER,      // ������ Ȯ���Ѵ�
	NM_CHANNELLIST,   // ä�� ����Ʈ �䱸
	NM_CHANNELIN,     // ä�� ���� ��û
	NM_CHANNELOUT,    // ä�� ���� ��û
	NM_CHANNELFIND,   // ä�ο��� ����� ã��
	NM_ROOMLIST,      // �渮��Ʈ ��û
	NM_HEARTBEAT,     // �������� �÷���
	NM_DISCONMES,     // ���� ���� �޽���
	NM_CHATSNDTO,     // �Ӹ� ������
	NM_CHATSNDTO2,    // �κ� ���� ���� ����� �Ӹ�
	NM_CHATSNDTOERROR,
	NM_SMALLUSERINFO, // ���Ӱ�� ����
	NM_CHANGEROOM,



	// 2. 1)�̴� ��ȭ�� �ۿ���....
	NM_LISTCHATROOM,  // ��ȭ�� ����� ����Դ�.
	NM_LISTCHATNEW,   // ���ο��ȭ���� �����Ǿ��ܴ�.
	NM_LISTCHATOUT,   // ��ȭ���� �ϳ� ������.
	NM_CONCHATOK,     // ��ȭ�� ������ ���ι޾Ҵ�.
	//    2)�̴� ��ȭ�� �ȿ���...
	NM_CONCHATUSERS,  // ��ȭ�濡 ���ӵǾ��ִ� �����ڸ� �����Դ�.
	NM_NEWCHATUSER,   // ���ο� ����ڰ� ���Դ�.
	NM_OUTCHATUSER,   // �Ѹ��� ������.
	NM_INHERCROOM,    // ��ȭ���� �°�޾Ҵ�.

	// 3. ���ӹ� ��ƾ �ۿ���.
	NM_LISTGAMEROOM,  // ���ӹ� ����� ����Դ�.
	NM_LISTGAMENEW,   // ���ο� ���ӹ��� �����Ǿ��ܴ�.
	NM_LISTGAMEOUT,   // ���ӹ��� �ϳ� ������.
	NM_CONGAMEOK,     // ���ӹ� ������ ���ι޾Ҵ�.

	// 4. �Խ��� �� �޽���ó��.
	NM_MESSAGE,       // ������ ���� �޽����� ����Դ�.
	NM_MESSAGEFROM,   // �������� ���� �޽����� ����Դ�.

	
	NM_ADV,           // ���� �����Դ�...�̷� ...

	///// ������ �޽���.
	NM_QMCHATDATA,   // ä�õ���Ÿ�̴�.
	NM_CANCELMAKEROOM, // ���ӹ� ����⸦ �������.
	NM_MAKEROOMOK,    // ���� ����� ���Ҵ�.
	NM_QMOUTSERVER,   // ���Ӽ����� ���� ������..
	NM_QMOUTONSERVER,
	NM_QMDELROOM,     // �渮��Ʈ�� �ϳ� ���ŵǾ���.
	NM_QMILREGALROOM, // �ùٸ� ���ӹ��̾ƴϴ�.
	NM_QMCONNECTROOMERROR, // ���ӹ�� ������ �ȵȴ�.
	NM_QMFULLROOM,   // ���ӹ��ο��� ��ã��.
	NM_QMROOMPASSNO, // �׹� ��й�ȣ�� Ʋ����.
	NM_QMROOMSRVIP,  // �׹� �����ǰ� ����Դ�.
	NM_QMROOMINOK,   // �׹濡 �Ѹ� ���ӵǾ���.
	NM_QMROOMFULLNOW,// �׹��ο��� ��� Ǯ�̴�.
	NM_QMROOMINOKCLNT, // �׹濡 ���ӵǾ��ٰ� �����ڰ� �˷��Դ�.
	NM_QMPLAYGAME,   // ������ ���۵Ǿ���.
	NM_OUTGAMEONROOM, // �������� �׹濡�� ������.
	NM_QMGAMEOVER,   // ������ ������.
	NM_QMCHANGESRV,  // ���Ӽ����� �ٲ��.
	NM_QMYOUSRV,     // ���Ӽ����� �Ǿ���.
	NM_SETPOINT,     // ����� �������� �����̴�.
	NM_SETPOINT2,
	NM_QMCHANGEINFO, // ����� ��������.
	NM_QMGAMEDATA,
	NM_QMONINROOMERROR,
	NM_ROOMCHATDATA,
	NM_ROOMINFO,
	NM_MYINFO,
	NM_CHANGECARD,





	//// ����� ���ӹ� ��ƾ.
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


	// �Ű���� �޽���.
	NM_CAPFILE,      // ���� ��������.
	NM_CAPFILEALL,   // ���� ��������.
	NM_STOPUSER,     // ��������� �����.
	NM_DELUSER,      // ������ �����.
	NM_CAPRESULTMES, // �Ű����� �޽��� ����.


	// ������ �޽���
	NM_JKPLAY, // �뱹��û
	NM_JKPLAYOK,//�뱹 �¶�.
	NM_JKDENY, // �뱹����
	NM_JKREADY,    // ������ �޽���
	NM_JKPLAYNOTUSER, // �׹濡 ���� ������̴�.
	NM_JKALEADYON, // �̹� ������ ���۵Ǿ���.
	NM_JKGAMEDATA, // ��� ���ӵ���Ÿ�̴�.
	NM_JKPLAYGAME, // ������ ������ �����Ѵ�.
	NM_JKPASS, // �Ѽ� ����Ͽ���.
	NM_JKOHNO, // �������û
	NM_JKOHNONO,// ������ ����
	NM_JKOHNOOK,//������ ���.
	NM_JKSTARTERROR, // �����ۿ� �����ִ�.
	NM_JKRESET, // �������� ���½�Ų��.
	NM_JKFOLD,  // ��⸦ �����Ѵ�.
	NM_JKSAME,  // ����û�� �Ѵ�.
	NM_JKSAMEOK,
	NM_JKSAMENO,
	NM_JKENDGAME, // ������ ������.
	NM_JKDATA, // ��� ����Ÿ�̴�.
	NM_JKNEWLEVEL,

	NM_JKWITHGAME,   // �뱹�� ��û�Ѵ�
	NM_JKWITHGAMEERR,// �뱹��û����(�̹� �濡 �ִ�,�̹� ������ �����)
	NM_JKWITHGAMENO, // �뱹��û ����
	NM_JKWITHGAMEOK, // �뱹��û ����
	NM_JKROOMERROR,  // ���ӹ濡��
	NM_JKBIGJANG,    // ����ó��
	NM_ACTROOMSRV,
	NM_ACTROOMCON,
	NM_LISTGAMENEWACT,

	NM_AGENT_CONNECT, // [���� ������Ʈ �۾�] : ������Ʈ�� ���ӵǾ���
	NM_AGENT_NEWUSER,
	NM_AGENT_OUTUSER,
	NM_AGENT_NEWUSERERROR,
	NM_AGENT_USERLIST,

	NM_AGENT_USERLIST1,
	NM_AGENT_USERLIST2,
	NM_AGENT_USERLIST3,
	NM_AGENT_USERLIST4,
	NM_AGENT_USERLIST5,

	NM_ITEM_LOGIN,   // ������ ���� �α��� �㰡
	NM_ITEM_LOGOUT,  // ������ ���� �α׾ƿ� �㰡
	NM_ITEM_LIST,    // ������ ����Ʈ
	NM_ITEM_NOITEM,  // ������ �������� ����
	NM_ITEM_READERR, // ������ �б� ����
	NM_ITEM_NOUSE,   // ������ ������� ����
	NM_ITEM_EXPIRED, // ������ ��ȿ�Ⱓ ����
	NM_ITEM_RETRY,   // ������ ��� ����, �ٽ� �õ��ϼ���
	NM_ITEM_ERROR1,  // ������ ����
	NM_ITEM_ERROR2,  // ������ ����
	NM_ITEM_ERROR3,  // ������ ����
	NM_ITEM_ERROR4,  // ������ ����

	NM_ITME_RLIST,
	NM_ITME_RENTER,
	NM_ITME_RUSE,

	// [������ ��� �۾�]

	NM_ADMIN_CHANGEROOM, // ����� �ɼǺ���
	NM_ADMIN_DELROOM,    // ������
	NM_ADMIN_WARNUSER,   // �������� ����ϱ�
	NM_ADMIN_OUTUSER,    // ����� �����Ű��



	DUMY



};

// ��Ʈ��ũ ����Ÿ ���.(32 byte)
typedef struct tagNetHeader{ 
	char      Header[8];   // ( 8 byte)����ü ���. �ν� ����.
	UINT      Size;        // ( 4 byte)�������� ����Ÿ ������.
	SIGNAL    Signal;      // ( 4 byte)��ɾ��...
	DATATYPE  Type;        // ( 4 byte)�� ����ü�� ���´� ...
	BOOL      Compress;    // ( 4 byte)���࿩��.
	UINT      Code;        // ( 4 byte)��ȣ�ڵ�.
	int       UserVal;     // ( 4 byte)����� ��
}NETHEADER,*LPNETHEADER;

// ���ӹ� ���ӷ�ƾ.
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

#define  NETMARBLEHEADER   "NMJANGGI" // (**����)
#define  NETCODENUM        (12)

// �ݵ���Ÿ.
class CNetData : public CObject 
{
protected:
	DECLARE_SERIAL(CNetData)
public:
	int  GetUserVal(); // ����� ���� ���� ���Ѵ�
	BOOL CheckHeader(char *lpdata, int *psignal, int *pmsglen);

	int  InitClass(); // ���� �� �޸� �ʱ�ȭ.
	CNetData();
	virtual ~CNetData();
	CNetData& operator=( CNetData &assign );

	DATATYPE  GetType(); // ����Ÿ ���¸� ���Ѵ�.
	SIGNAL    GetSign(); // ����Ÿ ��ȣ�� ���Ѵ�.
	char*     GetData(); // ���� ��� ����Ÿ�� ���Ѵ�.
	UINT      GetSize(); // ���� ��� ����Ÿ ����� ���Ѵ�.

	BOOL      SetData(LPVOID lpdata, UINT nsize, DATATYPE type, SIGNAL signal, int nuserval=0); // ����Ÿ ����.
	BOOL      IsData(LPVOID lpData, int size); // ����Ÿ���� Ȯ���ϰ� ������ �����Ѵ�.

	NETHEADER m_NetHeader; // ����Ÿ ���.
	LPVOID    m_lpData;    // �ٽ����� ���� ����Ÿ.
	UINT      m_Size;      // ���� ����Ÿ ������.

	LPVOID    m_NetData;   // ���������� ��ſ��� ���� ����Ÿ.(���+����Ÿ)
	
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
