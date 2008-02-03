//
// ���ϸ��̼� ���� Ŭ����
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
	���ϸ��̼� ���� Ŭ���� �ʱ�ȭ
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
	���� ������ �׼����� ���ϸ��̼��� �÷����Ѵ�
	Return Value : ���н� 0�� ����
	               ������ 0�� �ƴѰ��� ����
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
		// ���⿡ ��������Ʈ�� ��� ��ƾ�� ����
		//xDraw::PutSprite(x+Ps[i].Rx, y+Ps[i].Ry, &pSpr->spr[Ps[i].SprNo]);
		Page->PutSprAuto(x+Ps[i].Rx, y+Ps[i].Ry, &pSpr->spr[Ps[i].SprNo], opt, grade, color);
		
		//xDraw::PutSprite(x+Ps[i].Rx, y+Ps[i].Ry, pSpr, Ps[i].SprNo);
	}

	bPause=oldPause;
	return 1;
}

/*----------------------------------------------------------------------
	���� ������ �׼����� ���ϸ��̼��� �÷����Ѵ�(���̾ ��Ͻ����ش�)
	Return Value : ���н� 0�� ����
	               ������ 0�� �ƴѰ��� ����
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
	���� �÷��̵� ��������Ʈ ������ ���Ѵ�(���� ������ ����ȴ� )
	Return Value : ���н� FALSE�� ����
	               ������ TRUE�� ����
-----------------------------------------------------------------------*/
BOOL xAniMan::GetPlayStuff(PLAYSTUFF* Ps, int* cellnum)
{
	if(pChar==NULL || pAction==NULL || pFrame==NULL || pCell==NULL) return FALSE;

	int i;

	*cellnum = pFrame->TotCellNum;
	
	// PLAYSTUFF����ü�� ����� ��������Ʈ ������ ä���
	for(i=0; i<pFrame->TotCellNum; i++)
	{
		SetCell(i);
		Ps[i].SprNo = pCell->SprNo;
		Ps[i].Rx	= pCell->Rx;
		Ps[i].Ry	= pCell->Ry;
		Ps[i].Flag	= pCell->OverrideFlag;
	}

	if(bPause) return TRUE; // ���ϸ��̼��� �Ͻ� ���� ���̸� ����
	
	if(iDelayCnt==0)	// ������ ��������
	{
		if(bForward)		// ���� �÷���
		{
			nNowFrame++;
			if(nNowFrame >= nFrameNum) 
			{
				if(bLoop==FALSE) nNowFrame = nFrameNum-1;
				else nNowFrame = 0;
			}
		}
		else				// ���� �÷���
		{
			nNowFrame--;
			if(nNowFrame < 0) 
			{
				if(bLoop==FALSE) nNowFrame = 0;
				else nNowFrame = nFrameNum-1;
			}
		}

		// ���� �������� �����Ѵ�
		if(SetFrame(nNowFrame))
		{
			iDelayCnt = pFrame->Delay;
			//SetCell(0);
		}
		
		return TRUE;
	}
	else		// ������ ������ �ʾ�����
	{ 
		if(iDelayCnt>0) iDelayCnt--;
	}
	
	return TRUE;
}

/*----------------------------------------------------------------------
	���ϸ��̼� �÷��̸� ���� ��Ų��
-----------------------------------------------------------------------*/
void xAniMan::Pause()
{
	bPause = TRUE;
}

/*----------------------------------------------------------------------
	���ϸ��̼� �÷��̸� �簳 ��Ų��
-----------------------------------------------------------------------*/
void xAniMan::Resume()
{
	bPause = FALSE;
}


//======================= ���ϸ��̼� �÷��� �ɼ� ���� �Լ� ========================

// ���� ���� �÷��� ����
void xAniMan::SetLoop(BOOL loop)
{
	bLoop = loop;
}

// ���� �÷��� ����
void xAniMan::SetForward()
{
	bForward = TRUE;
}

// ���� �÷��� ����
void xAniMan::SetBackward()
{
	bForward = FALSE;
}


//=============================== ������ ���� =================================

// ĳ���� ��� �ʱ�ȭ
void xAniMan::ResetCharacter(BOOL resetchild)
{
	SetCharacter(0, FALSE);
	nNowGroup = 0;
	if(resetchild) ResetGroup();
}

// �׷� ��� �ʱ�ȭ
void xAniMan::ResetGroup(BOOL resetchild)
{
	SetGroup(0, FALSE);
	nNowAction = 0;
	if(resetchild) ResetAction();
}

