
#include "StdAfx.h"
#include "xAniFile.h"

//////////////////////////////////// xAniFile Class ///////////////////////////////////

xAniFile::xAniFile()
{
	TotCharacterNum = 0;
	ppCharacter = NULL;
}

xAniFile::~xAniFile()
{
	Destroy();
}

void xAniFile::Destroy()
{
	if(ppCharacter) 
	{
		for(int i=0; i<TotCharacterNum; i++) 
			if(ppCharacter[i] != NULL) { delete(ppCharacter[i]); ppCharacter[i]=NULL; }
		free(ppCharacter);
		ppCharacter= NULL;
	}

	TotCharacterNum = 0;
}

void xAniFile::AddCharacter(char* name)
{
	TotCharacterNum += 1;

	Character** ppNew = (Character**) malloc(sizeof(Character*) * TotCharacterNum);
	
	if(ppCharacter) 
	{
		memcpy(ppNew, ppCharacter, sizeof(Character*) * TotCharacterNum);
		free(ppCharacter);
	}
	ppCharacter = ppNew;

	ppCharacter[TotCharacterNum-1] = new Character;
	(ppCharacter[TotCharacterNum-1])->Set(name);
}

void xAniFile::DelCharacter(int num)
{
	if(TotCharacterNum <= 0) return;
	if(num < 0 || num >= TotCharacterNum) return;
	if(TotCharacterNum == 1) { Destroy(); return; }

	TotCharacterNum -= 1;
	Character** ppNew = (Character**) malloc(sizeof(Character*) * TotCharacterNum);
	
	int i;
	for(i=0; i<num; i++) ppNew[i] = ppCharacter[i];
	for(i=num; i<TotCharacterNum; i++) ppNew[i] = ppCharacter[i+1];

	delete(ppCharacter[num]);
	if(ppCharacter) free(ppCharacter);
	ppCharacter = ppNew;
}

void xAniFile::MovCharacter(int num, int tnum)
{
	if(TotCharacterNum <= 1) return;
	if(tnum < 0 || tnum >= TotCharacterNum) return;
	if(num < 0 || num >= TotCharacterNum) return;
	if(num == tnum) return;

	Character** ppNew = (Character**) malloc(sizeof(Character*) * TotCharacterNum);

	int t,s;
	for(t=0, s=0; t<TotCharacterNum; t++, s++)
	{
		if(t == tnum) t++;
		if(s == num) s++;
		if(t < TotCharacterNum) ppNew[t] = ppCharacter[s];
	}
	ppNew[tnum] = ppCharacter[num];

	if(ppCharacter) free(ppCharacter);
	ppCharacter = ppNew;
}

// 캐릭터의 포인터를 이름으로 얻는다(*pos = 인덱스 번호를 받을 포인터)
Character* xAniFile::GetPtrCharacter(char* name, int* pos)
{
	if(TotCharacterNum==0) return NULL;
	int i, Num=-1;
	for(i=0; i<TotCharacterNum; i++)
		if(strcmp(name,(ppCharacter[i])->CharacterName)==0){Num=i; break;}
	if(Num == -1) return NULL;
	else
	{
		if(pos!=NULL) *pos=Num;
		return ppCharacter[Num];
	}
}

// 캐릭터의 포인터를 인덱스로 얻는다
Character* xAniFile::GetPtrCharacter(int num)
{
	if(num<0 || num >= TotCharacterNum) return NULL;
	return ppCharacter[num];
}


// 각 노드의 포인터를 인덱스 번호로 얻는다 
Character* xAniFile::GetCharacterPt(int CharNum)
{
	if(CharNum >= TotCharacterNum) return NULL;
	//if(ppCharacter == NULL) return NULL;
	return ppCharacter[CharNum];
}

Group* xAniFile::GetGroupPt(int CharNum, int GroupNum)
{
	if(CharNum >= TotCharacterNum) return NULL;
	if(GroupNum >= (ppCharacter[CharNum])->TotGroupNum) return NULL;
	//if((ppCharacter[CharNum])->ppGroup == NULL) return NULL;
	return ((ppCharacter[CharNum])->ppGroup[GroupNum]);
}

