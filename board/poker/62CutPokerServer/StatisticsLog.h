// StatisticsLog.h: interface for the CStatisticsLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATISTICSLOG_H__64A67AD3_6386_40CF_9304_16F52C796AB4__INCLUDED_)
#define AFX_STATISTICSLOG_H__64A67AD3_6386_40CF_9304_16F52C796AB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////
////////////        [ ����� ]       //////////////
/////////////////////////////////////////////////////
/*
Kind :
		11		������ ������					10��
		12		������ ���μ�					10��
		13		������ ����Ƚ��					10��
		14		������ ����̿� �ð�			1��
		101		��޺� ��հ��� �ǵ�			10��
		102		��޺� ���Ӻ� ������			10��
		103     ��޺� �̼�Ƚ��, �̼Ǳݾ�		10��
		104     ��޺� ����Ƚ��, ���̱ݾ�		10��

	!!! ���Ӻ� ����ϴ� �ʵ尡 �ٸ� �� �ִ�.
								db ���� ���Ǵ� �ʵ�
		11 ����               - c_SmallData_0 ~ 14 
		12 ����               - c_SmallData_0 ~ 14 
		13 �������Ӽ�         - c_SmallData_0 ~ 14 
		14 ����̿�ð�       - c_SmallData_0 ~ 14  (�� ���� ����)
		
		101 �ǵ�              - c_BigData_0 ~ 4   (õ�� ����) Ƚ�� c_SmallData_0 ~ 4
		102 ���Ӻ�            - c_BigData_0 ~ 4   (õ   ����) Ƚ�� c_SmallData_0 ~ 4
		103 �̼Ǳݾ�,Ƚ��     - c_BigData_0 ~ 4   (��   ����) Ƚ�� c_SmallData_0 ~ 4
		104 ���̱ݾ�,Ƚ��     - c_BigData_0 ~ 4   (��   ����) Ƚ�� c_SmallData_0 ~ 4
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
