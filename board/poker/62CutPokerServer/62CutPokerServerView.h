// 62CutPokerServerView.h : interface of the C62CutPokerServerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TETRISSERVERVIEW_H__8D85588A_B8D2_4EBF_ACC2_BAA9322B72F3__INCLUDED_)
#define AFX_TETRISSERVERVIEW_H__8D85588A_B8D2_4EBF_ACC2_BAA9322B72F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "62CutPokerServer.h"
#include <afxmt.h>			// ũ��Ƽ�� ���� Ŭ������
#include "SockMan.h"		// ���� �޴���
#include "CommMsg.h"		// ��� �޼��� ó�� Ŭ����
#include "Channel.h"		// ä�� Ŭ����
#include "User.h"			// ����� Ŭ����
#include "Room.h"			// ���ӹ� Ŭ����
#include "MsgDlg.h"			// �޽��� ���̾�α�
#include "LogFile.h"		// �α����� ��� Ŭ����
#include "DBMan.h"			// DB �޴��� ���
#include <direct.h>         // ���͸� ����

#include "LogManDBMan.h"    // ### [�α� ��Ͽ�] ###
#include "NMSTRMAN.h"       // ���ڿ� Ŭ����
#include "ServerCommy.h"	// ���� ���� �Լ� ���	[PC�� �۾�]
#include "Global.h"			// ���� ���
#include "62CutPokerServerDoc.h"
#include "StatisticsLog.h"  // [ ����� ] <---- �߰�

// ���� ����
#define VERSION 102


#define MAX_CHAN 17//10		// �ִ� ä�� ����
#define MAX_USER 2501//3001//2001	// �ִ� �����ڼ� (MAX_CHUSER*MAX_CHAN+1)
						// ������ FirstInclude.h�� MAX_DATA_SOCKET�� �Բ� ������ ��
						// Channel.h�� MAX_CHUSER�� �Բ� ������ ��
						// DBMan.h�� MAX_DBTHREAD�� �Բ� ������ ��
						// DBThread.h�� MAX_DBWORK�� �Բ� ������ ��
						// �Ʒ��� MAX_LOGIN_DBWORKNUM �Բ� ������ ��

#define MAX_TIMEOUT 1200				// ���� ���ú� Time Out 20��(1�ʴ���)
#define MAX_DBTHREAD_TIMEOUT 1800000	// DB ������ ���� ���� Ÿ�Ӿƿ�(30��) = 60000 * 30
//#define MAX_LOGIN_DBWORKNUM 18			// �α��� ����� ������ DB ť �۾����� 

#define WIN_POINT			20		// �¸� ����Ʈ
#define WIN_MARBLE			100		// �¸� ����
#define LOOSE_POINT			10		// �й� ����Ʈ
#define LOOSE_MARBLE		50		// �й� ����

#define GLOBALGAME_TIMER	1000
#define MSGPOOLING_TIMER	1001
#define CLOSE_TIMER			1002
#define USERCNT_TIMER		1003   // [PC�� �۾�]

////////////////////////////////////////////////////////////////
#define EXITRESERVE_TIMER	1005		// �ڵ�����

class C62CutPokerServerView;
extern C62CutPokerServerView *g_pMainView;	// ���κ� ����������
extern CDBMan DBMan;					// DB �޴��� ���� ��ü
extern CSockMan SockMan;				// ���� �޴��� ���� ��ü
extern CChannel Chan[MAX_CHAN];			// ä�� Ŭ����
extern CUser User[MAX_USER];			// ����� ���� Ŭ����
extern CNMStrMan g_StrMan;
extern POKERSET g_POKERSET;			//[������Ŀ] �׼����� ����

extern CLogManDBMan LogDBMan;		// ### [�α� ��Ͽ�] ###
extern INT64 g_LogRangeValue;

extern INT64 g_JackPotMoney;		// [ ###����### ]
extern int g_ServerGrade;			// ���

extern CString      g_AppStartPath;	// ���� ���͸� ����

extern ADMIN_MANAGER             g_AdminInfo; // [������ ��� �۾�]
extern ADMIN_ACCESSDENY_USER	 g_AccessDeny[MAX_DENY_USER];

// [���� ������Ʈ �۾�]
#define CONNECT_ID   "CTRA_CONNECT"
#define CONNECT_PASS "9669"
extern int  g_nAgentIndex;

