// DBThread.h: interface for the CDBThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBTHREAD_H__E5A49F86_7CD0_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_DBTHREAD_H__E5A49F86_7CD0_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include <mmsystem.h>
#include "UserDB.h"
#include "GameDB.h"
#include "BadUserDB.h"
#include "StatusDB.h"
#include "AvatarDB.h"

#include "ItemDB.h"		// [���� ������ �۾�]
#include "PremiumDB.h"	// [�����̾� �۾�]
#include "PremIPDB.h"	// [PC�� �۾�]
#include "EventPrizeDB.h" // ### [�̺�Ʈ Ƽ��] ###


#include "UserStruct.h"

// [SSO �۾�] - �Ʒ� ���� ����
#define MAX_DBWORK 2700//3100//2100					// �ִ� DB ť �۾� ����(�ִ� �������� 1.5������)
#define UM_DBWORK_LOGIN (WM_USER+1201)					// ���� �α��� ����� �޽���
#define UM_DBWORK_LOGOUT (WM_USER+1202)					// ���� �α׾ƿ� ����� �޽���
#define UM_DBWORK_DCON  (WM_USER+1203)					// �ߺ����� ���̴�
#define UM_DBWORK_SETDB (WM_USER+1204)					// ���� �߰��� ��� �Ϸ� �޽���
#define UM_DBWORK_GAMEITEMLIST	(WM_USER+1209)			// ������ ��� ���� ��� �޽���	[���� ������ �۾�]
#define UM_DBWORK_USEITEMRESULT (WM_USER+1210)			// ������ ��� ��� �޽���		[���� ������ �۾�]
#define UM_DBWORK_EVENTSET (WM_USER+1220)				// ### [�̺�Ʈ Ƽ��] ### �̺�Ʈ ���(�߰����)




typedef struct
{
	BOOL			bValid;					// ��ȿ�� �����ΰ�?
	int				WorkKind;				// ó���� ���� ����(1:�α��� ���ɿ��� ó�� 2:�α��� ��� ó�� 3:���ӱ�� ó�� 4:�α׾ƿ� ó��)
	int				SID;					// ���� ��ȣ
	UINT			StartTick;				// DB ó�� ���� �ð�
	UINT			UniqueTick;				// ó������ ����ũ�� �ο��ϱ����� Ÿ�� Tick
	USERINFO		UI;						// ó���� ����� ����
	ORGINFO			OI;						// ����� ����� ����
	INT64			LostMoney;				// [��ȣõ��] 2004.07.08 ������ ���� �ݾ�
	char			RegID[16];				// �ֹε�� ��ȣ [���� ������ �۾�]
	char			UniqNo[16];				// ����ũ �ѹ�
	char			UserIP[24];				// ������� ���� IP����		[PC�� �۾�]
	char			Passwd[16];				// �н�����(��ȣȭ�Ǿ� Ŀ��)	[SSO �۾�] - �߰�

	int				ConnectTime;			// ���� �ð�(�� ����)
	// �Ʒ��� �������� DBó�� ����� ���õ�
	BOOL			bIsWrongPwd;			// �н����� Ʋ�� ����
	BOOL			bIsWrongUniqNo;			// ����ũ �ѹ��� Ʋ�� ����	[SSO �۾�] - �߰�
	BOOL			bIsNotUserID;			// ȸ�� ���̵� �������� �ʴ°�?
	int				IDState;				// ���̵� ���� ��밡�� ���� �÷���(0:���� 1:������ 2:��������)
	BOOL			bAnotherConnect;	// �ٸ� ���ӿ� �̹� �α��� ���ΰ�?
	// [���� ������ �۾�]
	GAMEITEM_LIST	Item;					// ������ ����Ʈ
	int				bErrorItemWork;			// ������ �۾� ��� �ڵ�(������ ��� Ȥ�� ������ ����Ʈ ����)
	GAMEITEM		UseItem;				// ����� ������
	BOOL			bCertificate;			// [��������]
	EVENT_PRIZE     EventPrize;				// ### [�̺�Ʈ Ƽ��] ###
	int				bForceLogin;		//2004.05.07 �������Ѿ� ����    <--------- �߰�

} DBWORKS;

//////////////////////////////////////////////////////////////////////
// DBť Ŭ����
//////////////////////////////////////////////////////////////////////
// [SSO �۾�] - �Ʒ� Ŭ������ �� �ڵ�� ��ü
class CDBQue  
{
public:
	CCriticalSection DBQueLock;

	list<DBWORKS*> Work;

	CDBQue();
	virtual ~CDBQue();

