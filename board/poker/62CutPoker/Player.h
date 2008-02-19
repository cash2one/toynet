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
	int ServPNum;			// ���� ������ �÷��̾� ��ȣ
	int PNum;				// �÷��̾� ��ȣ
	BOOL bShutMouse;		// �Ը������ΰ�?

	CAvatar Avatar;			// �ƹ�Ÿ Ŭ����

	// �÷��̾� ����
	BOOL bNowWinner;		// �̹��� �����ΰ�?

	// ���� ����
	int JoinState;			// ���� ���� ����(0:����, 1:����)
	int PlayState;			// �÷��� ����(0:��������, 1:������)
//	int Ranking;			// ���� ����(���� �������̰� ��������� ���¿����� ��ȿ)
//	int Team;				// �÷��̾��� �Ҽ� ��(0:�̱�, 1:A��-����, 2:B��-���, 3:C��-�׸�)
	//char ItemState[8];		// ������ ���� ����
	//int ItemStateCnt[8];	// ������ ���� ī����
//	int WinCase;			// ���� ������ ����

	int nCardTotal;
	int nOpenTotal;

	// ### [ �ߺ��� IP�� ] ###
	int MyIpCheck;

	// ### [���� �߰� �۾�] ###
	int nSndFxKind;

	// +- ������ ǥ��
	INT64 PrevMoney;

	INT64 BankMoney;
	INT64 LoseMoney;

	BOOL bFold;
	BOOL bHiddenOpen;
	BOOL m_bSelectCard;
	char bChangeWhether;    // ī�带 �ٲ���� �ƴ��� 0 = �ٲ��� ���� 1 = �ٲ�
	BOOL bAllIn;			// �� �����̴�.
	int  m_ChangeCardNum;
	int  m_KD_SeleteCard;	//	KeyDown����Ű�϶��� �� 

	int	 nWinMoney;		// ���� �̱� �� - jeong

	CPlayer();
	virtual ~CPlayer();

	void Clear();
	void SetPlayerNum(int pnum);
	void SetNewPlayer(USERINFO *pUI);
	void Reset();
	
	//void ResetItemState();
	// �÷��̾��� �� �����
//	void DrawMyBlock();
};

#endif // !defined(AFX_PLAYER_H__A4DF4943_11B0_11D4_97A2_0050BF0FBE67__INCLUDED_)
