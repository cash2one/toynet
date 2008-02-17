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
		m_msgx[0]=339;			m_msgy[0]=423;		//타임바
	//	m_mgx[0]=322;			m_mgy[0]=396;		//배팅 (콜 채크 삥)
	//	m_msgx[0]=358;			m_msgy[0]=424;		//타임바

		//7인
		m_xp[1]=257;			m_yp[1]=422;
		//m_rxp[1]=m_xp[1]+104;   m_ryp[1]=m_yp[1]+78;
		m_rxp[1]=300;   m_ryp[1]=300;
		m_mgx[1]=322;			m_mgy[1]=400;
		m_msgx[1]=358;			m_msgy[1]=428;


		m_AllinXp[0] = 353;			// 올인 애니 좌표 시작 좌표
		m_AllinYp[0] = 489;			// 올인 애니 좌표	

		m_AllinXp[1] = 353;			// 올인 애니 좌표 끝 좌표
		m_AllinYp[1] = 263;			// 올인 애니 좌표	

	//	m_AllinXp[2] = 353;			// 올인 글씨
	//	m_AllinYp[2] = 489;			// 올인 글씨

		break;
	case 1:						
	case 2:
	case 3:
	case 4:
	case 5:
		m_xp[0]=0;				m_yp[0]=244;
		m_rxp[0]=m_xp[0]+86;   m_ryp[0]=m_yp[0]+48;	
		m_mgx[0]=400;			m_mgy[0]=152;				// 타임바 좌표 변경 - jeong
		m_msgx[0]=277;			m_msgy[0]=162;
		//	m_mgx[0]=223;			m_mgy[0]=354;
		//  m_msgx[0]=85;			m_msgy[0]=373;
		
		//6인
		m_xp[1]=0;				m_yp[1]=252;
		m_rxp[1]=m_xp[1]+86;   m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=223;			m_mgy[1]=358;
		m_msgx[1]=85;			m_msgy[1]=383;
		
		m_AllinXp[0] = 95;			// 올인 애니 좌표
		m_AllinYp[0] = 289;			// 올인 애니 좌표	
		
		m_AllinXp[1] = 276;			// 올인 애니 좌표 끝 좌표
		m_AllinYp[1] = 244;			// 올인 애니 좌표	
		
		//	m_AllinXp[2] = 353;			// 올인 글씨
		//	m_AllinYp[2] = 489;			// 올인 글씨
		break;
	/*
	// 좌측
	case 1:
		m_xp[0]=0;				m_yp[0]=244;
		m_rxp[0]=m_xp[0]+86;   m_ryp[0]=m_yp[0]+48;	
		m_mgx[0]=211;			m_mgy[0]=354;
		m_msgx[0]=80;			m_msgy[0]=375;
	//	m_mgx[0]=223;			m_mgy[0]=354;
	//  m_msgx[0]=85;			m_msgy[0]=373;

		//6인
		m_xp[1]=0;				m_yp[1]=252;
		m_rxp[1]=m_xp[1]+86;   m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=223;			m_mgy[1]=358;
		m_msgx[1]=85;			m_msgy[1]=383;

		m_AllinXp[0] = 95;			// 올인 애니 좌표
		m_AllinYp[0] = 289;			// 올인 애니 좌표	

		m_AllinXp[1] = 276;			// 올인 애니 좌표 끝 좌표
		m_AllinYp[1] = 244;			// 올인 애니 좌표	

	//	m_AllinXp[2] = 353;			// 올인 글씨
	//	m_AllinYp[2] = 489;			// 올인 글씨

		break;
	
	case 2:
		m_xp[0]=0;				m_yp[0]=75;
		m_rxp[0]=m_xp[0]+86;   m_ryp[0]=m_yp[0]+48;
	
		m_mgx[0]=211;			m_mgy[0]=187;
		m_msgx[0]=80;			m_msgy[0]=207;
	//	m_mgx[0]=223;			m_mgy[0]=184;
	//	m_msgx[0]=85;			m_msgy[0]=205;

		//6인
		m_xp[1]=0;				m_yp[1]=77;
		m_rxp[1]=m_xp[1]+86;   m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=223;			m_mgy[1]=184;
		m_msgx[1]=85;			m_msgy[1]=208;

		m_AllinXp[0] = 95;			// 올인 애니 좌표
		m_AllinYp[0] = 122;			// 올인 애니 좌표	

		m_AllinXp[1] = 276;			// 올인 애니 좌표 끝 좌표
		m_AllinYp[1] = 141;			// 올인 애니 좌표	

	//	m_AllinXp[2] = 353;			// 올인 글씨
	//	m_AllinYp[2] = 489;			// 올인 글씨
		break;
	
	case 3:
		m_xp[0]=537;			m_yp[0]=75;
		m_rxp[0]=m_xp[0]+5;    m_ryp[0]=m_yp[0]+48;
		m_mgx[0]=538;			m_mgy[0]=187;	
		m_msgx[0]=560;			m_msgy[0]=207;
	//	m_mgx[0]=515;			m_mgy[0]=184;
	//	m_msgx[0]=543;			m_msgy[0]=205;

		//6인
		m_xp[1]=269;			m_yp[1]=27;
		m_rxp[1]=m_xp[1]+86;    m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=322;			m_mgy[1]=150;
		m_msgx[1]=350;			m_msgy[1]=155;

		m_AllinXp[0] = 579;			// 올인 애니 좌표
		m_AllinYp[0] = 122;			// 올인 애니 좌표	

		m_AllinXp[1] = 414;			// 올인 애니 좌표 끝 좌표
		m_AllinYp[1] = 141;			// 올인 애니 좌표	

	//	m_AllinXp[2] = 353;			// 올인 글씨
	//	m_AllinYp[2] = 489;			// 올인 글씨
		break;

	case 4:
		m_xp[0]=537;			m_yp[0]=244;
		m_rxp[0]=m_xp[0]+5;    m_ryp[0]=m_yp[0]+48;
		m_mgx[0]=538;			m_mgy[0]=354;	
		m_msgx[0]=560;			m_msgy[0]=375;
	//	m_mgx[0]=515;			m_mgy[0]=354;
	//	m_msgx[0]=543;			m_msgy[0]=373;

		//7인
		m_xp[1]=536;			m_yp[1]=77;
		m_rxp[1]=m_xp[1]+5;    m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=515;			m_mgy[1]=184;
		m_msgx[1]=543;			m_msgy[1]=208;

		m_AllinXp[0] = 579;			// 올인 애니 좌표
		m_AllinYp[0] = 289;			// 올인 애니 좌표	

		m_AllinXp[1] = 414;			// 올인 애니 좌표 끝 좌표
		m_AllinYp[1] = 244;			// 올인 애니 좌표	

	//	m_AllinXp[2] = 353;			// 올인 글씨
	//	m_AllinYp[2] = 489;			// 올인 글씨
		break;
//////////
	case 5:
	
		//6인
		m_xp[1]=536;			m_yp[1]=252;
		m_rxp[1]=m_xp[1]+5;    m_ryp[1]=m_yp[1]+48;
		m_mgx[1]=515;			m_mgy[1]=358;
		m_msgx[1]=543;			m_msgy[1]=383;

		break;
	*/
	}

	
	//잭팟 머니 좌표
	m_jackpotx[0] = 778; m_jackpoty[0] = 44;
	m_jackpotx[1] = 782; m_jackpoty[1] = 44;

	// 잭팟 bg 좌표
	m_jackpotanix[0] = 553; m_jackpotaniy[0] = 26;
	m_jackpotanix[1] = 553; m_jackpotaniy[1] = 26;
	

	playscr_changeinitpos();
}


