// CharacterView.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "CharView.h"
#include "Global.h"
#include "ProfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHARVIEWREPAINT_TIMER	3234

/////////////////////////////////////////////////////////////////////////////
// CCharView

CCharView::CCharView()
{
	UseMode = 0;
	CharNum = 0;
	bNowSeleted = FALSE;
	ZeroMemory(&UI, sizeof(USERINFO));
}

CCharView::~CCharView()
{
}


BEGIN_MESSAGE_MAP(CCharView, CWnd)
	//{{AFX_MSG_MAP(CCharView)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCharView message handlers

void CCharView::Create(CWnd *pParentWnd, CRect &rect, int usemode, int nID)
{
	Page.Init(rect.Width(), rect.Height(), 16);
	UseMode = usemode;
	CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID);
	ShowWindow(SW_HIDE);

	SetTimer(CHARVIEWREPAINT_TIMER, 100, NULL);
}

/*
void CCharView::SetCharacter(int charnum)
{
	CharNum = charnum;
	Invalidate(FALSE);
}
*/

void CCharView::SetCharacter(USERINFO *pUI)
{
	memcpy(&UI, pUI, sizeof(USERINFO));
	Avatar.SetAvatar(pUI->AvatarInfo);
	// 랜덤하게 에니메이션 시간을 설정
	Avatar.SetAnimationTime((WORD)rand());
}


void CCharView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(Page.lpBit == NULL) return;
	
	// TODO: Add your message handler code here
	//Page.DrawFillBox(0,0, Page.width, Page.height, BColor);

	switch(UseMode)
	{
	case 0:	// 대기실용
		{
			Page.PutSprAuto(0, 0, &CharBackSpr, 2);
			//Page.PutSprAuto(0,0, &LCharSpr, CharNum);
			Avatar.Draw(0, 0, Page.width, Page.height, (WORD*)Page.lpBit, Page.lPitch, FALSE);
		}
		break;
	case 1:	// 프로필 보기용
		{
			Page.PutSprAuto(0, 0, &CharBackSpr, 0);
			//Page.PutSprAuto(30,15, &LCharSpr, CharNum);
			Avatar.Draw(0, 3, Page.width, Page.height, (WORD*)Page.lpBit, Page.lPitch, FALSE);
		}
		break;
	case 2:	// 캐릭터 변경창(크게)
		{
			//Page.PutSprAuto(0,0, &CharBackSpr, 2);
			//Page.PutSprAuto(30,15, &LCharSpr, CharNum);
		}
		break;
	case 3:	// 캐릭터 변경창(작음)
		{
			//if(bNowSeleted)	Page.DrawFillBox(0,0, Page.width, Page.height, RGBmix(31,25,10));
			//else Page.DrawFillBox(0,0, Page.width, Page.height, RGBmix(31,31,31));
			//Page.PutSprAuto(0,0, &SCharSpr, CharNum);
		}
		break;
	case 4:	// 참여자 정보창(작음)
		{
			Page.PutSprAuto(0,0, &CharBackSpr, 1);
			if(strlen(UI.ID) > 0) {
				//Page.PutSprAuto(3,3, &SCharSpr, UI.Character);
				Avatar.Draw(0, 0, Page.width, Page.height, (WORD*)Page.lpBit, Page.lPitch, FALSE);
			}
		}
		break;
	case 5: //  하이 결과창
		{
		//	if( g_MyObserver )
		//		Page.PutSprAuto(0, 0, &CharBackSpr, 2);
		//	else if(Play[0].ServPNum == Game.WinnerPNum)
		//		Page.PutSprAuto(0, 0, &CharBackSpr, 0);
		//	else
				Page.PutSprAuto(0, 0, &CharBackSpr, 0);

			//Page.PutSprAuto(30,15, &LCharSpr, CharNum);
			Avatar.Draw(2, 0, Page.width, Page.height, (WORD*)Page.lpBit, Page.lPitch, TRUE);
		}
		break;
	case 6:
		{// 파산
			static char charid = -1;
			Page.PutSprAuto(0,0, &CharBackSpr, 3);
			if( charid < 0 )
			{
				charid = rand()%3;
				if( Play[0].UI.Sex == 0 ) charid += 3;
			}
			Page.PutSprAuto(0,0, &OringCharSpr, charid);
		}
		break;
	}

	Page.FlipToGDI(dc.m_hDC);

	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CCharView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(UseMode==0 || UseMode==3)
	{ 
		SetCursor(g_pGameView->hCurHand);
		return TRUE; 
	}

	if(UseMode==4)
	{ 
		if(strlen(UI.ID) > 0)
		{
			SetCursor(g_pGameView->hCurHand);
			return TRUE; 
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CCharView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(UseMode == 0)
	{
		//GetParent()->SendMessage(WM_COMMAND, GetDlgCtrlID(), (long)GetSafeHwnd());
		
		// 사운드 출력
		Sound.Play(SND45);

		CProfileDlg Dlg;
		Dlg.Init(&Play[0].UI);
		Dlg.DoModal();
	}
	if(UseMode == 3)
	{
		GetParent()->SendMessage(WM_COMMAND, GetDlgCtrlID(), (long)GetSafeHwnd());
	}
	if(UseMode == 4)
	{
		if(strlen(UI.ID) > 0)
		{
			// 사운드 출력
			Sound.Play(SND45);

			CProfileDlg Dlg;
			Dlg.Init(&UI);
			Dlg.DoModal();
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CCharView::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(CHARVIEWREPAINT_TIMER);
}

void CCharView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate(FALSE);
	CWnd::OnTimer(nIDEvent);
}