extern CAPTURE_INFO       g_CaptureID; // [��ȭ ����]
extern SERACH_INFO         g_SearchID; // [��ġ ����]


typedef struct
{
	char ServerName[20];
	int  GameCode;
	int  ServerCode;
	UINT ServerPort;
	char UserDSN[40];
	char UserTable[40];
	char UserUID[20];
	char UserPass[20];
	char GameDSN[40];
	char GameTable[40];
	char GameUID[20];
	char GamePass[20];
	char BadUserDSN[40];
	char BadUserTable[40];
	char BadUserUID[20];
	char BadUserPass[20];
	char StatusDSN[40];
	char StatusTable[40];
	char StatusUID[20];
	char StatusPass[20];
	char AvatarDSN[40];
	char AvatarTable[40];
	char AvatarUID[20];
	char AvatarPass[20];
	// ��� �ڵ�
	int  nMSCode;
	INT64 MAX_PMONEY;

	char ItemDSN[40];		// [���� ������ �۾�]
	char ItemTable[40];		// [���� ������ �۾�]
	char ItemUID[20];		// [���� ������ �۾�]
	char ItemPass[20];		// [���� ������ �۾�]
	char PremiumDSN[40];	// [�����̾� �۾�]
	char PremiumTable[40];	// [�����̾� �۾�]
	char PremiumUID[20];	// [�����̾� �۾�]
	char PremiumPass[20];	// [�����̾� �۾�]
	char PremIPDSN[40];		// [PC�� �۾�]
	char PremIPTable[40];	// [PC�� �۾�]
	char PremIPUID[20];		// [PC�� �۾�]
	char PremIPPass[20];	// [PC�� �۾�]

	//////////////////       
	char LogRecDSN[40];     //### [�α� ��Ͽ�] ###
	char LogRecTable[40];
	char LogRecUID[20];
   	char LogRecPass[20];
	BOOL bLogUseDB;

	//////////////////
	char EventDSN[40];     // ### [�̺�Ʈ Ƽ��] ###
	char EventTable[40];
	char EventUID[20];
   	char EventPass[20];
	BOOL bEventDB;

	char StatisticsDSN[40];     // [ ����� ]
	char StatisticsTable[40];
	char StatisticsUID[20];
 	char StatisticsPass[20];
	BOOL bStatisticsDB;         // ����Ͽ� ���̺� �������(üũ��ư)

		//////////////////////////////////////////
	char PreUserLGMLogDSN[40];     //2004.05.07 �������Ѿ� ����
	char PreUserLGMLogTable[40];
	char PreUserLGMLogUID[20];
   	char PreUserLGMLogPass[20];
	BOOL bPreUserLGMLogDB;

	char AllinChargeDSN[40];     // [��ȣõ��]
	char AllinChargeTable[40];   // [��ȣõ��]		
	char AllinChargeUID[20];	 // [��ȣõ��]
 	char AllinChargePass[20];	 // [��ȣõ��]
	BOOL bAllinDB;

	char Resreved[120*2];	// ���̺��� �þ �Ϳ� ����� ���� ����Ʈ [�����̾� �۾�]	[PC�� �۾�]

} CONFIG;

typedef struct
{
	char NoticeInfoMsg[79];
	char NoticeURL[64];
	int  MaxUser;
	int  MaxDBThread;
	int  LoginDBQueNum;
	BOOL bDBClose;
	char BannerURL1[64];
	char BannerURL2[64];
	char ProfileURL[64];
	char LocationURL[64];	// <- �߰�

	char ItemURL[64];		// [���� ������ �۾�]
	char ItemHelpURL[64];	// [���� ������ �۾�]

/////////////////////////   //  //### [�α� ��Ͽ�] ###
	int  MaxDBThread2;
	int  LoginDBQueNum2;
	BOOL bDBClose2;
////////////////////////
	int	 SameIPLimit;// ### [ �ߺ��� IP�� ] ###
	BOOL UseMacLimit;	//�ƾ�巹�� �˻�
	char TitleNotice[1024];
	char LeadersJoinURL[64];//2004.05.07 �������Ѿ� ����
	char CashURL[64];		// ĳ������ URL 
	char Resreved[64*4];	// URL������ �þ �Ϳ� ����� ���� ����Ʈ [�����̾� �۾�]

} CONFIG2;

