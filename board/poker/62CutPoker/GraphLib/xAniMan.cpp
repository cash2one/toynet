//
// 에니메이션 관리 클래스
//

#include "StdAfx.h"
#include "xAniMan.h"

PLAYSTUFF xAniMan::Ps[100]; // for static member

xAniMan::xAniMan()
{
	Clear();
}

xAniMan::~xAniMan()
{
}

void xAniMan::Clear()
{
	//pSpr=NULL

	pAni = NULL;
	pChar = NULL;
	pGroup = NULL;
	pAction = NULL;
	pFrame = NULL;
	pCell = NULL;

	nNowChar = nNowGroup = nNowAction = nNowFrame = nNowCell = 0;
	nCharNum = nGroupNum = nActionNum = nFrameNum = nCellNum = 0;

	bLoop = TRUE;
	bForward = TRUE;

	bPause = FALSE;

	iDelayCnt = 0;
}

/*----------------------------------------------------------------------
	에니메이션 관리 클래스 초기화
-----------------------------------------------------------------------*/
BOOL xAniMan::Init(xSprite *pSprite)
{
	Clear();
	if(pSprite) pSpr = pSprite;
	else return FALSE;
	pAni = &pSprite->ani;

	SetCharacter(0);
	//SetGroup(0);
	//SetFrame(0);
	//SetCell(0);

	return TRUE;
}

BOOL xAniMan::Init(xAniFile* pAniFile)
{
	//Clear();
	if(pAniFile) pAni = pAniFile;
	else return FALSE;

	SetCharacter(0);
	SetGroup(0);
	SetFrame(0);
	SetCell(0);

	return TRUE;
}


/*----------------------------------------------------------------------
	현재 지정된 액션으로 에니메이션을 플레이한다
	Return Value : 실패시 0을 리턴
	               성공시 0이 아닌값을 리턴
-----------------------------------------------------------------------*/
int xAniMan::PlayAction(CPage *Page, BOOL bNext, int x, int y, int opt, int grade, WORD color)
{
	int CellNum, i;

	BOOL oldPause = bPause;
	if(bNext==FALSE) bPause=TRUE;

	if(pSpr==NULL) return 0;
	if(GetPlayStuff(Ps, &CellNum)==FALSE) return 0;
	for(i=0; i<CellNum; i++)
	{
		// 여기에 스프라이트를 찍는 루틴이 들어간다
		//xDraw::PutSprite(x+Ps[i].Rx, y+Ps[i].Ry, &pSpr->spr[Ps[i].SprNo]);
		Page->PutSprAuto(x+Ps[i].Rx, y+Ps[i].Ry, &pSpr->spr[Ps[i].SprNo], opt, grade, color);
		
		//xDraw::PutSprite(x+Ps[i].Rx, y+Ps[i].Ry, pSpr, Ps[i].SprNo);
	}

	bPause=oldPause;
	return 1;
}

/*----------------------------------------------------------------------
	현재 지정된 액션으로 에니메이션을 플레이한다(레이어에 등록시켜준다)
	Return Value : 실패시 0을 리턴
	               성공시 0이 아닌값을 리턴
-----------------------------------------------------------------------*/
int xAniMan::PlayAction( xLayer *pLay, int x, int y, int opt, int grade, WORD color)
{
	int CellNum, i;

	if(pSpr==NULL) return 0;
	if(GetPlayStuff(Ps, &CellNum)==FALSE) return 0;
	for(i=0; i<CellNum; i++)
	{
		pLay->Set(x, y, Ps[i].Rx, Ps[i].Ry, pSpr, Ps[i].SprNo);
	}

	return 1;
}



