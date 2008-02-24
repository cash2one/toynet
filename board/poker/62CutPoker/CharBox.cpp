// CharBox.cpp: implementation of the CCharBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "CharBox.h"
#include "62CutPokerDlg.h"
#include "Global.h"
#include "ProfileDlg.h"
//#include "UserStruc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharBox::CCharBox()
{
	pParent = NULL;
	pPage = NULL;
	bShow = FALSE;
	bLock = FALSE;
	Xp = Yp = 0;
	Width = 0;
	Height = 0;
	CharXp = CharYp = 0;
	bClick = FALSE;

	PNum = -1;
}

CCharBox::~CCharBox()
{
	if(Font1.m_hObject != NULL) Font1.DeleteObject();
	if(Font2.m_hObject != NULL) Font2.DeleteObject();
}

void CCharBox::Init(CWnd *pWnd, CPage *ppage, int defpos)
{
	pParent = pWnd;
	pPage = ppage;
	bShow = TRUE;

	m_width[0] = 77;
	m_height[0] = 130;

	m_width[1] = 75;
	m_height[1] = 119;

	//캐릭 인덱스 랜덤선택 - jeong	
	srand(time(NULL));
	m_nCharIndex[0] = rand() % 5;			// 남성캐릭
	m_nCharIndex[1] = rand() % 5;
	m_nCharIndex[1] = m_nCharIndex[1] + 5; //여성캐릭

	switch(defpos)
	{
	case 0:
		m_xp[0] = 249; m_yp[0] = 419;
		m_charxp[0] = m_xp[0]+13; m_charyp[0] = m_yp[0]+25;
	//	m_cardxp[0] = m_xp[0] +87 ; m_cardyp[0] = m_yp[0]+38;
		
		
	//	m_xp[1] = 257; m_yp[1] = 422;
	//	m_charxp[1] = m_xp[1]+16; m_charyp[1] = m_yp[1]+25;
	//	m_cardxp[1] = m_xp[1] +87 ; m_cardyp[1] = m_yp[1]+38;
		break;
		
	// AI 캐릭터 위치 변경 - jeong
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		m_xp[0] = 278; m_yp[0] = 54;
		m_charxp[0] = m_xp[0]+175; m_charyp[0] = m_yp[0]-18;
	//	m_cardxp[0] = m_xp[0]+81; m_cardyp[0] = m_yp[0];
		
		
	//	m_xp[1] = 2; m_yp[1] = 252;
	//	m_charxp[1] = m_xp[1]+8; m_charyp[1] = m_yp[1]+26;
	//	m_cardxp[1] = m_xp[1]+81; m_cardyp[1] = m_yp[1];
		break;
	/*
	case 2:
		m_xp[0] = 2; m_yp[0] = 71;
		m_charxp[0] = m_xp[0]+5; m_charyp[0] = m_yp[0]+26;
	//	m_cardxp[0] = m_xp[0]+81; m_cardyp[0] = m_yp[0];
		
		
	//	m_xp[1] = 0; m_yp[1] = 77;
	//	m_charxp[1] = m_xp[1]+8; m_charyp[1] = m_yp[1]+26;
	//	m_cardxp[1] = m_xp[1]+81; m_cardyp[1] = m_yp[1];
		break;
	case 3:
		m_xp[0] = 554; m_yp[0] = 71;		
		m_charxp[0] = m_xp[0]+168; m_charyp[0] = m_yp[0]+26;
	//	m_cardxp[0] = m_xp[0]-174; m_cardyp[0] = m_yp[0];
		
		
	//	m_xp[1] = 269; m_yp[1] = 27;
	//	m_charxp[1] = m_xp[1]+8; m_charyp[1] = m_yp[1]+26;
	//	m_cardxp[1] = m_xp[1]-174; m_cardyp[1] = m_yp[1];
		break;
		
	case 4:
		m_xp[0] = 554; m_yp[0] = 239;
		m_charxp[0] = m_xp[0]+168; m_charyp[0] = m_yp[0]+26;
	//	m_cardxp[0] = m_xp[0]-174; m_cardyp[0] = m_yp[0];
		
		
	//	m_xp[1] = 536; m_yp[1] = 77;
	//	m_charxp[1] = m_xp[1]+184; m_charyp[1] = m_yp[1]+26;
	//	m_cardxp[1] = m_xp[1]-174; m_cardyp[1] = m_yp[1];
		break;
		////////////
	case 5:
		m_xp[0] = 715; m_yp[0] = 180;
		m_charxp[0] = m_xp[0]; m_charyp[0] = m_yp[0]+15;
	//	m_cardxp[0] = 548; m_cardyp[0] = 210;
		
	//	m_xp[1] = 536; m_yp[1] = 252;
	//	m_charxp[1] = m_xp[1]+184; m_charyp[1] = m_yp[1]+26;
	//	m_cardxp[1] = m_xp[1]-174; m_cardyp[1] = m_yp[1];
		break;
	*/
	}
		
	

	// 폰트 생성
	if(Font1.m_hObject == NULL)
	Font1.CreateFont(12,0,0,0,
		//FW_NORMAL,
		FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	if(Font2.m_hObject == NULL)
	Font2.CreateFont(12,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");

	charbox_changeinitpos();
}


void CCharBox::charbox_changeinitpos()
{
	//////////////////////////
	switch(PNum)
	{
	case 0:
		
		break;
	case 1:
	
		break;
	case 2:
	
		break;
	case 3:
		
		break;

	case 4:
	
		break;
	case 5:
	
		break;
	case 6:
	
		break;
	}


	////////////////////////

	if( g_RI.bPlayer == 0  ) {
		Xp = m_xp[0]; Yp = m_yp[0];
		CharXp = m_charxp[0];  CharYp = m_charyp[0];
	//	CardXp = m_cardxp[0] ; CardYp = m_cardyp[0];

		Width  = m_width[0];
		Height = m_height[0];

	} else {

		Xp = m_xp[1]; Yp = m_yp[1];
		CharXp = m_charxp[1];  CharYp = m_charyp[1];
	//	CardXp = m_cardxp[1] ; CardYp = m_cardyp[1];

		Width  = m_width[1];
		Height = m_height[1];
	}
}


void CCharBox::Lock(BOOL bvalLock)
{
	bLock = bvalLock;
}

void CCharBox::SetPlayer(int pnum)
{
	PNum = pnum;
}

void CCharBox::ClearPlayer()
{
	PNum = -1;
}


BOOL CCharBox::OnLButtonDown(int x, int y)
{
	if(x >= CharXp+10 && x <= CharXp+Width && y >= CharYp+10 && y <= CharYp+Height) { bClick = TRUE; return TRUE;}
	else bClick = FALSE;
	return FALSE;
}

BOOL CCharBox::OnLButtonUp(int x, int y)
{
	if(bClick == TRUE)
	if(x >= CharXp+10 && x <= CharXp+Width && y >= CharYp+10 && y <= CharYp+Height)
	{
		bClick = FALSE;
		if(strlen(Play[PNum].UI.ID)==0) return FALSE;
		
		// 사운드 출력
		Sound.Play(SND45);

		// 사용자 정보를 보여줌
		CProfileDlg Dlg;
		Dlg.Init(&Play[PNum].UI);
		Dlg.DoModal();
		return TRUE;
	}

	return FALSE;
}

BOOL CCharBox::OnMouseMove(int x, int y)
{
	if(x >= CharXp+10 && x <= CharXp+Width && y >= CharYp+10 && y <= CharYp+Height)
	{
		if(strlen(Play[PNum].UI.ID)==0) return FALSE;
		SetCursor(g_pGameView->hCurHand);
		return TRUE;
	}
	return FALSE;
}

void CCharBox::DrawDigit(int bKind, int Pos, INT64 money)
{

	int xp=0, yp=0;
	if(bKind == 0) yp = 375;
	else yp = 394;

	switch(Pos)
	{
	case 1:	xp = 372;	break;
	case 2:	xp = 417;	break;
	case 3:	xp = 461;	break;	
	case 4: xp = 506;    break;
	}

	for( int j = 0; j < 20 ; j ++ ) {
		if( money <= 0 ) 	{
			break;
		}
		pPage->PutSprAuto( xp -  j * 8, yp, &JackPotSpr,25 + (int)(money%(INT64)10) );
		money = money / 10;
	}
}


void CCharBox::Draw(CDC *pDC)
{
	if(pPage == NULL) return;
	if(bShow == FALSE) return;
	if(PNum < 0 || PNum >= g_Max_Player ) return;
	if(strlen(Play[PNum].UI.ID) == 0) return;

	if(g_RI.bPlayer==0)
		draw_5user(pDC);
	else
		draw_6user(pDC);
}

void CCharBox::ResultCharDraw(CPage *pRPage, int nPlayerNum)
{
	if( nPlayerNum == 0 )
		pRPage->PutSprAuto(30, 30, &AvaSpr, m_nCharIndex[0]);
	else
		pRPage->PutSprAuto(30, 30, &AvaSpr, m_nCharIndex[1]);
	
}

void CCharBox::draw_5user(CDC *pDC) 
{
	if( !PlayScr[0].bGameOver )
	{
		if( Play[PNum].PlayState == 1 && Game.CurPlayer == Play[PNum].ServPNum )
		{	
		//	if(PNum == 0) pPage->PutSprAuto(Xp , Yp , &FocusSpr, 0 );		
		//	else pPage->PutSprAuto(Xp , Yp , &FocusSpr, 1 );			
		}
	}
	else
	{
		if(Game.WinnerPNum == Play[PNum].ServPNum || Game.LowWinnerPNum == Play[PNum].ServPNum )
		{
		
			if((Game.GameCnt/25)%2  == 0) {
				if(PNum == 0) 
					pPage->PutSprAuto(Xp , Yp , &FocusSpr, 0 );
			//	else if(PNum < 3) pPage->PutSprAuto(Xp , Yp , &FocusSpr, 1 );
				else 
					pPage->PutSprAuto(270, 24, &FocusSpr, 1 );		// 승리시 깜박 금색줄 출력위치 조정 - modefied by jeong
			}
		
			
		}
	}

	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&Font2);
	CString str= "";

	
	// 캐릭터 그리기 - jeong

	if( PNum == 0 )
		pPage->PutSprAuto(CharXp, CharYp, &AvaSpr, m_nCharIndex[0]);
	else
		pPage->PutSprAuto(CharXp, CharYp, &AvaSpr, m_nCharIndex[1]);

	/*
	if(PNum < 3)
		Play[PNum].Avatar.Draw(CharXp, CharYp, pPage->width, pPage->height, (WORD*)pPage->lpBit, pPage->lPitch, TRUE);
	else
		Play[PNum].Avatar.Draw(CharXp, CharYp, pPage->width, pPage->height, (WORD*)pPage->lpBit, pPage->lPitch, FALSE);
	*/
	


	// 유저 위치정보
	int x = Xp;
	int y = Yp;
	
	//////////////////////////////////////////////////////////////////////////////////////////
	/*		deleted by jeong
	// 레벨
	int level = Play[PNum].UI.nIcon;
	CMyBitmap bmp;
	bmp.LoadBitmap(IDB_LEVEL15);

	if(PNum == 0)
		bmp.TransDraw(pDC->m_hDC, x+7, y+161, 15, 15, 15*level, 0, RGB(255,0,255));
	else if(PNum < 3)
		bmp.TransDraw(pDC->m_hDC, x+10, y+5, 15, 15, 15*level, 0, RGB(255,0,255));	
	else
		bmp.TransDraw(pDC->m_hDC, x+145, y+5, 15, 15, 15*level, 0, RGB(255,0,255));
	bmp.DeleteObject();
	//////////////////////////////////////////////////////////////////////////////////////////
	
	int idx = 0; //= x+94;
	int idy = 0;// = y+118;
	// 아디 
	if(PNum == 0) {
		idx = x + 24;
		idy = y+164;
	} else if(PNum < 3) {	
		idx = x + 26;
		idy = y +8;
	} else {
		idx = x +165 ;
		idy = y +8;
	}

	str.Format("%s", Play[PNum].UI.ID);
//	pDC->SetTextColor(RGB(10,10,10));
//	pDC->TextOut(idx-1, idy, str);
//	pDC->TextOut(idx, idy-1, str);
//	pDC->TextOut(idx+1, idy, str);
//	pDC->TextOut(idx, idy+1, str);
//	pDC->SetTextColor(RGB(10, 10, 10));
	//pDC->SetTextColor(RGB(172,252,255));
	pDC->TextOut(idx, idy, str);
	*/


	CRect rect;
	CString strM = "";
	pDC->SetTextColor(RGB(244,255,75));

	// 나의 머니(Credit)
	INT64 roundingoff = Play[PNum].PrevMoney-Play[PNum].LoseMoney;

	if( roundingoff < 0 )
		roundingoff = 0;

	strM = NumberToOrientalString(roundingoff);
	strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));

	if(PNum == 0) 
	{	
		pDC->SetTextColor(RGB(23, 175, 214));

		//pot
		CString strPotM = "";
		CRect PotRt;
		strPotM = NumberToOrientalString(500);
		strPotM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
		PotRt.SetRect(0,0,200,14);
		PotRt.OffsetRect(590,340);
		pDC->DrawText(strPotM, &PotRt, DT_RIGHT | DT_WORDBREAK);

		// bat
		CString strBatM = "";
		CRect BatRt;
		strBatM = NumberToOrientalString(200);
		strBatM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
		BatRt.SetRect(0,0,200,14);
		BatRt.OffsetRect(590,360);
		pDC->DrawText(strBatM, &BatRt, DT_RIGHT | DT_WORDBREAK);

		
		

		// 이긴 금액 (전판)
		CString strWinM = "";
		CRect WinRt;
		strWinM = NumberToOrientalString(Play[0].nWinMoney);
		strWinM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));
		WinRt.SetRect(0,0,200,14);
		WinRt.OffsetRect(590,385);
		pDC->DrawText(strWinM, &WinRt, DT_RIGHT | DT_WORDBREAK);

		pDC->SetTextColor(RGB(244,255,75));

		// 나의 금액
		rect.SetRect(0,0,200,14);
		rect.OffsetRect(590,411);
		pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK);

		// 실제 돈 (서버)
		roundingoff = Play[0].UI.PMoney;
		strM = NumberToOrientalString(roundingoff);
		rect.SetRect(0,0,200,14);
		rect.OffsetRect(590,450);
		//pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK);
		
		
		
		// 총베팅액
		roundingoff = Game.GetRealBet();
		
		// 총 배팅 금액 
		int num1 = 0, num2 = 0, num3=0, num4=0;
		NumberToParse(roundingoff, num1, num2, num3, num4);
		rect.SetRect(0,0,33,14);
		rect.OffsetRect(346, 379);
		BOOL bZero = FALSE;

		CFont *Oldf = pDC->SelectObject(&Font1);
		
		if(num1>0) { bZero = TRUE; strM.Format("%d", num1); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num2>0 || bZero) { bZero = TRUE; strM.Format("%d", num2); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num3>0 || bZero) { bZero = TRUE; strM.Format("%d", num3); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num4>0 || bZero) { strM.Format("%d", num4); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }

	
		pDC->SelectObject(Oldf);

		num1 = 0, num2 = 0, num3=0, num4=0;
		int pnum = Play[PNum].ServPNum;
		roundingoff = g_Poker.RU[pnum].nRealBat;

		if(roundingoff > 100)
			roundingoff = (roundingoff/100)*100;

		//추가 배팅 금액 
		NumberToParse(roundingoff, num1, num2, num3, num4);
		rect.SetRect(0,0,33,14);
		rect.OffsetRect(346, 398);
		bZero = FALSE;
		if(num1>0 || bZero) { bZero = TRUE; strM.Format("%d", num1); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num2>0 || bZero) { bZero = TRUE; strM.Format("%d", num2); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num3>0 || bZero) { bZero = TRUE; strM.Format("%d", num3); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num4>0 || bZero) { bZero = TRUE; strM.Format("%d", num4); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }

		
		// +- 증가액 표시
		//roundingoff = Play[PNum].UI.PMoney - Play[PNum].PrevMoney;
		roundingoff = Play[0].BankMoney;
		strM = NumberToOrientalString(roundingoff);//g_Poker.nTotBat); // 임시 추가

		CString str;
		str.Format("%I64d",roundingoff);

		if(roundingoff > 0)	{
			strM.Insert(0,"+");
			/*
			// 현재의 방에서 얻은 금액 표시 			
			rect.SetRect(0,0,200,14);
			rect.OffsetRect(590, 389);
			CString str1;
			str1 = strM + str;
			str1 += "원";
			pDC->DrawText(str1, &rect, DT_RIGHT | DT_WORDBREAK);
			*/
		}
		/*
		else 
		{
			rect.SetRect(0,0,200,14);
			rect.OffsetRect(590, 389);
			CString str1;
			str1 = "0원";
			pDC->DrawText(str1, &rect, DT_RIGHT | DT_WORDBREAK);
		}
		*/
		

		
	
		//배팅 건금액
		pDC->SetTextColor(RGB(200,252,255));
		strM+=g_StrMan.Get(_T("DEFAULT_UNIT2"));
		rect.SetRect(0,0,200,14);
		rect.OffsetRect(590, 430);
		pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK);

		//전적 표시
	//	pDC->SetTextColor(RGB(175,252,255));
	//	strM.Format("%d승%d패%d포기", Play[PNum].UI.WinNum, Play[PNum].UI.LooseNum, Play[PNum].UI.DrawNum);
	//	rect.OffsetRect(0, 23);
	//	pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK);
	} 
	else //if( PNum < 3)	 상대방금액을 아바타 아래로 위치변경 - modefied by jeong 
	{
		CString strAIM = "";
		
		strAIM = NumberToOrientalString(Play[PNum].UI.PMoney);
		strAIM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));

		pDC->SetTextColor(RGB(172,252,255));
		rect.SetRect(0,0,180,14);			
		rect.OffsetRect(310, 34);				
		//pDC->DrawText(strAIM, &rect, DT_RIGHT | DT_WORDBREAK);
	}
	/*
	else {
		pDC->SetTextColor(RGB(172,252,255));
		rect.SetRect(0,0,180,14);
		rect.OffsetRect(x+10, y+7);
		pDC->DrawText(strM, &rect, DT_LEFT | DT_WORDBREAK);
	}
	*/

	// ### [ 중복된 IP가 ] ###
	if(Play[PNum].MyIpCheck >= 0)
	{
		int imageIndex = 2;//Play[PNum].MyIpCheck * 2;
		if(PNum==0) pPage->PutSprAuto(Xp+5, Yp+25+5, &WarningSpr, ( Game.GameCnt/15 )%2+ imageIndex);
		else if(PNum<3) pPage->PutSprAuto(Xp+5, Yp+45, &WarningSpr, ( Game.GameCnt/15 )%2+ imageIndex);
		else pPage->PutSprAuto(Xp+180, Yp+45, &WarningSpr, ( Game.GameCnt/15 )%2+imageIndex, LR);
	}

	// 방장 마크 그리기