Action* xAniFile::GetActionPt(int CharNum, int GroupNum, int ActionNum)
{
	if(CharNum >= TotCharacterNum) return NULL;
	if(GroupNum >= (ppCharacter[CharNum])->TotGroupNum) return NULL;
	if(ActionNum >= ((ppCharacter[CharNum])->ppGroup[GroupNum])->TotActionNum) return NULL;
	//if(((ppCharacter[CharNum])->ppGroup[GroupNum])->ppAction == NULL) return NULL;
	return ((ppCharacter[CharNum])->ppGroup[GroupNum])->ppAction[ActionNum];
}

Frame* xAniFile::GetFramePt(int CharNum, int GroupNum, int ActionNum, int FrameNum)
{
	if(CharNum >= TotCharacterNum) return NULL;
	if(GroupNum >= (ppCharacter[CharNum])->TotGroupNum) return NULL;
	if(ActionNum >= ((ppCharacter[CharNum])->ppGroup[GroupNum])->TotActionNum) return NULL;
	if(FrameNum >= (((ppCharacter[CharNum])->ppGroup[GroupNum])->ppAction[ActionNum])->TotFrameNum) return NULL;
	//if((((ppCharacter[CharNum])->ppGroup[GroupNum])->ppAction[ActionNum])->ppFrame == NULL) return NULL;
	return (((ppCharacter[CharNum])->ppGroup[GroupNum])->ppAction[ActionNum])->ppFrame[FrameNum];
}

Cell* xAniFile::GetCellPt(int CharNum, int GroupNum, int ActionNum, int FrameNum, int CellNum)
{
	if(CharNum >= TotCharacterNum) return NULL;
	if(GroupNum >= (ppCharacter[CharNum])->TotGroupNum) return NULL;
	if(ActionNum >= ((ppCharacter[CharNum])->ppGroup[GroupNum])->TotActionNum) return NULL;
	if(FrameNum >= (((ppCharacter[CharNum])->ppGroup[GroupNum])->ppAction[ActionNum])->TotFrameNum) return NULL;
	if(CellNum >= ((((ppCharacter[CharNum])->ppGroup[GroupNum])->ppAction[ActionNum])->ppFrame[FrameNum])->TotCellNum) return NULL;
	//if(((((ppCharacter[CharNum])->ppGroup[GroupNum])->ppAction[ActionNum])->ppFrame[FrameNum])->ppCell == NULL) return NULL;
	return ((((ppCharacter[CharNum])->ppGroup[GroupNum])->ppAction[ActionNum])->ppFrame[FrameNum])->ppCell[CellNum];
}

