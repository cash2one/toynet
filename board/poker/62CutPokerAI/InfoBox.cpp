// InfoBox.cpp: implementation of the CInfoBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "InfoBox.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfoBox::CInfoBox()
{
	NowMarble = 0;
	pParent = NULL;
	pPage = NULL;
	bShow = FALSE;
	Xp = Yp = 0;
	Width = 0;
	Height = 0;

	bMove = FALSE;
	ClickX = ClickY = 0;

	bSize = FALSE;
	OrgWidth = OrgHeight = 0;
}

CInfoBox::~CInfoBox()
{

}

void CInfoBox::Init(CWnd *pWnd, CPage *ppage)
{
	pParent = pWnd;
	pPage = ppage;
	bShow = TRUE;

	Xp = 662;
	Yp = 462;
	Width = 800 - Xp;
	Height = 591 - Yp;//158;

	//Xp = 315;
	//Yp = 5;
//	Xp = 662;
//	Yp = 462;
//	Xp = 300;
//	Yp = 157;
//	Width = 200;
//	Height = 200;

	// 폰트 생성
	if(Font.m_hObject == NULL)
	Font.CreateFont(12,0,0,0,
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

	// 폰트 생성
	if(Font2.m_hObject == NULL)
	Font2.CreateFont(12,0,0,0,
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

	CRect rc;
	// 채팅 뷰 리치에디트 컨트롤 생성
	rc.SetRect(Xp+3, Yp+22, Xp+Width-3, Yp+Height-13);
	ChatViewEdit.Create(ES_AUTOVSCROLL|ES_WANTRETURN|ES_MULTILINE, rc, pParent, 124);
	ChatViewEdit.SetFont(&Font);
	// 채팅 뷰 리치에디트를 안보이는 곳으로 옮겨둔다
	ChatViewEdit.GetClientRect(rc);
	rc.OffsetRect(-2000, 0);
	ChatViewEdit.MoveWindow(rc);
	ChatViewEdit.ShowWindow(SW_SHOW);
}

BOOL CInfoBox::OnLButtonDown(int x, int y)
{
	// 이동 바 눌림 체크
	if(x >= Xp && x <= Xp+Width && y >= Yp && y <= Yp+16) bMove = TRUE;
	else bMove = FALSE;
	// 사이즈 조정 버튼 눌림 체크
	if(x >= Xp+Width-10 && x <= Xp+Width && y >= Yp+Height-10 && y <= Yp+Height){ bSize = TRUE; bMove=FALSE; }
	else bSize = FALSE;

	if(bMove==FALSE && bSize==FALSE) return FALSE;
	if(bMove) { ClickX=x-Xp; ClickY=y-Yp; }
	if(bSize) { ClickX=x; ClickY=y; OrgWidth=Width; OrgHeight=Height; }
	ChatViewEdit.ShowWindow(SW_HIDE);
	return TRUE;
}

void CInfoBox::OnLButtonUp()
{
	if(bMove || bSize)
	{
		CRect rc;
		// 채팅 뷰 리치에디트를 안보이는 곳으로 옮겨둔다
		ChatViewEdit.GetClientRect(rc);
		rc.OffsetRect(-3000, 0);
		ChatViewEdit.MoveWindow(rc);
		ChatViewEdit.ShowWindow(SW_SHOW);

		// 센드 에디트를 안보이는 곳으로 옮겨둔다
		SendEdit.GetClientRect(rc);
		rc.OffsetRect(-2000, 0);
		SendEdit.MoveWindow(rc);
		SendEdit.ShowWindow(SW_SHOW);
		SendEdit.SetFocus();
	}
	bMove = FALSE;
	bSize = FALSE;
}


void CInfoBox::OnMouseMove(int x, int y)
{
	if(bMove == FALSE && bSize == FALSE) return;
	if(pPage == NULL) return;

	
	if(bMove) // 이동하는 중이면
	{
		x -= ClickX;
		y -= ClickY;

		if(x < 0) Xp=0;
		else if(x+Width > pPage->width-1) Xp = pPage->width - Width - 1;
		else Xp=x;

		if(y < 0) Yp=0;
		else if(y+Height > pPage->height-1) Yp = pPage->height - Height - 1;
		else Yp=y;
	}
	else if(bSize) // 사이즈를 조정하는 중이면
	{
		if(x < Xp+70) x = Xp+70;
		else if(x >= pPage->width-2) x = pPage->width-2;
		if(y < Yp+55) y = Yp+55;
		else if(y >= pPage->height-2) y = pPage->height-2;

		int dx = x - ClickX;
		int dy = y - ClickY;

		if(OrgWidth+dx > 300) dx=300-OrgWidth;

		Width = OrgWidth+dx;
		Height = OrgHeight+dy;

		// 채팅뷰의 사이즈를 조정한다.
		CRect rc;
		rc.SetRect(Xp+3, Yp+20, Xp+Width-3, Yp+Height-18);
		ChatViewEdit.MoveWindow(rc, TRUE);
	}
}


void CInfoBox::Draw(CDC *pDC)
{
	// 채팅박스 배경 그리기
	if(pPage == NULL) return;
	if(bShow == FALSE) return;
	pPage->DrawFillBoxGB(Xp, Yp, Xp+Width-1, Yp+Height, RGBmix(0,2,5), 8);
	
	//SetClipArea(Xp, Yp, Xp+Width-1, Yp+20);
	//_PutCsp( Xp, Yp, ChatBoxSpr.spr[0].xl, ChatBoxSpr.spr[0].yl, pPage->lpBit, ChatBoxSpr.spr[0].data );
	//pPage->PutSprAuto(Xp+Width-3, Yp, &ChatBoxSpr, 1);
	//pPage->PutSprAuto(Xp+Width-12, Yp+Height-12, &ChatBoxSpr, 2);

	pPage->PutSprAuto(Xp, Yp, &InfoBoxSpr, 0);

	pPage->PutSprAuto(Xp, Yp+Height-14, &InfoBoxSpr, 1);

	CRect rc;
	rc.SetRect(Xp+50, Yp+5, Xp+Width-3, Yp+18);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(81, 13, 13));
	pDC->SelectObject(&Font2);

	CString str;
	//str.Format("%d 마블",Play[0].UI.Marble);
//#ifdef UPGRADE1
	str.Format("%I64d 원", NowMarble );
//#else
//	str.Format("%d 마블", NowMarble);
//#endif

	rc.OffsetRect(0,1);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->DrawText(str, &rc, DT_RIGHT|DT_SINGLELINE);
	rc.OffsetRect(0,-2);
	pDC->DrawText(str, &rc, DT_RIGHT|DT_SINGLELINE);
	rc.OffsetRect(1,1);
	pDC->DrawText(str, &rc, DT_RIGHT|DT_SINGLELINE);
	rc.OffsetRect(-2,0);
	pDC->DrawText(str, &rc, DT_RIGHT|DT_SINGLELINE);

	rc.OffsetRect(1,0);

	pDC->SetTextColor(RGB(230, 230, 230));
	pDC->DrawText(str, &rc, DT_RIGHT|DT_SINGLELINE);

/*
	pDC->SetTextColor(RGB(240, 240, 240));
	pDC->DrawText(str, &rc, DT_RIGHT|DT_SINGLELINE);
	rc.OffsetRect(2,0);
*/

/*
	// 누군가 흔들거나 총통을 했다면 흔듬/총통 표시상자 그리기
	if(Game.PlayerShakeCnt>0)
	{
		rc.SetRect(Xp+3, Yp+24, Xp+Width, Yp+20+15);
		pDC->SelectObject(&Font);
		
		if(Game.ShakeKind==2) // 총통
		{
			for(int i=0; i<4; i++) pPage->PutSprAuto(rc.left+5 + i*40, rc.top+11, &HwatuNSpr, Game.ShakeCardNo[i]);
			str.Format("%s님 총통!!", Play[Game.ShakePlayerNum].UI.ID);
			pDC->SetTextColor(RGB(0,0,0));
			pDC->DrawText(str, &rc, DT_CENTER);
			rc.OffsetRect(-1,-1);
			pDC->SetTextColor(RGB(180,255,190));
			pDC->DrawText(str, &rc, DT_CENTER);
		}
		else // 흔듬, 폭탄
		{
			for(int i=0; i<3; i++) pPage->PutSprAuto(rc.left+20 + i*45, rc.top+11, &HwatuNSpr, Game.ShakeCardNo[i]);
			if(Game.ShakeKind == 0) str.Format("%s님 흔듬!!", Play[Game.ShakePlayerNum].UI.ID);
			else str.Format("%s님 폭탄!!", Play[Game.ShakePlayerNum].UI.ID);
			pDC->SetTextColor(RGB(0,0,0));
			pDC->DrawText(str, &rc, DT_CENTER);
			rc.OffsetRect(-1,-1);
			if(Game.ShakeKind == 0) pDC->SetTextColor(RGB(190,180,255));
			else pDC->SetTextColor(RGB(255,180,190));
			pDC->DrawText(str, &rc, DT_CENTER);
		}
	}
	else
*/
	{
		// 정보 문자열 그리기
		int totline = ChatViewEdit.GetLineCount();
		if(totline==0) return;
		int y=20;

		pDC->SetTextColor(RGB(255, 200, 220));
		pDC->SelectObject(&Font);

		rc.SetRect(Xp+3, Yp+21, Xp+Width, Yp+20+15);
		int wline = ((Height-18) / 14)+1;
		if(wline>0)
		for(int i=totline-wline; i<totline; i++)
		{
			if(i>=0)
			{
				// 윈도2000과 일부 98 시스템에서 문자가 깨지는걸 방지하기 위해 처리
				char buf[256]={0,};
				ChatViewEdit.GetLine(i, buf, 255);
				buf[ChatViewEdit.LineLength(ChatViewEdit.LineIndex(i))]=0;

				int slen = strlen(buf);
				if(slen>0)
				{
					// 윈도 2000의 경우 문장의 끝이 제대로 끊어지지 않음
					for(int k=0; k<slen; k++)
					{
						if(buf[k]==10) buf[k]=0;
						if(buf[k]==-1) buf[k]=0;
						if(buf[k]==13) buf[k]=0;
					}
					buf[slen]=0;

					// 첫번째 글자에 따라 라인단위로 색상을 변경한다
					int nindex = ChatViewEdit.LineIndex(i);
					CHARFORMAT cf;
					cf.cbSize = sizeof(CHARFORMAT);
					cf.dwMask = CFM_COLOR;
					ChatViewEdit.SetSel(nindex, nindex);
					ChatViewEdit.GetSelectionCharFormat(cf);
				
					// 만일 검정색이면 밝은 분홍색으로 찍어주고 다른색이라면 그색 그대로 세팅
					if(cf.crTextColor==0) pDC->SetTextColor(RGB(255, 200, 220));
					else pDC->SetTextColor(cf.crTextColor);

					str.Format("%s", buf);
					pDC->DrawText(str, &rc, DT_LEFT);
				}

				rc.OffsetRect(0, 14);
			}
		}
	}
//	*/
}

