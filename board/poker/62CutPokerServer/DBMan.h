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
#include "ItemDB.h"		// [게임 아이템 작업]
#include "PremiumDB.h"	// [프리미엄 작업]
#include "PremIPDB.h"	// [PC방 작업]

#include "EventPrizeDB.h"	// ### [이벤트 티켓] ###


#include "UserStruct.h"
#include "DBThread.h"
#include "BadUserDBThread.h"

// 설정 가능한 DB 쓰레드 개수 범위
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
	CItemDB		m_ItemDB;			// [게임 아이템 작업]
	CPremiumDB	m_PremiumDB;		// [프리미엄 작업]
	CPremIPDB	m_PremIPDB;			// [PC방 작업]
	CEventPrizeDB m_EventPrizeDB;	// ### [이벤트 티켓] ###

	// DB 큐
	CDBQue DBQue;

	int TotThreadNum;

	// 유저 DB 처리 쓰레드 클래스 배열 포인터
	CDBThread** pDBThread;

	// 버려진 쓰레드 개수
	int DiscardNum;

	CDBMan();
	virtual ~CDBMan();

	CDBThread** AllocDBThreadPtr(int threadnum);
	BOOL ChangeDBThreadNum(int threadnum);

	// DB 오픈 및 초기화 함수
	BOOL Init(int threadnum);
	BOOL CloseTempDB();
	BOOL CreateKeepAliveThread();

	BOOL SetDBWork(DBWORKS *pWork);
	BOOL DiscardDBThread(int tnum);
	int GetWorkingDBThreadNum();
	int GetLiveDBThreadNum();
	void CheckDBThread();
	void TermanateAllDBThread();

	// 동일한 서버 정보 존재 여부 판별 함수
	BOOL CheckOtherServerExist(int GameCode, int ServerCode, const char *strIP, UINT nPort);

	// 접속 코드 초기화 함수
	int GetSameConnectCodeNum(int GameCode, int ServerCode);
	BOOL ClearConnectCode(int GameCode, int ServerCode);

	// 접속자 인원 현황 DB 초기화 함수
	BOOL InitStatusDB(int GameCode, int ServerCode, const char *strIP, UINT nPort);

	// 불량 사용자 신고 DB추가 함수
	BOOL AddNewBadUserDB(CBadUserDBWorks *pWork);

	// 로그 기록을 위한 파일이름 세팅
	CString GetLogFileName();

};

#endif // !defined(AFX_DBMAN_H__A31B62E7_02AE_4734_985D_DB70E44C8897__INCLUDED_)
