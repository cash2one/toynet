#ifndef _XANIFILE_H_
#define _XANIFILE_H_

#include <windows.h>
#include <stdio.h>
#include <string.h>

class xAniFile;
class Character;
class Group;
class Action;
class Frame;
class Cell;


// xAniFile Class
class xAniFile
{
public:
	short TotCharacterNum;
	Character** ppCharacter;

	xAniFile();
	~xAniFile();
	void Destroy();

	// 에니메이션 데이터를 로드, 세이브
	BOOL Load(FILE* fp);
	BOOL Save(FILE* fp);

	// 스프라이트 데이터의 변동이 있을때 사용하는 함수들
	void OnDeleteSpr(int snum);
	void OnMoveSpr(int snum, int tnum);
	
	// 캐릭터 추가, 삭제, 이동
	void AddCharacter(char* name);
	void DelCharacter(int num);
	void MovCharacter(int num, int tnum);
	Character* GetPtrCharacter(char* name, int* pos=NULL);
	Character* GetPtrCharacter(int num);

	// 각 노드의 포인터를 구하는 함수
	Character* GetCharacterPt(int CharNum);
	Group* GetGroupPt(int CharNum, int GroupNum);
	Action* GetActionPt(int CharNum, int GroupNum, int ActionNum);
	Frame* GetFramePt(int CharNum, int GroupNum, int ActionNum, int FrameNum);
	Cell* GetCellPt(int CharNum, int GroupNum, int ActionNum, int FrameNum, int CellNum);
};

// Character Class
class Character
{
public:
	char CharacterName[14];
	short TotGroupNum;
	Group** ppGroup;

	Character();
	~Character();
	void Destroy();
	void Set(char* name);

	void AddGroup(char* name);
	void DelGroup(int num);
	void MovGroup(int num, int tnum);
	Group* GetPtrGroup(char* name, int* pos=NULL);
	Group* GetPtrGroup(int num);
};

// Group Class
class Group
{
public:
	char GroupName[14];
	short TotActionNum;
	Action** ppAction;

	Group();
	~Group();
	void Destroy();
	void Set(char* name);

	void AddAction(char* name);
	void DelAction(int num);
	void MovAction(int num, int tnum);
	Action* GetPtrAction(char* name, int* pos=NULL);
	Action* GetPtrAction(int num);
};

// Action Class
class Action
{
public:
	char ActionName[14];
	short TotFrameNum;
	Frame** ppFrame;

	Action();
	~Action();
	void Destroy();
	void Set(char* name);

	void AddFrame(int delay);
	void DelFrame(int num);
	void MovFrame(int num, int tnum);
	Frame* GetPtrFrame(int num);
};

// Frame Class
class Frame
{
public:
	short TotCellNum;
	short Delay;
	Cell** ppCell;

	Frame();
	~Frame();
	void Destroy();
	void Set(int delay);

	void AddCell(int sprno, int rx, int ry, int overrideflag);
	void DelCell(int num);
	void MovCell(int num, int tnum);
	Cell* GetPtrCell(int num);
};


// Cel Class
class Cell
{
public:
	short SprNo;
	short Rx, Ry;
	short OverrideFlag;

	Cell();
	void Set(int sprno, int rx, int ry, int overrideflag);
};


#endif
