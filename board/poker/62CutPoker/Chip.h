// Chip.h: interface for the CChip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHIP_H__A37227E1_8E37_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_CHIP_H__A37227E1_8E37_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"

class CChip  
{
public:
	CPage *pPage, *pBatPage;

	// 칩 속성
	int ChipNo;			// 칩번호(0:안쓰임 1=1칩 2=5칩  3=10칩 4=50칩 5=100칩 6=500칩 7=1000칩 ## 추가 8=5000 9=10000)
	int Price;			// 칩의 금액
	int Kind;			// 칩의 각도별 종류
	int SprNo;			// 스프라이트 번호

	int ShowMode;		// 보여주기 방법(0:세팅 없음 1:딜레이 후 이동시 보여줌)

	// 사운드
	char *MoveStartSnd;	// 이동이 시작될때의 사운드
	char *MoveEndSnd;	// 이동이 끝났을때의 사운드

	// 칩 상태
	BOOL bLive;			// 칩이 살아있는가?
	BOOL bMove;			// 이동중인가?
	BOOL bEvent;		// 이벤트가 있는가?

	// 카드 이동 관련 변수
	int Xp, Yp;			// 그려질 좌표
	int MoveLen;		// 이동한 거리
	int SXp, SYp;		// 이동시 출발 좌표
	int TXp, TYp;		// 이동시 목표 좌표
	int MoveDelay;		// 이동 시작 지연치
	int EndMoveDelay;	// 이동 완료 지연치
	int MoveSpeed;		// 이동 속도

	// 반투명 관련 변수
	BOOL bTwincle;
	int TwincleCnt;		// 반투명 카운트

	CChip();
	virtual ~CChip();

	void Init(CPage *ppage, CPage *pbatpage);
	void SetChip(int chipno, int kind);
	void Reset();
	void OnTimer();
	void SetMoveStartSnd(char *sndfilename);
	void SetMoveEndSnd(char *sndfilename);
	void SetPos(int tx, int ty);
	void SetMove(int tx, int ty, int sdelay=0, int edelay=0, int speed=-1);
	void Draw();
};

#endif // !defined(AFX_CHIP_H__A37227E1_8E37_11D4_97A4_0050BF0FBE67__INCLUDED_)
