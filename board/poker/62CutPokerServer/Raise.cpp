// Raise.cpp: implementation of the CRaise class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "NMServer.h"
//#include "NMView.h"
#include "62CutPokerServer.h"
#include "62CutPokerServerView.h"


#include "Raise.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 세븐포카 레이즈 클래스.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaise::CRaise()
{
	Init(0);
}

CRaise::~CRaise()
{
}

void CRaise::Reset()
{
	for(int i=0;i<RAISE_USER;i++) memset(&m_User[i],0,sizeof(RAISEUSER));

	m_nDouble     = 0;
	m_nStyle      = 0;
	m_nRound      = 0;
	m_nTotalRaise = 0;
	m_fBatRate    = 0.0;

}

// 초기화한다
void CRaise::Init(int nBat)
{
	Reset();
}

// 사용자를 세팅한다(이전에 사용자머니는 가감된다)
void CRaise::SetUser(int nNum, INT64 nSchool, USERINFO &ui)
{
	if(!AnalNum(nNum)) return;
	
	memset(&m_User[nNum],0,sizeof(RAISEUSER));
	m_User[nNum].nUNum    = ui.UNum;
	m_User[nNum].nPMoney  = ui.PMoney;
	m_User[nNum].nNowBat  = 0;
	m_User[nNum].nTotBat  = nSchool;
	m_User[nNum].nRealBat = nSchool;
	m_User[nNum].bInUse   = TRUE;
}

// 초기설정을 한다(세팅전에 사용자를 먼저 세팅하여야한다)
void CRaise::Set(int nRaise,int nBat) // level
{
	if(nBat==0)      m_fBatRate = 0.25; // 쿼터방
	else if(nBat==1) m_fBatRate = 0.5;  // 하프방
///////////	else if(nBat==2) m_fBatRate = 1.0;  // 풀방
	else             m_fBatRate = 0.25; // 쿼터방

	m_nStyle  = nRaise; // 레이즈 스타일
	m_nDouble = 0;
	int nUser = 0;
	for(int i=0; i<RAISE_USER; i++){
		m_User[i].nLastCmd = 0;
		if(m_User[i].bInUse){ // 즉 게임에 참가한 사람들이란 이야기
			nUser++;
		}
	}

	// [ ###베팅룰### ]
	switch(m_nStyle){//m_nStyle = ( 자유 초보 중수 = 0, 고수 = 1 도신 =2)
		case 0:  m_nTotalRaise = nUser*1; break; // 1회 레이즈 자유
		case 1:  m_nTotalRaise = nUser*1; break; // 2회 레이즈 초보
		case 2:  m_nTotalRaise = nUser*1; break; // 2회 레이즈 중수
		case 3:  m_nTotalRaise = nUser*1; break; // 3회 레이즈 고수
		case 4:  m_nTotalRaise = nUser*1; break; // 3회 레이즈 도신
		default: m_nTotalRaise = nUser*1; break;
	}
	m_nRound    = 1; // 세팅하면서 4(1+3)구시작
}



// 1 - 1 - 1 - 2
void CRaise::Start(PLAYSTATE *ps)
{
	if(ps==NULL) return;
	int i=0,nUser = 0;
	m_nDouble     = 0;
	for(i=0; i<RAISE_USER; i++){
		m_User[i].nLastCmd= 0;     // 기억된 명령
		m_User[i].nNowBat = 0;     // 이번레이즈 배팅 초기화
		m_User[i].nUse    = 0;     // 자기기회 사용 초기화
		m_User[i].bCall   = FALSE; // 콜사인 초기화
		if(m_User[i].bInUse && ps[i].bOnGame && !ps[i].bFold){ // 즉 게임에 참가한 사람들이란 이야기
			nUser++;
		}
	}

	m_nRound++;

	// [ ###베팅룰### ]
	switch(m_nStyle){
		case 0:  // 자유
		case 1:  // 초보
			{
				// 1 - 2 - 2 - 2
				if(m_nRound>=2) 
					m_nTotalRaise = nUser*2;  // 2회 레이즈
				else
					m_nTotalRaise = nUser*1; // 1회 레이즈 
			} break; 
		case 2: // 중수
			{
				// 1 - 2 - 2 - 2
				if(m_nRound>=2) 
					m_nTotalRaise = nUser*2; // 히든은 2회
				else
					m_nTotalRaise = nUser*1; // 1회 레이즈 

			} break; 
		case 3: // 고수
			{
				// 1 - 2 - 2 - 2 half
				if(m_nRound>=2) 
					m_nTotalRaise = nUser*2; // 히든은 2회
				else
					m_nTotalRaise = nUser*1; // 1회 레이즈 

			} break; 

		case 4: // 도신
			{
				// 1 - 2 - 2 - 2 half
				if(m_nRound>=2) 
					m_nTotalRaise = nUser*2; // 히든은 2회
				else
					m_nTotalRaise = nUser*1; // 1회 레이즈 

			} break; 
		default:
			{ 
				m_nTotalRaise = nUser*1; 

			} break; 
	}
	

/*	
	m_nRound++;
	if(m_nRound>=4){
		m_nTotalRaise = nUser*2; // 히든은 2회
		if(m_nStyle==2){
			m_nTotalRaise = nUser*3; // 도신방은 히든 3회
		}
	}
*/
}