// 에니메이션을 읽어들인다
BOOL xAniFile::Load(FILE* fp)
{
	int n0=0, n1=0, n2=0, n3=0, n4=0;
	int cnt0=0, cnt1=0, cnt2=0, cnt3=0, cnt4=0;
		
	Destroy();

	fread(&cnt0, sizeof(TotCharacterNum), 1, fp);

	for(n0=0; n0<cnt0; n0++) // 캐릭터
	{		
		Character pChar;
		
		fread(pChar.CharacterName, sizeof(pChar.CharacterName), 1, fp);
		fread(&pChar.TotGroupNum, sizeof(pChar.TotGroupNum), 1, fp);
		AddCharacter(pChar.CharacterName);
		cnt1 = pChar.TotGroupNum;

		for(n1=0; n1<cnt1; n1++) // 그룹
		{
			Group pGroup;
			
			fread(pGroup.GroupName, sizeof(pGroup.GroupName), 1, fp);
			fread(&pGroup.TotActionNum, sizeof(pGroup.TotActionNum), 1, fp);
			GetCharacterPt(n0)->AddGroup(pGroup.GroupName);
			cnt2 = pGroup.TotActionNum;

			for(n2=0; n2<cnt2; n2++) // 액션
			{
				Action pAction;
				
				fread(pAction.ActionName, sizeof(pAction.ActionName), 1, fp);
				fread(&pAction.TotFrameNum, sizeof(pAction.TotFrameNum), 1, fp);
				GetGroupPt(n0, n1)->AddAction(pAction.ActionName);
				cnt3 = pAction.TotFrameNum;

				for(n3=0; n3<cnt3; n3++) // 프레임
				{
					Frame pFrame;
					
					fread(&pFrame.TotCellNum, sizeof(pFrame.TotCellNum), 1, fp);
					fread(&pFrame.Delay, sizeof(pFrame.Delay), 1, fp);
					if(GetActionPt(n0, n1, n2)==NULL) continue;
					GetActionPt(n0, n1, n2)->AddFrame(pFrame.Delay);
					cnt4 = pFrame.TotCellNum;
					
					for(n4=0; n4<cnt4; n4++) // 셀
					{
						Cell pCell;
						
						fread(&pCell.SprNo, sizeof(pCell.SprNo), 1, fp);
						fread(&pCell.Rx, sizeof(pCell.Rx), 1, fp);
						fread(&pCell.Ry, sizeof(pCell.Ry), 1, fp);
						fread(&pCell.OverrideFlag, sizeof(pCell.OverrideFlag), 1, fp);
						GetFramePt(n0, n1, n2, n3)->AddCell(pCell.SprNo, pCell.Rx, pCell.Ry, pCell.OverrideFlag);
					}
				}
			}
		}
	}

	return TRUE;
}

// 에니메이션을 파일에 저장한다(받은 파일포인터에 이어서 기록)
BOOL xAniFile::Save(FILE* fp)
{
	if(TotCharacterNum == 0) return TRUE;

	int n0, n1, n2, n3, n4;
	int cnt0, cnt1, cnt2, cnt3, cnt4;
		
	fwrite(&TotCharacterNum, sizeof(TotCharacterNum), 1, fp);

	cnt0 = TotCharacterNum;

	for(n0=0; n0<cnt0; n0++) // 캐릭터
	{		
		Character* pChar = GetCharacterPt(n0);
		if(pChar == NULL) return FALSE;
		fwrite(pChar->CharacterName, sizeof(pChar->CharacterName), 1, fp);
		fwrite(&pChar->TotGroupNum, sizeof(pChar->TotGroupNum), 1, fp);
		
		cnt1 = pChar->TotGroupNum;
		
		for(n1=0; n1<cnt1; n1++) // 그룹
		{
			Group* pGroup = GetGroupPt(n0,n1);
			if(pGroup == NULL) return FALSE;
			fwrite(pGroup->GroupName, sizeof(pGroup->GroupName), 1, fp);
			fwrite(&pGroup->TotActionNum, sizeof(pGroup->TotActionNum), 1, fp);

			cnt2 = pGroup->TotActionNum;

			for(n2=0; n2<cnt2; n2++) // 액션
			{
				Action* pAction = GetActionPt(n0,n1,n2);
				if(pAction == NULL) return FALSE;
				fwrite(pAction->ActionName, sizeof(pAction->ActionName), 1, fp);
				fwrite(&pAction->TotFrameNum, sizeof(pAction->TotFrameNum), 1, fp);

				cnt3 = pAction->TotFrameNum;

				for(n3=0; n3<cnt3; n3++) // 프레임
				{
					Frame* pFrame = GetFramePt(n0,n1,n2,n3);
					if(pFrame == NULL) return FALSE;
					fwrite(&pFrame->TotCellNum, sizeof(pFrame->TotCellNum), 1, fp);
					fwrite(&pFrame->Delay, sizeof(pFrame->Delay), 1, fp);

					cnt4 = pFrame->TotCellNum;

					for(n4=0; n4<cnt4; n4++) // 셀
					{
						Cell* pCell = GetCellPt(n0, n1, n2, n3, n4);
						if(pCell == NULL) return FALSE;
						fwrite(&pCell->SprNo, sizeof(pCell->SprNo), 1, fp);
						fwrite(&pCell->Rx, sizeof(pCell->Rx), 1, fp);
						fwrite(&pCell->Ry, sizeof(pCell->Ry), 1, fp);
						fwrite(&pCell->OverrideFlag, sizeof(pCell->OverrideFlag), 1, fp);
					}
				}
			}
		}
	}

	return TRUE;
}

