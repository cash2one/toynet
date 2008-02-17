// PlayScr.cpp: implementation of the CPlayScr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "PlayScr.h"

#include "Global.h"
#include "ResultHi.h"


#include "MyVoteDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayScr::CPlayScr()
{
	pParent = NULL;
	pPage = NULL;
	PNum = 0;
	Reset();
}

CPlayScr::~CPlayScr()
{

}

void CPlayScr::Init(CWnd *pWnd, CPage *ppage, int pnum)
{

	pParent = pWnd;
	pPage = ppage;
	PNum = pnum;

	switch(pnum)
	{
	case 0:
		m_xp[0]=254;			m_yp[0]=414;
		m_rxp[0]=m_xp[0]+104;   m_ryp[0]=m_yp[0]+78;	
		m_mgx[0]=320;			m_mgy[0]=404;
		m_msgx[0]=339;			m_msgy[0]=423;		//Ÿ�ӹ�
	//	m_mgx[0]=322;			m_mgy[0]=396;		//���� (�� äũ ��)
	//	m_msgx[0]=358;			m_msgy[0]=424;		//Ÿ�ӹ�

		//7��
		m_xp[1]=257;			m_yp[1]=422;
		//m_rxp[1]=m_xp[1]+104;   m_ryp[1]=m_yp[1]+78;
		m_rxp[1]=300;   m_ryp[1]=300;
		m_mgx[1]=322;			m_mgy[1]=400;
		m_msgx[1]=358;			m_msgy[1]=428;


		m_AllinXp[0] = 353;			// ���� �ִ� ��ǥ ���� ��ǥ
		m_AllinYp[0] = 489;			// ���� �ִ� ��ǥ	

		m_AllinXp[1] = 353;			// ���� �ִ� ��ǥ �� ��ǥ
		m_AllinYp[1] = 263;			// ���� �ִ� ��ǥ	

	//	m_AllinXp[2] = 353;			// ���� �۾�
	//	m_AllinYp[2] = 489;			// ���� �۾�

		break;
	case 1:						
	case 2:
	case 3:
	case 4:
	case 5:
		m_xp[0]=0;				m_yp[0]=244;
		m_rxp[0]=m_xp[0]+86;   m_ryp[0]=m_yp[0]+48;	
		m_mgx[0]=400;			m_mgy[0]=152;				// Ÿ�ӹ� ��ǥ ���� - jeong
		m_msgx[0]=277;			m_msgy[0]=162;
		//	m_mgx[0]=223;			m_mgy[0]=354;
		//  m_msgx[0]=85;			m_msgy[0]=373;
		
		//6��
		m_xp[1]=0;				m_yp[1]=252;
		m_rxp[1]=m_xp[1]+86;   m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=223;			m_mgy[1]=358;
		m_msgx[1]=85;			m_msgy[1]=383;
		
		m_AllinXp[0] = 95;			// ���� �ִ� ��ǥ
		m_AllinYp[0] = 289;			// ���� �ִ� ��ǥ	
		
		m_AllinXp[1] = 276;			// ���� �ִ� ��ǥ �� ��ǥ
		m_AllinYp[1] = 244;			// ���� �ִ� ��ǥ	
		
		//	m_AllinXp[2] = 353;			// ���� �۾�
		//	m_AllinYp[2] = 489;			// ���� �۾�
		break;
	/*
	// ����
	case 1:
		m_xp[0]=0;				m_yp[0]=244;
		m_rxp[0]=m_xp[0]+86;   m_ryp[0]=m_yp[0]+48;	
		m_mgx[0]=211;			m_mgy[0]=354;
		m_msgx[0]=80;			m_msgy[0]=375;
	//	m_mgx[0]=223;			m_mgy[0]=354;
	//  m_msgx[0]=85;			m_msgy[0]=373;

		//6��
		m_xp[1]=0;				m_yp[1]=252;
		m_rxp[1]=m_xp[1]+86;   m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=223;			m_mgy[1]=358;
		m_msgx[1]=85;			m_msgy[1]=383;

		m_AllinXp[0] = 95;			// ���� �ִ� ��ǥ
		m_AllinYp[0] = 289;			// ���� �ִ� ��ǥ	

		m_AllinXp[1] = 276;			// ���� �ִ� ��ǥ �� ��ǥ
		m_AllinYp[1] = 244;			// ���� �ִ� ��ǥ	

	//	m_AllinXp[2] = 353;			// ���� �۾�
	//	m_AllinYp[2] = 489;			// ���� �۾�

		break;
	
	case 2:
		m_xp[0]=0;				m_yp[0]=75;
		m_rxp[0]=m_xp[0]+86;   m_ryp[0]=m_yp[0]+48;
	
		m_mgx[0]=211;			m_mgy[0]=187;
		m_msgx[0]=80;			m_msgy[0]=207;
	//	m_mgx[0]=223;			m_mgy[0]=184;
	//	m_msgx[0]=85;			m_msgy[0]=205;

		//6��
		m_xp[1]=0;				m_yp[1]=77;
		m_rxp[1]=m_xp[1]+86;   m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=223;			m_mgy[1]=184;
		m_msgx[1]=85;			m_msgy[1]=208;

		m_AllinXp[0] = 95;			// ���� �ִ� ��ǥ
		m_AllinYp[0] = 122;			// ���� �ִ� ��ǥ	

		m_AllinXp[1] = 276;			// ���� �ִ� ��ǥ �� ��ǥ
		m_AllinYp[1] = 141;			// ���� �ִ� ��ǥ	

	//	m_AllinXp[2] = 353;			// ���� �۾�
	//	m_AllinYp[2] = 489;			// ���� �۾�
		break;
	
	case 3:
		m_xp[0]=537;			m_yp[0]=75;
		m_rxp[0]=m_xp[0]+5;    m_ryp[0]=m_yp[0]+48;
		m_mgx[0]=538;			m_mgy[0]=187;	
		m_msgx[0]=560;			m_msgy[0]=207;
	//	m_mgx[0]=515;			m_mgy[0]=184;
	//	m_msgx[0]=543;			m_msgy[0]=205;

		//6��
		m_xp[1]=269;			m_yp[1]=27;
		m_rxp[1]=m_xp[1]+86;    m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=322;			m_mgy[1]=150;
		m_msgx[1]=350;			m_msgy[1]=155;

		m_AllinXp[0] = 579;			// ���� �ִ� ��ǥ
		m_AllinYp[0] = 122;			// ���� �ִ� ��ǥ	

		m_AllinXp[1] = 414;			// ���� �ִ� ��ǥ �� ��ǥ
		m_AllinYp[1] = 141;			// ���� �ִ� ��ǥ	

	//	m_AllinXp[2] = 353;			// ���� �۾�
	//	m_AllinYp[2] = 489;			// ���� �۾�
		break;

	case 4:
		m_xp[0]=537;			m_yp[0]=244;
		m_rxp[0]=m_xp[0]+5;    m_ryp[0]=m_yp[0]+48;
		m_mgx[0]=538;			m_mgy[0]=354;	
		m_msgx[0]=560;			m_msgy[0]=375;
	//	m_mgx[0]=515;			m_mgy[0]=354;
	//	m_msgx[0]=543;			m_msgy[0]=373;

		//7��
		m_xp[1]=536;			m_yp[1]=77;
		m_rxp[1]=m_xp[1]+5;    m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=515;			m_mgy[1]=184;
		m_msgx[1]=543;			m_msgy[1]=208;

		m_AllinXp[0] = 579;			// ���� �ִ� ��ǥ
		m_AllinYp[0] = 289;			// ���� �ִ� ��ǥ	

		m_AllinXp[1] = 414;			// ���� �ִ� ��ǥ �� ��ǥ
		m_AllinYp[1] = 244;			// ���� �ִ� ��ǥ	

	//	m_AllinXp[2] = 353;			// ���� �۾�
	//	m_AllinYp[2] = 489;			// ���� �۾�
		break;
//////////
	case 5:
	
		//6��
		m_xp[1]=536;			m_yp[1]=252;
		m_rxp[1]=m_xp[1]+5;    m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=515;			m_mgy[1]=358;
		m_msgx[1]=543;			m_msgy[1]=383;

		break;
	*/
	}

	
	//���� �Ӵ� ��ǥ
	m_jackpotx[0] = 778; m_jackpoty[0] = 44;
	m_jackpotx[1] = 782; m_jackpoty[1] = 44;

	// ���� bg ��ǥ
	m_jackpotanix[0] = 553; m_jackpotaniy[0] = 26;
	m_jackpotanix[1] = 553; m_jackpotaniy[1] = 26;
	

	playscr_changeinitpos();
}


void CPlayScr::playscr_changeinitpos()
{
	if(g_RI.bPlayer==0)
	{
		Xp = m_xp[0]; Yp = m_yp[0];			// �÷��̾� ��ũ���� ������ǥ
		RXp = m_rxp[0]; RYp = m_ryp[0];		// ���(��, ��, ����)
		MgX = m_mgx[0]; MgY = m_mgy[0];		// �޼��� �ڽ�
		MsgX = m_msgx[0]; MsgY = m_msgy[0];
		JackPotX = m_jackpotx[0];	JackPotY = m_jackpoty[0];
		JackPotaniX = m_jackpotanix[0]; JackPotaniY = m_jackpotaniy[0];
	}
	else {

		Xp = m_xp[1];   Yp = m_yp[1];		// �÷��̾� ��ũ���� ������ǥ
		RXp = m_rxp[1]; RYp = m_ryp[1];		// ���(��, ��, ����)
		MgX = m_mgx[1]; MgY = m_mgy[1];		// �޼��� �ڽ�
		MsgX = m_msgx[1]; MsgY = m_msgy[1];
		JackPotX = m_jackpotx[1];	JackPotY = m_jackpoty[1];
		JackPotaniX = m_jackpotanix[1]; JackPotaniY = m_jackpotaniy[1];
	}
}


