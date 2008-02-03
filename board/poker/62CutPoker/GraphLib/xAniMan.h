#ifndef _XANIMAN_H_
#define _XANIMAN_H_

#include <windows.h>
#include "xAniFile.h"
#include "xSprite.h"
//#include "xDraw.h"
#include "xLayer.h"

#include "Graphic.h"

//// 찍혀질 스프라이트들의 정보 구조체
typedef struct
{
	int SprNo;
	int Rx, Ry;
	int Flag;
} PLAYSTUFF;

//// 에니메이션 관리 클래스
class xAniMan
{
public:
	// 스프라이트 파일의 포인터
	xSprite* pSpr;

	// 각 노드의 포인터
	xAniFile*	pAni;
	Character*	pChar;
	Group*		pGroup;
	Action*		pAction;
	Frame*		pFrame;
	Cell*		pCell;

	// 현재 지정된 각 노드별 인덱스번호
	int nNowChar, nNowGroup, nNowAction, nNowFrame, nNowCell;

	// 현재 지정된 노드의 하위노드 개수
	int nCharNum, nGroupNum, nActionNum, nFrameNum, nCellNum;

	// 에니메이션 플레이 출력 정보를 담을 배열
	static PLAYSTUFF Ps[100]; // 메모리 절약을 위해 static으로 선언

	// 에니메이션 출력 방법 설정 변수
	BOOL bLoop;
	BOOL bForward;

	// 에니메이션 출력 상태 설정 변수
	BOOL bPause;

	// 에니메이션용 변수
	int iDelayCnt;

public:
	xAniMan();
	~xAniMan();
	void Clear();

	// 에니메이션 관리 클래스를 초기화한다
	BOOL Init(xSprite* pSprite);
	BOOL Init(xAniFile* pAniFile);

	// 에니메이션 플레이
	int  PlayAction(CPage *Page, BOOL bNext, int x, int y, int opt=NULL, int grade=NULL, WORD color=NULL);
	int  PlayAction( xLayer *pLay, int x, int y, int opt=NULL, int grade=NULL, WORD color=NULL);
	BOOL GetPlayStuff(PLAYSTUFF* Ps, int* cellnum);
	void Pause();
	void Resume();
	
	// 에니메이션 플레이 옵션 설정 함수
	void SetForward();
	void SetBackward();
	void SetLoop(BOOL loop);


	///////////// Low Level Functions /////////////

	// 각 노드의 변수를 초기화한다( resetchild가 TRUE이면 하위 노드도 초기화 한다 )
	void ResetCharacter(BOOL resetchild = TRUE);
	void ResetGroup(BOOL resetchild = TRUE);
	void ResetAction(BOOL resetchild = TRUE);
	void ResetFrame(BOOL resetchild = TRUE);
	void ResetCell();

	// 각 노드에 에니메이션 노드를 연결한다
	// (상위 노드가 반드시 지정되어 있어야 한다)
	// ( reset이 TRUE이면 하위노드의 값을 초기화한다)
	BOOL SetCharacter(int num, BOOL reset=TRUE);
	BOOL SetCharacter(char* name, BOOL reset=TRUE);	// Override Function
	BOOL SetGroup(int num, BOOL reset=TRUE);
	BOOL SetGroup(char* name, BOOL reset=TRUE);		// Override Function
	BOOL SetAction(int num, BOOL reset=TRUE);
	BOOL SetAction(char* name, BOOL reset=TRUE);	// Override Function
	BOOL SetFrame(int num, BOOL reset=TRUE);
	BOOL SetCell(int num);

	// 현재 노드의 정보를 얻는다
	BOOL GetNowFrameInfo(int* delay, int* totcell);
	BOOL GetNowFrameInfo(int FrameNum, int* delay, int* totcell);				// Override Function
	BOOL GetNowCellInfo(int* sprno, int* rx, int* ry, int* flag);
	BOOL GetNowCellInfo(int CellNum, int* sprno, int* rx, int* ry, int* flag);	// Override Function
	BOOL GetNowCellInfo(PLAYSTUFF* Ps, int* totcell);							// Override Function
};




#endif