// 스프라이트가 삭제될때 에니메이션 정보도 갱신시킨다
void xAniFile::OnDeleteSpr(int snum)
{
	int n0, n1, n2, n3, n4;
	int cnt0, cnt1, cnt2, cnt3, cnt4;

	cnt0 = TotCharacterNum;

	for(n0=0; n0<cnt0; n0++) // 캐릭터
	{		
		Character* pChar = GetCharacterPt(n0);
		if(pChar == NULL) return;
		cnt1 = pChar->TotGroupNum;
		
		for(n1=0; n1<cnt1; n1++) // 그룹
		{
			Group* pGroup = GetGroupPt(n0,n1);
			if(pGroup == NULL) return;
			cnt2 = pGroup->TotActionNum;

			for(n2=0; n2<cnt2; n2++) // 액션
			{
				Action* pAction = GetActionPt(n0,n1,n2);
				if(pAction == NULL) return;
				cnt3 = pAction->TotFrameNum;

				for(n3=0; n3<cnt3; n3++) // 프레임
				{
					Frame* pFrame = GetFramePt(n0,n1,n2,n3);
					if(pFrame == NULL) return;
					cnt4 = pFrame->TotCellNum;

					for(n4=0; n4<cnt4; n4++) // 셀
					{
						Cell* pCell = GetCellPt(n0, n1, n2, n3, n4);
						if(pCell == NULL) return;
						if(pCell->SprNo == snum) {pFrame->DelCell(n4); cnt4--;}
						else if(pCell->SprNo > snum) pCell->SprNo--;
					}
				}
			}
		}
	}
}

// 스프라이트의 순서를 바꿀때 에니메이션 정보도 갱신시킨다
void xAniFile::OnMoveSpr(int snum, int tnum)
{
	int n0, n1, n2, n3, n4;
	int cnt0, cnt1, cnt2, cnt3, cnt4;

	cnt0 = TotCharacterNum;

	for(n0=0; n0<cnt0; n0++) // 캐릭터
	{		
		Character* pChar = GetCharacterPt(n0);
		if(pChar == NULL) return;
		cnt1 = pChar->TotGroupNum;
		
		for(n1=0; n1<cnt1; n1++) // 그룹
		{
			Group* pGroup = GetGroupPt(n0,n1);
			if(pGroup == NULL) return;
			cnt2 = pGroup->TotActionNum;

			for(n2=0; n2<cnt2; n2++) // 액션
			{
				Action* pAction = GetActionPt(n0,n1,n2);
				if(pAction == NULL) return;
				cnt3 = pAction->TotFrameNum;

				for(n3=0; n3<cnt3; n3++) // 프레임
				{
					Frame* pFrame = GetFramePt(n0,n1,n2,n3);
					if(pFrame == NULL) return;
					cnt4 = pFrame->TotCellNum;

					for(n4=0; n4<cnt4; n4++) // 셀
					{
						Cell* pCell = GetCellPt(n0, n1, n2, n3, n4);
						if(pCell == NULL) return;
						if(snum>tnum)
						{
							if(pCell->SprNo == snum) pCell->SprNo = tnum;
							else if(pCell->SprNo >= tnum && pCell->SprNo < snum) pCell->SprNo++;
						}
						if(snum<tnum)
						{
							if(pCell->SprNo == snum) pCell->SprNo = tnum;
							else if(pCell->SprNo > snum && pCell->SprNo <= tnum) pCell->SprNo--;
						}
					}
				}
			}
		}
	}
}


//////////////////////////////////// Character Class ///////////////////////////////////

Character::Character()
{
	memset(CharacterName, 0, sizeof(CharacterName));
	TotGroupNum = 0;
	ppGroup = NULL;
}

Character::~Character()
{
	Destroy();
}