void CPlayScr::Reset()
{
	bGameOver = FALSE;
	PlayCnt = 0;
	PlayStartTick = timeGetTime();
	//m_AllinCnt = 0;			// ���� �ִ� ���� Ÿ�� ���� 
	m_AllinAniStartTime = 0;

	g_CvCard.bAllowControl = FALSE;
}



void CPlayScr::OnTimer()
{
	return;

	if(Play[PNum].JoinState==0) return;
	PlayCnt++;
	if(PNum==0)
	{
	}
	if(bGameOver) return;
}

void CPlayScr::OnGameKey(int nVKey)
{
	if(nVKey == 0x09 ) //VK_TAB
	{
	//	Game.bViewTab  = !Game.bViewTab;
		Game.SetChangeViewTabBtn( !Game.bViewTab);
		return;
	}	
#ifdef _DEBUG
	if(nVKey == VK_F12)
	{
		Sound.Play( SND51 );
		Game.ChangeCard_Init();
	}
#endif
	if(Play[0].JoinState==0) return;
	if(PNum!=0) return;
	if(g_MyObserver == TRUE ) return;// ### [ ������� ] ###
	

	switch(nVKey)
	{
	case VK_LEFT:
	case VK_NUMPAD4:
		{
			if(!g_MyObserver && g_bChangeCardStage == TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0){				 

				if(Play[0].m_KD_SeleteCard <= 0)Play[0].m_KD_SeleteCard = 5;
				else Play[0].m_KD_SeleteCard -= 1;

				POINT pt = Game.CardDeck[0].Card[Play[0].m_KD_SeleteCard].m_move.GetCurPos();
				Game.CardDeck[0].PtInCardRect(pt.x+1,pt.y+1);			
				break;
			}			

			if(Game.m_bchoice_card == TRUE && !g_MyObserver && g_bChangeCardStage != TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0)
			{
				if(Game.m_ncur_selectcard >0) 
				Game.m_ncur_selectcard -= 1;
				
				if(Game.m_ncur_selectcard <= 0 ){
					if(g_RI.FormKind==0) {
						Game.m_ncur_selectcard = 3;
					}
					else{
						Game.m_ncur_selectcard = 4;
					}
				}
				break;
			}

		} break;

	case VK_RIGHT:
	case VK_NUMPAD6:
		{
			if(!g_MyObserver && g_bChangeCardStage == TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0){				 
				
				if(Play[0].m_KD_SeleteCard >= Play[0].nCardTotal)
					Play[0].m_KD_SeleteCard = 0;
				else 
				{
					if(Play[0].m_KD_SeleteCard < Play[0].nCardTotal)
					Play[0].m_KD_SeleteCard += 1;
				}

				POINT pt = Game.CardDeck[0].Card[Play[0].m_KD_SeleteCard].m_move.GetCurPos();
				Game.CardDeck[0].PtInCardRect(pt.x+1,pt.y+1);			
				break;
			}	
			
			if(Game.m_bchoice_card == TRUE && !g_MyObserver && g_bChangeCardStage != TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0)
			{
				if(Game.m_ncur_selectcard >0) 
					Game.m_ncur_selectcard ++;				
				
				if(g_RI.FormKind==0) {
					if(Game.m_ncur_selectcard > 3 ){						
						Game.m_ncur_selectcard = 1;
					}
					else if(Game.m_ncur_selectcard == 0)
					{
						Game.m_ncur_selectcard = 1;
					}
				}
				else 
				{				
					if(Game.m_ncur_selectcard > 4 ){
						Game.m_ncur_selectcard = 1;
					}
					else if(Game.m_ncur_selectcard == 0)
					{
						Game.m_ncur_selectcard = 1;
					}					
				}
				break;
			}

		} break;

	case VK_UP: // ��ư ī�� ���δ�
	case VK_NUMPAD5:
		{
			// ### [ ������� ] ###//����			
			if(!g_MyObserver && !Game.FlyWindCard && Play[0].JoinState != 0 && Play[0].PlayState != 0 && Play[0].nCardTotal == 6 && !Play[0].bHiddenOpen && g_RI.ChangeCardStep == 2)
			{
				
					Sound.Play(SND16);
					Play[0].bHiddenOpen = TRUE; // ī�� Ŭ�������� ���̱� �Ѵ�.
					Game.FlyWindCard = FALSE;
					Game.CardDeck[0].SetFaceUp(5); // ���̱�
					Game.m_bHiddenCardShow = TRUE;
			}				
			
			//[62] 
			if(g_bChangeCardStage == TRUE){
				if(!g_MyObserver && g_bChangeCardStage == TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0){
					if(nVKey == VK_UP){
						g_pGameView->SendMessage(WM_COMMAND,IDM_CHANGE_OK, 0);
						break;
					}		
				}
			}
			
			if(Game.m_bchoice_card == TRUE && !g_MyObserver && g_bChangeCardStage != TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0)
			{
				int xp=0;
				int yp=0;
				switch(Game.m_ncur_selectcard)
				{
				case 1:
					{
						if(g_RI.FormKind==0) {
							xp = 274;
							yp = 328;
						}
						else
						{						
							xp = 248;
							yp = 328;
						}
					}break;
				case 2:
					{
						if(g_RI.FormKind==0) {
							xp = 276+(1*95);
							yp = 328;
						}
						else
						{
							xp = 250+(1*80);
							yp = 328;	
						}
					}break;
				case 3:
					{
						if(g_RI.FormKind==0) {
							xp = 278+(2*95);
							yp = 328;
						}
						else
						{
							xp = 252+(2*80);
							yp = 328;
						}
					}break;
				case 4:
					{
						if(g_RI.FormKind==0) {
							xp = 280+(3*95);
							yp = 328;
						}
						else
						{
							xp = 254+(3*80);
							yp = 328;
						}
					}break;				
				}
				OnLButtonDown(xp,yp);
				break;
			}

		} break;

	case VK_DOWN:
	case VK_NUMPAD2:
		{
			// ### [ ������� ] ###
			
			if(!g_MyObserver && !Game.FlyWindCard && Play[0].JoinState != 0 && Play[0].PlayState != 0 && Play[0].nCardTotal == 6 && !Game.m_bHiddenCardShow && g_RI.ChangeCardStep ==2)
			{
				if(Game.m_nHinddenArrow==0)
				{
					Play[0].bHiddenOpen = TRUE; // ī�� Ŭ�������� ���̱� �Ѵ�.
					Game.FlyWindCard = FALSE;
					Game.CardDeck[0].SetFaceUp(5); // ���̱�
				}

				Game.m_nHinddenArrow += 4;
				if(Game.m_nHinddenArrow>20)
				{
					Sound.Play(SND16);
					Game.m_bHiddenCardShow = TRUE;
				}
			}

			//[62]
			if(g_bChangeCardStage == TRUE){
				if(!g_MyObserver && g_bChangeCardStage == TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0)				
				{
					if(nVKey == VK_DOWN){
						g_pGameView->SendMessage(WM_COMMAND,IDM_CHANGE_CANCEL,0);
						break;
					}			
				}
			}


			if(Game.m_bchoice_card == TRUE && !g_MyObserver && g_bChangeCardStage != TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0)
			{
				int xp=0;
				int yp=0;
				switch(Game.m_ncur_selectcard)
				{
				case 1:
					{
						if(g_RI.FormKind==0) {
							xp = 274;
							yp = 328;
						}
						else
						{						
							xp = 248;
							yp = 328;
						}
					}break;
				case 2:
					{
						if(g_RI.FormKind==0) {
							xp = 276+(1*95);
							yp = 326;
						}
						else
						{
							xp = 250+(1*80);
							yp = 326;	
						}
					}break;
				case 3:
					{
						if(g_RI.FormKind==0) {
							xp = 278+(2*95);
							yp = 328;
						}
						else
						{
							xp = 252+(2*80);
							yp = 328;
						}
					}break;
				case 4:
					{
						if(g_RI.FormKind==0) {
							xp = 280+(3*95);
							yp = 328;
						}
						else
						{
							xp = 254+(3*80);
							yp = 328;
						}
					}break;				
				}
				OnLButtonDown(xp,yp);
				break;
			}
		} break;	

	case 'D':
	case 229:	// �ѱ��� ���
		{

		} break;

	case '1': case '2': case '3': case '4': case '5': case '6':
		{
			// ī�� �ٲٱ� �϶� [62]
			if(!g_MyObserver && g_bChangeCardStage == TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0){				 
				POINT pt = Game.CardDeck[0].Card[nVKey-49].m_move.GetCurPos();
				Game.CardDeck[0].PtInCardRect(pt.x+1,pt.y+1);			
			}

		} break;

	case VK_INSERT: 
	case VK_F2:
		{	
			if(!g_MyObserver && g_bChangeCardStage != TRUE)
			{
				if(g_RI.State == 1 && Game.CurPlayer == Play[0].ServPNum && Play[0].JoinState == 1 && Play[0].PlayState == 1 && !Game.FlyWindCard){
					g_pGameView->OnBtnFold();
				}
			}
		
		} break;

	case VK_HOME: 
	case VK_F3:
		{
			if(!g_MyObserver && g_bChangeCardStage != TRUE){
				if(g_RI.State == 1 && Game.CurPlayer == Play[0].ServPNum && Play[0].JoinState == 1 && Play[0].PlayState == 1 && !Game.FlyWindCard){
					g_pGameView->OnBtnCheck();
				}
			}		
		} break;

	case 33: // VK_PAGE_UP: 
	case VK_F4:
	case VK_F8:
		{
			if(!g_MyObserver && g_bChangeCardStage != TRUE)
			{
				if(g_RI.State == 1 && Game.CurPlayer == Play[0].ServPNum && Play[0].JoinState == 1 && Play[0].PlayState == 1 && !Game.FlyWindCard){
					g_pGameView->OnBtnCall();
				}
				else if( Game.m_bchoice_card ){
					if(g_RI.FormKind==1){
						if(Game.m_discardindex == 0) {
							Game.m_discardindex = 4;
						}
						else {
							if(Game.m_discardindex != 4)
								SelectChoiceCard( 4, Game.m_discardindex);
						}
					}
				}
			}
		//	}
		} break;
	case VK_DELETE: 
	case VK_F5:
		{
			if(!g_MyObserver && g_bChangeCardStage != TRUE)
			{
				if(g_RI.State == 1 && Game.CurPlayer == Play[0].ServPNum && Play[0].JoinState == 1 && Play[0].PlayState == 1 && !Game.FlyWindCard){
					g_pGameView->OnBtnPing();
				}
				else if( Game.m_bchoice_card ){
					if(g_RI.FormKind==0){
						SelectChoiceCard( 1 , 0);
					} 
					else{
						if(Game.m_discardindex == 0) {
							Game.m_discardindex = 1;
						}
						else {
							if(Game.m_discardindex != 1)
								SelectChoiceCard( 1, Game.m_discardindex);
						}
					}
				}
			}
		//	}
		} break;

	case VK_END: 
	case VK_F6:
		{	
			if(!g_MyObserver && g_bChangeCardStage != TRUE){
				if(g_RI.State == 1 && Game.CurPlayer == Play[0].ServPNum && Play[0].JoinState == 1 && Play[0].PlayState == 1 && !Game.FlyWindCard){
					g_pGameView->OnBtnDda();
				}
				else if( Game.m_bchoice_card ){
					if(g_RI.FormKind==0) {
						SelectChoiceCard( 2 , 0);
					} 
					else{
						if(Game.m_discardindex == 0) {
							Game.m_discardindex = 2;
						}
						else {
							if(Game.m_discardindex != 2)
								SelectChoiceCard( 2, Game.m_discardindex);
						}
					}
				}
			}
		//	}
		} break;

	case 34: // VK_PAGE_DOWN: 
	case VK_F7:
		{			
			if(!g_MyObserver && g_bChangeCardStage != TRUE){
				if(g_RI.State == 1 && Game.CurPlayer == Play[0].ServPNum && Play[0].JoinState == 1 && Play[0].PlayState == 1 && !Game.FlyWindCard){
					if(g_pGameView->m_bAllin == TRUE && g_pGameView->m_bBMax == FALSE)g_pGameView->OnAllInBtn();//����
					else g_pGameView->OnBtnMax();
				}
				else if( Game.m_bchoice_card ){
					if(g_RI.FormKind==0) {
						SelectChoiceCard( 3 , 0 );
					} 
					else{
						if(Game.m_discardindex == 0) {
							Game.m_discardindex = 3;
						}
						else {
							if(Game.m_discardindex != 3)
								SelectChoiceCard( 3, Game.m_discardindex);
						}
					}
				}
			}
		//	}
		} break;	

	case VK_SPACE:
		{
			g_pGameView->OnAutoBatBtn();
		}break;
	}
}