void CPlayScr::playscr_changeinitpos()
{
	if(g_RI.bPlayer==0)
	{
		Xp = m_xp[0]; Yp = m_yp[0];			// 플레이어 스크린의 기준좌표
		RXp = m_rxp[0]; RYp = m_ryp[0];		// 결과(승, 패, 폴드)
		MgX = m_mgx[0]; MgY = m_mgy[0];		// 메세지 박스
		MsgX = m_msgx[0]; MsgY = m_msgy[0];
		JackPotX = m_jackpotx[0];	JackPotY = m_jackpoty[0];
		JackPotaniX = m_jackpotanix[0]; JackPotaniY = m_jackpotaniy[0];
	}
	else {

		Xp = m_xp[1];   Yp = m_yp[1];		// 플레이어 스크린의 기준좌표
		RXp = m_rxp[1]; RYp = m_ryp[1];		// 결과(승, 패, 폴드)
		MgX = m_mgx[1]; MgY = m_mgy[1];		// 메세지 박스
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
	//m_AllinCnt = 0;			// 올인 애닉 관련 타임 변수 
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
	if(g_MyObserver == TRUE ) return;// ### [ 관전기능 ] ###
	

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

	case VK_UP: // 히튼 카드 보인다
	case VK_NUMPAD5:
		{
			// ### [ 관전기능 ] ###//히든			
			if(!g_MyObserver && !Game.FlyWindCard && Play[0].JoinState != 0 && Play[0].PlayState != 0 && Play[0].nCardTotal == 6 && !Play[0].bHiddenOpen && g_RI.ChangeCardStep == 2)
			{
				
					Sound.Play(SND16);
					Play[0].bHiddenOpen = TRUE; // 카드 클래스에서 보이기 한다.
					Game.FlyWindCard = FALSE;
					Game.CardDeck[0].SetFaceUp(5); // 보이기
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
			// ### [ 관전기능 ] ###
			
			if(!g_MyObserver && !Game.FlyWindCard && Play[0].JoinState != 0 && Play[0].PlayState != 0 && Play[0].nCardTotal == 6 && !Game.m_bHiddenCardShow && g_RI.ChangeCardStep ==2)
			{
				if(Game.m_nHinddenArrow==0)
				{
					Play[0].bHiddenOpen = TRUE; // 카드 클래스에서 보이기 한다.
					Game.FlyWindCard = FALSE;
					Game.CardDeck[0].SetFaceUp(5); // 보이기
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
	case 229:	// 한글인 경우
		{

		} break;

	case '1': case '2': case '3': case '4': case '5': case '6':
		{
			// 카드 바꾸기 일때 [62]
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
					if(g_pGameView->m_bAllin == TRUE && g_pGameView->m_bBMax == FALSE)g_pGameView->OnAllInBtn();//올인
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
				{   // 초이스 게임
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
						SelectChoiceCard(i+1, 0); // 사용자카드재배치
						return TRUE;
					}
				}
			}
			else
			{
				if(Game.m_discardindex == 0)
				{
					// 버릴카드 선택
					for(int i=0; i<4; i++) {
						if( CardRect[i].PtInRect(pt) ) {
							Game.m_discardindex = i+1;
							return TRUE;
						}
					}
				}
				else {
					// 오픈카드 선택
					for(int i=0; i<4; i++) {
						if( Game.m_discardindex-1 != i && CardRect[i].PtInRect(pt) ) {
							SelectChoiceCard(i+1, Game.m_discardindex); // 사용자카드재배치
							return TRUE;
						}
					}
				}
			}
			return TRUE;
		}

		// 카드 바꾸기 일때 [62]
		if(!g_MyObserver && g_bChangeCardStage == TRUE && Play[0].JoinState != 0 && Play[0].PlayState != 0){

			if(Game.CardDeck[0].PtInCardRect(xp,yp)){			
				return TRUE;
			}
			
		}		
		// 히든 카드 클릭
		// ### [ 관전기능 ] ###
		if(!g_MyObserver && Play[0].JoinState != 0 && Play[0].PlayState != 0 
			&& Play[0].nCardTotal == 6 && !Play[0].bHiddenOpen && g_RI.ChangeCardStep ==2 && g_bChangeCardStage == FALSE)
		{
			if( Game.CardDeck[0].Card[5].PtInCard(xp, yp) )
			{			
				Game.m_bHiddenClick = TRUE;			
				Game.m_hpos.x = xp;
				Game.m_hpos.y = yp;
				Game.CardDeck[0].SetFaceUp(5); // 보이기			
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
				Play[0].bHiddenOpen = TRUE; // 카드 클래스에서 보이기 한다.
				Game.FlyWindCard = FALSE;
				Game.CardDeck[0].SetFaceUp(5); // 보이기
				Game.m_bHiddenCardShow = TRUE;
				
				Game.m_bHiddenClick = FALSE;
			}
		}
		else if(Game.m_nHinddenArrow >= 0 && Game.m_nHinddenArrow <=3)
		{
			if( Game.m_bHiddenClick ) {
				Sound.Play(SND16);
				Play[0].bHiddenOpen = TRUE; // 카드 클래스에서 보이기 한다.
				Game.FlyWindCard = FALSE;
				Game.CardDeck[0].SetFaceUp(5); // 보이기
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
			{   // 초이스 게임
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
			// 오픈카드 선택
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
	// 사용자카드재배치
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
	
		
		//select = dlg.Select; // 선택된 카드값(0~51)
		//index = dlg.Index;   // 인덱스
		//DisCard = dlg.DisCard;			 // 버린 카드값(0~51)
		//DisCardIndex = dlg.DisCardIndex; // 버린카드 인덱스(1~4)

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
			if(aBuf[i] == select) // 카드값
			{	
				index = i+1;
				break;
			}
		}
		
		// 기존 방법
		int cnt = 0;
		for( i=0;i<3;i++) {
			if(index!=i+1){
				Game.CardDeck[0].Card[cnt].CardNo = aBuf[i];
				cnt++;
			}
		}
		
		//#################################################
		// 카드덱에서 제거
		Game.CardDeck[0].Card[3].Clear();
		Game.CardDeck[0].TotalCardNum -= 1;
		//#################################################
	}

	
	
	Sound.Play( SND16 );
	//서버로 내가 선택한 그림 보내준다.
	POKERCLNT pc={0,}; 
	pc.nStyle = 3; // 배팅
	pc.nBtn   = index; // 선택카드 (인덱스 1 , 2, 3)
	
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
		if(Play[PNum].JoinState != 0 )	{// 유저 카드 그린다.
			if( Play[PNum].bFold )
				Game.CardDeck[PNum].Draw( TRUE );
			else
				Game.CardDeck[PNum].Draw();
		}			
		
		// ###### 카드 족보  ######
		// ### [ 관전기능 ] ###	
		
		if(!g_MyObserver &&  PNum == 0 && Play[0].JoinState == 1 && Play[0].nOpenTotal > 0 && g_Poker.nState != RSTATE_CHANGECARD && !Game.FlyWindCard)
		{ // 나의 카드 족보
			CString str = "";
			str = Game.GetMyName();
			
			if(str!=""){ // 히든 카드는 클릭후에 보여준다.			
				if((g_Poker.nRound<5 && !Play[0].bHiddenOpen && !Game.m_bHiddenCardShow && g_RI.ChangeCardStep < 2) 
					|| (Play[0].bHiddenOpen && Game.m_bHiddenCardShow && !Game.bOpen)) {					
					pDC->SetTextColor(RGB(255,255,0));
					CRect rect;
					rect.SetRect(0,0,160,14);
					rect.OffsetRect(MsgX, Yp+167);
					if( !g_CvCard.bAllowControl )				// 쪼이기시 족보 보여주지 않음 - jeong
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

	// ### 게임중 ###
	if(g_RI.State == 1)  {
		
		for(int n=0; n<MAX_CHIP; n++) {//// ### 칩 그리기 ###
			if(Game.m_Chip[n].bLive) Game.m_Chip[n].Draw();
		}

		// ### 타이머 막대 그리기, 사운드 ###
		draw_timer_and_soundfx( PNum );
		
		// ### 게임진행 메세지 (진행) ###
		if(Play[PNum].JoinState == 1)
		{
			int nSNum = Play[PNum].ServPNum;
			if(nSNum < 0 || nSNum >= g_Max_Player) return;

			CString str ="";
			CString strTemp = "";
		
			// ### [ 정보 창에 메세지 ] ###
			if(g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < g_Max_Player && Game.CurPlayer != nSNum)
			{
				if( g_Poker.RU[nSNum].nLastCmd>0 ) 
				{				
					CString strTemp1 = "";
					CString strM = "";
					COLORREF nCmdCol = RGB(0,255,0); //RGB(255,255,0); // 레이즈 칼라
					int nLastCmd = g_Poker.RU[nSNum].nLastCmd;
				
					INT64 roundingoff = 0;

					switch(nLastCmd)
					{
						case 1: 
							{
								//strTemp.Format("다이");
								nCmdCol = RGB(200,200,200); 
							} break; // 다이
						case 2:
							{
								//strTemp.Format("체크");
								nCmdCol = RGB(0,255,0); 
							}break; // 체크
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
							} break; // 콜
						case 4: 
							{
								strTemp.Format("%d원",  g_RI.BetMoney);
												
							}  break; // 삥
						case 5:
							{
								roundingoff = g_Poker.RU[nSNum].nReCall;
								if(roundingoff > 100)
									roundingoff = (roundingoff/100)*100;
								strM = NumberToOrientalString(roundingoff);

								strM +=g_StrMan.Get(_T("DEFAULT_UNIT2")); // 원
								strTemp.Format("%s 받고", strM ); 
							
								roundingoff = g_Poker.RU[nSNum].nCalcBat;
								if(roundingoff > 100)
									roundingoff = (roundingoff/100)*100;

								strM = NumberToOrientalString(roundingoff);
								strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
								strTemp1.Format("%s 더", strM); 
							}  break; // 따당
						case 6:
							{
								if(g_Poker.RU[nSNum].nReCall > 0)
								{
									roundingoff = g_Poker.RU[nSNum].nReCall;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;
									strM = NumberToOrientalString(roundingoff);

									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp.Format("%s 받고", strM ); 
									
									roundingoff = g_Poker.RU[nSNum].nCalcBat;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;
										
									strM = NumberToOrientalString(roundingoff);
									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp1.Format("%s 더", strM); 
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

							} break; // 맥스
					
					}
					
					const int nShowBetX = MsgX-100;
					const int nShowBetY = 162;
					// 배팅건금액 위치 수정 - jeong
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
			
			// ##### [ 메세지 박스 그리기 ] #####
			if(g_Poker.PS[nSNum].bOnGame && g_Poker.PS[nSNum].PlayState==1 && 
				Game.CurPlayer != nSNum && g_Poker.RU[nSNum].nLastCmd>0)
			{					
				int nLastCmd = g_Poker.RU[nSNum].nLastCmd-1;
				if(g_Poker.LastPlayer == nSNum)
				{			
					if(nLastCmd==5){ //맥스
						if(g_RI.GameKind==0){ 
							pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, 13); //올인
						}else{
							pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd+6); //확대해 그린다.
						}
					}else{
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd+6); //확대해 그린다.
					}
				}
				else {
					if(nLastCmd==5){ //맥스
						if(g_RI.GameKind==0)
						{ 
							pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, 12); //올인
						}else{
							pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd); //확대해 그린다.
						}
					}else if(nLastCmd == 6){
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, 13); //올인 
					}
					else{
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd); //확대해 그린다.
					}
				}
			}
		}
		
		//[62] 패 바꾸기 안내 
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
		
		// 바꾸기 상태 표시 (바꿨는지 아닌지)	
		if(g_Poker.nState != RSTATE_STARTGAME  && g_RI.ChangeCardStep > 0 ){
			if(Play[PNum].bChangeWhether > -1 && Play[PNum].PlayState == 1){
				pPage->PutSprAuto(m_AllinXp[0]-13, m_AllinYp[0]+39, &EtcBtnSpr, Play[PNum].bChangeWhether+14); //올인 애니
				/*
				if(PNum == 0){					
					pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, Play[PNum].bChangeWhether+14); //확대해 그린다.;					
				}
				else{
				//	pPage->PutSprAuto(m_AllinXp[0]-13, m_AllinYp[0]+39, &EtcBtnSpr, Play[PNum].bChangeWhether+14); //확대해 그린다.;					
					if(PNum >= 1 && PNum <=2){
						pPage->PutSprAuto(MgX+10, MgY-92, &EtcBtnSpr, Play[PNum].bChangeWhether+14); //확대해 그린다.;
					}
					else if(PNum >= 3 && PNum <=4){
						pPage->PutSprAuto(MgX-10, MgY-92, &EtcBtnSpr, Play[PNum].bChangeWhether+14); //확대해 그린다.;
					}					
				}
				*/
			}		
		}
		
		if(g_RI.ChangeCardStep>0 && g_bChangeCardStage == FALSE){
			if(PNum == 0 ){
				pPage->PutSprAuto(482, 86, &EtcBtnSpr, g_RI.ChangeCardStep+16); //바꾸기 횟수를 표시
			}	
		}
		
	}	
	

	// ### [ 게임 오버 ] ###
	if(bGameOver == TRUE && Play[PNum].JoinState == 1)
	{
		// ### [ 관전기능 ] ###
		if(!Play[PNum].bFold && Game.bOpen)		
		{
			Game.m_bHiddenCardShow = TRUE;
			CString str="";
			str = Game.GetName(PNum);	  // 하이 
			pDC->SetTextColor(RGB(255,255,0));
			
			CRect rect;
			rect.SetRect(0,0,160,14);
			rect.OffsetRect(MsgX+5, MsgY+5);
			if(str!="") {
				pDC->DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);
			}
		}		

		if(Play[PNum].ServPNum == Game.WinnerPNum )
		{ // 승
			if(Play[PNum].ServPNum == Game.WinnerPNum )
			{
				if(PNum == 0){
					pPage->PutSprAuto(RXp-16, RYp+15, &FocusSpr, 5); // 하이
				}
				else{
					
					pPage->PutSprAuto(WIN_OFFSET_X, WIN_OFFSET_Y, &FocusSpr, 5); // 하이
					
				}		
			} 
		}
		else // 패
		{
			//if(Play[PNum].PlayState == 1)
			//	pPage->PutSprAuto(RXp-9, RYp+65, &FocusSpr, 11); // 패
			// ## 파산 ##
			if(Play[PNum].UI.PMoney <= 0)
			{
				if(PNum == 0){
					pPage->PutSprAuto(RXp-16, RYp+15, &FocusSpr, 6); // 하이
				}
				else{

					pPage->PutSprAuto(WIN_OFFSET_X, WIN_OFFSET_Y, &FocusSpr, 6); // 하이
					
				}			
			}
		}
	}

	// 콜 금액 표시 해준다			- jeong
	if(g_Poker.nState == RSTATE_STARTGAME && Play[0].PlayState == 1 && Game.CurPlayer == Play[0].ServPNum)
	{
		INT64 Callmoney =  Game.GetRaiseBat();
		if(Callmoney > 0 )
		{			
			pDC->SelectObject(g_pGameView->Font2);
			CString callmstr;
			callmstr.Format("콜금액:%I64d원",Callmoney);
			CRect rt;
			const int nXp = 323;
			const int nYp = 416;
			//rt.SetRect(323,396,206+323,13+396);
			rt.SetRect(nXp,nYp,206+nXp,13+nYp);
			pDC->DrawText(callmstr,&rt,DT_RIGHT);
			pDC->SelectObject(g_pGameView->Font2);
		}				
	}
		
	// ### 화살표 그리기 ###
	// ### [ 관전기능 ] ###히든
	
	if(!g_MyObserver && PNum == 0 && Play[0].PlayState != 0	&& Play[0].nCardTotal == 6 
		&& !Game.FlyWindCard && g_RI.ChangeCardStep == 2 && Play[0].bChangeWhether == 1)
	{		
		if( !Game.m_bHiddenCardShow)//Game.m_bHinddenArrow == TRUE)
		{
			int hY = 462 + Game.m_nHinddenArrow;
			pPage->PutSprAuto(421, hY, g_pGameView->m_pcarddeckspr, 52);
			
			pDC->SetTextColor(RGB(255,225,255));
			pDC->TextOut(445,447,"↑↓");
		}
	}

	//올인 애니 표시 한다 
	if(Play[PNum].bAllIn == TRUE && m_AllinAniStartTime > 0 && g_RI.State == 1)
	{
		DWORD CurTime = timeGetTime();
		
		CurTime = CurTime - m_AllinAniStartTime;			
		
		if(CurTime < 800)
			pPage->PutSprAuto(m_AllinXp[0], m_AllinYp[0], &AllinAniSpr, 0); //올인 표시		
		else if(CurTime > 900 && CurTime <= 1600)
		{
			pPage->PutSprAuto(m_AllinXp[1], m_AllinYp[1], &AllinAniSpr, 0); //올인 표시		
		}
		else if(CurTime > 1650)
			if(g_Poker.nState == RSTATE_STARTGAME)
			pPage->PutSprAuto(m_AllinXp[0]-13, m_AllinYp[0]+39, &AllinAniSpr, 1); //올인 표시				
		
	}
	
	//// ### 칩 그리기 ###
	if( Game.m_JackPotActive > 0)
	{
		DrawChip();

	//	for(int n=0; n<MAX_CHIP; n++) {
	//		if(Game.m_Chip[n].bLive) Game.m_Chip[n].Draw();
	//	}
	}

	// [ ###잭팟### ] 
	if(PNum ==0 || Game.bWindCardEnd) // 카드 날릴때 처리땜시 계속 그려준다.
	{
		// [게임 아이템 작업]
		//if(g_RI.bSuperMaster) {
		//	// 슈퍼방이면 슈퍼방임을 보여줌
		//	CString str;
		//	pDC->SetTextColor(RGB(255,70,70));
		//	str.Format("[슈퍼방]");
		//	pDC->TextOut(118, 8, str);
		//}

		draw_jackpot_digit();
	}
	// #### [ 잭팟 효과 ] ### 
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
	//바꾼 카드 표시
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
		if(Play[PNum].JoinState != 0 )	{// 유저 카드 그린다.
			if( Play[PNum].bFold )
				Game.CardDeck[PNum].Draw( TRUE );
			else
				Game.CardDeck[PNum].Draw();
		}

		// ###### 카드 족보  ######
		// ### [ 관전기능 ] ###
		if(!g_MyObserver &&  PNum == 0 && Play[0].JoinState == 1 && Play[0].nOpenTotal > 0 && !Game.FlyWindCard
			&& !g_CvCard.bAllowControl )	// 쪼이기시 족보 보여주지 않음 - jeong
		{ // 나의 카드 족보
			CString str = "";
			str = Game.GetMyName();
			
			if(str!=""){ // 히든 카드는 클릭후에 보여준다.
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

	// ### 게임중 ###
	if(g_RI.State == 1)  {
		
		for(int n=0; n<MAX_CHIP; n++) {//// ### 칩 그리기 ###
			if(Game.m_Chip[n].bLive) Game.m_Chip[n].Draw();
		}


		// ### 타이머 막대 그리기, 사운드 ###
		draw_timer_and_soundfx( PNum );
		
		// ### 게임진행 메세지 (진행) ###
		if(Play[PNum].JoinState == 1)
		{
			int nSNum = Play[PNum].ServPNum;
			if(nSNum < 0 || nSNum >= g_Max_Player) return;

			CString str ="";
			CString strTemp = "";
		
			// ### [ 정보 창에 메세지 ] ###
			if(g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < g_Max_Player && Game.CurPlayer != nSNum)
			{
				if( g_Poker.RU[nSNum].nLastCmd>0 ) 
				{				
					CString strTemp1 = "";
					CString strM = "";
					COLORREF nCmdCol = RGB(0,255,0); //RGB(255,255,0); // 레이즈 칼라
					int nLastCmd = g_Poker.RU[nSNum].nLastCmd;
				
					
					INT64 roundingoff = 0;

					switch(nLastCmd)
					{
						case 1: 
							{
								strTemp.Format("다이");
								nCmdCol = RGB(200,200,200); 
							} break; // 다이
						case 2:
							{
								strTemp.Format("체크");
								nCmdCol = RGB(0,255,0); 
							}break; // 체크
						case 3: 
							{
								if(g_Poker.RU[nSNum].nReCall <= 0)
									strTemp.Format("콜"); 
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
							} break; // 콜
						case 4: 
							{
								strTemp.Format("%d원",  g_RI.BetMoney);
												
							}  break; // 삥
						case 5:
							{
								roundingoff = g_Poker.RU[nSNum].nReCall;
								if(roundingoff > 100)
									roundingoff = (roundingoff/100)*100;
								strM = NumberToOrientalString(roundingoff);

								strM +=g_StrMan.Get(_T("DEFAULT_UNIT2")); // 원
								strTemp.Format("%s 받고", strM ); 
							
								roundingoff = g_Poker.RU[nSNum].nCalcBat;
								if(roundingoff > 100)
									roundingoff = (roundingoff/100)*100;

								strM = NumberToOrientalString(roundingoff);
								strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
								strTemp1.Format("%s 더", strM); 
							}  break; // 따당
						case 6:
							{
								if(g_Poker.RU[nSNum].nReCall > 0)
								{
									roundingoff = g_Poker.RU[nSNum].nReCall;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;
									strM = NumberToOrientalString(roundingoff);

									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp.Format("%s 받고", strM ); 
									
									roundingoff = g_Poker.RU[nSNum].nCalcBat;
									if(roundingoff > 100)
										roundingoff = (roundingoff/100)*100;
										
									strM = NumberToOrientalString(roundingoff);
									strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
									strTemp1.Format("%s 더", strM); 
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

							} break; // 맥스
					
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

			// ##### [ 메세지 박스 그리기 ] #####
			if(g_Poker.PS[nSNum].bOnGame && g_Poker.PS[nSNum].PlayState==1 && Game.CurPlayer != nSNum &&
				g_Poker.RU[nSNum].nLastCmd>0)
			{
				
				int nLastCmd = g_Poker.RU[nSNum].nLastCmd-1;
				if(g_Poker.LastPlayer == nSNum)
				{			
					if(nLastCmd==5) //맥스
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, 13); //확대해 그린다.;
					else
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd+6); //확대해 그린다.
				}
				else {
					if(nLastCmd==5) //맥스
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr,12);
					else
						pPage->PutSprAuto(MgX, MgY, &EtcBtnSpr, nLastCmd);
				}
			}
		}
	}

	// ### [ 게임 오버 ] ###
	if(bGameOver == TRUE && Play[PNum].JoinState == 1)
	{
		// ### [ 관전기능 ] ###
		if( g_MyObserver && PNum==0 ||  !Play[PNum].bFold && Game.bOpen)
		//if((g_MyObserver && PNum==0 && !Play[PNum].bFold && Game.bOpen) || PNum != 0 && !Play[PNum].bFold && Game.bOpen)
		{
			Game.m_bHiddenCardShow = TRUE;
			CString str="";
			str = Game.GetName(PNum);	  // 하이 
			pDC->SetTextColor(RGB(255,255,0));
			
			CRect rect;
			rect.SetRect(0,0,160,14);
			rect.OffsetRect(MsgX+5, MsgY+8);
			if(str!="") {
				pDC->DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);
			}
		}
	

		if(Play[PNum].ServPNum == Game.WinnerPNum )
		{ // 승
			if(Play[PNum].ServPNum == Game.WinnerPNum )
			{
				pPage->PutSprAuto(RXp, RYp+11, &FocusSpr, 6); // 하이
			} 
		}
		else // 패
		{
			//if(Play[PNum].PlayState == 1)
			//	pPage->PutSprAuto(RXp-9, RYp+65, &FocusSpr, 11); // 패
			// ## 파산 ##
			if(Play[PNum].UI.PMoney <= 0)
			{
				pPage->PutSprAuto(RXp, RYp+11, &FocusSpr, 7); // 파산
			}
		}
	}


	// ### 화살표 그리기 ###
	// ### [ 관전기능 ] ###히든
	
	if(!g_MyObserver && PNum == 0 && Play[0].PlayState != 0 && Play[0].nCardTotal == 6 
		&& !Game.FlyWindCard && g_RI.ChangeCardStep == 2)
	{		
		if( !Game.m_bHiddenCardShow)//Game.m_bHinddenArrow == TRUE)
		{
			int hY = 464 + Game.m_nHinddenArrow;
			pPage->PutSprAuto(421, hY, g_pGameView->m_pcarddeckspr, 52);
			
			pDC->SetTextColor(RGB(255,225,255));
			pDC->TextOut(445,447,"↑↓");
		}
	}
	

	
	//// ### 칩 그리기 ###
	if( Game.m_JackPotActive > 0)
	{
		for(int n=0; n<MAX_CHIP; n++) {
			if(Game.m_Chip[n].bLive) Game.m_Chip[n].Draw();
		}
	}

	// [ ###잭팟### ] 
	if(PNum ==0 || Game.bWindCardEnd) // 카드 날릴때 처리땜시 계속 그려준다.
	{
		// [게임 아이템 작업]
		//if(g_RI.bSuperMaster) {
		//	// 슈퍼방이면 슈퍼방임을 보여줌
		//	CString str;
		//	pDC->SetTextColor(RGB(255,70,70));
		//	str.Format("[슈퍼방]");
		//	pDC->TextOut(118, 8, str);
		//}

		draw_jackpot_digit();
	}
	// #### [ 잭팟 효과 ] ### 
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

// [ ###잭팟### ] 
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

		g_JackPotMoney+=plus; // 증가 보여주기
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

		g_JackPotMoney-=plus; // 증가 보여주기

		if(PNum==0 && Game.m_JackPotActive > 0) {
			Game.CastChipJackPot(7, plus, 0); 
		}
	}

	//if(g_TmpJackPotMoney > g_JackPotMoney) g_JackPotMoney+=plus; // 증가 보여주기
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


// ### 타이머 막대 그리기, 사운드 ###
void CPlayScr::draw_timer_and_soundfx(int PNum)
{
	Game.CurTime = timeGetTime();
	if(g_RI.bPlayer==0) 
	{
		////////////////////////////////////////////////////////////////
		// 카드선택시 타이머 찍기

		if( Play[PNum].m_bSelectCard == TRUE && Play[PNum].PlayState==1 )
		{
			int tcnt = ( Game.CurTime - Game.BackTime ) / 1000;
			int timeLimit = 10; // 3장 10초

			//[62]
			if(g_bChangeCardStage  == TRUE && g_Poker.nState == RSTATE_CHANGECARD)
			{
				timeLimit = 10; // Changecard 10초				
			}

			// 카드 선택 하기
			if(PNum == 0 && Game.m_bchoice_card) {
				if(g_RI.FormKind==0) {
				//  pPage->PutSprAuto(253, 417, g_pGameView->m_pgamebackspr, 3);
					pPage->PutSprAuto(229, 271, g_pGameView->m_pgamebackspr, 3,10,10);
					pPage->PutSprAuto(251, 432, g_pGameView->m_pgamebackspr, 16);
					for(int i=0; i<3;i++) { // 카드 3장 그리기
						pPage->PutSprAuto(270+(i*95), 324, &BigCardSpr, Game.CardDeck[0].GetCardNo(i) );

					}
					pPage->PutSprAuto(240,271, g_pGameView->m_pgamebackspr, 10);
					if(Game.m_ncur_selectcard > 0) { // 마우스 오버
						pPage->PutSprAuto(267+((Game.m_ncur_selectcard-1)*95), 315, &FocusSpr, 2);
					}
				} 
				else 
				{
				//	pPage->PutSprAuto(253, 417, g_pGameView->m_pgamebackspr, 4);
					pPage->PutSprAuto(229, 271, g_pGameView->m_pgamebackspr, 3,10,10);
					pPage->PutSprAuto(251, 432, g_pGameView->m_pgamebackspr, 17);
					// 카드 4장 그리기
					for(int i=0; i<4;i++) {
						if(Game.m_discardindex-1 == i)
							pPage->PutSprAuto(245+(i*79), 324, &BigCardSpr, 52 );
						else
							pPage->PutSprAuto(245+(i*79), 324, &BigCardSpr, Game.CardDeck[0].GetCardNo(i) );
					}
					pPage->PutSprAuto(240,271, g_pGameView->m_pgamebackspr, Game.m_discardindex==0?9:10);
					if(Game.m_ncur_selectcard > 0) { // 마우스 오버
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
					
				//	int sx = 339; // 그려질 시작위치
				//	int sy = 423;

					int sx = 245; // 그려질 시작위치
					int sy = 302;
					//	pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
					int imagex = sx + 310; //이미지 사이즈
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

					// 그려질 시작위치
			//		int sx = MsgX + ((PNum <= 2) ? -26:17);
			//		int sy = MsgY + 10;				

					int sx = MsgX;
					int sy = MsgY;									

					pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
					int imagex = sx + 160; //이미지 사이즈
					int imagey = sy + 11; 
					imagex -= (16*tcnt);					
					pPage->SetTempClip(sx, sy, imagex, imagey);
					pPage->PutSprAuto(sx, sy, &TimerSpr, 1, TEMPCLIP);
				}
			}
			return;
		}
		////////////////////////////////////////////////////////////////
		// 턴타이머
		if(Play[PNum].PlayState==1 && Game.CurPlayer == Play[PNum].ServPNum && !Game.FlyWindCard  && g_Poker.nState != RSTATE_CHANGECARD ) // 카드 날리는 중에는 그리지 않는다.
		{			
			int tcnt = ( Game.CurTime - Game.BackTime ) / 1000;
			if( tcnt >= 0 && tcnt <= 10 ) 
			{
				memset( Game.ClockFlag, TRUE, sizeof( BOOL ) * 7 );
				if(!g_MyObserver && PNum == 0 ) {// ### [ 관전기능 ] ###
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
				int imagex = sx + 160; //이미지 사이즈
				int imagey = sy + 11; 
				imagex -= (16*tcnt);				
				pPage->SetTempClip(sx, sy, imagex, imagey);
				pPage->PutSprAuto(sx, sy, &TimerSpr, 1, TEMPCLIP);
			}
		}		
	}
	else 
	{ //6인

		/////////////////////////////////////////////////////////////////////
		//// 카드선택시 타이머 찍기
		if(Play[PNum].m_bSelectCard == TRUE && Play[PNum].PlayState==1 )
		{
			int tcnt = ( Game.CurTime - Game.BackTime ) / 1000;
			int timeLimit = 7; // 3장 7초
			int timeNum = 18;
			// 카드 선택 하기
			if(PNum == 0 && Game.m_bchoice_card) {
				if(g_RI.FormKind==0) {
					pPage->PutSprAuto(261, 416, g_pGameView->m_pgamebackspr, 3);
					for(int i=0; i<3;i++) { // 카드 3장 그리기
						pPage->PutSprAuto(278+(i*79), 472, &BigCardSpr, Game.CardDeck[0].GetCardNo(i) );
					}
					pPage->PutSprAuto(271,421, g_pGameView->m_pgamebackspr, 10);
					if(Game.m_ncur_selectcard > 0) { // 마우스 오버
						pPage->PutSprAuto(275+((Game.m_ncur_selectcard-1)*79), 463, &FocusSpr, 3);
					}
				} 
				else 
				{
					pPage->PutSprAuto(261, 416, g_pGameView->m_pgamebackspr, 4);
					// 카드 4장 그리기
					for(int i=0; i<4;i++) {
						if(Game.m_discardindex-1 == i)
							pPage->PutSprAuto(273+(i*64), 487, &SmallCardSpr, 52 );
						else
							pPage->PutSprAuto(273+(i*64), 487, &SmallCardSpr, Game.CardDeck[0].GetCardNo(i) );
					}
					pPage->PutSprAuto(271,421, g_pGameView->m_pgamebackspr, Game.m_discardindex==0?9:10);
					if(Game.m_ncur_selectcard > 0) { // 마우스 오버
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
					
					int sx = 272; // 그려질 시작위치
					int sy = 448;
					//	pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
					int imagex = sx + 253; //이미지 사이즈
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
					
					int sx = MsgX-4; // 그려질 시작위치
					int sy = MsgY+4;
					pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
					int imagex = sx + 180; //이미지 사이즈
					int imagey = sy + 13; 
					imagex -= (18*tcnt);
					pPage->SetTempClip(sx, sy, imagex, imagey);
					pPage->PutSprAuto(sx, sy, &TimerSpr, 1, TEMPCLIP);
				}
			}
			return;
		}
		/////////////////////////////////////////////////////////////////////

		// 턴타이머
		if( Play[PNum].PlayState==1 && Game.CurPlayer == Play[PNum].ServPNum && !Game.FlyWindCard ) // 카드 날리는 중에는 그리지 않는다.
		{
			
			int tcnt = ( Game.CurTime - Game.BackTime ) / 1000;
			if( tcnt >= 0 && tcnt <= 10 ) 
			{
				memset( Game.ClockFlag, TRUE, sizeof( BOOL ) * 7 );
				if(!g_MyObserver && PNum == 0 ) {// ### [ 관전기능 ] ###
					if(!Game.ClockFlag[tcnt])
					{
						Game.ClockFlag[tcnt] = TRUE;
						Sound.Play( SND20 );
					}
				}
				int sx = MsgX-4; // 그려질 시작위치
				int sy = MsgY+4;
				pPage->PutSprAuto( sx, sy, &TimerSpr, 0);
				int imagex = sx + 180; //이미지 사이즈
				int imagey = sy + 13; 
				imagex -= (18*tcnt);
				pPage->SetTempClip(sx, sy, imagex, imagey);
				pPage->PutSprAuto(sx, sy, &TimerSpr, 1, TEMPCLIP);
			}
		}
	}
}	