void Character::Destroy()
{
	if(ppGroup) 
	{
		for(int i=0; i<TotGroupNum; i++) 
			if(ppGroup[i] != NULL) { delete(ppGroup[i]); ppGroup[i]=NULL; }
		free(ppGroup);
		ppGroup= NULL;
	}

	memset(CharacterName, 0, sizeof(CharacterName));
	TotGroupNum = 0;
}

void Character::Set(char* name)
{
	strncpy(CharacterName, name, 14);
}

void Character::AddGroup(char* name)
{
	TotGroupNum += 1;

	Group** ppNew = (Group**) malloc(sizeof(Group*) * TotGroupNum);
	
	if(ppGroup)
	{
		memcpy(ppNew, ppGroup, sizeof(Group*) * TotGroupNum);
		free(ppGroup);
	}
	ppGroup = ppNew;

	ppGroup[TotGroupNum-1] = new Group;
	(ppGroup[TotGroupNum-1])->Set(name);
}

void Character::DelGroup(int num)
{
	if(TotGroupNum <= 0) return;
	if(num < 0 || num >= TotGroupNum) return;
	//if(TotGroupNum == 1) { Destroy(); return; }

	TotGroupNum -= 1;
	Group** ppNew = (Group**) malloc(sizeof(Group*) * TotGroupNum);
	
	int i;
	for(i=0; i<num; i++) ppNew[i] = ppGroup[i];
	for(i=num; i<TotGroupNum; i++) ppNew[i] = ppGroup[i+1];

	delete(ppGroup[num]);
	if(ppGroup) free(ppGroup);
	ppGroup = ppNew;
}

void Character::MovGroup(int num, int tnum)
{
	if(TotGroupNum <= 1) return;
	if(tnum < 0 || tnum >= TotGroupNum) return;
	if(num < 0 || num >= TotGroupNum) return;
	if(num == tnum) return;

	Group** ppNew = (Group**) malloc(sizeof(Group*) * TotGroupNum);

	int t,s;
	for(t=0, s=0; t<TotGroupNum; t++, s++)
	{
		if(t == tnum) t++;
		if(s == num) s++;
		if(t < TotGroupNum) ppNew[t] = ppGroup[s];
	}
	ppNew[tnum] = ppGroup[num];

	if(ppGroup) free(ppGroup);
	ppGroup = ppNew;
}

// 그룹의 포인터를 이름으로 얻는다(*pos = 인덱스 번호를 받을 포인터)
Group* Character::GetPtrGroup(char* name, int* pos)
{
	if(TotGroupNum==0) return NULL;
	int i, Num=-1;
	for(i=0; i<TotGroupNum; i++)
		if(strcmp(name,(ppGroup[i])->GroupName)==0){Num=i; break;}

	if(Num == -1) return NULL;
	else
	{ 
		if(pos!=NULL) *pos = Num;
		return ppGroup[Num];
	}
}

// 그룹의 포인터를 인덱스로 얻는다
Group* Character::GetPtrGroup(int num)
{
	if(num<0 || num >= TotGroupNum) return NULL;
	return ppGroup[num];
}



//////////////////////////////////// Group Class ///////////////////////////////////

Group::Group()
{
	memset(GroupName, 0, sizeof(GroupName));
	TotActionNum = 0;
	ppAction = NULL;
}

Group::~Group()
{
	Destroy();
}

void Group::Destroy()
{
	if(ppAction) 
	{
		for(int i=0; i<TotActionNum; i++) 
			if(ppAction[i] != NULL) { delete(ppAction[i]); ppAction[i]=NULL; }
		free(ppAction);
		ppAction= NULL;
	}

	memset(GroupName, 0, sizeof(GroupName));
	TotActionNum = 0;
}

void Group::Set(char* name)
{
	strncpy(GroupName, name, 14);
}

void Group::AddAction(char* name)
{
	TotActionNum += 1;

	Action** ppNew = (Action**) malloc(sizeof(Action*) * TotActionNum);
	
	if(ppAction)
	{
		memcpy(ppNew, ppAction, sizeof(Action*) * TotActionNum);
		free(ppAction);
	}
	ppAction = ppNew;

	ppAction[TotActionNum-1] = new Action;
	(ppAction[TotActionNum-1])->Set(name);
}