//	if(strcmp(g_RI.ID, Play[PNum].UI.ID)==0)
//	{
//		if(PNum==0) pPage->PutSprAuto(Xp+5, Yp+5, &EtcSpr, (Game.GameCnt/10)%4);
//		else if(PNum<3) pPage->PutSprAuto(Xp, Yp+20, &EtcSpr, (Game.GameCnt/10)%4);
//		else pPage->PutSprAuto(Xp+180, Yp+20, &EtcSpr, (Game.GameCnt/10)%4);
//	}
	
	/*
	// 방장 마크 그리기		// 삭제 - jeong
	if(strcmp(g_RI.ID, Play[PNum].UI.ID)==0)
	{
		if( g_RI.bSuperMaster ) {
			if(PNum==0) pPage->PutSprAuto(Xp, Yp+5, g_pGameView->m_pgamebackspr, 11+(Game.GameCnt/10)%5);
			else if(PNum<3) pPage->PutSprAuto(Xp, Yp+20, g_pGameView->m_pgamebackspr, 11+(Game.GameCnt/10)%5);
			else pPage->PutSprAuto(Xp+168, Yp+20, g_pGameView->m_pgamebackspr, 11+(Game.GameCnt/10)%5);
		}
		else {
			if(PNum==0) pPage->PutSprAuto(Xp, Yp+5, &EtcSpr, (Game.GameCnt/10)%4);
			else if(PNum<3) pPage->PutSprAuto(Xp, Yp+20, &EtcSpr, (Game.GameCnt/10)%4);
			else pPage->PutSprAuto(Xp+168, Yp+20, &EtcSpr, (Game.GameCnt/10)%4);
		}
	}
	*/

}

