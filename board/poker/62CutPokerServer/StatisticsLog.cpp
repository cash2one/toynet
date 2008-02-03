// StatisticsLog.cpp: implementation of the CStatisticsLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatisticsLog.h"
#include "62CutPokerServerView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatisticsLog::CStatisticsLog()
{
	ZeroMemory(&m_SAInfo, sizeof(m_SAInfo));
}

CStatisticsLog::~CStatisticsLog()
{

}
/* data 수집 */
void CStatisticsLog::Make_Log(int Kind, int index, BOOL bIncrease/*=FALSE*/, INT64 pmoney/*=0*/, INT64 usurymoney/*=0*/)
{
	if( !g_pMainView->Cfg.bStatisticsDB || !g_pMainView->StatisCfg.bSaveSetLog ) return;
	if(Kind == LEVEL_CONCURRENT_USER) { 
		// 11 동접 구하기, 올인수, 유저 접속시간
		int Level = User[index].m_UserLevel;
		if( bIncrease ) {
			m_SAInfo.Level_CurUser[ Level ] += 1;		// 동접수 증가
		} else {
			m_SAInfo.Level_CurUser[ Level ] -= 1;		// 동접수 감소
			if( User[index].UI.PMoney <=  0) m_SAInfo.Level_AllIn_User[ Level ] += 1; // 올인수 증가
			COleDateTime nt = COleDateTime::GetCurrentTime();
			COleDateTimeSpan span = nt - User[index].m_StartTime;
			if(User[index].OI.WinNum != User[index].UI.WinNum ||
				User[index].OI.LooseNum != User[index].UI.LooseNum ||
				User[index].OI.DrawNum != User[index].UI.DrawNum )
			{
				int time = span.GetSeconds();	
				m_SAInfo.Level_UseTime[ Level ] += time;	// 유저 접속시간
				m_SAInfo.Level_UseTimeUserCnt[ Level ] += 1;			
			}
		}
	} else if(Kind == LEVEL_USE_GAMECNT) {						// 13 누적게임수
		int Level = User[index].m_UserLevel;
		m_SAInfo.Level_UseGameCnt[ Level ] += 1;
	} else if(Kind == GRADE_AVERAGE_MONEY) {					// 등급별 게임머니, 게임비
		int grade = index; // 채널 구분
		if(pmoney > 10000000) {
			m_SAInfo.Grade_GameCnt[ grade ]	+= 1;			// 등급별 게임판수
			m_SAInfo.Grade_AverGameMoney[ grade ]+= pmoney	/ 10000000  ;	// 총 베팅머니
		}
		if(usurymoney > 1000) {
			m_SAInfo.Grade_UsuryCnt[ grade ]	+= 1;			// 등급별 게임판수
			m_SAInfo.Grade_UsuryMoney[ grade ]+= usurymoney	/ 1000;	// 게임비	
		}
	} else if(Kind == GRADE_MISSION) {							// 미션 성공, 지급 머니
		int grade = index; // 채널 구분
		m_SAInfo.Grade_MissonCnt[ grade ] += 1;
		m_SAInfo.Grade_MissonMoney[ grade ] += pmoney    / 100000000;
	} else if(Kind == GRADE_JACKPOT) {							// 잭팟 성공, 지급머니
		int grade = index; // 채널 구분
		m_SAInfo.Grade_JackPotCnt[ grade ] += 1;
		m_SAInfo.Grade_JackPotMoney[ grade ] += pmoney   / 100000000;
	}

}