void Group::DelAction(int num)
{
	if(TotActionNum <= 0) return;
	if(num < 0 || num >= TotActionNum) return;
	//if(TotActionNum == 1) { Destroy(); return; }

	TotActionNum -= 1;
	Action** ppNew = (Action**) malloc(sizeof(Action*) * TotActionNum);
	
	int i;
	for(i=0; i<num; i++) ppNew[i] = ppAction[i];
	for(i=num; i<TotActionNum; i++) ppNew[i] = ppAction[i+1];

	delete(ppAction[num]);
	if(ppAction) free(ppAction);
	ppAction = ppNew;
}

void Group::MovAction(int num, int tnum)
{
	if(TotActionNum <= 1) return;
	if(tnum < 0 || tnum >= TotActionNum) return;
	if(num < 0 || num >= TotActionNum) return;
	if(num == tnum) return;

	Action** ppNew = (Action**) malloc(sizeof(Action*) * TotActionNum);

	int t,s;
	for(t=0, s=0; t<TotActionNum; t++, s++)
	{
		if(t == tnum) t++;
		if(s == num) s++;
		if(t < TotActionNum) ppNew[t] = ppAction[s];
	}
	ppNew[tnum] = ppAction[num];

	if(ppAction) free(ppAction);
	ppAction = ppNew;
}

// 액션의 포인터를 이름으로 얻는다(*pos = 인덱스 번호를 받을 포인터)
Action* Group::GetPtrAction(char* name, int* pos)
{
	if(TotActionNum==0) return NULL;
	int i, Num=-1;
	for(i=0; i<TotActionNum; i++)
		if(strcmp(name,(ppAction[i])->ActionName)==0){Num=i; break;}
	if(Num == -1) return NULL;
	else
	{
		if(pos!=NULL) *pos = Num;
		return ppAction[Num];
	}
}

// 액션의 포인터를 인덱스로 얻는다
Action* Group::GetPtrAction(int num)
{
	if(num<0 || num >= TotActionNum) return NULL;
	return ppAction[num];
}

//////////////////////////////////// Action Class ///////////////////////////////////

Action::Action()
{
	memset(ActionName, 0, sizeof(ActionName));
	TotFrameNum = 0;
	ppFrame = NULL;
}

Action::~Action()
{
	Destroy();
}

void Action::Destroy()
{
	if(ppFrame) 
	{
		for(int i=0; i<TotFrameNum; i++) 
			if(ppFrame[i] != NULL) { delete(ppFrame[i]); ppFrame[i]=NULL; }
		free(ppFrame);
		ppFrame = NULL;
	}

	memset(ActionName, 0, sizeof(ActionName));
	TotFrameNum = 0;
}

void Action::Set(char* name)
{
	strncpy(ActionName, name, 14);
}

void Action::AddFrame(int delay)
{
	TotFrameNum += 1;

	Frame** ppNew = (Frame**) malloc(sizeof(Frame*) * TotFrameNum);
	
	if(ppFrame)
	{
		if(TotFrameNum > 1) 
			memcpy(ppNew, ppFrame, sizeof(Frame*) * (TotFrameNum-1));
		free(ppFrame);
	}
	ppFrame = ppNew;

	ppFrame[TotFrameNum-1] = new Frame;
	(ppFrame[TotFrameNum-1])->Set(delay);
}

void Action::DelFrame(int num)
{
	if(TotFrameNum <= 0) return;
	if(num < 0 || num >= TotFrameNum) return;
	//if(TotFrameNum == 1) { Destroy(); return; }

	TotFrameNum -= 1;
	Frame** ppNew = (Frame**) malloc(sizeof(Frame*) * TotFrameNum);
	
	int i;
	for(i=0; i<num; i++) ppNew[i] = ppFrame[i];
	for(i=num; i<TotFrameNum; i++) ppNew[i] = ppFrame[i+1];

	delete(ppFrame[num]);
	if(ppFrame) free(ppFrame);
	ppFrame = ppNew;
}