BOOL CPlayScr::OnLButtonDown(int xp, int yp)
{
	if(PNum==0)	
	{	
		
		/*
		CMyVoteDlg Dlg;
		Dlg.m_TargetID = "a1";
		Dlg.m_BanishReson = "wotjqtj";
		Dlg.DoModal();
		*/
		if( Game.m_bchoice_card )
		{
			int xSize;
			int ySize;
			int startx;
			int starty;

			int gap = 0;
			
			if(g_RI.bPlayer==0)	{
				
				if(g_RI.FormKind==0) {
					xSize = 74;
					ySize = 99;
					startx = 270;
					starty = 325;

					gap = 22 ; 
				}
				else
				{   // ���̽� ����
					xSize = 74;
					ySize = 99;
					startx = 246;
					starty = 325;

					gap = 7 ; 
				}
			}
			else {
				if(g_RI.FormKind==0) {
					xSize = 74;
					ySize = 99;
					startx = 278;
					starty = 472;
				}
				else {
					xSize = 59;
					ySize = 80;
					startx = 273;
					starty = 487;
				}
			}
			
			CRect CardRect[4];
			
			CardRect[0].SetRect(startx, starty, startx+xSize, starty+ySize);
			CardRect[1].SetRect(startx+xSize+gap, starty, startx+(xSize*2)+gap, starty+ySize);
			CardRect[2].SetRect(startx+(xSize*2)+gap+gap, starty, startx+(xSize*3)+gap+gap, starty+ySize);
			CardRect[3].SetRect(startx+(xSize*3)+gap+gap+gap, starty, startx+(xSize*4)+gap+gap+gap, starty+ySize);
			
			POINT pt;
			pt.x = xp;
			pt.y = yp;
			
			if(g_RI.FormKind==0) {
				for(int i=0; i<3; i++) {
					if( CardRect[i].PtInRect(pt) ) {
						SelectChoiceCard(i+1, 0); // �����ī�����ġ
						return TRUE;
					}
				}
			}
			else
			{
				if(Game.m_discardindex == 0)
				{
					// ����ī�� ����
					for(int i=0; i<4; i++) {
						if( CardRect[i].PtInRect(pt) ) {
							Game.m_discardindex = i+1;
							return TRUE;
						}
					}
				}
				else {
					// ����ī�� ����
					for(int i=0; i<4; i++) {
						if( Game.m_discardindex-1 != i && CardRect[i].PtInRect(pt) ) {
							SelectChoiceCard(i+1, Game.m_discardindex); // �����ī�����ġ
							return TRUE;
						}
					}
				}
			}
			return TRUE;
		}

		// ī�� �ٲٱ� �϶� [62]
		if(!g_MyObserver && g_bChangeCardStage == TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0){

			if(Game.CardDeck[0].PtInCardRect(xp,yp)){			
				return TRUE;
			}
			
		}		
		// ���� ī�� Ŭ��
		// ### [ ������� ] ###
		if(!g_MyObserver && Play[0].JoinState != 0 && Play[0].PlayState != 0 
			&& Play[0].nCardTotal == 6 && !Play[0].bHiddenOpen && g_RI.ChangeCardStep ==2 && g_bChangeCardStage == FALSE)
		{
			if( Game.CardDeck[0].Card[5].PtInCard(xp, yp) )
			{			
				Game.m_bHiddenClick = TRUE;			
				Game.m_hpos.x = xp;
				Game.m_hpos.y = yp;
				Game.CardDeck[0].SetFaceUp(5); // ���̱�			
				return TRUE;
			}
			Game.m_bHiddenClick = FALSE;
		}
	}
	return FALSE;
}


BOOL CPlayScr::OnLButtonUp(int x, int y)
{
	////////////////////////////
	if(!g_MyObserver && !Game.FlyWindCard && Play[0].JoinState != 0 && Play[0].PlayState != 0 
		&& Play[0].nCardTotal == 6 && !Game.m_bHiddenCardShow && g_RI.ChangeCardStep ==2 )
	{
		if(Game.m_nHinddenArrow > 20)
		{
			if( Game.m_bHiddenClick ) {
				Sound.Play(SND16);
				Play[0].bHiddenOpen = TRUE; // ī�� Ŭ�������� ���̱� �Ѵ�.
				Game.FlyWindCard = FALSE;
				Game.CardDeck[0].SetFaceUp(5); // ���̱�
				Game.m_bHiddenCardShow = TRUE;
				
				Game.m_bHiddenClick = FALSE;
			}
		}
		else if(Game.m_nHinddenArrow >= 0 && Game.m_nHinddenArrow <=3)
		{
			if( Game.m_bHiddenClick ) {
				Sound.Play(SND16);
				Play[0].bHiddenOpen = TRUE; // ī�� Ŭ�������� ���̱� �Ѵ�.
				Game.FlyWindCard = FALSE;
				Game.CardDeck[0].SetFaceUp(5); // ���̱�
				Game.m_bHiddenCardShow = TRUE;
				
				Game.m_bHiddenClick = FALSE;
			}
		}
		else Game.m_nHinddenArrow = 0;
	}
	////////////////////////////
	return FALSE;
}
	

BOOL CPlayScr::OnMouseMove(int xp, int yp)
{
	if(!g_MyObserver && !Game.FlyWindCard && Play[0].JoinState != 0 && Play[0].PlayState != 0 
		&& Play[0].nCardTotal == 6 && !Game.m_bHiddenCardShow && g_RI.ChangeCardStep == 2)
	{
		if( Game.m_bHiddenClick ) {
			if(Game.m_hpos.y < yp) {
				int tmp = yp - Game.m_hpos.y;
				Game.m_nHinddenArrow += tmp;
				Game.m_hpos.y = yp;
			}
			else if(Game.m_hpos.y > yp) {
				int tmp = Game.m_hpos.y - yp;
				Game.m_nHinddenArrow -= tmp;
				if(Game.m_nHinddenArrow <=0) {
					Game.m_nHinddenArrow =0;
					Game.m_bHiddenClick = FALSE;
				}
				Game.m_hpos.y = yp;
			}
		}
	}		
	
	if( Game.m_bchoice_card )
	{
		int xSize;
		int ySize;
		int startx;
		int starty;
		int gap = 0;
		if(g_RI.bPlayer==0)	{
			
			if(g_RI.FormKind==0) {
				xSize = 74;
				ySize = 99;
				startx = 270;
				starty = 325;
				gap = 22 ; 
			}
			else
			{   // ���̽� ����
				xSize = 74;
				ySize = 99;
				startx = 246;
				starty = 325;

				gap = 7; 
			}
		}
		else {
			if(g_RI.FormKind==0) {
				xSize = 74;
				ySize = 99;
				startx = 278;
				starty = 472;
			}
			else {
				xSize = 59;
				ySize = 80;
				startx = 273;
				starty = 487;
			}
		}
		
		CRect CardRect[4];
		
		CardRect[0].SetRect(startx, starty, startx+xSize, starty+ySize);
		CardRect[1].SetRect(startx+xSize+gap, starty, startx+(xSize*2)+gap, starty+ySize);
		CardRect[2].SetRect(startx+(xSize*2)+gap+gap, starty, startx+(xSize*3)+gap+gap, starty+ySize);
		CardRect[3].SetRect(startx+(xSize*3)+gap+gap+gap, starty, startx+(xSize*4)+gap+gap+gap, starty+ySize);
		
		//	
		
		POINT pt;
		pt.x = xp;
		pt.y = yp;

		if(g_RI.FormKind == 0) {
			for(int i=0; i<3; i++)
			{
				if( CardRect[i].PtInRect(pt) ) {
					Game.m_ncur_selectcard = i+1; // 1~3
					SetCursor(g_pGameView->hCurHand);
					return TRUE;
				}
			}
		}
		else
		{
			// ����ī�� ����
			for(int i=0; i<4; i++) {
				if( Game.m_discardindex-1 != i && CardRect[i].PtInRect(pt) ) {
					Game.m_ncur_selectcard = i+1; // 1~4
					SetCursor(g_pGameView->hCurHand);
					return TRUE;
				}
			}
		}
	}
	
//	Game.m_ncur_selectcard = 0;
	return FALSE;
}