/*----------------------------------------------------------------------
	현재 플레이될 스프라이트 정보를 구한다(다음 순서가 진행된다 )
	Return Value : 실패시 FALSE를 리턴
	               성공시 TRUE를 리턴
-----------------------------------------------------------------------*/
BOOL xAniMan::GetPlayStuff(PLAYSTUFF* Ps, int* cellnum)
{
	if(pChar==NULL || pAction==NULL || pFrame==NULL || pCell==NULL) return FALSE;

	int i;

	*cellnum = pFrame->TotCellNum;
	
	// PLAYSTUFF구조체에 찍어줄 스프라이트 정보를 채운다
	for(i=0; i<pFrame->TotCellNum; i++)
	{
		SetCell(i);
		Ps[i].SprNo = pCell->SprNo;
		Ps[i].Rx	= pCell->Rx;
		Ps[i].Ry	= pCell->Ry;
		Ps[i].Flag	= pCell->OverrideFlag;
	}

	if(bPause) return TRUE; // 에니메이션이 일시 중지 중이면 리턴
	
	if(iDelayCnt==0)	// 지연이 끝났으면
	{
		if(bForward)		// 순차 플레이
		{
			nNowFrame++;
			if(nNowFrame >= nFrameNum) 
			{
				if(bLoop==FALSE) nNowFrame = nFrameNum-1;
				else nNowFrame = 0;
			}
		}
		else				// 역순 플레이
		{
			nNowFrame--;
			if(nNowFrame < 0) 
			{
				if(bLoop==FALSE) nNowFrame = 0;
				else nNowFrame = nFrameNum-1;
			}
		}

		// 다음 프레임을 지정한다
		if(SetFrame(nNowFrame))
		{
			iDelayCnt = pFrame->Delay;
			//SetCell(0);
		}
		
		return TRUE;
	}
	else		// 지연이 끝나지 않았으면
	{ 
		if(iDelayCnt>0) iDelayCnt--;
	}
	
	return TRUE;
}

/*----------------------------------------------------------------------
	에니메이션 플레이를 포즈 시킨다
-----------------------------------------------------------------------*/
void xAniMan::Pause()
{
	bPause = TRUE;
}

/*----------------------------------------------------------------------
	에니메이션 플레이를 재개 시킨다
-----------------------------------------------------------------------*/
void xAniMan::Resume()
{
	bPause = FALSE;
}


//======================= 에니메이션 플레이 옵션 설정 함수 ========================

// 무한 루프 플레이 설정
void xAniMan::SetLoop(BOOL loop)
{
	bLoop = loop;
}

// 순차 플레이 설정
void xAniMan::SetForward()
{
	bForward = TRUE;
}

// 역순 플레이 설정
void xAniMan::SetBackward()
{
	bForward = FALSE;
}


//=============================== 저수준 제어 =================================

// 캐릭터 노드 초기화
void xAniMan::ResetCharacter(BOOL resetchild)
{
	SetCharacter(0, FALSE);
	nNowGroup = 0;
	if(resetchild) ResetGroup();
}

// 그룹 노드 초기화
void xAniMan::ResetGroup(BOOL resetchild)
{
	SetGroup(0, FALSE);
	nNowAction = 0;
	if(resetchild) ResetAction();
}

// 액션 노드 초기화
void xAniMan::ResetAction(BOOL resetchild)
{
	SetAction(0, FALSE);
	nNowFrame = 0;
	iDelayCnt = 0;
	if(resetchild) ResetFrame();
}

// 프레임 노드 초기화
void xAniMan::ResetFrame(BOOL resetchild)
{
	SetFrame(0, FALSE);
	nNowCell = 0;
	if(resetchild) ResetCell();
}

// 셀 노드 초기화
void xAniMan::ResetCell()
{
	SetCell(0);
}

// 캐릭터를 지정(클래스가 초기화 되어 있어야한다)
BOOL xAniMan::SetCharacter(int num, BOOL reset)
{
	if(pAni == NULL) return FALSE;
	pChar = pAni->GetPtrCharacter(num);
	if(pChar == NULL) return FALSE;
	nGroupNum = pChar->TotGroupNum;
	nNowChar = num;
	if(reset) ResetGroup(TRUE); // 하위 초기화
	return TRUE;
}

BOOL xAniMan::SetCharacter(char* name, BOOL reset)
{
	if(pAni == NULL) return FALSE;
	pChar = pAni->GetPtrCharacter(name, &nNowChar);
	if(pChar == NULL) return FALSE;
	nGroupNum = pChar->TotGroupNum;
	if(reset) ResetGroup(TRUE); // 하위 초기화
	return TRUE;
}

// 그룹을 지정(캐릭터가 지정되어 있어야 한다)
BOOL xAniMan::SetGroup(int num, BOOL reset)
{
	if(pChar == NULL) return FALSE;
	pGroup = pChar->GetPtrGroup(num);
	if(pGroup == NULL) return FALSE;
	nActionNum = pGroup->TotActionNum;
	nNowGroup = num;
	if(reset) ResetAction(TRUE); // 하위 초기화
	return TRUE;
}

BOOL xAniMan::SetGroup(char* name, BOOL reset)
{
	if(pChar == NULL) return FALSE;
	pGroup = pChar->GetPtrGroup(name, &nNowGroup);
	if(pGroup == NULL) return FALSE;
	nActionNum = pGroup->TotActionNum;
	if(reset) ResetAction(TRUE); // 하위 초기화
	return TRUE;
}