// 레이즈 명령을 준다.(레이즈마지막명령: 0:다이,1:체크,2:콜,3:삥,4:따당,5:맥스)
// 투루면 레이즈 진행. 폴스면 레이즈 끝이며 다음 카드를 돌리던가, 게임 오바다.
BOOL CRaise::RaiseCommand(int nNum, int nUNum, enum ERAISESIGN sign, 
						  BOOL bCheck,int nBtn, int &nNext,BOOL &bNextBtnBat,BOOL &bNextBtnCheck)
{
	if(!AnalNum(nNum)) return FALSE; // 레이즈 끝내라
	bNextBtnCheck = bCheck; //레이즈 한번없이 선이 죽었을 경우.(콜을 대신한다.)

	// 사용자가 다이하였다.
	if(sign == ERAISE_FOLD){
		if(m_User[nNum].nUNum == nUNum){
			m_User[nNum].nLastCmd = 1;     // 기억된 명령
			m_User[nNum].bFold = TRUE;
			m_User[nNum].nUse++;		   // 첫턴 폴드

			if(m_nRound>=RAISE_END) 
			{
				if(m_User[nNum].nUse==1){
					m_nTotalRaise -= 2;
				}
				
				if(m_User[nNum].nUse==2)
						m_nTotalRaise -= 1;			
			}
			else
			{
				// [ ###베팅룰### ]
				switch(m_nStyle)
				{
					case 0:  // 자유
					case 1:  // 초보
						{// 1 - 2 - 2 - 2
							if(m_nRound>=RAISE_4)
							{
								if(m_User[nNum].nUse==1)
									m_nTotalRaise -= 2;
								else if(m_User[nNum].nUse==2)
									m_nTotalRaise -= 1;
							}
							else
							{
								if(m_User[nNum].nUse==1)
									m_nTotalRaise -= 1;

							}
							
						} break;
					case 2: // 중수
						{
							// 1 - 2 - 2 - 2
							if(m_nRound>=RAISE_4)
							{
								if(m_User[nNum].nUse==1)
									m_nTotalRaise -= 2;
								else if(m_User[nNum].nUse==2)
									m_nTotalRaise -= 1;
							}
							else
							{
								if(m_User[nNum].nUse==1)
									m_nTotalRaise -= 1;
							}
						} break;
					case 3: // 고수
						{
							// 1 - 2 - 2 - 2
							if(m_nRound>=RAISE_4)
							{
								if(m_User[nNum].nUse==1)
									m_nTotalRaise -= 2;
								else if(m_User[nNum].nUse==2)
									m_nTotalRaise -= 1;
							}
							else
							{
								if(m_User[nNum].nUse==1)
									m_nTotalRaise -= 1;
							}
						} break;

					case 4: // 도신
						{
							// 1 - 2 - 2 - 2
							if(m_nRound>=RAISE_4)
							{
								if(m_User[nNum].nUse==1)
									m_nTotalRaise -= 2;
								else if(m_User[nNum].nUse==2)
									m_nTotalRaise -= 1;
							}
							else
							{
								if(m_User[nNum].nUse==1)
									m_nTotalRaise -= 1;
							}
						} break;
				}
			}

			if(m_nTotalRaise<=0) m_nTotalRaise=0;
		}
	}
	// 사용자가 배팅하였다. (하프 || 따당 || 삥)
	else if(sign == ERAISE_BAT){
		m_nTotalRaise--; // 레이즈 감소
		if(m_nTotalRaise<0) m_nTotalRaise=0;

		INT64 nCalcBat  = 0;
		INT64 nTotalBat = GetTotalBat();
		if(nBtn==0){ // 삥배팅
			nCalcBat = (INT64)BetMoney;//100;
			m_User[nNum].nLastCmd = 4;     // 기억된 명령
		}
		if(nBtn==1){ // 따당배팅
			nCalcBat = m_nDouble;
			m_User[nNum].nLastCmd = 5;     // 기억된 명령
		}
		if(nBtn==2 || nBtn==3){ // 최대배팅
			nCalcBat = (INT64)(nTotalBat*m_fBatRate);
			m_User[nNum].nLastCmd = 6;     // 기억된 명령
		}
		if(nBtn == 4){//올인
			
			nCalcBat = m_User[nNum].nPMoney;
			m_User[nNum].nLastCmd = 7;     // 기억된 명령	
		}
		
		m_nDouble = nCalcBat;

		if(m_User[nNum].nUNum == nUNum){
			// 먼저 콜금액 산출
			INT64 nCall = GetRaiseBat() - m_User[nNum].nNowBat;
			/*
			/////////////////////////////////////////////////////////////
			//  새로운 배팅 (판돈 + 받고+ 더 /2 :하프)
			if(nBtn==2 || nBtn==3){ // 최대배팅
				nCalcBat = (INT64)((nTotalBat+nCall)*m_fBatRate);
				m_nDouble = nCalcBat;
			}
			/////////////////////////////////////////////////////////////
			*/

			// 사용자 배팅 결정(받고+더)
			m_User[nNum].nTotBat += (nCall+nCalcBat);			

			// ### [ 인포 메세지 ] ###
			m_User[nNum].nReCall =  nCall;
			m_User[nNum].nCalcBat = nCalcBat;


			m_User[nNum].nNowBat += (nCall+nCalcBat);
			m_User[nNum].nUse++;
			m_User[nNum].nPMoney -= (nCall+nCalcBat);

			// 실제배팅 환산 #####
			INT64 nSideMoney = 0;
			if(m_User[nNum].nPMoney<0) {
				nSideMoney = m_User[nNum].nPMoney;
			}
			m_User[nNum].nRealBat = m_User[nNum].nTotBat+nSideMoney;

		}
	}
	// 사용자가 콜이나 체크를 하였다.
	// 배팅액은 최고 배팅액을 따라간다.
	else if(sign == ERAISE_CALL || sign == ERAISE_CHECK){
		if(m_User[nNum].nUNum == nUNum){

			// [ ###베팅룰### ]
			// ###############################################
			// #### [ 테스트 ] #### 레이즈 승계 하지 않는다.
			m_nTotalRaise--; // 레이즈 감소
			if(m_nTotalRaise<0) m_nTotalRaise=0;
			// ###############################################


			// 먼저 콜금액 산출
			INT64 nCall = GetRaiseBat() - m_User[nNum].nNowBat;
			// 사용자 배팅 결정(받는다)
			m_User[nNum].nTotBat += nCall;
			m_User[nNum].nNowBat += nCall;
			m_User[nNum].nUse++;
			m_User[nNum].bCall    = TRUE; // 이제 레이즈 기회 없다

			// ### [ 인포 메세지 ] ###
			m_User[nNum].nReCall =  nCall;
			m_User[nNum].nCalcBat = 0;


			// 사용자 마블 환산 #######################
			m_User[nNum].nPMoney -= (nCall);
			
			// 실제배팅 환산 #####
			INT64 nSideMoney = 0;
			if(m_User[nNum].nPMoney < 0) {
				nSideMoney = m_User[nNum].nPMoney;
			}
			
			m_User[nNum].nRealBat = m_User[nNum].nTotBat+nSideMoney;

			if(sign==ERAISE_CHECK)
				m_User[nNum].nLastCmd = 2;     // 기억된 명령
			if(sign==ERAISE_CALL)
				m_User[nNum].nLastCmd = 3;     // 기억된 명령
		}
	}

	// 먼저 계속 레이즈를 진행시켜야하는지 여부를 판단하자.
	// 한바퀴 이상 실행되었고, 모든사용자가 같은 금액으로 배팅완료 되었다면..

	int i = 0, nActive =0, nNotUseNum = 0, nNotSameBat = 0;
	for(i=0; i<RAISE_USER; i++){
		if(m_User[i].bInUse && !m_User[i].bFold){
			nActive++;
			if(m_User[i].nUse == 0) // 아직한번이라도 안한사람이 있다면 레이즈는 계속이다.
				nNotUseNum++;
			if(m_User[i].nNowBat < GetRaiseBat()) // 한명이라도 기본 배팅에 모자라면 레이즈 지속.
				nNotSameBat++;
		}
	}
	// 한명남았다면 레이즈 끝이다.
	if(nActive==1) return FALSE;
	// 전부 다돌고 배팅이 완료되었다면, 레이즈 끝이다
	if(nNotUseNum == 0 && nNotSameBat ==0) return FALSE;

	// 그렇지 않다면 레이즈는 계속 되어야 한다.

	// 다음 사용자를 구해 세팅하자.
	int  nIndex = 0, nFind = 0;
	BOOL bFind = FALSE;
	for(i=nNum+1; i<RAISE_USER+nNum; i++){
		nIndex = i;
		if(nIndex>=RAISE_USER) nIndex = i-RAISE_USER;
		if(m_User[nIndex].bInUse && !m_User[nIndex].bFold){
			bFind = TRUE; 
			nFind = nIndex;
			break;
		}
	}
	if(bFind){ // 세컨드 사용자 찾았다. 그사람을 세팅하자.
		bNextBtnBat = FALSE;
		if(m_nTotalRaise>0){
			if(!m_User[nFind].bCall) bNextBtnBat = TRUE;
			if(m_nRound>=4){
				if(m_User[nFind].nUse>4) bNextBtnBat = FALSE;
			}
			else{
				if(m_User[nFind].nUse>2) bNextBtnBat = FALSE;
			}
		}
		nNext = nFind;
	}
	else{ // 없을 수 있나, 레이즈 끝이다
		return FALSE;
	}
	return TRUE;
}