void CPlayScr::SelectChoiceCard(int index, int discard)
{
	// �����ī�����ġ
	int aBuf[4] ={0,};
	aBuf[0] = Game.CardDeck[0].GetCardNo(0);
	aBuf[1] = Game.CardDeck[0].GetCardNo(1);
	aBuf[2] = Game.CardDeck[0].GetCardNo(2);
	
	
	if(g_RI.FormKind == 0) {
		int cnt = 0;
		for(int j=0;j<3;j++) {
			if(index!=j+1){
				Game.CardDeck[0].Card[cnt].CardNo = aBuf[j];
				cnt++;
			}
		}

		discard =1;
	}
	else {
		
		aBuf[3] = Game.CardDeck[0].GetCardNo(3);
	
		
		//select = dlg.Select; // ���õ� ī�尪(0~51)
		//index = dlg.Index;   // �ε���
		//DisCard = dlg.DisCard;			 // ���� ī�尪(0~51)
		//DisCardIndex = dlg.DisCardIndex; // ����ī�� �ε���(1~4)

		int select = Game.CardDeck[0].GetCardNo( index-1 );
		
		int i=0;
		if(discard != 4) {
			for( i = discard-1; i < 3; i++) {
				aBuf[i] = aBuf[i+1];
				Game.CardDeck[0].Card[i].CardNo = aBuf[i];
				aBuf[i+1] = 0;
			}
		}
		for(i = 0; i < 3; i++) {
			if(aBuf[i] == select) // ī�尪
			{	
				index = i+1;
				break;
			}
		}
		
		// ���� ���
		int cnt = 0;
		for( i=0;i<3;i++) {
			if(index!=i+1){
				Game.CardDeck[0].Card[cnt].CardNo = aBuf[i];
				cnt++;
			}
		}
		
		//#################################################
		// ī�嵦���� ����
		Game.CardDeck[0].Card[3].Clear();
		Game.CardDeck[0].TotalCardNum -= 1;
		//#################################################
	}

	
	
	Sound.Play( SND16 );
	//������ ���� ������ �׸� �����ش�.
	POKERCLNT pc={0,}; 
	pc.nStyle = 3; // ����
	pc.nBtn   = index; // ����ī�� (�ε��� 1 , 2, 3)
	
	CCL_ASK_SELECTCARD scard;
	scard.Set(g_RI.RoomNum,  Play[0].UI.UNum, Play[0].ServPNum, Play[0].UI.ID, &pc, discard);
	SockMan.SendData(g_MainSrvSID, scard.pData, scard.GetTotalSize());
	
	Game.m_bchoice_card = FALSE;
}

void CPlayScr::DrawChip()
{
	if( Game.m_JackPotActive > 0)
	{
		for(int n=0; n<MAX_CHIP; n++) {
			if(Game.m_Chip[n].bLive) 
			{
				Game.m_Chip[n].Draw();
				Game.m_ChipEx[n].Draw();
			}
		}
	}
}

