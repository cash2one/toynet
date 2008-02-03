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
/* data ���� */
void CStatisticsLog::Make_Log(int Kind, int index, BOOL bIncrease/*=FALSE*/, INT64 pmoney/*=0*/, INT64 usurymoney/*=0*/)
{
	if( !g_pMainView->Cfg.bStatisticsDB || !g_pMainView->StatisCfg.bSaveSetLog ) return;
	if(Kind == LEVEL_CONCURRENT_USER) { 
		// 11 ���� ���ϱ�, ���μ�, ���� ���ӽð�
		int Level = User[index].m_UserLevel;
		if( bIncrease ) {
			m_SAInfo.Level_CurUser[ Level ] += 1;		// ������ ����
		} else {
			m_SAInfo.Level_CurUser[ Level ] -= 1;		// ������ ����
			if( User[index].UI.PMoney <=  0) m_SAInfo.Level_AllIn_User[ Level ] += 1; // ���μ� ����
			COleDateTime nt = COleDateTime::GetCurrentTime();
			COleDateTimeSpan span = nt - User[index].m_StartTime;
			if(User[index].OI.WinNum != User[index].UI.WinNum ||
				User[index].OI.LooseNum != User[index].UI.LooseNum ||
				User[index].OI.DrawNum != User[index].UI.DrawNum )
			{
				int time = span.GetSeconds();	
				m_SAInfo.Level_UseTime[ Level ] += time;	// ���� ���ӽð�
				m_SAInfo.Level_UseTimeUserCnt[ Level ] += 1;			
			}
		}
	} else if(Kind == LEVEL_USE_GAMECNT) {						// 13 �������Ӽ�
		int Level = User[index].m_UserLevel;
		m_SAInfo.Level_UseGameCnt[ Level ] += 1;
	} else if(Kind == GRADE_AVERAGE_MONEY) {					// ��޺� ���ӸӴ�, ���Ӻ�
		int grade = index; // ä�� ����
		if(pmoney > 10000000) {
			m_SAInfo.Grade_GameCnt[ grade ]	+= 1;			// ��޺� �����Ǽ�
			m_SAInfo.Grade_AverGameMoney[ grade ]+= pmoney	/ 10000000  ;	// �� ���øӴ�
		}
		if(usurymoney > 1000) {
			m_SAInfo.Grade_UsuryCnt[ grade ]	+= 1;			// ��޺� �����Ǽ�
			m_SAInfo.Grade_UsuryMoney[ grade ]+= usurymoney	/ 1000;	// ���Ӻ�	
		}
	} else if(Kind == GRADE_MISSION) {							// �̼� ����, ���� �Ӵ�
		int grade = index; // ä�� ����
		m_SAInfo.Grade_MissonCnt[ grade ] += 1;
		m_SAInfo.Grade_MissonMoney[ grade ] += pmoney    / 100000000;
	} else if(Kind == GRADE_JACKPOT) {							// ���� ����, ���޸Ӵ�
		int grade = index; // ä�� ����
		m_SAInfo.Grade_JackPotCnt[ grade ] += 1;
		m_SAInfo.Grade_JackPotMoney[ grade ] += pmoney   / 100000000;
	}

}