typedef struct
{
	int EnterLimitMode;
	BOOL bLoginLimit;
	BOOL bChanLimit;
	int LoginLimitLevel1;
	int LoginLimitLevel2;

	int LoginGrade;
	BOOL bExamServer;

	INT64 nCM; // �ʺ� ���ѸӴ�
	INT64 nHM; // �߼� ���ѸӴ�
	INT64 nFM; // ��� ���ѸӴ�
	INT64 nGM; // ���� ���ѸӴ�

	int   nHL; // �߼� ���ѷ���
	int   nFL; // ��� ���ѷ���
	int   nGL; // ���� ���ѷ���

	CHANINFO_CFG Info[MAX_CHAN];
	char Resreved[64*5];	// URL������ �þ �Ϳ� ����� ���� ����Ʈ [�����̾� �۾�]

} CONFIG3;

// ### [ ���� ] ###
typedef struct
{
int JackPotMode;	  // ���� ����
	INT64 nJackPotMoney;  // ���� �Ӵ�
	INT64 ChoBo;		  // �⺻ ���� �Ӵ� �ʺ�
	INT64 JungSu;		  //	
	INT64 GoSu;			  //
	INT64 God;		      //
	INT64 Free;           //
	// ���� ���� ����
	int   rChoBo;
	int   rJungSu;
	int   rGosu;
	int   rGod;
	int   rFree;
	// ���� ��� ���� ��Ƽ��
	int   jackPotChobo;
	int   jackPotJungSu;
	int   jackPotGoSu;
	int   jackPotDosin;
	int   jackPotFree;

	// ���� ��� ���� ��Ƽ��
	int   jackPotChoboS;
	int   jackPotJungSuS;
	int   jackPotGoSuS;
	int   jackPotDosinS;
	int   jackPotFreeS;

	// ���� ��� ���� ��ī��
	int   jackPotChoboF;
	int   jackPotJungSuF;
	int   jackPotGoSuF;
	int   jackPotDosinF;
	int   jackPotFreeF;

	// ### [�α� ��Ͽ�] ###
	BOOL bRealLogUse;
	int  Range;
	char Resreved[64*5];	// URL������ �þ �Ϳ� ����� ���� ����Ʈ [�����̾� �۾�]

} CONFIG4;

// ### [�̺�Ʈ Ƽ��] ###
typedef struct
{
	BOOL  bRunEvent;
	CTime StartDate;
	CTime EndDate;
	int   EventTicketNum;
	BOOL  bNoEventTime;
	int   NoEventStartHour;
	int   NoEventHour;
	int   MinUserNum;
	char  EventNotice[256];
	char  RoomEventNotice[256];
	BOOL  bAutoEventEnd;
	char Resreved[64*5];	// URL������ �þ �Ϳ� ����� ���� ����Ʈ [�����̾� �۾�]
} CONFIGEVENT;

// �����
typedef struct
{
BOOL	bSaveSetLog;	// [ ����� ]
	
	//2004.05.07 �������Ѿ� ����
	BOOL    bTotalMoneyLimit;	// �Ӵ� ���� �������
	INT64   NormalMoney;		// �Ϲ� �����
	INT64   LeadersMoney;		// ������ 
	INT64   LeadersPlusMoney;	// ������ �÷���

	BOOL    bSafeAngel;	// [��ȣõ��] 2004.07.09
	INT64   MinMoneyLimit;
	INT64   MaxMoneyLimit;

	char    NormalNotice[256];		// ���� ����
	char    LeadersNotice[256];
	char    LeadersPlusNotice[256];

	char Resreved[64*5];	// ������ �þ �Ϳ� ����� ���� ����Ʈ
} CONFIGSTATIS;

/////////////////////////////////////////////////////