void CCharBox::draw_6user(CDC *pDC) 
{

	if( !PlayScr[0].bGameOver )
	{
		if( Play[PNum].PlayState == 1 && Game.CurPlayer == Play[PNum].ServPNum )
		{
			if(PNum == 0) pPage->PutSprAuto(Xp , Yp , &FocusSpr, 8 );
			else if(PNum < 3) pPage->PutSprAuto(Xp , Yp , &FocusSpr, 9 );
			else if(PNum == 3) pPage->PutSprAuto(Xp , Yp , &FocusSpr, 10 );
			else pPage->PutSprAuto(Xp , Yp , &FocusSpr, 11 );
		}
	}
	else
	{
		if(Game.WinnerPNum == Play[PNum].ServPNum || Game.LowWinnerPNum == Play[PNum].ServPNum )
		{
			if((Game.GameCnt/25)%2  == 0) {
				if(PNum == 0) pPage->PutSprAuto(Xp , Yp , &FocusSpr, 8 );
				else if(PNum < 3) pPage->PutSprAuto(Xp , Yp , &FocusSpr, 9 );
				else if(PNum == 3) pPage->PutSprAuto(Xp , Yp , &FocusSpr, 10 );
				else pPage->PutSprAuto(Xp , Yp , &FocusSpr, 11 );
			}
			
		}
	}
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&Font2);
	CString str= "";
	
	// 캐릭터 그리기
	if(PNum <= 3)
		Play[PNum].Avatar.Draw(CharXp, CharYp, pPage->width, pPage->height, (WORD*)pPage->lpBit, pPage->lPitch, TRUE);
	else
		Play[PNum].Avatar.Draw(CharXp, CharYp, pPage->width, pPage->height, (WORD*)pPage->lpBit, pPage->lPitch, FALSE);
	
	// 유저 위치정보
	int x = Xp;
	int y = Yp;
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// 레벨
	int level = Play[PNum].UI.nIcon;
	CMyBitmap bmp;
	bmp.LoadBitmap(IDB_LEVEL15);

	if(PNum == 0)
		bmp.TransDraw(pDC->m_hDC, x+7, y+157, 15, 15, 15*level, 0, RGB(255,0,255));
	else if(PNum <= 3)
		bmp.TransDraw(pDC->m_hDC, x+10, y+5, 15, 15, 15*level, 0, RGB(255,0,255));	
	else
		bmp.TransDraw(pDC->m_hDC, x+173, y+5, 15, 15, 15*level, 0, RGB(255,0,255));
	bmp.DeleteObject();
	//////////////////////////////////////////////////////////////////////////////////////////
	
	int idx = 0; //= x+94;
	int idy = 0;// = y+118;
	// 아디 
	if(PNum == 0) {
		idx = x + 24;
		idy = y+161;
	} else if(PNum <= 3) {	
		idx = x + 26;
		idy = y +8;
	} else {
		idx = x +190 ;
		idy = y +8;
	}

	str.Format("%s", Play[PNum].UI.ID);
