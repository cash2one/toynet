// Player.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "Player.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayer::CPlayer()
{
	Clear();
	ServPNum = 0;
	PNum = 0;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Clear()
{
	ZeroMemory(&UI, sizeof(UI));
	Reset();
//	Team = 0;
	bFold = FALSE;
	bShutMouse = FALSE;
	bHiddenOpen = FALSE;

	// ### [ �ߺ��� IP�� ] ###
	MyIpCheck = -1;

	// ### [���� �߰� �۾�] ###
	nSndFxKind = 0;

	// +- ������ ǥ��
	PrevMoney = 0;

	m_ChangeCardNum = -2; // [62] �ٲ� ī�� �ѹ�
	m_KD_SeleteCard = -1;
	bChangeWhether = -1;    // ī�带 �ٲ���� �ƴ��� 
	bAllIn = FALSE;
}

void CPlayer::SetPlayerNum(int pnum)
{
	PNum = pnum;
}

void CPlayer::SetNewPlayer(USERINFO *pUI)
{
	Clear();
	memcpy(&UI, pUI, sizeof(USERINFO));
	Avatar.SetAvatar(UI.AvatarInfo);
}

void CPlayer::Reset()
{
	bNowWinner = FALSE;
	JoinState = 0;
	PlayState = 0;
//	Ranking = 0;
//	WinCase = 0;
	
	nCardTotal = 0;
	nOpenTotal = 0;
	bHiddenOpen = FALSE;
	bFold = FALSE;
	m_bSelectCard = FALSE;
	bChangeWhether = -1;    // ī�带 �ٲ���� �ƴ��� 
}