class C62CutPokerServerView : public CFormView
{
public:
	//{{AFX_DATA(C62CutPokerServerView)
	enum { IDD = IDD_62CUTPOKERSERVER_FORM };
	CButton	m_ExitReserve;
	CButton	m_ExitCancel;
	CString	m_ServerMsg;
	CString	m_ServerLoginMsg;
	CString	m_DiscTargetID;
	CString	m_DelRoomNo;
	int		m_iExitRadio;
	CString	m_strMSCode;
	//}}AFX_DATA

// Attributes
public:
	CMsgDlg MsgDlg;			// �޽��� ��¿� ��ȭ����
	CONFIG Cfg;				// ȯ�漳�� ���� 1 (����� ���� ������ �簡���ؾ� �����)
	CONFIG2 Cfg2;			// ȯ�漳�� ���� 2 (����� �ǽð����� �����)
	CONFIG3 Cfg3;			// ȯ�漳�� ���� 3 (����� ���� ������ �簡���ؾ� �����)
	CONFIG4 Cfg4;			// ȯ�漳�� ���� ���� �Ӵ�
	CONFIGEVENT EventCfg;   // ### [�̺�Ʈ Ƽ��] ###
	CONFIGSTATIS StatisCfg; // �����


	/////////// ���� �̺�Ʈ�� ���� //////////
	// ### [�̺�Ʈ Ƽ��] ###
	int			TotEventTicketNum;	// ���ݱ��� ���޵� �̺�Ʈ Ƽ���� ����
	int			NowEventTicketNum;	// ���� �̺�Ʈ Ƽ���� �� �� �ִ� ����
	DWORD		LastEventTick;		// ���������� Ƽ���� ���޵� Time Tick

	BOOL bNowStartUpDlg;	// ���� ��ŸƮ�� ��ȭ���ڰ� �������ΰ�?
	BOOL bIsForceClose;		// ���� ���� ���� �����ΰ�?
	BOOL bConnectDeny;		// ���� �ź� ����
	int SendMsgMode;		// ���� �޼��� ������ ���
	int ClientMode;			// Ŭ���̾�Ʈ ���� ���� ���� ���
	int IPMode;				// Ŭ���̾�Ʈ IP ���� �� ���� ���

	int TotUser;			// �� �������� ����� ��(����� ���� ��)
	int RealUser;			// �� �������� ���� ����� ��(������ ����� ��)

	
	// [ ����� ]
//	STATISTICS_SINFO m_StatisticsLog; 
	CStatisticsLog   m_cSALog;		//<-- �߰�

	////////////////////////////////////////////////////////////////
	///// [ ### �ڵ����� ### ]
	int SendExitMode;				//	���� ��ư �ð� ����(1��, 5��...)
	BOOL m_bExitReserve;			//  ���� ����(����, ���)
	UINT hExitReserveTimer;			//  1000 Ÿ�̸�
	int m_ReservTimeLimit;			//	���� �ð� ����(�ʴ���)
	void ShowState(LPCTSTR szChar); //  ���� ����
	void ExitReserveTimer();		//  ���� �޼���
	///////////////////////////////////////////////////////////////


	UINT hMsgPoolingTimer;
	UINT hGlobalGameTimer;
	UINT hCloseTimer;
	UINT		hMsgUerCntTimer;	// [PC�� �۾�] ����� ī��Ʈ Ÿ�̸�

	CCommMsg TmpMsg;			// ��Ŷ �м��� �ӽ� �޽��� Ŭ����
	char MsgBuf[RECVQUESIZE];	// ��Ŷ �м��� �ӽ� ����
	int MsgSignal;				// ��Ŷ �м��� �ӽ� �ñ׳� ����
//	char SndBuf[SENDQUESIZE];	// ��Ŷ �۽ſ� �ӽ� ����

	int CheckPacket(CMySocket* pSock, char *lpBuf, int *pSignal);
	BOOL ProcessPacket(int sid, CMySocket *pSocket, char* lpdata, int totsize, int signal);

	void LoadConfig();
	void LoadConfig2();
	void LoadConfig3();
	void LoadConfig4(); // jackpot3
	void LoadConfigEvent();// ### [�̺�Ʈ Ƽ��] ###
	void LoadConfigStatis();// [ ����� ]

//	void Make_StatisticsLog(int Kind, int index, BOOL bIncrease = TRUE, INT64 pmoney= 0, INT64 usurymoney=0);// [ ����� ]
//	void Check_StatisticsLog();			// [ ����� ]
//	void Make_SetSADBWork(int Kind);	// [ ����� ]
//	void CloseServer_SALog();			// [ ����� ]


	void SendMsgToAll(char *pMsgData, int nMsgSize);
	BOOL FindID(RESULTFINDID *pRF, char *id);
	BOOL UserDisconnectProccess(int unum);
	void PrepareClose();
	void OnCloseTimer();

	CString GetLogFileName_view();