void CPlayScr::draw_5user(CDC *pDC)
{
	if(g_RI.State == 1 && Play[0].PlayState == 1){
		if(g_bChangeCardStage == TRUE && g_Poker.nState == RSTATE_CHANGECARD && PNum == 0){
			int xp = Game.CardDeck[0].Card[0].Xp;
			int yp = Game.CardDeck[0].Card[0].Yp;

			pDC->TextOut(xp,yp-30,"[1]");
			pDC->TextOut(xp+16,yp-30,"[2]");
			pDC->TextOut(xp+32,yp-30,"[3]");
			pDC->TextOut(xp+48,yp-30,"[4]");
			pDC->TextOut(xp+66,yp-30,"[5]");
			pDC->TextOut(xp+84,yp-30,"[6]");
		}
	}

	if(g_RI.State == 1 || PlayScr[0].bGameOver)	{
		if(Play[PNum].JoinState != 0 )	{// ���� ī�� �׸���.
			if( Play[PNum].bFold )
				Game.CardDeck[PNum].Draw( TRUE );
			else
				Game.CardDeck[PNum].Draw();
		}			
		
		// ###### ī�� ����  ######
		// ### [ ������� ] ###	
		
		if(!g_MyObserver &&  PNum == 0 && Play[0].JoinState == 1 && Play[0].nOpenTotal > 0 && g_Poker.nState != RSTATE_CHANGECARD && !Game.FlyWindCard)
		{ // ���� ī�� ����
			CString str = "";
			str = Game.GetMyName();
			
			if(str!=""){ // ���� ī��� Ŭ���Ŀ� �����ش�.			
				if((g_Poker.nRound<5 && !Play[0].bHiddenOpen && !Game.m_bHiddenCardShow && g_RI.ChangeCardStep < 2) 
					|| (Play[0].bHiddenOpen && Game.m_bHiddenCardShow && !Game.bOpen)) {					
					pDC->SetTextColor(RGB(255,255,0));
					CRect rect;
					rect.SetRect(0,0,160,14);
					rect.OffsetRect(MsgX, Yp+167);
					if( !g_CvCard.bAllowControl )				// ���̱�� ���� �������� ���� - jeong
						pDC->DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);

					if( Game.bViewTab ) {
						int rule = Game.MyHighRule;
						if(rule > 1 && rule < 20)
						{	
							// 2 ~ 13
							if(rule >= 13)
								rule =  0;
							else
								rule = 13 - rule;							
					
							if(rule < 8) pPage->PutSprAuto(10, 430+20*rule, &EtcSpr, rule+4);
							else pPage->PutSprAuto(125, 510+20*(rule-8), &EtcSpr, rule+4);
						}
					}
				}
			}
		}		
	}

	// ### ������ ###
	if(g_RI.State == 1)  {
		
		for(int n=0; n<MAX_CHIP; n++) {//// ### Ĩ �׸��� ###
			if(Game.m_Chip[n].bLive) Game.m_Chip[n].Draw();
		}

		// ### Ÿ�̸� ���� �׸���, ���� ###
		draw_timer_and_soundfx( PNum );
		
		// ### �������� �޼��� (����) ###
		if(Play[PNum].JoinState == 1)
		{
			int nSNum = Play[PNum].ServPNum;
			if(nSNum < 0 || nSNum >= g_Max_Player) return;

			CString str ="";
			CString strTemp = "";
		
			// ### [ ���� â�� �޼��� ] ###
			if(g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < g_Max_Player && Game.CurPlayer != nSNum)
			{
				if( g_Poker.RU[nSNum].nLastCmd>0 ) 
				{				
					CString strTemp1 = "";
					CString strM = "";
					COLORREF nCmdCol = RGB(0,255,0); //RGB(255,255,0); // ������ Į��
					int nLastCmd = g_Poker.RU[nSNum].nLastCmd;
				
					INT64 roundingoff = 0;

					switch(nLastCmd)
					{
						case 1: 
							{
								//strTemp.Format("����");
								nCmdCol = RGB(200,200,200); 
							} break; // ����
						case 2:
							{
								//strTemp.Format("üũ");
								nCmdCol = RGB(0,255,0); 
							}break; // üũ
						case 3: 
							{
								if(g_Poker.RU[nSNum].nReCall <= 0)
									strTemp.Format(""); 
								else
								{
									roundingoff = g_Poker.RU[nSNum].nReCall;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;

									strM = NumberToOrientalString(roundingoff);
									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp.Format("%s", strM); 
									
								}
								nCmdCol = RGB(0,255,0); 
							} break; // ��
						case 4: 
							{
								strTemp.Format("%d��",  g_RI.BetMoney);
												
							}  break; // ��
						case 5:
							{
								roundingoff = g_Poker.RU[nSNum].nReCall;
								if(roundingoff > 100)
									roundingoff = (roundingoff/100)*100;
								strM = NumberToOrientalString(roundingoff);

								strM +=g_StrMan.Get(_T("DEFAULT_UNIT2")); // ��
								strTemp.Format("%s �ް�", strM ); 
							
								roundingoff = g_Poker.RU[nSNum].nCalcBat;
								if(roundingoff > 100)
									roundingoff = (roundingoff/100)*100;

								strM = NumberToOrientalString(roundingoff);
								strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
								strTemp1.Format("%s ��", strM); 
							}  break; // ����
						case 6:
							{
								if(g_Poker.RU[nSNum].nReCall > 0)
								{
									roundingoff = g_Poker.RU[nSNum].nReCall;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;
									strM = NumberToOrientalString(roundingoff);

									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp.Format("%s �ް�", strM ); 
									
									roundingoff = g_Poker.RU[nSNum].nCalcBat;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;
										
									strM = NumberToOrientalString(roundingoff);
									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp1.Format("%s ��", strM); 
								}
								else
								{
									roundingoff = g_Poker.RU[nSNum].nCalcBat;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;

									strM = NumberToOrientalString(roundingoff);

									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp.Format("%s", strM); 
								}

							} break; // �ƽ�
					
					}
					
					const int nShowBetX = MsgX-100;
					const int nShowBetY = 162;
					// ���ðǱݾ� ��ġ ���� - jeong
					if(strTemp!="")
					{						
						pDC->SetTextColor(nCmdCol);
						CRect rect;
						rect.SetRect(0,0,200,14);
																	
						if(PNum == 0) {
							rect.OffsetRect(MsgX + 35+10, MsgY);
							pDC->DrawText(strTemp, &rect, DT_LEFT | DT_WORDBREAK);
						} 
						else 
						{
							rect.OffsetRect(nShowBetX, nShowBetY);
							pDC->DrawText(strTemp, &rect, DT_RIGHT | DT_WORDBREAK);
						}
						/*
						else if(PNum<3) {
							rect.OffsetRect(MsgX+5, MsgY);
							pDC->DrawText(strTemp, &rect, DT_LEFT | DT_WORDBREAK);
						} else {
							rect.OffsetRect(MsgX + 20-61, MsgY);
							pDC->DrawText(strTemp, &rect, DT_RIGHT | DT_WORDBREAK);
						}
						*/
					}

					if(strTemp1!="")
					{				
						pDC->SetTextColor(nCmdCol);
						CRect rect;
						rect.SetRect(0,0,200,14);
																	
						if(PNum == 0) {
							rect.OffsetRect(MsgX + 35+10, MsgY+15);
							pDC->DrawText(strTemp1, &rect, DT_LEFT | DT_WORDBREAK);
						} 
						else {
							rect.OffsetRect(nShowBetX, nShowBetY+15);
							pDC->DrawText(strTemp1, &rect, DT_RIGHT | DT_WORDBREAK);
						}
						/*
						else if(PNum<3) {
							rect.OffsetRect(MsgX+5 , MsgY+15);
							pDC->DrawText(strTemp1, &rect, DT_LEFT | DT_WORDBREAK);
						} else {
							rect.OffsetRect(MsgX + 20-61, MsgY+15);
							pDC->DrawText(strTemp1, &rect, DT_RIGHT | DT_WORDBREAK);
						}
						*/
					}
				}
			}			
			
			// ##### [ �޼��� �ڽ� �׸��� ] #####
			if(g_Poker.PS[nSNum].bOnGame && g_Poker.PS[nSNum].PlayState==1 && 
				Game.CurPlayer != nSNum && g_Poker.RU[nSNum].nLastCmd>0)
			{					
				int nLastCmd = g_Poker.RU[nSNum].nLastCmd-1;
				if(g_Poker.LastPlayer == nSNum)
				{			
					if(nLastCmd==5){ //�ƽ�
						if(g_RI.GameKind==0){ 
							pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, 13); //����
						}else{
							pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd+6); //Ȯ���� �׸���.
						}
					}else{
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd+6); //Ȯ���� �׸���.
					}
				}
				else {
					if(nLastCmd==5){ //�ƽ�
						if(g_RI.GameKind==0)
						{ 
							pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, 12); //����
						}else{
							pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd); //Ȯ���� �׸���.
						}
					}else if(nLastCmd == 6){
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, 13); //���� 
					}
					else{
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd); //Ȯ���� �׸���.
					}
				}
			}
		}
		
		//[62] �� �ٲٱ� �ȳ� 
		if(g_bChangeCardStage == TRUE && g_Poker.nState == RSTATE_CHANGECARD){
			if(g_RI.ChangeCardStep>0){
				if(PNum == 0 ){
					static DWORD flashcnt=0;
					int num =flashcnt+((g_RI.ChangeCardStep-1)*8);
					pPage->PutSprAuto(299,172,&ChangeCardInfoSpr,num);
					if(Game.GameCnt%25 == 0)flashcnt ++; 
					if(flashcnt > 7)flashcnt = 0;	
				}	
			}	
		}
		
		// �ٲٱ� ���� ǥ�� (�ٲ���� �ƴ���)	
		if(g_Poker.nState != RSTATE_STARTGAME  && g_RI.ChangeCardStep > 0 ){
			if(Play[PNum].bChangeWhether > -1 && Play[PNum].PlayState == 1){
				pPage->PutSprAuto(m_AllinXp[0]-13, m_AllinYp[0]+39, &EtcBtnSpr, Play[PNum].bChangeWhether+14); //���� �ִ�
				/*
				if(PNum == 0){					
					pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, Play[PNum].bChangeWhether+14); //Ȯ���� �׸���.;					
				}
				else{
				//	pPage->PutSprAuto(m_AllinXp[0]-13, m_AllinYp[0]+39, &EtcBtnSpr, Play[PNum].bChangeWhether+14); //Ȯ���� �׸���.;					
					if(PNum >= 1 && PNum <=2){
						pPage->PutSprAuto(MgX+10, MgY-92, &EtcBtnSpr, Play[PNum].bChangeWhether+14); //Ȯ���� �׸���.;
					}
					else if(PNum >= 3 && PNum <=4){
						pPage->PutSprAuto(MgX-10, MgY-92, &EtcBtnSpr, Play[PNum].bChangeWhether+14); //Ȯ���� �׸���.;
					}					
				}
				*/
			}		
		}
		
		if(g_RI.ChangeCardStep>0 && g_bChangeCardStage == FALSE){
			if(PNum == 0 ){
				pPage->PutSprAuto(482, 86, &EtcBtnSpr, g_RI.ChangeCardStep+16); //�ٲٱ� Ƚ���� ǥ��
			}	
		}
		
	}	
	

	// ### [ ���� ���� ] ###
	if(bGameOver == TRUE && Play[PNum].JoinState == 1)
	{
		// ### [ ������� ] ###
		if(!Play[PNum].bFold && Game.bOpen)		
		{
			Game.m_bHiddenCardShow = TRUE;
			CString str="";
			str = Game.GetName(PNum);	  // ���� 
			pDC->SetTextColor(RGB(255,255,0));
			
			CRect rect;
			rect.SetRect(0,0,160,14);
			rect.OffsetRect(MsgX+5, MsgY+5);
			if(str!="") {
				pDC->DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);
			}
		}		

		if(Play[PNum].ServPNum == Game.WinnerPNum )
		{ // ��
			if(Play[PNum].ServPNum == Game.WinnerPNum )
			{
				if(PNum == 0){
					pPage->PutSprAuto(RXp-16, RYp+15, &FocusSpr, 5); // ����
				}
				else{
					
					pPage->PutSprAuto(WIN_OFFSET_X, WIN_OFFSET_Y, &FocusSpr, 5); // ����
					
				}		
			} 
		}
		else // ��
		{
			//if(Play[PNum].PlayState == 1)
			//	pPage->PutSprAuto(RXp-9, RYp+65, &FocusSpr, 11); // ��
			// ## �Ļ� ##
			if(Play[PNum].UI.PMoney <= 0)
			{
				if(PNum == 0){
					pPage->PutSprAuto(RXp-16, RYp+15, &FocusSpr, 6); // ����
				}
				else{

					pPage->PutSprAuto(WIN_OFFSET_X, WIN_OFFSET_Y, &FocusSpr, 6); // ����
					
				}			
			}
		}
	}

	// �� �ݾ� ǥ�� ���ش�			- jeong
	if(g_Poker.nState == RSTATE_STARTGAME && Play[0].PlayState == 1 && Game.CurPlayer == Play[0].ServPNum)
	{
		INT64 Callmoney =  Game.GetRaiseBat();
		if(Callmoney > 0 )
		{			
			pDC->SelectObject(g_pGameView->Font2);
			CString callmstr;
			callmstr.Format("�ݱݾ�:%I64d��",Callmoney);
			CRect rt;
			const int nXp = 323;
			const int nYp = 416;
			//rt.SetRect(323,396,206+323,13+396);
			rt.SetRect(nXp,nYp,206+nXp,13+nYp);
			pDC->DrawText(callmstr,&rt,DT_RIGHT);
			pDC->SelectObject(g_pGameView->Font2);
		}				
	}
		
	// ### ȭ��ǥ �׸��� ###
	// ### [ ������� ] ###����
	
	if(!g_MyObserver && PNum == 0 && Play[0].PlayState != 0	&& Play[0].nCardTotal == 6 
		&& !Game.FlyWindCard && g_RI.ChangeCardStep == 2 && Play[0].bChangeWhether == 1)
	{		
		if( !Game.m_bHiddenCardShow)//Game.m_bHinddenArrow == TRUE)
		{
			int hY = 462 + Game.m_nHinddenArrow;
			pPage->PutSprAuto(421, hY, g_pGameView->m_pcarddeckspr, 52);
			
			pDC->SetTextColor(RGB(255,225,255));
			pDC->TextOut(445,447,"���");
		}
	}

	//���� �ִ� ǥ�� �Ѵ� 
	if(Play[PNum].bAllIn == TRUE && m_AllinAniStartTime > 0 && g_RI.State == 1)
	{
		DWORD CurTime = timeGetTime();
		
		CurTime = CurTime - m_AllinAniStartTime;			
		
		if(CurTime < 800)
			pPage->PutSprAuto(m_AllinXp[0], m_AllinYp[0], &AllinAniSpr, 0); //���� ǥ��		
		else if(CurTime > 900 && CurTime <= 1600)
		{
			pPage->PutSprAuto(m_AllinXp[1], m_AllinYp[1], &AllinAniSpr, 0); //���� ǥ��		
		}
		else if(CurTime > 1650)
			if(g_Poker.nState == RSTATE_STARTGAME)
			pPage->PutSprAuto(m_AllinXp[0]-13, m_AllinYp[0]+39, &AllinAniSpr, 1); //���� ǥ��				
		
	}
	
	//// ### Ĩ �׸��� ###
	if( Game.m_JackPotActive > 0)
	{
		DrawChip();

	//	for(int n=0; n<MAX_CHIP; n++) {
	//		if(Game.m_Chip[n].bLive) Game.m_Chip[n].Draw();
	//	}
	}

	// [ ###����### ] 
	if(PNum ==0 || Game.bWindCardEnd) // ī�� ������ ó������ ��� �׷��ش�.
	{
		// [���� ������ �۾�]
		//if(g_RI.bSuperMaster) {
		//	// ���۹��̸� ���۹����� ������
		//	CString str;
		//	pDC->SetTextColor(RGB(255,70,70));
		//	str.Format("[���۹�]");
		//	pDC->TextOut(118, 8, str);
		//}

		draw_jackpot_digit();
	}
	// #### [ ���� ȿ�� ] ### 
	if( Game.m_JackPotActive > 0)
	{
		int time = (Game.GameCnt/14)%12;
		pPage->PutSprAuto(146, 160, &JackPotBackSpr, 1+time);
				
		if(time > 1 ) pDC->TextOut(491, 311, Game.m_jackpot_winnerid);

		INT64 tmp = 0;
		int kk = 0;			
		int addx = 0;
		int count = 0;
		tmp = Game.m_JackPotActive;
		
		if((Game.GameCnt/20)%3 != 0 ) {
			for( int j = 0; j < 20 ; j ++ ) {
				if( tmp <= 0 ) 	{
					pPage->PutSprAuto( 580 - ((j * 19)+(count*18)+19), 343-2, &JackPotBackSpr, 24 ); // $
					break;
				}
				if(kk >= 3)	{
					kk = 0;
					pPage->PutSprAuto( 580 + addx - j * 19, 343, &JackPotBackSpr, 13); // ,
					addx -= 19;
					count++;
				}

				pPage->PutSprAuto( 580+ addx - j * 19, 343, &JackPotBackSpr,14 + (int)(tmp%(INT64)10) );
				tmp = tmp / 10;
				kk++;
			}
		}
	}

	
	/*	
	/////////////////////////////////////////////////////////////
	//�ٲ� ī�� ǥ��
	if(PNum == 0)
	{
		if(Play[PNum].m_ChangeCardNum > -2)
		{			
			CString str="";
			str = Game.GetCardName(Play[PNum].m_ChangeCardNum);
			pDC->SetTextColor(RGB(255,255,0));
			
			CRect rect;
			rect.SetRect(0,0,160,14);
			rect.OffsetRect(MsgX+5,MsgY+5);
			if(str!="") {
				pDC->DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);
			}
		}
	}
	else 
	{
		if(Play[PNum].m_ChangeCardNum > -2)
		{
			int sx = MsgX + ((PNum <= 2) ? -26:17);
			int sy = MsgY + 10;			
			
			CString str="";
			str = Game.GetCardName(Play[PNum].m_ChangeCardNum);
			pDC->SetTextColor(RGB(255,255,0));
			
			CRect rect;
			rect.SetRect(0,0,160,14);
			rect.OffsetRect(sx,sy);
			if(str!="") {
				pDC->DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);
			}
		}
	}
	*/
	
	
}

