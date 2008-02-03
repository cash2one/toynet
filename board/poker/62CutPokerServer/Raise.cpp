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

// ������ī ������ Ŭ����.

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

// �ʱ�ȭ�Ѵ�
void CRaise::Init(int nBat)
{
	Reset();
}

// ����ڸ� �����Ѵ�(������ ����ڸӴϴ� �����ȴ�)
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

// �ʱ⼳���� �Ѵ�(�������� ����ڸ� ���� �����Ͽ����Ѵ�)
void CRaise::Set(int nRaise,int nBat) // level
{
	if(nBat==0)      m_fBatRate = 0.25; // ���͹�
	else if(nBat==1) m_fBatRate = 0.5;  // ������
///////////	else if(nBat==2) m_fBatRate = 1.0;  // Ǯ��
	else             m_fBatRate = 0.25; // ���͹�

	m_nStyle  = nRaise; // ������ ��Ÿ��
	m_nDouble = 0;
	int nUser = 0;
	for(int i=0; i<RAISE_USER; i++){
		m_User[i].nLastCmd = 0;
		if(m_User[i].bInUse){ // �� ���ӿ� ������ ������̶� �̾߱�
			nUser++;
		}
	}

	// [ ###���÷�### ]
	switch(m_nStyle){//m_nStyle = ( ���� �ʺ� �߼� = 0, ��� = 1 ���� =2)
		case 0:  m_nTotalRaise = nUser*1; break; // 1ȸ ������ ����
		case 1:  m_nTotalRaise = nUser*1; break; // 2ȸ ������ �ʺ�
		case 2:  m_nTotalRaise = nUser*1; break; // 2ȸ ������ �߼�
		case 3:  m_nTotalRaise = nUser*1; break; // 3ȸ ������ ���
		case 4:  m_nTotalRaise = nUser*1; break; // 3ȸ ������ ����
		default: m_nTotalRaise = nUser*1; break;
	}
	m_nRound    = 1; // �����ϸ鼭 4(1+3)������
}



// 1 - 1 - 1 - 2
void CRaise::Start(PLAYSTATE *ps)
{
	if(ps==NULL) return;
	int i=0,nUser = 0;
	m_nDouble     = 0;
	for(i=0; i<RAISE_USER; i++){
		m_User[i].nLastCmd= 0;     // ���� ���
		m_User[i].nNowBat = 0;     // �̹������� ���� �ʱ�ȭ
		m_User[i].nUse    = 0;     // �ڱ��ȸ ��� �ʱ�ȭ
		m_User[i].bCall   = FALSE; // �ݻ��� �ʱ�ȭ
		if(m_User[i].bInUse && ps[i].bOnGame && !ps[i].bFold){ // �� ���ӿ� ������ ������̶� �̾߱�
			nUser++;
		}
	}

	m_nRound++;

	// [ ###���÷�### ]
	switch(m_nStyle){
		case 0:  // ����
		case 1:  // �ʺ�
			{
				// 1 - 2 - 2 - 2
				if(m_nRound>=2) 
					m_nTotalRaise = nUser*2;  // 2ȸ ������
				else
					m_nTotalRaise = nUser*1; // 1ȸ ������ 
			} break; 
		case 2: // �߼�
			{
				// 1 - 2 - 2 - 2
				if(m_nRound>=2) 
					m_nTotalRaise = nUser*2; // ������ 2ȸ
				else
					m_nTotalRaise = nUser*1; // 1ȸ ������ 

			} break; 
		case 3: // ���
			{
				// 1 - 2 - 2 - 2 half
				if(m_nRound>=2) 
					m_nTotalRaise = nUser*2; // ������ 2ȸ
				else
					m_nTotalRaise = nUser*1; // 1ȸ ������ 

			} break; 

		case 4: // ����
			{
				// 1 - 2 - 2 - 2 half
				if(m_nRound>=2) 
					m_nTotalRaise = nUser*2; // ������ 2ȸ
				else
					m_nTotalRaise = nUser*1; // 1ȸ ������ 

			} break; 
		default:
			{ 
				m_nTotalRaise = nUser*1; 

			} break; 
	}
	

/*	
	m_nRound++;
	if(m_nRound>=4){
		m_nTotalRaise = nUser*2; // ������ 2ȸ
		if(m_nStyle==2){
			m_nTotalRaise = nUser*3; // ���Ź��� ���� 3ȸ
		}
	}
*/
}



