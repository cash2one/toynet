// Player.h: interface for the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_H__A4DF4943_11B0_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_PLAYER_H__A4DF4943_11B0_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "Avatar.h"

class CPlayer  
{
public:
	USERINFO UI;
	int ServPNum;			// 서버 기준의 플레이어 번호
	int PNum;				// 플레이어 번호
	BOOL bShutMouse;		// 입막음중인가?

	CAvatar Avatar;			// 아바타 클래스

	// 플레이어 정보
	BOOL bNowWinner;		// 이번판 승자인가?

	// 게임 정보
	int JoinState;			// 게임 참여 상태(0:불참, 1:참여)
	int PlayState;			// 플레이 상태(0:게임종료, 1:게임중)
//	int Ranking;			// 게임 순위(게임 참여중이고 게임종료된 상태에서만 유효)
//	int Team;				// 플레이어의 소속 팀(0:싱글, 1:A팀-레드, 2:B팀-블루, 3:C팀-그린)
	//char ItemState[8];		// 아이템 상태 정보
	//int ItemStateCnt[8];	// 아이템 상태 카운터
//	int WinCase;			// 게임 오버시 선택

	int nCardTotal;
	int nOpenTotal;

	// ### [ 중복된 IP가 ] ###
	int MyIpCheck;

	// ### [사운드 추가 작업] ###
	int nSndFxKind;

	// +- 증가액 표시
	INT64 PrevMoney;

	INT64 BankMoney;
	INT64 LoseMoney;

	BOOL bFold;
	BOOL bHiddenOpen;
	BOOL m_bSelectCard;
	char bChangeWhether;    // 카드를 바꿨는지 아닌지 0 = 바꾸지 않음 1 = 바꿈
	BOOL bAllIn;			// 나 올인이다.
	int  m_ChangeCardNum;
	int  m_KD_SeleteCard;	//	KeyDown방향키일때의 값 

	int	 nWinMoney;		// 전판 이긴 돈 - jeong

	CPlayer();
	virtual ~CPlayer();

	void Clear();
	void SetPlayerNum(int pnum);
	void SetNewPlayer(USERINFO *pUI);
	void Reset();
	
	//void ResetItemState();
	// 플레이어의 블럭 찍어줌
//	void DrawMyBlock();
};

#endif // !defined(AFX_PLAYER_H__A4DF4943_11B0_11D4_97A2_0050BF0FBE67__INCLUDED_)