void CPlayScr::draw_6user(CDC *pDC)
{
//	Game.CardDeck[PNum].Draw();
//  draw_timer_and_soundfx(PNum);

	if(g_RI.State == 1 || PlayScr[0].bGameOver)	{
		if(Play[PNum].JoinState != 0 )	{// ���� ī�� �׸���.
			if( Play[PNum].bFold )
				Game.CardDeck[PNum].Draw( TRUE );
			else
				Game.CardDeck[PNum].Draw();
		}

		// ###### ī�� ����  ######
		// ### [ ������� ] ###
		if(!g_MyObserver &&  PNum == 0 && Play[0].JoinState == 1 && Play[0].nOpenTotal > 0 && !Game.FlyWindCard
			&& !g_CvCard.bAllowControl )	// ���̱�� ���� �������� ���� - jeong
		{ // ���� ī�� ����
			CString str = "";
			str = Game.GetMyName();
			
			if(str!=""){ // ���� ī��� Ŭ���Ŀ� �����ش�.
			//	if(g_Poker.nRound<6){// || (Play[0].bHiddenOpen && Game.m_bHiddenCardShow && !Game.bOpen)) { //&& pG->m_nCardWindDelay==0){//[62
				if(g_Poker.nRound<6 || (Play[0].bHiddenOpen && Game.m_bHiddenCardShow && !Game.bOpen)) {
					pDC->SetTextColor(RGB(255,255,0));
					CRect rect;
					rect.SetRect(0,0,160,14);
					rect.OffsetRect(MsgX, Yp+160);
					pDC->DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);
		
					if( Game.bViewTab ) {
						int rule = Game.MyHighRule;
						if(rule > 1 && rule < 20)
						{	

							// 2 ~ 13
							if(rule >= 13)
								rule =  0;
							else
								rule = 13 - rule;
							if(rule < 8) pPage->PutSprAuto(10, 448+18*rule, &EtcSpr, rule+18);
							else pPage->PutSprAuto(129, 520+18*(rule-8), &EtcSpr, rule+18);
						}
					}
				}
			}
		}
	}

	// ### ������ ###
	if(g_RI.State == 1)  {
		
		for(int n=0; n<MAX_CHIP; n++) {//// ### Ĩ �׸��� ###
			if(Game.m_Chip[n].bLive) Game.m_Chip[n].Draw();
		}


		// ### Ÿ�̸� ���� �׸���, ���� ###
		draw_timer_and_soundfx( PNum );
		
		// ### �������� �޼��� (����) ###
		if(Play[PNum].JoinState == 1)
		{
			int nSNum = Play[PNum].ServPNum;
			if(nSNum < 0 || nSNum >= g_Max_Player) return;

			CString str ="";
			CString strTemp = "";
		
			// ### [ ���� â�� �޼��� ] ###
			if(g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < g_Max_Player && Game.CurPlayer != nSNum)
			{
				if( g_Poker.RU[nSNum].nLastCmd>0 ) 
				{				
					CString strTemp1 = "";
					CString strM = "";
					COLORREF nCmdCol = RGB(0,255,0); //RGB(255,255,0); // ������ Į��
					int nLastCmd = g_Poker.RU[nSNum].nLastCmd;
				
					
					INT64 roundingoff = 0;

					switch(nLastCmd)
					{
						case 1: 
							{
								strTemp.Format("����");
								nCmdCol = RGB(200,200,200); 
							} break; // ����
						case 2:
							{
								strTemp.Format("üũ");
								nCmdCol = RGB(0,255,0); 
							}break; // üũ
						case 3: 
							{
								if(g_Poker.RU[nSNum].nReCall <= 0)
									strTemp.Format("��"); 
								else
								{
									roundingoff = g_Poker.RU[nSNum].nReCall;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;

									strM = NumberToOrientalString(roundingoff);
									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp.Format("%s", strM); 
									
								}
								nCmdCol = RGB(0,255,0); 
							} break; // ��
						case 4: 
							{
								strTemp.Format("%d��",  g_RI.BetMoney);
												
							}  break; // ��
						case 5:
							{
								roundingoff = g_Poker.RU[nSNum].nReCall;
								if(roundingoff > 100)
									roundingoff = (roundingoff/100)*100;
								strM = NumberToOrientalString(roundingoff);

								strM +=g_StrMan.Get(_T("DEFAULT_UNIT2")); // ��
								strTemp.Format("%s �ް�", strM ); 
							
								roundingoff = g_Poker.RU[nSNum].nCalcBat;
								if(roundingoff > 100)
									roundingoff = (roundingoff/100)*100;

								strM = NumberToOrientalString(roundingoff);
								strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
								strTemp1.Format("%s ��", strM); 
							}  break; // ����
						case 6:
							{
								if(g_Poker.RU[nSNum].nReCall > 0)
								{
									roundingoff = g_Poker.RU[nSNum].nReCall;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;
									strM = NumberToOrientalString(roundingoff);

									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp.Format("%s �ް�", strM ); 
									
									roundingoff = g_Poker.RU[nSNum].nCalcBat;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;
										
									strM = NumberToOrientalString(roundingoff);
									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp1.Format("%s ��", strM); 
								}
								else
								{
									roundingoff = g_Poker.RU[nSNum].nCalcBat;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;

									strM = NumberToOrientalString(roundingoff);

									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp.Format("%s", strM); 
								}

							} break; // �ƽ�
					
					}
					
					if(strTemp!="")
					{						
						pDC->SetTextColor(nCmdCol);
						CRect rect;
						rect.SetRect(0,0,200,14);
																	
						if(PNum == 0) {
							rect.OffsetRect(MsgX + 35, MsgY);
							pDC->DrawText(strTemp, &rect, DT_LEFT | DT_WORDBREAK);
						} else if(PNum<3) {
							rect.OffsetRect(MsgX, MsgY);
							pDC->DrawText(strTemp, &rect, DT_LEFT | DT_WORDBREAK);
						} else {
							rect.OffsetRect(MsgX + 35-61, MsgY);
							pDC->DrawText(strTemp, &rect, DT_RIGHT | DT_WORDBREAK);
						}
					}

					if(strTemp1!="")
					{				
						pDC->SetTextColor(nCmdCol);
						CRect rect;
						rect.SetRect(0,0,200,14);
																	
						if(PNum == 0) {
							rect.OffsetRect(MsgX + 35, MsgY+15);
							pDC->DrawText(strTemp1, &rect, DT_LEFT | DT_WORDBREAK);
						} else if(PNum<3) {
							rect.OffsetRect(MsgX , MsgY+15);
							pDC->DrawText(strTemp1, &rect, DT_LEFT | DT_WORDBREAK);
						} else {
							rect.OffsetRect(MsgX + 35-61, MsgY+15);
							pDC->DrawText(strTemp1, &rect, DT_RIGHT | DT_WORDBREAK);
						}
					}
				}
			}

			// ##### [ �޼��� �ڽ� �׸��� ] #####
			if(g_Poker.PS[nSNum].bOnGame && g_Poker.PS[nSNum].PlayState==1 && Game.CurPlayer != nSNum &&
				g_Poker.RU[nSNum].nLastCmd>0)
			{
				
				int nLastCmd = g_Poker.RU[nSNum].nLastCmd-1;
				if(g_Poker.LastPlayer == nSNum)
				{			
					if(nLastCmd==5) //�ƽ�
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, 13); //Ȯ���� �׸���.;
					else
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd+6); //Ȯ���� �׸���.
				}
				else {
					if(nLastCmd==5) //�ƽ�
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr,12);
					else
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd);
				}
			}
		}
	}

	// ### [ ���� ���� ] ###
	if(bGameOver == TRUE && Play[PNum].JoinState == 1)
	{
		// ### [ ������� ] ###
		if( g_MyObserver && PNum==0 ||  !Play[PNum].bFold && Game.bOpen)
		//if((g_MyObserver && PNum==0 && !Play[PNum].bFold && Game.bOpen) || PNum != 0 && !Play[PNum].bFold && Game.bOpen)
		{
			Game.m_bHiddenCardShow = TRUE;
			CString str="";
			str = Game.GetName(PNum);	  // ���� 
			pDC->SetTextColor(RGB(255,255,0));
			
			CRect rect;
			rect.SetRect(0,0,160,14);
			rect.OffsetRect(MsgX+5, MsgY+8);
			if(str!="") {
				pDC->DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);
			}
		}
	

		if(Play[PNum].ServPNum == Game.WinnerPNum )
		{ // ��
			if(Play[PNum].ServPNum == Game.WinnerPNum )
			{
				pPage->PutSprAuto(RXp, RYp+11, &FocusSpr, 6); // ����
			} 
		}
		else // ��
		{
			//if(Play[PNum].PlayState == 1)
			//	pPage->PutSprAuto(RXp-9, RYp+65, &FocusSpr, 11); // ��
			// ## �Ļ� ##
			if(Play[PNum].UI.PMoney <= 0)
			{
				pPage->PutSprAuto(RXp, RYp+11, &FocusSpr, 7); // �Ļ�
			}
		}
	}


	// ### ȭ��ǥ �׸��� ###
	// ### [ ������� ] ###����
	
	if(!g_MyObserver && PNum == 0 && Play[0].PlayState != 0 && Play[0].nCardTotal == 6 
		&& !Game.FlyWindCard && g_RI.ChangeCardStep == 2)
	{		
		if( !Game.m_bHiddenCardShow)//Game.m_bHinddenArrow == TRUE)
		{
			int hY = 464 + Game.m_nHinddenArrow;
			pPage->PutSprAuto(421, hY, g_pGameView->m_pcarddeckspr, 52);
			
			pDC->SetTextColor(RGB(255,225,255));
			pDC->TextOut(445,447,"���");
		}
	}
	

	
	//// ### Ĩ �׸��� ###
	if( Game.m_JackPotActive > 0)
	{
		for(int n=0; n<MAX_CHIP; n++) {
			if(Game.m_Chip[n].bLive) Game.m_Chip[n].Draw();
		}
	}

	// [ ###����### ] 
	if(PNum ==0 || Game.bWindCardEnd) // ī�� ������ ó������ ��� �׷��ش�.
	{
		// [���� ������ �۾�]
		//if(g_RI.bSuperMaster) {
		//	// ���۹��̸� ���۹����� ������
		//	CString str;
		//	pDC->SetTextColor(RGB(255,70,70));
		//	str.Format("[���۹�]");
		//	pDC->TextOut(118, 8, str);
		//}

		draw_jackpot_digit();
	}
	// #### [ ���� ȿ�� ] ### 
	if( Game.m_JackPotActive > 0)
	{
		int time = (Game.GameCnt/14)%12;
		pPage->PutSprAuto(146, 160, &JackPotBackSpr, 1+time);
				
		if(time > 1 ) pDC->TextOut(491, 311, Game.m_jackpot_winnerid);

		INT64 tmp = 0;
		int kk = 0;			
		int addx = 0;
		int count = 0;
		tmp = Game.m_JackPotActive;
		
		if((Game.GameCnt/20)%3 != 0 ) {
			for( int j = 0; j < 20 ; j ++ ) {
				if( tmp <= 0 ) 	{
					pPage->PutSprAuto( 580 - ((j * 19)+(count*18)+19), 343-2, &JackPotBackSpr, 24 ); // $
					break;
				}
				if(kk >= 3)	{
					kk = 0;
					pPage->PutSprAuto( 580 + addx - j * 19, 343, &JackPotBackSpr, 13); // ,
					addx -= 19;
					count++;
				}

				pPage->PutSprAuto( 580+ addx - j * 19, 343, &JackPotBackSpr,14 + (int)(tmp%(INT64)10) );
				tmp = tmp / 10;
				kk++;
			}
		}
	}

}