/* db - data set */
void CStatisticsLog::Set_sadbwork(int Kind)
{
	LOGREC_DBWORKS SWork;
	ZeroMemory(&SWork, sizeof(LOGREC_DBWORKS));
	SWork.WorkKind = 2;  // ����� code 2
	SWork.StartTick = timeGetTime();
	SWork.bValid = TRUE;
	SWork.StatisticsLog.Kind = Kind; 

	if(Kind == LEVEL_CONCURRENT_USER) {		// ����
		for(int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Level_CurUser[i];
		}
	} else if(Kind == LEVEL_ALLIN_USER) {	// ��������
		for( int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Level_AllIn_User[i];
			m_SAInfo.Level_AllIn_User[i]= 0;	
		}
	} else if(Kind == GRADE_AVERAGE_MONEY) {// ��޺� �����ǵ�
		for(int i=0;i<5;i++) {
			if(m_SAInfo.Grade_GameCnt[i] > 0) {
				SWork.StatisticsLog.BigData[i]	= m_SAInfo.Grade_AverGameMoney[i]/m_SAInfo.Grade_GameCnt[i];
				SWork.StatisticsLog.SmallData[i]= m_SAInfo.Grade_GameCnt[i];
				m_SAInfo.Grade_AverGameMoney[i] = 0; 
				m_SAInfo.Grade_GameCnt[i] = 0;		
			}
		}
	} else if(Kind ==  LEVEL_USE_GAMECNT) { // ������ �������Ӽ�
		for( int i=0; i<20; i++) {
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Level_UseGameCnt[i];
			m_SAInfo.Level_UseGameCnt[i] = 0;
		}
	} else if(Kind == LEVEL_USE_TIME) {		// ����̿�ð�
		for( int i=0; i<20; i++) {
			if(m_SAInfo.Level_UseTimeUserCnt[i]>0) {
				SWork.StatisticsLog.SmallData[i] = m_SAInfo.Level_UseTime[i]/m_SAInfo.Level_UseTimeUserCnt[i];
				m_SAInfo.Level_UseTime[i]= 0;			
				m_SAInfo.Level_UseTimeUserCnt[i] = 0;	
			}
		}
	} else if(Kind == GRADE_USURY_MONEY) {  // ���Ӻ� ������
		for( int i=0; i<5; i++) {
			SWork.StatisticsLog.BigData[i]   = m_SAInfo.Grade_UsuryMoney[i];
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Grade_UsuryCnt[i];
			m_SAInfo.Grade_UsuryMoney[i] = 0;
			m_SAInfo.Grade_UsuryCnt[i] = 0;
		}

	} else if(Kind == GRADE_JACKPOT) {		// ���� ���
		for( int i=0; i<5; i++) {
			SWork.StatisticsLog.SmallData[i] = m_SAInfo.Grade_JackPotCnt[i];
			SWork.StatisticsLog.BigData[i]	 = m_SAInfo.Grade_JackPotMoney[i];
			m_SAInfo.Grade_JackPotCnt[i]   = 0;
			m_SAInfo.Grade_JackPotMoney[i] = 0;
		}
	} else if(Kind == GRADE_MISSION) {		// �̼� ���
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

/* �ð���� üũ */
void CStatisticsLog::Time_check()
{
	if( !g_pMainView->Cfg.bStatisticsDB || !g_pMainView->StatisCfg.bSaveSetLog ) return;
	/*
	CTime nt = CTime::GetCurrentTime();				// ������ ���� �ð�
	CTime st = m_SAInfo.PrevTime_1;					// ���� ��� �ð�
	CTimeSpan TimeSpan = nt - st;					// ����ð����� ���̸� ���Ѵ�
	int passedS = TimeSpan.GetTotalSeconds();
	if(passedS >= 60*10) {					
	*/

	CTime nt = CTime::GetCurrentTime();				// ������ ���� �ð�
	if(nt.GetMinute()%10==0 && nt.GetSecond() == 0 ) { // 10�� ���� ���
		Set_sadbwork(LEVEL_CONCURRENT_USER);	// ����
		Set_sadbwork(LEVEL_ALLIN_USER);			// ��������
		Set_sadbwork(LEVEL_USE_GAMECNT);		// ������ �������Ӽ�
		Set_sadbwork(GRADE_AVERAGE_MONEY);		// ��޺� �����ǵ�
		Set_sadbwork(GRADE_USURY_MONEY);		// ���Ӻ� ������
		
		// !!! ���ӿ� ���� ���ٸ� �ּ� ó�� �� ��
		Set_sadbwork(GRADE_JACKPOT);			// ���� ���
		// !!! ���ӿ� �̼� ���ٸ� �ּ� ó�� �� ��
		//Set_sadbwork(GRADE_MISSION);			// �̼� ���
	}
	
	// 24�� 10�п� �����Ѵ�.
	if(nt.GetHour() == 0 && nt.GetMinute() == 10 && nt.GetSecond() == 0 ) { 
		Set_sadbwork(LEVEL_USE_TIME);					// ����̿�ð�
	}

}

/* ���� ����� data ���� */
void CStatisticsLog::CloseServer_savedata()
{
	if( !g_pMainView->Cfg.bStatisticsDB || !g_pMainView->StatisCfg.bSaveSetLog ) return;

	Set_sadbwork(LEVEL_CONCURRENT_USER);	// ����
	Set_sadbwork(LEVEL_ALLIN_USER);			// ��������
	Set_sadbwork(GRADE_AVERAGE_MONEY);		// ��޺� �����ǵ�
	Set_sadbwork(LEVEL_USE_GAMECNT);		// ������ �������Ӽ�
	Set_sadbwork(LEVEL_USE_TIME);			// ����̿�ð�
	Set_sadbwork(GRADE_USURY_MONEY);		// ���Ӻ� ������
	
	// !!! ���ӿ� ���� ���ٸ� �ּ� ó�� �� ��
	Set_sadbwork(GRADE_JACKPOT);			// ���� ���
	
	// !!! ���ӿ� �̼� ���ٸ� �ּ� ó�� �� ��
	//Set_sadbwork(GRADE_MISSION);			// �̼� ���
}