// 라운드를 구한다.
int CRaise::GetRound()
{
	return m_nRound;
}

// 총금액을 구한다.
INT64 CRaise::GetTotalBat()
{
	INT64 nTotalBat = 0;

	for(int i=0;i<RAISE_USER;i++)
		if(m_User[i].bInUse) nTotalBat+= m_User[i].nTotBat;

	return nTotalBat;
}

// 실배팅 머니
INT64 CRaise::GetRealBet()
{
	INT64 nRealBet = 0;

	for(int i=0;i<RAISE_USER;i++)
		if(m_User[i].bInUse) nRealBet+= m_User[i].nRealBat;

	return nRealBet;
}

// 게임결과시의 위너가 가지고 갈수 있는 금액을 산출한다
INT64 CRaise::GetWinnerMoney(int nWinner)
{
	if(!AnalNum(nWinner)) return 0;
	if(!m_User[nWinner].bInUse || m_User[nWinner].bFold) return 0;

	INT64 nTotalBat  = 0;
	INT64 nWinnerBet = GetWinnerBet(nWinner); // 현재 승자의 총배팅액(실배팅액)을 구한다

	// 각각의 실배팅을 계산한후...
	// 승리자의 실배팅보다 작으면 합산한다 (모두 준다)
	// 승리자의 실배팅보다 많으면 승리자의 실배팅만큼만 합산한다 (승리자가 올인 베팅)
	for(int i=0;i<RAISE_USER;i++){
		if(m_User[i].bInUse){
			if(m_User[i].nRealBat<=nWinnerBet){
				nTotalBat+= m_User[i].nRealBat;
			}
			else if(m_User[i].nRealBat>nWinnerBet){
				nTotalBat+= nWinnerBet;
			}
		}
	}

	return nTotalBat; // 승리자가 베팅 한만큼 가져간다
}