void CPlayScr::Draw(CDC *pDC)
{
	if(pPage == NULL) return;
	if(PNum < 0 || PNum >= g_Max_Player) return;
	
	//TestDraw(pDC); return;	
	if(g_RI.bPlayer==0)
		draw_5user(pDC);
	else
		draw_6user(pDC);
}

// [ ###����### ] 
void CPlayScr::draw_jackpot_digit()
{		
	/*
	if(PNum==0 && Game.m_JackPotActive > 0) 
		pPage->PutSprAuto(JackPotaniX, JackPotaniY, &JackPotSpr, 12+ Game.GameCnt%2);
	else
		pPage->PutSprAuto(JackPotaniX, JackPotaniY, &JackPotSpr, 12+ (Game.GameCnt/12)%2);


	INT64 minus = g_TmpJackPotMoney - g_JackPotMoney;
	INT64 plus = 0;

	if(g_TmpJackPotMoney > g_JackPotMoney) {
		if(		minus > 10000000000)plus = 987654321;
		else if(minus > 1000000000) plus = 98765431;
		else if(minus > 100000000)  plus = 9876541;
		else if(minus > 10000000)   plus = 987651;
		else if(minus > 1000000)    plus = 98761;
		else if(minus > 100000)	    plus = 9871;
		else if(minus > 10000)	    plus = 981;
		else if(minus > 1000)       plus = 91; 
		else if(minus > 50 )        plus = 9;
		else plus = 1;

		g_JackPotMoney+=plus; // ���� �����ֱ�
	}
	else  if(g_TmpJackPotMoney < g_JackPotMoney) {
		if(		minus < -100000000000000000) plus = 9999999987654321;
		else if(minus < -10000000000000000)  plus = 999999987654321;
		else if(minus < -1000000000000000)   plus = 99999987654321;
		else if(minus < -100000000000000)    plus = 9999987654321;
		else if(minus < -10000000000000)     plus = 999987654321;
		else if(minus < -1000000000000)      plus = 99987654321;
		else if(minus < -100000000000)       plus = 9987654321;
		else if(minus < -10000000000)        plus = 987654321;
		else if(minus < -1000000000)         plus = 98765431;
		else if(minus < -100000000)	         plus = 9876521;
		else if(minus < -10000000)           plus = 987651;
		else if(minus < -1000000)            plus = 98761;
		else if(minus < -100000)	         plus = 9871;
		else if(minus < -10000)	             plus = 981;
		else if(minus < -1000)               plus = 91; 
		else if(minus < -50 )                plus = 9;
		else plus = 1;

		g_JackPotMoney-=plus; // ���� �����ֱ�

		if(PNum==0 && Game.m_JackPotActive > 0) {
			Game.CastChipJackPot(7, plus, 0); 
		}
	}

	//if(g_TmpJackPotMoney > g_JackPotMoney) g_JackPotMoney+=plus; // ���� �����ֱ�
	INT64 tmp = g_JackPotMoney;
	int kk = 0;			
	int addx = 0;
	int count = 0;
	
	for( int j = 0; j < 20 ; j ++ ) {
		if( tmp <= 0 ) 	{
			pPage->PutSprAuto( JackPotX - ((j * 9)+(count*8)+9), JackPotY, &JackPotSpr,11 ); 
			break;
		}
		if(kk >= 3)	{
			kk = 0;
			pPage->PutSprAuto( JackPotX + addx - j * 9, JackPotY, &JackPotSpr, 10 ); // ,
			addx -= 8;
			count++;
		}
		pPage->PutSprAuto( JackPotX + addx - j * 9, JackPotY, &JackPotSpr,(int)(tmp%(INT64)10) );
		tmp = tmp / 10;
		kk++;
	}
	*/
}