// �׼� ��� �ʱ�ȭ
void xAniMan::ResetAction(BOOL resetchild)
{
	SetAction(0, FALSE);
	nNowFrame = 0;
	iDelayCnt = 0;
	if(resetchild) ResetFrame();
}

// ������ ��� �ʱ�ȭ
void xAniMan::ResetFrame(BOOL resetchild)
{
	SetFrame(0, FALSE);
	nNowCell = 0;
	if(resetchild) ResetCell();
}

// �� ��� �ʱ�ȭ
void xAniMan::ResetCell()
{
	SetCell(0);
}

// ĳ���͸� ����(Ŭ������ �ʱ�ȭ �Ǿ� �־���Ѵ�)
BOOL xAniMan::SetCharacter(int num, BOOL reset)
{
	if(pAni == NULL) return FALSE;
	pChar = pAni->GetPtrCharacter(num);
	if(pChar == NULL) return FALSE;
	nGroupNum = pChar->TotGroupNum;
	nNowChar = num;
	if(reset) ResetGroup(TRUE); // ���� �ʱ�ȭ
	return TRUE;
}

BOOL xAniMan::SetCharacter(char* name, BOOL reset)
{
	if(pAni == NULL) return FALSE;
	pChar = pAni->GetPtrCharacter(name, &nNowChar);
	if(pChar == NULL) return FALSE;
	nGroupNum = pChar->TotGroupNum;
	if(reset) ResetGroup(TRUE); // ���� �ʱ�ȭ
	return TRUE;
}

// �׷��� ����(ĳ���Ͱ� �����Ǿ� �־�� �Ѵ�)
BOOL xAniMan::SetGroup(int num, BOOL reset)
{
	if(pChar == NULL) return FALSE;
	pGroup = pChar->GetPtrGroup(num);
	if(pGroup == NULL) return FALSE;
	nActionNum = pGroup->TotActionNum;
	nNowGroup = num;
	if(reset) ResetAction(TRUE); // ���� �ʱ�ȭ
	return TRUE;
}

BOOL xAniMan::SetGroup(char* name, BOOL reset)
{
	if(pChar == NULL) return FALSE;
	pGroup = pChar->GetPtrGroup(name, &nNowGroup);
	if(pGroup == NULL) return FALSE;
	nActionNum = pGroup->TotActionNum;
	if(reset) ResetAction(TRUE); // ���� �ʱ�ȭ
	return TRUE;
}

// �׼��� ����(�׷��� �����Ǿ� �־�� �Ѵ�)
BOOL xAniMan::SetAction(int num, BOOL reset)
{
	if(pGroup == NULL) return FALSE;
	pAction = pGroup->GetPtrAction(num);
	if(pAction == NULL) return FALSE;
	nFrameNum = pAction->TotFrameNum;
	nNowAction = num;
	if(reset) ResetFrame(); // ���� �ʱ�ȭ
	return TRUE;
}

BOOL xAniMan::SetAction(char* name, BOOL reset)
{
	if(pGroup == NULL) return FALSE;
	pAction = pGroup->GetPtrAction(name, &nNowAction);
	if(pAction == NULL) return FALSE;
	nFrameNum = pAction->TotFrameNum;
	if(reset) ResetFrame(); // ���� �ʱ�ȭ
	return TRUE;
}

// �������� ����(�׼��� �����Ǿ� �־�� �Ѵ�)
BOOL xAniMan::SetFrame(int num, BOOL reset)
{
	if(pAction == NULL) return FALSE;
	pFrame = pAction->GetPtrFrame(num);
	if(pFrame == NULL) return FALSE;
	nCellNum = pFrame->TotCellNum;
	if(reset) ResetCell(); // ���� �ʱ�ȭ
	nNowFrame = num;
	
	iDelayCnt = pFrame->Delay;
	return TRUE;
}

// ���� ����(�������� �����Ǿ� �־�� �Ѵ�)
BOOL xAniMan::SetCell(int num)
{
	if(pFrame == NULL) return FALSE;
	pCell = pFrame->GetPtrCell(num);
	if(pCell == NULL) return FALSE;
	nNowCell = num;
	return TRUE;
}

//========================== ���ϸ��̼� ���� ��� =================================

// �������� ������ ��´�(�׼��� �����Ǿ� �־�� �Ѵ�)
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

// ���� ���� ������ ��´�(�������� �����Ǿ� �־�� �Ѵ�)
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
// Override Function(Ư�� ��ȣ�� �� ������ ��´�)
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
// Override Function(���� ������ ��ü�� �� ������ ��´�)
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
