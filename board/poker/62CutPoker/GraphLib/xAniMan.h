#ifndef _XANIMAN_H_
#define _XANIMAN_H_

#include <windows.h>
#include "xAniFile.h"
#include "xSprite.h"
//#include "xDraw.h"
#include "xLayer.h"

#include "Graphic.h"

//// ������ ��������Ʈ���� ���� ����ü
typedef struct
{
	int SprNo;
	int Rx, Ry;
	int Flag;
} PLAYSTUFF;

//// ���ϸ��̼� ���� Ŭ����
class xAniMan
{
public:
	// ��������Ʈ ������ ������
	xSprite* pSpr;

	// �� ����� ������
	xAniFile*	pAni;
	Character*	pChar;
	Group*		pGroup;
	Action*		pAction;
	Frame*		pFrame;
	Cell*		pCell;

	// ���� ������ �� ��庰 �ε�����ȣ
	int nNowChar, nNowGroup, nNowAction, nNowFrame, nNowCell;

	// ���� ������ ����� ������� ����
	int nCharNum, nGroupNum, nActionNum, nFrameNum, nCellNum;

	// ���ϸ��̼� �÷��� ��� ������ ���� �迭
	static PLAYSTUFF Ps[100]; // �޸� ������ ���� static���� ����

	// ���ϸ��̼� ��� ��� ���� ����
	BOOL bLoop;
	BOOL bForward;

	// ���ϸ��̼� ��� ���� ���� ����
	BOOL bPause;

	// ���ϸ��̼ǿ� ����
	int iDelayCnt;

public:
	xAniMan();
	~xAniMan();
	void Clear();

	// ���ϸ��̼� ���� Ŭ������ �ʱ�ȭ�Ѵ�
	BOOL Init(xSprite* pSprite);
	BOOL Init(xAniFile* pAniFile);

	// ���ϸ��̼� �÷���
	int  PlayAction(CPage *Page, BOOL bNext, int x, int y, int opt=NULL, int grade=NULL, WORD color=NULL);
	int  PlayAction( xLayer *pLay, int x, int y, int opt=NULL, int grade=NULL, WORD color=NULL);
	BOOL GetPlayStuff(PLAYSTUFF* Ps, int* cellnum);
	void Pause();
	void Resume();
	
	// ���ϸ��̼� �÷��� �ɼ� ���� �Լ�
	void SetForward();
	void SetBackward();
	void SetLoop(BOOL loop);


	///////////// Low Level Functions /////////////

	// �� ����� ������ �ʱ�ȭ�Ѵ�( resetchild�� TRUE�̸� ���� ��嵵 �ʱ�ȭ �Ѵ� )
	void ResetCharacter(BOOL resetchild = TRUE);
	void ResetGroup(BOOL resetchild = TRUE);
	void ResetAction(BOOL resetchild = TRUE);
	void ResetFrame(BOOL resetchild = TRUE);
	void ResetCell();

	// �� ��忡 ���ϸ��̼� ��带 �����Ѵ�
	// (���� ��尡 �ݵ�� �����Ǿ� �־�� �Ѵ�)
	// ( reset�� TRUE�̸� ��������� ���� �ʱ�ȭ�Ѵ�)
	BOOL SetCharacter(int num, BOOL reset=TRUE);
	BOOL SetCharacter(char* name, BOOL reset=TRUE);	// Override Function
	BOOL SetGroup(int num, BOOL reset=TRUE);
	BOOL SetGroup(char* name, BOOL reset=TRUE);		// Override Function
	BOOL SetAction(int num, BOOL reset=TRUE);
	BOOL SetAction(char* name, BOOL reset=TRUE);	// Override Function
	BOOL SetFrame(int num, BOOL reset=TRUE);
	BOOL SetCell(int num);

	// ���� ����� ������ ��´�
	BOOL GetNowFrameInfo(int* delay, int* totcell);
	BOOL GetNowFrameInfo(int FrameNum, int* delay, int* totcell);				// Override Function
	BOOL GetNowCellInfo(int* sprno, int* rx, int* ry, int* flag);
	BOOL GetNowCellInfo(int CellNum, int* sprno, int* rx, int* ry, int* flag);	// Override Function
	BOOL GetNowCellInfo(PLAYSTUFF* Ps, int* totcell);							// Override Function
};




#endif