	// [������ ��� �۾�]
	BOOL IsAccessDeny(char* id, UINT &nSec); // [������ ��� �۾�]
	BOOL IsAdminUser(char* id, char* ip);    // [������ ��� �۾�]
	void LoadAdminInfo();                    // [������ ��� �۾�]
	void FindDoubleConnect(const int unum) ;

	// ### [ ä�� ���� ���� ] ###
	int CheckLevel(int nIcon ,INT64 money);

	// ### [�̺�Ʈ Ƽ��] ###
	void ReadUsedEventTicketNum();
	void WriteUsedEventTicketNum();
	void CheckGameEvent();

	// �Լ� ���� ���� ������ �Ѿ����� 
	
	INT64 Get_angel_defmoney(const INT64 &lmoney); // [��ȣõ��] 2004.07.08

	////////////////////////////////////////////////////////////  <----- �߰�
	//2004.05.07 �������Ѿ� ����
	void SendPremLeadersMsg(int unum, int PremLeadersCode);
	int CheckTotalmoneylimit(USERINFO* pUI, char *uniqno, int bCheckWrite=0); // 0 üũ, 1 �Ӵ����� 2 �Ӵ����� ���
	CString MoneyToString(INT64 number);
	////////////////////////////////////////////////////////////

	C62CutPokerServerDoc* GetDocument();

	C62CutPokerServerView();
	DECLARE_DYNCREATE(C62CutPokerServerView)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C62CutPokerServerView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	void SendChannelEventNotice(int unum);
	BOOL IsSearchUserID(char *id);
	void LoadSearchID();
	
// [��ȭ ����]
	BOOL IsCaptureUserID(char* id);
	void LoadCaptureTalkID();


	void ShowServerState();
	void JackPotSave();
	
	virtual ~C62CutPokerServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// �ڵ�����
// Generated message map functions
protected:
	//{{AFX_MSG(C62CutPokerServerView)
	afx_msg LONG OnServAccept(UINT, LONG);
	afx_msg LONG OnDataClose(UINT, LONG);
	afx_msg LONG OnDataConnect(UINT, LONG);
	afx_msg LONG OnDbworkLogin(UINT, LONG);
	afx_msg LONG OnDbworkLogout(UINT, LONG);
	afx_msg LONG OnDbworkSetOK(UINT, LONG);
	afx_msg LONG OnDbworkDoubleConnect(UINT, LONG);
	afx_msg void OnViewConfig();
	afx_msg void OnButtonSend();
	afx_msg void OnRadioSendtext();
	afx_msg void OnRadioSendmsgbox();
	afx_msg void OnRadioSendmsgboth();
	afx_msg void OnButtonSetloginmsg();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonForcedisconnect();
	afx_msg void OnButtonForcedelroom();
	afx_msg void OnViewConfig2();
	afx_msg void OnViewConfig3();
	afx_msg void OnViewConfig4();
	afx_msg void OnButtonJackpot();
	afx_msg void OnRadioExit1();
	afx_msg void OnRadioExit10();
	afx_msg void OnRadioExit3();
	afx_msg void OnRadioExit30();
	afx_msg void OnRadioExit30s();
	afx_msg void OnRadioExit5();
	afx_msg void OnButtonExitok();
	afx_msg void OnButtonExitcancel();
	afx_msg LONG OnDbworkUseitemresult(UINT, LONG);	// [���� ������ �۾�]
	afx_msg LONG OnDbworkGameitemlist(UINT, LONG);	// [���� ������ �۾�]
	afx_msg void OnSetAdmin();
	afx_msg void OnBtnDenyuser();
	afx_msg void OnSetCaptureid();
	afx_msg void OnSetSearch();
	afx_msg void OnViewConfigevent();  // ### [�̺�Ʈ Ƽ��] ###
	afx_msg void OnButtonEventstate();
	afx_msg LONG OnDbworkEventSet(UINT, LONG);	// ### [�̺�Ʈ Ƽ��] ###
	afx_msg void OnViewConfigsa();	   // [�����]
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SutdaServerView.cpp
inline C62CutPokerServerDoc* C62CutPokerServerView::GetDocument()
   { return (C62CutPokerServerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TETRISSERVERVIEW_H__8D85588A_B8D2_4EBF_ACC2_BAA9322B72F3__INCLUDED_)
