// DBMan.h: interface for the CDBMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBMAN_H__A31B62E7_02AE_4734_985D_DB70E44C8897__INCLUDED_)
#define AFX_DBMAN_H__A31B62E7_02AE_4734_985D_DB70E44C8897__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserDB.h"
#include "GameDB.h"
#include "BadUserDB.h"
#include "StatusDB.h"
#include "AvatarDB.h"
#include "ItemDB.h"		// [���� ������ �۾�]
#include "PremiumDB.h"	// [�����̾� �۾�]
#include "PremIPDB.h"	// [PC�� �۾�]

#include "EventPrizeDB.h"	// ### [�̺�Ʈ Ƽ��] ###


#include "UserStruct.h"
#include "DBThread.h"
#include "BadUserDBThread.h"

// ���� ������ DB ������ ���� ����
#define MINIMUM_DBTHREAD 1
#define MAXIMUM_DBTHREAD 200


class CDBMan  
{
public:
	CUserDB		m_UserDB;
	CGameDB		m_GameDB;
	CBadUserDB	m_BadUserDB;
	CStatusDB	m_StatusDB;
	CAvatarDB	m_AvatarDB;
	CItemDB		m_ItemDB;			// [���� ������ �۾�]
	CPremiumDB	m_PremiumDB;		// [�����̾� �۾�]
	CPremIPDB	m_PremIPDB;			// [PC�� �۾�]
	CEventPrizeDB m_EventPrizeDB;	// ### [�̺�Ʈ Ƽ��] ###

	// DB ť
	CDBQue DBQue;

	int TotThreadNum;

	// ���� DB ó�� ������ Ŭ���� �迭 ������
	CDBThread** pDBThread;

	// ������ ������ ����
	int DiscardNum;

	CDBMan();
	virtual ~CDBMan();

	CDBThread** AllocDBThreadPtr(int threadnum);
	BOOL ChangeDBThreadNum(int threadnum);

	// DB ���� �� �ʱ�ȭ �Լ�
	BOOL Init(int threadnum);
	BOOL CloseTempDB();
	BOOL CreateKeepAliveThread();

	BOOL SetDBWork(DBWORKS *pWork);
	BOOL DiscardDBThread(int tnum);
	int GetWorkingDBThreadNum();
	int GetLiveDBThreadNum();
	void CheckDBThread();
	void TermanateAllDBThread();

	// ������ ���� ���� ���� ���� �Ǻ� �Լ�
	BOOL CheckOtherServerExist(int GameCode, int ServerCode, const char *strIP, UINT nPort);

	// ���� �ڵ� �ʱ�ȭ �Լ�
	int GetSameConnectCodeNum(int GameCode, int ServerCode);
	BOOL ClearConnectCode(int GameCode, int ServerCode);

	// ������ �ο� ��Ȳ DB �ʱ�ȭ �Լ�
	BOOL InitStatusDB(int GameCode, int ServerCode, const char *strIP, UINT nPort);

	// �ҷ� ����� �Ű� DB�߰� �Լ�
	BOOL AddNewBadUserDB(CBadUserDBWorks *pWork);

	// �α� ����� ���� �����̸� ����
	CString GetLogFileName();

};

#endif // !defined(AFX_DBMAN_H__A31B62E7_02AE_4734_985D_DB70E44C8897__INCLUDED_)