//	pDC->SetTextColor(RGB(10,10,10));
//	pDC->TextOut(idx-1, idy, str);
//	pDC->TextOut(idx, idy-1, str);
//	pDC->TextOut(idx+1, idy, str);
//	pDC->TextOut(idx, idy+1, str);
//	pDC->SetTextColor(RGB(10, 10, 10));
	pDC->SetTextColor(RGB(172,252,255));
	pDC->TextOut(idx, idy, str);
	
	CRect rect;
	CString strM = "";
	pDC->SetTextColor(RGB(244,255,75));
	// 나의 머니
	INT64 roundingoff = Play[PNum].UI.PMoney;
	roundingoff = (roundingoff/100)*100;

	strM = NumberToOrientalString(roundingoff);
	strM +=g_StrMan.Get(_T("DEFAULT_UNIT2"));

	if(PNum == 0) {
		rect.SetRect(0,0,200,14);
		rect.OffsetRect(590,416 + 21);
		pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK);
		
		// 총베팅액
		roundingoff = Game.GetRealBet();
		if(roundingoff > 100)
			roundingoff = (roundingoff/100)*100;
	
	//	nTotBat = 99999999999999999;
		int num1 = 0, num2 = 0, num3=0, num4=0;
		NumberToParse(roundingoff, num1, num2, num3, num4);
		rect.SetRect(0,0,33,14);
		rect.OffsetRect(340, 369+17);
		BOOL bZero = FALSE;

		if(num1>0) { bZero = TRUE; strM.Format("%d", num1); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num2>0 || bZero) { bZero = TRUE; strM.Format("%d", num2); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num3>0 || bZero) { bZero = TRUE; strM.Format("%d", num3); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num4>0 || bZero) { strM.Format("%d", num4); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }


		/* // 숫자로 그리기
		if(num1>0) { bZero = TRUE; DrawDigit(0, 1, num1); }
		rect.OffsetRect(45, 0);
		if(num2>0 || bZero) { bZero = TRUE; DrawDigit(0, 2, num2); }
		rect.OffsetRect(45, 0);
		if(num3>0 || bZero) { bZero = TRUE; DrawDigit(0, 3, num3); }
		rect.OffsetRect(45, 0);
		if(num4>0 || bZero) { bZero = TRUE; DrawDigit(0, 4, num4); }
		*/




		num1 = 0, num2 = 0, num3=0, num4=0;
		int pnum = Play[PNum].ServPNum;
		roundingoff = g_Poker.RU[pnum].nRealBat;

		if(roundingoff > 100)
			roundingoff = (roundingoff/100)*100;

	//	nTotBat = 99999000099990000;
		NumberToParse(roundingoff, num1, num2, num3, num4);
		rect.SetRect(0,0,33,14);
		rect.OffsetRect(340, 389+17);
		bZero = FALSE;
		if(num1>0 || bZero) { bZero = TRUE; strM.Format("%d", num1); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num2>0 || bZero) { bZero = TRUE; strM.Format("%d", num2); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num3>0 || bZero) { bZero = TRUE; strM.Format("%d", num3); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }
		rect.OffsetRect(45, 0);
		if(num4>0 || bZero) { bZero = TRUE; strM.Format("%d", num4); pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK); }

		// +- 증가액 표시
		roundingoff = Play[PNum].UI.PMoney - Play[PNum].PrevMoney;
		roundingoff = (roundingoff/100)*100;

		strM = NumberToOrientalString(roundingoff);//g_Poker.nTotBat); // 임시 추가

		CString str;
		str.Format("%I64d",roundingoff);

		if(roundingoff > 0)	{
			strM.Insert(0,"+");

			// 현재의 방에서 얻은 금액 표시 			
			rect.SetRect(0,0,200,14);
			rect.OffsetRect(590, 389);
			CString str1;
			str1 = strM + str;
			pDC->DrawText(str1, &rect, DT_RIGHT | DT_WORDBREAK);
		}else 
		{
			rect.SetRect(0,0,200,14);
			rect.OffsetRect(590, 389);
			CString str1;
			str1 = "0";
			pDC->DrawText(str1, &rect, DT_RIGHT | DT_WORDBREAK);
		}
	
		strM+=g_StrMan.Get(_T("DEFAULT_UNIT2"));
		rect.SetRect(0,0,200,14);
		rect.OffsetRect(590, 438+21);
		pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK);

	/*	//전적 표시
		pDC->SetTextColor(RGB(175,252,255));
		strM.Format("%d승%d패%d포기", Play[PNum].UI.WinNum, Play[PNum].UI.LooseNum, Play[PNum].UI.DrawNum);
		rect.OffsetRect(0, 23);
		pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK);
	*/
	} 
	else if( PNum <= 3)	{
		pDC->SetTextColor(RGB(172,252,255));
		rect.SetRect(0,0,180,14);
		rect.OffsetRect(x+76, y+7);
		pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK);
	}
	else {
		pDC->SetTextColor(RGB(172,252,255));
		rect.SetRect(0,0,180,14);
		rect.OffsetRect(x-10, y+7);
		pDC->DrawText(strM, &rect, DT_RIGHT | DT_WORDBREAK);
	}

	// ### [ 중복된 IP가 ] ###
	if(Play[PNum].MyIpCheck >= 0)
	{
		int imageIndex = 2;//Play[PNum].MyIpCheck * 2;
		if(PNum==0) pPage->PutSprAuto(Xp+5, Yp+25+5, &WarningSpr, ( Game.GameCnt/15 )%2+ imageIndex);
		else if(PNum<3) pPage->PutSprAuto(Xp+5, Yp+45, &WarningSpr, ( Game.GameCnt/15 )%2+ imageIndex);
		else pPage->PutSprAuto(Xp+180, Yp+45, &WarningSpr, ( Game.GameCnt/15 )%2+imageIndex, LR);
	}

	// 방장 마크 그리기
	if(strcmp(g_RI.ID, Play[PNum].UI.ID)==0)
	{
		if( g_RI.bSuperMaster ) {
			if(PNum==0) pPage->PutSprAuto(Xp+5, Yp+5, g_pGameView->m_pgamebackspr, 11+(Game.GameCnt/10)%5);
			else if(PNum<=3) pPage->PutSprAuto(Xp, Yp+20, g_pGameView->m_pgamebackspr, 11+(Game.GameCnt/10)%5);
			else pPage->PutSprAuto(Xp+180, Yp+20, g_pGameView->m_pgamebackspr, 11+(Game.GameCnt/10)%5);
		}
		else {
			if(PNum==0) pPage->PutSprAuto(Xp+5, Yp+5, &EtcSpr, (Game.GameCnt/10)%4);
			else if(PNum<=3) pPage->PutSprAuto(Xp, Yp+20, &EtcSpr, (Game.GameCnt/10)%4);
			else pPage->PutSprAuto(Xp+180, Yp+20, &EtcSpr, (Game.GameCnt/10)%4);
		}
	}
}











