void Action::MovFrame(int num, int tnum)
{
	if(TotFrameNum <= 1) return;
	if(tnum < 0 || tnum >= TotFrameNum) return;
	if(num < 0 || num >= TotFrameNum) return;
	if(num == tnum) return;

	Frame** ppNew = (Frame**) malloc(sizeof(Frame*) * TotFrameNum);

	int t,s;
	for(t=0, s=0; t<TotFrameNum; t++, s++)
	{
		if(t == tnum) t++;
		if(s == num) s++;
		if(t < TotFrameNum) ppNew[t] = ppFrame[s];
	}
	ppNew[tnum] = ppFrame[num];

	if(ppFrame) free(ppFrame);
	ppFrame = ppNew;
}

// 프레임의 포인터를 인덱스로 얻는다
Frame* Action::GetPtrFrame(int num)
{
	if(num<0 || num >= TotFrameNum) return NULL;
	return ppFrame[num];
}

//////////////////////////////////// Frame Class ///////////////////////////////////

Frame::Frame()
{
	TotCellNum = 0;
	Delay = 0;
	ppCell = NULL;
}

Frame::~Frame()
{
	Destroy();
}

void Frame::Destroy()
{
	if(ppCell) 
	{
		for(int i=0; i<TotCellNum; i++) 
			if(ppCell[i] != NULL) { delete(ppCell[i]); ppCell[i]=NULL; }
		free(ppCell);
		ppCell = NULL;
	}

	TotCellNum = 0;
	Delay = 0;
}

void Frame::Set(int delay)
{
	Delay = delay;
}

void Frame::AddCell(int sprno, int rx, int ry, int overrideflag)
{
	TotCellNum += 1;

	Cell** ppNew = (Cell**) malloc(sizeof(Cell*) * TotCellNum);
	
	if(ppCell)
	{
		memcpy(ppNew, ppCell, sizeof(Cell*) * TotCellNum);
		free(ppCell);
	}
	ppCell = ppNew;

	ppCell[TotCellNum-1] = new Cell;
	(ppCell[TotCellNum-1])->Set(sprno, rx, ry, overrideflag);
}

void Frame::DelCell(int num)
{
	if(TotCellNum <= 0) return;
	if(num < 0 || num >= TotCellNum) return;
	//if(TotCellNum == 1) { Destroy(); return; }

	TotCellNum -= 1;
	Cell** ppNew = (Cell**) malloc(sizeof(Cell*) * TotCellNum);
	
	int i;
	for(i=0; i<num; i++) ppNew[i] = ppCell[i];
	for(i=num; i<TotCellNum; i++) ppNew[i] = ppCell[i+1];

	delete(ppCell[num]);
	if(ppCell) free(ppCell);
	ppCell = ppNew;
}

void Frame::MovCell(int num, int tnum)
{
	if(TotCellNum <= 1) return;
	if(tnum < 0 || tnum >= TotCellNum) return;
	if(num < 0 || num >= TotCellNum) return;
	if(num == tnum) return;

	Cell** ppNew = (Cell**) malloc(sizeof(Cell*) * TotCellNum);

	int t,s;
	for(t=0, s=0; t<TotCellNum; t++, s++)
	{
		if(t == tnum) t++;
		if(s == num) s++;
		if(t < TotCellNum) ppNew[t] = ppCell[s];
	}
	ppNew[tnum] = ppCell[num];

	if(ppCell) free(ppCell);
	ppCell = ppNew;
}

// 셀의 포인터를 인덱스로 얻는다
Cell* Frame::GetPtrCell(int num)
{
	if(num<0 || num >= TotCellNum) return NULL;
	return ppCell[num];
}

//////////////////////////////////// Cell Class ///////////////////////////////////

Cell::Cell()
{
	SprNo = 0;
	Rx = Ry = 0;
	OverrideFlag = NULL;
}

void Cell::Set(int sprno, int rx, int ry, int overrideflag)
{
	SprNo = (short)sprno;
	Rx = (short)rx;
	Ry = (short)ry;
	OverrideFlag = (short)overrideflag;
}