// 패배자의 포커머니를 구한다
INT64 CRaise::GetFailMoney(int nWinner, int nUser)
{
	if(nWinner==nUser)    return 0;
	if(!AnalNum(nWinner)) return 0;
	if(!AnalNum(nUser))   return 0;

	if(!m_User[nWinner].bInUse) return 0;
	if(!m_User[nUser].bInUse)   return 0;

	INT64 nFailMoney = 0;
	INT64 nWinnerBet = GetWinnerBet(nWinner);

	// 승리자의 실배팅보다 많으면 그차액만큼은 실패한 사람에게 돌려주어야한다(올인처리)
	if(m_User[nUser].nTotBat>nWinnerBet)
		nFailMoney = m_User[nUser].nRealBat-nWinnerBet;

	return nFailMoney;
}

INT64 CRaise::GetFailMoneyLow(int nHighWin, int nLowWin, int nUser)
{
	if(nHighWin==nUser)    return 0;
	if(!AnalNum(nHighWin)) return 0;
	if(!AnalNum(nUser))   return 0;

	if(!m_User[nHighWin].bInUse) return 0;
	if(!m_User[nUser].bInUse)   return 0;

	if(nLowWin==nUser)    return 0;
	if(!AnalNum(nLowWin)) return 0;
	if(!m_User[nLowWin].bInUse) return 0;


	INT64 nFailMoney = 0;
	INT64 nFailMoneyLow = 0;
	INT64 nWinnerBet    = GetWinnerBet(nHighWin)/2;
	INT64 nWinnerBetLow = GetWinnerBet(nLowWin)/2;

	// 승리자의 실배팅보다 많으면 그차액만큼은 실패한 사람에게 돌려주어야한다
	INT64 divid = m_User[nUser].nTotBat / 2;
	if(divid > nWinnerBet)
		nFailMoney = divid - nWinnerBet;
	
	if(divid > nWinnerBetLow)
		nFailMoneyLow = divid - nWinnerBetLow;

	nFailMoney += nFailMoneyLow;
	return nFailMoney;
}