// ### Ÿ�̸� ���� �׸���, ���� ###
void CPlayScr::draw_timer_and_soundfx(int PNum)
{
	Game.CurTime = timeGetTime();
	if(g_RI.bPlayer==0) 
	{
		////////////////////////////////////////////////////////////////
		// ī�弱�ý� Ÿ�̸� ���

		if( Play[PNum].m_bSelectCard == TRUE && Play[PNum].PlayState==1 )
		{
			int tcnt = ( Game.CurTime - Game.BackTime ) / 1000;
			int timeLimit = 10; // 3�� 10��

			//[62]
			if(g_bChangeCardStage  == TRUE && g_Poker.nState == RSTATE_CHANGECARD)
			{
				timeLimit = 10; // Changecard 10��				
			}

			// ī�� ���� �ϱ�
			if(PNum == 0 && Game.m_bchoice_card) {
				if(g_RI.FormKind==0) {
				//  pPage->PutSprAuto(253, 417, g_pGameView->m_pgamebackspr, 3);
					pPage->PutSprAuto(229, 271, g_pGameView->m_pgamebackspr, 3,10,10);
					pPage->PutSprAuto(251, 432, g_pGameView->m_pgamebackspr, 16);
					for(int i=0; i<3;i++) { // ī�� 3�� �׸���
						pPage->PutSprAuto(270+(i*95), 324, &BigCardSpr, Game.CardDeck[0].GetCardNo(i) );

					}
					pPage->PutSprAuto(240,271, g_pGameView->m_pgamebackspr, 10);
					if(Game.m_ncur_selectcard > 0) { // ���콺 ����
						pPage->PutSprAuto(267+((Game.m_ncur_selectcard-1)*95), 315, &FocusSpr, 2);
					}
				} 
				else 
				{
				//	pPage->PutSprAuto(253, 417, g_pGameView->m_pgamebackspr, 4);
					pPage->PutSprAuto(229, 271, g_pGameView->m_pgamebackspr, 3,10,10);
					pPage->PutSprAuto(251, 432, g_pGameView->m_pgamebackspr, 17);
					// ī�� 4�� �׸���
					for(int i=0; i<4;i++) {
						if(Game.m_discardindex-1 == i)
							pPage->PutSprAuto(245+(i*79), 324, &BigCardSpr, 52 );
						else
							pPage->PutSprAuto(245+(i*79), 324, &BigCardSpr, Game.CardDeck[0].GetCardNo(i) );
					}
					pPage->PutSprAuto(240,271, g_pGameView->m_pgamebackspr, Game.m_discardindex==0?9:10);
					if(Game.m_ncur_selectcard > 0) { // ���콺 ����
						pPage->PutSprAuto(242+((Game.m_ncur_selectcard-1)*79), 315, &FocusSpr,Game.m_discardindex==0?3:2);
					}
				}				
				
				if( tcnt >= 0 && tcnt <= timeLimit ) 
				{
					memset( Game.ClockFlag, TRUE, sizeof( BOOL ) * 5 );
					if(!Game.ClockFlag[tcnt])
					{
						Game.ClockFlag[tcnt] = TRUE;
						Sound.Play( SND20 );
					}
					
				//	int sx = 339; // �׷��� ������ġ
				//	int sy = 423;

					int sx = 245; // �׷��� ������ġ
					int sy = 302;
					//	pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
					int imagex = sx + 310; //�̹��� ������
					int imagey = sy + 11; 
					imagex -= (31*tcnt);
					pPage->SetTempClip(sx, sy, imagex, imagey);
					pPage->PutSprAuto(sx, sy, &TimerSpr, 3, TEMPCLIP);
				}
			}
			else
			{
				if( tcnt >= 0 && tcnt <= timeLimit ) 
				{
					memset( Game.ClockFlag, TRUE, sizeof( BOOL ) * 5 );
					if(!Game.ClockFlag[tcnt])
					{
						Game.ClockFlag[tcnt] = TRUE;
						Sound.Play( SND20 );
					}

					// �׷��� ������ġ
			//		int sx = MsgX + ((PNum <= 2) ? -26:17);
			//		int sy = MsgY + 10;				

					int sx = MsgX;
					int sy = MsgY;									

					pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
					int imagex = sx + 160; //�̹��� ������
					int imagey = sy + 11; 
					imagex -= (16*tcnt);					
					pPage->SetTempClip(sx, sy, imagex, imagey);
					pPage->PutSprAuto(sx, sy, &TimerSpr, 1, TEMPCLIP);
				}
			}
			return;
		}
		////////////////////////////////////////////////////////////////
		// ��Ÿ�̸�
		if(Play[PNum].PlayState==1 && Game.CurPlayer == Play[PNum].ServPNum && !Game.FlyWindCard  && g_Poker.nState != RSTATE_CHANGECARD ) // ī�� ������ �߿��� �׸��� �ʴ´�.
		{			
			int tcnt = ( Game.CurTime - Game.BackTime ) / 1000;
			if( tcnt >= 0 && tcnt <= 10 ) 
			{
				memset( Game.ClockFlag, TRUE, sizeof( BOOL ) * 7 );
				if(!g_MyObserver && PNum == 0 ) {// ### [ ������� ] ###
					if(!Game.ClockFlag[tcnt])
					{
						Game.ClockFlag[tcnt] = TRUE;
						Sound.Play( SND20 );
					}
				}
				int sx = MsgX;// + ((PNum <= 2) ? -26:17);
				int sy = MsgY;// + 10;			

				if(g_Poker.nState != RSTATE_CHANGECARD)if(PNum == 0)sy += 18;

				pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
				int imagex = sx + 160; //�̹��� ������
				int imagey = sy + 11; 
				imagex -= (16*tcnt);				
				pPage->SetTempClip(sx, sy, imagex, imagey);
				pPage->PutSprAuto(sx, sy, &TimerSpr, 1, TEMPCLIP);
			}
		}		
	}
	else 
	{ //6��

		/////////////////////////////////////////////////////////////////////
		//// ī�弱�ý� Ÿ�̸� ���
		if(Play[PNum].m_bSelectCard == TRUE && Play[PNum].PlayState==1 )
		{
			int tcnt = ( Game.CurTime - Game.BackTime ) / 1000;
			int timeLimit = 7; // 3�� 7��
			int timeNum = 18;
			// ī�� ���� �ϱ�
			if(PNum == 0 && Game.m_bchoice_card) {
				if(g_RI.FormKind==0) {
					pPage->PutSprAuto(261, 416, g_pGameView->m_pgamebackspr, 3);
					for(int i=0; i<3;i++) { // ī�� 3�� �׸���
						pPage->PutSprAuto(278+(i*79), 472, &BigCardSpr, Game.CardDeck[0].GetCardNo(i) );
					}
					pPage->PutSprAuto(271,421, g_pGameView->m_pgamebackspr, 10);
					if(Game.m_ncur_selectcard > 0) { // ���콺 ����
						pPage->PutSprAuto(275+((Game.m_ncur_selectcard-1)*79), 463, &FocusSpr, 3);
					}
				} 
				else 
				{
					pPage->PutSprAuto(261, 416, g_pGameView->m_pgamebackspr, 4);
					// ī�� 4�� �׸���
					for(int i=0; i<4;i++) {
						if(Game.m_discardindex-1 == i)
							pPage->PutSprAuto(273+(i*64), 487, &SmallCardSpr, 52 );
						else
							pPage->PutSprAuto(273+(i*64), 487, &SmallCardSpr, Game.CardDeck[0].GetCardNo(i) );
					}
					pPage->PutSprAuto(271,421, g_pGameView->m_pgamebackspr, Game.m_discardindex==0?9:10);
					if(Game.m_ncur_selectcard > 0) { // ���콺 ����
						pPage->PutSprAuto(270+((Game.m_ncur_selectcard-1)*64), 470, &FocusSpr, Game.m_discardindex==0?4:5);
					}
				}
				
				
				if( tcnt >= 0 && tcnt <= timeLimit ) 
				{
					memset( Game.ClockFlag, TRUE, sizeof( BOOL ) * 7 );
					if(!Game.ClockFlag[tcnt])
					{
						Game.ClockFlag[tcnt] = TRUE;
						Sound.Play( SND20 );
					}
					
					int sx = 272; // �׷��� ������ġ
					int sy = 448;
					//	pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
					int imagex = sx + 253; //�̹��� ������
					int imagey = sy + 13; 
					imagex -= (25.3*tcnt);
					pPage->SetTempClip(sx, sy, imagex, imagey);
					pPage->PutSprAuto(sx, sy, &TimerSpr, 2, TEMPCLIP);
				}
			}
			else
			{
				if( tcnt >= 0 && tcnt <= timeLimit ) 
				{
					memset( Game.ClockFlag, TRUE, sizeof( BOOL ) * 7 );
					if(!Game.ClockFlag[tcnt])
					{
						Game.ClockFlag[tcnt] = TRUE;
						Sound.Play( SND20 );
					}
					
					int sx = MsgX-4; // �׷��� ������ġ
					int sy = MsgY+4;
					pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
					int imagex = sx + 180; //�̹��� ������
					int imagey = sy + 13; 
					imagex -= (18*tcnt);
					pPage->SetTempClip(sx, sy, imagex, imagey);
					pPage->PutSprAuto(sx, sy, &TimerSpr, 1, TEMPCLIP);
				}
			}
			return;
		}
		/////////////////////////////////////////////////////////////////////

		// ��Ÿ�̸�
		if( Play[PNum].PlayState==1 && Game.CurPlayer == Play[PNum].ServPNum && !Game.FlyWindCard ) // ī�� ������ �߿��� �׸��� �ʴ´�.
		{
			
			int tcnt = ( Game.CurTime - Game.BackTime ) / 1000;
			if( tcnt >= 0 && tcnt <= 10 ) 
			{
				memset( Game.ClockFlag, TRUE, sizeof( BOOL ) * 7 );
				if(!g_MyObserver && PNum == 0 ) {// ### [ ������� ] ###
					if(!Game.ClockFlag[tcnt])
					{
						Game.ClockFlag[tcnt] = TRUE;
						Sound.Play( SND20 );
					}
				}
				int sx = MsgX-4; // �׷��� ������ġ
				int sy = MsgY+4;
				pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
				int imagex = sx + 180; //�̹��� ������
				int imagey = sy + 13; 
				imagex -= (18*tcnt);
				pPage->SetTempClip(sx, sy, imagex, imagey);
				pPage->PutSprAuto(sx, sy, &TimerSpr, 1, TEMPCLIP);
			}
		}
	}
}	