// ������ ����� �ش�.(������������: 0:����,1:üũ,2:��,3:��,4:����,5:�ƽ�)
// ����� ������ ����. ������ ������ ���̸� ���� ī�带 ��������, ���� ���ٴ�.
BOOL CRaise::RaiseCommand(int nNum, int nUNum, enum ERAISESIGN sign, 
						  BOOL bCheck,int nBtn, int &nNext,BOOL &bNextBtnBat,BOOL &bNextBtnCheck)
{
	if(!AnalNum(nNum)) return FALSE; // ������ ������
	bNextBtnCheck = bCheck; //������ �ѹ����� ���� �׾��� ���.(���� ����Ѵ�.)

	// ����ڰ� �����Ͽ���.
	if(sign == ERAISE_FOLD){
		if(m_User[nNum].nUNum == nUNum){
			m_User[nNum].nLastCmd = 1;     // ���� ���
			m_User[nNum].bFold = TRUE;
			m_User[nNum].nUse++;		   // ù�� ����

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
				// [ ###���÷�### ]
				switch(m_nStyle)
				{
					case 0:  // ����
					case 1:  // �ʺ�
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
					case 2: // �߼�
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
					case 3: // ���
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

					case 4: // ����
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
	// ����ڰ� �����Ͽ���. (���� || ���� || ��)
	else if(sign == ERAISE_BAT){
		m_nTotalRaise--; // ������ ����
		if(m_nTotalRaise<0) m_nTotalRaise=0;

		INT64 nCalcBat  = 0;
		INT64 nTotalBat = GetTotalBat();
		if(nBtn==0){ // �����
			nCalcBat = (INT64)BetMoney;//100;
			m_User[nNum].nLastCmd = 4;     // ���� ���
		}
		if(nBtn==1){ // �������
			nCalcBat = m_nDouble;
			m_User[nNum].nLastCmd = 5;     // ���� ���
		}
		if(nBtn==2 || nBtn==3){ // �ִ����
			nCalcBat = (INT64)(nTotalBat*m_fBatRate);
			m_User[nNum].nLastCmd = 6;     // ���� ���
		}
		if(nBtn == 4){//����
			
			nCalcBat = m_User[nNum].nPMoney;
			m_User[nNum].nLastCmd = 7;     // ���� ���	
		}
		
		m_nDouble = nCalcBat;

		if(m_User[nNum].nUNum == nUNum){
			// ���� �ݱݾ� ����
			INT64 nCall = GetRaiseBat() - m_User[nNum].nNowBat;
			/*
			/////////////////////////////////////////////////////////////
			//  ���ο� ���� (�ǵ� + �ް�+ �� /2 :����)
			if(nBtn==2 || nBtn==3){ // �ִ����
				nCalcBat = (INT64)((nTotalBat+nCall)*m_fBatRate);
				m_nDouble = nCalcBat;
			}
			/////////////////////////////////////////////////////////////
			*/

			// ����� ���� ����(�ް�+��)
			m_User[nNum].nTotBat += (nCall+nCalcBat);			

			// ### [ ���� �޼��� ] ###
			m_User[nNum].nReCall =  nCall;
			m_User[nNum].nCalcBat = nCalcBat;


			m_User[nNum].nNowBat += (nCall+nCalcBat);
			m_User[nNum].nUse++;
			m_User[nNum].nPMoney -= (nCall+nCalcBat);

			// �������� ȯ�� #####
			INT64 nSideMoney = 0;
			if(m_User[nNum].nPMoney<0) {
				nSideMoney = m_User[nNum].nPMoney;
			}
			m_User[nNum].nRealBat = m_User[nNum].nTotBat+nSideMoney;

		}
	}
	// ����ڰ� ���̳� üũ�� �Ͽ���.
	// ���þ��� �ְ� ���þ��� ���󰣴�.
	else if(sign == ERAISE_CALL || sign == ERAISE_CHECK){
		if(m_User[nNum].nUNum == nUNum){

			// [ ###���÷�### ]
			// ###############################################
			// #### [ �׽�Ʈ ] #### ������ �°� ���� �ʴ´�.
			m_nTotalRaise--; // ������ ����
			if(m_nTotalRaise<0) m_nTotalRaise=0;
			// ###############################################


			// ���� �ݱݾ� ����
			INT64 nCall = GetRaiseBat() - m_User[nNum].nNowBat;
			// ����� ���� ����(�޴´�)
			m_User[nNum].nTotBat += nCall;
			m_User[nNum].nNowBat += nCall;
			m_User[nNum].nUse++;
			m_User[nNum].bCall    = TRUE; // ���� ������ ��ȸ ����

			// ### [ ���� �޼��� ] ###
			m_User[nNum].nReCall =  nCall;
			m_User[nNum].nCalcBat = 0;


			// ����� ���� ȯ�� #######################
			m_User[nNum].nPMoney -= (nCall);
			
			// �������� ȯ�� #####
			INT64 nSideMoney = 0;
			if(m_User[nNum].nPMoney < 0) {
				nSideMoney = m_User[nNum].nPMoney;
			}
			
			m_User[nNum].nRealBat = m_User[nNum].nTotBat+nSideMoney;

			if(sign==ERAISE_CHECK)
				m_User[nNum].nLastCmd = 2;     // ���� ���
			if(sign==ERAISE_CALL)
				m_User[nNum].nLastCmd = 3;     // ���� ���
		}
	}

	// ���� ��� ����� ������Ѿ��ϴ��� ���θ� �Ǵ�����.
	// �ѹ��� �̻� ����Ǿ���, ������ڰ� ���� �ݾ����� ���ÿϷ� �Ǿ��ٸ�..

	int i = 0, nActive =0, nNotUseNum = 0, nNotSameBat = 0;
	for(i=0; i<RAISE_USER; i++){
		if(m_User[i].bInUse && !m_User[i].bFold){
			nActive++;
			if(m_User[i].nUse == 0) // �����ѹ��̶� ���ѻ���� �ִٸ� ������� ����̴�.
				nNotUseNum++;
			if(m_User[i].nNowBat < GetRaiseBat()) // �Ѹ��̶� �⺻ ���ÿ� ���ڶ�� ������ ����.
				nNotSameBat++;
		}
	}
	// �Ѹ��Ҵٸ� ������ ���̴�.
	if(nActive==1) return FALSE;
	// ���� �ٵ��� ������ �Ϸ�Ǿ��ٸ�, ������ ���̴�
	if(nNotUseNum == 0 && nNotSameBat ==0) return FALSE;

	// �׷��� �ʴٸ� ������� ��� �Ǿ�� �Ѵ�.

	// ���� ����ڸ� ���� ��������.
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
	if(bFind){ // ������ ����� ã�Ҵ�. �׻���� ��������.
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
	else{ // ���� �� �ֳ�, ������ ���̴�
		return FALSE;
	}
	return TRUE;
}


// ���带 ���Ѵ�.
int CRaise::GetRound()
{
	return m_nRound;
}

// �ѱݾ��� ���Ѵ�.
INT64 CRaise::GetTotalBat()
{
	INT64 nTotalBat = 0;

	for(int i=0;i<RAISE_USER;i++)
		if(m_User[i].bInUse) nTotalBat+= m_User[i].nTotBat;

	return nTotalBat;
}

// �ǹ��� �Ӵ�
INT64 CRaise::GetRealBet()
{
	INT64 nRealBet = 0;

	for(int i=0;i<RAISE_USER;i++)
		if(m_User[i].bInUse) nRealBet+= m_User[i].nRealBat;

	return nRealBet;
}

// ���Ӱ������ ���ʰ� ������ ���� �ִ� �ݾ��� �����Ѵ�
INT64 CRaise::GetWinnerMoney(int nWinner)
{
	if(!AnalNum(nWinner)) return 0;
	if(!m_User[nWinner].bInUse || m_User[nWinner].bFold) return 0;

	INT64 nTotalBat  = 0;
	INT64 nWinnerBet = GetWinnerBet(nWinner); // ���� ������ �ѹ��þ�(�ǹ��þ�)�� ���Ѵ�

	// ������ �ǹ����� �������...
	// �¸����� �ǹ��ú��� ������ �ջ��Ѵ� (��� �ش�)
	// �¸����� �ǹ��ú��� ������ �¸����� �ǹ��ø�ŭ�� �ջ��Ѵ� (�¸��ڰ� ���� ����)
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

	return nTotalBat; // �¸��ڰ� ���� �Ѹ�ŭ ��������
}

// �й����� ��Ŀ�Ӵϸ� ���Ѵ�
INT64 CRaise::GetFailMoney(int nWinner, int nUser)
{
	if(nWinner==nUser)    return 0;
	if(!AnalNum(nWinner)) return 0;
	if(!AnalNum(nUser))   return 0;

	if(!m_User[nWinner].bInUse) return 0;
	if(!m_User[nUser].bInUse)   return 0;

	INT64 nFailMoney = 0;
	INT64 nWinnerBet = GetWinnerBet(nWinner);

	// �¸����� �ǹ��ú��� ������ �����׸�ŭ�� ������ ������� �����־���Ѵ�(����ó��)
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

	// �¸����� �ǹ��ú��� ������ �����׸�ŭ�� ������ ������� �����־���Ѵ�
	INT64 divid = m_User[nUser].nTotBat / 2;
	if(divid > nWinnerBet)
		nFailMoney = divid - nWinnerBet;
	
	if(divid > nWinnerBetLow)
		nFailMoneyLow = divid - nWinnerBetLow;

	nFailMoney += nFailMoneyLow;
	return nFailMoney;
}


// �¸����� ���̵�Ӵϸ� ���� �ѹ��þ�(�ǹ��þ�)�� ���Ѵ�
INT64 CRaise::GetWinnerBet(int nWinner)
{
	if(!AnalNum(nWinner)) return 0;

	if(!m_User[nWinner].bInUse || m_User[nWinner].bFold) return 0;

	return m_User[nWinner].nRealBat;
}

// ������ݾ��� �ƽø��� ���Ѵ�.
INT64 CRaise::GetRaiseBat()
{
	INT64 nRaiseBat = 0;

	for(int i=0;i<RAISE_USER;i++)
		nRaiseBat = Max(nRaiseBat,m_User[i].nNowBat);

	return nRaiseBat;
}

// ����� ��ȣ�� �����ڵ� ����
BOOL CRaise::AnalNum(int nNum)
{
	if(nNum<0 || nNum>=RAISE_USER) return FALSE;
	return TRUE;
}

BOOL CRaise::EndRaise()
{
	// ###[8��Ŀ] ### 
	//if(m_nRound>=5){
	if(m_nRound>=RAISE_END){
		int i = 0, nActive =0, nNotUseNum = 0, nNotSameBat = 0;
		for(i=0; i<RAISE_USER; i++){
			if(m_User[i].bInUse && !m_User[i].bFold){
				nActive++;
				if(m_User[i].nUse == 0) // �����ѹ��̶� ���ѻ���� �ִٸ� ������� ����̴�.
					nNotUseNum++;
				if(m_User[i].nNowBat < GetRaiseBat()) // �Ѹ��̶� �⺻ ���ÿ� ���ڶ�� ������ ����.
					nNotSameBat++;
			}
		}
		// �Ѹ��Ҵٸ� ������ ���̴�.
		if(nActive==1) return TRUE;
		// ���� �ٵ��� ������ �Ϸ�Ǿ��ٸ�, ������ ���̴�
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
	INT64 nWinnerBet = GetWinnerBet(nWinner) / 2; // ���� ������ �ѹ��þ�(�ǹ��þ�)�� ���Ѵ�
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
				else if(divid > nWinnerBet) // �������� ȸ����
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
				else if(divid > nWinnerLowBet) // �������� ȸ����
				{
					nTotalBat+= nWinnerLowBet;
				}
			}
		}

	}


	// ���� 2�� ���� .............
	if(bHigh)
	{
		if(nWinnerBet > nWinnerLowBet) // �ο���ڰ� ���� �����ߴٸ� �����´�.
			nTotalBat += (nWinnerBet-nWinnerLowBet); // ȸ������ ���Ѵ�.
	//	else
	//		nTotalBat += nWinnerBet;

	}
	else
	{
		if(nWinnerLowBet > nWinnerBet)
			nTotalBat += (nWinnerLowBet-nWinnerBet); // ȸ������ ���Ѵ�.
	//	else
	//		nTotalBat += nWinnerLowBet;

	}
	
	return nTotalBat; // �¸��ڰ� ���� �Ѹ�ŭ ��������
}