// 승리자의 사이드머니를 제한 총배팅액(실배팅액)을 구한다
INT64 CRaise::GetWinnerBet(int nWinner)
{
	if(!AnalNum(nWinner)) return 0;

	if(!m_User[nWinner].bInUse || m_User[nWinner].bFold) return 0;

	return m_User[nWinner].nRealBat;
}

// 레이즈금액의 맥시멈을 구한다.
INT64 CRaise::GetRaiseBat()
{
	INT64 nRaiseBat = 0;

	for(int i=0;i<RAISE_USER;i++)
		nRaiseBat = Max(nRaiseBat,m_User[i].nNowBat);

	return nRaiseBat;
}

// 사용자 번호의 에러코드 감지
BOOL CRaise::AnalNum(int nNum)
{
	if(nNum<0 || nNum>=RAISE_USER) return FALSE;
	return TRUE;
}

BOOL CRaise::EndRaise()
{
	// ###[8포커] ### 
	//if(m_nRound>=5){
	if(m_nRound>=RAISE_END){
		int i = 0, nActive =0, nNotUseNum = 0, nNotSameBat = 0;
		for(i=0; i<RAISE_USER; i++){
			if(m_User[i].bInUse && !m_User[i].bFold){
				nActive++;
				if(m_User[i].nUse == 0) // 아직한번이라도 안한사람이 있다면 레이즈는 계속이다.
					nNotUseNum++;
				if(m_User[i].nNowBat < GetRaiseBat()) // 한명이라도 기본 배팅에 모자라면 레이즈 지속.
					nNotSameBat++;
			}
		}
		// 한명남았다면 레이즈 끝이다.
		if(nActive==1) return TRUE;
		// 전부 다돌고 배팅이 완료되었다면, 레이즈 끝이다
		if(nNotUseNum == 0 && nNotSameBat ==0) return TRUE;
	}
	return FALSE;
}


INT64 CRaise::GetWinnerHiLowMoney(BOOL bHigh, int nWinner, int nLowWinner)
{
	if(!AnalNum(nWinner)) return 0;
	if(!AnalNum(nLowWinner)) return 0;

	if(!m_User[nWinner].bInUse || m_User[nWinner].bFold) return 0;
	if(!m_User[nLowWinner].bInUse || m_User[nLowWinner].bFold) return 0;

	INT64 nTotalBat  = 0;
	INT64 nWinnerBet = GetWinnerBet(nWinner) / 2; // 현재 승자의 총배팅액(실배팅액)을 구한다
	INT64 nWinnerLowBet = GetWinnerBet(nLowWinner) / 2;
	
	if( bHigh )
	{

		for(int i=0;i<RAISE_USER;i++)
		{
			if(m_User[i].bInUse)
			{
				INT64 divid = m_User[i].nRealBat/2;
				if(divid <= nWinnerBet)
				{
					nTotalBat+= divid;
				}
				else if(divid > nWinnerBet) // 나머지는 회수금
				{
					nTotalBat+= nWinnerBet;
				}
			}
		}
	}
	else
	{
		for(int i=0;i<RAISE_USER;i++)
		{
			if(m_User[i].bInUse)
			{
				INT64 divid = m_User[i].nRealBat/2;
				if(divid <= nWinnerLowBet)
				{
					nTotalBat+= divid;
				}
				else if(divid > nWinnerLowBet) // 나머지는 회수금
				{
					nTotalBat+= nWinnerLowBet;
				}
			}
		}

	}


	// 승자 2명에 관한 .............
	if(bHigh)
	{
		if(nWinnerBet > nWinnerLowBet) // 로우승자가 적게 베팅했다면 가져온다.
			nTotalBat += (nWinnerBet-nWinnerLowBet); // 회수금을 더한다.
	//	else
	//		nTotalBat += nWinnerBet;

	}
	else
	{
		if(nWinnerLowBet > nWinnerBet)
			nTotalBat += (nWinnerLowBet-nWinnerBet); // 회수금을 더한다.
	//	else
	//		nTotalBat += nWinnerLowBet;

	}
	
	return nTotalBat; // 승리자가 베팅 한만큼 가져간다
}