/* db - data set */
void CStatisticsLog::Set_sadbwork(int Kind)
{
	LOGREC_DBWORKS SWork;
	ZeroMemory(&SWork, sizeof(LOGREC_DBWORKS));
	SWork.WorkKind = 2;  // 통계기록 code 2
	SWork.StartTick = timeGetTime();
	SWork.bValid = TRUE;
	SWork.StatisticsLog.Kind = Kind; 

	if(Kind == LEVEL_CONCURRENT_USER) {		// 동접
		for(int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Level_CurUser[i];
		}
	} else if(Kind == LEVEL_ALLIN_USER) {	// 올인유저
		for( int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Level_AllIn_User[i];
			m_SAInfo.Level_AllIn_User[i]= 0;	
		}
	} else if(Kind == GRADE_AVERAGE_MONEY) {// 등급별 게임판돈
		for(int i=0;i<5;i++) {
			if(m_SAInfo.Grade_GameCnt[i] > 0) {
				SWork.StatisticsLog.BigData[i]	= m_SAInfo.Grade_AverGameMoney[i]/m_SAInfo.Grade_GameCnt[i];
				SWork.StatisticsLog.SmallData[i]= m_SAInfo.Grade_GameCnt[i];
				m_SAInfo.Grade_AverGameMoney[i] = 0; 
				m_SAInfo.Grade_GameCnt[i] = 0;		
			}
		}
	} else if(Kind ==  LEVEL_USE_GAMECNT) { // 레벨별 누적게임수
		for( int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Level_UseGameCnt[i];
			m_SAInfo.Level_UseGameCnt[i] = 0;
		}
	} else if(Kind == LEVEL_USE_TIME) {		// 평균이용시간
		for( int i=0; i<20; i++) {
			if(m_SAInfo.Level_UseTimeUserCnt[i]>0) {
				SWork.StatisticsLog.SmallData[i] = m_SAInfo.Level_UseTime[i]/m_SAInfo.Level_UseTimeUserCnt[i];
				m_SAInfo.Level_UseTime[i]= 0;			
				m_SAInfo.Level_UseTimeUserCnt[i] = 0;	
			}
		}
	} else if(Kind == GRADE_USURY_MONEY) {  // 게임비 누적액
		for( int i=0; i<5; i++) {
			SWork.StatisticsLog.BigData[i]   = m_SAInfo.Grade_UsuryMoney[i];
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Grade_UsuryCnt[i];
			m_SAInfo.Grade_UsuryMoney[i] = 0;
			m_SAInfo.Grade_UsuryCnt[i] = 0;
		}

	} else if(Kind == GRADE_JACKPOT) {		// 잭팟 기록
		for( int i=0; i<5; i++) {
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Grade_JackPotCnt[i];
			SWork.StatisticsLog.BigData[i]	 = m_SAInfo.Grade_JackPotMoney[i];
			m_SAInfo.Grade_JackPotCnt[i]   = 0;
			m_SAInfo.Grade_JackPotMoney[i] = 0;
		}
	} else if(Kind == GRADE_MISSION) {		// 미션 기록
		for( int i=0; i<5; i++) {
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Grade_MissonCnt[i];
			SWork.StatisticsLog.BigData[i]	 = m_SAInfo.Grade_MissonMoney[i];
			m_SAInfo.Grade_MissonCnt[i]   = 0;
			m_SAInfo.Grade_MissonMoney[i] = 0;
		}
	}

	SWork.StatisticsLog.LDate = COleDateTime::GetCurrentTime();	

	BOOL rtn = LogDBMan.SetDBWork(&SWork);
	if(rtn == FALSE) {
		CLogFile logfile("StatisticsLogDB.txt");
		logfile.Writef("Check_StatisticsLog() Error");
	}

}

/* 시간경과 체크 */
void CStatisticsLog::Time_check()
{
	if( !g_pMainView->Cfg.bStatisticsDB || !g_pMainView->StatisCfg.bSaveSetLog ) return;
	/*
	CTime nt = CTime::GetCurrentTime();				// 현재의 실제 시간
	CTime st = m_SAInfo.PrevTime_1;					// 이전 기록 시간
	CTimeSpan TimeSpan = nt - st;					// 현재시간과의 차이를 구한다
	int passedS = TimeSpan.GetTotalSeconds();
	if(passedS >= 60*10) {					
	*/

	CTime nt = CTime::GetCurrentTime();				// 현재의 실제 시간
	if(nt.GetMinute()%10==0 && nt.GetSecond() == 0 ) { // 10분 마다 기록
		Set_sadbwork(LEVEL_CONCURRENT_USER);	// 동접
		Set_sadbwork(LEVEL_ALLIN_USER);			// 올인유저
		Set_sadbwork(LEVEL_USE_GAMECNT);		// 레벨별 누적게임수
		Set_sadbwork(GRADE_AVERAGE_MONEY);		// 등급별 게임판돈
		Set_sadbwork(GRADE_USURY_MONEY);		// 게임비 누적액
		
		// !!! 게임에 잭팟 없다면 주석 처리 할 것
		Set_sadbwork(GRADE_JACKPOT);			// 잭팟 기록
		// !!! 게임에 미션 없다면 주석 처리 할 것
		//Set_sadbwork(GRADE_MISSION);			// 미션 기록
	}
	
	// 24시 10분에 저장한다.
	if(nt.GetHour() == 0 && nt.GetMinute() == 10 && nt.GetSecond() == 0 ) { 
		Set_sadbwork(LEVEL_USE_TIME);					// 평균이용시간
	}

}

/* 서버 종료시 data 저장 */
void CStatisticsLog::CloseServer_savedata()
{
	if( !g_pMainView->Cfg.bStatisticsDB || !g_pMainView->StatisCfg.bSaveSetLog ) return;

	Set_sadbwork(LEVEL_CONCURRENT_USER);	// 동접
	Set_sadbwork(LEVEL_ALLIN_USER);			// 올인유저
	Set_sadbwork(GRADE_AVERAGE_MONEY);		// 등급별 게임판돈
	Set_sadbwork(LEVEL_USE_GAMECNT);		// 레벨별 누적게임수
	Set_sadbwork(LEVEL_USE_TIME);			// 평균이용시간
	Set_sadbwork(GRADE_USURY_MONEY);		// 게임비 누적액
	
	// !!! 게임에 잭팟 없다면 주석 처리 할 것
	Set_sadbwork(GRADE_JACKPOT);			// 잭팟 기록
	
	// !!! 게임에 미션 없다면 주석 처리 할 것
	//Set_sadbwork(GRADE_MISSION);			// 미션 기록
}