// 액션을 지정(그룹이 지정되어 있어야 한다)
BOOL xAniMan::SetAction(int num, BOOL reset)
{
	if(pGroup == NULL) return FALSE;
	pAction = pGroup->GetPtrAction(num);
	if(pAction == NULL) return FALSE;
	nFrameNum = pAction->TotFrameNum;
	nNowAction = num;
	if(reset) ResetFrame(); // 하위 초기화
	return TRUE;
}

BOOL xAniMan::SetAction(char* name, BOOL reset)
{
	if(pGroup == NULL) return FALSE;
	pAction = pGroup->GetPtrAction(name, &nNowAction);
	if(pAction == NULL) return FALSE;
	nFrameNum = pAction->TotFrameNum;
	if(reset) ResetFrame(); // 하위 초기화
	return TRUE;
}

// 프레임을 지정(액션이 지정되어 있어야 한다)
BOOL xAniMan::SetFrame(int num, BOOL reset)
{
	if(pAction == NULL) return FALSE;
	pFrame = pAction->GetPtrFrame(num);
	if(pFrame == NULL) return FALSE;
	nCellNum = pFrame->TotCellNum;
	if(reset) ResetCell(); // 하위 초기화
	nNowFrame = num;
	
	iDelayCnt = pFrame->Delay;
	return TRUE;
}

// 셀을 지정(프레임이 지정되어 있어야 한다)
BOOL xAniMan::SetCell(int num)
{
	if(pFrame == NULL) return FALSE;
	pCell = pFrame->GetPtrCell(num);
	if(pCell == NULL) return FALSE;
	nNowCell = num;
	return TRUE;
}

//========================== 에니메이션 정보 얻기 =================================

// 프레임의 정보를 얻는다(액션이 지정되어 있어야 한다)
BOOL xAniMan::GetNowFrameInfo(int* delay, int* totcell)
{
	if(pFrame==NULL) return FALSE;
	*delay	= pFrame->Delay;
	*totcell= pFrame->TotCellNum;
	return TRUE;
}
// Override Function
BOOL xAniMan::GetNowFrameInfo(int FrameNum, int* delay, int* totcell)
{
	if(pAction==NULL) return FALSE;
	if(FrameNum<0 || FrameNum>= pAction->TotFrameNum) return FALSE;
	*delay	= (pAction->ppFrame[FrameNum])->Delay;
	*totcell= (pAction->ppFrame[FrameNum])->TotCellNum;
	return TRUE;
}

// 현재 셀의 정보를 얻는다(프레임이 지정되어 있어야 한다)
BOOL xAniMan::GetNowCellInfo(int* sprno, int* rx, int* ry, int* flag)
{
	if(pFrame==NULL) return FALSE;
	if(nNowCell<0 || nNowCell>= pFrame->TotCellNum) return FALSE;
	*sprno	= (pFrame->ppCell[nNowCell])->SprNo;
	*rx		= (pFrame->ppCell[nNowCell])->Rx;
	*ry		= (pFrame->ppCell[nNowCell])->Ry;
	*flag	= (pFrame->ppCell[nNowCell])->OverrideFlag;
	return TRUE;
}
// Override Function(특정 번호의 셀 정보를 얻는다)
BOOL xAniMan::GetNowCellInfo(int CellNum, int* sprno, int* rx, int* ry, int* flag)
{
	if(pFrame==NULL) return FALSE;
	if(CellNum<0 || CellNum>= pFrame->TotCellNum) return FALSE;
	*sprno	= (pFrame->ppCell[CellNum])->SprNo;
	*rx		= (pFrame->ppCell[CellNum])->Rx;
	*ry		= (pFrame->ppCell[CellNum])->Ry;
	*flag	= (pFrame->ppCell[CellNum])->OverrideFlag;
	return TRUE;
}
// Override Function(현재 프레임 전체의 셀 정보를 얻는다)
BOOL xAniMan::GetNowCellInfo(PLAYSTUFF* Ps, int* totcell)
{
	if(pFrame==NULL) { *totcell = 0; return FALSE;}
	if(pFrame->TotCellNum < 1) { *totcell = 0; return FALSE;}
	if(pFrame->ppCell==NULL) { *totcell = 0; return FALSE;}
	*totcell = pFrame->TotCellNum;
	for(int i=0; i<pFrame->TotCellNum; i++)
	{
		Ps[i].SprNo	= (pFrame->ppCell[i])->SprNo;
		Ps[i].Rx	= (pFrame->ppCell[i])->Rx;
		Ps[i].Ry	= (pFrame->ppCell[i])->Ry;
		Ps[i].Flag	= (pFrame->ppCell[i])->OverrideFlag;
	}
	return TRUE;
}
