// StatisticsLog.h: interface for the CStatisticsLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATISTICSLOG_H__64A67AD3_6386_40CF_9304_16F52C796AB4__INCLUDED_)
#define AFX_STATISTICSLOG_H__64A67AD3_6386_40CF_9304_16F52C796AB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////
////////////        [ 통계기록 ]       //////////////
/////////////////////////////////////////////////////
/*
Kind :
		11		레벨별 동접수					10분
		12		레벨별 올인수					10분
		13		레벨별 게임횟수					10분
		14		레벨별 평균이용 시간			1일
		101		등급별 평균게임 판돈			10분
		102		등급별 게임비 누적액			10분
		103     등급별 미션횟수, 미션금액		10분
		104     등급별 잭팟횟수, 잭팟금액		10분

	!!! 게임별 사용하는 필드가 다를 수 있다.
								db 에서 사용되는 필드
		11 동접               - c_SmallData_0 ~ 14 
		12 올인               - c_SmallData_0 ~ 14 
		13 누적게임수         - c_SmallData_0 ~ 14 
		14 평균이용시간       - c_SmallData_0 ~ 14  (초 단위 저장)
		
		101 판돈              - c_BigData_0 ~ 4   (천만 단위) 횟수 c_SmallData_0 ~ 4
		102 게임비            - c_BigData_0 ~ 4   (천   단위) 횟수 c_SmallData_0 ~ 4
		103 미션금액,횟수     - c_BigData_0 ~ 4   (억   단위) 횟수 c_SmallData_0 ~ 4
		104 잭팟금액,횟수     - c_BigData_0 ~ 4   (억   단위) 횟수 c_SmallData_0 ~ 4
*/

// kind - value
enum { 
	LEVEL_CONCURRENT_USER=11,
	LEVEL_ALLIN_USER,
	LEVEL_USE_GAMECNT,
	LEVEL_USE_TIME,
	GRADE_AVERAGE_MONEY=101,
	GRADE_USURY_MONEY,
	GRADE_MISSION,
	GRADE_JACKPOT,
};


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////


class CStatisticsLog  
{
	// server
	typedef struct {
		int     Level_CurUser[20];			// 11
		int     Level_AllIn_User[20];		// 12
		int     Level_UseTime[20];			// 102
		int     Level_UseTimeUserCnt[20];	// 102
		int		Level_UseGameCnt[20];		// 13
		
		int     Grade_GameCnt[5];
		INT64   Grade_AverGameMoney[5];		// 14
		int     Grade_UsuryCnt[5];			
		INT64   Grade_UsuryMoney[5];		// 103
		int     Grade_MissonCnt[5];			// 104
		INT64   Grade_MissonMoney[5];		
		int     Grade_JackPotCnt[5];        // 105
		INT64   Grade_JackPotMoney[5];
	} STATISTICS_SINFO;
	
public:
	STATISTICS_SINFO m_SAInfo;

	CStatisticsLog();
	virtual ~CStatisticsLog();
public:
		
	void Make_Log(int Kind, int index, BOOL bIncrease=FALSE, INT64 pmoney=0, INT64 usurymoney=0);
	void Set_sadbwork(int Kind);
	void Time_check();
	void CloseServer_savedata();
};

#endif // !defined(AFX_STATISTICSLOG_H__64A67AD3_6386_40CF_9304_16F52C796AB4__INCLUDED_)