	BOOL EnQue(DBWORKS *pNewWork);
	BOOL DeQue(DBWORKS *pWork);
	int GetTotNum();
};
/*
class CDBQue  
{
public:
	CCriticalSection DBQueLock;
	int SPos, EPos;
	int TotNum;
	DBWORKS Work[MAX_DBWORK];

	CDBQue();
	virtual ~CDBQue();

	BOOL EnQue(DBWORKS *pNewWork);
	BOOL DeQue(DBWORKS *pWork);
	int GetTotNum();
};
*/
//////////////////////////////////////////////////////////////////////
// DB ó�� ������ Ŭ����
//////////////////////////////////////////////////////////////////////

class CDBThread
{
public:
	
	CWinThread *pThread;	// ������ ������
	CDBQue *pDBQue;			// DBť ������

	BOOL bNowTerminate;		// ������ ���� �÷���
	BOOL bWorking;			// DBó�� ���ΰ�?
	int ErrorCnt;			// DB���� ī��Ʈ
	DBWORKS Work;			// ó���� DB

	// Database Ŭ����
	CDatabase*		pDatabase_User;
	CDatabase*		pDatabase_Game;
	CDatabase*		pDatabase_Status;
	CDatabase*		pDatabase_Avatar;
	CDatabase*		pDatabase_Item;		// [���� ������ �۾�]
	CDatabase*		pDatabase_Premium;	// [�����̾� �۾�]
	CDatabase*		pDatabase_PremIP;	// [PC�� �۾�]
	CDatabase*      pDatabase_EventPrize;    // ### [�̺�Ʈ Ƽ��] ###
	

	// DB ���ڵ�� Ŭ���� ������
	CUserDB*		m_pUserDB;
	CGameDB*		m_pGameDB;
	CStatusDB*		m_pStatusDB;
	CAvatarDB*		m_pAvatarDB;
	CItemDB*		m_pItemDB;		// [���� ������ �۾�]
	CPremiumDB*		m_pPremiumDB;	// [�����̾� �۾�]
	CPremIPDB*		m_pPremIPDB;	// [PC�� �۾�]
	CEventPrizeDB*  m_pEventPrizeDB;		// ### [�̺�Ʈ Ƽ��] ###
	
	CDBThread();
	virtual ~CDBThread();

	// ������
	BOOL Begin(CDBQue *pQue);
	BOOL Terminate();

	// ���ڵ�� ���� �Լ�
	BOOL CreateUserDB();
	BOOL CreateGameDB();
	BOOL CreateStatusDB();
	BOOL CreateAvatarDB();
	BOOL CreateItemDB();	// [���� ������ �۾�]
	BOOL CreatePremiumDB();	// [�����̾� �۾�]
	BOOL CreatePremIPDB();	// [PC�� �۾�]
	BOOL CreateEventPrizeDB();// ### [�̺�Ʈ Ƽ��] ###


	// ��� ���ڵ���� ������
	void DeleteAllDB();

	// DB�۾� ó���Լ�
	void ProcessDBWork();

	// ����� ���� �б� �� ���� �Լ�
	// ### [�̺�Ʈ Ƽ��] ###
	BOOL SetUserInfo(DBWORKS *pWork);// [SSO �۾�] - ����
	// ���� DB ���� �߰� �Լ�
	BOOL AddNewGameDB(USERINFO *pUI);
	BOOL GetUserInfo(DBWORKS *pWork);			// [���� ������ �۾�]


	BOOL GetMainUserInfo_FromDB(DBWORKS *pWork);	// [SSO �۾�] - �߰�
	BOOL GetGameInfo(DBWORKS *pWork);				// [SSO �۾�] - �߰�
	BOOL UpdateConnectStatus();						// [SSO �۾�] - �߰�
	
	// ���� ������ �б� �� ���� �Լ�	[���� ������ �۾�]
	BOOL GetItemList(char *id, GAMEITEM_LIST *pItemList);
	BOOL UseGameItem(char *id, GAMEITEM *pItem);
	// �����̾� ���� �б�				[�����̾� �۾�]
	BOOL GetPremiumInfo(LPCTSTR uniqno, USERINFO* pUI);
	// �����̾� IP ���� �б�			[PC�� �۾�] [SSO �۾�]
	BOOL GetPremiumIP(DBWORKS *pWork);
	
	// �̺�Ʈ ���� �б�					 ### [�̺�Ʈ Ƽ��] ###
	BOOL GetEventPrize(DBWORKS *pWork);
	BOOL AddNewEventPrizeDB(USERINFO *pUI);
	BOOL SetEventPrizeDB(char *id, EVENT_PRIZE *pEvent, BOOL bInit = FALSE);
	CString GetLogFileName();

	// DB ������ ���
	static UINT DBThreadFunc(LPVOID pParam);
};

// for static function
static UINT CDBThread::DBThreadFunc(LPVOID pParam);


#endif // !defined(AFX_DBTHREAD_H__E5A49F86_7CD0_11D4_97A4_0050BF0FBE67__INCLUDED_)
